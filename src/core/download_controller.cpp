// SPDX-License-Identifier: GPL-3.0-or-later
#include "core/download_controller.hpp"
#include "brisk_engine/http_client.hpp"

#include "common/json_mini.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"

#include <algorithm>
#include <chrono>

namespace openidm {

DownloadController::DownloadController(ControllerConfig cfg)
    : m_cfg(std::move(cfg)),
      m_max_concurrent(m_cfg.max_concurrent_tasks),
      m_global_speed_limit_bps(m_cfg.global_speed_limit_bps) {
    m_settings.max_concurrent_tasks = m_cfg.max_concurrent_tasks;
    m_settings.speed_limit_bps = m_cfg.global_speed_limit_bps;
    m_settings.default_download_dir = m_cfg.default_download_dir.string();
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
        m_settings.browser_capture_enabled = val["browserCaptureEnabled"].as_bool(true);
        m_settings.close_to_tray = val["closeToTray"].as_bool(true);
        m_settings.start_minimized = val["startMinimized"].as_bool(false);
        m_settings.reduced_motion = val["reducedMotion"].as_bool(false);

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
    obj["browserCaptureEnabled"] = m_settings.browser_capture_enabled;
    obj["closeToTray"] = m_settings.close_to_tray;
    obj["startMinimized"] = m_settings.start_minimized;
    obj["reducedMotion"] = m_settings.reduced_motion;
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
    if (category.empty() || category == "General") {
        return m_cfg.default_download_dir;
    }
    return m_cfg.default_download_dir / category;
}

Result<TaskModel> DownloadController::AddTask(TaskModel task, bool start_immediately, bool force_duplicate) {
    if (task.url.empty()) {
        return Result<TaskModel>::Err("ERR_INVALID_URL", "Task URL cannot be empty");
    }
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
    if (task.created_at_ms <= 0) {
        task.created_at_ms = NowEpochMs();
    }
    task.updated_at_ms = task.created_at_ms;
    task.state = (start_immediately && !task.scheduled) ? TaskState::kQueued : TaskState::kPaused;
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

        uint32_t max_conc = m_max_concurrent.load();
        while (m_active.size() < max_conc) {
            std::string next_id;
            int64_t oldest_ts = INT64_MAX;
            for (const auto& [id, t] : m_tasks) {
                if (t.state == TaskState::kQueued && m_active.find(id) == m_active.end()) {
                    if (t.created_at_ms < oldest_ts) {
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
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(task_id);
        if (it == m_tasks.end() || m_deleted_tasks.count(task_id)) {
            ctrl->finished.store(true);
            return;
        }
        local_task = it->second;
    }

    auto last_db_sync = std::chrono::steady_clock::now();

    if (local_task.task_type == TaskType::kHttpFile) {
        brisk::EngineConfig ecfg;
        ecfg.stall_timeout_ms = m_cfg.stall_timeout_ms;
        ecfg.min_split_bytes = m_cfg.min_split_bytes;
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
            });
        (void)res;
    } else {
        stream::StreamAdapterConfig scfg{
            m_cfg.nm3u8dl_binary_path,
            m_cfg.ffmpeg_binary_path,
            m_cfg.temp_root_dir,
            m_cfg.allow_loopback
        };
        stream::Nm3u8dlReAdapter adapter(std::move(scfg));
        auto res = adapter.ExecuteStreamDownload(
            local_task,
            ctrl->pause_requested,
            ctrl->cancel_requested,
            ctrl->task_speed_limit_bps.load(),
            [&](const stream::StreamProgress& sp) {
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
            });
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
    }
    return Result<void>::Ok();
}

Result<void> DownloadController::ResumeTask(const std::string& task_id) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_tasks.find(task_id);
    if (it == m_tasks.end()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
    if (it->second.state == TaskState::kPaused || it->second.state == TaskState::kFailed) {
        it->second.scheduled = false;
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
        if (m_settings.theme != "dark" && m_settings.theme != "light" && m_settings.theme != "system") {
            m_settings.theme = "dark";
        }
        if (!m_settings.default_download_dir.empty()) {
            m_cfg.default_download_dir = m_settings.default_download_dir;
            std::error_code ec;
            std::filesystem::create_directories(m_cfg.default_download_dir, ec);
        }
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
    defaults.browser_capture_enabled = true;
    defaults.close_to_tray = true;
    defaults.start_minimized = false;
    defaults.reduced_motion = false;
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
    PauseAllTasks();
}

void DownloadController::OnSystemResume() {
    ResumeAllTasks();
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

} // namespace openidm
