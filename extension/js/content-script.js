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
    if (!src) {
      continue;
    }
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
          gap: 8px;
          background: #2D2E32;
          color: #F2F2F3;
          border: 1px solid #414248;
          border-radius: 7px;
          padding: 7px 14px;
          font: 600 13px "Segoe UI Variable Text", "Segoe UI", -apple-system, BlinkMacSystemFont, sans-serif;
          letter-spacing: 0.01em;
          cursor: pointer;
          opacity: 0.94;
          box-shadow: 0 4px 14px rgba(0, 0, 0, 0.55);
          transition: opacity 0.14s ease, border-color 0.14s ease, background 0.14s ease, transform 0.1s ease;
        }
        button::before {
          content: "";
          width: 7px;
          height: 7px;
          border-radius: 50%;
          background: #FF4F57;
          box-shadow: 0 0 8px rgba(255, 79, 87, 0.75);
          flex-shrink: 0;
        }
        button:hover:not(:disabled), button:focus-visible {
          opacity: 1;
          border-color: #FF4F57;
          background: #37383D;
          color: #FFFFFF;
          outline: none;
        }
        button:active:not(:disabled) {
          transform: scale(0.98);
        }
        button:disabled {
          opacity: 0.72;
          border-color: #414248;
          color: #A4A5AC;
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
        "background:#2D2E32;color:#F2F2F3;border:1px solid #414248;border-radius:7px;" +
        "padding:7px 14px;font:600 13px 'Segoe UI Variable Text','Segoe UI',sans-serif;cursor:pointer;box-shadow:0 4px 14px rgba(0,0,0,0.55);";
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


function findAnchorFromEvent(ev) {
  if (!ev) return null;
  if (typeof ev.composedPath === "function") {
    try {
      const path = ev.composedPath();
      for (const el of path) {
        if (el && el.nodeType === 1 && typeof el.matches === "function") {
          if (el.matches("a[href], [data-download-url], [data-href]")) {
            return el;
          }
        }
      }
    } catch (_) {}
  }
  let target = ev.target;
  if (target && target.nodeType === 3) {
    target = target.parentElement;
  }
  if (target && typeof target.closest === "function") {
    return target.closest("a[href], [data-download-url], [data-href]");
  }
  return null;
}

function handleLinkClick(ev) {
  try {
    const correlationId = "click-" + Date.now().toString(36) + "-" + Math.random().toString(36).slice(2, 8);

    const anchor = findAnchorFromEvent(ev);
    if (!anchor) return;

    let href = anchor.href || (typeof anchor.getAttribute === "function" ? (anchor.getAttribute("data-download-url") || anchor.getAttribute("data-href")) : "") || "";
    if (!href) return;

    let isFrame = false;
    try { isFrame = (window !== window.top); } catch (_) { isFrame = true; }
    let referrer = "";
    try { referrer = location.href; } catch (_) {}
    let pageTitle = "";
    try { pageTitle = document.title; } catch (_) {}

    const isBlobOrData = href.startsWith("blob:") || href.startsWith("data:");
    let mechanism = isBlobOrData ? "BROWSER_BLOB_OR_DATA" : (isFrame ? "FRAME_ORIGINATED" : "ORDINARY_LINK");
    if (!ev.isTrusted) {
      mechanism = "JS_TRIGGERED_DYNAMIC_ANCHOR";
    }

    console.log(`[Flux Trace] [CLICK_SEEN] [${correlationId}]`, {
      url: isBlobOrData ? (href.slice(0, 48) + "...") : href,
      target: anchor.tagName,
      button: ev.button,
      altKey: ev.altKey,
      mechanism,
      isFrame
    });

    if (ev.altKey) {
      // IDM Alt-key bypass behavior: user explicitly requested browser-native download
      console.log(`[Flux Trace] [CLICK_IGNORED] [${correlationId}]`, { url: isBlobOrData ? (href.slice(0, 48) + "...") : href, reason: "ALT_KEY_BYPASS", mechanism });
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({ action: "bypassNextDownload", correlationId });
      }
      return;
    }

    // 1. Browser-generated in-memory Blob and Data URLs:
    // Intercept client-side files and stream bytes directly into Flux via chunked native transfer.
    if (isBlobOrData) {
      if (anchor.dataset && anchor.dataset.fluxBypass === "true") {
        delete anchor.dataset.fluxBypass;
        console.log(`[Flux Trace] [CLICK_IGNORED] [${correlationId}]`, { url: href.slice(0, 48) + "...", reason: "EXPLICIT_BYPASS_FLAG", mechanism });
        return;
      }

      if (typeof ev.preventDefault === "function") ev.preventDefault();
      if (typeof ev.stopPropagation === "function") ev.stopPropagation();
      if (typeof ev.stopImmediatePropagation === "function") ev.stopImmediatePropagation();

      console.log(`[Flux Trace] [INTERCEPTION_STARTED] [${correlationId}]`, {
        url: href.slice(0, 48) + "...",
        filename: anchor.download || "",
        mechanism
      });

      // Request main-world catch.js to stream the in-memory Blob/Data content
      window.postMessage({
        action: "fluxRequestBlobTransfer",
        href,
        filename: anchor.download || "",
        correlationId
      }, "*");
      return;
    }

    if (ev.button !== undefined && ev.button !== 0 && ev.button !== 1) {
      console.log(`[Flux Trace] [CLICK_IGNORED] [${correlationId}]`, { url: href, reason: "UNSUPPORTED_BUTTON", mechanism });
      return;
    }

    // Check if this anchor was already marked for browser fallback
    if (anchor.dataset && anchor.dataset.fluxBypass === "true") {
      delete anchor.dataset.fluxBypass;
      console.log(`[Flux Trace] [CLICK_IGNORED] [${correlationId}]`, { url: href, reason: "EXPLICIT_BYPASS_FLAG", mechanism });
      return;
    }

    if (href.startsWith("javascript:") || href.startsWith("#") || href.startsWith("mailto:")) {
      console.log(`[Flux Trace] [CLICK_IGNORED] [${correlationId}]`, { url: href, reason: "NON_HTTP_PROTOCOL", mechanism });
      return;
    }

    let pathname = anchor.pathname || "";
    if (!pathname && typeof URL !== "undefined") {
      try { pathname = new URL(href, (typeof location !== "undefined" ? location.href : undefined)).pathname; } catch (_) {}
    }
    const lastDot = pathname.lastIndexOf(".");
    const ext = lastDot !== -1 ? pathname.slice(lastDot + 1).toLowerCase() : "";
    const hasDownloadAttr = typeof anchor.hasAttribute === "function" && anchor.hasAttribute("download");

    const interceptExts = new Set([
      "zip", "rar", "7z", "tar", "gz", "bz2", "xz", "tgz", "iso", "exe", "msi", "dmg", "pdf",
      "apk", "bin", "deb", "rpm", "docx", "xlsx", "pptx", "csv", "epub",
      "mp4", "mkv", "webm", "mov", "avi", "flv", "mp3", "m4a", "flac", "wav"
    ]);

    let isEligible = hasDownloadAttr || interceptExts.has(ext) ||
      pathname.includes("/redirect") || pathname.includes("/download") || pathname.includes("/dl/");

    if (!isEligible && typeof URL !== "undefined") {
      try {
        const parsed = new URL(href, (typeof location !== "undefined" ? location.href : undefined));
        for (const [key, val] of parsed.searchParams.entries()) {
          const kLower = key.toLowerCase();
          if (kLower.includes("download") || kLower === "dl" || kLower === "file" || kLower === "filename") {
            const vDot = val.lastIndexOf(".");
            const vExt = vDot !== -1 ? val.slice(vDot + 1).toLowerCase() : "";
            if (interceptExts.has(vExt) || val === "1" || val === "true" || kLower.includes("download")) {
              isEligible = true;
              break;
            }
          }
          const vDot = val.lastIndexOf(".");
          if (vDot !== -1 && interceptExts.has(val.slice(vDot + 1).toLowerCase())) {
            isEligible = true;
            break;
          }
        }
      } catch (_) {}
    }

    if (!isEligible) {
      console.log(`[Flux Trace] [CLICK_IGNORED] [${correlationId}]`, {
        url: href,
        reason: "NOT_DOWNLOAD_ELIGIBLE",
        mechanism: "UNKNOWN_FILENAME_DYNAMIC",
        note: "Deferred to browser network response for Content-Disposition determination"
      });
      return;
    }

    console.log(`[Flux Trace] [INTERCEPTION_STARTED] [${correlationId}]`, {
      url: href,
      filename: anchor.download || "",
      hasDownloadAttr,
      mechanism
    });

    // Prevent duplicate in-flight click offers from rapid double-clicks on the same control
    if (anchor.__fluxClickInFlight) {
      console.log(`[Flux Trace] [CLICK_IGNORED] [${correlationId}]`, { url: href, reason: "DUPLICATE_IN_FLIGHT", mechanism });
      return;
    }
    anchor.__fluxClickInFlight = true;
    setTimeout(() => { delete anchor.__fluxClickInFlight; }, 4000);

    // Record link click metadata for correlation
    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      chrome.runtime.sendMessage({
        action: "linkClicked",
        url: href,
        filename: anchor.download || "",
        hasDownloadAttr,
        referrer,
        pageTitle,
        bypass: false,
        mechanism,
        isFrame,
        correlationId
      });
    }

    // Intercept click prior to browser's default download action
    // If not a modified click (Ctrl/Cmd/Shift), prevent default, stop immediate propagation, and offer to Flux
    if (!ev.ctrlKey && !ev.metaKey && !ev.shiftKey && typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      if (typeof ev.preventDefault === "function") ev.preventDefault();
      if (typeof ev.stopPropagation === "function") ev.stopPropagation();
      if (typeof ev.stopImmediatePropagation === "function") ev.stopImmediatePropagation();

      console.log(`[Flux Trace] [BROWSER_ACTION_CANCELLED] [${correlationId}]`, { url: href, mechanism });

      chrome.runtime.sendMessage({
        action: "interceptDownloadClick",
        url: href,
        filename: anchor.download || "",
        hasDownloadAttr,
        referrer,
        pageTitle,
        mechanism,
        isFrame,
        correlationId
      }, (response) => {
        delete anchor.__fluxClickInFlight;
        const lastErr = (chrome.runtime && chrome.runtime.lastError) ? chrome.runtime.lastError.message : null;
        if (lastErr) {
          console.warn(`[Flux Trace] [INTERCEPTION_FAILED] [${correlationId}]`, { error: lastErr, mechanism, stage: "STAGE_FAILURE_RECOVERY" });
          triggerBrowserFallback(anchor, href, "RUNTIME_ERROR: " + lastErr, correlationId, mechanism);
          return;
        }
        if (!response) {
          console.warn(`[Flux Trace] [INTERCEPTION_FAILED] [${correlationId}]`, { error: "NO_RESPONSE", mechanism, stage: "STAGE_FAILURE_RECOVERY" });
          triggerBrowserFallback(anchor, href, "NO_RESPONSE", correlationId, mechanism);
          return;
        }
        if (!response.accepted) {
          const reason = response.error || response.reason || "DECLINED";
          const cat = response.errorCategory ? ` [${response.errorCategory}]` : "";
          console.warn(`[Flux Trace] [INTERCEPTION_FAILED] [${correlationId}]`, { reason, category: cat, mechanism, stage: "STAGE_FAILURE_RECOVERY" });
          triggerBrowserFallback(anchor, href, reason, correlationId, mechanism);
          return;
        }
        // ACCEPTED: The download is now safely accepted in Flux!
        // Never trigger fallback for successfully accepted downloads.
        console.log(`[Flux Trace] [DOWNLOAD_OFFER_ACCEPTED] [${correlationId}]`, {
          taskId: response.taskId,
          fileName: response.fileName,
          mechanism
        });
      });
    }
  } catch (err) {
    console.error("[Flux] Error in handleLinkClick:", err);
  }
}

