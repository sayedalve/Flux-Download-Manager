// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/background.js) & Brisk Browser Extension for Flux Download Manager.
// Handles Service Worker module initialization, webRequest header & redirect correlation,
// explicit typed media detection (HLS_MANIFEST, DASH_MANIFEST, DIRECT_PROGRESSIVE, YOUTUBE_EXTRACTION, UNSUPPORTED),
// ordinary download interception with per-tab/per-URL click tracking and attachment support,
// message routing with popup/content scripts, tab badge synchronization, and Native Messaging RPC handoff.

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
var SourceType = globalThis.SourceType || (G && G.SourceType) || {
  HLS_MANIFEST: "HLS_MANIFEST",
  DASH_MANIFEST: "DASH_MANIFEST",
  DIRECT_PROGRESSIVE: "DIRECT_PROGRESSIVE",
  YOUTUBE_EXTRACTION: "YOUTUBE_EXTRACTION",
  UNSUPPORTED: "UNSUPPORTED"
};

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

// Sends a JSON-RPC 2.0 message to flux-nmh via Native Messaging (with fallback host names & correlation logging)
async function sendNativeRpc(method, params, customTransport = null) {
  const correlationId = (params && params.correlationId) || generateCorrelationId("rpc");
  const payload = {
    jsonrpc: "2.0",
    id: "ext-" + (rpcRequestCounter++),
    method,
    params: Object.assign({}, params, { correlationId })
  };

  logPipelineStage(correlationId, "NATIVE_RPC_SENT", { method, rpcId: payload.id });

  if (customTransport) {
    const res = await customTransport(payload);
    logPipelineStage(correlationId, "NATIVE_RPC_RESULT", {
      method,
      accepted: res ? Boolean(res.accepted !== false) : true,
      taskId: res ? (res.taskId || "") : ""
    });
    return res;
  }

  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendNativeMessage) {
    const hostsToTry = [G.nativeHostName, ...(G.fallbackNativeHostNames || [])];
    let lastErrorMsg = "Native Messaging runtime unavailable";

    for (const hostName of hostsToTry) {
      if (!hostName) continue;
      try {
        const res = await new Promise((resolve, reject) => {
          let settled = false;
          const timer = setTimeout(() => {
            if (!settled) {
              settled = true;
              reject(new Error("Native Messaging RPC timed out after 5000ms"));
            }
          }, 5000);
          try {
            chrome.runtime.sendNativeMessage(hostName, payload, (response) => {
              if (settled) return;
              settled = true;
              clearTimeout(timer);
              if (chrome.runtime.lastError) {
                reject(new Error(chrome.runtime.lastError.message));
              } else if (response && response.error) {
                const rpcErr = new Error(response.error.message || "RPC Error");
                rpcErr.isRpcError = true;
                rpcErr.code = response.error.code;
                reject(rpcErr);
              } else {
                resolve(response ? response.result : null);
              }
            });
          } catch (e) {
            if (!settled) {
              settled = true;
              clearTimeout(timer);
              reject(e);
            }
          }
        });
        logPipelineStage(correlationId, "NATIVE_RPC_RESULT", {
          method,
          hostName,
          accepted: res ? Boolean(res.accepted !== false) : true,
          taskId: res ? (res.taskId || "") : ""
        });
        return res;
      } catch (err) {
        if (err && err.isRpcError) {
          logPipelineStage(correlationId, "NATIVE_RPC_ERROR", { method, error: err.message });
          throw err;
        }
        lastErrorMsg = err ? err.message : String(err);
      }
    }
    logPipelineStage(correlationId, "NATIVE_RPC_UNAVAILABLE", { method, error: lastErrorMsg });
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
  if (typeof chrome !== "undefined" && chrome.tabs && typeof chrome.tabs.sendMessage === "function" && typeof tabId === "number" && tabId > 0) {
    try {
      chrome.tabs.sendMessage(tabId, { action: "mediaUpdated", tabId }, () => {
        if (chrome.runtime) void chrome.runtime.lastError;
      });
    } catch (_) {}
  }
}

async function autoProbeManifestQualities(tabId, candidate) {
  if (!candidate || !candidate.url) return false;
  if (candidate.cachedManifestBody && Array.isArray(candidate.qualities) && candidate.qualities.length > 0) {
    return true;
  }
  const isHls = candidate.sourceType === SourceType.HLS_MANIFEST || candidate.streamType === "HLS";
  const isDash = candidate.sourceType === SourceType.DASH_MANIFEST || candidate.streamType === "DASH";
  if (!isHls && !isDash) return false;
  if (!candidate.url.startsWith("http://") && !candidate.url.startsWith("https://")) return false;
  if (typeof fetch !== "function") return false;
  if (candidate.__probingQualities) return false;
  candidate.__probingQualities = true;

  try {
    const resp = await fetch(candidate.url, { method: "GET" });
    if (!resp || !resp.ok) return false;
    const bodyText = await resp.text();
    if (!bodyText) return false;
    if (isHls && isValidHlsManifestText(bodyText)) {
      candidate.cachedManifestBody = bodyText;
      const parsed = parseHlsMasterQualities(bodyText, candidate.url);
      if (parsed.length > 0) {
        candidate.qualities = mergeQualityLists(candidate.qualities || [], parsed);
        if (!candidate.resolution || candidate.resolution === "Auto") {
          candidate.resolution = candidate.qualities[0].resolution;
        }
        notifyPopupMediaUpdated(tabId);
        return true;
      }
    } else if (isDash && isValidDashManifestText(bodyText)) {
      candidate.cachedManifestBody = bodyText;
      const parsed = parseDashManifestQualities(bodyText, candidate.url);
      if (parsed.length > 0) {
        candidate.qualities = mergeQualityLists(candidate.qualities || [], parsed);
        if (!candidate.resolution || candidate.resolution === "Auto") {
          candidate.resolution = candidate.qualities[0].resolution;
        }
        notifyPopupMediaUpdated(tabId);
        return true;
      }
    }
  } catch (_) {
  } finally {
    candidate.__probingQualities = false;
  }
  return false;
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

  if (G.requestMetaMap && details.url) {
    const prev = G.requestMetaMap.get(details.requestId) || {};
    G.requestMetaMap.set(details.requestId, Object.assign(prev, {
      initialUrl: prev.initialUrl || details.url,
      currentUrl: details.url,
      tabId: resolveEffectiveTabId(details),
      headers: captured,
      timestamp: Date.now()
    }));
    if (G.requestMetaMap.size > 2000) {
      G.requestMetaMap.delete(G.requestMetaMap.keys().next().value);
    }
  }
}

