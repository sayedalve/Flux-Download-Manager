# Windows OS Compatibility & Cloud Testing Architecture for Radium Download Manager

**Document Status:** Approved Architecture & Cloud Testing Specification  
**Last Updated:** 2026-10-21  
**Scope:** Automated Windows Server & Desktop Compatibility Matrix, GitHub Actions Runner Infrastructure, Differences Audit, and Cloud VM Provisioning.

---

## 1. Executive Summary & Zero-Hallucination Policy

Radium Download Manager targets Windows 11 and Windows 10 (`x64`, with future `ARM64` support).  
In accordance with project governance rules (**AGENTS.md** § 9.3):
- **Never claim that Radium supports a Windows version merely because it builds successfully on Windows Server.**
- Compilation on a Windows Server runner verifies toolchain compatibility (MSVC C++20, Windows SDK, WinHTTP headers, CMake scripts).
- Automated execution of headless integration tests on Windows Server verifies segmented HTTP networking, SQLite ACID locking, process execution, and Native Messaging IPC.
- Features that depend on the interactive Windows Desktop Window Manager (DWM), Explorer Shell, Taskbar APIs (`ITaskbarList3`), Per-Monitor v2 DPI scaling, or Windows 11 Mica composition **cannot be verified on Windows Server runners**. These features are strictly marked as **Unverified (Requires Windows 10/11 Desktop Host)** until executed on a genuine desktop host.

---

## 2. Windows Environment Compatibility Matrix

| Operating System | Microsoft Support Status | GitHub Actions Runner | Build Verification | Core Headless Engine | Inno Setup Silent Install | Browser Extension Playwright | Interactive Desktop UI / Mica | System Tray & Taskbar Progress | Overall Status |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Windows Server 2022 Datacenter** | Supported Server | windows-2022 | ✅ **Verified** | ✅ **Verified** | ✅ **Verified** | ✅ **Verified** | ❌ Not Verified (Server OS) | ❌ Not Verified (Session 0) | **Core Verified (Server)** |
| **Windows Server 2025 Datacenter** | Supported Server | windows-2025 | ✅ **Verified** | ✅ **Verified** | ✅ **Verified** | ✅ **Verified** | ❌ Not Verified (Server OS) | ❌ Not Verified (Session 0) | **Core Verified (Server)** |
| **Windows 10 22H2 (Build 19045) x64** | Supported (End of Support Oct 14, 2025 / ESU) | Cloud VM / Self-Hosted | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⛔ Unsupported (No Mica on Win10) | ⚠️ Unverified | **Pending Desktop Runner** |
| **Windows 11 23H2 (Build 22631) x64** | Supported GA Desktop | Cloud VM / Self-Hosted | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | **Pending Desktop Runner** |
| **Windows 11 24H2 / 25H2 (Build 26100+) x64** | Supported GA Desktop (Germanium Base) | Cloud VM / Self-Hosted | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | **Pending Desktop Runner** |
| **Windows 11 26H1 (ARM64 / Germanium)** | Distinct hardware scope (Snapdragon X / OEM silicon baseline) | Specialized ARM64 VM / Runner | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | **Distinct Hardware Scope (ARM64)** |
| **Windows 11 26H2 (Build 26xxx+) x64** | Future GA Desktop Servicing Branch | Future Desktop VM | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | ⚠️ Unverified | **Future Desktop Release** |

---

## 3. Detailed Architectural Differences: Windows Server CI vs. Windows Desktop

GitHub-hosted runners (`windows-2022` and `windows-2025`) run Windows Server Datacenter editions in headless hypervisor environments. Real users run Windows 10 or Windows 11 client editions with an active desktop session.

### 3.1 Desktop Window Manager (DWM) and Visual Backdrops
- **Windows Server:** DWM composition is restricted. APIs such as `DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, ...)` return `E_NOTIMPL` or fail silently because Mica, Acrylic, and custom caption text styling are disabled by default on Windows Server.
- **Windows 11 Desktop:** `DWMWA_SYSTEMBACKDROP_TYPE = 2` renders dynamic Mica material matching the user's desktop wallpaper and light/dark theme.
- **Windows 10 Desktop:** Mica does not exist in Windows 10 (Build 19045). Radium falls back to custom dark solid background (`0x1E1E1E`).

### 3.2 Windows Shell & Taskbar Progress (`ITaskbarList3`)
- **Windows Server CI:** GitHub Actions runner agent executes as a background service or console job in Session 0 / non-interactive session without a running `explorer.exe` shell. COM calls to `CoCreateInstance(CLSID_TaskbarList)` may succeed, but `pTaskbar->SetProgressValue()` and `SetProgressState()` have no visual effect.
- **Windows Desktop:** `ITaskbarList3` provides green, yellow (paused), and red (error) progress states directly over the Radium taskbar icon.

### 3.3 High-DPI Scaling & Display Configurations
- **Windows Server CI:** Virtual display adapter is fixed to 100% scaling (96 DPI) with no physical monitor attached.
- **Windows Desktop:** Real users frequently use 125%, 150%, 175%, and 200% Per-Monitor v2 DPI scaling, or multi-monitor setups with mixed DPI. Windows messages like `WM_DPICHANGED` only fire on genuine display hardware or configured virtual display drivers.

### 3.4 Privilege Levels & User Account Control (UAC)
- **Windows Server CI:** Runner runs with elevated administrator privileges (`RUNNER_USER`). File writes to `%PROGRAMFILES%` or `HKLM` do not prompt for UAC.
- **Windows Desktop:** Radium installers default to `PrivilegesRequired=lowest` and install into `%LOCALAPPDATA%\Programs\Radium` or `{autopf}` without UAC elevation. Running tests on standard desktop user accounts is essential to catch accidental root or administrator path dependencies.

