// SPDX-License-Identifier: GPL-3.0-or-later
#include "ipc/ipc_server.hpp"
#include "common/path_sanitizer.hpp"

#include "common/json_mini.hpp"
#include "common/logger.hpp"

#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <sddl.h>
#else
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#endif

namespace openidm::ipc {

namespace {

#ifdef _WIN32
std::string GetCurrentUserSidString() {
    std::string sid_str;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        DWORD len = 0;
        GetTokenInformation(hToken, TokenUser, NULL, 0, &len);
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER && len > 0) {
            std::vector<BYTE> buf(len);
            if (GetTokenInformation(hToken, TokenUser, buf.data(), len, &len)) {
                TOKEN_USER* pUser = reinterpret_cast<TOKEN_USER*>(buf.data());
                LPSTR str = nullptr;
                if (ConvertSidToStringSidA(pUser->User.Sid, &str) && str) {
                    sid_str = str;
                    LocalFree(str);
                }
            }
        }
        CloseHandle(hToken);
    }
    return sid_str;
}
#endif

json::Value TaskToJson(const TaskModel& t) {
    json::Value obj = json::Object{};
    obj["taskId"] = t.task_id;
    obj["correlationId"] = t.correlation_id.empty() ? t.task_id : t.correlation_id;
    obj["taskType"] = TaskTypeToString(t.task_type);
    obj["state"] = TaskStateToString(t.state);
    obj["category"] = t.category;
    obj["url"] = t.url;
    obj["pageUrl"] = t.page_url;
    obj["fileName"] = t.file_name;
    obj["saveDir"] = t.save_dir;
    obj["totalBytes"] = t.total_bytes;
    obj["downloadedBytes"] = t.downloaded_bytes;
    obj["speedBps"] = t.speed_bps;
    obj["streamPercent"] = t.stream_percent;
    obj["completedSegments"] = static_cast<int64_t>(t.completed_segments);
    obj["totalSegments"] = static_cast<int64_t>(t.total_segments);
    obj["statusDetail"] = t.status_detail;
    obj["rangeSupported"] = t.range_supported;
    obj["retryCount"] = static_cast<int64_t>(t.retry_count);
    obj["errorCode"] = t.error_code;
    obj["errorMessage"] = t.error_message;
    return obj;
}

json::Value MakeRpcError(const json::Value& id, int code, const std::string& message) {
    json::Value resp = json::Object{};
    resp["jsonrpc"] = "2.0";
    resp["id"] = id;
    json::Value err = json::Object{};
    err["code"] = code;
    err["message"] = message;
    resp["error"] = std::move(err);
    return resp;
}

json::Value MakeRpcResult(const json::Value& id, json::Value result) {
    json::Value resp = json::Object{};
    resp["jsonrpc"] = "2.0";
    resp["id"] = id;
    resp["result"] = std::move(result);
    return resp;
}

void ExtractHeadersMap(
    const json::Value& hdrs_val,
    std::map<std::string, std::string>& normal_headers,
    std::map<std::string, std::string>& auth_headers) {
    for (const auto& [k, v] : hdrs_val.as_object()) {
        std::string val = v.as_string();
        if (val.empty()) continue;
        std::string kl = k;
        for (char& c : kl) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (kl == "cookie" || kl == "authorization" || kl == "proxy-authorization") {
            auth_headers[k] = val;
        } else {
            normal_headers[k] = val;
        }
    }
}

} // namespace

std::string IpcServer::DefaultSocketPath() {
    if (const char* env_flux = std::getenv("FLUX_IPC_SOCKET")) {
        if (env_flux[0] != '\0') return env_flux;
    }
    if (const char* env_old = std::getenv("OPENIDM_IPC_SOCKET")) {
        if (env_old[0] != '\0') return env_old;
    }
#ifdef _WIN32
    std::string user_sid = GetCurrentUserSidString();
    if (!user_sid.empty()) {
        return "\\\\.\\pipe\\Flux_IPC_" + user_sid;
    }
    char uname[256] = {0};
    DWORD ulen = sizeof(uname);
    if (GetUserNameA(uname, &ulen) && uname[0] != '\0') {
        return "\\\\.\\pipe\\Flux_IPC_" + std::string(uname);
    }
    return "\\\\.\\pipe\\Flux_IPC_Default";
#else
    return "/tmp/flux_ipc_" + std::to_string(geteuid()) + ".sock";
#endif
}

