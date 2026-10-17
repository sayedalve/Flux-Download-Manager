// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM — HTTP/1.1 & HTTPS Ranged Client (POSIX Sockets + OpenSSL & Windows WinHTTP)
// Implements FR-001, FR-002, FR-004, RISK-006, THREAT-005.
#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include "common/types.hpp"

namespace openidm::brisk {

struct ParsedUrl {
    std::string scheme; // "http" or "https"
    std::string host;
    uint16_t port{80};
    std::string path_and_query{"/"};
    bool is_loopback_or_private{false};
};

struct HttpResponseMeta {
    int status_code{0};
    std::string final_url;
    uint64_t content_length{0};
    bool has_content_length{false};
    bool accept_ranges_bytes{false};
    bool is_partial_content{false};
    uint64_t range_start{0};
    uint64_t range_end{0};
    uint64_t range_total{0};
    std::string etag;
    std::string last_modified;
    std::string content_disposition;
    std::string content_type;
    int retry_after_sec{0};
    bool is_chunked{false};
    std::map<std::string, std::string> raw_headers;
};

// State machine for decoding HTTP/1.1 chunked transfer encoding (RFC 9112 Section 7.1)
// Removes chunk framing (hex size, extensions, chunk CRLFs, trailers) and delivers
// pure entity body bytes to the consumer (FDM-003, TG-04).
class ChunkedDecoder {
public:
    enum class State {
        kChunkHeader,
        kChunkData,
        kChunkCrlf,
        kTrailers,
        kComplete,
        kError
    };

    State state{State::kChunkHeader};
    uint64_t current_chunk_size{0};
    uint64_t current_chunk_remaining{0};
    std::string buffer;
    std::string error_message;

    bool Feed(const char* data, size_t len, const std::function<bool(const uint8_t*, size_t)>& on_chunk);
    bool IsComplete() const { return state == State::kComplete; }
    bool HasError() const { return state == State::kError; }
};

class HttpClient {
public:
    static Result<ParsedUrl> ParseUrl(std::string_view url);
    static std::string ResolveRedirectTarget(const ParsedUrl& base, const std::string& loc);

    // Executes an HTTP/HTTPS GET request with optional byte range [range_start, range_end].
    // Calls on_headers once response headers arrive; if on_headers returns false, aborts before reading body.
    // Calls on_chunk for each body chunk; if on_chunk returns false or cancel_flag becomes true, aborts immediately.
    static Result<HttpResponseMeta> PerformGet(
        const std::string& url,
        const std::map<std::string, std::string>& headers,
        std::optional<uint64_t> range_start,
        std::optional<uint64_t> range_end,
        int timeout_ms,
        bool allow_loopback_redirect,
        std::atomic<bool>& cancel_flag,
        std::function<bool(const HttpResponseMeta&)> on_headers,
        std::function<bool(const uint8_t* data, size_t len)> on_chunk);
};

} // namespace openidm::brisk
