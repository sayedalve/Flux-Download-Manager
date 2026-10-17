#!/usr/bin/env node
// SPDX-License-Identifier: GPL-3.0-or-later
// Automated Browser Extension Verification Suite (13 Suites)
// Verifies Cat Catch media detection, HLS/DASH discovery, master/child deduplication,
// segment suppression, per-site exclusion, DOM overlay attachment (including empty-<video> rejection
// and active blob: URL discovery), Deep Search hook, MV3 Service Worker session persistence,
// Popup filtering/states, 5-way typed stream handoff (never passing JSON as .m3u8),
// and per-tab/per-URL ordinary download click/redirect correlation + Alt-bypass race guards.

const assert = require("assert");
const { G, SourceType } = require("../extension/js/init.js");
const {
  isSiteDisabled,
  setSiteDisabled,
  classifyMediaSourceType,
  classifyNetworkResource,
  addOrDeduplicateMediaCandidate,
  recordClickedLink,
  recordRedirectHop,
  findCorrelatedClick
} = require("../extension/js/function.js");
const {
  persistSessionState,
  restoreSessionState,
  onBeforeSendHeadersHandler,
  onHeadersReceivedHandler,
  onDownloadCreatedHandler,
  submitMediaCandidateToFlux,
  submitMediaCandidateToOpenIDM,
  resolveCandidateForDownloadRequest,
  handleRuntimeMessage
} = require("../extension/js/background.js");
const {
  scanAndAttachVideoOverlays,
  observeDynamicVideosAndFullscreen,
  handleLinkClick
} = require("../extension/js/content-script.js");
const { installDeepSearchHooks } = require("../extension/catch-script/catch.js");
const {
  filterPopupCandidates,
  renderDetectedMediaList
} = require("../extension/js/popup.js");

