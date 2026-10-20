// SPDX-License-Identifier: GPL-3.0-or-later
// Radium Download Manager — Coral Dark UI Design System & High-DPI Metrics
// Defines the Coral Dark color palette, 4px spacing grid, Segoe UI typography ramp,
// High-DPI scaling math, minimum window constraints, geometric vector icons, and
// the lightweight native animation state for smooth UI transitions.
#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

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
    kBrowserConnected,

    kSpeedLimit,
    kFolderRule,
    kDiagnostics
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

    constexpr ColorRgba BlendOver(const ColorRgba& bg, uint8_t override_alpha = 255) const {
        uint32_t eff_a = (static_cast<uint32_t>(a) * static_cast<uint32_t>(override_alpha)) / 255u;
        uint32_t inv_a = 255u - eff_a;
        return ColorRgba{
            static_cast<uint8_t>((r * eff_a + bg.r * inv_a) / 255u),
            static_cast<uint8_t>((g * eff_a + bg.g * inv_a) / 255u),
            static_cast<uint8_t>((b * eff_a + bg.b * inv_a) / 255u),
            255
        };
    }

    constexpr ColorRgba Interpolate(const ColorRgba& other, float t) const {
        t = std::clamp(t, 0.0f, 1.0f);
        float inv = 1.0f - t;
        return ColorRgba{
            static_cast<uint8_t>(std::clamp(r * inv + other.r * t, 0.0f, 255.0f)),
            static_cast<uint8_t>(std::clamp(g * inv + other.g * t, 0.0f, 255.0f)),
            static_cast<uint8_t>(std::clamp(b * inv + other.b * t, 0.0f, 255.0f)),
            static_cast<uint8_t>(std::clamp(a * inv + other.a * t, 0.0f, 255.0f))
        };
    }

    std::string ToCssHex() const {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "#%02X%02X%02X", r, g, b);
        return std::string(buf);
    }
};

struct ThemePalette {
    ThemeMode mode{ThemeMode::kDark};
    ColorRgba app_background;      // Main window canvas (#0F172A Deep Space Slate)
    ColorRgba cosmic_void;         // Deep space charcoal void (#070B14)
    ColorRgba cosmic_nebula;       // Subtle atmospheric navy tone (#111A2E)
    ColorRgba cosmic_indigo;       // Deep indigo atmospheric accent (#1E1B4B)
    ColorRgba surface_primary;     // Sidebar, toolbar, details pane (#1E293B)
    ColorRgba surface_elevated;    // Dialogs, dropdowns, cards (#334155)
    ColorRgba surface_hover;       // Row/button hover state (#475569)
    ColorRgba surface_selected;    // Selected row/category state (#1E3A8A)
    ColorRgba surface_pressed;     // Pressed button/control state
    ColorRgba border_subtle;       // 1px hairlines and dividers (#334155)
    ColorRgba border_focus;        // 2px keyboard focus ring (#38BDF8 / #60A5FA)
    ColorRgba text_primary;        // High-contrast body & filenames (#F8FAFC)
    ColorRgba text_secondary;      // Metadata, column headers, sizes (#94A3B8)
    ColorRgba text_muted;          // Disabled controls & placeholders (#64748B)
    ColorRgba accent_primary;      // Restrained blue primary action (#2563EB)
    ColorRgba accent_hover;        // Blue hover state (#3B82F6)
    ColorRgba accent_cyan;         // Restrained cosmic cyan accent (#06B6D4)
    ColorRgba accent_violet;       // Celestial violet accent (#8B5CF6)
    ColorRgba badge_stream;        // HLS/DASH stream indicator (#8B5CF6)
    ColorRgba badge_file;          // Ordinary HTTP file indicator (#2563EB)
    ColorRgba status_success;      // Completed (#22C55E)
    ColorRgba status_warning;      // Paused / Retrying (#F59E0B)
    ColorRgba status_danger;       // Failed / Error (#EF4444)
    ColorRgba progress_track;      // Unfilled progress bar background (#1E293B)
    ColorRgba star_bright;         // Foreground celestial star point (#E2E8F0)
    ColorRgba star_mid;            // Mid-field star point (#94A3B8)
    ColorRgba star_dim;            // Deep dust star point (#38BDF8)
    ColorRgba glass_surface;       // Translucent glass panel fill
    ColorRgba glass_elevated;      // Translucent floating card/modal fill
    ColorRgba glass_border;        // Fine low-contrast glass border
    ColorRgba glass_highlight;     // Fine specular top edge highlight
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
    int font_caption_px{12};
    int font_body_px{14};
    int font_body_strong_px{14};
    int font_subtitle_px{16};
    int font_title_px{20};
};

