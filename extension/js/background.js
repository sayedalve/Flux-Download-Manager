// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/background.js) & Brisk Browser Extension for OpenIDM.
// Handles webRequest header correlation, media detection, ordinary download interception,
// Manifest V3 service worker state persistence, and Native Messaging RPC handoff + browser fallback.

const { G } = (typeof require !== "undefined") ? require("./init.js") : { G: globalThis.G };
const {
  isSiteDisabled,
  setSiteDisabled,
  extractUrlExtension,
  classifyNetworkResource,
  addOrDeduplicateMediaCandidate
} = (typeof require !== "undefined") ? require("./function.js") : globalThis;

let rpcRequestCounter = 1;

// Persist & restore tab media candidates across Manifest V3 Service Worker suspension (BUG-013 Fix)
async function persistSessionState(storageArea = null) {
  const store = storageArea || ((typeof chrome !== "undefined" && chrome.storage && chrome.storage.session) ? chrome.storage.session : null);
  if (!store || typeof store.set !== "function") return false;
  const serializedTabs = {};
  for (const [tabId, list] of G.cacheData.entries()) {
    serializedTabs[String(tabId)] = list;
  }
  await store.set({
    openidm_cacheData: serializedTabs,
    openidm_disabledHosts: Array.from(G.disabledHosts)
  });
  return true;
}

async function restoreSessionState(storageArea = null) {
  const store = storageArea || ((typeof chrome !== "undefined" && chrome.storage && chrome.storage.session) ? chrome.storage.session : null);
  if (!store || typeof store.get !== "function") return false;
  const data = await store.get(["openidm_cacheData", "openidm_disabledHosts"]);
  if (data && data.openidm_cacheData) {
    for (const [k, v] of Object.entries(data.openidm_cacheData)) {
      if (Array.isArray(v)) {
        G.cacheData.set(Number(k), v);
      }
    }
  }
  if (data && Array.isArray(data.openidm_disabledHosts)) {
    for (const h of data.openidm_disabledHosts) {
      G.disabledHosts.add(h);
    }
  }
  return true;
}

// Sends a JSON-RPC 2.0 message to openidm-nmh via Native Messaging (or injected transport in tests)
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

// 2. Inspect incoming response headers to detect MP4/WebM/HLS/DASH resources (Cat Catch findMedia)
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
  persistSessionState().catch(() => {});
  return res;
}

// 3. Intercept eligible ordinary file downloads and hand off to OpenIDM (with safe browser fallback)
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
    // Safe fallback: if OpenIDM desktop app is unreachable, let browser finish downloading natively!
    return { intercepted: false, fallbackToBrowser: true, error: err.message };
  }
}

// 4. Submit a detected video or stream candidate to OpenIDM's unified queue (with browser fallback for direct media)
async function submitMediaCandidateToOpenIDM(candidate, options = {}, customTransport = null, browserDownloadFn = null) {
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
    candidate.handoffStatus = "Queued in OpenIDM";
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

if (typeof chrome !== "undefined" && chrome.webRequest) {
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

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    persistSessionState,
    restoreSessionState,
    sendNativeRpc,
    onBeforeSendHeadersHandler,
    onHeadersReceivedHandler,
    onDownloadCreatedHandler,
    submitMediaCandidateToOpenIDM
  };
}
