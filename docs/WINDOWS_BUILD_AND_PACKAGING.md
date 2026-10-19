# Windows Build and Packaging Specification

**Document Status:** Active Build & Packaging Specification (`v0.3.0`)  
**Last Reviewed:** 2026-10-18  
**Scope:** Compiler and Windows SDK requirements, CMake configuration, native C++20 stream worker compilation, dynamic worker discovery, Inno Setup installer, and clean-machine distribution validation.  
**Related Documents:** [RELEASE_AND_UPDATE_PROCESS.md](RELEASE_AND_UPDATE_PROCESS.md), [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md), [FEATURE_MATRIX.md](FEATURE_MATRIX.md), [adr/0008-packaging-and-updates.md](adr/0008-packaging-and-updates.md)

---

## 1. Build Targets & Executables (`CMakeLists.txt`)

Flux Download Manager `v0.3.0` compiles four native C++20 executables:

| Target Name | Source Entry Point | Windows 11 Output | Subsystem | Purpose |
| :--- | :--- | :--- | :--- | :--- |
| **`flux`** (alias: `openidm`) | `src/app/main.cpp` (`wWinMain` / `main`) | `build/win-x64/flux.exe` | Windows GUI (`/SUBSYSTEM:WINDOWS`) | Main Desktop GUI, Unified Download Controller, Brisk HTTP Engine, Stream Supervisor, and IPC Server |
| **`flux-nmh`** (alias: `openidm-nmh`) | `src/nmh/main.cpp` | `build/win-x64/flux-nmh.exe` | Windows Console (`/SUBSYSTEM:CONSOLE`) | Browser Native Messaging Host (`stdio` length-prefixed JSON-RPC 2.0 bridge over Named Pipe) |
| **`N_m3u8DL-RE`** | `src/worker_stream/nm3u8dl_re_worker.cpp` | `tools/win-x64/N_m3u8DL-RE.exe` | Windows Console | Standalone compiled C++20 HLS/DASH stream downloader, AES-128 decryptor, and FFmpeg muxer worker |
| **`flux_unit_tests`** | `tests/cpp/unit_tests.cpp` | `build/win-x64/flux_unit_tests.exe` | Windows Console | Automated 13-suite C++20 unit and regression test binary |

---

## 2. Dynamic Worker & Directory Resolution

Flux dynamically resolves all operational directories and worker binaries at runtime via `PathSanitizer` (`src/common/path_sanitizer.cpp`):

1. **Executable Directory (`PathSanitizer::GetExecutableDir()`):**
   - Resolves the directory containing `flux.exe` via `GetModuleFileNameW(nullptr, ...)`.
2. **Application Data Directory (`PathSanitizer::GetDefaultAppDataDir()`):**
   - Resolves `%LOCALAPPDATA%\Flux` on Windows 11 (`state.db`, `state.db-wal`, `flux.log`, `tmp\`). Preserves backward-compatibility with `%LOCALAPPDATA%\OpenIDM` if legacy state exists.
3. **Default Downloads Directory (`PathSanitizer::GetDefaultDownloadsDir()`):**
   - Resolves `%USERPROFILE%\Downloads\Flux` on Windows 11 (customizable in Settings and persisted in `state.db`).
4. **Stream Worker Binary Discovery (`PathSanitizer::FindWorkerBinary()`):**
   - Searches `<exe_dir>\tools\win-x64\<name>.exe`, `<exe_dir>\tools\<name>.exe`, `<exe_dir>\<name>.exe`, and the system `PATH` environment variable.
   - Allows users to either use the bundled `tools\win-x64\N_m3u8DL-RE.exe` and `ffmpeg.exe` or provide existing system installations.

---

## 3. Reproducible Build Commands

### 3.1 Windows 11 `x64` Native Build Pipeline
From a PowerShell terminal with Visual Studio 2022 (MSVC v143) and CMake 3.25+:

```powershell
# Compiles flux.exe, flux-nmh.exe, provisions stream workers, runs unit/extension tests,
# and stages portable binaries:
.\scripts\build-windows-msvc.ps1
```

### 3.2 Assembling Standalone Portable Release Packages
```powershell
# Assembles both Full and Core portable zip archives with SHA256SUMS.txt:
powershell -ExecutionPolicy Bypass -File scripts\package-release-zip.ps1 -Version "0.3.0" -PackageType Both
```

### 3.3 Compiling Inno Setup 6 Installer
```powershell
# Compiles dist\Flux-Setup-x64-0.3.0.exe:
& "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" installer\flux.iss
```

---

## 4. Inno Setup Installer (`installer/flux.iss`)

- **Per-User Non-Elevated Execution:** Uses `PrivilegesRequired=lowest` with `PrivilegesRequiredOverridesAllowed=dialog`. Installs cleanly into `{autopf}\Flux Download Manager` without prompting for UAC elevation on standard accounts.
- **Component Selection:**
  - `core` (Fixed): Desktop application, Native Messaging Host, extension, and documentation.
  - `stream_workers` (Optional): Pre-bundles `N_m3u8DL-RE.exe` and `ffmpeg.exe` for offline stream downloads.
- **Native Messaging Registry Keys (`HKCU`):**
  - `Software\Google\Chrome\NativeMessagingHosts\com.flux.downloadmanager`
  - `Software\Microsoft\Edge\NativeMessagingHosts\com.flux.downloadmanager`
  - `Software\BraveSoftware\Brave-Browser\NativeMessagingHosts\com.flux.downloadmanager`
  - `Software\Mozilla\NativeMessagingHosts\com.flux.downloadmanager`
  - Plus legacy aliases (`org.openidm.native`) for backward compatibility with existing installations.
- **Upgrade & Uninstall Safety:**
  - Uses `AppMutex=Local\Flux_SingleInstance,Local\OpenIDM_SingleInstance` to ensure clean shutdown before binary replacement.
  - Cleanly preserves `%LOCALAPPDATA%\Flux\state.db` and user download directories during uninstall and upgrades.
