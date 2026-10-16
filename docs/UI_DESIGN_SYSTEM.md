# OpenIDM Desktop UI Design System (`src/ui/design_system.hpp` & `src/ui/win32_main_window.cpp`)

## 1. Visual Direction & Design Philosophy

OpenIDM's desktop interface is engineered as a native, low-overhead Windows 11 desktop application (`WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN`) rendered via double-buffered Win32 GDI/Direct2D primitives (`src/ui/design_system.hpp`, `src/ui/win32_main_window.cpp`) and backed by a platform-independent view-model (`openidm::ui::DesktopView`).

Key design principles:
- **Restrained Windows 11 Aesthetic**: Deep charcoal surfaces (`#18181B` / `#1F1F23`), crisp typography (`Segoe UI Variable` / `Segoe UI`), 1px subtle dividers (`#3F3F46`), and a single restrained blue accent (`#3B82F6`) paired with a distinct stream indicator (`#8B5CF6`).
- **Zero Decorative Bloat**: No glass/blur shaders, no animated background gradients, no emoji icons, and no heavy Chromium/Electron/Flutter runtime overhead.
- **Strict Resource Discipline**: Double-buffered `WM_PAINT` updates only on state change or a 100 ms UI timer (`IDT_UI_REFRESH`) without querying SQLite on the paint thread.

---

## 2. Color Tokens (`openidm::ui::ThemePalette`)

OpenIDM provides three theme modes (`ThemeMode::kDark`, `ThemeMode::kLight`, and `ThemeMode::kSystem`), persisted in SQLite (`settings.theme`) and synchronized with Windows 11 `DWMWA_USE_IMMERSIVE_DARK_MODE`.

| Token Name | Dark Theme (Default) | Light Theme | Purpose |
|---|---|---|---|
| `app_background` | `#18181B` | `#F8FAFC` | Main window canvas and task table background |
| `surface_primary` | `#1F1F23` | `#FFFFFF` | Top toolbar, navigation sidebar, and bottom details panel |
| `surface_elevated` | `#27272A` | `#F1F5F9` | Secondary toolbar buttons, status bar, and modal dialog cards |
| `surface_hover` | `#2E2E33` | `#E2E8F0` | Interactive row and button hover background |
| `surface_selected` | `#1E293B` | `#DBEAFE` | Selected download row and active sidebar category |
| `border_subtle` | `#3F3F46` | `#CBD5E1` | 1px structural dividers between panes |
| `border_focus` | `#60A5FA` | `#2563EB` | 2px keyboard focus ring (`WM_KEYDOWN` navigation) |
| `text_primary` | `#F4F4F5` | `#0F172A` | Primary filenames, dialog titles, and active labels |
| `text_secondary` | `#A1A1AA` | `#475569` | Column headers, transfer sizes, timestamps, and details metadata |
| `text_muted` | `#71717A` | `#94A3B8` | Disabled toolbar buttons and empty-state hints |
| `accent_primary` | `#3B82F6` | `#2563EB` | Primary `+ Add URL` button and HTTP file progress bar fill |
| `accent_hover` | `#2563EB` | `#1D4ED8` | Primary action hover state |
| `badge_stream` | `#8B5CF6` | `#7C3AED` | `HLS STREAM` / `DASH STREAM` badge and stream progress bar fill |
| `badge_file` | `#3B82F6` | `#2563EB` | `HTTP MULTI` / `HTTP FILE` badge |
| `status_success` | `#22C55E` | `#16A34A` | `Completed` state indicator |
| `status_warning` | `#F59E0B` | `#D97706` | `Paused` / `Retrying` state indicator |
| `status_danger` | `#EF4444` | `#DC2626` | `Failed` / `Error` state indicator |
| `progress_track` | `#323238` | `#E2E8F0` | Unfilled progress bar track |

---

## 3. Spacing, Layout Regions & High-DPI Scaling (`openidm::ui::LayoutMetrics`)

