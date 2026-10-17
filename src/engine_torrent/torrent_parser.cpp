// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — BitTorrent Metainfo & Magnet URI Parser & Piece Validator
#include "engine_torrent/torrent_parser.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "common/crypto_utils.hpp"
#include "common/path_sanitizer.hpp"

namespace openidm::torrent {

namespace {

std::string ToLower(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    return out;
}

std::vector<uint8_t> HexToBytes(std::string_view hex) {
    std::vector<uint8_t> bytes;
    if (hex.size() % 2 != 0) return bytes;
    bytes.reserve(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        char hi = hex[i];
        char lo = hex[i + 1];
        auto hex_val = [](char c) -> int {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            return -1;
        };
        int h = hex_val(hi);
        int l = hex_val(lo);
        if (h < 0 || l < 0) return {};
        bytes.push_back(static_cast<uint8_t>((h << 4) | l));
    }
    return bytes;
}

std::vector<uint8_t> Base32ToBytes(std::string_view b32) {
    // RFC 4648 Base32 alphabet: A-Z (0-25), 2-7 (26-31)
    std::vector<uint8_t> out;
    uint32_t buffer = 0;
    int bits_left = 0;

    for (char c : b32) {
        int val = -1;
        if (c >= 'A' && c <= 'Z') val = c - 'A';
        else if (c >= 'a' && c <= 'z') val = c - 'a';
        else if (c >= '2' && c <= '7') val = c - '2' + 26;
        else if (c == '=') break;
        if (val < 0) continue;

        buffer = (buffer << 5) | static_cast<uint32_t>(val);
        bits_left += 5;
        if (bits_left >= 8) {
            bits_left -= 8;
            out.push_back(static_cast<uint8_t>((buffer >> bits_left) & 0xFF));
        }
    }
    return out;
}

std::string BytesToHex(const uint8_t* data, size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        oss << std::setw(2) << static_cast<unsigned int>(data[i]);
    }
    return oss.str();
}

} // namespace

Result<TorrentInfo> TorrentParser::ParseTorrentFile(const std::filesystem::path& file_path) {
    std::ifstream ifs(file_path, std::ios::binary);
    if (!ifs) {
        return Result<TorrentInfo>::Err("ERR_TORRENT_FILE_OPEN", "Cannot open torrent file: " + file_path.string());
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string content = ss.str();
    return ParseTorrentBuffer(content);
}

Result<TorrentInfo> TorrentParser::ParseTorrentBuffer(std::string_view data) {
    auto root_res = BencodeParser::Decode(data);
    if (!root_res.ok()) {
        return Result<TorrentInfo>::Err(root_res.error->code, root_res.error->message);
    }
    const auto& root = root_res.value.value();
    if (!root.is_dict() || !root.contains("info")) {
        return Result<TorrentInfo>::Err("ERR_TORRENT_MALFORMED", "Torrent metainfo missing root 'info' dictionary");
    }

    TorrentInfo info;
    info.is_magnet = false;

    // Trackers
    if (root.contains("announce") && root["announce"].is_string()) {
        info.announce = root["announce"].as_string();
        info.announce_list.push_back(info.announce);
    }
    if (root.contains("announce-list") && root["announce-list"].is_list()) {
        for (const auto& tier : root["announce-list"].as_list()) {
            if (tier.is_list()) {
                for (const auto& tr : tier.as_list()) {
                    if (tr.is_string()) {
                        const std::string& u = tr.as_string();
                        if (std::find(info.announce_list.begin(), info.announce_list.end(), u) == info.announce_list.end()) {
                            info.announce_list.push_back(u);
                        }
                    }
                }
            }
        }
    }

    if (root.contains("comment") && root["comment"].is_string()) {
        info.comment = root["comment"].as_string();
    }
    if (root.contains("created by") && root["created by"].is_string()) {
        info.created_by = root["created by"].as_string();
    }
    if (root.contains("creation date") && root["creation date"].is_int()) {
        info.creation_date = root["creation date"].as_int();
    }

    // Info dictionary
    const auto& info_dict = root["info"];
    if (!info_dict.is_dict()) {
        return Result<TorrentInfo>::Err("ERR_TORRENT_MALFORMED", "'info' must be a bencode dictionary");
    }

    // Compute info_hash by re-encoding info dict (or finding raw slice)
    std::string encoded_info = BencodeParser::Encode(info_dict);
    info.info_hash_raw = CryptoUtils::ComputeBufferSha1Raw(reinterpret_cast<const uint8_t*>(encoded_info.data()), encoded_info.size());
    info.info_hash_hex = ToLower(BytesToHex(info.info_hash_raw.data(), info.info_hash_raw.size()));

    if (info_dict.contains("name") && info_dict["name"].is_string()) {
        info.name = PathSanitizer::SanitizeFileName(info_dict["name"].as_string());
    } else {
        info.name = "torrent_" + info.info_hash_hex.substr(0, 8);
    }

    if (info_dict.contains("piece length") && info_dict["piece length"].is_int()) {
        info.piece_length = static_cast<uint64_t>(info_dict["piece length"].as_int());
    }

    // Pieces
    if (info_dict.contains("pieces") && info_dict["pieces"].is_string()) {
        const std::string& p_str = info_dict["pieces"].as_string();
        if (p_str.size() % 20 != 0) {
            return Result<TorrentInfo>::Err("ERR_TORRENT_PIECES", "Pieces string length not a multiple of 20");
        }
        size_t count = p_str.size() / 20;
        info.pieces.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            const uint8_t* p = reinterpret_cast<const uint8_t*>(p_str.data() + (i * 20));
            info.pieces.emplace_back(p, p + 20);
        }
    }

    // Files: single file vs multi-file
    if (info_dict.contains("files") && info_dict["files"].is_list()) {
        // Multi-file
        uint64_t current_offset = 0;
        for (const auto& f : info_dict["files"].as_list()) {
            if (!f.is_dict() || !f.contains("length") || !f.contains("path")) continue;
            uint64_t len = static_cast<uint64_t>(f["length"].as_int());
            std::filesystem::path rel_path;
            if (f["path"].is_list()) {
                for (const auto& seg : f["path"].as_list()) {
                    if (seg.is_string()) {
                        rel_path /= PathSanitizer::SanitizeFileName(seg.as_string());
                    }
                }
            }
            if (rel_path.empty()) rel_path = "unnamed.bin";

            TorrentFileEntry entry;
            entry.relative_path = rel_path;
            entry.length = len;
            entry.offset_in_torrent = current_offset;
            current_offset += len;

            info.files.push_back(std::move(entry));
        }
        info.total_length = current_offset;
    } else if (info_dict.contains("length") && info_dict["length"].is_int()) {
        // Single file
        uint64_t len = static_cast<uint64_t>(info_dict["length"].as_int());
        TorrentFileEntry entry;
        entry.relative_path = info.name;
        entry.length = len;
        entry.offset_in_torrent = 0;
        info.files.push_back(std::move(entry));
        info.total_length = len;
    }

    return Result<TorrentInfo>::Ok(std::move(info));
}

