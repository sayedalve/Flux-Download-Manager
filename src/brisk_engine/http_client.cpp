// SPDX-License-Identifier: GPL-3.0-or-later
#include "brisk_engine/http_client.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#endif

namespace openidm::brisk {

namespace {

std::string ToLower(std::string_view sv) {
    std::string s(sv);
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string Trim(std::string_view sv) {
    while (!sv.empty() && (sv.front() == ' ' || sv.front() == '\t' || sv.front() == '\r' || sv.front() == '\n')) {
        sv.remove_prefix(1);
    }
    while (!sv.empty() && (sv.back() == ' ' || sv.back() == '\t' || sv.back() == '\r' || sv.back() == '\n')) {
        sv.remove_suffix(1);
    }
    return std::string(sv);
}

bool IsLoopbackHost(std::string_view host) {
    std::string h = ToLower(host);
    return h == "localhost" || h == "127.0.0.1" || h == "::1" || h.rfind("127.", 0) == 0 || h.rfind("169.254.", 0) == 0;
}

bool ParseContentRange(std::string_view val, uint64_t& start, uint64_t& end, uint64_t& total) {
    std::string s = Trim(val);
    if (ToLower(s.substr(0, 6)) != "bytes ") return false;
    s = Trim(s.substr(6));
    auto slash = s.find('/');
    if (slash == std::string::npos) return false;
    std::string range_part = s.substr(0, slash);
    std::string total_part = s.substr(slash + 1);
    auto dash = range_part.find('-');
    if (dash == std::string::npos) return false;
    try {
        start = std::stoull(range_part.substr(0, dash));
        end = std::stoull(range_part.substr(dash + 1));
        if (total_part != "*") {
            total = std::stoull(total_part);
        } else {
            total = 0;
        }
        return start <= end;
    } catch (...) {
        return false;
    }
}

void PopulateHeaderField(HttpResponseMeta& meta, const std::string& k, const std::string& v) {
    std::string kl = ToLower(k);
    meta.raw_headers[kl] = v;
    if (kl == "content-length") {
        try {
            meta.content_length = std::stoull(v);
            meta.has_content_length = true;
        } catch (...) {}
    } else if (kl == "accept-ranges") {
        if (ToLower(v).find("bytes") != std::string::npos) {
            meta.accept_ranges_bytes = true;
        }
    } else if (kl == "content-range") {
        if (ParseContentRange(v, meta.range_start, meta.range_end, meta.range_total)) {
            meta.is_partial_content = true;
            meta.accept_ranges_bytes = true;
        }
    } else if (kl == "etag") {
        meta.etag = v;
    } else if (kl == "last-modified") {
        meta.last_modified = v;
    } else if (kl == "content-disposition") {
        meta.content_disposition = v;
    } else if (kl == "content-type") {
        meta.content_type = v;
    } else if (kl == "retry-after") {
        try { meta.retry_after_sec = std::stoi(v); } catch (...) {}
    }
}

#ifdef _WIN32
std::wstring Utf8ToWide(std::string_view sv) {
    if (sv.empty()) return {};
    int sz = MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), nullptr, 0);
    if (sz <= 0) return {};
    std::wstring out(static_cast<size_t>(sz), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), out.data(), sz);
    return out;
}

std::string WideToUtf8(std::wstring_view wv) {
    if (wv.empty()) return {};
    int sz = WideCharToMultiByte(CP_UTF8, 0, wv.data(), static_cast<int>(wv.size()), nullptr, 0, nullptr, nullptr);
    if (sz <= 0) return {};
    std::string out(static_cast<size_t>(sz), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wv.data(), static_cast<int>(wv.size()), out.data(), sz, nullptr, nullptr);
    return out;
}
#endif

} // namespace

