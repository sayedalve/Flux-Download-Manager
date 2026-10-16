# Internet Download Manager (IDM) Video Detection System Reverse Engineering Investigation

## 1. Executive Summary

This investigation reverse-engineers the end-to-end video detection and download interception pipeline of **Internet Download Manager (IDM)** on Windows 11. It determines how IDM observes media in web browsers, classifies network traffic and DOM elements, communicates across process and sandbox boundaries, renders the native "Download this video" overlay button, parses adaptive streaming manifests (HLS and MPEG-DASH), and creates download tasks in its core engine.

All investigations were performed inside `C:\Users\sayed\Downloads\IDM_RE_Investigation` without modifying any installed files in `C:\Program Files (x86)\Internet Download Manager`.

---

## 2. Directory Layout & Artifacts

```
C:\Users\sayed\Downloads\IDM_RE_Investigation\
│
├── README.md                                  # Investigation guide, methodology, and reproduction steps
│
├── binaries\                                  # Working copies of investigated binaries (unmodified)
│   ├── IDMan.exe                              # Main IDM desktop application and download coordinator
│   ├── IDMMsgHost.exe                         # Native Messaging host spawned by Chromium/Gecko browsers
│   ├── idmbrbtn.dll                           # Native Win32 floating download button / overlay panel DLL
│   ├── idmcchandler7.dll                      # Browser integration coordinator & MIME/URL sniffer
│   ├── IDMNetMon.dll                          # Network monitor & IPC named pipe / local socket server
│   ├── IDMVMPrs.dll                           # Video Manifest Parser (HLS m3u8 and DASH mpd parser)
│   └── idmBroker.exe                          # Low-integrity COM broker for IE/Edge sandboxes
│
├── extensions\                                # Extracted browser extension packages & analysis
│   ├── IDMGCExt\                              # Chrome/Edge Manifest V3 extension (version 6.43.1)
│   │   ├── manifest.json                      # Permissions, content scripts, background worker
│   │   ├── background.js                      # webRequest monitoring, MIME type dispatch, Native Messaging IPC
│   │   └── content.js                         # DOM observers (ResizeObserver, IntersectionObserver), viewport math
│   ├── IDMEdgeExt\                            # Microsoft Edge CRX package
│   ├── IDMGCExt59\                            # Legacy Chromium CRX package
│   ├── idmmzcc3\                              # Firefox XPI package
│   ├── extensions_analysis.json               # Structured manifest and permission analysis
│   └── extensions_analysis.md                 # Detailed report on extension scripts and protocol framing
│
├── ghidra\                                    # Ghidra reverse engineering projects and outputs
│   ├── scripts\
│   │   ├── IDMDeepAnalysisScript.java         # Headless Ghidra script: string scanning, xref tracing, decompilation
│   │   └── OptimizeAnalyzers.java             # Pre-script disabling whole-program Decompiler Parameter ID
│   ├── projects\                              # Ghidra project files for each target PE
│   ├── reports\                               # Per-binary Ghidra reports and execution logs
│   │   ├── IDMMsgHost.exe_ghidra_report.md
│   │   ├── IDMMsgHost.exe_headless.log
│   │   ├── idmbrbtn.dll_ghidra_report.md
│   │   ├── idmbrbtn.dll_headless.log
│   │   ├── idmcchandler7.dll_ghidra_report.md
│   │   ├── idmcchandler7.dll_headless.log
│   │   ├── IDMNetMon.dll_ghidra_report.md
│   │   ├── IDMNetMon.dll_headless.log
│   │   ├── IDMVMPrs.dll_ghidra_report.md
│   │   ├── IDMVMPrs.dll_headless.log
│   │   ├── idmBroker.exe_ghidra_report.md
│   │   ├── idmBroker.exe_headless.log
│   │   ├── IDMan.exe_ghidra_report.md
│   │   ├── IDMan.exe_headless.log
│   │   └── ghidra_runs_summary.json
│   └── decompiled\                            # Extracted C source code for decompiled functions
│       ├── IDMMsgHost_exe\
│       ├── idmbrbtn_dll\
│       ├── idmcchandler7_dll\
│       ├── IDMNetMon_dll\
│       └── IDMan_exe\
│
├── inventory\                                 # Full IDM installation inventory
│   ├── idm_files_inventory.json               # 189 installed files: sizes, hashes, PE architecture
│   ├── idm_files_inventory.csv                # Tabular CSV format
│   ├── idm_files_inventory.md                 # Markdown summary breakdown
│   └── ipc_and_strings_quickscan.md           # PE imports and string quick scan
│
├── registry\                                  # Native Messaging registry inspection
│   ├── IDMMsgHost.json                        # Chrome/Edge Native Messaging manifest
│   ├── IDMMsgHostMoz.json                     # Firefox Native Messaging manifest
│   ├── native_messaging_registrations.json    # Registry keys and manifest mappings
│   └── native_messaging_report.md             # Native Messaging analysis report
│
├── reports\                                   # Final synthesis reports
│   └── IDM_Video_Detection_Architecture_Report.md # Comprehensive evidence-based architecture report
│
└── scripts\                                   # Standalone PowerShell automation scripts (AST validated)
    ├── test_ast.ps1                           # Windows PowerShell AST syntax validator
    ├── inventory_idm.ps1                      # Recursive installer scanner & PE header parser
    ├── inspect_registry_nm.ps1                # Native Messaging registry & manifest inspector
    ├── extract_extensions.ps1                 # CRX3/CRX2/XPI package unpacker
    ├── inspect_pe_imports.ps1                 # Quick scanner for IPC imports and target strings
    └── launch_ghidra_analysis.ps1             # Ghidra headless runner with Adoptium JDK 25
```

