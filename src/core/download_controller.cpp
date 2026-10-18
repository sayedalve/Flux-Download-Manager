// SPDX-License-Identifier: GPL-3.0-or-later
#include "core/download_controller.hpp"
#include "brisk_engine/http_client.hpp"

#include "common/json_mini.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"

#include <algorithm>
#include <chrono>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace openidm {

DownloadController::DownloadController(ControllerConfig cfg)
    : m_cfg(std::move(cfg)),
      m_max_concurrent(m_cfg.max_concurrent_tasks),
      m_global_speed_limit_bps(m_cfg.global_speed_limit_bps) {
    m_settings.max_concurrent_tasks = m_cfg.max_concurrent_tasks;
    m_settings.speed_limit_bps = m_cfg.global_speed_limit_bps;
    m_settings.default_download_dir = m_cfg.default_download_dir.string();
    m_settings.stall_timeout_ms = m_cfg.stall_timeout_ms;
    m_settings.min_split_bytes = m_cfg.min_split_bytes;
}

DownloadController::~DownloadController() {
    Shutdown();
}

Result<void> DownloadController::Initialize() {
    std::error_code ec;
    std::filesystem::create_directories(m_cfg.default_download_dir, ec);
    std::filesystem::create_directories(m_cfg.temp_root_dir, ec);

    auto open_res = m_store.Open(m_cfg.state_db_path);
    if (!open_res.ok()) return open_res;

    auto rec_res = m_store.RecoverInterruptedTasks(65536);
    if (rec_res.ok() && *rec_res.value > 0) {
        OPENIDM_LOG_INFO("Controller", "Recovered " + std::to_string(*rec_res.value) + " interrupted tasks from SQLite WAL");
    }

    auto loaded = m_store.ListTasks();
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (loaded.ok()) {
            for (auto& t : *loaded.value) {
                m_tasks[t.task_id] = std::move(t);
            }
        }
        LoadPersistedSettingsLocked();
    }

    m_stopping = false;
    m_scheduler_thread = std::thread(&DownloadController::ScheduleLoop, this);
    return Result<void>::Ok();
}

void DownloadController::LoadPersistedSettingsLocked() {
    auto sres = m_store.GetSetting("app_settings", "");
    if (!sres.ok() || sres.value->empty()) return;
    try {
        auto val = json::Parse(*sres.value);
        if (!val.is_object()) return;
        m_settings.theme = val["theme"].as_string("dark");
        m_settings.max_concurrent_tasks = std::clamp<uint32_t>(
            static_cast<uint32_t>(val["maxConcurrentTasks"].as_int64(m_cfg.max_concurrent_tasks)), 1u, 16u);
        m_settings.speed_limit_bps = static_cast<uint64_t>(val["speedLimitBps"].as_int64(m_cfg.global_speed_limit_bps));
        m_settings.default_connections_per_task = std::clamp<uint32_t>(
            static_cast<uint32_t>(val["defaultConnectionsPerTask"].as_int64(8)), 1u, 32u);
        std::string saved_dir = val["defaultDownloadDir"].as_string("");
        if (!saved_dir.empty()) {
            m_settings.default_download_dir = saved_dir;
            m_cfg.default_download_dir = saved_dir;
        }
        m_settings.category_subfolders_enabled = val["categorySubfoldersEnabled"].as_bool(true);
        m_settings.conflict_policy = ConflictPolicyFromString(val["conflictPolicy"].as_string("auto_rename"));
        m_settings.auto_start_downloads = val["autoStartDownloads"].as_bool(true);
        m_settings.stall_timeout_ms = std::clamp<int>(
            static_cast<int>(val["stallTimeoutMs"].as_int64(m_cfg.stall_timeout_ms)), 500, 60000);
        m_settings.min_split_bytes = std::clamp<uint64_t>(
            static_cast<uint64_t>(val["minSplitBytes"].as_int64(static_cast<int64_t>(m_cfg.min_split_bytes))),
            65536ULL, 10485760ULL);
        m_settings.browser_capture_enabled = val["browserCaptureEnabled"].as_bool(true);
        m_settings.min_capture_size_bytes = std::clamp<uint64_t>(
            static_cast<uint64_t>(val["minCaptureSizeBytes"].as_int64(1048576)),
            0ULL, 10737418240ULL);
        m_settings.close_to_tray = val["closeToTray"].as_bool(true);
        m_settings.start_minimized = val["startMinimized"].as_bool(false);
        m_settings.reduced_motion = val["reducedMotion"].as_bool(false);
        m_settings.show_completion_notification = val["showCompletionNotification"].as_bool(true);

        m_cfg.stall_timeout_ms = m_settings.stall_timeout_ms;
        m_cfg.min_split_bytes = m_settings.min_split_bytes;
        m_max_concurrent.store(m_settings.max_concurrent_tasks);
        m_global_speed_limit_bps.store(m_settings.speed_limit_bps);
    } catch (...) {}
}