function triggerBrowserFallback(anchor, href, reason = "", correlationId = "", mechanism = "") {
  try {
    if (!href) return;

    if (href.startsWith("blob:") || href.startsWith("data:")) {
      console.log(`[Flux Trace] [BROWSER_FALLBACK_STARTED] [${correlationId || "fallback"}]`, {
        url: href.slice(0, 48) + "...",
        reason,
        mechanism
      });
      if (anchor) {
        anchor.dataset.fluxBypass = "true";
        try { anchor.click(); } catch (_) {}
      }
      return;
    }

    console.log(`[Flux Trace] [BROWSER_FALLBACK_STARTED] [${correlationId || "fallback"}]`, { url: href, reason, mechanism });

    // Guard against recursive fallback loops on the same URL
    if (typeof window !== "undefined") {
      window.__fluxFallbackActive = window.__fluxFallbackActive || new Set();
      if (window.__fluxFallbackActive.has(href)) {
        console.warn("[Flux] Recursive fallback prevented for:", href);
        return;
      }
      window.__fluxFallbackActive.add(href);
      setTimeout(() => {
        if (window.__fluxFallbackActive) window.__fluxFallbackActive.delete(href);
      }, 5000);
    }

    if (anchor && anchor.dataset) {
      anchor.dataset.fluxBypass = "true";
    }
    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      chrome.runtime.sendMessage({
        action: "bypassNextDownload",
        url: href,
        reason: reason || "FALLBACK",
        mechanism,
        correlationId
      });
    }

    if (anchor && typeof anchor.click === "function") {
      anchor.click();
    } else if (typeof location !== "undefined" && location.assign) {
      location.assign(href);
    }
  } catch (err) {
    console.error("[Flux] Error in triggerBrowserFallback:", err);
  }
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

