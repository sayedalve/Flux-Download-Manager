# Radium Download Manager — Product Identity & Branding Specification

**Document Status:** Approved Product Branding Baseline  
**Date:** 2026-10-20  
**Scope:** Defines the complete visual, typographic, naming, and architectural identity for **Radium Download Manager** across the desktop application, browser extension, in-page video overlay, installer, and documentation.

---

## 1. Product Naming Hierarchy

| Attribute | Specification | Permitted Usage | Prohibited Usage |
| :--- | :--- | :--- | :--- |
| **Official Full Name** | **Radium Download Manager** | Main window title bar, installer welcome & finish pages, Add/Remove Programs display name, documentation titles, browser extension manifest name. | Never use "Flux" or "OpenIDM" as the current product name. |
| **Short Brand Name** | **Radium** | System tray icon tooltip, extension badge, in-page download button ("Download in Radium"), toolbar buttons, dialog prompts, CLI banner. | Never use alone in legal notices or copyright declarations. |
| **Abbreviation** | **RDM** | Compact UI labels, compact logos, release tags where short codes are required. | Do not use ambiguous abbreviations. |
| **Executable Names** | `radium.exe`<br>`radium-nmh.exe` | Binary filenames on disk, Task Manager process list, start menu shortcuts, registry launch paths. (Compatibility aliases: `flux.exe`, `flux-nmh.exe`). | Do not rely on deprecated executable names in new distributions. |
| **Native Host Identifier** | `com.flux.downloadmanager`<br>`org.openidm.native` | Browser native messaging registrations in Windows Registry and host JSON manifests (preserved for backward compatibility). | Do not break existing browser extension registrations. |
| **Application Data Path** | `%LOCALAPPDATA%\Radium Download Manager\` | Default directory for `state.db`, `radium.log`, `settings.json`, and temporary files. Automatically migrates legacy state from `%LOCALAPPDATA%\Flux` or `%LOCALAPPDATA%\OpenIDM`. | Never scatter files across user documents or system directories. |
| **Default Download Path** | `%USERPROFILE%\Downloads\Radium\` | Default destination folder for completed user downloads. (Preserves legacy `%USERPROFILE%\Downloads\Flux` or `%USERPROFILE%\Downloads\OpenIDM` if present). | Do not download directly into root C:\ or temp folders. |

---

## 2. Color Palette & Visual Identity

Radium employs a sophisticated, modern Windows 11 palette characterized by a **deep charcoal dark theme**, a **restrained electric blue accent**, and a crisp, high-contrast light theme.

```
       DEEP CHARCOAL DARK THEME                   ELECTRIC BLUE ACCENT
┌──────────────────────────────────────┐       ┌────────────────────────┐
│ App Background:    #0F172A (Slate 900)│       │ Primary Accent: #2563EB│
│ Surface Primary:   #1E293B (Slate 800)│       │ Accent Hover:   #3B82F6│
│ Surface Elevated:  #334155 (Slate 700)│       │ Accent Active:  #1D4ED8│
│ Surface Selected:  #1E3A8A (Blue 900) │       │ Progress Fill:  #3B82F6│
└──────────────────────────────────────┘       └────────────────────────┘
```

### 2.1 Design Tokens Table

| Token Name | Dark Theme (Default) | Light Theme | Usage |
| :--- | :--- | :--- | :--- |
| `app_background` | `#0F172A` (Deep Slate) | `#F8FAFC` (Cool Off-White) | Desktop client window background |
| `surface_primary` | `#1E293B` (Dark Navy Surface) | `#FFFFFF` (Pure White) | Toolbar, sidebar, table header, details panel |
| `surface_elevated` | `#334155` (Elevated Card) | `#F1F5F9` (Subtle Gray) | Button backgrounds, badges, cards |
| `surface_hover` | `#475569` (Interactive Hover) | `#E2E8F0` (Hover Gray) | Hovered list rows and toolbar buttons |
| `surface_selected` | `#1E3A8A` (Deep Blue Select) | `#DBEAFE` (Light Blue Select) | Selected task row in queue table |
| `accent_primary` | `#2563EB` (Restrained Blue) | `#2563EB` (Restrained Blue) | Primary action buttons, selection indicators |
| `accent_hover` | `#3B82F6` (Vibrant Blue Hover) | `#1D4ED8` (Deep Blue Hover) | Hover state for primary action buttons |
| `text_primary` | `#F8FAFC` (Near White) | `#0F172A` (Deep Charcoal) | Primary headings, task filenames, speeds |
| `text_secondary` | `#94A3B8` (Muted Slate) | `#64748B` (Muted Gray) | Subheadings, dates, metadata labels |
| `text_muted` | `#64748B` (Dimmed Slate) | `#94A3B8` (Light Slate) | Disabled buttons, inactive status |
| `border_subtle` | `#334155` (Subtle Border) | `#E2E8F0` (Light Border) | Card borders, column dividers, status bar |
| `status_success` | `#22C55E` (Emerald Green) | `#16A34A` (Forest Green) | Completed downloads, connected indicators |
| `status_warning` | `#F59E0B` (Amber) | `#D97706` (Deep Amber) | Stalled transfers, retrying, warnings |
| `status_error` | `#EF4444` (Vibrant Red) | `#DC2626` (Crimson Red) | Failed downloads, disk full, network errors |
| `badge_stream` | `#8B5CF6` (Violet / Purple) | `#7C3AED` (Deep Violet) | HLS & DASH adaptive stream badges |

---

## 3. Typography & Spacing System

- **Primary Font Family**: `Segoe UI Variable`, `Segoe UI`, `-apple-system`, `sans-serif`.
- **Monospace Code/Hash Font**: `Consolas`, `Cascadia Code`, `Courier New`, monospace.
- **Base Grid**: 4px / 8px grid system for all layout padding, margins, and control heights.
- **Border Radius**: 6px for standard buttons and dialog cards; 8px for modal containers; 9999px for pills and badges.

---

## 4. Logo & Iconography

- **Official Mark**: Stylized letter **R** within an electric blue rounded hexagonal / shield container with energetic gradient refraction.
- **Desktop Executable Icon**: Multi-size Windows icon file (`radium.ico`) supporting 16x16, 24x24, 32x32, 48x48, and 256x256 dimensions.
- **Browser Extension Icons**: PNG suite at 16x16, 32x32, 48x48, and 128x128 pixels in `extension/icons/`.
- **System Tray Icon**: High-contrast icon rendered dynamically with GDI vector fallback.
