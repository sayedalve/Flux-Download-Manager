# Flux Download Manager v0.3.0 — Native Windows 11 Download & Stream Manager

**Version:** `0.3.0` (Production Release: Windows 11 Win32 UI, Segmented HTTP Engine, Upstream Stream Engine, Native Messaging IPC & Validated Benchmarks)  
**Short Name:** `Flux`  
**Last Reviewed:** 2026-10-16  
**Target Platform:** Windows 11 `x64` (`MSVC` / Native `Win32` + `WinHTTP` + `Direct2D`/`GDI`) & POSIX/Linux `x86_64`  
**License:** GNU General Public License v3.0 or later (`GPL-3.0-or-later`)  

---

## Overview

**Flux Download Manager** (short name: **Flux**) is an open-source download manager and browser media capture application engineered as a high-performance, lightweight, privacy-respecting native alternative to Internet Download Manager (IDM). It integrates the verified core mechanisms of three upstream projects into one coherent application, unified queue, and modern Windows 11 desktop design system:

1. **[Brisk Download Manager](https://github.com/BrisklyDev/brisk) (`src/brisk_engine/`):** Dynamic connection spawning (`1 -> N`), work-stealing byte-range splitting (`SegmentScheduler::AcquireNextOrStealSegment`), secondary range rejection rollback (`RollbackUnstartedStolenSegment`), automatic stall connection reset, pre-flight disk space verification, and single-stream fallback for non-range servers.
2. **[Cat Catch (`cat-catch`)](https://github.com/xifangczy/cat-catch) & [Brisk Browser Extension](https://github.com/BrisklyDev/brisk-browser-extension) (`extension/`):** Multi-layered `webRequest` media and manifest detection (`.mp4`, `.webm`, `.m3u8` HLS, `.mpd` DASH), master/child playlist deduplication, `.ts`/`.m4s` segment suppression, header correlation (`Referer`, `Origin`, `Cookie`, `Authorization`, `User-Agent`), Manifest V3 Service Worker session state persistence, opt-in `catch-script/catch.js` Deep Search hooks, popup filter tabs/search with browser fallback, and a closed Shadow DOM in-page "Download with Flux" video button with fullscreen and scroll repositioning.
3. **[N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE) (`src/engine_stream/`, `tools/win-x64/N_m3u8DL-RE.exe`):** Managed stream worker subprocess integration inside Windows Job Objects using ACL-restricted `@job.rsp` response files, `--skip-download --write-meta-json` track probing, `-sv`/`-sa`/`-ss` track selection, native in-process EVP/BCrypt AES-128-CBC segment decryption, and [FFmpeg](https://ffmpeg.org/) container muxing (`-M format=mp4|mkv:muxer=ffmpeg`) with container magic-byte verification.

---

## Desktop User Interface (`src/ui/`)

- **Windows 11 Design System (`src/ui/design_system.hpp`, `docs/FLUX_BRANDING.md`):** Deep Charcoal dark theme (`#0F172A` Slate 900 background, `#1E293B` Slate 800 surface, `#334155` border) with Electric Blue primary accent (`#2563EB` / `#3B82F6`), clean light theme (`#F8FAFC` / `#FFFFFF`), and System theme sync (`DWMWA_USE_IMMERSIVE_DARK_MODE`), 4px spacing grid, `Segoe UI Variable Text` typography ramp, High-DPI scaling (`100%`, `125%`, `150%`, `200%+`), minimum window dimensions (`900x560`), and geometric vector icons.
- **Main Application Window (`src/ui/win32_main_window.cpp`):**
  - Context-sensitive toolbar (`+ Add URL`, `Start`, `Pause`, `Resume`, `Retry`, `Cancel`, `Remove`, `Clear History`, `Open Folder`, `Settings`).
  - Navigation sidebar with live badge counts across 7 categories (`All Downloads`, `In Progress`, `Queued`, `Completed`, `Failed`, `Video Downloads`, `Scheduled`).
  - Virtualized download list with responsive column hiding (`< 1024px`), multi-column sorting, real-time search, graceful middle-ellipsis filename truncation, honest unknown-size formatting, and granular stream stages (`Probing`, `Downloading video segments`, `Downloading audio segments`, `Merging (FFmpeg)`, `Verifying`, `Completed`, `Failed`).
  - Redacted Details Panel (automatically redacting `Cookie`, `Authorization`, and URL query tokens), helpful Empty States, Status Bar, System Tray icon (`Show`, `Hide`, `Pause All`, `Resume All`, `Exit`), and supporting dialogs (Add Download, Settings, Browser Extension Integration, About Flux).

---

## Automated Test Verification Summary (`67 / 67` Suites & Fixtures Passing on Windows 11)

| Verification Suite | Command | Result | Verified Scope |
| :--- | :--- | :--- | :--- |
| **C++20 Unit & Regression Test Suite** | `.\build\win-x64\Release\flux_unit_tests.exe` | **PASSED (`27 / 27` suites)** | Path sanitization, dynamic worker discovery, disk space guard, log redaction, DPAPI/AES-256-GCM, Brisk work-stealing & range rollback, SQLite crash recovery, DRM rejection, controller lifecycle, UI design system & High-DPI layout, hit-testing & mouse interactions, keyboard navigation, worker compatibility, part-file isolation & atomic non-overwriting commit, resource-aware resume & validator mismatch, boundary split race guard, IPC security DACL & path traversal, concurrent global rate limiting, relative/protocol-relative redirect resolution, complete segment range map partition validation, POSIX HTTP chunked decoder, URL parsing & SSRF security checks, IPC clean shutdown & first pipe instance, conflict policies (AutoRename/Overwrite/FailIfExists), and system suspend/resume lifecycle. |
| **Browser Extension Test Suite** | `node tests/test_extension_detection.js` | **PASSED (`11 / 11` suites)** | Direct MP4/WebM detection, Master/Child HLS deduplication, `.ts` segment suppression, DASH `.mpd` discovery, per-site exclusion, Deep Search hook, closed Shadow DOM `<video>` overlay + fullscreen repositioning, MV3 session state persistence, popup filter tabs/search/states, link-click interception & Alt-key bypass, watch URL guard & message routing, and cosmic dark theme styling. |
| **End-to-End Integration & Benchmark Suite** | `python tests/run_all_integration_tests.py` | **PASSED (`8 / 8` suites)** | Native PE32+ binary checks, 8-connection segmented download with 302 redirect & RFC 5987 filename, non-range fallback, dropped connection recovery, HLS multi-track probe & FFmpeg muxing, DASH `.mpd` download & muxing, Native Messaging IPC (`flux-nmh.exe`) + AES-128 decryption, Desktop UI snapshot, and performance budgets. |
| **Upstream Stream Engine Integration Suite** | `python tests/test_upstream_stream_worker.py` | **PASSED (`11 / 11` suites)** | Upstream `N_m3u8DL-RE.exe` discovery, version 0.3.0 verification, response file argument parsing, CLI flag compatibility, bundled FFmpeg discovery, manifest probing, track selection, progress regex parsing, Job Object sandboxing, container verification, and incompatible worker rejection. |
| **Deterministic E2E Repair Pipeline Suite** | `python tests/test_repair_pipeline_e2e.py` | **PASSED (`10 / 10` fixtures)** | Direct PDF, direct ZIP + Alt-bypass, 302 redirect + Content-Disposition correlation, direct MP4 video, WebM video, dynamically inserted `<video>` & real Chromium DOM verification, HLS master playlist variant grouping & muxing, DASH MPD muxing, blob: URL discovery & unbacked error handling, and YouTube extraction routing & adaptive muxing. |

---

## Measured Performance & Resource Budgets (`benchmarks/latest_metrics.json`)

All metrics measured on a native Windows 11 x64 host:

- **Combined Core Executables (`flux.exe` + `flux-nmh.exe`):** `2,743.5 KB` (Budget: $\le 3,072\text{ KB}$).
- **Cold Startup Latency (`flux.exe --list`):** `42.98 ms` (Budget: $\le 150\text{ ms}$).
- **Idle Daemon Memory Footprint (`flux.exe --daemon`):** `9.27 MB` Working Set (Budget: $\le 25\text{ MB}$).
- **Peak Memory During 8-Connection 16 MiB HTTP Download:** `13.57 MB` (Budget: $\le 65\text{ MB}$).
- **Peak Stream Worker Memory Footprint:** `10.41 MB` (Budget: $\le 65\text{ MB}$).
- **Large Task List UI Responsiveness (1,000 tasks snapshot):** `182.63 ms` (Budget: $\le 300\text{ ms}$).
- **Loopback Multi-Connection Throughput (8 connections):** `344.1 Mbps` (Floor: $\ge 50\text{ Mbps}$).
- **Residual Temporary Files (`*.flux.part` / `*.openidm.part`):** `0 files`.

---

## Quick Start Commands (Windows 11)

```powershell
# 1. Run the complete automated Windows 11 build, test & packaging pipeline
powershell -ExecutionPolicy Bypass -File scripts/build-windows-msvc.ps1

# 2. Run the C++20 unit test suite
.\build\win-x64\Release\flux_unit_tests.exe

# 3. Run the browser extension test suite
node tests/test_extension_detection.js

# 4. Run the end-to-end integration & benchmark suite
python tests/run_all_integration_tests.py

# 5. Run the dedicated upstream stream worker integration suite
python tests/test_upstream_stream_worker.py

# 6. Run automated benchmarks and persist metrics
python scripts/run-benchmarks.py

# 7. Launch the Flux Desktop Application + IPC Server
.\build\win-x64\Release\flux.exe

# 8. Render a complete Desktop UI snapshot
.\build\win-x64\Release\flux.exe --gui-snapshot

# 9. Download a file using 8 dynamic work-stealing connections
.\build\win-x64\Release\flux.exe --download "https://example.com/file.zip" --out .\downloads --connections 8

# 10. Probe an HLS or DASH stream manifest
.\build\win-x64\Release\flux.exe --probe-stream "https://example.com/master.m3u8"
```

---

## Release Artifacts (`dist/`)

- **Installer:** `dist/Flux-Setup-x64-0.3.0.exe` (51.95 MB) — Inno Setup 6 installer with automatic Native Messaging manifest registration for Google Chrome, Microsoft Edge, Brave, and Mozilla Firefox.
- **Portable Archive:** `dist/Flux-v0.3.0-Windows-x64.zip` (201.8 MB) — Standalone portable archive containing `flux.exe`, `flux-nmh.exe`, `tools/win-x64/` (N_m3u8DL-RE + FFmpeg), browser extension, manifests, and documentation.
- **Cryptographic Checksums:** `dist/SHA256SUMS.txt` — SHA-256 cryptographic hashes for all executables, installer packages, and release archives.
- **Standalone Binaries:** `build/win-x64/Release/flux.exe`, `build/win-x64/Release/flux-nmh.exe`, `tools/win-x64/N_m3u8DL-RE.exe`, `tools/win-x64/ffmpeg.exe`.

---

## Key Documentation Links

- **[docs/FLUX_BRANDING.md](docs/FLUX_BRANDING.md):** Complete Flux visual identity, color tokens, typography ramp, icon design, and branding guidelines.
- **[docs/IDM_AUDIT_IMPLEMENTATION_MAP.md](docs/IDM_AUDIT_IMPLEMENTATION_MAP.md):** Reverse engineering evidence mapping from IDM audit to Flux components and test suites.
- **[docs/UPSTREAM_INTEGRATION_MAP.md](docs/UPSTREAM_INTEGRATION_MAP.md):** Source traceability matrix mapping reused algorithms from Brisk, Cat Catch, and N_m3u8DL-RE.
- **[docs/CURRENT_STATE_AUDIT.md](docs/CURRENT_STATE_AUDIT.md):** Initial repository audit, architectural assessment, defect catalog, and 10-phase execution plan.
- **[docs/BENCHMARK_REPORT.md](docs/BENCHMARK_REPORT.md):** Detailed benchmark analysis, memory footprint measurements, and performance comparisons.
- **[docs/INDEX.md](docs/INDEX.md):** Master index for all architecture, design, and specification documents.