Result<void> DownloadController::SaveSettingsLocked() {
    json::Value obj = json::Object{};
    obj["theme"] = m_settings.theme;
    obj["maxConcurrentTasks"] = static_cast<int64_t>(m_settings.max_concurrent_tasks);
    obj["speedLimitBps"] = static_cast<int64_t>(m_settings.speed_limit_bps);
    obj["defaultConnectionsPerTask"] = static_cast<int64_t>(m_settings.default_connections_per_task);
    obj["defaultDownloadDir"] = m_settings.default_download_dir;
    obj["categorySubfoldersEnabled"] = m_settings.category_subfolders_enabled;
    obj["conflictPolicy"] = ConflictPolicyToString(m_settings.conflict_policy);
    obj["autoStartDownloads"] = m_settings.auto_start_downloads;
    obj["stallTimeoutMs"] = static_cast<int64_t>(m_settings.stall_timeout_ms);
    obj["minSplitBytes"] = static_cast<int64_t>(m_settings.min_split_bytes);
    obj["browserCaptureEnabled"] = m_settings.browser_capture_enabled;
    obj["minCaptureSizeBytes"] = static_cast<int64_t>(m_settings.min_capture_size_bytes);
    obj["closeToTray"] = m_settings.close_to_tray;
    obj["startMinimized"] = m_settings.start_minimized;
    obj["reducedMotion"] = m_settings.reduced_motion;
    obj["showCompletionNotification"] = m_settings.show_completion_notification;
    return m_store.SetSetting("app_settings", obj.dump());
}

void DownloadController::Shutdown() {
    std::vector<std::shared_ptr<ActiveTaskControl>> to_join;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_stopping) return;
        m_stopping = true;
        for (auto& [id, ctrl] : m_active) {
            ctrl->pause_requested.store(true);
            to_join.push_back(ctrl);
        }
    }
    m_cv.notify_all();
    if (m_scheduler_thread.joinable()) {
        m_scheduler_thread.join();
    }
    for (auto& ctrl : to_join) {
        if (ctrl->worker_thread.joinable()) {
            ctrl->worker_thread.join();
        }
    }
    m_store.Close();
}

std::filesystem::path DownloadController::ResolveCategorySaveDir(const std::string& category) const {
    if (!m_settings.category_subfolders_enabled || category.empty() || category == "General") {
        return m_cfg.default_download_dir;
    }
    return m_cfg.default_download_dir / category;
}

Result<TaskModel> DownloadController::AddTask(TaskModel task, bool start_immediately, bool force_duplicate) {
    if (task.url.empty()) {
        return Result<TaskModel>::Err("ERR_INVALID_URL", "Task URL cannot be empty");
    }
    if (task.url.find("ftp://") == 0 || task.url.find("ftps://") == 0) {
        task.task_type = TaskType::kFtp;
        auto ftp_res = ftp::FtpUrl::Parse(task.url);
        if (!ftp_res.ok()) return Result<TaskModel>::Err(ftp_res.error->code, ftp_res.error->message);
        if (task.file_name.empty()) task.file_name = ftp_res.value->file_name;
    } else if (task.url.find("magnet:?") == 0) {
        task.task_type = TaskType::kTorrent;
        auto mag_res = torrent::TorrentParser::ParseMagnetUri(task.url);
        if (!mag_res.ok()) return Result<TaskModel>::Err(mag_res.error->code, mag_res.error->message);
        if (task.file_name.empty()) task.file_name = mag_res.value->name;
    } else if (stream::RtspMmsAdapter::IsStreamUrl(task.url)) {
        task.task_type = TaskType::kRtsp;
        if (task.file_name.empty()) task.file_name = "stream_" + GenerateUuidV4().substr(0, 8) + ".mp4";
    } else {
        if (task.task_type == TaskType::kHttpFile && PathSanitizer::IsWebpageWatchUrl(task.url)) {
            return Result<TaskModel>::Err(
                "ERR_URL_IS_WEBPAGE",
                "The specified URL is an HTML webpage, not a direct media file or stream. "
                "To download video from this site, play it in your web browser with the Flux extension enabled."
            );
        }
        auto parsed_u = brisk::HttpClient::ParseUrl(task.url);
        if (!parsed_u.ok()) {
            return Result<TaskModel>::Err(parsed_u.error->code, parsed_u.error->message);
        }
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!force_duplicate) {
        for (const auto& [id, existing] : m_tasks) {
            if (existing.url == task.url &&
                (existing.state == TaskState::kQueued ||
                 existing.state == TaskState::kProbing ||
                 existing.state == TaskState::kDownloading ||
                 existing.state == TaskState::kMuxing)) {
                return Result<TaskModel>::Ok(existing);
            }
        }
    }

    if (task.task_id.empty()) {
        task.task_id = GenerateUuidV4();
    }
    m_deleted_tasks.erase(task.task_id);

    if (task.file_name.empty()) {
        std::string fallback_ext = (task.task_type == TaskType::kHttpFile) ? "" : task.stream_options.output_format;
        task.file_name = PathSanitizer::ExtractFromUrl(task.url, fallback_ext);
    } else {
        task.file_name = PathSanitizer::SanitizeFileName(task.file_name);
    }
    task.category = InferCategoryFromFilename(task.file_name);
    if (task.save_dir.empty()) {
        task.save_dir = ResolveCategorySaveDir(task.category).string();
    }
    if (task.max_connections == 0) {
        task.max_connections = m_settings.default_connections_per_task;
    }
    task.conflict_policy = m_settings.conflict_policy;
    if (task.created_at_ms <= 0) {
        task.created_at_ms = NowEpochMs();
    }
    task.updated_at_ms = task.created_at_ms;
    task.state = (start_immediately && !task.scheduled) ? TaskState::kQueued : TaskState::kPaused;
    if (task.state == TaskState::kPaused) {
        task.pause_reason = PauseReason::kUser;
    }
    task.status_detail = (task.state == TaskState::kQueued)
                             ? "Queued"
                             : (task.scheduled ? ("Scheduled (" + task.schedule_time_local + ")") : "Paused");

    auto db_res = m_store.UpsertTask(task);
    if (!db_res.ok()) {
        return Result<TaskModel>::Err(db_res.error->code, db_res.error->message);
    }

    m_tasks[task.task_id] = task;
    m_cv.notify_all();
    return Result<TaskModel>::Ok(task);
}

