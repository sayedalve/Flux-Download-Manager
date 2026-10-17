# Flux Download Manager -- Requirement-to-Test Verification Matrix

**Document Status:** Active Verification Matrix (`v0.3.0` — `31 / 31` Automated Suites Executed & Passed on Windows 11 Host)  
**Last Reviewed:** 2026-10-16  
**Scope:** Maps every functional (`FR-001`–`FR-028`), non-functional (`NFR-001`–`NFR-016`), and bug-regression (`BUG-001`–`BUG-035`) test to its exact command, verification category, and outcome.  
**Related Documents:** [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md), [BUG_FIX_LOG.md](BUG_FIX_LOG.md), [FEATURE_MATRIX.md](FEATURE_MATRIX.md), [FLUX_FAILURE_AUDIT.md](FLUX_FAILURE_AUDIT.md)

---

## 1. Automated Verification Summary (`Windows 11 x64 Host`)

### 1.1 C++20 Unit & Regression Test Suites (`.\build\win-x64\Release\flux_unit_tests.exe` — `36 / 36 PASSED`)

| Suite # | Suite Function (`tests/cpp/unit_tests.cpp`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/36]`** | `TestPathSanitizerAndContentDisposition` | `FR-001`, `FR-026`, `NFR-006`, `NFR-012`, `SEC-PATH-01` | `Verified by Execution` | **PASSED** |
| **`[2/36]`** | `TestDynamicWorkerAndDiskSpaceGuard` | `FR-020`, `FR-026`, `BUG-005`, `BUG-009`, `BUG-022` | `Verified by Execution` | **PASSED** |
| **`[3/36]`** | `TestLoggerRedactionAndCrypto` | `FR-007`, `NFR-009`, `SEC-PRIV-01`, `UT-HASH-01` | `Verified by Execution` | **PASSED** |
| **`[4/36]`** | `TestBriskSegmentSchedulerWorkStealingAndRollback` | `FR-003`, `FR-004`, `UT-SEG-01`, `BUG-008` | `Verified by Execution` | **PASSED** |
| **`[5/36]`** | `TestSqliteStoreAndCrashRecovery` | `FR-005`, `NFR-005`, `NFR-009`, `IT-CRASH-01` | `Verified by Execution` | **PASSED** |
| **`[6/36]`** | `TestStreamMetaJsonAndDrmRejection` | `FR-019`, `NFR-015`, `SEC-DRM-01`, `THREAT-004`, `BUG-024` | `Verified by Execution` | **PASSED** |
| **`[7/36]`** | `TestControllerLifecycleAndDeletedTaskRace` | `FR-023`, `BUG-007`, `BUG-010`, `BUG-034` (Webpage Guard) | `Verified by Execution` | **PASSED** |
| **`[8/36]`** | `TestDesktopUiDesignSystemAndHighDpiLayout` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |
| **`[9/36]`** | `TestCosmicStarfieldPerformanceAndFallback` | `FR-011`, `NFR-011` | `Verified by Execution` | **PASSED** |
| **`[10/36]`** | `TestProfessionalSettingsCenterValidationAndPersistence` | `FR-011`, `FR-025` | `Verified by Execution` | **PASSED** |
| **`[11/36]`** | `TestDesktopUiInteractionsFilteringSortingAndRedaction` | `FR-011`, `FR-023`–`FR-025`, `NFR-009`, `BUG-001`, `BUG-010`, `BUG-011` | `Verified by Execution` | **PASSED** |
| **`[12/36]`** | `TestKeyboardNavigationAndShortcuts` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |
| **`[13/36]`** | `TestDesktopUiHitTestingAndMouseInteractions` | `FR-011`, `BUG-018`, `BUG-026` | `Verified by Execution` | **PASSED** |
| **`[14/36]`** | `TestWorkerLifecycleAndIncompatibleWorkerDetection` | `FR-020`, `BUG-006`, `BUG-027` | `Verified by Execution` | **PASSED** |
| **`[15/36]`** | `TestFluxBrandingAndLegacyDataMigration` | `FR-028`, `BUG-030` | `Verified by Execution` | **PASSED** |
| **`[16/36]`** | `TestPartFileIsolationAndSafeCommit` | `FR-026`, `NFR-006` | `Verified by Execution` | **PASSED** |
| **`[17/36]`** | `TestResourceAwareResumeAndValidatorMismatch` | `FR-005`, `NFR-005` | `Verified by Execution` | **PASSED** |
| **`[18/36]`** | `TestSegmentSchedulerBoundarySplitRaceGuard` | `FR-003`, `FR-004` | `Verified by Execution` | **PASSED** |
| **`[19/36]`** | `TestIpcSecurityDaclAndPathTraversal` | `NFR-006`, `NFR-008` | `Verified by Execution` | **PASSED** |
| **`[20/36]`** | `TestGlobalRateLimiting` | `FR-024` | `Verified by Execution` | **PASSED** |
| **`[21/36]`** | `TestRelativeAndProtocolRelativeRedirectResolution` | `FR-001` | `Verified by Execution` | **PASSED** |
| **`[22/36]`** | `TestCompleteSegmentRangeMapValidation` | `FR-003` | `Verified by Execution` | **PASSED** |
| **`[23/36]`** | `TestPosixHttpChunkedDecoder` | `FR-001`, `FR-002` | `Verified by Execution` | **PASSED** |
| **`[24/36]`** | `TestUrlParsingAndSecurityChecks` | `FR-001`, `NFR-006` | `Verified by Execution` | **PASSED** |
| **`[25/36]`** | `TestIpcShutdownAndFirstPipeInstance` | `NFR-007`, `NFR-008` | `Verified by Execution` | **PASSED** |
| **`[26/36]`** | `TestConflictPolicies` | `FR-026` | `Verified by Execution` | **PASSED** |
| **`[27/36]`** | `TestSystemSuspendResumeAndPauseReason` | `FR-005`, `FR-023` | `Verified by Execution` | **PASSED** |
| **`[28/36]`** | `TestConcurrentAtomicCommitsAndSha256` | `FR-007`, `FR-026` | `Verified by Execution` | **PASSED** |
| **`[29/36]`** | `TestDnsRebindingAndDestinationSecurity` | `FR-001`, `NFR-007` | `Verified by Execution` | **PASSED** |
| **`[30/36]`** | `TestStreamContainerValidation` | `FR-020` | `Verified by Execution` | **PASSED** |
| **`[31/36]`** | `TestFtpClientParsingAndCommands` | `FR-029` (FTP / FTPS RFC 959/2428/3659/4217) | `Verified by Execution` | **PASSED** |
| **`[32/36]`** | `TestBencodeAndTorrentParser` | `FR-030` (BitTorrent Bencode & Magnet URI) | `Verified by Execution` | **PASSED** |
| **`[33/36]`** | `TestRtspMmsStreamDetection` | `FR-031` (RTSP, MMS & RTMP Legacy Streams) | `Verified by Execution` | **PASSED** |
| **`[34/36]`** | `TestSiteGrabberCrawlerAndSsrf` | `FR-032` (Site Grabber & Crawler SSRF Guard) | `Verified by Execution` | **PASSED** |
| **`[35/36]`** | `TestZipArchivePreviewAndSecurity` | `FR-033` (ZIP Previewer, Zip Slip & Zip Bomb Guard) | `Verified by Execution` | **PASSED** |
| **`[36/36]`** | `TestTaskImportExportAndMultiQueue` | `FR-034`, `FR-035` (Import/Export & Multi-Queue) | `Verified by Execution` | **PASSED** |

