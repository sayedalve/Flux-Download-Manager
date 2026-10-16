# ADR-0004: Browser Media Discovery Architecture & Download Boundary

- **Date:** 2026-10-16
- **Status:** Proposed
- **Deciders:** Browser Extension Engineer, Download Engine Specialist, Application Security Engineer
- **Related Documents:** [../MEDIA_DETECTION.md](../MEDIA_DETECTION.md), [../BROWSER_EXTENSION.md](../BROWSER_EXTENSION.md), [../STREAM_DOWNLOAD_PIPELINE.md](../STREAM_DOWNLOAD_PIPELINE.md), [../UPSTREAM_PROJECT_ANALYSIS.md](../UPSTREAM_PROJECT_ANALYSIS.md)

---

## Status

**Proposed** (Baseline Media Discovery Decision for Milestone 2 & 3).

## Context

To compete with IDM's automatic video download panel, OpenIDM must reliably discover videos playing on modern websites and attach a convenient "Download Video" button to the active video player. Modern web video delivery falls into four categories:
1. **Progressive HTTP Media:** Direct `<video src="https://.../video.mp4">` or byte-range `.mp4`/`.webm` requests.
2. **Adaptive Streaming via MSE (`hls.js`, `dash.js`, Shaka Player):** The `<video>` element's `src` is a synthetic `blob:https://...` URL backed by a `MediaSource` object, while the actual `.m3u8` (HLS) or `.mpd` (DASH) manifest and `.ts`/`.m4s` segments are fetched via `fetch()` or `XMLHttpRequest`.
3. **Obfuscated / Dynamic Manifests:** Sites that fetch `.m3u8` or `.mpd` content from URLs without `.m3u8`/`.mpd` extensions and return generic `text/plain` or `application/octet-stream` headers, or expire the manifest URL immediately after the first read.
4. **DRM-Protected Streams (Widevine / PlayReady / FairPlay):** Encrypted streams controlled by a browser Content Decryption Module (CDM).