Result<std::vector<TaskModel>> DownloadController::AddBatchTasks(
    const std::vector<std::string>& urls,
    const std::string& page_url,
    const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& auth_headers) {

    std::vector<TaskModel> added;
    for (const auto& u : urls) {
        TaskModel t;
        t.url = u;
        t.page_url = page_url;
        t.headers = headers;
        t.auth_headers = auth_headers;
        if (u.find(".m3u8") != std::string::npos) {
            t.task_type = TaskType::kStreamHls;
        } else if (u.find(".mpd") != std::string::npos) {
            t.task_type = TaskType::kStreamDash;
        } else {
            t.task_type = TaskType::kHttpFile;
        }
        auto res = AddTask(std::move(t), true, false);
        if (res.ok()) {
            added.push_back(*res.value);
        }
    }
    return Result<std::vector<TaskModel>>::Ok(std::move(added));
}

Result<stream::StreamProbeResult> DownloadController::ProbeStream(
    const std::string& manifest_url,
    const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& auth_headers,
    const std::string& cached_manifest_body) {

    stream::StreamAdapterConfig scfg{
        m_cfg.nm3u8dl_binary_path,
        m_cfg.ffmpeg_binary_path,
        m_cfg.temp_root_dir,
        m_cfg.allow_loopback
    };
    stream::Nm3u8dlReAdapter adapter(std::move(scfg));
    return adapter.ProbeManifest(manifest_url, headers, auth_headers, cached_manifest_body);
}

void DownloadController::ScheduleLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait_for(lock, std::chrono::milliseconds(50), [&]() { return m_stopping; });
        if (m_stopping) break;

        for (auto it = m_active.begin(); it != m_active.end();) {
            if (it->second->finished.load()) {
                if (it->second->worker_thread.joinable()) {
                    it->second->worker_thread.join();
                }
                it = m_active.erase(it);
            } else {
                ++it;
            }
        }

        // 1. Check for scheduled tasks that reached their local schedule time
        auto now_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm local_tm{};
#ifdef _WIN32
        localtime_s(&local_tm, &now_time_t);
#else
        localtime_r(&now_time_t, &local_tm);
#endif
        char time_str[16] = {0};
        std::strftime(time_str, sizeof(time_str), "%H:%M", &local_tm);
        std::string cur_hh_mm(time_str);

        for (auto& [id, t] : m_tasks) {
            if (t.scheduled && t.state == TaskState::kPaused && !t.schedule_time_local.empty()) {
                if (cur_hh_mm >= t.schedule_time_local) {
                    t.scheduled = false;
                    t.state = TaskState::kQueued;
                    t.status_detail = "Queued (Schedule Started)";
                    m_store.UpsertTask(t);
                }
            }
        }

        uint32_t max_conc = m_max_concurrent.load();
        while (m_active.size() < max_conc) {
            std::string next_id;
            int32_t min_order = INT32_MAX;
            int64_t oldest_ts = INT64_MAX;
            for (const auto& [id, t] : m_tasks) {
                if (t.state == TaskState::kQueued && m_active.find(id) == m_active.end()) {
                    if (t.queue_order < min_order || (t.queue_order == min_order && t.created_at_ms < oldest_ts)) {
                        min_order = t.queue_order;
                        oldest_ts = t.created_at_ms;
                        next_id = id;
                    }
                }
            }
            if (next_id.empty()) break;

            m_tasks[next_id].state = TaskState::kProbing;
            m_tasks[next_id].status_detail = "Starting transfer...";
            auto ctrl = std::make_shared<ActiveTaskControl>();
            ctrl->task_speed_limit_bps.store(m_global_speed_limit_bps.load());
            m_active[next_id] = ctrl;
            ctrl->worker_thread = std::thread(&DownloadController::RunTaskWorker, this, next_id, ctrl);
        }
    }
}

