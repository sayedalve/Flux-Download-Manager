# Browser Extension Architecture Specification

**Document Status:** Proposed Subsystem Specification  
**Last Reviewed:** 2026-10-16  
**Scope:** Manifest V3 architecture, permissions minimization, background Service Worker, isolated content scripts, closed Shadow DOM video overlay, popup UI, and cross-browser packaging (`extension/`).  
**Related Documents:** [MEDIA_DETECTION.md](MEDIA_DETECTION.md), [NATIVE_MESSAGING_AND_IPC.md](NATIVE_MESSAGING_AND_IPC.md), [PRIVACY_AND_DATA_HANDLING.md](PRIVACY_AND_DATA_HANDLING.md), [adr/0003-browser-communication.md](adr/0003-browser-communication.md), [adr/0004-media-detection.md](adr/0004-media-detection.md)

---

## 1. Architectural Overview

The OpenIDM Browser Extension (`extension/`) is a lightweight Manifest V3 companion built from a single strict TypeScript codebase that compiles to two browser targets:
- **Chromium MV3 Target:** Google Chrome (114+), Microsoft Edge (114+), and Brave (114+).
- **Firefox MV3 Target:** Mozilla Firefox (115+ ESR / 128+).

Unlike [Cat Catch](https://github.com/xifangczy/cat-catch) (which bundles a 7.8 MB extension containing `hls.min.js`, preview players, and in-browser downloaders), the OpenIDM extension is strictly a **discovery and capture sensor** (`< 200 KB` total unpacked size, `< 15 KB` content script) that forwards downloads and media descriptors to `openidm.exe` over Native Messaging.

---

## 2. Internal Component Architecture

```mermaid
flowchart LR
    subgraph WebPage["Host Webpage (Main Frame & Iframes)"]
        DOM["<video> / <a> Elements"]
        Hook["Opt-In MAIN World Hook<br/>(page_hook/DeepSearchHook.ts)"]
        Overlay["ISOLATED Content Script<br/>(content/VideoOverlay.ts +<br/>Closed Shadow DOM)"]
        Hook -->|CustomEvent (Manifest Text)| Overlay
        DOM <--> Overlay
    end

    subgraph Background["Extension Background (MV3 Service Worker / Event Page)"]
        Sniffer["MediaSniffer.ts<br/>(webRequest Listener)"]
        Dedup["ManifestDeduplicator.ts"]
        DLInt["DownloadInterceptor.ts<br/>(chrome.downloads Listener)"]
        State["SessionStateStore.ts<br/>(chrome.storage.session)"]
        NMClient["NativeHostClient.ts<br/>(runtime.connectNative)"]

        Sniffer --> Dedup
        Dedup --> State
        DLInt --> NMClient
        State <--> NMClient
    end

    subgraph PopupUI["Extension Action Popup (popup/)"]
        Popup["Popup View<br/>(Detected Streams, Site Exclusion Toggle,<br/>Desktop Connection Status)"]
    end

    Overlay <-->|runtime.sendMessage| State
    Popup <-->|runtime.sendMessage| State
    NMClient <-->|stdio JSON-RPC| NMH["openidm-nmh.exe"]
```

---

## 3. Narrow Permission Model & Justification (`Section 8 Requirement`)

To minimize security risk and pass Chrome Web Store / Firefox AMO review without friction, every permission in `manifest.json` is strictly justified:

| Permission | Scope | Exact Engineering Justification | Why It Cannot Be Removed |
| :--- | :--- | :--- | :--- |
| **`nativeMessaging`** | Required | Allows `runtime.connectNative("com.flux.downloadmanager")` to spawn `flux-nmh.exe`. | Primary secure IPC mechanism (`ADR-0003`); eliminates localhost HTTP servers. |
| **`downloads`** | Required | Allows listening to `downloads.onCreated` / `onDeterminingFilename` (`FR-008`) and canceling browser downloads once `flux.exe` accepts the task. | Required to intercept browser-initiated downloads automatically. |
| **`webRequest`** | Required | Observational read-only access to `onBeforeSendHeaders` and `onHeadersReceived` to detect `.m3u8`/`.mpd` manifests and capture transient `Referer`/`Cookie` headers (`FR-014`). | Without `webRequest`, MSE-fetched manifests (`hls.js`/`dash.js`) are invisible to isolated content scripts. |
| **`storage`** | Required | Stores user exclusion lists (`chrome.storage.sync`/`local`) and survives MV3 Service Worker suspension via `chrome.storage.session` (`RISK-002`). | Required for MV3 state persistence. |
| **`contextMenus`** | Required | Adds `"Download with Flux"` and `"Download selected links with Flux"` (`FR-010`). | Required for right-click user workflows. |
| **`scripting`** | Required | Dynamically executes or injects the Layer 3 `MAIN`-world hook (`catch.js`) into webpage DOM (`FR-017`, `BUG-032`). | Allows deep search for obfuscated HLS/DASH streams in page JavaScript context. |
| **`tabs`** | Required | Queries active tab info and URLs for media classification, watch URL guard, and badge counts. | Required for popup tab binding and per-site capture toggle. |
| **`host_permissions`: `["*://*/*"]`** | Required | Needed by `webRequest` to observe media CDN requests (which are almost always hosted on third-party CDN domains different from the tab's top-level origin, e.g., `*.akamaized.net`, `*.cloudfront.net`). | Cross-origin CDN requests cannot be observed with `activeTab` alone. |


**Permissions Explicitly Excluded (Compared to Cat Catch):**
- **No `declarativeNetRequest`:** OpenIDM performs all downloading in `openidm.exe`, never inside the browser, so the extension never needs to modify browser request headers.
- **No `tabs` permission (where avoidable) / No browsing history access:** Tab URLs and titles are read only for tabs that actively trigger media detection or context-menu actions.
- **No `tabCapture` / `desktopCapture`:** Screen recording is out of scope.

---

## 4. In-Page Video Overlay Design (`content/VideoOverlay.ts`)

1. **Closed Shadow DOM Isolation (`FR-015`):**
   - When a qualifying `<video>` element (`width >= 240 && height >= 135 && duration > 3.0`) is observed, the content script creates a lightweight host element `<div data-openidm-overlay>` and attaches a shadow root with `attachShadow({ mode: "closed" })`.
   - All CSS styles are self-contained inside the closed shadow root (`all: initial; position: fixed; z-index: 2147483647; pointer-events: auto;`), preventing host page CSS from distorting the button and preventing page scripts from probing the extension DOM.
2. **Non-Intrusive Visibility Rules:**
   - The overlay button remains hidden (`opacity: 0; pointer-events: none`) until the user moves the pointer within the `<video>` bounding rect (tracked via passive `mousemove` / `mouseenter` listeners) and auto-hides after `2,500 ms` of cursor inactivity or when the user clicks the `[x]` dismiss button for that player.
3. **Iframe Coordination:**
   - Many video players run inside cross-origin `<iframe>` embeds. Because `content_scripts` runs with `"all_frames": true`, the overlay attaches directly inside the iframe housing the `<video>` element, while querying the background Service Worker for all media candidates matched to `(tabId, frameId)` or parent `tabId`.

---

## 5. Cross-Browser Differences: Chromium vs. Firefox

| Aspect | Chromium (Chrome, Edge, Brave) | Mozilla Firefox | OpenIDM Handling Strategy |
| :--- | :--- | :--- | :--- |
| **Manifest Background Key** | `"background": { "service_worker": "background.js", "type": "module" }` | `"background": { "scripts": ["background.js"], "type": "module" }` (Event Page) | Generated from `manifest.base.json` during `npm run build:chromium` vs `npm run build:firefox`. |
| **Extension ID & Native Messaging Origin** | Uses 32-char `chrome-extension://<id>/` derived from manifest `"key"` in `allowed_origins`. | Uses `"browser_specific_settings": { "gecko": { "id": "extension@openidm.org" } }` in `allowed_extensions`. | Installer writes both `org.openidm.native.chromium.json` and `org.openidm.native.firefox.json` ([NATIVE_MESSAGING_AND_IPC.md](NATIVE_MESSAGING_AND_IPC.md)). |
| **Download Interception Timing** | Supports `chrome.downloads.onDeterminingFilename` (fires before target file is created on disk). | Uses `browser.downloads.onCreated` + `webRequest.onHeadersReceived` (`Content-Disposition: attachment`). | `DownloadInterceptor.ts` binds `onDeterminingFilename` when defined (Chromium) and falls back to `onCreated` + header inspection on Firefox. |
| **`webRequest` Extra Headers** | Requires `'extraHeaders'` in `OnBeforeSendHeadersOptions` to read `Cookie` and `Referer`. | Does not support/require `'extraHeaders'` string in options array (throws if passed on older Gecko). | Feature-detect `chrome.webRequest.OnBeforeSendHeadersOptions.EXTRA_HEADERS` before appending `'extraHeaders'`. |

---

## 6. Avoiding Conflicts with Duplicate Extensions or Handlers (`FR-012`)

1. When intercepting a download in `DownloadInterceptor.ts`, the extension checks `downloadItem.byExtensionId`: if the download was initiated by another browser extension (such as a PDF generator or Cat Catch), OpenIDM ignores it so it does not break in-browser blob saves.
2. Before calling `chrome.downloads.cancel(downloadItem.id)`, the extension awaits a synchronous RPC acknowledgement (`download.offer -> { accepted: true, taskId: "..." }`, timeout `800 ms`) from `openidm.exe`. If `openidm.exe` is not installed, disabled, or times out, the browser download proceeds uninterrupted.

---

## 7. Client-Side and Browser-Generated File Capture (Blob / Data URLs)

1. **Architecture:** When web applications generate files in-memory (`Blob`, `data:` URLs, ChatGPT code/artifact exports) and trigger downloads programmatically via `HTMLAnchorElement.prototype.click()` or `dispatchEvent()`, external download processes (WinHTTP) cannot access the ephemeral in-memory URLs due to browser sandboxing and cross-origin isolation.
2. **Delayed Revocation & Chunked Streaming:**
   - The Layer 3 MAIN-world hook (`catch.js`) intercepts `URL.createObjectURL` to retain in-memory `Blob` handles and delays `URL.revokeObjectURL` by 30 seconds to prevent premature garbage collection.
   - On anchor click or synthetic dispatch, `catch.js` reads the `Blob` in bounded 256 KB slices and converts them to base64.
   - Chunks are streamed via JSON-RPC (`download.startChunkedTransfer`, `download.transferChunk`, `download.completeChunkedTransfer`) directly into a staging file in `%LOCALAPPDATA%\Flux\tmp\`.
   - Upon completion, `openidm.exe` moves the file to the user's download directory, executes a Windows Defender antivirus scan, and notifies the desktop UI.
3. **Fail-Safe Browser Fallback:** If the desktop application is offline or the native messaging RPC rejects the transfer, the extension triggers a fallback with an explicit bypass flag (`fluxBypass`), ensuring the browser handles the download normally without data loss.
