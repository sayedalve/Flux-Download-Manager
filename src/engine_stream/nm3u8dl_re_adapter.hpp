// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM — N_m3u8DL-RE Stream Download Adapter & Managed Worker Process Supervisor
// Integrates nilaoda/N_m3u8DL-RE (v0.6.0-beta) & FFmpeg via Job Objects / Process Groups
// and ACL-restricted @job.rsp response files.
// Implements FR-019, FR-020, FR-021, FR-022, NFR-015, THREAT-002, THREAT-004.
#pragma once

#include <atomic>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>
#include "common/types.hpp"

namespace openidm::stream {

struct StreamProbeResult {
    std::string manifest_url;
    bool is_live{false};
    bool drm_protected{false};
    std::string drm_system;
    std::vector<StreamTrack> tracks;
};

struct StreamProgress {
    std::string task_id;
    TaskState phase{TaskState::kDownloading}; // kDownloading vs kMuxing reported separately!
    double percent{0.0};
    uint64_t downloaded_bytes{0};
    uint64_t total_bytes{0};
    uint64_t speed_bps{0};
    uint32_t completed_segments{0};
    uint32_t total_segments{0};
    std::string status_line;
};

struct StreamAdapterConfig {
    std::filesystem::path nm3u8dl_binary_path;
    std::filesystem::path ffmpeg_binary_path;
    std::filesystem::path temp_root_dir;
    bool allow_loopback_manifests{false};
};

class Nm3u8dlReAdapter {
public:
    explicit Nm3u8dlReAdapter(StreamAdapterConfig cfg);

    // Verifies that the configured N_m3u8DL-RE worker binary exists and responds to --version
    Result<std::string> VerifyWorkerBinary() const;

    // Validates manifest URL or cached manifest body against SSRF / local file inclusion (THREAT-004)
    Result<void> ValidateManifestSafety(std::string_view url, std::string_view body) const;

    // Writes ACL-restricted @job.rsp response file (0600 permissions) so Cookie/Authorization
    // never appear in process command-line arguments (FR-021, THREAT-002).
    static Result<std::filesystem::path> WriteRestrictedResponseFile(
        const std::filesystem::path& task_tmp_dir,
        const std::string& filename,
        const std::vector<std::string>& lines);

    // Parses N_m3u8DL-RE meta.json output and detects unsupported DRM (NFR-015)
    static Result<StreamProbeResult> ParseMetaJson(const std::filesystem::path& meta_json_path);

    // Phase 1: Probes a detected HLS/DASH manifest using the actual N_m3u8DL-RE worker (--skip-download --write-meta-json)
    Result<StreamProbeResult> ProbeManifest(
        const std::string& manifest_url,
        const std::map<std::string, std::string>& headers,
        const std::map<std::string, std::string>& auth_headers,
        const std::string& cached_manifest_body = "");

    // Phase 2: Executes stream downloading and FFmpeg muxing via N_m3u8DL-RE worker process
    using ProgressCallback = std::function<void(const StreamProgress&)>;
    Result<std::filesystem::path> ExecuteStreamDownload(
        TaskModel& task,
        std::atomic<bool>& pause_requested,
        std::atomic<bool>& cancel_requested,
        uint64_t speed_limit_bps,
        ProgressCallback on_progress);

private:
    StreamAdapterConfig m_cfg;
};

} // namespace openidm::stream