void DownloadController::RunTaskWorker(std::string task_id, std::shared_ptr<ActiveTaskControl> ctrl) {
    TaskModel local_task;
    int stall_timeout_ms = 3000;
    uint64_t min_split_bytes = 131072;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(task_id);
        if (it == m_tasks.end() || m_deleted_tasks.count(task_id)) {
            ctrl->finished.store(true);
            return;
        }
        local_task = it->second;
        stall_timeout_ms = m_cfg.stall_timeout_ms;
        min_split_bytes = m_cfg.min_split_bytes;
    }

    std::string task_type_str = TaskTypeToString(local_task.task_type);
    if (local_task.url.find("youtube.com") != std::string::npos ||
        local_task.url.find("watch?v=") != std::string::npos ||
        local_task.page_url.find("youtube.com") != std::string::npos ||
        local_task.page_url.find("watch?v=") != std::string::npos ||
        local_task.stream_options.cached_manifest_body.find("YOUTUBE") != std::string::npos) {
        task_type_str = "YOUTUBE_EXTRACTION";
    }

    OPENIDM_LOG_INFO("Controller", "Dispatching task=" + task_id + " type=" + task_type_str +
                     " correlation_id=" + local_task.correlation_id + " url=" + local_task.url);

    auto last_db_sync = std::chrono::steady_clock::now();

    if (local_task.task_type == TaskType::kFtp) {
        auto ftp_url_res = ftp::FtpUrl::Parse(local_task.url);
        if (ftp_url_res.ok()) {
            std::filesystem::path part_p = std::filesystem::path(local_task.save_dir) / (local_task.file_name + "." + local_task.task_id + ".part");
            local_task.part_path = part_p.string();
            local_task.state = TaskState::kDownloading;

            auto dl_res = ftp::FtpClient::DownloadFile(
                ftp_url_res.value.value(),
                part_p,
                local_task.downloaded_bytes,
                ctrl->pause_requested,
                ctrl->cancel_requested,
                ctrl->task_speed_limit_bps,
                [&](const ftp::FtpProgress& fp) {
                    {
                        std::lock_guard<std::mutex> lock(m_mutex);
                        auto it = m_tasks.find(task_id);
                        if (it != m_tasks.end() && !m_deleted_tasks.count(task_id)) {
                            it->second.state = fp.state;
                            it->second.downloaded_bytes = fp.downloaded_bytes;
                            it->second.total_bytes = fp.total_bytes;
                            it->second.speed_bps = fp.speed_bps;
                            it->second.status_detail = fp.status_detail;
                        }
                    }
                },
                m_global_limiter,
                10000,
                m_cfg.allow_loopback);

            if (dl_res.ok()) {
                std::error_code ec;
                std::filesystem::path final_p = std::filesystem::path(local_task.save_dir) / local_task.file_name;
                std::filesystem::rename(part_p, final_p, ec);
                local_task.state = TaskState::kCompleted;
                local_task.downloaded_bytes = dl_res.value.value();
                local_task.total_bytes = dl_res.value.value();
                local_task.status_detail = "Completed (FTP)";
            } else {
                if (ctrl->cancel_requested.load()) {
                    local_task.state = TaskState::kCanceled;
                } else if (ctrl->pause_requested.load()) {
                    local_task.state = TaskState::kPaused;
                } else {
                    local_task.state = TaskState::kFailed;
                    local_task.error_code = dl_res.error->code;
                    local_task.error_message = dl_res.error->message;
                }
            }
        }
    } else if (local_task.task_type == TaskType::kTorrent) {
        torrent::TorrentInfo tinfo;
        if (local_task.url.find("magnet:?") == 0) {
            auto mag_res = torrent::TorrentParser::ParseMagnetUri(local_task.url);
            if (mag_res.ok()) tinfo = mag_res.value.value();
        } else {
            auto tor_res = torrent::TorrentParser::ParseTorrentFile(local_task.url);
            if (tor_res.ok()) tinfo = tor_res.value.value();
        }
        auto tor_dl = torrent::TorrentEngine::ExecuteTorrentDownload(
            local_task,
            tinfo,
            ctrl->pause_requested,
            ctrl->cancel_requested,
            ctrl->task_speed_limit_bps,
            [&](const torrent::TorrentProgress& tp) {
                std::lock_guard<std::mutex> lock(m_mutex);
                auto it = m_tasks.find(task_id);
                if (it != m_tasks.end() && !m_deleted_tasks.count(task_id)) {
                    it->second.state = tp.state;
                    it->second.downloaded_bytes = tp.downloaded_bytes;
                    it->second.total_bytes = tp.total_bytes;
                    it->second.speed_bps = tp.speed_bps;
                    it->second.completed_segments = tp.completed_pieces;
                    it->second.total_segments = tp.total_pieces;
                    it->second.status_detail = tp.status_detail;
                }
            },
            m_global_limiter);
        (void)tor_dl;
    } else if (local_task.task_type == TaskType::kRtsp) {
        stream::RtspMmsAdapter rtsp_ad(m_cfg.ffmpeg_binary_path, m_cfg.allow_loopback);
        auto rtsp_res = rtsp_ad.ExecuteStreamCapture(
            local_task,
            ctrl->pause_requested,
            ctrl->cancel_requested,
            [&](const stream::RtspProgress& rp) {
                std::lock_guard<std::mutex> lock(m_mutex);
                auto it = m_tasks.find(task_id);
                if (it != m_tasks.end() && !m_deleted_tasks.count(task_id)) {
                    it->second.state = rp.state;
                    it->second.downloaded_bytes = rp.downloaded_bytes;
                    it->second.speed_bps = rp.speed_bps;
                    it->second.status_detail = rp.status_detail;
                }
            });
        (void)rtsp_res;
    } else if (local_task.task_type == TaskType::kHttpFile) {
        brisk::EngineConfig ecfg;
        ecfg.stall_timeout_ms = stall_timeout_ms;
        ecfg.min_split_bytes = min_split_bytes;
        ecfg.allow_loopback_redirect = m_cfg.allow_loopback;

        brisk::HttpDownloadEngine engine(ecfg);
        auto res = engine.ExecuteDownload(
            local_task,
            ctrl->pause_requested,
            ctrl->cancel_requested,
            ctrl->task_speed_limit_bps,
            [&](const brisk::EngineProgress& ep) {
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    auto it = m_tasks.find(task_id);
                    if (it != m_tasks.end() && !m_deleted_tasks.count(task_id)) {
                        auto& t = it->second;
                        t.state = ep.state;
                        t.downloaded_bytes = ep.downloaded_bytes;
                        t.total_bytes = ep.total_bytes;
                        t.speed_bps = ep.speed_bps;
                        t.range_supported = ep.range_supported;
                        t.status_detail = local_task.status_detail;
                        if (!ep.resolved_filename.empty()) t.file_name = ep.resolved_filename;
                        if (!ep.etag.empty()) t.etag = ep.etag;
                        if (!ep.last_modified.empty()) t.last_modified = ep.last_modified;
                        t.segments = ep.segments;
                    }
                }
                auto now = std::chrono::steady_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_db_sync).count() >= 500) {
                    last_db_sync = now;
                    m_store.UpdateTaskProgressAndSegments(
                        task_id, ep.state, ep.downloaded_bytes, ep.total_bytes, ep.range_supported, ep.segments);
                }
            },
            m_global_limiter);
        (void)res;
    } else {
        stream::StreamAdapterConfig scfg{
            m_cfg.nm3u8dl_binary_path,
            m_cfg.ffmpeg_binary_path,
            m_cfg.temp_root_dir,
            m_cfg.allow_loopback
        };
        stream::Nm3u8dlReAdapter adapter(std::move(scfg));
        Result<std::filesystem::path> res;
        bool is_yt = (task_type_str == "YOUTUBE_EXTRACTION" ||
                      local_task.url.find("googlevideo.com") != std::string::npos ||
                      local_task.url.find("youtube.com/watch") != std::string::npos ||
                      local_task.url.find("youtu.be/") != std::string::npos ||
                      local_task.url.find("youtube.com/shorts/") != std::string::npos ||
                      local_task.stream_options.cached_manifest_body.find("\"fluxType\":\"YOUTUBE_STREAM\"") != std::string::npos ||
                      local_task.stream_options.cached_manifest_body.find("\"fluxType\": \"YOUTUBE_STREAM\"") != std::string::npos);
        auto progress_cb = [&](const stream::StreamProgress& sp) {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_tasks.find(task_id);
            if (it != m_tasks.end() && !m_deleted_tasks.count(task_id)) {
                auto& t = it->second;
                t.state = sp.phase;
                t.downloaded_bytes = sp.downloaded_bytes;
                t.total_bytes = sp.total_bytes;
                t.speed_bps = sp.speed_bps;
                t.stream_percent = sp.percent;
                t.completed_segments = sp.completed_segments;
                t.total_segments = sp.total_segments;
                t.status_detail = sp.status_line;
            }
        };

        if (is_yt) {
            res = adapter.ExecuteYouTubeExtraction(
                local_task,
                ctrl->pause_requested,
                ctrl->cancel_requested,
                ctrl->task_speed_limit_bps.load(),
                progress_cb);
        } else {
            res = adapter.ExecuteStreamDownload(
                local_task,
                ctrl->pause_requested,
                ctrl->cancel_requested,
                ctrl->task_speed_limit_bps.load(),
                progress_cb);
        }
        (void)res;
    }

    local_task.speed_bps = 0;
    local_task.updated_at_ms = NowEpochMs();

    // BUG-007 Fix: Granular merge under m_mutex without resurrecting deleted tasks
    bool should_persist = false;
    TaskModel to_save;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_deleted_tasks.count(task_id)) {
            auto it = m_tasks.find(task_id);
            if (it != m_tasks.end()) {
                auto& t = it->second;
                t.state = local_task.state;
                t.file_name = local_task.file_name;
                t.category = local_task.category;
                t.part_path = local_task.part_path;
                t.total_bytes = local_task.total_bytes;
                t.downloaded_bytes = local_task.downloaded_bytes;
                t.speed_bps = 0;
                t.stream_percent = local_task.stream_percent;
                t.completed_segments = local_task.completed_segments;
                t.total_segments = local_task.total_segments;
                t.range_supported = local_task.range_supported;
                t.status_detail = local_task.status_detail;
                t.etag = local_task.etag;
                t.last_modified = local_task.last_modified;
                t.segments = local_task.segments;
                t.error_code = local_task.error_code;
                t.error_message = local_task.error_message;
                t.updated_at_ms = local_task.updated_at_ms;
                t.completed_at_ms = local_task.completed_at_ms;
                if (t.state == TaskState::kCompleted || t.state == TaskState::kCanceled) {
                    t.auth_headers.clear();
                }
                if (t.state == TaskState::kCompleted) {
                    // Post-Download Antivirus Scan
                    if (security::AntivirusScanner::IsWindowsDefenderAvailable()) {
                        std::filesystem::path final_p = std::filesystem::path(t.save_dir) / t.file_name;
                        std::error_code ec;
                        if (std::filesystem::exists(final_p, ec)) {
                            auto scan_res = security::AntivirusScanner::ScanWithDefender(final_p);
                            if (scan_res.ok()) {
                                if (scan_res.value->status == security::ScanStatus::kThreatDetected) {
                                    t.status_detail = "Threat detected by Antivirus (" + scan_res.value->scanner_name + ")";
                                    OPENIDM_LOG_WARN("Controller", "Antivirus threat detected: " + final_p.string());
                                } else if (scan_res.value->status == security::ScanStatus::kClean) {
                                    t.status_detail = "Completed (Antivirus Clean)";
                                }
                            }
                        }
                    }
#ifdef _WIN32
                    if (m_settings.show_completion_notification) {
                        MessageBeep(MB_ICONASTERISK);
                    }
#endif
                }
                to_save = t;
                should_persist = true;
            }
        }
    }
    if (should_persist) {
        m_store.UpsertTask(to_save);
    }

    ctrl->finished.store(true);
    m_cv.notify_all();
}

