// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/content-script.js) & Brisk Browser Extension.
// Observes <video> elements (including dynamic insertion & resize), verifies candidate/source
// presence before displaying or submitting from the closed Shadow DOM "Download with Flux" badge,
// performs active discovery for blob: URLs, extracts & syncs multi-quality options (1080p/720p/480p/360p/Audio)
// automatically on video playback, provides instant download handoff + clear error feedback on click,
// intercepts link clicks (with Alt-key bypass and correlation IDs), and relays MAIN world events.

function makeContentCorrelationId(prefix = "cs") {
  return `${prefix}-${Date.now().toString(36)}-${Math.random().toString(36).slice(2, 7)}`;
}

function positionOverlayHost(video, hostEl) {
  const rect = typeof video.getBoundingClientRect === "function"
    ? video.getBoundingClientRect()
    : { width: video.width || 640, height: video.height || 360, top: 20, left: 20, right: 660 };

  // Place badge 10px from top-right corner of the video element so bottom transport controls are never obscured
  const topPx = Math.max(8, Math.round((rect.top || 20) + 10));
  const rightEdge = rect.right || ((rect.left || 20) + (rect.width || 640));
  const leftPx = Math.max(8, Math.round(rightEdge - 168));

  const nextCss = `position:fixed;top:${topPx}px;left:${leftPx}px;z-index:2147483647;pointer-events:auto;display:flex;align-items:center;gap:4px;`;
  if (hostEl.__fluxLastCss !== nextCss) {
    hostEl.__fluxLastCss = nextCss;
    hostEl.style.cssText = nextCss;
  }
}

function extractVideoElementSrc(video) {
  if (!video) return "";
  let src = video.currentSrc || video.src || "";
  if (!src && typeof video.getAttribute === "function") {
    src = video.getAttribute("src") || "";
  }
  if (!src && typeof video.querySelector === "function") {
    const sourceEl = video.querySelector("source[src]");
    if (sourceEl) src = sourceEl.src || (typeof sourceEl.getAttribute === "function" ? sourceEl.getAttribute("src") : "") || "";
  }
  return src;
}

function extractVideoElementQualities(video) {
  if (!video) return [];
  const qualities = [];
  const seen = new Set();

  if (typeof video.querySelectorAll === "function") {
    const sources = Array.from(video.querySelectorAll("source")) || [];
    for (const s of sources) {
      const sUrl = s.src || (typeof s.getAttribute === "function" ? s.getAttribute("src") : "") || "";
      if (!sUrl || sUrl.startsWith("blob:") || sUrl.startsWith("data:")) continue;
      const labelAttr = (typeof s.getAttribute === "function")
        ? (s.getAttribute("label") || s.getAttribute("data-quality") || s.getAttribute("data-res") || s.getAttribute("res") || s.getAttribute("size") || "")
        : "";
      const urlResM = sUrl.match(/(?:^|[_\-/\.])(2160p|1440p|1080p|720p|480p|360p|240p|144p)(?:[_\-/\.\?&]|$)/i);
      const rawRes = labelAttr
        ? (/^\d+$/.test(labelAttr) ? `${labelAttr}p` : labelAttr)
        : (urlResM ? urlResM[1].toLowerCase() : "");
      const hM = rawRes.match(/(\d{3,4})/);
      const h = hM ? parseInt(hM[1], 10) : 0;
      const id = (rawRes || `src-${qualities.length}`).toLowerCase();
      if (seen.has(id)) continue;
      seen.add(id);
      const ext = sUrl.toLowerCase().includes(".webm") ? "webm" : "mp4";
      qualities.push({
        id,
        label: rawRes ? `${rawRes} (${ext.toUpperCase()})` : `Direct (${ext.toUpperCase()})`,
        resolution: rawRes || "HD",
        height: h,
        ext,
        videoUrl: sUrl,
        videoFilter: rawRes || "best"
      });
    }
  }

  const directSrc = extractVideoElementSrc(video);
  if (directSrc && !directSrc.startsWith("blob:") && !directSrc.startsWith("data:") && qualities.length === 0) {
    const liveH = Number(video.videoHeight || 0);
    const urlResM = directSrc.match(/(?:^|[_\-/\.])(2160p|1440p|1080p|720p|480p|360p|240p|144p)(?:[_\-/\.\?&]|$)/i);
    const resLabel = liveH > 0 ? `${liveH}p` : (urlResM ? urlResM[1].toLowerCase() : "");
    if (resLabel) {
      const ext = directSrc.toLowerCase().includes(".webm") ? "webm" : "mp4";
      qualities.push({
        id: resLabel.toLowerCase(),
        label: `${resLabel} (${ext.toUpperCase()})`,
        resolution: resLabel,
        height: liveH || parseInt(resLabel, 10) || 0,
        ext,
        videoUrl: directSrc,
        videoFilter: resLabel
      });
    }
  }

  qualities.sort((a, b) => (b.height || 0) - (a.height || 0));
  return qualities;
}

