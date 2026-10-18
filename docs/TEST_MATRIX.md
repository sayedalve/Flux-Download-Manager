# Flux Download Manager -- Requirement-to-Test Verification Matrix

**Document Status:** Active Verification Matrix (`v0.3.0` — `31 / 31` Automated Suites Executed & Passed on Windows 11 Host)  
**Last Reviewed:** 2026-10-16  
**Scope:** Maps every functional (`FR-001`–`FR-028`), non-functional (`NFR-001`–`NFR-016`), and bug-regression (`BUG-001`–`BUG-035`) test to its exact command, verification category, and outcome.  
**Related Documents:** [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md), [BUG_FIX_LOG.md](BUG_FIX_LOG.md), [FEATURE_MATRIX.md](FEATURE_MATRIX.md), [FLUX_FAILURE_AUDIT.md](FLUX_FAILURE_AUDIT.md)

---

## 1. Automated Verification Summary (`Windows 11 x64 Host`)

### 1.1 C++20 Unit & Regression Test Suites (`tests/cpp/unit_tests.cpp` — `53 / 53` Suites: Verified by Execution)

| Suite # | Suite Function (`tests/cpp/unit_tests.cpp`) | Mapped Requirements & Bug IDs | Verification Category | Result |
| :--- | :--- | :--- | :--- | :--- |
| **`[1/53]`** | `TestPathSanitizerAndContentDisposition` | `FR-001`, `FR-026`, `NFR-006`, `NFR-012`, `SEC-PATH-01` | `Verified by Execution` | **PASSED** |
| **`[2/53]`** | `TestDynamicWorkerAndDiskSpaceGuard` | `FR-020`, `FR-026`, `BUG-005`, `BUG-009`, `BUG-022` | `Verified by Execution` | **PASSED** |
| **`[3/53]`** | `TestLoggerRedactionAndCrypto` | `FR-007`, `NFR-009`, `SEC-PRIV-01`, `UT-HASH-01` | `Verified by Execution` | **PASSED** |
| **`[4/53]`** | `TestBriskSegmentSchedulerWorkStealingAndRollback` | `FR-003`, `FR-004`, `UT-SEG-01`, `BUG-008` | `Verified by Execution` | **PASSED** |
| **`[5/53]`** | `TestSqliteStoreAndCrashRecovery` | `FR-005`, `NFR-005`, `NFR-009`, `IT-CRASH-01` | `Verified by Execution` | **PASSED** |
| **`[6/53]`** | `TestStreamMetaJsonAndDrmRejection` | `FR-019`, `NFR-015`, `SEC-DRM-01`, `THREAT-004`, `BUG-024` | `Verified by Execution` | **PASSED** |
| **`[7/53]`** | `TestControllerLifecycleAndDeletedTaskRace` | `FR-023`, `BUG-007`, `BUG-010`, `BUG-034` (Webpage Guard) | `Verified by Execution` | **PASSED** |
| **`[8/53]`** | `TestDesktopUiDesignSystemAndHighDpiLayout` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |
| **`[9/53]`** | `TestCosmicStarfieldPerformanceAndFallback` | `FR-011`, `NFR-011` | `Verified by Execution` | **PASSED** |
| **`[10/53]`** | `TestProfessionalSettingsCenterValidationAndPersistence` | `FR-011`, `FR-025` | `Verified by Execution` | **PASSED** |
| **`[11/53]`** | `TestDesktopUiInteractionsFilteringSortingAndRedaction` | `FR-011`, `FR-023`–`FR-025`, `NFR-009`, `BUG-001`, `BUG-010`, `BUG-011` | `Verified by Execution` | **PASSED** |
| **`[12/53]`** | `TestKeyboardNavigationAndShortcuts` | `FR-011`, `NFR-011`, `BUG-001` | `Verified by Execution` | **PASSED** |
| **`[13/53]`** | `TestDesktopUiHitTestingAndMouseInteractions` | `FR-011`, `BUG-018`, `BUG-026` | `Verified by Execution` | **PASSED** |
| **`[14/53]`** | `TestWorkerLifecycleAndIncompatibleWorkerDetection` | `FR-020`, `BUG-006`, `BUG-027` | `Verified by Execution` | **PASSED** |
| **`[15/53]`** | `TestFluxBrandingAndLegacyDataMigration` | `FR-028`, `BUG-030` | `Verified by Execution` | **PASSED** |
| **`[16/53]`** | `TestPartFileIsolationAndSafeCommit` | `FR-026`, `NFR-006` | `Verified by Execution` | **PASSED** |
| **`[17/53]`** | `TestResourceAwareResumeAndValidatorMismatch` | `FR-005`, `NFR-005` | `Verified by Execution` | **PASSED** |
| **`[18/53]`** | `TestSegmentSchedulerBoundarySplitRaceGuard` | `FR-003`, `FR-004` | `Verified by Execution` | **PASSED** |
| **`[19/53]`** | `TestIpcSecurityDaclAndPathTraversal` | `NFR-006`, `NFR-008` | `Verified by Execution` | **PASSED** |
| **`[20/53]`** | `TestGlobalRateLimiting` | `FR-024` | `Verified by Execution` | **PASSED** |
| **`[21/53]`** | `TestRelativeAndProtocolRelativeRedirectResolution` | `FR-001` | `Verified by Execution` | **PASSED** |
| **`[22/53]`** | `TestCompleteSegmentRangeMapValidation` | `FR-003` | `Verified by Execution` | **PASSED** |
| **`[23/53]`** | `TestPosixHttpChunkedDecoder` | `FR-001`, `FR-002` | `Verified by Execution` | **PASSED** |
| **`[24/53]`** | `TestUrlParsingAndSecurityChecks` | `FR-001`, `NFR-006` | `Verified by Execution` | **PASSED** |
| **`[25/53]`** | `TestIpcShutdownAndFirstPipeInstance` | `NFR-007`, `NFR-008` | `Verified by Execution` | **PASSED** |
| **`[26/53]`** | `TestConflictPolicies` | `FR-026` | `Verified by Execution` | **PASSED** |
| **`[27/53]`** | `TestSystemSuspendResumeAndPauseReason` | `FR-005`, `FR-023` | `Verified by Execution` | **PASSED** |
| **`[28/53]`** | `TestConcurrentAtomicCommitsAndSha256` | `FR-007`, `FR-026` | `Verified by Execution` | **PASSED** |
| **`[29/53]`** | `TestDnsRebindingAndDestinationSecurity` | `FR-001`, `NFR-007` | `Verified by Execution` | **PASSED** |
| **`[30/53]`** | `TestStreamContainerValidation` | `FR-020` | `Verified by Execution` | **PASSED** |
| **`[31/53]`** | `TestFtpClientParsingAndCommands` | `FR-029` (FTP / FTPS RFC 959/2428/3659/4217) | `Verified by Execution` | **PASSED** |
| **`[32/53]`** | `TestBencodeAndTorrentParser` | `FR-030` (BitTorrent Bencode & Magnet URI) | `Verified by Execution` | **PASSED** |
| **`[33/53]`** | `TestRtspMmsStreamDetection` | `FR-031` (RTSP, MMS & RTMP Legacy Streams) | `Verified by Execution` | **PASSED** |
| **`[34/53]`** | `TestSiteGrabberCrawlerAndSsrf` | `FR-032` (Site Grabber & Crawler SSRF Guard) | `Verified by Execution` | **PASSED** |
| **`[35/53]`** | `TestZipArchivePreviewAndSecurity` | `FR-033` (ZIP Previewer, Zip Slip & Zip Bomb Guard) | `Verified by Execution` | **PASSED** |
| **`[36/53]`** | `TestTaskImportExportAndMultiQueue` | `FR-034`, `FR-035` (Import/Export & Multi-Queue) | `Verified by Execution` | **PASSED** |
| **`[37/53]`** | `TestAntivirusScannerSafetyAndHandling` | `FR-034` (Antivirus Scanner Safety & Malicious Execution Block) | `Verified by Execution` | **PASSED** |
| **`[38/53]`** | `TestDualRateLimiterAndPerTaskSpeedLimit` | `FR-024` (Dual Token-Bucket Rate Limiter & Per-Task Speed Control) | `Verified by Execution` | **PASSED** |
| **`[39/53]`** | `TestDynamicCategorizationAndSiteRulesPriority` | `FR-010`, `FR-023` (Category Rules, Custom Extension Mappings & Priority Hierarchy) | `Verified by Execution` | **PASSED** |
| **`[40/53]`** | `TestAlternateUrlsAndMirrorValidation` | `FR-001`, `FR-004` (Mirror Failover & Content-Length Identity Validation) | `Verified by Execution` | **PASSED** |
| **`[41/53]`** | `TestFtpReliabilityEpsvAndResumeTruncation` | `FR-029` (RFC 2428 EPSV, PASV Fallback, NAT Bypass & REST 350 Truncation) | `Verified by Execution` | **PASSED** |
| **`[42/53]`** | `TestSchedulerStopTimesAndPostCompletionActions` | `FR-025` (Stop Times, Queue Scheduling Loop & Post-Completion Sleep/Shutdown) | `Verified by Execution` | **PASSED** |
| **`[43/53]`** | `TestPeerWireProtocolFramingAndCompactPeers` | `FR-030` (BEP 3 Peer Wire Framing & Compact Peer Unpacking) | `Verified by Execution` | **PASSED** |
| **`[44/53]`** | `TestMultiFileStorageMappingAndBoundarySplitting` | `FR-030` (Multi-File Storage Offset Mapping & File Boundary Slicing) | `Verified by Execution` | **PASSED** |
| **`[45/53]`** | `TestPieceManagerAndBlockScheduling` | `FR-030` (Piece Manager, Block Request Generation & SHA-1 Verification) | `Verified by Execution` | **PASSED** |
| **`[46/53]`** | `TestTrackerClientQueryEncodingAndResponseParsing` | `FR-030` (Tracker Query Binary URL Percent-Encoding & Response Parsing) | `Verified by Execution` | **PASSED** |
| **`[47/53]`** | `TestHttpRangeSatisfactionValidation` | `FR-001`, `FR-004` (HTTP 206 Range Satisfaction & Entity Integrity Validation) | `Verified by Execution` | **PASSED** |
| **`[48/53]`** | `TestDuplicateTaskDetectionThreeTiers` | `FR-010`, `FR-023` (3-Tier Duplicate Detection: Normalized URL, Name/Size, Strong ETag & Magnet BTIH) | `Verified by Execution` | **PASSED** |
| **`[49/53]`** | `TestExpiredLinkRefreshAndCookieReplacement` | `FR-006` (Expired Link 403/410 Detection, Presigned URL Refresh & Cookie Replacement) | `Verified by Execution` | **PASSED** |
| **`[50/53]`** | `TestSegmentSchedulerChunkNormalizationAndTruncationGuard` | `FR-003`, `FR-005` (Segment Restored Chunk Normalization & Truncation Guard) | `Verified by Execution` | **PASSED** |
| **`[51/53]`** | `TestUrlUserinfoExtractionAndBasicAuthGeneration` | `FR-001` (URL Userinfo Extraction & HTTP Basic Authorization Header Generation) | `Verified by Execution` | **PASSED** |
| **`[52/53]`** | `TestModernCoralThemeSettingsModalBoundsAndSafeDelete` | `FR-011`, `FR-026`, `NFR-011` (Coral Charcoal Theme `#FF4F57`, Modal Hit-Testing Bounds, Recycle Bin Safe Delete & Header Sorting Direction) | `Verified by Execution` | **PASSED** |
| **`[53/53]`** | `TestYouTubeSpeedParsingMidnightCrossingAndBitTorrentUpload` | `FR-019`, `FR-024`, `FR-030` (YouTube Regex Speed Extraction, Scheduled Midnight-Crossing Window & BT Unchoke/Block Upload Accounting) | `Verified by Execution` | **PASSED** |

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