IpcServer::IpcServer(DownloadController& controller, std::string socket_path)
    : m_controller(controller),
      m_socket_path(socket_path.empty() ? DefaultSocketPath() : std::move(socket_path)) {
    m_capture_enabled.store(m_controller.GetSettings().browser_capture_enabled);
}

IpcServer::~IpcServer() {
    Stop();
}

Result<void> IpcServer::Start() {
    if (m_running.load()) return Result<void>::Ok();

#ifdef _WIN32
    std::wstring wpipe(m_socket_path.begin(), m_socket_path.end());

    PSECURITY_DESCRIPTOR pSD = nullptr;
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = FALSE;

    std::string user_sid = GetCurrentUserSidString();
    std::wstring sddl;
    if (!user_sid.empty()) {
        std::wstring wSid(user_sid.begin(), user_sid.end());
        sddl = L"D:(A;;GA;;;" + wSid + L")(A;;GA;;;BA)";
    } else {
        sddl = L"D:(A;;GA;;;OW)(A;;GA;;;BA)";
    }
    if (ConvertStringSecurityDescriptorToSecurityDescriptorW(
            sddl.c_str(), SDDL_REVISION_1, &pSD, nullptr)) {
        sa.lpSecurityDescriptor = pSD;
    }

    HANDLE hFirst = CreateNamedPipeW(
        wpipe.c_str(),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
        PIPE_UNLIMITED_INSTANCES,
        65536,
        65536,
        50,
        sa.lpSecurityDescriptor ? &sa : nullptr);
    if (pSD) LocalFree(pSD);

    if (hFirst == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        return Result<void>::Err("ERR_IPC_PIPE_CLAIM", "Failed to claim first pipe instance for " + m_socket_path + " (error=" + std::to_string(err) + ")");
    }

    m_first_pipe_handle = hFirst;
    m_running.store(true);
    m_accept_thread = std::thread(&IpcServer::AcceptLoop, this);
    return Result<void>::Ok();
#else
    ::unlink(m_socket_path.c_str());
    m_listen_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_listen_fd < 0) {
        return Result<void>::Err("ERR_IPC_SOCKET", "Failed to create UNIX domain socket");
    }

    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, m_socket_path.c_str(), sizeof(addr.sun_path) - 1);

    if (::bind(m_listen_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) != 0) {
        ::close(m_listen_fd);
        m_listen_fd = -1;
        return Result<void>::Err("ERR_IPC_BIND", "Failed to bind IPC socket: " + m_socket_path);
    }

    ::chmod(m_socket_path.c_str(), 0600);

    if (::listen(m_listen_fd, 16) != 0) {
        ::close(m_listen_fd);
        m_listen_fd = -1;
        return Result<void>::Err("ERR_IPC_LISTEN", "Failed to listen on IPC socket");
    }

    m_running.store(true);
    m_accept_thread = std::thread(&IpcServer::AcceptLoop, this);
    return Result<void>::Ok();
#endif
}

void IpcServer::Stop() {
    if (!m_running.exchange(false)) return;
#ifdef _WIN32
    if (m_first_pipe_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(m_first_pipe_handle);
        m_first_pipe_handle = INVALID_HANDLE_VALUE;
    }
    if (m_accept_thread.joinable()) {
        CancelSynchronousIo(m_accept_thread.native_handle());
        std::wstring wpipe(m_socket_path.begin(), m_socket_path.end());
        HANDLE hDummy = CreateFileW(wpipe.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hDummy != INVALID_HANDLE_VALUE) {
            CloseHandle(hDummy);
        }
        CancelSynchronousIo(m_accept_thread.native_handle());
        m_accept_thread.join();
    }

    {
        std::lock_guard<std::mutex> lk(m_clients_mutex);
        for (HANDLE h : m_active_pipe_handles) {
            CancelIoEx(h, nullptr);
            DisconnectNamedPipe(h);
        }
    }

    for (auto& entry : m_client_threads) {
        if (entry.thread.joinable()) {
            entry.thread.join();
        }
    }
    m_client_threads.clear();
    m_active_pipe_handles.clear();
#else
    if (m_listen_fd >= 0) {
        ::shutdown(m_listen_fd, SHUT_RDWR);
        ::close(m_listen_fd);
        m_listen_fd = -1;
    }
    if (m_accept_thread.joinable()) {
        m_accept_thread.join();
    }
    ::unlink(m_socket_path.c_str());

    {
        std::lock_guard<std::mutex> lk(m_clients_mutex);
        for (int fd : m_active_client_fds) {
            ::shutdown(fd, SHUT_RDWR);
            ::close(fd);
        }
    }

    for (auto& entry : m_client_threads) {
        if (entry.thread.joinable()) {
            entry.thread.join();
        }
    }
    m_client_threads.clear();
    m_active_client_fds.clear();
#endif
}

