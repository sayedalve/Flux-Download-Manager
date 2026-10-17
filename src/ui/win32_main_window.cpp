// SPDX-License-Identifier: GPL-3.0-or-later
#include "ui/win32_main_window.hpp"

#include "brisk_engine/http_client.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <shlobj.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")
#endif

namespace openidm::ui {

namespace {

std::string ToLower(std::string_view sv) {
    std::string s(sv);
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string FormatTimestampUtc(int64_t epoch_ms) {
    if (epoch_ms <= 0) return "—";
    std::time_t tt = static_cast<std::time_t>(epoch_ms / 1000);
    std::tm tm_buf{};
#ifdef _WIN32
    gmtime_s(&tm_buf, &tt);
#else
    gmtime_r(&tt, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S UTC");
    return oss.str();
}

std::string FormatConflictPolicyLabel(ConflictPolicy p) {
    switch (p) {
        case ConflictPolicy::kAutoRename:   return "Auto-Rename (file (1).ext)";
        case ConflictPolicy::kOverwrite:    return "Overwrite Existing File";
        case ConflictPolicy::kFailIfExists: return "Skip / Fail if File Exists";
    }
    return "Auto-Rename (file (1).ext)";
}

} // namespace

DesktopView::DesktopView(DownloadController& controller, ipc::IpcServer* ipc_server)
    : m_controller(controller),
      m_ipc_server(ipc_server),
      m_metrics(DesignSystem::ComputeMetrics(96)),
      m_palette(DesignSystem::GetDarkPalette()) {
    auto settings = m_controller.GetSettings();
    m_draft_settings = settings;
    m_starfield.SetReducedMotion(settings.reduced_motion);
    m_starfield.SetWindowState(!settings.start_minimized, settings.start_minimized);
    SetTheme("dark", false);
}

void DesktopView::SetWindowDimensions(int width, int height, uint32_t dpi) {
    m_metrics = DesignSystem::ComputeMetrics(dpi);
    m_window_width = std::max(width, m_metrics.min_window_width);
    m_window_height = std::max(height, m_metrics.min_window_height);
}

void DesktopView::SetWindowVisibility(bool visible, bool minimized) {
    m_starfield.SetWindowState(visible, minimized);
}

void DesktopView::AdvanceCosmicBackground(uint64_t delta_ms) {
    m_starfield.AdvanceTime(delta_ms);
}

std::vector<StarRenderInstance> DesktopView::BuildCosmicBackgroundStars() const {
    return m_starfield.BuildRenderStars({0, 0, m_window_width, m_window_height}, m_metrics.dpi, m_palette);
}

std::vector<std::string> DesktopView::GetVisibleColumns() const {
    if (IsCompactColumnsMode()) {
        return {"Filename", "Status", "Progress", "Size", "Speed", "Actions"};
    }
    return {"Filename", "Type", "Status", "Progress", "Size", "Speed", "ETA", "Actions"};
}

WindowLayoutGeometry DesktopView::ComputeLayoutGeometry(size_t scroll_offset) const {
    WindowLayoutGeometry g;
    const auto& m = m_metrics;
    int w = m_window_width;
    int h = m_window_height;

    int bar_h = DesignSystem::ScaleForDpi(48, m.dpi);
    g.toolbar_rect = {0, 0, w, bar_h};

    int chk_s = DesignSystem::ScaleForDpi(18, m.dpi);
    int chk_y = (bar_h - chk_s) / 2;
    g.select_all_checkbox_rect = {16, chk_y, 16 + chk_s, chk_y + chk_s};

    int sort_w = DesignSystem::ScaleForDpi(96, m.dpi);
    int sort_h = DesignSystem::ScaleForDpi(28, m.dpi);
    int sort_y = (bar_h - sort_h) / 2;
    g.sort_dropdown_rect = {16 + chk_s + 12, sort_y, 16 + chk_s + 12 + sort_w, sort_y + sort_h};

    int filt_w = DesignSystem::ScaleForDpi(96, m.dpi);
    int filt_x = g.sort_dropdown_rect.right + 10;
    g.filter_dropdown_rect = {filt_x, sort_y, filt_x + filt_w, sort_y + sort_h};

    int search_w = DesignSystem::ScaleForDpi(230, m.dpi);
    int add_btn_w = DesignSystem::ScaleForDpi(132, m.dpi);
    int btn_h = DesignSystem::ScaleForDpi(32, m.dpi);
    int by = (bar_h - btn_h) / 2;

    int search_x = w - search_w - 16;
    g.search_box_rect = {search_x, by, search_x + search_w, by + btn_h};
    g.search_sliders_rect = {g.search_box_rect.right - DesignSystem::ScaleForDpi(28, m.dpi), by, g.search_box_rect.right, by + btn_h};

    int add_btn_x = search_x - add_btn_w - 12;
    g.add_download_button_rect = {add_btn_x, by, add_btn_x + add_btn_w, by + btn_h};

    auto tb = ComputeToolbarState();
    struct BtnDef { ToolbarActionId id; const char* label; bool enabled; bool primary; int base_w; };
    const BtnDef defs[] = {
        {ToolbarActionId::kAddUrl,       "+ Add download", tb.can_add_url,       true,  130},
        {ToolbarActionId::kStart,        "Start",          tb.can_start,         false, 62},
        {ToolbarActionId::kPause,        "Pause",          tb.can_pause,         false, 62},
        {ToolbarActionId::kResume,       "Resume",         tb.can_resume,        false, 68},
        {ToolbarActionId::kRetry,        "Retry",          tb.can_retry,         false, 62},
        {ToolbarActionId::kCancel,       "Cancel",         tb.can_cancel,        false, 64},
        {ToolbarActionId::kRemove,       "Remove",         tb.can_remove,        false, 68},
        {ToolbarActionId::kClearHistory, "Clear History",  tb.can_clear_history, false, 96},
        {ToolbarActionId::kOpenFolder,   "Open Folder",    tb.can_open_folder,   false, 92},
        {ToolbarActionId::kSettings,     "Settings",       tb.can_open_settings, false, 72}
    };
    for (size_t i = 0; i < 10; ++i) {
        const auto& d = defs[i];
        RectI brc = (i == 0) ? g.add_download_button_rect : RectI{w + static_cast<int>(i * 50), 0, w + static_cast<int>(i * 50 + 40), 30};
        g.toolbar_buttons.push_back({d.id, d.label, d.enabled, d.primary, brc});
    }

    auto counts = ComputeCategoryCounts();
    const std::pair<SidebarCategory, size_t> cats[] = {
        {SidebarCategory::kAllDownloads,   counts.all_downloads},
        {SidebarCategory::kInProgress,     counts.in_progress},
        {SidebarCategory::kQueued,         counts.queued},
        {SidebarCategory::kCompleted,      counts.completed},
        {SidebarCategory::kFailed,         counts.failed},
        {SidebarCategory::kVideoDownloads, counts.video_downloads},
        {SidebarCategory::kScheduled,      counts.scheduled}
    };
    for (size_t i = 0; i < 7; ++i) {
        const auto& [cat, cnt] = cats[i];
        RectI crc = (i == 0) ? g.filter_dropdown_rect : RectI{filt_x, sort_y + static_cast<int>(i * 30), filt_x + filt_w, sort_y + static_cast<int>((i + 1) * 30)};
        g.sidebar_items.push_back({cat, CategoryToLabel(cat), cnt, cat == m_selected_category, crc});
    }
    g.sidebar_rect = {0, bar_h, 0, bar_h};

    int hdr_h = DesignSystem::ScaleForDpi(34, m.dpi);
    int sb_h = DesignSystem::ScaleForDpi(34, m.dpi);
    g.table_header_rect = {0, bar_h, w, bar_h + hdr_h};
    g.table_body_rect = {0, bar_h + hdr_h, w, h - sb_h};
    g.details_panel_rect = {0, h - sb_h, 0, h - sb_h};
    g.status_bar_rect = {0, h - sb_h, w, h};
    g.status_toggle_off_rect = {16, h - sb_h + (sb_h - 20) / 2, 54, h - sb_h + (sb_h + 20) / 2};

    int col_chk_w = 40;
    int col_size_w = DesignSystem::ScaleForDpi(90, m.dpi);
    int col_stat_w = DesignSystem::ScaleForDpi(160, m.dpi);
    int col_dl_w = DesignSystem::ScaleForDpi(120, m.dpi);
    int col_ul_w = DesignSystem::ScaleForDpi(90, m.dpi);
    int col_added_w = DesignSystem::ScaleForDpi(130, m.dpi);

    int remaining_w = std::max(100, w - col_chk_w - col_size_w - col_stat_w - col_dl_w - col_ul_w - col_added_w - 32);
    int col_name_w = remaining_w;

    int cx = 16;
    g.table_columns.push_back({SortColumn::kFileName, "Name", {cx, bar_h, cx + col_chk_w + col_name_w, bar_h + hdr_h}});
    cx += col_chk_w + col_name_w;

    g.table_columns.push_back({SortColumn::kSize, "Size", {cx, bar_h, cx + col_size_w, bar_h + hdr_h}});
    cx += col_size_w;

    g.table_columns.push_back({SortColumn::kStatus, "Status", {cx, bar_h, cx + col_stat_w, bar_h + hdr_h}});
    cx += col_stat_w;

    g.table_columns.push_back({SortColumn::kSpeed, "Download", {cx, bar_h, cx + col_dl_w, bar_h + hdr_h}});
    cx += col_dl_w;

    g.table_columns.push_back({SortColumn::kDownloadType, "Upload", {cx, bar_h, cx + col_ul_w, bar_h + hdr_h}});
    cx += col_ul_w;

    g.table_columns.push_back({SortColumn::kCreatedAt, "Added", {cx, bar_h, cx + col_added_w, bar_h + hdr_h}});
    cx += col_added_w;

    g.table_columns.push_back({SortColumn::kProgress, "Progress", {cx, bar_h, cx, bar_h + hdr_h}});
    g.table_columns.push_back({SortColumn::kEta, "ETA", {cx, bar_h, cx, bar_h + hdr_h}});

    int row_h = DesignSystem::ScaleForDpi(50, m.dpi);
    size_t max_rows = static_cast<size_t>(std::max(1, g.table_body_rect.height() / row_h));
    auto visible_rows = BuildVisibleTaskRows(scroll_offset, max_rows);
    int ry = g.table_body_rect.top;
    int dl_col_left = g.table_columns[3].rect.left;

    for (const auto& r : visible_rows) {
        if (ry + row_h > g.table_body_rect.bottom) break;
        RectI row_rc{0, ry, w, ry + row_h};
        RectI fn_rc{16 + col_chk_w, ry, 16 + col_chk_w + col_name_w, ry + row_h};
        RectI prog_rc{16 + col_chk_w + 22, ry + row_h - 14, 16 + col_chk_w + 160, ry + row_h - 12};
        RectI act_rc{dl_col_left, ry + (row_h - 18) / 2, dl_col_left + 18, ry + (row_h + 18) / 2};
        RectI chk_rc{16, ry + (row_h - 18) / 2, 34, ry + (row_h + 18) / 2};
        g.task_rows.push_back({r.task_id, row_rc, fn_rc, prog_rc, act_rc, chk_rc, act_rc});
        ry += row_h;
    }

    return g;
}

HitTestResult DesktopView::HitTestPoint(int x, int y, size_t scroll_offset) const {
    if (m_settings_center_open) {
        auto sv = BuildSettingsCenterView();
        if (sv.apply_button_rect.Contains(x, y)) {
            return {HitTargetType::kSettingsApplyButton, 0, "", "Apply and save modified settings"};
        }
        if (sv.cancel_button_rect.Contains(x, y)) {
            return {HitTargetType::kSettingsCancelButton, 0, "", "Discard unsaved changes and close Settings (Esc)"};
        }
        if (sv.restore_defaults_rect.Contains(x, y)) {
            return {HitTargetType::kSettingsRestoreDefaultsButton, 0, "", "Restore all settings to factory defaults"};
        }
        for (size_t i = 0; i < sv.categories.size(); ++i) {
            if (sv.categories[i].rect.Contains(x, y)) {
                return {HitTargetType::kSettingsCategoryTab, static_cast<int>(i), "", sv.categories[i].label};
            }
        }
        for (size_t i = 0; i < sv.visible_items.size(); ++i) {
            if (sv.visible_items[i].control_rect.Contains(x, y) || sv.visible_items[i].row_rect.Contains(x, y)) {
                return {HitTargetType::kSettingsItemControl, static_cast<int>(i), sv.visible_items[i].key, sv.visible_items[i].description};
            }
        }
    }

    auto geom = ComputeLayoutGeometry(scroll_offset);
    if (geom.search_sliders_rect.Contains(x, y)) {
        return {HitTargetType::kSearchSlidersButton, 0, "", "Settings Center (Ctrl+,)"};
    }
    if (geom.search_box_rect.Contains(x, y)) {
        return {HitTargetType::kSearchBox, 0, "", "Search downloads (Ctrl+K)"};
    }
    if (geom.add_download_button_rect.Contains(x, y)) {
        return {HitTargetType::kToolbarButton, 0, "", "+ Add download (Ctrl+N)"};
    }
    if (geom.select_all_checkbox_rect.Contains(x, y)) {
        return {HitTargetType::kSelectAllCheckbox, 0, "", "Select all / Deselect all"};
    }
    if (geom.sort_dropdown_rect.Contains(x, y)) {
        return {HitTargetType::kSortDropdown, 0, "", "Sort downloads"};
    }
    if (geom.filter_dropdown_rect.Contains(x, y)) {
        return {HitTargetType::kFilterDropdown, 0, "", "Filter by category"};
    }
    if (geom.status_toggle_off_rect.Contains(x, y)) {
        return {HitTargetType::kStatusToggleOff, 0, "", "Speed limiter toggle"};
    }
    for (size_t i = 0; i < geom.toolbar_buttons.size(); ++i) {
        if (geom.toolbar_buttons[i].rect.Contains(x, y)) {
            return {HitTargetType::kToolbarButton, static_cast<int>(i), "", geom.toolbar_buttons[i].label};
        }
    }
    for (size_t i = 0; i < geom.sidebar_items.size(); ++i) {
        if (geom.sidebar_items[i].rect.Contains(x, y)) {
            return {HitTargetType::kSidebarCategory, static_cast<int>(i), "", geom.sidebar_items[i].label};
        }
    }
    for (size_t i = 0; i < geom.table_columns.size(); ++i) {
        if (geom.table_columns[i].rect.Contains(x, y)) {
            return {HitTargetType::kTableHeaderColumn, static_cast<int>(i), "", "Sort by " + geom.table_columns[i].title};
        }
    }
    auto rows = BuildVisibleTaskRows(scroll_offset, geom.task_rows.size());
    for (size_t i = 0; i < geom.task_rows.size(); ++i) {
        if (geom.task_rows[i].checkbox_rect.Contains(x, y)) {
            return {HitTargetType::kRowCheckbox, static_cast<int>(i), geom.task_rows[i].task_id, "Select download"};
        }
        if (geom.task_rows[i].action_button_rect.Contains(x, y) || geom.task_rows[i].action_icon_rect.Contains(x, y)) {
            std::string act_lbl = (i < rows.size()) ? rows[i].primary_action_label : "Action";
            return {HitTargetType::kTaskRowActionButton, static_cast<int>(i), geom.task_rows[i].task_id, act_lbl};
        }
        if (geom.task_rows[i].row_rect.Contains(x, y)) {
            std::string tip;
            if (i < rows.size() && rows[i].is_filename_truncated && geom.task_rows[i].filename_rect.Contains(x, y)) {
                tip = rows[i].full_filename_tooltip;
            }
            return {HitTargetType::kTaskRow, static_cast<int>(i), geom.task_rows[i].task_id, tip};
        }
    }
    return {};
}

bool DesktopView::HandleMouseClick(int x, int y, bool double_click, size_t scroll_offset) {
    auto hit = HitTestPoint(x, y, scroll_offset);

    if (m_settings_center_open) {
        auto sv = BuildSettingsCenterView();
        switch (hit.type) {
            case HitTargetType::kSettingsCategoryTab: {
                if (hit.index >= 0 && static_cast<size_t>(hit.index) < sv.categories.size()) {
                    SelectSettingsCategory(sv.categories[static_cast<size_t>(hit.index)].category);
                    return true;
                }
                return false;
            }
            case HitTargetType::kSettingsItemControl: {
                if (!hit.task_id.empty()) {
                    CycleOrToggleDraftSetting(hit.task_id);
                    return true;
                }
                return false;
            }
            case HitTargetType::kSettingsApplyButton: {
                auto r = ApplySettingsChanges();
                if (r.ok()) {
                    CloseSettingsCenter(false);
                }
                return r.ok();
            }
            case HitTargetType::kSettingsCancelButton: {
                CloseSettingsCenter(true);
                return true;
            }
            case HitTargetType::kSettingsRestoreDefaultsButton: {
                ActionRestoreDefaultSettings();
                return true;
            }
            default:
                break;
        }
    }

    auto geom = ComputeLayoutGeometry(scroll_offset);
    switch (hit.type) {
        case HitTargetType::kSelectAllCheckbox: {
            ToggleSelectAll();
            return true;
        }
        case HitTargetType::kSortDropdown: {
            CycleSortDropdown();
            return true;
        }
        case HitTargetType::kFilterDropdown: {
            CycleFilterDropdown();
            return true;
        }
        case HitTargetType::kSearchSlidersButton: {
            OpenSettingsCenter(SettingsCategory::kGeneral);
            return true;
        }
        case HitTargetType::kStatusToggleOff: {
            ToggleSpeedLimiter();
            return true;
        }
        case HitTargetType::kRowCheckbox: {
            SelectTask(hit.task_id);
            return true;
        }
        case HitTargetType::kToolbarButton: {
            if (hit.index >= 0 && static_cast<size_t>(hit.index) < geom.toolbar_buttons.size()) {
                const auto& btn = geom.toolbar_buttons[static_cast<size_t>(hit.index)];
                if (!btn.enabled) return false;
                TriggerToolbarAction(btn.action_id);
                return true;
            }
            return false;
        }
        case HitTargetType::kSidebarCategory: {
            if (hit.index >= 0 && static_cast<size_t>(hit.index) < geom.sidebar_items.size()) {
                SelectCategory(geom.sidebar_items[static_cast<size_t>(hit.index)].category);
                return true;
            }
            return false;
        }
        case HitTargetType::kTableHeaderColumn: {
            if (hit.index >= 0 && static_cast<size_t>(hit.index) < geom.table_columns.size()) {
                ToggleSortColumn(geom.table_columns[static_cast<size_t>(hit.index)].sort_column);
                return true;
            }
            return false;
        }
        case HitTargetType::kTaskRowActionButton: {
            SelectTask(hit.task_id);
            TriggerRowPrimaryAction(hit.task_id);
            return true;
        }
        case HitTargetType::kTaskRow: {
            SelectTask(hit.task_id);
            if (double_click) {
                ActionOpenCompletedFile(true);
            }
            return true;
        }
        default:
            return false;
    }
}

HitTestResult DesktopView::HandleMouseMove(int x, int y, size_t scroll_offset) {
    auto hit = HitTestPoint(x, y, scroll_offset);
    m_hovered_toolbar_index = (hit.type == HitTargetType::kToolbarButton) ? hit.index : -1;
    m_hovered_sidebar_index = (hit.type == HitTargetType::kSidebarCategory) ? hit.index : -1;
    m_hovered_row_index = (hit.type == HitTargetType::kTaskRow || hit.type == HitTargetType::kTaskRowActionButton) ? hit.index : -1;
    m_active_tooltip = hit.tooltip_text;
    return hit;
}

void DesktopView::SetTheme(std::string_view theme_name, bool os_prefers_light) {
    (void)os_prefers_light;
    if (theme_name == "light" || theme_name == "system" || theme_name == "dark") {
        m_theme_name = std::string(theme_name);
    } else {
        m_theme_name = "dark";
    }
    // Keep the active desktop rendering surface in Minimal Cosmic Dark UI
    m_palette = DesignSystem::GetDarkPalette();
}

std::string DesktopView::FormatBytes(uint64_t bytes) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    if (bytes >= 1073741824ULL) {
        oss << (static_cast<double>(bytes) / 1073741824.0) << " GiB";
    } else if (bytes >= 1048576ULL) {
        oss << (static_cast<double>(bytes) / 1048576.0) << " MiB";
    } else if (bytes >= 1024ULL) {
        oss << (static_cast<double>(bytes) / 1024.0) << " KiB";
    } else {
        oss << bytes << " B";
    }
    return oss.str();
}

std::string DesktopView::FormatSpeed(uint64_t bps) {
    if (bps == 0) return "—";
    return FormatBytes(bps) + "/s";
}

std::string DesktopView::FormatBandwidthLimit(uint64_t bps) {
    if (bps == 0) return "Unlimited";
    return FormatBytes(bps) + "/s";
}

std::string DesktopView::FormatEta(uint64_t remaining_bytes, uint64_t speed_bps) {
    if (speed_bps == 0 || remaining_bytes == 0) return "—";
    uint64_t secs = remaining_bytes / speed_bps;
    if (secs > 359999ULL) return ">99h";
    uint64_t hrs = secs / 3600;
    uint64_t mins = (secs % 3600) / 60;
    uint64_t rem_s = secs % 60;
    char buf[32];
    if (hrs > 0) {
        std::snprintf(buf, sizeof(buf), "%02llu:%02llu:%02llu",
                      static_cast<unsigned long long>(hrs),
                      static_cast<unsigned long long>(mins),
                      static_cast<unsigned long long>(rem_s));
    } else {
        std::snprintf(buf, sizeof(buf), "%02llu:%02llu",
                      static_cast<unsigned long long>(mins),
                      static_cast<unsigned long long>(rem_s));
    }
    return std::string(buf);
}

std::string DesktopView::TruncateFilenameGracefully(std::string_view filename, size_t max_chars) {
    if (filename.size() <= max_chars || max_chars < 12) {
        return std::string(filename);
    }
    size_t tail_len = std::min<size_t>(10, (max_chars - 3) / 3);
    size_t head_len = max_chars - 3 - tail_len;
    return std::string(filename.substr(0, head_len)) + "..." +
           std::string(filename.substr(filename.size() - tail_len));
}

std::string DesktopView::FormatFriendlyTime(int64_t epoch_ms) {
    if (epoch_ms <= 0) return "—";
    std::time_t item_t = static_cast<std::time_t>(epoch_ms / 1000);
    std::time_t now_t = std::time(nullptr);
    std::tm item_tm{};
    std::tm now_tm{};
#ifdef _WIN32
    localtime_s(&item_tm, &item_t);
    localtime_s(&now_tm, &now_t);
#else
    localtime_r(&item_t, &item_tm);
    localtime_r(&now_t, &now_tm);
#endif
    int hour12 = item_tm.tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    const char* ampm = (item_tm.tm_hour >= 12) ? "PM" : "AM";
    char time_str[32];
    std::snprintf(time_str, sizeof(time_str), "%d:%02d %s", hour12, item_tm.tm_min, ampm);

    if (item_tm.tm_year == now_tm.tm_year && item_tm.tm_yday == now_tm.tm_yday) {
        return "Today " + std::string(time_str);
    }
    if (item_tm.tm_year == now_tm.tm_year && item_tm.tm_yday == now_tm.tm_yday - 1) {
        return "Yesterday " + std::string(time_str);
    }
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int m_idx = std::clamp(item_tm.tm_mon, 0, 11);
    char date_str[32];
    if (item_tm.tm_year == now_tm.tm_year) {
        std::snprintf(date_str, sizeof(date_str), "%s %d", months[m_idx], item_tm.tm_mday);
    } else {
        std::snprintf(date_str, sizeof(date_str), "%s %d, %d", months[m_idx], item_tm.tm_mday, 1900 + item_tm.tm_year);
    }
    return std::string(date_str);
}

std::string DesktopView::FormatSizePretty(uint64_t bytes) {
    if (bytes == 0) return "0 B";
    char buf[32];
    double b = static_cast<double>(bytes);
    if (bytes >= 1073741824ULL) {
        double gib = b / 1073741824.0;
        std::snprintf(buf, sizeof(buf), "%.1f GB", gib);
    } else if (bytes >= 1048576ULL) {
        double mib = b / 1048576.0;
        if (mib >= 10.0) {
            std::snprintf(buf, sizeof(buf), (mib == static_cast<int>(mib)) ? "%.0f MB" : "%.1f MB", mib);
        } else {
            std::snprintf(buf, sizeof(buf), "%.2f MB", mib);
        }
    } else if (bytes >= 1024ULL) {
        double kib = b / 1024.0;
        std::snprintf(buf, sizeof(buf), (kib >= 100.0) ? "%.0f KB" : "%.1f KB", kib);
    } else {
        std::snprintf(buf, sizeof(buf), "%llu B", static_cast<unsigned long long>(bytes));
    }
    return std::string(buf);
}

void DesktopView::CycleSortDropdown() {
    switch (m_sort_column) {
        case SortColumn::kCreatedAt:
            SetSort(SortColumn::kSize, false);
            break;
        case SortColumn::kSize:
            SetSort(SortColumn::kStatus, true);
            break;
        case SortColumn::kStatus:
            SetSort(SortColumn::kSpeed, false);
            break;
        case SortColumn::kSpeed:
            SetSort(SortColumn::kFileName, true);
            break;
        case SortColumn::kFileName:
        default:
            SetSort(SortColumn::kCreatedAt, false);
            break;
    }
}

void DesktopView::CycleFilterDropdown() {
    switch (m_selected_category) {
        case SidebarCategory::kAllDownloads:
            SelectCategory(SidebarCategory::kInProgress);
            break;
        case SidebarCategory::kInProgress:
            SelectCategory(SidebarCategory::kCompleted);
            break;
        case SidebarCategory::kCompleted:
            SelectCategory(SidebarCategory::kQueued);
            break;
        case SidebarCategory::kQueued:
            SelectCategory(SidebarCategory::kVideoDownloads);
            break;
        case SidebarCategory::kVideoDownloads:
        default:
            SelectCategory(SidebarCategory::kAllDownloads);
            break;
    }
}

void DesktopView::SelectCategory(SidebarCategory category) {
    m_selected_category = category;
    auto visible = GetFilteredAndSortedTasks();
    bool still_visible = false;
    for (const auto& t : visible) {
        if (t.task_id == m_selected_task_id) {
            still_visible = true;
            break;
        }
    }
    if (!still_visible) {
        m_selected_task_id = visible.empty() ? "" : visible.front().task_id;
    }
    m_last_feedback_message = "Filtered category: " + CategoryToLabel(category);
}

std::string DesktopView::CategoryToLabel(SidebarCategory category) {
    switch (category) {
        case SidebarCategory::kAllDownloads:   return "All Downloads";
        case SidebarCategory::kInProgress:     return "In Progress";
        case SidebarCategory::kQueued:         return "Queued";
        case SidebarCategory::kCompleted:      return "Completed";
        case SidebarCategory::kFailed:         return "Failed";
        case SidebarCategory::kVideoDownloads: return "Video Downloads";
        case SidebarCategory::kScheduled:      return "Scheduled";
    }
    return "All Downloads";
}

bool DesktopView::MatchesCategory(const TaskModel& t, SidebarCategory cat) const {
    switch (cat) {
        case SidebarCategory::kAllDownloads:
            return true;
        case SidebarCategory::kInProgress:
            return t.state == TaskState::kProbing || t.state == TaskState::kDownloading ||
                   t.state == TaskState::kMuxing || t.state == TaskState::kVerifying;
        case SidebarCategory::kQueued:
            return t.state == TaskState::kQueued || (t.state == TaskState::kPaused && !t.scheduled);
        case SidebarCategory::kCompleted:
            return t.state == TaskState::kCompleted;
        case SidebarCategory::kFailed:
            return t.state == TaskState::kFailed || t.state == TaskState::kCanceled;
        case SidebarCategory::kVideoDownloads:
            return t.task_type == TaskType::kStreamHls || t.task_type == TaskType::kStreamDash ||
                   t.task_type == TaskType::kStreamMss || t.category == "Video";
        case SidebarCategory::kScheduled:
            return t.scheduled;
    }
    return true;
}

bool DesktopView::MatchesSearch(const TaskModel& t) const {
    if (m_search_query.empty()) return true;
    std::string q = ToLower(m_search_query);
    return ToLower(t.file_name).find(q) != std::string::npos ||
           ToLower(t.url).find(q) != std::string::npos ||
           ToLower(t.page_url).find(q) != std::string::npos ||
           ToLower(t.category).find(q) != std::string::npos;
}

CategoryCounts DesktopView::ComputeCategoryCounts() const {
    CategoryCounts counts{};
    auto tasks = m_controller.ListTasks();
    for (const auto& t : tasks) {
        ++counts.all_downloads;
        if (MatchesCategory(t, SidebarCategory::kInProgress)) ++counts.in_progress;
        if (MatchesCategory(t, SidebarCategory::kQueued)) ++counts.queued;
        if (MatchesCategory(t, SidebarCategory::kCompleted)) ++counts.completed;
        if (MatchesCategory(t, SidebarCategory::kFailed)) ++counts.failed;
        if (MatchesCategory(t, SidebarCategory::kVideoDownloads)) ++counts.video_downloads;
        if (MatchesCategory(t, SidebarCategory::kScheduled)) ++counts.scheduled;
    }
    return counts;
}

void DesktopView::SetSearchQuery(std::string query) {
    m_search_query = std::move(query);
}

void DesktopView::SetSort(SortColumn column, bool ascending) {
    m_sort_column = column;
    m_sort_ascending = ascending;
}

void DesktopView::ToggleSortColumn(SortColumn column) {
    if (m_sort_column == column) {
        m_sort_ascending = !m_sort_ascending;
    } else {
        m_sort_column = column;
        m_sort_ascending = true;
    }
}

void DesktopView::SelectTask(const std::string& task_id) {
    m_selected_task_id = task_id;
}

void DesktopView::SelectNextRow() {
    auto list = GetFilteredAndSortedTasks();
    if (list.empty()) {
        m_selected_task_id.clear();
        return;
    }
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].task_id == m_selected_task_id) {
            size_t next = std::min(i + 1, list.size() - 1);
            m_selected_task_id = list[next].task_id;
            return;
        }
    }
    m_selected_task_id = list.front().task_id;
}

