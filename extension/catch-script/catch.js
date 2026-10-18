// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (catch-script/search.js & catch.js) for Flux Download Manager.
// Runs in page MAIN world at document_start. Hooks window.fetch, window.XMLHttpRequest,
// window.URL.createObjectURL, MediaSource, HTMLAnchorElement.prototype.click, and inspects
// YouTube player responses (ytInitialPlayerResponse / #movie_player) with explicit typed
// descriptors (never passing JSON as an HLS/DASH manifest).

(function () {
  function installDeepSearchHooks(win, sessionNonce, emitEvent) {
    if (!win || win.__fluxCatchScriptInstalled) return;
    win.__fluxCatchScriptInstalled = true;

    const seenUrls = new Set();
    const blobRegistry = new Map();
    let lastInspectedYtVideoId = "";

    function makeCorrelationId(prefix) {
      return `${prefix || "catch"}-${Date.now().toString(36)}-${Math.random().toString(36).slice(2, 7)}`;
    }

    function postToExtension(payload) {
      if (typeof emitEvent === "function") {
        emitEvent(Object.assign({ nonce: sessionNonce }, payload));
      } else if (typeof win.postMessage === "function") {
        try {
          win.postMessage(Object.assign({ nonce: sessionNonce }, payload), "*");
        } catch (_) {}
      }
    }

    function sendMedia(data) {
      if (!data || !data.url) return;
      const dedupKey = (data.streamType || "") + "|" + (data.sourceType || "") + "|" + data.url + "|" + (data.videoUrl || "");
      if (seenUrls.has(dedupKey) && !data.cachedManifestBody && !data.extractorJson) return;
      seenUrls.add(dedupKey);
      postToExtension(Object.assign({
        action: "catCatchAddMedia",
        correlationId: data.correlationId || makeCorrelationId("media")
      }, data));
    }

    function sendKey(key, url) {
      if (!key) return;
      postToExtension({ action: "catCatchAddKey", key, url });
    }

    function sendPlayerError(reason, url) {
      if (!reason) return;
      postToExtension({ action: "catCatchPlayerError", reason, url });
    }

    function normalizeGooglevideoUrl(rawUrl) {
      try {
        const u = new URL(rawUrl, win.location ? win.location.href : "https://www.youtube.com");
        for (const p of ["range", "rn", "rbuf", "ump", "srfvp", "alr", "cpn"]) {
          u.searchParams.delete(p);
        }
        return u.toString();
      } catch (_) {
        return rawUrl;
      }
    }

    const PROGRESSIVE_YOUTUBE_ITAGS = new Set(["18", "22", "37", "43", "59", "78"]);

    function inspectYouTubePlayerObject(playerResp, pageUrl) {
      if (!playerResp || typeof playerResp !== "object") return false;
      const playStatus = playerResp.playabilityStatus || {};
      const videoDetails = playerResp.videoDetails || {};
      const streamingData = playerResp.streamingData || null;

      if (playStatus.status && playStatus.status !== "OK" && !streamingData) {
        const reason = playStatus.reason || ("YouTube stream unavailable (" + playStatus.status + ")");
        sendPlayerError(reason, pageUrl || (win.location ? win.location.href : ""));
        return false;
      }

      if (!streamingData) return false;

      const videoId = videoDetails.videoId || "";
      const rawTitle = videoDetails.title || (win.document ? win.document.title : "") || "youtube_video";
      const cleanTitle = rawTitle.replace(/\s*-\s*YouTube$/i, "").trim() || "youtube_video";
      const canonicalWatchUrl = videoId
        ? ("https://www.youtube.com/watch?v=" + videoId)
        : (pageUrl || (win.location ? win.location.href : ""));

      // 1. Check if actual HLS or DASH manifests are directly exposed by YouTube
      if (streamingData.hlsManifestUrl) {
        sendMedia({
          streamType: "HLS",
          sourceType: "HLS_MANIFEST",
          ext: "m3u8",
          url: streamingData.hlsManifestUrl,
          pageTitle: cleanTitle,
          referer: canonicalWatchUrl
        });
      }
      if (streamingData.dashManifestUrl) {
        sendMedia({
          streamType: "DASH",
          sourceType: "DASH_MANIFEST",
          ext: "mpd",
          url: streamingData.dashManifestUrl,
          pageTitle: cleanTitle,
          referer: canonicalWatchUrl
        });
      }

      // 2. Inspect progressive formats (combined audio + video) and adaptiveFormats (separate video & audio)
      const formats = Array.isArray(streamingData.formats) ? streamingData.formats : [];
      const adaptive = Array.isArray(streamingData.adaptiveFormats) ? streamingData.adaptiveFormats : [];
      const allFormats = formats.concat(adaptive);

      // Check if all formats are DRM protected
      if (allFormats.length > 0 && allFormats.every(f => f && (f.drmFamilies || (f.signatureCipher && String(f.signatureCipher).includes("drm"))))) {
        sendPlayerError("YouTube stream is DRM protected (Unsupported)", canonicalWatchUrl);
        return false;
      }

      let bestProgressive = null;
      for (const f of formats) {
        if (f && f.url && !f.drmFamilies) {
          const h = Number(f.height || 0);
          if (!bestProgressive || h > Number(bestProgressive.height || 0)) {
            bestProgressive = f;
          }
        }
      }

      let bestVideo = null;
      let bestAudio = null;
      let bestAudioMp4 = null;
      for (const f of adaptive) {
        if (!f || f.drmFamilies) continue;
        const mime = String(f.mimeType || "").toLowerCase();
        const bw = Number(f.bitrate || f.averageBitrate || 0);
        if (mime.startsWith("video/") && f.url) {
          if (!bestVideo || bw > Number(bestVideo.bitrate || bestVideo.averageBitrate || 0)) {
            bestVideo = f;
          }
        } else if (mime.startsWith("audio/") && f.url) {
          if (!bestAudio || bw > Number(bestAudio.bitrate || bestAudio.averageBitrate || 0)) {
            bestAudio = f;
          }
          if (mime.includes("mp4") && (!bestAudioMp4 || bw > Number(bestAudioMp4.bitrate || bestAudioMp4.averageBitrate || 0))) {
            bestAudioMp4 = f;
          }
        }
      }
      if (bestAudioMp4) bestAudio = bestAudioMp4;

      // Build complete multi-quality list across all available adaptive and progressive resolutions
      const qualityMap = new Map();
      const audioByteLen = bestAudio ? parseInt(bestAudio.contentLength || "0", 10) : 0;
      const cleanBestAudioUrl = (bestAudio && bestAudio.url) ? normalizeGooglevideoUrl(bestAudio.url) : "";

      const addVideoFormatToQualities = (f, isAdaptiveFmt) => {
        if (!f || f.drmFamilies) return;
        const mime = String(f.mimeType || "").toLowerCase();
        if (mime.startsWith("audio/")) return;
        const height = Number(f.height || 0);
        const width = Number(f.width || 0);
        const fps = Number(f.fps || 30);
        const bw = Number(f.bitrate || f.averageBitrate || 0);
        const resLabel = f.qualityLabel || (height > 0 ? `${height}p` : "HD");
        const key = resLabel.toLowerCase();
        const hasDirectUrl = Boolean(f.url);
        const cleanVUrl = hasDirectUrl ? normalizeGooglevideoUrl(f.url) : "";
        const needsMux = Boolean(isAdaptiveFmt && cleanBestAudioUrl);
        const unpairedAdaptive = Boolean(isAdaptiveFmt && hasDirectUrl && !cleanBestAudioUrl);
        const requiresCipher = Boolean(!hasDirectUrl || unpairedAdaptive);
        const vSize = parseInt(f.contentLength || "0", 10);
        const totalSize = vSize > 0 ? (vSize + (needsMux ? audioByteLen : 0)) : 0;
        const isMp4 = mime.includes("mp4");

        const entry = {
          id: key,
          label: `${resLabel} (${isMp4 ? "MP4" : "WebM"})`,
          resolution: resLabel,
          width,
          height,
          fps,
          itag: f.itag || 0,
          bandwidth: bw,
          size: totalSize,
          ext: "mp4",
          mime: mime.split(";")[0] || "video/mp4",
          videoUrl: unpairedAdaptive ? "" : cleanVUrl,
          pairedAudioUrl: needsMux ? cleanBestAudioUrl : "",
          requiresAudioMux: needsMux,
          requiresCipherExtraction: requiresCipher,
          videoFilter: height > 0 ? `${height}p` : resLabel
        };

        const existing = qualityMap.get(key);
        if (!existing) {
          qualityMap.set(key, entry);
        } else {
          // Prefer direct URL over cipher, and MP4 (avc1) over WebM at the same resolution
          const existingDirect = Boolean(existing.videoUrl);
          if ((hasDirectUrl && !existingDirect) ||
              (hasDirectUrl === existingDirect && isMp4 && !existing.mime.includes("mp4")) ||
              (hasDirectUrl === existingDirect && isMp4 === existing.mime.includes("mp4") && bw > existing.bandwidth)) {
            qualityMap.set(key, entry);
          }
        }
      };

      for (const f of adaptive) addVideoFormatToQualities(f, true);
      for (const f of formats) addVideoFormatToQualities(f, false);

      const qualities = Array.from(qualityMap.values()).sort((a, b) => {
        if ((b.height || 0) !== (a.height || 0)) return (b.height || 0) - (a.height || 0);
        return (b.bandwidth || 0) - (a.bandwidth || 0);
      });

      if (bestAudio && cleanBestAudioUrl) {
        const audioBwKbps = bestAudio.bitrate ? `${Math.round(Number(bestAudio.bitrate) / 1000)} Kbps` : "M4A";
        qualities.push({
          id: "audio-only",
          label: `Audio Only (${audioBwKbps})`,
          resolution: "Audio",
          width: 0,
          height: 0,
          itag: bestAudio.itag || 140,
          bandwidth: Number(bestAudio.bitrate || 128000),
          size: audioByteLen,
          isAudioOnly: true,
          ext: "m4a",
          mime: String(bestAudio.mimeType || "audio/mp4").split(";")[0],
          videoUrl: cleanBestAudioUrl,
          pairedAudioUrl: "",
          requiresAudioMux: false,
          requiresCipherExtraction: false,
          videoFilter: "audio"
        });
      }

      // Adaptive video tracks on YouTube have no audio; only use adaptive bestVideo if bestAudio is also available,
      // otherwise prefer bestProgressive or mark requiresCipherExtraction so yt-dlp extracts & merges both tracks.
      const canMuxAdaptive = Boolean(bestVideo && bestVideo.url && bestAudio && bestAudio.url);
      const chosenVideo = canMuxAdaptive ? bestVideo : (bestProgressive || bestVideo);

      if (chosenVideo && chosenVideo.url) {
        const cleanVideoUrl = normalizeGooglevideoUrl(chosenVideo.url);
        const cleanAudioUrl = canMuxAdaptive ? normalizeGooglevideoUrl(bestAudio.url) : "";
        const isUnpairedAdaptiveVideo = Boolean(chosenVideo === bestVideo && !cleanAudioUrl);
        const resolution = chosenVideo.qualityLabel ||
                           (chosenVideo.width && chosenVideo.height ? `${chosenVideo.width}x${chosenVideo.height}` : "HD");
        const sizeBytes = parseInt(chosenVideo.contentLength || "0", 10) +
                          (canMuxAdaptive ? parseInt(bestAudio.contentLength || "0", 10) : 0);

        const descriptor = {
          fluxType: "YOUTUBE_STREAM",
          sourceType: "YOUTUBE_EXTRACTION",
          videoId,
          title: cleanTitle,
          watchUrl: canonicalWatchUrl,
          resolution,
          videoUrl: isUnpairedAdaptiveVideo ? "" : cleanVideoUrl,
          audioUrl: cleanAudioUrl,
          requiresAudioMux: canMuxAdaptive,
          requiresCipherExtraction: isUnpairedAdaptiveVideo,
          videoMime: String(chosenVideo.mimeType || "video/mp4").split(";")[0],
          audioMime: (canMuxAdaptive && bestAudio) ? String(bestAudio.mimeType || "audio/mp4").split(";")[0] : "",
          hlsManifestUrl: streamingData.hlsManifestUrl || "",
          dashManifestUrl: streamingData.dashManifestUrl || "",
          size: sizeBytes > 0 ? sizeBytes : 0,
          qualities
        };

        lastInspectedYtVideoId = videoId || canonicalWatchUrl;
        sendMedia({
          streamType: "YOUTUBE",
          sourceType: "YOUTUBE_EXTRACTION",
          ext: "mp4",
          url: canonicalWatchUrl,
          videoId,
          pageTitle: cleanTitle,
          resolution,
          size: sizeBytes > 0 ? sizeBytes : 0,
          videoUrl: isUnpairedAdaptiveVideo ? "" : cleanVideoUrl,
          pairedAudioUrl: cleanAudioUrl,
          requiresAudioMux: canMuxAdaptive,
          requiresCipherExtraction: isUnpairedAdaptiveVideo,
          qualities,
          referer: canonicalWatchUrl,
          extractorJson: JSON.stringify(descriptor),
          cachedManifestBody: "" // Never pass YouTube JSON as an HLS/DASH manifest
        });
        return true;
      }

      // If formats use signatureCipher / cipher or serverAbrStreamingUrl, register the YouTube extraction target for yt-dlp
      if (formats.length > 0 || adaptive.length > 0 || streamingData.serverAbrStreamingUrl) {
        const topFmt = qualities[0] || adaptive[0] || formats[0];
        const resolution = (topFmt && (topFmt.resolution || topFmt.qualityLabel)) || "Auto";
        const descriptor = {
          fluxType: "YOUTUBE_STREAM",
          sourceType: "YOUTUBE_EXTRACTION",
          videoId,
          title: cleanTitle,
          watchUrl: canonicalWatchUrl,
          resolution,
          requiresCipherExtraction: true,
          qualities
        };
        lastInspectedYtVideoId = videoId || canonicalWatchUrl;
        sendMedia({
          streamType: "YOUTUBE",
          sourceType: "YOUTUBE_EXTRACTION",
          ext: "mp4",
          url: canonicalWatchUrl,
          videoId,
          pageTitle: cleanTitle,
          resolution,
          requiresCipherExtraction: true,
          qualities,
          referer: canonicalWatchUrl,
          extractorJson: JSON.stringify(descriptor),
          cachedManifestBody: ""
        });
        return true;
      }

      return false;
    }

    function scanPageForYouTubeData() {
      try {
        if (!win.location || !win.location.hostname) return false;
        const host = win.location.hostname.toLowerCase();
        if (!host.includes("youtube.com") && host !== "youtu.be") return false;

        // 1. Check #movie_player API (most up-to-date during SPA navigation)
        if (win.document && typeof win.document.getElementById === "function") {
          const playerEl = win.document.getElementById("movie_player");
          if (playerEl && typeof playerEl.getPlayerResponse === "function") {
            const pr = playerEl.getPlayerResponse();
            if (pr && inspectYouTubePlayerObject(pr, win.location.href)) {
              return true;
            }
          }
        }

        // 2. Check window.ytInitialPlayerResponse
        if (win.ytInitialPlayerResponse) {
          if (inspectYouTubePlayerObject(win.ytInitialPlayerResponse, win.location.href)) {
            return true;
          }
        }

        // 3. Fallback: if on a valid YouTube watch/shorts/embed URL and #movie_player or <video> exists,
        // register a site extraction candidate so the popup and overlay can route via yt-dlp
        const href = win.location.href || "";
        const u = new URL(href);
        const path = u.pathname.toLowerCase();
        const isWatchPage = (path === "/watch" && u.searchParams.get("v")) ||
                            path.startsWith("/shorts/") ||
                            path.startsWith("/embed/") ||
                            path.startsWith("/live/") ||
                            (host === "youtu.be" && path.length > 1);
        if (isWatchPage && win.document && win.document.querySelector && win.document.querySelector("video")) {
          const vid = u.searchParams.get("v") || path.split("/").filter(Boolean).pop() || "";
          const rawTitle = (win.document.title || "youtube_video").replace(/\s*-\s*YouTube$/i, "").trim() || "youtube_video";
          const descriptor = {
            fluxType: "YOUTUBE_STREAM",
            sourceType: "YOUTUBE_EXTRACTION",
            videoId: vid,
            title: rawTitle,
            watchUrl: href,
            resolution: "Auto",
            requiresCipherExtraction: true
          };
          sendMedia({
            streamType: "YOUTUBE",
            sourceType: "YOUTUBE_EXTRACTION",
            ext: "mp4",
            url: href,
            videoId: vid,
            pageTitle: rawTitle,
            resolution: "Auto",
            requiresCipherExtraction: true,
            referer: href,
            extractorJson: JSON.stringify(descriptor),
            cachedManifestBody: ""
          });
          return true;
        }
      } catch (_) {}
      return false;
    }

    function inspectManifestText(url, text, referer) {
      if (typeof text !== "string" || !url) return false;
      const trimmed = text.trimStart();
      const effectiveUrl = url.startsWith("blob:")
        ? (referer || (win.location ? win.location.href : url))
        : url;

      if (trimmed.startsWith("#EXTM3U") || trimmed.includes("#EXT-X-STREAM-INF") || trimmed.includes("#EXT-X-TARGETDURATION")) {
        const hlsQualities = [];
        const lines = text.split(/\r?\n/);
        for (let i = 0; i < lines.length; ++i) {
          const ln = lines[i].trim();
          if (!ln.startsWith("#EXT-X-STREAM-INF:")) continue;
          const bwM = ln.match(/BANDWIDTH=(\d+)/i);
          const resM = ln.match(/RESOLUTION=(\d+)x(\d+)/i);
          let vUri = "";
          for (let j = i + 1; j < lines.length; ++j) {
            const c = lines[j].trim();
            if (c && !c.startsWith("#")) { vUri = c; i = j; break; }
          }
          if (!vUri) continue;
          let resolvedVUri = vUri;
          try { resolvedVUri = new URL(vUri, effectiveUrl).href; } catch (_) {}
          const w = resM ? parseInt(resM[1], 10) : 0;
          const h = resM ? parseInt(resM[2], 10) : 0;
          const bw = bwM ? parseInt(bwM[1], 10) : 0;
          const resLabel = h > 0 ? `${h}p` : "Auto";
          const bwStr = bw >= 1000000 ? `${(bw / 1000000).toFixed(1)} Mbps` : (bw >= 1000 ? `${Math.round(bw / 1000)} Kbps` : "");
          hlsQualities.push({
            id: h > 0 ? `${h}p` : `hls-${hlsQualities.length}`,
            label: `${resLabel}${bwStr ? " (" + bwStr + ")" : ""}`,
            resolution: resLabel,
            width: w,
            height: h,
            bandwidth: bw,
            ext: "mp4",
            variantUrl: resolvedVUri,
            videoFilter: h > 0 ? `${h}p` : "best"
          });
        }
        hlsQualities.sort((a, b) => ((b.height || 0) - (a.height || 0)) || ((b.bandwidth || 0) - (a.bandwidth || 0)));
        sendMedia({
          streamType: "HLS",
          sourceType: "HLS_MANIFEST",
          ext: "m3u8",
          url: effectiveUrl,
          blobUrl: url.startsWith("blob:") ? url : "",
          resolution: hlsQualities.length > 0 ? hlsQualities[0].resolution : "",
          qualities: hlsQualities,
          referer: referer || (win.location ? win.location.href : ""),
          cachedManifestBody: text
        });

        // Parse AES-128 key URI if present: #EXT-X-KEY:METHOD=AES-128,URI="key_url"
        const keyMatch = text.match(/#EXT-X-KEY:METHOD=([^,\r\n]+)(?:,URI="([^"\r\n]+)")?/i);
        if (keyMatch && keyMatch[2]) {
          try {
            const resolvedKeyUrl = new URL(keyMatch[2], effectiveUrl).href;
            sendKey(resolvedKeyUrl, effectiveUrl);
          } catch (_) {}
        }
        return true;
      } else if ((trimmed.startsWith("<?xml") && trimmed.includes("<MPD")) || trimmed.startsWith("<MPD")) {
        const dashQualities = [];
        const repRe = /<Representation\b([^>]*)>/gi;
        let rm;
        while ((rm = repRe.exec(text)) !== null) {
          const attrs = rm[1] || "";
          const idM = attrs.match(/\bid="([^"]+)"/i);
          const wM = attrs.match(/\bwidth="(\d+)"/i);
          const hM = attrs.match(/\bheight="(\d+)"/i);
          const bwM = attrs.match(/\bbandwidth="(\d+)"/i);
          const w = wM ? parseInt(wM[1], 10) : 0;
          const h = hM ? parseInt(hM[1], 10) : 0;
          const bw = bwM ? parseInt(bwM[1], 10) : 0;
          if (h <= 0 && w <= 0 && bw <= 0) continue;
          const resLabel = h > 0 ? `${h}p` : (idM ? idM[1] : "Video");
          const bwStr = bw >= 1000000 ? `${(bw / 1000000).toFixed(1)} Mbps` : (bw >= 1000 ? `${Math.round(bw / 1000)} Kbps` : "");
          dashQualities.push({
            id: h > 0 ? `${h}p` : (idM ? idM[1] : `dash-${dashQualities.length}`),
            label: `${resLabel}${bwStr ? " (" + bwStr + ")" : ""}`,
            resolution: resLabel,
            width: w,
            height: h,
            bandwidth: bw,
            ext: "mp4",
            variantUrl: effectiveUrl,
            videoFilter: h > 0 ? `${h}p` : (idM ? idM[1] : "best")
          });
        }
        dashQualities.sort((a, b) => ((b.height || 0) - (a.height || 0)) || ((b.bandwidth || 0) - (a.bandwidth || 0)));
        sendMedia({
          streamType: "DASH",
          sourceType: "DASH_MANIFEST",
          ext: "mpd",
          url: effectiveUrl,
          blobUrl: url.startsWith("blob:") ? url : "",
          resolution: dashQualities.length > 0 ? dashQualities[0].resolution : "",
          qualities: dashQualities,
          referer: referer || (win.location ? win.location.href : ""),
          cachedManifestBody: text
        });
        return true;
      } else if (url.includes("/youtubei/v1/player") && trimmed.startsWith("{")) {
        try {
          const ytObj = JSON.parse(trimmed);
          return inspectYouTubePlayerObject(ytObj, win.location ? win.location.href : url);
        } catch (_) {}
      } else if (/^https:\/\/[^.]*\.vimeocdn\.com\/.*playlist\.json/i.test(url)) {
        try {
          const vimeoObj = JSON.parse(text);
          if (vimeoObj && vimeoObj.clip_id) {
            sendMedia({
              streamType: "VIMEO",
              sourceType: "YOUTUBE_EXTRACTION",
              ext: "mp4",
              url,
              referer: referer || (win.location ? win.location.href : ""),
              extractorJson: text,
              cachedManifestBody: ""
            });
            return true;
          }
        } catch (_) {}
      }
      return false;
    }

    function inspectUrl(url, referer) {
      if (!url || typeof url !== "string") return;
      if (url.startsWith("blob:") || url.startsWith("data:") || url.startsWith("javascript:")) return;

      let resolvedUrl = url;
      try {
        if (win.location && win.location.href) {
          resolvedUrl = new URL(url, win.location.href).href;
        }
      } catch (_) {}

      const lower = resolvedUrl.toLowerCase();
      const ref = referer || (win.location ? win.location.href : "");

      if (lower.includes(".m3u8")) {
        sendMedia({
          streamType: "HLS",
          sourceType: "HLS_MANIFEST",
          ext: "m3u8",
          url: resolvedUrl,
          referer: ref
        });
      } else if (lower.includes(".mpd")) {
        sendMedia({
          streamType: "DASH",
          sourceType: "DASH_MANIFEST",
          ext: "mpd",
          url: resolvedUrl,
          referer: ref
        });
      } else if (lower.includes("googlevideo.com/videoplayback") || lower.includes("/videoplayback?")) {
        // Do NOT pretend an adaptive or chunked googlevideo URL is a standalone HTTP_FILE
        const cleanUrl = normalizeGooglevideoUrl(resolvedUrl);
        try {
          const rawU = new URL(resolvedUrl);
          const u = new URL(cleanUrl);
          const mime = (u.searchParams.get("mime") || "video/mp4").toLowerCase();
          const clen = parseInt(u.searchParams.get("clen") || "0", 10);
          const itag = u.searchParams.get("itag") || "";
          const hasUmp = rawU.searchParams.get("ump") === "1" || rawU.searchParams.has("srfvp");
          const hadRange = rawU.searchParams.has("range") || rawU.searchParams.has("rbuf");
          const isAudio = mime.startsWith("audio/");
          const isProgressive = PROGRESSIVE_YOUTUBE_ITAGS.has(itag) && !isAudio && !hasUmp && !hadRange;

          if (!scanPageForYouTubeData()) {
            const watchRef = (ref && (ref.includes("youtube.com/watch") || ref.includes("youtube.com/shorts/") || ref.includes("youtu.be/")))
              ? ref
              : cleanUrl;
            sendMedia({
              streamType: isProgressive ? "HTTP_FILE" : "YOUTUBE",
              sourceType: isProgressive ? "DIRECT_PROGRESSIVE" : "YOUTUBE_EXTRACTION",
              ext: isProgressive ? (mime.includes("webm") ? "webm" : "mp4") : "mp4",
              mime,
              size: clen > 0 ? clen : 0,
              itag,
              isAudioOnly: isAudio,
              isGoogleVideo: true,
              isAdaptiveTrack: !isProgressive,
              videoUrl: (!isAudio && !hasUmp) ? cleanUrl : "",
              pairedAudioUrl: (isAudio && !hasUmp) ? cleanUrl : "",
              requiresCipherExtraction: hasUmp,
              url: isProgressive ? cleanUrl : watchRef,
              referer: ref
            });
          }
        } catch (_) {
          scanPageForYouTubeData();
        }
      } else if (/\.(mp4|webm|mkv|mov|flv)(\?.*)?$/i.test(lower)) {
        // Suppress init segments
        if (/\/(init|header)[-_0-9]*\.(mp4|m4s)$/i.test(lower)) return;
        const extMatch = lower.match(/\.(mp4|webm|mkv|mov|flv)(\?.*)?$/i);
        sendMedia({
          streamType: "HTTP_FILE",
          sourceType: "DIRECT_PROGRESSIVE",
          ext: extMatch ? extMatch[1].toLowerCase() : "mp4",
          url: resolvedUrl,
          referer: ref
        });
      }
    }

    function scanPerformanceResources() {
      try {
        if (!win.performance || typeof win.performance.getEntriesByType !== "function") return;
        const entries = win.performance.getEntriesByType("resource") || [];
        for (const entry of entries) {
          if (entry && entry.name) {
            inspectUrl(entry.name, win.location ? win.location.href : "");
          }
        }
      } catch (_) {}
    }

    // 1. Hook window.fetch
    if (typeof win.fetch === "function") {
      const origFetch = win.fetch;
      win.fetch = async function (...args) {
        let reqUrl = "";
        try {
          const req = args[0];
          reqUrl = (typeof req === "string") ? req : (req && req.url) ? req.url : "";
          if (reqUrl) inspectUrl(reqUrl);
        } catch (_) {}

        const response = await origFetch.apply(this, args);
        try {
          const finalUrl = (response && response.url) ? response.url : reqUrl;
          if (finalUrl && finalUrl !== reqUrl) inspectUrl(finalUrl);

          if (response && typeof response.clone === "function") {
            const contentType = (response.headers && typeof response.headers.get === "function" && response.headers.get("content-type")) || "";
            const isMediaOrDoc = !contentType ||
                                contentType.includes("mpegurl") ||
                                contentType.includes("dash+xml") ||
                                contentType.includes("xml") ||
                                contentType.includes("json") ||
                                contentType.includes("text") ||
                                finalUrl.includes(".m3u8") ||
                                finalUrl.includes(".mpd") ||
                                finalUrl.includes("manifest") ||
                                finalUrl.includes("playlist") ||
                                finalUrl.includes("/youtubei/v1/player");
            if (isMediaOrDoc) {
              const cloned = response.clone();
              if (cloned && typeof cloned.text === "function") {
                cloned.text().then(text => {
                  inspectManifestText(finalUrl, text, win.location ? win.location.href : "");
                }).catch(() => {});
              }
            }
          }
        } catch (_) {}
        return response;
      };
    }

    // 2. Hook window.XMLHttpRequest
    if (typeof win.XMLHttpRequest === "function" && win.XMLHttpRequest.prototype) {
      const origOpen = win.XMLHttpRequest.prototype.open;
      const origSend = win.XMLHttpRequest.prototype.send;

      win.XMLHttpRequest.prototype.open = function (method, url, ...rest) {
        try {
          this.__fluxUrl = (typeof url === "string") ? url : String(url);
          inspectUrl(this.__fluxUrl);
        } catch (_) {}
        return origOpen.apply(this, [method, url, ...rest]);
      };

      win.XMLHttpRequest.prototype.send = function (...bodyArgs) {
        if (typeof this.addEventListener === "function") {
          this.addEventListener("load", function () {
            try {
              const url = this.responseURL || this.__fluxUrl;
              if (!url) return;
              inspectUrl(url);
              const rType = this.responseType;
              if (rType === "" || rType === "text") {
                if (typeof this.responseText === "string") {
                  inspectManifestText(url, this.responseText, win.location ? win.location.href : "");
                }
              }
            } catch (_) {}
          }, { passive: true });
        }
        return origSend.apply(this, bodyArgs);
      };
    }

    // 3. Hook URL.createObjectURL & URL.revokeObjectURL (Blob tracking with delayed revocation)
    if (win.URL && typeof win.URL.createObjectURL === "function") {
      const origCreateObjectUrl = win.URL.createObjectURL;
      win.URL.createObjectURL = function (obj) {
        const blobUrl = origCreateObjectUrl.apply(this, arguments);
        try {
          if (blobRegistry.size > 200) {
            const cutoff = Date.now() - 600000;
            for (const [k, v] of blobRegistry.entries()) {
              if (v.createdAt < cutoff) blobRegistry.delete(k);
            }
          }
          const isMediaSource = (typeof MediaSource !== "undefined" && obj instanceof MediaSource) ||
                                (typeof ManagedMediaSource !== "undefined" && obj instanceof ManagedMediaSource);
          if (isMediaSource) {
            blobRegistry.set(blobUrl, {
              blobUrl,
              kind: "MediaSource",
              mimeType: "video/mp4",
              size: 0,
              createdAt: Date.now()
            });
            scanPerformanceResources();
            scanPageForYouTubeData();
          } else if (typeof Blob !== "undefined" && obj instanceof Blob) {
            const type = (obj.type || "").toLowerCase();
            blobRegistry.set(blobUrl, {
              blobUrl,
              blob: obj,
              kind: "Blob",
              mimeType: type,
              size: obj.size || 0,
              createdAt: Date.now()
            });
            if (type.includes("mpegurl") || type.includes("dash") || type.includes("xml") || obj.size < 524288) {
              obj.text().then(text => {
                inspectManifestText(blobUrl, text, win.location ? win.location.href : "");
              }).catch(() => {});
            }
          }
          scanPageForYouTubeData();
        } catch (_) {}
        return blobUrl;
      };
    }

    if (win.URL && typeof win.URL.revokeObjectURL === "function") {
      const origRevokeObjectUrl = win.URL.revokeObjectURL;
      win.URL.revokeObjectURL = function (url) {
        if (typeof url === "string" && blobRegistry.has(url)) {
          setTimeout(() => {
            try { origRevokeObjectUrl.call(win.URL, url); } catch (_) {}
            blobRegistry.delete(url);
          }, 30000);
          return;
        }
        return origRevokeObjectUrl.apply(this, arguments);
      };
    }

    function scanDomMediaElements() {
      try {
        if (!win.document || typeof win.document.querySelectorAll !== "function") return 0;
        const videos = Array.from(win.document.querySelectorAll("video"));
        let foundCount = 0;
        for (const v of videos) {
          const sources = typeof v.querySelectorAll === "function" ? Array.from(v.querySelectorAll("source")) : [];
          const domQualities = [];
          for (const s of sources) {
            const sUrl = s.src || (typeof s.getAttribute === "function" ? s.getAttribute("src") : "") || "";
            if (!sUrl || sUrl.startsWith("blob:") || sUrl.startsWith("data:")) continue;
            const labelAttr = (typeof s.getAttribute === "function")
              ? (s.getAttribute("label") || s.getAttribute("data-quality") || s.getAttribute("data-res") || s.getAttribute("res") || s.getAttribute("size") || "")
              : "";
            const urlResM = sUrl.match(/(?:^|[_\-/\.])(2160p|1440p|1080p|720p|480p|360p|240p|144p)(?:[_\-/\.?&]|$)/i);
            const rawRes = labelAttr
              ? (/^\d+$/.test(labelAttr) ? `${labelAttr}p` : labelAttr)
              : (urlResM ? urlResM[1].toLowerCase() : "");
            const hMatch = rawRes.match(/(\d{3,4})/);
            const h = hMatch ? parseInt(hMatch[1], 10) : 0;
            domQualities.push({
              id: (rawRes || `src-${domQualities.length}`).toLowerCase(),
              label: rawRes ? `${rawRes} (Direct)` : "Direct Stream",
              resolution: rawRes || "HD",
              height: h,
              ext: sUrl.toLowerCase().includes(".webm") ? "webm" : "mp4",
              videoUrl: sUrl,
              videoFilter: rawRes || "best"
            });
          }
          domQualities.sort((a, b) => (b.height || 0) - (a.height || 0));

          const primarySrc = v.currentSrc || v.src || (typeof v.getAttribute === "function" ? v.getAttribute("src") : "") ||
                             (domQualities.length > 0 ? domQualities[0].videoUrl : "");
          if (!primarySrc || primarySrc.startsWith("data:")) continue;
          if (primarySrc.startsWith("blob:")) {
            scanPerformanceResources();
            scanPageForYouTubeData();
            continue;
          }

          const liveH = Number(v.videoHeight || 0);
          const liveRes = liveH > 0 ? `${liveH}p` : (domQualities.length > 0 ? domQualities[0].resolution : "");
          const lower = primarySrc.toLowerCase();
          const isHls = lower.includes(".m3u8") || lower.includes(".m3u");
          const isDash = lower.includes(".mpd");
          sendMedia({
            streamType: isHls ? "HLS" : (isDash ? "DASH" : "HTTP_FILE"),
            sourceType: isHls ? "HLS_MANIFEST" : (isDash ? "DASH_MANIFEST" : "DIRECT_PROGRESSIVE"),
            ext: isHls ? "m3u8" : (isDash ? "mpd" : (lower.includes(".webm") ? "webm" : "mp4")),
            url: primarySrc,
            resolution: liveRes,
            qualities: domQualities,
            pageTitle: (win.document ? win.document.title : "") || "video",
            referer: win.location ? win.location.href : ""
          });
          foundCount++;
        }
        return foundCount;
      } catch (_) {
        return 0;
      }
    }

    win.__FluxCatchTestHook = {
      scanDomMediaElements,
      scanPageForYouTubeData,
      scanPerformanceResources
    };

    // 5. Listen for active blob/MSE & page media discovery requests from content-script.js
    if (typeof win.addEventListener === "function") {
      win.addEventListener("message", (ev) => {
        if (!ev || !ev.data) return;
        if (ev.data.action === "fluxScanPageMedia") {
          scanPerformanceResources();
          scanPageForYouTubeData();
          scanDomMediaElements();
          return;
        }
        if (ev.data.action !== "fluxDiscoverBlobSource") return;
        try {
          const blobUrl = ev.data.blobUrl || "";
          const correlationId = ev.data.correlationId || makeCorrelationId("blob");
          scanPerformanceResources();
          const ytFound = scanPageForYouTubeData();
          scanDomMediaElements();
          const blobMeta = blobRegistry.get(blobUrl) || null;
          postToExtension({
            action: "catCatchBlobDiscoveryResult",
            blobUrl,
            correlationId,
            ytFound,
            blobMeta,
            pageUrl: win.location ? win.location.href : ""
          });
        } catch (_) {}
      });
    }

    // 6. Listen for YouTube SPA & DOM lifecycle events (automatic video + quality capture on play)
    if (win.document && typeof win.document.addEventListener === "function") {
      win.document.addEventListener("DOMContentLoaded", () => {
        scanPerformanceResources();
        scanPageForYouTubeData();
        scanDomMediaElements();
      });
      win.document.addEventListener("yt-navigate-finish", scanPageForYouTubeData);
      win.document.addEventListener("yt-page-data-updated", scanPageForYouTubeData);
      win.document.addEventListener("loadedmetadata", () => {
        scanPerformanceResources();
        scanPageForYouTubeData();
        scanDomMediaElements();
      }, true);
      win.document.addEventListener("play", () => {
        scanPerformanceResources();
        scanPageForYouTubeData();
        scanDomMediaElements();
      }, true);
      win.document.addEventListener("playing", () => {
        scanPerformanceResources();
        scanPageForYouTubeData();
        scanDomMediaElements();
      }, true);
    }
    if (typeof win.addEventListener === "function") {
      win.addEventListener("load", () => {
        scanPerformanceResources();
        scanPageForYouTubeData();
        scanDomMediaElements();
      });
    }

    scanPageForYouTubeData();
    scanDomMediaElements();
  }

  // Execute in browser window
  if (typeof window !== "undefined") {
    installDeepSearchHooks(window, "flux-deepsearch", null);
  }

  if (typeof module !== "undefined" && module.exports) {
    module.exports = { installDeepSearchHooks };
  }
})();
