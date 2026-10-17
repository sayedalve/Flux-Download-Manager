// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — BitTorrent Engine Coordinator
#pragma once

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include "brisk_engine/http_download_engine.hpp"
#include "common/types.hpp"
#include "engine_torrent/torrent_parser.hpp"

namespace openidm::torrent {

struct TorrentProgress {
    std::string task_id;
    TaskState state{TaskState::kDownloading};
    uint64_t downloaded_bytes{0};
    uint64_t total_bytes{0};
    uint64_t speed_bps{0};
    uint32_t completed_pieces{0};
    uint32_t total_pieces{0};
    std::string status_detail;
};

using TorrentProgressCallback = std::function<void(const TorrentProgress&)>;

class TorrentEngine {
public:
    TorrentEngine() = default;
    ~TorrentEngine() = default;

    static Result<void> ExecuteTorrentDownload(
        TaskModel& task,
        const TorrentInfo& torrent_info,
        std::atomic<bool>& pause_requested,
        std::atomic<bool>& cancel_requested,
        std::atomic<uint64_t>& speed_limit_bps,
        TorrentProgressCallback on_progress,
        std::shared_ptr<brisk::TokenBucketLimiter> global_limiter = nullptr);
};

} // namespace openidm::torrent
