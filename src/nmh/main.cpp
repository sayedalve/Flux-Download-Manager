// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM — Browser Native Messaging Host (openidm-nmh)
// Bridges 32-bit length-prefixed JSON messages on stdin/stdout from Chrome/Edge/Brave/Firefox
// to the OpenIDM IPC Server (Named Pipe on Windows 11, UNIX Domain Socket on POSIX),
// auto-launching openidm --daemon if the server is not yet running.
// Implements NATIVE_MESSAGING_AND_IPC.md & BUG-004 Fix.

#include "ipc/ipc_server.hpp"

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#endif

namespace {

void WriteStdoutFrame(const std::string& json_str) {
    uint32_t len = static_cast<uint32_t>(json_str.size());
    std::fwrite(&len, sizeof(len), 1, stdout);
    std::fwrite(json_str.data(), 1, json_str.size(), stdout);
    std::fflush(stdout);
}

#ifdef _WIN32
HANDLE ConnectToDesktopPipe(const std::string& pipe_path, const std::filesystem::path& daemon_bin_path) {
    std::wstring wpipe(pipe_path.begin(), pipe_path.end());
    auto try_connect = [&]() -> HANDLE {
        HANDLE h = CreateFileW(wpipe.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (h != INVALID_HANDLE_VALUE) return h;
        return INVALID_HANDLE_VALUE;
    };

    HANDLE h = try_connect();
    if (h != INVALID_HANDLE_VALUE) return h;

    if (!daemon_bin_path.empty() && std::filesystem::exists(daemon_bin_path)) {
        std::wstring cmd = L"\"" + daemon_bin_path.wstring() + L"\" --daemon --ipc-socket \"" + wpipe + L"\"";
        STARTUPINFOW si{sizeof(STARTUPINFOW)};
        PROCESS_INFORMATION pi{};
        if (CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW | DETACHED_PROCESS, nullptr, nullptr, &si, &pi)) {
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
        for (int attempt = 0; attempt < 25; ++attempt) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            h = try_connect();
            if (h != INVALID_HANDLE_VALUE) return h;
        }
    }
    return INVALID_HANDLE_VALUE;
}
#else
int ConnectToDesktopIpc(const std::string& sock_path, const std::string& daemon_bin_path) {
    auto try_connect = [&]() -> int {
        int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd < 0) return -1;
        struct sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        std::strncpy(addr.sun_path, sock_path.c_str(), sizeof(addr.sun_path) - 1);
        if (::connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == 0) {
            return fd;
        }
        ::close(fd);
        return -1;
    };

    int fd = try_connect();
    if (fd >= 0) return fd;

    if (!daemon_bin_path.empty() && std::filesystem::exists(daemon_bin_path)) {
        pid_t pid = fork();
        if (pid == 0) {
            setsid();
            execl(daemon_bin_path.c_str(), daemon_bin_path.c_str(), "--daemon", "--ipc-socket", sock_path.c_str(), nullptr);
            _exit(127);
        }
        for (int attempt = 0; attempt < 25; ++attempt) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            fd = try_connect();
            if (fd >= 0) return fd;
        }
    }
    return -1;
}
#endif

} // namespace

