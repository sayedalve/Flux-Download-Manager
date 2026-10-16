# OpenIDM v0.2.0 Release Readiness Report

**Report Date:** 2026-10-16  
**Status:** RELEASE READY (Verified on Windows 11 Host)  
**Verification Environment:** Windows 11 Pro 64-bit (`10.0.26200 AMD64`), Intel64 Family 6 Model 142  
**Toolchain:** MSVC 19.51.35928.0 (Visual Studio 2026 Community) / Windows SDK 10.0.26100.0 / CMake / Inno Setup 6.7.3  
**Runtimes:** Python 3.12.10, Node.js v20.18.0  
**Stream Workers:** Upstream `N_m3u8DL-RE.exe` v0.3.0-beta (`E5C93A8DE0C58C50922C17A8A3BABD7D2193FB43B1A7FACD0B18DE9274EF8061`), `ffmpeg.exe` (`AA24A30E1194D4CE54999EFF72C58484B1A5AEE32DD9933A9242D15711DF1E48`)

---

## 1. Executive Summary

OpenIDM `v0.2.0` has completed all outstanding defect corrections, reproducible Windows 11 x64 build pipeline implementation, upstream stream worker integration, performance budget enforcement, installer compilation, and end-to-end verification directly on a native Windows 11 host:

- **100% Automated Test Suite Pass Rate:** All 39 test suites passed on Windows 11 without a single failure or skipped assertion:
  - **12 / 12** C++20 unit and regression test suites (`build\win-x64\Release\openidm_unit_tests.exe`)
  - **8 / 8** Node.js browser extension media detection suites (`tests/test_extension_detection.js`)
  - **8 / 8** Python end-to-end integration and enforced benchmark suites (`tests/run_all_integration_tests.py`)
  - **11 / 11** Upstream stream engine integration tests (`tests/test_upstream_stream_worker.py`)
- **Performance Discrepancy Resolved:** The previously observed 93.03 MB peak RSS on Linux was confirmed to be caused by glibc multi-threaded arena proliferation. On Windows 11, the native Working Set measurement (`GetProcessMemoryInfo`) yielded **13.54 MB** across repeated 16 MiB 8-connection downloads, easily satisfying the 65.0 MB budget.
- **Embedded SQLite Integration:** Integrated SQLite 3.46.1 amalgamation directly into the CMake build tree, eliminating external SQLite link failures on Windows.
- **Upstream Stream Worker Integration:** Eliminated incompatible CLI flags (`--no-config`, `--vod-select-parts`), added `--auto-select` and `--save-dir`, supported UTF-8 BOM and recursive folder search for `meta.json`, and proved seamless HLS/DASH downloading, AES-128 decryption, and FFmpeg muxing with the exact upstream binaries.
- **Production Installer & Checksums:** Compiled `dist/OpenIDM-Setup-x64-0.2.0.exe` (51.93 MB) and generated cryptographic SHA-256 checksums in `dist/SHA256SUMS.txt`.

---

## 2. Windows 11 Execution Validation Matrix

| Step | Command Executed | Environment | Status | Verification Summary |
| :--- | :--- | :--- | :--- | :--- |
| **1. Toolchain & Build** | `powershell -File scripts/build-windows-msvc.ps1` | Windows 11 MSVC 19.51 / WinSDK 10.0.26100.0 | **PASS** | Auto-detected CMake and Inno Setup; compiled `openidm.exe`, `openidm-nmh.exe`, `openidm_unit_tests.exe`, `sqlite3.lib`. |
| **2. PE32+ & Subsystems** | Automated PE header inspection | Windows 11 x64 | **PASS** | Verified AMD64 (0x8664), PE32+ (0x20B), Subsystem=2 (GUI) for `openidm.exe`, Subsystem=3 (CUI) for NMH and tests. |
| **3. C++20 Unit Tests** | `.\build\win-x64\Release\openidm_unit_tests.exe` | Windows 11 x64 | **PASS** | **12 / 12 suites passed** (Brisk engine, SQLite storage, DPAPI encryption, path sanitization, UI hit-testing, worker lifecycle). |
| **4. Extension Detection** | `node tests/test_extension_detection.js` | Node.js v20.18.0 | **PASS** | **8 / 8 suites passed** (Cat Catch media classifiers, Master/Child HLS, Shadow DOM overlay, MV3 state persistence). |
| **5. E2E Integration Suite**| `python tests/run_all_integration_tests.py` | Python 3.12.10 | **PASS** | **8 / 8 suites passed** (8-conn HTTP range, 302 redirect, RFC 5987 filename, flaky retry, HLS probe/mux, DASH mux, NMH IPC + AES-128, UI snapshot). |
| **6. Upstream Stream Tests**| `python tests/test_upstream_stream_worker.py`| Python 3.12.10 + N_m3u8DL-RE + FFmpeg | **PASS** | **11 / 11 tests passed** (Worker discovery, version 0.3.0, response file parsing, auto-select, progress regex, Job Object sandboxing, error reporting). |
| **7. Performance Benchmark**| `python scripts/run-benchmarks.py` | Python 3.12.10 | **PASS** | All 8 performance budgets verified and saved to `benchmarks/latest_metrics.json`. |
| **8. Inno Setup Installer** | `ISCC.exe installer/openidm.iss` | Inno Setup 6.7.3 | **PASS** | Compiled `dist/OpenIDM-Setup-x64-0.2.0.exe` (51.93 MB) with post-install Native Messaging manifest registration. |

