// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/background.js) & Brisk Browser Extension for Flux Download Manager.
// Handles Service Worker module initialization, webRequest header correlation, media detection,
// ordinary download interception, message routing with popup/content scripts, tab badge synchronization,
// and Native Messaging RPC handoff + safe browser fallback.

if (typeof importScripts === "function" && typeof globalThis.G === "undefined") {
  try {
    importScripts("/js/init.js", "/js/function.js");
  } catch (_) {
    importScripts("init.js", "function.js");
  }
} else if (typeof require !== "undefined") {
  Object.assign(globalThis, require("./init.js"), require("./function.js"));
}
var G = globalThis.G;

let rpcRequestCounter = 1;
let sessionRestorePromise = null;

// Persist & restore tab media candidates across Manifest V3 Service Worker suspension
async function persistSessionState(storageArea = null) {
  const store = storageArea || ((typeof chrome !== "undefined" && chrome.storage && chrome.storage.session) ? chrome.storage.session : null);
  if (!store || typeof store.set !== "function") return false;
  const serializedTabs = {};
  for (const [tabId, list] of G.cacheData.entries()) {
    serializedTabs[String(tabId)] = list;
  }
  await store.set({
    flux_cacheData: serializedTabs,
    flux_disabledHosts: Array.from(G.disabledHosts)
  });
  return true;
}

async function restoreSessionState(storageArea = null) {
  const store = storageArea || ((typeof chrome !== "undefined" && chrome.storage && chrome.storage.session) ? chrome.storage.session : null);
  if (!store || typeof store.get !== "function") return false;
  const data = await store.get(["flux_cacheData", "flux_disabledHosts"]);
  const cacheSource = data ? data.flux_cacheData : null;
  if (cacheSource) {
    for (const [k, v] of Object.entries(cacheSource)) {
      if (Array.isArray(v)) {
        const tid = Number(k);
        const existing = G.cacheData.get(tid) || [];
        if (existing.length === 0) {
          G.cacheData.set(tid, v);
        } else {
          for (const item of v) {
            addOrDeduplicateMediaCandidate(tid, item);
          }
        }
      }
    }
  }
  const disabledSource = data ? data.flux_disabledHosts : null;
  if (Array.isArray(disabledSource)) {
    for (const h of disabledSource) {
      G.disabledHosts.add(h);
    }
  }
  return true;
}

function ensureSessionRestored(storageArea = null) {
  if (!sessionRestorePromise) {
    sessionRestorePromise = restoreSessionState(storageArea).catch(() => false);
  }
  return sessionRestorePromise;
}

// Sends a JSON-RPC 2.0 message to flux-nmh via Native Messaging (with fallback host names)
async function sendNativeRpc(method, params, customTransport = null) {
  const payload = {
    jsonrpc: "2.0",
    id: "ext-" + (rpcRequestCounter++),
    method,
    params
  };

  if (customTransport) {
    return await customTransport(payload);
  }

  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendNativeMessage) {
    const hostsToTry = [G.nativeHostName, ...(G.fallbackNativeHostNames || [])];
    let lastErrorMsg = "Native Messaging runtime unavailable";

    for (const hostName of hostsToTry) {
      if (!hostName) continue;
      try {
        const res = await new Promise((resolve, reject) => {
          chrome.runtime.sendNativeMessage(hostName, payload, (response) => {
            if (chrome.runtime.lastError) {
              reject(new Error(chrome.runtime.lastError.message));
            } else if (response && response.error) {
              const rpcErr = new Error(response.error.message || "RPC Error");
              rpcErr.isRpcError = true;
              reject(rpcErr);
            } else {
              resolve(response ? response.result : null);
            }
          });
        });
        return res;
      } catch (err) {
        if (err && err.isRpcError) {
          throw err;
        }
        lastErrorMsg = err ? err.message : String(err);
      }
    }
    throw new Error(lastErrorMsg);
  }
  throw new Error("Native Messaging runtime unavailable");
}

