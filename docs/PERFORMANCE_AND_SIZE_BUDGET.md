# Performance and Size Budget Specification

**Document Status:** Provisional Targets (Pre-Implementation — Zero Benchmarks Executed Yet)  
**Last Reviewed:** 2026-10-16  
**Scope:** Measurable engineering targets for binary size, startup latency, memory footprint, CPU utilization, disk I/O, and network throughput, plus the deterministic benchmarking methodology.  
**Related Documents:** [PRODUCT_REQUIREMENTS.md](PRODUCT_REQUIREMENTS.md), [TECHNOLOGY_STACK.md](TECHNOLOGY_STACK.md), [WINDOWS_BUILD_AND_PACKAGING.md](WINDOWS_BUILD_AND_PACKAGING.md), [TEST_STRATEGY.md](TEST_STRATEGY.md)

---

## 1. Important Notice on Provisional Targets

> **NO FABRICATED BENCHMARKS:**  
> In compliance with Section 2 and Section 6 of the Master Prompt, **no application code has been built or benchmarked yet**. All numerical budgets in Section 2 are **provisional engineering targets** derived from comparable native C++20/Win32 and `WinHTTP` baselines. Once Milestone 1 (`M1`) is compiled on a Windows 11 x64 host, actual measured values must be recorded alongside these targets.

---

## 2. Provisional Performance & Footprint Budgets

| Metric ID | Metric Name | Provisional Target Budget | Hard Ceiling (CI Gate Failure) | Related Requirement | Measurement Tool |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **BUD-001** | **Core Uncompressed Binary Size** (`openidm.exe` + `openidm-nmh.exe`) | `1.8 – 2.5 MB` | **`3.0 MB`** | `NFR-001` | PowerShell `(Get-Item openidm*.exe \| Measure-Object Length -Sum).Sum` |
| **BUD-002** | **Core Compressed Installer Size** (`OpenIDM-Setup-x64-Core.exe`) | `0.9 – 1.3 MB` | **`1.8 MB`** | `NFR-001` | File size of Inno Setup output |
| **BUD-003** | **Full Offline Installer Size** (including `N_m3u8DL-RE.exe` + minimal mux `ffmpeg.exe`) | `17.5 – 22.0 MB` (or `~34 MB` with full `ffmpeg.exe`) | **`38.0 MB`** | `ADR-0008` | File size of Inno Setup output |
| **BUD-004** | **Cold Startup Time** (Process creation to `WM_PAINT` of Main Window on NVMe SSD) | `35 – 75 ms` | **`100 ms`** | `NFR-002` | `QueryPerformanceCounter` logged from `wWinMain` entry to first `EndPaint` |
| **BUD-005** | **Warm Tray Restore Latency** (Tray icon click to visible window) | `5 – 12 ms` | **`20 ms`** | `NFR-002` | `QueryPerformanceCounter` delta |
| **BUD-006** | **Native Messaging Host Startup Latency** (`openidm-nmh.exe` spawn to first Named Pipe write) | `4 – 10 ms` | **`20 ms`** | `NFR-002` | Extension `performance.now()` around `system.handshake` |
| **BUD-007** | **Idle Memory (Private Working Set in System Tray)** with 1,000 tasks in `state.db` | `8 – 12 MB` | **`15 MB`** | `NFR-003` | `GetProcessMemoryInfo` (`PROCESS_MEMORY_COUNTERS_EX2.PrivateWorkingSetSize`) |
| **BUD-008** | **Peak Memory (`openidm.exe`)** during 3 concurrent 8-connection downloads at 1 Gbps | `35 – 50 MB` | **`65 MB`** | `NFR-003` | `GetProcessMemoryInfo` Peak Working Set |
| **BUD-009** | **Sustained Loopback/LAN Throughput** (Single 4 GiB file, 8 connections) | `>= 950 Mbps` (`~118 MB/s`) on 1 Gbps link; `>= 2.5 Gbps` on local loopback | **`>= 900 Mbps`** on 1 Gbps link | `NFR-004` | Local high-speed HTTP range test server (`tests/integration/perf_server.py` / nginx) |
| **BUD-010** | **CPU Utilization at 1 Gbps Saturated Download** (4-core / 8-thread x64 CPU) | `4% – 8%` total CPU (including 15 Hz UI progress bar redraws) | **`10%`** | `NFR-004` | `GetProcessTimes` (`KernelTime + UserTime` / wall clock) |
| **BUD-011** | **Disk Write Amplification on `state.db`** during active download | `<= 1` SQLite transaction per second (`< 16 KiB/s` WAL writes) | **`<= 2` tx/sec** | `NFR-005` | Windows Performance Recorder (WPR) / `GetProcessIoCounters` |
| **BUD-012** | **Browser Extension Content Script Overhead** (Per frame memory & bundle size) | `< 15 KB` minified JS; `< 1 ms` DOM observer execution on page load | **`25 KB` / `3 ms`** | `ADR-0004` | Chrome DevTools Performance profiler |

---

## 3. Memory Architecture That Enforces the Budget

To keep `openidm.exe` within `35–50 MB` during multi-gigabyte 1 Gbps downloads (`BUD-008`):
1. **Bounded Per-Worker Ring Buffers:** Each `WinHTTP` segment worker allocates exactly **two `64 KiB` buffers** (`128 KiB` per connection). Even at the maximum 32 connections across 3 concurrent tasks (`96` connections total), total network buffer allocation is bounded at **`12.0 MiB`**, and for the default 3 tasks x 8 connections (`24` connections), buffer memory is only **`3.0 MiB`**.
2. **Virtual ListView (`LVS_OWNERDATA`):** The Win32 task table does not allocate string copies inside `ComCtl32.dll` for invisible rows. It keeps a compact in-memory vector of `TaskSummaryRow` structs (`~160 bytes` per task = `1.6 MB` for 10,000 tasks) and formats text on demand inside `LVN_GETDISPINFO`.
3. **UI Update Throttling (`15 Hz` Coalescing Timer):** Network workers never post a `WM_APP_PROGRESS` message on every `64 KiB` socket read (which would flood the Win32 message queue with 1,800 messages/sec at 115 MB/s!). Instead, workers update atomic counters (`std::atomic<uint64_t> m_current_offset`), and a `66 ms` (`15 Hz`) UI timer polls dirty active tasks to repaint the progress bar.

---

## 4. Benchmarking Process & Reproducibility Protocol

When executing performance benchmarks in Milestone 1 and Milestone 4 (`PERF-SIZE-01`, `PERF-START-01`, `PERF-MEM-01`, `PERF-NET-01`):
1. **Hardware & OS Baseline Recording:** Every benchmark report committed to the repository must record the exact CPU model, RAM speed, SSD model, Windows 11 build (`winver`), and MSVC compiler version.
2. **Loopback Reference Server (`PERF-NET-01`):** Run a local native HTTP server (`nginx` on Windows or a C++ IOCP static buffer responder) serving an in-memory 4 GiB zero-copy buffer with `Accept-Ranges: bytes` so disk read bottlenecks on the server side do not skew client download throughput measurements.
3. **Automated Regression Gate:** The GitHub Actions CI workflow (`windows-2022` / `windows-11`) checks `BUD-001` (`openidm.exe` + `openidm-nmh.exe <= 3.0 MB`) on every pull request build.
