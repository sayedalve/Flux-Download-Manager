# Audit of `IDM_RE_Results_Bundle.zip`

## Executive summary

The uploaded ZIP contains useful primary artifacts: extracted IDM browser-extension source, Ghidra decompilations, native-messaging manifests, file inventory, logs, and an architecture report. This is a meaningful step beyond the earlier string-only investigation. I reviewed the actual files inside the ZIP, not just Antigravity's narrative.

The core flow is now supported by several independent static-code observations. However, the architecture report should not be treated as wholly verified: a few claims are overstated, one registry-path check is incorrectly reported as a missing executable, the aggregate Ghidra status file only records `IDMan.exe`, and some components (notably `IDMVMPrs.dll` and `idmBroker.exe`) were not meaningfully decompiled in the provided reports.

## Findings supported by the files in the ZIP

### 1. The browser extension observes both page media elements and network responses

`extensions\IDMEdgeExt\content.js` contains `MutationObserver`, `ResizeObserver`, and `IntersectionObserver` logic. It searches `video`, `audio`, `object`, and `embed` elements, reads their source/data values, measures visible element geometry with `getBoundingClientRect()`, and posts element information to the extension's background context (for example, message type `41`).

`extensions\IDMEdgeExt\background.js` registers `webRequest` listeners including `onBeforeRequest`, `onBeforeSendHeaders`, `onHeadersReceived`, `onResponseStarted`, and `onErrorOccurred`. The response handler calls `yc(...)`, which checks response statuses (`200`, `206`, `304`), obtains `Content-Type` and `Content-Disposition`, normalizes the MIME type, maps MIME types to filename extensions, and applies URL/host-specific rules. It has handling for manifest-like formats such as `M3U8` and `MPD`, including code paths that retrieve a response body as text or a blob.

This is stronger than merely finding strings: the code includes active event registration and classification logic. It still does not prove that every supported site/format works in practice.

### 2. The extension tries local WebSocket endpoints before Native Messaging

In `extensions\IDMEdgeExt\background.js`, the connection routine attempts:

- `ws://127.0.0.1:1001/` with subprotocol `plugin.v3.internetdownloadmanager.com`
- `ws://0.1.0.1:1001/`
- then `browser.runtime.connectNative("com.tonec.idm")`

The retry routine cycles through these connection options. Therefore, the report's statement that the browser communicates **solely** with `IDMMsgHost.exe` via Native Messaging is inaccurate. The code attempts the WebSocket endpoints first and Native Messaging after those attempts. This proves the client-side connection strategy, but the provided static code does not by itself prove which route was successfully used during a live browser session.

### 3. `IDMMsgHost.exe` bridges standard I/O to an IDM named pipe

The extracted native messaging manifest says:

```json
{
  "name": "com.tonec.idm",
  "type": "stdio",
  "path": "IDMMsgHost.exe"
}
```

The decompilation of `IDMMsgHost.exe` function `FUN_00402830` obtains standard input/output handles, constructs `\\.\pipe\IDMNetworkMonitor.<session>`, and calls `CreateFileW` on that pipe. It also dynamically resolves `GetNamedPipeServerProcessId` and `GetNamedPipeClientProcessId`.

`IDMNetMon.dll` function `FUN_10013d30` constructs the same session-specific `IDMNetworkMonitor` pipe name and initializes Winsock/socket state. This is strong evidence of a native-host-to-IDM-network-monitor bridge. `IDMNetMon.dll` function `FUN_10034b40` builds internal messages beginning with `MSG#%u#%u#%u#%u` and appends colon-delimited fields.

**Important caveat:** the registry report marks the native-host executable as `Exists: False`, but the manifest's `path` value is relative (`IDMMsgHost.exe`). The report's resolver appears to test that relative string without resolving it against the installation/manifest directory. That `False` is not proof that the executable is absent; the binary was present in the installation and was copied into the analysis bundle. The actual installed registration and whether the browser accepts the manifest path should be verified separately if runtime integration fails.

### 4. The video panel is strongly supported as a native Win32 overlay

`idmcchandler7.dll` function `FUN_1000e990` calls `CreateWindowExA` with the window class `IDM_Download_Panel_controller2_class`, then runs a Windows message loop.

`idmbrbtn.dll` function `FUN_10003c30` filters media entries against format names such as `TS`, `MP4`, `M4V`, `M4A`, `AAC`, `MP3`, `AC3`, and `EAC3`. It calls `SetWindowPos` with flags `0x40` or `0x80`, corresponding to showing or hiding a window. Together with the content script's media-element geometry messages, this strongly supports a native-window panel positioned relative to browser media rather than a panel rendered entirely as ordinary HTML inside the page.

The report's exact formula using `window.screenX` and `window.outerHeight` is not supported by the extracted `content.js` that I inspected. The code instead uses `getBoundingClientRect()`, visibility/overflow checks, and a scale factor derived from `devicePixelRatio`/document dimensions. Do not reuse the report's `screenX` formula as a verified fact.

### 5. Native code handles MIME types and internal stream schemes

