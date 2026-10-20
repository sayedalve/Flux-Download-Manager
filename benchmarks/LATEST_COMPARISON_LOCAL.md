# Flux Download Manager — Verified Local Competitor Benchmark Report

**Execution Environment:** `Local Developer Workstation (Windows 11)`  
**Execution Timestamp (UTC):** `2026-10-20`  
**Host Platform:** `Windows 11 (AMD64)`  
**Processor:** `Intel64 Family 6 Model 142 Stepping 12, GenuineIntel` (8 logical cores)  
**System RAM:** `15.82 GB`  
**Runner Label:** `DESKTOP-0PPMPK2` (GitHub Actions: `False`)  
**Commit SHA:** [`e85e388`](https://github.com/sayedalve/Flux-Download-Manager/commit/e85e388af796fdcf01e213deb34df5e168f2eb55)  

---

## 1. Executive Summary & Tool Testability Status

This local benchmark suite executes a controlled, repeatable performance and resource comparison between **Flux Download Manager**, **aria2 1.37.0**, authentic licensed **Internet Download Manager (IDM 6.43.15)**, and **Free Download Manager (FDM 6.33.1.6648)** directly on the developer's workstation.

| Candidate Tool | Tested Configuration | Acquisition / Test Status | Technical Details & Transparency Notes |
| :--- | :--- | :--- | :--- |
| **Flux Download Manager** (`flux_8`) | 8 conns | **`READY`** | Native C++20 / WinHTTP segmented download engine (8 connections) |
| **Flux Download Manager** (`flux_32`) | 32 conns | **`READY`** | Native C++20 / WinHTTP segmented download engine (32 connections) |
| **aria2** (`aria2_16`) | 16 conns | **`READY`** | aria2 1.37.0 official x64 build (-x 16 -s 16 -k 1M) |
| **Internet Download Manager** (`idm`) | 16 conns | **`READY`** | IDM 6.43.15 licensed Windows x64 install (silent /n automation) |
| **Free Download Manager** (`fdm`) | 16 conns | **`READY`** | FDM 6.33.1.6648 Windows x64 install (silent headless profile automation) |

> [!NOTE]
> **Verified Local Workstation Execution:** In strict accordance with repository benchmarking governance, all 5 competitors were tested natively on the developer's Windows 11 machine without mocks or emulators. IDM 6.43.15 was driven via its native Win32 silent execution interface (`/n /d /p /f`), and FDM 6.33.1.6648 was automated via its native unattended database profile mode to target trial-specific isolated directories. Every completed file was verified against 100% authoritative SHA-256 digests on loopback (127.0.0.1) with zero lingering temporary files.

---

## 2. Performance Comparison Across Scenarios

### Scenario A: 256 MiB High-Speed Range Download (Unthrottled)

Evaluates maximum unthrottled loopback throughput, segment scheduling, and memory footprint.

- **Target Dataset:** `bench_256mb.bin` (256 MiB)
- **Throttling Profile:** `Unthrottled loopback`
- **RFC 7233 Range Support:** `Yes`

| Tool | Connections | Status | Median Time | Median Throughput (MiB/s) | Median Throughput (Mbps) | Peak RSS (MB) | CPU Time (ms) | Peak Conns | SHA256 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Flux Download Manager** | 8 | `COMPLETED` | **2.428 s** | **105.44 MiB/s** | 884.5 Mbps | 17.00 MB | 1453.1 ms | 8 | PASS |
| **Flux Download Manager** | 32 | `COMPLETED` | **1.520 s** | **168.48 MiB/s** | 1413.3 Mbps | 17.37 MB | 1421.9 ms | 15 | PASS |
| **aria2** | 16 | `COMPLETED` | **2.582 s** | **99.13 MiB/s** | 831.6 Mbps | 26.88 MB | 1843.8 ms | 16 | PASS |
| **Internet Download Manager** | 16 | `COMPLETED` | **6.699 s** | **38.22 MiB/s** | 320.6 Mbps | 54.18 MB | 3171.9 ms | 8 | PASS |
| **Free Download Manager** | 16 | `COMPLETED` | **1.971 s** | **129.88 MiB/s** | 1089.5 Mbps | 577.56 MB | 2593.8 ms | 9 | PASS |


### Scenario B: 64 MiB Multi-Connection Scaling (1 MiB/s per connection)

Evaluates parallel work-stealing speedup under server-enforced per-connection rate limiting (8 vs 16 vs 32 connections).

- **Target Dataset:** `bench_64mb.bin` (64 MiB)
- **Throttling Profile:** `1 MiB/s per connection`
- **RFC 7233 Range Support:** `Yes`

| Tool | Connections | Status | Median Time | Median Throughput (MiB/s) | Median Throughput (Mbps) | Peak RSS (MB) | CPU Time (ms) | Peak Conns | SHA256 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Flux Download Manager** | 8 | `COMPLETED` | **8.819 s** | **7.26 MiB/s** | 60.9 Mbps | 16.52 MB | 609.4 ms | 8 | PASS |
| **Flux Download Manager** | 32 | `COMPLETED` | **3.474 s** | **18.42 MiB/s** | 154.6 Mbps | 19.64 MB | 859.4 ms | 32 | PASS |
| **aria2** | 16 | `COMPLETED` | **4.518 s** | **14.17 MiB/s** | 118.8 Mbps | 27.54 MB | 1031.2 ms | 16 | PASS |
| **Internet Download Manager** | 16 | `COMPLETED` | **13.377 s** | **4.78 MiB/s** | 40.1 Mbps | 54.41 MB | 5828.1 ms | 8 | PASS |
| **Free Download Manager** | 16 | `COMPLETED` | **5.243 s** | **12.21 MiB/s** | 102.4 Mbps | 344.31 MB | 4000.0 ms | 18 | PASS |


### Scenario C: 16 MiB Non-Range Server (Single-Stream Fallback)

Verifies graceful single-stream fallback when server does not support RFC 7233 byte ranges.

- **Target Dataset:** `bench_16mb.bin` (16 MiB)
- **Throttling Profile:** `Unthrottled loopback`
- **RFC 7233 Range Support:** `No (Single-stream fallback)`

| Tool | Connections | Status | Median Time | Median Throughput (MiB/s) | Median Throughput (Mbps) | Peak RSS (MB) | CPU Time (ms) | Peak Conns | SHA256 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Flux Download Manager** | 8 | `COMPLETED` | **0.546 s** | **29.28 MiB/s** | 245.7 Mbps | 16.09 MB | 281.2 ms | 1 | PASS |
| **Flux Download Manager** | 32 | `COMPLETED` | **0.518 s** | **30.91 MiB/s** | 259.3 Mbps | 15.98 MB | 343.8 ms | 1 | PASS |
| **aria2** | 16 | `COMPLETED` | **0.459 s** | **34.85 MiB/s** | 292.3 Mbps | 13.64 MB | 390.6 ms | 14 | PASS |
| **Internet Download Manager** | 16 | `COMPLETED` | **4.362 s** | **3.67 MiB/s** | 30.8 Mbps | 54.85 MB | 921.9 ms | 1 | PASS |
| **Free Download Manager** | 16 | `COMPLETED` | **0.570 s** | **28.07 MiB/s** | 235.4 Mbps | 343.83 MB | 328.1 ms | 2 | PASS |


---

## 3. Analysis of Multi-Connection Work-Stealing Efficiency

In **Scenario B** (throttled at 1 MiB/s per connection), the download engine's ability to spawn and saturate parallel connections is directly measured:

1. **Flux (32 connections)** dynamically scales connections up to the configured limit, providing maximum throughput on rate-limited CDN endpoints.
2. **Flux (8 connections)** saturates exactly 8 connections with minimal CPU and working set memory footprint.
3. **Brisk Work-Stealing Algorithm:** Splits remaining segments dynamically without blocking or leaving unmerged chunk files.
4. **Zero Leftover Temporary Files:** All temporary files are cleanly renamed upon completion, leaving 0 orphan files.

---

## 4. Benchmark Methodology & Reproducibility

To reproduce this exact benchmark suite locally or on a private CI runner:

```powershell
# 1. Build Flux Release binary
cmake -B build -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target flux

# 2. Execute benchmark comparison suite
python scripts/run-competitor-benchmarks.py --mode local
```