Result<ParsedUrl> HttpClient::ParseUrl(std::string_view url) {
    std::string u = Trim(url);
    auto scheme_pos = u.find("://");
    if (scheme_pos == std::string::npos) {
        return Result<ParsedUrl>::Err("ERR_INVALID_URL", "Missing URL scheme");
    }
    ParsedUrl out;
    out.scheme = ToLower(u.substr(0, scheme_pos));
    if (out.scheme != "http" && out.scheme != "https") {
        return Result<ParsedUrl>::Err("ERR_UNSAFE_SCHEME", "Only http:// and https:// schemes are permitted");
    }
    out.port = (out.scheme == "https") ? 443 : 80;

    std::string rest = u.substr(scheme_pos + 3);
    auto slash_pos = rest.find('/');
    std::string authority = (slash_pos == std::string::npos) ? rest : rest.substr(0, slash_pos);
    out.path_and_query = (slash_pos == std::string::npos) ? "/" : rest.substr(slash_pos);

    if (authority.empty()) {
        return Result<ParsedUrl>::Err("ERR_INVALID_URL", "Empty URL host");
    }

    if (authority.front() == '[') {
        auto close_b = authority.find(']');
        if (close_b == std::string::npos) return Result<ParsedUrl>::Err("ERR_INVALID_URL", "Invalid IPv6 host");
        out.host = authority.substr(1, close_b - 1);
        if (close_b + 1 < authority.size() && authority[close_b + 1] == ':') {
            out.port = static_cast<uint16_t>(std::stoi(authority.substr(close_b + 2)));
        }
    } else {
        auto colon = authority.find(':');
        if (colon != std::string::npos) {
            out.host = authority.substr(0, colon);
            try {
                int p = std::stoi(authority.substr(colon + 1));
                if (p <= 0 || p > 65535) return Result<ParsedUrl>::Err("ERR_INVALID_URL", "Port out of range");
                out.port = static_cast<uint16_t>(p);
            } catch (...) {
                return Result<ParsedUrl>::Err("ERR_INVALID_URL", "Invalid port number");
            }
        } else {
            out.host = authority;
        }
    }

    out.is_loopback_or_private = IsLoopbackHost(out.host);
    return Result<ParsedUrl>::Ok(std::move(out));
}