function triggerBlobDiscovery(video, blobUrl, doc) {
  const corrId = makeContentCorrelationId("blobdisc");
  const pageUrl = (doc && doc.location) ? doc.location.href : (typeof location !== "undefined" ? location.href : "");
  const pageTitle = (doc && doc.title) ? doc.title : (typeof document !== "undefined" ? document.title : "video");

  if (typeof window !== "undefined" && typeof window.postMessage === "function") {
    try {
      window.postMessage({
        action: "fluxDiscoverBlobSource",
        blobUrl,
        correlationId: corrId,
        pageUrl
      }, "*");
    } catch (_) {}
  }

  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
    try {
      chrome.runtime.sendMessage({
        action: "discoverBlobMedia",
        blobUrl,
        pageTitle,
        tabUrl: pageUrl,
        correlationId: corrId
      }, (resp) => {
        if (typeof chrome !== "undefined" && chrome.runtime) {
          void chrome.runtime.lastError;
        }
        if (video && resp) {
          if (resp.hasCandidate && resp.candidate) {
            video.dataset.fluxCandidateReady = "true";
            video.dataset.fluxCandidateId = resp.candidate.candidateId || "";
            video.dataset.fluxSourceType = resp.candidate.sourceType || resp.candidate.streamType || "";
            if (Array.isArray(resp.candidate.qualities) && resp.candidate.qualities.length > 0) {
              video.__fluxQualities = resp.candidate.qualities;
              if (typeof video.__fluxUpdateQualities === "function") {
                video.__fluxUpdateQualities(resp.candidate.qualities, resp.candidate.resolution);
              }
            }
          } else {
            video.dataset.fluxCandidateReady = "false";
            if (resp.playerError) {
              video.dataset.fluxPlayerError = resp.playerError;
            }
          }
        }
      });
    } catch (_) {}
  }
  return corrId;
}