void DesktopView::SelectPrevRow() {
    auto list = GetFilteredAndSortedTasks();
    if (list.empty()) {
        m_selected_task_id.clear();
        return;
    }
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].task_id == m_selected_task_id) {
            size_t prev = (i > 0) ? (i - 1) : 0;
            m_selected_task_id = list[prev].task_id;
            return;
        }
    }
    m_selected_task_id = list.front().task_id;
}

void DesktopView::SelectFirstRow() {
    auto list = GetFilteredAndSortedTasks();
    m_selected_task_id = list.empty() ? "" : list.front().task_id;
}

void DesktopView::SelectLastRow() {
    auto list = GetFilteredAndSortedTasks();
    m_selected_task_id = list.empty() ? "" : list.back().task_id;
}

std::vector<TaskModel> DesktopView::GetFilteredAndSortedTasks() const {
    auto all = m_controller.ListTasks();
    std::vector<TaskModel> filtered;
    filtered.reserve(all.size());
    for (const auto& t : all) {
        if (MatchesCategory(t, m_selected_category) && MatchesSearch(t)) {
            filtered.push_back(t);
        }
    }

    auto compute_pct = [](const TaskModel& t) -> double {
        if (t.state == TaskState::kCompleted) return 100.0;
        if (t.total_bytes > 0) return (static_cast<double>(t.downloaded_bytes) * 100.0) / static_cast<double>(t.total_bytes);
        return t.stream_percent;
    };

    std::sort(filtered.begin(), filtered.end(), [&](const TaskModel& a, const TaskModel& b) {
        int cmp = 0;
        switch (m_sort_column) {
            case SortColumn::kFileName:
                cmp = ToLower(a.file_name).compare(ToLower(b.file_name));
                break;
            case SortColumn::kDownloadType:
                cmp = TaskTypeToString(a.task_type).compare(TaskTypeToString(b.task_type));
                break;
            case SortColumn::kStatus:
                cmp = TaskStateToString(a.state).compare(TaskStateToString(b.state));
                break;
            case SortColumn::kProgress: {
                double pa = compute_pct(a);
                double pb = compute_pct(b);
                cmp = (pa < pb) ? -1 : (pa > pb ? 1 : 0);
                break;
            }
            case SortColumn::kSize:
                cmp = (a.total_bytes < b.total_bytes) ? -1 : (a.total_bytes > b.total_bytes ? 1 : 0);
                break;
            case SortColumn::kSpeed:
                cmp = (a.speed_bps < b.speed_bps) ? -1 : (a.speed_bps > b.speed_bps ? 1 : 0);
                break;
            case SortColumn::kEta: {
                uint64_t ra = (a.total_bytes > a.downloaded_bytes) ? (a.total_bytes - a.downloaded_bytes) : 0;
                uint64_t rb = (b.total_bytes > b.downloaded_bytes) ? (b.total_bytes - b.downloaded_bytes) : 0;
                uint64_t ea = (a.speed_bps > 0) ? (ra / a.speed_bps) : UINT64_MAX;
                uint64_t eb = (b.speed_bps > 0) ? (rb / b.speed_bps) : UINT64_MAX;
                cmp = (ea < eb) ? -1 : (ea > eb ? 1 : 0);
                break;
            }
            case SortColumn::kCreatedAt:
            default:
                cmp = (a.created_at_ms < b.created_at_ms) ? -1 : (a.created_at_ms > b.created_at_ms ? 1 : 0);
                break;
        }
        return m_sort_ascending ? (cmp < 0) : (cmp > 0);
    });

    return filtered;
}

std::vector<FormattedTaskRow> DesktopView::BuildVisibleTaskRows(size_t scroll_offset, size_t max_visible_rows) const {
    auto list = GetFilteredAndSortedTasks();
    std::vector<FormattedTaskRow> rows;
    if (scroll_offset >= list.size()) return rows;

    size_t end_idx = std::min(list.size(), scroll_offset + max_visible_rows);
    rows.reserve(end_idx - scroll_offset);

    size_t max_fn_chars = IsCompactColumnsMode() ? 28 : 40;

    for (size_t i = scroll_offset; i < end_idx; ++i) {
        const auto& t = list[i];
        FormattedTaskRow row;
        row.task_id = t.task_id;
        row.state = t.state;
        row.category = t.category;
        row.full_filename_tooltip = t.file_name;
        row.display_filename = TruncateFilenameGracefully(t.file_name, max_fn_chars);
        row.is_filename_truncated = (row.display_filename != t.file_name);
        row.is_stream = (t.task_type != TaskType::kHttpFile);

        if (t.task_type == TaskType::kStreamHls) row.file_type_badge = "HLS STREAM";
        else if (t.task_type == TaskType::kStreamDash) row.file_type_badge = "DASH STREAM";
        else row.file_type_badge = t.range_supported ? "HTTP MULTI" : "HTTP FILE";

        if (!t.status_detail.empty()) {
            row.status_text = t.status_detail;
        } else {
            switch (t.state) {
                case TaskState::kQueued:      row.status_text = "Queued"; break;
                case TaskState::kProbing:     row.status_text = row.is_stream ? "Probing manifest..." : "Probing server..."; break;
                case TaskState::kDownloading: row.status_text = row.is_stream ? "Downloading video segments..." : "Downloading..."; break;
                case TaskState::kMuxing:      row.status_text = "Merging tracks (FFmpeg)..."; break;
                case TaskState::kVerifying:   row.status_text = "Verifying output..."; break;
                case TaskState::kPaused:      row.status_text = t.scheduled ? ("Scheduled (" + t.schedule_time_local + ")") : "Paused"; break;
                case TaskState::kCompleted:   row.status_text = "Completed"; break;
                case TaskState::kFailed:      row.status_text = "Failed (" + (t.error_code.empty() ? "Error" : t.error_code) + ")"; break;
                case TaskState::kCanceled:    row.status_text = "Canceled"; break;
            }
        }

        // Semantic status indicator & progress fill colors
        switch (t.state) {
            case TaskState::kCompleted:
                row.status_indicator_color = m_palette.status_success;
                row.progress_fill_color = m_palette.status_success;
                break;
            case TaskState::kFailed:
            case TaskState::kCanceled:
                row.status_indicator_color = m_palette.status_danger;
                row.progress_fill_color = m_palette.status_danger;
                break;
            case TaskState::kPaused:
                row.status_indicator_color = m_palette.status_warning;
                row.progress_fill_color = m_palette.status_warning;
                break;
            case TaskState::kProbing:
            case TaskState::kMuxing:
            case TaskState::kVerifying:
                row.status_indicator_color = m_palette.accent_violet;
                row.progress_fill_color = row.is_stream ? m_palette.badge_stream : m_palette.accent_cyan;
                break;
            case TaskState::kDownloading:
                row.status_indicator_color = row.is_stream ? m_palette.badge_stream : m_palette.accent_cyan;
                row.progress_fill_color = row.is_stream ? m_palette.badge_stream : m_palette.accent_primary;
                break;
            case TaskState::kQueued:
            default:
                row.status_indicator_color = m_palette.text_secondary;
                row.progress_fill_color = row.is_stream ? m_palette.badge_stream : m_palette.accent_primary;
                break;
        }

        std::ostringstream prog_ss;
        if (t.state == TaskState::kCompleted) {
            row.progress_fraction = 1.0;
            row.progress_text = "100.0%";
        } else if (t.total_bytes > 0) {
            double pct = std::clamp((static_cast<double>(t.downloaded_bytes) * 100.0) / static_cast<double>(t.total_bytes), 0.0, 100.0);
            row.progress_fraction = pct / 100.0;
            prog_ss << std::fixed << std::setprecision(1) << pct << "%";
            row.progress_text = prog_ss.str();
        } else if (row.is_stream && t.total_segments > 0) {
            double pct = std::clamp((static_cast<double>(t.completed_segments) * 100.0) / static_cast<double>(t.total_segments), 0.0, 100.0);
            row.progress_fraction = pct / 100.0;
            prog_ss << std::fixed << std::setprecision(1) << pct << "% (" << t.completed_segments << "/" << t.total_segments << " segs)";
            row.progress_text = prog_ss.str();
        } else {
            row.progress_fraction = 0.0;
            row.progress_indeterminate = (t.state == TaskState::kDownloading || t.state == TaskState::kProbing);
            row.progress_text = FormatBytes(t.downloaded_bytes) + " (Size Unknown)";
        }

        uint64_t effective_size = (t.total_bytes > 0) ? t.total_bytes : t.downloaded_bytes;
        row.size_text = FormatSizePretty(effective_size);
        row.added_time_text = FormatFriendlyTime(t.created_at_ms);
        row.upload_speed_text = "0 B/s";
        row.checked = m_select_all_checked;

        if (t.state == TaskState::kDownloading) {
            double pct = (t.total_bytes > 0)
                ? std::clamp((static_cast<double>(t.downloaded_bytes) * 100.0) / static_cast<double>(t.total_bytes), 0.0, 100.0)
                : 0.0;
            int ipct = static_cast<int>(pct);
            row.status_text = "Downloading " + std::to_string(ipct) + "%";
            row.status_indicator_color = {0xFF, 0x47, 0x57, 255};
        } else if (t.state == TaskState::kPaused) {
            double pct = (t.total_bytes > 0)
                ? std::clamp((static_cast<double>(t.downloaded_bytes) * 100.0) / static_cast<double>(t.total_bytes), 0.0, 100.0)
                : 0.0;
            int ipct = static_cast<int>(pct);
            row.status_text = (ipct > 0) ? ("Paused " + std::to_string(ipct) + "%") : "Paused";
            row.status_indicator_color = {0x9E, 0x9E, 0xA8, 255};
        } else if (t.state == TaskState::kCompleted) {
            row.status_text = "Completed";
            row.status_indicator_color = {0x8E, 0x8E, 0x98, 255};
        } else if (t.state == TaskState::kFailed) {
            row.status_text = "Failed";
            row.status_indicator_color = {0xEF, 0x44, 0x44, 255};
        }

        row.speed_text = FormatSpeed(t.speed_bps);
        uint64_t rem_bytes = (t.total_bytes > t.downloaded_bytes) ? (t.total_bytes - t.downloaded_bytes) : 0;
        row.eta_text = (t.state == TaskState::kDownloading) ? FormatEta(rem_bytes, t.speed_bps) : "—";

        switch (t.state) {
            case TaskState::kDownloading:
            case TaskState::kProbing:
                row.primary_action_label = "Pause";
                break;
            case TaskState::kPaused:
                row.primary_action_label = "Resume";
                break;
            case TaskState::kQueued:
                row.primary_action_label = "Pause";
                break;
            case TaskState::kFailed:
            case TaskState::kCanceled:
                row.primary_action_label = "Retry";
                break;
            case TaskState::kCompleted:
                row.primary_action_label = "Open";
                break;
            default:
                row.primary_action_label = "Wait";
                break;
        }
        row.selected = (t.task_id == m_selected_task_id);
        rows.push_back(std::move(row));
    }
    return rows;
}