- `idmcchandler7.dll` function `FUN_10016f10` explicitly compares response MIME types, including `video/mp4`, `video/webm`, `video/avi`, `video/mpeg`, `video/x-ms-wmv`, and other formats. This is actual classification code, not just strings.
- `idmcchandler7.dll` function `FUN_100230a0` reads a path from the IDM registry configuration, loads `idmvmprs.dll`, resolves the export `Parse1`, and passes manifest-like data to it. It iterates returned entries and builds `mpdtoidm://` or `mpdatoidm://` URLs with selected IDs and displays resolution/bitrate metadata. This is strong evidence that `IDMVMPrs.dll!Parse1` participates in media-manifest parsing, although its internal implementation is not shown in the ZIP's Ghidra report.
- `IDMan.exe` function `FUN_00426f10` explicitly branches on schemes including `dhstoidm://`, `hlstoidm://`, `idmhlstomp4://`, `hls2toidm://`, `mpdtoidm://`, `mpdatoidm://`, and `ytatoidm://`. It translates them into internal processing modes.
- `IDMan.exe` function `FUN_00430560` builds a MIME/content-type database and reads the Windows `MIME\Database\Content Type` registry area.
- `IDMan.exe` function `FUN_00592d50` includes branches checking `application/x-mpegURL`, `application/vnd.apple.mpegurl`, `video/MP2T`, `video/`, and URL patterns including `.googlevideo.com` and `videoplayback`; the code also handles `Referer` in this function.
- `IDMan.exe` function `FUN_0058c7f0` is associated with the HTTP range/chunk download engine and `Content-Range` handling in the reports.

These files support a connected static picture: browser-side media and request/response observation, a native integration channel, a native window panel, internal manifest descriptors, and the main downloader's stream processing.

## Findings that remain unverified or overstated

1. **`idmBroker.exe` role:** its Ghidra report says zero candidate functions were decompiled. The bundle does not establish that it is strictly an options reader or prove that it has no video-related functionality. Do not rely on that claim yet. It is not the executable named in the included `com.tonec.idm` manifest; that manifest names `IDMMsgHost.exe`.
2. **`IDMVMPrs.dll` internals:** its report contains zero decompiled functions. Its use through `GetProcAddress(..., "Parse1")` is visible in `idmcchandler7.dll`, but the parser's implementation and exact format support were not fully reverse engineered.
3. **WebSocket server activation:** extension code proves client attempts to connect to loopback port 1001, and the `IDMNetMon.dll` code initializes socket state. The supplied evidence does not completely show the bind/listen/accept implementation or prove the loopback route was active at runtime.
4. **Size-threshold semantics:** the source contains size-related branches, including an `ASF` check using `Content-Length` and a 102400-byte comparison, and the main UI has text for a configurable minimum video size. The report has not fully traced the setting value into all panel decisions, so its general claim that a single threshold filters all streams should be treated cautiously.
5. **Cookies/headers:** the extension has access to and handles request headers, including `Referer`, and `navigator.userAgent` is included in some messages. The bundle does not prove every relevant cookie is always captured and replayed for each eventual download.
6. **Runtime behavior:** this is static analysis. The bundle contains no live trace demonstrating which transport wins, how a given website's player maps to detected stream candidates, or whether individual detections successfully download.
7. **Aggregate status:** `ghidra_runs_summary.json` records only the `IDMan.exe` run, although individual reports/logs exist for other targets. It is not a complete aggregate run manifest.

## Best current architecture model

```text
Browser page
  ├─ content.js observes video/audio/object/embed elements and their geometry
  └─ background.js observes network requests and response headers
        ├─ classifies MIME / extension / host-specific cases
        ├─ retrieves manifest bodies for selected cases
        └─ tries loopback WebSocket transport, then Native Messaging
                  ↓
          IDMMsgHost.exe (stdio native host)
                  ↓ named pipe: IDMNetworkMonitor.<session>
          IDMNetMon.dll / idmcchandler7.dll
                  ├─ MIME classification and stream selection
                  ├─ manifest parser loaded from IDMVMPrs.dll!Parse1
                  └─ native panel window class / button positioning via idmbrbtn.dll
                              ↓ custom internal stream URLs
          IDMan.exe
                  ├─ handles mpd/hls and other internal schemes
                  ├─ performs manifest/metadata processing
                  └─ passes jobs to HTTP/range download logic
```

This is a **static evidence-supported model**, not a runtime trace of a single request. Some exact message semantics and the full WebSocket server path remain open.

## Recommended next step

Do not rerun the inventory or the successful Ghidra runs yet. The bundle already gives enough evidence to focus the next analysis on a small number of gaps:

1. Inspect and decompile the `Parse1` export inside `IDMVMPrs.dll` (the current report did not do this).
2. Trace `IDMNetMon.dll` from socket creation into bind/listen/accept and map the `MSG#` message fields between the extension, host and monitor.
3. Confirm the manifest's relative `IDMMsgHost.exe` path resolution and determine the transport actually used in a controlled local runtime test.
4. Trace the preference that controls the minimum video size into the exact response-classification decision.

The current bundle is useful and not a total failure. The most valuable finding is that video detection is split across browser JavaScript and native components; reading `IDMan.exe` alone could not have revealed the full pipeline.