function updateTabBadge(tabId) {
  if (typeof chrome === "undefined" || !chrome.action || !chrome.action.setBadgeText) return;
  const list = G.cacheData.get(tabId) || [];
  const count = list.length;
  const text = count > 0 ? String(count) : "";
  try {
    chrome.action.setBadgeText({ text, tabId: (typeof tabId === "number" && tabId > 0) ? tabId : undefined });
    chrome.action.setBadgeBackgroundColor({ color: "#2563EB", tabId: (typeof tabId === "number" && tabId > 0) ? tabId : undefined });
  } catch (_) {}
}

function notifyPopupMediaUpdated(tabId) {
  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
    try {
      chrome.runtime.sendMessage({ action: "mediaUpdated", tabId }, () => {
        if (chrome.runtime) void chrome.runtime.lastError;
      });
    } catch (_) {}
  }
}

function resolveEffectiveTabId(details) {
  if (details && typeof details.tabId === "number" && details.tabId > 0) {
    return details.tabId;
  }
  const sourceUrl = (details && (details.documentUrl || details.initiator)) || "";
  if (sourceUrl) {
    for (const [tid, turl] of G.tabUrlMap.entries()) {
      if (turl && (turl === sourceUrl || turl.startsWith(sourceUrl))) {
        return tid;
      }
    }
  }
  return (details && typeof details.tabId === "number") ? details.tabId : 0;
}

// 1. Record outgoing request headers (Referer, Origin, Cookie, User-Agent, Authorization) per requestId
function onBeforeSendHeadersHandler(details) {
  if (!details || !details.requestId) return;
  const captured = {};
  if (Array.isArray(details.requestHeaders)) {
    for (const h of details.requestHeaders) {
      if (!h || !h.name) continue;
      const lower = h.name.toLowerCase();
      if (lower === "referer" || lower === "origin" || lower === "cookie" ||
          lower === "user-agent" || lower === "authorization") {
        captured[h.name] = h.value || "";
      }
    }
  }
  G.requestHeadersMap.set(details.requestId, captured);
  if (G.requestHeadersMap.size > 2000) {
    const oldestKey = G.requestHeadersMap.keys().next().value;
    G.requestHeadersMap.delete(oldestKey);
  }
}

// 2. Inspect incoming response headers to detect MP4/WebM/HLS/DASH/googlevideo resources
function onHeadersReceivedHandler(details) {
  const effectiveTabId = resolveEffectiveTabId(details);
  const enrichedDetails = Object.assign({}, details, { tabId: effectiveTabId });
  const classified = classifyNetworkResource(enrichedDetails);
  if (!classified) return null;

  const reqHeaders = Object.assign({}, G.requestHeadersMap.get(details.requestId) || {});
  const tabUrl = G.tabUrlMap.get(effectiveTabId) || details.documentUrl || details.initiator || classified.url;
  if (!reqHeaders.Referer && !reqHeaders.referer && tabUrl && !tabUrl.startsWith("chrome-extension:")) {
    reqHeaders.Referer = tabUrl;
  }

  const candidate = {
    url: classified.url,
    ext: classified.ext,
    streamType: classified.streamType,
    mime: classified.mime,
    size: classified.size,
    itag: classified.itag || "",
    isAudioOnly: Boolean(classified.isAudioOnly),
    isGoogleVideo: Boolean(classified.isGoogleVideo),
    fileName: classified.fileName || "",
    tabId: effectiveTabId,
    frameId: details.frameId || 0,
    tabUrl,
    pageTitle: details.pageTitle || "",
    headers: reqHeaders
  };

  const res = addOrDeduplicateMediaCandidate(effectiveTabId, candidate);
  updateTabBadge(effectiveTabId);
  persistSessionState().catch(() => {});
  notifyPopupMediaUpdated(effectiveTabId);
  return res;
}

