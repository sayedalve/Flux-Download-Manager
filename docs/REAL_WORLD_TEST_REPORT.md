# Real-World Reliability Audit Report

**Audit date:** 2026-10-21  
**Repository:** Radium Download Manager / OpenIDM  
**Scope:** Comprehensive reliability audit covering concurrency stress, segmented rate limiting, worker lifecycle, crash recovery, and Windows-specific system integrations. Existing user changes were strictly preserved.

---

## Executive Result

The baseline test suites and integration tests passed. During continuation auditing, the HTTP segmented engine was remediated for rate limiting (HTTP 429), worker retirement, and unassigned segment coordination. 

Crucially, **the real-world public-network benchmark against Hetzner's 100 MiB test file was successfully executed and verified end-to-end**:
- **Initial concurrency:** 32 configured connections.
- **Observed server response:** HTTP 429 Too Many Requests (rate limiting).
- **Adaptive fallback:** Concurrency progressively scaled down ($21 \rightarrow 16 \rightarrow 8 \rightarrow 4 \rightarrow 2 \rightarrow 1$).
- **Range protocol classification:** 0 `ERR_HTTP_RANGE_PROTOCOL` errors (no false range failures).
- **Data integrity:** Exact byte size (`104,857,600` bytes) and **exact match** to authoritative SHA-256 (`d91a64cda4e30fe6b42e971eb8c6681ae73ddb11b6a8b60f4769ec47a8e22c44`).

Additionally, post-download Windows Defender scanning was audited and corrected: `MpCmdRun.exe` path normalization prevents exit code 2 errors on relative paths from falsely flagging clean downloads as malware.

> [!IMPORTANT]
> **Production Readiness Statement:** Radium is **not** declared production-ready solely because the current test suites and Hetzner benchmark pass. While core segmentation, rate-limit recovery, and crash resume are solid, interactive Windows GUI automation under unpredictable system interruptions, low-level disk exhaustion during concurrent writes, and edge-case browser handoffs require further multi-day testing across diverse Windows 11 host environments.

---

## Baseline Execution Matrix

| Test Suite | Initial Result | Final Result |
|---|---:|---:|
| Core & Engine Unit Tests (`radium_unit_tests.exe`) | 13/13 passed | **64/64 passed** |
| Extension Unit Harness | 16/16 passed | 16/16 passed |
| Playwright Browser Runtime Integration | 8/8 passed | 8/8 passed |
| Core Controller Operations | 10/10 passed | 10/10 passed |
| Deterministic Integration & Performance Benchmarks (`run_all_integration_tests.py`) | Failed at IPC handshake (leaked child process) | **8/8 passed** |
| Live Public Internet 100 MiB Benchmark | *Superseded (originally unrun)* | **PASSED (100% verified)** |

---

## Verified by Execution

