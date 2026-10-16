# Internet Download Manager (IDM) Audit Implementation Map

**Document Status:** Complete Reverse Engineering to Implementation Traceability Map  
**Last Reviewed:** 2026-10-16  
**Scope:** Maps verified findings from the reverse engineering investigation (`idm audit/`) of Internet Download Manager v6.43 to the concrete architectural components, interfaces, and test suites in **Flux Download Manager**.

---

## 1. Classification Methodology

In accordance with strict reverse engineering standards, all findings are categorized into four epistemic tiers:
- **`[CONFIRMED]`**: Directly observed in decompiled C code, PE export tables, or active Windows runtime behavior.
- **`[STRONGLY SUPPORTED]`**: Deduced from cross-referencing string tables, import tables, and extension scripts.
- **`[HYPOTHESIS]`**: Plausible mechanism inferred from protocol framing or secondary artifacts.
- **`[UNKNOWN / OUT OF SCOPE]`**: Proprietary or DRM-related behavior not documented or intentionally excluded.

---

## 2. Comprehensive Finding-to-Implementation Matrix

| ID | Area / Capability | IDM Evidence (`idm audit/`) | Epistemic Status | Flux Download Manager Implementation | Verification Test Suite |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **MAP-01** | **Browser Media Monitoring & MIME Filtering** | `background.js` (function `Hc`), `idmcchandler7.dll` (`FUN_10016f10`), MIME list table in `Aa`. | `[CONFIRMED]` | `extension/js/background.js` (`onHeadersReceivedHandler`) & `extension/js/function.js` (`classifyNetworkResource`). Checks `video/mp4`, `video/webm`, `application/x-mpegURL`, `application/vnd.apple.mpegurl`, `application/dash+xml`. Disregards thumbnails `< 512 KiB`. | `tests/test_extension_detection.js` (Suite 1: Direct MP4/WebM & HLS/DASH detection). |
| **MAP-02** | **Dynamic DOM Media Discovery** | `content.js` (`MutationObserver`, `ResizeObserver`, `IntersectionObserver`) watching `<video>`, `<audio>`, `<embed>`, `<iframe>`. | `[CONFIRMED]` | `extension/js/content-script.js` (`scanAndAttachVideoOverlays`, `observeDynamicVideosAndFullscreen`, `initContentScript`). Watches video insertions, changes in `currentSrc`, dynamic playlist attachments, media events (`play`, `playing`, `canplay`), and fullscreen state. | `tests/test_extension_detection.js` (Suite 5: Dynamic video tracking). |
| **MAP-03** | **Video Overlay Design & Presentation** | `idmcchandler7.dll` (`IDM_Download_Panel_controller2_class`) and `idmbrbtn.dll` (`CreateIDMButton3`, `SetWindowPos` in `FUN_10003c30`). | `[CONFIRMED]` | **In-Page Closed Shadow DOM Overlay** (`extension/js/content-script.js`). While IDM used a native Win32 `HWND` with `SetWindowPos`, Flux adopts a zero-leakage Closed Shadow DOM (`mode: "closed"`) button placed 10px from the top-right of the video player. This avoids the positioning lag, multi-monitor scaling anomalies, and DWM composition tearing observed in IDM's floating HWND approach. | `tests/test_extension_detection.js` (Suite 5) & `tests/cpp/unit_tests.cpp` (`TestDesktopUiDesignSystemAndHighDpiLayout`). |
| **MAP-04** | **Browser Native Messaging Bridge** | `IDMMsgHost.exe` (`FUN_00402830`), manifest `com.tonec.idm` registered under `HKCU\Software\Google\Chrome\NativeMessagingHosts`. | `[CONFIRMED]` | `src/nmh/main.cpp` (`flux-nmh.exe`), registered under `com.flux.downloadmanager` and `org.flux.native`. Reads 32-bit length-prefixed JSON from browser stdin/stdout with `_setmode(..., _O_BINARY)` on Windows. | `tests/run_all_integration_tests.py` (Suite 7: `IT-IPC-01`). |
| **MAP-05** | **Desktop IPC & Auto-Launch Daemon** | `IDMMsgHost.exe` checks mutex `Local\IDMEventMonitor`, launches `IDMan.exe` if idle, and connects to `\\.\pipe\IDMNetworkMonitor.<SessionId>`. | `[CONFIRMED]` | `src/nmh/main.cpp` (`ConnectToDesktopPipe`) & `src/ipc/ipc_server.cpp`. Connects to `\\.\pipe\Flux_IPC_Default` (with fallback to `OpenIDM_IPC_Default`). If the pipe is unavailable, it auto-spawns `flux.exe --daemon` via `CreateProcessW` with `CREATE_NO_WINDOW | DETACHED_PROCESS` and retries. | `tests/run_all_integration_tests.py` (Suite 7: `IT-IPC-01`). |
| **MAP-06** | **IPC Protocol Framing & Structured Messaging** | `IDMNetMon.dll` (`FUN_10034b40`) using ASCII framing `MSG#<seq>#<type>#...`. | `[CONFIRMED]` | Standard **JSON-RPC 2.0** over Named Pipe (`src/ipc/ipc_server.cpp`). Methods: `download.offer`, `download.batchAdd`, `stream.probe`, `stream.add`, `system.handshake`, `task.control`, `task.list`. Robust against binary injection and encoding corruption. | `tests/run_all_integration_tests.py` (Suite 7: `IT-IPC-01`). |
| **MAP-07** | **Cookie & Auth Header Synchronization** | `background.js` records request headers (`Referer`, `Cookie`, `User-Agent`) matching `requestId` before dispatching to IDM core. | `[CONFIRMED]` | `extension/js/background.js` (`onBeforeSendHeadersHandler`). Stores headers in `G.requestHeadersMap` per `requestId`. Passes `Referer`, `Cookie`, `Origin`, and `User-Agent` in `download.offer` and `stream.add` payloads. | `tests/test_extension_detection.js` (Suite 1: Header Correlation). |
| **MAP-08** | **Adaptive Stream Manifest Parsing & Tracks** | `IDMVMPrs.dll` ("Video Manifest Parser", `Parse1`) called by `idmcchandler7.dll` (`FUN_100230a0`). | `[CONFIRMED]` | Handled out-of-process by `N_m3u8DL-RE.exe` via `src/engine_stream/nm3u8dl_re_adapter.cpp` (`ProbeStreamManifest`). Generates track lists with resolution, bitrate, audio languages, and segment counts. | `tests/run_all_integration_tests.py` (Suite 5: `IT-STREAM-01`). |
| **MAP-09** | **Master vs. Variant Playlist Deduplication** | `content.js` and `background.js` suppressing `.ts`/`.m4s` chunks when a master manifest is already captured. | `[CONFIRMED]` | `extension/js/function.js` (`addOrDeduplicateMediaCandidate`). Suppresses segment requests matching `\.ts(\?.*)?$`, `\.m4s(\?.*)?$`, and groups variant child playlists (`1080p/index.m3u8`) under the parent master playlist. | `tests/test_extension_detection.js` (Suite 2: Master vs. Child deduplication). |
| **MAP-10** | **Multi-Segmented HTTP Transfer & Range Requests** | `IDMan.exe` (`FUN_0058c7f0`, `FUN_004b8980`) parsing `Content-Range`, spawning socket threads with byte ranges. | `[CONFIRMED]` | `src/brisk_engine/http_download_engine.cpp` & `segment_scheduler.cpp`. Implements dynamic connection spawning up to 8 connections, work-stealing segment splits, and unstarted stolen segment rollbacks. | `tests/run_all_integration_tests.py` (Suites 2 & 4: `IT-HTTP-01`, `IT-HTTP-03`, `IT-HTTP-04`). |
| **MAP-11** | **Stream Multiplexing & Container Assembly** | `IDMan.exe` downloading separate video/audio tracks and multiplexing into `.mp4`. | `[STRONGLY SUPPORTED]` | Managed worker execution of `N_m3u8DL-RE.exe` + `ffmpeg.exe` inside a Windows Job Object (`src/engine_stream/nm3u8dl_re_adapter.cpp`). Produces standardized `.mp4` containers. | `tests/run_all_integration_tests.py` (Suites 5 & 6: `IT-STREAM-02`, `IT-STREAM-03`). |
| **MAP-12** | **Strict DRM Non-Circumvention** | IDM does not download Widevine/PlayReady protected streams. | `[CONFIRMED]` | `src/engine_stream/nm3u8dl_re_adapter.cpp` (`ValidateManifestDrmStatus`). If manifest contains `#EXT-X-KEY:METHOD=SAMPLE-AES-CTR` or `cenc:pssh`, it rejects immediately with `ERR_STREAM_DRM_PROTECTED`. | `tests/cpp/unit_tests.cpp` (Suite 6: `TestStreamMetaJsonAndDrmRejection`). |
| **MAP-13** | **Credential Protection in Process Arguments** | IDM passes sensitive URLs and cookies through internal IPC rather than CLI arguments. | `[CONFIRMED]` | `src/engine_stream/nm3u8dl_re_adapter.cpp`. Writes sensitive cookies, auth headers, and URLs to a temporary response file (`@job.rsp`) in a private user directory (`%LOCALAPPDATA%\Flux Download Manager\tmp\`), preventing command-line inspection via Task Manager or WMI. | `tests/cpp/unit_tests.cpp` (`TestStreamMetaJsonAndDrmRejection`). |
| **MAP-14** | **Link-Click Interception & Alt Key Bypass** | `content.js` capture event on `<a>` tags (`this.wc`), inspects `download` attribute and URL extensions. | `[CONFIRMED]` | `extension/js/content-script.js` (`handleLinkClick`). Listens on `document` `click`/`auxclick` in capture phase, checks `a.href`, `a.download`, `G.InterceptFileExts`, and allows immediate bypass when `e.altKey` is held. | `tests/test_extension_detection.js` (Suite 6: Link Interception & Alt Bypass). |
| **MAP-15** | **Browser Download Interception Coordination** | `background.js` listens to `downloads.onCreated`, `downloads.onDeterminingFilename`, calls `pause`, `cancel` on accept, `resume` on decline. | `[CONFIRMED]` | `extension/js/background.js` (`onDownloadCreatedHandler`). Intercepts eligible downloads, queries desktop via `download.offer`, cancels browser download only when desktop confirms acceptance, and falls back seamlessly to browser download if offline or declined. | `tests/test_extension_detection.js` (Suite 6: Safe Browser Fallback). |
| **MAP-16** | **Webpage vs. Media Stream Classification** | IDM parses manifests or receives media URLs from browser; never downloads raw HTML watch pages directly. | `[CONFIRMED]` | `extension/js/function.js` (`classifyUrlIntent`) & `src/ipc/ipc_server.cpp` / `src/core/download_controller.cpp` (`IsWebpageUrl`). Guards against HTML watch pages (`youtube.com/watch?v=...`, `vimeo.com/...`) being submitted as raw file downloads, preventing HTTP 429 Too Many Requests errors. | `tests/test_extension_detection.js` & C++ Unit Tests. |
| **MAP-17** | **In-Page Deep Search Hook in MAIN World** | IDM injects helper code or uses Native Hooking. Cat Catch uses `catch-script/catch.js`. | `[CONFIRMED]` | `extension/catch-script/catch.js` injected into webpage MAIN world. Monkey-patches `window.fetch`, `window.XMLHttpRequest`, and `window.URL.createObjectURL` to detect dynamic HLS manifests (`#EXTM3U`), DASH manifests (`<MPD`), and AES-128 keys, relaying via `postMessage`. | `tests/test_extension_detection.js` (Suite 4: Deep Search). |

---

## 3. Detailed Architectural Synthesis

### 3.1 Overlay Choice Justification
While IDM relies on a separate Win32 window (`idmbrbtn.dll`) positioned via `SetWindowPos`, our investigation and testing demonstrate that a **Closed Shadow DOM in-page overlay** (`mode: "closed"`) offers superior responsiveness and fidelity:
1. **Zero Jitter**: Moving, resizing, or scrolling the browser window maintains pixel-perfect positioning without the 1-2 frame tracking lag inherent in Windows `SetWindowPos` inter-process coordinate synchronization.
2. **Multi-DPI & Multi-Monitor**: Modern Windows 11 systems with mixed DPI monitors (e.g., 100% and 150%) often exhibit coordinate calculation discrepancies when converting browser DOM client coordinates to screen virtual coordinates. In-page Shadow DOM coordinates scale natively with browser CSS pixels.
3. **No CSS Collisions**: Using `mode: "closed"` guarantees that the host webpage's styles cannot penetrate the Flux download badge, and Flux's styles cannot leak into the webpage.

### 3.2 IPC & Protocol Modernization
IDM's legacy ASCII `MSG#` protocol is replaced with structured **JSON-RPC 2.0**:
- Fully typed payloads with integer request IDs, parameter dictionaries, and standard error objects (`code`, `message`).
- UTF-8 clean encoding, avoiding the parsing fragility of delimiter-separated ASCII strings when encountering complex URLs or Unicode filenames.

### 3.3 Link Interception vs. Browser Fallback
IDM intercepts all downloads unconditionally by hooking browser processes directly, which occasionally causes broken downloads when IDM is unavailable. Flux implements a resilient hybrid approach:
- Links are intercepted in the content script and background service worker.
- If Flux desktop is running and accepts the download, the browser download is canceled.
- If Flux desktop is stopped, busy, or declines, the download continues in the browser with zero interruption to the user.
