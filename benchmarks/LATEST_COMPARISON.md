# Flux Download Manager — Automated Competitor Benchmark Report

**Execution Timestamp (UTC):** `2026-10-20`  
**Host Platform:** `Windows 2022Server (AMD64)`  
**Processor:** `AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD` (4 logical cores)  
**System RAM:** `15.99 GB`  
**Runner Label:** `GitHub Actions 1000005542` (GitHub Actions: `True`)  
**Commit SHA:** [`d320692`](https://github.com/sayedalve/Flux-Download-Manager/commit/d320692fa108370a5724240d61335004f4f51ec0)  

---

## 1. Executive Summary & Tool Testability Status

This automated benchmark suite executes a controlled, repeatable performance and resource comparison between **Flux Download Manager** and major industry download engines on a dedicated Windows runner.

| Candidate Tool | Tested Configuration | Acquisition / Test Status | Technical Details & Transparency Notes |
| :--- | :--- | :--- | :--- |
| **Flux Download Manager** (`flux_8`) | 8 conns | **`READY`** | Native C++20 / WinHTTP segmented download engine (8 connections) |
| **Flux Download Manager** (`flux_32`) | 32 conns | **`READY`** | Native C++20 / WinHTTP segmented download engine (32 connections) |
| **aria2** (`aria2_16`) | 16 conns | **`READY`** | aria2 1.37.0 official x64 build (-x 16 -s 16 -k 1M) |
| **Internet Download Manager** (`idm_642`) | 16 conns | **`SKIPPED`** | Unattended licensing dialog / interactive trial prompt requires desktop user session in CI; Tonec terms prohibit redistributable trial cracking |
| **Free Download Manager** (`fdm_634`) | 16 conns | **`SKIPPED`** | FDM v6 does not provide a headless CLI download/completion interface; requires interactive desktop GUI session |

> [!NOTE]
> **Zero Hallucination Policy Enforced:** In strict accordance with the repository benchmarking charter, aria2 1.37.0 was acquired from its official release and verified via SHA256. For proprietary GUI managers (IDM and FDM), unattended headless automation without desktop sessions or redistributable license crack injection is transparently documented as skipped rather than substituting unverified builds or synthesizing synthetic data.

---

## 2. Performance Comparison Across Scenarios

### Scenario A: 256 MiB High-Speed Range Download (Unthrottled)

Evaluates maximum unthrottled loopback throughput, segment scheduling, and memory footprint.

- **Target Dataset:** `bench_256mb.bin` (256 MiB)
- **Throttling Profile:** `Unthrottled loopback`
- **RFC 7233 Range Support:** `Yes`

| Tool | Connections | Status | Median Time | Median Throughput (MiB/s) | Median Throughput (Mbps) | Peak RSS (MB) | CPU Time (ms) | Peak Conns | SHA256 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Flux Download Manager** | 8 | `COMPLETED` | **0.897 s** | **285.36 MiB/s** | 2393.8 Mbps | 12.20 MB | 343.8 ms | 5 | PASS |
| **Flux Download Manager** | 32 | `COMPLETED` | **0.938 s** | **272.92 MiB/s** | 2289.4 Mbps | 12.24 MB | 296.9 ms | 5 | PASS |
| **aria2** | 16 | `COMPLETED` | **1.382 s** | **185.25 MiB/s** | 1554.0 Mbps | 21.00 MB | 453.1 ms | 13 | PASS |
| **Internet Download Manager** | 16 | `SKIPPED` | N/A | N/A | N/A | N/A | N/A | N/A | N/A |
| **Free Download Manager** | 16 | `SKIPPED` | N/A | N/A | N/A | N/A | N/A | N/A | N/A |


### Scenario B: 64 MiB Multi-Connection Scaling (1 MiB/s per connection)

Evaluates parallel work-stealing speedup under server-enforced per-connection rate limiting (8 vs 16 vs 32 connections).

- **Target Dataset:** `bench_64mb.bin` (64 MiB)
- **Throttling Profile:** `1 MiB/s per connection`
- **RFC 7233 Range Support:** `Yes`

| Tool | Connections | Status | Median Time | Median Throughput (MiB/s) | Median Throughput (Mbps) | Peak RSS (MB) | CPU Time (ms) | Peak Conns | SHA256 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Flux Download Manager** | 8 | `COMPLETED` | **8.474 s** | **7.55 MiB/s** | 63.4 Mbps | 12.31 MB | 15.6 ms | 8 | PASS |
| **Flux Download Manager** | 32 | `COMPLETED` | **3.150 s** | **20.32 MiB/s** | 170.4 Mbps | 16.34 MB | 46.9 ms | 32 | PASS |
| **aria2** | 16 | `COMPLETED` | **4.560 s** | **14.04 MiB/s** | 117.7 Mbps | 24.68 MB | 125.0 ms | 16 | PASS |
| **Internet Download Manager** | 16 | `SKIPPED` | N/A | N/A | N/A | N/A | N/A | N/A | N/A |
| **Free Download Manager** | 16 | `SKIPPED` | N/A | N/A | N/A | N/A | N/A | N/A | N/A |


### Scenario C: 16 MiB Non-Range Server (Single-Stream Fallback)

Verifies graceful single-stream fallback when server does not support RFC 7233 byte ranges.

- **Target Dataset:** `bench_16mb.bin` (16 MiB)
- **Throttling Profile:** `Unthrottled loopback`
- **RFC 7233 Range Support:** `No (Single-stream fallback)`

| Tool | Connections | Status | Median Time | Median Throughput (MiB/s) | Median Throughput (Mbps) | Peak RSS (MB) | CPU Time (ms) | Peak Conns | SHA256 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Flux Download Manager** | 8 | `COMPLETED` | **0.202 s** | **79.20 MiB/s** | 664.4 Mbps | 11.89 MB | 46.9 ms | 1 | PASS |
| **Flux Download Manager** | 32 | `COMPLETED` | **0.207 s** | **77.29 MiB/s** | 648.3 Mbps | 11.88 MB | 31.2 ms | 1 | PASS |
| **aria2** | 16 | `COMPLETED` | **0.091 s** | **174.83 MiB/s** | 1466.6 Mbps | 11.44 MB | 78.1 ms | 10 | PASS |
| **Internet Download Manager** | 16 | `SKIPPED` | N/A | N/A | N/A | N/A | N/A | N/A | N/A |
| **Free Download Manager** | 16 | `SKIPPED` | N/A | N/A | N/A | N/A | N/A | N/A | N/A |


---

## 3. Analysis of Multi-Connection Work-Stealing Efficiency

In **Scenario B** (throttled at 1 MiB/s per connection), the download engine's ability to spawn and saturate parallel connections is directly measured:

1. **Flux (32 connections)** dynamically scales connections up to the configured limit, providing maximum throughput on rate-limited CDN endpoints.
2. **Flux (8 connections)** saturates exactly 8 connections with minimal CPU and working set memory footprint.
3. **Brisk Work-Stealing Algorithm:** Splits remaining segments dynamically without blocking or leaving unmerged chunk files.
4. **Zero Leftover Temporary Files:** All `.part` temporary files are cleanly renamed upon completion, leaving 0 orphan files.

---

## 4. Benchmark Methodology & Reproducibility

To reproduce this exact benchmark suite locally or on a private CI runner:

```powershell
# 1. Build Flux Release binary
cmake -B build -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target flux

# 2. Execute benchmark comparison suite
python scripts/run-competitor-benchmarks.py
```