Result<void> DownloadController::StartTask(const std::string& task_id) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
    if (it->second.state == TaskState::kPaused || it->second.state == TaskState::kFailed ||
        it->second.state == TaskState::kQueued) {
        it->second.scheduled = false;
        it->second.state = TaskState::kQueued;
        it->second.status_detail = "Queued";
        it->second.error_code.clear();
        it->second.error_message.clear();
        m_store.UpsertTask(it->second);
        m_cv.notify_all();
    }
    return Result<void>::Ok();
}

Result<void> DownloadController::PauseTask(const std::string& task_id) {
    std::shared_ptr<ActiveTaskControl> ctrl;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(task_id);
        if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
        it->second.pause_reason = PauseReason::kUser;
        if (it->second.state == TaskState::kQueued) {
            it->second.state = TaskState::kPaused;
            it->second.status_detail = "Paused";
            m_store.UpsertTask(it->second);
            return Result<void>::Ok();
        }
        auto ait = m_active.find(task_id);
        if (ait != m_active.end()) {
            ctrl = ait->second;
            ctrl->pause_requested.store(true);
        }
    }
    if (ctrl) {
        while (!ctrl->finished.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(task_id);
        if (it != m_tasks.end() && it->second.state == TaskState::kPaused) {
            it->second.pause_reason = PauseReason::kUser;
            m_store.UpsertTask(it->second);
        }
    }
    return Result<void>::Ok();
}

