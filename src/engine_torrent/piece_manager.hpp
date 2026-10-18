// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — BitTorrent Piece & Block Scheduler
// Direct reuse of aria2 BitfieldMan, Piece, and PieceStorage algorithms
#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "common/crypto_utils.hpp"
#include "common/logger.hpp"
#include "engine_torrent/multi_file_storage.hpp"
#include "engine_torrent/peer_wire_protocol.hpp"
#include "engine_torrent/torrent_parser.hpp"

namespace openidm::torrent {

struct BlockRequest {
    uint32_t piece_index{0};
    uint32_t begin{0};
    uint32_t length{0};
};

struct IncompletePiece {
    uint32_t piece_index{0};
    uint64_t piece_size{0};
    std::vector<uint8_t> buffer;
    std::vector<bool> blocks_received;
    std::vector<bool> blocks_requested;
    size_t completed_blocks{0};
    size_t total_blocks{0};
};

class PieceManager {
public:
    PieceManager(const TorrentInfo& info, std::shared_ptr<MultiFileStorage> storage)
        : m_info(info),
          m_storage(storage),
          m_total_pieces(info.num_pieces()),
          m_total_bytes(info.total_length) {
        m_have_pieces.assign(m_total_pieces, false);
    }

    size_t TotalPieces() const { return m_total_pieces; }
    size_t CompletedPieces() const { return m_completed_pieces; }
    uint64_t DownloadedBytes() const { return m_downloaded_bytes; }
    uint64_t TotalBytes() const { return m_total_bytes; }
    bool IsAllCompleted() const { return m_completed_pieces >= m_total_pieces; }
    const std::vector<bool>& HavePieces() const { return m_have_pieces; }

    uint64_t GetPieceSize(size_t piece_index) const {
        return m_info.get_piece_size(piece_index);
    }

    // Direct adoption of aria2 Piece::getMissingUnusedBlockIndex
    std::vector<BlockRequest> GetNextRequestsForPeer(
        const std::vector<bool>& peer_have,
        size_t max_requests = kDefaultMaxOutstandingRequests) {
        std::vector<BlockRequest> requests;
        if (IsAllCompleted() || max_requests == 0) return requests;

        // 1. First priority: complete existing in-flight active pieces
        for (auto& [piece_idx, inc] : m_active_pieces) {
            if (piece_idx < peer_have.size() && peer_have[piece_idx]) {
                for (size_t b = 0; b < inc.total_blocks && requests.size() < max_requests; ++b) {
                    if (!inc.blocks_received[b] && !inc.blocks_requested[b]) {
                        inc.blocks_requested[b] = true;
                        uint32_t begin = static_cast<uint32_t>(b * kBlockLength);
                        uint32_t len = static_cast<uint32_t>(std::min<uint64_t>(kBlockLength, inc.piece_size - begin));
                        requests.push_back(BlockRequest{piece_idx, begin, len});
                    }
                }
            }
            if (requests.size() >= max_requests) return requests;
        }

        // 2. Second priority: activate new missing pieces present on peer
        for (size_t p = 0; p < m_total_pieces && requests.size() < max_requests; ++p) {
            if (!m_have_pieces[p] && m_active_pieces.find(static_cast<uint32_t>(p)) == m_active_pieces.end()) {
                if (p < peer_have.size() && peer_have[p]) {
                    uint32_t p_u32 = static_cast<uint32_t>(p);
                    uint64_t p_sz = GetPieceSize(p);
                    if (p_sz == 0) continue;

                    size_t blocks = static_cast<size_t>((p_sz + kBlockLength - 1) / kBlockLength);
                    IncompletePiece inc;
                    inc.piece_index = p_u32;
                    inc.piece_size = p_sz;
                    inc.buffer.assign(static_cast<size_t>(p_sz), 0);
                    inc.blocks_received.assign(blocks, false);
                    inc.blocks_requested.assign(blocks, false);
                    inc.completed_blocks = 0;
                    inc.total_blocks = blocks;

                    for (size_t b = 0; b < blocks && requests.size() < max_requests; ++b) {
                        inc.blocks_requested[b] = true;
                        uint32_t begin = static_cast<uint32_t>(b * kBlockLength);
                        uint32_t len = static_cast<uint32_t>(std::min<uint64_t>(kBlockLength, p_sz - begin));
                        requests.push_back(BlockRequest{p_u32, begin, len});
                    }

                    m_active_pieces[p_u32] = std::move(inc);
                    break;
                }
            }
        }

        return requests;
    }

