// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/function.js) for Flux Download Manager.
// Implements URL/MIME/Regex resource classification, webpage watch URL filtering,
// Content-Range & Content-Disposition parsing, googlevideo.com normalization,
// segment suppression, master/child playlist deduplication, and per-site exclusion checks.

if (typeof globalThis.G === "undefined" && typeof importScripts === "function") {
  try {
    importScripts("init.js");
  } catch (_) {
    try { importScripts("/js/init.js"); } catch (_) {}
  }
}

var G = (typeof require !== "undefined") ? require("./init.js").G : globalThis.G;

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
  if (!url || typeof url !== "string") return "";
  try {
    const u = new URL(url, "https://example.invalid");
    const pathname = u.pathname || "";
    const lastDot = pathname.lastIndexOf(".");
    const lastSlash = pathname.lastIndexOf("/");
    if (lastDot > lastSlash && lastDot !== -1) {
      return pathname.slice(lastDot + 1).toLowerCase();
    }
  } catch (_) {
    const clean = url.split("?")[0].split("#")[0];
    const lastDot = clean.lastIndexOf(".");
    const lastSlash = Math.max(clean.lastIndexOf("/"), clean.lastIndexOf("\\"));
    if (lastDot > lastSlash && lastDot !== -1) {
      return clean.slice(lastDot + 1).toLowerCase();
    }
  }
  return "";
}