// 3. Intercept eligible ordinary file downloads and hand off to Flux (with safe browser fallback)
async function onDownloadCreatedHandler(downloadItem, customTransport = null, cancelBrowserDownloadFn = null) {
  if (!G.enableCapture || !downloadItem || (!downloadItem.url && !downloadItem.finalUrl)) {
    return { intercepted: false, reason: "CAPTURE_DISABLED" };
  }
  if (G.bypassNextDownload) {
    G.bypassNextDownload = false;
    return { intercepted: false, reason: "BYPASS_MODIFIER_ACTIVE" };
  }

  const targetUrl = downloadItem.finalUrl || downloadItem.url;
  if (downloadItem.byExtensionId || targetUrl.startsWith("blob:") || targetUrl.startsWith("data:") || targetUrl.startsWith("chrome-extension:")) {
    return { intercepted: false, reason: "EXTENSION_OR_BLOB_URL" };
  }

  const clickedInfo = (G.lastClickedLink && (Date.now() - G.lastClickedLink.time < 15000)) ? G.lastClickedLink : null;
  const pageReferrer = downloadItem.referrer || (clickedInfo ? clickedInfo.referrer : "") || "";

  if (isSiteDisabled(pageReferrer || targetUrl)) {
    return { intercepted: false, reason: "SITE_EXCLUDED" };
  }

  // Never intercept HTML watch pages as ordinary binary downloads
  if (isWebpageWatchUrl(targetUrl)) {
    return { intercepted: false, reason: "WEBPAGE_URL" };
  }

  const candidateFilename = downloadItem.filename || (clickedInfo ? clickedInfo.filename : "") || "";
  const ext = extractUrlExtension(candidateFilename) ||
              extractUrlExtension(targetUrl) ||
              extractUrlExtension(downloadItem.url || "");
  const mime = (downloadItem.mime || "").split(";")[0].trim().toLowerCase();
  const fileSize = downloadItem.fileSize || downloadItem.totalBytes || 0;

  const isEligibleExt = G.InterceptFileExts.has(ext);
  const isEligibleMime = G.InterceptMimeTypes && G.InterceptMimeTypes.has(mime);
  const wasExplicitLinkClick = Boolean(clickedInfo && (clickedInfo.url === downloadItem.url || clickedInfo.url === targetUrl));

  if (!isEligibleExt && !isEligibleMime && !wasExplicitLinkClick && (fileSize > 0 && fileSize < G.minFileSizeBytes)) {
    return { intercepted: false, reason: "BELOW_SIZE_OR_EXT_FILTER" };
  }

  // Prevent duplicate interception of the same browser download ID
  if (downloadItem.id !== undefined && G.handledDownloadIds) {
    if (G.handledDownloadIds.has(downloadItem.id)) {
      return { intercepted: false, reason: "ALREADY_HANDLED" };
    }
    G.handledDownloadIds.add(downloadItem.id);
    if (G.handledDownloadIds.size > 500) {
      const oldest = G.handledDownloadIds.values().next().value;
      G.handledDownloadIds.delete(oldest);
    }
  }

  if (!customTransport && typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.pause === "function" && downloadItem.id !== undefined) {
    try { chrome.downloads.pause(downloadItem.id, () => { if (chrome.runtime) void chrome.runtime.lastError; }); } catch (_) {}
  }

  const headers = Object.assign({}, downloadItem.headers || {});
  if (pageReferrer && !headers.Referer && !headers.referer) {
    headers.Referer = pageReferrer;
  }

  try {
    const result = await sendNativeRpc("download.offer", {
      url: targetUrl,
      originalUrl: downloadItem.url || targetUrl,
      filename: candidateFilename ? candidateFilename.split(/[/\\]/).pop() : "",
      fileSize: fileSize > 0 ? fileSize : 0,
      tabUrl: pageReferrer,
      bypassModifier: false,
      headers
    }, customTransport);

    if (result && result.accepted) {
      if (cancelBrowserDownloadFn) {
        await cancelBrowserDownloadFn(downloadItem.id);
      } else if (typeof chrome !== "undefined" && chrome.downloads && chrome.downloads.cancel) {
        chrome.downloads.cancel(downloadItem.id, () => {
          if (chrome.runtime) void chrome.runtime.lastError;
          if (chrome.downloads.erase) {
            chrome.downloads.erase({ id: downloadItem.id }, () => {
              if (chrome.runtime) void chrome.runtime.lastError;
            });
          }
        });
      }
      return { intercepted: true, taskId: result.taskId, fileName: result.fileName };
    }

    if (!customTransport && typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.resume === "function" && downloadItem.id !== undefined) {
      try { chrome.downloads.resume(downloadItem.id, () => { if (chrome.runtime) void chrome.runtime.lastError; }); } catch (_) {}
    }
    return { intercepted: false, reason: (result && result.reason) || "DECLINED_BY_DESKTOP" };
  } catch (err) {
    if (!customTransport && typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.resume === "function" && downloadItem.id !== undefined) {
      try { chrome.downloads.resume(downloadItem.id, () => { if (chrome.runtime) void chrome.runtime.lastError; }); } catch (_) {}
    }
    return { intercepted: false, fallbackToBrowser: true, error: err.message };
  }
}

