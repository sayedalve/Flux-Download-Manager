// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/content-script.js) & Brisk Browser Extension.
// Observes <video> elements, positions the non-intrusive closed Shadow DOM "Download with Flux" badge,
// intercepts link clicks (with Alt-key bypass), captures playback events, and relays page deep search messages.

function positionOverlayHost(video, hostEl) {
  const rect = typeof video.getBoundingClientRect === "function"
    ? video.getBoundingClientRect()
    : { width: video.width || 640, height: video.height || 360, top: 20, left: 20, right: 660 };

  // Place badge 10px from top-right corner of the video element so bottom transport controls are never obscured
  const topPx = Math.max(8, Math.round((rect.top || 20) + 10));
  const rightEdge = rect.right || ((rect.left || 20) + (rect.width || 640));
  const leftPx = Math.max(8, Math.round(rightEdge - 168));

  hostEl.style.cssText =
    `position:fixed;top:${topPx}px;left:${leftPx}px;z-index:2147483647;pointer-events:auto;`;
}

function scanAndAttachVideoOverlays(doc, onDownloadClick, minWidth = 240, minHeight = 135) {
  if (!doc || !doc.querySelectorAll) return [];
  const videos = Array.from(doc.querySelectorAll("video"));
  const attached = [];

  for (const video of videos) {
    const rect = typeof video.getBoundingClientRect === "function"
      ? video.getBoundingClientRect()
      : { width: video.width || 640, height: video.height || 360, top: 20, left: 20, right: 660 };

    if (rect.width < minWidth || rect.height < minHeight) {
      continue;
    }

    const src = video.currentSrc || video.src || (video.querySelector && video.querySelector("source") ? video.querySelector("source").src : "");
    const isBlob = src.startsWith("blob:");
    const isDrm = Boolean(video.mediaKeys);

    if (!video.dataset.fluxOverlayAttached && !video.dataset.openidmOverlayAttached) {
      video.dataset.fluxOverlayAttached = "true";
      video.dataset.openidmOverlayAttached = "true";

      const hostEl = doc.createElement("div");
      hostEl.setAttribute("data-flux-shadow-host", "true");
      hostEl.setAttribute("data-openidm-shadow-host", "true");
      positionOverlayHost(video, hostEl);

      // Closed Shadow DOM ensures zero CSS leakage into or from the host webpage
      const shadow = hostEl.attachShadow ? hostEl.attachShadow({ mode: "closed" }) : hostEl;
      const btn = doc.createElement("button");
      btn.type = "button";
      btn.textContent = isDrm ? "DRM Protected (Unsupported)" : "Download with Flux";
      btn.disabled = isDrm;
      if (btn.style) {
        btn.style.cssText =
          "background:#0F172A;color:#F8FAFC;border:1px solid #2563EB;border-radius:6px;" +
          "padding:6px 12px;font:600 12px 'Segoe UI Variable Text','Segoe UI',sans-serif;cursor:pointer;box-shadow:0 2px 8px rgba(0,0,0,0.5);";
      }
      btn.addEventListener("click", (ev) => {
        ev.stopPropagation();
        ev.preventDefault();
        if (!isDrm && onDownloadClick) {
          onDownloadClick({
            videoSrc: isBlob ? "" : src,
            isMseBlob: isBlob,
            pageTitle: doc.title || "video",
            frameUrl: (doc.location ? doc.location.href : ""),
            isIframe: (typeof window !== "undefined" && window.self !== window.top)
          });
        }
      });
      shadow.appendChild(btn);
      if (doc.body && doc.body.appendChild) {
        doc.body.appendChild(hostEl);
      }
      attached.push({
        video,
        hostEl,
        btn,
        isBlob,
        isDrm,
        src,
        reposition: () => positionOverlayHost(video, hostEl)
      });
    }
  }
  return attached;
}

function observeDynamicVideosAndFullscreen(doc, win, onDownloadClick) {
  let activeOverlays = scanAndAttachVideoOverlays(doc, onDownloadClick);

  const refreshPositions = () => {
    for (const item of activeOverlays) {
      item.reposition();
    }
  };

  if (win && typeof win.addEventListener === "function") {
    win.addEventListener("scroll", refreshPositions, { passive: true });
    win.addEventListener("resize", refreshPositions, { passive: true });
  }
  if (doc && typeof doc.addEventListener === "function") {
    doc.addEventListener("fullscreenchange", refreshPositions);
  }
  if (typeof MutationObserver !== "undefined" && doc && doc.documentElement) {
    const observer = new MutationObserver(() => {
      const newlyAttached = scanAndAttachVideoOverlays(doc, onDownloadClick);
      if (newlyAttached.length > 0) {
        activeOverlays = activeOverlays.concat(newlyAttached);
      }
      refreshPositions();
    });
    observer.observe(doc.documentElement, { childList: true, subtree: true });
    return { activeOverlays, observer, refreshPositions };
  }
  return { activeOverlays, observer: null, refreshPositions };
}

