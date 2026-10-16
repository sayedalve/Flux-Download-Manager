// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/background.js) & Brisk Browser Extension for Flux Download Manager.
// Handles webRequest header correlation, media detection, ordinary download interception,
// message routing with popup/content scripts, tab badge synchronization, and Native Messaging RPC handoff + browser fallback.

const { G } = (typeof require !== "undefined") ? require("./init.js") : { G: globalThis.G };
const {
  isSiteDisabled,
  setSiteDisabled,
  extractUrlExtension,
  classifyNetworkResource,
  addOrDeduplicateMediaCandidate
} = (typeof require !== "undefined") ? require("./function.js") : globalThis;

let rpcRequestCounter = 1;

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
        G.cacheData.set(Number(k), v);
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

// Sends a JSON-RPC 2.0 message to flux-nmh via Native Messaging
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
    return new Promise((resolve, reject) => {
      chrome.runtime.sendNativeMessage(G.nativeHostName, payload, (response) => {
        if (chrome.runtime.lastError) {
          reject(new Error(chrome.runtime.lastError.message));
        } else if (response && response.error) {
          reject(new Error(response.error.message || "RPC Error"));
        } else {
          resolve(response ? response.result : null);
        }
      });
    });
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

// 1. Record outgoing request headers (Referer, Origin, Cookie, User-Agent) per requestId
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

// 2. Inspect incoming response headers to detect MP4/WebM/HLS/DASH resources
function onHeadersReceivedHandler(details) {
  const classified = classifyNetworkResource(details);
  if (!classified) return null;

  const reqHeaders = G.requestHeadersMap.get(details.requestId) || {};
  const tabId = typeof details.tabId === "number" ? details.tabId : 0;

  const candidate = {
    url: classified.url,
    ext: classified.ext,
    streamType: classified.streamType,
    mime: classified.mime,
    size: classified.size,
    tabId,
    frameId: details.frameId || 0,
    tabUrl: details.documentUrl || details.initiator || classified.url,
    pageTitle: details.pageTitle || "",
    headers: reqHeaders
  };

  const res = addOrDeduplicateMediaCandidate(tabId, candidate);
  updateTabBadge(tabId);
  persistSessionState().catch(() => {});
  return res;
}

// 3. Intercept eligible ordinary file downloads and hand off to Flux (with safe browser fallback)
async function onDownloadCreatedHandler(downloadItem, customTransport = null, cancelBrowserDownloadFn = null) {
  if (!G.enableCapture || !downloadItem || !downloadItem.url) {
    return { intercepted: false, reason: "CAPTURE_DISABLED" };
  }
  if (G.bypassNextDownload) {
    G.bypassNextDownload = false;
    return { intercepted: false, reason: "BYPASS_MODIFIER_ACTIVE" };
  }
  if (downloadItem.byExtensionId || downloadItem.url.startsWith("blob:") || downloadItem.url.startsWith("data:")) {
    return { intercepted: false, reason: "EXTENSION_OR_BLOB_URL" };
  }
  if (isSiteDisabled(downloadItem.referrer || downloadItem.url)) {
    return { intercepted: false, reason: "SITE_EXCLUDED" };
  }

  const ext = extractUrlExtension(downloadItem.filename || downloadItem.url);
  const fileSize = downloadItem.fileSize || 0;
  if (!G.InterceptFileExts.has(ext) && (fileSize > 0 && fileSize < G.minFileSizeBytes)) {
    return { intercepted: false, reason: "BELOW_SIZE_OR_EXT_FILTER" };
  }

  try {
    const result = await sendNativeRpc("download.offer", {
      url: downloadItem.url,
      filename: downloadItem.filename || "",
      fileSize,
      tabUrl: downloadItem.referrer || "",
      bypassModifier: false,
      headers: downloadItem.headers || (downloadItem.referrer ? { Referer: downloadItem.referrer } : {})
    }, customTransport);

    if (result && result.accepted) {
      if (cancelBrowserDownloadFn) {
        await cancelBrowserDownloadFn(downloadItem.id);
      } else if (typeof chrome !== "undefined" && chrome.downloads && chrome.downloads.cancel) {
        chrome.downloads.cancel(downloadItem.id);
      }
      return { intercepted: true, taskId: result.taskId, fileName: result.fileName };
    }
    return { intercepted: false, reason: (result && result.reason) || "DECLINED_BY_DESKTOP" };
  } catch (err) {
    // Safe fallback: if Flux desktop app is unreachable, let browser finish downloading natively!
    return { intercepted: false, fallbackToBrowser: true, error: err.message };
  }
}

