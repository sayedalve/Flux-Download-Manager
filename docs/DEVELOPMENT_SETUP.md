# Developer Environment Setup Guide

**Document Status:** Proposed Setup Guide (Pre-Implementation)  
**Last Reviewed:** 2026-10-16  
**Scope:** Preparing a clean Windows 11 `x64` workstation to build, debug, and test OpenIDM once Milestone 1 scaffolding is committed.  
**Related Documents:** [WINDOWS_BUILD_AND_PACKAGING.md](WINDOWS_BUILD_AND_PACKAGING.md), [TECHNOLOGY_STACK.md](TECHNOLOGY_STACK.md), [REPOSITORY_AUDIT.md](REPOSITORY_AUDIT.md), [TEST_STRATEGY.md](TEST_STRATEGY.md)

---

## 1. Current Repository State Notice

> **IMPORTANT:**  
> As documented in [REPOSITORY_AUDIT.md](REPOSITORY_AUDIT.md), as of **2026-10-16**, this repository contains engineering documentation only (`CMakeLists.txt`, `vcpkg.json`, `src/`, and `extension/` have not been created yet).  
> The commands below define the **target developer workflow** that Milestone 1 (`M1`) and Milestone 2 (`M2`) will implement. Do not assume these build scripts already exist until `M1` is committed.

---

## 2. Required & Optional Tools on Windows 11 x64

### 2.1 Required Tools
1. **Git for Windows** (`2.43+`)
2. **Visual Studio 2022 Community / Professional / Enterprise** (`17.8+`) or **Visual Studio Build Tools 2022** with workload:
   - *Desktop development with C++*
   - *MSVC v143 - VS 2022 C++ x64/x86 build tools*
   - *Windows 11 SDK (10.0.22621.0 or 10.0.26100.0)*
   - *C++ CMake tools for Windows*
3. **vcpkg** (installed standalone or bundled with Visual Studio 2022; `VCPKG_ROOT` environment variable set).
4. **Node.js** (`20 LTS+` or `22 LTS+`) & `npm` (for building and testing `extension/`).
5. **Python** (`3.11+` with `aiohttp` and `pytest`) — required only for running the deterministic local HTTP/HLS/DASH fault-injection test server (`tests/integration/`).

### 2.2 Optional Tools
- **Inno Setup 6 (`6.3+`):** Required only when building the `.exe` installer (`installer/openidm.iss`).
- **Windows Sandbox:** Built-in Windows 11 Pro/Enterprise feature used for clean-machine installer testing (`SYS-INST-01`).
- **Sysinternals Process Explorer & Process Monitor:** Recommended for verifying Job Object child termination (`IT-STREAM-04`), Named Pipe DACLs (`SEC-IPC-01`), and DLL load hygiene (`SEC-BIN-01`).

### 2.3 Quick Toolchain Installation via `winget` (PowerShell)
```powershell
winget install --id Git.Git -e
winget install --id Microsoft.VisualStudio.2022.BuildTools -e --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
winget install --id OpenJS.NodeJS.LTS -e
winget install --id Python.Python.3.12 -e
winget install --id JRSoftware.InnoSetup -e
```

---

## 3. Target Build & Test Workflow (Once Milestone 1 & 2 Are Scaffolded)

### 3.1 Provisioning Pinned Stream Worker Binaries (`N_m3u8DL-RE.exe` & `ffmpeg.exe`)
During Milestone 3 implementation, `scripts/fetch-workers.ps1` will download and verify the SHA-256 digests of the pinned upstream worker releases into `build/tools/`:
- `N_m3u8DL-RE` (`v0.6.0-beta` `win-x64` NativeAOT binary from [nilaoda/N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE))
- `ffmpeg.exe` (`win-x64` GPL/LGPL release build)

### 3.2 Building `openidm.exe`, `openidm-nmh.exe`, and C++ Unit Tests
From a **x64 Native Tools Command Prompt for VS 2022** (or PowerShell with `VsDevCmd.bat -arch=amd64`):

```powershell
# 1. Configure Debug build with Ninja and vcpkg manifest mode
cmake -B build/debug -G Ninja `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows-static

# 2. Compile desktop app, native messaging host, and test binaries
cmake --build build/debug

# 3. Run C++ unit tests
ctest --test-dir build/debug --output-on-failure

# 4. Configure & build optimized Release binaries (/O2 /GL /LTCG /MT)
cmake -B build/release -G Ninja `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build build/release
```

### 3.3 Building and Loading the Browser Extension (`extension/`)
```powershell
cd extension
npm ci
npm run test          # Runs Vitest unit tests (UT-EXT-*)
npm run build:chromium # Emits extension/dist/chromium/
npm run build:firefox  # Emits extension/dist/firefox/
```

**Loading the Unpacked Extension for Local Debugging:**
1. **Register the Debug Native Messaging Host in `HKCU`:**
   - Run `build\debug\openidm.exe --register-dev-nmh` (which writes `org.openidm.native.chromium.json` pointing to `build\debug\openidm-nmh.exe` and registers `HKCU\Software\Google\Chrome\NativeMessagingHosts\org.openidm.native`).
2. **Chrome / Edge / Brave:**
   - Open `chrome://extensions` (or `edge://extensions`), enable **Developer mode**, click **Load unpacked**, and select `extension/dist/chromium`.
   - Confirm the extension ID matches the deterministic ID derived from the development `"key"` in `manifest.chromium.json`.
3. **Firefox:**
   - Open `about:debugging#/runtime/this-firefox`, click **Load Temporary Add-on...**, and select `extension/dist/firefox/manifest.json`.

---

## 4. Troubleshooting Common Setup & Debugging Failures

| Symptom / Error | Root Cause | Resolution |
| :--- | :--- | :--- |
| **`Specified native messaging host not found`** in Chrome/Edge Service Worker console | Either `HKCU\Software\...\NativeMessagingHosts\org.openidm.native` is not registered, or the path to `openidm-nmh.exe` in the JSON manifest is relative instead of absolute. | Run `openidm.exe --register-dev-nmh` and verify the JSON `"path"` contains an escaped absolute Windows path (`C:\\...\\openidm-nmh.exe`). |
| **`Access to the specified native messaging host is forbidden`** | The unpacked extension's ID in `chrome://extensions` does not match the ID in `org.openidm.native.chromium.json` `"allowed_origins"`. | Ensure `"key"` is present in `manifest.chromium.json` so Chrome assigns the fixed development extension ID. |
| **`N_m3u8DL-RE.exe` exits immediately with `Init log failed! Access to the path is denied`** (Upstream Issue #819) | `N_m3u8DL-RE` defaults to creating a `Logs/` folder next to its executable (`C:\Program Files\OpenIDM\tools\Logs`), which fails when installed under `%ProgramFiles%` and run as a non-admin user! | Always pass `--no-log` (or `--log-file-path "%LOCALAPPDATA%\OpenIDM\logs\re_<task_uuid>.log"`) in `Nm3u8dlReAdapter` ([STREAM_DOWNLOAD_PIPELINE.md](STREAM_DOWNLOAD_PIPELINE.md)). |
| **MSVC Linker mismatch (`LNK2038: mismatch detected for 'RuntimeLibrary'`)** | Mixing `/MD` (dynamic CRT) vcpkg packages with `/MT` (static CRT) application targets. | Always pass `-DVCPKG_TARGET_TRIPLET=x64-windows-static` consistently. |
