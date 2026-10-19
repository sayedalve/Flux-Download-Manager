// SPDX-License-Identifier: GPL-3.0-or-later
#include "brisk_engine/http_download_engine.hpp"

#include "brisk_engine/http_client.hpp"
#include "common/crypto_utils.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace openidm::brisk {

namespace {

// Cross-platform random-access .openidm.part file wrapper supporting concurrent thread writes at exact byte offsets
class PlatformPartFile {
public:
    PlatformPartFile() = default;
    ~PlatformPartFile() { Close(); }

    bool Open(const std::filesystem::path& path) {
#ifdef _WIN32
        m_handle = CreateFileW(
            path.wstring().c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);
        return m_handle != INVALID_HANDLE_VALUE;
#else
        m_fd = ::open(path.c_str(), O_CREAT | O_RDWR, 0644);
        return m_fd >= 0;
#endif
    }

    bool Truncate(uint64_t size) {
#ifdef _WIN32
        if (m_handle == INVALID_HANDLE_VALUE) return false;
        LARGE_INTEGER li{};
        li.QuadPart = static_cast<LONGLONG>(size);
        if (!SetFilePointerEx(m_handle, li, nullptr, FILE_BEGIN)) return false;
        return SetEndOfFile(m_handle) != 0;
#else
        if (m_fd < 0) return false;
        return ::ftruncate(m_fd, static_cast<off_t>(size)) == 0;
#endif
    }

    bool PWrite(const uint8_t* data, size_t len, uint64_t offset) {
#ifdef _WIN32
        if (m_handle == INVALID_HANDLE_VALUE) return false;
        OVERLAPPED ov{};
        ov.Offset = static_cast<DWORD>(offset & 0xFFFFFFFFULL);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFFULL);
        DWORD written = 0;
        return WriteFile(m_handle, data, static_cast<DWORD>(len), &written, &ov) && written == static_cast<DWORD>(len);
#else
        if (m_fd < 0) return false;
        ssize_t w = ::pwrite(m_fd, data, len, static_cast<off_t>(offset));
        return w == static_cast<ssize_t>(len);
#endif
    }

    void SyncAndClose() {
#ifdef _WIN32
        if (m_handle != INVALID_HANDLE_VALUE) {
            FlushFileBuffers(m_handle);
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
        }
#else
        if (m_fd >= 0) {
            ::fsync(m_fd);
            ::close(m_fd);
            m_fd = -1;
        }
#endif
    }

    void Close() {
#ifdef _WIN32
        if (m_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
        }
#else
        if (m_fd >= 0) {
            ::close(m_fd);
            m_fd = -1;
        }
#endif
    }

private:
#ifdef _WIN32
    HANDLE m_handle{INVALID_HANDLE_VALUE};
#else
    int m_fd{-1};
#endif
};

} // namespace

HttpDownloadEngine::HttpDownloadEngine(EngineConfig cfg) : m_cfg(cfg) {}
HttpDownloadEngine::~HttpDownloadEngine() = default;

