// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM — Unified Download Controller, Queue Scheduler & Settings Service
// Combines ordinary HTTP/HTTPS downloads (Brisk engine) and HLS/DASH streams (N_m3u8DL-RE worker)
// into a single persistent queue and unified task model.
// Implements Phase 3, Phase 5, Phase 7, and Phase 8 requirements.
#pragma once

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "archive/zip_preview.hpp"
#include "brisk_engine/http_download_engine.hpp"
#include "common/types.hpp"
#include "crawler/site_grabber.hpp"
#include "engine_ftp/ftp_client.hpp"
#include "engine_stream/nm3u8dl_re_adapter.hpp"
#include "engine_stream/rtsp_mms_adapter.hpp"
#include "engine_torrent/torrent_engine.hpp"
#include "engine_torrent/torrent_parser.hpp"
#include "security/antivirus_scanner.hpp"
#include "storage/import_export.hpp"
#include "storage/sqlite_store.hpp"

namespace openidm {

struct ControllerConfig {
    std::filesystem::path state_db_path;
    std::filesystem::path default_download_dir;
    std::filesystem::path temp_root_dir;
    std::filesystem::path nm3u8dl_binary_path;
    std::filesystem::path ffmpeg_binary_path;
    uint32_t max_concurrent_tasks{3};
    uint64_t global_speed_limit_bps{0};
    bool allow_loopback{false};
    int stall_timeout_ms{3000};
    uint64_t min_split_bytes{131072};
};

class DownloadController {
public:
    explicit DownloadController(ControllerConfig cfg);
    ~DownloadController();

    Result<void> Initialize();
    void Shutdown();

    // Unified task creation (HTTP_FILE, STREAM_HLS, STREAM_DASH)
    // Automatically deduplicates active/queued tasks for the same URL unless force_duplicate=true.
    Result<TaskModel> AddTask(TaskModel task, bool start_immediately = true, bool force_duplicate = false);

    // Ingest an already completed/transferred file (e.g. from browser-generated Blob/Data transfer)
    // Moves staged file to final destination, runs AV scan, stores task in DB, and notifies UI.
    Result<TaskModel> IngestCompletedTask(TaskModel task, const std::filesystem::path& staged_file_path);

    // Batch add links (from browser text selection FR-010)
    Result<std::vector<TaskModel>> AddBatchTasks(
        const std::vector<std::string>& urls,
        const std::string& page_url,
        const std::map<std::string, std::string>& headers,
        const std::map<std::string, std::string>& auth_headers);

    // Probe a streaming manifest (HLS/DASH) and return available tracks (FR-019)
    Result<stream::StreamProbeResult> ProbeStream(
        const std::string& manifest_url,
        const std::map<std::string, std::string>& headers,
        const std::map<std::string, std::string>& auth_headers,
        const std::string& cached_manifest_body = "");

    // Unambiguous Task Lifecycle Controls (Phase 3):
    // - StartTask: Starts a Paused, Queued, or Scheduled task immediately
    // - PauseTask: Pauses an active or queued transfer while preserving .part / cached segments
    // - ResumeTask: Resumes a Paused transfer from saved byte/segment offsets
    // - RetryTask: Retries a Failed or Canceled transfer (increments retry_count, resets corrupted state if needed)
    // - CancelTask: Stops transfer, deletes .part/temp files, wipes credentials, marks CANCELED (never deletes completed files)
    // - RemoveTask: Removes task from queue & DB; only deletes completed output file on disk if delete_file == true
    // - ClearCompletedHistory: Removes all COMPLETED and CANCELED tasks from list & DB while keeping all files on disk
    Result<void> StartTask(const std::string& task_id);
    Result<void> PauseTask(const std::string& task_id);
    Result<void> ResumeTask(const std::string& task_id);
    Result<void> CancelTask(const std::string& task_id);
    Result<void> RetryTask(const std::string& task_id);
    Result<void> RemoveTask(const std::string& task_id, bool delete_file = false);
    Result<void> UpdateTask(const TaskModel& task);
    Result<int> ClearCompletedHistory();

