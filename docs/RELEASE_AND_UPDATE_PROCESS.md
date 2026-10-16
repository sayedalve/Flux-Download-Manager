# Release and Update Process Specification

**Document Status:** Proposed Release Engineering Specification  
**Last Reviewed:** 2026-10-16  
**Scope:** Semantic versioning, release branching, CI artifact pipeline, cryptographic signing (Authenticode + Ed25519), third-party worker pinning, security advisories, and rollback procedures.  
**Related Documents:** [WINDOWS_BUILD_AND_PACKAGING.md](WINDOWS_BUILD_AND_PACKAGING.md), [SECURITY_THREAT_MODEL.md](SECURITY_THREAT_MODEL.md), [ ../LICENSE_REVIEW.md](../LICENSE_REVIEW.md), [adr/0008-packaging-and-updates.md](adr/0008-packaging-and-updates.md)

---

## 1. Versioning Scheme

OpenIDM follows **Semantic Versioning 2.0.0 (`MAJOR.MINOR.PATCH`)**:
- **`0.y.z` (Pre-1.0 Milestones `M1`–`M4`):** Initial public alpha/beta releases while completing the roadmap.
- **`MAJOR`:** Incremented on breaking changes to the Native Messaging IPC protocol (`protocolVersion` major bump) or incompatible SQLite schema changes that cannot be migrated automatically.
- **`MINOR`:** Backwards-compatible new features (e.g., adding DASH Live recording or scheduled secondary queues) or pinned upgrades of `N_m3u8DL-RE` / `FFmpeg`.
- **`PATCH`:** Bug fixes, security patches, and media detection rule updates.

Both the desktop app (`VS_VERSION_INFO` in `openidm.rc`) and browser extension (`manifest.json` `"version"`) share synchronized `MAJOR.MINOR.PATCH` numbers on every release tag (`vX.Y.Z`).

---

## 2. Release Branching & CI Build Pipeline

1. **`main` Branch:** Protected trunk; all merges require passing unit tests (`UT-*`), integration tests (`IT-*`), security tests (`SEC-*`), and binary size budget gates (`BUD-001`).
2. **Release Tags (`vX.Y.Z`):** Pushing an annotated signed Git tag `vX.Y.Z` triggers the reproducible release workflow (`.github/workflows/release.yml`):
   - Compiles `openidm.exe` and `openidm-nmh.exe` in `Release` mode (`/O2 /GL /LTCG /MT /guard:cf /Brepro`).
   - Fetches and verifies the exact SHA-256-pinned `N_m3u8DL-RE.exe` (`v0.6.0-beta`) and `ffmpeg.exe` binaries.
   - Signs `openidm.exe` and `openidm-nmh.exe` with an **OV/EV Authenticode Certificate** (via SignPath.io Open Source Tier or Azure Trusted Signing).
   - Builds `OpenIDM-Setup-x64-Full.exe` and `OpenIDM-Setup-x64-Core.exe` via Inno Setup 6 and signs both installer executables with Authenticode.
   - Builds `openidm-extension-chromium-vX.Y.Z.zip` and `openidm-extension-firefox-vX.Y.Z.xpi`.

---

## 3. Published Release Artifacts & GPL-3.0 Compliance Bundle

Every GitHub Release (`vX.Y.Z`) must publish the following complete artifact set:

| Artifact Filename | Description | Verification / Compliance Purpose |
| :--- | :--- | :--- |
| `OpenIDM-Setup-x64-Full-vX.Y.Z.exe` | Full offline installer (`openidm.exe`, `openidm-nmh.exe`, `N_m3u8DL-RE.exe`, `ffmpeg.exe`). | Primary end-user installer. |
| `OpenIDM-Setup-x64-Core-vX.Y.Z.exe` | Lightweight core installer (`openidm.exe` + `openidm-nmh.exe` only, `< 1.8 MB`). | Minimal footprint option. |
| `OpenIDM-Portable-x64-vX.Y.Z.zip` | Portable archive with `register-nmh.cmd` and `unregister-nmh.cmd` scripts. | For testing / USB deployment (verified in M4). |
| `openidm-extension-chromium-vX.Y.Z.zip` | Chromium MV3 extension bundle (Chrome, Edge, Brave). | Store submission & offline load. |
| `openidm-extension-firefox-vX.Y.Z.xpi` | Signed Firefox MV3 add-on bundle. | Mozilla AMO & offline load. |
| `OpenIDM-ThirdParty-Sources-vX.Y.Z.tar.xz` | Exact source archives of bundled `FFmpeg` and `N_m3u8DL-RE` versions + build scripts. | **Satisfies GPL-3.0 Section 6** so source availability never depends on third-party repos (`VR-LIC-03`). |
| `SHA256SUMS.txt` & `SHA256SUMS.txt.sig` | SHA-256 digests of all release files + detached **Ed25519** signature. | Tamper verification by users and the built-in updater. |
| `update-manifest.json` & `update-manifest.json.sig` | Machine-readable update metadata signed with the project's offline Ed25519 release key. | Consumed by `src/core/Updater` (`FR-028`). |

---

## 4. Cryptographic Update Verification & Rollback Procedure (`FR-028`, `THREAT-010`)

### 4.1 Update Verification Sequence
When the user triggers **Check for Updates** (or enables weekly background checks):
1. `openidm.exe` fetches `update-manifest.json` and `update-manifest.json.sig` over HTTPS (`WinHTTP` with TLS 1.2/1.3).
2. Verifies the Ed25519 signature (`64 bytes`) over the raw bytes of `update-manifest.json` using the hardcoded 32-byte Ed25519 public key compiled into `openidm.exe`.
3. Compares `manifest.version` against current `APP_VERSION` (preventing downgrade attacks unless the user explicitly runs a manual downgrade installer).
4. Downloads the installer `.exe` to `%LOCALAPPDATA%\OpenIDM\updates\`, verifies its `SHA-256` against `manifest.sha256`, and calls `WinVerifyTrust` (`WINTRUST_ACTION_GENERIC_VERIFY_V2`) to confirm the Authenticode certificate chain and publisher subject name match before executing the installer.

### 4.2 Emergency Rollback & Bad-Release Revocation
If a critical regression is discovered in `vX.Y.Z`:
1. Immediately publish `vX.Y.(Z+1)` reverting the offending commit, or update `update-manifest.json` to point `latest_stable` to the last known good version while adding `"revoked_versions": ["X.Y.Z"]`.
2. Because SQLite schema migrations (`DATA_MODEL_AND_STORAGE.md`) follow an **expand-only additive rule** across minor/patch releases (never dropping columns required by the prior minor version), users who reinstall the previous `vX.Y.(Z-1)` installer retain their `state.db` download history without database corruption.
