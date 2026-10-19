# System Architecture Specification

**Document Status:** Proposed Architecture  
**Last Reviewed:** 2026-10-16  
**Scope:** End-to-end logical and physical architecture, process boundaries, component responsibilities, and module interfaces for OpenIDM.  
**Related Documents:** [TECHNOLOGY_STACK.md](TECHNOLOGY_STACK.md), [DOWNLOAD_ENGINE_DESIGN.md](DOWNLOAD_ENGINE_DESIGN.md), [STREAM_DOWNLOAD_PIPELINE.md](STREAM_DOWNLOAD_PIPELINE.md), [BROWSER_EXTENSION.md](BROWSER_EXTENSION.md), [NATIVE_MESSAGING_AND_IPC.md](NATIVE_MESSAGING_AND_IPC.md), [adr/0001-desktop-framework.md](adr/0001-desktop-framework.md), [adr/0002-process-and-module-boundaries.md](adr/0002-process-and-module-boundaries.md)

---

## 1. High-Level System Overview

OpenIDM is structured as a multi-process Windows system divided across four trust and execution zones:
1. **Browser Zone (Untrusted Web Content & Semi-Trusted Extension Context):** Content scripts (`ISOLATED` world Shadow DOM overlay) and the extension background Service Worker (`webRequest` passive observer and download interceptor).
2. **IPC Bridge Zone (`openidm-nmh.exe`):** A tiny, stateless Native Messaging Host executable spawned by the browser that translates length-prefixed `stdio` JSON messages into Named Pipe RPC calls to the desktop process.
3. **Core Desktop Process (`openidm.exe`):** The native C++20 Win32 application hosting the UI, the central `DownloadController`, the in-process `HttpDownloadEngine` (`WinHTTP` + IOCP), the `StreamEngineAdapter`, and the embedded SQLite (`WAL`) + DPAPI persistence layer.
4. **Sandboxed Media Worker Zone (`N_m3u8DL-RE.exe` & `ffmpeg.exe`):** Out-of-process command-line workers spawned inside a Windows Job Object (`JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`) to probe, download, decrypt (standard HLS AES-128 only), and mux adaptive streaming media.

---

## 2. System Component Diagram