// 4. Submit a detected video or stream candidate to Flux
async function submitMediaCandidateToFlux(candidate, options = {}, customTransport = null, browserDownloadFn = null) {
  let cachedManifestBody = candidate.cachedManifestBody || "";

  if ((candidate.streamType === "YOUTUBE" || candidate.pairedAudioUrl) && !cachedManifestBody) {
    cachedManifestBody = JSON.stringify({
      fluxType: "YOUTUBE_STREAM",
      title: options.pageTitle || candidate.pageTitle || "youtube_video",
      watchUrl: candidate.tabUrl || candidate.url,
      videoUrl: candidate.url,
      audioUrl: candidate.pairedAudioUrl || ""
    });
  }

  const effectiveStreamType = (candidate.pairedAudioUrl && candidate.streamType === "HTTP_FILE")
    ? "YOUTUBE"
    : (candidate.streamType || "HLS");

  const defaultExt = (effectiveStreamType === "HTTP_FILE" && candidate.ext)
    ? candidate.ext
    : "mp4";

  const params = {
    candidateId: candidate.candidateId || "",
    streamType: effectiveStreamType,
    manifestUrl: candidate.url,
    url: candidate.url,
    pageTitle: options.pageTitle || candidate.pageTitle || candidate.fileName || "",
    tabUrl: candidate.tabUrl || "",
    frameUrl: candidate.frameUrl || "",
    outputFormat: options.outputFormat || defaultExt,
    videoFilter: options.videoFilter || "best",
    audioFilter: options.audioFilter || "all",
    subtitleFilter: options.subtitleFilter || "all",
    customKeyHex: options.customKeyHex || "",
    cachedManifestBody,
    headers: candidate.headers || {}
  };

  try {
    const res = await sendNativeRpc("stream.add", params, customTransport);
    candidate.handoffStatus = "Queued in Flux";
    return res;
  } catch (err) {
    const isDirectFile = effectiveStreamType === "HTTP_FILE" && !isWebpageWatchUrl(candidate.url);
    if (isDirectFile) {
      if (browserDownloadFn) {
        await browserDownloadFn({ url: candidate.url });
        candidate.handoffStatus = "Browser Fallback";
        return { accepted: false, fallbackToBrowser: true, url: candidate.url };
      } else if (typeof chrome !== "undefined" && chrome.downloads && chrome.downloads.download) {
        chrome.downloads.download({ url: candidate.url });
        candidate.handoffStatus = "Browser Fallback";
        return { accepted: false, fallbackToBrowser: true, url: candidate.url };
      }
    }
    throw err;
  }
}

var submitMediaCandidateToOpenIDM = submitMediaCandidateToFlux;

