// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — BitTorrent Tracker Client (BEP 3 HTTP/HTTPS Trackers, BEP 23 Compact)
// Direct reuse of aria2 DefaultBtAnnounce tracker query construction and compact peer unpacking
#pragma once

#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "brisk_engine/http_client.hpp"
#include "common/logger.hpp"
#include "common/types.hpp"
#include "engine_torrent/bencode.hpp"
#include "engine_torrent/peer_wire_protocol.hpp"

namespace openidm::torrent {

struct TrackerAnnounceResponse {
    bool ok{false};
    std::string failure_reason;
    std::string warning_message;
    uint32_t interval{1800};
    uint32_t min_interval{300};
    uint32_t complete{0};   // seeders
    uint32_t incomplete{0}; // leechers
    std::vector<PeerEndpoint> peers;
};

class TrackerClient {
public:
    // Percent-encodes binary byte buffer according to RFC 3986 (aria2 util::percentEncode)
    static std::string PercentEncodeBinary(const uint8_t* data, size_t len) {
        std::string out;
        if (!data || len == 0) return out;
        out.reserve(len * 3);
        const char hex[] = "0123456789ABCDEF";

        for (size_t i = 0; i < len; ++i) {
            uint8_t b = data[i];
            // Unreserved characters: A-Z a-z 0-9 - _ . ~
            if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z') || (b >= '0' && b <= '9') ||
                b == '-' || b == '_' || b == '.' || b == '~') {
                out.push_back(static_cast<char>(b));
            } else {
                out.push_back('%');
                out.push_back(hex[(b >> 4) & 0x0F]);
                out.push_back(hex[b & 0x0F]);
            }
        }
        return out;
    }

    // Direct adoption of aria2 DefaultBtAnnounce::processAnnounceResponse
    static Result<TrackerAnnounceResponse> ParseTrackerResponse(std::string_view response_body) {
        auto dec_res = BencodeParser::Decode(response_body);
        if (!dec_res.ok()) {
            return Result<TrackerAnnounceResponse>::Err("ERR_TORRENT_TRACKER_MALFORMED", "Failed to decode tracker response bencode");
        }

        const auto& root = dec_res.value.value();
        if (!root.is_dict()) {
            return Result<TrackerAnnounceResponse>::Err("ERR_TORRENT_TRACKER_MALFORMED", "Tracker response is not a bencode dictionary");
        }

        TrackerAnnounceResponse resp;
        resp.ok = true;

        if (root.contains("failure reason") && root["failure reason"].is_string()) {
            resp.ok = false;
            resp.failure_reason = root["failure reason"].as_string();
            return Result<TrackerAnnounceResponse>::Err("ERR_TORRENT_TRACKER_FAILURE", resp.failure_reason);
        }

        if (root.contains("warning message") && root["warning message"].is_string()) {
            resp.warning_message = root["warning message"].as_string();
        }

        if (root.contains("interval") && root["interval"].is_int()) {
            resp.interval = static_cast<uint32_t>(root["interval"].as_int());
        }
        if (root.contains("min interval") && root["min interval"].is_int()) {
            resp.min_interval = static_cast<uint32_t>(root["min interval"].as_int());
        }
        if (root.contains("complete") && root["complete"].is_int()) {
            resp.complete = static_cast<uint32_t>(root["complete"].as_int());
        }
        if (root.contains("incomplete") && root["incomplete"].is_int()) {
            resp.incomplete = static_cast<uint32_t>(root["incomplete"].as_int());
        }

        resp.peers = PeerWireProtocol::ExtractPeersFromBencode(root);
        return Result<TrackerAnnounceResponse>::Ok(std::move(resp));
    }