Result<void> DownloadController::ResumeTask(const std::string& task_id) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
    if (it->second.state == TaskState::kPaused || it->second.state == TaskState::kFailed) {
        it->second.scheduled = false;
        it->second.pause_reason = PauseReason::kNone;
        it->second.state = TaskState::kQueued;
        it->second.status_detail = "Queued (Resuming)";
        it->second.error_code.clear();
        it->second.error_message.clear();
        m_store.UpsertTask(it->second);
        m_cv.notify_all();
    }
    return Result<void>::Ok();
}

Result<void> DownloadController::CancelTask(const std::string& task_id) {
    std::shared_ptr<ActiveTaskControl> ctrl;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(task_id);
        if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
        if (it->second.state == TaskState::kCompleted) {
            return Result<void>::Err("ERR_ALREADY_COMPLETED", "Completed task cannot be canceled");
        }
        auto ait = m_active.find(task_id);
        if (ait != m_active.end() && !ait->second->finished.load()) {
            ctrl = ait->second;
            ctrl->cancel_requested.store(true);
        } else {
            std::error_code ec;
            if (!it->second.part_path.empty()) {
                std::filesystem::remove_all(it->second.part_path, ec);
            }
            it->second.part_path.clear();
            it->second.segments.clear();
            it->second.downloaded_bytes = 0;
            it->second.speed_bps = 0;
            it->second.state = TaskState::kCanceled;
            it->second.status_detail = "Canceled";
            it->second.auth_headers.clear();
            m_store.UpsertTask(it->second);
            return Result<void>::Ok();
        }
    }
    if (ctrl) {
        while (!ctrl->finished.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    return Result<void>::Ok();
}

Result<void> DownloadController::RetryTask(const std::string& task_id) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
    auto& t = it->second;
    if (t.state != TaskState::kFailed && t.state != TaskState::kCanceled && t.state != TaskState::kPaused) {
        return Result<void>::Err("ERR_INVALID_STATE", "Only failed, canceled, or paused tasks can be retried");
    }

    // If task was canceled or failed due to validator mismatch / corruption, reset segments for a fresh download
    if (t.state == TaskState::kCanceled ||
        t.error_code == "ERR_HTTP_VALIDATOR_MISMATCH" ||
        t.error_code == "ERR_STORAGE_PART_CORRUPTED") {
        std::error_code ec;
        if (!t.part_path.empty()) {
            std::filesystem::remove_all(t.part_path, ec);
        }
        t.part_path.clear();
        t.segments.clear();
        t.downloaded_bytes = 0;
        t.etag.clear();
        t.last_modified.clear();
    }

    ++t.retry_count;
    t.state = TaskState::kQueued;
    t.status_detail = "Queued (Retry #" + std::to_string(t.retry_count) + ")";
    t.error_code.clear();
    t.error_message.clear();
    m_store.UpsertTask(t);
    m_cv.notify_all();
    return Result<void>::Ok();
}

Result<void> DownloadController::RemoveTask(const std::string& task_id, bool delete_file) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(task_id);
        if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
        m_deleted_tasks.insert(task_id);
    }

    // Stop active worker if running (without failing if already completed)
    std::shared_ptr<ActiveTaskControl> ctrl;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto ait = m_active.find(task_id);
        if (ait != m_active.end() && !ait->second->finished.load()) {
            ctrl = ait->second;
            ctrl->cancel_requested.store(true);
        }
    }
    if (ctrl) {
        while (!ctrl->finished.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it != m_tasks.end()) {
        std::error_code ec;
        // Always clean up incomplete .openidm.part or temp directory
        if (!it->second.part_path.empty()) {
            std::filesystem::remove_all(it->second.part_path, ec);
        }
        // Only delete completed user output file if delete_file == true was explicitly requested!
        if (delete_file && it->second.state == TaskState::kCompleted) {
            std::filesystem::path fp = std::filesystem::path(it->second.save_dir) / it->second.file_name;
            std::filesystem::remove(fp, ec);
        }
        m_tasks.erase(it);
    }
    return m_store.DeleteTask(task_id);
}

Result<void> DownloadController::UpdateTask(const TaskModel& task) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task.task_id);
    if (it == m_tasks.end() || m_deleted_tasks.count(task.task_id)) {
        return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
    }
    it->second = task;
    it->second.updated_at_ms = NowEpochMs();
    auto db_res = m_store.UpsertTask(it->second);
    if (!db_res.ok()) {
        return Result<void>::Err(db_res.error->code, db_res.error->message);
    }
    m_cv.notify_all();
    return Result<void>::Ok();
}