function resolveCandidateForDownloadRequest(tabId, msg, sender) {
  const tabList = G.cacheData.get(tabId) || [];
  const effectiveTabUrl = msg.tabUrl || (sender && sender.tab && sender.tab.url) || G.tabUrlMap.get(tabId) || "";
  const effectiveTitle = (msg.pageTitle || (sender && sender.tab && sender.tab.title) || "video").replace(/\s*-\s*YouTube$/i, "").trim();

  // 1. Explicit candidateId match (e.g. from Popup UI)
  if (msg.candidateId) {
    const byId = tabList.find(c => c.candidateId === msg.candidateId);
    if (byId) return { candidate: byId };
  }

  // 2. Direct non-blob URL match
  if (msg.url && !msg.url.startsWith("blob:") && !isWebpageWatchUrl(msg.url)) {
    const normUrl = normalizeMediaUrl(msg.url);
    const existing = tabList.find(c => c.url === normUrl || c.url === msg.url);
    if (existing) {
      if (effectiveTitle && (!existing.pageTitle || existing.pageTitle === "video")) {
        existing.pageTitle = effectiveTitle;
      }
      return { candidate: existing };
    }
    return {
      candidate: {
        url: normUrl,
        ext: extractUrlExtension(normUrl) || "mp4",
        streamType: (normUrl.includes(".m3u8") ? "HLS" : (normUrl.includes(".mpd") ? "DASH" : "HTTP_FILE")),
        pageTitle: effectiveTitle,
        tabUrl: effectiveTabUrl,
        headers: effectiveTabUrl ? { Referer: effectiveTabUrl } : {}
      }
    };
  }

  // 3. Highest-confidence detected candidate on this tab (for MSE blob: videos like YouTube, HLS.js, dash.js)
  if (tabList.length > 0) {
    const best = tabList[0];
    if (effectiveTitle && (!best.pageTitle || best.pageTitle === "video")) {
      best.pageTitle = effectiveTitle;
    }
    return { candidate: best };
  }

  // 4. Check if the page reported a specific player error (e.g. YouTube unplayable / private / DRM video)
  if (G.playerErrors && G.playerErrors.has(tabId)) {
    return { error: G.playerErrors.get(tabId) };
  }

  // 5. If on a valid YouTube watch/shorts/embed URL, route via YOUTUBE site extraction worker
  const targetPageUrl = effectiveTabUrl || msg.frameUrl || msg.url || "";
  if (isYouTubeWatchOrShortsUrl(targetPageUrl)) {
    const ytCand = {
      url: targetPageUrl,
      ext: "mp4",
      streamType: "YOUTUBE",
      pageTitle: effectiveTitle || "youtube_video",
      tabUrl: targetPageUrl,
      headers: { Referer: targetPageUrl }
    };
    addOrDeduplicateMediaCandidate(tabId, ytCand);
    updateTabBadge(tabId);
    return { candidate: ytCand };
  }

  return {
    error: "No downloadable media stream detected on this tab yet. Start video playback and try again."
  };
}