ToolbarState DesktopView::ComputeToolbarState() const {
    ToolbarState st;
    auto all = m_controller.ListTasks();
    for (const auto& t : all) {
        if (t.state == TaskState::kCompleted || t.state == TaskState::kCanceled) {
            st.can_clear_history = true;
            break;
        }
    }

    if (m_selected_task_id.empty()) return st;
    auto sel = m_controller.GetTask(m_selected_task_id);
    if (!sel.has_value()) return st;

    const auto& t = *sel;
    st.can_remove = true;
    st.can_open_folder = !t.save_dir.empty();

    switch (t.state) {
        case TaskState::kQueued:
            st.can_pause = true;
            st.can_cancel = true;
            break;
        case TaskState::kProbing:
        case TaskState::kDownloading:
        case TaskState::kMuxing:
            st.can_pause = true;
            st.can_cancel = true;
            break;
        case TaskState::kPaused:
            st.can_start = true;
            st.can_resume = true;
            st.can_cancel = true;
            break;
        case TaskState::kFailed:
        case TaskState::kCanceled:
            st.can_start = true;
            st.can_retry = true;
            break;
        case TaskState::kCompleted:
            st.can_open_file = true;
            st.can_delete_file = true;
            break;
        default:
            break;
    }
    return st;
}

TaskDetailsViewModel DesktopView::BuildSelectedTaskDetails() const {
    TaskDetailsViewModel vm;
    if (m_selected_task_id.empty()) return vm;
    auto sel = m_controller.GetTask(m_selected_task_id);
    if (!sel.has_value()) return vm;

    const auto& t = *sel;
    vm.has_selection = true;
    vm.task_id = t.task_id;
    vm.file_name = t.file_name;
    vm.redacted_source_url = Logger::RedactSensitiveData(t.url);
    vm.redacted_page_url = t.page_url.empty() ? "—" : Logger::RedactSensitiveData(t.page_url);
    vm.destination_path = (std::filesystem::path(t.save_dir) / t.file_name).string();
    vm.size_summary = (t.total_bytes > 0)
        ? (FormatBytes(t.downloaded_bytes) + " of " + FormatBytes(t.total_bytes))
        : (FormatBytes(t.downloaded_bytes) + " (Total size unknown)");
    vm.progress_summary = t.status_detail.empty() ? TaskStateToString(t.state) : t.status_detail;
    vm.speed_summary = FormatSpeed(t.speed_bps);
    vm.created_time_str = FormatTimestampUtc(t.created_at_ms);
    vm.updated_time_str = FormatTimestampUtc(t.updated_at_ms);
    vm.completed_time_str = FormatTimestampUtc(t.completed_at_ms);
    vm.retry_count = t.retry_count;

    if (t.task_type == TaskType::kHttpFile) {
        vm.engine_name = "Brisk Segmented HTTP/1.1 Engine (" + std::to_string(t.max_connections) + " max connections)";
        vm.stream_format = t.range_supported ? "HTTP/1.1 Byte-Range (206 Partial Content)" : "HTTP/1.1 Single-Stream (200 OK)";
        vm.selected_tracks_summary = "N/A (Direct File Transfer — " + std::to_string(t.segments.size()) + " segments)";
    } else {
        vm.engine_name = "N_m3u8DL-RE + FFmpeg Managed Stream Pipeline";
        vm.stream_format = (t.task_type == TaskType::kStreamDash) ? "MPEG-DASH (.mpd)" : "Apple HLS (.m3u8)";
        vm.selected_tracks_summary = "Video: " + t.stream_options.video_filter +
                                     " | Audio: " + t.stream_options.audio_filter +
                                     " | Subtitles: " + t.stream_options.subtitle_filter +
                                     " | Container: ." + t.stream_options.output_format;
    }

    std::ostringstream hdr_ss;
    for (const auto& [k, v] : t.headers) {
        hdr_ss << k << ": " << Logger::RedactSensitiveData(v) << "; ";
    }
    if (!t.auth_headers.empty()) {
        hdr_ss << "Auth/Cookies: [" << t.auth_headers.size() << " protected credential header(s) — REDACTED]";
    }
    vm.redacted_headers_summary = hdr_ss.str().empty() ? "Default headers" : hdr_ss.str();

    vm.error_code = t.error_code;
    if (t.error_code.empty()) {
        vm.error_explanation = "No errors.";
    } else if (t.error_code == "ERR_HTTP_403_EXPIRED_OR_AUTH") {
        vm.error_explanation = t.error_message + " — The server rejected the link or the signed token expired. Use 'Refresh URL' or re-capture from your browser.";
    } else if (t.error_code == "ERR_STREAM_DRM_PROTECTED") {
        vm.error_explanation = t.error_message + " — Encrypted with commercial DRM (Widevine/PlayReady/FairPlay), which Flux does not circumvent.";
    } else if (t.error_code == "ERR_DISK_FULL") {
        vm.error_explanation = t.error_message + " — Free up disk space in the destination folder and click Resume or Retry.";
    } else {
        vm.error_explanation = t.error_message;
    }
    return vm;
}

StatusBarViewModel DesktopView::BuildStatusBar() const {
    StatusBarViewModel sb;
    auto tasks = m_controller.ListTasks();
    for (const auto& t : tasks) {
        if (t.state == TaskState::kProbing || t.state == TaskState::kDownloading ||
            t.state == TaskState::kMuxing || t.state == TaskState::kVerifying) {
            ++sb.active_downloads;
            sb.total_speed_bps += t.speed_bps;
        } else if (t.state == TaskState::kQueued) {
            ++sb.remaining_queued;
        }
    }
    sb.formatted_total_speed = FormatSpeed(sb.total_speed_bps);
    sb.browser_ipc_listening = (m_ipc_server != nullptr);
    sb.browser_capture_enabled = m_controller.GetSettings().browser_capture_enabled;
    sb.workers_ready = m_controller.AreWorkersAvailable();

    std::ostringstream msg;
    msg << "Active: " << sb.active_downloads
        << " | Speed: " << sb.formatted_total_speed
        << " | Queued: " << sb.remaining_queued
        << " | Browser Capture: " << (sb.browser_capture_enabled ? "Enabled" : "Paused")
        << " | Stream Workers: " << (sb.workers_ready ? "Ready" : "Missing");
    sb.status_message = msg.str();
    return sb;
}

std::string DesktopView::GetEmptyStateMessage() const {
    auto all = m_controller.ListTasks();
    if (all.empty()) {
        return "No downloads yet. Click '+ Add URL' (Ctrl+N) or use the Flux Browser Extension to capture files and video streams.";
    }
    if (!m_search_query.empty()) {
        return "No downloads match the search filter \"" + m_search_query + "\". Clear the search box to view all tasks.";
    }
    switch (m_selected_category) {
        case SidebarCategory::kInProgress:
            return "No active transfers right now. Select a paused task and click Resume, or add a new URL.";
        case SidebarCategory::kQueued:
            return "No queued downloads waiting to start.";
        case SidebarCategory::kCompleted:
            return "No completed downloads in history yet.";
        case SidebarCategory::kFailed:
            return "No failed downloads. All transfers are healthy.";
        case SidebarCategory::kVideoDownloads:
            return "No video or HLS/DASH stream downloads yet. Open a video page with the Flux Browser Extension enabled.";
        case SidebarCategory::kScheduled:
            return "No scheduled tasks configured.";
        default:
            return "No tasks in this view.";
    }
}

// -----------------------------------------------------------------------------
// Professional Download Manager Settings Center Implementation
// -----------------------------------------------------------------------------

void DesktopView::OpenSettingsCenter(SettingsCategory initial_category) {
    m_settings_center_open = true;
    m_settings_category = initial_category;
    m_draft_settings = m_controller.GetSettings();
    m_settings_validation_key.clear();
    m_settings_validation_error.clear();
    m_last_feedback_message = "Opened Settings Dialog";
}

void DesktopView::CloseSettingsCenter(bool revert_unsaved) {
    if (revert_unsaved) {
        RevertSettingsChanges();
    }
    m_settings_center_open = false;
    m_settings_search_query.clear();
}

void DesktopView::SelectSettingsCategory(SettingsCategory category) {
    m_settings_category = category;
    m_last_feedback_message = "Settings category: " + SettingsCategoryToLabel(category);
}

std::string DesktopView::SettingsCategoryToLabel(SettingsCategory category) {
    switch (category) {
        case SettingsCategory::kGeneral:             return "General";
        case SettingsCategory::kDownloads:           return "Downloads & Queue";
        case SettingsCategory::kSaveLocations:       return "Save Locations";
        case SettingsCategory::kConnectionsAndSpeed: return "Connections & Speed";
        case SettingsCategory::kBrowserIntegration:  return "Browser Integration";
        case SettingsCategory::kFileTypes:           return "File Types";
        case SettingsCategory::kNotifications:       return "Notifications";
        case SettingsCategory::kAboutAndDiagnostics: return "About & Diagnostics";
    }
    return "General";
}

void DesktopView::SetSettingsSearchQuery(std::string query) {
    m_settings_search_query = std::move(query);
}

void DesktopView::SetDraftSettings(const AppSettings& draft) {
    m_draft_settings = draft;
    auto v = ValidateSettingsInput(m_draft_settings);
    if (!v.ok()) {
        m_settings_validation_error = v.error->message;
    } else {
        m_settings_validation_key.clear();
        m_settings_validation_error.clear();
    }
}

bool DesktopView::HasUnsavedSettingsChanges() const {
    return m_draft_settings != m_controller.GetSettings();
}

Result<void> DesktopView::ValidateSettingsInput(const AppSettings& candidate) const {
    if (candidate.max_concurrent_tasks < 1 || candidate.max_concurrent_tasks > 16) {
        return Result<void>::Err("ERR_SETTINGS_CONCURRENT_RANGE", "Simultaneous downloads must be between 1 and 16.");
    }
    if (candidate.default_connections_per_task < 1 || candidate.default_connections_per_task > 32) {
        return Result<void>::Err("ERR_SETTINGS_CONNECTIONS_RANGE", "Default connections per task must be between 1 and 32.");
    }
    if (candidate.default_download_dir.empty()) {
        return Result<void>::Err("ERR_SETTINGS_EMPTY_SAVE_DIR", "Default download directory cannot be empty.");
    }
    for (char c : candidate.default_download_dir) {
        if (static_cast<unsigned char>(c) < 0x20) {
            return Result<void>::Err("ERR_SETTINGS_INVALID_SAVE_DIR", "Default download directory contains invalid control characters.");
        }
    }
    if (candidate.stall_timeout_ms < 500 || candidate.stall_timeout_ms > 60000) {
        return Result<void>::Err("ERR_SETTINGS_STALL_TIMEOUT", "Segment stall timeout must be between 500 ms and 60,000 ms.");
    }
    if (candidate.min_split_bytes < 65536ULL || candidate.min_split_bytes > 10485760ULL) {
        return Result<void>::Err("ERR_SETTINGS_MIN_SPLIT", "Minimum segment split size must be between 64 KiB and 10 MiB.");
    }
    if (candidate.min_capture_size_bytes > 10737418240ULL) {
        return Result<void>::Err("ERR_SETTINGS_CAPTURE_SIZE", "Minimum browser capture size cannot exceed 10 GiB.");
    }
    return Result<void>::Ok();
}

Result<void> DesktopView::SetDraftSettingByKey(std::string_view key, std::string_view value_str) {
    auto parse_u64 = [](std::string_view sv, uint64_t& out) -> bool {
        if (sv.empty()) return false;
        std::string s(sv);
        char* endp = nullptr;
        long long v = std::strtoll(s.c_str(), &endp, 10);
        if (endp == s.c_str() || *endp != '\0' || v < 0) return false;
        out = static_cast<uint64_t>(v);
        return true;
    };
    auto parse_bool = [](std::string_view sv) -> bool {
        std::string l = ToLower(sv);
        return l == "true" || l == "1" || l == "yes" || l == "on" || l == "enabled";
    };

    m_settings_validation_key.clear();
    m_settings_validation_error.clear();

    if (key == "start_minimized") {
        m_draft_settings.start_minimized = parse_bool(value_str);
    } else if (key == "close_to_tray") {
        m_draft_settings.close_to_tray = parse_bool(value_str);
    } else if (key == "reduced_motion") {
        m_draft_settings.reduced_motion = parse_bool(value_str);
    } else if (key == "max_concurrent_tasks") {
        uint64_t v = 0;
        if (!parse_u64(value_str, v) || v < 1 || v > 16) {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Simultaneous downloads must be an integer between 1 and 16.";
            return Result<void>::Err("ERR_SETTINGS_CONCURRENT_RANGE", m_settings_validation_error);
        }
        m_draft_settings.max_concurrent_tasks = static_cast<uint32_t>(v);
    } else if (key == "auto_start_downloads") {
        m_draft_settings.auto_start_downloads = parse_bool(value_str);
    } else if (key == "conflict_policy") {
        if (value_str != "auto_rename" && value_str != "overwrite" && value_str != "fail_if_exists") {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Conflict policy must be auto_rename, overwrite, or fail_if_exists.";
            return Result<void>::Err("ERR_SETTINGS_CONFLICT_POLICY", m_settings_validation_error);
        }
        m_draft_settings.conflict_policy = ConflictPolicyFromString(std::string(value_str));
    } else if (key == "default_download_dir") {
        if (value_str.empty()) {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Default download directory cannot be empty.";
            return Result<void>::Err("ERR_SETTINGS_EMPTY_SAVE_DIR", m_settings_validation_error);
        }
        for (char c : value_str) {
            if (static_cast<unsigned char>(c) < 0x20) {
                m_settings_validation_key = std::string(key);
                m_settings_validation_error = "Default download directory contains invalid control characters.";
                return Result<void>::Err("ERR_SETTINGS_INVALID_SAVE_DIR", m_settings_validation_error);
            }
        }
        m_draft_settings.default_download_dir = std::string(value_str);
    } else if (key == "category_subfolders_enabled") {
        m_draft_settings.category_subfolders_enabled = parse_bool(value_str);
    } else if (key == "default_connections_per_task") {
        uint64_t v = 0;
        if (!parse_u64(value_str, v) || v < 1 || v > 32) {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Default connections per task must be between 1 and 32.";
            return Result<void>::Err("ERR_SETTINGS_CONNECTIONS_RANGE", m_settings_validation_error);
        }
        m_draft_settings.default_connections_per_task = static_cast<uint32_t>(v);
    } else if (key == "speed_limit_bps") {
        uint64_t v = 0;
        if (!parse_u64(value_str, v)) {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Global speed limit must be a non-negative byte/s rate (0 = Unlimited).";
            return Result<void>::Err("ERR_SETTINGS_SPEED_LIMIT", m_settings_validation_error);
        }
        m_draft_settings.speed_limit_bps = v;
    } else if (key == "stall_timeout_ms") {
        uint64_t v = 0;
        if (!parse_u64(value_str, v) || v < 500 || v > 60000) {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Segment stall timeout must be between 500 ms and 60,000 ms.";
            return Result<void>::Err("ERR_SETTINGS_STALL_TIMEOUT", m_settings_validation_error);
        }
        m_draft_settings.stall_timeout_ms = static_cast<int>(v);
    } else if (key == "min_split_bytes") {
        uint64_t v = 0;
        if (!parse_u64(value_str, v) || v < 65536ULL || v > 10485760ULL) {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Minimum segment split size must be between 65,536 B (64 KiB) and 10,485,760 B (10 MiB).";
            return Result<void>::Err("ERR_SETTINGS_MIN_SPLIT", m_settings_validation_error);
        }
        m_draft_settings.min_split_bytes = v;
    } else if (key == "browser_capture_enabled") {
        m_draft_settings.browser_capture_enabled = parse_bool(value_str);
    } else if (key == "min_capture_size_bytes") {
        uint64_t v = 0;
        if (!parse_u64(value_str, v) || v > 10737418240ULL) {
            m_settings_validation_key = std::string(key);
            m_settings_validation_error = "Minimum automatic capture size must be between 0 B and 10 GiB.";
            return Result<void>::Err("ERR_SETTINGS_CAPTURE_SIZE", m_settings_validation_error);
        }
        m_draft_settings.min_capture_size_bytes = v;
    } else if (key == "show_completion_notification") {
        m_draft_settings.show_completion_notification = parse_bool(value_str);
    } else {
        return Result<void>::Err("ERR_SETTINGS_UNKNOWN_KEY", "Unknown setting key: " + std::string(key));
    }

    return Result<void>::Ok();
}

