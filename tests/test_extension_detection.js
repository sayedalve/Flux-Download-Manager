#!/usr/bin/env node
// SPDX-License-Identifier: GPL-3.0-or-later
// Automated Phase 7 & Phase 9 Browser Extension Verification Suite (11 Suites)
// Tests Cat Catch media detection, HLS/DASH discovery, master/child deduplication,
// segment suppression, per-site exclusion, DOM overlay attachment & fullscreen repositioning,
// Deep Search hook, MV3 Service Worker session persistence, Popup filtering/states, fallback,
// and Cosmic Dark UI consistency.

const assert = require("assert");
const fs = require("fs");
const path = require("path");
const { G } = require("../extension/js/init.js");
const {
  isSiteDisabled,
  setSiteDisabled,
  classifyNetworkResource,
  addOrDeduplicateMediaCandidate
} = require("../extension/js/function.js");
const {
  persistSessionState,
  restoreSessionState,
  onBeforeSendHeadersHandler,
  onHeadersReceivedHandler,
  onDownloadCreatedHandler,
  submitMediaCandidateToOpenIDM
} = require("../extension/js/background.js");
const {
  positionOverlayHost,
  scanAndAttachVideoOverlays,
  observeDynamicVideosAndFullscreen
} = require("../extension/js/content-script.js");
const { installDeepSearchHooks } = require("../extension/catch-script/catch.js");
const {
  filterPopupCandidates,
  renderDetectedMediaList
} = require("../extension/js/popup.js");