// 2. Inspect incoming response headers to track redirects, attachment downloads, and media resources
function onHeadersReceivedHandler(details) {
  if (!details || !details.url) return null;
  const effectiveTabId = resolveEffectiveTabId(details);
  const hmap = parseResponseHeaderMap(details.responseHeaders);
  const reqHeaders = Object.assign({}, G.requestHeadersMap.get(details.requestId) || {});
  const tabUrl = G.tabUrlMap.get(effectiveTabId) || details.documentUrl || details.initiator || details.url;
  if (!reqHeaders.Referer && !reqHeaders.referer && tabUrl && !tabUrl.startsWith("chrome-extension:")) {
    reqHeaders.Referer = tabUrl;
  }

  // 2a. Track HTTP Redirect chains (301, 302, 303, 307, 308) for ordinary download correlation
  if (typeof details.statusCode === "number" &&
      (details.statusCode === 301 || details.statusCode === 302 || details.statusCode === 303 ||
       details.statusCode === 307 || details.statusCode === 308)) {
    const loc = hmap["location"] || "";
    if (loc) {
      let resolvedTarget = loc;
      try { resolvedTarget = new URL(loc, details.url).href; } catch (_) {}
      recordRedirectHop(details.url, resolvedTarget, details.requestId, effectiveTabId);
      if (details.requestId && G.requestMetaMap) {
        const meta = G.requestMetaMap.get(details.requestId) || { initialUrl: details.url };
        meta.currentUrl = resolvedTarget;
        G.requestMetaMap.set(details.requestId, meta);
      }
    }
    return null;
  }

  // 2b. Record eligible attachment / binary download response metadata for correlation with chrome.downloads
  const cdHeader = hmap["content-disposition"] || "";
  const cdFilename = extractFilenameFromContentDisposition(cdHeader);
  const isAttachment = isAttachmentContentDisposition(cdHeader);
  const contentType = (hmap["content-type"] || "").split(";")[0].trim().toLowerCase();
  const totalSize = parseTotalSizeFromHeaders(hmap, details.url);
  const ext = extractUrlExtension(cdFilename) || extractUrlExtension(details.url);
  const reqMeta = (details.requestId && G.requestMetaMap && G.requestMetaMap.get(details.requestId)) || null;
  const originalUrl = (reqMeta && reqMeta.initialUrl) || resolveInitialUrlFromRedirects(details.url) || details.url;

  if (isAttachment || G.InterceptFileExts.has(ext) || (G.InterceptMimeTypes && G.InterceptMimeTypes.has(contentType))) {
    const respMeta = {
      url: details.url,
      originalUrl,
      filename: cdFilename || "",
      isAttachment,
      mime: contentType,
      size: totalSize,
      ext,
      tabId: effectiveTabId,
      tabUrl,
      headers: reqHeaders,
      timestamp: Date.now(),
      correlationId: generateCorrelationId("netdl")
    };
    if (G.recentDownloadResponses) {
      G.recentDownloadResponses.set(normalizeCorrelationUrl(details.url), respMeta);
      if (originalUrl !== details.url) {
        G.recentDownloadResponses.set(normalizeCorrelationUrl(originalUrl), respMeta);
      }
      if (G.recentDownloadResponses.size > 500) {
        G.recentDownloadResponses.delete(G.recentDownloadResponses.keys().next().value);
      }
    }
    logPipelineStage(respMeta.correlationId, "NETWORK_DOWNLOAD_CLASSIFIED", {
      tabId: effectiveTabId,
      url: details.url,
      originalUrl,
      filename: cdFilename,
      isAttachment,
      mime: contentType,
      size: totalSize
    });
  }

  // 2c. Classify streaming/video media resources for Popup & Video Overlay
  const enrichedDetails = Object.assign({}, details, { tabId: effectiveTabId });
  const classified = classifyNetworkResource(enrichedDetails);
  if (!classified) return null;

  const candidate = {
    url: classified.url,
    videoUrl: classified.videoUrl || "",
    pairedAudioUrl: classified.pairedAudioUrl || "",
    ext: classified.ext,
    streamType: classified.streamType,
    sourceType: classified.sourceType || classifyMediaSourceType(classified, classified.mime),
    mime: classified.mime,
    size: classified.size,
    itag: classified.itag || "",
    isAudioOnly: Boolean(classified.isAudioOnly),
    isGoogleVideo: Boolean(classified.isGoogleVideo),
    isAdaptiveTrack: Boolean(classified.isAdaptiveTrack),
    requiresAudioMux: Boolean(classified.requiresAudioMux),
    requiresCipherExtraction: Boolean(classified.requiresCipherExtraction),
    videoId: classified.videoId || "",
    fileName: classified.fileName || cdFilename || "",
    tabId: effectiveTabId,
    frameId: details.frameId || 0,
    tabUrl,
    pageTitle: details.pageTitle || "",
    headers: reqHeaders,
    correlationId: generateCorrelationId("netmedia")
  };

  const res = addOrDeduplicateMediaCandidate(effectiveTabId, candidate);
  if (res && res.candidate && (!res.candidate.qualities || res.candidate.qualities.length === 0)) {
    autoProbeManifestQualities(effectiveTabId, res.candidate).catch(() => {});
  }
  updateTabBadge(effectiveTabId);
  persistSessionState().catch(() => {});
  notifyPopupMediaUpdated(effectiveTabId);
  return res;
}

function findCorrelatedNetworkResponse(downloadItem) {
  if (!downloadItem || !G.recentDownloadResponses) return null;
  const now = Date.now();
  const urls = [
    normalizeCorrelationUrl(downloadItem.finalUrl || ""),
    normalizeCorrelationUrl(downloadItem.url || ""),
    resolveInitialUrlFromRedirects(downloadItem.finalUrl || downloadItem.url || "")
  ].filter(Boolean);

  for (const u of urls) {
    const meta = G.recentDownloadResponses.get(u);
    if (meta && (now - meta.timestamp) < 60000) {
      return meta;
    }
  }
  return null;
}

