// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/function.js) for Flux Download Manager.
// Implements URL/MIME/Regex resource classification, webpage watch URL filtering,
// segment suppression, master/child playlist deduplication, and per-site exclusion checks.

const { G } = (typeof require !== "undefined") ? require("./init.js") : { G: globalThis.G };

function extractHostname(url) {
  try {
    return new URL(url).hostname.toLowerCase();
  } catch (_) {
    return "";
  }
}

function isSiteDisabled(tabUrl) {
  const host = extractHostname(tabUrl);
  if (!host) return false;
  if (G.disabledHosts.has(host)) return true;
  for (const pattern of G.disabledHosts) {
    if (pattern.startsWith("*.") && host.endsWith(pattern.slice(1))) {
      return true;
    }
  }
  return false;
}

function setSiteDisabled(tabUrl, disabled) {
  const host = extractHostname(tabUrl);
  if (!host) return false;
  if (disabled) {
    G.disabledHosts.add(host);
  } else {
    G.disabledHosts.delete(host);
  }
  return true;
}

function extractUrlExtension(url) {
  try {
    const u = new URL(url);
    const pathname = u.pathname;
    const lastDot = pathname.lastIndexOf(".");
    const lastSlash = pathname.lastIndexOf("/");
    if (lastDot > lastSlash && lastDot !== -1) {
      return pathname.slice(lastDot + 1).toLowerCase();
    }
  } catch (_) {}
  return "";
}

function isWebpageWatchUrl(url) {
  if (!url || typeof url !== "string") return false;
  try {
    const u = new URL(url);
    const host = u.hostname.toLowerCase();
    const path = u.pathname.toLowerCase();

    // YouTube watch pages and shorts
    if (host.includes("youtube.com") && (path.includes("/watch") || path.includes("/shorts/"))) return true;
    if (host === "youtu.be") return true;

    // Vimeo watch pages (e.g. vimeo.com/12345678)
    if (host.includes("vimeo.com") && /^\/\d+$/.test(path)) return true;

    // Twitch stream/video pages
    if (host.includes("twitch.tv") && (path.includes("/videos/") || path.split("/").filter(Boolean).length === 1)) {
      if (!path.endsWith(".m3u8") && !path.endsWith(".mpd") && !path.endsWith(".mp4")) return true;
    }

    // Generic HTML pages
    if (path.endsWith(".html") || path.endsWith(".htm") || path.endsWith(".php") || path.endsWith(".asp") || path.endsWith(".aspx")) {
      return true;
    }
  } catch (_) {}
  return false;
}

function classifyUrlIntent(url) {
  if (!url || typeof url !== "string") return "UNKNOWN";
  if (isWebpageWatchUrl(url)) return "WEBPAGE";

  const ext = extractUrlExtension(url);
  if (ext === "m3u8" || ext === "m3u") return "HLS";
  if (ext === "mpd") return "DASH";
  if (ext === "ism") return "MSS";
  if (G.SegmentExts.has(ext)) return "SEGMENT";
  if (G.InterceptFileExts.has(ext)) return "DIRECT_FILE";
  return "UNKNOWN";
}

function parseResponseHeaderMap(responseHeaders) {
  const map = {};
  if (!Array.isArray(responseHeaders)) return map;
  for (const h of responseHeaders) {
    if (h && h.name) {
      map[h.name.toLowerCase()] = h.value || "";
    }
  }
  return map;
}

function isMediaSegmentRequest(url, ext, responseHeaderMap, tabId) {
  if (G.SegmentExts.has(ext)) return true;
  try {
    const u = new URL(url);
    if (/\/(seg|segment|chunk|frag)[-_0-9]+/i.test(u.pathname) && ext !== "m3u8" && ext !== "mpd") {
      return true;
    }
  } catch (_) {}

  // Suppress small (< 4 MiB) 206 byte-range requests if an HLS/DASH manifest already exists on this tab
  const contentRange = responseHeaderMap["content-range"] || "";
  if (contentRange.startsWith("bytes ")) {
    const tabItems = G.cacheData.get(tabId) || [];
    const hasManifest = tabItems.some(item => item.streamType === "HLS" || item.streamType === "DASH");
    const len = parseInt(responseHeaderMap["content-length"] || "0", 10);
    if (hasManifest && len > 0 && len < 4 * 1048576) {
      return true;
    }
  }
  return false;
}

