# ADR-0008: Windows Packaging, Extension Distribution, and Update Architecture

- **Date:** 2026-10-16
- **Status:** Proposed
- **Deciders:** Release Engineer, Senior Windows Systems Engineer, Application Security Engineer
- **Related Documents:** [../WINDOWS_BUILD_AND_PACKAGING.md](../WINDOWS_BUILD_AND_PACKAGING.md), [../RELEASE_AND_UPDATE_PROCESS.md](../RELEASE_AND_UPDATE_PROCESS.md), [../PERFORMANCE_AND_SIZE_BUDGET.md](../PERFORMANCE_AND_SIZE_BUDGET.md)

---

## Status

**Proposed** (Baseline Packaging Decision for Milestone 4 & 5).

## Context

Delivering OpenIDM to Windows 11 users requires coordinating four distinct artifacts:
1. The core native binaries (`openidm.exe` and `openidm-nmh.exe`).
2. The stream toolchain binaries (`N_m3u8DL-RE.exe` and `ffmpeg.exe`).
3. Native Messaging Host JSON manifests and Windows Registry keys for Chrome, Edge, Brave, and Firefox.
4. The Browser Extension (`.crx` / Web Store package for Chromium and `.xpi` / AMO package for Firefox).

We must also provide a secure update path, a clean uninstaller, and honest size accounting.

## Decision

1. **Installer Technology:** Use **Inno Setup 6** (`installer/openidm.iss`) with `PrivilegesRequiredOverridesAllowed=dialog` so users can choose **Per-User installation** (`%LOCALAPPDATA%\Programs\OpenIDM`, requiring zero UAC admin elevation) or **Machine-Wide installation** (`%ProgramFiles%\OpenIDM`).
2. **Two Packaging Flavors (Honest Size Trade-off):**
   - **Full Offline Installer (`OpenIDM-Setup-x64-Full.exe`, Default Recommended):** Bundles `openidm.exe`, `openidm-nmh.exe`, `N_m3u8DL-RE.exe`, and a minimal mux-focused `ffmpeg.exe` build inside a single LZMA2-Ultra compressed installer (~`18–33 MB` compressed, ~`45–90 MB` installed). Works 100% offline out-of-the-box for both ordinary files and HLS/DASH streams.
   - **Core-Only Installer (`OpenIDM-Setup-x64-Core.exe`):** Bundles only `openidm.exe` and `openidm-nmh.exe` (~`1.2 MB` compressed, ~`2.5 MB` installed). Ordinary HTTP/HTTPS downloads work immediately; if the user initiates a stream download and `N_m3u8DL-RE.exe` / `ffmpeg.exe` are not present in `%LOCALAPPDATA%\OpenIDM\tools\` or `PATH`, OpenIDM offers a one-click verified download of the pinned worker bundle (verified via SHA-256 + Ed25519 signature).
3. **Native Messaging Registration:**
   - The installer generates `org.openidm.native.chromium.json` and `org.openidm.native.firefox.json` in the installation directory with the resolved absolute path to `openidm-nmh.exe` and writes the corresponding `HKCU\Software\...\NativeMessagingHosts\org.openidm.native` registry keys.
4. **Update Architecture:**
   - Opt-in background check fetches `https://releases.openidm.org/v1/manifest.json` (or GitHub Releases latest JSON) + `manifest.json.sig`.
   - `openidm.exe` verifies the manifest signature against an embedded **Ed25519 public key**, verifies the downloaded installer's **SHA-256** digest, and verifies the installer's **Authenticode signature** (`WinVerifyTrust`) before launching the silent upgrade (`/VERYSILENT /SUPPRESSMSGBOXES /NORESTART`).
5. **Clean Uninstallation:**
   - Before removing files, the uninstaller signals any running `openidm.exe` instance via WM_CLOSE / mutex check and terminates any active `openidm-nmh.exe` instances, removes all `NativeMessagingHosts\org.openidm.native` registry keys, and asks the user whether to keep or delete `%LOCALAPPDATA%\OpenIDM` (`state.db` and logs).

## Alternatives Considered

1. **MSIX / Windows Store Package Only (`Rejected as Primary`):**
   - *Why Rejected:* MSIX virtualizes the filesystem and registry inside an AppContainer, which complicates registering Native Messaging Hosts for non-Store browsers (portable Firefox, Brave) and requires paid trusted certificates even for local developer installation. Inno Setup `.exe` + WinGet manifest (`OpenIDM.OpenIDM`) provides frictionless installation for all browsers.
2. **WiX Toolset MSI (`Rejected`):**
   - *Why Rejected:* Higher XML complexity for dual per-user/per-machine dialogs and weaker compression ratios than Inno Setup 6's solid LZMA2-Ultra compression.
3. **Silent Auto-Install of Unpacked Browser Extensions via Registry Force-Install (`Rejected`):**
   - *Why Rejected:* Modern Chrome and Edge block `HKCU\Software\Google\Chrome\Extensions` side-loading to prevent malware abuse. Instead, OpenIDM provides a clear "Install Browser Extension" dialog inside the app with direct links to Chrome Web Store, Microsoft Edge Add-ons, Firefox AMO, and local `.crx`/`.xpi` instructions.

## Rationale

Inno Setup 6 + WinGet provides the smallest compressed installer, supports non-admin per-user installs, registers Native Messaging keys cleanly, and pairs with cryptographic Ed25519 + Authenticode verification for tamper-proof updates.

## Consequences

- **Positive:** Transparent choice between a `~1.2 MB` Core installer and an all-in-one `~18–33 MB` Full installer; zero UAC requirement for per-user installs.
- **Negative:** Requires maintaining pinned SHA-256 + Ed25519 manifests for the on-demand worker downloader used by the Core installer.

## Risks & Mitigations

- **Risk (`RISK-008`):** Unsigned installers trigger Windows SmartScreen warnings.
- **Mitigation:** Integrate SignPath.io or Azure Trusted Signing into the GitHub Actions release workflow prior to public Milestone 4 builds ([../RELEASE_AND_UPDATE_PROCESS.md](../RELEASE_AND_UPDATE_PROCESS.md)).