function handleLinkClick(ev) {
  try {
    if (ev.altKey) {
      // IDM Alt-key bypass behavior: user explicitly requested browser-native download
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({ action: "bypassNextDownload" });
      }
      return;
    }

    const anchor = ev.target && typeof ev.target.closest === "function" ? ev.target.closest("a") : null;
    if (!anchor || !anchor.href) return;

    const href = anchor.href;
    if (href.startsWith("javascript:") || href.startsWith("#")) return;

    const pathname = anchor.pathname || "";
    const lastDot = pathname.lastIndexOf(".");
    const ext = lastDot !== -1 ? pathname.slice(lastDot + 1).toLowerCase() : "";
    const hasDownloadAttr = anchor.hasAttribute("download");

    const interceptExts = new Set([
      "zip", "rar", "7z", "tar", "gz", "bz2", "xz", "iso", "exe", "msi", "dmg", "pdf",
      "mp4", "mkv", "webm", "mp3", "flac", "apk", "bin", "dmg"
    ]);

    if (hasDownloadAttr || interceptExts.has(ext)) {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "linkClicked",
          url: href,
          filename: anchor.download || "",
          referrer: (typeof location !== "undefined" ? location.href : ""),
          pageTitle: (typeof document !== "undefined" ? document.title : ""),
          bypass: false
        });
      }
    }
  } catch (_) {}
}

function handleMediaPlaybackEvent(ev) {
  try {
    const target = ev.target;
    if (!target) return;
    const tagName = (target.tagName || "").toLowerCase();
    if (tagName !== "video" && tagName !== "audio") return;

    const src = target.currentSrc || target.src;
    if (src && !src.startsWith("blob:") && !src.startsWith("data:")) {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addMedia",
          url: src,
          streamType: "HTTP_FILE",
          pageTitle: (typeof document !== "undefined" ? document.title : "video"),
          tabUrl: (typeof location !== "undefined" ? location.href : "")
        });
      }
    }
  } catch (_) {}
}

function injectDeepSearchScript() {
  if (typeof document === "undefined" || !document.documentElement) return;
  try {
    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.getURL) {
      const script = document.createElement("script");
      script.src = chrome.runtime.getURL("catch-script/catch.js");
      script.async = false;
      (document.head || document.documentElement).appendChild(script);
      script.onload = () => { script.remove(); };
    }
  } catch (_) {}
}

function initContentScript() {
  if (typeof document === "undefined" || typeof window === "undefined") return;

  // 1. Intercept link clicks with capture phase
  document.addEventListener("click", handleLinkClick, true);
  document.addEventListener("auxclick", handleLinkClick, true);

  // 2. Listen to media playback events with capture phase
  document.addEventListener("play", handleMediaPlaybackEvent, true);
  document.addEventListener("playing", handleMediaPlaybackEvent, true);
  document.addEventListener("canplay", handleMediaPlaybackEvent, true);
  document.addEventListener("loadedmetadata", handleMediaPlaybackEvent, true);

  // 3. Relay page MAIN world messages from catch-script/catch.js to background
  window.addEventListener("message", (ev) => {
    if (!ev.data || ev.origin !== window.location.origin) return;
    if (ev.data.action === "catCatchAddMedia") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addMedia",
          url: ev.data.url,
          streamType: ev.data.streamType || "HLS",
          ext: ev.data.ext || "m3u8",
          cachedManifestBody: ev.data.cachedManifestBody || "",
          tabUrl: location.href,
          pageTitle: document.title || "video"
        });
      }
    } else if (ev.data.action === "catCatchAddKey") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addKey",
          key: ev.data.key,
          url: ev.data.url
        });
      }
    }
  }, { capture: true });

  // 4. Inject deep search script into page DOM
  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", injectDeepSearchScript, { once: true });
  } else {
    injectDeepSearchScript();
  }

  // 5. Observe videos and attach overlay
  const onDownloadClick = (mediaMeta) => {
    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      chrome.runtime.sendMessage({
        action: "downloadMedia",
        url: mediaMeta.videoSrc,
        isMseBlob: mediaMeta.isMseBlob,
        pageTitle: mediaMeta.pageTitle,
        frameUrl: mediaMeta.frameUrl
      });
    }
  };

  observeDynamicVideosAndFullscreen(document, window, onDownloadClick);
}

// Auto-run in browser execution
if (typeof window !== "undefined" && typeof document !== "undefined") {
  initContentScript();
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    positionOverlayHost,
    scanAndAttachVideoOverlays,
    observeDynamicVideosAndFullscreen,
    handleLinkClick,
    handleMediaPlaybackEvent,
    initContentScript
  };
}
