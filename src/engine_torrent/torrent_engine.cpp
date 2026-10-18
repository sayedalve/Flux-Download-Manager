// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — BitTorrent Engine Coordinator
// Direct reuse of aria2 BitTorrent wire framing, piece management, and tracker announce architecture
#include "engine_torrent/torrent_engine.hpp"

#include <chrono>
#include <fstream>
#include <memory>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"
#include "engine_torrent/multi_file_storage.hpp"
#include "engine_torrent/peer_wire_protocol.hpp"
#include "engine_torrent/piece_manager.hpp"
#include "engine_torrent/tracker_client.hpp"

namespace openidm::torrent {

namespace {

#ifdef _WIN32
struct WinSockInit {
    WinSockInit() {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    }
    ~WinSockInit() {
        WSACleanup();
    }
};

void EnsureWinSock() {
    static WinSockInit s_init;
}
#else
void EnsureWinSock() {}
#endif

struct UniqueSocket {
    SOCKET s{INVALID_SOCKET};
    UniqueSocket() = default;
    explicit UniqueSocket(SOCKET sock) : s(sock) {}
    ~UniqueSocket() {
        if (s != INVALID_SOCKET) {
            closesocket(s);
        }
    }
    UniqueSocket(const UniqueSocket&) = delete;
    UniqueSocket& operator=(const UniqueSocket&) = delete;
    UniqueSocket(UniqueSocket&& o) noexcept : s(o.s) { o.s = INVALID_SOCKET; }
    UniqueSocket& operator=(UniqueSocket&& o) noexcept {
        if (s != INVALID_SOCKET) closesocket(s);
        s = o.s;
        o.s = INVALID_SOCKET;
        return *this;
    }
};

} // namespace

Result<void> TorrentEngine::ExecuteTorrentDownload(
    TaskModel& task,
    const TorrentInfo& torrent_info,
    std::atomic<bool>& pause_requested,
    std::atomic<bool>& cancel_requested,
    std::atomic<uint64_t>& speed_limit_bps,
    TorrentProgressCallback on_progress,
    std::shared_ptr<brisk::TokenBucketLimiter> global_limiter) {

    EnsureWinSock();

    // 1. Metainfo Pre-flight Validation
    if (torrent_info.total_length == 0 || torrent_info.num_pieces() == 0 || torrent_info.piece_length == 0) {
        task.state = TaskState::kFailed;
        task.error_code = "ERR_TORRENT_MALFORMED";
        task.error_message = "Invalid torrent metainfo: zero length or zero pieces";
        task.status_detail = "Failed: Invalid metainfo";
        OPENIDM_LOG_ERROR("TorrentEngine", "Malformed metainfo for task " + task.task_id);
        return Result<void>::Err("ERR_TORRENT_MALFORMED", "Invalid torrent metainfo: zero length or zero pieces");
    }

    uint64_t total_bytes = torrent_info.total_length;
    task.total_bytes = total_bytes;
    task.total_segments = static_cast<uint32_t>(torrent_info.num_pieces());
    task.downloaded_bytes = 0;
    task.completed_segments = 0;
    task.speed_bps = 0;
    task.state = TaskState::kDownloading;
    task.status_detail = "Allocating storage files...";

    if (on_progress) {
        TorrentProgress tp;
        tp.task_id = task.task_id;
        tp.state = task.state;
        tp.total_bytes = total_bytes;
        tp.total_pieces = static_cast<uint32_t>(torrent_info.num_pieces());
        tp.status_detail = task.status_detail;
        on_progress(tp);
    }

    // 2. Storage Setup (aria2 MultiDiskAdaptor)
    auto storage = std::make_shared<MultiFileStorage>();
    auto stor_res = storage->Initialize(task.save_dir, torrent_info);
    if (!stor_res.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = stor_res.error->code;
        task.error_message = stor_res.error->message;
        task.status_detail = "Failed: " + stor_res.error->message;
        return stor_res;
    }
    storage->AllocateFiles();

    // 3. Piece & Block Scheduler (aria2 PieceStorage)
    auto piece_mgr = std::make_shared<PieceManager>(torrent_info, storage);
    std::string client_peer_id = PeerWireProtocol::GeneratePeerId();

    OPENIDM_LOG_INFO("TorrentEngine", "Starting BitTorrent download for task " + task.task_id +
                                      " (hash: " + torrent_info.info_hash_hex +
                                      ", pieces: " + std::to_string(torrent_info.num_pieces()) + ")");

    // 4. Tracker Announcement (aria2 DefaultBtAnnounce)
    task.status_detail = "Connecting to tracker...";
    if (on_progress) {
        TorrentProgress tp;
        tp.task_id = task.task_id;
        tp.state = task.state;
        tp.total_bytes = total_bytes;
        tp.total_pieces = static_cast<uint32_t>(torrent_info.num_pieces());
        tp.status_detail = task.status_detail;
        on_progress(tp);
    }

    std::vector<std::string> announce_urls = torrent_info.announce_list;
    if (announce_urls.empty() && !torrent_info.announce.empty()) {
        announce_urls.push_back(torrent_info.announce);
    }

    TrackerAnnounceResponse tracker_resp;
    if (!announce_urls.empty()) {
        auto tr_res = TrackerClient::AnnounceAny(
            announce_urls,
            torrent_info.info_hash_raw,
            client_peer_id,
            6881,
            0,
            0,
            total_bytes,
            "started",
            8000);

        if (tr_res.ok()) {
            tracker_resp = tr_res.value.value();
        } else {
            OPENIDM_LOG_WARN("TorrentEngine", "All announce trackers failed for task " + task.task_id + ": " + tr_res.error->message);
        }
    }

    if (tracker_resp.peers.empty()) {
        task.state = TaskState::kFailed;
        task.error_code = "ERR_TORRENT_NO_PEERS";
        task.error_message = "No BitTorrent peers available from announce trackers";
        task.status_detail = "Failed: No peers available";
        return Result<void>::Err("ERR_TORRENT_NO_PEERS", "No BitTorrent peers available from announce trackers");
    }

    task.status_detail = "Connected to tracker; probing " + std::to_string(tracker_resp.peers.size()) + " peers...";
    if (on_progress) {
        TorrentProgress tp;
        tp.task_id = task.task_id;
        tp.state = task.state;
        tp.total_bytes = total_bytes;
        tp.total_pieces = static_cast<uint32_t>(torrent_info.num_pieces());
        tp.status_detail = task.status_detail;
        on_progress(tp);
    }

    auto start_time = std::chrono::steady_clock::now();

    // 5. Peer Wire Transfer Loop (aria2 PeerInteractionCommand / DefaultBtInteractive)
    for (const auto& peer : tracker_resp.peers) {
        if (cancel_requested.load() || pause_requested.load() || piece_mgr->IsAllCompleted()) {
            break;
        }

        OPENIDM_LOG_INFO("TorrentEngine", "Attempting connection to peer " + peer.ip + ":" + std::to_string(peer.port));
        SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s == INVALID_SOCKET) continue;
        UniqueSocket sock(s);

        // Set socket timeouts (4 seconds)
#ifdef _WIN32
        DWORD to_ms = 4000;
        setsockopt(sock.s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&to_ms), sizeof(to_ms));
        setsockopt(sock.s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&to_ms), sizeof(to_ms));