// 5. Message listener handling Popup and Content Script RPCs
function handleRuntimeMessage(msg, sender, sendResponse) {
  if (!msg || !msg.action) return false;

  const tabId = (msg.tabId !== undefined && msg.tabId !== null && msg.tabId !== 0)
    ? Number(msg.tabId)
    : (sender && sender.tab && sender.tab.id ? sender.tab.id : 0);

  if (sender && sender.tab && sender.tab.id && sender.tab.url) {
    G.tabUrlMap.set(sender.tab.id, sender.tab.url);
  }

  if (msg.action === "getMediaList") {
    ensureSessionRestored().then(() => {
      const targetTabUrl = msg.tabUrl || G.tabUrlMap.get(tabId) || "";
      return sendNativeRpc("system.handshake", { protocolVersion: "1.0" })
        .then(() => {
          sendResponse({
            success: true,
            desktopConnected: true,
            captureEnabled: G.enableCapture,
            siteDisabled: isSiteDisabled(targetTabUrl),
            playerError: (G.playerErrors && G.playerErrors.get(tabId)) || "",
            candidates: G.cacheData.get(tabId) || []
          });
        })
        .catch(() => {
          sendResponse({
            success: true,
            desktopConnected: false,
            captureEnabled: G.enableCapture,
            siteDisabled: isSiteDisabled(targetTabUrl),
            playerError: (G.playerErrors && G.playerErrors.get(tabId)) || "",
            candidates: G.cacheData.get(tabId) || []
          });
        });
    });
    return true;
  }

  if (msg.action === "addMedia") {
    if (!msg.url || (isWebpageWatchUrl(msg.url) && msg.streamType !== "YOUTUBE" && msg.streamType !== "SITE_EXTRACTION")) {
      sendResponse({ success: false, added: false, error: "Invalid or webpage URL" });
      return true;
    }
    const sourceTabUrl = msg.tabUrl || (sender && sender.tab ? sender.tab.url : "") || G.tabUrlMap.get(tabId) || "";
    if (isSiteDisabled(sourceTabUrl)) {
      sendResponse({ success: false, added: false, error: "Site capture disabled" });
      return true;
    }

    if (G.playerErrors) G.playerErrors.delete(tabId);

    const candidate = {
      url: normalizeMediaUrl(msg.url),
      ext: msg.ext || extractUrlExtension(msg.url) || "mp4",
      streamType: msg.streamType || (msg.url.includes(".m3u8") ? "HLS" : (msg.url.includes(".mpd") ? "DASH" : "HTTP_FILE")),
      mime: msg.mime || "",
      size: msg.size || 0,
      itag: msg.itag || "",
      isAudioOnly: Boolean(msg.isAudioOnly),
      isGoogleVideo: Boolean(msg.isGoogleVideo),
      videoId: msg.videoId || "",
      resolution: msg.resolution || "",
      pairedAudioUrl: msg.pairedAudioUrl || "",
      tabId,
      tabUrl: sourceTabUrl,
      pageTitle: (msg.pageTitle || (sender && sender.tab ? sender.tab.title : "video") || "video").replace(/\s*-\s*YouTube$/i, "").trim(),
      cachedManifestBody: msg.cachedManifestBody || "",
      headers: sourceTabUrl ? { Referer: sourceTabUrl } : {}
    };
    const res = addOrDeduplicateMediaCandidate(tabId, candidate);
    updateTabBadge(tabId);
    persistSessionState().catch(() => {});
    notifyPopupMediaUpdated(tabId);
    sendResponse({ success: true, added: res.added, count: (G.cacheData.get(tabId) || []).length });
    return true;
  }

  if (msg.action === "playerError") {
    if (G.playerErrors && msg.reason) {
      G.playerErrors.set(tabId, msg.reason);
    }
    sendResponse({ success: true });
    return true;
  }

  if (msg.action === "addKey") {
    if (msg.key) {
      if (!G.capturedKeys.has(tabId)) G.capturedKeys.set(tabId, new Set());
      G.capturedKeys.get(tabId).add(msg.key);
    }
    sendResponse({ success: true });
    return true;
  }

  if (msg.action === "downloadMedia") {
    ensureSessionRestored().then(() => {
      const resolved = resolveCandidateForDownloadRequest(tabId, msg, sender);
      if (resolved.error || !resolved.candidate) {
        sendResponse({ success: false, error: resolved.error || "Media candidate not found" });
        return;
      }
      submitMediaCandidateToFlux(resolved.candidate, { pageTitle: msg.pageTitle })
        .then(result => {
          persistSessionState().catch(() => {});
          notifyPopupMediaUpdated(tabId);
          sendResponse({ success: true, result });
        })
        .catch(err => {
          sendResponse({ success: false, error: err.message || "Failed to submit download to Flux" });
        });
    });
    return true;
  }

  if (msg.action === "clearMedia") {
    G.cacheData.delete(tabId);
    if (G.playerErrors) G.playerErrors.delete(tabId);
    updateTabBadge(tabId);
    persistSessionState().catch(() => {});
    sendResponse({ success: true });
    return true;
  }

  if (msg.action === "setSiteCapture") {
    if (msg.tabUrl) {
      setSiteDisabled(msg.tabUrl, !msg.enabled);
      persistSessionState().catch(() => {});
    }
    sendResponse({ success: true, enabled: msg.enabled });
    return true;
  }

  if (msg.action === "bypassNextDownload") {
    G.bypassNextDownload = true;
    sendResponse({ success: true });
    return true;
  }

  if (msg.action === "linkClicked") {
    G.lastClickedLink = {
      url: msg.url,
      filename: msg.filename,
      referrer: msg.referrer,
      pageTitle: msg.pageTitle || "",
      time: Date.now()
    };
    if (msg.bypass) {
      G.bypassNextDownload = true;
    }
    sendResponse({ success: true });
    return true;
  }

  if (msg.action === "pingDesktop") {
    sendNativeRpc("system.handshake", { protocolVersion: "1.0" })
      .then(() => sendResponse({ connected: true }))
      .catch(() => sendResponse({ connected: false }));
    return true;
  }

  return false;
}

