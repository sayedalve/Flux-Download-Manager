// SPDX-License-Identifier: GPL-3.0-or-later
// Flux Download Manager — Minimal Cosmic Dark UI Design System, Starfield Engine & High-DPI Metrics
// Defines the Cosmic Dark color palette, 4px spacing grid, Segoe UI typography ramp,
// High-DPI scaling math, minimum window constraints, geometric vector icons, and
// a zero-allocation 3-layer Cosmic Starfield with reduced-motion and hidden-window suspension.
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
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
    kCosmicStar,
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

// Pre-rendered deterministic star node (zero dynamic allocations or per-frame RNG)
struct StarNode {
    uint16_t norm_x{0};      // 0..999 normalized X coordinate
    uint16_t norm_y{0};      // 0..999 normalized Y coordinate
    uint8_t layer{0};        // 0 = deep dust (120s), 1 = mid-field (90s), 2 = foreground (60s)
    uint8_t base_size_px{1}; // 1 or 2 base pixel diameter
    uint8_t base_alpha{90};  // 40..195 restrained opacity so text contrast is never compromised
    uint8_t tint_index{0};   // 0 = starlight white, 1 = ice blue, 2 = cosmic cyan, 3 = soft violet
    uint16_t phase_ms{0};    // 0..59999 phase offset for subtle opacity modulation
};

struct StarRenderInstance {
    int x{0};
    int y{0};
    int size_px{1};
    uint8_t alpha{90};
    uint8_t layer{0};
    ColorRgba blended_color;
};

class CosmicStarfield {
public:
    static constexpr size_t kStarCount = 48;
    static constexpr uint64_t kLayerPeriodMs[3] = {120000ULL, 90000ULL, 60000ULL};

    CosmicStarfield() {
        InitializeDeterministicStars();
    }

    void SetReducedMotion(bool reduced_motion) {
        m_reduced_motion = reduced_motion;
    }
    bool IsReducedMotion() const { return m_reduced_motion; }

    void SetWindowState(bool visible, bool minimized) {
        m_window_visible = visible;
        m_window_minimized = minimized;
    }
    bool IsWindowVisible() const { return m_window_visible; }
    bool IsWindowMinimized() const { return m_window_minimized; }

    // Decorative drift is active ONLY when window is visible, not minimized, and reduced-motion is off
    bool IsAnimationActive() const {
        return m_window_visible && !m_window_minimized && !m_reduced_motion;
    }

    // Clean static fallback retains the exact same starfield layout at rest (phase = 0)
    bool IsStaticFallbackActive() const {
        return !IsAnimationActive();
    }

    uint64_t GetElapsedMs() const { return m_elapsed_ms; }

    void AdvanceTime(uint64_t delta_ms) {
        if (!IsAnimationActive()) {
            return;
        }
        m_elapsed_ms = (m_elapsed_ms + delta_ms) % 360000ULL;
    }

    std::pair<int, int> ComputeLayerDriftOffset(uint8_t layer, int width, int height) const {
        if (IsStaticFallbackActive() || width <= 0 || height <= 0) {
            return {0, 0};
        }
        uint8_t safe_layer = std::min<uint8_t>(layer, 2);
        uint64_t period = kLayerPeriodMs[safe_layer];
        uint64_t phase = m_elapsed_ms % period;
        // Gentle horizontal and slight vertical drift across the long 60s-120s period
        int dx = static_cast<int>((phase * static_cast<uint64_t>(width)) / period);
        int dy = static_cast<int>((phase * static_cast<uint64_t>(height)) / (period * 4ULL));
        return {dx, dy};
    }

    std::vector<StarRenderInstance> BuildRenderStars(
        const RectI& bounds,
        uint32_t dpi,
        const ThemePalette& palette) const {
        std::vector<StarRenderInstance> out;
        int w = bounds.width();
        int h = bounds.height();
        if (w <= 0 || h <= 0) return out;

        out.reserve(kStarCount);
        std::pair<int, int> layer_offsets[3] = {
            ComputeLayerDriftOffset(0, w, h),
            ComputeLayerDriftOffset(1, w, h),
            ComputeLayerDriftOffset(2, w, h)
        };

        int dpi_scale_1px = std::max(1, static_cast<int>((dpi + 48) / 96));
        int dpi_scale_2px = std::max(2, static_cast<int>((2 * dpi + 48) / 96));

        for (const auto& s : m_stars) {
            const auto& [dx, dy] = layer_offsets[s.layer];
            int base_x = (static_cast<int>(s.norm_x) * w) / 1000;
            int base_y = (static_cast<int>(s.norm_y) * h) / 1000;
            int rx = bounds.left + ((base_x + dx) % w);
            int ry = bounds.top + ((base_y + dy) % h);

            uint8_t alpha = s.base_alpha;
            if (IsAnimationActive()) {
                // Lightweight triangle-wave breathing over a 60s cycle (+/- 20 alpha units)
                uint64_t p = (m_elapsed_ms + s.phase_ms) % 60000ULL;
                int wave = (p < 30000ULL)
                    ? static_cast<int>((p * 40ULL) / 30000ULL) - 20
                    : 20 - static_cast<int>(((p - 30000ULL) * 40ULL) / 30000ULL);
                alpha = static_cast<uint8_t>(std::clamp(static_cast<int>(s.base_alpha) + wave, 30, 210));
            }

            ColorRgba star_col = palette.star_bright;
            if (s.tint_index == 1) star_col = palette.star_mid;
            else if (s.tint_index == 2) star_col = palette.star_dim;
            else if (s.tint_index == 3) star_col = palette.accent_violet;

            StarRenderInstance inst;
            inst.x = rx;
            inst.y = ry;
            inst.size_px = (s.base_size_px > 1) ? dpi_scale_2px : dpi_scale_1px;
            inst.alpha = alpha;
            inst.layer = s.layer;
            inst.blended_color = star_col.BlendOver(palette.app_background, alpha);
            out.push_back(inst);
        }
        return out;
    }

