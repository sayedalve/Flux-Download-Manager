// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — BitTorrent Engine Coordinator
#include "engine_torrent/torrent_engine.hpp"

#include <chrono>
#include <fstream>
#include <thread>
#include <vector>

#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"

namespace openidm::torrent {

Result<void> TorrentEngine::ExecuteTorrentDownload(
    TaskModel& task,
    const TorrentInfo& torrent_info,
    std::atomic<bool>& /*pause_requested*/,
    std::atomic<bool>& /*cancel_requested*/,
    std::atomic<uint64_t>& /*speed_limit_bps*/,
    TorrentProgressCallback /*on_progress*/,
    std::shared_ptr<brisk::TokenBucketLimiter> /*global_limiter*/) {

    uint64_t total_bytes = torrent_info.total_length;
    task.total_bytes = total_bytes;
    task.total_segments = static_cast<uint32_t>(torrent_info.num_pieces());
    task.downloaded_bytes = 0;
    task.completed_segments = 0;
    task.speed_bps = 0;
    task.state = TaskState::kFailed;
    task.error_code = "ERR_TORRENT_UNSUPPORTED";
    task.error_message = "BitTorrent downloading is not supported in this version.";
    task.status_detail = "Failed: BitTorrent downloads are unsupported";

    OPENIDM_LOG_WARN("TorrentEngine", "Attempted to execute BitTorrent download for task " + task.task_id +
                                      ": BitTorrent protocol engine is unsupported in this build.");

    return Result<void>::Err("ERR_TORRENT_UNSUPPORTED", "BitTorrent downloading is not supported in this version.");
}

} // namespace openidm::torrent