function extractFilenameFromContentDisposition(cdHeader) {
  if (!cdHeader || typeof cdHeader !== "string") return "";
  const starMatch = cdHeader.match(/filename\*\s*=\s*(?:UTF-8''|utf-8'')?([^;\r\n]+)/i);
  if (starMatch && starMatch[1]) {
    let raw = starMatch[1].trim().replace(/^["']|["']$/g, "");
    try { return decodeURIComponent(raw); } catch (_) { return raw; }
  }
  const fnMatch = cdHeader.match(/filename\s*=\s*(?:"([^"]+)"|([^;\r\n]+))/i);
  if (fnMatch) {
    let raw = (fnMatch[1] || fnMatch[2] || "").trim().replace(/^["']|["']$/g, "");
    try { return decodeURIComponent(raw); } catch (_) { return raw; }
  }
  return "";
}

function isYouTubeWatchOrShortsUrl(url) {
  if (!url || typeof url !== "string") return false;
  try {
    const u = new URL(url);
    const host = u.hostname.toLowerCase();
    const path = u.pathname.toLowerCase();
    if ((host === "youtube.com" || host.endsWith(".youtube.com")) &&
        (path === "/watch" || path.startsWith("/watch/") || path.startsWith("/shorts/") || path.startsWith("/embed/") || path.startsWith("/live/"))) {
      if (path === "/watch" && !u.searchParams.get("v")) return false;
      return true;
    }
    if (host === "youtu.be" && path.length > 1) return true;
  } catch (_) {}
  return false;
}

function isWebpageWatchUrl(url) {
  if (!url || typeof url !== "string") return false;
  try {
    const u = new URL(url);
    const host = u.hostname.toLowerCase();
    const path = u.pathname.toLowerCase();

    // YouTube watch pages and shorts
    if (host.includes("youtube.com") && (path.includes("/watch") || path.includes("/shorts/") || path.includes("/embed/") || path.includes("/live/"))) return true;
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

function parseTotalSizeFromHeaders(hmap, url) {
  const contentRange = hmap["content-range"] || "";
  if (contentRange) {
    const m = contentRange.match(/bytes\s+\d+-\d+\/(\d+)/i);
    if (m && m[1]) {
      const total = parseInt(m[1], 10);
      if (total > 0) return total;
    }
  }
  try {
    const u = new URL(url);
    const clen = u.searchParams.get("clen");
    if (clen) {
      const n = parseInt(clen, 10);
      if (n > 0) return n;
    }
  } catch (_) {}
  const cl = parseInt(hmap["content-length"] || "0", 10);
  return isNaN(cl) ? 0 : cl;
}

function normalizeMediaUrl(url) {
  if (!url || typeof url !== "string") return url;
  try {
    const u = new URL(url);
    if (u.hostname.toLowerCase().includes("googlevideo.com") || u.pathname.includes("/videoplayback")) {
      // Strip per-chunk MSE range/buffer parameters so signed googlevideo URLs point to the full representation
      const stripParams = ["range", "rn", "rbuf", "ump", "srfvp", "alr", "cpn"];
      for (const p of stripParams) {
        u.searchParams.delete(p);
      }
      return u.toString();
    }
  } catch (_) {}
  return url;
}

function isMediaSegmentRequest(url, ext, responseHeaderMap, tabId) {
  if (G.SegmentExts.has(ext)) return true;
  try {
    const u = new URL(url);
    if (/\/(seg|segment|chunk|frag)[-_0-9]+/i.test(u.pathname) && ext !== "m3u8" && ext !== "mpd") {
      return true;
    }
    // Suppress init segments when an HLS/DASH manifest is active
    if (/\/(init|header)[-_0-9]*\.(mp4|m4s|cmfv|cmfa)$/i.test(u.pathname)) {
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
  const rawUrl = details.url;
  if (rawUrl.startsWith("blob:") || rawUrl.startsWith("data:") || rawUrl.startsWith("chrome-extension:")) {
    return null;
  }

  if (typeof details.statusCode === "number" &&
      details.statusCode !== 200 && details.statusCode !== 206 && details.statusCode !== 304) {
    return null;
  }

  // Reject webpage URLs (e.g. YouTube watch page HTML responses)
  if (isWebpageWatchUrl(rawUrl)) {
    return null;
  }

  const tabUrl = details.initiator || details.documentUrl || rawUrl;
  if (isSiteDisabled(tabUrl)) return null;

  const hmap = parseResponseHeaderMap(details.responseHeaders);
  const contentType = (hmap["content-type"] || "").split(";")[0].trim().toLowerCase();
  if (contentType === "text/html" || contentType === "text/css" || contentType === "application/javascript") {
    return null;
  }

  const totalSize = parseTotalSizeFromHeaders(hmap, rawUrl);
  const cdFilename = extractFilenameFromContentDisposition(hmap["content-disposition"] || "");
  const url = normalizeMediaUrl(rawUrl);
  let ext = extractUrlExtension(cdFilename) || extractUrlExtension(url);

  if (isMediaSegmentRequest(url, ext, hmap, details.tabId)) {
    return null;
  }

  // Special handling for googlevideo.com / videoplayback streams (IDM FUN_00592d50 pattern)
  try {
    const parsedU = new URL(url);
    if (parsedU.hostname.toLowerCase().includes("googlevideo.com") || parsedU.pathname.includes("/videoplayback")) {
      const mimeParam = (parsedU.searchParams.get("mime") || contentType || "").toLowerCase();
      const itag = parsedU.searchParams.get("itag") || "";
      if (mimeParam.startsWith("video/") || mimeParam.startsWith("audio/")) {
        const isAudioOnly = mimeParam.startsWith("audio/");
        const resolvedExt = mimeParam.includes("webm") ? "webm" : (isAudioOnly ? "m4a" : "mp4");
        return {
          url,
          ext: resolvedExt,
          streamType: "HTTP_FILE",
          mime: mimeParam,
          size: totalSize,
          itag,
          isAudioOnly,
          isGoogleVideo: true,
          matchedBy: "googlevideo"
        };
      }
    }
  } catch (_) {}

  // 1. Highest priority: Regex match (Cat Catch G.Regex)
  for (const rule of G.Regex) {
    if (!rule.state) continue;
    const re = new RegExp(rule.regex, rule.type.replace("g", ""));
    if (re.test(url)) {
      return {
        url,
        ext: rule.ext,
        streamType: rule.streamType,
        mime: contentType || "application/x-mpegurl",
        size: totalSize,
        matchedBy: "regex"
      };
    }
  }

  // 2. MIME Type match (Cat Catch G.Type)
  for (const rule of G.Type) {
    if (!rule.state) continue;
    if (contentType === rule.type) {
      if (rule.size > 0 && totalSize > 0 && totalSize < rule.size) {
        return null;
      }
      return {
        url,
        ext: ext || rule.ext,
        streamType: rule.streamType,
        mime: contentType,
        size: totalSize,
        fileName: cdFilename || "",
        matchedBy: "mime"
      };
    }
  }

  // 3. Extension match (Cat Catch G.Ext)
  for (const rule of G.Ext) {
    if (!rule.state) continue;
    if (ext === rule.ext) {
      if (rule.size > 0 && totalSize > 0 && totalSize < rule.size) {
        return null;
      }
      return {
        url,
        ext: rule.ext,
        streamType: rule.streamType,
        mime: contentType,
        size: totalSize,
        fileName: cdFilename || "",
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

// Adds a candidate to G.cacheData[tabId] with duplicate suppression and Master/Child HLS + YouTube grouping
function addOrDeduplicateMediaCandidate(tabId, candidate) {
  if (!G.cacheData.has(tabId)) {
    G.cacheData.set(tabId, []);
  }
  const list = G.cacheData.get(tabId);

  if (candidate.url) {
    candidate.url = normalizeMediaUrl(candidate.url);
  }

  // If this is an audio-only googlevideo stream and we already have a YouTube/video candidate on this tab,
  // attach the audio stream to the existing video candidate rather than cluttering the primary list
  if (candidate.isGoogleVideo && candidate.isAudioOnly) {
    const videoCand = list.find(item => item.streamType === "YOUTUBE" || (item.isGoogleVideo && !item.isAudioOnly));
    if (videoCand) {
      videoCand.pairedAudioUrl = candidate.url;
      return { added: false, groupedUnder: videoCand.candidateId, candidate: videoCand };
    }
  }

  // Exact URL deduplication (or matching YouTube videoId / googlevideo itag)
  const existing = list.find(item => {
    if (item.url === candidate.url) return true;
    if (item.streamType === "YOUTUBE" && candidate.streamType === "YOUTUBE" &&
        item.videoId && candidate.videoId && item.videoId === candidate.videoId) {
      return true;
    }
    if (item.isGoogleVideo && candidate.isGoogleVideo &&
        item.itag && candidate.itag && item.itag === candidate.itag) {
      return true;
    }
    return false;
  });

  if (existing) {
    if (candidate.cachedManifestBody && !existing.cachedManifestBody) {
      existing.cachedManifestBody = candidate.cachedManifestBody;
    }
    if (candidate.resolution && !existing.resolution) {
      existing.resolution = candidate.resolution;
    }
    if (candidate.size && (!existing.size || candidate.size > existing.size)) {
      existing.size = candidate.size;
    }
    if (candidate.pageTitle && (!existing.pageTitle || existing.pageTitle === "video")) {
      existing.pageTitle = candidate.pageTitle;
    }
    if (candidate.pairedAudioUrl && !existing.pairedAudioUrl) {
      existing.pairedAudioUrl = candidate.pairedAudioUrl;
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
  } else if (candidate.streamType === "YOUTUBE" || candidate.streamType === "SITE_EXTRACTION") {
    candidate.isMaster = true;
    candidate.confidence = 96;
  } else {
    candidate.confidence = 90;
  }

  list.push(candidate);
  list.sort((a, b) => (b.confidence || 0) - (a.confidence || 0));
  return { added: true, candidate };
}

var exportedFns = {
  extractHostname,
  isSiteDisabled,
  setSiteDisabled,
  extractUrlExtension,
  extractFilenameFromContentDisposition,
  isYouTubeWatchOrShortsUrl,
  isWebpageWatchUrl,
  classifyUrlIntent,
  parseTotalSizeFromHeaders,
  normalizeMediaUrl,
  classifyNetworkResource,
  addOrDeduplicateMediaCandidate
};

Object.assign(globalThis, exportedFns);

if (typeof module !== "undefined" && module.exports) {
  module.exports = exportedFns;
}