```mermaid
flowchart TB
    subgraph Browser["Web Browser (Chrome / Edge / Brave / Firefox)"]
        Page["Webpage DOM & <video> Elements<br/>(Untrusted Host Context)"]
        CS["Content Script + Closed Shadow DOM<br/>(Video Overlay Button & DOM Observer)"]
        SW["Extension Background Service Worker<br/>(webRequest Sniffer, Download Interceptor,<br/>Manifest Deduplicator)"]
        Popup["Extension Popup UI<br/>(Detected Streams & Site Toggle)"]
        
        Page -->|DOM Events / Layout| CS
        CS <-->|chrome.runtime.sendMessage| SW
        Popup <-->|chrome.runtime.sendMessage| SW
    end

    subgraph NMH["Native Messaging Bridge Process"]
        NMHBin["openidm-nmh.exe<br/>(Stateless stdio <-> Named Pipe Proxy)"]
    end

    subgraph Desktop["OpenIDM Desktop Process (openidm.exe — Native C++20 / Win32)"]
        IPCServer["Named Pipe IPC Server<br/>(\\.\pipe\OpenIDM_IPC_<UserSID>)"]
        UI["Win32 Native UI Layer<br/>(Virtual ListView, Track Selector,<br/>Progress Dialogs, System Tray)"]
        Controller["DownloadController & QueueScheduler<br/>(Task State Machine, Rate Limiter)"]
        HttpEngine["HttpDownloadEngine (In-Process)<br/>(WinHTTP + IOCP, Dynamic Spawn,<br/>Work-Stealing Byte-Range Reuse)"]
        StreamAdapter["StreamEngineAdapter<br/>(Process Spawner, Job Object,<br/>meta.json & Progress Parser)"]
        Storage["Storage & Security Manager<br/>(SQLite3 WAL + Windows DPAPI +<br/>Redacted Ring-Buffer Logger)"]
        Updater["Signed Update Verifier<br/>(WinHTTP + Ed25519 + Authenticode)"]

        IPCServer <--> Controller
        UI <--> Controller
        Controller <--> HttpEngine
        Controller <--> StreamAdapter
        Controller <--> Storage
        HttpEngine --> Storage
        StreamAdapter --> Storage
        UI --> Updater
    end

    subgraph Workers["Windows Job Object (Kill-on-Close Worker Subprocesses)"]
        NM3U8["N_m3u8DL-RE.exe<br/>(HLS / DASH / MSS Probe & Segment Downloader)"]
        FFmpeg["ffmpeg.exe<br/>(Container Muxer: TS/fMP4 -> MP4/MKV)"]
        NM3U8 -->|Invokes via --ffmpeg-binary-path| FFmpeg
    end

    subgraph Disk["Local Filesystem (%LOCALAPPDATA%\OpenIDM & User Downloads)"]
        SQLiteDB[("state.db (SQLite WAL)<br/>tasks, segments, queues, settings")]
        TempDir[("Restricted Temp Dir (ACL 0700)<br/>@headers.rsp, meta.json, segments")]
        PartFiles[("<filename>.openidm.part<br/>Pre-allocated Sparse/Ranged File")]
        FinalFiles[("Final Verified Output<br/>(.mp4, .mkv, .zip, .iso)")]
    end

    SW <-->|Native Messaging (stdio 32-bit len + JSON)| NMHBin
    NMHBin <-->|Named Pipe JSON-RPC 2.0 (User SID DACL)| IPCServer
    StreamAdapter -->|CreateProcessW + Job Object + Anonymous Pipes| NM3U8
    Storage <--> SQLiteDB
    StreamAdapter --> TempDir
    NM3U8 <--> TempDir
    HttpEngine --> PartFiles
    PartFiles -->|Atomic Rename (SetFileInformationByHandle)| FinalFiles
    NM3U8 -->|Muxed Output + Atomic Move| FinalFiles
```

---

## 3. Component Responsibilities, Ownership, and Process Boundaries