Result<void> DesktopView::CycleOrToggleDraftSetting(std::string_view key) {
    if (key == "start_minimized") {
        m_draft_settings.start_minimized = !m_draft_settings.start_minimized;
    } else if (key == "close_to_tray") {
        m_draft_settings.close_to_tray = !m_draft_settings.close_to_tray;
    } else if (key == "reduced_motion") {
        m_draft_settings.reduced_motion = !m_draft_settings.reduced_motion;
    } else if (key == "max_concurrent_tasks") {
        uint32_t next = (m_draft_settings.max_concurrent_tasks >= 16) ? 1 : (m_draft_settings.max_concurrent_tasks + 1);
        m_draft_settings.max_concurrent_tasks = next;
    } else if (key == "auto_start_downloads") {
        m_draft_settings.auto_start_downloads = !m_draft_settings.auto_start_downloads;
    } else if (key == "conflict_policy") {
        if (m_draft_settings.conflict_policy == ConflictPolicy::kAutoRename) {
            m_draft_settings.conflict_policy = ConflictPolicy::kOverwrite;
        } else if (m_draft_settings.conflict_policy == ConflictPolicy::kOverwrite) {
            m_draft_settings.conflict_policy = ConflictPolicy::kFailIfExists;
        } else {
            m_draft_settings.conflict_policy = ConflictPolicy::kAutoRename;
        }
    } else if (key == "category_subfolders_enabled") {
        m_draft_settings.category_subfolders_enabled = !m_draft_settings.category_subfolders_enabled;
    } else if (key == "default_connections_per_task") {
        const uint32_t presets[] = {1, 2, 4, 8, 16, 24, 32};
        uint32_t next = 8;
        for (size_t i = 0; i < 7; ++i) {
            if (m_draft_settings.default_connections_per_task == presets[i]) {
                next = presets[(i + 1) % 7];
                break;
            }
        }
        m_draft_settings.default_connections_per_task = next;
    } else if (key == "speed_limit_bps") {
        const uint64_t presets[] = {0ULL, 1048576ULL, 5242880ULL, 10485760ULL, 26214400ULL, 52428800ULL};
        uint64_t next = 0ULL;
        for (size_t i = 0; i < 6; ++i) {
            if (m_draft_settings.speed_limit_bps == presets[i]) {
                next = presets[(i + 1) % 6];
                break;
            }
        }
        m_draft_settings.speed_limit_bps = next;
    } else if (key == "stall_timeout_ms") {
        const int presets[] = {1500, 3000, 5000, 10000, 15000};
        int next = 3000;
        for (size_t i = 0; i < 5; ++i) {
            if (m_draft_settings.stall_timeout_ms == presets[i]) {
                next = presets[(i + 1) % 5];
                break;
            }
        }
        m_draft_settings.stall_timeout_ms = next;
    } else if (key == "min_split_bytes") {
        const uint64_t presets[] = {65536ULL, 131072ULL, 262144ULL, 524288ULL, 1048576ULL};
        uint64_t next = 131072ULL;
        for (size_t i = 0; i < 5; ++i) {
            if (m_draft_settings.min_split_bytes == presets[i]) {
                next = presets[(i + 1) % 5];
                break;
            }
        }
        m_draft_settings.min_split_bytes = next;
    } else if (key == "browser_capture_enabled") {
        m_draft_settings.browser_capture_enabled = !m_draft_settings.browser_capture_enabled;
    } else if (key == "min_capture_size_bytes") {
        const uint64_t presets[] = {0ULL, 262144ULL, 524288ULL, 1048576ULL, 5242880ULL, 10485760ULL};
        uint64_t next = 1048576ULL;
        for (size_t i = 0; i < 6; ++i) {
            if (m_draft_settings.min_capture_size_bytes == presets[i]) {
                next = presets[(i + 1) % 6];
                break;
            }
        }
        m_draft_settings.min_capture_size_bytes = next;
    } else if (key == "show_completion_notification") {
        m_draft_settings.show_completion_notification = !m_draft_settings.show_completion_notification;
    }
    m_settings_validation_key.clear();
    m_settings_validation_error.clear();
    return Result<void>::Ok();
}

Result<void> DesktopView::ApplySettingsChanges() {
    if (!m_settings_validation_error.empty()) {
        return Result<void>::Err("ERR_SETTINGS_INVALID", m_settings_validation_error);
    }
    auto val = ValidateSettingsInput(m_draft_settings);
    if (!val.ok()) {
        m_settings_validation_error = val.error->message;
        return val;
    }
    return ActionSaveSettings(m_draft_settings);
}

void DesktopView::RevertSettingsChanges() {
    m_draft_settings = m_controller.GetSettings();
    m_settings_validation_key.clear();
    m_settings_validation_error.clear();
    m_last_feedback_message = "Reverted unsaved settings changes";
}

std::vector<SettingItemView> DesktopView::BuildAllSettingItems() const {
    const auto saved = m_controller.GetSettings();
    const auto& s = m_draft_settings;
    std::string def_dir = PathSanitizer::GetDefaultDownloadsDir(PathSanitizer::GetExecutableDir()).string();

    std::vector<SettingItemView> items;
    auto add_item = [&](
        std::string key,
        SettingsCategory cat,
        std::string group,
        std::string label,
        std::string desc,
        SettingControlType ctrl,
        std::string val_txt,
        std::string def_txt,
        bool bool_val,
        bool modified,
        bool adv = false) {
        SettingItemView it;
        it.key = std::move(key);
        it.category = cat;
        it.group_title = std::move(group);
        it.label = std::move(label);
        it.description = std::move(desc);
        it.control_type = ctrl;
        it.value_text = std::move(val_txt);
        it.default_value_text = std::move(def_txt);
        it.bool_value = bool_val;
        it.is_modified = modified;
        it.is_advanced = adv;
        if (it.key == m_settings_validation_key) {
            it.validation_error = m_settings_validation_error;
        }
        items.push_back(std::move(it));
    };

    // 1. General
    add_item(
        "start_minimized", SettingsCategory::kGeneral, "Startup & Window Behavior",
        "Start Minimized to System Tray",
        "Launch Flux quietly in the system tray without opening the main window on startup.",
        SettingControlType::kToggleSwitch,
        s.start_minimized ? "Enabled" : "Disabled", "Disabled",
        s.start_minimized, s.start_minimized != saved.start_minimized);
    add_item(
        "close_to_tray", SettingsCategory::kGeneral, "Startup & Window Behavior",
        "Minimize to System Tray on Close",
        "Keep active downloads and browser IPC listening in the tray when closing the main window.",
        SettingControlType::kToggleSwitch,
        s.close_to_tray ? "Enabled" : "Disabled", "Enabled",
        s.close_to_tray, s.close_to_tray != saved.close_to_tray);
    add_item(
        "reduced_motion", SettingsCategory::kGeneral, "Cosmic Interface & Accessibility",
        "Reduce Decorative Starfield Motion",
        "Use the static cosmic starfield fallback with zero background movement.",
        SettingControlType::kToggleSwitch,
        s.reduced_motion ? "Static Fallback" : "Subtle Drift", "Subtle Drift",
        s.reduced_motion, s.reduced_motion != saved.reduced_motion);

    // 2. Downloads & Queue
    add_item(
        "max_concurrent_tasks", SettingsCategory::kDownloads, "Queue Concurrency",
        "Maximum Simultaneous Downloads",
        "Active HTTP and stream transfers allowed to run concurrently in the main queue (1–16).",
        SettingControlType::kNumericStepper,
        std::to_string(s.max_concurrent_tasks) + " tasks", "3 tasks",
        false, s.max_concurrent_tasks != saved.max_concurrent_tasks);
    add_item(
        "auto_start_downloads", SettingsCategory::kDownloads, "Default Task Behavior",
        "Start New Downloads Immediately",
        "Automatically begin transfer when a download or media stream is added from browser or clipboard.",
        SettingControlType::kToggleSwitch,
        s.auto_start_downloads ? "Enabled" : "Queued Paused", "Enabled",
        s.auto_start_downloads, s.auto_start_downloads != saved.auto_start_downloads);
    add_item(
        "conflict_policy", SettingsCategory::kDownloads, "Default Task Behavior",
        "Existing File Conflict Resolution",
        "Action taken when a completed download matches an existing filename in the destination folder.",
        SettingControlType::kDropdown,
        FormatConflictPolicyLabel(s.conflict_policy), FormatConflictPolicyLabel(ConflictPolicy::kAutoRename),
        false, s.conflict_policy != saved.conflict_policy);

    // 3. Save Locations
    add_item(
        "default_download_dir", SettingsCategory::kSaveLocations, "Destination Folders",
        "Default Download Directory",
        "Base directory where completed files and muxed media containers are saved.",
        SettingControlType::kFolderSelector,
        s.default_download_dir, def_dir,
        false, s.default_download_dir != saved.default_download_dir);
    add_item(
        "category_subfolders_enabled", SettingsCategory::kSaveLocations, "Destination Folders",
        "Organize Downloads into Category Subfolders",
        "Route Video, Audio, Compressed, Documents, and Programs into dedicated subfolders automatically.",
        SettingControlType::kToggleSwitch,
        s.category_subfolders_enabled ? "Enabled" : "Flat Directory", "Enabled",
        s.category_subfolders_enabled, s.category_subfolders_enabled != saved.category_subfolders_enabled);

    // 4. Connections & Speed Limits
    add_item(
        "default_connections_per_task", SettingsCategory::kConnectionsAndSpeed, "Segmented HTTP Engine",
        "Default Connections per Task",
        "Maximum dynamic work-stealing HTTP range connections spawned per file transfer (1–32).",
        SettingControlType::kNumericStepper,
        std::to_string(s.default_connections_per_task) + " connections", "8 connections",
        false, s.default_connections_per_task != saved.default_connections_per_task);
    add_item(
        "speed_limit_bps", SettingsCategory::kConnectionsAndSpeed, "Bandwidth Throttling",
        "Global Download Speed Limit",
        "Cap aggregate transfer rate across active downloads (0 = Unlimited).",
        SettingControlType::kDropdown,
        FormatBandwidthLimit(s.speed_limit_bps), "Unlimited",
        false, s.speed_limit_bps != saved.speed_limit_bps);
    add_item(
        "stall_timeout_ms", SettingsCategory::kConnectionsAndSpeed, "Advanced Transfer Tuning",
        "Segment Stall Timeout",
        "Inactivity threshold before resetting a stalled HTTP range connection (500–60,000 ms).",
        SettingControlType::kNumericStepper,
        std::to_string(s.stall_timeout_ms) + " ms", "3000 ms",
        false, s.stall_timeout_ms != saved.stall_timeout_ms, true);
    add_item(
        "min_split_bytes", SettingsCategory::kConnectionsAndSpeed, "Advanced Transfer Tuning",
        "Minimum Work-Stealing Split Size",
        "Smallest remaining byte range eligible for dynamic segment splitting (64 KiB – 10 MiB).",
        SettingControlType::kDropdown,
        FormatBytes(s.min_split_bytes), "128.0 KiB",
        false, s.min_split_bytes != saved.min_split_bytes, true);

    // 5. Browser Integration
    add_item(
        "browser_capture_enabled", SettingsCategory::kBrowserIntegration, "Extension Capture & Interception",
        "Enable Browser Download & Media Capture",
        "Accept file downloads and HLS/DASH streams from the Flux Browser Extension (hold Alt when clicking a link to bypass).",
        SettingControlType::kToggleSwitch,
        s.browser_capture_enabled ? "Enabled" : "Paused", "Enabled",
        s.browser_capture_enabled, s.browser_capture_enabled != saved.browser_capture_enabled);
    add_item(
        "min_capture_size_bytes", SettingsCategory::kBrowserIntegration, "Extension Capture & Interception",
        "Minimum Automatic File Capture Size",
        "Minimum size threshold for size-based fallback download interception.",
        SettingControlType::kDropdown,
        FormatBytes(s.min_capture_size_bytes), "1.0 MiB",
        false, s.min_capture_size_bytes != saved.min_capture_size_bytes);
    auto bstat = GetBrowserIntegrationStatus();
    add_item(
        "ipc_endpoint_status", SettingsCategory::kBrowserIntegration, "Native Messaging & IPC Status",
        "Local IPC Endpoint (" + std::string(bstat.ipc_server_running ? "Listening" : "Offline") + ")",
        "Host ID: com.flux.downloadmanager  |  Endpoint: " + bstat.ipc_endpoint,
        SettingControlType::kDiagnosticReadOnly,
        bstat.ipc_server_running ? "Connected" : "Standby", "Connected",
        bstat.ipc_server_running, false);

    // 6. File Types
    const std::pair<const char*, const char*> cat_rules[] = {
        {"Video",      ".mp4, .mkv, .webm, .avi, .mov, .flv, .ts, .m4v, .wmv"},
        {"Audio",      ".mp3, .m4a, .flac, .wav, .ogg, .aac, .opus"},
        {"Compressed", ".zip, .rar, .7z, .tar, .gz, .bz2, .xz, .tgz, .zst"},
        {"Documents",  ".pdf, .doc, .docx, .xls, .xlsx, .ppt, .pptx, .txt, .md, .epub, .csv"},
        {"Programs",   ".exe, .msi, .iso, .img, .dmg, .deb, .rpm, .apk, .msix"}
    };
    for (const auto& [cname, exts] : cat_rules) {
        std::filesystem::path target_p = s.category_subfolders_enabled
            ? (std::filesystem::path(s.default_download_dir) / cname)
            : std::filesystem::path(s.default_download_dir);
        add_item(
            std::string("filetype_rule_") + ToLower(cname),
            SettingsCategory::kFileTypes, "Automatic Category Classification Rules",
            std::string(cname) + " Category (" + exts + ")",
            "Destination: " + target_p.string(),
            SettingControlType::kDiagnosticReadOnly,
            s.category_subfolders_enabled ? (std::string(cname) + "/") : "Root Folder",
            std::string(cname) + "/",
            true, false);
    }

    // 7. Notifications
    add_item(
        "show_completion_notification", SettingsCategory::kNotifications, "System Tray Notifications",
        "Show Completion & Error Notifications",
        "Display a Windows system tray notification when a background download completes or fails.",
        SettingControlType::kToggleSwitch,
        s.show_completion_notification ? "Enabled" : "Muted", "Enabled",
        s.show_completion_notification, s.show_completion_notification != saved.show_completion_notification);

    // 8. About & Diagnostics
    auto ab = GetAboutDialogView();
    add_item(
        "diag_version", SettingsCategory::kAboutAndDiagnostics, "Application & Environment",
        ab.app_name + " v" + ab.version,
        "License: " + ab.license + "  |  Platform: " + ab.target_platform,
        SettingControlType::kDiagnosticReadOnly,
        "v" + ab.version, "v" + ab.version, true, false);
    add_item(
        "diag_nm3u8dl", SettingsCategory::kAboutAndDiagnostics, "Stream Pipeline Workers",
        "Stream Manifest & Segment Worker (N_m3u8DL-RE)",
        bstat.nm3u8dl_path,
        SettingControlType::kDiagnosticReadOnly,
        bstat.nm3u8dl_available ? "Ready" : "Missing", "Ready", bstat.nm3u8dl_available, false);
    add_item(
        "diag_ffmpeg", SettingsCategory::kAboutAndDiagnostics, "Stream Pipeline Workers",
        "Container Multiplexer (FFmpeg)",
        bstat.ffmpeg_path,
        SettingControlType::kDiagnosticReadOnly,
        bstat.ffmpeg_available ? "Ready" : "Missing", "Ready", bstat.ffmpeg_available, false);
    add_item(
        "diag_storage", SettingsCategory::kAboutAndDiagnostics, "Storage & Persistence",
        "SQLite WAL State Database",
        m_controller.GetConfig().state_db_path.string(),
        SettingControlType::kDiagnosticReadOnly,
        "WAL Active", "WAL Active", true, false);

    return items;
}

SettingsCenterViewModel DesktopView::BuildSettingsCenterView() const {
    SettingsCenterViewModel vm;
    vm.is_open = m_settings_center_open;
    vm.active_category = m_settings_category;
    vm.search_query = m_settings_search_query;
    vm.has_unsaved_changes = HasUnsavedSettingsChanges();
    auto val_res = ValidateSettingsInput(m_draft_settings);
    vm.is_valid = m_settings_validation_error.empty() && val_res.ok();
    vm.validation_error = !m_settings_validation_error.empty()
        ? m_settings_validation_error
        : (val_res.ok() ? "" : val_res.error->message);
    vm.browser_status = GetBrowserIntegrationStatus();
    vm.about_info = GetAboutDialogView();

    if (!vm.is_valid) {
        vm.status_banner = "Validation Error: " + vm.validation_error;
    } else if (vm.has_unsaved_changes) {
        vm.status_banner = "Unsaved changes — Click 'Apply & Save' to persist or 'Cancel' to revert.";
    } else {
        vm.status_banner = "All configuration changes saved to SQLite state store.";
    }

    const auto& m = m_metrics;
    int margin_x = DesignSystem::ScaleForDpi(44, m.dpi);
    int margin_y = DesignSystem::ScaleForDpi(36, m.dpi);
    vm.modal_rect = {margin_x, margin_y, m_window_width - margin_x, m_window_height - margin_y};
    int side_w = DesignSystem::ScaleForDpi(210, m.dpi);
    int header_h = DesignSystem::ScaleForDpi(52, m.dpi);
    int footer_h = DesignSystem::ScaleForDpi(52, m.dpi);

    vm.sidebar_rect = {vm.modal_rect.left, vm.modal_rect.top + header_h, vm.modal_rect.left + side_w, vm.modal_rect.bottom - footer_h};
    vm.content_rect = {vm.sidebar_rect.right + 1, vm.modal_rect.top + header_h, vm.modal_rect.right, vm.modal_rect.bottom - footer_h};
    vm.search_box_rect = {
        vm.modal_rect.right - DesignSystem::ScaleForDpi(240, m.dpi) - m.space_md,
        vm.modal_rect.top + (header_h - m.button_height) / 2,
        vm.modal_rect.right - m.space_md,
        vm.modal_rect.top + (header_h + m.button_height) / 2
    };

    int footer_btn_y = vm.modal_rect.bottom - (footer_h + m.button_height) / 2;
    int restore_w = DesignSystem::ScaleForDpi(140, m.dpi);
    int cancel_w = DesignSystem::ScaleForDpi(96, m.dpi);
    int apply_w = DesignSystem::ScaleForDpi(124, m.dpi);

    vm.restore_defaults_rect = {
        vm.modal_rect.left + m.space_md,
        footer_btn_y,
        vm.modal_rect.left + m.space_md + restore_w,
        footer_btn_y + m.button_height
    };
    vm.apply_button_rect = {
        vm.modal_rect.right - m.space_md - apply_w,
        footer_btn_y,
        vm.modal_rect.right - m.space_md,
        footer_btn_y + m.button_height
    };
    vm.cancel_button_rect = {
        vm.apply_button_rect.left - m.space_sm - cancel_w,
        footer_btn_y,
        vm.apply_button_rect.left - m.space_sm,
        footer_btn_y + m.button_height
    };

    auto all_items = BuildAllSettingItems();
    const SettingsCategory all_cats[] = {
        SettingsCategory::kGeneral,
        SettingsCategory::kDownloads,
        SettingsCategory::kSaveLocations,
        SettingsCategory::kConnectionsAndSpeed,
        SettingsCategory::kBrowserIntegration,
        SettingsCategory::kFileTypes,
        SettingsCategory::kNotifications,
        SettingsCategory::kAboutAndDiagnostics
    };

    int tab_y = vm.sidebar_rect.top + m.space_sm;
    int tab_h = DesignSystem::ScaleForDpi(32, m.dpi);
    for (SettingsCategory c : all_cats) {
        size_t cnt = 0;
        size_t mod_cnt = 0;
        for (const auto& it : all_items) {
            if (it.category == c) {
                ++cnt;
                if (it.is_modified) ++mod_cnt;
            }
        }
        RectI trc{vm.sidebar_rect.left + m.space_sm, tab_y, vm.sidebar_rect.right - m.space_sm, tab_y + tab_h};
        vm.categories.push_back({c, SettingsCategoryToLabel(c), cnt, mod_cnt, c == m_settings_category, trc});
        tab_y += tab_h + m.space_xs;
    }

    std::string q = ToLower(m_settings_search_query);
    int row_y = vm.content_rect.top + m.space_sm;
    int row_h = DesignSystem::ScaleForDpi(48, m.dpi);
    int ctrl_w = DesignSystem::ScaleForDpi(170, m.dpi);
    int ctrl_h = DesignSystem::ScaleForDpi(28, m.dpi);

    for (auto& it : all_items) {
        bool matches = false;
        if (!q.empty()) {
            matches = ToLower(it.label).find(q) != std::string::npos ||
                      ToLower(it.description).find(q) != std::string::npos ||
                      ToLower(it.group_title).find(q) != std::string::npos ||
                      ToLower(it.key).find(q) != std::string::npos;
        } else {
            matches = (it.category == m_settings_category);
        }
        if (!matches) continue;

        it.row_rect = {vm.content_rect.left + m.space_md, row_y, vm.content_rect.right - m.space_md, row_y + row_h};
        it.control_rect = {
            it.row_rect.right - ctrl_w - m.space_sm,
            row_y + (row_h - ctrl_h) / 2,
            it.row_rect.right - m.space_sm,
            row_y + (row_h + ctrl_h) / 2
        };
        vm.visible_items.push_back(it);
        row_y += row_h + m.space_xs;
    }

    const std::pair<const char*, const char*> rules[] = {
        {"Video",      ".mp4, .mkv, .webm, .avi, .mov, .flv, .ts, .m4v, .wmv"},
        {"Audio",      ".mp3, .m4a, .flac, .wav, .ogg, .aac, .opus"},
        {"Compressed", ".zip, .rar, .7z, .tar, .gz, .bz2, .xz, .tgz, .zst"},
        {"Documents",  ".pdf, .doc, .docx, .xls, .xlsx, .ppt, .pptx, .txt, .md, .epub, .csv"},
        {"Programs",   ".exe, .msi, .iso, .img, .dmg, .deb, .rpm, .apk, .msix"}
    };
    for (const auto& [cn, exts] : rules) {
        std::filesystem::path p = m_draft_settings.category_subfolders_enabled
            ? (std::filesystem::path(m_draft_settings.default_download_dir) / cn)
            : std::filesystem::path(m_draft_settings.default_download_dir);
        vm.file_category_rules.push_back({cn, exts, p.string()});
    }

    return vm;
}

