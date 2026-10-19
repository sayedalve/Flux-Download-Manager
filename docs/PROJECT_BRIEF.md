# Project Brief: OpenIDM

**Document Status:** Proposed Specification  
**Last Reviewed:** 2026-10-16  
**Related Documents:** [../README.md](../README.md), [PRODUCT_REQUIREMENTS.md](PRODUCT_REQUIREMENTS.md), [UPSTREAM_PROJECT_ANALYSIS.md](UPSTREAM_PROJECT_ANALYSIS.md), [ROADMAP.md](ROADMAP.md)

---

## 1. Problem Statement

Windows users have long relied on Internet Download Manager (IDM) for two core workflows that standard browser downloaders handle poorly:
1. **High-speed, resilient downloading of large ordinary files** (able to saturate high-bandwidth connections via dynamic byte-range segmentation, survive mid-transfer network drops, and refresh expired signed URLs without losing gigabytes of downloaded data).
2. **Intuitive, automatic detection and one-click downloading of web videos** (presenting a floating download button over active video players and cleanly assembling segmented HLS `.m3u8` and DASH `.mpd` streams alongside their audio and subtitle tracks into a single playable `.mp4` or `.mkv` file).

Existing open-source alternatives each solve only a fraction of this problem:
- **[Brisk](https://github.com/BrisklyDev/brisk)** implements an intelligent HTTP download engine with dynamic connection spawning and work-stealing connection reuse, but it is built on Flutter/Dart (incurring a ~30–45 MB desktop runtime footprint and higher idle RAM), communicates with its browser extension via an unauthenticated localhost port, and has basic M3U8 support compared to dedicated stream downloaders.
- **[Cat Catch (`cat-catch`)](https://github.com/xifangczy/cat-catch)** provides best-in-class browser media sniffing (`webRequest` MIME/manifest inspection, DOM observation, and page-context hooks), but its built-in downloader runs inside the browser tab—where it is throttled by Chromium's 6-connection-per-origin limit, risks tab crashes from buffering gigabytes of TS segments in RAM, hits a 2 GB ceiling in WebAssembly `ffmpeg.wasm`, and lacks full encrypted/live DASH support.
- **[N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE)** is a mature, high-performance C# NativeAOT command-line stream downloader for HLS, DASH, and MSS, but it has no official desktop queue manager or integrated browser extension, requiring users to manually copy URLs and headers or register fragile `m3u8dl://` URL protocols that expose command-injection risks.

---

## 2. Project Vision

Build **OpenIDM**: a cohesive, genuinely open-source (`GPL-3.0-or-later`), native Windows 11 download manager that unifies:
- A fast, low-memory **Native C++20 / Win32 Desktop Application** (`openidm.exe`) with an in-process `WinHTTP` segmented download engine implementing Brisk's dynamic connection spawning and work-stealing byte-range algorithms.
- A **Manifest V3 Browser Extension** combining Brisk's download interception and non-intrusive in-page video button with Cat Catch's multi-layered media and manifest discovery.
- A secure **Native Messaging + ACL Named Pipe IPC** bridge (`openidm-nmh.exe`) that passes URLs, transient `Referer`/`Cookie` headers, and stream metadata from the browser to the desktop application without opening any network port or exposing command-line injection surfaces.
- First-class **Stream Download Pipeline** orchestration that delegates HLS, DASH, and MSS manifests to a sandboxed `N_m3u8DL-RE.exe` + `ffmpeg.exe` worker pipeline with full GUI track selection, live progress parsing, and clean cancellation via Windows Job Objects.

---

## 3. Intended Users & Personas

1. **The Everyday Power User:** Wants a lightweight background utility (`< 15 MB` idle RAM in the Windows system tray) that automatically catches large browser downloads, accelerates them reliably, and displays a clean "Download This Video (1080p MP4)" button on educational, news, and social video sites.
2. **The Media Archivist / Educator:** Needs to select specific video resolutions (`1080p` vs `4K`), codecs (`avc1` vs `hvc1`), multiple audio tracks, and SRT/VTT subtitles from HLS/DASH manifests without wrestling with terminal commands or manual `Referer`/`Cookie` header copying.
3. **The Security-Minded Developer / Enterprise User:** Requires verifiable open-source code, zero telemetry, DPAPI-encrypted local credential handling, and no unauthenticated localhost HTTP listeners.

---

## 4. Scope: Minimum Viable Product (MVP) vs. Later Ambitions

To ensure engineering discipline and prevent scope creep, development is strictly separated into an **Immediate MVP (Milestones 1–3)** and **Post-MVP Enhancements (Milestones 4–5)**.

### 4.1 Minimum Viable Product (MVP — Milestones 1–3)
- **Native Win32 Desktop Application (`Windows 11 x64`):**
  - Main window with Virtual ListView (`LVS_OWNERDATA`) for tasks, category filter sidebar, toolbar, and status bar.
  - Per-Monitor v2 High-DPI awareness, Windows 11 system Dark/Light theme support, and system tray (`Shell_NotifyIconW`) integration.
  - Task progress dialog showing overall progress, per-segment byte progress bars, transfer speed, ETA, and log tab.
- **Ordinary File Download Engine (`WinHTTP` + IOCP):**
  - HTTP/1.1 and HTTP/2 downloads with `Accept-Ranges` / `206 Partial Content` probing.
  - Dynamic connection spawning (starting at 1 connection and scaling up to configurable max, default 8) and work-stealing dynamic connection reuse.
  - Automatic stall detection and connection reset; exponential backoff retry.
  - Pause, resume, and "Refresh Download Address" for expired signed URLs (`403 Forbidden` / `410 Gone`).
  - Crash-safe SQLite (`WAL` mode) segment checkpointing and atomic final file commit.
- **Browser Extension (Chrome, Edge, Brave) & Native Messaging IPC:**
  - Native Messaging Host (`openidm-nmh.exe`) bridging browser `stdio` to `\\.\pipe\OpenIDM_IPC_<UserSID>`.
  - Automatic capture of large file downloads (`chrome.downloads.onDeterminingFilename` / `onCreated`) with user-configurable file-type and minimum-size filters.
  - Passive `webRequest` detection of progressive videos (`.mp4`, `.webm`) and streaming manifests (`.m3u8`, `.mpd`).
  - Non-intrusive Shadow DOM floating "Download Video" button anchored to `<video>` elements.
- **Stream Download Pipeline (`N_m3u8DL-RE` + `FFmpeg`):**
  - Out-of-process manifest probing (`--skip-download --write-meta-json`) and Win32 track selection dialog (video resolution/codec, audio track, subtitle track).
  - Concurrent segment downloading (`-mt`), real-time progress parsing (`--no-ansi-color`), FFmpeg muxing to `.mp4` or `.mkv`, and Job Object process lifecycle management.

### 4.2 Post-MVP Ambitions (Milestones 4–5)
- **Firefox Extension Port & Cross-Browser Polish:** Full Firefox Manifest V3 port leveraging Firefox's persistent `webRequest` capabilities.
- **Advanced Queue & Scheduler Engine:** Main/secondary queues, time-window scheduling (start at `02:00`, stop at `06:00`), global and per-task speed limiters, and post-queue actions (sleep/shutdown).
- **Opt-In Page-Context ("Deep Search") Instrumentation:** `MAIN`-world XHR/fetch/MSE manifest discovery for sites that obfuscate manifest URLs, plus live stream recording controls (`--live-real-time-merge`).
- **Windows ARM64 Native Build & WinGet Packaging:** Native `arm64` compilation of `openidm.exe` and `openidm-nmh.exe` paired with `win-arm64` builds of `N_m3u8DL-RE` and `ffmpeg`.

---

## 5. Explicit Non-Goals

OpenIDM will **never** implement the following:
1. **DRM Circumvention:** OpenIDM will not decrypt Widevine, PlayReady, FairPlay, or WisePlay protected streams, will not bundle or invoke CDM key extraction tools, and will not bypass cryptographic content protection (`cenc:pssh` / `SAMPLE-AES-CTR`).
2. **Proprietary Site-Specific Extractor Scraping:** OpenIDM is a general HTTP and standard streaming (HLS/DASH/MSS) download manager with browser-assisted media capture—not a replacement for `yt-dlp`'s 1,800+ site-specific Python scrapers. (Future plugin hooks for external extractors are out of scope for MVP.)
3. **BitTorrent / Peer-to-Peer Protocols:** P2P torrent downloading (`libtorrent` / `aria2` BitTorrent mode) adds significant attack surface, firewall complexity, and binary bloat, and is explicitly excluded.
4. **In-Browser Video Transcoding or RAM Buffering:** Unlike Cat Catch's standalone mode, OpenIDM will never buffer full M3U8/MPD segment arrays inside the browser extension or run `ffmpeg.wasm` in browser memory.
5. **Cross-Platform Desktop GUI in MVP:** Linux and macOS desktop GUIs are non-goals; the desktop architecture is purpose-built for Windows 11 `x64` (and later `ARM64`).

---

## 6. Definition of Success

The project achieves its engineering goals when:
1. **Correctness:** 100% of test matrix downloads (multi-gigabyte HTTP range transfers, interrupted resumes, single-stream non-range transfers, VOD HLS `.m3u8`, and VOD DASH `.mpd`) produce byte-verified or container-verified (`ffprobe`) output files ([TEST_STRATEGY.md](TEST_STRATEGY.md)).
2. **Resource Efficiency:** The installed core desktop binary (`openidm.exe` + `openidm-nmh.exe`) is under `3 MB`, starts in `< 100 ms`, and consumes `< 15 MB` private working set when idle in the system tray ([PERFORMANCE_AND_SIZE_BUDGET.md](PERFORMANCE_AND_SIZE_BUDGET.md)).
3. **Security:** Zero network ports are opened by default; all IPC is restricted by OS user SID DACLs; all untrusted filenames and manifest parameters are sanitized against path traversal and argument injection ([SECURITY_THREAT_MODEL.md](SECURITY_THREAT_MODEL.md)).
