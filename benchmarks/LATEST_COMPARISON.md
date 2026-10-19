# Flux Download Manager — Automated Competitor Benchmark Report

**Status:** Initialized & Ready for Execution  
**Target Runner:** GitHub Actions Hosted Runner (`windows-2022`)  
**Workflow:** [`.github/workflows/benchmark-comparison.yml`](../.github/workflows/benchmark-comparison.yml)  
**Execution Mode:** Sequential on clean runner; zero synthetic data; full SHA-256 integrity verification  

---

## 1. Executive Summary & Tool Testability Status

This benchmark lab establishes an automated, repeatable performance comparison for **Flux Download Manager** against major download managers inside a controlled GitHub Actions Windows runner.

| Candidate Tool | Tested Configuration | Test Status | Technical Details & Transparency Notes |
| :--- | :---: | :---: | :--- |
| **Flux Download Manager** (`flux_8`) | 8 conns | **`READY`** | Native C++20 / WinHTTP segmented download engine with dynamic work-stealing |
| **Flux Download Manager** (`flux_32`) | 32 conns | **`READY`** | Native C++20 / WinHTTP segmented download engine with 32 maximum connections |
| **aria2** (`aria2_16`) | 16 conns | **`READY`** | Pinned official release `1.37.0` (`-x 16 -s 16 -k 1M`, SHA-256 verified) |
| **Internet Download Manager** (`idm_642`) | 16 conns | **`PENDING`** | Evaluated under headless CI licensing and unattended automation constraints |
| **Free Download Manager** (`fdm_634`) | 16 conns | **`PENDING`** | Evaluated under headless CLI availability constraints |

> [!NOTE]
> **Zero Hallucination Policy:** In accordance with repository standards, results are never collected from developer workstations or synthetically fabricated. When the GitHub Actions workflow runs, live measurements (wall time, throughput, working set RSS, CPU time, and server-side peak connections) will automatically populate this report.

---

## 2. Benchmark Scenarios

### Scenario A: 256 MiB High-Speed Range Download (Unthrottled)
- **Target Dataset:** `bench_256mb.bin` (256 MiB = 268,435,456 bytes)
- **Throttling Profile:** Unthrottled loopback
- **RFC 7233 Range Support:** Yes
- **Objective:** Measures raw throughput, segment scheduling efficiency, memory footprint, and CPU utilization.

### Scenario B: 64 MiB Multi-Connection Scaling (1 MiB/s per connection)
- **Target Dataset:** `bench_64mb.bin` (64 MiB = 67,108,864 bytes)
- **Throttling Profile:** 1 MiB/s (1,048,576 bytes/s) per connection
- **RFC 7233 Range Support:** Yes
- **Objective:** Demonstrates parallel work-stealing speedup under server-enforced per-connection rate limiting (8 vs 16 vs 32 connections).

### Scenario C: 16 MiB Non-Range Server (Single-Stream Fallback)
- **Target Dataset:** `bench_16mb.bin` (16 MiB = 16,777,216 bytes)
- **Throttling Profile:** Unthrottled loopback
- **RFC 7233 Range Support:** No (`Accept-Ranges: none`)
- **Objective:** Verifies graceful single-stream fallback when the server does not support byte ranges.

---

## 3. Workflow Trigger & Automation

To trigger this benchmark suite:
1. Navigate to **Actions** -> **Download Manager Benchmark Lab** in the repository.
2. Click **Run workflow** (optionally toggle `extended_mode` for 5 trials per tool).
3. The workflow runs sequentially on `windows-2022`, publishes the results directly to the job summary, uploads test artifacts, and commits updated reports to `main` with `[skip ci]`.
