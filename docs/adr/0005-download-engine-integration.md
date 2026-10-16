# ADR-0005: Ordinary and Stream Download Engine Integration Strategy

- **Date:** 2026-10-16
- **Status:** Proposed
- **Deciders:** Download Engine Specialist, Principal Software Architect
- **Related Documents:** [../DOWNLOAD_ENGINE_DESIGN.md](../DOWNLOAD_ENGINE_DESIGN.md), [../STREAM_DOWNLOAD_PIPELINE.md](../STREAM_DOWNLOAD_PIPELINE.md), [../UPSTREAM_PROJECT_ANALYSIS.md](../UPSTREAM_PROJECT_ANALYSIS.md)

---

## Status

**Proposed** (Baseline Engine Decision for Milestones 1 & 3).

## Context

OpenIDM requires two specialized download engines:
1. **Ordinary File Download Engine:** For single-file HTTP/HTTPS transfers (`.zip`, `.iso`, `.exe`, progressive `.mp4`), requiring dynamic connection spawning, work-stealing byte-range splitting, stall reset, and crash-safe resume (capabilities demonstrated in [Brisk](https://github.com/BrisklyDev/brisk)).
2. **Adaptive Stream Download Engine:** For HLS (`.m3u8`), DASH (`.mpd`), and MSS (`.ism`) manifests requiring variant parsing, track selection, concurrent segment downloading, and container muxing (capabilities demonstrated in [N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE)).

We must decide how to integrate or implement these engines within our C++20/Win32 architecture.

## Decision

1. **Ordinary File Engine (`IFileDownloadEngine` -> `WinHttpDownloadEngine`):**
   - Implement **natively in C++20 inside `openidm.exe`** on top of the Windows `WinHTTP` asynchronous API and Windows I/O Completion Ports (IOCP).
   - **Adapt Brisk's core algorithms cleanly without embedding Dart/Flutter:**
     - *Dynamic Connection Spawning:* Start with 1 connection (`Segment 0` covering `[0, total_size - 1]`). Once `206 Partial Content` is verified and throughput is flowing, progressively spawn additional workers up to `max_connections` (default `8`).
     - *Dynamic Connection Reuse (Work-Stealing Split):* Whenever a worker completes its assigned byte range, find the active segment with the largest remaining byte count (`remaining >= 2 * min_split_size`, where `min_split_size = 1 MiB`), atomically truncate that active worker's `end_offset` to the midpoint `M`, and assign `[M + 1, old_end]` to the idle worker.
     - *Automatic Stall Reset:* Per-worker sliding-window timer resets and reconnects any individual stalled byte-range worker at its exact `current_offset`.
2. **Stream Download Engine (`IStreamDownloadEngine` -> `Nm3u8dlReAdapter`):**
   - Integrate **[N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE)** (`v0.6.0-beta`, MIT License) as an **unmodified out-of-process worker executable (`N_m3u8DL-RE.exe`)** paired with `ffmpeg.exe`.
   - Use a deterministic two-phase invocation contract:
     - **Phase 1 (Probe):** Invoke `N_m3u8DL-RE.exe <url> --skip-download --write-meta-json --tmp-dir <probe_dir> --no-ansi-color --no-config --disable-update-check --vod-select-parts false @<headers.rsp>`, then parse `<probe_dir>/meta.json` in C++ to populate the Win32 Track Selection dialog.
     - **Phase 2 (Download & Mux):** Invoke `N_m3u8DL-RE.exe <url> --tmp-dir <task_tmp> --save-dir <dest_dir> --save-name <temp_stem> -sv <video_filter> -sa <audio_filter> -ss <sub_filter> -mt --thread-count <N> --ffmpeg-binary-path <ffmpeg_path> -M format=<mp4|mkv>:muxer=ffmpeg:bin_path=<ffmpeg_path> --no-ansi-color --no-config --disable-update-check --vod-select-parts false @<headers.rsp>`, parsing line-buffered stdout/stderr progress into `StreamProgressEvent` updates.

## Alternatives Considered

1. **Embedding `brisk_download_engine` via Dart AOT / Flutter Engine (`Rejected`):**
   - *Why Rejected:* `brisk_download_engine` is written in pure Dart using Dart's `http` library. Embedding the Dart VM adds `20–40 MB` of runtime overhead and FFI marshaling complexity for functionality (`HTTP Range GET` + file offset writes) that takes ~1,200 lines of clean C++20 over `WinHTTP`.
2. **Embedding `aria2c.exe` for Ordinary File Downloads (`Rejected`):**
   - *Why Rejected:* Unlike `WinHTTP` with Brisk-style dynamic work-stealing, `aria2c` uses static initial piece splitting with a fixed `--min-split-size` and struggles to dynamically steal sub-megabyte tail ranges when one CDN edge node is slow. Writing `WinHttpDownloadEngine` in-process also gives us native Windows proxy (`WPAD`), OS certificate store (`Schannel`), and sub-second SQLite segment checkpointing.
3. **Rewriting `N_m3u8DL-RE` in C++ from Scratch (`Rejected`):**
   - *Why Rejected:* `N_m3u8DL-RE` represents 4+ years and 600+ commits of battle-tested HLS, DASH, and MSS manifest parsing, discontinuity handling, subtitle timeline fixing, and `LOCAL_HTTP` FFmpeg concatenation. Rewriting it in C++ would violate Section 5 of the Master Prompt (*"Do not fork or rewrite an entire component simply because its implementation language differs from the preferred desktop language"*).

## Rationale

- `WinHTTP` in C++20 provides the smallest, fastest, most OS-integrated engine for ordinary files while implementing all of Brisk's algorithmic advantages.
- Wrapping `N_m3u8DL-RE.exe` via `IStreamDownloadEngine` leverages the best open-source stream downloader on Windows without code duplication or license friction.

## Consequences

- **Positive:** Both engines sit behind pure C++ interfaces (`IFileDownloadEngine`, `IStreamDownloadEngine`), allowing mock-based unit testing (`UT-CORE-01`) and future engine additions (e.g., an optional `BrisklyDev/kasma` Rust adapter if Kasma matures).
- **Negative:** Requires parsing `N_m3u8DL-RE`'s `meta.json` and stdout progress lines, meaning `N_m3u8DL-RE.exe` versions must be pinned and verified by integration tests (`IT-STREAM-01`..`04`) before upgrading.

## Risks & Mitigations

- **Risk:** Upstream `N_m3u8DL-RE` changes CLI flag syntax or stdout progress format in a future version.
- **Mitigation:** Pin exact `N_m3u8DL-RE` release version and SHA-256 hash in `scripts/fetch-workers.ps1`, isolate all CLI formatting and stdout regexes inside `src/engine_stream/Nm3u8dlReAdapter.cpp`, and run automated contract tests before bumping the pinned version.