Result<int> DownloadController::ClearCompletedHistory() {
    std::vector<std::string> to_remove;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& [id, t] : m_tasks) {
            if (t.state == TaskState::kCompleted || t.state == TaskState::kCanceled) {
                to_remove.push_back(id);
            }
        }
        for (const auto& id : to_remove) {
            m_deleted_tasks.insert(id);
            m_tasks.erase(id);
        }
    }
    for (const auto& id : to_remove) {
        m_store.DeleteTask(id);
    }
    return Result<int>::Ok(static_cast<int>(to_remove.size()));
}

void DownloadController::PauseAllTasks() {
    std::vector<std::string> ids;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& [id, t] : m_tasks) {
            if (t.state == TaskState::kQueued || t.state == TaskState::kProbing ||
                t.state == TaskState::kDownloading || t.state == TaskState::kMuxing) {
                ids.push_back(id);
            }
        }
    }
    for (const auto& id : ids) {
        PauseTask(id);
    }
}

void DownloadController::ResumeAllTasks() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& [id, t] : m_tasks) {
        if (t.state == TaskState::kPaused && !t.scheduled) {
            t.state = TaskState::kQueued;
            t.pause_reason = PauseReason::kNone;
            t.status_detail = "Queued";
            m_store.UpsertTask(t);
        }
    }
    m_cv.notify_all();
}

Result<void> DownloadController::RefreshTaskUrl(
    const std::string& task_id,
    const std::string& new_url,
    const std::map<std::string, std::string>& new_headers,
    const std::map<std::string, std::string>& new_auth_headers) {

    PauseTask(task_id);
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");

    it->second.url = new_url;
    for (const auto& [k, v] : new_headers) it->second.headers[k] = v;
    for (const auto& [k, v] : new_auth_headers) it->second.auth_headers[k] = v;
    it->second.state = TaskState::kQueued;
    it->second.status_detail = "Queued (Refreshed URL)";
    it->second.error_code.clear();
    it->second.error_message.clear();
    m_store.UpsertTask(it->second);
    m_cv.notify_all();
    return Result<void>::Ok();
}

void DownloadController::SetMaxConcurrentTasks(uint32_t max_tasks) {
    uint32_t clamped = std::clamp(max_tasks, 1u, 16u);
    m_max_concurrent.store(clamped);
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_settings.max_concurrent_tasks = clamped;
        SaveSettingsLocked();
    }
    m_cv.notify_all();
}

uint32_t DownloadController::GetMaxConcurrentTasks() const {
    return m_max_concurrent.load();
}

void DownloadController::SetGlobalSpeedLimit(uint64_t bps) {
    m_global_speed_limit_bps.store(bps);
    std::lock_guard<std::mutex> lock(m_mutex);
    m_settings.speed_limit_bps = bps;
    for (auto& [id, ctrl] : m_active) {
        ctrl->task_speed_limit_bps.store(bps);
    }
    SaveSettingsLocked();
}

uint64_t DownloadController::GetGlobalSpeedLimit() const {
    return m_global_speed_limit_bps.load();
}

AppSettings DownloadController::GetSettings() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_settings;
}

Result<void> DownloadController::UpdateSettings(const AppSettings& settings) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_settings = settings;
        m_settings.max_concurrent_tasks = std::clamp(m_settings.max_concurrent_tasks, 1u, 16u);
        m_settings.default_connections_per_task = std::clamp(m_settings.default_connections_per_task, 1u, 32u);
        m_settings.stall_timeout_ms = std::clamp(m_settings.stall_timeout_ms, 500, 60000);
        m_settings.min_split_bytes = std::clamp<uint64_t>(m_settings.min_split_bytes, 65536ULL, 10485760ULL);
        m_settings.min_capture_size_bytes = std::clamp<uint64_t>(m_settings.min_capture_size_bytes, 0ULL, 10737418240ULL);
        if (m_settings.theme != "dark" && m_settings.theme != "light" && m_settings.theme != "system") {
            m_settings.theme = "dark";
        }
        if (!m_settings.default_download_dir.empty()) {
            m_cfg.default_download_dir = m_settings.default_download_dir;
            std::error_code ec;
            std::filesystem::create_directories(m_cfg.default_download_dir, ec);
        }
        m_cfg.stall_timeout_ms = m_settings.stall_timeout_ms;
        m_cfg.min_split_bytes = m_settings.min_split_bytes;
        m_max_concurrent.store(m_settings.max_concurrent_tasks);
        m_global_speed_limit_bps.store(m_settings.speed_limit_bps);
        for (auto& [id, ctrl] : m_active) {
            ctrl->task_speed_limit_bps.store(m_settings.speed_limit_bps);
        }
        auto sres = SaveSettingsLocked();
        if (!sres.ok()) return sres;
    }
    m_cv.notify_all();
    return Result<void>::Ok();
}

Result<AppSettings> DownloadController::RestoreDefaultSettings() {
    AppSettings defaults;
    defaults.theme = "dark";
    defaults.max_concurrent_tasks = 3;
    defaults.speed_limit_bps = 0;
    defaults.default_connections_per_task = 8;
    defaults.default_download_dir = PathSanitizer::GetDefaultDownloadsDir(PathSanitizer::GetExecutableDir()).string();
    defaults.category_subfolders_enabled = true;
    defaults.conflict_policy = ConflictPolicy::kAutoRename;
    defaults.auto_start_downloads = true;
    defaults.stall_timeout_ms = 3000;
    defaults.min_split_bytes = 131072;
    defaults.browser_capture_enabled = true;
    defaults.min_capture_size_bytes = 1048576;
    defaults.close_to_tray = true;
    defaults.start_minimized = false;
    defaults.reduced_motion = false;
    defaults.show_completion_notification = true;
    auto ures = UpdateSettings(defaults);
    if (!ures.ok()) return Result<AppSettings>::Err(ures.error->code, ures.error->message);
    return Result<AppSettings>::Ok(defaults);
}