Result<TorrentInfo> TorrentParser::ParseMagnetUri(std::string_view magnet_uri) {
    std::string s(magnet_uri);
    std::string prefix = "magnet:?";
    if (s.compare(0, prefix.size(), prefix) != 0) {
        return Result<TorrentInfo>::Err("ERR_INVALID_MAGNET", "Magnet URI must begin with magnet:?");
    }

    TorrentInfo info;
    info.is_magnet = true;

    std::string query = s.substr(prefix.size());
    std::stringstream ss(query);
    std::string param;

    while (std::getline(ss, param, '&')) {
        if (param.empty()) continue;
        size_t eq = param.find('=');
        if (eq == std::string::npos) continue;
        std::string key = param.substr(0, eq);
        std::string val = PathSanitizer::UrlDecode(param.substr(eq + 1));

        if (key == "xt") {
            // Exact Topic: urn:btih:<hash>
            std::string btih_prefix = "urn:btih:";
            if (val.compare(0, btih_prefix.size(), btih_prefix) == 0) {
                std::string hash = val.substr(btih_prefix.size());
                if (hash.size() == 40) {
                    info.info_hash_hex = ToLower(hash);
                    info.info_hash_raw = HexToBytes(info.info_hash_hex);
                } else if (hash.size() == 32) {
                    info.info_hash_raw = Base32ToBytes(hash);
                    info.info_hash_hex = ToLower(BytesToHex(info.info_hash_raw.data(), info.info_hash_raw.size()));
                }
            }
        } else if (key == "dn") {
            // Display Name
            info.name = PathSanitizer::SanitizeFileName(val);
        } else if (key == "tr") {
            // Tracker
            if (std::find(info.announce_list.begin(), info.announce_list.end(), val) == info.announce_list.end()) {
                info.announce_list.push_back(val);
            }
            if (info.announce.empty()) info.announce = val;
        }
    }

    if (info.info_hash_hex.empty() || info.info_hash_raw.size() != 20) {
        return Result<TorrentInfo>::Err("ERR_MAGNET_NO_HASH", "Magnet URI missing valid urn:btih hash");
    }

    if (info.name.empty()) {
        info.name = "magnet_" + info.info_hash_hex.substr(0, 8);
    }

    return Result<TorrentInfo>::Ok(std::move(info));
}

bool TorrentParser::VerifyPiece(const uint8_t* piece_data, size_t piece_len, const std::vector<uint8_t>& expected_sha1) {
    if (expected_sha1.size() != 20) return false;
    auto computed = CryptoUtils::ComputeBufferSha1Raw(piece_data, piece_len);
    return computed == expected_sha1;
}

bool TorrentParser::VerifyPieceHex(const uint8_t* piece_data, size_t piece_len, std::string_view expected_sha1_hex) {
    auto computed = CryptoUtils::ComputeBufferSha1Hex(std::string_view(reinterpret_cast<const char*>(piece_data), piece_len));
    return ToLower(computed) == ToLower(expected_sha1_hex);
}

} // namespace openidm::torrent
