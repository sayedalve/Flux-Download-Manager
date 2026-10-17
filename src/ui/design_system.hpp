// SPDX-License-Identifier: GPL-3.0-or-later
// Flux Download Manager — Minimal Cosmic Dark UI Design System, Star-Field Engine,
// Typography Ramp, High-DPI Layout Metrics & Accessible Geometric Glyphs.
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
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
    kBrowserConnected
};

enum class CosmicRenderMode {
    kAnimatedDrift,
    kStaticFallback,
    kSuspendedHidden
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

    static constexpr ColorRgba BlendOver(ColorRgba bg, ColorRgba fg, uint8_t alpha) {
        uint32_t a = alpha;
        uint32_t inv_a = 255u - a;
        return ColorRgba{
            static_cast<uint8_t>((bg.r * inv_a + fg.r * a) / 255u),
            static_cast<uint8_t>((bg.g * inv_a + fg.g * a) / 255u),
            static_cast<uint8_t>((bg.b * inv_a + fg.b * a) / 255u),
            255
        };
    }
};

struct ThemePalette {
    ThemeMode mode{ThemeMode::kDark};
    ColorRgba cosmic_void;         // Deepest space obsidian backdrop (#060913)
    ColorRgba app_background;      // Main window deep space charcoal canvas (#090D16)
    ColorRgba cosmic_atmosphere;   // Subtle navy-indigo atmospheric band (#0C1322)
    ColorRgba surface_primary;     // Sidebar, header, details pane (#0F172A)
    ColorRgba surface_elevated;    // Elevated controls, badges, table headers (#172033)
    ColorRgba surface_hover;       // Interactive row/button hover state (#1E293B)
    ColorRgba surface_selected;    // Selected row/category state (#172554)
    ColorRgba border_subtle;       // 1px restrained structural hairlines (#1E293B)
    ColorRgba border_strong;       // Control & input border (#26334D)
    ColorRgba border_focus;        // 2px keyboard focus ring (#38BDF8)
    ColorRgba text_primary;        // High-contrast starlight off-white (#F8FAFC)
    ColorRgba text_secondary;      // Atmospheric slate metadata & headers (#94A3B8)
    ColorRgba text_muted;          // Disabled controls & subtle hints (#64748B)
    ColorRgba accent_primary;      // Restrained cosmic blue primary action (#2563EB)
    ColorRgba accent_hover;        // Luminous blue hover state (#3B82F6)
    ColorRgba accent_cyan;         // Celestial cyan highlight (#38BDF8)
    ColorRgba accent_indigo;       // Atmospheric indigo accent (#6366F1)
    ColorRgba badge_stream;        // HLS/DASH/YouTube stream indicator (#8B5CF6)
    ColorRgba badge_file;          // Direct segmented HTTP file indicator (#2563EB)
    ColorRgba status_success;      // Completed emerald (#10B981)
    ColorRgba status_warning;      // Paused / Retrying amber (#F59E0B)
    ColorRgba status_danger;       // Failed / Error crimson (#EF4444)
    ColorRgba status_queued;       // Queued / Scheduled indigo (#6366F1)
    ColorRgba progress_track;      // Unfilled progress bar track (#131C2E)
    ColorRgba star_bright;         // Delicate bright star point (#E2E8F0)
    ColorRgba star_cool;           // Cool cyan-blue star point (#93C5FD)
    ColorRgba star_violet;         // Understated violet star point (#C4B5FD)
    ColorRgba star_dim;            // Distant faint star dust (#475569)
};

struct CosmicLayerTransform {
    uint8_t layer_index{0};
    int period_ms{180000};
    double offset_x_px{0.0};
    double offset_y_px{0.0};
    uint8_t opacity_scale{255};
};

struct CosmicStarPoint {
    int x_px{0};
    int y_px{0};
    int size_px{1};
    uint8_t opacity{100};
    uint8_t layer{0};
    bool has_celestial_cross{false};
    ColorRgba color;
};

struct CosmicBackgroundState {
    CosmicRenderMode mode{CosmicRenderMode::kAnimatedDrift};
    bool animation_active{true};
    bool reduced_motion{false};
    bool window_hidden_or_minimized{false};
    int64_t sample_time_ms{0};
    std::vector<CosmicLayerTransform> layers;
    std::vector<CosmicStarPoint> stars;
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

