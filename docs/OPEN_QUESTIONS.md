# Open Engineering Questions

**Document Status:** Active Decision Log for Unresolved Questions  
**Last Reviewed:** 2026-10-16  
**Scope:** Tracks unresolved technical trade-offs that require empirical benchmarking, legal verification, or maintainer sign-off before locking in final implementation details.  
**Related Documents:** [KNOWN_LIMITATIONS.md](KNOWN_LIMITATIONS.md), [TECHNICAL_RISK_REGISTER.md](TECHNICAL_RISK_REGISTER.md), [ROADMAP.md](ROADMAP.md)

---

## 1. Governance Rule for Open Questions

Per Section 6 of the Master Prompt: **Never silently convert an uncertain decision into a settled architectural fact.** Coding agents encountering any of the questions below must follow the designated **Interim Baseline Behavior** until the required evidence is gathered and the question is formally closed via an ADR update.

---

## 2. Unresolved Questions Register

### `OQ-001`: Custom Minimal `ffmpeg.exe` Build vs. Official Pre-Built `ffmpeg-release-essentials` Binary
- **Importance:** High (Directly determines whether the Full Offline Installer is `~18 MB` or `~34 MB` compressed).
- **Available Options:**
  - *Option A:* Compile a custom minimal `ffmpeg.exe` in GitHub Actions (`--disable-all --enable-avcodec --enable-avformat --enable-protocol=file,http,tcp,pipe,concat --enable-demuxer=mpegts,mov,mp3,aac,matroska,webvtt,srt,concat --enable-muxer=mp4,matroska,mpegts,mp3,adts,srt,webvtt --enable-bsf=aac_adtstoasc,h264_mp4toannexb,hevc_mp4toannexb,extract_extradata`), reducing `ffmpeg.exe` from `~75 MB` to `~15–28 MB` uncompressed (`LGPL-2.1`).
  - *Option B:* Bundle a pinned official build from `gyan.dev` / `BtbN/FFmpeg-Builds` (`ffmpeg.exe` `~75 MB` uncompressed, `~26 MB` LZMA2 compressed, `GPL-3.0`).
- **Evidence Needed:** Benchmark build time and verify that Option A's minimal demuxer/muxer/bitstream-filter set passes 100% of `N_m3u8DL-RE` HLS/DASH/MSS test streams (`IT-STREAM-02`, `IT-STREAM-03`).
- **Impact on Implementation:** Affects only `scripts/fetch-workers.ps1` and `installer/openidm.iss`; zero impact on C++ code.
- **Who / What Resolves It:** Release Engineer during Milestone 3 (`M3`) integration testing.
- **Interim Baseline Behavior:** Use a pinned standard `ffmpeg.exe` binary for `M3` local development, then test the minimal build flags in `M4`.

---

### `OQ-002`: Direct2D vs. GDI+ / Owner-Draw UXTheme for the Per-Segment Progress Bar Control
- **Importance:** Low-Medium (Affects visual polish and GPU vs. CPU rendering of the 32-segment progress bar inside `TaskProgressDialog`).
- **Available Options:**
  - *Option A:* Pure Win32 GDI with double-buffered `CreateCompatibleDC` / `BitBlt` (`0 KB` extra dependency, works over RDP and in basic safe-mode display drivers).
  - *Option B:* `ID2D1HwndRenderTarget` (`Direct2D`, hardware-accelerated anti-aliased rounded rectangles, smooth 120 Hz animation).
- **Evidence Needed:** Compare CPU usage and RDP compatibility at 15 Hz redraw rate during `PERF-NET-01`.
- **Impact on Implementation:** Isolated inside a single control class `src/ui/SegmentProgressBar.cpp`.
- **Who / What Resolves It:** Senior Windows Systems Engineer during Milestone 1 (`M1`).
- **Interim Baseline Behavior:** Implement double-buffered GDI (`CreateCompatibleDC`) in `M1` for maximum simplicity and zero GPU memory overhead; upgrade to `Direct2D` only if GDI exceeds `0.5%` CPU at 15 Hz.

---

### `OQ-003`: Optional External Extractor Plugin Interface (e.g., Optional `yt-dlp.exe` Integration)
- **Importance:** Medium (Determines whether post-MVP OpenIDM can optionally delegate complex video sites like YouTube/Bilibili to an external `yt-dlp.exe` worker).
- **Available Options:**
  - *Option A:* Keep OpenIDM strictly focused on direct HTTP/HTTPS and standard HLS/DASH/MSS (`N_m3u8DL-RE`).
  - *Option B:* Extend `IStreamDownloadEngine` in Milestone 5 to support an optional user-installed `yt-dlp.exe` adapter (`--dump-single-json` for probing, external downloader handoff to OpenIDM or `N_m3u8DL-RE`).
- **Evidence Needed:** Evaluate Chrome Web Store compliance impact (`RISK-007`) and maintenance scope.
- **Impact on Implementation:** None in `M1`–`M4` because `IStreamDownloadEngine` (`NFR-014`) is already designed as an abstract interface.
- **Who / What Resolves It:** Project Maintainer prior to Milestone 5 (`M5`).
- **Interim Baseline Behavior:** Out of scope for MVP (`M1`–`M4`).

---

### `OQ-004`: Experimental MSE `SourceBuffer.appendBuffer` In-Browser Recording Fallback
- **Importance:** Low (Applies only to niche non-DRM sites that stream video via custom binary WebSockets into MSE without an `.m3u8` or `.mpd` manifest).
- **Available Options:**
  - *Option A:* Do not implement in-browser `SourceBuffer` recording (avoids tab OOM crashes and corrupted buffers when users seek or switch quality).
  - *Option B:* Stream captured `SourceBuffer` chunks immediately over the Native Messaging port (`stream.appendChunk`) to `openidm.exe` so chunks are written to disk rather than held in browser RAM.
- **Evidence Needed:** Measure Native Messaging `stdio` throughput ceiling (Chromium caps single messages at `1 MiB`, though streaming `256 KiB` chunks at `5–10 MB/s` is feasible).
- **Impact on Implementation:** Would add `stream.appendChunk` to [NATIVE_MESSAGING_AND_IPC.md](NATIVE_MESSAGING_AND_IPC.md).
- **Who / What Resolves It:** Browser Extension Engineer in Milestone 5 (`M5`).
- **Interim Baseline Behavior:** Explicitly unsupported in MVP (`KNOWN_LIMITATIONS.md`).
