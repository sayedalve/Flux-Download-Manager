// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM — Main Desktop GUI, CLI & Daemon Application Entry Point (openidm)
// Default launch (no arguments):
//   Starts the background IPC server and launches the OpenIDM Desktop GUI application window.
// CLI flags:
//   --version                    Print version info
//   --gui-snapshot               Render full desktop UI snapshot (sidebar counts, table, details, status bar) and exit
//   --list --db <state.db>       Display the unified task queue table
//   --download <url> --out <dir> Download an ordinary HTTP/HTTPS file using Brisk engine
//   --stream <url> --out <dir>   Download & mux an HLS/DASH stream using N_m3u8DL-RE + FFmpeg
//   --probe-stream <url>         Probe an HLS/DASH manifest and print JSON tracks
//   --daemon --ipc-socket <path> Run background daemon with IPC Server listening for Browser Extension

#include "common/json_mini.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"
#include "core/download_controller.hpp"
#include "ipc/ipc_server.hpp"
#include "ui/win32_main_window.hpp"

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <fcntl.h>
#include <io.h>
#elif defined(__GLIBC__)
#include <malloc.h>
#endif

namespace {
std::atomic<bool> g_terminate_daemon{false};
void SignalHandler(int) { g_terminate_daemon.store(true); }
} // namespace

int main(int argc, char* argv[]) {
#if defined(__GLIBC__)
    // Cap glibc malloc arenas to 2 so 8-connection worker threads do not allocate 8 x 64 MiB virtual/resident arenas
    mallopt(M_ARENA_MAX, 2);
#endif
    std::filesystem::path exe_dir = openidm::PathSanitizer::GetExecutableDir(argc > 0 ? argv[0] : nullptr);
    std::filesystem::path app_data_dir = openidm::PathSanitizer::GetDefaultAppDataDir();

    openidm::ControllerConfig cfg;
    cfg.state_db_path = app_data_dir / "state.db";
    cfg.default_download_dir = openidm::PathSanitizer::GetDefaultDownloadsDir(exe_dir);
    cfg.temp_root_dir = app_data_dir / "tmp";
    cfg.nm3u8dl_binary_path = openidm::PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", exe_dir);
    cfg.ffmpeg_binary_path = openidm::PathSanitizer::FindWorkerBinary("ffmpeg", exe_dir);
    cfg.allow_loopback = true;

    std::string mode;
    std::string target_url;
    std::string ipc_socket_path = openidm::ipc::IpcServer::DefaultSocketPath();
    uint32_t connections = 8;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--version" || arg == "-v") {
            std::cout << "OpenIDM v0.2.0 (Brisk Engine + Cat Catch Integration + N_m3u8DL-RE Stream Pipeline)\n";
            return 0;
        } else if (arg == "--db" && i + 1 < argc) {
            cfg.state_db_path = argv[++i];
        } else if (arg == "--out" && i + 1 < argc) {
            cfg.default_download_dir = argv[++i];
        } else if (arg == "--connections" && i + 1 < argc) {
            connections = static_cast<uint32_t>(std::stoul(argv[++i]));
        } else if (arg == "--download" && i + 1 < argc) {
            mode = "download";
            target_url = argv[++i];
        } else if (arg == "--stream" && i + 1 < argc) {
            mode = "stream";
            target_url = argv[++i];
        } else if (arg == "--probe-stream" && i + 1 < argc) {
            mode = "probe";
            target_url = argv[++i];
        } else if (arg == "--list") {
            mode = "list";
        } else if (arg == "--gui-snapshot") {
            mode = "gui-snapshot";
        } else if (arg == "--daemon") {
            mode = "daemon";
        } else if (arg == "--ipc-socket" && i + 1 < argc) {
            ipc_socket_path = argv[++i];
        } else if (arg == "--worker-binary" && i + 1 < argc) {
            cfg.nm3u8dl_binary_path = argv[++i];
        } else if (arg == "--ffmpeg-binary" && i + 1 < argc) {
            cfg.ffmpeg_binary_path = argv[++i];
        }
    }

    openidm::Logger::Instance().SetLogFile(cfg.state_db_path.parent_path() / "openidm.log");
    openidm::DownloadController controller(cfg);
    auto init_res = controller.Initialize();
    if (!init_res.ok()) {
        std::cerr << "ERROR: Failed to initialize OpenIDM controller: " << init_res.error->message << "\n";
        return 1;
    }

    // Default launch path (no CLI flags): Launch the Desktop GUI Application + IPC Server (BUG-002 Fix)
    if (mode.empty()) {
        openidm::ipc::IpcServer server(controller, ipc_socket_path);
        server.Start();
        openidm::ui::DesktopView view(controller, &server);
        int rc = view.RunApplicationLoop(false);
        server.Stop();
        controller.Shutdown();
        return rc;
    }

    if (mode == "list") {
        openidm::ui::DesktopView view(controller);
        std::cout << view.RenderQueueTableText();
        controller.Shutdown();
        return 0;
    }

    if (mode == "gui-snapshot") {
        openidm::ipc::IpcServer server(controller, ipc_socket_path);
        server.Start();
        openidm::ui::DesktopView view(controller, &server);
        view.SelectFirstRow();
        std::cout << view.RenderFullDesktopSnapshotText();
        server.Stop();
        controller.Shutdown();
        return 0;
    }

    if (mode == "probe") {
        auto pres = controller.ProbeStream(target_url, {}, {});
        if (!pres.ok()) {
            std::cerr << "ERROR: " << pres.error->code << ": " << pres.error->message << "\n";
            controller.Shutdown();
            return 2;
        }
        std::cout << "Probed " << pres.value->tracks.size() << " tracks from " << pres.value->manifest_url << "\n";
        for (const auto& tr : pres.value->tracks) {
            std::cout << "  [" << tr.media_type << "] id=" << tr.id
                      << " res=" << tr.resolution << " bw=" << tr.bandwidth
                      << " lang=" << tr.language << " segs=" << tr.segments_count << "\n";
        }
        controller.Shutdown();
        return 0;
    }

    if (mode == "download" || mode == "stream") {
        openidm::TaskModel t;
        t.url = target_url;
        t.max_connections = connections;
        t.save_dir = cfg.default_download_dir.string();
        if (mode == "stream") {
            t.task_type = (target_url.find(".mpd") != std::string::npos)
                              ? openidm::TaskType::kStreamDash
                              : openidm::TaskType::kStreamHls;
        } else {
            t.task_type = openidm::TaskType::kHttpFile;
        }
        auto added = controller.AddTask(std::move(t), true, true);
        if (!added.ok()) {
            std::cerr << "ERROR: " << added.error->message << "\n";
            controller.Shutdown();
            return 1;
        }
        auto done = controller.WaitForTaskCompletion(added.value->task_id, 60000);
        controller.Shutdown();
        if (!done.ok() || done.value->state != openidm::TaskState::kCompleted) {
            std::cerr << "ERROR: Task failed: " << (done.ok() ? done.value->error_message : done.error->message) << "\n";
            return 2;
        }
        std::cout << "COMPLETED: " << (std::filesystem::path(done.value->save_dir) / done.value->file_name).string()
                  << " (" << done.value->downloaded_bytes << " bytes)\n";
        return 0;
    }

    if (mode == "daemon") {
        std::signal(SIGINT, SignalHandler);
        std::signal(SIGTERM, SignalHandler);
        openidm::ipc::IpcServer server(controller, ipc_socket_path);
        auto sres = server.Start();
        if (!sres.ok()) {
            std::cerr << "ERROR: Failed to start IPC server: " << sres.error->message << "\n";
            controller.Shutdown();
            return 1;
        }
        while (!g_terminate_daemon.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        server.Stop();
        controller.Shutdown();
        return 0;
    }

    controller.Shutdown();
    return 0;
}

