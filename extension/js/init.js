// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/init.js) for Flux Browser Integration.
// Preserves Cat Catch's G configuration model, Ext/Type/Regex rule tables, and header cache.

const G = {
  version: "0.3.0",
  nativeHostName: "com.flux.downloadmanager",
  fallbackNativeHostNames: ["org.flux.native", "org.openidm.native"],
  enableCapture: true,
  enableVideoOverlay: true,
  minFileSizeBytes: 1048576, // 1 MiB minimum for automatic ordinary file interception
  disabledHosts: new Set(),
  bypassNextDownload: false,

  // Adapted from Cat Catch default media extension rules (G.Ext)
  Ext: [
    { ext: "m3u8", size: 0, state: true, streamType: "HLS" },
    { ext: "m3u", size: 0, state: true, streamType: "HLS" },
    { ext: "mpd", size: 0, state: true, streamType: "DASH" },
    { ext: "ism", size: 0, state: true, streamType: "MSS" },
    { ext: "mp4", size: 524288, state: true, streamType: "HTTP_FILE" },
    { ext: "webm", size: 524288, state: true, streamType: "HTTP_FILE" },
    { ext: "mkv", size: 524288, state: true, streamType: "HTTP_FILE" },
    { ext: "flv", size: 524288, state: true, streamType: "HTTP_FILE" },
    { ext: "mov", size: 524288, state: true, streamType: "HTTP_FILE" },
    { ext: "mp3", size: 131072, state: true, streamType: "HTTP_FILE" },
    { ext: "m4a", size: 131072, state: true, streamType: "HTTP_FILE" },
    { ext: "flac", size: 131072, state: true, streamType: "HTTP_FILE" },
    { ext: "vtt", size: 0, state: true, streamType: "SUBTITLE" },
    { ext: "srt", size: 0, state: true, streamType: "SUBTITLE" }
  ],

  // Adapted from Cat Catch MIME type rules (G.Type)
  Type: [
    { type: "application/vnd.apple.mpegurl", size: 0, state: true, ext: "m3u8", streamType: "HLS" },
    { type: "application/x-mpegurl", size: 0, state: true, ext: "m3u8", streamType: "HLS" },
    { type: "audio/mpegurl", size: 0, state: true, ext: "m3u8", streamType: "HLS" },
    { type: "audio/x-mpegurl", size: 0, state: true, ext: "m3u8", streamType: "HLS" },
    { type: "application/dash+xml", size: 0, state: true, ext: "mpd", streamType: "DASH" },
    { type: "application/vnd.ms-sstr+xml", size: 0, state: true, ext: "ism", streamType: "MSS" },
    { type: "video/mp4", size: 524288, state: true, ext: "mp4", streamType: "HTTP_FILE" },
    { type: "video/webm", size: 524288, state: true, ext: "webm", streamType: "HTTP_FILE" },
    { type: "video/x-matroska", size: 524288, state: true, ext: "mkv", streamType: "HTTP_FILE" },
    { type: "audio/mpeg", size: 131072, state: true, ext: "mp3", streamType: "HTTP_FILE" },
    { type: "audio/mp4", size: 131072, state: true, ext: "m4a", streamType: "HTTP_FILE" }
  ],

  // Adapted from Cat Catch URL Regex rules (G.Regex)
  Regex: [
    { type: "ig", regex: "https?://[^\\s\"']+\\.m3u8(\\?[^\\s\"']*)?$", ext: "m3u8", streamType: "HLS", state: true },
    { type: "ig", regex: "https?://[^\\s\"']+\\.mpd(\\?[^\\s\"']*)?$", ext: "mpd", streamType: "DASH", state: true }
  ],

  // Ordinary file extensions intercepted by DownloadInterceptor (Brisk Browser Extension style)
  InterceptFileExts: new Set([
    "zip", "rar", "7z", "tar", "gz", "bz2", "xz", "iso", "exe", "msi", "dmg", "pdf", "mp4", "mkv", "webm", "mp3", "flac"
  ]),

  // Segment extensions that must be suppressed when an HLS/DASH manifest is active on the tab
  SegmentExts: new Set(["ts", "m4s", "cmfv", "cmfa", "m4f"]),

  // Short-lived requestId -> { Referer, Origin, Cookie, User-Agent } map
  requestHeadersMap: new Map(),

  // Per-tab detected media candidates: tabId -> Array<MediaCandidate>
  cacheData: new Map(),

  // Captured AES-128 keys from catch-script/catch.js: tabId -> Set<string>
  capturedKeys: new Map()
};

if (typeof module !== "undefined" && module.exports) {
  module.exports = { G };
}