bool DownloadController::AreWorkersAvailable() const {
    std::error_code ec;
    return std::filesystem::exists(m_cfg.nm3u8dl_binary_path, ec) &&
           std::filesystem::exists(m_cfg.ffmpeg_binary_path, ec);
}

void DownloadController::OnSystemSuspend() {
    std::vector<std::string> ids;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& [id, t] : m_tasks) {
            if (t.state == TaskState::kQueued || t.state == TaskState::kProbing ||
                t.state == TaskState::kDownloading || t.state == TaskState::kMuxing) {
                ids.push_back(id);
            }
        }
    }
    for (const auto& id : ids) {
        PauseTask(id);
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& id : ids) {
            auto it = m_tasks.find(id);
            if (it != m_tasks.end() && it->second.state == TaskState::kPaused) {
                it->second.pause_reason = PauseReason::kSystemSuspend;
                it->second.status_detail = "Paused (System Suspend)";
                m_store.UpsertTask(it->second);
            }
        }
    }
}

void DownloadController::OnSystemResume() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& [id, t] : m_tasks) {
        // Only resume tasks that were automatically paused due to system suspend (CTRL-02)
        // User-paused tasks (pause_reason == kUser) remain strictly paused!
        if (t.state == TaskState::kPaused && t.pause_reason == PauseReason::kSystemSuspend && !t.scheduled) {
            t.state = TaskState::kQueued;
            t.pause_reason = PauseReason::kNone;
            t.status_detail = "Queued (Resumed after sleep)";
            m_store.UpsertTask(t);
        }
    }
    m_cv.notify_all();
}

std::vector<TaskModel> DownloadController::ListTasks() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<TaskModel> out;
    out.reserve(m_tasks.size());
    for (const auto& [id, t] : m_tasks) {
        out.push_back(t);
    }
    std::sort(out.begin(), out.end(), [](const TaskModel& a, const TaskModel& b) {
        return a.created_at_ms > b.created_at_ms;
    });
    return out;
}

std::optional<TaskModel> DownloadController::GetTask(const std::string& task_id) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it != m_tasks.end()) return it->second;
    return std::nullopt;
}

Result<TaskModel> DownloadController::WaitForTaskCompletion(const std::string& task_id, int timeout_ms) {
    auto start = std::chrono::steady_clock::now();
    while (true) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_tasks.find(task_id);
            if (it == m_tasks.end()) {
                return Result<TaskModel>::Err("ERR_NOT_FOUND", "Task not found");
            }
            auto st = it->second.state;
            auto ait = m_active.find(task_id);
            bool worker_done = (ait == m_active.end() || ait->second->finished.load());
            if (worker_done && (st == TaskState::kCompleted || st == TaskState::kFailed ||
                                st == TaskState::kPaused || st == TaskState::kCanceled)) {
                return Result<TaskModel>::Ok(it->second);
            }
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();
        if (elapsed > timeout_ms) {
            return Result<TaskModel>::Err("ERR_TIMEOUT", "Timed out waiting for task completion");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}

Result<void> DownloadController::UpsertQueue(const QueueConfig& queue) {
    return m_store.UpsertQueue(queue);
}

std::vector<QueueConfig> DownloadController::ListQueues() {
    auto res = m_store.ListQueues();
    return res.ok() ? res.value.value() : std::vector<QueueConfig>{};
}

Result<void> DownloadController::MoveTaskToQueue(const std::string& task_id, const std::string& new_queue_id) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
    it->second.queue_id = new_queue_id;
    m_store.UpsertTask(it->second);
    return Result<void>::Ok();
}

Result<std::vector<crawler::GrabbedItem>> DownloadController::CrawlSite(
    const crawler::SiteGrabberConfig& cfg,
    std::atomic<bool>& cancel_requested,
    crawler::GrabbedItemCallback on_item_found) {
    return crawler::SiteGrabber::Crawl(cfg, cancel_requested, on_item_found);
}

Result<security::ScanReport> DownloadController::ScanTaskFile(const std::string& task_id) {
    std::filesystem::path file_p;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(task_id);
        if (it == m_tasks.end()) return Result<security::ScanReport>::Err("ERR_NOT_FOUND", "Task not found");
        file_p = std::filesystem::path(it->second.save_dir) / it->second.file_name;
    }
    return security::AntivirusScanner::ScanWithDefender(file_p);
}

Result<archive::ZipArchiveSummary> DownloadController::PreviewZipArchive(const std::filesystem::path& zip_path) {
    return archive::ZipPreviewer::InspectArchive(zip_path);
}

Result<std::string> DownloadController::ExportTasksData(storage::ExportFormat format, bool include_credentials) {
    std::vector<TaskModel> task_list = ListTasks();
    return storage::TaskImportExport::ExportTasksToString(task_list, format, include_credentials);
}

Result<int> DownloadController::ImportTasksData(const std::string& data, storage::ExportFormat format) {
    Result<std::vector<TaskModel>> imported;
    if (format == storage::ExportFormat::kJson) {
        imported = storage::TaskImportExport::ImportFromJson(data);
    } else {
        imported = storage::TaskImportExport::ImportFromCsv(data);
    }
    if (!imported.ok()) return Result<int>::Err(imported.error->code, imported.error->message);

    int count = 0;
    for (auto& t : imported.value.value()) {
        auto add_res = AddTask(std::move(t), true, false);
        if (add_res.ok()) count++;
    }
    return Result<int>::Ok(count);
}

} // namespace openidm
