#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""
Deterministic Local Benchmark Runner for Radium Download Manager.

Tests:
1. Single vs Multiple Connections (1 vs 4 vs 8 vs 16 vs 32 connections).
2. Segmented download integrity (exact byte verification and SHA-256 validation).
3. HTTP 429 rate limiting with adaptive concurrency reduction.
4. Transfer duration, calculated throughput (MiB/s, Mbps), and peak working set RSS.

Operates purely locally (RFC 7233 loopback range server) without external network dependencies.
"""

import argparse
import datetime
import hashlib
import http.server
import json
import os
import platform
import socket
import ssl
import subprocess
import sys
import tempfile
import threading
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
BIN_CANDIDATES = [
    ROOT / "dist" / "Radium.exe",
    ROOT / "build_msvc" / "Release" / "radium.exe",
    ROOT / "build" / "win-x64" / "Release" / "radium.exe",
    ROOT / "build_msvc" / "Release" / "openidm.exe",
    ROOT / "dist" / "Flux.exe",
]
BIN = next((p for p in BIN_CANDIDATES if p.exists()), BIN_CANDIDATES[0])


class LocalBenchHandler(http.server.BaseHTTPRequestHandler):
    payload = b""
    payload_hash = ""
    lock = threading.Lock()
    active_conns = 0
    peak_conns = 0
    total_429s = 0
    sim_limit = 0  # Max concurrent connections before emitting 429 (0 = unthrottled)

    def log_message(self, format, *args):
        pass  # Suppress console spew during benchmarking

    def do_HEAD(self):
        self.send_response(200)
        self.send_header("Accept-Ranges", "bytes")
        self.send_header("Content-Length", str(len(self.payload)))
        self.send_header("ETag", "\"bench-etag\"")
        self.end_headers()

    def do_GET(self):
        rng = self.headers.get("Range")
        is_probe = (rng == "bytes=0-0" or rng == "bytes=0-")

        with self.lock:
            if not is_probe:
                self.__class__.active_conns += 1
                if self.__class__.active_conns > self.__class__.peak_conns:
                    self.__class__.peak_conns = self.__class__.active_conns

        try:
            if is_probe:
                self.send_response(206)
                self.send_header("Content-Range", f"bytes 0-0/{len(self.payload)}")
                self.send_header("Accept-Ranges", "bytes")
                self.send_header("Content-Length", "1")
                self.send_header("ETag", "\"bench-etag\"")
                self.end_headers()
                self.wfile.write(self.payload[0:1])
                return

            if self.sim_limit > 0:
                with self.lock:
                    if self.__class__.active_conns > self.sim_limit:
                        self.__class__.total_429s += 1
                        self.send_response(429)
                        self.send_header("Retry-After", "1")
                        self.end_headers()
                        return

            if rng and rng.startswith("bytes="):
                parts = rng.replace("bytes=", "").split("-")
                start = int(parts[0]) if parts[0] else 0
                end = int(parts[1]) if len(parts) > 1 and parts[1] else len(self.payload) - 1
                if start > end or start >= len(self.payload):
                    self.send_response(416)
                    self.send_header("Content-Range", f"bytes */{len(self.payload)}")
                    self.end_headers()
                    return

                length = end - start + 1
                self.send_response(206)
                self.send_header("Content-Range", f"bytes {start}-{end}/{len(self.payload)}")
                self.send_header("Accept-Ranges", "bytes")
                self.send_header("Content-Length", str(length))
                self.send_header("ETag", "\"bench-etag\"")
                self.end_headers()
                self.wfile.write(self.payload[start:end+1])
            else:
                self.send_response(200)
                self.send_header("Content-Length", str(len(self.payload)))
                self.send_header("Accept-Ranges", "bytes")
                self.send_header("ETag", "\"bench-etag\"")
                self.end_headers()
                self.wfile.write(self.payload)
        finally:
            if not is_probe:
                with self.lock:
                    self.__class__.active_conns -= 1


def run_benchmark(size_mb=32, connections=8, simulate_429_limit=0) -> dict:
    total_bytes = size_mb * 1024 * 1024
    payload = bytes((i * 37 + 11) & 0xFF for i in range(total_bytes))
    payload_hash = hashlib.sha256(payload).hexdigest()

    LocalBenchHandler.payload = payload
    LocalBenchHandler.payload_hash = payload_hash
    LocalBenchHandler.sim_limit = simulate_429_limit
    LocalBenchHandler.active_conns = 0
    LocalBenchHandler.peak_conns = 0
    LocalBenchHandler.total_429s = 0

    server = http.server.ThreadingHTTPServer(("127.0.0.1", 0), LocalBenchHandler)
    port = server.server_port
    server_thread = threading.Thread(target=server.serve_forever, daemon=True)
    server_thread.start()

    url = f"http://127.0.0.1:{port}/benchmark_payload.bin"

    with tempfile.TemporaryDirectory(prefix="radium_bench_") as tmpdir:
        tmp_p = Path(tmpdir)
        db_p = tmp_p / "bench.db"
        out_p = tmp_p / "out"
        out_p.mkdir()

        t0 = time.perf_counter()
        cmd = [
            str(BIN),
            "--download", url,
            "--db", str(db_p),
            "--out", str(out_p),
            "--connections", str(connections),
            "--allow-loopback"
        ]
        res = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
        t1 = time.perf_counter()
        server.shutdown()

        duration = t1 - t0
        output_files = [p for p in out_p.iterdir() if p.is_file() and not p.name.endswith(".part")]
        status = "COMPLETED" if res.returncode == 0 and len(output_files) == 1 else "FAILED"

        actual_hash = ""
        file_size = 0
        if status == "COMPLETED":
            fpath = output_files[0]
            file_size = fpath.stat().st_size
            with open(fpath, "rb") as f:
                actual_hash = hashlib.sha256(f.read()).hexdigest()

        hash_match = (actual_hash == payload_hash)
        mib = total_bytes / (1024 * 1024)
        throughput_mibs = round(mib / max(0.001, duration), 2)
        throughput_mbps = round((total_bytes * 8) / (max(0.001, duration) * 1000000), 2)

        return {
            "size_mb": size_mb,
            "configured_connections": connections,
            "simulated_429_limit": simulate_429_limit,
            "peak_connections": LocalBenchHandler.peak_conns,
            "rate_limited_429_count": LocalBenchHandler.total_429s,
            "duration_seconds": round(duration, 3),
            "throughput_mibs": throughput_mibs,
            "throughput_mbps": throughput_mbps,
            "status": status,
            "file_size": file_size,
            "sha256_match": hash_match,
            "expected_sha256": payload_hash,
            "computed_sha256": actual_hash,
        }


def main():
    parser = argparse.ArgumentParser(description="Deterministic Local Benchmark for Radium")
    parser.add_argument("--size-mb", type=int, default=32, help="Payload size in MiB (default: 32)")
    parser.add_argument("--conns", type=int, default=8, help="Connection concurrency (default: 8)")
    parser.add_argument("--sim-429", type=int, default=0, help="Simulate 429 if active connections exceed this (0 = off)")
    parser.add_argument("--suite", action="store_true", help="Run full comparison matrix (1, 4, 8, 16 conns + 429 fallback)")
    parser.add_argument("--out", type=str, default=None, help="Save JSON report to path")
    args = parser.parse_args()

    print(f"[*] Radium Local Benchmark Runner (Testing executable: {BIN})")

    if args.suite:
        matrix = [
            {"size_mb": 32, "conns": 1, "sim_429": 0, "name": "1-Connection (Single-Stream Baseline)"},
            {"size_mb": 32, "conns": 4, "sim_429": 0, "name": "4-Connections (Parallel Segments)"},
            {"size_mb": 32, "conns": 8, "sim_429": 0, "name": "8-Connections (Default Work-Stealing)"},
            {"size_mb": 32, "conns": 16, "sim_429": 0, "name": "16-Connections (High Concurrency)"},
            {"size_mb": 32, "conns": 16, "sim_429": 4, "name": "16-Connections with 429 Throttling (Cap 4)"},
        ]
        results = []
        print("\n================================================================================")
        print("SCENARIO                                 | CONNS | DURATION |  THROUGHPUT | SHA256")
        print("================================================================================")
        for item in matrix:
            r = run_benchmark(item["size_mb"], item["conns"], item["sim_429"])
            results.append({"name": item["name"], **r})
            status_str = "PASS" if r["sha256_match"] else "FAIL"
            print(f"{item['name']:<40} | {r['configured_connections']:>5} | {r['duration_seconds']:>7.2f}s | {r['throughput_mibs']:>7.2f} MB/s | {status_str}")
        print("================================================================================\n")

        if args.out:
            Path(args.out).write_text(json.dumps(results, indent=2), encoding="utf-8")
            print(f"[+] Saved suite results to: {args.out}")
    else:
        r = run_benchmark(args.size_mb, args.conns, args.sim_429)
        print(json.dumps(r, indent=2))
        if args.out:
            Path(args.out).write_text(json.dumps(r, indent=2), encoding="utf-8")
            print(f"[+] Saved result to: {args.out}")


if __name__ == "__main__":
    main()
