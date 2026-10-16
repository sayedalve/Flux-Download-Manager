# Flux Download Manager -- Requirement-to-Test Verification Matrix

**Document Status:** Active Verification Matrix (`v0.3.0` — `31 / 31` Automated Suites Executed & Passed on Windows 11 Host)  
**Last Reviewed:** 2026-10-16  
**Scope:** Maps every functional (`FR-001`–`FR-028`), non-functional (`NFR-001`–`NFR-016`), and bug-regression (`BUG-001`–`BUG-035`) test to its exact command, verification category, and outcome.  
**Related Documents:** [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md), [BUG_FIX_LOG.md](BUG_FIX_LOG.md), [FEATURE_MATRIX.md](FEATURE_MATRIX.md), [FLUX_FAILURE_AUDIT.md](FLUX_FAILURE_AUDIT.md)

---

## 1. Automated Verification Summary (`Windows 11 x64 Host`)

### 1.1 C++20 Unit & Regression Test Suites (`.\build\win-x64\Release\flux_unit_tests.exe` — `13 / 13 PASSED`)

| Suite # | Suite Function (`tests/cpp/unit_tests.cpp`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/13]`** | `TestPathSanitizerAndContentDisposition` | `FR-001`, `FR-026`, `NFR-006`, `NFR-012`, `SEC-PATH-01` | `Verified by Execution` | **PASSED** |
| **`[2/13]`** | `TestDynamicWorkerAndDiskSpaceGuard` | `FR-020`, `FR-026`, `BUG-005`, `BUG-009`, `BUG-022` | `Verified by Execution` | **PASSED** |
| **`[3/13]`** | `TestLoggerRedactionAndCrypto` | `FR-007`, `NFR-009`, `SEC-PRIV-01`, `UT-HASH-01` | `Verified by Execution` | **PASSED** |
| **`[4/13]`** | `TestBriskSegmentSchedulerWorkStealingAndRollback` | `FR-003`, `FR-004`, `UT-SEG-01`, `BUG-008` | `Verified by Execution` | **PASSED** |
| **`[5/13]`** | `TestSqliteStoreAndCrashRecovery` | `FR-005`, `NFR-005`, `NFR-009`, `IT-CRASH-01` | `Verified by Execution` | **PASSED** |
| **`[6/13]`** | `TestStreamMetaJsonAndDrmRejection` | `FR-019`, `NFR-015`, `SEC-DRM-01`, `THREAT-004`, `BUG-024` | `Verified by Execution` | **PASSED** |
| **`[7/13]`** | `TestControllerLifecycleAndDeletedTaskRace` | `FR-023`, `BUG-007`, `BUG-010`, `BUG-034` (Webpage Guard) | `Verified by Execution` | **PASSED** |
| **`[8/13]`** | `TestDesktopUiDesignSystemAndHighDpiLayout` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |
| **`[9/13]`** | `TestDesktopUiInteractionsFilteringSortingAndRedaction` | `FR-011`, `FR-023`–`FR-025`, `NFR-009`, `BUG-001`, `BUG-010`, `BUG-011` | `Verified by Execution` | **PASSED** |
| **`[10/13]`** | `TestKeyboardNavigationAndShortcuts` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |
| **`[11/13]`** | `TestDesktopUiHitTestingAndMouseInteractions` | `FR-011`, `BUG-018`, `BUG-026` | `Verified by Execution` | **PASSED** |
| **`[12/13]`** | `TestWorkerLifecycleAndIncompatibleWorkerDetection` | `FR-020`, `BUG-006`, `BUG-027` | `Verified by Execution` | **PASSED** |
| **`[13/13]`** | `TestFluxBrandingAndLegacyDataMigration` | `FR-028`, `BUG-030` | `Verified by Execution` | **PASSED** |

---

### 1.2 Browser Extension & Media Detection Test Suites (`node tests/test_extension_detection.js` — `10 / 10 PASSED`)