---

## 3. Toolchain & Environment

- **Operating System**: Windows 11 64-bit
- **Java Development Kit**: Eclipse Adoptium OpenJDK 25.0.4.101-hotspot (`C:\Program Files\Eclipse Adoptium\jdk-25.0.4.101-hotspot`)
- **Ghidra**: Ghidra 12.1.4 PUBLIC (`C:\Users\sayed\Downloads\ghidra\ghidra_12.1.4_PUBLIC`)
  - Execution mode: Headless analyzer (`support\analyzeHeadless.bat`)
  - Pre-script: `OptimizeAnalyzers.java` (disables whole-program `Decompiler Parameter ID` to prevent multi-minute auto-analysis hangs)
  - Post-script: `IDMDeepAnalysisScript.java` (searches for target patterns, traces direct and indirect references, decompiles matching functions, exports C source files, logs callers/callees)
- **Script Validation**: All PowerShell scripts are verified before execution with `[System.Management.Automation.Language.Parser]::ParseInput` via `scripts/test_ast.ps1`.

---

## 4. How to Reproduce the Analysis

All commands are run from PowerShell in `C:\Users\sayed\Downloads\IDM_RE_Investigation`.

### 4.1 Validate PowerShell Syntax
```powershell
& '.\scripts\test_ast.ps1' -ScriptPath '.\scripts\launch_ghidra_analysis.ps1'
```

### 4.2 Re-run IDM Installation Inventory
```powershell
& '.\scripts\inventory_idm.ps1'
```

### 4.3 Re-inspect Native Messaging Registrations
```powershell
& '.\scripts\inspect_registry_nm.ps1'
```

### 4.4 Unpack and Inspect Browser Extensions
```powershell
& '.\scripts\extract_extensions.ps1'
```

### 4.5 Run Ghidra Headless Analysis on Target Binaries
```powershell
& '.\scripts\launch_ghidra_analysis.ps1' -Targets @("IDMMsgHost.exe", "idmbrbtn.dll", "idmcchandler7.dll", "IDMNetMon.dll", "IDMVMPrs.dll", "IDMan.exe")
```

---

## 5. Summary of Architecture Findings

1. **Dual Detection Mechanism**:
   - **DOM Observation**: `content.js` monitors `<video>`, `<audio>`, `<embed>`, and `<object>` elements via `IntersectionObserver`, `MutationObserver`, and `ResizeObserver`. It computes viewport coordinates, zoom/device pixel ratio, and bounding rectangles.
   - **Network Sniffing**: `background.js` listens on `chrome.webRequest.onHeadersReceived` for media MIME types (`video/mp4`, `video/webm`, `video/MP2T`, `application/dash+xml`, `application/vnd.apple.mpegurl`, etc.) and matching Content-Disposition / URL extensions.

2. **IPC & Native Messaging**:
   - The browser launches `IDMMsgHost.exe` via Chrome/Edge/Firefox Native Messaging (`com.tonec.idm`).
   - `IDMMsgHost.exe` checks for mutex `Local\IDMEventMonitor`. If IDM is not running, it queries the registry and launches `IDMan.exe`.
   - `IDMMsgHost.exe` connects to IDM's named pipe: `\\.\pipe\IDMNetworkMonitor.<SessionId>`.
   - Communication uses framed text messages: `MSG#<seq>#<type>#<subType>#<flags>:<params>;`.

3. **Native Overlay Button**:
   - The "Download this video" panel is **not** HTML injected into the webpage DOM.
   - It is a native Win32 window (`HWND`) belonging to window class `IDM_Download_Panel_controller2_class`, created by `idmcchandler7.dll` and rendered via `idmbrbtn.dll`.
   - `idmbrbtn.dll` calls `SetWindowPos` directly over the browser's video viewport using the screen coordinates relayed from `content.js`.

4. **Adaptive Stream Interception (HLS / DASH)**:
   - When `.m3u8` or `.mpd` streams are intercepted, `idmcchandler7.dll` dynamically loads `IDMVMPrs.dll` to parse the manifest, extract bitrate/resolution tracks, and generate synthetic internal scheme URLs (`hlstoidm://`, `mpdtoidm:///`, `mpdatoidm:///`).
   - When the user selects a resolution from the native button menu, the synthetic URL and segment metadata are passed into `IDMan.exe` to download segments concurrently and mux them into a single media file.