// 6. Register extension runtime listeners
if (typeof chrome !== "undefined") {
  // WebRequest Listeners
  if (chrome.webRequest) {
    if (chrome.webRequest.onBeforeRequest) {
      chrome.webRequest.onBeforeRequest.addListener(
        (details) => {
          if (details && details.type === "main_frame" && details.tabId > 0 && details.url) {
            const prevUrl = G.tabUrlMap.get(details.tabId) || "";
            const normPrev = prevUrl.split("#")[0];
            const normNext = details.url.split("#")[0];
            G.tabUrlMap.set(details.tabId, details.url);
            if (normPrev && normPrev !== normNext) {
              G.cacheData.delete(details.tabId);
              if (G.playerErrors) G.playerErrors.delete(details.tabId);
              updateTabBadge(details.tabId);
            }
          }
        },
        { urls: ["*://*/*"] }
      );
    }

    const extraReqOpts = ["requestHeaders"];
    if (chrome.webRequest.OnBeforeSendHeadersOptions && chrome.webRequest.OnBeforeSendHeadersOptions.EXTRA_HEADERS) {
      extraReqOpts.push("extraHeaders");
    }
    chrome.webRequest.onBeforeSendHeaders.addListener(
      onBeforeSendHeadersHandler,
      { urls: ["*://*/*"] },
      extraReqOpts
    );

    const extraRespOpts = ["responseHeaders"];
    if (chrome.webRequest.OnHeadersReceivedOptions && chrome.webRequest.OnHeadersReceivedOptions.EXTRA_HEADERS) {
      extraRespOpts.push("extraHeaders");
    }
    chrome.webRequest.onHeadersReceived.addListener(
      onHeadersReceivedHandler,
      { urls: ["*://*/*"] },
      extraRespOpts
    );
  }

  // Runtime Message Listener
  if (chrome.runtime && chrome.runtime.onMessage) {
    chrome.runtime.onMessage.addListener(handleRuntimeMessage);
  }

  // Downloads API Listeners
  if (chrome.downloads) {
    if (chrome.downloads.onDeterminingFilename) {
      chrome.downloads.onDeterminingFilename.addListener((downloadItem, suggest) => {
        try { if (typeof suggest === "function") suggest(); } catch (_) {}
        onDownloadCreatedHandler(downloadItem).catch(() => {});
      });
    }
    if (chrome.downloads.onCreated) {
      chrome.downloads.onCreated.addListener((downloadItem) => {
        const ext = extractUrlExtension(downloadItem.filename || downloadItem.finalUrl || downloadItem.url || "");
        if (G.bypassNextDownload || G.InterceptFileExts.has(ext) || !chrome.downloads.onDeterminingFilename) {
          onDownloadCreatedHandler(downloadItem).catch(() => {});
        }
      });
    }
  }

  // Tab Lifecycle Listeners
  if (chrome.tabs) {
    if (chrome.tabs.onActivated) {
      chrome.tabs.onActivated.addListener((activeInfo) => {
        updateTabBadge(activeInfo.tabId);
      });
    }
    if (chrome.tabs.onUpdated) {
      chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
        if (tab && tab.url) {
          G.tabUrlMap.set(tabId, tab.url);
        }
        if (changeInfo && changeInfo.status === "complete") {
          updateTabBadge(tabId);
        }
      });
    }
    if (chrome.tabs.onRemoved) {
      chrome.tabs.onRemoved.addListener((tabId) => {
        G.cacheData.delete(tabId);
        G.tabUrlMap.delete(tabId);
        if (G.playerErrors) G.playerErrors.delete(tabId);
      });
    }
  }

  // Restore state on service worker startup
  ensureSessionRestored();
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    persistSessionState,
    restoreSessionState,
    ensureSessionRestored,
    sendNativeRpc,
    updateTabBadge,
    onBeforeSendHeadersHandler,
    onHeadersReceivedHandler,
    onDownloadCreatedHandler,
    submitMediaCandidateToFlux,
    submitMediaCandidateToOpenIDM,
    resolveCandidateForDownloadRequest,
    handleRuntimeMessage
  };
}
