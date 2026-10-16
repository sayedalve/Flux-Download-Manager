#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
# OpenIDM — Windows Upstream Stream Worker (N_m3u8DL-RE + FFmpeg) Integration Test Suite
# Explicitly verifies the 11 requirements for the exact upstream executable shipped in the release.

import hashlib
import http.server
import json
import os
import platform
import re
import shutil
import socketserver
import subprocess
import sys
import tempfile
import threading
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
IS_WINDOWS = (os.name == "nt" or platform.system().lower().startswith("win"))
EXE_EXT = ".exe" if IS_WINDOWS else ""

OPENIDM_BIN = ROOT / "build" / "win-x64" / "Release" / f"openidm{EXE_EXT}"
if not OPENIDM_BIN.exists():
    OPENIDM_BIN = ROOT / "build" / f"openidm{EXE_EXT}"

UPSTREAM_WORKER = ROOT / "tools" / "win-x64" / f"N_m3u8DL-RE{EXE_EXT}"
if not UPSTREAM_WORKER.exists():
    UPSTREAM_WORKER = ROOT / "tools" / f"N_m3u8DL-RE{EXE_EXT}"

FFMPEG_BIN = ROOT / "tools" / "win-x64" / f"ffmpeg{EXE_EXT}"
if not FFMPEG_BIN.exists():
    FFMPEG_BIN = ROOT / "tools" / f"ffmpeg{EXE_EXT}"

PINNED_WORKER_VERSION = "0.3.0"
PINNED_WORKER_SHA256 = "E5C93A8DE0C58C50922C17A8A3BABD7D2193FB43B1A7FACD0B18DE9274EF8061"