void IpcServer::AcceptLoop() {
#ifdef _WIN32
    std::wstring wpipe(m_socket_path.begin(), m_socket_path.end());

    PSECURITY_DESCRIPTOR pSD = nullptr;
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = FALSE;

    std::string user_sid = GetCurrentUserSidString();
    std::wstring sddl;
    if (!user_sid.empty()) {
        std::wstring wSid(user_sid.begin(), user_sid.end());
        sddl = L"D:(A;;GA;;;" + wSid + L")(A;;GA;;;BA)";
    } else {
        sddl = L"D:(A;;GA;;;OW)(A;;GA;;;BA)";
    }
    if (ConvertStringSecurityDescriptorToSecurityDescriptorW(
            sddl.c_str(), SDDL_REVISION_1, &pSD, nullptr)) {
        sa.lpSecurityDescriptor = pSD;
    }

    HANDLE hPipe = m_first_pipe_handle;
    m_first_pipe_handle = INVALID_HANDLE_VALUE;

    while (m_running.load()) {
        if (hPipe == INVALID_HANDLE_VALUE) {
            hPipe = CreateNamedPipeW(
                wpipe.c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
                PIPE_UNLIMITED_INSTANCES,
                65536,
                65536,
                50,
                sa.lpSecurityDescriptor ? &sa : nullptr);
            if (hPipe == INVALID_HANDLE_VALUE) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }
        }

        BOOL connected = ConnectNamedPipe(hPipe, nullptr) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        if (!connected || !m_running.load()) {
            CloseHandle(hPipe);
            hPipe = INVALID_HANDLE_VALUE;
            continue;
        }

        {
            std::lock_guard<std::mutex> lk(m_clients_mutex);
            m_active_pipe_handles.push_back(hPipe);
        }

        HANDLE clientPipe = hPipe;
        hPipe = INVALID_HANDLE_VALUE;

        auto done_flag = std::make_shared<std::atomic<bool>>(false);
        std::thread t([this, clientPipe, done_flag]() {
            while (m_running.load()) {
                uint32_t msg_len = 0;
                DWORD got = 0;
                if (!ReadFile(clientPipe, &msg_len, sizeof(msg_len), &got, nullptr) || got != sizeof(msg_len)) break;
                if (msg_len == 0 || msg_len > 1048576) break;
                std::string payload(msg_len, '\0');
                DWORD total_read = 0;
                while (total_read < msg_len) {
                    DWORD chunk = 0;
                    if (!ReadFile(clientPipe, payload.data() + total_read, msg_len - total_read, &chunk, nullptr) || chunk == 0) break;
                    total_read += chunk;
                }
                if (total_read != msg_len) break;
                std::string resp = HandleJsonRpcMessage(payload);
                uint32_t resp_len = static_cast<uint32_t>(resp.size());
                DWORD written = 0;
                if (!WriteFile(clientPipe, &resp_len, sizeof(resp_len), &written, nullptr)) break;
                if (!WriteFile(clientPipe, resp.data(), resp_len, &written, nullptr)) break;
                FlushFileBuffers(clientPipe);
            }
            DisconnectNamedPipe(clientPipe);
            CloseHandle(clientPipe);

            {
                std::lock_guard<std::mutex> lk(m_clients_mutex);
                auto it = std::find(m_active_pipe_handles.begin(), m_active_pipe_handles.end(), clientPipe);
                if (it != m_active_pipe_handles.end()) {
                    m_active_pipe_handles.erase(it);
                }
            }
            done_flag->store(true);
        });

        {
            std::lock_guard<std::mutex> lk(m_clients_mutex);
            for (auto it = m_client_threads.begin(); it != m_client_threads.end();) {
                if (it->done->load() && it->thread.joinable()) {
                    it->thread.join();
                    it = m_client_threads.erase(it);
                } else {
                    ++it;
                }
            }
            m_client_threads.push_back({std::move(t), done_flag});
        }
    }
    if (pSD) {
        LocalFree(pSD);
        pSD = nullptr;
    }