// Lightweight Native UI Animation System (short, polished 100-200ms transitions)
struct UiAnimationState {
    bool reduced_motion{false};
    bool window_visible{true};
    bool window_minimized{false};

    // 1. Button hover & press feedback
    int hovered_toolbar_idx{-1};
    uint64_t toolbar_hover_start_ms{0};
    int pressed_toolbar_idx{-1};

    // 2. Row hover & selection transitions
    int hovered_row_idx{-1};
    uint64_t row_hover_start_ms{0};
    int selected_row_idx{-1};
    uint64_t row_select_start_ms{0};

    // 3. Smoothly interpolated progress bar rendering
    std::unordered_map<std::string, float> progress_interpolated;

    // 4. Dialog transitions (Settings & Add Download)
    bool settings_open{false};
    uint64_t settings_open_start_ms{0};
    bool add_dialog_open{false};
    uint64_t add_dialog_open_start_ms{0};

    // 5. Category selection transition
    int prev_category_idx{-1};
    int current_category_idx{0};
    uint64_t category_transition_start_ms{0};

    // 6. Action icon one-time pulses (pause, resume, retry, completed)
    struct ActionPulse {
        uint64_t start_ms{0};
        std::string action;
    };
    std::unordered_map<std::string, ActionPulse> icon_pulses;

    bool IsActive() const {
        return window_visible && !window_minimized && !reduced_motion;
    }

    float ComputeTransitionProgress(uint64_t start_ms, uint64_t duration_ms = 150) const {
        if (!IsActive() || start_ms == 0) return 1.0f;
        uint64_t now = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
        if (now <= start_ms) return 0.0f;
        uint64_t elapsed = now - start_ms;
        if (elapsed >= duration_ms) return 1.0f;
        float t = static_cast<float>(elapsed) / static_cast<float>(duration_ms);
        float inv = 1.0f - t;
        return 1.0f - (inv * inv * inv);
    }
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