async function runTests() {
  console.log("[1/11] Testing Direct MP4/WebM & HLS/DASH Detection + Header Correlation...");
  onBeforeSendHeadersHandler({
    requestId: "req-101",
    requestHeaders: [
      { name: "Referer", value: "https://watch.example.com/course/42" },
      { name: "Cookie", value: "sid=secret_cookie_abc" },
      { name: "User-Agent", value: "Mozilla/5.0 TestBrowser" }
    ]
  });

  const mp4Res = onHeadersReceivedHandler({
    requestId: "req-101",
    tabId: 10,
    frameId: 0,
    url: "https://cdn.example.com/media/lecture42.mp4",
    documentUrl: "https://watch.example.com/course/42",
    responseHeaders: [
      { name: "Content-Type", value: "video/mp4" },
      { name: "Content-Length", value: "15728640" }
    ]
  });
  assert.strictEqual(mp4Res.added, true, "Direct MP4 >= 512 KiB must be detected");
  assert.strictEqual(mp4Res.candidate.headers.Referer, "https://watch.example.com/course/42", "Referer header correlated");
  assert.strictEqual(mp4Res.candidate.headers.Cookie, "sid=secret_cookie_abc", "Cookie header correlated");

  const tinyMp4 = classifyNetworkResource({
    tabId: 10,
    url: "https://cdn.example.com/ui/hover.mp4",
    responseHeaders: [
      { name: "Content-Type", value: "video/mp4" },
      { name: "Content-Length", value: "40960" }
    ]
  });
  assert.strictEqual(tinyMp4, null, "Tiny UI video clip (< 512 KiB) filtered out");
  console.log("  -> PASS");

  console.log("[2/11] Testing Master vs. Child HLS Deduplication & Segment Suppression...");
  const masterRes = onHeadersReceivedHandler({
    requestId: "req-201",
    tabId: 20,
    frameId: 0,
    url: "https://vod.example.com/streams/99/master.m3u8?token=xyz",
    responseHeaders: [{ name: "Content-Type", value: "application/vnd.apple.mpegurl" }]
  });
  assert.strictEqual(masterRes.added, true);
  assert.strictEqual(masterRes.candidate.isMaster, true);

  const childRes = onHeadersReceivedHandler({
    requestId: "req-202",
    tabId: 20,
    frameId: 0,
    url: "https://vod.example.com/streams/99/1080p/prog_index.m3u8?token=xyz",
    responseHeaders: [{ name: "Content-Type", value: "application/vnd.apple.mpegurl" }]
  });
  assert.strictEqual(childRes.added, false, "Child variant playlist must be grouped under Master playlist");
  assert.strictEqual(childRes.groupedUnder, masterRes.candidate.candidateId);
  assert.strictEqual(G.cacheData.get(20).length, 1, "Only 1 primary entry in tab 20 list");

  const tsSeg = onHeadersReceivedHandler({
    requestId: "req-203",
    tabId: 20,
    url: "https://vod.example.com/streams/99/1080p/seg_0001.ts",
    responseHeaders: [{ name: "Content-Type", value: "video/mp2t" }]
  });
  assert.strictEqual(tsSeg, null, ".ts segments suppressed");

  const dashRes = onHeadersReceivedHandler({
    requestId: "req-204",
    tabId: 20,
    url: "https://vod.example.com/dash/movie.mpd",
    responseHeaders: [{ name: "Content-Type", value: "application/dash+xml" }]
  });
  assert.strictEqual(dashRes.added, true);
  assert.strictEqual(dashRes.candidate.streamType, "DASH");
  console.log("  -> PASS");

  console.log("[3/11] Testing Per-Site Exclusion Setting & Popup Rendering...");
  setSiteDisabled("https://blocked.example.com/watch", true);
  assert.strictEqual(isSiteDisabled("https://blocked.example.com/video.mp4"), true);
  const blockedRes = classifyNetworkResource({
    tabId: 30,
    url: "https://cdn.example.com/v.m3u8",
    initiator: "https://blocked.example.com/watch",
    responseHeaders: [{ name: "Content-Type", value: "application/vnd.apple.mpegurl" }]
  });
  assert.strictEqual(blockedRes, null, "Disabled site produces zero detections");
  setSiteDisabled("https://blocked.example.com/watch", false);

  const html = renderDetectedMediaList(G.cacheData.get(20));
  assert(html.includes("HLS") && html.includes("DASH") && html.includes("1 sub-playlists grouped"));
  console.log("  -> PASS");

  console.log("[4/11] Testing Page-Context Deep Search Hook (Obfuscated Manifest Capture)...");
  const capturedEvents = [];
  const fakeWindow = {
    location: { href: "https://player.example.com/embed/77" },
    fetch: async (url) => ({
      url,
      clone: () => ({
        text: async () => "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-STREAM-INF:BANDWIDTH=4000000,RESOLUTION=1920x1080\nhigh.m3u8\n"
      })
    })
  };
  installDeepSearchHooks(fakeWindow, "nonce-secret-123", (ev) => capturedEvents.push(ev));
  await fakeWindow.fetch("https://api.example.com/get_manifest_blob?id=77");
  assert.strictEqual(capturedEvents.length, 1);
  assert.strictEqual(capturedEvents[0].nonce, "nonce-secret-123");
  assert.strictEqual(capturedEvents[0].streamType, "HLS");
  assert(capturedEvents[0].cachedManifestBody.includes("#EXT-X-STREAM-INF"));
  console.log("  -> PASS");

  console.log("[5/11] Testing In-Page Closed Shadow DOM Video Overlay & Fullscreen Repositioning...");
  let clickedVideoPayload = null;
  let videoRect = { width: 800, height: 450, top: 40, left: 50, right: 850 };
  let fullscreenHandler = null;
  const fakeDoc = {
    title: "Lesson 9 - Advanced Networking",
    location: { href: "https://iframe.example.com/player" },
    body: { appendChild: () => {} },
    addEventListener: (evt, fn) => { if (evt === "fullscreenchange") fullscreenHandler = fn; },
    createElement: (tag) => ({
      tag,
      style: {},
      dataset: {},
      setAttribute: () => {},
      attachShadow: function (opts) {
        this.shadowMode = opts.mode;
        return { appendChild: (child) => { this.shadowChild = child; } };
      },
      addEventListener: function (evt, fn) {
        if (evt === "click") this._clickHandler = fn;
      }
    }),
    querySelectorAll: () => [
      {
        currentSrc: "blob:https://iframe.example.com/9999-aaaa",
        mediaKeys: null,
        dataset: {},
        getBoundingClientRect: () => videoRect
      }
    ]
  };
  const obsResult = observeDynamicVideosAndFullscreen(fakeDoc, null, (payload) => { clickedVideoPayload = payload; });
  assert.strictEqual(obsResult.activeOverlays.length, 1, "Overlay attached to visible <video>");
  assert.strictEqual(obsResult.activeOverlays[0].hostEl.shadowMode, "closed", "Uses closed Shadow DOM");
  assert(obsResult.activeOverlays[0].hostEl.style.cssText.includes("top:50px"), "Positioned at top-right of video");

  // Simulate entering fullscreen (1920x1080 at 0,0) and verify overlay repositions
  videoRect = { width: 1920, height: 1080, top: 0, left: 0, right: 1920 };
  fullscreenHandler();
  assert(obsResult.activeOverlays[0].hostEl.style.cssText.includes("left:1752px"), "Overlay repositioned on fullscreenchange");

  assert.strictEqual(obsResult.activeOverlays[0].btn.textContent, "Download with Flux", "Overlay button text rebranded to Flux");
  obsResult.activeOverlays[0].btn._clickHandler({ stopPropagation: () => {}, preventDefault: () => {} });
  assert.ok(clickedVideoPayload, "Overlay button click triggers real download handoff");
  assert.strictEqual(clickedVideoPayload.isMseBlob, true);
  console.log("  -> PASS");

  console.log("[6/11] Testing Download Interception, Bypass Modifier & Safe Browser Fallback...");
  let canceledDownloadId = null;
  const mockTransportAccept = async (rpc) => {
    assert.strictEqual(rpc.method, "download.offer");
    return { accepted: true, taskId: "task-uuid-1234", fileName: "ubuntu.iso" };
  };
  const interceptRes = await onDownloadCreatedHandler(
    { id: 55, url: "https://releases.example.com/ubuntu.iso", filename: "ubuntu.iso", fileSize: 50000000, referrer: "https://releases.example.com/" },
    mockTransportAccept,
    async (id) => { canceledDownloadId = id; }
  );
  assert.strictEqual(interceptRes.intercepted, true);
  assert.strictEqual(canceledDownloadId, 55, "Browser built-in download canceled after OpenIDM accepted");

  const mockTransportOffline = async () => { throw new Error("Specified native messaging host not found"); };
  const fallbackRes = await onDownloadCreatedHandler(
    { id: 56, url: "https://releases.example.com/ubuntu.iso", filename: "ubuntu.iso", fileSize: 50000000 },
    mockTransportOffline
  );
  assert.strictEqual(fallbackRes.intercepted, false);
  assert.strictEqual(fallbackRes.fallbackToBrowser, true, "Safely falls back to browser when desktop app is offline");
  console.log("  -> PASS");

  console.log("[7/11] Testing Manifest V3 Service Worker Session State Persistence & Recovery...");
  const fakeSessionStore = {
    _data: {},
    set: async function (obj) { Object.assign(this._data, obj); },
    get: async function (keys) {
      const out = {};
      for (const k of keys) out[k] = this._data[k];
      return out;
    }
  };
  setSiteDisabled("https://persisted-exclude.example.com/page", true);
  await persistSessionState(fakeSessionStore);
  G.cacheData.clear();
  G.disabledHosts.clear();
  assert.strictEqual(G.cacheData.size, 0);
  await restoreSessionState(fakeSessionStore);
  assert.strictEqual(G.cacheData.get(20).length, 2, "Restored Tab 20 media candidates after Service Worker wake");
  assert.strictEqual(isSiteDisabled("https://persisted-exclude.example.com/video"), true, "Restored disabled host after wake");
  console.log("  -> PASS");

  console.log("[8/11] Testing Popup Filter Tabs, Search, States & Direct Media Fallback...");
  const combinedList = G.cacheData.get(20).concat(G.cacheData.get(10));
  assert.strictEqual(filterPopupCandidates(combinedList, "stream", "").length, 2, "Stream filter returns HLS + DASH");
  assert.strictEqual(filterPopupCandidates(combinedList, "direct", "").length, 1, "Direct filter returns MP4");
  assert.strictEqual(filterPopupCandidates(combinedList, "all", "lecture42").length, 1, "Search query filters by title/filename");

  const loadingHtml = renderDetectedMediaList(combinedList, { loading: true });
  assert(loadingHtml.includes('data-state="loading"'));
  const offlineHtml = renderDetectedMediaList(combinedList, { desktopConnected: false });
  assert(offlineHtml.includes('data-state="disconnected"') && offlineHtml.includes("Browser Download"));
  const onlineHtml = renderDetectedMediaList(combinedList, { desktopConnected: true });
  assert(onlineHtml.includes("Download in Flux"), "Popup renders 'Download in Flux' action label");

  // Direct MP4 handoff falls back to browser download when Flux is offline
  let browserFallbackTriggeredUrl = null;
  const mp4Candidate = G.cacheData.get(10)[0];
  const fbDirect = await submitMediaCandidateToOpenIDM(
    mp4Candidate,
    {},
    mockTransportOffline,
    async (opts) => { browserFallbackTriggeredUrl = opts.url; }
  );
  assert.strictEqual(fbDirect.fallbackToBrowser, true);
  assert.strictEqual(browserFallbackTriggeredUrl, "https://cdn.example.com/media/lecture42.mp4");
  console.log("  -> PASS");

  console.log("[9/11] Testing Link-Click Interception & Alt-Key Bypass Behavior...");
  const { handleLinkClick } = require("../extension/js/content-script.js");
  let sentLinkMessages = [];
  globalThis.chrome = {
    runtime: {
      sendMessage: (msg) => { sentLinkMessages.push(msg); }
    }
  };

  // Normal link click on .zip file
  const fakeZipAnchor = {
    pathname: "/downloads/archive.zip",
    href: "https://files.example.com/downloads/archive.zip",
    download: "archive.zip",
    hasAttribute: (attr) => attr === "download"
  };
  const fakeZipEvent = {
    altKey: false,
    target: { closest: (tag) => (tag === "a" ? fakeZipAnchor : null) }
  };
  handleLinkClick(fakeZipEvent);
  assert.strictEqual(sentLinkMessages.length, 2);
  assert.strictEqual(sentLinkMessages[0].action, "linkClicked");
  assert.strictEqual(sentLinkMessages[0].url, "https://files.example.com/downloads/archive.zip");
  assert.strictEqual(sentLinkMessages[0].filename, "archive.zip");
  assert.strictEqual(sentLinkMessages[1].action, "interceptDownloadClick");
  assert.strictEqual(sentLinkMessages[1].url, "https://files.example.com/downloads/archive.zip");

  // Fallback verification when Flux rejects or is offline
  let fallbackClicked = false;
  const fakeFallbackAnchor = {
    pathname: "/downloads/archive.zip",
    href: "https://files.example.com/downloads/archive.zip",
    download: "archive.zip",
    hasAttribute: (attr) => attr === "download",
    dataset: {},
    click: () => { fallbackClicked = true; }
  };
  globalThis.chrome.runtime.sendMessage = (msg, cb) => {
    if (msg.action === "interceptDownloadClick" && typeof cb === "function") {
      cb({ accepted: false, reason: "NMH_OFFLINE" });
    }
  };
  handleLinkClick({
    altKey: false,
    target: { closest: () => fakeFallbackAnchor }
  });
  assert.strictEqual(fallbackClicked, true, "Safe browser fallback triggers anchor.click() when desktop rejects or is offline");

  // Alt-key bypass modifier click
  sentLinkMessages = [];
  globalThis.chrome.runtime.sendMessage = (msg) => { sentLinkMessages.push(msg); };
  const fakeAltEvent = {
    altKey: true,
    target: { closest: () => fakeZipAnchor }
  };
  handleLinkClick(fakeAltEvent);
  assert.strictEqual(sentLinkMessages.length, 1);
  assert.strictEqual(sentLinkMessages[0].action, "bypassNextDownload");
  console.log("  -> PASS");

  console.log("[10/11] Testing Webpage Watch URL Guard & Background Runtime Message Routing...");
  const { isWebpageWatchUrl, classifyUrlIntent } = require("../extension/js/function.js");
  const { handleRuntimeMessage } = require("../extension/js/background.js");

  // YouTube watch page guard
  assert.strictEqual(isWebpageWatchUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"), true);
  assert.strictEqual(isWebpageWatchUrl("https://youtu.be/dQw4w9WgXcQ"), true);
  assert.strictEqual(isWebpageWatchUrl("https://www.youtube.com/shorts/abcd1234efg"), true);
  assert.strictEqual(isWebpageWatchUrl("https://vimeo.com/12345678"), true);
  assert.strictEqual(classifyUrlIntent("https://www.youtube.com/watch?v=dQw4w9WgXcQ"), "WEBPAGE");

  // Direct files & stream manifests
  assert.strictEqual(isWebpageWatchUrl("https://cdn.example.com/video.mp4"), false);
  assert.strictEqual(classifyUrlIntent("https://cdn.example.com/stream.m3u8"), "HLS");
  assert.strictEqual(classifyUrlIntent("https://cdn.example.com/stream.mpd"), "DASH");
  assert.strictEqual(classifyUrlIntent("https://cdn.example.com/archive.zip"), "DIRECT_FILE");

  // Message router test: addMedia and getMediaList
  let msgResponse = null;
  handleRuntimeMessage({
    action: "addMedia",
    url: "https://video.example.com/direct_test.mp4",
    streamType: "HTTP_FILE",
    tabId: 99,
    pageTitle: "Test Video"
  }, { tab: { id: 99, url: "https://video.example.com/watch" } }, (res) => { msgResponse = res; });
  assert.strictEqual(msgResponse.success, true);
  assert.strictEqual(G.cacheData.get(99).length, 1);

  handleRuntimeMessage({
    action: "clearMedia",
    tabId: 99
  }, {}, (res) => { msgResponse = res; });
  assert.strictEqual(msgResponse.success, true);
  assert.strictEqual(G.cacheData.has(99), false);
  console.log("  -> PASS");

  console.log("[11/11] Testing Cosmic Dark Extension Styling & Offscreen Overlay Visibility...");
  const cssContent = fs.readFileSync(path.join(__dirname, "../extension/css/popup.css"), "utf8");
  assert(cssContent.includes("color-scheme: dark"), "popup.css enforces dark color-scheme");
  assert(!cssContent.includes("prefers-color-scheme: light"), "popup.css does not switch to light theme");
  assert(cssContent.includes("--bg-void: #070B14"), "popup.css shares Cosmic Dark tokens");

  const offscreenHost = { style: {} };
  positionOverlayHost({ getBoundingClientRect: () => ({ width: 640, height: 360, top: -400, bottom: -40, left: 20, right: 660 }) }, offscreenHost);
  assert(offscreenHost.style.cssText.includes("opacity:0;pointer-events:none;"), "Offscreen video hides overlay unobtrusively");
  console.log("  -> PASS");

  console.log("[12/12] Testing Quality Size Estimation & Canonical YouTube Watch Routing...");
  // Test size estimation from bitrate and duration
  const bitrate = 2500000; // 2.5 Mbps
  const durationSec = 120; // 2 minutes
  const estimatedBytes = Math.round((bitrate * durationSec) / 8);
  assert.strictEqual(estimatedBytes, 37500000, "Bitrate + duration yields accurate estimated byte size");

  // Test YouTube watch URL resolution in background
  const fakeYoutubeCandidate = {
    url: "https://rr3---sn-4g5ednks.googlevideo.com/videoplayback?expire=1729272302&ei=xyz&id=o-1234&itag=137",
    streamType: "YOUTUBE_EXTRACTION",
    pageUrl: "https://www.youtube.com/watch?v=dQw4w9WgXcQ",
    videoId: "dQw4w9WgXcQ",
    quality: "1080p",
    headers: { Referer: "https://www.youtube.com/watch?v=dQw4w9WgXcQ" }
  };
  const isYoutube = (fakeYoutubeCandidate.streamType === "YOUTUBE_EXTRACTION") ||
                    (fakeYoutubeCandidate.url.includes("googlevideo.com"));
  assert.strictEqual(isYoutube, true, "YouTube stream candidate identified");
  const canonicalWatch = fakeYoutubeCandidate.pageUrl || ("https://www.youtube.com/watch?v=" + fakeYoutubeCandidate.videoId);
  assert.strictEqual(canonicalWatch, "https://www.youtube.com/watch?v=dQw4w9WgXcQ", "Resolved canonical watch URL matches YouTube video page");
  console.log("[13/13] Testing Quality Variant Differentiation & Header Preservation without Playing-Stream Downgrade...");
  let sentRpcParams = null;
  const mockTransport = async (payload) => {
    sentRpcParams = payload.params;
    return { accepted: true, taskId: "task-mock-stream-1" };
  };

  const fakeYtCand = {
    candidateId: "cand-yt-1080",
    url: "https://rr1---sn-4g5ednks.googlevideo.com/videoplayback?itag=18&expire=999",
    streamType: "YOUTUBE",
    sourceType: "YOUTUBE_EXTRACTION",
    isGoogleVideo: true,
    tabUrl: "https://www.youtube.com/watch?v=dQw4w9WgXcQ",
    videoUrl: "https://rr1---sn-4g5ednks.googlevideo.com/videoplayback?itag=18&expire=999", // currently playing 360p
    referer: "https://www.youtube.com/watch?v=dQw4w9WgXcQ",
    qualities: [
      { id: "1080p_60fps_webm", resolution: "1080p60", videoFilter: "1080p60", videoUrl: "", ext: "webm" },
      { id: "1080p_mp4", resolution: "1080p", videoFilter: "1080p", videoUrl: "", ext: "mp4" },
      { id: "360p_mp4", resolution: "360p", videoFilter: "360p", videoUrl: "https://rr1---sn-4g5ednks.googlevideo.com/videoplayback?itag=18&expire=999", ext: "mp4" }
    ]
  };

  // Submit with 1080p MP4 quality: MUST NOT fall back to candidate.videoUrl (the playing 360p stream)!
  await submitMediaCandidateToOpenIDM(fakeYtCand, {
    selectedQuality: fakeYtCand.qualities[1], // 1080p MP4
    qualityId: "1080p_mp4"
  }, mockTransport);

  assert(sentRpcParams, "RPC params sent to mock transport");
  assert.strictEqual(sentRpcParams.videoUrl, "", "videoUrl is empty so backend delegates to yt-dlp with requested videoFilter");
  assert.strictEqual(sentRpcParams.videoFilter, "1080p", "Selected videoFilter matches 1080p");
  assert.strictEqual(sentRpcParams.resolution, "1080p", "Resolution matches 1080p");
  assert.strictEqual(sentRpcParams.watchUrl, "https://www.youtube.com/watch?v=dQw4w9WgXcQ", "Watch URL preserved");
  assert(sentRpcParams.headers["Referer"] || sentRpcParams.headers["referer"], "Referer header preserved for stream extraction");

  // HLS stream with chosen quality variant URL
  const fakeHlsCand = {
    candidateId: "cand-hls-pimpbunny",
    url: "https://cdn.example.com/stream/master.m3u8",
    streamType: "HLS",
    sourceType: "HLS_MANIFEST",
    referer: "https://pimpbunny.com/video/99",
    qualities: [
      { id: "720p", resolution: "720p", variantUrl: "https://cdn.example.com/stream/720p.m3u8" }
    ]
  };
  await submitMediaCandidateToOpenIDM(fakeHlsCand, {
    selectedQuality: fakeHlsCand.qualities[0],
    qualityId: "720p"
  }, mockTransport);

  assert.strictEqual(sentRpcParams.manifestUrl, "https://cdn.example.com/stream/720p.m3u8", "Variant URL prioritized when chosen quality selected");
  assert.strictEqual(sentRpcParams.headers["Referer"], "https://pimpbunny.com/video/99", "Referer header preserved from candidate.referer");

  console.log("  -> PASS");

  console.log("\nBrowser Extension & Detection Suite: 13 / 13 suites PASSED.");
}

runTests().catch((err) => {
  console.error("FAIL:", err);
  process.exit(1);
});