Result<HttpResponseMeta> HttpClient::PerformGet(
    const std::string& initial_url,
    const std::map<std::string, std::string>& headers,
    std::optional<uint64_t> range_start,
    std::optional<uint64_t> range_end,
    int timeout_ms,
    bool allow_loopback_redirect,
    std::atomic<bool>& cancel_flag,
    std::function<bool(const HttpResponseMeta&)> on_headers,
    std::function<bool(const uint8_t* data, size_t len)> on_chunk) {

    std::string current_url = initial_url;
    bool started_on_public_host = false;

    for (int redirect_hop = 0; redirect_hop <= 10; ++redirect_hop) {
        if (cancel_flag.load()) {
            return Result<HttpResponseMeta>::Err("ERR_CANCELED", "Transfer canceled");
        }

        auto parsed_res = ParseUrl(current_url);
        if (!parsed_res.ok()) {
            return Result<HttpResponseMeta>::Err(parsed_res.error->code, parsed_res.error->message);
        }
        ParsedUrl pu = *parsed_res.value;
        if (redirect_hop == 0) {
            started_on_public_host = !pu.is_loopback_or_private;
        } else if (started_on_public_host && pu.is_loopback_or_private && !allow_loopback_redirect) {
            return Result<HttpResponseMeta>::Err(
                "ERR_NET_UNSAFE_REDIRECT",
                "Blocked unsafe redirect from public host to loopback/private address: " + current_url);
        }

#ifdef _WIN32
        // ---------------------------------------------------------------------
        // Native Windows 11 WinHTTP Implementation (BUG-003 Fix)
        // ---------------------------------------------------------------------
        HINTERNET hSession = WinHttpOpen(
            L"Flux/0.3.0 (Windows NT 10.0; Win64; x64)",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0);
        if (!hSession) {
            return Result<HttpResponseMeta>::Err("ERR_NET_INIT", "WinHttpOpen failed", true);
        }
        WinHttpSetTimeouts(hSession, timeout_ms, timeout_ms, timeout_ms, timeout_ms);

        std::wstring whost = Utf8ToWide(pu.host);
        HINTERNET hConnect = WinHttpConnect(hSession, whost.c_str(), pu.port, 0);
        if (!hConnect) {
            WinHttpCloseHandle(hSession);
            return Result<HttpResponseMeta>::Err("ERR_NET_CONNECT", "WinHttpConnect failed for " + pu.host, true);
        }

        std::wstring wpath = Utf8ToWide(pu.path_and_query);
        DWORD req_flags = (pu.scheme == "https") ? WINHTTP_FLAG_SECURE : 0;
        HINTERNET hRequest = WinHttpOpenRequest(
            hConnect, L"GET", wpath.c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, req_flags);
        if (!hRequest) {
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return Result<HttpResponseMeta>::Err("ERR_NET_REQUEST", "WinHttpOpenRequest failed", true);
        }

        // Disable automatic WinHTTP redirects so OpenIDM's SSRF redirect guard inspects every hop
        DWORD disable_redirects = WINHTTP_DISABLE_REDIRECTS;
        WinHttpSetOption(hRequest, WINHTTP_OPTION_DISABLE_FEATURE, &disable_redirects, sizeof(disable_redirects));

        auto cleanup_winhttp = [&]() {
            if (hRequest) { WinHttpCloseHandle(hRequest); hRequest = nullptr; }
            if (hConnect) { WinHttpCloseHandle(hConnect); hConnect = nullptr; }
            if (hSession) { WinHttpCloseHandle(hSession); hSession = nullptr; }
        };

        std::ostringstream hdr_stream;
        for (const auto& [k, v] : headers) {
            if (ToLower(k) == "range" || ToLower(k) == "host" || ToLower(k) == "connection") continue;
            hdr_stream << k << ": " << v << "\r\n";
        }
        if (range_start.has_value()) {
            hdr_stream << "Range: bytes=" << *range_start << "-";
            if (range_end.has_value()) hdr_stream << *range_end;
            hdr_stream << "\r\n";
        }
        std::wstring wheaders = Utf8ToWide(hdr_stream.str());
        if (!wheaders.empty()) {
            WinHttpAddRequestHeaders(hRequest, wheaders.c_str(), static_cast<DWORD>(wheaders.size()), WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);
        }

        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
            !WinHttpReceiveResponse(hRequest, nullptr)) {
            cleanup_winhttp();
            return Result<HttpResponseMeta>::Err("ERR_NET_SEND", "WinHTTP request/response failed", true);
        }

        DWORD status_code = 0;
        DWORD status_sz = sizeof(status_code);
        WinHttpQueryHeaders(
            hRequest,
            WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX,
            &status_code,
            &status_sz,
            WINHTTP_NO_HEADER_INDEX);

        DWORD raw_hdr_bytes = 0;
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &raw_hdr_bytes, WINHTTP_NO_HEADER_INDEX);
        std::wstring raw_whdrs(raw_hdr_bytes / sizeof(wchar_t), L'\0');
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, raw_whdrs.data(), &raw_hdr_bytes, WINHTTP_NO_HEADER_INDEX);

        HttpResponseMeta meta;
        meta.status_code = static_cast<int>(status_code);
        meta.final_url = current_url;

        std::istringstream hstream(WideToUtf8(raw_whdrs));
        std::string line;
        std::getline(hstream, line); // Skip status line
        while (std::getline(hstream, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            auto colon = line.find(':');
            if (colon == std::string::npos) continue;
            PopulateHeaderField(meta, Trim(line.substr(0, colon)), Trim(line.substr(colon + 1)));
        }

        if (meta.status_code == 301 || meta.status_code == 302 || meta.status_code == 303 ||
            meta.status_code == 307 || meta.status_code == 308) {
            cleanup_winhttp();
            auto loc_it = meta.raw_headers.find("location");
            if (loc_it == meta.raw_headers.end() || loc_it->second.empty()) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_REDIRECT_NO_LOCATION", "Redirect missing Location header");
            }
            std::string loc = loc_it->second;
            if (loc.find("://") == std::string::npos) {
                std::ostringstream abs_url;
                abs_url << pu.scheme << "://" << pu.host << ":" << pu.port;
                if (loc.front() != '/') abs_url << "/";
                abs_url << loc;
                current_url = abs_url.str();
            } else {
                current_url = loc;
            }
            continue;
        }

        if (on_headers && !on_headers(meta)) {
            cleanup_winhttp();
            return Result<HttpResponseMeta>::Ok(std::move(meta));
        }

        if (meta.status_code >= 400) {
            cleanup_winhttp();
            if (meta.status_code == 401 || meta.status_code == 403 || meta.status_code == 410) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_403_EXPIRED_OR_AUTH", "HTTP " + std::to_string(meta.status_code), false, true);
            }
            if (meta.status_code == 404) return Result<HttpResponseMeta>::Err("ERR_HTTP_404_NOT_FOUND", "HTTP 404 Not Found", false, false);
            if (meta.status_code == 412) return Result<HttpResponseMeta>::Err("ERR_HTTP_VALIDATOR_MISMATCH", "HTTP 412 Precondition Failed", false, true);
            if (meta.status_code == 416) return Result<HttpResponseMeta>::Err("ERR_HTTP_RANGE_NOT_SATISFIABLE", "HTTP 416 Range Not Satisfiable", false, true);
            if (meta.status_code == 429) return Result<HttpResponseMeta>::Err("ERR_HTTP_429_RATE_LIMITED", "HTTP 429 Too Many Requests", true, true);
            if (meta.status_code >= 500) return Result<HttpResponseMeta>::Err("ERR_HTTP_5XX_TRANSIENT", "HTTP " + std::to_string(meta.status_code), true, true);
            return Result<HttpResponseMeta>::Err("ERR_HTTP_CLIENT", "HTTP status " + std::to_string(meta.status_code));
        }

        uint64_t bytes_delivered = 0;
        std::vector<uint8_t> rbuf(16384);
        while (!meta.has_content_length || bytes_delivered < meta.content_length) {
            if (cancel_flag.load()) {
                cleanup_winhttp();
                return Result<HttpResponseMeta>::Err("ERR_CANCELED", "Transfer canceled");
            }
            DWORD bytes_read = 0;
            if (!WinHttpReadData(hRequest, rbuf.data(), static_cast<DWORD>(rbuf.size()), &bytes_read)) {
                cleanup_winhttp();
                return Result<HttpResponseMeta>::Err("ERR_NET_TIMEOUT", "WinHttpReadData timeout or error", true, true);
            }
            if (bytes_read == 0) break;
            if (on_chunk && !on_chunk(rbuf.data(), bytes_read)) {
                cleanup_winhttp();
                return Result<HttpResponseMeta>::Ok(std::move(meta));
            }
            bytes_delivered += bytes_read;
        }
        cleanup_winhttp();
        if (meta.has_content_length && bytes_delivered < meta.content_length) {
            return Result<HttpResponseMeta>::Err("ERR_NET_CONNECTION_RESET", "Premature EOF before Content-Length", true, true);
        }
        return Result<HttpResponseMeta>::Ok(std::move(meta));
