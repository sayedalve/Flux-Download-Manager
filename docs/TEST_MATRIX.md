# Requirement-to-Test Verification Matrix

**Document Status:** Active Verification Matrix (`v0.2.0` — `26 / 26` Automated Suites Executed & Passed)  
**Last Reviewed:** 2026-10-16  
**Scope:** Maps every functional (`FR-001`–`FR-028`), non-functional (`NFR-001`–`NFR-016`), and bug-regression (`BUG-001`–`BUG-014`) test to its exact command, verification category, and outcome.  
**Related Documents:** [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md), [BUG_FIX_LOG.md](BUG_FIX_LOG.md), [FEATURE_MATRIX.md](FEATURE_MATRIX.md)

---

## 1. Automated Verification Summary (`make -f Makefile test`)

### 1.1 C++20 Unit & Regression Test Suites (`./build/openidm_unit_tests` — `10 / 10 PASSED`)

| Suite # | Suite Function (`tests/cpp/unit_tests.cpp`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/10]`** | `TestPathSanitizerAndContentDisposition` | `FR-001`, `FR-026`, `NFR-006`, `NFR-012`, `SEC-PATH-01` | `Verified by Execution` | **PASSED** |
| **`[2/10]`** | `TestDynamicWorkerAndDiskSpaceGuard` | `FR-020`, `FR-026`, `BUG-005`, `BUG-009` | `Verified by Execution` | **PASSED** |
| **`[3/10]`** | `TestLoggerRedactionAndCrypto` | `FR-007`, `NFR-009`, `SEC-PRIV-01`, `UT-HASH-01` | `Verified by Execution` | **PASSED** |
| **`[4/10]`** | `TestBriskSegmentSchedulerWorkStealingAndRollback` | `FR-003`, `FR-004`, `UT-SEG-01`, `BUG-008` | `Verified by Execution` | **PASSED** |
| **`[5/10]`** | `TestSqliteStoreAndCrashRecovery` | `FR-005`, `NFR-005`, `NFR-009`, `IT-CRASH-01` | `Verified by Execution` | **PASSED** |
| **`[6/10]`** | `TestStreamMetaJsonAndDrmRejection` | `FR-019`, `NFR-015`, `SEC-DRM-01`, `THREAT-004` | `Verified by Execution` | **PASSED** |
| **`[7/10]`** | `TestControllerLifecycleAndDeletedTaskRace` | `FR-023`, `BUG-007`, `BUG-010` | `Verified by Execution` | **PASSED** |
| **`[8/10]`** | `TestDesktopUiDesignSystemAndHighDpiLayout` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |
| **`[9/10]`** | `TestDesktopUiInteractionsFilteringSortingAndRedaction` | `FR-011`, `FR-023`–`FR-025`, `NFR-009`, `BUG-001`, `BUG-010`, `BUG-011` | `Verified by Execution` | **PASSED** |
| **`[10/10]`** | `TestKeyboardNavigationAndShortcuts` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |

---

### 1.2 Browser Extension & Media Detection Test Suites (`node tests/test_extension_detection.js` — `8 / 8 PASSED`)

| Suite # | Suite Description (`tests/test_extension_detection.js`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/8]`** | Direct MP4/WebM & HLS/DASH Detection + Header Correlation | `FR-014` | `Verified by Execution` | **PASSED** |
| **`[2/8]`** | Master vs. Child HLS Deduplication & `.ts` Segment Suppression | `FR-014`, `FR-016` | `Verified by Execution` | **PASSED** |
| **`[3/8]`** | Per-Site Exclusion Setting & Popup Rendering | `FR-009`, `FR-014` | `Verified by Execution` | **PASSED** |
| **`[4/8]`** | Page-Context Deep Search Hook (Obfuscated Manifest Capture + Nonce) | `FR-013`, `FR-017` | `Verified by Execution` | **PASSED** |
| **`[5/8]`** | In-Page Closed Shadow DOM Video Overlay & Fullscreen Repositioning | `FR-015`, `BUG-013` | `Verified by Execution` | **PASSED** |
| **`[6/8]`** | Download Interception, Bypass Modifier & Safe Browser Fallback | `FR-008`, `FR-009`, `FR-012` | `Verified by Execution` | **PASSED** |
| **`[7/8]`** | Manifest V3 Service Worker Session State Persistence & Recovery | `FR-014`, `RISK-002`, `BUG-013` | `Verified by Execution` | **PASSED** |
| **`[8/8]`** | Popup Filter Tabs, Search, Disconnected States & Direct Media Fallback | `FR-008`, `FR-014`, `BUG-012` | `Verified by Execution` | **PASSED** |

