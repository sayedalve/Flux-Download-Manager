# ADR-0001: Selection of Native C++20 and Win32 API for the Desktop Application

- **Date:** 2026-10-16
- **Status:** Proposed (Accepted for Milestone 1 Implementation)
- **Deciders:** Principal Software Architect, Senior Windows Systems Engineer
- **Related Documents:** [../TECHNOLOGY_STACK.md](../TECHNOLOGY_STACK.md), [../PERFORMANCE_AND_SIZE_BUDGET.md](../PERFORMANCE_AND_SIZE_BUDGET.md), [0002-process-and-module-boundaries.md](0002-process-and-module-boundaries.md)

---

## Status

**Proposed** (Baseline Architecture Decision for Milestone 1).

## Context

OpenIDM targets Windows 11 `x64` (with future `ARM64` support) as a direct open-source competitor to Internet Download Manager (IDM). Unlike standard productivity apps that run only while the user interacts with a document, a download manager is a **resident background system utility**:
1. It launches at Windows login and stays minimized in the Windows 11 Notification Area (system tray) 24/7, waiting for browser download interception or media detection events via Native Messaging IPC.
2. When triggered by a browser click, its "Add Download" or "Select Video Stream" dialog must appear instantaneously (`< 50 ms` perceived latency).
3. During active multi-connection downloads at `1 Gbps+`, it must update per-segment byte progress bars at 15–30 Hz while writing hundreds of megabytes per second to disk with minimal CPU and memory overhead.
4. It requires deep integration with Windows system APIs: `WinHTTP` (for HTTP/1.1, HTTP/2, OS Schannel TLS, and WPAD proxy auto-detection), I/O Completion Ports (IOCP), Security Descriptors (`SECURITY_ATTRIBUTES`) on Named Pipes, Windows Job Objects (`CreateJobObjectW`), Windows Data Protection API (`DPAPI`), and `SetFileInformationByHandle` for atomic file replacement.