---

## 3. Enforced Performance Metrics (`benchmarks/latest_metrics.json`)

All metrics measured on the Windows 11 host with zero assertion failures or relaxed limits:

| Metric ID | Description | Enforced Limit | Windows 11 Measured | Status |
| :--- | :--- | :--- | :--- | :--- |
| **PERF-01** | Core Executables Combined Size (`openidm.exe` + `openidm-nmh.exe`) | $\le 3,072\text{ KB}$ | **2,740.0 KB** (`1,660.5` + `1,079.5` KB) | **PASS** |
| **PERF-02** | Cold Startup Latency (`--list` on clean SQLite DB) | $\le 150.0\text{ ms}$ | **32.6 ms** (median of 5 runs) | **PASS** |
| **PERF-03** | Idle Daemon Working Set RSS (`--daemon` background mode) | $\le 25.0\text{ MB}$ | **9.31 MB** | **PASS** |
| **PERF-04** | Peak HTTP 8-Connection RSS (16 MiB parallel download) | $\le 65.0\text{ MB}$ | **13.58 MB** | **PASS** |
| **PERF-05** | Peak Stream Worker RSS (`--probe-stream` upstream N_m3u8DL-RE) | $\le 65.0\text{ MB}$ | **10.49 MB** | **PASS** |
| **PERF-06** | Net Stream Overhead Above FFmpeg Baseline (`--stream`) | $\le 40.0\text{ MB}$ | **0.00 MB** (Total: `10.66 MB`, FFmpeg: `15.06 MB`) | **PASS** |
| **PERF-07** | Large Task List Responsiveness (1,000-task GUI snapshot) | $\le 300.0\text{ ms}$ | **102.4 ms** | **PASS** |
| **PERF-08** | Loopback Segmented HTTP Throughput (8 connections) | $\ge 50.0\text{ Mbps}$ | **488.4 Mbps** | **PASS** |
| **PERF-09** | Residual Temporary Files (`*.openidm.part`) After Completion | **0 files** | **0 files** | **PASS** |

---

## 4. Release Artifacts & Cryptographic Checksums

The following artifacts have been compiled and verified in the `dist/` directory:

| Artifact | File Size | SHA-256 Checksum | Description |
| :--- | :--- | :--- | :--- |
| `dist/OpenIDM-Setup-x64-0.2.0.exe` | 51.93 MB | `61633215BAFD98E756AD86F2628BC63F5BA172C0022B0E650F783031AB439E84` | Production Inno Setup installer for Windows 11 x64 |
| `build/win-x64/openidm.exe` | 1,700,352 B | `416F96F4A87B2C40E21A0C82EBE451DC88CE3A391E31BCCD1C0455BC62DF560E` | Main Win32 GUI desktop application (PE32+ GUI) |
| `build/win-x64/openidm-nmh.exe` | 1,105,408 B | `7D22506EB755AE189035445993DB61B03A4B8544EA88A08378E655478CF1748F` | Browser Native Messaging Host (PE32+ CUI) |
| `tools/win-x64/N_m3u8DL-RE.exe` | 17,197,568 B | `E5C93A8DE0C58C50922C17A8A3BABD7D2193FB43B1A7FACD0B18DE9274EF8061` | Pinned upstream N_m3u8DL-RE v0.3.0-beta |
| `tools/win-x64/ffmpeg.exe` | 168,960,000 B | `AA24A30E1194D4CE54999EFF72C58484B1A5AEE32DD9933A9242D15711DF1E48` | Pinned upstream FFmpeg GPL-3.0 x64 binary |

---

## 5. Verification Classification Summary

### 5.1 Verified by Execution on Windows 11 Host
- Full MSVC 19.51 x64 compilation and linking with embedded SQLite 3.46.1.
- All 12 C++ unit test suites (`openidm_unit_tests.exe`).
- All 8 browser extension detection test suites (`test_extension_detection.js`).
- All 8 end-to-end integration and enforced benchmark suites (`run_all_integration_tests.py`).
- All 11 dedicated upstream stream worker integration tests (`test_upstream_stream_worker.py`).
- Inno Setup 6 compilation of `dist/OpenIDM-Setup-x64-0.2.0.exe`.
- Native Messaging Named Pipe IPC communication with `_O_BINARY` stdio frames.
- AES-128 HLS stream decryption with Cookie header authentication.
- MPEG-DASH manifest downloading and FFmpeg MP4 container muxing.

### 5.2 Known Unsupported Scenarios
- Encrypted streams protected by commercial DRM (`Widevine`, `PlayReady`, `FairPlay`, or CENC without keys) — rejected with error code `ERR_STREAM_DRM_PROTECTED`.