### 1. Live Hetzner 100 MiB Public Internet Benchmark
Executed live against `https://fsn1-speed.hetzner.com/100MB.bin` on a Windows 11 host with 32 connections (full machine-readable artifact at [`benchmarks/results/20261021_hetzner_100mb_radium.json`](file:///c:/Users/sayed/Downloads/openidm/benchmarks/results/20261021_hetzner_100mb_radium.json), summary table at [`benchmarks/RESULTS.md`](file:///c:/Users/sayed/Downloads/openidm/benchmarks/RESULTS.md), and comparison spreadsheet at [`benchmarks/results/summary_comparison.csv`](file:///c:/Users/sayed/Downloads/openidm/benchmarks/results/summary_comparison.csv)):
- **Target URL:** `https://fsn1-speed.hetzner.com/100MB.bin`
- **Configured Concurrency:** 32 connections
- **Peak Concurrency Attempted:** 21 connections spawned before rate-limit triggered
- **Server Rate-Limit Signaling:** HTTP 429 Too Many Requests
- **Adaptive Fallback Trajectory:** Concurrency scaled down $21 \rightarrow 16 \rightarrow 8 \rightarrow 4 \rightarrow 2 \rightarrow 1$ active worker
- **Worker Retirement Behavior:** Retiring workers safely released assigned segment ranges back to the pool, decremented `running_workers`, and terminated cleanly without deadlocking or racing
- **Segment Reassignment:** Surviving / re-spawned workers cleanly acquired and completed remaining segments
- **Retries Handled:** 28 rate-limited retries handled with exponential backoff and `Retry-After` adherence
- **Range Incompatibility Protocol Errors:** **0** (eliminated the defect where 429 was misclassified as `ERR_HTTP_RANGE_PROTOCOL`)
- **Total Duration:** 120.95 seconds (~2.01 min)
- **Calculated Throughput:** **0.83 MiB/s** (6.94 Mbps)
- **Effective Transfer Concurrency:** 1 connection (due to server-side IP rate limiting)
- **Downloaded File Size:** Exactly `104,857,600` bytes
- **Calculated SHA-256:** `d91a64cda4e30fe6b42e971eb8c6681ae73ddb11b6a8b60f4769ec47a8e22c44`
- **Authoritative SHA-256:** `d91a64cda4e30fe6b42e971eb8c6681ae73ddb11b6a8b60f4769ec47a8e22c44`
- **Integrity Result:** **BYTE-FOR-BYTE IDENTICAL MATCH**

### 2. Windows Defender Antivirus Scanner Integration
Audited `AntivirusScanner::ScanWithDefender` in `src/security/antivirus_scanner.cpp`:
- **Absolute Path Normalization:** `MpCmdRun.exe` fails with `CmdTool: Failed with hr = 0x80508023` (exit code 2) if passed a relative path. Path normalization via `std::filesystem::absolute()` was verified.
- **Clean File Verification:** Verified scanning clean files (e.g. `100MB.bin`) produces `Scanning ... found no threats` and exit code 0 (`ScanStatus::kClean`).
- **Missing File Behavior:** Verified nonexistent file path returns `ERR_FILE_NOT_FOUND` before spawning Defender.
- **Defender Error vs Threat Detection:** Verified that exit code 2 is properly disambiguated:
  - Scanner error outputs (`Failed with hr =`, `ERROR:`, `error`) map strictly to `ScanStatus::kScannerError` (does NOT treat a scanner tool error as malware).
  - Actual infected files (tested with EICAR standard test string) trigger threat reporting and map to `ScanStatus::kThreatDetected`.
- **Controller Post-Download Action:** Verified in `DownloadController`: `kScannerError` leaves the completed task marked `Completed (Antivirus Scan Failed)` without deleting the user's file. Real threats (`kThreatDetected`) safely delete the infected file and fail the task with `ERR_THREAT_DETECTED`.

### 3. Core Engine & Deterministic Integration Test Suites
- **Unit & Regression Suites:** 64 / 64 suites passed in `radium_unit_tests.exe`.
- **End-to-End Integration Suite:** All 8 suites passed in `tests/run_all_integration_tests.py`:
  - Native binary PE32+ validation
  - 8-connection segmented HTTP range download with 302 redirect
  - Non-range server single-stream fallback
  - Dropped connection reset & resume from exact offset
  - HLS & DASH probe, stream segment download & FFmpeg muxing
  - Native Messaging Host RPC, AES-128 decryption, log redaction, and UI snapshot
  - 8-metric performance budget compliance: startup 38.66 ms, idle RSS 10.72 MB, peak HTTP RSS 16.09 MB, UI snapshot 85.44 ms, throughput 410.2 Mbps.

---

## Reconciliation of Prior Audit Statements

| Prior Audit Statement | Status in Current Codebase | Current Evidence |
| :--- | :--- | :--- |
| *"Public-network Hetzner 100 MiB 429 reproduction and effective-concurrency telemetry unverified"* | **Superseded & Verified** | Live benchmark successfully executed against Hetzner 100MB file with 32 connections, falling back to 1 connection, 28 retries, 120.95s, exact SHA-256 match. |
| *"Test-harness child-process leak"* | **Resolved** | `tests/run_all_integration_tests.py` `finally` block unconditionally terminates daemon and NMH processes. |
| *"HTTP 429 probe retry limitation"* | **Resolved** | `http_download_engine.cpp` updated with bounded 3-attempt probe retry honoring `Retry-After`. |
| *"Range protocol error on 429"* | **Resolved** | Range satisfaction validation strictly distinguishes 429/5xx status codes from genuine range header refusal (200 OK / mismatched range start). |
| *"Worker counter race during adaptive concurrency reduction"* | **Resolved** | Retiring workers decrement `running_workers` via `fetch_sub` and coordinator loop checks `scheduler.HasIncompleteUnassignedSegment()`. |

---

## What Remains Untested / Future Hardening

1. **Host-Level Sudden Power Loss / OS Crash during Multi-Stream PWrite:**
   While controlled termination and SQLite WAL reconciliation pass, testing power cut simulation or VM hard reset during simultaneous segment disk flushes remains outside standard CI capability.
2. **Exotic Anti-Cheat and Third-Party Antivirus File Locks:**
   Antivirus solutions that temporarily lock partial `.part` files during byte inspection without sharing rights could theoretically cause transient sharing violations (`ERROR_SHARING_VIOLATION`) if not retried with backoff.
3. **Interactive Windows 11 GUI Automation under DWM Composition Resets:**
   DirectComposition / Win32 DPI scaling transitions across multiple heterogeneous monitors during active multi-queue download transfers.
