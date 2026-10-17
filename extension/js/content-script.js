// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/content-script.js) & Brisk Browser Extension.
// Observes <video> elements (including dynamic insertion & resize), positions the closed Shadow DOM
// "Download with Flux" cosmic pill badge, provides real-time status/error feedback on click, intercepts link clicks
// (with Alt-key bypass), captures media events, and relays page MAIN world messages.

function positionOverlayHost(video, hostEl) {
  const rect = typeof video.getBoundingClientRect === "function"
    ? video.getBoundingClientRect()
    : { width: video.width || 640, height: video.height || 360, top: 20, left: 20, right: 660 };

  // Hide unobtrusively if the video player is collapsed or scrolled out of the visible viewport
  const isCollapsed = (rect.width !== undefined && rect.width < 120) || (rect.height !== undefined && rect.height < 80);
  const isOffscreen = (rect.bottom !== undefined && rect.bottom < 28) || (rect.right !== undefined && rect.right < 48);

  // Place badge 10px from top-right corner of the video element so bottom transport controls are never obscured
  const topPx = Math.max(8, Math.round((rect.top || 20) + 10));
  const rightEdge = rect.right || ((rect.left || 20) + (rect.width || 640));
  const leftPx = Math.max(8, Math.round(rightEdge - 168));

  const visCss = (isCollapsed || isOffscreen)
    ? "opacity:0;pointer-events:none;"
    : "pointer-events:auto;";
  const nextCss = `position:fixed;top:${topPx}px;left:${leftPx}px;z-index:2147483647;${visCss}`;
  if (hostEl.__fluxLastCss !== nextCss) {
    hostEl.__fluxLastCss = nextCss;
    hostEl.style.cssText = nextCss;
  }
}

function extractVideoElementSrc(video) {
  if (!video) return "";
  let src = video.currentSrc || video.src || "";
  if (!src && typeof video.querySelector === "function") {
    const sourceEl = video.querySelector("source[src]");
    if (sourceEl) src = sourceEl.src || sourceEl.getAttribute("src") || "";
  }
  return src;
}

function notifyDirectVideoElementDiscovered(video, doc) {
  try {
    const src = extractVideoElementSrc(video);
    if (!src || src.startsWith("blob:") || src.startsWith("data:")) return;
    if (video.__fluxLastReportedSrc === src) return;
    video.__fluxLastReportedSrc = src;

    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      const lower = src.toLowerCase();
      const streamType = lower.includes(".m3u8") ? "HLS" : (lower.includes(".mpd") ? "DASH" : "HTTP_FILE");
      chrome.runtime.sendMessage({
        action: "addMedia",
        url: src,
        streamType,
        pageTitle: (doc && doc.title) ? doc.title : "video",
        tabUrl: (doc && doc.location) ? doc.location.href : ""
      }, () => {
        if (typeof chrome !== "undefined" && chrome.runtime) {
          void chrome.runtime.lastError;
        }
      });
    }
  } catch (_) {}
}

