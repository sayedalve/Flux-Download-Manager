#!/usr/bin/env python3
import hashlib
import http.server
import json
import os
import shutil
import socketserver
import struct
import subprocess
import tempfile
import threading
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DIST_FLUX = ROOT / "dist" / "Flux.exe"
DIST_NMH = ROOT / "dist" / "flux-nmh.exe"

assert DIST_FLUX.is_file(), f"Missing {DIST_FLUX}"
assert DIST_NMH.is_file(), f"Missing {DIST_NMH}"

os.environ["FLUX_ALLOW_LOOPBACK"] = "1"
os.environ["OPENIDM_ALLOW_LOOPBACK"] = "1"

PAYLOAD_2MB = os.urandom(2 * 1024 * 1024)
SHA256_2MB = hashlib.sha256(PAYLOAD_2MB).hexdigest()

class MockMediaHandler(http.server.BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        pass

    def do_GET(self):
        path = self.path.split("?")[0]
        if path == "/files/ordinary_2mb.bin":
            self.send_response(200)
            self.send_header("Content-Type", "application/octet-stream")
            self.send_header("Content-Length", str(len(PAYLOAD_2MB)))
            self.send_header("Accept-Ranges", "bytes")
            self.send_header("Content-Disposition", 'attachment; filename="ordinary_2mb.bin"')
            self.end_headers()
            self.wfile.write(PAYLOAD_2MB)
        elif path == "/hls/master.m3u8":
            manifest = (
                "#EXTM3U\n"
                "#EXT-X-VERSION:3\n"
                "#EXT-X-STREAM-INF:BANDWIDTH=800000,RESOLUTION=640x360\n"
                "media.m3u8\n"
            ).encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "application/vnd.apple.mpegurl")
            self.send_header("Content-Length", str(len(manifest)))
            self.end_headers()
            self.wfile.write(manifest)
        elif path == "/hls/media.m3u8":
            playlist = (
                "#EXTM3U\n"
                "#EXT-X-TARGETDURATION:4\n"
                "#EXTINF:4.0,\n"
                "seg0.ts\n"
                "#EXT-X-ENDLIST\n"
            ).encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "application/vnd.apple.mpegurl")
            self.send_header("Content-Length", str(len(playlist)))
            self.end_headers()
            self.wfile.write(playlist)
        elif path == "/hls/seg0.ts":
            ts_data = b"\x47" + (b"\x00" * 187)
            self.send_response(200)
            self.send_header("Content-Type", "video/mp2t")
            self.send_header("Content-Length", str(len(ts_data)))
            self.end_headers()
            self.wfile.write(ts_data)
        elif path == "/dash/manifest.mpd":
            mpd = (
                '<?xml version="1.0" encoding="UTF-8"?>\n'
                '<MPD xmlns="urn:mpeg:dash:schema:mpd:2011" minBufferTime="PT1.5S" type="static" mediaPresentationDuration="PT4S">\n'
                '  <Period>\n'
                '    <AdaptationSet mimeType="video/mp4" contentType="video">\n'
                '      <Representation id="v1" bandwidth="1000000" width="640" height="360">\n'
                '        <SegmentTemplate media="dash_seg_$Number$.m4s" initialization="dash_init.mp4" startNumber="1" duration="4" timescale="1"/>\n'
                '      </Representation>\n'
                '    </AdaptationSet>\n'
                '  </Period>\n'
                '</MPD>\n'
            ).encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "application/dash+xml")
            self.send_header("Content-Length", str(len(mpd)))
            self.end_headers()
            self.wfile.write(mpd)
        elif path in ("/dash/dash_init.mp4", "/dash/dash_seg_1.m4s"):
            dummy_m4s = b"\x00\x00\x00\x20ftypiso5\x00\x00\x02\x00iso5dash\x00\x00\x00\x08free"
            self.send_response(200)
            self.send_header("Content-Type", "video/mp4")
            self.send_header("Content-Length", str(len(dummy_m4s)))
            self.end_headers()
            self.wfile.write(dummy_m4s)
        else:
            self.send_error(404)

def main():
    print("Testing dist/Flux.exe Portable Executable...")
    httpd = socketserver.TCPServer(("127.0.0.1", 16890), MockMediaHandler)
    t = threading.Thread(target=httpd.serve_forever, daemon=True)
    t.start()
    base_url = "http://127.0.0.1:16890"

    tmp_dir = Path(tempfile.mkdtemp(prefix="flux_dist_test_"))
    try:
        # 1. Ordinary File Download
        print("  [1/3] Testing Ordinary File Download via dist/Flux.exe...")
        dl_dir = tmp_dir / "downloads"
        dl_dir.mkdir(parents=True, exist_ok=True)
        db_file = tmp_dir / "flux_test.db"
        res = subprocess.run(
            [str(DIST_FLUX), "--db", str(db_file), "--out", str(dl_dir), "--download", f"{base_url}/files/ordinary_2mb.bin"],
            capture_output=True,
            text=True,
            check=True
        )
        downloaded = dl_dir / "ordinary_2mb.bin"
        if not downloaded.exists():
            downloaded = dl_dir / "General" / "ordinary_2mb.bin"
        assert downloaded.exists(), f"Downloaded file missing: {downloaded}"
        assert hashlib.sha256(downloaded.read_bytes()).hexdigest() == SHA256_2MB
        print("    -> PASS (Ordinary 2 MiB file downloaded and verified via SHA-256)")

        # 2. HLS Stream Probe & Download
        print("  [2/3] Testing HLS Manifest Probe via dist/Flux.exe...")
        probe = subprocess.run(
            [str(DIST_FLUX), "--db", str(db_file), "--probe-stream", f"{base_url}/hls/master.m3u8"],
            capture_output=True,
            text=True,
            check=True
        )
        assert "[VIDEO]" in probe.stdout, "HLS probe failed to detect video track"
        print("    -> PASS (HLS Video track detected)")

        # 3. DASH Stream Probe
        print("  [3/3] Testing DASH Manifest Probe via dist/Flux.exe...")
        dash_probe = subprocess.run(
            [str(DIST_FLUX), "--db", str(db_file), "--probe-stream", f"{base_url}/dash/manifest.mpd"],
            capture_output=True,
            text=True,
            check=True
        )
        assert "[VIDEO]" in dash_probe.stdout, "DASH probe failed to detect video track"
        print("    -> PASS (DASH Video track detected)")

        print("dist/Flux.exe Portable Verification SUCCEEDED!")
    finally:
        httpd.shutdown()
        shutil.rmtree(tmp_dir, ignore_errors=True)

if __name__ == "__main__":
    main()
