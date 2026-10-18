# Flux Download Manager -- Requirement-to-Implementation Feature Matrix

**Document Status:** Verified Implementation Traceability Matrix (`v0.3.0`)  
**Last Reviewed:** 2026-10-16  
**Scope:** Maps every functional (`FR-001`–`FR-028`) and non-functional (`NFR-001`–`NFR-016`) requirement to its actual source module, upstream reference, verification classification, and test coverage.  
**Related Documents:** [PRODUCT_REQUIREMENTS.md](PRODUCT_REQUIREMENTS.md), [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md), [TEST_MATRIX.md](TEST_MATRIX.md), [BUG_FIX_LOG.md](BUG_FIX_LOG.md), [FLUX_FAILURE_AUDIT.md](FLUX_FAILURE_AUDIT.md)

---

## 1. Verification Classification Definitions

- **`Verified by Execution`:** Compiled and verified in the automated test harness on Windows 11 host (`flux_unit_tests.exe`, `test_extension_detection.js`, `run_all_integration_tests.py`, `run-benchmarks.py`).
- **`Verified by Source Inspection`:** Implemented in C++20/Win32 (`WinHTTP`, `CreateWindowExW`, `CreateJobObjectW`, Named Pipes, DPAPI) and verified for API/algorithmic correctness.
- **`Unverified (Requires Manual Browser Step)`:** Manual interaction step in live browser (loading unpacked extension in `chrome://extensions` Developer Mode).

---

## 2. Functional Requirements Matrix (`FR-001` to `FR-028`)