We evaluated seven candidate technology stacks (documented quantitatively in [../TECHNOLOGY_STACK.md](../TECHNOLOGY_STACK.md)):
1. **C++20 with the native Win32 API** (`ComCtl32` v6, `UXTheme`, `Direct2D`)
2. **C++20 with Win32 + WebView2 UI**
3. **Rust with Windows API bindings** (`windows-rs` + `egui` / `iced` / raw Win32)
4. **C# (.NET 8/9)** with WinUI 3 or WPF
5. **C++ with Qt 6** (`QtWidgets` / `QtQuick`)
6. **Flutter Desktop (Dart)** (the stack used by upstream [Brisk](https://github.com/BrisklyDev/brisk))
7. **Tauri v2** (Rust core + WebView2 UI)

## Decision

We select **ISO C++20 (compiled with MSVC 2022) and the Native Windows Win32 API** (`ComCtl32.dll` Common Controls v6, `UXTheme` Dark Mode, and `Direct2D`/`GDI+` for custom segment progress bar rendering) for `openidm.exe` and `openidm-nmh.exe`.

To prevent the maintainability pitfalls of legacy C-style Win32 code, we enforce:
- A modern C++20 RAII windowing layer (`src/ui/WindowBase.hpp`) using `CreateWindowExW` + `GWLP_USERDATA` `this`-pointer routing and Microsoft WIL (`wil::unique_hwnd`, `wil::unique_handle`).
- Strict **Model-View-Presenter (MVP)** separation: `src/ui/` contains zero networking, SQLite, or process-spawning logic and communicates with `src/core/DownloadController` exclusively via asynchronous command methods and `PostMessageW` UI notifications.
- Virtual ListView (`LVS_OWNERDATA`) for the main task table so rendering 10,000+ download history items requires `O(visible_rows)` memory and zero control-creation overhead.

## Alternatives Considered

1. **C++20 + Win32 + WebView2 UI (`Rejected`):**
   - *Pros:* Easy HTML/CSS layout for dialogs and dark mode.
   - *Cons:* Spawns a multi-process Chromium browser tree (`msedgewebview2.exe`) that consumes `65–140 MB` of resident RAM and adds `250–600 ms` cold initialization latency when popping up a download confirmation dialog. Even if the WebView2 control is destroyed when minimized to tray, recreating it on every browser download interception causes a noticeable 300+ ms visual flash.
2. **Rust + Windows API / GUI Crates (`Rejected`):**
   - *Pros:* Memory safety and modern cargo tooling.
   - *Cons:* Native Windows GUI crates in Rust (`egui`, `iced`, `slint`) either render non-native GPU canvases with weak Windows 11 screen-reader (UI Automation) and IME support, or require verbose `unsafe` FFI boilerplate via `windows-rs` to manage cyclic HWND parent/child lifetimes and COM interfaces (`ITaskbarList3`, `IFileOpenDialog`).
3. **C# with WinUI 3 or WPF (`Rejected`):**
   - *Pros:* Rich native-looking controls and high developer familiarity; shares language with `N_m3u8DL-RE`.
   - *Cons:* Self-contained deployment adds `65–110 MB` to installed size (`35–75 MB` idle tray RAM); framework-dependent deployment fails if the exact .NET Desktop Runtime is missing. Furthermore, upstream `N_m3u8DL-RE` is structured as a console executable coupled to `Spectre.Console` rather than a reusable multi-instance library, so choosing C# still requires running `N_m3u8DL-RE.exe` out-of-process.
4. **C++ with Qt 6 (`Rejected`):**
   - *Pros:* Mature cross-platform widget toolkit (`QTableView`, `QSystemTrayIcon`).
   - *Cons:* Adds `45–75 MB` of Qt DLLs (`Qt6Core`, `Qt6Gui`, `Qt6Widgets`, ICU), `28–50 MB` idle RAM, and LGPLv3 dynamic-linking packaging complexity for a Windows-only desktop target.
5. **Flutter Desktop / Dart (`Rejected`):**
   - *Pros:* Used by upstream [Brisk](https://github.com/BrisklyDev/brisk); fast cross-platform UI prototyping.
   - *Cons:* Ships `flutter_windows.dll` + `icudtl.dat` (~40 MB), consumes `45–85 MB` idle RAM, lacks native Win32 control accessibility defaults, and requires C++ platform channels or FFI for every Windows Job Object, Named Pipe DACL, and WinHTTP call.
6. **Tauri v2 (`Rejected`):**
   - *Pros:* Smaller disk binary than Electron.
   - *Cons:* Inherits the exact same runtime memory (`70–150 MB`) and cold-popup latency (`300–650 ms`) drawbacks as C++ + WebView2 because Tauri on Windows renders via WebView2.

## Rationale

Why direct Win32 C++20 outweighs the extra UI implementation effort for OpenIDM:
1. **Bounded UI Surface Area:** A download manager does not require hundreds of complex free-form screens. Its entire UI consists of **5 well-defined views**:
   - Main Window (Menu, Toolbar, Category TreeView, Virtual ListView `LVS_OWNERDATA`, Splitter, Bottom Details/Log Pane, Status Bar).
   - Add Download / Confirm Intercept Dialog.
   - Stream Track Selection Dialog (ListViews with checkboxes for Video, Audio, and Subtitle tracks parsed from `meta.json`).
   - Active Download Progress Dialog (Tabs for Status/Speed/Segment Map and Task Log).
   - Settings Dialog (standard Win32 PropertySheet / Tabbed Dialog).
2. **Unmatched Resident Footprint (`NFR-001`–`NFR-003`):** Core binary size `< 2.5 MB`, cold startup `< 80 ms`, and system-tray idle working set `8–15 MB`—a **5x to 10x reduction in resident memory** compared to Flutter, WinUI 3, or WebView2.
3. **Zero-Impedance Windows Systems Integration:** `WinHTTP`, IOCP, Named Pipe `SECURITY_ATTRIBUTES`, `CryptProtectData` (DPAPI), `CreateJobObjectW`, and `SetFileInformationByHandle` are native C/C++ Win32 APIs. Calling them directly in C++20 eliminates FFI marshaling bugs and external runtime failures.

## Consequences

- **Positive:**
  - Instantaneous UI responsiveness when intercepting browser downloads.
  - Zero external runtime dependencies (`vcruntime` linked statically or via inbox Windows 11 UCRT).
  - Decades-long ABI stability on Windows 10/11 `x64` and `ARM64`.
- **Negative / Trade-offs:**
  - Custom layout, Per-Monitor v2 DPI scaling (`WM_DPICHANGED`), and Windows 11 Dark Mode (`DwmSetWindowAttribute` + `UXTheme` owner-draw/custom-draw) must be implemented explicitly in `src/ui/`.

## Risks & Mitigations

- **Risk (`RISK-009`):** Raw Win32 UI code can become verbose or inconsistent across DPI scales.
- **Mitigation:** Centralize all DPI math, font creation (`SystemParametersInfoForDpi`), and RAII dialog templates inside `src/ui/DpiHelper.hpp` and `src/ui/WindowBase.hpp`, and forbid business logic inside UI classes.
