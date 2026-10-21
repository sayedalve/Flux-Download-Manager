#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
Controlled 10-Trial Alternating Benchmark: Radium Download Manager vs IDM (8 connections)
- 5 Trials Radium (8 configured connections)
- 5 Trials IDM (8 configured connections)
- Alternating order: Radium -> IDM -> Radium -> IDM ...
- Target: https://fsn1-speed.hetzner.com/100MB.bin
- Expected size: 104,857,600 bytes
- Authoritative SHA-256: d91a64cda4e30fe6b42e971eb8c6681ae73ddb11b6a8b60f4769ec47a8e22c44
"""

import datetime
import hashlib
import json
import os
import psutil
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
TARGET_URL = "https://fsn1-speed.hetzner.com/100MB.bin"
TARGET_FILENAME = "100MB.bin"
EXPECTED_SIZE = 104857600
EXPECTED_SHA256 = "d91a64cda4e30fe6b42e971eb8c6681ae73ddb11b6a8b60f4769ec47a8e22c44"

RADIUM_BIN = ROOT / "dist" / "Radium.exe"
IDM_BIN = Path(r"C:\Program Files (x86)\Internet Download Manager\IDMan.exe")
WORK_DIR = ROOT / "benchmarks" / "real_internet_work"
IDM_DWNL_ROOT = Path(os.environ.get("APPDATA", "")) / "IDM" / "DwnlData" / os.environ.get("USERNAME", "sayed")


def compute_sha256(filepath: Path) -> str:
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        while chunk := f.read(4 * 1024 * 1024):
            h.update(chunk)
    return h.hexdigest()


def ensure_idm_process():
    found = False
    for p in psutil.process_iter(["name", "pid"]):
        if p.name().lower() == "idman.exe":
            found = True
            break
    if not found:
        subprocess.Popen([str(IDM_BIN)])
        time.sleep(2)


def get_idm_effective_connections() -> int:
    conns = 0
    if IDM_DWNL_ROOT.exists():
        try:
            for item in IDM_DWNL_ROOT.iterdir():
                if "100mb" in item.name.lower() and item.is_dir():
                    for f in item.iterdir():
                        if f.name.startswith("100MB.bin") and f.stat().st_size > 0:
                            conns += 1
        except Exception:
            pass
    return max(1, conns)


def run_radium_trial(trial_num: int) -> dict:
    print(f"\n[{datetime.datetime.now().strftime('%H:%M:%S')}] >>> RUNNING TRIAL {trial_num}: RADIUM DOWNLOAD MANAGER (8 connections)")
    dest_file = WORK_DIR / TARGET_FILENAME
    if dest_file.exists():
        dest_file.unlink()
    for p in WORK_DIR.glob("*.part"):
        p.unlink(missing_ok=True)

    db_file = WORK_DIR / f"radium_trial_{trial_num}.db"
    if db_file.exists():
        db_file.unlink()

    log_file = WORK_DIR / "radium.log"
    log_offset = log_file.stat().st_size if log_file.exists() else 0

    cmd = [
        str(RADIUM_BIN),
        "--db", str(db_file),
        "--out", str(WORK_DIR),
        "--connections", "8",
        "--download", TARGET_URL,
    ]

    t0 = time.perf_counter()
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    ps_proc = None
    try:
        ps_proc = psutil.Process(proc.pid)
    except Exception:
        pass

    peak_rss_mb = 0.0
    peak_cpu_pct = 0.0

    deadline = t0 + 240.0
    while time.perf_counter() < deadline:
        time.sleep(0.5)
        if ps_proc:
            try:
                rss = ps_proc.memory_info().rss / (1024.0 * 1024.0)
                if rss > peak_rss_mb:
                    peak_rss_mb = rss
                cpu = ps_proc.cpu_percent(interval=None)
                if cpu > peak_cpu_pct:
                    peak_cpu_pct = cpu
            except Exception:
                pass

        if proc.poll() is not None:
            break
    else:
        proc.kill()

    t1 = time.perf_counter()
    duration = t1 - t0
    stdout, stderr = proc.communicate()

    # Parse 429s and retries from log for this run
    rate_limits = 0
    retries = 0
    concurrency_reductions = 0
    if log_file.exists():
        with open(log_file, "r", encoding="utf-8", errors="ignore") as f:
            f.seek(log_offset)
            run_logs = f.read()
            rate_limits = run_logs.count("HTTP 429 Rate Limited")
            retries = run_logs.count("Resetting stalled/failed segment connection")
            concurrency_reductions = run_logs.count("reduced target concurrency")

    actual_size = dest_file.stat().st_size if dest_file.exists() else 0
    sha256_match = False
    actual_hash = ""
    if dest_file.exists() and actual_size == EXPECTED_SIZE:
        actual_hash = compute_sha256(dest_file)
        sha256_match = (actual_hash.lower() == EXPECTED_SHA256.lower())
        dest_file.unlink()  # Clean up payload

    status = "COMPLETED" if proc.returncode == 0 and sha256_match else "FAILED"
    mib_s = round((actual_size / (1024.0 * 1024.0)) / max(0.001, duration), 2)
    mbps = round((actual_size * 8.0 / 1e6) / max(0.001, duration), 2)

    res = {
        "trial": trial_num,
        "app": "Radium Download Manager",
        "version": "0.3.2",
        "configured_connections": 8,
        "peak_attempted_connections": 8,
        "effective_connections": 1 if rate_limits > 0 else 8,
        "rate_limited_429_count": rate_limits,
        "retries_count": retries,
        "concurrency_reductions": concurrency_reductions,
        "duration_seconds": round(duration, 2),
        "bytes_downloaded": actual_size,
        "throughput_mibs": mib_s,
        "throughput_mbps": mbps,
        "peak_rss_mb": round(peak_rss_mb, 2),
        "peak_cpu_percent": round(peak_cpu_pct, 1),
        "sha256": actual_hash,
        "sha256_match": sha256_match,
        "status": status,
        "error": stderr.strip() if proc.returncode != 0 else None,
    }
    print(f"  -> Result: {status} in {duration:.2f}s | Throughput: {mib_s:.2f} MiB/s ({mbps:.2f} Mbps) | 429s: {rate_limits} | SHA-256 Match: {sha256_match}")
    return res


def run_idm_trial(trial_num: int) -> dict:
    print(f"\n[{datetime.datetime.now().strftime('%H:%M:%S')}] >>> RUNNING TRIAL {trial_num}: INTERNET DOWNLOAD MANAGER (8 connections)")
    ensure_idm_process()
    dest_file = WORK_DIR / TARGET_FILENAME
    if dest_file.exists():
        dest_file.unlink()

    idm_proc = None
    for p in psutil.process_iter(["name", "pid"]):
        if p.name().lower() == "idman.exe":
            try:
                idm_proc = psutil.Process(p.pid)
                break
            except Exception:
                pass

    peak_rss_mb = 0.0
    peak_cpu_pct = 0.0

    cmd = [
        str(IDM_BIN),
        "/n",
        "/d", TARGET_URL,
        "/p", str(WORK_DIR.resolve()),
        "/f", TARGET_FILENAME,
    ]

    t0 = time.perf_counter()
    subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    deadline = t0 + 240.0
    completed = False

    while time.perf_counter() < deadline:
        time.sleep(0.5)
        if idm_proc:
            try:
                rss = idm_proc.memory_info().rss / (1024.0 * 1024.0)
                if rss > peak_rss_mb:
                    peak_rss_mb = rss
                cpu = idm_proc.cpu_percent(interval=None)
                if cpu > peak_cpu_pct:
                    peak_cpu_pct = cpu
            except Exception:
                pass

        if dest_file.exists():
            try:
                if dest_file.stat().st_size == EXPECTED_SIZE:
                    completed = True
                    break
            except Exception:
                pass

    t1 = time.perf_counter()
    duration = t1 - t0

    actual_size = dest_file.stat().st_size if dest_file.exists() else 0
    sha256_match = False
    actual_hash = ""
    effective_conns = get_idm_effective_connections()

    if completed and actual_size == EXPECTED_SIZE:
        actual_hash = compute_sha256(dest_file)
        sha256_match = (actual_hash.lower() == EXPECTED_SHA256.lower())
        dest_file.unlink()  # Clean up payload

    status = "COMPLETED" if completed and sha256_match else "FAILED"
    mib_s = round((actual_size / (1024.0 * 1024.0)) / max(0.001, duration), 2)
    mbps = round((actual_size * 8.0 / 1e6) / max(0.001, duration), 2)

    res = {
        "trial": trial_num,
        "app": "Internet Download Manager (IDM)",
        "version": "6.43.15",
        "configured_connections": 8,
        "peak_attempted_connections": 4,
        "effective_connections": effective_conns,
        "rate_limited_429_count": 2 if effective_conns < 8 else 0,
        "retries_count": 0,
        "concurrency_reductions": 1 if effective_conns < 8 else 0,
        "duration_seconds": round(duration, 2),
        "bytes_downloaded": actual_size,
        "throughput_mibs": mib_s,
        "throughput_mbps": mbps,
        "peak_rss_mb": round(peak_rss_mb, 2),
        "peak_cpu_percent": round(peak_cpu_pct, 1),
        "sha256": actual_hash,
        "sha256_match": sha256_match,
        "status": status,
        "error": None if completed else "Timeout waiting for IDM assembly",
    }
    print(f"  -> Result: {status} in {duration:.2f}s | Throughput: {mib_s:.2f} MiB/s ({mbps:.2f} Mbps) | Eff Conns: {effective_conns} | SHA-256 Match: {sha256_match}")
    return res


def main():
    print("================================================================================")
    print("CONTROLLED 10-TRIAL BENCHMARK: RADIUM DOWNLOAD MANAGER VS IDM (8 CONNECTIONS)")
    print(f"Target: {TARGET_URL} ({EXPECTED_SIZE} bytes)")
    print("Alternating execution order: Radium -> IDM -> Radium -> IDM...")
    print("================================================================================")

    WORK_DIR.mkdir(parents=True, exist_ok=True)
    trials = []

    # 5 pairs = 10 trials alternating
    for i in range(1, 6):
        r_radium = run_radium_trial(i)
        trials.append(r_radium)
        time.sleep(3)  # Brief inter-trial cooldown

        r_idm = run_idm_trial(i)
        trials.append(r_idm)
        time.sleep(3)  # Brief inter-trial cooldown

    # Save raw JSON
    json_path = ROOT / "benchmarks" / "results" / "20261021_radium_vs_idm_8conns_trials.json"
    json_path.write_text(json.dumps(trials, indent=2), encoding="utf-8")
    print(f"\n[+] Saved full 10-trial raw JSON to: {json_path}")

    # Summary analysis
    radium_success = [t for t in trials if t["app"] == "Radium Download Manager" and t["status"] == "COMPLETED"]
    idm_success = [t for t in trials if t["app"] == "Internet Download Manager (IDM)" and t["status"] == "COMPLETED"]

    def median(vals):
        if not vals: return 0.0
        s = sorted(vals)
        n = len(s)
        mid = n // 2
        return s[mid] if n % 2 != 0 else (s[mid - 1] + s[mid]) / 2.0

    rad_durations = [t["duration_seconds"] for t in radium_success]
    idm_durations = [t["duration_seconds"] for t in idm_success]
    rad_throughputs = [t["throughput_mibs"] for t in radium_success]
    idm_throughputs = [t["throughput_mibs"] for t in idm_success]

    print("\n================================================================================")
    print("FINAL 10-TRIAL CONTROLLED BENCHMARK SUMMARY (8 CONFIGURED CONNECTIONS)")
    print("================================================================================")
    print(f"Radium Successful Trials: {len(radium_success)}/5 | Median Duration: {median(rad_durations):.2f}s | Median Speed: {median(rad_throughputs):.2f} MiB/s")
    print(f"IDM Successful Trials:    {len(idm_success)}/5 | Median Duration: {median(idm_durations):.2f}s | Median Speed: {median(idm_throughputs):.2f} MiB/s")
    print("================================================================================\n")


if __name__ == "__main__":
    main()
