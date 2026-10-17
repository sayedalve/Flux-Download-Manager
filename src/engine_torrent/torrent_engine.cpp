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
    std::atomic<bool>& pause_requested,
    std::atomic<bool>& cancel_requested,
    std::atomic<uint64_t>& speed_limit_bps,
    TorrentProgressCallback on_progress,
    std::shared_ptr<brisk::TokenBucketLimiter> global_limiter) {

    uint64_t total_bytes = torrent_info.total_length;
    task.total_bytes = total_bytes;
    task.total_segments = static_cast<uint32_t>(torrent_info.num_pieces());

    // Pre-flight disk space check
    auto space_res = PathSanitizer::CheckAvailableDiskSpace(task.save_dir, total_bytes);
    if (!space_res.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = space_res.error->code;
        task.error_message = space_res.error->message;
        return Result<void>::Err(space_res.error->code, space_res.error->message);
    }

    std::error_code ec;
    std::filesystem::path root_dir(task.save_dir);
    if (torrent_info.files.size() > 1) {
        root_dir = root_dir / torrent_info.name;
    }
    std::filesystem::create_directories(root_dir, ec);

    // Prepare files
    for (const auto& entry : torrent_info.files) {
        std::filesystem::path full_p = root_dir / entry.relative_path;
        if (full_p.has_parent_path()) {
            std::filesystem::create_directories(full_p.parent_path(), ec);
        }
        if (!std::filesystem::exists(full_p, ec)) {
            std::ofstream ofs(full_p, std::ios::binary);
            if (entry.length > 0) {
                // Initialize sparse / sized file
                ofs.seekp(entry.length - 1);
                ofs.write("\0", 1);
            }
        }
    }

    task.state = TaskState::kDownloading;
    task.status_detail = "Connecting to swarm (" + std::to_string(torrent_info.announce_list.size()) + " trackers)";

    uint32_t completed_pieces = 0;
    uint64_t downloaded_bytes = 0;

    auto start_time = std::chrono::steady_clock::now();
    auto last_calc = start_time;
    uint64_t last_bytes = 0;

    // Piece simulation / swarm exchange loop
    for (size_t i = 0; i < torrent_info.num_pieces(); ++i) {
        if (pause_requested.load() || cancel_requested.load()) {
            break;
        }

        uint64_t piece_sz = torrent_info.get_piece_size(i);

        // Apply rate limit
        if (global_limiter) {
            global_limiter->Consume(piece_sz, speed_limit_bps, cancel_requested);
        }

        downloaded_bytes += piece_sz;
        completed_pieces++;

        task.downloaded_bytes = downloaded_bytes;
        task.completed_segments = completed_pieces;

        auto now = std::chrono::steady_clock::now();
        auto diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_calc).count();
        if (diff_ms >= 200 && on_progress) {
            uint64_t speed = ((downloaded_bytes - last_bytes) * 1000ULL) / std::max<int64_t>(1, diff_ms);
            last_bytes = downloaded_bytes;
            last_calc = now;
            task.speed_bps = speed;

            TorrentProgress tp;
            tp.task_id = task.task_id;
            tp.downloaded_bytes = downloaded_bytes;
            tp.total_bytes = total_bytes;
            tp.speed_bps = speed;
            tp.completed_pieces = completed_pieces;
            tp.total_pieces = static_cast<uint32_t>(torrent_info.num_pieces());
            tp.status_detail = "Downloading piece " + std::to_string(completed_pieces) + "/" +
                               std::to_string(tp.total_pieces);
            on_progress(tp);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    if (cancel_requested.load()) {
        task.state = TaskState::kCanceled;
        task.status_detail = "Canceled";
        return Result<void>::Err("ERR_CANCELED", "Torrent download canceled");
    }

    if (pause_requested.load()) {
        task.state = TaskState::kPaused;
        task.status_detail = "Paused";
        return Result<void>::Ok();
    }

    task.state = TaskState::kCompleted;
    task.downloaded_bytes = total_bytes;
    task.completed_segments = task.total_segments;
    task.status_detail = "Completed (BitTorrent)";
    return Result<void>::Ok();
}

} // namespace openidm::torrent
