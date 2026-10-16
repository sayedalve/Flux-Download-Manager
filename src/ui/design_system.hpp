// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM — Windows 11 Desktop UI Design System, Palettes, Typography & Hit-Test Geometry
// Defines reusable color palettes (Dark Charcoal default, Light, System), 4px spacing grid,
// Segoe UI typography ramp, High-DPI scaling math, minimum window constraints, and geometric vector icons.
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <string>

namespace openidm::ui {

enum class ThemeMode {
    kDark,
    kLight,
    kSystem
};

enum class IconId {
    kAddUrl,
    kStart,
    kPause,
    kResume,
    kRetry,
    kCancel,
    kRemove,
    kClearHistory,
    kOpenFolder,
    kSettings,
    kSearch,
    kFileBadge,
    kStreamBadge,
    kCheckCircle,
    kWarningTriangle,
    kErrorCircle,
    kBrowserConnected
};

struct RectI {
    int left{0};
    int top{0};
    int right{0};
    int bottom{0};

    constexpr int width() const { return right - left; }
    constexpr int height() const { return bottom - top; }
    constexpr bool Contains(int x, int y) const {
        return x >= left && x < right && y >= top && y < bottom;
    }
};

struct ColorRgba {
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
    uint8_t a{255};

    constexpr uint32_t ToWin32ColorRef() const {
        return static_cast<uint32_t>(r) | (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(b) << 16);
    }

    std::string ToCssHex() const {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "#%02X%02X%02X", r, g, b);
        return std::string(buf);
    }
};

struct ThemePalette {
    ThemeMode mode{ThemeMode::kDark};
    ColorRgba app_background;      // Main window canvas
    ColorRgba surface_primary;     // Sidebar, toolbar, details pane
    ColorRgba surface_elevated;    // Dialogs, dropdowns, cards
    ColorRgba surface_hover;       // Row/button hover state
    ColorRgba surface_selected;    // Selected row/category state
    ColorRgba border_subtle;       // 1px hairlines and dividers
    ColorRgba border_focus;        // 2px keyboard focus ring
    ColorRgba text_primary;        // High-contrast body & filenames
    ColorRgba text_secondary;      // Metadata, column headers, sizes
    ColorRgba text_muted;          // Disabled controls & placeholders
    ColorRgba accent_primary;      // Restrained blue primary action
    ColorRgba accent_hover;        // Blue hover state
    ColorRgba badge_stream;        // HLS/DASH stream indicator (#8B5CF6)
    ColorRgba badge_file;          // Ordinary HTTP file indicator (#3B82F6)
    ColorRgba status_success;      // Completed (#22C55E)
    ColorRgba status_warning;      // Paused / Retrying (#F59E0B)
    ColorRgba status_danger;       // Failed / Error (#EF4444)
    ColorRgba progress_track;      // Unfilled progress bar background
};

struct LayoutMetrics {
    uint32_t dpi{96};
    double scale{1.0};

    // 4px base spacing grid scaled for DPI
    int space_xs{4};
    int space_sm{8};
    int space_md{12};
    int space_lg{16};
    int space_xl{24};
    int space_2xl{32};

    // Structural regions
    int min_window_width{900};
    int min_window_height{560};
    int compact_width_threshold{1024};
    int toolbar_height{48};
    int sidebar_width{216};
    int table_header_height{34};
    int table_row_height{44};
    int details_panel_height{188};
    int status_bar_height{28};
    int button_height{32};
    int progress_bar_height{6};
    int corner_radius{6};
    int focus_ring_width{2};

    // Typography ramp (in pixels at current DPI)
    int font_caption_px{11};
    int font_body_px{13};
    int font_body_strong_px{13};
    int font_subtitle_px{15};
    int font_title_px{18};
};

class DesignSystem {
public:
    static constexpr int ScaleForDpi(int base_px, uint32_t dpi) {
        return static_cast<int>((static_cast<int64_t>(base_px) * static_cast<int64_t>(dpi) + 48) / 96);
    }

    static LayoutMetrics ComputeMetrics(uint32_t dpi = 96) {
        dpi = std::clamp(dpi, 96u, 384u);
        LayoutMetrics m;
        m.dpi = dpi;
        m.scale = static_cast<double>(dpi) / 96.0;
        m.space_xs = ScaleForDpi(4, dpi);
        m.space_sm = ScaleForDpi(8, dpi);
        m.space_md = ScaleForDpi(12, dpi);
        m.space_lg = ScaleForDpi(16, dpi);
        m.space_xl = ScaleForDpi(24, dpi);
        m.space_2xl = ScaleForDpi(32, dpi);

        m.min_window_width = ScaleForDpi(900, dpi);
        m.min_window_height = ScaleForDpi(560, dpi);
        m.compact_width_threshold = ScaleForDpi(1024, dpi);
        m.toolbar_height = ScaleForDpi(48, dpi);
        m.sidebar_width = ScaleForDpi(216, dpi);
        m.table_header_height = ScaleForDpi(34, dpi);
        m.table_row_height = ScaleForDpi(44, dpi);
        m.details_panel_height = ScaleForDpi(188, dpi);
        m.status_bar_height = ScaleForDpi(28, dpi);
        m.button_height = ScaleForDpi(32, dpi);
        m.progress_bar_height = ScaleForDpi(6, dpi);
        m.corner_radius = ScaleForDpi(6, dpi);
        m.focus_ring_width = std::max(1, ScaleForDpi(2, dpi));

        // Enforce >= 11pt/px caption floor and >= 13px body floor
        m.font_caption_px = ScaleForDpi(11, dpi);
        m.font_body_px = ScaleForDpi(13, dpi);
        m.font_body_strong_px = ScaleForDpi(13, dpi);
        m.font_subtitle_px = ScaleForDpi(15, dpi);
        m.font_title_px = ScaleForDpi(18, dpi);
        return m;
    }

