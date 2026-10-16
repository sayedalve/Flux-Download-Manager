// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/content-script.js) & Brisk Browser Extension.
// Observes <video> elements (including dynamically inserted SPA players and iframes),
// positions the non-intrusive closed Shadow DOM "Download with OpenIDM" badge at the top-right
// of the video player (never covering bottom transport controls), and repositions on
// scroll, resize, and fullscreen transitions (Phase 7 & BUG-013 Fix).

function positionOverlayHost(video, hostEl) {
  const rect = typeof video.getBoundingClientRect === "function"
    ? video.getBoundingClientRect()
    : { width: video.width || 640, height: video.height || 360, top: 20, left: 20, right: 660 };

  // Place badge 10px from top-right corner of the video element so bottom player controls are never obscured
  const topPx = Math.max(8, Math.round((rect.top || 20) + 10));
  const rightEdge = rect.right || ((rect.left || 20) + (rect.width || 640));
  const leftPx = Math.max(8, Math.round(rightEdge - 168));

  hostEl.style.cssText =
    `position:fixed;top:${topPx}px;left:${leftPx}px;z-index:2147483647;pointer-events:auto;`;
}

function scanAndAttachVideoOverlays(doc, onDownloadClick, minWidth = 240, minHeight = 135) {
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

    if (!video.dataset.openidmOverlayAttached) {
      video.dataset.openidmOverlayAttached = "true";

      const hostEl = doc.createElement("div");
      hostEl.setAttribute("data-openidm-shadow-host", "true");
      positionOverlayHost(video, hostEl);

      // Closed Shadow DOM ensures zero CSS leakage into or from the host webpage
      const shadow = hostEl.attachShadow ? hostEl.attachShadow({ mode: "closed" }) : hostEl;
      const btn = doc.createElement("button");
      btn.type = "button";
      btn.textContent = isDrm ? "DRM Protected (Unsupported)" : "Download with OpenIDM";
      btn.disabled = isDrm;
      if (btn.style) {
        btn.style.cssText =
          "background:#18181B;color:#F4F4F5;border:1px solid #3B82F6;border-radius:6px;" +
          "padding:5px 10px;font:600 12px 'Segoe UI',sans-serif;cursor:pointer;box-shadow:0 2px 8px rgba(0,0,0,0.45);";
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

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    positionOverlayHost,
    scanAndAttachVideoOverlays,
    observeDynamicVideosAndFullscreen
  };
}
