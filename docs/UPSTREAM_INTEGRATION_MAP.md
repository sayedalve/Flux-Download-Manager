# Upstream Integration & Traceability Map

**Document Status:** Complete Integration Architecture & File-by-File Traceability Matrix  
**Last Reviewed:** 2026-10-16  
**Scope:** Defines the direct reuse, adaptation, and runtime invocation of the three upstream repositories located in `third_party/`: **Brisk**, **Cat Catch**, and **N_m3u8DL-RE**.

---

## 1. Upstream Repositories Directory Reference

The source code of all three projects resides unmodified in `third_party/`:
- **Brisk**: [`third_party/brisk/`](file:///c:/Users/sayed/Downloads/openidm/third_party/brisk) (Flutter / Dart segmented engine)
- **Cat Catch**: [`third_party/cat-catch/`](file:///c:/Users/sayed/Downloads/openidm/third_party/cat-catch) (WebExtension media detection & manifest parser)
- **N_m3u8DL-RE**: [`third_party/N_m3u8DL-RE/`](file:///c:/Users/sayed/Downloads/openidm/third_party/N_m3u8DL-RE) (.NET C# adaptive streaming engine)

---

## 2. Component Integration Matrix

### 2.1 Brisk Download Manager (`third_party/brisk`)

Brisk's core contribution is its algorithmic segmentation model, work-stealing range reallocation, dynamic connection spawning, and bandwidth token bucket scheduler. Because Flux utilizes a native C++20 / Win32 desktop architecture for optimal Windows 11 performance, low idle memory (`< 15 MB`), and zero heavy runtime dependencies, Brisk's Dart algorithms have been directly translated into ISO C++20 modules in `src/brisk_engine/`.

| Upstream Module (`third_party/brisk/`) | Flux Source Module | Architectural Adaptation & Reused Logic |
| :--- | :--- | :--- |
| `brisk_download_engine/lib/src/download_engine/segment/download_segment_tree.dart` | `src/brisk_engine/segment_scheduler.hpp`, `src/brisk_engine/segment_scheduler.cpp` | **Work-Stealing Segment Tree**: Dynamic range splitting, range validation, remaining byte calculation, and `RollbackUnstartedStolenSegment` when a secondary connection fails. |
| `brisk_download_engine/lib/src/download_engine/engine/http_download_engine.dart` | `src/brisk_engine/http_download_engine.hpp`, `src/brisk_engine/http_download_engine.cpp` | **Multi-Connection Engine**: Dynamic spawn from 1 to 8 connections based on throughput milestones (256 KiB, 1 MiB), stall detection timer, connection reset, and atomic piece assembly. |
| `brisk_download_engine/lib/src/download_engine/connection/http_download_connection.dart` | `src/brisk_engine/http_client.hpp`, `src/brisk_engine/http_client.cpp` | **HTTP Client & Range Worker**: `WinHTTP` (Windows) / OpenSSL (POSIX) range requests (`Range: bytes=X-Y`), 302 redirect following, chunked transfer handling, and response header parsing. |
| `brisk_download_engine/lib/src/download_engine/setting/download_settings.dart` | `src/common/types.hpp` (`AppSettings`), `src/storage/sqlite_store.cpp` | **Settings Model**: Connection limits, rate limiter configuration (bytes per second), default save path, and conflict policy. |

### 2.2 Cat Catch (`third_party/cat-catch`)

Cat Catch is the industry benchmark for browser media sniffing and manifest discovery. The Flux browser extension (`extension/`) directly incorporates and refines Cat Catch's network filtering, DOM inspection, and playlist parsing routines into a unified Manifest V3 extension.

| Upstream Module (`third_party/cat-catch/`) | Flux Extension Module | Architectural Adaptation & Reused Logic |
| :--- | :--- | :--- |
| `js/background.js` | `extension/js/background.js` | **Network Interception**: `chrome.webRequest.onHeadersReceived` listener, MIME type dispatch table, content disposition filename extraction, size threshold filtering (`>= 512 KiB`), and header correlation. |
| `js/function.js` | `extension/js/function.js` | **Classification & Deduplication**: Master playlist vs. variant stream grouping, `.ts`/`.m4s` segment suppression, per-site exclusion lists (`G.disabledHosts`), and regex pattern matching. |
| `js/content-script.js` | `extension/js/content-script.js` | **Video Element Observers**: `MutationObserver` and `ResizeObserver` tracking `<video>` elements, calculating bounding rects, and maintaining fullscreen synchronization. |
| `catch-script/catch.js` | `extension/catch-script/catch.js` | **Deep Search Hook**: Injected page-context script intercepting `fetch()`, `XMLHttpRequest.open()`, and `HTMLMediaElement.src` to catch obfuscated and MSE blob manifests. |
| `popup.html`, `js/popup.js` | `extension/popup.html`, `extension/js/popup.js` | **Media Candidate Popup**: Media candidate list with resolution badges, filter tabs ("All", "Streams", "Direct"), search filtering, and download action triggers. |

### 2.3 N_m3u8DL-RE (`third_party/N_m3u8DL-RE`)

N_m3u8DL-RE is the premier engine for downloading and muxing HLS, MPEG-DASH, and MSS adaptive streams. Rather than reimplementing a fragile custom HLS/DASH parser and muxer in C++, Flux runs the authentic, verified upstream executable out-of-process inside a restricted Windows Job Object.

| Upstream Component | Flux Adapter Module | Architectural Adaptation & Reused Logic |
| :--- | :--- | :--- |
| `tools/win-x64/N_m3u8DL-RE.exe` (v0.3.0 Release) | `src/engine_stream/nm3u8dl_re_adapter.hpp`, `src/engine_stream/nm3u8dl_re_adapter.cpp` | **Managed Worker Subprocess**: Invokes `N_m3u8DL-RE.exe` with structured CLI flags, response file `@job.rsp` (preventing credential exposure on cmdline), and parsed JSON track probing. |
| Command-line Argument Parser | `src/engine_stream/nm3u8dl_re_adapter.cpp` | **Track Selection & Filters**: Passes `--select-video`, `--select-audio`, `--select-subtitle`, `--auto-select`, and `--log-level OFF`. |
| FFmpeg Muxer Integration | `tools/win-x64/ffmpeg.exe` via `nm3u8dl_re_adapter.cpp` | **Multiplexing Pipeline**: Directs `--ffmpeg-binary-path` to the verified local FFmpeg binary to mux downloaded audio/video segments into `.mp4` or `.mkv`. |
| Process Lifecycle & Termination | `src/engine_stream/nm3u8dl_re_adapter.cpp` | **Job Object Isolation**: Uses Win32 `CreateJobObjectW` with `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE` to ensure child worker processes are instantly terminated if Flux crashes or the task is canceled. |

---

## 3. Upstream Maintenance and Change Log

Any patch applied to integrate or adapt upstream functionality is recorded below:

1. **2026-10-16 (`PATCH-001`)**: Translated Brisk's `download_segment_tree.dart` into `src/brisk_engine/segment_scheduler.cpp`, adding the `RollbackUnstartedStolenSegment` fix (`BUG-008`).
2. **2026-10-16 (`PATCH-002`)**: Enhanced Cat Catch `js/content-script.js` with a non-intrusive Closed Shadow DOM overlay button positioned 10px from the top-right of the video player.
3. **2026-10-16 (`PATCH-003`)**: Configured `nm3u8dl_re_adapter.cpp` to discover the official compiled `N_m3u8DL-RE.exe` in `tools/win-x64/` and communicate sensitive tokens through private `@job.rsp` temporary files.
4. **2026-10-16 (`PATCH-004`)**: Rebranded extension UI, notifications, and native messaging communication to **Flux Download Manager**.