    static ThemePalette GetDarkPalette() {
        ThemePalette p;
        p.mode = ThemeMode::kDark;
        p.app_background   = {0x18, 0x18, 0x1B, 255}; // #18181B Deep Charcoal
        p.surface_primary  = {0x1F, 0x1F, 0x23, 255}; // #1F1F23 Elevated Charcoal
        p.surface_elevated = {0x27, 0x27, 0x2A, 255}; // #27272A Card / Dialog Surface
        p.surface_hover    = {0x2E, 0x2E, 0x33, 255}; // #2E2E33 Interactive Hover
        p.surface_selected = {0x1E, 0x29, 0x3B, 255}; // #1E293B Restrained Blue-Slate Selection
        p.border_subtle    = {0x3F, 0x3F, 0x46, 255}; // #3F3F46 Hairline Divider
        p.border_focus     = {0x60, 0xA5, 0xFA, 255}; // #60A5FA High-Contrast Focus Ring
        p.text_primary     = {0xF4, 0xF4, 0xF5, 255}; // #F4F4F5 Crisp Off-White
        p.text_secondary   = {0xA1, 0xA1, 0xAA, 255}; // #A1A1AA Secondary Metadata
        p.text_muted       = {0x71, 0x71, 0x7A, 255}; // #71717A Disabled / Hint
        p.accent_primary   = {0x3B, 0x82, 0xF6, 255}; // #3B82F6 Restrained Blue Accent
        p.accent_hover     = {0x25, 0x63, 0xEB, 255}; // #2563EB Accent Hover
        p.badge_stream     = {0x8B, 0x5C, 0xF6, 255}; // #8B5CF6 Media Stream Badge
        p.badge_file       = {0x3B, 0x82, 0xF6, 255}; // #3B82F6 Direct File Badge
        p.status_success   = {0x22, 0xC5, 0x5E, 255}; // #22C55E Completed Green
        p.status_warning   = {0xF5, 0x9E, 0x0B, 255}; // #F59E0B Paused / Warning Amber
        p.status_danger    = {0xEF, 0x44, 0x44, 255}; // #EF4444 Failed / Error Red
        p.progress_track   = {0x32, 0x32, 0x38, 255}; // #323238 Progress Track
        return p;
    }

    static ThemePalette GetLightPalette() {
        ThemePalette p;
        p.mode = ThemeMode::kLight;
        p.app_background   = {0xF8, 0xFA, 0xFC, 255}; // #F8FAFC Alabaster
        p.surface_primary  = {0xFF, 0xFF, 0xFF, 255}; // #FFFFFF Pure White
        p.surface_elevated = {0xF1, 0xF5, 0xF9, 255}; // #F1F5F9 Slate 100
        p.surface_hover    = {0xE2, 0xE8, 0xF0, 255}; // #E2E8F0 Hover
        p.surface_selected = {0xDB, 0xEA, 0xFE, 255}; // #DBEAFE Blue 100 Selection
        p.border_subtle    = {0xCB, 0xD5, 0xE1, 255}; // #CBD5E1 Border
        p.border_focus     = {0x25, 0x63, 0xEB, 255}; // #2563EB Focus Ring
        p.text_primary     = {0x0F, 0x17, 0x2A, 255}; // #0F172A Slate 900
        p.text_secondary   = {0x47, 0x55, 0x69, 255}; // #475569 Slate 600
        p.text_muted       = {0x94, 0xA3, 0xB8, 255}; // #94A3B8 Slate 400
        p.accent_primary   = {0x25, 0x63, 0xEB, 255}; // #2563EB Blue 600
        p.accent_hover     = {0x1D, 0x4E, 0xD8, 255}; // #1D4ED8 Blue 700
        p.badge_stream     = {0x7C, 0x3A, 0xED, 255}; // #7C3AED Stream Badge
        p.badge_file       = {0x25, 0x63, 0xEB, 255}; // #2563EB File Badge
        p.status_success   = {0x16, 0xA3, 0x4A, 255}; // #16A34A Green 600
        p.status_warning   = {0xD9, 0x77, 0x06, 255}; // #D97706 Amber 600
        p.status_danger    = {0xDC, 0x26, 0x26, 255}; // #DC2626 Red 600
        p.progress_track   = {0xE2, 0xE8, 0xF0, 255}; // #E2E8F0 Progress Track
        return p;
    }

    static ThemePalette ResolvePalette(std::string_view theme_setting, bool os_prefers_light = false) {
        if (theme_setting == "light") return GetLightPalette();
        if (theme_setting == "system") return os_prefers_light ? GetLightPalette() : GetDarkPalette();
        return GetDarkPalette();
    }

    static const char* GetAccessibleIconGlyph(IconId icon) {
        switch (icon) {
            case IconId::kAddUrl:           return "[+]";
            case IconId::kStart:            return "[>]";
            case IconId::kPause:            return "[||]";
            case IconId::kResume:           return "[|>]";
            case IconId::kRetry:            return "[R]";
            case IconId::kCancel:           return "[X]";
            case IconId::kRemove:           return "[-]";
            case IconId::kClearHistory:     return "[C]";
            case IconId::kOpenFolder:       return "[D]";
            case IconId::kSettings:         return "[S]";
            case IconId::kSearch:           return "[?]";
            case IconId::kFileBadge:        return "FILE";
            case IconId::kStreamBadge:      return "STREAM";
            case IconId::kCheckCircle:      return "OK";
            case IconId::kWarningTriangle:  return "WARN";
            case IconId::kErrorCircle:      return "ERR";
            case IconId::kBrowserConnected: return "IPC";
        }
        return "";
    }
};

} // namespace openidm::ui
