// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/popup.js) for Flux Cosmic Dark Popup UI.
// Supports media type filtering (All / Streams / Direct), live count badges, search filtering,
// resolution/size formatting, real-time background query, and batch/single download handoff.

function formatMediaSize(bytes) {
  if (!bytes || bytes <= 0) return "Live / Stream";
  if (bytes >= 1073741824) return (bytes / 1073741824).toFixed(1) + " GiB";
  if (bytes >= 1048576) return (bytes / 1048576).toFixed(1) + " MiB";
  if (bytes >= 1024) return (bytes / 1024).toFixed(1) + " KiB";
  return bytes + " B";
}

function extractSourceHost(urlStr) {
  try {
    return new URL(urlStr).hostname;
  } catch (_) {
    return "web";
  }
}

function isStreamCandidate(item) {
  if (!item) return false;
  return item.streamType === "HLS" ||
         item.streamType === "DASH" ||
         item.streamType === "MSS" ||
         item.streamType === "YOUTUBE" ||
         item.streamType === "SITE_EXTRACTION" ||
         item.streamType === "VIMEO";
}

function filterPopupCandidates(candidates, filterMode = "all", searchQuery = "") {
  if (!Array.isArray(candidates)) return [];
  const q = (searchQuery || "").trim().toLowerCase();
  return candidates.filter(item => {
    const isStream = isStreamCandidate(item);
    if (filterMode === "stream" && !isStream) return false;
    if (filterMode === "direct" && isStream) return false;
    if (q) {
      const title = (item.pageTitle || item.fileName || item.url || "").toLowerCase();
      const badge = (item.streamType || item.ext || "").toLowerCase();
      const res = (item.resolution || "").toLowerCase();
      return title.includes(q) || badge.includes(q) || res.includes(q);
    }
    return true;
  });
}

function renderDetectedMediaList(candidates, options = {}) {
  const {
    filterMode = "all",
    searchQuery = "",
    loading = false,
    desktopConnected = true,
    errorMessage = ""
  } = options;

  if (loading) {
    return '<p class="empty" data-state="loading">Probing tab media &amp; connecting to Flux...</p>';
  }
  if (errorMessage) {
    return `<div class="error-banner" data-state="error">${errorMessage}</div>`;
  }

  const filtered = filterPopupCandidates(candidates, filterMode, searchQuery);
  if (filtered.length === 0) {
    return '<p class="empty" data-state="empty">No media detected on this tab yet. Play a video or start a download to capture streams.</p>';
  }

  const connBanner = desktopConnected
    ? ""
    : '<div class="error-banner" data-state="disconnected">Flux Desktop is offline. Direct files will fall back to browser download.</div>';

  const itemsHtml = filtered.map(item => {
    const isStream = isStreamCandidate(item);
    const isUnavailable = Boolean(item.unavailable || item.drmProtected || !item.url);
    const badge = isStream
      ? item.streamType
      : (item.ext ? item.ext.toUpperCase() : (item.streamType || "MEDIA"));
    const badgeClass = isStream ? "badge badge-stream" : "badge";
    const rawName = item.pageTitle || item.fileName || (item.url ? item.url.split("?")[0].split("/").pop() : "media");
    const sourceHost = extractSourceHost(item.tabUrl || item.url);
    const qualityTag = item.resolution
      ? `<span class="media-size-tag">${item.resolution}${item.size > 0 ? " &middot; " + formatMediaSize(item.size) : ""}</span>`
      : `<span class="media-size-tag">${formatMediaSize(item.size)}</span>`;
    const variantNote = (item.variants && item.variants.length > 0)
      ? `<small>(+${item.variants.length} sub-playlists grouped)</small>`
      : "";
    const qualitiesList = Array.isArray(item.qualities) ? item.qualities : [];
    const qualitySelectHtml = qualitiesList.length > 0
      ? `<select class="quality-select" data-id="${item.candidateId}" title="Select video quality"${isUnavailable ? " disabled" : ""}>${qualitiesList.map(q => {
          const qSize = (q.size && q.size > 0) ? (" · " + formatMediaSize(q.size)) : "";
          const qLabel = (q.label || q.resolution || "Auto") + qSize;
          return `<option value="${q.id || q.resolution || "best"}">${qLabel}</option>`;
        }).join("")}</select>`
      : "";

    const statusText = isUnavailable
      ? (item.drmProtected ? "DRM Protected" : "Unavailable")
      : (item.handoffStatus || "Ready");
    const statusAttr = isUnavailable
      ? "unavailable"
      : (statusText.includes("Queued") ? "queued" : (statusText.includes("Fallback") ? "fallback" : "ready"));
    const actionLabel = isUnavailable
      ? "Unavailable"
      : ((!desktopConnected && !isStream) ? "Browser Download" : "Download in Flux");

    return `<div class="media-item" data-id="${item.candidateId}" data-stream="${isStream}"${isUnavailable ? ' data-unavailable="true"' : ""}>
      <div class="media-meta">
        <div class="media-title-row">
          <span class="${badgeClass}">${badge}</span>
          <strong class="title" title="${rawName}">${rawName}</strong>
        </div>
        <div class="media-sub-row">
          <span>${sourceHost}</span>
          ${qualityTag}
          <span class="item-status" data-status="${statusAttr}">${statusText}</span>
          ${variantNote}
        </div>
      </div>
      <div class="media-actions">
        ${qualitySelectHtml}
        <button class="dl-btn" data-url="${item.url || ""}" data-id="${item.candidateId}"${isUnavailable ? " disabled" : ""}>${actionLabel}</button>
      </div>
    </div>`;
  }).join("\n");

  return connBanner + itemsHtml;
}

