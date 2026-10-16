// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/popup.js) for Flux Popup UI.
// Supports media type filtering (All / Streams / Direct), search filtering,
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

function filterPopupCandidates(candidates, filterMode = "all", searchQuery = "") {
  if (!Array.isArray(candidates)) return [];
  const q = (searchQuery || "").trim().toLowerCase();
  return candidates.filter(item => {
    const isStream = (item.streamType === "HLS" || item.streamType === "DASH");
    if (filterMode === "stream" && !isStream) return false;
    if (filterMode === "direct" && isStream) return false;
    if (q) {
      const title = (item.pageTitle || item.url || "").toLowerCase();
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
    const isStream = (item.streamType === "HLS" || item.streamType === "DASH");
    const badge = item.streamType || (item.ext ? item.ext.toUpperCase() : "MEDIA");
    const badgeClass = isStream ? "badge badge-stream" : "badge";
    const rawName = item.pageTitle || (item.url ? item.url.split("?")[0].split("/").pop() : "media");
    const sourceHost = extractSourceHost(item.tabUrl || item.url);
    const qualityTag = item.resolution ? `<span>${item.resolution}</span>` : `<span>${formatMediaSize(item.size)}</span>`;
    const variantNote = (item.variants && item.variants.length > 0)
      ? `<small>(+${item.variants.length} sub-playlists grouped)</small>`
      : "";
    const statusText = item.handoffStatus || "Ready";
    const actionLabel = (!desktopConnected && !isStream) ? "Browser Download" : "Download in Flux";

    return `<div class="media-item" data-id="${item.candidateId}" data-stream="${isStream}">
      <div class="media-meta">
        <div class="media-title-row">
          <span class="${badgeClass}">${badge}</span>
          <strong class="title" title="${rawName}">${rawName}</strong>
        </div>
        <div class="media-sub-row">
          <span>${sourceHost}</span>
          ${qualityTag}
          <span>${statusText}</span>
          ${variantNote}
        </div>
      </div>
      <button class="dl-btn" data-url="${item.url}" data-id="${item.candidateId}">${actionLabel}</button>
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
  let isDesktopConnected = true;

  const mediaListEl = document.getElementById("mediaList");
  const connectionBadge = document.getElementById("connectionBadge");
  const siteToggle = document.getElementById("siteToggle");
  const searchInput = document.getElementById("searchInput");
  const clearListBtn = document.getElementById("clearListBtn");
  const batchDownloadBtn = document.getElementById("batchDownloadBtn");
  const filterTabs = document.querySelectorAll(".tab-btn");

  function refreshListDisplay() {
    if (!mediaListEl) return;
    mediaListEl.innerHTML = renderDetectedMediaList(currentCandidates, {
      filterMode: activeFilterMode,
      searchQuery: activeSearchQuery,
      desktopConnected: isDesktopConnected
    });
    attachItemClickListeners();
  }

  function attachItemClickListeners() {
    if (!mediaListEl) return;
    const buttons = mediaListEl.querySelectorAll(".dl-btn");
    buttons.forEach(btn => {
      btn.addEventListener("click", (ev) => {
        const candidateId = ev.target.getAttribute("data-id");
        const url = ev.target.getAttribute("data-url");
        ev.target.textContent = "Connecting...";
        ev.target.disabled = true;

        if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.sendMessage) {
          chrome.runtime.sendMessage({
            action: "downloadMedia",
            candidateId,
            url,
            tabId: currentTabId
          }, (resp) => {
            if (resp && resp.success) {
              ev.target.textContent = "Queued in Flux";
            } else {
              ev.target.textContent = (resp && resp.error) ? "Error" : "Failed";
              ev.target.disabled = false;
            }
          });
        }
      });
    });
  }

  function fetchTabData() {
    if (typeof chrome === "undefined" || !chrome.tabs || !chrome.runtime) return;

    chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
      if (tabs && tabs[0]) {
        currentTabId = tabs[0].id;
        currentTabUrl = tabs[0].url || "";

        chrome.runtime.sendMessage({
          action: "getMediaList",
          tabId: currentTabId,
          tabUrl: currentTabUrl
        }, (res) => {
          if (res) {
            isDesktopConnected = Boolean(res.desktopConnected);
            if (connectionBadge) {
              if (isDesktopConnected) {
                connectionBadge.textContent = "Desktop Connected";
                connectionBadge.className = "conn-badge conn-connected";
              } else {
                connectionBadge.textContent = "Desktop Offline";
                connectionBadge.className = "conn-badge conn-disconnected";
              }
            }
            if (siteToggle) {
              siteToggle.checked = Boolean(res.captureEnabled && !res.siteDisabled);
            }
            currentCandidates = res.candidates || [];
            refreshListDisplay();
          }
        });
      }
    });
  }

  // Filter tab buttons
  filterTabs.forEach(tab => {
    tab.addEventListener("click", () => {
      filterTabs.forEach(t => t.classList.remove("active"));
      tab.classList.add("active");
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
            tabId: currentTabId
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