        // Enforce >= 12px caption floor and >= 15px body floor for modern Windows 11 readability
        m.font_caption_px = ScaleForDpi(12, dpi);
        m.font_body_px = ScaleForDpi(15, dpi);
        m.font_body_strong_px = ScaleForDpi(15, dpi);
        m.font_subtitle_px = ScaleForDpi(16, dpi);
        m.font_title_px = ScaleForDpi(20, dpi);
        return m;
    }

    static ThemePalette GetDarkPalette() {
        ThemePalette p;
        p.mode = ThemeMode::kDark;
        p.app_background   = {0x0F, 0x17, 0x2A, 255}; // #0F172A Radium Deep Space Slate
        p.cosmic_void      = {0x07, 0x0B, 0x14, 255}; // #070B14 Deep Space Void
        p.cosmic_nebula    = {0x11, 0x1A, 0x2E, 255}; // #111A2E Atmospheric Navy
        p.cosmic_indigo    = {0x1E, 0x1B, 0x4B, 255}; // #1E1B4B Deep Indigo Tone
        p.surface_primary  = {0x1E, 0x29, 0x3B, 255}; // #1E293B Dark Navy Surface
        p.surface_elevated = {0x33, 0x41, 0x55, 255}; // #334155 Elevated Surface
        p.surface_hover    = {0x47, 0x55, 0x69, 255}; // #475569 Interactive Hover
        p.surface_selected = {0x1E, 0x3A, 0x8A, 255}; // #1E3A8A Deep Blue Selection
        p.surface_pressed  = {0x16, 0x22, 0x38, 255};
        p.border_subtle    = {0x33, 0x41, 0x55, 255}; // #334155 Hairline Divider
        p.border_focus     = {0x38, 0xBD, 0xF8, 255}; // #38BDF8 Cosmic Cyan Focus Ring
        p.text_primary     = {0xF8, 0xFA, 0xFC, 255}; // #F8FAFC Crisp Off-White
        p.text_secondary   = {0x94, 0xA3, 0xB8, 255}; // #94A3B8 Secondary Metadata
        p.text_muted       = {0x64, 0x74, 0x8B, 255}; // #64748B Disabled / Hint
        p.accent_primary   = {0x25, 0x63, 0xEB, 255}; // #2563EB Radium Blue Accent
        p.accent_hover     = {0x3B, 0x82, 0xF6, 255}; // #3B82F6 Accent Hover
        p.accent_cyan      = {0x06, 0xB6, 0xD4, 255}; // #06B6D4 Cosmic Cyan Accent
        p.accent_violet    = {0x8B, 0x5C, 0xF6, 255}; // #8B5CF6 Celestial Violet
        p.badge_stream     = {0x8B, 0x5C, 0xF6, 255}; // #8B5CF6 Media Stream Badge
        p.badge_file       = {0x25, 0x63, 0xEB, 255}; // #2563EB Direct File Badge
        p.status_success   = {0x22, 0xC5, 0x5E, 255}; // #22C55E Completed Green
        p.status_warning   = {0xF5, 0x9E, 0x0B, 255}; // #F59E0B Paused / Warning Amber
        p.status_danger    = {0xEF, 0x44, 0x44, 255}; // #EF4444 Failed / Error Red
        p.progress_track   = {0x1E, 0x29, 0x3B, 255}; // #1E293B Progress Track
        p.star_bright      = {0xE2, 0xE8, 0xF0, 210}; // #E2E8F0 Bright Starlight
        p.star_mid         = {0x94, 0xA3, 0xB8, 145}; // #94A3B8 Mid Starlight
        p.star_dim         = {0x38, 0xBD, 0xF8, 95};  // #38BDF8 Cyan Dust Star
        p.glass_surface    = {0x18, 0x22, 0x34, 215}; // Translucent dark glass
        p.glass_elevated   = {0x24, 0x30, 0x44, 230}; // Elevated translucent glass
        p.glass_border     = {0x4A, 0x55, 0x68, 140}; // Subtle glass border
        p.glass_highlight  = {0xFF, 0xFF, 0xFF, 36};  // Subtle specular edge
        return p;
    }

    static ThemePalette GetLightPalette() {
        ThemePalette p = GetDarkPalette();
        p.mode = ThemeMode::kLight;
        p.app_background   = {0xF8, 0xFA, 0xFC, 255};
        p.surface_primary  = {0xFF, 0xFF, 0xFF, 255};
        p.surface_elevated = {0xF1, 0xF5, 0xF9, 255};
        p.surface_hover    = {0xE2, 0xE8, 0xF0, 255};
        p.surface_selected = {0xDB, 0xEA, 0xFE, 255};
        p.surface_pressed  = {0xC8, 0xD4, 0xEE, 255};
        p.border_subtle    = {0xCB, 0xD5, 0xE1, 255};
        p.border_focus     = {0x25, 0x63, 0xEB, 255};
        p.text_primary     = {0x0F, 0x17, 0x2A, 255};
        p.text_secondary   = {0x47, 0x55, 0x69, 255};
        p.text_muted       = {0x94, 0xA3, 0xB8, 255};
        p.accent_primary   = {0x25, 0x63, 0xEB, 255};
        p.accent_hover     = {0x1D, 0x4E, 0xD8, 255};
        p.badge_stream     = {0x7C, 0x3A, 0xED, 255};
        p.badge_file       = {0x25, 0x63, 0xEB, 255};
        p.status_success   = {0x16, 0xA3, 0x4A, 255};
        p.status_warning   = {0xD9, 0x77, 0x06, 255};
        p.status_danger    = {0xDC, 0x26, 0x26, 255};
        p.progress_track   = {0xE2, 0xE8, 0xF0, 255};
        p.glass_surface    = {0xFA, 0xFA, 0xFC, 225};
        p.glass_elevated   = {0xFF, 0xFF, 0xFF, 240};
        p.glass_border     = {0xCB, 0xD5, 0xE1, 160};
        p.glass_highlight  = {0xFF, 0xFF, 0xFF, 120};
        return p;
    }

    static ThemePalette GetRedesignPalette() {
        ThemePalette p = GetDarkPalette();
        p.app_background   = {0x16, 0x16, 0x18, 255}; // #161618 Warm Matte Charcoal
        p.cosmic_void      = {0x12, 0x12, 0x14, 255}; // #121214
        p.cosmic_nebula    = {0x1A, 0x1A, 0x1E, 255}; // #1A1A1E
        p.surface_primary  = {0x1E, 0x1E, 0x22, 255}; // #1E1E22 Surface
        p.surface_elevated = {0x28, 0x28, 0x2E, 255}; // #28282E Elevated
        p.surface_hover    = {0x25, 0x25, 0x2B, 255}; // #25252B Row hover
        p.surface_selected = {0x2A, 0x2A, 0x32, 255}; // #2A2A32 Selected row
        p.surface_pressed  = {0x32, 0x32, 0x3C, 255};
        p.border_subtle    = {0x2E, 0x2F, 0x35, 255}; // #2E2F35 Hairline divider
        p.border_focus     = {0xFF, 0x4F, 0x57, 255}; // #FF4F57 Reference Coral Red
        p.text_primary     = {0xF2, 0xF2, 0xF3, 255}; // #F2F2F3 Crisp off-white
        p.text_secondary   = {0xA4, 0xA5, 0xAC, 255}; // #A4A5AC Secondary metadata
        p.text_muted       = {0x72, 0x73, 0x7A, 255}; // #72737A Muted text
        p.accent_primary   = {0xFF, 0x4F, 0x57, 255}; // #FF4F57 Reference Coral Red
        p.accent_hover     = {0xFF, 0x66, 0x6D, 255}; // #FF666D Hover Coral Red
        p.badge_stream     = {0xFF, 0x4F, 0x57, 255}; // #FF4F57 Coral stream badge
        p.badge_file       = {0xFF, 0x4F, 0x57, 255}; // #FF4F57 Coral file badge
        p.status_success   = {0x8E, 0x8E, 0x98, 255}; // #8E8E98
        p.status_warning   = {0x9E, 0x9E, 0xA8, 255}; // #9E9EA8
        p.status_danger    = {0xFF, 0x4F, 0x57, 255}; // #FF4F57 Error Coral Red
        p.progress_track   = {0x2E, 0x2E, 0x36, 255}; // #2E2E36 Progress track
        p.glass_surface    = {0x1C, 0x1C, 0x22, 215}; // Translucent charcoal glass
        p.glass_elevated   = {0x26, 0x26, 0x2E, 230}; // Elevated translucent card
        p.glass_border     = {0x40, 0x42, 0x4E, 140}; // Subtle glass border
        p.glass_highlight  = {0xFF, 0xFF, 0xFF, 36};  // Subtle specular edge
        return p;
    }

    static ThemePalette ResolvePalette(std::string_view theme_setting, bool os_prefers_light = false) {
        if (theme_setting == "light") return GetLightPalette();
        if (theme_setting == "classic_blue" || theme_setting == "cosmic_blue") return GetDarkPalette();
        if (theme_setting == "system") return os_prefers_light ? GetLightPalette() : GetRedesignPalette();
        // "coral_dark", "coral", "charcoal", "dark", or any unrecognized string maps to the Coral Charcoal Redesign Palette
        return GetRedesignPalette();
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

            case IconId::kSpeedLimit:       return "[BW]";
            case IconId::kFolderRule:       return "[DIR]";
            case IconId::kDiagnostics:      return "[DIAG]";
        }
        return "";
    }
};

} // namespace openidm::ui