async function runTests() {
  console.log("[1/14] Testing Direct MP4/WebM & HLS/DASH Detection + Header Correlation...");
  onBeforeSendHeadersHandler({
    requestId: "req-101",
    url: "https://cdn.example.com/media/lecture42.mp4",
    tabId: 10,
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
  assert.strictEqual(mp4Res.added, true, "Direct MP4 >= 64 KiB must be detected");
  assert.strictEqual(mp4Res.candidate.sourceType, SourceType.DIRECT_PROGRESSIVE);
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
  assert.strictEqual(tinyMp4, null, "Tiny UI video clip (< 64 KiB) filtered out");
  console.log("  -> PASS");

  console.log("[2/14] Testing Master vs. Child HLS Deduplication & Segment Suppression...");
  const masterRes = onHeadersReceivedHandler({
    requestId: "req-201",
    tabId: 20,
    frameId: 0,
    url: "https://vod.example.com/streams/99/master.m3u8?token=xyz",
    responseHeaders: [{ name: "Content-Type", value: "application/vnd.apple.mpegurl" }]
  });
  assert.strictEqual(masterRes.added, true);
  assert.strictEqual(masterRes.candidate.isMaster, true);
  assert.strictEqual(masterRes.candidate.sourceType, SourceType.HLS_MANIFEST);

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
    frameId: 0,
    url: "https://vod.example.com/dash/movie.mpd",
    responseHeaders: [{ name: "Content-Type", value: "application/dash+xml" }]
  });
  assert.strictEqual(dashRes.added, true);
  assert.strictEqual(dashRes.candidate.streamType, "DASH");
  assert.strictEqual(dashRes.candidate.sourceType, SourceType.DASH_MANIFEST);
  console.log("  -> PASS");

  console.log("[3/14] Testing Per-Site Exclusion Setting & Popup Rendering...");
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

  console.log("[4/14] Testing Page-Context Deep Search Hook (Obfuscated Manifest Capture)...");
  const capturedEvents = [];
  const fakeWindow = {
    location: { href: "https://player.example.com/embed/77", hostname: "player.example.com" },
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
  assert.strictEqual(capturedEvents[0].sourceType, "HLS_MANIFEST");
  assert(capturedEvents[0].cachedManifestBody.includes("#EXT-X-STREAM-INF"));
  console.log("  -> PASS");

  console.log("[5/14] Testing In-Page Closed Shadow DOM Video Overlay & Fullscreen Repositioning...");
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
  assert.strictEqual(obsResult.activeOverlays.length, 1, "Overlay attached to visible <video> with blob: src");
  assert.strictEqual(obsResult.activeOverlays[0].hostEl.shadowMode, "closed", "Uses closed Shadow DOM");
  assert(obsResult.activeOverlays[0].hostEl.style.cssText.includes("top:50px"), "Positioned at top-right of video");

  videoRect = { width: 1920, height: 1080, top: 0, left: 0, right: 1920 };
  fullscreenHandler();
  assert(obsResult.activeOverlays[0].hostEl.style.cssText.includes("left:1752px"), "Overlay repositioned on fullscreenchange");

  assert.strictEqual(obsResult.activeOverlays[0].btn.textContent, "Download with Flux", "Overlay button text rebranded to Flux");
  obsResult.activeOverlays[0].btn._clickHandler({ stopPropagation: () => {}, preventDefault: () => {} });
  assert.ok(clickedVideoPayload, "Overlay button click triggers real download handoff");
  assert.strictEqual(clickedVideoPayload.isMseBlob, true);
  assert.strictEqual(clickedVideoPayload.blobUrl, "blob:https://iframe.example.com/9999-aaaa", "Passes actual blobUrl rather than losing context");
  console.log("  -> PASS");

  console.log("[6/14] Testing Download Interception, Bypass Modifier & Safe Browser Fallback...");
  let canceledDownloadId = null;
  const mockTransportAccept = async (rpc) => {
    assert.strictEqual(rpc.method, "download.offer");
    assert.ok(rpc.params.correlationId, "Correlation ID included in download.offer");
    return { accepted: true, taskId: "task-uuid-1234", fileName: "ubuntu.iso" };
  };
  const interceptRes = await onDownloadCreatedHandler(
    { id: 55, url: "https://releases.example.com/ubuntu.iso", filename: "ubuntu.iso", fileSize: 50000000, referrer: "https://releases.example.com/" },
    mockTransportAccept,
    async (id) => { canceledDownloadId = id; }
  );
  assert.strictEqual(interceptRes.intercepted, true);
  assert.strictEqual(canceledDownloadId, 55, "Browser built-in download canceled after Flux accepted");

  const mockTransportOffline = async () => { throw new Error("Specified native messaging host not found"); };
  const fallbackRes = await onDownloadCreatedHandler(
    { id: 56, url: "https://releases.example.com/ubuntu.iso", filename: "ubuntu.iso", fileSize: 50000000 },
    mockTransportOffline
  );
  assert.strictEqual(fallbackRes.intercepted, false);
  assert.strictEqual(fallbackRes.fallbackToBrowser, true, "Safely falls back to browser when desktop app is offline");
  console.log("  -> PASS");

  console.log("[7/14] Testing Manifest V3 Service Worker Session State Persistence & Recovery...");
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

  console.log("[8/14] Testing Popup Filter Tabs, Search, States & Direct Media Fallback...");
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

  console.log("[9/14] Testing Link-Click Interception & Alt-Key Bypass Behavior...");
  let sentLinkMessages = [];
  globalThis.chrome = {
    runtime: {
      sendMessage: (msg) => { sentLinkMessages.push(msg); }
    }
  };

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
  assert.strictEqual(sentLinkMessages.length, 1);
  assert.strictEqual(sentLinkMessages[0].action, "linkClicked");
  assert.strictEqual(sentLinkMessages[0].url, "https://files.example.com/downloads/archive.zip");
  assert.strictEqual(sentLinkMessages[0].filename, "archive.zip");

  sentLinkMessages = [];
  const fakeAltEvent = {
    altKey: true,
    target: { closest: () => fakeZipAnchor }
  };
  handleLinkClick(fakeAltEvent);
  assert.strictEqual(sentLinkMessages.length, 1);
  assert.strictEqual(sentLinkMessages[0].action, "bypassNextDownload");
  console.log("  -> PASS");

  console.log("[10/14] Testing Webpage Watch URL Guard & Background Runtime Message Routing...");
  const { isWebpageWatchUrl, classifyUrlIntent } = require("../extension/js/function.js");

  assert.strictEqual(isWebpageWatchUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"), true);
  assert.strictEqual(isWebpageWatchUrl("https://youtu.be/dQw4w9WgXcQ"), true);
  assert.strictEqual(isWebpageWatchUrl("https://www.youtube.com/shorts/abcd1234efg"), true);
  assert.strictEqual(isWebpageWatchUrl("https://vimeo.com/12345678"), true);
  assert.strictEqual(classifyUrlIntent("https://www.youtube.com/watch?v=dQw4w9WgXcQ"), "WEBPAGE");

  assert.strictEqual(isWebpageWatchUrl("https://cdn.example.com/video.mp4"), false);
  assert.strictEqual(classifyUrlIntent("https://cdn.example.com/stream.m3u8"), "HLS");
  assert.strictEqual(classifyUrlIntent("https://cdn.example.com/stream.mpd"), "DASH");
  assert.strictEqual(classifyUrlIntent("https://cdn.example.com/archive.zip"), "DIRECT_FILE");

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

  console.log("[11/14] Testing Defect A Fix: Empty <video> Overlay Suppression & Unresolved Blob Error...");
  const emptyVideoDoc = {
    title: "Empty Player Page",
    location: { href: "https://empty.example.com/" },
    body: { appendChild: () => {} },
    createElement: (tag) => ({
      tag,
      style: {},
      dataset: {},
      setAttribute: () => {},
      attachShadow: () => ({ appendChild: () => {} }),
      addEventListener: () => {}
    }),
    querySelectorAll: () => [
      {
        currentSrc: "",
        src: "",
        mediaKeys: null,
        dataset: {},
        querySelector: () => null,
        getBoundingClientRect: () => ({ width: 640, height: 360, top: 10, left: 10, right: 650 })
      }
    ]
  };
  const emptyAttached = scanAndAttachVideoOverlays(emptyVideoDoc, () => {});
  assert.strictEqual(emptyAttached.length, 0, "Must NOT attach Flux button to empty <video> with no src or candidate");

  const unresolvedBlobRes = resolveCandidateForDownloadRequest(777, {
    url: "",
    blobUrl: "blob:https://unbacked.example.com/1111-2222",
    isMseBlob: true,
    tabUrl: "https://unbacked.example.com/watch"
  }, {});
  assert.ok(unresolvedBlobRes.error, "Unbacked blob: player must return a clear error rather than submitting an empty URL");
  console.log("  -> PASS");

  console.log("[12/14] Testing Defect B Fix: Typed YouTube Extraction & Never Passing JSON as cachedManifestBody...");
  const ytEvents = [];
  const fakeYtWindow = {
    location: { href: "https://www.youtube.com/watch?v=abc123xyz", hostname: "www.youtube.com" },
    document: { title: "Sample YouTube Lecture - YouTube" },
    ytInitialPlayerResponse: {
      playabilityStatus: { status: "OK" },
      videoDetails: { videoId: "abc123xyz", title: "Sample YouTube Lecture" },
      streamingData: {
        adaptiveFormats: [
          { itag: 137, mimeType: 'video/mp4; codecs="avc1.640028"', bitrate: 4000000, qualityLabel: "1080p", url: "https://rr1---sn-test.googlevideo.com/videoplayback?itag=137&mime=video%2Fmp4&range=0-999" },
          { itag: 140, mimeType: 'audio/mp4; codecs="mp4a.40.2"', bitrate: 128000, url: "https://rr1---sn-test.googlevideo.com/videoplayback?itag=140&mime=audio%2Fmp4&rn=2" }
        ]
      }
    }
  };
  installDeepSearchHooks(fakeYtWindow, "yt-nonce", (ev) => ytEvents.push(ev));
  assert.strictEqual(ytEvents.length, 1);
  assert.strictEqual(ytEvents[0].streamType, "YOUTUBE");
  assert.strictEqual(ytEvents[0].sourceType, SourceType.YOUTUBE_EXTRACTION);
  assert.strictEqual(ytEvents[0].cachedManifestBody, "", "YouTube JSON must NEVER be placed in cachedManifestBody");
  assert.ok(ytEvents[0].extractorJson.includes("YOUTUBE_STREAM"), "YouTube descriptor placed in extractorJson");
  assert.ok(!ytEvents[0].videoUrl.includes("range="), "Stripped MSE range param from adaptive videoUrl");
  assert.ok(ytEvents[0].pairedAudioUrl.includes("itag=140"), "Paired adaptive audio track URL included");

  let submittedRpcPayload = null;
  await submitMediaCandidateToFlux(ytEvents[0], {}, async (rpc) => {
    submittedRpcPayload = rpc;
    return { accepted: true, taskId: "yt-task-1" };
  });
  assert.strictEqual(submittedRpcPayload.method, "stream.add");
  assert.strictEqual(submittedRpcPayload.params.sourceType, SourceType.YOUTUBE_EXTRACTION);
  assert.strictEqual(submittedRpcPayload.params.cachedManifestBody, "", "stream.add must receive empty cachedManifestBody for YouTube");
  assert.ok(submittedRpcPayload.params.extractorJson.includes("YOUTUBE_STREAM"));

  // Adaptive video-only googlevideo network response must be classified as YOUTUBE_EXTRACTION, not standalone HTTP_FILE
  const gvAdaptive = classifyNetworkResource({
    tabId: 88,
    url: "https://rr2---sn-test.googlevideo.com/videoplayback?itag=137&mime=video%2Fmp4&clen=10485760",
    initiator: "https://www.youtube.com/watch?v=abc123xyz",
    responseHeaders: [{ name: "Content-Type", value: "video/mp4" }]
  });
  assert.strictEqual(gvAdaptive.sourceType, SourceType.YOUTUBE_EXTRACTION, "Adaptive googlevideo itag=137 classified as YOUTUBE_EXTRACTION");
  console.log("  -> PASS");

  console.log("[13/14] Testing Defect C Fix: Multi-Tab Click Correlation, 302 Redirects & Alt-Bypass across onCreated + onDeterminingFilename...");
  // 1. Record click on /redirect/release on Tab 42
  recordClickedLink(42, {
    url: "https://mirror.example.com/redirect/release",
    filename: "",
    referrer: "https://mirror.example.com/downloads",
    pageTitle: "Mirror Downloads"
  });
  // 2. Simulate 302 redirect to final CDN URL without file extension in initial path
  onHeadersReceivedHandler({
    requestId: "req-redir-1",
    tabId: 42,
    url: "https://mirror.example.com/redirect/release",
    statusCode: 302,
    responseHeaders: [{ name: "Location", value: "https://cdn.mirror.example.com/blobs/pkg_9988" }]
  });
  // 3. Simulate 200 attachment response on redirected URL
  onHeadersReceivedHandler({
    requestId: "req-redir-1",
    tabId: 42,
    url: "https://cdn.mirror.example.com/blobs/pkg_9988",
    statusCode: 200,
    responseHeaders: [
      { name: "Content-Type", value: "application/octet-stream" },
      { name: "Content-Disposition", value: 'attachment; filename="Flux_Portable_Package.zip"' },
      { name: "Content-Length", value: "262144" }
    ]
  });

  let offeredRedirectParams = null;
  const redirDownloadItem = {
    id: 901,
    tabId: 42,
    url: "https://mirror.example.com/redirect/release",
    finalUrl: "https://cdn.mirror.example.com/blobs/pkg_9988",
    filename: ""
  };
  const redirIntercept = await onDownloadCreatedHandler(
    redirDownloadItem,
    async (rpc) => {
      offeredRedirectParams = rpc.params;
      return { accepted: true, taskId: "redir-task-901", fileName: "Flux_Portable_Package.zip" };
    },
    async () => {}
  );
  assert.strictEqual(redirIntercept.intercepted, true, "Redirected attachment download intercepted reliably");
  assert.strictEqual(offeredRedirectParams.filename, "Flux_Portable_Package.zip", "Extracted filename from correlated Content-Disposition");
  assert.strictEqual(offeredRedirectParams.originalUrl, "https://mirror.example.com/redirect/release", "Preserved original clicked URL across redirect");
  assert.strictEqual(offeredRedirectParams.tabUrl, "https://mirror.example.com/downloads", "Preserved referrer from per-tab click record");

  // 4. Verify Alt-bypass survives BOTH onCreated and onDeterminingFilename for the same downloadItem.id
  handleRuntimeMessage({ action: "bypassNextDownload", tabId: 42, url: "https://mirror.example.com/files/manual.pdf" }, { tab: { id: 42 } }, () => {});
  const bypassItem = { id: 902, tabId: 42, url: "https://mirror.example.com/files/manual.pdf", filename: "manual.pdf" };
  const bypassOnCreated = await onDownloadCreatedHandler(bypassItem, null, null, null, "created");
  const bypassOnDetermine = await onDownloadCreatedHandler(bypassItem, null, null, null, "determine");
  assert.strictEqual(bypassOnCreated.intercepted, false);
  assert.strictEqual(bypassOnCreated.reason, "BYPASS_MODIFIER_ACTIVE");
  assert.strictEqual(bypassOnDetermine.intercepted, false, "onDeterminingFilename must ALSO honor Alt-bypass for the same downloadItem.id");
  assert.strictEqual(bypassOnDetermine.reason, "BYPASS_MODIFIER_ACTIVE");
  console.log("  -> PASS");

  // -------------------------------------------------------------------------
  // 14. Automatic Multi-Quality Video Grab on Playback (YouTube, HLS, DASH, HTML5 <source>)
  // -------------------------------------------------------------------------
  console.log("[14/14] Testing Automatic Multi-Quality Video Extraction & Selection (1080p / 720p / 360p / Audio)...");
  G.cacheData.clear();

  // 14a. HLS Master Playlist multi-quality parsing
  const hlsMasterBody = [
    "#EXTM3U",
    "#EXT-X-VERSION:3",
    '#EXT-X-STREAM-INF:BANDWIDTH=4500000,RESOLUTION=1920x1080,CODECS="avc1.640028,mp4a.40.2"',
    "1080p/index.m3u8",
    '#EXT-X-STREAM-INF:BANDWIDTH=2500000,RESOLUTION=1280x720,CODECS="avc1.64001f,mp4a.40.2"',
    "720p/index.m3u8",
    '#EXT-X-STREAM-INF:BANDWIDTH=800000,RESOLUTION=640x360,CODECS="avc1.4d401e,mp4a.40.2"',
    "360p/index.m3u8"
  ].join("\n");
  const hlsAdded = addOrDeduplicateMediaCandidate(55, {
    url: "https://cdn.example.com/live/master.m3u8",
    streamType: "HLS",
    sourceType: "HLS_MANIFEST",
    pageTitle: "Multi-Quality HLS Lecture",
    cachedManifestBody: hlsMasterBody
  });
  assert.strictEqual(hlsAdded.candidate.qualities.length, 3, "HLS master should expose 3 distinct video qualities");
  assert.deepStrictEqual(hlsAdded.candidate.qualities.map(q => q.resolution), ["1080p", "720p", "360p"]);

  // Verify selecting 720p passes videoFilter="720p" and appends [720p] to title
  let submittedHls720 = null;
  await submitMediaCandidateToFlux(
    hlsAdded.candidate,
    { qualityId: "720p" },
    async (rpc) => {
      submittedHls720 = rpc;
      return { accepted: true, taskId: "task-hls-720" };
    }
  );
  assert.strictEqual(submittedHls720.params.videoFilter, "720p");
  assert.ok(submittedHls720.params.pageTitle.includes("[720p]"), "Selected quality tag should be included in title");

  // 14b. YouTube Player multi-quality extraction via catch.js hook
  const mqYtEvents = [];
  const mqYtWindow = {
    location: { href: "https://www.youtube.com/watch?v=MultiQual99", hostname: "www.youtube.com" },
    ytInitialPlayerResponse: {
      playabilityStatus: { status: "OK" },
      videoDetails: { videoId: "MultiQual99", title: "Flux Multi Quality Demo - YouTube" },
      streamingData: {
        adaptiveFormats: [
          { itag: 137, mimeType: 'video/mp4; codecs="avc1.640028"', bitrate: 4200000, width: 1920, height: 1080, qualityLabel: "1080p", contentLength: "50000000", url: "https://rr1---sn-abc.googlevideo.com/videoplayback?itag=137&mime=video%2Fmp4&range=0-999" },
          { itag: 136, mimeType: 'video/mp4; codecs="avc1.64001f"', bitrate: 2200000, width: 1280, height: 720, qualityLabel: "720p", contentLength: "25000000", url: "https://rr1---sn-abc.googlevideo.com/videoplayback?itag=136&mime=video%2Fmp4&range=0-999" },
          { itag: 134, mimeType: 'video/mp4; codecs="avc1.4d401e"', bitrate: 700000, width: 640, height: 360, qualityLabel: "360p", contentLength: "9000000", url: "https://rr1---sn-abc.googlevideo.com/videoplayback?itag=134&mime=video%2Fmp4&range=0-999" },
          { itag: 140, mimeType: 'audio/mp4; codecs="mp4a.40.2"', bitrate: 128000, contentLength: "3500000", url: "https://rr1---sn-abc.googlevideo.com/videoplayback?itag=140&mime=audio%2Fmp4&rn=1" }
        ]
      }
    }
  };
  installDeepSearchHooks(mqYtWindow, "nonce-mq", (ev) => mqYtEvents.push(ev));
  assert.strictEqual(mqYtEvents.length, 1);
  assert.strictEqual(mqYtEvents[0].qualities.length, 4, "Should extract 1080p, 720p, 360p, and Audio Only");
  assert.deepStrictEqual(mqYtEvents[0].qualities.map(q => q.resolution), ["1080p", "720p", "360p", "Audio"]);

  // Render Popup HTML and verify quality selector <select class="quality-select">
  const ytCandRes = addOrDeduplicateMediaCandidate(56, mqYtEvents[0]);
  const mqPopupHtml = renderDetectedMediaList([ytCandRes.candidate], { desktopConnected: true });
  assert.ok(mqPopupHtml.includes('class="quality-select"'), "Popup must render interactive quality selector dropdown");
  assert.ok(mqPopupHtml.includes("1080p") && mqPopupHtml.includes("720p") && mqPopupHtml.includes("360p") && mqPopupHtml.includes("Audio Only"));
  console.log("  -> PASS");

  console.log("\nBrowser Extension & Detection Suite: 14 / 14 suites PASSED.");
}

runTests().catch((err) => {
  console.error("FAIL:", err);
  process.exit(1);
});
