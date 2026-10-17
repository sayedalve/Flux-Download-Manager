// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM — Local IPC Server & JSON-RPC 2.0 Dispatcher
// Uses Windows Named Pipe (\\.\pipe\OpenIDM_IPC_<UserSID>) on Windows 11 and
// user-ACL-restricted UNIX Domain Socket (/tmp/openidm_ipc_<uid>.sock, chmod 0600) on POSIX.
// Implements NATIVE_MESSAGING_AND_IPC.md, ADR-0003, NFR-007, NFR-008, Phase 6.
#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "core/download_controller.hpp"

namespace openidm::ipc {

class IpcServer {
public:
    IpcServer(DownloadController& controller, std::string socket_path = "");
    ~IpcServer();

    Result<void> Start();
    void Stop();

    std::string GetEndpointPath() const { return m_socket_path; }

    // Direct JSON-RPC 2.0 request handler (used both over socket/pipe and in unit tests)
    std::string HandleJsonRpcMessage(std::string_view raw_json);

    void SetCaptureEnabled(bool enabled) { m_capture_enabled.store(enabled); }
    bool IsCaptureEnabled() const { return m_capture_enabled.load(); }

    static std::string DefaultSocketPath();

private:
    void AcceptLoop();
    void HandleClientConnection(int client_fd);

    DownloadController& m_controller;
    std::string m_socket_path;
    int m_listen_fd{-1};
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_capture_enabled{true};
    std::thread m_accept_thread;

    std::mutex m_clients_mutex;
    std::vector<std::thread> m_client_threads;
#ifdef _WIN32
    HANDLE m_first_pipe_handle{INVALID_HANDLE_VALUE};
    std::vector<HANDLE> m_active_pipe_handles;
#else
    std::vector<int> m_active_client_fds;
#endif
};

} // namespace openidm::ipc