int main(int argc, char* argv[]) {
#ifdef _WIN32
    // Critical on Windows: prevent CRT from translating 0x0A (\n) <-> 0x0D 0x0A (\r\n) on Native Messaging stdio!
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    std::string sock_path = openidm::ipc::IpcServer::DefaultSocketPath();
    if (const char* env_flux = std::getenv("FLUX_IPC_SOCKET")) {
        sock_path = env_flux;
    } else if (const char* env_old = std::getenv("OPENIDM_IPC_SOCKET")) {
        sock_path = env_old;
    }
    std::filesystem::path daemon_bin;
    if (argc > 0 && argv[0]) {
        std::filesystem::path self_path = std::filesystem::path(argv[0]);
#ifdef _WIN32
        std::filesystem::path candidate = self_path.parent_path() / "flux.exe";
        if (!std::filesystem::exists(candidate)) {
            candidate = self_path.parent_path() / "openidm.exe";
        }
#else
        std::filesystem::path candidate = self_path.parent_path() / "flux";
        if (!std::filesystem::exists(candidate)) {
            candidate = self_path.parent_path() / "openidm";
        }
#endif
        if (std::filesystem::exists(candidate)) {
            daemon_bin = candidate;
        }
    }

#ifdef _WIN32
    HANDLE hPipe = INVALID_HANDLE_VALUE;
    while (true) {
        uint32_t in_len = 0;
        if (std::fread(&in_len, sizeof(in_len), 1, stdin) != 1) break;
        if (in_len == 0 || in_len > 1048576) {
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32600,"message":"ERR_IPC_FRAME_SIZE"}})");
            break;
        }
        std::string payload(in_len, '\0');
        if (std::fread(payload.data(), 1, in_len, stdin) != in_len) break;

        if (hPipe == INVALID_HANDLE_VALUE) {
            hPipe = ConnectToDesktopPipe(sock_path, daemon_bin);
        }
        if (hPipe == INVALID_HANDLE_VALUE) {
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32003,"message":"ERR_IPC_DESKTOP_UNAVAILABLE"}})");
            continue;
        }

        DWORD written = 0;
        if (!WriteFile(hPipe, &in_len, sizeof(in_len), &written, nullptr) ||
            !WriteFile(hPipe, payload.data(), in_len, &written, nullptr)) {
            CloseHandle(hPipe);
            hPipe = INVALID_HANDLE_VALUE;
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32003,"message":"ERR_IPC_DISCONNECTED"}})");
            continue;
        }

        uint32_t out_len = 0;
        DWORD got = 0;
        if (!ReadFile(hPipe, &out_len, sizeof(out_len), &got, nullptr) || got != sizeof(out_len) ||
            out_len == 0 || out_len > 1048576) {
            CloseHandle(hPipe);
            hPipe = INVALID_HANDLE_VALUE;
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32003,"message":"ERR_IPC_BAD_RESPONSE"}})");
            continue;
        }
        std::string resp(out_len, '\0');
        DWORD total_read = 0;
        while (total_read < out_len) {
            DWORD chunk = 0;
            if (!ReadFile(hPipe, resp.data() + total_read, out_len - total_read, &chunk, nullptr) || chunk == 0) break;
            total_read += chunk;
        }
        if (total_read != out_len) {
            CloseHandle(hPipe);
            hPipe = INVALID_HANDLE_VALUE;
            break;
        }
        WriteStdoutFrame(resp);
    }
    if (hPipe != INVALID_HANDLE_VALUE) CloseHandle(hPipe);
#else
    int ipc_fd = -1;
    while (true) {
        uint32_t in_len = 0;
        if (std::fread(&in_len, sizeof(in_len), 1, stdin) != 1) {
            break;
        }
        if (in_len == 0 || in_len > 1048576) {
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32600,"message":"ERR_IPC_FRAME_SIZE"}})");
            break;
        }
        std::string payload(in_len, '\0');
        if (std::fread(payload.data(), 1, in_len, stdin) != in_len) {
            break;
        }

        if (ipc_fd < 0) {
            ipc_fd = ConnectToDesktopIpc(sock_path, daemon_bin.string());
        }
        if (ipc_fd < 0) {
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32003,"message":"ERR_IPC_DESKTOP_UNAVAILABLE"}})");
            continue;
        }

        if (::send(ipc_fd, &in_len, sizeof(in_len), 0) != sizeof(in_len) ||
            ::send(ipc_fd, payload.data(), in_len, 0) != static_cast<ssize_t>(in_len)) {
            ::close(ipc_fd);
            ipc_fd = -1;
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32003,"message":"ERR_IPC_DISCONNECTED"}})");
            continue;
        }

        uint32_t out_len = 0;
        if (::recv(ipc_fd, &out_len, sizeof(out_len), MSG_WAITALL) != sizeof(out_len) ||
            out_len == 0 || out_len > 1048576) {
            ::close(ipc_fd);
            ipc_fd = -1;
            WriteStdoutFrame(R"({"jsonrpc":"2.0","id":null,"error":{"code":-32003,"message":"ERR_IPC_BAD_RESPONSE"}})");
            continue;
        }

        std::string resp(out_len, '\0');
        if (::recv(ipc_fd, resp.data(), out_len, MSG_WAITALL) != static_cast<ssize_t>(out_len)) {
            ::close(ipc_fd);
            ipc_fd = -1;
            break;
        }
        WriteStdoutFrame(resp);
    }

    if (ipc_fd >= 0) ::close(ipc_fd);
#endif
    return 0;
}
