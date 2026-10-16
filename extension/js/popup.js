// SPDX-License-Identifier: GPL-3.0-or-later
// Adapted from xifangczy/cat-catch (js/popup.js) for OpenIDM Popup UI.
// Supports media type filtering (All / Streams / Direct), search filtering,
// resolution/size formatting, and disconnected/loading/empty states.

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
    return '<p class="empty" data-state="loading">Probing tab media &amp; connecting to OpenIDM...</p>';
  }
  if (errorMessage) {
    return `<div class="error-banner" data-state="error">${errorMessage}</div>`;
  }

  const filtered = filterPopupCandidates(candidates, filterMode, searchQuery);
  if (filtered.length === 0) {
    return '<p class="empty" data-state="empty">No media detected on this tab yet.</p>';
  }

  const connBanner = desktopConnected
    ? ""
    : '<div class="error-banner" data-state="disconnected">OpenIDM Desktop is offline. Direct files will fall back to browser download.</div>';

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
    const actionLabel = (!desktopConnected && !isStream) ? "Browser Download" : "Download in OpenIDM";

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

if (typeof module !== "undefined" && module.exports) {
  module.exports = {
    formatMediaSize,
    filterPopupCandidates,
    renderDetectedMediaList
  };
}