---

### 1.3 End-to-End Integration, Stream Pipeline & Benchmark Suites (`python3 tests/run_all_integration_tests.py` — `8 / 8 PASSED`)

| Suite # | Test ID & Description (`tests/run_all_integration_tests.py`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/8]`** | `IT-BIN-01`: Verify `tools/N_m3u8DL-RE` is a Compiled Native C++20 Executable | `FR-020`, `BUG-006` | `Verified by Execution` | **PASSED** |
| **`[2/8]`** | `IT-HTTP-01` & `IT-HTTP-03`: Segmented HTTP Range Download (8 Connections + 302 Redirect + RFC 5987) | `FR-001`, `FR-003` | `Verified by Execution` | **PASSED** |
| **`[3/8]`** | `IT-HTTP-02`: Non-Range Server Single-Stream Fallback | `FR-002`, `BUG-008` | `Verified by Execution` | **PASSED** |
| **`[4/8]`** | `IT-HTTP-04`: Dropped Connection Reset & Resume from Exact Byte Offset | `FR-004` | `Verified by Execution` | **PASSED** |
| **`[5/8]`** | `IT-STREAM-01` & `IT-STREAM-02`: HLS Master Manifest Probe & FFmpeg Container Muxing | `FR-019`, `FR-020` | `Verified by Execution` | **PASSED** |
| **`[6/8]`** | `IT-STREAM-03`: MPEG-DASH (`.mpd`) Manifest Download & FFmpeg Muxing | `FR-020` | `Verified by Execution` | **PASSED** |
| **`[7/8]`** | `IT-IPC-01`, `IT-STREAM-06` & `IT-UI-01`: Native Messaging Host, AES-128 Decryption, Log Redaction & Desktop UI Snapshot | `FR-021`, `FR-022`, `NFR-007`–`NFR-009`, `BUG-002` | `Verified by Execution` | **PASSED** |
| **`[8/8]`** | `PERF-BENCH-01`: Binary Size (`1,274.2 KB`), Cold Startup (`27.93 ms`), Throughput (`216.1 Mbps`) & Peak RSS (`29.92 MB`) | `NFR-001`–`NFR-004` | `Verified by Execution` | **PASSED** |

---

## 2. Host-Dependent Tests Requiring a Windows 11 `x64` Workstation

| Test ID | Mapped Req IDs | Target Environment | Verification Procedure | Current Status |
| :--- | :--- | :--- | :--- | :--- |
| **`SYS-WIN-BUILD-01`** | `FR-027`, `NFR-001` | Windows 11 `x64` + VS 2022 MSVC | Run `.\scripts\build-windows-msvc.ps1` and verify `MZ` / PE32+ headers on `openidm.exe`, `openidm-nmh.exe`, and `N_m3u8DL-RE.exe`. | `Not Yet Verified (Requires Win11 Host)` |
| **`SYS-INST-01..02`** | `FR-027` | Clean Windows 11 Sandbox VM | Compile `installer/openidm.iss` with `ISCC.exe`, install per-user, verify `HKCU\...\NativeMessagingHosts\org.openidm.native`, upgrade in place, and uninstall. | `Not Yet Verified (Requires Win11 Host)` |
| **`SYS-UI-01`** | `NFR-011` | Windows 11 `x64` Multi-Monitor GUI | Launch `openidm.exe`, verify Win32 dark titlebar, system tray menu, multi-monitor `WM_DPICHANGED` scaling, and Windows Narrator traversal. | `Not Yet Verified (Requires Win11 Host)` |