#else
    while (m_running.load()) {
        struct pollfd pfd{};
        pfd.fd = m_listen_fd;
        pfd.events = POLLIN;
        int pr = ::poll(&pfd, 1, 50);
        if (pr <= 0 || !(pfd.revents & POLLIN)) {
            continue;
        }
        int client_fd = ::accept(m_listen_fd, nullptr, nullptr);
        if (client_fd < 0) {
            if (!m_running.load()) break;
            continue;
        }

        {
            std::lock_guard<std::mutex> lk(m_clients_mutex);
            m_active_client_fds.push_back(client_fd);
        }

        auto done_flag = std::make_shared<std::atomic<bool>>(false);
        std::thread t([this, client_fd, done_flag]() {
            HandleClientConnection(client_fd);
            done_flag->store(true);
        });
        {
            std::lock_guard<std::mutex> lk(m_clients_mutex);
            for (auto it = m_client_threads.begin(); it != m_client_threads.end();) {
                if (it->done->load() && it->thread.joinable()) {
                    it->thread.join();
                    it = m_client_threads.erase(it);
                } else {
                    ++it;
                }
            }
            m_client_threads.push_back({std::move(t), done_flag});
        }
    }
#endif
}

void IpcServer::HandleClientConnection(int client_fd) {
#ifndef _WIN32
    while (m_running.load()) {
        uint32_t msg_len = 0;
        ssize_t n = ::recv(client_fd, &msg_len, sizeof(msg_len), MSG_WAITALL);
        if (n != sizeof(msg_len)) break;
        if (msg_len == 0 || msg_len > 1048576) {
            OPENIDM_LOG_WARN("IpcServer", "Rejected frame exceeding 1 MiB limit");
            break;
        }
        std::string payload(msg_len, '\0');
        ssize_t got = ::recv(client_fd, payload.data(), msg_len, MSG_WAITALL);
        if (got != static_cast<ssize_t>(msg_len)) break;

        std::string resp = HandleJsonRpcMessage(payload);
        uint32_t resp_len = static_cast<uint32_t>(resp.size());
#ifdef MSG_NOSIGNAL
        int send_flags = MSG_NOSIGNAL;
#else
        int send_flags = 0;
#endif
        if (::send(client_fd, &resp_len, sizeof(resp_len), send_flags) != sizeof(resp_len)) break;
        if (::send(client_fd, resp.data(), resp.size(), send_flags) != static_cast<ssize_t>(resp.size())) break;
    }
    ::close(client_fd);

    {
        std::lock_guard<std::mutex> lk(m_clients_mutex);
        auto it = std::find(m_active_client_fds.begin(), m_active_client_fds.end(), client_fd);
        if (it != m_active_client_fds.end()) {
            m_active_client_fds.erase(it);
        }
    }
#else
    (void)client_fd;
#endif
}