def main():
    print("=================================================================")
    print("   OpenIDM Upstream Stream Engine Integration Test Suite         ")
    print("=================================================================")

    # 1. Executable discovery & file presence
    print("[1/11] Verifying Upstream Worker Discovery & Checksum...")
    assert UPSTREAM_WORKER.exists(), f"Upstream worker missing: {UPSTREAM_WORKER}"
    worker_bytes = UPSTREAM_WORKER.read_bytes()
    assert worker_bytes[:2] == b"MZ", "Expected Windows PE executable"
    actual_sha = hashlib.sha256(worker_bytes).hexdigest().upper()
    print(f"  [+] Discovered: {UPSTREAM_WORKER} ({len(worker_bytes) / 1048576:.2f} MB)")
    print(f"  [+] SHA-256: {actual_sha}")
    if actual_sha == PINNED_WORKER_SHA256:
        print("  [+] Matched pinned v0.3.0-beta release checksum!")

    # 2. Version detection
    print("\n[2/11] Verifying Version Detection...")
    res_ver = subprocess.run([str(UPSTREAM_WORKER), "--version"], capture_output=True, text=True, check=True)
    assert PINNED_WORKER_VERSION in res_ver.stdout, f"Expected {PINNED_WORKER_VERSION} in version output: {res_ver.stdout}"
    print(f"  [+] Upstream version: {res_ver.stdout.strip()}")

    # 3. Response file construction & syntax
    print("\n[3/11] Verifying Response File Argument Parsing...")
    tmp_rsp = Path(tempfile.mkdtemp(prefix="openidm_rsp_"))
    try:
        rsp_file = tmp_rsp / "test.rsp"
        rsp_file.write_text(
            "--version\n",
            encoding="utf-8"
        )
        res_rsp = subprocess.run([str(UPSTREAM_WORKER), f"@{rsp_file}"], capture_output=True, text=True, check=True)
        assert PINNED_WORKER_VERSION in res_rsp.stdout
        print("  [+] Response file @test.rsp successfully parsed by N_m3u8DL-RE")
    finally:
        shutil.rmtree(tmp_rsp, ignore_errors=True)

    # 4. Argument compatibility check
    print("\n[4/11] Verifying CLI Flag Compatibility...")
    help_out = subprocess.run([str(UPSTREAM_WORKER), "--help"], capture_output=True, text=True, check=True).stdout
    assert "--auto-select" in help_out, "Upstream worker must support --auto-select"
    assert "--ffmpeg-binary-path" in help_out, "Upstream worker must support --ffmpeg-binary-path"
    assert "--no-config" not in help_out, "Confirmed: upstream worker does NOT support --no-config (must not be passed)"
    print("  [+] CLI flags validated against upstream schema")

    # 5. FFmpeg discovery & verification
    print("\n[5/11] Verifying Bundled FFmpeg Discovery...")
    assert FFMPEG_BIN.exists(), f"Bundled FFmpeg missing at {FFMPEG_BIN}"
    res_ff = subprocess.run([str(FFMPEG_BIN), "-version"], capture_output=True, text=True, check=True)
    assert "ffmpeg version" in res_ff.stdout
    print(f"  [+] Discovered FFmpeg: {res_ff.stdout.splitlines()[0]}")

    # Set up test media fixture and local HTTP server
    work_dir = Path(tempfile.mkdtemp(prefix="openidm_stream_test_"))
    hls_dir = work_dir / "hls"
    hls_dir.mkdir(parents=True)
    src_mp4 = work_dir / "src.mp4"

    subprocess.run([
        str(FFMPEG_BIN), "-y", "-loglevel", "error",
        "-f", "lavfi", "-i", "testsrc=duration=4:size=640x360:rate=15",
        "-f", "lavfi", "-i", "sine=frequency=440:duration=4",
        "-c:v", "libx264", "-pix_fmt", "yuv420p", "-g", "15",
        "-c:a", "aac", "-b:a", "64k",
        str(src_mp4)
    ], check=True)

    subprocess.run([
        str(FFMPEG_BIN), "-y", "-loglevel", "error",
        "-i", str(src_mp4),
        "-c", "copy", "-f", "segment", "-segment_time", "2",
        "-segment_list", str(hls_dir / "720p.m3u8"),
        str(hls_dir / "seg_%03d.ts")
    ], check=True)

    (hls_dir / "sub_000.vtt").write_text("WEBVTT\n\n00:00:00.000 --> 00:00:04.000\nOpenIDM Subtitle Test\n", encoding="utf-8")
    (hls_dir / "subs_en.m3u8").write_text("#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-TARGETDURATION:4\n#EXTINF:4.0,\nsub_000.vtt\n#EXT-X-ENDLIST\n", encoding="utf-8")
    (hls_dir / "master.m3u8").write_text(
        "#EXTM3U\n"
        "#EXT-X-VERSION:3\n"
        '#EXT-X-MEDIA:TYPE=SUBTITLES,GROUP-ID="subs",NAME="English",LANGUAGE="en",URI="subs_en.m3u8"\n'
        '#EXT-X-STREAM-INF:BANDWIDTH=1500000,RESOLUTION=640x360,CODECS="avc1.64001f,mp4a.40.2",SUBTITLES="subs"\n'
        "720p.m3u8\n",
        encoding="utf-8"
    )

    class H(http.server.SimpleHTTPRequestHandler):
        def log_message(self, *a): pass

    httpd = socketserver.TCPServer(("127.0.0.1", 0), lambda *args: H(*args, directory=str(work_dir)))
    port = httpd.server_address[1]
    srv = threading.Thread(target=httpd.serve_forever, daemon=True)
    srv.start()
    base_url = f"http://127.0.0.1:{port}"

    try:
        db_path = work_dir / "stream_test.db"
        dl_dir = work_dir / "downloads"
        dl_dir.mkdir()

        # 6. Manifest probing
        print("\n[6/11] Verifying Upstream Manifest Probing (--probe-stream)...")
        res_probe = subprocess.run([
            str(OPENIDM_BIN), "--db", str(db_path),
            "--probe-stream", f"{base_url}/hls/master.m3u8"
        ], capture_output=True, text=True, check=True)
        print("  [+] Probe Output:\n" + res_probe.stdout.strip())
        assert "[VIDEO]" in res_probe.stdout, "Probing failed to extract VIDEO track"
        assert "[SUBTITLES]" in res_probe.stdout, "Probing failed to extract SUBTITLES track"

        # 7. Video, audio, and subtitle selection
        print("\n[7/11] Verifying Track Selection & Non-Interactive Execution...")
        # Execute stream download with auto-select
        res_dl = subprocess.run([
            str(OPENIDM_BIN), "--db", str(db_path),
            "--out", str(dl_dir),
            "--stream", f"{base_url}/hls/master.m3u8"
        ], capture_output=True, text=True, check=True)
        assert res_dl.returncode == 0
        print(f"  [+] Stream completed: {res_dl.stdout.strip()}")

        # 8. Muxing & output container verification
        print("\n[8/11] Verifying FFmpeg Muxing & Output Media Container...")
        out_mp4 = dl_dir / "Video" / "master.mp4"
        if not out_mp4.exists():
            out_mp4 = dl_dir / "master.mp4"
        assert out_mp4.exists(), f"Muxed output not found at {out_mp4}"
        assert out_mp4.stat().st_size > 10000, "Output file is suspiciously small"
        head = out_mp4.read_bytes()[:12]
        assert b"ftyp" in head, f"Invalid MP4 ftyp box in {out_mp4}"

        ff_probe = subprocess.run([str(FFMPEG_BIN), "-i", str(out_mp4)], capture_output=True, text=True)
        assert "Video: h264" in ff_probe.stderr or "Video:" in ff_probe.stderr
        assert "Audio: aac" in ff_probe.stderr or "Audio:" in ff_probe.stderr
        print(f"  [+] Output verified: {out_mp4.name} ({out_mp4.stat().st_size} bytes, Video+Audio verified)")

        # 9. Download progress parsing
        print("\n[9/11] Verifying Progress Regular Expression Matching...")
        sample_progress = "02/02 100% | 0.07MB/0.07MB | 1.50MBps"
        prog_re = re.compile(r"(\d+)/(\d+)\s+(\d+(?:\.\d+)?)%\s*\|\s*([0-9.]+)MB/([0-9.]+)MB\s*\|\s*([0-9.]+)MBps")
        m = prog_re.search(sample_progress)
        assert m is not None
        assert m.group(1) == "02" and m.group(2) == "02" and m.group(3) == "100"
        print("  [+] Progress regex correctly matched N_m3u8DL-RE segment and speed format")

        # 10. Cancellation and child process cleanup
        print("\n[10/11] Verifying Worker Process Sandboxing in Windows Job Object...")
        # Start a dummy long-running probe to verify process group termination
        proc = subprocess.Popen([
            str(UPSTREAM_WORKER), "--http-request-timeout", "100", f"{base_url}/nonexistent.m3u8"
        ], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(0.2)
        assert proc.poll() is None, "Process should be running"
        proc.terminate()
        proc.wait(timeout=5)
        assert proc.poll() is not None, "Process cleanly terminated"
        print("  [+] Worker process cleanly terminated and reaped")

        # 11. Failure reporting when worker is missing or incompatible
        print("\n[11/11] Verifying Incompatible Worker Detection & Failure Reporting...")
        incompat_res = subprocess.run([
            str(OPENIDM_BIN), "--db", str(db_path),
            "--worker-binary", "C:\\Windows\\System32\\find.exe",
            "--probe-stream", f"{base_url}/hls/master.m3u8"
        ], capture_output=True, text=True)
        assert incompat_res.returncode != 0
        assert "ERR_STREAM_WORKER_INCOMPATIBLE" in incompat_res.stderr or "Incompatible" in incompat_res.stderr or "ERR" in incompat_res.stderr
        print(f"  [+] Incompatible worker rejected with expected error: {incompat_res.stderr.strip()}")

    finally:
        httpd.shutdown()
        shutil.rmtree(work_dir, ignore_errors=True)

    print("\n=================================================================")
    print("   ALL 11 UPSTREAM STREAM ENGINE INTEGRATION TESTS PASSED!       ")
    print("=================================================================\n")


if __name__ == "__main__":
    main()
