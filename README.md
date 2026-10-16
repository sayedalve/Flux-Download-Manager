# OpenIDM v0.2.0 — Open Source Windows 11 Download & Stream Manager

**Version:** `0.2.0` (Production Release: Windows 11 Win32 UI, Upstream Stream Engine, Native Messaging IPC & Validated Benchmarks)  
**Last Reviewed:** 2026-10-16  
**Target Platform:** Windows 11 `x64` (`MSVC` / Native `Win32` + `WinHTTP` + `Direct2D`/`GDI`) & POSIX/Linux `x86_64`  
**License:** GNU General Public License v3.0 or later (`GPL-3.0-or-later`)  

---

## Overview

OpenIDM is an open-source download manager and browser media capture suite engineered as a fast, lightweight, privacy-respecting native alternative to Internet Download Manager (IDM). It integrates and adapts the core mechanisms of three upstream projects into one coherent application, unified queue, and desktop design system:

1. **[Brisk Download Manager](https://github.com/BrisklyDev/brisk) (`src/brisk_engine/`):** Dynamic connection spawning (`1 -> N`), work-stealing byte-range splitting (`SegmentScheduler::AcquireNextOrStealSegment`), secondary range rejection rollback (`RollbackUnstartedStolenSegment`), automatic stall connection reset, pre-flight disk space verification, and single-stream fallback for non-range servers.
2. **[Cat Catch (`cat-catch`)](https://github.com/xifangczy/cat-catch) & [Brisk Browser Extension](https://github.com/BrisklyDev/brisk-browser-extension) (`extension/`):** Multi-layered `webRequest` media/manifest detection (`.mp4`, `.webm`, `.m3u8` HLS, `.mpd` DASH), master/child playlist deduplication, `.ts`/`.m4s` segment suppression, header correlation (`Referer`, `Origin`, `Cookie`, `Authorization`, `User-Agent`), Manifest V3 Service Worker session state persistence, opt-in `catch-script/catch.js` Deep Search hooks, popup filter tabs/search with browser fallback, and a closed Shadow DOM in-page "Download with OpenIDM" video button with fullscreen/scroll repositioning.
3. **[N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE) (`src/engine_stream/`, `tools/win-x64/N_m3u8DL-RE.exe`):** Managed stream worker subprocess integration inside Windows Job Objects using ACL-restricted `@job.rsp` response files, `--skip-download --write-meta-json` track probing, `-sv`/`-sa`/`-ss` track selection, native in-process EVP/BCrypt AES-128-CBC segment decryption, and [FFmpeg](https://ffmpeg.org/) container muxing (`-M format=mp4|mkv:muxer=ffmpeg`) with container magic-byte verification.

---

## Desktop User Interface (`src/ui/`)

- **Windows 11 Design System (`src/ui/design_system.hpp`):** Deep Charcoal default theme (`#18181B` / `#1F1F23`), Light theme (`#F8FAFC` / `#FFFFFF`), and System theme sync (`DWMWA_USE_IMMERSIVE_DARK_MODE`), 4px spacing grid, `Segoe UI` typography ramp, High-DPI scaling (`100%`, `125%`, `150%`, `200%+`), minimum window dimensions (`900x560`), and geometric vector icons.
- **Main Application Window (`src/ui/win32_main_window.cpp`):**
  - Context-sensitive toolbar (`+ Add URL`, `Start`, `Pause`, `Resume`, `Retry`, `Cancel`, `Remove`, `Clear History`, `Open Folder`, `Settings`).
  - Navigation sidebar with live badge counts across 7 categories (`All Downloads`, `In Progress`, `Queued`, `Completed`, `Failed`, `Video Downloads`, `Scheduled`).
  - Virtualized download list with responsive column hiding (`< 1024px`), multi-column sorting, real-time search, graceful middle-ellipsis filename truncation, honest unknown-size formatting, and granular stream stages (`Probing`, `Downloading video segments`, `Downloading audio segments`, `Merging (FFmpeg)`, `Verifying`, `Completed`, `Failed`).
  - Redacted Details Panel (automatically redacting `Cookie`, `Authorization`, and URL query tokens), helpful Empty States, Status Bar, System Tray icon (`Show`, `Hide`, `Pause All`, `Resume All`, `Exit`), and supporting dialogs.

---

## Automated Test Verification Summary (`39 / 39` Suites Passing on Windows 11)

| Verification Suite | Command | Result | Verified Scope |
| :--- | :--- | :--- | :--- |
| **C++20 Unit & Regression Test Suite** | `.\build\win-x64\Release\openidm_unit_tests.exe` | **PASSED (`12 / 12` suites)** | Path sanitization, dynamic worker discovery & disk space guard, log redaction & DPAPI/AES-256-GCM, Brisk work-stealing & range rollback, SQLite crash recovery, `meta.json` DRM rejection, controller lifecycle & deleted task race, UI design system & High-DPI layout, UI hit-testing/mouse interactions, keyboard navigation, and worker compatibility verification. |
| **Browser Extension Test Suite** | `node tests/test_extension_detection.js` | **PASSED (`8 / 8` suites)** | Direct MP4/WebM detection, Master/Child HLS deduplication, `.ts` segment suppression, DASH `.mpd` discovery, per-site exclusion, Deep Search hook, closed Shadow DOM `<video>` overlay + fullscreen repositioning, MV3 session state persistence, popup filter tabs/search/states, and safe browser download fallback. |
| **End-to-End Integration & Benchmark Suite** | `python tests/run_all_integration_tests.py` | **PASSED (`8 / 8` suites)** | Native PE32+ binary checks, 8-connection segmented download with 302 redirect & RFC 5987 filename, non-range fallback, dropped connection recovery, HLS multi-track probe & FFmpeg muxing, DASH `.mpd` download & muxing, Native Messaging IPC + AES-128 decryption, Desktop UI snapshot, and performance budgets. |
| **Upstream Stream Engine Integration Suite** | `python tests/test_upstream_stream_worker.py` | **PASSED (`11 / 11` suites)** | Upstream `N_m3u8DL-RE.exe` discovery, version 0.3.0 verification, response file argument parsing, CLI flag compatibility, bundled FFmpeg discovery, manifest probing, track selection, progress regex parsing, Job Object sandboxing, container verification, and incompatible worker rejection. |

---

## Measured Performance & Resource Budgets (`benchmarks/latest_metrics.json`)

All metrics measured on a native Windows 11 x64 host:

- **Combined Core Executables (`openidm.exe` + `openidm-nmh.exe`):** `2,740.0 KB` (Budget: $\le 3,072\text{ KB}$).
- **Cold Startup Latency (`openidm.exe --list`):** `32.6 ms` (Budget: $\le 150\text{ ms}$).
- **Idle Daemon Memory Footprint (`openidm.exe --daemon`):** `9.31 MB` Working Set (Budget: $\le 25\text{ MB}$).
- **Peak Memory During 8-Connection 16 MiB HTTP Download:** `13.58 MB` (Budget: $\le 65\text{ MB}$).
- **Peak Stream Worker Memory Footprint:** `10.49 MB` (Budget: $\le 65\text{ MB}$).
- **Large Task List UI Responsiveness (1,000 tasks snapshot):** `102.4 ms` (Budget: $\le 300\text{ ms}$).
- **Loopback Multi-Connection Throughput (8 connections):** `488.4 Mbps` (Floor: $\ge 50\text{ Mbps}$).
- **Residual Temporary Files (`*.openidm.part`):** `0 files`.

---

## Quick Start Commands (Windows 11)

```powershell
# 1. Run the complete automated Windows 11 build, test & packaging pipeline
powershell -ExecutionPolicy Bypass -File scripts/build-windows-msvc.ps1

# 2. Run the C++ unit test suite
.\build\win-x64\Release\openidm_unit_tests.exe

# 3. Run the browser extension test suite
node tests/test_extension_detection.js

# 4. Run the end-to-end integration & benchmark suite
python tests/run_all_integration_tests.py

# 5. Run the dedicated upstream stream worker integration suite
python tests/test_upstream_stream_worker.py

# 6. Run automated benchmarks and persist metrics
python scripts/run-benchmarks.py

# 7. Launch the OpenIDM Desktop Application + IPC Server
.\build\win-x64\Release\openidm.exe

# 8. Render a complete Desktop UI snapshot
.\build\win-x64\Release\openidm.exe --gui-snapshot

# 9. Download a file using 8 dynamic work-stealing connections
.\build\win-x64\Release\openidm.exe --download "https://example.com/file.zip" --out .\downloads --connections 8

# 10. Probe an HLS or DASH stream manifest
.\build\win-x64\Release\openidm.exe --probe-stream "https://example.com/master.m3u8"
```

---

## Release Artifacts (`dist/`)

- **Installer:** `dist/OpenIDM-Setup-x64-0.2.0.exe` (51.93 MB) — Full Inno Setup 6 installer with automatic Native Messaging manifest registration for Chrome, Edge, Brave, and Firefox.
- **Checksums:** `dist/SHA256SUMS.txt` — SHA-256 cryptographic hashes for all executables and installer packages.
- **Standalone Binaries:** `build/win-x64/Release/openidm.exe`, `build/win-x64/Release/openidm-nmh.exe`, `tools/win-x64/N_m3u8DL-RE.exe`, `tools/win-x64/ffmpeg.exe`.

---

## Key Documentation Links

- **[docs/BENCHMARK_REPORT.md](docs/BENCHMARK_REPORT.md):** Detailed benchmark analysis, memory discrepancy root-cause report, and performance comparisons.
- **[docs/RELEASE_READINESS_REPORT.md](docs/RELEASE_READINESS_REPORT.md):** Complete Windows 11 validation log, test matrix, and release readiness audit.
- **[docs/BUG_FIX_LOG.md](docs/BUG_FIX_LOG.md):** Defect catalog and regression fix records for `BUG-001` through `BUG-029`.
- **[docs/PRODUCT_COMPLETION_AUDIT.md](docs/PRODUCT_COMPLETION_AUDIT.md):** Comprehensive engineering audit against requirements.
- **[docs/INDEX.md](docs/INDEX.md):** Master index for all architecture, design, and specification documents.