---

## 4. Options for Automated Desktop Testing via Cloud Virtual Machines

GitHub does **not** provide hosted runners for Windows 10 or Windows 11 client editions. Standard hosted runners (`runs-on: windows-latest`, `windows-2025`, `windows-2022`) are strictly Windows Server.

To execute tests on authentic Windows 10 22H2 and Windows 11 23H2/24H2 desktop operating systems without using your local PC, you must connect external cloud virtual machines as GitHub Actions self-hosted runners.

### Option 1: Microsoft Azure Virtual Machines (Recommended for Windows Client)

Microsoft Azure is the only cloud provider with direct multi-tenant licensing for Windows 10 and Windows 11 client desktop editions.

#### A. Required Virtual Machine Images
- **Windows 11 Desktop:** `MicrosoftWindowsDesktop:windows-11:win11-23h2-pro:latest` or `win11-24h2-pro`
- **Windows 10 Desktop:** `MicrosoftWindowsDesktop:windows-10:win10-22h2-pro:latest`
- **Recommended VM Size:** `Standard_D4s_v5` (4 vCPUs, 16 GiB RAM, Premium SSD).

#### B. Licensing Requirements
- Azure requires Windows Client Multitenant Hosting Rights (available via Windows 10/11 Enterprise E3/E5 per user, Microsoft 365 E3/E5, or Visual Studio Enterprise subscriptions).
- Running Windows 10/11 client on Azure without an authorized desktop license is restricted by Microsoft product terms.

#### C. Approximate Paid Service Costs
- **Compute:** ~$0.096 to $0.14 per hour per VM (Pay-As-You-Go).
- **Storage:** ~$5 per month for 128 GB Managed SSD disk.
- **Ephemeral Auto-Shutdown:** By configuring Azure Automation or an auto-scaling ephemeral runner pool, the VM only runs during CI execution (~$10–$25/month depending on commit volume).

#### D. Runner Configuration with Interactive Desktop (Session 1)
To verify DWM and UI components, the runner **must not** run as a Windows Service (which runs in Session 0). It must run in Session 1:
1. Configure Windows `Autologon` for a dedicated test user.
2. Place the GitHub Actions runner `run.cmd` script in the user's `Startup` folder (`shell:startup`) or a scheduled task configured for "Run only when user is logged on".
3. Use a virtual display driver (such as the IddCx Indirect Display Driver or V-Display) to simulate 1080p, 1440p, and 4K displays at 150% and 200% DPI.

---

### Option 2: AWS EC2 Dedicated Hosts / Bare Metal

AWS does not offer multi-tenant Windows 10 or Windows 11 AMIs due to Microsoft licensing terms.
- **Licensing:** Requires an AWS EC2 Dedicated Host (`mac` or `x86`) where you Bring Your Own License (BYOL).
- **Cost:** High fixed cost ($200–$500+/month) due to dedicated host reservations. Not cost-effective for individual open-source repositories unless an enterprise sponsorship is active.

---

### Option 3: Dedicated Cloud Bare-Metal / MacStadium / Hetzner

Providers offering bare-metal physical servers where custom Windows 10/11 ISOs can be installed.
- **Setup:** Install Windows 11 Pro with genuine retail/OEM digital license.
- **Cost:** ~$40–$70/month for a dedicated bare-metal server.
- **Runner Setup:** Register the GitHub Actions Runner using repository registration tokens.

---

## 5. Required Secrets and Setup for External Cloud Runners

When configuring a cloud desktop VM runner, add the following GitHub Actions secrets to the repository:

| Secret Name | Purpose | Example Value / Description |
| :--- | :--- | :--- |
| `AZURE_CREDENTIALS` | Service Principal JSON for Azure CLI VM lifecycle | `{"clientId": "...", "clientSecret": "...", "tenantId": "...", "subscriptionId": "..."}` |
| `AZURE_RESOURCE_GROUP` | Azure resource group containing test runners | `rg-radium-ci-runners` |
| `AZURE_VM_NAME_WIN11` | Name of the Windows 11 Desktop VM | `vm-radium-win11-desktop` |
| `RUNNER_PAT_TOKEN` | GitHub Personal Access Token (PAT) with `repo` scope | For dynamically registering ephemeral self-hosted runners |

---

## 6. Template Workflow for External Cloud Desktop Runner

A template workflow has been provided at `.github/workflows/desktop-cloud-runner.yml.template`.  
It uses `runs-on: [self-hosted, windows-desktop-11]` and triggers automatically whenever a runner with those tags is connected to the repository.

---

## 7. Status of Unverified Features

The following features remain explicitly marked as **Unverified (Requires Windows 10/11 Desktop Host)** until a desktop runner is attached:

1. **Windows 11 Native Mica Backdrop:** Rendering verification of DWM material backdrops.
2. **DWM Dark Mode Caption Color:** Validation that titlebars render `0x1E1E1E` caption bars via `DwmSetWindowAttribute`.
3. **Taskbar Progress & State Badges:** Live verification that `ITaskbarList3` sets determinate progress bars in the Windows Explorer taskbar.
4. **Dynamic Per-Monitor v2 DPI Scaling:** Layout recalculation when moving windows between 100% and 150%/200% monitors.
5. **System Tray Lifecycle:** Minimizing to notification area, tooltips, and double-click restore under Windows Explorer.