| Component | Process / Binary | Primary Responsibilities | Ownership Boundaries (What It Must NOT Do) |
| :--- | :--- | :--- | :--- |
| **Content Script & Overlay (`extension/src/content/`)** | Browser Renderer Process | Observe `<video>` elements via `MutationObserver` and `IntersectionObserver`; render a closed Shadow DOM "Download Video" button; notify Service Worker of active player dimensions and frame context. | Must **never** fetch manifests directly, parse M3U8/MPD content, store cookies, or communicate directly with `openidm-nmh.exe`. |
| **Extension Service Worker (`extension/src/background/`)** | Browser Extension Service Worker | Intercept file downloads (`chrome.downloads`); observe `webRequest` headers to detect progressive media and `.m3u8`/`.mpd`/`.ism` manifests; correlate `Referer`/`Cookie`/`User-Agent` headers by `requestId`; deduplicate master vs. variant manifests; maintain Native Messaging port to `openidm-nmh.exe`. | Must **never** buffer media segments in browser RAM, run `ffmpeg.wasm`, or construct raw CLI flag strings for local executables. |
| **Native Messaging Host (`openidm-nmh.exe`)** | Separate Win32 Console Process (spawned by Browser) | Read 32-bit length-prefixed JSON frames from `stdin`; validate frame size (`<= 1 MiB`) and basic JSON syntax; connect to `\\.\pipe\OpenIDM_IPC_<UserSID>` (auto-starting `openidm.exe --minimized` if pipe is absent); forward requests and relay responses to `stdout`. | Stateless proxy only. Must **never** access `state.db`, spawn download workers, or open network connections. |
| **Named Pipe IPC Server (`src/ipc/`)** | `openidm.exe` (Background I/O Thread) | Create and listen on `\\.\pipe\OpenIDM_IPC_<UserSID>` with a strict per-user DACL and `FILE_FLAG_FIRST_PIPE_INSTANCE`; verify client process identity (`GetNamedPipeClientProcessId`); parse JSON-RPC 2.0 payloads; dispatch validated commands to `DownloadController`. | Must **never** trust file paths or shell flags from IPC messages; passes only strongly typed structs (`AddDownloadRequest`, `MediaCandidateList`) to `DownloadController`. |
| **Download Controller & Queue Scheduler (`src/core/`)** | `openidm.exe` (Core Strand / Worker Thread) | Own the canonical task state machine (`Queued`, `Probing`, `Downloading`, `Muxing`, `Paused`, `Completed`, `Failed`, `Canceled`); enforce queue concurrency limits and token-bucket bandwidth throttling; route tasks to `IFileDownloadEngine` or `IStreamDownloadEngine`. | UI-agnostic. Communicates with the Win32 UI exclusively through an asynchronous observer interface (`IDownloadEvents`) marshaled via `PostMessageW`. |
| **HTTP Download Engine (`src/engine_http/`)** | `openidm.exe` (WinHTTP Async Callbacks + IOCP Pool) | Execute HTTP/HTTPS probes; manage pre-allocated `.openidm.part` file handles; implement Brisk-style dynamic connection spawning, work-stealing byte-range splitting, and hung-connection resets; checkpoint byte progress to `StorageManager`. | Handles single-file HTTP/HTTPS transfers only. Does **not** parse HLS/DASH manifests or spawn external processes. |
| **Stream Engine Adapter (`src/engine_stream/`)** | `openidm.exe` (Subprocess Supervisor Thread) | Write restricted-ACL temporary `@args.rsp` / `cookies.txt` files; spawn `N_m3u8DL-RE.exe` inside a Windows Job Object; parse `meta.json` during probe phase; parse line-oriented stdout/stderr progress during download/mux phases; delete temporary directories on completion. | Does **not** link against C#/.NET assemblies in-process; interacts with `N_m3u8DL-RE.exe` strictly via documented CLI flags, response files, `meta.json`, and exit codes. |
| **Storage & Security Manager (`src/storage/`)** | `openidm.exe` (Dedicated SQLite Serializer Thread) | Manage `state.db` in SQLite `WAL` mode; batch high-frequency segment offset writes (every 1s or 4 MiB); encrypt/decrypt transient `Cookie` and `Authorization` headers via Windows DPAPI (`CryptProtectData`); write redacted rotating logs. | Single writer thread to SQLite (`SQLITE_OPEN_NOMUTEX` on dedicated strand) to eliminate lock contention with UI or network threads. |
| **Win32 Native UI (`src/ui/`)** | `openidm.exe` (Main UI STA Thread) | Render main window, Virtual ListView (`LVS_OWNERDATA`), Add Download Dialog, Stream Track Selector Dialog, Per-Task Progress Dialog, Settings PropertySheet, and System Tray icon; handle Windows shutdown (`WM_QUERYENDSESSION`) and power events (`WM_POWERBROADCAST`). | Never performs blocking disk I/O, SQLite queries, or `WinHTTP` calls on the UI thread. |

---

## 4. Proposed Source Directory Structure

Because the repository is currently in a pre-implementation greenfield state ([REPOSITORY_AUDIT.md](REPOSITORY_AUDIT.md)), future implementation must follow this exact directory layout:

```text
openidm/
├── CMakeLists.txt                  # Root CMake configuration (C++20, MSVC x64/ARM64)
├── vcpkg.json                      # Locked C++ dependencies (sqlite3, nlohmann-json, wil, gtest)
├── README.md
├── AGENTS.md
├── CONTRIBUTING.md
├── LICENSE                         # GPL-3.0-or-later full text
├── LICENSE_REVIEW.md
├── THIRD_PARTY_NOTICES.md          # License notices for N_m3u8DL-RE, FFmpeg, SQLite, WIL, nlohmann-json
├── docs/                           # Complete engineering documentation & ADRs
│   └── adr/
├── src/
│   ├── common/                     # UTF-8/UTF-16 conversion, Result<T,E>, PathSanitizer, Logger, DPAPI
│   ├── ipc/                        # NamedPipeServer, NamedPipeClient, JsonRpcProtocol
│   ├── storage/                    # SqliteStore, Migrations, TaskRepository, SettingsRepository
│   ├── core/                       # DownloadController, QueueScheduler, BandwidthLimiter, TaskModel
│   ├── engine_http/                # IFileDownloadEngine, WinHttpDownloadEngine, SegmentScheduler
│   ├── engine_stream/              # IStreamDownloadEngine, Nm3u8dlReAdapter, MetaJsonParser
│   ├── ui/                         # Win32 MainWindow, VirtualTaskListView, ProgressDialog, TrackDialog, TrayIcon
│   ├── app/                        # openidm.exe WinMain entry point, single-instance mutex, crash handler
│   └── nmh/                        # openidm-nmh.exe main() entry point (stdio <-> Named Pipe proxy)
├── extension/
│   ├── package.json                # TypeScript + Rollup/esbuild build scripts (zero runtime dependencies)
│   ├── tsconfig.json
│   ├── manifest.chromium.json      # Chrome / Edge / Brave MV3 manifest
│   ├── manifest.firefox.json       # Firefox MV3 manifest (background.scripts + webRequest)
│   ├── _locales/                   # i18n message catalogs (en, zh_CN)
│   └── src/
│       ├── background/             # Service worker: webRequest sniffer, downloads interceptor, native port
│       ├── content/                # Isolated world DOM observer + closed Shadow DOM video button
│       ├── page_hook/              # Optional opt-in MAIN world XHR/fetch/MSE manifest detector
│       ├── popup/                  # Extension popup UI (detected streams list, site exclusion toggle)
│       └── shared/                 # Shared TypeScript interfaces & URL/MIME classification rules
├── installer/
│   ├── openidm.iss                 # Inno Setup 6 script (per-user & machine-wide, NMH registry keys)
│   └── manifests/                  # Native Messaging Host JSON templates (Chromium & Firefox)
└── tests/
    ├── unit/                       # GoogleTest C++ unit tests (SegmentScheduler, PathSanitizer, MetaJsonParser)
    ├── integration/                # Local Python/C++ HTTP & HLS/DASH deterministic test server tests
    └── extension/                  # Vitest / Playwright browser extension tests
```

---

## 5. Logical Architecture vs. Initial Implementation Phases

To minimize integration risk, the logical architecture above will be realized incrementally across the milestones defined in [ROADMAP.md](ROADMAP.md):
- **Milestone 1 (Core Desktop + HTTP Engine):** Implements `src/common/`, `src/storage/`, `src/core/`, `src/engine_http/`, and `src/ui/` inside `openidm.exe`. Manual URL addition and clipboard capture work end-to-end without the browser extension or `N_m3u8DL-RE`.
- **Milestone 2 (Native Messaging + Browser Extension):** Implements `src/ipc/`, `src/nmh/` (`openidm-nmh.exe`), and `extension/`. Enables automatic ordinary file interception and passive media URL forwarding from Chrome/Edge/Brave to `openidm.exe`.
- **Milestone 3 (Stream Download Pipeline):** Implements `src/engine_stream/` (`Nm3u8dlReAdapter`), the Win32 Stream Track Selection Dialog, and `N_m3u8DL-RE.exe` + `ffmpeg.exe` Job Object orchestration.
- **Milestone 4 & 5 (Queues, Firefox Port, Opt-in Deep Search, Installer & ARM64):** Completes scheduled queues, Firefox packaging, opt-in page-context hooks, Inno Setup installer, and ARM64 builds.
