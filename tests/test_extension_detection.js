#!/usr/bin/env node
// SPDX-License-Identifier: GPL-3.0-or-later
// Automated Phase 7 & Phase 9 Browser Extension Verification Suite (8 Suites)
// Tests Cat Catch media detection, HLS/DASH discovery, master/child deduplication,
// segment suppression, per-site exclusion, DOM overlay attachment & fullscreen repositioning,
// Deep Search hook, MV3 Service Worker session persistence, Popup filtering/states, and fallback.

const assert = require("assert");
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
  scanAndAttachVideoOverlays,
  observeDynamicVideosAndFullscreen
} = require("../extension/js/content-script.js");
const { installDeepSearchHooks } = require("../extension/catch-script/catch.js");
const {
  filterPopupCandidates,
  renderDetectedMediaList
} = require("../extension/js/popup.js");

async function runTests() {
  console.log("[1/8] Testing Direct MP4/WebM & HLS/DASH Detection + Header Correlation...");
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

  console.log("[2/8] Testing Master vs. Child HLS Deduplication & Segment Suppression...");
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

  console.log("[3/8] Testing Per-Site Exclusion Setting & Popup Rendering...");
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

  console.log("[4/8] Testing Page-Context Deep Search Hook (Obfuscated Manifest Capture)...");
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

  console.log("[5/8] Testing In-Page Closed Shadow DOM Video Overlay & Fullscreen Repositioning...");
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

  obsResult.activeOverlays[0].btn._clickHandler({ stopPropagation: () => {}, preventDefault: () => {} });
  assert.ok(clickedVideoPayload, "Overlay button click triggers real download handoff");
  assert.strictEqual(clickedVideoPayload.isMseBlob, true);
  console.log("  -> PASS");

  console.log("[6/8] Testing Download Interception, Bypass Modifier & Safe Browser Fallback...");
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

  console.log("[7/8] Testing Manifest V3 Service Worker Session State Persistence & Recovery...");
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

  console.log("[8/8] Testing Popup Filter Tabs, Search, States & Direct Media Fallback...");
  const combinedList = G.cacheData.get(20).concat(G.cacheData.get(10));
  assert.strictEqual(filterPopupCandidates(combinedList, "stream", "").length, 2, "Stream filter returns HLS + DASH");
  assert.strictEqual(filterPopupCandidates(combinedList, "direct", "").length, 1, "Direct filter returns MP4");
  assert.strictEqual(filterPopupCandidates(combinedList, "all", "lecture42").length, 1, "Search query filters by title/filename");

  const loadingHtml = renderDetectedMediaList(combinedList, { loading: true });
  assert(loadingHtml.includes('data-state="loading"'));
  const offlineHtml = renderDetectedMediaList(combinedList, { desktopConnected: false });
  assert(offlineHtml.includes('data-state="disconnected"') && offlineHtml.includes("Browser Download"));

  // Direct MP4 handoff falls back to browser download when OpenIDM is offline
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

  console.log("\nBrowser Extension & Detection Suite: 8 / 8 suites PASSED.");
}

runTests().catch((err) => {
  console.error("FAIL:", err);
  process.exit(1);
});