All layout dimensions use a **4px base grid** and scale proportionally across Per-Monitor V2 DPI settings (`96` DPI / 100%, `120` DPI / 125%, `144` DPI / 150%, `192` DPI / 200%, `240` DPI / 250%) via `DesignSystem::ScaleForDpi(base_px, dpi)`.

| Metric | 100% (`96 DPI`) | 125% (`120 DPI`) | 150% (`144 DPI`) | 200% (`192 DPI`) | Notes |
|---|---|---|---|---|---|
| `min_window_width` | `900 px` | `1125 px` | `1350 px` | `1800 px` | Enforced via `WM_GETMINMAXINFO` |
| `min_window_height` | `560 px` | `700 px` | `840 px` | `1120 px` | Enforced via `WM_GETMINMAXINFO` |
| `compact_width_threshold` | `1024 px` | `1280 px` | `1536 px` | `2048 px` | Below this width, `Type` and `ETA` columns auto-hide |
| `toolbar_height` | `48 px` | `60 px` | `72 px` | `96 px` | Top action bar |
| `sidebar_width` | `216 px` | `270 px` | `324 px` | `432 px` | Left navigation category pane |
| `table_header_height` | `34 px` | `43 px` | `51 px` | `68 px` | Sortable column header bar |
| `table_row_height` | `44 px` | `55 px` | `66 px` | `88 px` | Virtualized task row + 6px progress bar |
| `details_panel_height` | `188 px` | `235 px` | `282 px` | `376 px` | Bottom inspector panel |
| `status_bar_height` | `28 px` | `35 px` | `42 px` | `56 px` | Bottom application status bar |

### Typography Ramp
- **Font Family**: `Segoe UI Variable Display` / `Segoe UI` (Windows 11 native system font).
- **Caption / Badge Floor**: `11 px` (`font_caption_px`) — never smaller than 11px at 100% DPI.
- **Body / Table Cell Floor**: `13 px` (`font_body_px` Regular, `font_body_strong_px` Semibold).
- **Section Subtitle**: `15 px` (`font_subtitle_px` Semibold).
- **Dialog Title**: `18 px` (`font_title_px` Semibold).

---

## 4. Main Window Architecture & Control States

### A. Context-Sensitive Main Toolbar (`ToolbarState`)
The toolbar exposes 10 actions (`+ Add URL`, `Start`, `Pause`, `Resume`, `Retry`, `Cancel`, `Remove`, `Clear History`, `Open Folder`, `Settings`).
- Every control's enabled/disabled state is dynamically computed by `DesktopView::ComputeToolbarState()` based on the currently selected task's state (`kQueued`, `kProbing`, `kDownloading`, `kMuxing`, `kPaused`, `kCompleted`, `kFailed`, `kCanceled`).
- No decorative placeholder buttons exist.

### B. Navigation Sidebar (`SidebarCategory` & `CategoryCounts`)
Displays 7 categories with live badge counts computed in a single pass over `DownloadController::ListTasks()`:
1. `All Downloads`
2. `In Progress` (`PROBING`, `DOWNLOADING`, `MUXING`, `VERIFYING`)
3. `Queued` (`QUEUED`, non-scheduled `PAUSED`)
4. `Completed` (`COMPLETED`)
5. `Failed` (`FAILED`, `CANCELED`)
6. `Video Downloads` (`STREAM_HLS`, `STREAM_DASH`, `STREAM_MSS`, or `category == "Video"`)
7. `Scheduled` (`scheduled == true`)

Selecting a category filters the single unified queue view without creating separate queues.