// 3. Intercept eligible ordinary file downloads and hand off to Flux (with safe browser fallback)
async function onDownloadCreatedHandler(
  downloadItem,
  customTransport = null,
  cancelBrowserDownloadFn = null,
  suggestCallback = null,
  phase = "determine"
) {
  const invokeSuggestIfNeeded = () => {
    if (typeof suggestCallback === "function") {
      try { suggestCallback(); } catch (_) {}
      suggestCallback = null;
    }
  };

  if (!G.enableCapture || !downloadItem || (!downloadItem.url && !downloadItem.finalUrl)) {
    invokeSuggestIfNeeded();
    return { intercepted: false, reason: "CAPTURE_DISABLED" };
  }

  // Check per-tab, per-URL, and global Alt-key bypass state (Defect C fix)
  if (consumeBypassIfActive(downloadItem.tabId, downloadItem)) {
    invokeSuggestIfNeeded();
    return { intercepted: false, reason: "BYPASS_MODIFIER_ACTIVE" };
  }

  const targetUrl = downloadItem.finalUrl || downloadItem.url;
  if (downloadItem.byExtensionId || targetUrl.startsWith("blob:") || targetUrl.startsWith("data:") || targetUrl.startsWith("chrome-extension:")) {
    invokeSuggestIfNeeded();
    return { intercepted: false, reason: "EXTENSION_OR_BLOB_URL" };
  }

  const netMeta = findCorrelatedNetworkResponse(downloadItem);
  const effectiveTabId = downloadItem.tabId || (netMeta && netMeta.tabId) || 0;
  const clickedInfo = findCorrelatedClick(effectiveTabId, downloadItem);
  const originalUrl = (clickedInfo && clickedInfo.url) ||
                      (netMeta && netMeta.originalUrl) ||
                      resolveInitialUrlFromRedirects(downloadItem.url || targetUrl) ||
                      downloadItem.url ||
                      targetUrl;
  const pageReferrer = downloadItem.referrer ||
                       (clickedInfo ? clickedInfo.referrer : "") ||
                       (netMeta ? netMeta.tabUrl : "") || "";
  const correlationId = (clickedInfo && clickedInfo.correlationId) ||
                        (netMeta && netMeta.correlationId) ||
                        generateCorrelationId("dl");

  if (isSiteDisabled(pageReferrer || targetUrl)) {
    invokeSuggestIfNeeded();
    return { intercepted: false, reason: "SITE_EXCLUDED" };
  }

  // Never intercept HTML watch pages as ordinary binary downloads
  if (isWebpageWatchUrl(targetUrl)) {
    invokeSuggestIfNeeded();
    return { intercepted: false, reason: "WEBPAGE_URL" };
  }

  const rawFilename = downloadItem.filename ||
                      (netMeta && netMeta.filename) ||
                      (clickedInfo && clickedInfo.filename) || "";
  const cleanFilename = rawFilename ? rawFilename.split(/[/\\]/).pop() : "";
  const ext = extractUrlExtension(cleanFilename) ||
              extractUrlExtension(targetUrl) ||
              extractUrlExtension(originalUrl);
  const mime = ((downloadItem.mime || (netMeta && netMeta.mime) || "").split(";")[0]).trim().toLowerCase();
  const fileSize = downloadItem.fileSize || downloadItem.totalBytes || (netMeta && netMeta.size) || 0;

  const isEligibleExt = G.InterceptFileExts.has(ext);
  const isEligibleMime = Boolean(G.InterceptMimeTypes && G.InterceptMimeTypes.has(mime));
  const isAttachment = Boolean(netMeta && netMeta.isAttachment);
  const wasExplicitLinkClick = Boolean(clickedInfo);

  // In Chromium, onDeterminingFilename always fires right after onCreated for every download.
  if (phase === "created") {
    // If this download URL was already pre-intercepted at link-click time, cancel competing download immediately
    const normTarget = normalizeCorrelationUrl(targetUrl);
    const normOrig = normalizeCorrelationUrl(downloadItem.url || "");
    if (G.handledDownloadUrls && (G.handledDownloadUrls.has(normTarget) || G.handledDownloadUrls.has(normOrig))) {
      if (typeof chrome !== "undefined" && chrome.downloads && chrome.downloads.cancel) {
        chrome.downloads.cancel(downloadItem.id, () => {
          if (chrome.downloads.erase) chrome.downloads.erase({ id: downloadItem.id }, () => {});
        });
      }
      return { intercepted: true, reason: "ALREADY_INTERCEPTED_VIA_CLICK" };
    }

    // Immediately pause browser download so it cannot complete or download competing bytes
    if (typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.pause === "function" && downloadItem.id !== undefined) {
      try {
        chrome.downloads.pause(downloadItem.id, () => {
          if (chrome.runtime) void chrome.runtime.lastError;
        });
      } catch (_) {}
    }

    if (typeof chrome !== "undefined" && chrome.downloads && chrome.downloads.onDeterminingFilename) {
      return { intercepted: false, reason: "DEFERRED_TO_FILENAME_DETERMINATION" };
    }
  }

  if (!isEligibleExt && !isEligibleMime && !wasExplicitLinkClick && !isAttachment) {
    if (fileSize === 0 || fileSize < G.minFileSizeBytes) {
      invokeSuggestIfNeeded();
      if (!customTransport && typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.resume === "function" && downloadItem.id !== undefined) {
        try { chrome.downloads.resume(downloadItem.id, () => { if (chrome.runtime) void chrome.runtime.lastError; }); } catch (_) {}
      }
      return { intercepted: false, reason: "BELOW_SIZE_OR_EXT_FILTER" };
    }
  }

  // Prevent duplicate interception of the same browser download ID; if onCreated already initiated an offer
  // and onDeterminingFilename fires while it is in-flight or completed, await the same offer before calling suggest()
  G.inFlightDownloadOffers = G.inFlightDownloadOffers || new Map();
  if (downloadItem.id !== undefined && G.inFlightDownloadOffers.has(downloadItem.id)) {
    try {
      const prevRes = await G.inFlightDownloadOffers.get(downloadItem.id);
      invokeSuggestIfNeeded();
      return prevRes;
    } catch (_) {
      invokeSuggestIfNeeded();
      return { intercepted: false, reason: "ALREADY_HANDLED" };
    }
  }
  if (downloadItem.id !== undefined && G.handledDownloadIds) {
    if (G.handledDownloadIds.has(downloadItem.id)) {
      invokeSuggestIfNeeded();
      return { intercepted: false, reason: "ALREADY_HANDLED" };
    }
    G.handledDownloadIds.add(downloadItem.id);
    if (G.handledDownloadIds.size > 500) {
      const oldest = G.handledDownloadIds.values().next().value;
      G.handledDownloadIds.delete(oldest);
    }
  }

  // Pause browser download while waiting for Flux acknowledgment; NEVER cancel before Flux accepts!
  if (!customTransport && typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.pause === "function" && downloadItem.id !== undefined) {
    try { chrome.downloads.pause(downloadItem.id, () => { if (chrome.runtime) void chrome.runtime.lastError; }); } catch (_) {}
  }

  const headers = Object.assign({}, (netMeta && netMeta.headers) || {}, downloadItem.headers || {});
  if (pageReferrer && !headers.Referer && !headers.referer) {
    headers.Referer = pageReferrer;
  }
  if (!headers.Cookie && !headers.cookie && typeof chrome !== "undefined" && chrome.cookies && typeof chrome.cookies.getAll === "function") {
    try {
      const cookieList = await new Promise((resolve) => {
        chrome.cookies.getAll({ url: targetUrl }, (cList) => {
          if (chrome.runtime) void chrome.runtime.lastError;
          resolve(cList || []);
        });
      });
      if (cookieList && cookieList.length > 0) {
        headers.Cookie = cookieList.map(c => `${c.name}=${c.value}`).join("; ");
      }
    } catch (_) {}
  }

  logPipelineStage(correlationId, "DOWNLOAD_OFFER_INITIATED", {
    downloadId: downloadItem.id,
    url: targetUrl,
    originalUrl,
    filename: cleanFilename,
    fileSize,
    mime,
    wasExplicitLinkClick,
    isAttachment
  });

  const executeOfferFlow = async () => {
    try {
      const result = await sendNativeRpc("download.offer", {
        url: targetUrl,
        originalUrl,
        filename: cleanFilename,
        fileSize: fileSize > 0 ? fileSize : 0,
        mime,
        tabUrl: pageReferrer,
        bypassModifier: false,
        correlationId,
        headers
      }, customTransport);

      if (result && result.accepted) {
        logPipelineStage(correlationId, "DOWNLOAD_ACCEPTED_BY_FLUX", {
          downloadId: downloadItem.id,
          taskId: result.taskId,
          fileName: result.fileName
        });
        if (cancelBrowserDownloadFn) {
          await cancelBrowserDownloadFn(downloadItem.id);
        } else if (typeof chrome !== "undefined" && chrome.downloads && chrome.downloads.cancel) {
          const tryCancel = (retries = 3) => {
            chrome.downloads.cancel(downloadItem.id, () => {
              if (chrome.runtime && chrome.runtime.lastError) {
                if (retries > 0) {
                  setTimeout(() => tryCancel(retries - 1), 50);
                  return;
                }
              }
              if (chrome.downloads.erase) {
                chrome.downloads.erase({ id: downloadItem.id }, () => {
                  if (chrome.runtime) void chrome.runtime.lastError;
                });
              }
            });
          };
          tryCancel();
        }
        return { intercepted: true, taskId: result.taskId, fileName: result.fileName, correlationId };
      }

      // Flux declined: allow native browser download to continue seamlessly
      logPipelineStage(correlationId, "DOWNLOAD_DECLINED_BY_FLUX", {
        downloadId: downloadItem.id,
        reason: (result && result.reason) || "DECLINED_BY_DESKTOP"
      });
      invokeSuggestIfNeeded();
      if (!customTransport && typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.resume === "function" && downloadItem.id !== undefined) {
        try { chrome.downloads.resume(downloadItem.id, () => { if (chrome.runtime) void chrome.runtime.lastError; }); } catch (_) {}
      }
      return { intercepted: false, reason: (result && result.reason) || "DECLINED_BY_DESKTOP", correlationId };
    } catch (err) {
      // Flux unreachable or RPC error: resume native browser download safely
      logPipelineStage(correlationId, "DOWNLOAD_FALLBACK_TO_BROWSER", {
        downloadId: downloadItem.id,
        error: err ? err.message : String(err)
      });
      invokeSuggestIfNeeded();
      if (!customTransport && typeof chrome !== "undefined" && chrome.downloads && typeof chrome.downloads.resume === "function" && downloadItem.id !== undefined) {
        try { chrome.downloads.resume(downloadItem.id, () => { if (chrome.runtime) void chrome.runtime.lastError; }); } catch (_) {}
      }
      return { intercepted: false, fallbackToBrowser: true, error: err.message, correlationId };
    }
  };

  const offerPromise = executeOfferFlow();
  if (downloadItem.id !== undefined && G.inFlightDownloadOffers) {
    G.inFlightDownloadOffers.set(downloadItem.id, offerPromise);
    offerPromise.finally(() => {
      setTimeout(() => {
        if (G.inFlightDownloadOffers) G.inFlightDownloadOffers.delete(downloadItem.id);
      }, 15000);
    });
  }
  return await offerPromise;
}

