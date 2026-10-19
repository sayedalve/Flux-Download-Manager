# ADR-0003: Browser-to-Desktop Communication Architecture

- **Date:** 2026-10-16
- **Status:** Proposed
- **Deciders:** Application Security Engineer, Browser Extension Engineer, Senior Windows Systems Engineer
- **Related Documents:** [../NATIVE_MESSAGING_AND_IPC.md](../NATIVE_MESSAGING_AND_IPC.md), [../SECURITY_THREAT_MODEL.md](../SECURITY_THREAT_MODEL.md), [0002-process-and-module-boundaries.md](0002-process-and-module-boundaries.md)

---

## Status

**Proposed** (Baseline Security & IPC Decision for Milestone 2).

## Context

The OpenIDM browser extension (running in Chrome, Edge, Brave, and Firefox) must communicate bi-directionally with the OpenIDM desktop application (`openidm.exe`) to:
1. Check whether `openidm.exe` is running and negotiate protocol versions.
2. Forward intercepted file downloads (`URL`, `Referer`, `Cookie`, `User-Agent`, suggested filename).
3. Push or query detected streaming manifests (`.m3u8`, `.mpd`) and trigger the desktop Track Selection dialog when the user clicks the in-page "Download Video" button.

Upstream projects use risky or fragile mechanisms:
- **[Brisk](https://github.com/BrisklyDev/brisk-browser-extension/activity)** opens a localhost HTTP server on a fixed port and sends `fetch()` requests from the extension background script.
- **[Cat Catch](https://o2bmm.gitbook.io/cat-catch/docs/m3u8dl)** invokes external downloaders via custom Windows URL protocols (`m3u8dl://` or `m3u8dlre:` registered via `URLProtocol.exe`).

## Decision

1. **Primary Transport:** Use the standard **Browser Native Messaging API** (`chrome.runtime.connectNative("org.openidm.native")` / `browser.runtime.connectNative("org.openidm.native")`) connected to `openidm-nmh.exe`, which bridges messages over a **per-user ACL-protected Windows Named Pipe** (`\\.\pipe\OpenIDM_IPC_<UserSID>`) to `openidm.exe`.
2. **Protocol Framing:** Length-prefixed JSON-RPC 2.0 (`uint32_le` byte length followed by UTF-8 JSON payload, capped at `1 MiB` per message).
3. **Explicitly Prohibited Mechanisms:**
   - **Never** register a custom URL protocol (`openidm://` or `m3u8dl://`) that passes webpage strings onto a command line.
   - **Never** run an unauthenticated localhost HTTP or WebSocket server.
4. **Opt-In Fallback for Restricted Environments:** Only if a user runs OpenIDM in an environment where registry keys (`HKCU\Software\...\NativeMessagingHosts`) are blocked by enterprise Group Policy, provide an **off-by-default Authenticated Loopback Fallback** (`127.0.0.1` only, ephemeral port, requiring a 256-bit cryptographic Bearer secret generated via `BCryptGenRandom` and manually pasted by the user into Extension Options, plus strict `Origin: chrome-extension://<id>` validation).

## Alternatives Considered

1. **Unauthenticated Localhost HTTP / WebSocket Server (Used by Brisk) (`Rejected`):**
   - *Security Flaw:* Any malicious website visited in the user's browser can scan `127.0.0.1` ports or exploit DNS rebinding / simple CORS requests to silently queue malicious downloads (`.exe` / `.dll` drops) or exfiltrate task status. It also triggers Windows Defender Firewall prompts on first run and suffers from port collisions (`Fix Brisk port not...`).
2. **Custom Windows URL Protocol (`m3u8dl://` — Used by Cat Catch) (`Rejected`):**
   - *Security Flaw:* One-way only (no response back to the browser), triggers repetitive browser confirmation popups (`Always allow site to open m3u8dl links?`), truncates long URLs/cookies at shell length limits, and is notoriously vulnerable to command-line argument injection (CVE class found in numerous protocol handlers).
3. **Direct Browser-to-Named-Pipe Connection (`Not Possible`):**
   - Browser Web/Extension sandboxes cannot open `\\.\pipe\*` directly without a Native Messaging Host executable (`openidm-nmh.exe`).

## Rationale

- **OS-Enforced Authentication:** Browser Native Messaging verifies the extension ID (`allowed_origins`: `chrome-extension://<id>/` for Chromium; `allowed_extensions`: `["extension@openidm.org"]` for Firefox) before spawning `openidm-nmh.exe`. Arbitrary webpages cannot talk to `openidm-nmh.exe`.
- **Multi-User Isolation:** The Named Pipe `\\.\pipe\OpenIDM_IPC_<UserSID>` uses an explicit Win32 DACL (`SECURITY_ATTRIBUTES`) granting access solely to the current user's logon SID (`TokenUser`) and `SYSTEM`, plus `FILE_FLAG_FIRST_PIPE_INSTANCE` to prevent pipe-squatting on multi-user Windows systems (`NFR-008`).
- **Auto-Wake Capability:** If the user closed `openidm.exe`, clicking "Download Video" in the browser causes the browser to spawn `openidm-nmh.exe`, which automatically starts `openidm.exe --background` and completes the RPC seamlessly.

## Consequences

- **Positive:** Zero listening TCP/UDP ports (`NFR-007`), zero Windows Firewall prompts, bi-directional RPC with request/response correlation, and immunity to web-origin CSRF/DNS rebinding.
- **Negative:** Requires registering the Native Messaging Host JSON manifest path in `HKCU` during installation (or via a `register-nmh.cmd` helper in developer setups), and requires pinning the extension's public key in `manifest.chromium.json` so the extension ID remains constant across developer and store builds.

## Risks & Mitigations

- **Risk:** Extension ID changes during development if `key` is omitted from `manifest.chromium.json`, causing Native Messaging `Specified native messaging host not found` errors.
- **Mitigation:** Commit a fixed development RSA public `"key"` in `extension/manifest.chromium.json` so the unpacked extension ID is deterministic on all developer machines ([../DEVELOPMENT_SETUP.md](../DEVELOPMENT_SETUP.md)).
