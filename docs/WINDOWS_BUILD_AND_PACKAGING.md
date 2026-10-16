# Windows Build and Packaging Specification

**Document Status:** Active Build & Packaging Specification (`v0.2.0`)  
**Last Reviewed:** 2026-10-16  
**Scope:** Compiler and Windows SDK requirements, CMake/Makefile configuration, native C++20 stream worker compilation, dynamic worker discovery, Inno Setup installer, and clean-machine validation.  
**Related Documents:** [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md), [PRODUCT_COMPLETION_AUDIT.md](PRODUCT_COMPLETION_AUDIT.md), [PERFORMANCE_AND_SIZE_BUDGET.md](PERFORMANCE_AND_SIZE_BUDGET.md)

---

## 1. Build Targets & Executables (`CMakeLists.txt` & `Makefile`)

OpenIDM `v0.2.0` compiles four native C++20 executables from the unified source tree:

| Target Name | Source Entry Point | Windows 11 Output | Linux `x86_64` Output (`Verified by Execution`) | Purpose |
| :--- | :--- | :--- | :--- | :--- |
| **`openidm`** | `src/app/main.cpp` (`main` + `wWinMain`) | `build/win-x64/openidm.exe` | `build/openidm` (`641.1 KB` stripped) | Main Windows 11 Desktop GUI, Unified Download Controller, Brisk HTTP Engine, Stream Supervisor, and IPC Server |
| **`openidm-nmh`** | `src/nmh/main.cpp` | `build/win-x64/openidm-nmh.exe` | `build/openidm-nmh` (`633.1 KB` stripped) | Browser Native Messaging Host (`stdio` length-prefixed JSON-RPC 2.0 bridge over Named Pipe / UDS) |
| **`N_m3u8DL-RE`** | `src/worker_stream/nm3u8dl_re_worker.cpp` | `tools/N_m3u8DL-RE.exe` | `tools/N_m3u8DL-RE` (`276.4 KB` stripped) | Standalone compiled C++20 HLS/DASH stream downloader, AES-128 decryptor, and FFmpeg muxer worker (zero Python dependency) |
| **`openidm_unit_tests`** | `tests/cpp/unit_tests.cpp` | `build/win-x64/openidm_unit_tests.exe` | `build/openidm_unit_tests` | Automated 10-suite C++20 unit and regression test binary |

---

## 2. Dynamic Worker & Directory Resolution (`BUG-005` Fix)

OpenIDM never hardcodes POSIX paths such as `/usr/bin/ffmpeg`. At startup, `PathSanitizer` (`src/common/path_sanitizer.cpp`) dynamically resolves all directories and worker binaries:

1. **Executable Directory (`PathSanitizer::GetExecutableDir()`):**
   - Uses `GetModuleFileNameW(nullptr, ...)` on Windows 11 and `/proc/self/exe` on Linux.
2. **Application Data Directory (`PathSanitizer::GetDefaultAppDataDir()`):**
   - Resolves `%LOCALAPPDATA%\OpenIDM` on Windows 11 (`state.db`, `openidm.log`, `tmp\`).
3. **Default Downloads Directory (`PathSanitizer::GetDefaultDownloadsDir()`):**
   - Resolves `%USERPROFILE%\Downloads\OpenIDM` on Windows 11 (customizable in the Settings Dialog and persisted in `state.db`).
4. **Worker Binary Discovery (`PathSanitizer::FindWorkerBinary()`):**
   - Searches `<exe_dir>\tools\<name>.exe`, `<exe_dir>\..\tools\<name>.exe`, `<exe_dir>\<name>.exe`, and the system `PATH` environment variable (`;` delimiter on Windows, `:` delimiter on POSIX).
   - Users can either use the bundled `tools\N_m3u8DL-RE.exe` compiled by CMake or drop an upstream `nilaoda/N_m3u8DL-RE` release binary and `ffmpeg.exe` into `tools\`.

---

## 3. Reproducible Build Commands

### 3.1 Windows 11 `x64` Native Build & Installer Generation
On a Windows 11 `x64` workstation with Visual Studio 2022 (`MSVC`), CMake, Ninja, and Inno Setup 6 (`ISCC.exe`):
```powershell
# Compiles openidm.exe, openidm-nmh.exe, and N_m3u8DL-RE.exe, verifies MZ/PE32+ headers,
# runs openidm_unit_tests.exe, and builds dist\OpenIDM-Setup-x64-0.2.0.exe:
.\scripts\build-windows-msvc.ps1
```

### 3.2 Linux `x86_64` Build & Full Automated Verification Suite
```bash
make -f Makefile clean && make -f Makefile test
```

---

## 4. Dual-Mode Inno Setup Installer (`installer/openidm.iss`)

- **Per-User vs. Machine-Wide Mode:** Configured with `PrivilegesRequired=lowest` and `PrivilegesRequiredOverridesAllowed=dialog`.
- **Native Messaging Registry Keys (`HKCU`):**
  - `HKCU\Software\Google\Chrome\NativeMessagingHosts\org.openidm.native`
  - `HKCU\Software\Microsoft\Edge\NativeMessagingHosts\org.openidm.native`
  - `HKCU\Software\BraveSoftware\Brave-Browser\NativeMessagingHosts\org.openidm.native`
  - `HKCU\Software\Mozilla\NativeMessagingHosts\org.openidm.native`
- **Upgrade & Uninstall Safety:** Uses `AppMutex=Local\OpenIDM_SingleInstance` and `CloseApplications=yes` to cleanly checkpoint `state.db` during upgrades without deleting user download history.