    // Settings center dimensions
    int settings_sidebar_width{210};
    int settings_row_height{48};

    // Typography ramp (in pixels at current DPI)
    int font_caption_px{11};
    int font_body_px{13};
    int font_body_strong_px{13};
    int font_subtitle_px{15};
    int font_title_px{18};
};

class DesignSystem {
public:
    struct StarSeed {
        uint16_t norm_x; // 0..999
        uint16_t norm_y; // 0..999
        uint8_t layer;   // 0 = deep dust, 1 = midfield, 2 = foreground celestial
        uint8_t base_size_px;
        uint8_t base_opacity;
        uint8_t tint;    // 0 = dim, 1 = cool blue, 2 = violet, 3 = bright starlight
        bool cross_accent;
    };

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

        m.settings_sidebar_width = ScaleForDpi(210, dpi);
        m.settings_row_height = ScaleForDpi(48, dpi);

        // Enforce >= 11px caption floor and >= 13px body floor
        m.font_caption_px = ScaleForDpi(11, dpi);
        m.font_body_px = ScaleForDpi(13, dpi);
        m.font_body_strong_px = ScaleForDpi(13, dpi);
        m.font_subtitle_px = ScaleForDpi(15, dpi);
        m.font_title_px = ScaleForDpi(18, dpi);
        return m;
    }

    static ThemePalette GetDarkPalette() {
        ThemePalette p;
        p.mode              = ThemeMode::kDark;
        p.cosmic_void       = {0x06, 0x09, 0x13, 255}; // #060913 Deepest Space Void
        p.app_background    = {0x09, 0x0D, 0x16, 255}; // #090D16 Deep Space Charcoal
        p.cosmic_atmosphere = {0x0C, 0x13, 0x22, 255}; // #0C1322 Subtle Navy/Indigo Atmosphere
        p.surface_primary   = {0x0F, 0x17, 0x2A, 255}; // #0F172A Cosmic Navy Surface
        p.surface_elevated  = {0x17, 0x20, 0x33, 255}; // #172033 Elevated Slate-Indigo Surface
        p.surface_hover     = {0x1E, 0x29, 0x3B, 255}; // #1E293B Interactive Hover
        p.surface_selected  = {0x17, 0x25, 0x54, 255}; // #172554 Deep Indigo Selection
        p.border_subtle     = {0x1E, 0x29, 0x3B, 255}; // #1E293B Restrained 1px Divider
        p.border_strong     = {0x26, 0x33, 0x4D, 255}; // #26334D Control Border
        p.border_focus      = {0x38, 0xBD, 0xF8, 255}; // #38BDF8 Celestial Cyan Focus Ring
        p.text_primary      = {0xF8, 0xFA, 0xFC, 255}; // #F8FAFC Starlight Off-White
        p.text_secondary    = {0x94, 0xA3, 0xB8, 255}; // #94A3B8 Atmospheric Slate Metadata
        p.text_muted        = {0x64, 0x74, 0x8B, 255}; // #64748B Dimmed Nebula Slate
        p.accent_primary    = {0x25, 0x63, 0xEB, 255}; // #2563EB Restrained Cosmic Blue
        p.accent_hover      = {0x3B, 0x82, 0xF6, 255}; // #3B82F6 Luminous Blue Hover
        p.accent_cyan       = {0x38, 0xBD, 0xF8, 255}; // #38BDF8 Celestial Cyan Accent
        p.accent_indigo     = {0x63, 0x66, 0xF1, 255}; // #6366F1 Atmospheric Indigo Accent
        p.badge_stream      = {0x8B, 0x5C, 0xF6, 255}; // #8B5CF6 Violet Stream Badge
        p.badge_file        = {0x25, 0x63, 0xEB, 255}; // #2563EB Direct File Badge
        p.status_success    = {0x10, 0xB9, 0x81, 255}; // #10B981 Completed Emerald
        p.status_warning    = {0xF5, 0x9E, 0x0B, 255}; // #F59E0B Paused / Warning Amber
        p.status_danger     = {0xEF, 0x44, 0x44, 255}; // #EF4444 Failed / Error Crimson
        p.status_queued     = {0x63, 0x66, 0xF1, 255}; // #6366F1 Queued Indigo
        p.progress_track    = {0x13, 0x1C, 0x2E, 255}; // #131C2E Deep Space Progress Track
        p.star_bright       = {0xE2, 0xE8, 0xF0, 255}; // #E2E8F0 Bright Starlight Point
        p.star_cool         = {0x93, 0xC5, 0xFD, 255}; // #93C5FD Cool Cyan-Blue Star Point
        p.star_violet       = {0xC4, 0xB5, 0xFD, 255}; // #C4B5FD Soft Violet Star Point
        p.star_dim          = {0x47, 0x55, 0x69, 255}; // #475569 Distant Star Dust
        return p;
    }

    // Minimal Cosmic Dark UI policy: the entire application remains in dark mode.
    static ThemePalette GetLightPalette() {
        return GetDarkPalette();
    }

    static ThemePalette ResolvePalette(std::string_view /*theme_setting*/, bool /*os_prefers_light*/ = false) {
        return GetDarkPalette();
    }

    // Pre-computed 42-point 3-layer star catalog for negligible CPU/GPU overhead.
    // Never allocates random particles or runs heavy particle loops.
    static constexpr std::array<StarSeed, 42> GetStarCatalog() {
        return {{
            // Layer 0: Deep Space Dust (22 faint 1px points, period 240s)
            { 42,  68, 0, 1,  55, 0, false}, {118, 194, 0, 1,  65, 0, false},
            {186,  44, 0, 1,  50, 0, false}, {244, 312, 0, 1,  70, 1, false},
            {298, 142, 0, 1,  58, 0, false}, {356, 488, 0, 1,  62, 0, false},
            {412,  82, 0, 1,  52, 0, false}, {468, 264, 0, 1,  68, 1, false},
            {524, 618, 0, 1,  54, 0, false}, {578, 128, 0, 1,  64, 0, false},
            {634, 392, 0, 1,  50, 0, false}, {688, 742, 0, 1,  66, 2, false},
            {742, 216, 0, 1,  58, 0, false}, {796, 534, 0, 1,  60, 0, false},
            {848,  96, 0, 1,  52, 1, false}, {904, 668, 0, 1,  64, 0, false},
            {952, 348, 0, 1,  56, 0, false}, { 76, 812, 0, 1,  62, 0, false},
            {214, 724, 0, 1,  54, 0, false}, {382, 864, 0, 1,  68, 1, false},
            {612, 892, 0, 1,  56, 0, false}, {872, 838, 0, 1,  60, 0, false},

            // Layer 1: Midfield Stars (14 delicate 1-2px cool/violet points, period 160s)
            { 64, 152, 1, 1,  95, 1, false}, {154, 426, 1, 1, 105, 3, false},
            {232,  92, 1, 2,  92, 1, false}, {318, 574, 1, 1, 110, 2, false},
            {404, 228, 1, 1,  98, 1, false}, {492, 782, 1, 2,  90, 2, false},
            {566, 318, 1, 1, 108, 3, false}, {652, 114, 1, 1,  96, 1, false},
            {728, 642, 1, 2,  94, 1, false}, {814, 284, 1, 1, 112, 3, false},
            {886, 496, 1, 1, 100, 2, false}, {944, 168, 1, 1,  92, 1, false},
            {142, 884, 1, 1, 102, 1, false}, {764, 916, 1, 1,  96, 2, false},

            // Layer 2: Foreground Celestial Points (6 understated 2px points, period 110s)
            {128, 276, 2, 2, 145, 3, true }, {342, 136, 2, 2, 135, 1, false},
            {538, 464, 2, 2, 150, 3, true }, {712, 192, 2, 2, 130, 2, false},
            {862, 384, 2, 2, 142, 1, true }, {446, 692, 2, 2, 132, 3, false}
        }};
    }

    static CosmicBackgroundState ComputeCosmicBackground(
        int width,
        int height,
        uint32_t dpi = 96,
        int64_t elapsed_ms = 0,
        bool reduced_motion = false,
        bool window_hidden_or_minimized = false) {

        CosmicBackgroundState state;
        state.sample_time_ms = std::max<int64_t>(0, elapsed_ms);
        state.reduced_motion = reduced_motion;
        state.window_hidden_or_minimized = window_hidden_or_minimized;

        const int safe_w = std::max(320, width);
        const int safe_h = std::max(240, height);

        // Stop decorative animations immediately when hidden or minimized
        if (window_hidden_or_minimized) {
            state.mode = CosmicRenderMode::kSuspendedHidden;
            state.animation_active = false;
            return state;
        }

        const bool animate = !reduced_motion;
        state.mode = animate ? CosmicRenderMode::kAnimatedDrift : CosmicRenderMode::kStaticFallback;
        state.animation_active = animate;

        // 3 pre-defined parallax layers with long periods (240s, 160s, 110s)
        constexpr int kPeriodsMs[3] = {240000, 160000, 110000};
        constexpr double kDriftXScale[3] = {-0.35, -0.65, -1.00};
        constexpr double kDriftYScale[3] = {-0.12, -0.22, -0.35};

        state.layers.resize(3);
        for (uint8_t l = 0; l < 3; ++l) {
            CosmicLayerTransform lt;
            lt.layer_index = l;
            lt.period_ms = kPeriodsMs[l];
            if (animate && elapsed_ms > 0) {
                double phase = static_cast<double>(elapsed_ms % kPeriodsMs[l]) / static_cast<double>(kPeriodsMs[l]);
                lt.offset_x_px = phase * static_cast<double>(safe_w) * kDriftXScale[l];
                lt.offset_y_px = phase * static_cast<double>(safe_h) * kDriftYScale[l];
                // Subtle breathing modulation (+/- 10% opacity over full period)
                double wave = std::sin(phase * 6.283185307179586);
                lt.opacity_scale = static_cast<uint8_t>(std::clamp(230.0 + 25.0 * wave, 180.0, 255.0));
            } else {
                lt.offset_x_px = 0.0;
                lt.offset_y_px = 0.0;
                lt.opacity_scale = 235;
            }
            state.layers[l] = lt;
        }

        const auto pal = GetDarkPalette();
        const auto catalog = GetStarCatalog();
        state.stars.reserve(catalog.size());

        for (size_t idx = 0; idx < catalog.size(); ++idx) {
            const auto& seed = catalog[idx];
            const auto& lt = state.layers[seed.layer];

            double base_x = (static_cast<double>(seed.norm_x) / 1000.0) * static_cast<double>(safe_w);
            double base_y = (static_cast<double>(seed.norm_y) / 1000.0) * static_cast<double>(safe_h);

            int wrapped_x = static_cast<int>(std::fmod(base_x + lt.offset_x_px + safe_w * 4.0, static_cast<double>(safe_w)));
            int wrapped_y = static_cast<int>(std::fmod(base_y + lt.offset_y_px + safe_h * 4.0, static_cast<double>(safe_h)));

            uint32_t eff_opacity = (static_cast<uint32_t>(seed.base_opacity) * static_cast<uint32_t>(lt.opacity_scale)) / 255u;
            if (animate && elapsed_ms > 0 && seed.layer >= 1) {
                // Gentle per-star phase offset using deterministic index
                double star_phase = (static_cast<double>((elapsed_ms + static_cast<int64_t>(idx) * 7919) % 48000)) / 48000.0;
                double shimmer = 0.90 + 0.10 * std::cos(star_phase * 6.283185307179586);
                eff_opacity = static_cast<uint32_t>(std::clamp(eff_opacity * shimmer, 32.0, 175.0));
            }

            ColorRgba raw_tint = pal.star_dim;
            if (seed.tint == 1) raw_tint = pal.star_cool;
            else if (seed.tint == 2) raw_tint = pal.star_violet;
            else if (seed.tint == 3) raw_tint = pal.star_bright;

            CosmicStarPoint pt;
            pt.x_px = std::clamp(wrapped_x, 0, safe_w - 1);
            pt.y_px = std::clamp(wrapped_y, 0, safe_h - 1);
            pt.size_px = std::max(1, ScaleForDpi(seed.base_size_px, dpi));
            pt.opacity = static_cast<uint8_t>(eff_opacity);
            pt.layer = seed.layer;
            pt.has_celestial_cross = seed.cross_accent;
            pt.color = ColorRgba::BlendOver(pal.app_background, raw_tint, pt.opacity);
            state.stars.push_back(pt);
        }

        return state;
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