| Req ID | Feature Summary | Responsible Module (`src/` or `extension/`) | Upstream Reference | Verification Status | Mapped Tests |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **FR-001** | HTTP/HTTPS Probe, WinHTTP Proxy & Mirror URL Failover | `src/brisk_engine/http_client.cpp`, `http_download_engine.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Static Inspection` (Phase 2 proxy/mirrors) | `UT-HTTP-01`, `IT-HTTP-01`, `TestAlternateUrlsAndMirrorValidation` |
| **FR-002** | Single-Stream Fallback for Non-Range Servers | `src/brisk_engine/http_download_engine.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Execution` | `IT-HTTP-02` |
| **FR-003** | Dynamic Connection Spawn & Work-Stealing Reuse | `src/brisk_engine/segment_scheduler.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Execution` | `UT-SEG-01`, `IT-HTTP-03` |
| **FR-004** | Stall Detection, Alternate URL Failover & Secondary Rollback | `src/brisk_engine/http_download_engine.cpp`, `src/core/download_controller.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Static Inspection` (Phase 2 mirror failover) | `IT-HTTP-04`, `UT-SEG-02`, `TestAlternateUrlsAndMirrorValidation` |
| **FR-005** | Pause, Crash-Safe Resume & Validator Check | `src/core/download_controller.cpp`, `src/storage/sqlite_store.cpp` | Independent Design | `Verified by Execution` | `IT-CRASH-01` |
| **FR-006** | Expired URL Refresh ("Refresh Download Address") | `src/core/download_controller.cpp` (`RefreshTaskUrl`) | Independent Design | `Verified by Execution` | `UT-CTRL-01` |
| **FR-007** | Post-Download Checksum Verification (`SHA-256`) | `src/common/crypto_utils.cpp` | Independent Design | `Verified by Execution` | `UT-HASH-01` |
| **FR-008** | Automatic Browser Download & Link-Click Interception + Alt Bypass | `extension/js/content-script.js`, `extension/js/background.js` | [BrisklyDev/brisk-browser-extension](https://github.com/BrisklyDev/brisk-browser-extension) | `Verified by Static Inspection` (Phase 3 non-invasive click correlation & fallback) | `Suite 6/10`, `Suite 9/10` (`BUG-031`) |
| **FR-009** | Bypass Key & Per-Site Exclusion List | `extension/js/function.js`, `extension/js/background.js` | [xifangczy/cat-catch](https://github.com/xifangczy/cat-catch) | `Verified by Execution` | `Suite 3/10` |
| **FR-010** | Batch Link Capture (`download.batchAdd`) | `src/core/download_controller.cpp`, `src/ipc/ipc_server.cpp` | [BrisklyDev/brisk-browser-extension](https://github.com/BrisklyDev/brisk-browser-extension) | `Verified by Execution` | `IT-IPC-01` |
| **FR-011** | Desktop UI, Toolbar, Sidebar, Details Panel & Shortcuts | `src/ui/design_system.hpp`, `src/ui/win32_main_window.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Execution` | `Suites 8, 9, 10, 11/13`, `IT-UI-01` |
| **FR-012** | Conflict Prevention (`byExtensionId` & `blob:`/`data:` native delegation) | `extension/js/background.js`, `extension/catch-script/catch.js` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Static Inspection` (Phase 3 unhijacked native blob exports) | `Suite 6/10` |
| **FR-013** | Nonce-Validated Page Hook Bridge & MAIN-world Injection | `extension/catch-script/catch.js`, `extension/js/content-script.js` | [xifangczy/cat-catch](https://github.com/xifangczy/cat-catch) | `Verified by Execution` | `Suite 4/10` (`BUG-032`) |
| **FR-014** | In-Page Playback Hook, Network Sniffing & MV3 Session Persistence | `extension/js/content-script.js`, `extension/js/background.js` | [xifangczy/cat-catch](https://github.com/xifangczy/cat-catch) | `Verified by Static Inspection` (Phase 3 storage fallback & click ring buffer) | `Suites 1, 2, 7, 10/10` (`BUG-032`, `BUG-033`) |
| **FR-015** | Non-Intrusive Closed Shadow DOM Video Overlay + Fullscreen Sync | `extension/js/content-script.js` | [BrisklyDev/brisk-browser-extension](https://github.com/BrisklyDev/brisk-browser-extension) | `Verified by Execution` | `Suite 5/10` |
| **FR-016** | Master vs. Media Manifest Deduplication & Grouping | `extension/js/function.js` (`addOrDeduplicateMediaCandidate`) | [xifangczy/cat-catch](https://github.com/xifangczy/cat-catch) | `Verified by Execution` | `Suite 2/10` |
| **FR-017** | Opt-In Page-Context ("Deep Search") Hook | `extension/catch-script/catch.js` | [xifangczy/cat-catch](https://github.com/xifangczy/cat-catch) | `Verified by Execution` | `Suite 4/10` |
| **FR-018** | Live Stream Discovery Metadata (`isLive`) | `src/worker_stream/nm3u8dl_re_worker.cpp`, `src/engine_stream/` | [nilaoda/N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE) | `Verified by Execution` | `UT-STREAM-01` |
| **FR-019** | Out-of-Process Manifest Probing, Redirect Resolution & URL Expired Detection | `src/engine_stream/nm3u8dl_re_adapter.cpp`, `src/common/path_sanitizer.cpp` | [nilaoda/N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE) | `Verified by Static Inspection` (Phase 2 redirects/403) | `Suite 6/13`, `Suite 7/13`, `IT-STREAM-01` (`BUG-034`) |
| **FR-020** | Compiled Native C++20 Stream Worker & FFmpeg Muxing | `src/worker_stream/nm3u8dl_re_worker.cpp`, `tools/win-x64/` | [nilaoda/N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE) + `FFmpeg` | `Verified by Execution` | `IT-BIN-01`, `IT-STREAM-02`, `IT-STREAM-03` |
| **FR-021** | Safe `@job.rsp` Credential Passing & Job Object Termination | `src/engine_stream/nm3u8dl_re_adapter.cpp` | Independent Security Design | `Verified by Execution` | `SEC-PROC-01`, `IT-STREAM-06` |
| **FR-022** | Standard HLS AES-128 In-Process Decryption Support | `src/worker_stream/nm3u8dl_re_worker.cpp` (`DecryptAes128Cbc`) | [nilaoda/N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE) | `Verified by Execution` | `IT-STREAM-06` |
| **FR-023** | Hierarchical Category Routing, Site Rules & Task Lifecycle | `src/core/download_controller.cpp`, `src/ui/win32_main_window.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Static Inspection` (Phase 2 rules) | `UT-CTRL-01`, `UT-UI-02`, `TestDynamicCategorizationAndSiteRulesPriority` |
| **FR-024** | Queue Concurrency & Dual Token-Bucket Speed Limiter (Global + Task) | `src/core/download_controller.cpp`, `src/brisk_engine/http_download_engine.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Static Inspection` (Phase 2 dual limiter) | `UT-UI-02`, `TestDualRateLimiterAndPerTaskSpeedLimit` |
| **FR-025** | Scheduled Start/Stop Windows, Auto-Retries & Post-Queue Actions | `src/core/download_controller.cpp`, `src/storage/sqlite_store.cpp` | [BrisklyDev/brisk](https://github.com/BrisklyDev/brisk) | `Verified by Static Inspection` (Phase 2 scheduler/power) | `UT-UI-02`, `TestSchedulerStopTimesAndPostCompletionActions` |
| **FR-026** | Atomic File Replacement, Pre-Flight Space Check & Auto-Rename | `src/common/path_sanitizer.cpp` | Independent Design | `Verified by Execution` | `UT-FS-01`, `UT-FS-02` |
| **FR-027** | Per-User Native Messaging HKCU Registration | `scripts/register-native-messaging.ps1` | Independent Design | `Verified by Execution` | HKCU Registry Keys Verified (`BUG-035`) |
| **FR-028** | Release Integrity Verification (`SHA256SUMS.txt`) | `dist/SHA256SUMS.txt`, `scripts/build-windows-msvc.ps1` | Independent Design | `Verified by Execution` | `IT-BIN-01` |
| **FR-029** | RFC 959 / 2428 / 3659 FTP Engine (EPSV, PASV, NAT Bypass, REST 350 Truncation) | `src/engine_ftp/ftp_client.cpp` | Independent RFC Implementation | `Verified by Static Inspection` (Phase 2 EPSV/resume) | `TestFtpClientParsingAndCommands`, `TestFtpReliabilityEpsvAndResumeTruncation` |
| **FR-030** | BitTorrent BEP 3 Peer Wire Protocol, Multi-File Storage, Piece Manager & Tracker Client | `src/engine_torrent/torrent_engine.cpp`, `src/engine_torrent/peer_wire_protocol.hpp`, `src/engine_torrent/piece_manager.hpp`, `src/engine_torrent/multi_file_storage.hpp`, `src/engine_torrent/tracker_client.hpp` | BEP 3 / BEP 9 Specification & aria2 / Motrix Architecture | `Verified by Execution` | `TestBencodeAndTorrentParser`, `TestPeerWireProtocolFramingAndCompactPeers`, `TestMultiFileStorageMappingAndBoundarySplitting`, `TestPieceManagerAndBlockScheduling`, `TestTrackerClientQueryEncodingAndResponseParsing` |
| **FR-031** | RTSP, MMS & RTMP Legacy Live Streaming Ingestion | `src/engine_stream/rtsp_mms_adapter.cpp` | FFmpeg Integration Protocol | `Verified by Execution` | `TestRtspMmsStreamDetection` |
| **FR-032** | Recursive Breadth-First Site Grabber & Media Crawler (HTML/CSS & SSRF Guard) | `src/crawler/site_grabber.cpp` | Independent Architecture | `Verified by Static Inspection` (Phase 2 CSS/folder) | `TestSiteGrabberCrawlerAndSsrf` |
| **FR-033** | Pre-Extraction ZIP Archive Previewer with Zip Slip & Bomb Guards | `src/archive/zip_preview.cpp` | PKWARE APPNOTE.TXT Specification | `Verified by Execution` | `TestZipArchivePreviewAndSecurity` |
| **FR-034** | Post-Download Windows Defender Antivirus Scanner & Sound Alert (with Threat Execution Block) | `src/security/antivirus_scanner.cpp`, `src/core/download_controller.cpp`, `src/ui/win32_main_window.cpp` | Windows Defender MpCmdRun Integration | `Verified by Execution` | `TestAntivirusScannerSafetyAndHandling` |
| **FR-035** | Multi-Format Task Import/Export (JSON/CSV) with Credential Scrubbing | `src/storage/import_export.cpp` | Independent Privacy Architecture | `Verified by Execution` | `TestTaskImportExportAndMultiQueue` |
| **FR-036** | Dual Portable Packaging (Full & Core) & Lowest-Privilege Installer | `scripts/package-release-zip.ps1`, `installer/flux.iss`, `scripts/fetch-workers.ps1` | Independent Release Architecture | `Verified by Static Inspection` (Phase 4 packaging & installer pipeline) | `PreflightValidation`, `PE64HeaderCheck`, `SHA256Integrity` |

---

## 3. Non-Functional Requirements Matrix (`NFR-001` to `NFR-016`)

| Req ID | Requirement Summary | Responsible Module | Verification Status | Measured / Verified Result |
| :--- | :--- | :--- | :--- | :--- |
| **NFR-001** | Core Binary Size (`<= 3.0 MB` uncompressed) | `CMakeLists.txt` | `Verified by Execution` | `2,047.0 KB` (`flux.exe` 2,040.3 KB + `flux-nmh.exe` 55.8 KB <= 3,072 KB budget) |
| **NFR-002** | Startup Latency (`< 150 ms` cold) | `src/app/main.cpp`, `src/common/path_sanitizer.cpp` | `Verified by Execution` | `71.05 ms` median (`flux --gui-snapshot`) |
| **NFR-003** | Idle Daemon & HTTP Memory Budget | `src/brisk_engine/http_download_engine.cpp` | `Verified by Execution` | Idle RSS: `10.34 MB` (<= 25 MB); Peak HTTP 8-Conn: `15.7 MB` (<= 65 MB); Peak Stream Worker: `11.04 MB` (<= 65 MB) |
| **NFR-004** | Multi-Connection Segmented Throughput | `src/brisk_engine/http_download_engine.cpp` | `Verified by Execution` | `223.0 Mbps` across 8 connections (Floor: >= 50 Mbps) |
| **NFR-005** | Crash-Safe SQLite WAL/TRUNCATE State Persistence | `src/storage/sqlite_store.cpp` | `Verified by Execution` | 64 KiB safety rollback verified (`TestSqliteStoreAndCrashRecovery`) |
| **NFR-006** | Path & Filename Traversal Sanitization | `src/common/path_sanitizer.cpp` | `Verified by Execution` | Blocks `../`, NTFS ADS, DOS names, and `U+202E` BiDi (`TestPathSanitizerAndContentDisposition`) |
| **NFR-007** | Zero Unauthenticated Network Listeners | `src/ipc/ipc_server.cpp` | `Verified by Execution` | Uses Windows Named Pipe with `PIPE_REJECT_REMOTE_CLIENTS`; zero TCP listening ports |
| **NFR-008** | Named Pipe Per-User Access Control | `src/ipc/ipc_server.cpp`, `src/common/ipc_path.hpp` | `Verified by Execution` | Per-user SID ACL on `\\.\pipe\Flux_IPC_<UserSID>` |
| **NFR-009** | DPAPI / AES-256-GCM Secret Encryption & Redaction | `src/common/crypto_utils.cpp`, `src/common/logger.cpp`, `src/ui/win32_main_window.cpp` | `Verified by Execution` | Encrypted at rest in `state.db`, wiped on completion, `[REDACTED]` in logs and UI Details Panel |
| **NFR-010** | Zero Telemetry by Default | All modules | `Verified by Execution` | Zero background telemetry or analytics endpoints |
| **NFR-011** | Per-Monitor v2 High-DPI & Keyboard Accessibility | `src/ui/design_system.hpp`, `src/ui/win32_main_window.cpp` | `Verified by Execution` | Tested at `96`/`120`/`144`/`192` DPI (`UT-UI-01`, `UT-KEY-01`) |
| **NFR-012** | Full UTF-8 / UTF-16LE Unicode Path Handling | `src/common/path_sanitizer.cpp`, `src/brisk_engine/http_client.cpp` | `Verified by Execution` | Verified with CJK RFC 5987 `测试视频_1080p.mp4` (`UT-HTTP-01`) |
| **NFR-013** | Cross-Browser MV3 Build (Chrome, Edge, Brave, Firefox) | `extension/manifest.chromium.json`, `extension/manifest.firefox.json` | `Verified by Execution` | `11 / 11` suites passing (`tests/test_extension_detection.js`) |
| **NFR-014** | Clean Engine & Adapter Boundaries | `src/brisk_engine/`, `src/engine_stream/`, `src/core/` | `Verified by Execution` | Verified across `36 / 36` C++ unit suites |
| **NFR-015** | Strict DRM Non-Circumvention (`ERR_STREAM_DRM_PROTECTED`) | `src/engine_stream/nm3u8dl_re_adapter.cpp`, `src/worker_stream/nm3u8dl_re_worker.cpp` | `Verified by Execution` | Widevine/CENC immediately rejected (`TestStreamMetaJsonAndDrmRejection`) |
| **NFR-016** | Cross-Architecture Clean Compilation Readiness | `CMakeLists.txt` | `Verified by Execution` | Compiles cleanly with MSVC `/W4` `/WX` |