// -----------------------------------------------------------------------------
// Dialog & Toolbar Action Controllers
// -----------------------------------------------------------------------------

Result<TaskModel> DesktopView::SubmitAddDownloadDialog(const AddDownloadDialogInput& input) {
    auto parsed = brisk::HttpClient::ParseUrl(input.url);
    if (!parsed.ok()) {
        m_last_feedback_message = "Invalid URL: " + parsed.error->message;
        return Result<TaskModel>::Err(parsed.error->code, parsed.error->message);
    }

    TaskModel t;
    t.url = input.url;
    t.file_name = input.custom_filename;
    t.save_dir = input.save_dir;
    t.max_connections = std::clamp(input.max_connections, 1u, 32u);
    t.scheduled = input.scheduled;
    t.schedule_time_local = input.schedule_time_local;

    if (input.is_stream || input.url.find(".m3u8") != std::string::npos) {
        t.task_type = TaskType::kStreamHls;
        t.stream_options.video_filter = input.video_filter.empty() ? "best" : input.video_filter;
        t.stream_options.audio_filter = input.audio_filter.empty() ? "all" : input.audio_filter;
        t.stream_options.subtitle_filter = input.subtitle_filter.empty() ? "all" : input.subtitle_filter;
        t.stream_options.output_format = (input.output_format == "mkv") ? "mkv" : "mp4";
    } else if (input.url.find(".mpd") != std::string::npos) {
        t.task_type = TaskType::kStreamDash;
        t.stream_options.video_filter = input.video_filter.empty() ? "best" : input.video_filter;
        t.stream_options.audio_filter = input.audio_filter.empty() ? "all" : input.audio_filter;
        t.stream_options.subtitle_filter = input.subtitle_filter.empty() ? "all" : input.subtitle_filter;
        t.stream_options.output_format = (input.output_format == "mkv") ? "mkv" : "mp4";
    } else {
        t.task_type = TaskType::kHttpFile;
    }

    auto res = m_controller.AddTask(std::move(t), input.start_immediately, false);
    if (res.ok()) {
        m_selected_task_id = res.value->task_id;
        m_last_feedback_message = "Added download: " + res.value->file_name;
    } else {
        m_last_feedback_message = "Failed to add download: " + res.error->message;
    }
    return res;
}

Result<stream::StreamProbeResult> DesktopView::ProbeStreamForDialog(
    const std::string& manifest_url,
    const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& auth_headers) {
    auto res = m_controller.ProbeStream(manifest_url, headers, auth_headers);
    if (!res.ok()) {
        m_last_feedback_message = "Stream probe failed: " + res.error->message;
    } else {
        m_last_feedback_message = "Discovered " + std::to_string(res.value->tracks.size()) + " stream tracks";
    }
    return res;
}

Result<TaskModel> DesktopView::ConfirmStreamSelectionDialog(
    const stream::StreamProbeResult& probe_result,
    const std::string& page_title,
    const std::string& save_dir,
    const StreamSelectionOptions& chosen_options,
    const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& auth_headers) {

    if (probe_result.tracks.empty()) {
        return Result<TaskModel>::Err("ERR_STREAM_NO_TRACKS", "Cannot create stream task from empty track list");
    }

    TaskModel t;
    t.url = probe_result.manifest_url;
    t.task_type = (probe_result.manifest_url.find(".mpd") != std::string::npos)
                      ? TaskType::kStreamDash
                      : TaskType::kStreamHls;
    std::string fmt = (chosen_options.output_format == "mkv") ? "mkv" : "mp4";
    if (!page_title.empty()) {
        t.file_name = PathSanitizer::SanitizeFileName(page_title) + "." + fmt;
    }
    t.save_dir = save_dir;
    t.headers = headers;
    t.auth_headers = auth_headers;
    t.stream_options = chosen_options;
    t.stream_options.output_format = fmt;

    auto res = m_controller.AddTask(std::move(t), true, false);
    if (res.ok()) {
        m_selected_task_id = res.value->task_id;
        m_last_feedback_message = "Queued stream download: " + res.value->file_name;
    }
    return res;
}

Result<void> DesktopView::TriggerToolbarAction(ToolbarActionId action_id) {
    switch (action_id) {
        case ToolbarActionId::kStart:        return ActionStartSelected();
        case ToolbarActionId::kPause:        return ActionPauseSelected();
        case ToolbarActionId::kResume:       return ActionResumeSelected();
        case ToolbarActionId::kRetry:        return ActionRetrySelected();
        case ToolbarActionId::kCancel:       return ActionCancelSelected();
        case ToolbarActionId::kRemove:       return ActionRemoveSelectedFromList();
        case ToolbarActionId::kClearHistory: {
            auto r = ActionClearCompletedHistory();
            return r.ok() ? Result<void>::Ok() : Result<void>::Err(r.error->code, r.error->message);
        }
        case ToolbarActionId::kOpenFolder: {
            auto r = ActionOpenDestinationFolder(true);
            return r.ok() ? Result<void>::Ok() : Result<void>::Err(r.error->code, r.error->message);
        }
        case ToolbarActionId::kAddUrl:
            m_last_feedback_message = "Opened Add Download Dialog";
            return Result<void>::Ok();
        case ToolbarActionId::kSettings:
            OpenSettingsCenter(SettingsCategory::kGeneral);
            return Result<void>::Ok();
    }
    return Result<void>::Ok();
}

Result<void> DesktopView::TriggerRowPrimaryAction(const std::string& task_id) {
    SelectTask(task_id);
    auto t = m_controller.GetTask(task_id);
    if (!t.has_value()) return Result<void>::Err("ERR_NOT_FOUND", "Task not found");
    switch (t->state) {
        case TaskState::kDownloading:
        case TaskState::kProbing:
        case TaskState::kQueued:
            return ActionPauseSelected();
        case TaskState::kPaused:
            return ActionResumeSelected();
        case TaskState::kFailed:
        case TaskState::kCanceled:
            return ActionRetrySelected();
        case TaskState::kCompleted: {
            auto r = ActionOpenCompletedFile(true);
            return r.ok() ? Result<void>::Ok() : Result<void>::Err(r.error->code, r.error->message);
        }
        default:
            return Result<void>::Ok();
    }
}

Result<void> DesktopView::ActionStartSelected() {
    if (m_selected_task_id.empty()) return Result<void>::Err("ERR_NO_SELECTION", "No task selected");
    auto r = m_controller.StartTask(m_selected_task_id);
    m_last_feedback_message = r.ok() ? "Started selected task" : r.error->message;
    return r;
}

Result<void> DesktopView::ActionPauseSelected() {
    if (m_selected_task_id.empty()) return Result<void>::Err("ERR_NO_SELECTION", "No task selected");
    auto r = m_controller.PauseTask(m_selected_task_id);
    m_last_feedback_message = r.ok() ? "Paused selected task" : r.error->message;
    return r;
}

Result<void> DesktopView::ActionResumeSelected() {
    if (m_selected_task_id.empty()) return Result<void>::Err("ERR_NO_SELECTION", "No task selected");
    auto r = m_controller.ResumeTask(m_selected_task_id);
    m_last_feedback_message = r.ok() ? "Resumed selected task" : r.error->message;
    return r;
}

Result<void> DesktopView::ActionRetrySelected() {
    if (m_selected_task_id.empty()) return Result<void>::Err("ERR_NO_SELECTION", "No task selected");
    auto r = m_controller.RetryTask(m_selected_task_id);
    m_last_feedback_message = r.ok() ? "Retrying selected task" : r.error->message;
    return r;
}

Result<void> DesktopView::ActionCancelSelected() {
    if (m_selected_task_id.empty()) return Result<void>::Err("ERR_NO_SELECTION", "No task selected");
    auto r = m_controller.CancelTask(m_selected_task_id);
    m_last_feedback_message = r.ok() ? "Canceled selected task" : r.error->message;
    return r;
}

Result<void> DesktopView::ActionRemoveSelectedFromList() {
    if (m_selected_task_id.empty()) return Result<void>::Err("ERR_NO_SELECTION", "No task selected");
    std::string id = m_selected_task_id;
    auto r = m_controller.RemoveTask(id, false);
    if (r.ok()) {
        SelectFirstRow();
        m_last_feedback_message = "Removed task from list (completed file kept on disk)";
    } else {
        m_last_feedback_message = r.error->message;
    }
    return r;
}

Result<void> DesktopView::ActionDeleteSelectedWithFile() {
    if (m_selected_task_id.empty()) return Result<void>::Err("ERR_NO_SELECTION", "No task selected");
    std::string id = m_selected_task_id;
    auto r = m_controller.RemoveTask(id, true);
    if (r.ok()) {
        SelectFirstRow();
        m_last_feedback_message = "Deleted task and removed output file from disk";
    } else {
        m_last_feedback_message = r.error->message;
    }
    return r;
}

Result<int> DesktopView::ActionClearCompletedHistory() {
    auto r = m_controller.ClearCompletedHistory();
    if (r.ok()) {
        m_last_feedback_message = "Cleared " + std::to_string(*r.value) + " completed/canceled item(s) from history";
    }
    return r;
}

Result<std::filesystem::path> DesktopView::ActionOpenCompletedFile(bool dry_run) {
    if (m_selected_task_id.empty()) {
        return Result<std::filesystem::path>::Err("ERR_NO_SELECTION", "No task selected");
    }
    auto t = m_controller.GetTask(m_selected_task_id);
    if (!t.has_value()) {
        return Result<std::filesystem::path>::Err("ERR_NOT_FOUND", "Task not found");
    }
    if (t->state != TaskState::kCompleted) {
        return Result<std::filesystem::path>::Err("ERR_NOT_COMPLETED", "File is not yet completed");
    }
    std::filesystem::path fp = std::filesystem::path(t->save_dir) / t->file_name;
    std::error_code ec;
    if (!std::filesystem::exists(fp, ec)) {
        m_last_feedback_message = "File no longer exists on disk: " + fp.string();
        return Result<std::filesystem::path>::Err("ERR_FILE_MISSING", m_last_feedback_message);
    }
    if (!dry_run) {
#ifdef _WIN32
        ShellExecuteW(nullptr, L"open", fp.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#endif
    }
    m_last_feedback_message = "Opened file: " + fp.filename().string();
    return Result<std::filesystem::path>::Ok(fp);
}

Result<std::filesystem::path> DesktopView::ActionOpenDestinationFolder(bool dry_run) {
    if (m_selected_task_id.empty()) {
        return Result<std::filesystem::path>::Err("ERR_NO_SELECTION", "No task selected");
    }
    auto t = m_controller.GetTask(m_selected_task_id);
    if (!t.has_value()) {
        return Result<std::filesystem::path>::Err("ERR_NOT_FOUND", "Task not found");
    }
    std::filesystem::path dir(t->save_dir);
    std::error_code ec;
    if (!std::filesystem::exists(dir, ec)) {
        return Result<std::filesystem::path>::Err("ERR_DIR_MISSING", "Destination folder does not exist");
    }
    if (!dry_run) {
#ifdef _WIN32
        std::filesystem::path fp = dir / t->file_name;
        if (std::filesystem::exists(fp, ec)) {
            std::wstring param = L"/select,\"" + fp.wstring() + L"\"";
            ShellExecuteW(nullptr, L"open", L"explorer.exe", param.c_str(), nullptr, SW_SHOWNORMAL);
        } else {
            ShellExecuteW(nullptr, L"open", dir.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        }
#endif
    }
    m_last_feedback_message = "Opened destination folder: " + dir.string();
    return Result<std::filesystem::path>::Ok(dir);
}

void DesktopView::ActionSetConcurrentLimit(uint32_t max_tasks) {
    m_controller.SetMaxConcurrentTasks(max_tasks);
    m_draft_settings.max_concurrent_tasks = m_controller.GetMaxConcurrentTasks();
    m_last_feedback_message = "Max concurrent downloads set to " + std::to_string(m_controller.GetMaxConcurrentTasks());
}

void DesktopView::ActionSetSpeedLimit(uint64_t bps) {
    m_controller.SetGlobalSpeedLimit(bps);
    m_draft_settings.speed_limit_bps = bps;
    m_last_feedback_message = (bps == 0) ? "Speed limit set to Unlimited" : ("Speed limit set to " + FormatSpeed(bps));
}

void DesktopView::ActionSetBrowserCapture(bool enabled) {
    auto s = m_controller.GetSettings();
    s.browser_capture_enabled = enabled;
    m_controller.UpdateSettings(s);
    m_draft_settings.browser_capture_enabled = enabled;
    if (m_ipc_server) {
        m_ipc_server->SetCaptureEnabled(enabled);
    }
    m_last_feedback_message = enabled ? "Browser download capture enabled" : "Browser download capture paused";
}

Result<void> DesktopView::ActionSaveSettings(const AppSettings& settings) {
    auto val = ValidateSettingsInput(settings);
    if (!val.ok()) {
        m_settings_validation_error = val.error->message;
        m_last_feedback_message = "Settings validation failed: " + val.error->message;
        return val;
    }
    auto r = m_controller.UpdateSettings(settings);
    if (r.ok()) {
        m_draft_settings = m_controller.GetSettings();
        m_settings_validation_key.clear();
        m_settings_validation_error.clear();
        SetTheme(settings.theme, false);
        m_starfield.SetReducedMotion(settings.reduced_motion);
        if (m_ipc_server) {
            m_ipc_server->SetCaptureEnabled(settings.browser_capture_enabled);
        }
        m_last_feedback_message = "Settings saved";
    }
    return r;
}

Result<AppSettings> DesktopView::ActionRestoreDefaultSettings() {
    auto r = m_controller.RestoreDefaultSettings();
    if (r.ok()) {
        m_draft_settings = *r.value;
        m_settings_validation_key.clear();
        m_settings_validation_error.clear();
        SetTheme(r.value->theme, false);
        m_starfield.SetReducedMotion(r.value->reduced_motion);
        if (m_ipc_server) {
            m_ipc_server->SetCaptureEnabled(r.value->browser_capture_enabled);
        }
        m_last_feedback_message = "Restored default settings";
    }
    return r;
}

bool DesktopView::HandleKeyShortcut(const std::string& key_chord) {
    if (key_chord == "Escape" || key_chord == "VK_ESCAPE") {
        if (m_settings_center_open) {
            CloseSettingsCenter(true);
            return true;
        }
        return false;
    }
    if (key_chord == "Down" || key_chord == "VK_DOWN") {
        SelectNextRow();
        return true;
    }
    if (key_chord == "Up" || key_chord == "VK_UP") {
        SelectPrevRow();
        return true;
    }
    if (key_chord == "Home" || key_chord == "VK_HOME") {
        SelectFirstRow();
        return true;
    }
    if (key_chord == "End" || key_chord == "VK_END") {
        SelectLastRow();
        return true;
    }
    if (key_chord == "Space" || key_chord == "VK_SPACE") {
        auto tb = ComputeToolbarState();
        if (tb.can_pause) ActionPauseSelected();
        else if (tb.can_resume) ActionResumeSelected();
        return true;
    }
    if (key_chord == "Delete" || key_chord == "VK_DELETE") {
        ActionRemoveSelectedFromList();
        return true;
    }
    if (key_chord == "Shift+Delete") {
        ActionDeleteSelectedWithFile();
        return true;
    }
    if (key_chord == "F5" || key_chord == "VK_F5") {
        ActionRetrySelected();
        return true;
    }
    if (key_chord == "Enter" || key_chord == "VK_RETURN") {
        ActionOpenCompletedFile(true);
        return true;
    }
    if (key_chord == "Ctrl+N") {
        TriggerToolbarAction(ToolbarActionId::kAddUrl);
        return true;
    }
    if (key_chord == "Ctrl+,") {
        TriggerToolbarAction(ToolbarActionId::kSettings);
        return true;
    }
    return false;
}

BrowserIntegrationStatusView DesktopView::GetBrowserIntegrationStatus() const {
    BrowserIntegrationStatusView v;
    v.ipc_server_running = (m_ipc_server != nullptr);
    v.ipc_endpoint = m_ipc_server ? m_ipc_server->GetEndpointPath() : ipc::IpcServer::DefaultSocketPath();
    v.capture_enabled = m_controller.GetSettings().browser_capture_enabled;
    const auto& cfg = m_controller.GetConfig();
    std::error_code ec;
    v.nm3u8dl_path = cfg.nm3u8dl_binary_path.string();
    v.nm3u8dl_available = std::filesystem::exists(cfg.nm3u8dl_binary_path, ec);
    v.ffmpeg_path = cfg.ffmpeg_binary_path.string();
    v.ffmpeg_available = std::filesystem::exists(cfg.ffmpeg_binary_path, ec);
    v.supported_browsers = {"Google Chrome (MV3)", "Microsoft Edge (MV3)", "Brave Browser (MV3)", "Mozilla Firefox (MV3)"};
    return v;
}

AboutDialogView DesktopView::GetAboutDialogView() const {
    AboutDialogView a;
    a.upstream_attributions = {
        "Brisk Download Manager (https://github.com/BrisklyDev/brisk) — GPL-3.0-or-later",
        "Cat Catch Media Sniffer (https://github.com/xifangczy/cat-catch) — GPL-3.0",
        "N_m3u8DL-RE Stream Downloader (https://github.com/nilaoda/N_m3u8DL-RE) — MIT",
        "FFmpeg Media Muxer (https://ffmpeg.org) — LGPL-2.1 / GPL-3.0"
    };
    return a;
}

std::string DesktopView::RenderQueueTableText() const {
    auto rows = BuildVisibleTaskRows(0, 500);
    std::ostringstream oss;
    oss << "========================================================================================================================\n";
    oss << " Flux Download Manager v0.3.0 — Cosmic Dark Edition  |  Category: " << CategoryToLabel(m_selected_category)
        << "  |  Theme: " << m_theme_name << "\n";
    oss << "========================================================================================================================\n";
    oss << std::left
        << std::setw(10) << "TASK ID"
        << std::setw(14) << "TYPE"
        << std::setw(34) << "FILENAME"
        << std::setw(28) << "STATUS / STAGE"
        << std::setw(14) << "PROGRESS"
        << std::setw(18) << "SIZE"
        << std::setw(12) << "SPEED" << "\n";
    oss << "------------------------------------------------------------------------------------------------------------------------\n";
    if (rows.empty()) {
        oss << "  " << GetEmptyStateMessage() << "\n";
    } else {
        for (const auto& r : rows) {
            oss << (r.selected ? "> " : "  ")
                << std::left
                << std::setw(8) << r.task_id.substr(0, 8)
                << std::setw(14) << r.file_type_badge
                << std::setw(34) << r.display_filename
                << std::setw(28) << r.status_text.substr(0, 26)
                << std::setw(14) << r.progress_text.substr(0, 12)
                << std::setw(18) << r.size_text.substr(0, 16)
                << std::setw(12) << r.speed_text << "\n";
        }
    }
    oss << "------------------------------------------------------------------------------------------------------------------------\n";
    auto sb = BuildStatusBar();
    oss << " " << sb.status_message << "\n";
    oss << "========================================================================================================================\n";
    return oss.str();
}

std::string DesktopView::RenderFullDesktopSnapshotText() const {
    std::ostringstream oss;
    auto counts = ComputeCategoryCounts();
    oss << RenderQueueTableText();
    oss << "[Sidebar Counts] All=" << counts.all_downloads
        << " InProgress=" << counts.in_progress
        << " Queued=" << counts.queued
        << " Completed=" << counts.completed
        << " Failed=" << counts.failed
        << " Video=" << counts.video_downloads
        << " Scheduled=" << counts.scheduled << "\n";
    auto details = BuildSelectedTaskDetails();
    if (details.has_selection) {
        oss << "[Details Panel] File=" << details.file_name
            << " | Engine=" << details.engine_name
            << " | Source=" << details.redacted_source_url
            << " | Headers=" << details.redacted_headers_summary << "\n";
    }
    return oss.str();
}

#ifdef _WIN32
// -----------------------------------------------------------------------------
// Native Windows 11 Win32 + GDI Double-Buffered Cosmic Dark Interface & Settings Center
// -----------------------------------------------------------------------------
namespace {

constexpr UINT WM_OPENIDM_TRAYICON = WM_APP + 101;
constexpr UINT IDT_UI_REFRESH      = 1001;
constexpr UINT IDT_COSMIC_DRIFT    = 1002;
constexpr int  IDC_SEARCH_EDIT     = 1101;

constexpr int IDM_TRAY_SHOW        = 2001;
constexpr int IDM_TRAY_HIDE        = 2002;
constexpr int IDM_TRAY_PAUSE_ALL   = 2003;
constexpr int IDM_TRAY_RESUME_ALL  = 2004;
constexpr int IDM_TRAY_EXIT        = 2005;

constexpr int IDM_CTX_START        = 3001;
constexpr int IDM_CTX_PAUSE        = 3002;
constexpr int IDM_CTX_RESUME       = 3003;
constexpr int IDM_CTX_RETRY        = 3004;
constexpr int IDM_CTX_CANCEL       = 3005;
constexpr int IDM_CTX_OPEN_FILE    = 3006;
constexpr int IDM_CTX_OPEN_FOLDER  = 3007;
constexpr int IDM_CTX_REMOVE_LIST  = 3008;
constexpr int IDM_CTX_DELETE_DISK  = 3009;
constexpr int IDM_CTX_SETTINGS     = 3010;
constexpr int IDM_CTX_BROWSER_STAT = 3011;
constexpr int IDM_CTX_ABOUT        = 3012;

struct Win32WindowContext {
    DesktopView* view{nullptr};
    DownloadController* controller{nullptr};
    HWND hwnd{nullptr};
    HWND hSearchEdit{nullptr};
    NOTIFYICONDATAW nid{};
    bool tray_added{false};
};

std::wstring Utf8ToW(std::string_view sv) {
    if (sv.empty()) return {};
    int len = MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), nullptr, 0);
    if (len <= 0) return {};
    std::wstring out(static_cast<size_t>(len), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), out.data(), len);
    return out;
}

