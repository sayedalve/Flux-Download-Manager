// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (catch-script/search.js & catch.js) for Flux Download Manager.
// Hooks window.fetch, window.XMLHttpRequest, and window.URL.createObjectURL in page MAIN world,
// capturing dynamically fetched HLS (#EXTM3U) and DASH (<MPD) manifests, Vimeo configs, and AES-128 keys.

(function () {
  const isRunningInWorker = typeof WorkerGlobalScope !== "undefined" && self instanceof WorkerGlobalScope;

  function installDeepSearchHooks(win, sessionNonce, emitEvent) {
    if (!win || win.__fluxCatchScriptInstalled) return;
    win.__fluxCatchScriptInstalled = true;

    const seenUrls = new Set();

    function sendMedia(data) {
      if (!data || !data.url) return;
      if (seenUrls.has(data.url) && !data.cachedManifestBody) return;
      seenUrls.add(data.url);

      if (typeof emitEvent === "function") {
        emitEvent(Object.assign({ nonce: sessionNonce }, data));
      } else if (typeof win.postMessage === "function") {
        win.postMessage(Object.assign({ action: "catCatchAddMedia", nonce: sessionNonce }, data), "*");
      }
    }

    function sendKey(key, url) {
      if (!key) return;
      if (typeof emitEvent === "function") {
        emitEvent({ nonce: sessionNonce, action: "catCatchAddKey", key, url });
      } else if (typeof win.postMessage === "function") {
        win.postMessage({ action: "catCatchAddKey", nonce: sessionNonce, key, url }, "*");
      }
    }

    function inspectManifestText(url, text, referer) {
      if (typeof text !== "string" || !url) return;
      const trimmed = text.trimStart();
      if (trimmed.startsWith("#EXTM3U") || trimmed.includes("#EXT-X-STREAM-INF") || trimmed.includes("#EXT-X-TARGETDURATION")) {
        sendMedia({
          action: "catCatchAddMedia",
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
          action: "catCatchAddMedia",
          streamType: "DASH",
          ext: "mpd",
          url,
          referer: referer || (win.location ? win.location.href : ""),
          cachedManifestBody: text
        });
      } else if (/^https:\/\/[^.]*\.vimeocdn\.com\/.*playlist\.json/i.test(url)) {
        try {
          const vimeoObj = JSON.parse(text);
          if (vimeoObj && vimeoObj.clip_id) {
            sendMedia({
              action: "catCatchAddMedia",
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

      const lower = url.toLowerCase();
      if (lower.includes(".m3u8")) {
        sendMedia({
          action: "catCatchAddMedia",
          streamType: "HLS",
          ext: "m3u8",
          url,
          referer: referer || (win.location ? win.location.href : "")
        });
      } else if (lower.includes(".mpd")) {
        sendMedia({
          action: "catCatchAddMedia",
          streamType: "DASH",
          ext: "mpd",
          url,
          referer: referer || (win.location ? win.location.href : "")
        });
      }
    }

    // 1. Hook window.fetch
    if (typeof win.fetch === "function") {
      const origFetch = win.fetch;
      win.fetch = async function (...args) {
        const response = await origFetch.apply(this, args);
        try {
          const req = args[0];
          const reqUrl = (typeof req === "string") ? req : (req && req.url) ? req.url : response.url;
          inspectUrl(reqUrl);

          if (response && typeof response.clone === "function") {
            try {
              const contentType = (response.headers && typeof response.headers.get === "function" && response.headers.get("content-type")) || "";
              const isMediaOrDoc = !contentType ||
                                  contentType.includes("mpegurl") ||
                                  contentType.includes("dash+xml") ||
                                  contentType.includes("xml") ||
                                  contentType.includes("json") ||
                                  contentType.includes("text") ||
                                  reqUrl.includes(".m3u8") ||
                                  reqUrl.includes(".mpd") ||
                                  reqUrl.includes("manifest") ||
                                  reqUrl.includes("playlist");
              if (isMediaOrDoc) {
                const cloned = response.clone();
                if (cloned && typeof cloned.text === "function") {
                  const text = await cloned.text();
                  inspectManifestText(reqUrl, text, win.location ? win.location.href : "");
                }
              }
            } catch (_) {}
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
        this.addEventListener("load", function () {
          try {
            const url = this.__fluxUrl || this.responseURL;
            if (!url) return;
            const rType = this.responseType;
            if (rType === "" || rType === "text") {
              if (typeof this.responseText === "string") {
                inspectManifestText(url, this.responseText, win.location ? win.location.href : "");
              }
            }
          } catch (_) {}
        }, { passive: true });

        return origSend.apply(this, bodyArgs);
      };
    }

    // 3. Hook URL.createObjectURL (Blob manifest detection)
    if (win.URL && typeof win.URL.createObjectURL === "function") {
      const origCreateObjectUrl = win.URL.createObjectURL;
      win.URL.createObjectURL = function (obj) {
        const blobUrl = origCreateObjectUrl.apply(this, arguments);
        try {
          if (obj instanceof Blob) {
            const type = (obj.type || "").toLowerCase();
            if (type.includes("mpegurl") || type.includes("dash") || type.includes("xml") || obj.size < 524288) {
              obj.text().then(text => {
                inspectManifestText(blobUrl, text, win.location ? win.location.href : "");
              }).catch(() => {});
            }
          }
        } catch (_) {}
        return blobUrl;
      };
    }
  }

  // Execute in browser window
  if (typeof window !== "undefined") {
    installDeepSearchHooks(window, "flux-deepsearch", null);
  }

  if (typeof module !== "undefined" && module.exports) {
    module.exports = { installDeepSearchHooks };
  }
})();