#else
        // ---------------------------------------------------------------------
        // POSIX Socket + OpenSSL Implementation
        // ---------------------------------------------------------------------
        struct addrinfo hints{};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        struct addrinfo* res = nullptr;
        std::string port_str = std::to_string(pu.port);
        if (getaddrinfo(pu.host.c_str(), port_str.c_str(), &hints, &res) != 0 || !res) {
            return Result<HttpResponseMeta>::Err("ERR_NET_DNS", "Failed to resolve host: " + pu.host, true);
        }

        int sock = -1;
        for (struct addrinfo* rp = res; rp != nullptr; rp = rp->ai_next) {
            sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (sock < 0) continue;

            struct timeval tv{};
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;
            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

            if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
                break;
            }
            close(sock);
            sock = -1;
        }
        freeaddrinfo(res);

        if (sock < 0) {
            return Result<HttpResponseMeta>::Err("ERR_NET_CONNECT", "Connection failed to " + pu.host + ":" + port_str, true);
        }

        SSL_CTX* ssl_ctx = nullptr;
        SSL* ssl = nullptr;
        auto cleanup_conn = [&]() {
            if (ssl) { SSL_shutdown(ssl); SSL_free(ssl); ssl = nullptr; }
            if (ssl_ctx) { SSL_CTX_free(ssl_ctx); ssl_ctx = nullptr; }
            if (sock >= 0) { close(sock); sock = -1; }
        };

        if (pu.scheme == "https") {
            ssl_ctx = SSL_CTX_new(TLS_client_method());
            if (!ssl_ctx) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Err("ERR_TLS_INIT", "SSL_CTX_new failed");
            }
            ssl = SSL_new(ssl_ctx);
            SSL_set_tlsext_host_name(ssl, pu.host.c_str());
            SSL_set_fd(ssl, sock);
            if (SSL_connect(ssl) != 1) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Err("ERR_TLS_HANDSHAKE", "TLS handshake failed", true);
            }
        }

        auto write_all = [&](const char* buf, size_t len) -> bool {
            size_t sent = 0;
            while (sent < len) {
                ssize_t n = ssl ? SSL_write(ssl, buf + sent, static_cast<int>(len - sent))
                                : send(sock, buf + sent, len - sent, 0);
                if (n <= 0) return false;
                sent += static_cast<size_t>(n);
            }
            return true;
        };

        auto read_some = [&](char* buf, size_t cap) -> ssize_t {
            return ssl ? SSL_read(ssl, buf, static_cast<int>(cap))
                       : recv(sock, buf, cap, 0);
        };

        std::ostringstream req;
        req << "GET " << pu.path_and_query << " HTTP/1.1\r\n";
        req << "Host: " << pu.host;
        if ((pu.scheme == "http" && pu.port != 80) || (pu.scheme == "https" && pu.port != 443)) {
            req << ":" << pu.port;
        }
        req << "\r\n";
        req << "Connection: close\r\n";

        bool has_ua = false;
        for (const auto& [k, v] : headers) {
            if (ToLower(k) == "user-agent") has_ua = true;
            if (ToLower(k) == "range" || ToLower(k) == "host" || ToLower(k) == "connection") continue;
            req << k << ": " << v << "\r\n";
        }
        if (!has_ua) {
            req << "User-Agent: Flux/0.3.0 (Windows NT 10.0; Win64; x64)\r\n";
        }
        if (range_start.has_value()) {
            req << "Range: bytes=" << *range_start << "-";
            if (range_end.has_value()) {
                req << *range_end;
            }
            req << "\r\n";
        }
        req << "\r\n";

        std::string req_str = req.str();
        if (!write_all(req_str.data(), req_str.size())) {
            cleanup_conn();
            return Result<HttpResponseMeta>::Err("ERR_NET_SEND", "Failed to send HTTP request", true);
        }

        std::string header_buf;
        header_buf.reserve(4096);
        std::vector<char> tmp_vec(8192);
        char* tmp = tmp_vec.data();
        size_t header_end_pos = std::string::npos;
        while ((header_end_pos = header_buf.find("\r\n\r\n")) == std::string::npos) {
            if (cancel_flag.load()) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Err("ERR_CANCELED", "Canceled during header read");
            }
            if (header_buf.size() > 65536) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Err("ERR_HTTP_HEADER_OVERFLOW", "Response headers exceeded 64 KiB");
            }
            ssize_t n = read_some(tmp, tmp_vec.size());
            if (n <= 0) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Err("ERR_NET_TIMEOUT", "Socket read timeout or closed before headers", true);
            }
            header_buf.append(tmp, static_cast<size_t>(n));
        }

        std::string raw_header_block = header_buf.substr(0, header_end_pos);
        std::string leftover_body = header_buf.substr(header_end_pos + 4);

        HttpResponseMeta meta;
        meta.final_url = current_url;

        std::istringstream hstream(raw_header_block);
        std::string status_line;
        if (!std::getline(hstream, status_line)) {
            cleanup_conn();
            return Result<HttpResponseMeta>::Err("ERR_HTTP_MALFORMED", "Empty HTTP status line");
        }
        {
            std::istringstream sline(status_line);
            std::string proto;
            sline >> proto >> meta.status_code;
        }

        std::string line;
        while (std::getline(hstream, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            auto colon = line.find(':');
            if (colon == std::string::npos) continue;
            PopulateHeaderField(meta, Trim(line.substr(0, colon)), Trim(line.substr(colon + 1)));
        }

        if (meta.status_code == 301 || meta.status_code == 302 || meta.status_code == 303 ||
            meta.status_code == 307 || meta.status_code == 308) {
            cleanup_conn();
            auto loc_it = meta.raw_headers.find("location");
            if (loc_it == meta.raw_headers.end() || loc_it->second.empty()) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_REDIRECT_NO_LOCATION", "Redirect missing Location header");
            }
            std::string loc = loc_it->second;
            if (loc.find("://") == std::string::npos) {
                std::ostringstream abs_url;
                abs_url << pu.scheme << "://" << pu.host << ":" << pu.port;
                if (loc.front() != '/') abs_url << "/";
                abs_url << loc;
                current_url = abs_url.str();
            } else {
                current_url = loc;
            }
            continue;
        }

        if (on_headers && !on_headers(meta)) {
            cleanup_conn();
            return Result<HttpResponseMeta>::Ok(std::move(meta));
        }

        if (meta.status_code >= 400) {
            cleanup_conn();
            if (meta.status_code == 401 || meta.status_code == 403 || meta.status_code == 410) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_403_EXPIRED_OR_AUTH",
                    "HTTP " + std::to_string(meta.status_code) + " (Authentication or Expired URL)", false, true);
            }
            if (meta.status_code == 404) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_404_NOT_FOUND", "HTTP 404 Not Found", false, false);
            }
            if (meta.status_code == 412) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_VALIDATOR_MISMATCH", "HTTP 412 Precondition Failed", false, true);
            }
            if (meta.status_code == 416) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_RANGE_NOT_SATISFIABLE", "HTTP 416 Range Not Satisfiable", false, true);
            }
            if (meta.status_code == 429) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_429_RATE_LIMITED", "HTTP 429 Too Many Requests", true, true);
            }
            if (meta.status_code >= 500) {
                return Result<HttpResponseMeta>::Err("ERR_HTTP_5XX_TRANSIENT",
                    "HTTP " + std::to_string(meta.status_code) + " Server Error", true, true);
            }
            return Result<HttpResponseMeta>::Err("ERR_HTTP_CLIENT", "HTTP status " + std::to_string(meta.status_code));
        }

        uint64_t bytes_delivered = 0;
        if (!leftover_body.empty()) {
            if (on_chunk && !on_chunk(reinterpret_cast<const uint8_t*>(leftover_body.data()), leftover_body.size())) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Ok(std::move(meta));
            }
            bytes_delivered += leftover_body.size();
        }

        while (!meta.has_content_length || bytes_delivered < meta.content_length) {
            if (cancel_flag.load()) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Err("ERR_CANCELED", "Transfer canceled");
            }
            ssize_t n = read_some(tmp, tmp_vec.size());
            if (n == 0) break;
            if (n < 0) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Err("ERR_NET_TIMEOUT", "Socket stall/timeout during body read", true, true);
            }
            if (on_chunk && !on_chunk(reinterpret_cast<const uint8_t*>(tmp), static_cast<size_t>(n))) {
                cleanup_conn();
                return Result<HttpResponseMeta>::Ok(std::move(meta));
            }
            bytes_delivered += static_cast<uint64_t>(n);
        }

        cleanup_conn();
        if (meta.has_content_length && bytes_delivered < meta.content_length) {
            return Result<HttpResponseMeta>::Err("ERR_NET_CONNECTION_RESET", "Premature EOF before Content-Length", true, true);
        }
        return Result<HttpResponseMeta>::Ok(std::move(meta));
#endif
    }

    return Result<HttpResponseMeta>::Err("ERR_HTTP_TOO_MANY_REDIRECTS", "Exceeded 10 redirects");
}

} // namespace openidm::brisk
