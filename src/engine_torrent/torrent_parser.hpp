// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — BitTorrent Metainfo & Magnet URI Parser & Piece Validator
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "common/types.hpp"
#include "engine_torrent/bencode.hpp"

namespace openidm::torrent {

struct TorrentFileEntry {
    std::filesystem::path relative_path;
    uint64_t length{0};
    uint64_t offset_in_torrent{0}; // Start offset across concatenated file tree
};

struct TorrentInfo {
    std::string name;
    std::string info_hash_hex;
    std::vector<uint8_t> info_hash_raw;
    uint64_t piece_length{0};
    std::vector<std::vector<uint8_t>> pieces; // List of 20-byte SHA-1 hashes
    std::vector<TorrentFileEntry> files;
    uint64_t total_length{0};
    std::string announce;
    std::vector<std::string> announce_list;
    std::string comment;
    std::string created_by;
    int64_t creation_date{0};
    bool is_magnet{false};

    size_t num_pieces() const { return pieces.size(); }
    uint64_t get_piece_size(size_t piece_index) const {
        if (piece_index >= pieces.size() || piece_length == 0 || total_length == 0) return 0;
        if (piece_index == pieces.size() - 1) {
            uint64_t remainder = total_length % piece_length;
            return remainder == 0 ? piece_length : remainder;
        }
        return piece_length;
    }
};

class TorrentParser {
public:
    static Result<TorrentInfo> ParseTorrentFile(const std::filesystem::path& file_path);
    static Result<TorrentInfo> ParseTorrentBuffer(std::string_view data);
    static Result<TorrentInfo> ParseMagnetUri(std::string_view magnet_uri);

    static bool GetBencodeValueRange(std::string_view s, size_t& pos, size_t depth = 0);
    static bool VerifyPiece(const uint8_t* piece_data, size_t piece_len, const std::vector<uint8_t>& expected_sha1);
    static bool VerifyPieceHex(const uint8_t* piece_data, size_t piece_len, std::string_view expected_sha1_hex);
};

} // namespace openidm::torrent