Upstream [Cat Catch](https://github.com/xifangczy/cat-catch) detects these via `webRequest` and `catch-script/catch.js`, but attempts to parse, download, and merge segments inside the browser extension—hitting Chrome's 6-connection limit, running out of tab memory on large streams, hitting a 2 GB limit in `ffmpeg.wasm`, and failing on encrypted/live DASH.

## Decision

We adopt a **Three-Layer Browser Detection Architecture** paired with a **Strict Discovery-vs-Download Boundary**:

### 1. Three Detection Layers in the Browser Extension
- **Layer 1 — Passive `webRequest` Sniffer (Always On, Service Worker):**
  - Monitors `chrome.webRequest.onBeforeSendHeaders` (with `'requestHeaders'`, `'extraHeaders'`) and `onHeadersReceived` (with `'responseHeaders'`).
  - Identifies progressive media (`video/*`, `audio/*` with `Content-Length >= min_media_size_bytes`, default `512 KiB`) and streaming manifests (`.m3u8`, `.mpd`, `.ism`, `application/vnd.apple.mpegurl`, `application/x-mpegurl`, `application/dash+xml`, `application/vnd.ms-sstr+xml`).
  - Explicitly filters out individual media segments (`.ts`, `.m4s`, `.aac`, `range=` byte chunks of adaptive streams) so the candidate list is never flooded with thousands of segment URLs.
  - Captures exact transient request headers (`Referer`, `Origin`, `Cookie`, `User-Agent`, `Authorization`) keyed by `tabId` and `frameId`.
- **Layer 2 — Isolated Content Script DOM & Overlay Manager (Always On, `ISOLATED` World):**
  - Runs across main frames and subframes (`"all_frames": true`).
  - Uses `MutationObserver`, `ResizeObserver`, and `play`/`loadedmetadata` event listeners (capturing phase) to detect visible `<video>` elements (`>= 240x135` px).
  - Renders a closed Shadow DOM floating "Download Video" button over the active `<video>` element whenever Layer 1 (or Layer 3) has recorded at least one downloadable candidate for that tab, or when `<video>.currentSrc` is an `http(s):` URL.
- **Layer 3 — Opt-In Page-Context Instrumentation ("Deep Search", Per-Site Opt-In, `MAIN` World):**
  - Disabled by default to preserve page performance and anti-fingerprinting hygiene.
  - When enabled by the user for a specific site (`FR-017`), injects a lightweight wrapper around `window.fetch`, `XMLHttpRequest.prototype.open/send`, and `MediaSource.prototype.addSourceBuffer` to inspect text responses starting with `#EXTM3U` or `<MPD` even when served with `application/octet-stream` MIME types, forwarding the manifest URL and cached text body to the isolated content script.

### 2. Strict Boundary Between Discovery and Downloading
- **The Browser Extension NEVER downloads segments, parses full segment lists, or muxes media.**
- Its responsibility ends at producing a normalized `MediaCandidateDescriptor` (containing the manifest/media URL, optional cached manifest text, captured request headers, tab/frame metadata, and confidence score) and sending it over Native Messaging IPC to `openidm.exe`.
- **All manifest probing (`N_m3u8DL-RE --skip-download --write-meta-json`), track selection UI, multi-threaded downloading, and FFmpeg muxing execute natively on the desktop.**

## Alternatives Considered

1. **In-Browser Manifest Parsing & Segment Downloading (Cat Catch Standalone Approach) (`Rejected`):**
   - *Why Rejected:* Constrained by browser 6-connection-per-host limits, risks tab OOM crashes on multi-gigabyte videos, and cannot run native `N_m3u8DL-RE` + `ffmpeg.exe`.
2. **Always-On `MAIN`-World Monkey-Patching on Every Website (`Rejected`):**
   - *Why Rejected:* Overriding `window.fetch` and `XMLHttpRequest` on every webpage (`<all_urls>`) can break strict CSP sites, trigger banking/anti-bot fraud detectors, and degrade page script performance. Making Layer 3 per-site opt-in while keeping Layer 1 (`webRequest`) and Layer 2 (`ISOLATED` world content script) always-on achieves 95%+ site coverage with zero page-context intrusion.
3. **Attempting to Reconstruct Videos Directly from `blob:` URLs via MSE Buffer Recording (`Rejected for MVP`):**
   - *Why Rejected:* As documented in [Cat Catch Cache Capture Docs](https://o2bmm.gitbook.io/cat-catch/docs/popup-1), capturing raw `SourceBuffer.appendBuffer` bytes requires the user to watch the entire video from start to finish without seeking or changing resolution, and corrupts easily if adaptive bitrate switches mid-playback. Discovering the underlying `.m3u8`/`.mpd` manifest via Layer 1 or Layer 3 allows `N_m3u8DL-RE` to download the entire stream at 10x–50x real-time speed.

## Rationale

Separating passive network/DOM discovery in the browser from heavy stream processing in the native desktop app gives users the best of both worlds: effortless one-click browser capture with full native multi-threaded speed and zero browser memory bloat.

## Consequences

- **Positive:** Lightweight extension (`< 15 KB` content script), zero browser tab crashes, full support for multi-gigabyte 4K HLS/DASH streams, and clean handling of cross-origin video iframes.
- **Negative:** Requires `openidm.exe` to be installed for stream downloading (the extension is a companion to `openidm.exe`, not a standalone in-browser downloader).

## Risks & Mitigations

- **Risk (`RISK-004`):** One-time-use manifest URLs fail when re-fetched by `N_m3u8DL-RE.exe`.
- **Mitigation:** Support passing the captured manifest body text from Layer 3 to `openidm.exe`, which writes it to `<task_tmp>/manifest.m3u8` and invokes `N_m3u8DL-RE` with `--base-url <original_manifest_url>`.
