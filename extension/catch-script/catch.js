// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (catch-script/search.js & catch.js) for Flux Download Manager.
// Runs in page MAIN world at document_start. Hooks window.fetch, window.XMLHttpRequest,
// window.URL.createObjectURL, MediaSource, and inspects YouTube player responses (ytInitialPlayerResponse / movie_player).

(function () {
  function installDeepSearchHooks(win, sessionNonce, emitEvent) {
    if (!win || win.__fluxCatchScriptInstalled) return;
    win.__fluxCatchScriptInstalled = true;

    const seenUrls = new Set();
    let lastInspectedYtVideoId = "";

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
      const dedupKey = (data.streamType || "") + "|" + data.url;
      if (seenUrls.has(dedupKey) && !data.cachedManifestBody) return;
      seenUrls.add(dedupKey);
      postToExtension(Object.assign({ action: "catCatchAddMedia" }, data));
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

      // 1. Check if HLS or DASH manifests are directly exposed
      if (streamingData.hlsManifestUrl) {
        sendMedia({
          streamType: "HLS",
          ext: "m3u8",
          url: streamingData.hlsManifestUrl,
          pageTitle: cleanTitle,
          referer: canonicalWatchUrl
        });
      }
      if (streamingData.dashManifestUrl) {
        sendMedia({
          streamType: "DASH",
          ext: "mpd",
          url: streamingData.dashManifestUrl,
          pageTitle: cleanTitle,
          referer: canonicalWatchUrl
        });
      }

      // 2. Inspect progressive formats (combined audio + video) and adaptiveFormats (separate video & audio)
      const formats = Array.isArray(streamingData.formats) ? streamingData.formats : [];
      const adaptive = Array.isArray(streamingData.adaptiveFormats) ? streamingData.adaptiveFormats : [];

      let bestProgressive = null;
      for (const f of formats) {
        if (f && f.url) {
          const h = Number(f.height || 0);
          if (!bestProgressive || h > Number(bestProgressive.height || 0)) {
            bestProgressive = f;
          }
        }
      }

      let bestVideo = null;
      let bestAudio = null;
      for (const f of adaptive) {
        if (!f || !f.url) continue;
        const mime = String(f.mimeType || "").toLowerCase();
        if (mime.startsWith("video/")) {
          const bw = Number(f.bitrate || f.averageBitrate || 0);
          if (!bestVideo || bw > Number(bestVideo.bitrate || bestVideo.averageBitrate || 0)) {
            bestVideo = f;
          }
        } else if (mime.startsWith("audio/")) {
          const bw = Number(f.bitrate || f.averageBitrate || 0);
          if (!bestAudio || bw > Number(bestAudio.bitrate || bestAudio.averageBitrate || 0)) {
            bestAudio = f;
          }
        }
      }

      const chosenVideo = bestVideo || bestProgressive;
      if (chosenVideo && chosenVideo.url) {
        const cleanVideoUrl = normalizeGooglevideoUrl(chosenVideo.url);
        const cleanAudioUrl = (bestVideo && bestAudio && bestAudio.url) ? normalizeGooglevideoUrl(bestAudio.url) : "";
        const resolution = chosenVideo.qualityLabel ||
                           (chosenVideo.width && chosenVideo.height ? `${chosenVideo.width}x${chosenVideo.height}` : "HD");
        const sizeBytes = parseInt(chosenVideo.contentLength || "0", 10) +
                          (bestAudio ? parseInt(bestAudio.contentLength || "0", 10) : 0);

        const descriptor = {
          fluxType: "YOUTUBE_STREAM",
          videoId,
          title: cleanTitle,
          watchUrl: canonicalWatchUrl,
          resolution,
          videoUrl: cleanVideoUrl,
          audioUrl: cleanAudioUrl,
          videoMime: String(chosenVideo.mimeType || "video/mp4").split(";")[0],
          audioMime: bestAudio ? String(bestAudio.mimeType || "audio/mp4").split(";")[0] : "",
          hlsManifestUrl: streamingData.hlsManifestUrl || "",
          dashManifestUrl: streamingData.dashManifestUrl || "",
          size: sizeBytes > 0 ? sizeBytes : 0
        };

        lastInspectedYtVideoId = videoId || canonicalWatchUrl;
        sendMedia({
          streamType: "YOUTUBE",
          ext: "mp4",
          url: canonicalWatchUrl,
          videoId,
          pageTitle: cleanTitle,
          resolution,
          size: sizeBytes > 0 ? sizeBytes : 0,
          pairedAudioUrl: cleanAudioUrl,
          referer: canonicalWatchUrl,
          cachedManifestBody: JSON.stringify(descriptor)
        });
        return true;
      }

      // If formats all use signatureCipher / cipher, still register the YouTube extraction target for yt-dlp worker
      if (formats.length > 0 || adaptive.length > 0) {
        const topFmt = adaptive[0] || formats[0];
        const resolution = (topFmt && topFmt.qualityLabel) || "Auto";
        const descriptor = {
          fluxType: "YOUTUBE_STREAM",
          videoId,
          title: cleanTitle,
          watchUrl: canonicalWatchUrl,
          resolution,
          requiresCipherExtraction: true
        };
        lastInspectedYtVideoId = videoId || canonicalWatchUrl;
        sendMedia({
          streamType: "YOUTUBE",
          ext: "mp4",
          url: canonicalWatchUrl,
          videoId,
          pageTitle: cleanTitle,
          resolution,
          referer: canonicalWatchUrl,
          cachedManifestBody: JSON.stringify(descriptor)
        });
        return true;
      }

      return false;
    }

    function scanPageForYouTubeData() {
      try {
        if (!win.location || !win.location.hostname) return;
        const host = win.location.hostname.toLowerCase();
        if (!host.includes("youtube.com") && host !== "youtu.be") return;

        // 1. Check #movie_player API (most up-to-date during SPA navigation)
        if (win.document && typeof win.document.getElementById === "function") {
          const playerEl = win.document.getElementById("movie_player");
          if (playerEl && typeof playerEl.getPlayerResponse === "function") {
            const pr = playerEl.getPlayerResponse();
            if (pr && inspectYouTubePlayerObject(pr, win.location.href)) {
              return;
            }
          }
        }

        // 2. Check window.ytInitialPlayerResponse
        if (win.ytInitialPlayerResponse) {
          inspectYouTubePlayerObject(win.ytInitialPlayerResponse, win.location.href);
        }
      } catch (_) {}
    }

    function inspectManifestText(url, text, referer) {
      if (typeof text !== "string" || !url) return;
      const trimmed = text.trimStart();
      if (trimmed.startsWith("#EXTM3U") || trimmed.includes("#EXT-X-STREAM-INF") || trimmed.includes("#EXT-X-TARGETDURATION")) {
        sendMedia({
          streamType: "HLS",
          ext: "m3u8",
          url,
          referer: referer || (win.location ? win.location.href : ""),
          cachedManifestBody: text
        });

        // Parse AES-128 key URI if present: #EXT-X-KEY:METHOD=AES-128,URI="key_url"
        const keyMatch = text.match(/#EXT-X-KEY:METHOD=([^,\r\n]+)(?:,URI="([^"\r\n]+)")?/i);
        if (keyMatch && keyMatch[2]) {
          try {
            const resolvedKeyUrl = new URL(keyMatch[2], url).href;
            sendKey(resolvedKeyUrl, url);
          } catch (_) {}
        }
      } else if ((trimmed.startsWith("<?xml") && trimmed.includes("<MPD")) || trimmed.startsWith("<MPD")) {
        sendMedia({
          streamType: "DASH",
          ext: "mpd",
          url,
          referer: referer || (win.location ? win.location.href : ""),
          cachedManifestBody: text
        });
      } else if (url.includes("/youtubei/v1/player") && trimmed.startsWith("{")) {
        try {
          const ytObj = JSON.parse(trimmed);
          inspectYouTubePlayerObject(ytObj, win.location ? win.location.href : url);
        } catch (_) {}
      } else if (/^https:\/\/[^.]*\.vimeocdn\.com\/.*playlist\.json/i.test(url)) {
        try {
          const vimeoObj = JSON.parse(text);
          if (vimeoObj && vimeoObj.clip_id) {
            sendMedia({
              streamType: "VIMEO",
              ext: "json",
              url,
              referer: referer || (win.location ? win.location.href : ""),
              cachedManifestBody: text
            });
          }
        } catch (_) {}
      }
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
          ext: "m3u8",
          url: resolvedUrl,
          referer: ref
        });
      } else if (lower.includes(".mpd")) {
        sendMedia({
          streamType: "DASH",
          ext: "mpd",
          url: resolvedUrl,
          referer: ref
        });
      } else if (lower.includes("googlevideo.com/videoplayback") || lower.includes("/videoplayback?")) {
        const cleanUrl = normalizeGooglevideoUrl(resolvedUrl);
        try {
          const u = new URL(cleanUrl);
          const mime = (u.searchParams.get("mime") || "video/mp4").toLowerCase();
          const clen = parseInt(u.searchParams.get("clen") || "0", 10);
          const itag = u.searchParams.get("itag") || "";
          const isAudio = mime.startsWith("audio/");
          sendMedia({
            streamType: "HTTP_FILE",
            ext: mime.includes("webm") ? "webm" : (isAudio ? "m4a" : "mp4"),
            mime,
            size: clen > 0 ? clen : 0,
            itag,
            isAudioOnly: isAudio,
            isGoogleVideo: true,
            url: cleanUrl,
            referer: ref
          });
        } catch (_) {}
        scanPageForYouTubeData();
      } else if (/\.(mp4|webm|mkv|mov|flv)(\?.*)?$/i.test(lower)) {
        const extMatch = lower.match(/\.(mp4|webm|mkv|mov|flv)(\?.*)?$/i);
        sendMedia({
          streamType: "HTTP_FILE",
          ext: extMatch ? extMatch[1].toLowerCase() : "mp4",
          url: resolvedUrl,
          referer: ref
        });
      }
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
    if (typeof win.XMLHttpRequest === "function") {
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

    // 3. Hook URL.createObjectURL (Blob manifest detection)
    if (win.URL && typeof win.URL.createObjectURL === "function") {
      const origCreateObjectUrl = win.URL.createObjectURL;
      win.URL.createObjectURL = function (obj) {
        const blobUrl = origCreateObjectUrl.apply(this, arguments);
        try {
          if (typeof Blob !== "undefined" && obj instanceof Blob) {
            const type = (obj.type || "").toLowerCase();
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

    // 4. Listen for YouTube SPA & DOM lifecycle events
    if (win.document && typeof win.document.addEventListener === "function") {
      win.document.addEventListener("DOMContentLoaded", scanPageForYouTubeData);
      win.document.addEventListener("yt-navigate-finish", scanPageForYouTubeData);
      win.document.addEventListener("yt-page-data-updated", scanPageForYouTubeData);
      win.document.addEventListener("loadedmetadata", scanPageForYouTubeData, true);
      win.document.addEventListener("play", scanPageForYouTubeData, true);
    }
    if (typeof win.addEventListener === "function") {
      win.addEventListener("load", scanPageForYouTubeData);
    }
    scanPageForYouTubeData();
  }

  // Execute in browser window
  if (typeof window !== "undefined") {
    installDeepSearchHooks(window, "flux-deepsearch", null);
  }

  if (typeof module !== "undefined" && module.exports) {
    module.exports = { installDeepSearchHooks };
  }
})();