    void PauseAllTasks();
    void ResumeAllTasks();

    // Refresh expired signed download URL while preserving existing partial bytes (FR-006)
    Result<void> RefreshTaskUrl(
        const std::string& task_id,
        const std::string& new_url,
        const std::map<std::string, std::string>& new_headers = {},
        const std::map<std::string, std::string>& new_auth_headers = {});

    // Queue & bandwidth controls (FR-024, FR-025)
    void SetMaxConcurrentTasks(uint32_t max_tasks);
    uint32_t GetMaxConcurrentTasks() const;
    void SetGlobalSpeedLimit(uint64_t bps);
    uint64_t GetGlobalSpeedLimit() const;

    // Persistent Application Settings (Phase 3 & Phase 10)
    AppSettings GetSettings() const;
    Result<void> UpdateSettings(const AppSettings& settings);
    Result<AppSettings> RestoreDefaultSettings();

    const ControllerConfig& GetConfig() const { return m_cfg; }
    bool AreWorkersAvailable() const;

    // Sleep / wake & network change recovery (Phase 8)
    void OnSystemSuspend();
    void OnSystemResume();

    std::vector<TaskModel> ListTasks() const;
    std::optional<TaskModel> GetTask(const std::string& task_id) const;

    // Multi-Queue Management (Phase 3 & Advanced Scheduler)
    Result<void> UpsertQueue(const QueueConfig& queue);
    std::vector<QueueConfig> ListQueues();
    Result<void> MoveTaskToQueue(const std::string& task_id, const std::string& new_queue_id);

    // Site Grabber & Crawler
    Result<std::vector<crawler::GrabbedItem>> CrawlSite(
        const crawler::SiteGrabberConfig& cfg,
        std::atomic<bool>& cancel_requested,
        crawler::GrabbedItemCallback on_item_found = nullptr);

    // Post-Download Antivirus Scan
    Result<security::ScanReport> ScanTaskFile(const std::string& task_id);

    // ZIP Archive Preview
    Result<archive::ZipArchiveSummary> PreviewZipArchive(const std::filesystem::path& zip_path);

    // Import / Export
    Result<std::string> ExportTasksData(storage::ExportFormat format, bool include_credentials = false);
    Result<int> ImportTasksData(const std::string& data, storage::ExportFormat format);

    // Waits until a specific task reaches a terminal/paused state (useful for CLI & integration tests)
    Result<TaskModel> WaitForTaskCompletion(const std::string& task_id, int timeout_ms = 30000);

private:
    struct ActiveTaskControl {
        std::atomic<bool> pause_requested{false};
        std::atomic<bool> cancel_requested{false};
        std::atomic<uint64_t> task_speed_limit_bps{0};
        std::thread worker_thread;
        std::atomic<bool> finished{false};
    };

    void ScheduleLoop();
    void RunTaskWorker(std::string task_id, std::shared_ptr<ActiveTaskControl> ctrl);
    std::filesystem::path ResolveCategorySaveDir(const std::string& category) const;
    void LoadPersistedSettingsLocked();
    Result<void> SaveSettingsLocked();

    ControllerConfig m_cfg;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_stopping{false};

    SqliteStore m_store;
    std::map<std::string, TaskModel> m_tasks;
    std::set<std::string> m_deleted_tasks;
    std::map<std::string, std::shared_ptr<ActiveTaskControl>> m_active;
    std::atomic<uint32_t> m_max_concurrent{3};
    std::atomic<uint64_t> m_global_speed_limit_bps{0};
    std::shared_ptr<brisk::TokenBucketLimiter> m_global_limiter{std::make_shared<brisk::TokenBucketLimiter>()};
    AppSettings m_settings;
    std::thread m_scheduler_thread;
};

} // namespace openidm