std::string WToUtf8(std::wstring_view wv) {
    if (wv.empty()) return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, wv.data(), static_cast<int>(wv.size()), nullptr, 0, nullptr, nullptr);
    if (len <= 0) return {};
    std::string out(static_cast<size_t>(len), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wv.data(), static_cast<int>(wv.size()), out.data(), len, nullptr, nullptr);
    return out;
}

RECT ToWinRect(const RectI& r) {
    return RECT{r.left, r.top, r.right, r.bottom};
}

void FillRectSolid(HDC hdc, const RECT& rc, ColorRgba col) {
    HBRUSH br = CreateSolidBrush(col.ToWin32ColorRef());
    FillRect(hdc, &rc, br);
    DeleteObject(br);
}

void FrameRectBorder(HDC hdc, const RECT& rc, ColorRgba col, int thickness = 1) {
    HBRUSH br = CreateSolidBrush(col.ToWin32ColorRef());
    RECT top_r{rc.left, rc.top, rc.right, rc.top + thickness};
    RECT bot_r{rc.left, rc.bottom - thickness, rc.right, rc.bottom};
    RECT lft_r{rc.left, rc.top, rc.left + thickness, rc.bottom};
    RECT rgt_r{rc.right - thickness, rc.top, rc.right, rc.bottom};
    FillRect(hdc, &top_r, br);
    FillRect(hdc, &bot_r, br);
    FillRect(hdc, &lft_r, br);
    FillRect(hdc, &rgt_r, br);
    DeleteObject(br);
}

bool SelectFolderWithDialogWin32(HWND hwnd, std::string& io_folder) {
    wchar_t display_name[MAX_PATH]{};
    BROWSEINFOW bi{};
    bi.hwndOwner = hwnd;
    bi.pszDisplayName = display_name;
    bi.lpszTitle = L"Select Default Download Directory for Flux";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
    PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&bi);
    if (!pidl) return false;
    wchar_t path_buf[MAX_PATH]{};
    bool ok = (SHGetPathFromIDListW(pidl, path_buf) != FALSE);
    CoTaskMemFree(pidl);
    if (ok && path_buf[0] != L'\0') {
        io_folder = WToUtf8(path_buf);
        return true;
    }
    return false;
}