function notifyDirectVideoElementDiscovered(video, doc) {
  try {
    const src = extractVideoElementSrc(video);
    const domQualities = extractVideoElementQualities(video);
    if (domQualities.length > 0) {
      video.__fluxQualities = domQualities;
    }
    if (!src || src.startsWith("data:")) return;

    // Active discovery for blob: URLs instead of ignoring them (Defect A fix)
    if (src.startsWith("blob:")) {
      if (video.__fluxLastReportedBlob !== src) {
        video.__fluxLastReportedBlob = src;
        triggerBlobDiscovery(video, src, doc);
      }
      return;
    }

    const liveH = Number(video.videoHeight || 0);
    const liveRes = liveH > 0 ? `${liveH}p` : (domQualities.length > 0 ? domQualities[0].resolution : "");
    const reportKey = `${src}|${liveRes}|${domQualities.length}`;
    if (video.__fluxLastReportedSrc === reportKey) return;
    video.__fluxLastReportedSrc = reportKey;
    video.dataset.fluxCandidateReady = "true";

    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      const lower = src.toLowerCase();
      const isHls = lower.includes(".m3u8") || lower.includes(".m3u");
      const isDash = lower.includes(".mpd");
      const streamType = isHls ? "HLS" : (isDash ? "DASH" : "HTTP_FILE");
      const sourceType = isHls ? "HLS_MANIFEST" : (isDash ? "DASH_MANIFEST" : "DIRECT_PROGRESSIVE");
      chrome.runtime.sendMessage({
        action: "addMedia",
        url: src,
        streamType,
        sourceType,
        resolution: liveRes,
        qualities: domQualities,
        correlationId: makeContentCorrelationId("domvid"),
        pageTitle: (doc && doc.title) ? doc.title : "video",
        tabUrl: (doc && doc.location) ? doc.location.href : ""
      }, (resp) => {
        if (typeof chrome !== "undefined" && chrome.runtime) {
          void chrome.runtime.lastError;
        }
        if (resp && resp.candidate) {
          if (resp.candidate.candidateId && video.dataset) {
            video.dataset.fluxCandidateId = resp.candidate.candidateId;
          }
          if (Array.isArray(resp.candidate.qualities) && resp.candidate.qualities.length > 0) {
            video.__fluxQualities = resp.candidate.qualities;
            if (typeof video.__fluxUpdateQualities === "function") {
              video.__fluxUpdateQualities(resp.candidate.qualities, resp.candidate.resolution);
            }
          }
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
    const hasCandidateHint = video.dataset && video.dataset.fluxCandidateReady === "true";

    // Defect A fix: Do not attach the download button to an empty <video> element that has
    // no media source, no DRM keys, and no detected media candidate on the page.
    if (!src && !isDrm && !hasCandidateHint) {
      continue;
    }

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
      btn.className = "flux-btn";
      if (btn.classList && typeof btn.classList.add === "function") {
        btn.classList.add("flux-btn");
      }
      btn.textContent = isDrm ? "DRM Protected (Unsupported)" : "Download with Flux";
      btn.disabled = isDrm;
      const baseBtnStyle =
        "background:#090D16;color:#F8FAFC;border:1px solid #2563EB;border-radius:6px;" +
        "padding:6px 12px;font:600 12px 'Segoe UI Variable Text','Segoe UI',system-ui,sans-serif;" +
        "letter-spacing:0.01em;cursor:pointer;box-shadow:0 4px 12px rgba(6,9,19,0.68);";
      if (btn.style) {
        btn.style.cssText = isDrm
          ? (baseBtnStyle + "border-color:#475569;color:#94A3B8;cursor:not-allowed;opacity:0.8;")
          : baseBtnStyle;
      }

      // Quality selector dropdown inside Closed Shadow DOM (IDM floating resolution selector equivalent)
      const qualitySelect = doc.createElement("select");
      qualitySelect.className = "flux-quality-select";
      if (qualitySelect.classList && typeof qualitySelect.classList.add === "function") {
        qualitySelect.classList.add("flux-quality-select");
      }
      const selectStyle =
        "background:#0F172A;color:#F8FAFC;border:1px solid #26334D;border-radius:6px;" +
        "padding:5px 8px;font:600 11px 'Segoe UI Variable Text','Segoe UI',system-ui,sans-serif;cursor:pointer;" +
        "box-shadow:0 4px 12px rgba(6,9,19,0.68);margin-right:4px;";
      if (qualitySelect.style) {
        qualitySelect.style.cssText = selectStyle + "display:none;";
      }
      if (typeof qualitySelect.addEventListener === "function") {
        qualitySelect.addEventListener("click", (ev) => {
          if (ev && typeof ev.stopPropagation === "function") ev.stopPropagation();
        });
      }

      const updateQualities = (qualitiesList, defaultRes = "") => {
        const qArr = Array.isArray(qualitiesList) ? qualitiesList : [];
        video.__fluxQualities = qArr;
        if (video.dataset) {
          video.dataset.fluxQualities = qArr.map(q => q.resolution || q.id).join(",");
        }
        if (hostEl && typeof hostEl.setAttribute === "function") {
          hostEl.setAttribute("data-flux-qualities", qArr.map(q => q.resolution || q.id).join(","));
        }
        if (!qualitySelect) return;

        if (qArr.length > 0) {
          if (qualitySelect.style) {
            qualitySelect.style.cssText = selectStyle + "display:inline-block;";
          }
          if (Array.isArray(qualitySelect.children)) {
            qualitySelect.children.length = 0;
          }
          qualitySelect.innerHTML = "";
          for (const q of qArr) {
            const opt = doc.createElement("option");
            opt.value = q.id || q.resolution || "best";
            opt.textContent = q.label || q.resolution || "Auto";
            if (typeof qualitySelect.appendChild === "function") {
              qualitySelect.appendChild(opt);
            }
          }
          const initial = qArr[0];
          if (initial && !qualitySelect.value) {
            qualitySelect.value = initial.id || initial.resolution || "best";
          }
        } else if (defaultRes && defaultRes !== "Auto") {
          if (hostEl && typeof hostEl.setAttribute === "function") {
            hostEl.setAttribute("data-flux-qualities", defaultRes);
          }
        }
      };

      video.__fluxUpdateQualities = updateQualities;
      if (Array.isArray(video.__fluxQualities) && video.__fluxQualities.length > 0) {
        updateQualities(video.__fluxQualities);
      }

      const invokeClick = (ev) => {
        if (ev && ev.target === qualitySelect) return;
        if (ev && typeof ev.stopPropagation === "function") ev.stopPropagation();
        if (ev && typeof ev.preventDefault === "function") ev.preventDefault();

        const liveDrm = Boolean(video.mediaKeys) || isDrm;
        if (liveDrm) {
          btn.textContent = "DRM Protected (Unsupported)";
          btn.disabled = true;
          if (hostEl && hostEl.setAttribute) {
            hostEl.setAttribute("data-flux-status", "drm_unsupported");
          }
          return;
        }

        const liveSrc = extractVideoElementSrc(video);
        const liveIsBlob = liveSrc.startsWith("blob:");
        const candidateId = (video.dataset && video.dataset.fluxCandidateId) ? video.dataset.fluxCandidateId : "";

        // Never silently submit an empty URL when no source or candidate exists
        if (!liveSrc && !candidateId && (!video.dataset || video.dataset.fluxCandidateReady !== "true")) {
          const msg = "No supported download source";
          btn.textContent = msg;
          btn.title = "This video element has no downloadable stream or media URL.";
          if (btn.style) btn.style.cssText = baseBtnStyle + "border-color:#EF4444;color:#FCA5A5;";
          if (hostEl && hostEl.setAttribute) {
            hostEl.setAttribute("data-flux-status", "error");
            hostEl.setAttribute("data-flux-error", msg);
          }
          return;
        }

        if (liveIsBlob) {
          triggerBlobDiscovery(video, liveSrc, doc);
        }

        const qList = Array.isArray(video.__fluxQualities) ? video.__fluxQualities : [];
        const chosenQId = (qualitySelect && qualitySelect.value) ? qualitySelect.value : (qList[0] ? qList[0].id : "");
        const selectedQuality = qList.find(q => (q.id === chosenQId || q.resolution === chosenQId)) || qList[0] || null;

        if (onDownloadClick) {
          onDownloadClick({
            videoSrc: (selectedQuality && selectedQuality.videoUrl && !liveIsBlob)
              ? selectedQuality.videoUrl
              : (liveIsBlob ? "" : liveSrc),
            blobUrl: liveIsBlob ? liveSrc : "",
            candidateId,
            qualityId: chosenQId || "",
            selectedQuality,
            resolution: selectedQuality ? selectedQuality.resolution : "",
            videoFilter: selectedQuality ? (selectedQuality.videoFilter || selectedQuality.resolution || "best") : "best",
            isMseBlob: liveIsBlob,
            correlationId: makeContentCorrelationId("vidbtn"),
            pageTitle: doc.title || "video",
            frameUrl: (doc.location ? doc.location.href : ""),
            tabUrl: (doc.location ? doc.location.href : ""),
            isIframe: (typeof window !== "undefined" && window.self !== window.top),
            buttonEl: btn,
            qualitySelectEl: qualitySelect,
            hostEl,
            baseBtnStyle
          });
        }
      };

      btn.addEventListener("click", invokeClick);
      if (hostEl && typeof hostEl.addEventListener === "function") {
        hostEl.addEventListener("click", invokeClick);
      }

      if (typeof shadow.appendChild === "function") {
        shadow.appendChild(qualitySelect);
        shadow.appendChild(btn);
      }
      if (doc.body && doc.body.appendChild) {
        doc.body.appendChild(hostEl);
      }
      attached.push({
        video,
        hostEl,
        btn,
        qualitySelect,
        updateQualities,
        isBlob,
        isDrm,
        src,
        reposition: () => positionOverlayHost(video, hostEl)
      });
    } else if (typeof video.__fluxUpdateQualities === "function" && Array.isArray(video.__fluxQualities) && video.__fluxQualities.length > 0) {
      video.__fluxUpdateQualities(video.__fluxQualities);
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
    return { activeOverlays, observer, refreshPositions, rescanAndRefresh };
  }
  return { activeOverlays, observer: null, refreshPositions, rescanAndRefresh };
}

function handleLinkClick(ev) {
  try {
    const anchor = ev.target && typeof ev.target.closest === "function" ? ev.target.closest("a") : null;
    const href = (anchor && anchor.href) ? anchor.href : "";

    if (ev.altKey) {
      // IDM Alt-key bypass behavior: user explicitly requested browser-native download
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "bypassNextDownload",
          url: href,
          tabUrl: (typeof location !== "undefined" ? location.href : ""),
          correlationId: makeContentCorrelationId("bypass")
        });
      }
      return;
    }

    if (!anchor || !href) return;
    if (href.startsWith("javascript:") || href.startsWith("#") || href.startsWith("mailto:") || href.startsWith("tel:")) return;

    const pathname = anchor.pathname || "";
    const lastDot = pathname.lastIndexOf(".");
    const ext = lastDot !== -1 ? pathname.slice(lastDot + 1).toLowerCase() : "";
    const hasDownloadAttr = typeof anchor.hasAttribute === "function" && anchor.hasAttribute("download");

    const interceptExts = new Set([
      "zip", "rar", "7z", "tar", "gz", "bz2", "xz", "tgz", "iso", "exe", "msi", "dmg", "pdf",
      "mp4", "mkv", "webm", "mov", "avi", "flv", "mp3", "m4a", "flac", "wav", "apk", "bin",
      "deb", "rpm", "docx", "xlsx", "pptx", "csv", "epub"
    ]);

    const isLikelyDownloadLink = hasDownloadAttr ||
                                 interceptExts.has(ext) ||
                                 pathname.includes("/redirect") ||
                                 pathname.includes("/download") ||
                                 pathname.includes("/files/") ||
                                 pathname.includes("/release") ||
                                 href.includes("download=") ||
                                 href.includes("attachment");

    if (isLikelyDownloadLink) {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "linkClicked",
          url: href,
          filename: anchor.download || "",
          ext,
          hasDownloadAttr: Boolean(hasDownloadAttr),
          isDirectStaticFile: Boolean(hasDownloadAttr || interceptExts.has(ext)),
          referrer: (typeof location !== "undefined" ? location.href : ""),
          pageTitle: (typeof document !== "undefined" ? document.title : ""),
          correlationId: makeContentCorrelationId("click"),
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

    // Trigger MAIN world page media & quality scan immediately when playback starts
    if (typeof window !== "undefined" && typeof window.postMessage === "function") {
      try {
        window.postMessage({ action: "fluxScanPageMedia" }, "*");
      } catch (_) {}
    }

    const src = extractVideoElementSrc(target);
    const qualities = extractVideoElementQualities(target);
    if (qualities.length > 0) {
      target.__fluxQualities = qualities;
      if (typeof target.__fluxUpdateQualities === "function") {
        target.__fluxUpdateQualities(qualities);
      }
    }

    if (!src || src.startsWith("data:")) return;

    // Active discovery for blob: media elements on playback (Defect A fix)
    if (src.startsWith("blob:")) {
      triggerBlobDiscovery(target, src, typeof document !== "undefined" ? document : null);
      return;
    }

    if (target.dataset) {
      target.dataset.fluxCandidateReady = "true";
    }

    const liveH = Number(target.videoHeight || 0);
    const liveRes = liveH > 0 ? `${liveH}p` : (qualities.length > 0 ? qualities[0].resolution : "");
    const lower = src.toLowerCase();
    const isHls = lower.includes(".m3u8") || lower.includes(".m3u");
    const isDash = lower.includes(".mpd");
    const streamType = isHls ? "HLS" : (isDash ? "DASH" : "HTTP_FILE");
    const sourceType = isHls ? "HLS_MANIFEST" : (isDash ? "DASH_MANIFEST" : "DIRECT_PROGRESSIVE");
    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      chrome.runtime.sendMessage({
        action: "addMedia",
        url: src,
        streamType,
        sourceType,
        resolution: liveRes,
        qualities,
        correlationId: makeContentCorrelationId("play"),
        pageTitle: (typeof document !== "undefined" ? document.title : "video"),
        tabUrl: (typeof location !== "undefined" ? location.href : "")
      }, (resp) => {
        if (typeof chrome !== "undefined" && chrome.runtime) {
          void chrome.runtime.lastError;
        }
        if (resp && resp.candidate) {
          if (resp.candidate.candidateId && target.dataset) {
            target.dataset.fluxCandidateId = resp.candidate.candidateId;
          }
          if (Array.isArray(resp.candidate.qualities) && resp.candidate.qualities.length > 0) {
            target.__fluxQualities = resp.candidate.qualities;
            if (typeof target.__fluxUpdateQualities === "function") {
              target.__fluxUpdateQualities(resp.candidate.qualities, resp.candidate.resolution);
            }
          }
        }
      });
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

  let overlayObserverCtrl = null;

  const syncTabQualitiesToVideos = () => {
    if (typeof chrome === "undefined" || !chrome.runtime || !chrome.runtime.sendMessage) return;
    try {
      chrome.runtime.sendMessage({
        action: "getMediaList",
        tabUrl: location.href
      }, (res) => {
        if (typeof chrome !== "undefined" && chrome.runtime) {
          void chrome.runtime.lastError;
        }
        if (!res || !Array.isArray(res.candidates) || res.candidates.length === 0) return;
        const bestCand = res.candidates[0];
        const vids = document.querySelectorAll ? Array.from(document.querySelectorAll("video")) : [];
        for (const v of vids) {
          if (!v || !v.dataset) continue;
          const vSrc = extractVideoElementSrc(v);
          const matchesVideo = Boolean(
            (vSrc && (vSrc === bestCand.url || vSrc.startsWith("blob:"))) ||
            (!vSrc && (v.readyState > 0 || v.videoWidth > 0))
          );
          if (matchesVideo) {
            v.dataset.fluxCandidateReady = "true";
            if (bestCand.candidateId) v.dataset.fluxCandidateId = bestCand.candidateId;
            if (Array.isArray(bestCand.qualities) && bestCand.qualities.length > 0) {
              v.__fluxQualities = bestCand.qualities;
              if (typeof v.__fluxUpdateQualities === "function") {
                v.__fluxUpdateQualities(bestCand.qualities, bestCand.resolution);
              }
            }
          }
        }
        if (overlayObserverCtrl && typeof overlayObserverCtrl.rescanAndRefresh === "function") {
          overlayObserverCtrl.rescanAndRefresh();
        }
      });
    } catch (_) {}
  };

  // 1. Intercept link clicks with capture phase
  document.addEventListener("click", handleLinkClick, true);
  document.addEventListener("auxclick", handleLinkClick, true);

  // 2. Listen to media playback events with capture phase
  document.addEventListener("play", (ev) => {
    handleMediaPlaybackEvent(ev);
    if (overlayObserverCtrl && typeof overlayObserverCtrl.rescanAndRefresh === "function") {
      overlayObserverCtrl.rescanAndRefresh();
    }
  }, true);
  document.addEventListener("playing", (ev) => {
    handleMediaPlaybackEvent(ev);
    if (overlayObserverCtrl && typeof overlayObserverCtrl.rescanAndRefresh === "function") {
      overlayObserverCtrl.rescanAndRefresh();
    }
  }, true);
  document.addEventListener("canplay", handleMediaPlaybackEvent, true);
  document.addEventListener("loadedmetadata", handleMediaPlaybackEvent, true);

  // 3. Relay page MAIN world messages from catch-script/catch.js to background
  window.addEventListener("message", (ev) => {
    if (!ev || !ev.data) return;
    if (ev.source !== window && ev.origin !== window.location.origin) return;

    if (ev.data.action === "catCatchAddMedia") {
      const incomingQualities = Array.isArray(ev.data.qualities) ? ev.data.qualities : [];
      // Mark matching or active MSE/blob <video> elements as having a detected candidate & qualities
      if (document.querySelectorAll) {
        const vids = document.querySelectorAll("video");
        vids.forEach(v => {
          if (!v || !v.dataset) return;
          const vSrc = extractVideoElementSrc(v);
          let matched = false;
          if (vSrc && (vSrc === ev.data.url || vSrc === ev.data.blobUrl || vSrc.startsWith("blob:"))) {
            v.dataset.fluxCandidateReady = "true";
            matched = true;
          } else if (!vSrc && (v.readyState > 0 || v.videoWidth > 0) &&
                     (ev.data.streamType === "HLS" || ev.data.streamType === "DASH" || ev.data.streamType === "YOUTUBE")) {
            v.dataset.fluxCandidateReady = "true";
            matched = true;
          }
          if (matched && incomingQualities.length > 0) {
            v.__fluxQualities = incomingQualities;
            if (typeof v.__fluxUpdateQualities === "function") {
              v.__fluxUpdateQualities(incomingQualities, ev.data.resolution || "");
            }
          }
        });
      }
      if (overlayObserverCtrl && typeof overlayObserverCtrl.rescanAndRefresh === "function") {
        overlayObserverCtrl.rescanAndRefresh();
      }

      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "addMedia",
          url: ev.data.url,
          blobUrl: ev.data.blobUrl || "",
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
          qualities: incomingQualities,
          videoUrl: ev.data.videoUrl || "",
          pairedAudioUrl: ev.data.pairedAudioUrl || "",
          extractorJson: ev.data.extractorJson || "",
          cachedManifestBody: ev.data.cachedManifestBody || "",
          correlationId: ev.data.correlationId || makeContentCorrelationId("catch"),
          tabUrl: location.href,
          pageTitle: ev.data.pageTitle || document.title || "video"
        }, (resp) => {
          if (typeof chrome !== "undefined" && chrome.runtime) {
            void chrome.runtime.lastError;
          }
          if (resp && resp.candidate && Array.isArray(resp.candidate.qualities) && resp.candidate.qualities.length > 0) {
            syncTabQualitiesToVideos();
          }
        });
      }
    } else if (ev.data.action === "catCatchBlobDiscoveryResult") {
      if (ev.data.ytFound) {
        syncTabQualitiesToVideos();
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
          action: "linkClicked",
          url: ev.data.url,
          filename: ev.data.filename || "",
          hasDownloadAttr: Boolean(ev.data.hasDownloadAttr),
          referrer: ev.data.referrer || location.href,
          pageTitle: ev.data.pageTitle || document.title || "",
          correlationId: ev.data.correlationId || makeContentCorrelationId("jsclick"),
          bypass: false
        }, () => {
          if (typeof chrome !== "undefined" && chrome.runtime) {
            void chrome.runtime.lastError;
          }
        });
      }
    }
  }, { capture: true });

  // 4. Listen for background probe requests & live quality updates
  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.onMessage) {
    chrome.runtime.onMessage.addListener((msg, _sender, sendResponse) => {
      if (msg && msg.action === "mediaUpdated") {
        syncTabQualitiesToVideos();
        return false;
      }
      if (msg && msg.action === "probePageMedia") {
        if (typeof window !== "undefined" && typeof window.postMessage === "function") {
          try { window.postMessage({ action: "fluxScanPageMedia" }, "*"); } catch (_) {}
        }
        const videos = document.querySelectorAll ? Array.from(document.querySelectorAll("video")) : [];
        let blobCount = 0;
        let directCount = 0;
        for (const v of videos) {
          const s = extractVideoElementSrc(v);
          if (s.startsWith("blob:")) {
            blobCount++;
            triggerBlobDiscovery(v, s, document);
          } else if (s) {
            directCount++;
            notifyDirectVideoElementDiscovered(v, document);
          }
        }
        if (overlayObserverCtrl && typeof overlayObserverCtrl.rescanAndRefresh === "function") {
          overlayObserverCtrl.rescanAndRefresh();
        }
        sendResponse({
          videoCount: videos.length,
          blobCount,
          directCount,
          pageTitle: document.title || "",
          tabUrl: location.href
        });
        return true;
      }
      return false;
    });
  }

  // 5. Inject deep search script into page DOM
  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", injectDeepSearchScript, { once: true });
  } else {
    injectDeepSearchScript();
  }

  // 6. Observe videos and attach overlay with active visual status, quality selection & error reporting
  const onDownloadClick = (mediaMeta) => {
    const btn = mediaMeta.buttonEl;
    const hostEl = mediaMeta.hostEl;
    const baseStyle = mediaMeta.baseBtnStyle || "";
    const corrId = mediaMeta.correlationId || makeContentCorrelationId("dlclick");

    if (!mediaMeta.videoSrc && !mediaMeta.blobUrl && !mediaMeta.isMseBlob && !mediaMeta.candidateId) {
      const errText = "No supported download source found for this video";
      if (btn) {
        btn.textContent = "Error: No media source";
        btn.title = errText;
        btn.disabled = false;
        if (btn.style) btn.style.cssText = baseStyle + "border-color:#EF4444;color:#FCA5A5;";
      }
      if (hostEl && hostEl.setAttribute) {
        hostEl.setAttribute("data-flux-status", "error");
        hostEl.setAttribute("data-flux-error", errText);
      }
      return;
    }

    if (btn) {
      const qBadge = mediaMeta.resolution ? ` (${mediaMeta.resolution})` : "";
      btn.textContent = mediaMeta.isMseBlob ? `Resolving${qBadge}...` : `Starting${qBadge}...`;
      btn.disabled = true;
      if (btn.style) btn.style.cssText = baseStyle + "border-color:#38BDF8;color:#E0F2FE;";
    }
    if (hostEl && hostEl.setAttribute) {
      hostEl.setAttribute("data-flux-status", "starting");
      hostEl.setAttribute("data-flux-correlation-id", corrId);
    }

    if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
      chrome.runtime.sendMessage({
        action: "downloadMedia",
        url: mediaMeta.videoSrc || "",
        blobUrl: mediaMeta.blobUrl || "",
        candidateId: mediaMeta.candidateId || "",
        qualityId: mediaMeta.qualityId || "",
        resolution: mediaMeta.resolution || "",
        videoFilter: mediaMeta.videoFilter || "best",
        selectedQuality: mediaMeta.selectedQuality || null,
        isMseBlob: Boolean(mediaMeta.isMseBlob),
        correlationId: corrId,
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
            if (btn.style) {
              btn.style.cssText = isFallback
                ? (baseStyle + "border-color:#F59E0B;color:#FDE68A;")
                : (baseStyle + "border-color:#10B981;color:#A7F3D0;");
            }
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
      overlayObserverCtrl = observeDynamicVideosAndFullscreen(document, window, onDownloadClick);
      syncTabQualitiesToVideos();
    }, { once: true });
  } else {
    overlayObserverCtrl = observeDynamicVideosAndFullscreen(document, window, onDownloadClick);
    syncTabQualitiesToVideos();
  }
}

// Auto-run in browser execution
if (typeof window !== "undefined" && typeof document !== "undefined") {
  initContentScript();
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    positionOverlayHost,
    extractVideoElementSrc,
    extractVideoElementQualities,
    triggerBlobDiscovery,
    notifyDirectVideoElementDiscovered,
    scanAndAttachVideoOverlays,
    observeDynamicVideosAndFullscreen,
    handleLinkClick,
    handleMediaPlaybackEvent,
    initContentScript
  };
}