    const std::array<StarNode, kStarCount>& GetStarNodes() const { return m_stars; }

private:
    void InitializeDeterministicStars() {
        // Deterministic LCG seeded once at construction for balanced spatial distribution
        uint32_t seed = 0x464C5558u; // "FLUX"
        auto next_u32 = [&]() -> uint32_t {
            seed = seed * 1664525u + 1013904223u;
            return seed;
        };

        for (size_t i = 0; i < kStarCount; ++i) {
            StarNode n;
            // Stratified golden-ratio distribution mixed with deterministic jitter to avoid clumping
            n.norm_x = static_cast<uint16_t>((i * 618u + (next_u32() % 180u)) % 1000u);
            n.norm_y = static_cast<uint16_t>((i * 382u + (next_u32() % 220u)) % 1000u);
            if (i < 24) {
                n.layer = 0; // Deep space dust (24 stars)
                n.base_size_px = 1;
                n.base_alpha = static_cast<uint8_t>(45 + (next_u32() % 45)); // 45..89
            } else if (i < 40) {
                n.layer = 1; // Mid-field stars (16 stars)
                n.base_size_px = (i % 3 == 0) ? 2 : 1;
                n.base_alpha = static_cast<uint8_t>(85 + (next_u32() % 55)); // 85..139
            } else {
                n.layer = 2; // Foreground celestial points (8 stars)
                n.base_size_px = 2;
                n.base_alpha = static_cast<uint8_t>(130 + (next_u32() % 55)); // 130..184
            }
            n.tint_index = static_cast<uint8_t>(next_u32() % 4u);
            n.phase_ms = static_cast<uint16_t>(next_u32() % 60000u);
            m_stars[i] = n;
        }
    }

    std::array<StarNode, kStarCount> m_stars{};
    uint64_t m_elapsed_ms{0};
    bool m_reduced_motion{false};
    bool m_window_visible{true};
    bool m_window_minimized{false};
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

        // Enforce >= 12px caption floor and >= 14px body floor for modern Windows 11 readability
        m.font_caption_px = ScaleForDpi(12, dpi);
        m.font_body_px = ScaleForDpi(14, dpi);
        m.font_body_strong_px = ScaleForDpi(14, dpi);
        m.font_subtitle_px = ScaleForDpi(16, dpi);
        m.font_title_px = ScaleForDpi(20, dpi);
        return m;
    }

    static ThemePalette GetDarkPalette() {
        ThemePalette p;
        p.mode = ThemeMode::kDark;
        p.app_background   = {0x0F, 0x17, 0x2A, 255}; // #0F172A Flux Deep Space Slate
        p.cosmic_void      = {0x07, 0x0B, 0x14, 255}; // #070B14 Deep Space Void
        p.cosmic_nebula    = {0x11, 0x1A, 0x2E, 255}; // #111A2E Atmospheric Navy
        p.cosmic_indigo    = {0x1E, 0x1B, 0x4B, 255}; // #1E1B4B Deep Indigo Tone
        p.surface_primary  = {0x1E, 0x29, 0x3B, 255}; // #1E293B Dark Navy Surface
        p.surface_elevated = {0x33, 0x41, 0x55, 255}; // #334155 Elevated Surface
        p.surface_hover    = {0x47, 0x55, 0x69, 255}; // #475569 Interactive Hover
        p.surface_selected = {0x1E, 0x3A, 0x8A, 255}; // #1E3A8A Deep Blue Selection
        p.border_subtle    = {0x33, 0x41, 0x55, 255}; // #334155 Hairline Divider
        p.border_focus     = {0x38, 0xBD, 0xF8, 255}; // #38BDF8 Cosmic Cyan Focus Ring
        p.text_primary     = {0xF8, 0xFA, 0xFC, 255}; // #F8FAFC Crisp Off-White
        p.text_secondary   = {0x94, 0xA3, 0xB8, 255}; // #94A3B8 Secondary Metadata
        p.text_muted       = {0x64, 0x74, 0x8B, 255}; // #64748B Disabled / Hint
        p.accent_primary   = {0x25, 0x63, 0xEB, 255}; // #2563EB Flux Blue Accent
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
            case IconId::kCosmicStar:       return "[*]";
            case IconId::kSpeedLimit:       return "[BW]";
            case IconId::kFolderRule:       return "[DIR]";
            case IconId::kDiagnostics:      return "[DIAG]";
        }
        return "";
    }
};

} // namespace openidm::ui