### C. Virtualized Download List (`BuildVisibleTaskRows`)
- **Full Mode (`>= 1024 px` scaled)**: Displays 8 columns (`Filename`, `Type`, `Status`, `Progress`, `Size`, `Speed`, `ETA`, `Actions`).
- **Compact Mode (`< 1024 px` scaled)**: Automatically hides `Type` and `ETA` (`6` columns) to prevent horizontal clipping.
- **Graceful Filename Truncation**: `TruncateFilenameGracefully()` uses middle ellipsis (`very_long_lect..._1080p.mp4`) so the file extension is always visible while preserving `full_filename_tooltip` for tooltips and the Details Panel.
- **Honest Progress Formatting**:
  - When `total_bytes > 0`: displays exact percentage (`64.2%`) and determinate bar fill.
  - When `is_stream && total_segments > 0`: displays segment completion percentage (`50.0% (5/10 segs)`).
  - When `total_bytes == 0` on an unchunked HTTP transfer: displays `1.2 MiB (Size Unknown)` and sets `progress_indeterminate = true` — **never** fabricates a percentage.
- **Granular Stream Stages**: Distinguishes `Probing stream manifest...`, `Downloading video segments (X/Y)`, `Downloading audio segments (X/Y)`, `Downloading subtitle tracks (X/Y)`, `Merging audio/video tracks (FFmpeg)...`, `Verifying output container...`, `Completed`, and `Failed`.

### D. Redacted Details Panel (`TaskDetailsViewModel`)
Displays the selected task's Source URL, Page URL, Destination Path, Size/Progress/Speed, UTC Timestamps (`Created`, `Updated`, `Completed`), Retry Count, Engine (`Brisk Segmented HTTP/1.1` vs `N_m3u8DL-RE + FFmpeg`), Stream Format, Selected Tracks, and actionable Error Explanation.
- **Strict Credential Redaction**: Passes URLs and headers through `Logger::RedactSensitiveData()` and replaces `auth_headers` with `Auth/Cookies: [N protected credential header(s) — REDACTED]`.

### E. Supporting Dialogs & System Tray
1. **Add Download Dialog** (`SubmitAddDownloadDialog`): Validates `http://` / `https://` URLs, destination folder, connection count (`1..32`), and stream format (`mp4`/`mkv`) without exposing raw CLI flags.
2. **Stream Quality & Track Selector** (`ProbeStreamForDialog` & `ConfirmStreamSelectionDialog`): Probes real tracks via `N_m3u8DL-RE --skip-download --write-meta-json` and binds user track choices (`video_filter`, `audio_filter`, `subtitle_filter`, `output_format`).
3. **Settings Dialog** (`ActionSaveSettings` & `ActionRestoreDefaultSettings`): Configures and persists Theme (`dark`/`light`/`system`), Max Concurrent Downloads (`1..16`), Global Speed Limit, Default Download Directory, Browser Capture toggle, and Close-to-Tray behavior.
4. **Browser Integration Status** (`GetBrowserIntegrationStatus`): Displays live IPC socket/pipe endpoint, capture toggle state, and `N_m3u8DL-RE` / `FFmpeg` worker availability.
5. **About Dialog** (`GetAboutDialogView`): Displays version `0.2.0`, `GPL-3.0-or-later` license, and upstream attributions.
6. **System Tray Icon** (`Shell_NotifyIconW`): Supports `Show OpenIDM`, `Hide to Tray`, `Pause All Downloads`, `Resume All Downloads`, and `Exit OpenIDM`, honoring `close_to_tray` on `WM_CLOSE`.

---

## 5. Accessibility & Keyboard Navigation

- **Keyboard Shortcuts (`DesktopView::HandleKeyShortcut` & `WM_KEYDOWN`)**:
  - `Up` / `Down` / `Home` / `End`: Navigate rows in the download table.
  - `Space`: Toggle Pause / Resume on the selected download.
  - `Delete`: Remove selected task from the list (preserving completed files on disk).
  - `Shift+Delete`: Delete selected task and remove its output file from disk.
  - `F5`: Retry a failed or canceled download.
  - `Enter`: Open a completed file.
- **No Emoji Policy**: All icons and badges use geometric vector primitives or accessible text labels (`FILE`, `HLS STREAM`, `DASH STREAM`) compatible with Windows Narrator.