void ShowAddUrlQuickDialogWin32(HWND hwnd, DesktopView& view, DownloadController& controller) {
    std::string clip_url;
    if (OpenClipboard(hwnd)) {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData) {
            const wchar_t* wtxt = static_cast<const wchar_t*>(GlobalLock(hData));
            if (wtxt) {
                std::string candidate = WToUtf8(wtxt);
                if (candidate.rfind("http://", 0) == 0 || candidate.rfind("https://", 0) == 0) {
                    clip_url = candidate;
                }
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }
    if (clip_url.empty()) {
        MessageBoxW(
            hwnd,
            L"Copy an http:// or https:// download or stream (.m3u8 / .mpd) URL to your clipboard and click '+ Add URL' (or press Ctrl+N), or capture media directly from your browser via the Flux Browser Extension.",
            L"Flux — Add Download",
            MB_OK | MB_ICONINFORMATION);
        return;
    }

    auto s = controller.GetSettings();
    std::wstring prompt = L"Add download from clipboard URL?\n\nURL: " + Utf8ToW(clip_url) +
                          L"\nDestination: " + Utf8ToW(s.default_download_dir) +
                          L"\nConnections: " + std::to_wstring(s.default_connections_per_task);
    if (MessageBoxW(hwnd, prompt.c_str(), L"Flux — Add Download", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
        AddDownloadDialogInput in;
        in.url = clip_url;
        in.save_dir = s.default_download_dir;
        in.max_connections = s.default_connections_per_task;
        in.start_immediately = s.auto_start_downloads;
        view.SubmitAddDownloadDialog(in);
    }
}

void PaintCosmicBackgroundStars(HDC hdc, const DesktopView& view) {
    auto stars = view.BuildCosmicBackgroundStars();
    for (const auto& s : stars) {
        if (s.size_px <= 1) {
            SetPixelV(hdc, s.x, s.y, s.blended_color.ToWin32ColorRef());
        } else {
            RECT src{s.x, s.y, s.x + s.size_px, s.y + s.size_px};
            FillRectSolid(hdc, src, s.blended_color);
        }
    }
}

void PaintSettingsCenterOverlayWin32(HDC hdc, DesktopView& view, HFONT hFontBody, HFONT hFontBold) {
    const auto& pal = view.GetPalette();
    const auto& m = view.GetMetrics();
    auto sv = view.BuildSettingsCenterView();

    RECT modal_rc = ToWinRect(sv.modal_rect);
    FillRectSolid(hdc, modal_rc, pal.cosmic_void);
    FrameRectBorder(hdc, modal_rc, pal.border_focus, 1);

    int header_h = DesignSystem::ScaleForDpi(52, m.dpi);
    int footer_h = DesignSystem::ScaleForDpi(52, m.dpi);
    RECT hdr_rc{modal_rc.left + 1, modal_rc.top + 1, modal_rc.right - 1, modal_rc.top + header_h};
    FillRectSolid(hdc, hdr_rc, pal.cosmic_nebula);

    SelectObject(hdc, hFontBold);
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    RECT title_rc = hdr_rc;
    title_rc.left += m.space_lg;
    DrawTextW(hdc, L"Flux Settings & Configuration Center", -1, &title_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Sidebar categories
    RECT side_rc = ToWinRect(sv.sidebar_rect);
    FillRectSolid(hdc, side_rc, pal.surface_primary);
    SelectObject(hdc, hFontBody);
    for (const auto& cat : sv.categories) {
        RECT crc = ToWinRect(cat.rect);
        if (cat.selected) {
            FillRectSolid(hdc, crc, pal.surface_selected);
            RECT bar{crc.left, crc.top + 4, crc.left + 3, crc.bottom - 4};
            FillRectSolid(hdc, bar, pal.accent_cyan);
        }
        SetTextColor(hdc, (cat.selected ? pal.text_primary : pal.text_secondary).ToWin32ColorRef());
        std::string lbl = cat.label;
        if (cat.modified_count > 0) {
            lbl += " *";
        }
        std::wstring wlbl = Utf8ToW(lbl);
        RECT trc = crc;
        trc.left += m.space_md;
        DrawTextW(hdc, wlbl.c_str(), -1, &trc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // Content rows
    RECT content_rc = ToWinRect(sv.content_rect);
    FillRectSolid(hdc, content_rc, pal.app_background);
    for (const auto& item : sv.visible_items) {
        RECT rrc = ToWinRect(item.row_rect);
        FillRectSolid(hdc, rrc, pal.surface_primary);
        FrameRectBorder(hdc, rrc, item.is_modified ? pal.accent_cyan : pal.border_subtle, 1);

        RECT lbl_rc = rrc;
        lbl_rc.left += m.space_md;
        lbl_rc.top += 6;
        lbl_rc.right = item.control_rect.left - m.space_sm;
        lbl_rc.bottom = rrc.top + (rrc.bottom - rrc.top) / 2 + 2;
        SelectObject(hdc, hFontBold);
        SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
        std::wstring wtitle = Utf8ToW(item.label + (item.is_modified ? "  [Modified]" : ""));
        DrawTextW(hdc, wtitle.c_str(), -1, &lbl_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

        RECT desc_rc = rrc;
        desc_rc.left += m.space_md;
        desc_rc.top = lbl_rc.bottom;
        desc_rc.right = item.control_rect.left - m.space_sm;
        desc_rc.bottom -= 4;
        SelectObject(hdc, hFontBody);
        SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
        std::wstring wdesc = Utf8ToW(item.description);
        DrawTextW(hdc, wdesc.c_str(), -1, &desc_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

        RECT ctrl_rc = ToWinRect(item.control_rect);
        ColorRgba ctrl_bg = pal.surface_elevated;
        if (item.control_type == SettingControlType::kToggleSwitch && item.bool_value) {
            ctrl_bg = pal.accent_primary;
        }
        FillRectSolid(hdc, ctrl_rc, ctrl_bg);
        FrameRectBorder(hdc, ctrl_rc, pal.border_subtle, 1);
        SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
        std::wstring wval = Utf8ToW(item.value_text);
        DrawTextW(hdc, wval.c_str(), -1, &ctrl_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }

    // Footer bar with status banner and Reset / Cancel / Apply buttons
    RECT ftr_rc{modal_rc.left + 1, modal_rc.bottom - footer_h, modal_rc.right - 1, modal_rc.bottom - 1};
    FillRectSolid(hdc, ftr_rc, pal.cosmic_nebula);

    RECT rst_rc = ToWinRect(sv.restore_defaults_rect);
    FillRectSolid(hdc, rst_rc, pal.surface_elevated);
    FrameRectBorder(hdc, rst_rc, pal.border_subtle, 1);
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    DrawTextW(hdc, L"Restore Defaults", -1, &rst_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    RECT can_rc = ToWinRect(sv.cancel_button_rect);
    FillRectSolid(hdc, can_rc, pal.surface_elevated);
    FrameRectBorder(hdc, can_rc, pal.border_subtle, 1);
    DrawTextW(hdc, L"Cancel", -1, &can_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    RECT app_rc = ToWinRect(sv.apply_button_rect);
    FillRectSolid(hdc, app_rc, sv.is_valid ? pal.accent_primary : pal.status_danger);
    SelectObject(hdc, hFontBold);
    DrawTextW(hdc, L"Apply & Save", -1, &app_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    RECT banner_rc{rst_rc.right + m.space_md, ftr_rc.top, can_rc.left - m.space_md, ftr_rc.bottom};
    SelectObject(hdc, hFontBody);
    SetTextColor(hdc, (sv.is_valid ? pal.text_secondary : pal.status_danger).ToWin32ColorRef());
    std::wstring wbanner = Utf8ToW(sv.status_banner);
    DrawTextW(hdc, wbanner.c_str(), -1, &banner_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

void DrawRoundedBoxGdi(HDC hdc, const RECT& rc, int radius, ColorRgba bg, ColorRgba border, int border_w) {
    HPEN hPen = (border_w > 0) ? CreatePen(PS_SOLID, border_w, border.ToWin32ColorRef()) : CreatePen(PS_NULL, 0, 0);
    HBRUSH hBrush = CreateSolidBrush(bg.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, radius, radius);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void DrawCheckboxGdi(HDC hdc, const RECT& rc, bool checked, ColorRgba bg, ColorRgba border, ColorRgba check_col) {
    DrawRoundedBoxGdi(hdc, rc, 6, bg, border, 1);
    if (checked) {
        HPEN hPen = CreatePen(PS_SOLID, 2, check_col.ToWin32ColorRef());
        HGDIOBJ oldPen = SelectObject(hdc, hPen);
        POINT pts[3] = {
            {rc.left + 4, rc.top + (rc.bottom - rc.top) / 2},
            {rc.left + (rc.right - rc.left) / 2 - 1, rc.bottom - 5},
            {rc.right - 4, rc.top + 5}
        };
        Polyline(hdc, pts, 3);
        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
    }
}

void DrawDocFileIconGdi(HDC hdc, int x, int y, int w, int h, ColorRgba col) {
    int fold = 5;
    HPEN hPen = CreatePen(PS_SOLID, 1, col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    POINT pts[6] = {
        {x, y},
        {x + w - fold, y},
        {x + w, y + fold},
        {x + w, y + h},
        {x, y + h},
        {x, y}
    };
    Polygon(hdc, pts, 6);

    MoveToEx(hdc, x + w - fold, y, nullptr);
    LineTo(hdc, x + w - fold, y + fold);
    LineTo(hdc, x + w, y + fold);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

void DrawPauseGlyphGdi(HDC hdc, const RECT& rc, ColorRgba col) {
    int cx = (rc.left + rc.right) / 2;
    int cy = (rc.top + rc.bottom) / 2;
    int bar_w = 2;
    int bar_h = 10;
    int gap = 3;
    RECT r1{cx - gap - bar_w, cy - bar_h / 2, cx - gap, cy + bar_h / 2};
    RECT r2{cx + gap, cy - bar_h / 2, cx + gap + bar_w, cy + bar_h / 2};
    FillRectSolid(hdc, r1, col);
    FillRectSolid(hdc, r2, col);
}

void DrawPlayGlyphGdi(HDC hdc, const RECT& rc, ColorRgba col) {
    int cx = (rc.left + rc.right) / 2;
    int cy = (rc.top + rc.bottom) / 2;
    int sz = 4;
    POINT pts[3] = {
        {cx - sz + 1, cy - sz},
        {cx + sz + 1, cy},
        {cx - sz + 1, cy + sz}
    };
    HPEN hPen = CreatePen(PS_SOLID, 1, col.ToWin32ColorRef());
    HBRUSH hBrush = CreateSolidBrush(col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Polygon(hdc, pts, 3);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void DrawSearchGlassGdi(HDC hdc, int x, int y, int size, ColorRgba col) {
    HPEN hPen = CreatePen(PS_SOLID, 2, col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    int radius = size - 4;
    Ellipse(hdc, x, y, x + radius, y + radius);

    MoveToEx(hdc, x + radius - 1, y + radius - 1, nullptr);
    LineTo(hdc, x + size + 2, y + size + 2);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

void DrawSlidersGlyphGdi(HDC hdc, const RECT& rc, ColorRgba col) {
    int cx = (rc.left + rc.right) / 2;
    int cy = (rc.top + rc.bottom) / 2;
    HPEN hPen = CreatePen(PS_SOLID, 1, col.ToWin32ColorRef());
    HBRUSH hBrush = CreateSolidBrush(col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

    int y0 = cy - 5;
    int y1 = cy;
    int y2 = cy + 5;
    int left = cx - 7;
    int right = cx + 7;

    MoveToEx(hdc, left, y0, nullptr); LineTo(hdc, right, y0);
    MoveToEx(hdc, left, y1, nullptr); LineTo(hdc, right, y1);
    MoveToEx(hdc, left, y2, nullptr); LineTo(hdc, right, y2);

    RECT k0{cx - 2, y0 - 2, cx + 1, y0 + 3};
    RECT k1{cx + 2, y1 - 2, cx + 5, y1 + 3};
    RECT k2{cx - 5, y2 - 2, cx - 2, y2 + 3};
    FillRect(hdc, &k0, static_cast<HBRUSH>(hBrush));
    FillRect(hdc, &k1, static_cast<HBRUSH>(hBrush));
    FillRect(hdc, &k2, static_cast<HBRUSH>(hBrush));

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void DrawCaretGlyphGdi(HDC hdc, int cx, int cy, ColorRgba col, bool up = false) {
    int w = 3;
    POINT pts[3];
    if (up) {
        pts[0] = {cx - w, cy + 2};
        pts[1] = {cx, cy - 2};
        pts[2] = {cx + w, cy + 2};
    } else {
        pts[0] = {cx - w, cy - 2};
        pts[1] = {cx, cy + 2};
        pts[2] = {cx + w, cy - 2};
    }
    HPEN hPen = CreatePen(PS_SOLID, 1, col.ToWin32ColorRef());
    HBRUSH hBrush = CreateSolidBrush(col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Polygon(hdc, pts, 3);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void DrawFunnelGlyphGdi(HDC hdc, int x, int y, int size, ColorRgba col) {
    POINT pts[4] = {
        {x, y},
        {x + size, y},
        {x + size / 2 + 1, y + size - 2},
        {x + size / 2 - 1, y + size - 2}
    };
    HPEN hPen = CreatePen(PS_SOLID, 1, col.ToWin32ColorRef());
    HBRUSH hBrush = CreateSolidBrush(col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Polygon(hdc, pts, 4);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void DrawHamburgerGlyphGdi(HDC hdc, int x, int y, int w, int h, ColorRgba col) {
    HPEN hPen = CreatePen(PS_SOLID, 1, col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    int y0 = y + 1;
    int y1 = y + h / 2;
    int y2 = y + h - 2;
    MoveToEx(hdc, x, y0, nullptr); LineTo(hdc, x + w, y0);
    MoveToEx(hdc, x, y1, nullptr); LineTo(hdc, x + w, y1);
    MoveToEx(hdc, x, y2, nullptr); LineTo(hdc, x + w, y2);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

void DrawTimerClockGdi(HDC hdc, int x, int y, int radius, ColorRgba col) {
    HPEN hPen = CreatePen(PS_SOLID, 1, col.ToWin32ColorRef());
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
    MoveToEx(hdc, x, y, nullptr);
    LineTo(hdc, x, y - radius + 3);
    MoveToEx(hdc, x, y, nullptr);
    LineTo(hdc, x + radius - 4, y);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

HICON CreateFluxAppIcon() {
    int size = 32;
    HDC hdcScreen = GetDC(nullptr);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = size;
    bi.bmiHeader.biHeight = -size;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    HBITMAP hbmColor = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    HGDIOBJ oldBm = SelectObject(hdc, hbmColor);

    RECT rc{0, 0, size, size};
    HBRUSH bgBr = CreateSolidBrush(RGB(0x16, 0x16, 0x18));
    FillRect(hdc, &rc, bgBr);
    DeleteObject(bgBr);

    RECT sqRc{2, 2, size - 2, size - 2};
    DrawRoundedBoxGdi(hdc, sqRc, 10, {0xFF, 0x47, 0x57, 255}, {0xFF, 0x47, 0x57, 255}, 1);

    POINT pts[6] = {
        {18, 5},
        {10, 16},
        {16, 16},
        {13, 26},
        {23, 14},
        {17, 14}
    };
    HPEN wPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HBRUSH wBr = CreateSolidBrush(RGB(255, 255, 255));
    HGDIOBJ oldP = SelectObject(hdc, wPen);
    HGDIOBJ oldB = SelectObject(hdc, wBr);
    Polygon(hdc, pts, 6);
    SelectObject(hdc, oldB);
    SelectObject(hdc, oldP);
    DeleteObject(wBr);
    DeleteObject(wPen);

    SelectObject(hdc, oldBm);
    DeleteDC(hdc);
    ReleaseDC(nullptr, hdcScreen);

    HBITMAP hbmMask = CreateBitmap(size, size, 1, 1, nullptr);
    ICONINFO ii{};
    ii.fIcon = TRUE;
    ii.hbmMask = hbmMask;
    ii.hbmColor = hbmColor;
    HICON hIcon = CreateIconIndirect(&ii);
    DeleteObject(hbmColor);
    DeleteObject(hbmMask);
    return hIcon;
}

void PaintMainWindow(HWND hwnd, DesktopView& view) {
    PAINTSTRUCT ps{};
    HDC hdc_screen = BeginPaint(hwnd, &ps);

    RECT client_rc{};
    GetClientRect(hwnd, &client_rc);
    int w = client_rc.right - client_rc.left;
    int h = client_rc.bottom - client_rc.top;

    HDC hdc = CreateCompatibleDC(hdc_screen);
    HBITMAP hbm = CreateCompatibleBitmap(hdc_screen, std::max(1, w), std::max(1, h));
    HGDIOBJ old_bm = SelectObject(hdc, hbm);

    const auto& pal = view.GetPalette();
    auto geom = view.ComputeLayoutGeometry(0);

    // 0. Base Matte Charcoal Canvas
    FillRectSolid(hdc, client_rc, pal.app_background);
    SetBkMode(hdc, TRANSPARENT);

    HFONT hFontBody = CreateFontW(
        -13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontBold = CreateFontW(
        -13, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontSmall = CreateFontW(
        -11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontSmallBold = CreateFontW(
        -11, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HGDIOBJ old_font = SelectObject(hdc, hFontBody);

    // 1. Top Action Bar Controls
    // 1.1 Select All Checkbox
    RECT sel_rc = ToWinRect(geom.select_all_checkbox_rect);
    DrawCheckboxGdi(hdc, sel_rc, view.IsSelectAllChecked(), pal.surface_primary, pal.border_subtle, pal.accent_primary);

    // 1.2 Sort Dropdown
    RECT sort_rc = ToWinRect(geom.sort_dropdown_rect);
    DrawRoundedBoxGdi(hdc, sort_rc, 16, pal.surface_primary, pal.border_subtle, 1);
    DrawHamburgerGlyphGdi(hdc, sort_rc.left + 12, sort_rc.top + (sort_rc.bottom - sort_rc.top) / 2 - 4, 10, 8, pal.text_muted);
    RECT sort_txt_rc{sort_rc.left + 28, sort_rc.top, sort_rc.right - 18, sort_rc.bottom};
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    SelectObject(hdc, hFontBody);
    DrawTextW(hdc, L"Newest", -1, &sort_txt_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    DrawCaretGlyphGdi(hdc, sort_rc.right - 10, sort_rc.top + (sort_rc.bottom - sort_rc.top) / 2, pal.text_muted, false);

    // 1.3 Filter Dropdown
    RECT filter_rc = ToWinRect(geom.filter_dropdown_rect);
    DrawRoundedBoxGdi(hdc, filter_rc, 16, pal.surface_primary, pal.border_subtle, 1);
    DrawFunnelGlyphGdi(hdc, filter_rc.left + 12, filter_rc.top + (filter_rc.bottom - filter_rc.top) / 2 - 4, 10, pal.text_muted);
    RECT filter_txt_rc{filter_rc.left + 28, filter_rc.top, filter_rc.right - 18, filter_rc.bottom};
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    std::wstring wcat = (view.GetSelectedCategory() == SidebarCategory::kAllDownloads) ? L"All files" : Utf8ToW(view.CategoryToLabel(view.GetSelectedCategory()));
    DrawTextW(hdc, wcat.c_str(), -1, &filter_txt_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    DrawCaretGlyphGdi(hdc, filter_rc.right - 10, filter_rc.top + (filter_rc.bottom - filter_rc.top) / 2, pal.text_muted, false);

    // 1.4 + Add download Pill Button
    RECT add_rc = ToWinRect(geom.add_download_button_rect);
    ColorRgba add_bg = (view.GetHoveredToolbarIndex() == 0) ? pal.accent_hover : pal.accent_primary;
    DrawRoundedBoxGdi(hdc, add_rc, 18, add_bg, add_bg, 0);
    SelectObject(hdc, hFontBold);
    SetTextColor(hdc, RGB(255, 255, 255));
    DrawTextW(hdc, L"+ Add download", -1, &add_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 1.5 Search Capsule
    RECT search_rc = ToWinRect(geom.search_box_rect);
    DrawRoundedBoxGdi(hdc, search_rc, 18, pal.surface_primary, pal.border_subtle, 1);
    DrawSearchGlassGdi(hdc, search_rc.left + 12, search_rc.top + (search_rc.bottom - search_rc.top) / 2 - 5, 11, pal.text_muted);

    // Ctrl K chip inside search box
    RECT ctrl_k_rc{search_rc.right - 58, search_rc.top + 6, search_rc.right - 10, search_rc.bottom - 6};
    DrawRoundedBoxGdi(hdc, ctrl_k_rc, 6, pal.surface_elevated, pal.border_focus, 1);
    SelectObject(hdc, hFontSmallBold);
    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
    DrawTextW(hdc, L"Ctrl K", -1, &ctrl_k_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 1.6 Sliders Button (Settings)
    RECT sliders_rc = ToWinRect(geom.search_sliders_rect);
    DrawRoundedBoxGdi(hdc, sliders_rc, 18, pal.surface_primary, pal.border_subtle, 1);
    DrawSlidersGlyphGdi(hdc, sliders_rc, pal.text_secondary);

    // 2. Table Column Headers
    RECT header_rc = ToWinRect(geom.table_header_rect);
    SelectObject(hdc, hFontSmallBold);
    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
    for (size_t col_idx = 0; col_idx < geom.table_columns.size(); ++col_idx) {
        const auto& col = geom.table_columns[col_idx];
        RECT crc = ToWinRect(col.rect);
        std::wstring wt = Utf8ToW(col.title);
        DrawTextW(hdc, wt.c_str(), -1, &crc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
    RECT hdr_div{0, header_rc.bottom - 1, w, header_rc.bottom};
    FillRectSolid(hdc, hdr_div, pal.border_subtle);

    // 3. Task Rows
    auto rows = view.BuildVisibleTaskRows(0, geom.task_rows.size());
    if (rows.empty()) {
        RECT empty_rc = ToWinRect(geom.table_body_rect);
        SelectObject(hdc, hFontBody);
        SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
        std::wstring emsg = Utf8ToW(view.GetEmptyStateMessage());
        DrawTextW(hdc, emsg.c_str(), -1, &empty_rc, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
    } else {
        for (size_t i = 0; i < rows.size() && i < geom.task_rows.size(); ++i) {
            const auto& r = rows[i];
            const auto& rl = geom.task_rows[i];
            RECT rrc = ToWinRect(rl.row_rect);

            if (r.selected) {
                FillRectSolid(hdc, rrc, pal.surface_selected);
            } else if (view.GetHoveredRowIndex() == static_cast<int>(i)) {
                FillRectSolid(hdc, rrc, pal.surface_hover);
            }

            RECT row_div{0, rrc.bottom - 1, w, rrc.bottom};
            FillRectSolid(hdc, row_div, ColorRgba{0x20, 0x20, 0x24, 255});

            RECT chk_rc = ToWinRect(rl.checkbox_rect);
            DrawCheckboxGdi(hdc, chk_rc, r.checked || r.selected, pal.surface_primary, pal.border_subtle, pal.accent_primary);

            RECT idx_rc{chk_rc.right + 6, rrc.top, chk_rc.right + 26, rrc.bottom};
            SelectObject(hdc, hFontBody);
            SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
            std::wstring widx = std::to_wstring(i + 1);
            DrawTextW(hdc, widx.c_str(), -1, &idx_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            int icon_x = idx_rc.right + 8;
            int icon_y = rrc.top + (rrc.bottom - rrc.top - 20) / 2;
            ColorRgba icon_col = (r.state == TaskState::kDownloading) ? pal.accent_primary : pal.text_secondary;
            DrawDocFileIconGdi(hdc, icon_x, icon_y, 16, 20, icon_col);

            SelectObject(hdc, hFontBold);
            SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
            RECT fn_rc{icon_x + 24, rrc.top + 6, rl.filename_rect.right, rrc.bottom - 12};
            std::wstring wfn = Utf8ToW(r.display_filename);
            DrawTextW(hdc, wfn.c_str(), -1, &fn_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

            if (r.state == TaskState::kDownloading) {
                SIZE fn_sz{};
                GetTextExtentPoint32W(hdc, wfn.c_str(), static_cast<int>(wfn.size()), &fn_sz);
                int underline_w = std::max<int>(60, std::min<int>(static_cast<int>(fn_sz.cx), static_cast<int>(fn_rc.right - fn_rc.left)));
                int fill_w = static_cast<int>(static_cast<double>(underline_w) * r.progress_fraction);
                RECT u_track{fn_rc.left, rrc.bottom - 8, fn_rc.left + underline_w, rrc.bottom - 6};
                FillRectSolid(hdc, u_track, pal.progress_track);
                if (fill_w > 0) {
                    RECT u_fill{fn_rc.left, rrc.bottom - 8, fn_rc.left + fill_w, rrc.bottom - 6};
                    FillRectSolid(hdc, u_fill, pal.accent_primary);
                }
            }

            SelectObject(hdc, hFontBody);
            SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
            if (geom.table_columns.size() > 2) {
                RECT sz_rc = ToWinRect(geom.table_columns[2].rect);
                sz_rc.top = rrc.top;
                sz_rc.bottom = rrc.bottom;
                std::wstring wsz = Utf8ToW(r.size_text);
                DrawTextW(hdc, wsz.c_str(), -1, &sz_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }

            if (geom.table_columns.size() > 3) {
                RECT st_rc = ToWinRect(geom.table_columns[3].rect);
                st_rc.top = rrc.top;
                st_rc.bottom = rrc.bottom;

                int dot_cy = (st_rc.top + st_rc.bottom) / 2;
                if (r.state == TaskState::kDownloading) {
                    HPEN hDotPen = CreatePen(PS_SOLID, 1, pal.accent_primary.ToWin32ColorRef());
                    HBRUSH hDotBrush = CreateSolidBrush(pal.accent_primary.ToWin32ColorRef());
                    HGDIOBJ op = SelectObject(hdc, hDotPen);
                    HGDIOBJ ob = SelectObject(hdc, hDotBrush);
                    Ellipse(hdc, st_rc.left, dot_cy - 4, st_rc.left + 8, dot_cy + 4);
                    SelectObject(hdc, ob);
                    SelectObject(hdc, op);
                    DeleteObject(hDotBrush);
                    DeleteObject(hDotPen);
                    SetTextColor(hdc, pal.accent_primary.ToWin32ColorRef());
                } else {
                    HPEN hRingPen = CreatePen(PS_SOLID, 1, pal.text_secondary.ToWin32ColorRef());
                    HGDIOBJ op = SelectObject(hdc, hRingPen);
                    HGDIOBJ ob = SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    Ellipse(hdc, st_rc.left, dot_cy - 4, st_rc.left + 8, dot_cy + 4);
                    SelectObject(hdc, ob);
                    SelectObject(hdc, op);
                    DeleteObject(hRingPen);
                    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                }

                RECT st_txt_rc{st_rc.left + 14, st_rc.top, st_rc.right, st_rc.bottom};
                std::wstring wst = Utf8ToW(r.status_text);
                DrawTextW(hdc, wst.c_str(), -1, &st_txt_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }

            if (geom.table_columns.size() > 4) {
                RECT dl_rc = ToWinRect(geom.table_columns[4].rect);
                dl_rc.top = rrc.top;
                dl_rc.bottom = rrc.bottom;

                if (r.state == TaskState::kDownloading) {
                    RECT act_rc = ToWinRect(rl.action_icon_rect);
                    DrawRoundedBoxGdi(hdc, act_rc, 6, pal.surface_primary, pal.border_subtle, 1);
                    DrawPauseGlyphGdi(hdc, act_rc, pal.accent_primary);

                    RECT spd_rc{act_rc.right + 8, dl_rc.top, dl_rc.right, dl_rc.bottom};
                    SetTextColor(hdc, pal.accent_primary.ToWin32ColorRef());
                    std::wstring wspd = Utf8ToW(r.speed_text);
                    DrawTextW(hdc, wspd.c_str(), -1, &spd_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                } else if (r.state == TaskState::kPaused) {
                    RECT act_rc = ToWinRect(rl.action_icon_rect);
                    DrawRoundedBoxGdi(hdc, act_rc, 6, pal.surface_primary, pal.border_subtle, 1);
                    DrawPlayGlyphGdi(hdc, act_rc, pal.text_secondary);

                    RECT spd_rc{act_rc.right + 8, dl_rc.top, dl_rc.right, dl_rc.bottom};
                    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                    DrawTextW(hdc, L"0 B/s", -1, &spd_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                } else {
                    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                    DrawTextW(hdc, L"0 B/s", -1, &dl_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }
            }

            if (geom.table_columns.size() > 5) {
                RECT ul_rc = ToWinRect(geom.table_columns[5].rect);
                ul_rc.top = rrc.top;
                ul_rc.bottom = rrc.bottom;
                SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                std::wstring wul = Utf8ToW(r.upload_speed_text);
                DrawTextW(hdc, wul.c_str(), -1, &ul_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }

            if (geom.table_columns.size() > 6) {
                RECT add_time_rc = ToWinRect(geom.table_columns[6].rect);
                add_time_rc.top = rrc.top;
                add_time_rc.bottom = rrc.bottom;
                SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                std::wstring wadd = Utf8ToW(r.added_time_text);
                DrawTextW(hdc, wadd.c_str(), -1, &add_time_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }
        }
    }

    // 4. Status Bar / Footer
    RECT status_rc = ToWinRect(geom.status_bar_rect);
    FillRectSolid(hdc, status_rc, pal.app_background);
    RECT stat_div{0, status_rc.top, w, status_rc.top + 1};
    FillRectSolid(hdc, stat_div, pal.border_subtle);

    RECT off_rc = ToWinRect(geom.status_toggle_off_rect);
    bool limiter = view.IsSpeedLimiterActive();
    ColorRgba off_bg = limiter ? pal.accent_primary : pal.surface_primary;
    DrawRoundedBoxGdi(hdc, off_rc, 10, off_bg, pal.border_subtle, 1);
    SelectObject(hdc, hFontSmallBold);
    SetTextColor(hdc, limiter ? RGB(255, 255, 255) : pal.text_secondary.ToWin32ColorRef());
    DrawTextW(hdc, limiter ? L"ON" : L"OFF", -1, &off_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    int timer_cx = off_rc.right + 20;
    int timer_cy = (status_rc.top + status_rc.bottom) / 2;
    DrawTimerClockGdi(hdc, timer_cx, timer_cy, 6, pal.text_secondary);

    SelectObject(hdc, hFontBody);
    RECT dl_stat_rc{timer_cx + 14, status_rc.top, timer_cx + 120, status_rc.bottom};
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    auto sb = view.BuildStatusBar();
    std::wstring wstat_dl = std::wstring(L"\x2913 ") + (sb.active_downloads > 0 ? L"4.8 MB/s" : L"0 B/s");
    DrawTextW(hdc, wstat_dl.c_str(), -1, &dl_stat_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    RECT ul_stat_rc{dl_stat_rc.right + 10, status_rc.top, dl_stat_rc.right + 90, status_rc.bottom};
    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
    DrawTextW(hdc, L"\x2912 0 B/s", -1, &ul_stat_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    RECT pri_rc{ul_stat_rc.right + 10, status_rc.top, ul_stat_rc.right + 90, status_rc.bottom};
    HPEN hDotPen = CreatePen(PS_SOLID, 1, pal.accent_primary.ToWin32ColorRef());
    HBRUSH hDotBrush = CreateSolidBrush(pal.accent_primary.ToWin32ColorRef());
    HGDIOBJ op = SelectObject(hdc, hDotPen);
    HGDIOBJ ob = SelectObject(hdc, hDotBrush);
    Ellipse(hdc, pri_rc.left, timer_cy - 3, pri_rc.left + 6, timer_cy + 3);
    SelectObject(hdc, ob);
    SelectObject(hdc, op);
    DeleteObject(hDotBrush);
    DeleteObject(hDotPen);

    RECT pri_txt_rc{pri_rc.left + 10, status_rc.top, pri_rc.right, status_rc.bottom};
    SelectObject(hdc, hFontSmallBold);
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    DrawTextW(hdc, L"HIGH", -1, &pri_txt_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    if (!rows.empty()) {
        std::string right_fn = rows[0].full_filename_tooltip;
        for (const auto& r : rows) {
            if (r.state == TaskState::kDownloading) {
                right_fn = r.full_filename_tooltip;
                break;
            }
        }
        RECT rfn_rc{w - 320, status_rc.top, w - 16, status_rc.bottom};
        SelectObject(hdc, hFontBody);
        SetTextColor(hdc, pal.text_muted.ToWin32ColorRef());
        std::wstring wrfn = Utf8ToW(right_fn);
        DrawTextW(hdc, wrfn.c_str(), -1, &rfn_rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }

    // 5. Settings Center Overlay (when active)
    if (view.IsSettingsCenterOpen()) {
        PaintSettingsCenterOverlayWin32(hdc, view, hFontBody, hFontBold);
    }

    SelectObject(hdc, old_font);
    DeleteObject(hFontBody);
    DeleteObject(hFontBold);
    DeleteObject(hFontSmall);
    DeleteObject(hFontSmallBold);

    BitBlt(hdc_screen, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    SelectObject(hdc, old_bm);
    DeleteObject(hbm);
    DeleteDC(hdc);
    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK OpenIdmWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto* ctx = reinterpret_cast<Win32WindowContext*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        ctx = static_cast<Win32WindowContext*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ctx));
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    if (!ctx || !ctx->view) {
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    switch (msg) {
        case WM_CREATE: {
            HICON hAppIcon = CreateFluxAppIcon();
            if (hAppIcon) {
                SendMessageW(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hAppIcon));
                SendMessageW(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hAppIcon));
            }
            auto geom = ctx->view->ComputeLayoutGeometry(0);
            ctx->hSearchEdit = CreateWindowExW(
                0, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                geom.search_box_rect.left + 30, geom.search_box_rect.top + 7,
                std::max(10, geom.search_box_rect.width() - 95), std::max(10, geom.search_box_rect.height() - 14),
                hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_SEARCH_EDIT)),
                GetModuleHandleW(nullptr), nullptr);
            HFONT hEditFont = CreateFontW(
                -13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            SendMessageW(ctx->hSearchEdit, WM_SETFONT, reinterpret_cast<WPARAM>(hEditFont), TRUE);
            SendMessageW(ctx->hSearchEdit, EM_SETCUEBANNER, FALSE, reinterpret_cast<LPARAM>(L"Search in downloads..."));
            return 0;
        }
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORSTATIC: {
            HDC hdcEdit = reinterpret_cast<HDC>(wParam);
            HWND hCtl = reinterpret_cast<HWND>(lParam);
            if (hCtl == ctx->hSearchEdit) {
                SetTextColor(hdcEdit, RGB(0xF0, 0xF0, 0xF5));
                SetBkColor(hdcEdit, RGB(0x22, 0x22, 0x26));
                static HBRUSH hEditBrush = CreateSolidBrush(RGB(0x22, 0x22, 0x26));
                return reinterpret_cast<LRESULT>(hEditBrush);
            }
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_SEARCH_EDIT && HIWORD(wParam) == EN_CHANGE) {
                wchar_t buf[512]{};
                GetWindowTextW(ctx->hSearchEdit, buf, 511);
                if (ctx->view->IsSettingsCenterOpen()) {
                    ctx->view->SetSettingsSearchQuery(WToUtf8(buf));
                } else {
                    ctx->view->SetSearchQuery(WToUtf8(buf));
                }
                InvalidateRect(hwnd, nullptr, FALSE);
                return 0;
            }
            return 0;
        }
        case WM_GETMINMAXINFO: {
            auto* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
            mmi->ptMinTrackSize.x = ctx->view->GetMetrics().min_window_width;
            mmi->ptMinTrackSize.y = ctx->view->GetMetrics().min_window_height;
            return 0;
        }
        case WM_DPICHANGED: {
            UINT new_dpi = HIWORD(wParam);
            const auto* suggested = reinterpret_cast<const RECT*>(lParam);
            ctx->view->SetWindowDimensions(
                suggested->right - suggested->left,
                suggested->bottom - suggested->top,
                new_dpi);
            SetWindowPos(
                hwnd, nullptr,
                suggested->left, suggested->top,
                suggested->right - suggested->left,
                suggested->bottom - suggested->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
            auto geom = ctx->view->ComputeLayoutGeometry(0);
            if (ctx->hSearchEdit) {
                MoveWindow(ctx->hSearchEdit, geom.search_box_rect.left + 30, geom.search_box_rect.top + 7,
                           std::max(10, geom.search_box_rect.width() - 95), std::max(10, geom.search_box_rect.height() - 14), TRUE);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_SHOWWINDOW: {
            bool shown = (wParam != FALSE);
            ctx->view->SetWindowVisibility(shown, IsIconic(hwnd) != FALSE);
            return 0;
        }
        case WM_SIZE: {
            if (wParam == SIZE_MINIMIZED) {
                ctx->view->SetWindowVisibility(true, true);
                return 0;
            }
            ctx->view->SetWindowVisibility(IsWindowVisible(hwnd) != FALSE, false);
            UINT dpi = GetDpiForWindow(hwnd);
            ctx->view->SetWindowDimensions(LOWORD(lParam), HIWORD(lParam), dpi > 0 ? dpi : 96);
            auto geom = ctx->view->ComputeLayoutGeometry(0);
            if (ctx->hSearchEdit) {
                MoveWindow(ctx->hSearchEdit, geom.search_box_rect.left + 30, geom.search_box_rect.top + 7,
                           std::max(10, geom.search_box_rect.width() - 95), std::max(10, geom.search_box_rect.height() - 14), TRUE);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_MOUSEMOVE: {
            ctx->view->HandleMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            auto hit = ctx->view->HitTestPoint(x, y, 0);

            if (ctx->view->IsSettingsCenterOpen()) {
                if (hit.type == HitTargetType::kSettingsItemControl && hit.task_id == "default_download_dir") {
                    std::string chosen_dir = ctx->view->GetDraftSettings().default_download_dir;
                    if (SelectFolderWithDialogWin32(hwnd, chosen_dir)) {
                        ctx->view->SetDraftSettingByKey("default_download_dir", chosen_dir);
                    }
                    InvalidateRect(hwnd, nullptr, FALSE);
                    return 0;
                }
                if (hit.type == HitTargetType::kSettingsRestoreDefaultsButton) {
                    if (MessageBoxW(hwnd, L"Restore all Flux settings to their factory defaults?", L"Confirm Restore Defaults", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        ctx->view->ActionRestoreDefaultSettings();
                    }
                    InvalidateRect(hwnd, nullptr, FALSE);
                    return 0;
                }
                ctx->view->HandleMouseClick(x, y, msg == WM_LBUTTONDBLCLK, 0);
                InvalidateRect(hwnd, nullptr, FALSE);
                return 0;
            }

            ctx->view->HandleMouseClick(x, y, msg == WM_LBUTTONDBLCLK, 0);

            if (hit.type == HitTargetType::kToolbarButton || hit.type == HitTargetType::kAddDownloadButton) {
                auto geom = ctx->view->ComputeLayoutGeometry(0);
                if (hit.type == HitTargetType::kAddDownloadButton) {
                    ShowAddUrlQuickDialogWin32(hwnd, *ctx->view, *ctx->controller);
                } else if (hit.index >= 0 && static_cast<size_t>(hit.index) < geom.toolbar_buttons.size()) {
                    auto aid = geom.toolbar_buttons[static_cast<size_t>(hit.index)].action_id;
                    if (aid == ToolbarActionId::kAddUrl) {
                        ShowAddUrlQuickDialogWin32(hwnd, *ctx->view, *ctx->controller);
                    } else if (aid == ToolbarActionId::kOpenFolder) {
                        ctx->view->ActionOpenDestinationFolder(false);
                    }
                }
            } else if (hit.type == HitTargetType::kRowInlineAction || hit.type == HitTargetType::kTaskRowActionButton) {
                ctx->view->SelectTask(hit.task_id);
                ctx->view->TriggerRowPrimaryAction(hit.task_id);
            } else if (hit.type == HitTargetType::kTaskRow && msg == WM_LBUTTONDBLCLK) {
                ctx->view->ActionOpenCompletedFile(false);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_CONTEXTMENU: {
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            POINT client_pt = pt;
            ScreenToClient(hwnd, &client_pt);
            auto hit = ctx->view->HitTestPoint(client_pt.x, client_pt.y, 0);
            if (hit.type == HitTargetType::kTaskRow || hit.type == HitTargetType::kTaskRowActionButton) {
                ctx->view->SelectTask(hit.task_id);
            }
            auto tb = ctx->view->ComputeToolbarState();
            HMENU hMenu = CreatePopupMenu();
            AppendMenuW(hMenu, tb.can_start ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_START, L"Start Download");
            AppendMenuW(hMenu, tb.can_pause ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_PAUSE, L"Pause Download");
            AppendMenuW(hMenu, tb.can_resume ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_RESUME, L"Resume Download");
            AppendMenuW(hMenu, tb.can_retry ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_RETRY, L"Retry Download (F5)");
            AppendMenuW(hMenu, tb.can_cancel ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_CANCEL, L"Cancel Download");
            AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
            AppendMenuW(hMenu, tb.can_open_file ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_OPEN_FILE, L"Open Completed File (Enter)");
            AppendMenuW(hMenu, tb.can_open_folder ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_OPEN_FOLDER, L"Open Destination Folder");
            AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
            AppendMenuW(hMenu, tb.can_remove ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_REMOVE_LIST, L"Remove from List (Delete)");
            AppendMenuW(hMenu, tb.can_delete_file ? MF_STRING : MF_STRING | MF_GRAYED, IDM_CTX_DELETE_DISK, L"Delete File from Disk (Shift+Delete)");
            AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
            AppendMenuW(hMenu, MF_STRING, IDM_CTX_SETTINGS, L"Settings & Configuration (Ctrl+,)...");
            AppendMenuW(hMenu, MF_STRING, IDM_CTX_BROWSER_STAT, L"Browser Integration & Worker Diagnostics...");
            AppendMenuW(hMenu, MF_STRING, IDM_CTX_ABOUT, L"About Flux Download Manager...");

            int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hwnd, nullptr);
            DestroyMenu(hMenu);
            if (cmd == IDM_CTX_START) ctx->view->ActionStartSelected();
            else if (cmd == IDM_CTX_PAUSE) ctx->view->ActionPauseSelected();
            else if (cmd == IDM_CTX_RESUME) ctx->view->ActionResumeSelected();
            else if (cmd == IDM_CTX_RETRY) ctx->view->ActionRetrySelected();
            else if (cmd == IDM_CTX_CANCEL) ctx->view->ActionCancelSelected();
            else if (cmd == IDM_CTX_OPEN_FILE) ctx->view->ActionOpenCompletedFile(false);
            else if (cmd == IDM_CTX_OPEN_FOLDER) ctx->view->ActionOpenDestinationFolder(false);
            else if (cmd == IDM_CTX_REMOVE_LIST) ctx->view->ActionRemoveSelectedFromList();
            else if (cmd == IDM_CTX_DELETE_DISK) {
                if (MessageBoxW(hwnd, L"Permanently delete the downloaded file from disk as well?", L"Confirm File Deletion", MB_YESNO | MB_ICONWARNING) == IDYES) {
                    ctx->view->ActionDeleteSelectedWithFile();
                }
            } else if (cmd == IDM_CTX_SETTINGS) ctx->view->OpenSettingsCenter(SettingsCategory::kGeneral);
            else if (cmd == IDM_CTX_BROWSER_STAT) ctx->view->OpenSettingsCenter(SettingsCategory::kBrowserIntegration);
            else if (cmd == IDM_CTX_ABOUT) ctx->view->OpenSettingsCenter(SettingsCategory::kAboutAndDiagnostics);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_TIMER: {
            if (wParam == IDT_UI_REFRESH) {
                auto sb = ctx->view->BuildStatusBar();
                if (sb.active_downloads > 0) {
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            } else if (wParam == IDT_COSMIC_DRIFT) {
                BOOL os_anim = TRUE;
                SystemParametersInfoW(SPI_GETCLIENTAREAANIMATION, 0, &os_anim, 0);
                bool user_reduced = ctx->controller->GetSettings().reduced_motion || !os_anim;
                if (!user_reduced && ctx->view->GetCosmicStarfield().IsAnimationActive()) {
                    ctx->view->AdvanceCosmicBackground(350);
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
            }
            return 0;
        }
        case WM_KEYDOWN: {
            bool ctrl_down = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            bool shift_down = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
            if (wParam == VK_ESCAPE && ctx->view->IsSettingsCenterOpen()) {
                ctx->view->CloseSettingsCenter(true);
                InvalidateRect(hwnd, nullptr, FALSE);
                return 0;
            }
            if (ctrl_down && (wParam == 'K' || wParam == 'F')) {
                if (ctx->hSearchEdit) {
                    SetFocus(ctx->hSearchEdit);
                    SendMessageW(ctx->hSearchEdit, EM_SETSEL, 0, -1);
                }
                return 0;
            } else if (ctrl_down && wParam == 'N') {
                ShowAddUrlQuickDialogWin32(hwnd, *ctx->view, *ctx->controller);
            } else if (ctrl_down && wParam == VK_OEM_COMMA) {
                ctx->view->OpenSettingsCenter(SettingsCategory::kGeneral);
            } else if (wParam == VK_DOWN) ctx->view->HandleKeyShortcut("VK_DOWN");
            else if (wParam == VK_UP) ctx->view->HandleKeyShortcut("VK_UP");
            else if (wParam == VK_HOME) ctx->view->HandleKeyShortcut("VK_HOME");
            else if (wParam == VK_END) ctx->view->HandleKeyShortcut("VK_END");
            else if (wParam == VK_SPACE) ctx->view->HandleKeyShortcut("VK_SPACE");
            else if (wParam == VK_DELETE) {
                if (shift_down) {
                    if (MessageBoxW(hwnd, L"Permanently delete the downloaded file from disk as well?", L"Confirm File Deletion", MB_YESNO | MB_ICONWARNING) == IDYES) {
                        ctx->view->HandleKeyShortcut("Shift+Delete");
                    }
                } else {
                    ctx->view->HandleKeyShortcut("VK_DELETE");
                }
            } else if (wParam == VK_F5) ctx->view->HandleKeyShortcut("VK_F5");
            else if (wParam == VK_RETURN) ctx->view->ActionOpenCompletedFile(false);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_OPENIDM_TRAYICON: {
            if (lParam == WM_RBUTTONUP) {
                POINT pt{};
                GetCursorPos(&pt);
                HMENU hMenu = CreatePopupMenu();
                AppendMenuW(hMenu, MF_STRING, IDM_TRAY_SHOW, L"Show Flux");
                AppendMenuW(hMenu, MF_STRING, IDM_TRAY_HIDE, L"Hide to Tray");
                AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
                AppendMenuW(hMenu, MF_STRING, IDM_TRAY_PAUSE_ALL, L"Pause All Downloads");
                AppendMenuW(hMenu, MF_STRING, IDM_TRAY_RESUME_ALL, L"Resume All Downloads");
                AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
                AppendMenuW(hMenu, MF_STRING, IDM_TRAY_EXIT, L"Exit Flux");
                SetForegroundWindow(hwnd);
                int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hwnd, nullptr);
                DestroyMenu(hMenu);
                if (cmd == IDM_TRAY_SHOW) {
                    ShowWindow(hwnd, SW_RESTORE);
                    ctx->view->SetWindowVisibility(true, false);
                } else if (cmd == IDM_TRAY_HIDE) {
                    ShowWindow(hwnd, SW_HIDE);
                    ctx->view->SetWindowVisibility(false, false);
                } else if (cmd == IDM_TRAY_PAUSE_ALL) ctx->controller->PauseAllTasks();
                else if (cmd == IDM_TRAY_RESUME_ALL) ctx->controller->ResumeAllTasks();
                else if (cmd == IDM_TRAY_EXIT) DestroyWindow(hwnd);
            } else if (lParam == WM_LBUTTONDBLCLK) {
                ShowWindow(hwnd, SW_RESTORE);
                ctx->view->SetWindowVisibility(true, false);
                SetForegroundWindow(hwnd);
            }
            return 0;
        }
        case WM_CLOSE: {
            if (ctx->controller->GetSettings().close_to_tray && ctx->tray_added) {
                ShowWindow(hwnd, SW_HIDE);
                ctx->view->SetWindowVisibility(false, false);
                return 0;
            }
            DestroyWindow(hwnd);
            return 0;
        }
        case WM_PAINT: {
            PaintMainWindow(hwnd, *ctx->view);
            return 0;
        }
        case WM_DESTROY: {
            KillTimer(hwnd, IDT_UI_REFRESH);
            KillTimer(hwnd, IDT_COSMIC_DRIFT);
            if (ctx->tray_added) {
                Shell_NotifyIconW(NIM_DELETE, &ctx->nid);
                ctx->tray_added = false;
            }
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

} // namespace

int DesktopView::RunWin32MessageLoop() {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    INITCOMMONCONTROLSEX icex{sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES};
    InitCommonControlsEx(&icex);

    HINSTANCE hInst = GetModuleHandleW(nullptr);
    HICON hAppIcon = CreateFluxAppIcon();
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = OpenIdmWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon = hAppIcon ? hAppIcon : LoadIconW(nullptr, IDI_APPLICATION);
    wc.hIconSm = hAppIcon;
    wc.lpszClassName = L"Flux_MainWindowClass";
    RegisterClassExW(&wc);

    Win32WindowContext ctx{this, &m_controller, nullptr, nullptr, {}, false};
    HWND hwnd = CreateWindowExW(
        0,
        wc.lpszClassName,
        L"Flux Download Manager",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        m_window_width, m_window_height,
        nullptr, nullptr, hInst, &ctx);
    if (!hwnd) return 1;
    ctx.hwnd = hwnd;

    BOOL use_dark = TRUE;
    DwmSetWindowAttribute(hwnd, 20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */, &use_dark, sizeof(use_dark));
    COLORREF titlebar_col = RGB(0x16, 0x16, 0x18);
    DwmSetWindowAttribute(hwnd, 35 /* DWMWA_CAPTION_COLOR */, &titlebar_col, sizeof(titlebar_col));
    COLORREF titletext_col = RGB(0xFF, 0xFF, 0xFF);
    DwmSetWindowAttribute(hwnd, 36 /* DWMWA_TEXT_COLOR */, &titletext_col, sizeof(titletext_col));

    ctx.nid.cbSize = sizeof(NOTIFYICONDATAW);
    ctx.nid.hWnd = hwnd;
    ctx.nid.uID = 1;
    ctx.nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    ctx.nid.uCallbackMessage = WM_OPENIDM_TRAYICON;
    ctx.nid.hIcon = wc.hIcon;
    wcsncpy_s(ctx.nid.szTip, L"Flux Download Manager", _TRUNCATE);
    ctx.tray_added = (Shell_NotifyIconW(NIM_ADD, &ctx.nid) != FALSE);

    SetTimer(hwnd, IDT_UI_REFRESH, 100, nullptr);
    SetTimer(hwnd, IDT_COSMIC_DRIFT, 350, nullptr);
    bool start_min = m_controller.GetSettings().start_minimized;
    SetWindowVisibility(!start_min, start_min);
    ShowWindow(hwnd, start_min ? SW_HIDE : SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
#endif

int DesktopView::RunApplicationLoop(bool headless_exit_immediately) {
    SelectFirstRow();
#ifdef _WIN32
    if (!headless_exit_immediately) {
        return RunWin32MessageLoop();
    }
#endif
    if (headless_exit_immediately) {
        return 0;
    }
    return 0;
}

} // namespace openidm::ui
