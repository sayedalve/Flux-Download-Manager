# Known Limitations and Constraints

**Document Status:** Active Engineering Limitations Log (`v0.2.0`)  
**Last Reviewed:** 2026-10-16  
**Scope:** Documented browser restrictions, unsupported DRM/streaming formats, upstream tool constraints, and host environment boundaries.  
**Related Documents:** [PRODUCT_COMPLETION_AUDIT.md](PRODUCT_COMPLETION_AUDIT.md), [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md), [MEDIA_DETECTION.md](MEDIA_DETECTION.md), [STREAM_DOWNLOAD_PIPELINE.md](STREAM_DOWNLOAD_PIPELINE.md)

---

## 1. Known to Be Unsupported (By Design & Legal Policy)

1. **DRM-Protected Streams (Widevine, PlayReady, FairPlay, WisePlay, CENC `SAMPLE-AES-CTR`):**
   - **Limitation:** Streams encrypted with commercial DRM systems require hardware/browser Content Decryption Modules (CDMs) and license server key exchange.
   - **Behavior:** OpenIDM strictly refuses to bypass DRM (`NFR-015`). When a `<video>` element has `mediaKeys !== null` or `N_m3u8DL-RE`'s `meta.json` reports DRM encryption without an open standard key, OpenIDM rejects the task immediately with `ERR_STREAM_DRM_PROTECTED` (*"Stream uses unsupported DRM protection. OpenIDM does not circumvent DRM."*).
2. **Proprietary Non-Manifest MSE `blob:` Streams & WebRTC Video Calls:**
   - **Limitation:** Standard MSE players (`hls.js`, `dash.js`) fetch `.m3u8` or `.mpd` manifests over HTTP, which OpenIDM captures and downloads at full speed. However, if a website feeds raw video frames into a `MediaSource` `SourceBuffer` over a custom encrypted WebSocket or WebRTC `RTCPeerConnection` without any `.m3u8`/`.mpd` manifest, `N_m3u8DL-RE` cannot download the stream from a URL.
   - **Behavior:** `blob:` URLs themselves are never sent to `openidm` as downloadable file URLs because `blob:` handles exist only inside the browser renderer's memory.
3. **Sites Requiring Custom JavaScript Challenge Solving per Segment (e.g., YouTube Signature Cipher / PO Token):**
   - **Limitation:** OpenIDM is a general HTTP/HTTPS and standard HLS/DASH/MSS download manager, not a site-specific scraper engine like `yt-dlp`. Sites that dynamically sign or throttle every individual segment via proprietary JS VM challenges are not supported natively.
4. **BitTorrent Download Execution Disabled:**
   - **Limitation:** Bencode decoding and magnet URI parsing (BEP 3 / BEP 9) are supported for metadata inspection. However, peer-to-peer BitTorrent transfer is not implemented in this build.
   - **Behavior:** Starting a BitTorrent or magnet download fails cleanly with error code `ERR_TORRENT_UNSUPPORTED` (*"BitTorrent downloading is not supported in this version."*). Flux does not simulate progress, does not increment piece counters via timers, and does not create empty dummy files on disk.
5. **FTPS (FTP over TLS/SSL) Disabled Pending Verified TLS Implementation:**
   - **Limitation:** Plain FTP (RFC 959, 2428, 3659) with PASV mode and REST resume is supported. FTPS (RFC 4217) requires TLS negotiation on control and data channels with strict certificate validation, which is not yet implemented.
   - **Behavior:** `ftps://` URLs are rejected immediately with `ERR_FTPS_UNSUPPORTED` (*"FTPS (FTP over TLS/SSL) is not supported. Plaintext transmission over ftps:// is prohibited."*) before any socket connection is attempted, preventing plaintext credential or data leakage over unencrypted channels.

---

## 2. Browser & Extension Platform Restrictions

1. **Browser Internal & Store Pages:**
   - Chrome, Edge, Brave, and Firefox block all extension content scripts and network hooks on privileged internal pages (`chrome://`, `edge://`, `about:`, `view-source:`) and official extension store domains (`chromewebstore.google.com`, `microsoftedge.microsoft.com`, `addons.mozilla.org`).
2. **Cross-Origin Sandboxed Iframes Without `allow-scripts`:**
   - While OpenIDM's passive `webRequest` sniffer (`Layer 1`) still detects media and manifests inside all iframes and surfaces them in the extension toolbar popup, the in-page floating Shadow DOM button (`Layer 2`) cannot render inside an `<iframe>` if the host page sets a restrictive `sandbox` attribute that blocks script execution.
3. **Pause/Resume on Non-Range HTTP Servers (`FR-002`):**
   - When an HTTP server omits `Accept-Ranges: bytes` or responds `200 OK` to a `Range` request, pausing the download cannot preserve progress; resuming restarts cleanly from byte `0` after clearing stale segment state.
4. **Client-Side Generated Files (`blob:`, `data:` URLs, ChatGPT Exports):**
   - In-memory files created by client-side web application scripts (such as ChatGPT chat exports/artifacts, WebGL Canvas snapshots, and local CSV generators) exist purely in the browser renderer heap and cannot be downloaded by external processes like WinHTTP.
   - Flux intentionally leaves these files to the browser's native download pipeline without monkey-patching DOM events or intercepting user gestures, ensuring reliable, uncorrupted client saves.

---

## 3. Build Environment vs. Windows 11 Host Verification Boundary

1. **Linux Container Build vs. Windows 11 Native `.exe` Compilation:**
   - The automated build and verification container runs Debian 12 Linux `x86_64` (`g++ 12.2.0`, `node 18.20.4`, `python3 3.11.2`, `ffmpeg 5.1.9`) without a Windows cross-compiler (`mingw-w64` or `MSVC`).
   - All Win32 API implementations (`WinHTTP`, `CreateFileW` + `OVERLAPPED`, `CreateJobObjectW` + `CreateProcessW`, Named Pipes, DPAPI/BCrypt, and the Win32 `RegisterClassExW`/`CreateWindowExW` GDI double-buffered GUI in `src/ui/win32_main_window.cpp`) are implemented in `#ifdef _WIN32` blocks and verified by source inspection, while the shared C++20 core, UI view-model, native `N_m3u8DL-RE` worker, and extension are verified by execution (`26 / 26` test suites passing).
   - Producing signed PE32+ `.exe` binaries (`openidm.exe`, `openidm-nmh.exe`, `N_m3u8DL-RE.exe`) and compiling the Inno Setup installer (`OpenIDM-Setup-x64-0.2.0.exe`) requires running `.\scripts\build-windows-msvc.ps1` on a Windows 11 `x64` workstation with Visual Studio 2022 and Inno Setup 6 (`ISCC.exe`).
