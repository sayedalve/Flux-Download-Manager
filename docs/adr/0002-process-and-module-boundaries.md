# ADR-0002: Process and Module Boundaries

- **Date:** 2026-10-16
- **Status:** Proposed
- **Deciders:** Principal Software Architect, Senior Windows Systems Engineer, Download Engine Specialist
- **Related Documents:** [../SYSTEM_ARCHITECTURE.md](../SYSTEM_ARCHITECTURE.md), [0003-browser-communication.md](0003-browser-communication.md), [0005-download-engine-integration.md](0005-download-engine-integration.md)

---

## Status

**Proposed** (Baseline Architecture Decision for Milestones 1–3).

## Context

OpenIDM combines four distinct execution workloads:
1. **Desktop UI & Task Orchestration:** Managing queues, user dialogs, system tray state, and SQLite persistence.
2. **Browser Native Messaging Endpoint:** Receiving asynchronous messages from Chrome, Edge, Brave, and Firefox extensions.
3. **Ordinary HTTP/HTTPS Segmented Downloading:** High-concurrency byte-range requests (`1–32` connections per task).
4. **Adaptive Stream Downloading & Muxing:** Parsing `.m3u8`/`.mpd`/`.ism` manifests, downloading thousands of `.ts`/`.m4s` fragments, decrypting standard AES-128 segments, and muxing audio/video/subtitles into `.mp4`/`.mkv`.

We must decide which components run in-process inside `openidm.exe` and which run as separate OS processes.

## Decision

We establish **four explicit OS process boundaries**:

1. **`openidm.exe` (Main Desktop & HTTP Engine Process):**
   - Runs as a single-instance per-user GUI process (enforced via a per-user named mutex `Local\OpenIDM_SingleInstance_<UserSID>`).
   - Hosts the **Win32 UI**, **DownloadController**, **SQLite Persistence (`state.db`)**, **Named Pipe IPC Server**, and the **Ordinary HTTP/HTTPS Download Engine (`WinHttpDownloadEngine`)** in-process across dedicated worker threads.
2. **`openidm-nmh.exe` (Stateless Native Messaging Host Proxy Process):**
   - A tiny (`< 300 KB`) console executable spawned directly by each browser instance when the extension calls `runtime.connectNative("org.openidm.native")`.
   - Relays length-prefixed JSON messages between browser `stdin`/`stdout` and `openidm.exe`'s Named Pipe server (`\\.\pipe\OpenIDM_IPC_<UserSID>`), launching `openidm.exe --background` if the pipe does not exist.
3. **`N_m3u8DL-RE.exe` (Out-of-Process Stream Download Worker):**
   - Spawned by `openidm.exe` via `CreateProcessW` (one worker process per active stream probe or stream download task) inside a dedicated Windows Job Object (`JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`).
4. **`ffmpeg.exe` (Out-of-Process Media Container Muxer):**
   - Spawned as a child process of `N_m3u8DL-RE.exe` (via `--ffmpeg-binary-path`) or directly by `openidm.exe` inside the same Windows Job Object.

## Alternatives Considered

1. **Registering `openidm.exe` Directly as the Browser Native Messaging Host (`Rejected`):**
   - *Why Rejected:* When a user runs both Chrome and Edge (or multiple browser profiles), each browser spawns its own Native Messaging Host process with `stdin`/`stdout` pipes attached. If `openidm.exe` were the direct NMH binary, multiple instances would contend for `state.db` and the system tray icon, or shutting down the browser would close `stdin` and terminate the user's active downloads! Separating `openidm-nmh.exe` decouples browser lifecycle from download lifecycle.
2. **Running Ordinary HTTP Downloads in Separate Subprocesses (`aria2c.exe` or per-task worker `.exe`) (`Rejected`):**
   - *Why Rejected:* `WinHTTP` asynchronous requests backed by a shared Windows I/O Completion Port (IOCP) inside `openidm.exe` scale effortlessly to dozens of connections with `< 1 MB` memory overhead and zero IPC serialization cost for high-frequency per-segment byte progress updates.
3. **Embedding `N_m3u8DL-RE` and `FFmpeg` as In-Process DLLs (`libavformat.dll` / `.NET Hosting API`) (`Rejected`):**
   - *Why Rejected:* Embedding the .NET CLR and FFmpeg C libraries directly inside `openidm.exe` balloons resident memory, creates symbol/allocator conflicts, and means a single malformed media container or demuxer bug (`SIGSEGV` / access violation in `libavformat`) crashes the entire desktop application and aborts all other running downloads.

## Rationale

- **Decoupled Lifecycle:** Closing the browser kills `openidm-nmh.exe` while `openidm.exe` continues downloading in the background.
- **Fault Containment:** A crash or hang in `N_m3u8DL-RE.exe` or `ffmpeg.exe` on a malformed stream affects only that single stream task; `openidm.exe` catches the non-zero exit code and marks that task as `Failed` without disturbing other downloads.
- **Guaranteed Cleanup:** Binding `N_m3u8DL-RE.exe` (and its child `ffmpeg.exe`) to a Windows Job Object with `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE` ensures that if `openidm.exe` is force-killed via Task Manager, the Windows kernel immediately terminates all stream workers.

## Consequences

- **Positive:** Clean failure isolation, zero SQLite multi-process lock contention, and clean C++ abstract interfaces (`IFileDownloadEngine` in-process, `IStreamDownloadEngine` out-of-process).
- **Negative:** Requires building and shipping two small project binaries (`openidm.exe` and `openidm-nmh.exe`) alongside the two bundled external binaries (`N_m3u8DL-RE.exe` and `ffmpeg.exe`).

## Risks & Mitigations

- **Risk:** Orphaned child processes if `N_m3u8DL-RE.exe` spawns `ffmpeg.exe` outside a Job Object.
- **Mitigation:** Ensure the Job Object does **not** set `JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK`, so any `ffmpeg.exe` child spawned by `N_m3u8DL-RE.exe` automatically inherits membership in the same kill-on-close Job Object.