    // Direct adoption of aria2 DefaultBtAnnounce announce query construction
    static Result<TrackerAnnounceResponse> AnnounceHttp(
        const std::string& announce_url,
        const std::vector<uint8_t>& info_hash_raw,
        const std::string& peer_id,
        uint16_t port,
        uint64_t uploaded,
        uint64_t downloaded,
        uint64_t left,
        std::string_view event = "started",
        uint32_t timeout_ms = 10000) {

        if (info_hash_raw.size() < kInfoHashLength) {
            return Result<TrackerAnnounceResponse>::Err("ERR_TORRENT_INVALID_HASH", "Invalid info_hash for tracker announce");
        }

        std::string sep = (announce_url.find('?') == std::string::npos) ? "?" : "&";
        std::string full_url = announce_url + sep +
            "info_hash=" + PercentEncodeBinary(info_hash_raw.data(), kInfoHashLength) +
            "&peer_id=" + PercentEncodeBinary(reinterpret_cast<const uint8_t*>(peer_id.data()), peer_id.size()) +
            "&port=" + std::to_string(port) +
            "&uploaded=" + std::to_string(uploaded) +
            "&downloaded=" + std::to_string(downloaded) +
            "&left=" + std::to_string(left) +
            "&compact=1";

        if (!event.empty()) {
            full_url += "&event=" + std::string(event);
        }

        std::string response_body;
        std::atomic<bool> cancel_flag{false};
        std::map<std::string, std::string> req_headers;
        req_headers["User-Agent"] = "Flux/1.0 (BitTorrent; Win64)";
        req_headers["Accept"] = "*/*";

        brisk::HttpClient::HttpNetworkOptions net_opts;
        auto get_res = brisk::HttpClient::PerformGet(
            full_url,
            req_headers,
            std::nullopt,
            std::nullopt,
            timeout_ms,
            true,
            cancel_flag,
            [](const brisk::HttpResponseMeta& meta) -> bool {
                return meta.status_code >= 200 && meta.status_code < 400;
            },
            [&](const uint8_t* data, size_t len) -> bool {
                if (data && len > 0) {
                    response_body.append(reinterpret_cast<const char*>(data), len);
                }
                return true;
            },
            net_opts);

        if (!get_res.ok()) {
            return Result<TrackerAnnounceResponse>::Err(get_res.error->code, get_res.error->message);
        }

        return ParseTrackerResponse(response_body);
    }

    // Announces to tracker list with automatic tier failover (aria2 DefaultBtAnnounce::adjustAnnounceList)
    static Result<TrackerAnnounceResponse> AnnounceAny(
        const std::vector<std::string>& announce_urls,
        const std::vector<uint8_t>& info_hash_raw,
        const std::string& peer_id,
        uint16_t port,
        uint64_t uploaded,
        uint64_t downloaded,
        uint64_t left,
        std::string_view event = "started",
        uint32_t timeout_ms = 10000) {

        if (announce_urls.empty()) {
            return Result<TrackerAnnounceResponse>::Err("ERR_TORRENT_NO_TRACKERS", "No announce tracker URLs provided");
        }

        std::string last_error = "All announce trackers failed";
        for (const auto& url : announce_urls) {
            if (url.rfind("http://", 0) == 0 || url.rfind("https://", 0) == 0) {
                OPENIDM_LOG_INFO("TrackerClient", "Attempting announce to HTTP tracker: " + url);
                auto res = AnnounceHttp(url, info_hash_raw, peer_id, port, uploaded, downloaded, left, event, timeout_ms);
                if (res.ok()) {
                    OPENIDM_LOG_INFO("TrackerClient", "Tracker announce succeeded, peers received: " + std::to_string(res.value->peers.size()));
                    return res;
                } else {
                    last_error = res.error->message;
                    OPENIDM_LOG_WARN("TrackerClient", "Tracker announce failed for " + url + ": " + last_error);
                }
            } else if (url.rfind("udp://", 0) == 0) {
                OPENIDM_LOG_INFO("TrackerClient", "UDP tracker skipped in HTTP announce pass: " + url);
            }
        }

        return Result<TrackerAnnounceResponse>::Err("ERR_TORRENT_TRACKER_UNAVAILABLE", last_error);
    }
};

} // namespace openidm::torrent