#else
        struct timeval tv;
        tv.tv_sec = 4;
        tv.tv_usec = 0;
        setsockopt(sock.s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        setsockopt(sock.s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif

        sockaddr_in saddr{};
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(peer.port);
        if (inet_pton(AF_INET, peer.ip.c_str(), &saddr.sin_addr) <= 0) {
            continue;
        }

        if (connect(sock.s, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) == SOCKET_ERROR) {
            continue;
        }

        // Exchange Handshake (BEP 3)
        auto hs = PeerWireProtocol::CreateHandshake(torrent_info.info_hash_raw, client_peer_id);
        if (send(sock.s, reinterpret_cast<const char*>(hs.data()), static_cast<int>(hs.size()), 0) <= 0) {
            continue;
        }

        uint8_t peer_hs[68];
        int hs_read = 0;
        while (hs_read < 68) {
            int r = recv(sock.s, reinterpret_cast<char*>(peer_hs + hs_read), 68 - hs_read, 0);
            if (r <= 0) break;
            hs_read += r;
        }

        std::string remote_peer_id;
        if (!PeerWireProtocol::VerifyHandshake(peer_hs, hs_read, torrent_info.info_hash_raw, &remote_peer_id)) {
            continue;
        }

        OPENIDM_LOG_INFO("TorrentEngine", "Handshake established with peer " + peer.ip + " (" + remote_peer_id + ")");

        // Send Interested message
        auto interested = PeerWireProtocol::CreateInterested();
        send(sock.s, reinterpret_cast<const char*>(interested.data()), static_cast<int>(interested.size()), 0);

        std::vector<bool> peer_have(torrent_info.num_pieces(), false);
        bool peer_choked_us = true;
        std::vector<uint8_t> recv_buf;
        recv_buf.reserve(65536);

        while (!cancel_requested.load() && !pause_requested.load() && !piece_mgr->IsAllCompleted()) {
            // Apply speed limit throttling if enabled
            uint64_t limit = speed_limit_bps.load();
            if (limit > 0 || global_limiter) {
                // Rate limit token bucket application
                if (global_limiter) global_limiter->Consume(kBlockLength, speed_limit_bps, cancel_requested);
            }

            // Receive messages
            uint8_t temp[16384];
            int r = recv(sock.s, reinterpret_cast<char*>(temp), sizeof(temp), 0);
            if (r <= 0) break;
            recv_buf.insert(recv_buf.end(), temp, temp + r);

            // Process all framed messages in recv_buf
            while (recv_buf.size() >= 4) {
                uint32_t payload_len = PeerWireProtocol::ReadUint32BE(recv_buf.data());
                if (payload_len == 0) {
                    // Keep-alive message (4 zero bytes)
                    recv_buf.erase(recv_buf.begin(), recv_buf.begin() + 4);
                    continue;
                }
                if (recv_buf.size() < 4 + payload_len) {
                    break; // Incomplete message in buffer
                }

                auto msg_opt = PeerWireProtocol::ParseMessage(recv_buf.data(), 4 + payload_len);
                recv_buf.erase(recv_buf.begin(), recv_buf.begin() + 4 + payload_len);

                if (!msg_opt) continue;
                const auto& msg = *msg_opt;

                if (msg.id == BtMessageId::kChoke) {
                    peer_choked_us = true;
                } else if (msg.id == BtMessageId::kUnchoke) {
                    peer_choked_us = false;
                } else if (msg.id == BtMessageId::kHave) {
                    if (msg.piece_index < peer_have.size()) {
                        peer_have[msg.piece_index] = true;
                    }
                } else if (msg.id == BtMessageId::kBitfield) {
                    for (size_t i = 0; i < peer_have.size(); ++i) {
                        size_t byte_idx = i / 8;
                        size_t bit_idx = 7 - (i % 8);
                        if (byte_idx < msg.payload.size()) {
                            peer_have[i] = (msg.payload[byte_idx] & (1 << bit_idx)) != 0;
                        }
                    }
                } else if (msg.id == BtMessageId::kPiece) {
                    bool verified = false;
                    bool corrupt = false;
                    piece_mgr->ProcessBlock(msg.piece_index, msg.block_begin, msg.payload.data(), msg.payload.size(), verified, corrupt);

                    if (verified) {
                        // Send Have message to peer
                        auto have_msg = PeerWireProtocol::CreateHave(msg.piece_index);
                        send(sock.s, reinterpret_cast<const char*>(have_msg.data()), static_cast<int>(have_msg.size()), 0);

                        task.completed_segments = static_cast<uint32_t>(piece_mgr->CompletedPieces());
                        task.downloaded_bytes = piece_mgr->DownloadedBytes();

                        auto now = std::chrono::steady_clock::now();
                        auto elapsed_ms = std::max<int64_t>(1, std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count());
                        task.speed_bps = (task.downloaded_bytes * 1000ULL) / static_cast<uint64_t>(elapsed_ms);
                        task.status_detail = "Downloading pieces: " + std::to_string(task.completed_segments) +
                                             " / " + std::to_string(task.total_segments);

                        if (on_progress) {
                            TorrentProgress tp;
                            tp.task_id = task.task_id;
                            tp.state = TaskState::kDownloading;
                            tp.downloaded_bytes = task.downloaded_bytes;
                            tp.total_bytes = total_bytes;
                            tp.completed_pieces = task.completed_segments;
                            tp.total_pieces = task.total_segments;
                            tp.speed_bps = task.speed_bps;
                            tp.status_detail = task.status_detail;
                            on_progress(tp);
                        }
                    }
                }
            }

            // If unchoked, request next block batch
            if (!peer_choked_us && !piece_mgr->IsAllCompleted()) {
                auto reqs = piece_mgr->GetNextRequestsForPeer(peer_have, kDefaultMaxOutstandingRequests);
                for (const auto& rq : reqs) {
                    auto req_bytes = PeerWireProtocol::CreateRequest(rq.piece_index, rq.begin, rq.length);
                    send(sock.s, reinterpret_cast<const char*>(req_bytes.data()), static_cast<int>(req_bytes.size()), 0);
                }
            }
        }
    }

    if (cancel_requested.load()) {
        task.state = TaskState::kCanceled;
        task.status_detail = "Canceled";
        return Result<void>::Err("ERR_CANCELED", "BitTorrent task canceled by user");
    }

    if (pause_requested.load()) {
        task.state = TaskState::kPaused;
        task.status_detail = "Paused";
        return Result<void>::Err("ERR_PAUSED", "BitTorrent task paused by user");
    }

    if (piece_mgr->IsAllCompleted()) {
        task.state = TaskState::kCompleted;
        task.downloaded_bytes = total_bytes;
        task.completed_segments = task.total_segments;
        task.status_detail = "Completed (BitTorrent)";

        // Announce completion to tracker
        TrackerClient::AnnounceAny(announce_urls, torrent_info.info_hash_raw, client_peer_id, 6881, 0, total_bytes, 0, "completed", 5000);

        if (on_progress) {
            TorrentProgress tp;
            tp.task_id = task.task_id;
            tp.state = TaskState::kCompleted;
            tp.downloaded_bytes = total_bytes;
            tp.total_bytes = total_bytes;
            tp.completed_pieces = task.total_segments;
            tp.total_pieces = task.total_segments;
            tp.status_detail = task.status_detail;
            on_progress(tp);
        }
        return Result<void>::Ok();
    }

    task.state = TaskState::kFailed;
    task.error_code = "ERR_TORRENT_TRANSFER_INTERRUPTED";
    task.error_message = "Peer connection closed before torrent completion";
    task.status_detail = "Failed: Transfer interrupted";
    return Result<void>::Err("ERR_TORRENT_TRANSFER_INTERRUPTED", "Peer connection closed before torrent completion");
}

} // namespace openidm::torrent
