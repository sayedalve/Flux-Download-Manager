# Technology Stack & Desktop Framework Evaluation

**Document Status:** Proposed & Accepted Stack Specification  
**Last Reviewed:** 2026-10-16  
**Scope:** Comparative evaluation of the 7 candidate desktop frameworks required by Section 4 of the Master Prompt, and the approved stack for all OpenIDM components.  
**Related Documents:** [adr/0001-desktop-framework.md](adr/0001-desktop-framework.md), [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md), [PERFORMANCE_AND_SIZE_BUDGET.md](PERFORMANCE_AND_SIZE_BUDGET.md), [WINDOWS_BUILD_AND_PACKAGING.md](WINDOWS_BUILD_AND_PACKAGING.md)

---

## 1. Executive Summary of the Selected Stack

| Layer | Selected Technology | Rationale Summary |
| :--- | :--- | :--- |
| **Desktop Application (`openidm.exe`)** | **C++20 (MSVC 2022) + Native Win32 API (`ComCtl32` v6, `UXTheme`, `Direct2D`)** | `< 2.5 MB` binary, `< 80 ms` startup, `8–15 MB` idle tray RAM, zero external runtime, direct access to `WinHTTP`, IOCP, Named Pipes, DPAPI, and Job Objects. |
| **Native Messaging Host (`openidm-nmh.exe`)** | **C++20 + Win32 Console (`stdin`/`stdout` + Named Pipe Client)** | `< 300 KB` standalone executable; launches in `< 10 ms` when invoked by the browser. |
| **HTTP Networking Engine** | **Windows `WinHTTP` API (Asynchronous Mode) + I/O Completion Ports (IOCP)** | Built into Windows 11; supports HTTP/1.1, HTTP/2, OS Schannel TLS 1.3, automatic WPAD/PAC system proxy resolution, and zero-copy buffer management without bundling OpenSSL or `libcurl`. |
| **Stream Download Engine** | **`N_m3u8DL-RE.exe` (C# .NET NativeAOT Subprocess) + `ffmpeg.exe`** | Reuses the mature MIT-licensed HLS/DASH/MSS engine via a clean CLI/JSON boundary without embedding a .NET runtime inside `openidm.exe`. |
| **Local Persistence** | **Embedded SQLite 3 (`sqlite3.c` Amalgamation in `WAL` mode)** | Public Domain, ~900 KB compiled size, ACID crash safety for sub-second byte-range offset checkpointing. |
| **Credential Protection** | **Windows DPAPI (`CryptProtectData` / `CryptUnprotectData`)** | OS-enforced encryption bound to the current Windows user logon session (`CRYPTPROTECT_UI_FORBIDDEN`). |
| **JSON Serialization** | **`nlohmann/json` (or `yyjson`)** | Battle-tested MIT library for Native Messaging JSON-RPC 2.0 and `N_m3u8DL-RE` `meta.json` parsing. |
| **Browser Extension** | **TypeScript 5.x (Strict) + Manifest V3 + Closed Shadow DOM (Zero Runtime Deps)** | Compiles to clean ES2022 scripts for Chrome, Edge, Brave, and Firefox; `< 15 KB` content script footprint. |
| **Build & Packaging** | **CMake 3.28+ + Ninja + vcpkg (Manifest Mode) + Inno Setup 6** | Reproducible declarative builds; Inno Setup produces a compact LZMA2-compressed installer supporting both per-user and machine-wide installs. |
| **Testing Frameworks** | **GoogleTest (`gtest`) + Python Deterministic HTTP/HLS/DASH Server + Playwright** | Full coverage across C++ unit tests, network protocol edge cases, and browser extension E2E flows. |

---

## 2. Comprehensive Evaluation of Desktop Framework Candidates

A download manager has a unique runtime profile: unlike a document editor or media player that is opened only when actively used, a download manager **runs continuously in the Windows Notification Area (system tray)** waiting for browser events, and periodically bursts into high-frequency UI updates (updating 8–32 segment progress bars and transfer rates at 10–30 Hz while writing 100+ MB/s to disk).

### 2.1 Quantitative & Architectural Comparison Matrix (Windows 11 x64)

*Note: Size and memory figures below represent typical baseline figures for a production multi-window desktop app on Windows 11 x64, excluding the external `ffmpeg.exe` and `N_m3u8DL-RE.exe` binaries which add ~45–85 MB installed across all frameworks (see Section 3).*

| Evaluation Criterion | 1. C++20 + Native Win32 API (Selected) | 2. C++20 + Win32 + WebView2 UI | 3. Rust + Windows API (`windows-rs` / `egui` / `iced`) | 4. C# (.NET 8/9 WinUI 3 or WPF) | 5. C++ + Qt 6 (`QtWidgets` / `QtQuick`) | 6. Flutter Desktop (Dart — used by Brisk) | 7. Tauri v2 (Rust Core + WebView2 UI) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Core App Compressed Installer Size** | **~1.2 MB** | ~2.0 MB (if WebView2 Evergreen pre-installed) | ~2.5 – 6.0 MB | ~15 – 35 MB (Self-Contained) / ~3 MB (Framework-Dependent) | ~18 – 28 MB | ~14 – 22 MB | ~3.0 – 5.0 MB |
| **Core App Installed Size** | **~2.5 MB** | ~4.5 MB (+ Evergreen runtime on OS) | ~5.0 – 14 MB | ~65 – 110 MB (Self-Contained) | ~45 – 75 MB (`Qt6Core`, `Qt6Gui`, `Qt6Widgets`, ICU) | ~35 – 55 MB (`flutter_windows.dll`, `icudtl.dat`, AOT snapshot) | ~8.0 – 12 MB (+ Evergreen runtime on OS) |
| **External Runtime Required?** | **None** (`vcruntime` linked statically or inbox UCRT) | Requires **Microsoft Edge WebView2 Runtime** (inbox on Win11, can break if removed/corrupted) | **None** | Requires .NET Desktop Runtime OR heavy self-contained bundle | None if Qt DLLs bundled | None if Flutter engine DLLs bundled | Requires **Microsoft Edge WebView2 Runtime** |
| **Cold Startup Time** | **30 – 80 ms** | 250 – 600 ms (spawns Chromium renderer进程 tree) | 40 – 120 ms | 180 – 450 ms (JIT / R2R + XAML init) | 120 – 250 ms | 200 – 450 ms (Dart VM + Impeller/Skia init) | 300 – 650 ms (WebView2 init) |
| **Idle Memory (Minimized to Tray)** | **8 – 15 MB** | 65 – 140 MB (unless WebView2 process explicitly destroyed on minimize) | 12 – 28 MB | 35 – 75 MB | 28 – 50 MB | 45 – 85 MB | 70 – 150 MB (WebView2 multi-process overhead) |
| **Peak Memory (During 1 Gbps Multi-Segment Download)** | **35 – 65 MB** | 110 – 220 MB | 40 – 70 MB | 80 – 160 MB (GC heap pressure on byte buffers) | 60 – 110 MB | 90 – 180 MB (Dart isolate heap) | 110 – 230 MB |
| **Native Messaging & Named Pipe IPC** | **First-class** (`CreateNamedPipeW`, `SECURITY_ATTRIBUTES`) | First-class in C++ backend; extra IPC hop to JS UI | First-class via `windows-sys` | Good (`System.IO.Pipes.NamedPipeServerStreamAcl`) | Good (`QLocalServer`, though custom Win32 DACL needs native code) | Requires custom C++ platform channel or FFI | Extra serialization hop: NMH -> Rust -> WebView2 IPC |
| **Segmented Networking & Thread Cancellation** | **First-class** (`WinHTTP` async + IOCP + zero-copy buffers) | First-class in C++ backend | Very good (`tokio` + `reqwest` or `WinHTTP`), higher async/FFI complexity | Very good (`SocketsHttpHandler` + `CancellationToken`) | Good (`QNetworkAccessManager`), less granular socket control | Pure Dart `http` lacks low-level socket/IOCP control | Very good in Rust (`tokio` + `reqwest`) |
| **Direct Access to Windows APIs (Job Objects, DPAPI, Shell, Taskbar)** | **Zero-overhead direct C/C++ headers** (`windows.h`, `shobjidl.h`, `dpapi.h`) | Direct in backend | Requires `unsafe` FFI blocks via `windows-rs` for every Win32/COM call | Requires P/Invoke (`CsWin32`) for Job Objects, Shell, and Win32 ACLs | Direct C++ access alongside Qt | Requires C++ method channels or `dart:ffi` (`win32` package) | Requires `unsafe` `windows-rs` FFI in Rust backend |
| **UI Accessibility (MSAA / UIA) & Per-Monitor v2 High-DPI** | **Native OS support** via Common Controls v6 + `PerMonitorV2` manifest | High (Chromium accessibility tree) | **Poor to Moderate** (`egui`/`iced` have immature Windows screen-reader & IME support) | High (WinUI 3 / WPF native UIA) | High (`QAccessible`) | Moderate (custom semantics tree over Skia canvas) | High (Chromium accessibility tree) |
| **Internationalization (i18n) & RTL Support** | Native Win32 string tables / JSON catalog + `WS_EX_LAYOUTRTL` | Standard Web i18n (`i18next` + CSS `dir="rtl"`) | Manual library setup (`fluent-rs`) | `.resx` resources | `Qt Linguist` (`.ts`/`.qm`) | `flutter_localizations` (`arb`) | Standard Web i18n |
| **Testing & CI Compatibility** | GoogleTest + headless CMake/MSVC build on GitHub Actions `windows-latest` | Requires WebView2 headless driver for UI tests | `cargo test` on `windows-latest` | `dotnet test` | `QTest` + Qt installation in CI | `flutter test` | `cargo test` + WebDriver |
| **Developer Productivity & Debugging** | Moderate UI boilerplate; **excellent** native debugging in Visual Studio / WinDbg | Fast HTML/CSS UI prototyping; split C++/JS debugging | Steep borrow-checker friction with Win32 HWND/COM callback lifetimes | High productivity in Visual Studio | High productivity with Qt Creator / Designer | High UI velocity, but debugging native desktop plugin crashes is harder | High UI velocity; two toolchains (Rust + Node/TS) |
| **Dependency & Long-Term Maintenance Burden** | **Minimal** (OS ABI is stable for decades; only 3 small C++ libraries) | Low, tied to Evergreen WebView2 updates | Moderate (`cargo` crate ecosystem churn) | Moderate (.NET major version lifecycle every 1–2 years) | High (Qt LGPL dynamic linking rules + large SDK upgrades) | High (Flutter desktop breaking changes & plugin churn) | High (Rust crates + npm ecosystem + WebView2) |
| **Consequences for Reusing Upstream Code** | Adapts Brisk's HTTP algorithms in C++; wraps `N_m3u8DL-RE` as worker; shares TS types conceptually with extension | Same as Win32, plus could share CSS/HTML with extension popup | Could reuse concepts from `BrisklyDev/kasma`, but raw Win32 UI in Rust is ergonomic friction | Could reference `N_m3u8DL-RE` C# code directly, **but** upstream `N_m3u8DL-RE` is designed as a CLI app (using `Spectre.Console` globals), not a clean reusable class library | No upstream code uses Qt | Could reuse Brisk's Dart engine directly, **but** inherits Flutter's 45–85 MB RAM footprint and Dart HTTP limitations | No upstream code uses Tauri |

---

## 3. Distinguishing Core App Size vs. Bundled Media Tools

As required by Section 4 of the Master Prompt, our engineering analysis strictly distinguishes between the **Core Desktop Application** and the **Bundled Stream Toolchain**:

| Component Category | Binaries Included | Uncompressed Disk Size (x64) | LZMA2 Compressed Installer Impact | Runtime Memory Impact |
| :--- | :--- | :--- | :--- | :--- |
| **1. Core OpenIDM Desktop & IPC** | `openidm.exe` (includes SQLite3, WinHTTP engine, Win32 UI) + `openidm-nmh.exe` | **~2.2 MB** | **~0.9 MB** | **8 – 15 MB** (always resident when running in tray) |
| **2. Stream Downloader Worker** | `N_m3u8DL-RE.exe` (.NET 8/9 NativeAOT single-file release) | **~14.5 MB** | **~5.8 MB** | **0 MB** when idle; **25 – 60 MB** only while actively downloading an HLS/DASH stream |
| **3. Media Container Muxer** | `ffmpeg.exe` (minimal GPL/LGPL build compiled with `--disable-doc --disable-ffplay --disable-ffprobe --disable-filters` except concat/muxer/bsf) or standard essentials build | **~28 MB** (minimal mux-only build) to **~75 MB** (full build) | **~11 MB** (minimal) to **~26 MB** (full) | **0 MB** when idle; **20 – 50 MB** for 1–5 seconds only during final container muxing |
| **4. Browser Extension Package** | `openidm-extension.crx` / `.xpi` (installed via browser store or developer mode) | **~180 KB** | **~65 KB** | Managed by browser (`~10–20 MB` when Service Worker is awake; `0 MB` when suspended) |

### Why Win32 Still Matters Even When `ffmpeg.exe` and `N_m3u8DL-RE.exe` Are Bundled
A superficial analysis might argue: *"Since `ffmpeg.exe` + `N_m3u8DL-RE.exe` take 42–90 MB on disk, why not use Flutter, Qt, or WebView2 for the main GUI?"*  
That argument confuses **dormant disk footprint** with **resident 24/7 memory and startup latency**:
1. `N_m3u8DL-RE.exe` and `ffmpeg.exe` are **short-lived worker subprocesses**. When the user is not actively downloading an HLS/DASH stream, they consume **0 bytes of RAM** and **0 CPU cycles**.
2. By contrast, `openidm.exe` runs **continuously in the Windows 11 system tray** from boot to shutdown so it can respond within `< 50 ms` whenever the browser intercepts a download or detects a video.
3. Choosing Native C++20 + Win32 keeps the always-on resident process at **8–15 MB RAM** and instantaneous (`< 20 ms`) dialog pop-up latency—matching the responsiveness that made classic IDM famous—whereas Flutter, WinUI 3, or WebView2 would permanently pin **50–150 MB of RAM** just to keep a tray icon alive.

---

## 4. Why `N_m3u8DL-RE` Is Integrated as a Worker Process Rather Than Linked In-Process

Even if C# were chosen for the desktop UI, `N_m3u8DL-RE` ([nilaoda/N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE)) is architected as a command-line application tightly coupled to `System.CommandLine` and `Spectre.Console` live terminal rendering, rather than a multi-instance, thread-safe NuGet library. Running `N_m3u8DL-RE.exe` as an isolated child process per stream task provides:
1. **Fault Isolation:** A malformed manifest or unhandled exception in `N_m3u8DL-RE` or `ffmpeg` cannot crash the main `openidm.exe` process or interrupt other concurrent downloads.
2. **Deterministic Cancellation:** Assigning each `N_m3u8DL-RE.exe` invocation to a dedicated Windows Job Object (`CreateJobObjectW`) guarantees that canceling a task immediately kills both `N_m3u8DL-RE.exe` and any child `ffmpeg.exe` processes it spawned.
3. **Independent Upgradability:** When upstream `N_m3u8DL-RE` releases a fix for a new HLS/DASH edge case, `N_m3u8DL-RE.exe` can be updated and validated against our CLI contract tests without recompiling `openidm.exe`.