// Core network resource classifier (findMedia logic)
function classifyNetworkResource(details) {
  if (!details || !details.url) return null;
  const url = details.url;
  if (url.startsWith("blob:") || url.startsWith("data:") || url.startsWith("chrome-extension:")) {
    return null;
  }

  // Reject webpage URLs (e.g. YouTube watch page HTML responses)
  if (isWebpageWatchUrl(url)) {
    return null;
  }

  const tabUrl = details.initiator || details.documentUrl || url;
  if (isSiteDisabled(tabUrl)) return null;

  const hmap = parseResponseHeaderMap(details.responseHeaders);
  const contentType = (hmap["content-type"] || "").split(";")[0].trim().toLowerCase();
  const contentLength = parseInt(hmap["content-length"] || "0", 10);
  const ext = extractUrlExtension(url);

  if (isMediaSegmentRequest(url, ext, hmap, details.tabId)) {
    return null;
  }

  // 1. Highest priority: Regex match (Cat Catch G.Regex)
  for (const rule of G.Regex) {
    if (!rule.state) continue;
    const re = new RegExp(rule.regex, rule.type);
    if (re.test(url)) {
      return {
        url,
        ext: rule.ext,
        streamType: rule.streamType,
        mime: contentType || "application/x-mpegurl",
        size: contentLength,
        matchedBy: "regex"
      };
    }
  }

  // 2. MIME Type match (Cat Catch G.Type)
  for (const rule of G.Type) {
    if (!rule.state) continue;
    if (contentType === rule.type) {
      if (rule.size > 0 && contentLength > 0 && contentLength < rule.size) {
        return null;
      }
      return {
        url,
        ext: rule.ext,
        streamType: rule.streamType,
        mime: contentType,
        size: contentLength,
        matchedBy: "mime"
      };
    }
  }

  // 3. Extension match (Cat Catch G.Ext)
  for (const rule of G.Ext) {
    if (!rule.state) continue;
    if (ext === rule.ext) {
      if (rule.size > 0 && contentLength > 0 && contentLength < rule.size) {
        return null;
      }
      return {
        url,
        ext: rule.ext,
        streamType: rule.streamType,
        mime: contentType,
        size: contentLength,
        matchedBy: "ext"
      };
    }
  }

  return null;
}

function getParentUrlPrefix(url) {
  try {
    const u = new URL(url);
    const idx = u.pathname.lastIndexOf("/");
    return idx >= 0 ? (u.origin + u.pathname.slice(0, idx + 1)) : u.origin;
  } catch (_) {
    return "";
  }
}

function isLikelyMasterManifestUrl(url, cachedBody) {
  if (cachedBody && cachedBody.includes("#EXT-X-STREAM-INF")) return true;
  return /(master|manifest|playlist|index)\.m3u8/i.test(url) && !/(1080|720|480|360|240|audio|sub|chunklist)/i.test(url);
}

// Adds a candidate to G.cacheData[tabId] with duplicate suppression and Master/Child HLS grouping
function addOrDeduplicateMediaCandidate(tabId, candidate) {
  if (!G.cacheData.has(tabId)) {
    G.cacheData.set(tabId, []);
  }
  const list = G.cacheData.get(tabId);

  // Exact URL deduplication
  const existing = list.find(item => item.url === candidate.url);
  if (existing) {
    if (candidate.cachedManifestBody && !existing.cachedManifestBody) {
      existing.cachedManifestBody = candidate.cachedManifestBody;
    }
    if (candidate.headers && Object.keys(candidate.headers).length > 0) {
      existing.headers = Object.assign({}, existing.headers, candidate.headers);
    }
    return { added: false, candidate: existing };
  }

  candidate.candidateId = candidate.candidateId || ("cand-" + Math.random().toString(36).slice(2, 10));
  candidate.timestamp = Date.now();
  candidate.variants = candidate.variants || [];

  if (candidate.streamType === "HLS") {
    const isMaster = isLikelyMasterManifestUrl(candidate.url, candidate.cachedManifestBody);
    candidate.isMaster = isMaster;
    candidate.confidence = isMaster ? 95 : 80;
    const prefix = getParentUrlPrefix(candidate.url);

    if (!isMaster) {
      // Check if a Master playlist in the same directory tree was captured
      const parentMaster = list.find(item =>
        item.streamType === "HLS" &&
        item.isMaster &&
        prefix.startsWith(getParentUrlPrefix(item.url))
      );
      if (parentMaster) {
        parentMaster.variants.push(candidate);
        return { added: false, groupedUnder: parentMaster.candidateId, candidate: parentMaster };
      }
    } else {
      // Demote any previously added child playlists under the same directory tree into variants of this Master
      for (let i = list.length - 1; i >= 0; --i) {
        const item = list[i];
        if (item.streamType === "HLS" && !item.isMaster && getParentUrlPrefix(item.url).startsWith(prefix)) {
          candidate.variants.push(item);
          list.splice(i, 1);
        }
      }
    }
  } else if (candidate.streamType === "DASH") {
    candidate.isMaster = true;
    candidate.confidence = 95;
  } else {
    candidate.confidence = 90;
  }

  list.push(candidate);
  list.sort((a, b) => (b.confidence || 0) - (a.confidence || 0));
  return { added: true, candidate };
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    extractHostname,
    isSiteDisabled,
    setSiteDisabled,
    extractUrlExtension,
    isWebpageWatchUrl,
    classifyUrlIntent,
    classifyNetworkResource,
    addOrDeduplicateMediaCandidate
  };
}