function scanAndAttachVideoOverlays(doc, onDownloadClick, minWidth = 240, minHeight = 135) {
  if (!doc || !doc.querySelectorAll) return [];
  const videos = Array.from(doc.querySelectorAll("video"));
  const attached = [];

  for (const video of videos) {
    notifyDirectVideoElementDiscovered(video, doc);

    const rect = typeof video.getBoundingClientRect === "function"
      ? video.getBoundingClientRect()
      : { width: video.width || 640, height: video.height || 360, top: 20, left: 20, right: 660 };

    if (rect.width < minWidth || rect.height < minHeight) {
      continue;
    }

    const src = extractVideoElementSrc(video);
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

      const styleEl = doc.createElement("style");
      styleEl.textContent = `
        button {
          display: inline-flex;
          align-items: center;
          gap: 6px;
          background: linear-gradient(180deg, #111A2E 0%, #0F172A 100%);
          color: #F8FAFC;
          border: 1px solid #2563EB;
          border-radius: 6px;
          padding: 6px 12px;
          font: 600 12px "Segoe UI Variable Text", "Segoe UI", -apple-system, sans-serif;
          letter-spacing: 0.01em;
          cursor: pointer;
          opacity: 0.88;
          box-shadow: 0 4px 12px rgba(7, 11, 20, 0.65);
          transition: opacity 0.14s ease, border-color 0.14s ease, background 0.14s ease;
        }
        button::before {
          content: "";
          width: 6px;
          height: 6px;
          border-radius: 50%;
          background: #38BDF8;
          box-shadow: 0 0 6px rgba(56, 189, 248, 0.7);
          flex-shrink: 0;
        }
        button:hover:not(:disabled), button:focus-visible {
          opacity: 1;
          border-color: #38BDF8;
          background: linear-gradient(180deg, #1E293B 0%, #0F172A 100%);
          outline: none;
        }
        button:disabled {
          opacity: 0.72;
          border-color: #475569;
          color: #94A3B8;
          cursor: not-allowed;
        }
        button:disabled::before {
          background: #64748B;
          box-shadow: none;
        }
        @media (prefers-reduced-motion: reduce) {
          button { transition: none; }
        }
      `;
      if (shadow && typeof shadow.appendChild === "function") {
        shadow.appendChild(styleEl);
      }

      const btn = doc.createElement("button");
      btn.type = "button";
      btn.textContent = isDrm ? "DRM Protected (Unsupported)" : "Download with Flux";
      btn.disabled = isDrm;
      const baseBtnStyle =
        "background:#0F172A;color:#F8FAFC;border:1px solid #2563EB;border-radius:6px;" +
        "padding:6px 12px;font:600 12px 'Segoe UI Variable Text','Segoe UI',sans-serif;cursor:pointer;box-shadow:0 4px 12px rgba(7,11,20,0.65);";
      if (btn.style) {
        btn.style.cssText = baseBtnStyle;
      }

      const invokeClick = (ev) => {
        if (ev && typeof ev.stopPropagation === "function") ev.stopPropagation();
        if (ev && typeof ev.preventDefault === "function") ev.preventDefault();

        const liveDrm = Boolean(video.mediaKeys) || isDrm;
        if (liveDrm) {
          btn.textContent = "DRM Protected (Unsupported)";
          btn.disabled = true;
          return;
        }

        const liveSrc = extractVideoElementSrc(video);
        const liveIsBlob = liveSrc.startsWith("blob:");

        if (onDownloadClick) {
          onDownloadClick({
            videoSrc: liveIsBlob ? "" : liveSrc,
            isMseBlob: liveIsBlob,
            pageTitle: doc.title || "video",
            frameUrl: (doc.location ? doc.location.href : ""),
            tabUrl: (doc.location ? doc.location.href : ""),
            isIframe: (typeof window !== "undefined" && window.self !== window.top),
            buttonEl: btn,
            hostEl,
            baseBtnStyle
          });
        }
      };

      btn.addEventListener("click", invokeClick);
      if (hostEl && typeof hostEl.addEventListener === "function") {
        hostEl.addEventListener("click", invokeClick);
      }

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

  const rescanAndRefresh = () => {
    const newlyAttached = scanAndAttachVideoOverlays(doc, onDownloadClick);
    if (newlyAttached.length > 0) {
      activeOverlays = activeOverlays.concat(newlyAttached);
    }
    refreshPositions();
  };

  if (win && typeof win.addEventListener === "function") {
    win.addEventListener("scroll", refreshPositions, { passive: true });
    win.addEventListener("resize", refreshPositions, { passive: true });
  }
  if (doc && typeof doc.addEventListener === "function") {
    doc.addEventListener("fullscreenchange", refreshPositions);
    doc.addEventListener("loadedmetadata", rescanAndRefresh, true);
    doc.addEventListener("canplay", rescanAndRefresh, true);
  }
  if (typeof MutationObserver !== "undefined" && doc && doc.documentElement) {
    const observer = new MutationObserver((mutations) => {
      if (Array.isArray(mutations)) {
        const onlyOverlayChange = mutations.every(m =>
          m.target && m.target.getAttribute && m.target.getAttribute("data-flux-shadow-host") === "true"
        );
        if (onlyOverlayChange) return;
      }
      rescanAndRefresh();
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
    const hasDownloadAttr = typeof anchor.hasAttribute === "function" && anchor.hasAttribute("download");

    const interceptExts = new Set([
      "zip", "rar", "7z", "tar", "gz", "bz2", "xz", "tgz", "iso", "exe", "msi", "dmg", "pdf",
      "mp4", "mkv", "webm", "mov", "avi", "flv", "mp3", "m4a", "flac", "wav", "apk", "bin", "deb", "rpm"
    ]);

    if (hasDownloadAttr || interceptExts.has(ext) || pathname.includes("/redirect") || pathname.includes("/download")) {
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

    const src = extractVideoElementSrc(target);
    if (src && !src.startsWith("blob:") && !src.startsWith("data:")) {
      const lower = src.toLowerCase();
      const streamType = lower.includes(".m3u8") ? "HLS" : (lower.includes(".mpd") ? "DASH" : "HTTP_FILE");
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addMedia",
          url: src,
          streamType,
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
    if (!ev || !ev.data) return;
    if (ev.source !== window && ev.origin !== window.location.origin) return;

    if (ev.data.action === "catCatchAddMedia") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addMedia",
          url: ev.data.url,
          streamType: ev.data.streamType || "HLS",
          ext: ev.data.ext || "mp4",
          mime: ev.data.mime || "",
          size: ev.data.size || 0,
          itag: ev.data.itag || "",
          isAudioOnly: Boolean(ev.data.isAudioOnly),
          isGoogleVideo: Boolean(ev.data.isGoogleVideo),
          videoId: ev.data.videoId || "",
          resolution: ev.data.resolution || "",
          pairedAudioUrl: ev.data.pairedAudioUrl || "",
          cachedManifestBody: ev.data.cachedManifestBody || "",
          tabUrl: location.href,
          pageTitle: ev.data.pageTitle || document.title || "video"
        }, () => {
          if (typeof chrome !== "undefined" && chrome.runtime) {
            void chrome.runtime.lastError;
          }
        });
      }
    } else if (ev.data.action === "catCatchAddKey") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addKey",
          key: ev.data.key,
          url: ev.data.url
        }, () => {
          if (typeof chrome !== "undefined" && chrome.runtime) {
            void chrome.runtime.lastError;
          }
        });
      }
    } else if (ev.data.action === "catCatchPlayerError") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "playerError",
          reason: ev.data.reason,
          url: ev.data.url || location.href
        }, () => {
          if (typeof chrome !== "undefined" && chrome.runtime) {
            void chrome.runtime.lastError;
          }
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

  // 5. Observe videos and attach overlay with active visual status & error reporting
  const onDownloadClick = (mediaMeta) => {
    const btn = mediaMeta.buttonEl;
    const hostEl = mediaMeta.hostEl;
    const baseStyle = mediaMeta.baseBtnStyle || "";

    if (btn) {
      btn.textContent = "Starting in Flux...";
      btn.disabled = true;
    }
    if (hostEl && hostEl.setAttribute) {
      hostEl.setAttribute("data-flux-status", "starting");
    }

    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      chrome.runtime.sendMessage({
        action: "downloadMedia",
        url: mediaMeta.videoSrc || "",
        isMseBlob: Boolean(mediaMeta.isMseBlob),
        pageTitle: mediaMeta.pageTitle || document.title || "video",
        tabUrl: mediaMeta.tabUrl || location.href,
        frameUrl: mediaMeta.frameUrl || location.href
      }, (resp) => {
        const lastErr = (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.lastError)
          ? chrome.runtime.lastError.message
          : null;

        if (lastErr || !resp || !resp.success) {
          const errText = lastErr || (resp && resp.error) || "Download failed";
          if (btn) {
            btn.textContent = "Error: " + (errText.length > 28 ? errText.slice(0, 28) + "..." : errText);
            btn.title = errText;
            btn.disabled = false;
            if (btn.style) btn.style.cssText = baseStyle + "border-color:#EF4444;color:#FCA5A5;";
          }
          if (hostEl && hostEl.setAttribute) {
            hostEl.setAttribute("data-flux-status", "error");
            hostEl.setAttribute("data-flux-error", errText);
          }
        } else {
          const isFallback = Boolean(resp.result && resp.result.fallbackToBrowser);
          const taskId = (resp.result && resp.result.taskId) ? resp.result.taskId : "";
          if (btn) {
            btn.textContent = isFallback ? "Browser Download" : "Queued in Flux \u2713";
            btn.title = taskId ? ("Task ID: " + taskId) : "Queued in Flux";
            btn.disabled = false;
            if (btn.style) btn.style.cssText = baseStyle + "border-color:#22C55E;color:#86EFAC;";
          }
          if (hostEl && hostEl.setAttribute) {
            hostEl.setAttribute("data-flux-status", isFallback ? "browser_fallback" : "queued");
            if (taskId) hostEl.setAttribute("data-flux-task-id", taskId);
          }
        }

        if (btn && typeof setTimeout === "function") {
          setTimeout(() => {
            btn.textContent = "Download with Flux";
            if (btn.style && baseStyle) btn.style.cssText = baseStyle;
          }, 3500);
        }
      });
    }
  };

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", () => {
      observeDynamicVideosAndFullscreen(document, window, onDownloadClick);
    }, { once: true });
  } else {
    observeDynamicVideosAndFullscreen(document, window, onDownloadClick);
  }
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