    // Direct adoption of aria2 BtPieceMessage::doReceivedAction and SHA-1 verification
    bool ProcessBlock(
        uint32_t piece_index,
        uint32_t begin,
        const uint8_t* block_data,
        size_t block_len,
        bool& out_piece_verified,
        bool& out_piece_corrupt) {
        out_piece_verified = false;
        out_piece_corrupt = false;

        if (piece_index >= m_total_pieces || !block_data || block_len == 0) return false;
        if (m_have_pieces[piece_index]) return true; // Already downloaded

        auto it = m_active_pieces.find(piece_index);
        if (it == m_active_pieces.end()) {
            // Unsolicited block for inactive piece; initialize piece
            uint64_t p_sz = GetPieceSize(piece_index);
            if (p_sz == 0 || begin + block_len > p_sz) return false;

            size_t blocks = static_cast<size_t>((p_sz + kBlockLength - 1) / kBlockLength);
            IncompletePiece inc;
            inc.piece_index = piece_index;
            inc.piece_size = p_sz;
            inc.buffer.assign(static_cast<size_t>(p_sz), 0);
            inc.blocks_received.assign(blocks, false);
            inc.blocks_requested.assign(blocks, false);
            inc.completed_blocks = 0;
            inc.total_blocks = blocks;
            m_active_pieces[piece_index] = std::move(inc);
            it = m_active_pieces.find(piece_index);
        }

        auto& inc = it->second;
        if (begin + block_len > inc.piece_size) return false;

        size_t block_idx = begin / kBlockLength;
        if (block_idx >= inc.total_blocks) return false;

        if (!inc.blocks_received[block_idx]) {
            std::memcpy(inc.buffer.data() + begin, block_data, block_len);
            inc.blocks_received[block_idx] = true;
            inc.completed_blocks++;
        }

        // If piece is complete, perform cryptographic piece SHA-1 verification
        if (inc.completed_blocks == inc.total_blocks) {
            bool verified = false;
            if (piece_index < m_info.pieces.size()) {
                verified = TorrentParser::VerifyPiece(inc.buffer.data(), inc.buffer.size(), m_info.pieces[piece_index]);
            }

            if (verified) {
                if (m_storage) {
                    uint64_t torrent_offset = static_cast<uint64_t>(piece_index) * m_info.piece_length;
                    auto wr_res = m_storage->WriteBytes(torrent_offset, inc.buffer.data(), inc.buffer.size());
                    if (!wr_res.ok()) {
                        OPENIDM_LOG_ERROR("PieceManager", "Failed to write piece to disk: " + wr_res.error->message);
                        return false;
                    }
                }
                m_have_pieces[piece_index] = true;
                m_completed_pieces++;
                m_downloaded_bytes += inc.piece_size;
                m_active_pieces.erase(it);
                out_piece_verified = true;
                return true;
            } else {
                OPENIDM_LOG_WARN("PieceManager", "Piece #" + std::to_string(piece_index) + " failed SHA-1 hash check; discarding corrupt block data");
                out_piece_corrupt = true;
                inc.completed_blocks = 0;
                std::fill(inc.blocks_received.begin(), inc.blocks_received.end(), false);
                std::fill(inc.blocks_requested.begin(), inc.blocks_requested.end(), false);
                return false;
            }
        }

        return true;
    }

    void CancelInFlightRequests(const std::vector<BlockRequest>& requests) {
        for (const auto& req : requests) {
            auto it = m_active_pieces.find(req.piece_index);
            if (it != m_active_pieces.end()) {
                size_t b = req.begin / kBlockLength;
                if (b < it->second.blocks_requested.size() && !it->second.blocks_received[b]) {
                    it->second.blocks_requested[b] = false;
                }
            }
        }
    }

private:
    TorrentInfo m_info;
    std::shared_ptr<MultiFileStorage> m_storage;
    size_t m_total_pieces{0};
    size_t m_completed_pieces{0};
    uint64_t m_downloaded_bytes{0};
    uint64_t m_total_bytes{0};

    std::vector<bool> m_have_pieces;
    std::map<uint32_t, IncompletePiece> m_active_pieces;
};

} // namespace openidm::torrent