---

### 1.2 Browser Extension & Media Detection Test Suites (`node tests/test_extension_detection.js` — `11 / 11 PASSED`)

| Suite # | Suite Description (`tests/test_extension_detection.js`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/11]`** | Direct MP4/WebM & HLS/DASH Detection + Header Correlation | `FR-014` | `Verified by Execution` | **PASSED** |
| **`[2/11]`** | Master vs. Child HLS Deduplication & `.ts` Segment Suppression | `FR-014`, `FR-016` | `Verified by Execution` | **PASSED** |
| **`[3/11]`** | Per-Site Exclusion Setting & Popup Rendering | `FR-009`, `FR-014` | `Verified by Execution` | **PASSED** |
| **`[4/11]`** | Page-Context Deep Search Hook (Obfuscated Manifest Capture + Nonce) | `FR-013`, `FR-017`, `BUG-032` | `Verified by Execution` | **PASSED** |
| **`[5/11]`** | In-Page Closed Shadow DOM Video Overlay & Fullscreen Repositioning | `FR-015`, `BUG-013`, `BUG-032` | `Verified by Execution` | **PASSED** |
| **`[6/11]`** | Download Interception, Bypass Modifier & Safe Browser Fallback | `FR-008`, `FR-009`, `FR-012`, `BUG-031` | `Verified by Execution` | **PASSED** |
| **`[7/11]`** | Manifest V3 Service Worker Session State Persistence & Recovery | `FR-014`, `RISK-002`, `BUG-013` | `Verified by Execution` | **PASSED** |
| **`[8/11]`** | Popup Filter Tabs, Search, Disconnected States & Direct Media Fallback | `FR-008`, `FR-014`, `BUG-012`, `BUG-033` | `Verified by Execution` | **PASSED** |
| **`[9/11]`** | Link-Click Interception & Alt-Key Bypass Behavior | `FR-008`, `FR-009`, `BUG-031` | `Verified by Execution` | **PASSED** |
| **`[10/11]`** | Webpage Watch URL Guard & Background Runtime Message Routing | `FR-014`, `FR-021`, `BUG-033`, `BUG-034` | `Verified by Execution` | **PASSED** |
| **`[11/11]`** | Cosmic Dark Extension Styling & Offscreen Overlay Visibility | `FR-011`, `FR-015` | `Verified by Execution` | **PASSED** |

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
| **`[8/8]`** | `PERF-BENCH-01`: Core Binaries (`2,047.0 KB` <= 3072 KB), Startup (`71.05 ms` <= 150 ms), Idle RSS (`10.34 MB` <= 25 MB), HTTP Peak RSS (`15.7 MB` <= 65 MB), Stream Peak RSS (`11.04 MB` <= 65 MB), UI Snapshot (`143.09 ms` <= 300 ms), Throughput (`223.0 Mbps` >= 50 Mbps) | `NFR-001`–`NFR-004`, `BUG-017`, `BUG-021` | `Verified by Execution` | **PASSED** |

---

## 2. Release & Host Deployment Status

| Verification Item | Target | Verification Procedure | Status |
| :--- | :--- | :--- | :--- |
| **Native PE32+ Compilation** | Windows 11 `x64` | Built with MSVC 19.44 / CMake 4.2; verified AMD64 subsystem on `flux.exe` and `flux-nmh.exe` | **Verified by Execution** |
| **Per-User Native Messaging** | HKCU Registry | Executed `.\scripts\register-native-messaging.ps1`; verified registry keys for Chrome, Edge, Brave, and Firefox pointing to `flux-nmh.exe` | **Verified by Execution** |
| **Portable Distribution Directory** | `dist/` | Verified `dist/Flux.exe`, `dist/flux.exe`, `dist/flux-nmh.exe`, and `dist/tools/win-x64/` (`ffmpeg.exe`, `N_m3u8DL-RE.exe`) | **Verified by Execution** |
| **Live Browser Extension Runtime** | Chromium / Firefox | Manual user step: load unpacked `extension/` in `chrome://extensions` (Developer Mode) and browse media sites | **Unverified (Requires Manual Browser Step)** |
