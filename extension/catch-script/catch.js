// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (catch-script/catch.js) Deep Search & MSE Discovery.
// Hooks fetch, XMLHttpRequest, and MediaSource in page context when enabled on a site,
// capturing dynamically fetched HLS (#EXTM3U) and DASH (<MPD) manifest bodies and AES-128 keys.

function installDeepSearchHooks(win, sessionNonce, emitEvent) {
  if (!win || win.__openIdmDeepSearchInstalled) return;
  win.__openIdmDeepSearchInstalled = true;

  function inspectManifestText(url, text) {
    if (typeof text !== "string" || !url) return;
    const trimmed = text.trimStart();
    if (trimmed.startsWith("#EXTM3U")) {
      emitEvent({
        nonce: sessionNonce,
        action: "catCatchAddMedia",
        streamType: "HLS",
        ext: "m3u8",
        url,
        cachedManifestBody: text,
        href: win.location ? win.location.href : ""
      });
    } else if (trimmed.startsWith("<?xml") && trimmed.includes("<MPD") || trimmed.startsWith("<MPD")) {
      emitEvent({
        nonce: sessionNonce,
        action: "catCatchAddMedia",
        streamType: "DASH",
        ext: "mpd",
        url,
        cachedManifestBody: text,
        href: win.location ? win.location.href : ""
      });
    }
  }

  if (typeof win.fetch === "function") {
    const origFetch = win.fetch;
    win.fetch = async function (...args) {
      const response = await origFetch.apply(this, args);
      try {
        const reqUrl = (typeof args[0] === "string") ? args[0] : (args[0] && args[0].url) || response.url;
        if (response && typeof response.clone === "function") {
          const cloned = response.clone();
          const text = await cloned.text();
          inspectManifestText(reqUrl, text);
        }
      } catch (_) {}
      return response;
    };
  }
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = { installDeepSearchHooks };
}