// 4. Submit a detected video or stream candidate to Flux
async function submitMediaCandidateToFlux(candidate, options = {}, customTransport = null, browserDownloadFn = null) {
  const params = {
    candidateId: candidate.candidateId || "",
    streamType: candidate.streamType || "HLS",
    manifestUrl: candidate.url,
    url: candidate.url,
    pageTitle: options.pageTitle || candidate.pageTitle || "",
    tabUrl: candidate.tabUrl || "",
    frameUrl: candidate.frameUrl || "",
    outputFormat: options.outputFormat || "mp4",
    videoFilter: options.videoFilter || "best",
    audioFilter: options.audioFilter || "all",
    subtitleFilter: options.subtitleFilter || "all",
    customKeyHex: options.customKeyHex || "",
    cachedManifestBody: candidate.cachedManifestBody || "",
    headers: candidate.headers || {}
  };
  try {
    const res = await sendNativeRpc("stream.add", params, customTransport);
    candidate.handoffStatus = "Queued in Flux";
    return res;
  } catch (err) {
    const isDirectFile = candidate.streamType !== "HLS" && candidate.streamType !== "DASH";
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

const submitMediaCandidateToOpenIDM = submitMediaCandidateToFlux;

// 5. Message listener handling Popup and Content Script RPCs
function handleRuntimeMessage(msg, sender, sendResponse) {
  if (!msg || !msg.action) return false;

  const tabId = (msg.tabId !== undefined && msg.tabId !== null)
    ? msg.tabId
    : (sender.tab && sender.tab.id ? sender.tab.id : 0);

  if (msg.action === "getMediaList") {
    sendNativeRpc("system.handshake", { protocolVersion: "1.0" })
      .then(() => {
        sendResponse({
          success: true,
          desktopConnected: true,
          captureEnabled: G.enableCapture,
          siteDisabled: isSiteDisabled(msg.tabUrl || ""),
          candidates: G.cacheData.get(tabId) || []
        });
      })
      .catch(() => {
        sendResponse({
          success: true,
          desktopConnected: false,
          captureEnabled: G.enableCapture,
          siteDisabled: isSiteDisabled(msg.tabUrl || ""),
          candidates: G.cacheData.get(tabId) || []
        });
      });
    return true; // Keep channel open for async response
  }

  if (msg.action === "addMedia") {
    const candidate = {
      url: msg.url,
      ext: msg.ext || extractUrlExtension(msg.url),
      streamType: msg.streamType || "HLS",
      mime: msg.mime || "",
      size: msg.size || 0,
      tabId,
      tabUrl: msg.tabUrl || (sender.tab ? sender.tab.url : ""),
      pageTitle: msg.pageTitle || (sender.tab ? sender.tab.title : "video"),
      cachedManifestBody: msg.cachedManifestBody || "",
      headers: (sender.tab && sender.tab.url) ? { Referer: sender.tab.url } : {}
    };
    const res = addOrDeduplicateMediaCandidate(tabId, candidate);
    updateTabBadge(tabId);
    persistSessionState().catch(() => {});
    sendResponse({ success: true, added: res.added, count: (G.cacheData.get(tabId) || []).length });
    return true;
  }

  if (msg.action === "downloadMedia") {
    let candidate = null;
    const tabList = G.cacheData.get(tabId) || [];
    if (msg.candidateId) {
      candidate = tabList.find(c => c.candidateId === msg.candidateId);
    }
    if (!candidate && msg.url) {
      candidate = {
        url: msg.url,
        ext: extractUrlExtension(msg.url) || "mp4",
        streamType: (msg.url.includes(".m3u8") ? "HLS" : (msg.url.includes(".mpd") ? "DASH" : "HTTP_FILE")),
        pageTitle: msg.pageTitle || "",
        tabUrl: msg.tabUrl || (sender.tab ? sender.tab.url : "")
      };
    }
    if (candidate) {
      submitMediaCandidateToFlux(candidate)
        .then(result => sendResponse({ success: true, result }))
        .catch(err => sendResponse({ success: false, error: err.message }));
      return true;
    }
    sendResponse({ success: false, error: "Media candidate not found" });
    return true;
  }

  if (msg.action === "clearMedia") {
    G.cacheData.delete(tabId);
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
    const extraReqOpts = ["requestHeaders"];
    if (chrome.webRequest.OnBeforeSendHeadersOptions && chrome.webRequest.OnBeforeSendHeadersOptions.EXTRA_HEADERS) {
      extraReqOpts.push("extraHeaders");
    }
    chrome.webRequest.onBeforeSendHeaders.addListener(
      onBeforeSendHeadersHandler,
      { urls: ["*://*/*"] },
      extraReqOpts
    );
    chrome.webRequest.onHeadersReceived.addListener(
      onHeadersReceivedHandler,
      { urls: ["*://*/*"] },
      ["responseHeaders"]
    );
  }

  // Runtime Message Listener
  if (chrome.runtime && chrome.runtime.onMessage) {
    chrome.runtime.onMessage.addListener(handleRuntimeMessage);
  }

  // Downloads API Listener
  if (chrome.downloads && chrome.downloads.onCreated) {
    chrome.downloads.onCreated.addListener((downloadItem) => {
      onDownloadCreatedHandler(downloadItem).catch(() => {});
    });
  }

  // Tab Lifecycle Listeners
  if (chrome.tabs) {
    if (chrome.tabs.onActivated) {
      chrome.tabs.onActivated.addListener((activeInfo) => {
        updateTabBadge(activeInfo.tabId);
      });
    }
    if (chrome.tabs.onRemoved) {
      chrome.tabs.onRemoved.addListener((tabId) => {
        G.cacheData.delete(tabId);
      });
    }
  }

  // Restore state on service worker startup
  restoreSessionState().catch(() => {});
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    persistSessionState,
    restoreSessionState,
    sendNativeRpc,
    updateTabBadge,
    onBeforeSendHeadersHandler,
    onHeadersReceivedHandler,
    onDownloadCreatedHandler,
    submitMediaCandidateToFlux,
    submitMediaCandidateToOpenIDM,
    handleRuntimeMessage
  };
}