#ifdef _WIN32
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    int argc = 0;
    LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &argc);

    // If stdout/stderr was redirected to a pipe or file by caller, connect CRT stdout to it
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD outType = (hOut && hOut != INVALID_HANDLE_VALUE) ? GetFileType(hOut) : FILE_TYPE_UNKNOWN;
    if (outType == FILE_TYPE_PIPE || outType == FILE_TYPE_DISK) {
        int fd_out = _open_osfhandle(reinterpret_cast<intptr_t>(hOut), _O_TEXT);
        if (fd_out >= 0) {
            _dup2(fd_out, _fileno(stdout));
            setvbuf(stdout, nullptr, _IONBF, 0);
        }
    } else if (argc > 1 && AttachConsole(ATTACH_PARENT_PROCESS)) {
        FILE* fp = nullptr;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
    }

    HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
    DWORD errType = (hErr && hErr != INVALID_HANDLE_VALUE) ? GetFileType(hErr) : FILE_TYPE_UNKNOWN;
    if (errType == FILE_TYPE_PIPE || errType == FILE_TYPE_DISK) {
        int fd_err = _open_osfhandle(reinterpret_cast<intptr_t>(hErr), _O_TEXT);
        if (fd_err >= 0) {
            _dup2(fd_err, _fileno(stderr));
            setvbuf(stderr, nullptr, _IONBF, 0);
        }
    }

    std::ios::sync_with_stdio(true);
    std::vector<std::string> utf8_args;
    std::vector<char*> argv_ptrs;
    for (int i = 0; i < argc; ++i) {
        int len = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, nullptr, 0, nullptr, nullptr);
        std::string s(std::max(0, len - 1), '\0');
        if (len > 1) {
            WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, s.data(), len, nullptr, nullptr);
        }
        utf8_args.push_back(std::move(s));
    }
    if (wargv) LocalFree(wargv);
    for (auto& s : utf8_args) {
        argv_ptrs.push_back(s.data());
    }
    return main(static_cast<int>(argv_ptrs.size()), argv_ptrs.data());
}
#endif