// 4. Submit a detected video or stream candidate to Flux with explicit 5-way typed source handling (Defect B fix)
async function submitMediaCandidateToFlux(candidate, options = {}, customTransport = null, browserDownloadFn = null) {
  if (!candidate || typeof candidate !== "object") {
    throw new Error("Invalid media candidate");
  }

  const correlationId = options.correlationId || candidate.correlationId || generateCorrelationId("stream");
  const sourceType = candidate.sourceType || classifyMediaSourceType(candidate, candidate.mime, candidate.cachedManifestBody);

  // 5. Unsupported or unresolved media source must fail fast with a clear error
  if (sourceType === SourceType.UNSUPPORTED || candidate.isDrm || candidate.drmProtected) {
    const defaultMsg = (candidate.isDrm || candidate.drmProtected)
      ? "DRM protected streams are unsupported"
      : "Unsupported or unresolved media source";
    const err = new Error(candidate.errorMessage || defaultMsg);
    logPipelineStage(correlationId, "MEDIA_SUBMIT_REJECTED", {
      reason: err.message,
      url: candidate.url || ""
    });
    throw err;
  }

  // Resolve selected video quality (from Popup quality dropdown, Video Overlay dropdown, or options.qualityId)
  const qualitiesList = Array.isArray(candidate.qualities) ? candidate.qualities : [];
  let chosenQuality = options.selectedQuality || null;
  if (!chosenQuality && qualitiesList.length > 0) {
    const reqQ = String(options.qualityId || options.resolution || "").toLowerCase();
    if (reqQ && reqQ !== "best" && reqQ !== "auto") {
      chosenQuality = qualitiesList.find(q =>
        String(q.id || "").toLowerCase() === reqQ ||
        String(q.resolution || "").toLowerCase() === reqQ ||
        String(q.videoFilter || "").toLowerCase() === reqQ
      ) || null;
    }
    if (!chosenQuality) {
      chosenQuality = qualitiesList[0];
    }
  }

  const effectiveVideoUrl = (chosenQuality && chosenQuality.videoUrl)
    ? chosenQuality.videoUrl
    : (candidate.videoUrl || "");
  const effectiveAudioUrl = (chosenQuality && chosenQuality.pairedAudioUrl !== undefined)
    ? chosenQuality.pairedAudioUrl
    : (candidate.pairedAudioUrl || "");
  const effectiveRequiresCipher = (chosenQuality && chosenQuality.requiresCipherExtraction !== undefined)
    ? Boolean(chosenQuality.requiresCipherExtraction)
    : Boolean(candidate.requiresCipherExtraction);
  const effectiveVideoFilter = (options.videoFilter && options.videoFilter !== "best")
    ? options.videoFilter
    : ((chosenQuality && chosenQuality.videoFilter) ? chosenQuality.videoFilter : (options.videoFilter || "best"));
  const effectiveResolution = (chosenQuality && chosenQuality.resolution)
    ? chosenQuality.resolution
    : (options.resolution || candidate.resolution || "");

  // Sanitize cachedManifestBody: ONLY pass actual HLS (#EXTM3U) or DASH (<MPD) manifest text!
  // Never pass a YouTube JSON descriptor inside cachedManifestBody.
  let validCachedManifest = "";
  let extractorJson = candidate.extractorJson || "";
  if (candidate.cachedManifestBody) {
    if (sourceType === SourceType.HLS_MANIFEST && isValidHlsManifestText(candidate.cachedManifestBody)) {
      validCachedManifest = candidate.cachedManifestBody;
    } else if (sourceType === SourceType.DASH_MANIFEST && isValidDashManifestText(candidate.cachedManifestBody)) {
      validCachedManifest = candidate.cachedManifestBody;
    } else if (candidate.cachedManifestBody.trimStart().startsWith("{") && !extractorJson) {
      extractorJson = candidate.cachedManifestBody;
    }
  }

  if (sourceType === SourceType.YOUTUBE_EXTRACTION) {
    let baseDesc = {};
    if (extractorJson) {
      try { baseDesc = JSON.parse(extractorJson) || {}; } catch (_) { baseDesc = {}; }
    }
    const updatedDesc = Object.assign({}, baseDesc, {
      fluxType: "YOUTUBE_STREAM",
      sourceType: SourceType.YOUTUBE_EXTRACTION,
      videoId: candidate.videoId || baseDesc.videoId || extractYouTubeVideoId(candidate.url || candidate.tabUrl || ""),
      title: options.pageTitle || candidate.pageTitle || baseDesc.title || "youtube_video",
      watchUrl: candidate.tabUrl || baseDesc.watchUrl || candidate.url,
      resolution: effectiveResolution || baseDesc.resolution || "Auto",
      videoUrl: effectiveVideoUrl || (candidate.isGoogleVideo ? candidate.url : ""),
      audioUrl: effectiveAudioUrl,
      videoFilter: effectiveVideoFilter,
      requiresCipherExtraction: effectiveRequiresCipher,
      qualities: qualitiesList
    });
    extractorJson = JSON.stringify(updatedDesc);
  }

  let effectiveStreamType = "HLS";
  if (sourceType === SourceType.HLS_MANIFEST) {
    effectiveStreamType = "HLS";
  } else if (sourceType === SourceType.DASH_MANIFEST) {
    effectiveStreamType = "DASH";
  } else if (sourceType === SourceType.YOUTUBE_EXTRACTION) {
    effectiveStreamType = "YOUTUBE";
  } else if (sourceType === SourceType.DIRECT_PROGRESSIVE) {
    effectiveStreamType = "HTTP_FILE";
  }

  const defaultExt = (chosenQuality && chosenQuality.ext)
    ? chosenQuality.ext
    : ((sourceType === SourceType.DIRECT_PROGRESSIVE && candidate.ext) ? candidate.ext : "mp4");

  let targetManifestUrl = candidate.url;
  if (sourceType === SourceType.DIRECT_PROGRESSIVE && effectiveVideoUrl) {
    targetManifestUrl = effectiveVideoUrl;
  }

  let baseTitle = options.pageTitle || candidate.pageTitle || candidate.fileName || "";
  if (options.qualityId && effectiveResolution && qualitiesList.length > 1 && !baseTitle.includes(`[${effectiveResolution}]`)) {
    baseTitle = `${baseTitle} [${effectiveResolution}]`.trim();
  }

  const headers = Object.assign({}, candidate.headers || {});
  if (!headers.Referer && !headers.referer && candidate.tabUrl) {
    headers.Referer = candidate.tabUrl;
  }
  if (!headers.Cookie && !headers.cookie && typeof chrome !== "undefined" && chrome.cookies && typeof chrome.cookies.getAll === "function") {
    try {
      const cookieUrl = candidate.tabUrl || candidate.url || targetManifestUrl || "";
      if (cookieUrl && (cookieUrl.startsWith("http://") || cookieUrl.startsWith("https://"))) {
        const cookieList = await new Promise((resolve) => {
          chrome.cookies.getAll({ url: cookieUrl }, (cList) => {
            if (chrome.runtime) void chrome.runtime.lastError;
            resolve(cList || []);
          });
        });
        if (cookieList && cookieList.length > 0) {
          headers.Cookie = cookieList.map(c => `${c.name}=${c.value}`).join("; ");
        }
      }
    } catch (_) {}
  }

  const params = {
    candidateId: candidate.candidateId || "",
    correlationId,
    streamType: effectiveStreamType,
    sourceType,
    manifestUrl: targetManifestUrl,
    url: targetManifestUrl,
    videoUrl: effectiveVideoUrl,
    pairedAudioUrl: effectiveAudioUrl,
    videoId: candidate.videoId || "",
    resolution: effectiveResolution,
    requiresCipherExtraction: effectiveRequiresCipher,
    extractorJson,
    pageTitle: baseTitle,
    tabUrl: candidate.tabUrl || "",
    outputFormat: (options.audioOnly || (chosenQuality && chosenQuality.isAudioOnly))
      ? (options.outputFormat || (defaultExt === "mp3" || defaultExt === "m4a" ? defaultExt : "m4a"))
      : (options.outputFormat || defaultExt),
    videoFilter: (options.audioOnly || (chosenQuality && chosenQuality.isAudioOnly)) ? "none" : effectiveVideoFilter,
    audioFilter: options.videoOnly ? "none" : (options.audioFilter || (chosenQuality && chosenQuality.isAudioOnly ? "best" : "all")),
    subtitleFilter: options.subtitleFilter || "all",
    customKeyHex: options.customKeyHex || "",
    cachedManifestBody: validCachedManifest || (sourceType === SourceType.YOUTUBE_EXTRACTION ? extractorJson : "") || "",
    headers
  };

  logPipelineStage(correlationId, "MEDIA_SUBMIT_INITIATED", {
    candidateId: params.candidateId,
    streamType: effectiveStreamType,
    sourceType,
    url: candidate.url,
    hasManifestBody: Boolean(validCachedManifest),
    hasExtractorJson: Boolean(extractorJson)
  });

  try {
    const res = await sendNativeRpc("stream.add", params, customTransport);
    candidate.handoffStatus = "Queued in Flux";
    if (res && typeof res === "object") {
      res.taskType = res.taskType || sourceType;
    }
    return res;
  } catch (err) {
    const isDirectFile = sourceType === SourceType.DIRECT_PROGRESSIVE && !isWebpageWatchUrl(candidate.url);
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
  let tabList = G.cacheData.get(tabId) || [];
  const effectiveTabUrl = msg.tabUrl || (sender && sender.tab && sender.tab.url) || G.tabUrlMap.get(tabId) || "";
  const effectiveTitle = (msg.pageTitle || (sender && sender.tab && sender.tab.title) || "video").replace(/\s*-\s*YouTube$/i, "").trim();

  // 1. Explicit candidateId match (e.g. from Popup UI or resolved video overlay)
  if (msg.candidateId) {
    let byId = tabList.find(c => c.candidateId === msg.candidateId);
    if (!byId) {
      for (const [tId, cList] of G.cacheData.entries()) {
        if (Array.isArray(cList)) {
          byId = cList.find(c => c.candidateId === msg.candidateId);
          if (byId) break;
        }
      }
    }
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
    const isHls = normUrl.toLowerCase().includes(".m3u8") || normUrl.toLowerCase().includes(".m3u");
    const isDash = normUrl.toLowerCase().includes(".mpd");
    return {
      candidate: {
        url: normUrl,
        ext: extractUrlExtension(normUrl) || "mp4",
        streamType: isHls ? "HLS" : (isDash ? "DASH" : "HTTP_FILE"),
        sourceType: isHls ? SourceType.HLS_MANIFEST : (isDash ? SourceType.DASH_MANIFEST : SourceType.DIRECT_PROGRESSIVE),
        pageTitle: effectiveTitle,
        tabUrl: effectiveTabUrl,
        headers: effectiveTabUrl ? { Referer: effectiveTabUrl } : {}
      }
    };
  }

  // 3. Fallback candidate search across all cached tabs if current tabList is empty
  if (tabList.length === 0 && effectiveTabUrl) {
    for (const [tId, cList] of G.cacheData.entries()) {
      if (tId !== tabId && Array.isArray(cList)) {
        const matching = cList.filter(c => c.tabUrl === effectiveTabUrl);
        if (matching.length > 0) {
          tabList = matching;
          break;
        }
      }
    }
  }

  // 4. Highest-confidence detected candidate on this tab (prioritizing video / streams over stray audio)
  if (tabList.length > 0) {
    const nonAudio = tabList.filter(c => !c.isAudioOnly);
    const pool = nonAudio.length > 0 ? nonAudio : tabList;
    const streams = pool.filter(c => c.sourceType === SourceType.YOUTUBE_EXTRACTION || c.streamType === "YOUTUBE" || c.streamType === "HLS" || c.streamType === "DASH");
    const best = (streams.length > 0 ? streams[0] : pool[0]);
    if (effectiveTitle && (!best.pageTitle || best.pageTitle === "video")) {
      best.pageTitle = effectiveTitle;
    }
    return { candidate: best };
  }

  // 5. Check if the page reported a specific player error (e.g. YouTube unplayable / private / DRM video)
  if (G.playerErrors && G.playerErrors.has(tabId)) {
    return { error: G.playerErrors.get(tabId) };
  }

  // 6. If on a valid YouTube watch/shorts/embed URL, route via YOUTUBE_EXTRACTION worker
  const targetPageUrl = effectiveTabUrl || msg.frameUrl || msg.url || "";
  if (isYouTubeWatchOrShortsUrl(targetPageUrl)) {
    const videoId = extractYouTubeVideoId(targetPageUrl);
    const ytCand = {
      url: targetPageUrl,
      ext: "mp4",
      streamType: "YOUTUBE",
      sourceType: SourceType.YOUTUBE_EXTRACTION,
      videoId,
      requiresCipherExtraction: true,
      pageTitle: effectiveTitle || "youtube_video",
      tabUrl: targetPageUrl,
      headers: { Referer: targetPageUrl }
    };
    addOrDeduplicateMediaCandidate(tabId, ytCand);
    updateTabBadge(tabId);
    return { candidate: ytCand };
  }

  // 7. Unresolved blob: video with no manifest or network stream
  if (msg.isMseBlob || (msg.blobUrl && msg.blobUrl.startsWith("blob:"))) {
    return {
      error: "No supported download source found for this blob: video player."
    };
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
    ensureSessionRestored().then(async () => {
      const targetTabUrl = msg.tabUrl || G.tabUrlMap.get(tabId) || "";

      // If no candidates are cached yet, probe the active tab content script (or YouTube watch URL)
      let currentList = G.cacheData.get(tabId) || [];
      if (currentList.length === 0 && tabId > 0 && typeof chrome !== "undefined" && chrome.tabs && typeof chrome.tabs.sendMessage === "function") {
        try {
          await new Promise((resolve) => {
            chrome.tabs.sendMessage(tabId, { action: "probePageMedia" }, () => {
              if (chrome.runtime) void chrome.runtime.lastError;
              resolve();
            });
          });
        } catch (_) {}
        currentList = G.cacheData.get(tabId) || [];
      }

      if (currentList.length === 0 && isYouTubeWatchOrShortsUrl(targetTabUrl) && !isSiteDisabled(targetTabUrl) && !(G.playerErrors && G.playerErrors.has(tabId))) {
        addOrDeduplicateMediaCandidate(tabId, {
          url: targetTabUrl,
          ext: "mp4",
          streamType: "YOUTUBE",
          sourceType: SourceType.YOUTUBE_EXTRACTION,
          videoId: extractYouTubeVideoId(targetTabUrl),
          requiresCipherExtraction: true,
          pageTitle: "YouTube Video",
          tabUrl: targetTabUrl,
          headers: { Referer: targetTabUrl }
        });
        updateTabBadge(tabId);
      }

      return sendNativeRpc("system.handshake", { protocolVersion: "1.0" })
        .then((hs) => {
          sendResponse({
            success: true,
            desktopConnected: Boolean(hs && hs.protocolVersion),
            workersAvailable: Boolean(hs && hs.workersAvailable),
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
            workersAvailable: false,
            captureEnabled: G.enableCapture,
            siteDisabled: isSiteDisabled(targetTabUrl),
            playerError: (G.playerErrors && G.playerErrors.get(tabId)) || "",
            candidates: G.cacheData.get(tabId) || []
          });
        });
    });
    return true;
  }

  if (msg.action === "discoverBlobMedia") {
    ensureSessionRestored().then(() => {
      const sourceTabUrl = msg.tabUrl || (sender && sender.tab ? sender.tab.url : "") || G.tabUrlMap.get(tabId) || "";
      const list = G.cacheData.get(tabId) || [];
      if (list.length === 0 && isYouTubeWatchOrShortsUrl(sourceTabUrl) && !isSiteDisabled(sourceTabUrl) && !(G.playerErrors && G.playerErrors.has(tabId))) {
        const cleanTitle = (msg.pageTitle || "youtube_video").replace(/\s*-\s*YouTube$/i, "").trim();
        addOrDeduplicateMediaCandidate(tabId, {
          url: sourceTabUrl,
          ext: "mp4",
          streamType: "YOUTUBE",
          sourceType: SourceType.YOUTUBE_EXTRACTION,
          videoId: extractYouTubeVideoId(sourceTabUrl),
          requiresCipherExtraction: true,
          pageTitle: cleanTitle || "youtube_video",
          tabUrl: sourceTabUrl,
          headers: { Referer: sourceTabUrl }
        });
        updateTabBadge(tabId);
      }
      const updatedList = G.cacheData.get(tabId) || [];
      const best = updatedList.length > 0 ? updatedList[0] : null;
      if (G.blobMediaMap && msg.blobUrl) {
        if (!G.blobMediaMap.has(tabId)) G.blobMediaMap.set(tabId, new Map());
        G.blobMediaMap.get(tabId).set(msg.blobUrl, {
          blobUrl: msg.blobUrl,
          resolvedCandidateId: best ? best.candidateId : "",
          status: best ? "resolved" : "unresolved",
          timestamp: Date.now()
        });
      }
      logPipelineStage(msg.correlationId, "BLOB_MEDIA_DISCOVERY", {
        tabId,
        blobUrl: msg.blobUrl || "",
        hasCandidate: Boolean(best),
        candidateId: best ? best.candidateId : ""
      });
      sendResponse({
        success: true,
        hasCandidate: Boolean(best),
        candidate: best,
        playerError: (G.playerErrors && G.playerErrors.get(tabId)) || ""
      });
    });
    return true;
  }

  if (msg.action === "addMedia") {
    if (!msg.url || (isWebpageWatchUrl(msg.url) && msg.streamType !== "YOUTUBE" && msg.streamType !== "SITE_EXTRACTION" && msg.sourceType !== SourceType.YOUTUBE_EXTRACTION)) {
      sendResponse({ success: false, added: false, error: "Invalid or webpage URL" });
      return true;
    }
    const sourceTabUrl = msg.tabUrl || (sender && sender.tab ? sender.tab.url : "") || G.tabUrlMap.get(tabId) || "";
    if (isSiteDisabled(sourceTabUrl)) {
      sendResponse({ success: false, added: false, error: "Site capture disabled" });
      return true;
    }

    if (G.playerErrors) G.playerErrors.delete(tabId);

    const inferredStreamType = msg.streamType || (msg.url.includes(".m3u8") ? "HLS" : (msg.url.includes(".mpd") ? "DASH" : "HTTP_FILE"));
    const candidate = {
      url: normalizeMediaUrl(msg.url),
      blobUrl: msg.blobUrl || "",
      ext: msg.ext || extractUrlExtension(msg.url) || "mp4",
      streamType: inferredStreamType,
      sourceType: msg.sourceType || classifyMediaSourceType({
        url: msg.url,
        streamType: inferredStreamType,
        mime: msg.mime || "",
        cachedManifestBody: msg.cachedManifestBody || "",
        requiresCipherExtraction: msg.requiresCipherExtraction,
        pairedAudioUrl: msg.pairedAudioUrl
      }),
      mime: msg.mime || "",
      size: msg.size || 0,
      itag: msg.itag || "",
      isAudioOnly: Boolean(msg.isAudioOnly),
      isGoogleVideo: Boolean(msg.isGoogleVideo),
      isAdaptiveTrack: Boolean(msg.isAdaptiveTrack),
      requiresAudioMux: Boolean(msg.requiresAudioMux),
      requiresCipherExtraction: Boolean(msg.requiresCipherExtraction),
      videoId: msg.videoId || extractYouTubeVideoId(msg.url),
      resolution: msg.resolution || "",
      qualities: Array.isArray(msg.qualities) ? msg.qualities : [],
      videoUrl: msg.videoUrl || "",
      pairedAudioUrl: msg.pairedAudioUrl || "",
      extractorJson: msg.extractorJson || "",
      tabId,
      tabUrl: sourceTabUrl,
      pageTitle: (msg.pageTitle || (sender && sender.tab ? sender.tab.title : "video") || "video").replace(/\s*-\s*YouTube$/i, "").trim(),
      cachedManifestBody: msg.cachedManifestBody || "",
      correlationId: msg.correlationId || generateCorrelationId("addmedia"),
      headers: sourceTabUrl ? { Referer: sourceTabUrl } : {}
    };
    const res = addOrDeduplicateMediaCandidate(tabId, candidate);
    if (res && res.candidate && (!res.candidate.qualities || res.candidate.qualities.length === 0)) {
      autoProbeManifestQualities(tabId, res.candidate).catch(() => {});
    }
    updateTabBadge(tabId);
    persistSessionState().catch(() => {});
    notifyPopupMediaUpdated(tabId);
    sendResponse({ success: true, added: res.added, count: (G.cacheData.get(tabId) || []).length, candidate: res.candidate });
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
    ensureSessionRestored().then(async () => {
      const correlationId = msg.correlationId || generateCorrelationId("dlmedia");
      const resolved = resolveCandidateForDownloadRequest(tabId, msg, sender);
      if (resolved.error || !resolved.candidate) {
        logPipelineStage(correlationId, "MEDIA_RESOLVE_FAILED", {
          tabId,
          error: resolved.error || "Media candidate not found"
        });
        sendResponse({ success: false, error: resolved.error || "Media candidate not found", correlationId });
        return;
      }

      // Prevent duplicate task submissions if a download for this candidate/quality is already in flight
      const candIdentifier = resolved.candidate.candidateId || resolved.candidate.url || "";
      const inFlightKey = `${tabId}:${candIdentifier}:${msg.qualityId || ""}`;
      G.inFlightMediaDownloads = G.inFlightMediaDownloads || new Map();
      if (G.inFlightMediaDownloads.has(inFlightKey)) {
        try {
          const prevResult = await G.inFlightMediaDownloads.get(inFlightKey);
          sendResponse({ success: true, result: prevResult, correlationId, deduplicated: true });
        } catch (err) {
          sendResponse({ success: false, error: err.message || "Failed to submit download to Flux", correlationId });
        }
        return;
      }

      const dlPromise = submitMediaCandidateToFlux(resolved.candidate, {
        pageTitle: msg.pageTitle,
        qualityId: msg.qualityId || "",
        resolution: msg.resolution || "",
        videoFilter: msg.videoFilter || "best",
        selectedQuality: msg.selectedQuality || null,
        outputFormat: msg.outputFormat || "",
        correlationId
      });

      G.inFlightMediaDownloads.set(inFlightKey, dlPromise);
      dlPromise.finally(() => {
        setTimeout(() => {
          if (G.inFlightMediaDownloads) G.inFlightMediaDownloads.delete(inFlightKey);
        }, 8000);
      });

      try {
        const result = await dlPromise;
        persistSessionState().catch(() => {});
        notifyPopupMediaUpdated(tabId);
        sendResponse({ success: true, result, correlationId });
      } catch (err) {
        sendResponse({ success: false, error: err.message || "Failed to submit download to Flux", correlationId });
      }
    });
    return true;
  }

  if (msg.action === "clearMedia") {
    G.cacheData.delete(tabId);
    if (G.playerErrors) G.playerErrors.delete(tabId);
    if (G.blobMediaMap) G.blobMediaMap.delete(tabId);
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
    recordBypassIntent(tabId, msg.url || "");
    sendResponse({ success: true });
    return true;
  }

  if (msg.action === "linkClicked") {
    const rec = recordClickedLink(tabId, msg);
    sendResponse({ success: true, correlationId: rec ? rec.correlationId : "" });
    return true;
  }

  if (msg.action === "getDiagnosticLogs") {
    sendResponse({ success: true, logs: G.diagnosticLogs || [] });
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
              if (G.blobMediaMap) G.blobMediaMap.delete(details.tabId);
              updateTabBadge(details.tabId);
            }
          }
        },
        { urls: ["*://*/*"] }
      );
    }

    if (chrome.webRequest.onBeforeRedirect) {
      chrome.webRequest.onBeforeRedirect.addListener(
        (details) => {
          if (details && details.url && details.redirectUrl) {
            recordRedirectHop(details.url, details.redirectUrl, details.requestId, resolveEffectiveTabId(details));
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

  // Downloads API Listeners (Defect C fix: hold suggest() until download.offer completes, avoid race between onCreated and onDeterminingFilename)
  if (chrome.downloads) {
    if (chrome.downloads.onDeterminingFilename) {
      chrome.downloads.onDeterminingFilename.addListener((downloadItem, suggest) => {
        onDownloadCreatedHandler(downloadItem, null, null, suggest, "determine").catch(() => {
          try { if (typeof suggest === "function") suggest(); } catch (_) {}
        });
        return true;
      });
    }
    if (chrome.downloads.onCreated) {
      chrome.downloads.onCreated.addListener((downloadItem) => {
        onDownloadCreatedHandler(downloadItem, null, null, null, "created").catch(() => {});
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
          const prevUrl = G.tabUrlMap.get(tabId) || "";
          G.tabUrlMap.set(tabId, tab.url);
          if (prevUrl && prevUrl.split("#")[0] !== tab.url.split("#")[0]) {
            G.cacheData.delete(tabId);
            if (G.playerErrors) G.playerErrors.delete(tabId);
          }
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
        if (G.clickedLinksByTab) G.clickedLinksByTab.delete(tabId);
        if (G.bypassTabs) G.bypassTabs.delete(tabId);
        if (G.blobMediaMap) G.blobMediaMap.delete(tabId);
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