// Interactive DOM Controller for Popup
function initPopupUI() {
  if (typeof document === "undefined") return;

  let currentTabId = 0;
  let currentTabUrl = "";
  let currentCandidates = [];
  let activeFilterMode = "all";
  let activeSearchQuery = "";
  let isDesktopConnected = false;
  let activeErrorMessage = "";

  const mediaListEl = document.getElementById("mediaList");
  const connectionBadge = document.getElementById("connectionBadge");
  const siteHostLabel = document.getElementById("siteHostLabel");
  const siteToggle = document.getElementById("siteToggle");
  const searchInput = document.getElementById("searchInput");
  const clearListBtn = document.getElementById("clearListBtn");
  const batchDownloadBtn = document.getElementById("batchDownloadBtn");
  const footerMeta = document.getElementById("footerMeta");
  const countAllEl = document.getElementById("countAll");
  const countStreamEl = document.getElementById("countStream");
  const countDirectEl = document.getElementById("countDirect");
  const filterTabs = document.querySelectorAll(".tab-btn");

  function updateTabCountBadges() {
    const allCount = Array.isArray(currentCandidates) ? currentCandidates.length : 0;
    const streamCount = filterPopupCandidates(currentCandidates, "stream", "").length;
    const directCount = filterPopupCandidates(currentCandidates, "direct", "").length;
    if (countAllEl) countAllEl.textContent = String(allCount);
    if (countStreamEl) countStreamEl.textContent = String(streamCount);
    if (countDirectEl) countDirectEl.textContent = String(directCount);
    if (footerMeta) {
      footerMeta.textContent = allCount > 0
        ? `${allCount} item${allCount === 1 ? "" : "s"} detected`
        : "Alt+Click bypasses capture";
    }
    if (batchDownloadBtn) {
      const visible = filterPopupCandidates(currentCandidates, activeFilterMode, activeSearchQuery);
      batchDownloadBtn.disabled = visible.length === 0;
    }
  }

  function refreshListDisplay() {
    updateTabCountBadges();
    if (!mediaListEl) return;
    mediaListEl.innerHTML = renderDetectedMediaList(currentCandidates, {
      filterMode: activeFilterMode,
      searchQuery: activeSearchQuery,
      desktopConnected: isDesktopConnected,
      errorMessage: activeErrorMessage
    });
    attachItemClickListeners();
  }

  function attachItemClickListeners() {
    if (!mediaListEl) return;
    const buttons = mediaListEl.querySelectorAll(".dl-btn");
    buttons.forEach(btn => {
      btn.addEventListener("click", (ev) => {
        const targetBtn = ev.currentTarget || ev.target;
        if (targetBtn.disabled) return;
        const candidateId = targetBtn.getAttribute("data-id");
        const url = targetBtn.getAttribute("data-url");
        const qSelect = mediaListEl.querySelector(`.quality-select[data-id="${candidateId}"]`);
        const qualityId = qSelect ? qSelect.value : "";
        targetBtn.textContent = "Connecting...";
        targetBtn.disabled = true;

        if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
          chrome.runtime.sendMessage({
            action: "downloadMedia",
            candidateId,
            url,
            qualityId,
            tabId: currentTabId,
            tabUrl: currentTabUrl
          }, (resp) => {
            const lastErr = chrome.runtime.lastError ? chrome.runtime.lastError.message : null;
            if (!lastErr && resp && resp.success) {
              const isFallback = Boolean(resp.result && resp.result.fallbackToBrowser);
              targetBtn.textContent = isFallback ? "Browser Download" : "Queued in Flux";
              const taskId = (resp.result && resp.result.taskId) ? resp.result.taskId : "";
              if (taskId) targetBtn.setAttribute("data-task-id", taskId);
              const itemCand = currentCandidates.find(c => c.candidateId === candidateId);
              if (itemCand) itemCand.handoffStatus = isFallback ? "Browser Fallback" : "Queued in Flux";
            } else {
              const errMsg = lastErr || (resp && resp.error) || "Failed";
              targetBtn.textContent = "Failed";
              targetBtn.title = errMsg;
              targetBtn.disabled = false;
            }
          });
        }
      });
    });
  }

  function updateConnectionBadge(connected) {
    isDesktopConnected = Boolean(connected);
    if (!connectionBadge) return;
    if (isDesktopConnected) {
      connectionBadge.textContent = "Desktop Connected";
      connectionBadge.className = "conn-badge conn-connected";
    } else {
      connectionBadge.textContent = "Desktop Offline";
      connectionBadge.className = "conn-badge conn-disconnected";
    }
  }

  function fetchTabData(explicitTabId = null) {
    if (typeof chrome === "undefined" || !chrome.tabs || !chrome.runtime) return;

    const queryAndLoad = (tabId, tabUrl) => {
      currentTabId = tabId;
      currentTabUrl = tabUrl || "";
      if (siteHostLabel) {
        siteHostLabel.textContent = currentTabUrl ? extractSourceHost(currentTabUrl) : "Active tab";
      }
      chrome.runtime.sendMessage({
        action: "getMediaList",
        tabId: currentTabId,
        tabUrl: currentTabUrl
      }, (res) => {
        const lastErr = chrome.runtime.lastError ? chrome.runtime.lastError.message : null;
        if (lastErr || !res) {
          updateConnectionBadge(false);
          activeErrorMessage = lastErr ? ("Extension service worker error: " + lastErr) : "";
          refreshListDisplay();
          return;
        }
        updateConnectionBadge(res.desktopConnected);
        if (siteToggle) {
          siteToggle.checked = Boolean(res.captureEnabled && !res.siteDisabled);
        }
        currentCandidates = res.candidates || [];
        activeErrorMessage = (currentCandidates.length === 0 && res.playerError) ? res.playerError : "";
        refreshListDisplay();
      });
    };

    const urlParams = (typeof window !== "undefined" && window.location)
      ? new URLSearchParams(window.location.search)
      : null;
    const paramTabId = explicitTabId || (urlParams && urlParams.get("tabId") ? Number(urlParams.get("tabId")) : null);
    const paramTabUrl = (urlParams && urlParams.get("tabUrl")) || "";

    if (paramTabId && paramTabId > 0) {
      queryAndLoad(paramTabId, paramTabUrl);
      return;
    }

    chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
      if (tabs && tabs[0]) {
        let targetTab = tabs[0];
        if (targetTab.url && targetTab.url.startsWith("chrome-extension://")) {
          chrome.tabs.query({ currentWindow: true }, (allTabs) => {
            const nonExt = (allTabs || []).find(t => t.url && !t.url.startsWith("chrome-extension://"));
            if (nonExt) targetTab = nonExt;
            queryAndLoad(targetTab.id, targetTab.url || "");
          });
          return;
        }
        queryAndLoad(targetTab.id, targetTab.url || "");
      }
    });
  }

  // Listen for live updates from background service worker while popup is open
  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.onMessage) {
    chrome.runtime.onMessage.addListener((msg) => {
      if (msg && msg.action === "mediaUpdated" && (!msg.tabId || msg.tabId === currentTabId)) {
        fetchTabData(currentTabId);
      }
    });
  }

  // Filter tab buttons
  filterTabs.forEach(tab => {
    tab.addEventListener("click", () => {
      filterTabs.forEach(t => {
        t.classList.remove("active");
        t.setAttribute("aria-selected", "false");
      });
      tab.classList.add("active");
      tab.setAttribute("aria-selected", "true");
      activeFilterMode = tab.getAttribute("data-filter") || "all";
      refreshListDisplay();
    });
  });

  // Search input
  if (searchInput) {
    searchInput.addEventListener("input", (ev) => {
      activeSearchQuery = ev.target.value;
      refreshListDisplay();
    });
  }

  // Site capture toggle
  if (siteToggle) {
    siteToggle.addEventListener("change", (ev) => {
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "setSiteCapture",
          tabUrl: currentTabUrl,
          enabled: ev.target.checked
        });
      }
    });
  }

  // Clear list button
  if (clearListBtn) {
    clearListBtn.addEventListener("click", () => {
      currentCandidates = [];
      activeErrorMessage = "";
      refreshListDisplay();
      if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
        chrome.runtime.sendMessage({
          action: "clearMedia",
          tabId: currentTabId
        });
      }
    });
  }

  // Download all in Flux button
  if (batchDownloadBtn) {
    batchDownloadBtn.addEventListener("click", () => {
      const filtered = filterPopupCandidates(currentCandidates, activeFilterMode, activeSearchQuery);
      if (filtered.length === 0) return;
      batchDownloadBtn.textContent = "Submitting " + filtered.length + "...";
      batchDownloadBtn.disabled = true;

      filtered.forEach(item => {
        if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
          chrome.runtime.sendMessage({
            action: "downloadMedia",
            candidateId: item.candidateId,
            url: item.url,
            tabId: currentTabId,
            tabUrl: currentTabUrl
          });
        }
      });

      setTimeout(() => {
        batchDownloadBtn.textContent = "Queued " + filtered.length + " items";
        refreshListDisplay();
      }, 600);
    });
  }

  // Initial load
  fetchTabData();
}

if (typeof document !== "undefined") {
  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initPopupUI);
  } else {
    initPopupUI();
  }
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    formatMediaSize,
    filterPopupCandidates,
    renderDetectedMediaList,
    initPopupUI
  };
}