Result<std::filesystem::path> HttpDownloadEngine::ExecuteDownload(
    TaskModel& task,
    std::atomic<bool>& pause_requested,
    std::atomic<bool>& cancel_requested,
    std::atomic<uint64_t>& task_speed_limit_bps,
    ProgressCallback on_progress,
    std::shared_ptr<TokenBucketLimiter> global_limiter,
    std::atomic<uint64_t>* global_speed_limit_bps) {

    auto task_limiter = std::make_shared<TokenBucketLimiter>();

    auto consume_both_limiters = [&](size_t bytes, std::atomic<bool>& cancel_flag) {
        if (bytes == 0) return;
        if (task_speed_limit_bps.load(std::memory_order_relaxed) > 0) {
            task_limiter->Consume(bytes, task_speed_limit_bps, cancel_flag);
        }
        if (global_limiter && global_speed_limit_bps && global_speed_limit_bps->load(std::memory_order_relaxed) > 0) {
            global_limiter->Consume(bytes, *global_speed_limit_bps, cancel_flag);
        }
    };

    std::map<std::string, std::string> req_headers = task.headers;
    for (const auto& [k, v] : task.auth_headers) {
        req_headers[k] = v;
    }

    // -------------------------------------------------------------------------
    // Phase 1: Initial Probe (FR-001)
    // -------------------------------------------------------------------------
    task.state = TaskState::kProbing;
    task.status_detail = "Probing server & headers...";
    HttpResponseMeta probe_meta;
    std::atomic<bool> probe_cancel{false};

    auto probe_res = HttpClient::PerformGet(
        task.url,
        req_headers,
        0,
        0,
        m_cfg.stall_timeout_ms,
        m_cfg.allow_loopback_redirect,
        probe_cancel,
        [&](const HttpResponseMeta& meta) -> HeaderAction {
            probe_meta = meta;
            return HeaderAction::kStopSuccess;
        },
        nullptr,
        m_cfg.net_opts);

    // FDM-017: If server returned 416 Range Not Satisfiable during range probe (common for 0-byte files), retry un-ranged GET
    if (probe_meta.status_code == 416) {
        probe_cancel.store(false);
        auto unrange_res = HttpClient::PerformGet(
            task.url,
            req_headers,
            std::nullopt,
            std::nullopt,
            m_cfg.stall_timeout_ms,
            m_cfg.allow_loopback_redirect,
            probe_cancel,
            [&](const HttpResponseMeta& meta) -> HeaderAction {
                probe_meta = meta;
                return HeaderAction::kStopSuccess;
            },
            nullptr,
            m_cfg.net_opts);
        if (!unrange_res.ok()) {
            probe_res = unrange_res;
        }
    }

    if (!probe_res.ok() || probe_meta.status_code >= 400) {
        Error probe_err = !probe_res.ok() ? *probe_res.error : *HttpClient::MapHttpStatusToError(probe_meta).error;
        task.state = probe_err.user_action_resumable ? TaskState::kPaused : TaskState::kFailed;
        task.error_code = probe_err.code;
        task.error_message = probe_err.message;
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(
            probe_err.code, probe_err.message, probe_err.retryable, probe_err.user_action_resumable);
    }

    // FDM-007: Cross-origin redirect credential scrubbing
    if (!probe_meta.final_url.empty() && probe_meta.final_url != task.url) {
        auto orig_u = HttpClient::ParseUrl(task.url);
        auto final_u = HttpClient::ParseUrl(probe_meta.final_url);
        if (orig_u.ok() && final_u.ok()) {
            bool cross_origin = (orig_u.value->scheme != final_u.value->scheme ||
                                 orig_u.value->host != final_u.value->host ||
                                 orig_u.value->port != final_u.value->port);
            if (cross_origin) {
                OPENIDM_LOG_INFO("HttpEngine", "Cross-origin redirect detected (" + task.url + " -> " + probe_meta.final_url + "); scrubbing credentials and authorization headers");
                static const char* kSensitiveHeaders[] = {
                    "cookie", "cookie2", "authorization", "proxy-authorization",
                    "Cookie", "Cookie2", "Authorization", "Proxy-Authorization"
                };
                for (const char* h : kSensitiveHeaders) {
                    req_headers.erase(h);
                    task.headers.erase(h);
                    task.auth_headers.erase(h);
                }
            }
        }
        task.url = probe_meta.final_url;
    }

    bool range_supported = (probe_meta.status_code == 206 && probe_meta.is_partial_content);
    uint64_t total_bytes = 0;
    if (range_supported && probe_meta.range_total > 0) {
        total_bytes = probe_meta.range_total;
    } else if (probe_meta.has_content_length) {
        total_bytes = probe_meta.content_length;
    }

    // -------------------------------------------------------------------------
    // Resource-Aware Resume Validation (COR-03)
    // -------------------------------------------------------------------------
    bool is_resume = !task.segments.empty() && task.downloaded_bytes > 0;
    if (is_resume) {
        bool validator_mismatch = false;
        std::string mismatch_reason;

        if (!task.etag.empty() && !probe_meta.etag.empty() &&
            probe_meta.etag.rfind("W/", 0) != 0 && task.etag != probe_meta.etag) {
            validator_mismatch = true;
            mismatch_reason = "Remote ETag changed from " + task.etag + " to " + probe_meta.etag;
        } else if (!task.last_modified.empty() && !probe_meta.last_modified.empty() &&
                   task.last_modified != probe_meta.last_modified) {
            validator_mismatch = true;
            mismatch_reason = "Remote Last-Modified changed from " + task.last_modified + " to " + probe_meta.last_modified;
        } else if (task.total_bytes > 0 && total_bytes > 0 && task.total_bytes != total_bytes) {
            validator_mismatch = true;
            mismatch_reason = "Remote file size changed on resume (expected " +
                              std::to_string(task.total_bytes) + ", got " + std::to_string(total_bytes) + ")";
        }

        if (validator_mismatch) {
            OPENIDM_LOG_WARN("HttpEngine", "Validator mismatch detected (" + mismatch_reason + "); discarding stale partial segments and resetting file");
            task.segments.clear();
            task.downloaded_bytes = 0;
            if (!task.part_path.empty()) {
                std::error_code ec_remove;
                std::filesystem::remove(task.part_path, ec_remove);
            }
            task.state = TaskState::kFailed;
            task.error_code = "ERR_HTTP_VALIDATOR_MISMATCH";
            task.error_message = mismatch_reason;
            task.status_detail = task.error_message;
            return Result<std::filesystem::path>::Err(
                "ERR_HTTP_VALIDATOR_MISMATCH", task.error_message, false, false);
        }

        // Apply If-Range validator if available
        if (!task.etag.empty() && probe_meta.etag.rfind("W/", 0) != 0) {
            req_headers["If-Range"] = task.etag;
        } else if (!task.last_modified.empty()) {
            req_headers["If-Range"] = task.last_modified;
        }
    }

    task.range_supported = range_supported;
    task.total_bytes = total_bytes;
    if (!probe_meta.etag.empty()) task.etag = probe_meta.etag;
    if (!probe_meta.last_modified.empty()) task.last_modified = probe_meta.last_modified;

    if (!probe_meta.content_disposition.empty()) {
        std::string cd_name = PathSanitizer::ExtractFromContentDisposition(probe_meta.content_disposition);
        if (!cd_name.empty()) {
            task.file_name = cd_name;
        }
    }
    if (task.file_name.empty() || task.file_name == "download" || task.file_name == "index.html" || task.file_name == "file") {
        std::string url_name = PathSanitizer::ExtractFromUrl(task.url);
        if (!url_name.empty() && url_name != "download") {
            task.file_name = url_name;
        }
    }
    if (task.file_name.empty()) {
        task.file_name = "download.bin";
    }
    task.file_name = PathSanitizer::SanitizeFileName(task.file_name);
    task.category = InferCategoryFromFilename(task.file_name);

    // Pre-flight disk space check (BUG-009 fix)
    uint64_t remaining_to_download = (total_bytes > task.downloaded_bytes) ? (total_bytes - task.downloaded_bytes) : total_bytes;
    auto space_res = PathSanitizer::CheckAvailableDiskSpace(task.save_dir, remaining_to_download);
    if (!space_res.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = space_res.error->code;
        task.error_message = space_res.error->message;
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(
            space_res.error->code, space_res.error->message, false, true);
    }

    std::error_code ec;
    std::filesystem::create_directories(task.save_dir, ec);

    // -------------------------------------------------------------------------
    // Collision-Resistant Part File Path (COR-01)
    // -------------------------------------------------------------------------
    std::filesystem::path part_path;
    if (!task.part_path.empty() && std::filesystem::exists(task.part_path, ec) &&
        task.part_path.find(task.task_id) != std::string::npos) {
        part_path = task.part_path;
    } else {
        part_path = std::filesystem::path(task.save_dir) / (task.file_name + "." + task.task_id + ".part");
        task.part_path = part_path.string();
    }

    // -------------------------------------------------------------------------
    // Validate Partial File on Resume (COR-02)
    // -------------------------------------------------------------------------
    if (is_resume) {
        if (!std::filesystem::exists(part_path, ec)) {
            OPENIDM_LOG_WARN("HttpEngine", "Part file missing on resume; resetting segments to restart from byte 0");
            is_resume = false;
            task.segments.clear();
            task.downloaded_bytes = 0;
        } else {
            uint64_t actual_part_sz = std::filesystem::file_size(part_path, ec);
            bool segments_consistent = SegmentScheduler::ValidateSegmentPartition(task.segments, total_bytes);
            uint64_t verified_dl_bytes = 0;
            if (segments_consistent) {
                for (const auto& s : task.segments) {
                    if (s.current_offset > s.start_offset && s.current_offset > actual_part_sz) {
                        segments_consistent = false;
                        break;
                    }
                    verified_dl_bytes += (s.current_offset - s.start_offset);
                }
            }
            if (!segments_consistent) {
                OPENIDM_LOG_WARN("HttpEngine", "Segment metadata invalid or inconsistent with partial file on disk; resetting segments");
                is_resume = false;
                task.segments.clear();
                task.downloaded_bytes = 0;
            } else {
                task.downloaded_bytes = verified_dl_bytes;
            }
        }
    }

    PlatformPartFile part_file;
    if (!part_file.Open(part_path)) {
        task.state = TaskState::kFailed;
        task.error_code = "ERR_STORAGE_OPEN";
        task.error_message = "Failed to open part file: " + part_path.string();
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err("ERR_STORAGE_OPEN", task.error_message);
    }

    if (total_bytes > 0) {
        if (!is_resume) {
            if (!part_file.Truncate(total_bytes)) {
                part_file.Close();
                task.state = TaskState::kFailed;
                task.error_code = "ERR_DISK_FULL";
                task.error_message = "Failed to pre-allocate disk space";
                task.status_detail = task.error_message;
                return Result<std::filesystem::path>::Err("ERR_DISK_FULL", task.error_message, false, true);
            }
        } else {
            uint64_t actual_part_sz = std::filesystem::file_size(part_path, ec);
            if (actual_part_sz != total_bytes) {
                // XDM tempFileIntact rule: partial file on resume must match expected total_bytes exactly.
                // If it was enlarged by disk corruption or shorter than expected, normalize file size.
                part_file.Truncate(total_bytes);
            }
        }
    }

    task.state = TaskState::kDownloading;

    // -------------------------------------------------------------------------
    // Case A: Non-Range Server Single-Stream Fallback (FR-002 & BUG-008 Fix)
    // -------------------------------------------------------------------------
    if (!range_supported || total_bytes == 0) {
        // Clear any stale segments if server switched from Range to Non-Range on resume
        task.segments.clear();
        task.downloaded_bytes = 0;
        task.status_detail = "Downloading (Single Stream)...";
        part_file.Truncate(0);
        uint64_t written_offset = 0;
        auto start_tp = std::chrono::steady_clock::now();
        auto last_progress_tp = start_tp;
        uint64_t last_progress_bytes = 0;

        std::atomic<bool> stop_flag{false};
        auto single_res = HttpClient::PerformGet(
            task.url,
            req_headers,
            std::nullopt,
            std::nullopt,
            m_cfg.stall_timeout_ms,
            m_cfg.allow_loopback_redirect,
            stop_flag,
            [&](const HttpResponseMeta&) -> HeaderAction { return HeaderAction::kContinue; },
            [&](const uint8_t* data, size_t len) -> bool {
                if (pause_requested.load() || cancel_requested.load()) {
                    stop_flag.store(true);
                    return false;
                }
                consume_both_limiters(len, stop_flag);
                if (!part_file.PWrite(data, len, written_offset)) {
                    stop_flag.store(true);
                    return false;
                }
                written_offset += len;
                task.downloaded_bytes = written_offset;

                auto now = std::chrono::steady_clock::now();
                auto diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_progress_tp).count();
                if (diff_ms >= 50 && on_progress) {
                    uint64_t interval_bytes = written_offset - last_progress_bytes;
                    uint64_t speed = (interval_bytes * 1000ULL) / std::max<int64_t>(1, diff_ms);
                    last_progress_bytes = written_offset;
                    last_progress_tp = now;

                    EngineProgress ep;
                    ep.task_id = task.task_id;
                    ep.state = TaskState::kDownloading;
                    ep.downloaded_bytes = written_offset;
                    ep.total_bytes = total_bytes;
                    ep.speed_bps = speed;
                    ep.range_supported = false;
                    ep.resolved_filename = task.file_name;
                    on_progress(ep);
                }
                return true;
            },
            m_cfg.net_opts);

        part_file.SyncAndClose();

        if (cancel_requested.load()) {
            std::filesystem::remove(part_path, ec);
            task.state = TaskState::kCanceled;
            task.status_detail = "Canceled";
            return Result<std::filesystem::path>::Err("ERR_CANCELED", "Task canceled by user");
        }
        if (pause_requested.load()) {
            task.state = TaskState::kPaused;
            task.status_detail = "Paused";
            return Result<std::filesystem::path>::Err("ERR_PAUSED", "Task paused by user");
        }
        if (!single_res.ok()) {
            task.state = TaskState::kFailed;
            task.error_code = single_res.error->code;
            task.error_message = single_res.error->message;
            task.status_detail = task.error_message;
            return Result<std::filesystem::path>::Err(single_res.error->code, single_res.error->message);
        }
        task.total_bytes = written_offset;
    } else {
        // ---------------------------------------------------------------------
        // Case B: Brisk Multi-Segment Dynamic Spawn & Work-Stealing Engine (FR-003, FR-004)
        // ---------------------------------------------------------------------
        uint32_t max_conns = std::clamp(task.max_connections, 1u, 32u);
        SegmentScheduler scheduler(total_bytes, max_conns, m_cfg.min_split_bytes);
        scheduler.Initialize(is_resume ? task.segments : std::vector<Segment>{});

        std::atomic<bool> fatal_abort{false};
        std::atomic<bool> disable_splitting{false};
        std::atomic<uint32_t> running_workers{0};
        std::mutex err_mu;
        std::optional<Error> fatal_error;

        auto worker_fn = [&](uint32_t /*worker_idx*/) {
            running_workers.fetch_add(1, std::memory_order_relaxed);
            while (!fatal_abort.load() && !pause_requested.load() && !cancel_requested.load()) {
                auto seg = scheduler.AcquireNextOrStealSegment(!disable_splitting.load());
                if (!seg) {
                    if (scheduler.AllCompleted()) {
                        break;
                    }
                    // Back off briefly in case a running segment becomes eligible for work-stealing soon
                    bool acquired = false;
                    for (int idle_retry = 0; idle_retry < 6; ++idle_retry) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        if (fatal_abort.load() || pause_requested.load() || cancel_requested.load() || scheduler.AllCompleted()) {
                            break;
                        }
                        if (scheduler.CanSplitAnySegment()) {
                            seg = scheduler.AcquireNextOrStealSegment(!disable_splitting.load());
                            if (seg) {
                                acquired = true;
                                break;
                            }
                        }
                    }
                    if (!acquired && !seg) {
                        break;
                    }
                }

                int retries = 0;
                uint64_t last_progress_offset = seg->current_offset.load();
                while (!fatal_abort.load() && !pause_requested.load() && !cancel_requested.load()) {
                    uint64_t curr = seg->current_offset.load();
                    uint64_t end = seg->end_offset.load();
                    if (curr > end) {
                        seg->completed.store(true);
                        seg->active_worker.store(false);
                        break;
                    }

                    std::atomic<bool> req_cancel{false};
                    bool server_ignored_range = false;

                    auto res = HttpClient::PerformGet(
                        task.url,
                        req_headers,
                        curr,
                        end,
                        m_cfg.stall_timeout_ms,
                        m_cfg.allow_loopback_redirect,
                        req_cancel,
                        [&](const HttpResponseMeta& meta) -> HeaderAction {
                            if (meta.status_code == 416) {
                                if (curr == 0 && (total_bytes == 0 || meta.range_total == 0)) {
                                    seg->completed.store(true);
                                    return HeaderAction::kStopSuccess;
                                }
                                std::lock_guard<std::mutex> lk(err_mu);
                                fatal_error = Error{"ERR_HTTP_RANGE_NOT_SATISFIABLE", "HTTP 416 Range Not Satisfiable", false, true};
                                fatal_abort.store(true);
                                return HeaderAction::kAbort;
                            }
                            // Range satisfaction validation (aria2 HttpRequest::isRangeSatisfied)
                            if (meta.status_code != 206 || !meta.is_partial_content || meta.range_start != curr) {
                                server_ignored_range = true;
                                disable_splitting.store(true);
                                OPENIDM_LOG_WARN("BriskEngine", "Server ignored Range header on worker; aborting secondary range stream");
                                return HeaderAction::kAbort;
                            }
                            if (total_bytes > 0 && meta.range_total > 0 && meta.range_total != total_bytes) {
                                std::lock_guard<std::mutex> lk(err_mu);
                                fatal_error = Error{"ERR_HTTP_VALIDATOR_MISMATCH",
                                    "Remote entity length changed mid-download (expected " +
                                    std::to_string(total_bytes) + ", got " + std::to_string(meta.range_total) + ")", false, false};
                                fatal_abort.store(true);
                                return HeaderAction::kAbort;
                            }
                            return HeaderAction::kContinue;
                        },
                        [&](const uint8_t* data, size_t len) -> bool {
                            if (fatal_abort.load() || pause_requested.load() || cancel_requested.load()) {
                                req_cancel.store(true);
                                return false;
                            }

                            consume_both_limiters(len, req_cancel);

                            std::lock_guard<std::mutex> write_lk(seg->write_mu);
                            uint64_t write_pos = seg->current_offset.load();
                            uint64_t current_end = seg->end_offset.load();
                            if (write_pos > current_end) {
                                seg->completed.store(true);
                                req_cancel.store(true);
                                return false;
                            }

                            uint64_t max_writable = (current_end >= write_pos) ? (current_end - write_pos + 1) : 0;
                            if (max_writable == 0) {
                                seg->completed.store(true);
                                req_cancel.store(true);
                                return false;
                            }
                            size_t to_write = static_cast<size_t>(std::min<uint64_t>(static_cast<uint64_t>(len), max_writable));

                            if (!part_file.PWrite(data, to_write, write_pos)) {
                                std::lock_guard<std::mutex> lk(err_mu);
                                fatal_error = Error{"ERR_DISK_FULL", "Disk write failed on segment", false, true};
                                fatal_abort.store(true);
                                req_cancel.store(true);
                                return false;
                            }

                            seg->current_offset.fetch_add(to_write);
                            if (to_write < len || seg->current_offset.load() > seg->end_offset.load()) {
                                if (seg->current_offset.load() > seg->end_offset.load()) {
                                    seg->completed.store(true);
                                }
                                req_cancel.store(true);
                                return false;
                            }
                            return true;
                        },
                        m_cfg.net_opts);

                    if (seg->current_offset.load() > seg->end_offset.load()) {
                        seg->completed.store(true);
                        seg->active_worker.store(false);
                        break;
                    }

                    if (pause_requested.load() || cancel_requested.load() || fatal_abort.load()) {
                        seg->active_worker.store(false);
                        break;
                    }

                    if (server_ignored_range) {
                        // BUG-008 Fix: Merge unstarted stolen segment back into predecessor so Worker 0 finishes cleanly
                        if (!scheduler.RollbackUnstartedStolenSegment(seg->segment_id)) {
                            std::lock_guard<std::mutex> lk(err_mu);
                            fatal_error = Error{"ERR_HTTP_RANGE_PROTOCOL", "Server failed range check on active segment", false, false};
                            fatal_abort.store(true);
                        }
                        break;
                    }

                    // Forward progress check (NET-01): reset retries if bytes moved forward
                    uint64_t cur_pos = seg->current_offset.load();
                    if (cur_pos > last_progress_offset) {
                        last_progress_offset = cur_pos;
                        retries = 0;
                    }

                    if (!res.ok()) {
                        if (res.error->code == "ERR_HTTP_429_RATE_LIMITED") {
                            disable_splitting.store(true);
                        }
                        if (!res.error->retryable || retries >= m_cfg.max_retries_per_worker) {
                            seg->active_worker.store(false);
                            std::lock_guard<std::mutex> lk(err_mu);
                            if (!fatal_error.has_value()) {
                                fatal_error = res.error;
                            }
                            fatal_abort.store(true);
                            break;
                        }
                        ++retries;
                        OPENIDM_LOG_INFO("BriskEngine", "Resetting stalled/failed segment connection #" +
                            std::to_string(seg->segment_id) + " at offset " + std::to_string(seg->current_offset.load()) +
                            " (retry " + std::to_string(retries) + ")");
                        int backoff_ms = std::min(3000, (50 * (1 << std::min(retries, 5))) + (std::rand() % 100));
                        std::this_thread::sleep_for(std::chrono::milliseconds(backoff_ms));
                    }
                }
            }
            running_workers.fetch_sub(1, std::memory_order_relaxed);
        };

        auto start_tp = std::chrono::steady_clock::now();
        uint64_t initial_bytes = scheduler.TotalDownloadedBytes();
        auto last_sp_tp = start_tp;
        uint64_t last_sp_bytes = initial_bytes;
        uint64_t current_speed = 0;

        std::vector<std::thread> workers;
        workers.emplace_back(worker_fn, 0);

        uint32_t spawned = 1;
        while (!scheduler.AllCompleted() && !fatal_abort.load() && !pause_requested.load() && !cancel_requested.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));

            if (running_workers.load() < max_conns && spawned < (max_conns * 8) && !disable_splitting.load() &&
                scheduler.CanSplitAnySegment() && (scheduler.TotalDownloadedBytes() > initial_bytes || is_resume)) {
                workers.emplace_back(worker_fn, spawned++);
            }

            uint64_t dl = scheduler.TotalDownloadedBytes();
            task.downloaded_bytes = dl;
            task.segments = scheduler.GetSnapshots();
            task.status_detail = "Downloading (" + std::to_string(scheduler.ActiveWorkerCount()) + " active connections)";

            auto now = std::chrono::steady_clock::now();
            auto diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_sp_tp).count();
            if (diff_ms >= 100) {
                uint64_t delta_bytes = (dl >= last_sp_bytes) ? (dl - last_sp_bytes) : 0;
                current_speed = (delta_bytes * 1000ULL) / std::max<int64_t>(1, diff_ms);
                last_sp_bytes = dl;
                last_sp_tp = now;
            }

            if (on_progress) {
                EngineProgress ep;
                ep.task_id = task.task_id;
                ep.state = TaskState::kDownloading;
                ep.downloaded_bytes = dl;
                ep.total_bytes = total_bytes;
                ep.speed_bps = current_speed;
                ep.range_supported = true;
                ep.resolved_filename = task.file_name;
                ep.etag = task.etag;
                ep.last_modified = task.last_modified;
                ep.segments = task.segments;
                on_progress(ep);
            }
        }

        for (auto& t : workers) {
            if (t.joinable()) t.join();
        }

        part_file.SyncAndClose();

        task.downloaded_bytes = scheduler.TotalDownloadedBytes();
        task.segments = scheduler.GetSnapshots();

        if (cancel_requested.load()) {
            std::filesystem::remove(part_path, ec);
            task.state = TaskState::kCanceled;
            task.status_detail = "Canceled";
            return Result<std::filesystem::path>::Err("ERR_CANCELED", "Task canceled by user");
        }
        if (pause_requested.load()) {
            task.state = TaskState::kPaused;
            task.status_detail = "Paused";
            return Result<std::filesystem::path>::Err("ERR_PAUSED", "Task paused by user");
        }
        if (fatal_abort.load() || !scheduler.AllCompleted()) {
            std::lock_guard<std::mutex> lk(err_mu);
            Error err = fatal_error.value_or(Error{"ERR_NET_INCOMPLETE", "Segments did not complete", true, true});
            task.state = err.user_action_resumable ? TaskState::kPaused : TaskState::kFailed;
            task.error_code = err.code;
            task.error_message = err.message;
            task.status_detail = task.error_message;
            return Result<std::filesystem::path>::Err(err.code, err.message, err.retryable, err.user_action_resumable);
        }
    }

    // -------------------------------------------------------------------------
    // Phase 3: File Verification & Atomic Rename (FR-007, FR-026)
    // -------------------------------------------------------------------------
    task.state = TaskState::kVerifying;
    task.status_detail = "Verifying file integrity...";
    if (!task.expected_sha256.empty()) {
        auto sha_res = CryptoUtils::ComputeFileSha256Hex(part_path);
        if (!sha_res.ok()) {
            task.state = TaskState::kFailed;
            task.error_code = sha_res.error->code;
            task.error_message = sha_res.error->message;
            task.status_detail = task.error_message;
            return Result<std::filesystem::path>::Err(sha_res.error->code, sha_res.error->message);
        }
        std::string actual_sha = *sha_res.value;
        std::string expected_lower = task.expected_sha256;
        for (char& c : expected_lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (actual_sha != expected_lower) {
            task.state = TaskState::kFailed;
            task.error_code = "ERR_STORAGE_PART_CORRUPTED";
            task.error_message = "SHA-256 mismatch: expected " + expected_lower + ", got " + actual_sha;
            task.status_detail = task.error_message;
            return Result<std::filesystem::path>::Err("ERR_STORAGE_PART_CORRUPTED", task.error_message, false, true);
        }
    }

    auto commit_res = PathSanitizer::CommitPartFile(part_path, task.save_dir, task.file_name, task.conflict_policy, task.total_bytes);
    if (!commit_res.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = commit_res.error->code;
        task.error_message = commit_res.error->message;
        task.status_detail = task.error_message;
        return commit_res;
    }

    std::filesystem::path final_path = *commit_res.value;
    task.file_name = final_path.filename().string();
    task.part_path.clear();
    task.state = TaskState::kCompleted;
    task.status_detail = "Completed";
    task.completed_at_ms = NowEpochMs();
    task.auth_headers.clear();
    return Result<std::filesystem::path>::Ok(final_path);
}

} // namespace openidm::brisk