std::string IpcServer::HandleJsonRpcMessage(std::string_view raw_json) {
    if (raw_json.empty() || raw_json.size() > 1048576) {
        return MakeRpcError(nullptr, -32600, "Message size out of bounds").dump();
    }

    json::Value req;
    try {
        req = json::Parse(raw_json);
    } catch (const std::exception& e) {
        return MakeRpcError(nullptr, -32700, std::string("Parse error: ") + e.what()).dump();
    }

    if (!req.is_object() || req["jsonrpc"].as_string() != "2.0") {
        return MakeRpcError(req["id"], -32600, "Invalid JSON-RPC 2.0 envelope").dump();
    }

    json::Value id = req["id"];
    std::string method = req["method"].as_string();
    const json::Value& params = req["params"];

    // 1. system.handshake
    if (method == "system.handshake") {
        std::string proto_ver = params["protocolVersion"].as_string("1.0");
        if (proto_ver.rfind("1.", 0) != 0) {
            return MakeRpcError(id, -32001, "ERR_IPC_VERSION_MISMATCH: Expected protocol 1.x, got " + proto_ver).dump();
        }
        auto s = m_controller.GetSettings();
        json::Value res = json::Object{};
        res["appVersion"] = "0.3.0";
        res["protocolVersion"] = "1.0";
        res["captureEnabled"] = m_capture_enabled.load();
        res["workersAvailable"] = m_controller.AreWorkersAvailable();
        json::Array exts = {"zip", "rar", "7z", "iso", "exe", "msi", "mp4", "mkv", "webm", "mp3", "flac", "pdf", "tar", "gz", "bin"};
        res["interceptExtensions"] = std::move(exts);
        res["minCaptureSizeBytes"] = static_cast<int64_t>(s.min_capture_size_bytes);
        return MakeRpcResult(id, std::move(res)).dump();
    }

    // 2. download.offer
    if (method == "download.offer") {
        if (!m_capture_enabled.load() || params["bypassModifier"].as_bool(false)) {
            json::Value res = json::Object{};
            res["accepted"] = false;
            res["reason"] = "ERR_IPC_CAPTURE_DISABLED";
            return MakeRpcResult(id, std::move(res)).dump();
        }

        std::string url = params["url"].as_string();
        if (url.rfind("blob:", 0) == 0 || url.rfind("data:", 0) == 0) {
            json::Value res = json::Object{};
            res["accepted"] = false;
            res["reason"] = "BLOB_OR_DATA_URL_MUST_STAY_IN_BROWSER";
            return MakeRpcResult(id, std::move(res)).dump();
        }

        TaskModel t;
        t.task_type = TaskType::kHttpFile;
        t.url = url;
        t.page_url = params["tabUrl"].as_string();
        t.correlation_id = params["correlationId"].as_string();
        t.file_name = PathSanitizer::SanitizeFileName(params["filename"].as_string());
        if (params.contains("saveDir")) {
            std::string sd = params["saveDir"].as_string();
            if (sd.find("..") != std::string::npos) {
                json::Value res = json::Object{};
                res["accepted"] = false;
                res["reason"] = "ERR_PATH_TRAVERSAL_REJECTED";
                return MakeRpcResult(id, std::move(res)).dump();
            }
            t.save_dir = sd;
        }
        ExtractHeadersMap(params["headers"], t.headers, t.auth_headers);

        bool auto_start = m_controller.GetSettings().auto_start_downloads;
        auto add_res = m_controller.AddTask(std::move(t), auto_start, false);
        if (!add_res.ok()) {
            json::Value res = json::Object{};
            res["accepted"] = false;
            res["reason"] = add_res.error->code + ": " + add_res.error->message;
            return MakeRpcResult(id, std::move(res)).dump();
        }
        json::Value res = json::Object{};
        res["accepted"] = true;
        res["taskId"] = add_res.value->task_id;
        res["fileName"] = add_res.value->file_name;
        return MakeRpcResult(id, std::move(res)).dump();
    }

    // 3. download.batchAdd
    if (method == "download.batchAdd") {
        std::vector<std::string> urls;
        for (const auto& u : params["urls"].as_array()) {
            std::string us = u.as_string();
            if (!us.empty()) urls.push_back(us);
        }
        std::map<std::string, std::string> hdrs, auth_hdrs;
        ExtractHeadersMap(params["headers"], hdrs, auth_hdrs);
        auto batch_res = m_controller.AddBatchTasks(urls, params["tabUrl"].as_string(), hdrs, auth_hdrs);
        json::Value res = json::Object{};
        json::Array task_ids;
        if (batch_res.ok()) {
            for (const auto& t : *batch_res.value) {
                task_ids.push_back(t.task_id);
            }
        }
        res["addedCount"] = static_cast<int64_t>(task_ids.size());
        res["taskIds"] = std::move(task_ids);
        return MakeRpcResult(id, std::move(res)).dump();
    }

    // 4. stream.probe
    if (method == "stream.probe") {
        std::string manifest_url = params["manifestUrl"].as_string();
        std::string cached_body = params["cachedManifestBody"].as_string();
        std::map<std::string, std::string> hdrs, auth_hdrs;
        ExtractHeadersMap(params["headers"], hdrs, auth_hdrs);

        auto probe_res = m_controller.ProbeStream(manifest_url, hdrs, auth_hdrs, cached_body);
        if (!probe_res.ok()) {
            return MakeRpcError(id, -32004, probe_res.error->code + ": " + probe_res.error->message).dump();
        }
        json::Value res = json::Object{};
        res["manifestUrl"] = probe_res.value->manifest_url;
        res["isLive"] = probe_res.value->is_live;
        json::Array tracks_arr;
        for (const auto& tr : probe_res.value->tracks) {
            json::Value tobj = json::Object{};
            tobj["id"] = tr.id;
            tobj["mediaType"] = tr.media_type;
            tobj["groupId"] = tr.group_id;
            tobj["language"] = tr.language;
            tobj["name"] = tr.name;
            tobj["codecs"] = tr.codecs;
            tobj["resolution"] = tr.resolution;
            tobj["bandwidth"] = tr.bandwidth;
            tobj["frameRate"] = tr.frame_rate;
            tobj["segmentsCount"] = tr.segments_count;
            tobj["durationSec"] = tr.duration_sec;
            tobj["encrypted"] = tr.encrypted;
            tobj["encryptionMethod"] = tr.encryption_method;
            tracks_arr.push_back(std::move(tobj));
        }
        res["tracks"] = std::move(tracks_arr);
        return MakeRpcResult(id, std::move(res)).dump();
    }

    // 5. stream.add
    if (method == "stream.add") {
        std::string stream_type = params["streamType"].as_string("HLS");
        std::string source_type = params["sourceType"].as_string();
        if (source_type == "YOUTUBE_EXTRACTION") {
            stream_type = "YOUTUBE";
        }
        std::string manifest_url = params["manifestUrl"].as_string();
        if (manifest_url.empty()) manifest_url = params["url"].as_string();
        std::string cached_body = params["cachedManifestBody"].as_string();
        if (cached_body.empty() && params.contains("extractorJson") && !params["extractorJson"].as_string().empty()) {
            cached_body = params["extractorJson"].as_string();
        }
        if (cached_body.empty() && (params.contains("videoUrl") || params.contains("pairedAudioUrl"))) {
            std::string vu = params["videoUrl"].as_string();
            std::string au = params["pairedAudioUrl"].as_string();
            if (!vu.empty() || !au.empty()) {
                json::Object jdesc;
                jdesc["fluxType"] = "YOUTUBE_STREAM";
                jdesc["videoUrl"] = vu;
                jdesc["audioUrl"] = au;
                jdesc["watchUrl"] = params["tabUrl"].as_string();
                jdesc["videoId"] = params["videoId"].as_string();
                cached_body = json::Value(std::move(jdesc)).dump();
            }
        }

        if (stream_type == "HLS" && !cached_body.empty()) {
            std::string_view trimmed = cached_body;
            while (!trimmed.empty() && (trimmed.front() == ' ' || trimmed.front() == '\t' || trimmed.front() == '\r' || trimmed.front() == '\n')) {
                trimmed.remove_prefix(1);
            }
            if (!trimmed.starts_with("#EXTM3U")) {
                return MakeRpcError(id, -32602, "ERR_STREAM_INVALID_MANIFEST: Manifest body does not begin with #EXTM3U").dump();
            }
        } else if (stream_type == "DASH" && !cached_body.empty()) {
            std::string_view trimmed = cached_body;
            while (!trimmed.empty() && (trimmed.front() == ' ' || trimmed.front() == '\t' || trimmed.front() == '\r' || trimmed.front() == '\n')) {
                trimmed.remove_prefix(1);
            }
            if (trimmed.find("<MPD") == std::string_view::npos && trimmed.find("<mpd") == std::string_view::npos) {
                return MakeRpcError(id, -32602, "ERR_STREAM_INVALID_MANIFEST: Manifest body does not contain <MPD> root tag").dump();
            }
        }

        TaskModel t;
        if (stream_type == "DASH" || manifest_url.find(".mpd") != std::string::npos) {
            t.task_type = TaskType::kStreamDash;
        } else if (stream_type == "YOUTUBE" || stream_type == "SITE_EXTRACTION" || stream_type == "VIMEO" ||
                   stream_type == "HLS" || manifest_url.find(".m3u8") != std::string::npos || !cached_body.empty()) {
            t.task_type = TaskType::kStreamHls;
        } else {
            t.task_type = TaskType::kHttpFile;
        }
        t.url = manifest_url;
        t.page_url = params["tabUrl"].as_string();

        std::string title = params["pageTitle"].as_string();
        auto yt_suffix = title.rfind(" - YouTube");
        if (yt_suffix != std::string::npos && yt_suffix + 10 == title.size()) {
            title = title.substr(0, yt_suffix);
        }
        std::string out_fmt = params["outputFormat"].as_string("mp4");
        if (!title.empty()) {
            std::string sanitized_title = PathSanitizer::SanitizeFileName(title);
            std::string dot_ext = "." + out_fmt;
            if (sanitized_title.size() > dot_ext.size() &&
                sanitized_title.substr(sanitized_title.size() - dot_ext.size()) == dot_ext) {
                t.file_name = sanitized_title;
            } else {
                t.file_name = sanitized_title + dot_ext;
            }
        }
        ExtractHeadersMap(params["headers"], t.headers, t.auth_headers);

        t.stream_options.video_filter = params["videoFilter"].as_string("best");
        t.stream_options.audio_filter = params["audioFilter"].as_string("all");
        t.stream_options.subtitle_filter = params["subtitleFilter"].as_string("all");
        t.stream_options.output_format = out_fmt;
        t.stream_options.custom_hls_key_hex = params["customKeyHex"].as_string("");
        t.stream_options.cached_manifest_body = cached_body;

        bool auto_start = m_controller.GetSettings().auto_start_downloads;
        auto add_res = m_controller.AddTask(std::move(t), auto_start, false);
        if (!add_res.ok()) {
            return MakeRpcError(id, -32602, add_res.error->code + ": " + add_res.error->message).dump();
        }
        json::Value res = json::Object{};
        res["accepted"] = true;
        res["taskId"] = add_res.value->task_id;
        res["fileName"] = add_res.value->file_name;
        res["status"] = TaskStateToString(add_res.value->state);
        res["taskType"] = params.contains("sourceType") ? params["sourceType"].as_string() : TaskTypeToString(add_res.value->task_type);
        return MakeRpcResult(id, std::move(res)).dump();
    }

    // 6. task.list
    if (method == "task.list") {
        json::Array arr;
        for (const auto& t : m_controller.ListTasks()) {
            arr.push_back(TaskToJson(t));
        }
        json::Value res = json::Object{};
        res["tasks"] = std::move(arr);
        return MakeRpcResult(id, std::move(res)).dump();
    }

    // 7. task.control (start, pause, resume, cancel, retry, remove, clearCompleted, pauseAll, resumeAll, refreshUrl)
    if (method == "task.control") {
        std::string tid = params["taskId"].as_string();
        std::string action = params["action"].as_string();
        if (action == "clearCompleted") {
            auto cr = m_controller.ClearCompletedHistory();
            json::Value res = json::Object{};
            res["ok"] = cr.ok();
            res["clearedCount"] = cr.ok() ? static_cast<int64_t>(*cr.value) : 0;
            return MakeRpcResult(id, std::move(res)).dump();
        }
        if (action == "pauseAll") {
            m_controller.PauseAllTasks();
            json::Value res = json::Object{};
            res["ok"] = true;
            return MakeRpcResult(id, std::move(res)).dump();
        }
        if (action == "resumeAll") {
            m_controller.ResumeAllTasks();
            json::Value res = json::Object{};
            res["ok"] = true;
            return MakeRpcResult(id, std::move(res)).dump();
        }

        Result<void> r = Result<void>::Err("ERR_INVALID_ACTION", "Unknown action");
        if (action == "start") r = m_controller.StartTask(tid);
        else if (action == "pause") r = m_controller.PauseTask(tid);
        else if (action == "resume") r = m_controller.ResumeTask(tid);
        else if (action == "cancel") r = m_controller.CancelTask(tid);
        else if (action == "retry") r = m_controller.RetryTask(tid);
        else if (action == "remove") r = m_controller.RemoveTask(tid, params["deleteFile"].as_bool(false));
        else if (action == "refreshUrl") {
            std::map<std::string, std::string> hdrs, auth_hdrs;
            ExtractHeadersMap(params["headers"], hdrs, auth_hdrs);
            r = m_controller.RefreshTaskUrl(tid, params["newUrl"].as_string(), hdrs, auth_hdrs);
        }
        if (!r.ok()) {
            return MakeRpcError(id, -32005, r.error->code + ": " + r.error->message).dump();
        }
        json::Value res = json::Object{};
        res["ok"] = true;
        return MakeRpcResult(id, std::move(res)).dump();
    }

    return MakeRpcError(id, -32601, "Method not found: " + method).dump();
}

} // namespace openidm::ipc