function extractEligiblePageLinks(doc) {
  if (!doc || !doc.querySelectorAll) return [];
  const anchors = Array.from(doc.querySelectorAll("a[href]"));
  const seen = new Set();
  const results = [];

  const extCategories = {
    zip: "Compressed", rar: "Compressed", "7z": "Compressed", tar: "Compressed", gz: "Compressed", bz2: "Compressed", xz: "Compressed",
    pdf: "Documents", doc: "Documents", docx: "Documents", xls: "Documents", xlsx: "Documents", ppt: "Documents", pptx: "Documents", epub: "Documents", txt: "Documents", csv: "Documents",
    exe: "Applications", msi: "Applications", dmg: "Applications", pkg: "Applications", deb: "Applications", rpm: "Applications", apk: "Applications",
    mp4: "Video", mkv: "Video", webm: "Video", mov: "Video", avi: "Video", flv: "Video",
    mp3: "Music", m4a: "Music", flac: "Music", wav: "Music", ogg: "Music",
    jpg: "Images", jpeg: "Images", png: "Images", gif: "Images", webp: "Images", svg: "Images"
  };

  for (const a of anchors) {
    const rawHref = a.getAttribute("href") || "";
    if (!rawHref || rawHref.startsWith("#") || rawHref.startsWith("javascript:") || rawHref.startsWith("mailto:") || rawHref.startsWith("data:")) {
      continue;
    }
    let fullUrl = a.href;
    try { fullUrl = new URL(rawHref, doc.location ? doc.location.href : undefined).href; } catch (_) {}
    if (!fullUrl.startsWith("http://") && !fullUrl.startsWith("https://")) continue;
    if (seen.has(fullUrl)) continue;
    seen.add(fullUrl);

    let pathname = "";
    try { pathname = new URL(fullUrl).pathname; } catch (_) {}
    const rawFilename = (a.download || pathname.split("/").pop() || "download").split("?")[0];
    const lastDot = rawFilename.lastIndexOf(".");
    const ext = lastDot !== -1 ? rawFilename.slice(lastDot + 1).toLowerCase() : "";
    const category = extCategories[ext] || "Other";
    const linkText = (a.textContent || a.title || rawFilename).trim().replace(/\s+/g, " ");

    results.push({
      url: fullUrl,
      filename: rawFilename || "file",
      ext: ext || "file",
      category,
      text: linkText.slice(0, 80),
      isDirectFile: Boolean(extCategories[ext])
    });
  }
  return results;
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

  // 1. Intercept link clicks with capture phase at both window and document level
  window.addEventListener("click", handleLinkClick, { capture: true, passive: false });
  window.addEventListener("auxclick", handleLinkClick, { capture: true, passive: false });
  document.addEventListener("click", handleLinkClick, { capture: true, passive: false });
  document.addEventListener("auxclick", handleLinkClick, { capture: true, passive: false });

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
      if (typeof window !== "undefined") {
        window.__fluxLastCandidate = ev.data;
      }
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addMedia",
          url: ev.data.url,
          streamType: ev.data.streamType || "HLS",
          sourceType: ev.data.sourceType || "",
          ext: ev.data.ext || "mp4",
          mime: ev.data.mime || "",
          size: ev.data.size || 0,
          itag: ev.data.itag || "",
          isAudioOnly: Boolean(ev.data.isAudioOnly),
          isGoogleVideo: Boolean(ev.data.isGoogleVideo),
          isAdaptiveTrack: Boolean(ev.data.isAdaptiveTrack),
          requiresAudioMux: Boolean(ev.data.requiresAudioMux),
          requiresCipherExtraction: Boolean(ev.data.requiresCipherExtraction),
          videoId: ev.data.videoId || "",
          resolution: ev.data.resolution || "",
          videoUrl: ev.data.videoUrl || "",
          pairedAudioUrl: ev.data.pairedAudioUrl || "",
          qualities: Array.isArray(ev.data.qualities) ? ev.data.qualities : [],
          extractorJson: ev.data.extractorJson || "",
          candidateId: ev.data.candidateId || "",
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
    } else if (ev.data.action === "catCatchJsLinkClick") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "interceptDownloadClick",
          url: ev.data.url,
          filename: ev.data.filename || "",
          hasDownloadAttr: Boolean(ev.data.hasDownloadAttr),
          referrer: ev.data.referrer || location.href,
          pageTitle: ev.data.pageTitle || document.title,
          mechanism: "JS_TRIGGERED_PROGRAMMATIC_CLICK",
          isFrame: (window !== window.top),
          correlationId: ev.data.correlationId
        });
      }
    } else if (ev.data.action === "fluxStartBlobTransfer") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "startChunkedDownload",
          transferId: ev.data.transferId,
          correlationId: ev.data.correlationId,
          filename: ev.data.filename,
          totalBytes: ev.data.totalBytes,
          mimeType: ev.data.mimeType,
          pageUrl: ev.data.pageUrl || location.href
        }, (resp) => {
          const lastErr = (chrome.runtime && chrome.runtime.lastError) ? chrome.runtime.lastError.message : null;
          if (lastErr || (resp && !resp.accepted)) {
            window.postMessage({ action: "fluxFallbackBlob", transferId: ev.data.transferId }, "*");
          }
        });
      }
    } else if (ev.data.action === "fluxBlobChunk") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "transferDownloadChunk",
          transferId: ev.data.transferId,
          correlationId: ev.data.correlationId,
          chunkIndex: ev.data.chunkIndex,
          totalChunks: ev.data.totalChunks,
          dataBase64: ev.data.dataBase64
        });
      }
    } else if (ev.data.action === "fluxCompleteBlobTransfer") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "completeChunkedDownload",
          transferId: ev.data.transferId,
          correlationId: ev.data.correlationId,
          totalBytes: ev.data.totalBytes
        }, (resp) => {
          const lastErr = (chrome.runtime && chrome.runtime.lastError) ? chrome.runtime.lastError.message : null;
          if (lastErr || (resp && !resp.success)) {
            window.postMessage({ action: "fluxFallbackBlob", transferId: ev.data.transferId }, "*");
          }
        });
      }
    } else if (ev.data.action === "fluxAbortBlobTransfer") {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "abortChunkedDownload",
          transferId: ev.data.transferId,
          correlationId: ev.data.correlationId,
          reason: ev.data.reason
        });
      }
      window.postMessage({ action: "fluxFallbackBlob", transferId: ev.data.transferId }, "*");
    }
  }, { capture: true });

  // 3b. Handle background probePageMedia & extractPageLinks RPC
  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.onMessage) {
    chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
      if (request && request.action === "probePageMedia") {
        try {
          window.postMessage({ action: "fluxScanPageMedia" }, "*");
          sendResponse({ success: true });
        } catch (e) {
          sendResponse({ success: false, error: e.message });
        }
        return true;
      }
      if (request && request.action === "extractPageLinks") {
        try {
          const links = extractEligiblePageLinks(document);
          sendResponse({ success: true, links });
        } catch (e) {
          sendResponse({ success: false, error: e.message, links: [] });
        }
        return true;
      }
    });
  }

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

    const lastCand = (typeof window !== "undefined" && window.__fluxLastCandidate) ? window.__fluxLastCandidate : null;
    const candidateId = mediaMeta.candidateId || (lastCand && lastCand.candidateId) || "";
    const qualityId = mediaMeta.qualityId || (lastCand && lastCand.qualityId) || "";
    const videoSrc = mediaMeta.videoSrc || (lastCand ? (lastCand.videoUrl || lastCand.url) : "") || "";

    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      chrome.runtime.sendMessage({
        action: "downloadMedia",
        candidateId,
        qualityId,
        url: videoSrc,
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
    findAnchorFromEvent,
    handleLinkClick,
    handleMediaPlaybackEvent,
    extractEligiblePageLinks,
    initContentScript
  };
}