| Suite # | Suite Description (`tests/test_extension_detection.js`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/10]`** | Direct MP4/WebM & HLS/DASH Detection + Header Correlation | `FR-014` | `Verified by Execution` | **PASSED** |
| **`[2/10]`** | Master vs. Child HLS Deduplication & `.ts` Segment Suppression | `FR-014`, `FR-016` | `Verified by Execution` | **PASSED** |
| **`[3/10]`** | Per-Site Exclusion Setting & Popup Rendering | `FR-009`, `FR-014` | `Verified by Execution` | **PASSED** |
| **`[4/10]`** | Page-Context Deep Search Hook (Obfuscated Manifest Capture + Nonce) | `FR-013`, `FR-017`, `BUG-032` | `Verified by Execution` | **PASSED** |
| **`[5/10]`** | In-Page Closed Shadow DOM Video Overlay & Fullscreen Repositioning | `FR-015`, `BUG-013`, `BUG-032` | `Verified by Execution` | **PASSED** |
| **`[6/10]`** | Download Interception, Bypass Modifier & Safe Browser Fallback | `FR-008`, `FR-009`, `FR-012`, `BUG-031` | `Verified by Execution` | **PASSED** |
| **`[7/10]`** | Manifest V3 Service Worker Session State Persistence & Recovery | `FR-014`, `RISK-002`, `BUG-013` | `Verified by Execution` | **PASSED** |
| **`[8/10]`** | Popup Filter Tabs, Search, Disconnected States & Direct Media Fallback | `FR-008`, `FR-014`, `BUG-012`, `BUG-033` | `Verified by Execution` | **PASSED** |
| **`[9/10]`** | Link-Click Interception & Alt-Key Bypass Behavior | `FR-008`, `FR-009`, `BUG-031` | `Verified by Execution` | **PASSED** |
| **`[10/10]`** | Webpage Watch URL Guard & Background Runtime Message Routing | `FR-014`, `FR-021`, `BUG-033`, `BUG-034` | `Verified by Execution` | **PASSED** |

---

### 1.3 End-to-End Integration, Stream Pipeline & Benchmark Suites (`python tests/run_all_integration_tests.py` — `8 / 8 PASSED`)

| Suite # | Test ID & Description (`tests/run_all_integration_tests.py`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/8]`** | `IT-BIN-01`: Verify Platform-Aware Compiled Executables (`flux.exe`, `flux-nmh.exe`, `N_m3u8DL-RE.exe`) | `FR-020`, `BUG-006`, `BUG-030` | `Verified by Execution` | **PASSED** |
| **`[2/8]`** | `IT-HTTP-01` & `IT-HTTP-03`: Segmented HTTP Range Download (8 Connections + 302 Redirect + RFC 5987) | `FR-001`, `FR-003`, `BUG-003` | `Verified by Execution` | **PASSED** |
| **`[3/8]`** | `IT-HTTP-02`: Non-Range Server Single-Stream Fallback | `FR-002`, `BUG-008` | `Verified by Execution` | **PASSED** |
| **`[4/8]`** | `IT-HTTP-04`: Dropped Connection Reset & Resume from Exact Byte Offset | `FR-004` | `Verified by Execution` | **PASSED** |
| **`[5/8]`** | `IT-STREAM-01` & `IT-STREAM-02`: HLS Master Manifest Probe & FFmpeg Container Muxing | `FR-019`, `FR-020`, `BUG-023`, `BUG-024` | `Verified by Execution` | **PASSED** |
| **`[6/8]`** | `IT-STREAM-03`: MPEG-DASH (`.mpd`) Manifest Download & FFmpeg Muxing | `FR-020`, `BUG-023` | `Verified by Execution` | **PASSED** |
| **`[7/8]`** | `IT-IPC-01`, `IT-STREAM-06` & `IT-UI-01`: Native Messaging Host, AES-128 Decryption, Log Redaction & Desktop UI Snapshot | `FR-021`, `FR-022`, `NFR-007`–`NFR-009`, `BUG-002`, `BUG-004`, `BUG-025` | `Verified by Execution` | **PASSED** |
| **`[8/8]`** | `PERF-BENCH-01`: Core Binaries (`2745.5 KB` <= 3072 KB), Startup (`46.81 ms` <= 150 ms), Idle RSS (`9.32 MB` <= 25 MB), HTTP Peak RSS (`13.6 MB` <= 65 MB), Stream Peak RSS (`10.54 MB` <= 65 MB), UI Snapshot (`163.97 ms` <= 300 ms), Throughput (`386.9 Mbps` >= 50 Mbps) | `NFR-001`–`NFR-004`, `BUG-017`, `BUG-021` | `Verified by Execution` | **PASSED** |

---

## 2. Release & Host Deployment Status

| Verification Item | Target | Verification Procedure | Status |
| :--- | :--- | :--- | :--- |
| **Native PE32+ Compilation** | Windows 11 `x64` | Built with MSVC 19.44 / CMake 4.2; verified AMD64 subsystem on `flux.exe` and `flux-nmh.exe` | **Verified by Execution** |
| **Per-User Native Messaging** | HKCU Registry | Executed `.\scripts\register-native-messaging.ps1`; verified registry keys for Chrome, Edge, Brave, and Firefox pointing to `flux-nmh.exe` | **Verified by Execution** |
| **Portable Distribution Directory** | `dist/` | Verified `dist/Flux.exe`, `dist/flux.exe`, `dist/flux-nmh.exe`, and `dist/tools/win-x64/` (`ffmpeg.exe`, `N_m3u8DL-RE.exe`) | **Verified by Execution** |
| **Live Browser Extension Runtime** | Chromium / Firefox | Manual user step: load unpacked `extension/` in `chrome://extensions` (Developer Mode) and browse media sites | **Unverified (Requires Manual Browser Step)** |
