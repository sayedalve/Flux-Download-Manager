// SPDX-License-Identifier: GPL-3.0-or-later
#include "ui/win32_main_window.hpp"

#include "brisk_engine/http_client.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
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

std::string Trim(std::string_view sv) {
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front()))) sv.remove_prefix(1);
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back()))) sv.remove_suffix(1);
    return std::string(sv);
}

bool ParseBoolString(std::string_view sv, bool& out_val) {
    std::string s = ToLower(Trim(sv));
    if (s == "true" || s == "1" || s == "yes" || s == "on" || s == "enabled") {
        out_val = true;
        return true;
    }
    if (s == "false" || s == "0" || s == "no" || s == "off" || s == "disabled") {
        out_val = false;
        return true;
    }
    return false;
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

} // namespace

DesktopView::DesktopView(DownloadController& controller, ipc::IpcServer* ipc_server)
    : m_controller(controller),
      m_ipc_server(ipc_server),
      m_metrics(DesignSystem::ComputeMetrics(96)),
      m_palette(DesignSystem::GetDarkPalette()),
      m_settings_draft(controller.GetSettings()) {
    m_settings_draft.theme = "dark";
    SetTheme("dark", false);
}

void DesktopView::SetWindowDimensions(int width, int height, uint32_t dpi) {
    m_metrics = DesignSystem::ComputeMetrics(dpi);
    m_window_width = std::max(width, m_metrics.min_window_width);
    m_window_height = std::max(height, m_metrics.min_window_height);
}

void DesktopView::SetWindowVisibility(bool visible, bool minimized) {
    m_window_visible = visible;
    m_window_minimized = minimized;
}

bool DesktopView::IsCosmicAnimationActive() const {
    if (!m_window_visible || m_window_minimized) return false;
    return !m_controller.GetSettings().reduced_motion;
}

void DesktopView::AdvanceCosmicAnimation(int64_t delta_ms) {
    if (delta_ms <= 0 || !IsCosmicAnimationActive()) return;
    m_cosmic_elapsed_ms += delta_ms;
}

CosmicBackgroundState DesktopView::ComputeCosmicBackgroundState(int64_t elapsed_ms) const {
    int64_t eff_ms = (elapsed_ms >= 0) ? elapsed_ms : m_cosmic_elapsed_ms;
    bool reduced_motion = m_controller.GetSettings().reduced_motion;
    bool hidden_or_min = (!m_window_visible || m_window_minimized);
    return DesignSystem::ComputeCosmicBackground(
        m_window_width,
        m_window_height,
        m_metrics.dpi,
        eff_ms,
        reduced_motion,
        hidden_or_min);
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

    g.toolbar_rect = {0, 0, w, m.toolbar_height};
    int search_w = DesignSystem::ScaleForDpi(190, m.dpi);
    int by = (m.toolbar_height - m.button_height) / 2;
    g.search_box_rect = {w - search_w - m.space_md, by, w - m.space_md, by + m.button_height};

    auto tb = ComputeToolbarState();
    struct BtnDef { ToolbarActionId id; const char* label; bool enabled; bool primary; int base_w; };
    const BtnDef defs[] = {
        {ToolbarActionId::kAddUrl,       "+ Add URL",     tb.can_add_url,       true,  90},
        {ToolbarActionId::kStart,        "Start",         tb.can_start,         false, 62},
        {ToolbarActionId::kPause,        "Pause",         tb.can_pause,         false, 62},
        {ToolbarActionId::kResume,       "Resume",        tb.can_resume,        false, 68},
        {ToolbarActionId::kRetry,        "Retry",         tb.can_retry,         false, 62},
        {ToolbarActionId::kCancel,       "Cancel",        tb.can_cancel,        false, 64},
        {ToolbarActionId::kRemove,       "Remove",        tb.can_remove,        false, 68},
        {ToolbarActionId::kClearHistory, "Clear History", tb.can_clear_history, false, 96},
        {ToolbarActionId::kOpenFolder,   "Open Folder",   tb.can_open_folder,   false, 92},
        {ToolbarActionId::kSettings,     "Settings",      tb.can_open_settings, false, 72}
    };

    int bx = m.space_md;
    for (const auto& d : defs) {
        int bw = DesignSystem::ScaleForDpi(d.base_w, m.dpi);
        if (bx + bw > g.search_box_rect.left - m.space_sm) break;
        g.toolbar_buttons.push_back({
            d.id, d.label, d.enabled, d.primary,
            {bx, by, bx + bw, by + m.button_height}
        });
        bx += bw + m.space_xs;
    }

    int content_top = m.toolbar_height + 1;
    int content_bottom = h - m.status_bar_height - 1;
    g.sidebar_rect = {0, content_top, m.sidebar_width, content_bottom};

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
    int cy = content_top + m.space_md;
    for (const auto& [cat, cnt] : cats) {
        RectI crc{m.space_sm, cy, m.sidebar_width - m.space_sm, cy + m.button_height};
        g.sidebar_items.push_back({cat, CategoryToLabel(cat), cnt, cat == m_selected_category, crc});
        cy += m.button_height + m.space_xs;
    }

    int list_left = m.sidebar_width + 1;
    int list_bottom = content_bottom - m.details_panel_height - 1;
    g.table_header_rect = {list_left, content_top, w, content_top + m.table_header_height};
    g.table_body_rect = {list_left, content_top + m.table_header_height, w, list_bottom};
    g.details_panel_rect = {list_left, list_bottom + 1, w, content_bottom};
    g.status_bar_rect = {0, content_bottom + 1, w, h};

    // Table columns layout
    int list_w = std::max(100, w - list_left);
    if (IsCompactColumnsMode()) {
        const std::pair<SortColumn, const char*> cols[] = {
            {SortColumn::kFileName, "Filename"},
            {SortColumn::kStatus,   "Status"},
            {SortColumn::kProgress, "Progress"},
            {SortColumn::kSize,     "Size"},
            {SortColumn::kSpeed,    "Speed"},
            {SortColumn::kCreatedAt,"Actions"}
        };
        const double weights[] = {0.32, 0.22, 0.14, 0.14, 0.10, 0.08};
        int cx = list_left;
        for (size_t i = 0; i < 6; ++i) {
            int cw = (i == 5) ? (w - cx) : static_cast<int>(list_w * weights[i]);
            g.table_columns.push_back({cols[i].first, cols[i].second, {cx, content_top, cx + cw, content_top + m.table_header_height}});
            cx += cw;
        }
    } else {
        const std::pair<SortColumn, const char*> cols[] = {
            {SortColumn::kFileName,     "Filename"},
            {SortColumn::kDownloadType, "Type"},
            {SortColumn::kStatus,       "Status"},
            {SortColumn::kProgress,     "Progress"},
            {SortColumn::kSize,         "Size"},
            {SortColumn::kSpeed,        "Speed"},
            {SortColumn::kEta,          "ETA"},
            {SortColumn::kCreatedAt,    "Actions"}
        };
        const double weights[] = {0.26, 0.10, 0.20, 0.12, 0.12, 0.08, 0.06, 0.06};
        int cx = list_left;
        for (size_t i = 0; i < 8; ++i) {
            int cw = (i == 7) ? (w - cx) : static_cast<int>(list_w * weights[i]);
            g.table_columns.push_back({cols[i].first, cols[i].second, {cx, content_top, cx + cw, content_top + m.table_header_height}});
            cx += cw;
        }
    }

    // Visible task rows layout
    size_t max_rows = static_cast<size_t>(std::max(1, g.table_body_rect.height() / m.table_row_height));
    auto visible_rows = BuildVisibleTaskRows(scroll_offset, max_rows);
    int ry = g.table_body_rect.top;
    int action_btn_w = DesignSystem::ScaleForDpi(64, m.dpi);
    int action_btn_h = DesignSystem::ScaleForDpi(24, m.dpi);

    for (const auto& r : visible_rows) {
        if (ry + m.table_row_height > g.table_body_rect.bottom) break;
        RectI row_rc{list_left, ry, w, ry + m.table_row_height};
        RectI fn_rc = g.table_columns.empty()
            ? RectI{list_left + m.space_sm, ry, list_left + 220, ry + m.table_row_height}
            : RectI{g.table_columns[0].rect.left, ry, g.table_columns[0].rect.right, ry + m.table_row_height};

        // Progress bar positioned cleanly inside the Progress column or bottom of row
        size_t prog_col_idx = IsCompactColumnsMode() ? 2 : 3;
        RectI prog_rc{list_left + m.space_md, ry + m.table_row_height - 9, list_left + DesignSystem::ScaleForDpi(320, m.dpi), ry + m.table_row_height - 4};
        if (prog_col_idx < g.table_columns.size()) {
            const auto& pc = g.table_columns[prog_col_idx].rect;
            prog_rc = {pc.left + m.space_sm, ry + m.table_row_height - DesignSystem::ScaleForDpi(11, m.dpi),
                       pc.right - m.space_sm, ry + m.table_row_height - DesignSystem::ScaleForDpi(5, m.dpi)};
        }

        RectI act_rc{w - action_btn_w - m.space_md, ry + (m.table_row_height - action_btn_h) / 2, w - m.space_md, ry + (m.table_row_height + action_btn_h) / 2};

        TaskRowLayout trl{r.task_id, row_rc, fn_rc, prog_rc, act_rc, {}};
        trl.cell_rects.reserve(g.table_columns.size());
        for (const auto& col : g.table_columns) {
            trl.cell_rects.push_back({col.rect.left, ry, col.rect.right, ry + m.table_row_height});
        }
        g.task_rows.push_back(std::move(trl));
        ry += m.table_row_height;
    }

    return g;
}

HitTestResult DesktopView::HitTestPoint(int x, int y, size_t scroll_offset) const {
    auto geom = ComputeLayoutGeometry(scroll_offset);
    if (geom.search_box_rect.Contains(x, y)) {
        return {HitTargetType::kSearchBox, 0, "", "Search downloads by filename or URL (Ctrl+F)"};
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
        if (geom.task_rows[i].action_button_rect.Contains(x, y)) {
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
    auto geom = ComputeLayoutGeometry(scroll_offset);
    auto hit = HitTestPoint(x, y, scroll_offset);

    switch (hit.type) {
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

void DesktopView::SetTheme(std::string_view /*theme_name*/, bool /*os_prefers_light*/) {
    // Minimal Cosmic Dark UI: keep the entire application in dark mode.
    m_theme_name = "dark";
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

Result<uint64_t> DesktopView::ParseBandwidthLimitInput(std::string_view text) {
    std::string s = ToLower(Trim(text));
    if (s.empty() || s == "0" || s == "unlimited" || s == "none" || s == "off") {
        return Result<uint64_t>::Ok(0);
    }

    // Extract leading numeric part
    size_t idx = 0;
    bool seen_dot = false;
    while (idx < s.size() && (std::isdigit(static_cast<unsigned char>(s[idx])) || (s[idx] == '.' && !seen_dot))) {
        if (s[idx] == '.') seen_dot = true;
        ++idx;
    }
    if (idx == 0) {
        return Result<uint64_t>::Err("ERR_SETTINGS_INVALID_BANDWIDTH", "Enter 0 for Unlimited or a numeric speed (e.g., 512 KiB/s, 5 MiB/s)");
    }

    double val = 0.0;
    try {
        val = std::stod(s.substr(0, idx));
    } catch (...) {
        return Result<uint64_t>::Err("ERR_SETTINGS_INVALID_BANDWIDTH", "Invalid numeric bandwidth limit");
    }
    if (!std::isfinite(val) || val < 0.0) {
        return Result<uint64_t>::Err("ERR_SETTINGS_INVALID_BANDWIDTH", "Bandwidth limit cannot be negative");
    }
    if (val == 0.0) {
        return Result<uint64_t>::Ok(0);
    }

    std::string unit = Trim(s.substr(idx));
    uint64_t multiplier = 1ULL;
    if (unit.empty() || unit == "b" || unit == "b/s" || unit == "bps") {
        multiplier = 1ULL;
    } else if (unit == "k" || unit == "kb" || unit == "kib" || unit == "kb/s" || unit == "kib/s" || unit == "kbps") {
        multiplier = 1024ULL;
    } else if (unit == "m" || unit == "mb" || unit == "mib" || unit == "mb/s" || unit == "mib/s" || unit == "mbps") {
        multiplier = 1048576ULL;
    } else if (unit == "g" || unit == "gb" || unit == "gib" || unit == "gb/s" || unit == "gib/s") {
        multiplier = 1073741824ULL;
    } else {
        return Result<uint64_t>::Err("ERR_SETTINGS_INVALID_BANDWIDTH", "Unrecognized unit '" + unit + "'. Use KiB/s or MiB/s.");
    }

    double total_bps_d = val * static_cast<double>(multiplier);
    if (total_bps_d > static_cast<double>(10ULL * 1073741824ULL)) {
        return Result<uint64_t>::Err("ERR_SETTINGS_INVALID_BANDWIDTH", "Bandwidth limit exceeds maximum supported rate (10 GiB/s)");
    }
    uint64_t bps = static_cast<uint64_t>(std::llround(total_bps_d));
    if (bps > 0 && bps < 16384ULL) {
        return Result<uint64_t>::Err("ERR_SETTINGS_SPEED_TOO_LOW", "Bandwidth limit must be Unlimited (0) or at least 16 KiB/s");
    }
    return Result<uint64_t>::Ok(bps);
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
                   t.task_type == TaskType::kStreamMss || t.task_type == TaskType::kYouTubeExtraction ||
                   t.category == "Video";
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
        row.task_state = t.state;
        row.full_filename_tooltip = t.file_name;
        row.display_filename = TruncateFilenameGracefully(t.file_name, max_fn_chars);
        row.is_filename_truncated = (row.display_filename != t.file_name);
        row.is_stream = (t.task_type != TaskType::kHttpFile);
        row.badge_color = row.is_stream ? m_palette.badge_stream : m_palette.badge_file;

        if (t.task_type == TaskType::kStreamHls) row.file_type_badge = "HLS STREAM";
        else if (t.task_type == TaskType::kStreamDash) row.file_type_badge = "DASH STREAM";
        else if (t.task_type == TaskType::kYouTubeExtraction) row.file_type_badge = "YOUTUBE";
        else row.file_type_badge = t.range_supported ? "HTTP MULTI" : "HTTP FILE";

        switch (t.state) {
            case TaskState::kQueued:
                row.status_tone = "queued";
                row.status_color = m_palette.status_queued;
                break;
            case TaskState::kProbing:
            case TaskState::kMuxing:
            case TaskState::kVerifying:
                row.status_tone = "probing";
                row.status_color = m_palette.badge_stream;
                break;
            case TaskState::kDownloading:
                row.status_tone = "active";
                row.status_color = m_palette.accent_cyan;
                break;
            case TaskState::kPaused:
                row.status_tone = "paused";
                row.status_color = m_palette.status_warning;
                break;
            case TaskState::kCompleted:
                row.status_tone = "completed";
                row.status_color = m_palette.status_success;
                break;
            case TaskState::kFailed:
                row.status_tone = "failed";
                row.status_color = m_palette.status_danger;
                break;
            case TaskState::kCanceled:
                row.status_tone = "canceled";
                row.status_color = m_palette.text_muted;
                break;
        }

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

        if (t.total_bytes > 0) {
            row.size_text = FormatBytes(t.downloaded_bytes) + " / " + FormatBytes(t.total_bytes);
        } else {
            row.size_text = FormatBytes(t.downloaded_bytes);
        }

        row.speed_text = FormatSpeed(t.speed_bps);
        uint64_t rem_bytes = (t.total_bytes > t.downloaded_bytes) ? (t.total_bytes - t.downloaded_bytes) : 0;
        row.eta_text = (t.state == TaskState::kDownloading) ? FormatEta(rem_bytes, t.speed_bps) : "—";

        switch (t.state) {
            case TaskState::kDownloading:
            case TaskState::kProbing:
                row.primary_action_label = "Pause";
                row.primary_action_enabled = true;
                break;
            case TaskState::kPaused:
                row.primary_action_label = "Resume";
                row.primary_action_enabled = true;
                break;
            case TaskState::kQueued:
                row.primary_action_label = "Pause";
                row.primary_action_enabled = true;
                break;
            case TaskState::kFailed:
            case TaskState::kCanceled:
                row.primary_action_label = "Retry";
                row.primary_action_enabled = true;
                break;
            case TaskState::kCompleted:
                row.primary_action_label = "Open";
                row.primary_action_enabled = true;
                break;
            default:
                row.primary_action_label = "Wait";
                row.primary_action_enabled = false;
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
    } else if (t.task_type == TaskType::kYouTubeExtraction) {
        vm.engine_name = "YouTube Extractor (yt-dlp / Adaptive Track Muxer) + FFmpeg";
        vm.stream_format = "YouTube Adaptive / Progressive Stream";
        vm.selected_tracks_summary = "Video: " + t.stream_options.video_filter +
                                     " | Audio: " + t.stream_options.audio_filter +
                                     " | Container: ." + t.stream_options.output_format;
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
    auto settings = m_controller.GetSettings();
    sb.formatted_total_speed = FormatSpeed(sb.total_speed_bps);
    sb.formatted_speed_limit = FormatBandwidthLimit(settings.speed_limit_bps);
    sb.browser_ipc_listening = (m_ipc_server != nullptr);
    sb.browser_capture_enabled = settings.browser_capture_enabled;
    sb.workers_ready = m_controller.AreWorkersAvailable();
    sb.cosmic_motion_active = IsCosmicAnimationActive();

    std::ostringstream msg;
    msg << "Active: " << sb.active_downloads
        << " | Speed: " << sb.formatted_total_speed
        << " (Limit: " << sb.formatted_speed_limit << ")"
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

AppSettings DesktopView::BuildDefaultAppSettings(const DownloadController& controller) {
    AppSettings defaults;
    defaults.theme = "dark";
    defaults.max_concurrent_tasks = 3;
    defaults.speed_limit_bps = 0;
    defaults.default_connections_per_task = 8;
    std::string def_dir = PathSanitizer::GetDefaultDownloadsDir(PathSanitizer::GetExecutableDir()).string();
    if (def_dir.empty()) {
        def_dir = controller.GetConfig().default_download_dir.string();
    }
    defaults.default_download_dir = def_dir;
    defaults.browser_capture_enabled = true;
    defaults.close_to_tray = true;
    defaults.start_minimized = false;
    defaults.reduced_motion = false;
    return defaults;
}

std::string DesktopView::SettingsCategoryToLabel(SettingsCategory category) {
    switch (category) {
        case SettingsCategory::kGeneral:            return "General";
        case SettingsCategory::kDownloads:          return "Downloads";
        case SettingsCategory::kSaveLocations:      return "Save Locations";
        case SettingsCategory::kConnections:        return "Connections";
        case SettingsCategory::kSpeedLimits:        return "Speed Limits";
        case SettingsCategory::kBrowserIntegration: return "Browser Integration";
        case SettingsCategory::kAboutAndDiagnostics:return "About & Diagnostics";
    }
    return "General";
}

void DesktopView::OpenSettingsCenter(SettingsCategory initial_category) {
    m_settings_open = true;
    m_settings_category = initial_category;
    m_settings_draft = m_controller.GetSettings();
    m_settings_draft.theme = "dark";
    m_settings_validation_error.clear();
    m_last_feedback_message = "Opened Settings Center (" + SettingsCategoryToLabel(initial_category) + ")";
}

void DesktopView::CloseSettingsCenter(bool discard_unsaved) {
    if (discard_unsaved) {
        m_settings_draft = m_controller.GetSettings();
        m_settings_draft.theme = "dark";
        m_settings_validation_error.clear();
    }
    m_settings_open = false;
}

void DesktopView::SelectSettingsCategory(SettingsCategory category) {
    m_settings_category = category;
}

void DesktopView::SetSettingsSearchQuery(std::string query) {
    m_settings_search_query = std::move(query);
}

void DesktopView::SetShowAdvancedSettings(bool show_advanced) {
    m_settings_show_advanced = show_advanced;
}

void DesktopView::SetSettingsDraft(const AppSettings& draft) {
    m_settings_draft = draft;
    m_settings_draft.theme = "dark";
    auto v = ValidateSettingsDraft();
    m_settings_validation_error = v.ok() ? "" : v.error->message;
}

Result<void> DesktopView::SetSettingsDraftField(std::string_view key, std::string_view value_str) {
    std::string k = ToLower(Trim(key));
    m_settings_validation_error.clear();

    if (k == "start_minimized" || k == "general.start_minimized") {
        bool b = false;
        if (!ParseBoolString(value_str, b)) {
            m_settings_validation_error = "Start minimized requires a boolean value (true/false).";
            return Result<void>::Err("ERR_SETTINGS_INVALID_BOOL", m_settings_validation_error);
        }
        m_settings_draft.start_minimized = b;
        return Result<void>::Ok();
    }

    if (k == "close_to_tray" || k == "general.close_to_tray") {
        bool b = false;
        if (!ParseBoolString(value_str, b)) {
            m_settings_validation_error = "Close to tray requires a boolean value (true/false).";
            return Result<void>::Err("ERR_SETTINGS_INVALID_BOOL", m_settings_validation_error);
        }
        m_settings_draft.close_to_tray = b;
        return Result<void>::Ok();
    }

    if (k == "reduced_motion" || k == "general.reduced_motion") {
        bool b = false;
        if (!ParseBoolString(value_str, b)) {
            m_settings_validation_error = "Reduced motion requires a boolean value (true/false).";
            return Result<void>::Err("ERR_SETTINGS_INVALID_BOOL", m_settings_validation_error);
        }
        m_settings_draft.reduced_motion = b;
        return Result<void>::Ok();
    }

    if (k == "max_concurrent_tasks" || k == "downloads.max_concurrent_tasks") {
        int val = 0;
        try {
            std::string trimmed = Trim(value_str);
            size_t pos = 0;
            val = std::stoi(trimmed, &pos);
            if (pos != trimmed.size()) throw std::invalid_argument("trailing chars");
        } catch (...) {
            m_settings_validation_error = "Simultaneous downloads must be an integer between 1 and 16.";
            return Result<void>::Err("ERR_SETTINGS_CONCURRENT_RANGE", m_settings_validation_error);
        }
        if (val < 1 || val > 16) {
            m_settings_validation_error = "Simultaneous downloads must be between 1 and 16.";
            return Result<void>::Err("ERR_SETTINGS_CONCURRENT_RANGE", m_settings_validation_error);
        }
        m_settings_draft.max_concurrent_tasks = static_cast<uint32_t>(val);
        return Result<void>::Ok();
    }

    if (k == "default_download_dir" || k == "locations.default_download_dir") {
        std::string dir = Trim(value_str);
        if (dir.empty()) {
            m_settings_validation_error = "Default download directory cannot be empty.";
            return Result<void>::Err("ERR_SETTINGS_EMPTY_DIR", m_settings_validation_error);
        }
        for (unsigned char c : dir) {
            if (c < 0x20 || c == 0x7F) {
                m_settings_validation_error = "Default download directory contains invalid control characters.";
                return Result<void>::Err("ERR_SETTINGS_INVALID_DIR", m_settings_validation_error);
            }
        }
        m_settings_draft.default_download_dir = dir;
        return Result<void>::Ok();
    }

    if (k == "default_connections_per_task" || k == "connections.default_connections_per_task") {
        int val = 0;
        try {
            std::string trimmed = Trim(value_str);
            size_t pos = 0;
            val = std::stoi(trimmed, &pos);
            if (pos != trimmed.size()) throw std::invalid_argument("trailing chars");
        } catch (...) {
            m_settings_validation_error = "Default connections per task must be an integer between 1 and 32.";
            return Result<void>::Err("ERR_SETTINGS_CONNECTIONS_RANGE", m_settings_validation_error);
        }
        if (val < 1 || val > 32) {
            m_settings_validation_error = "Default connections per task must be between 1 and 32.";
            return Result<void>::Err("ERR_SETTINGS_CONNECTIONS_RANGE", m_settings_validation_error);
        }
        m_settings_draft.default_connections_per_task = static_cast<uint32_t>(val);
        return Result<void>::Ok();
    }

    if (k == "speed_limit_bps" || k == "speed.speed_limit_bps") {
        auto parsed = ParseBandwidthLimitInput(value_str);
        if (!parsed.ok()) {
            m_settings_validation_error = parsed.error->message;
            return Result<void>::Err(parsed.error->code, parsed.error->message);
        }
        m_settings_draft.speed_limit_bps = *parsed.value;
        return Result<void>::Ok();
    }

    if (k == "browser_capture_enabled" || k == "browser.capture_enabled") {
        bool b = false;
        if (!ParseBoolString(value_str, b)) {
            m_settings_validation_error = "Browser capture requires a boolean value (true/false).";
            return Result<void>::Err("ERR_SETTINGS_INVALID_BOOL", m_settings_validation_error);
        }
        m_settings_draft.browser_capture_enabled = b;
        return Result<void>::Ok();
    }

    return Result<void>::Err("ERR_SETTINGS_UNKNOWN_KEY", "Unknown setting key: " + std::string(key));
}

Result<void> DesktopView::ResetSettingsDraftField(std::string_view key) {
    std::string k = ToLower(Trim(key));
    auto defaults = BuildDefaultAppSettings(m_controller);
    m_settings_validation_error.clear();

    if (k == "start_minimized" || k == "general.start_minimized") {
        m_settings_draft.start_minimized = defaults.start_minimized;
    } else if (k == "close_to_tray" || k == "general.close_to_tray") {
        m_settings_draft.close_to_tray = defaults.close_to_tray;
    } else if (k == "reduced_motion" || k == "general.reduced_motion") {
        m_settings_draft.reduced_motion = defaults.reduced_motion;
    } else if (k == "max_concurrent_tasks" || k == "downloads.max_concurrent_tasks") {
        m_settings_draft.max_concurrent_tasks = defaults.max_concurrent_tasks;
    } else if (k == "default_download_dir" || k == "locations.default_download_dir") {
        m_settings_draft.default_download_dir = defaults.default_download_dir;
    } else if (k == "default_connections_per_task" || k == "connections.default_connections_per_task") {
        m_settings_draft.default_connections_per_task = defaults.default_connections_per_task;
    } else if (k == "speed_limit_bps" || k == "speed.speed_limit_bps") {
        m_settings_draft.speed_limit_bps = defaults.speed_limit_bps;
    } else if (k == "browser_capture_enabled" || k == "browser.capture_enabled") {
        m_settings_draft.browser_capture_enabled = defaults.browser_capture_enabled;
    } else {
        return Result<void>::Err("ERR_SETTINGS_UNKNOWN_KEY", "Unknown setting key: " + std::string(key));
    }
    return Result<void>::Ok();
}

Result<void> DesktopView::ValidateSettingsDraft() const {
    if (m_settings_draft.max_concurrent_tasks < 1 || m_settings_draft.max_concurrent_tasks > 16) {
        return Result<void>::Err("ERR_SETTINGS_CONCURRENT_RANGE", "Simultaneous downloads must be between 1 and 16.");
    }
    if (m_settings_draft.default_connections_per_task < 1 || m_settings_draft.default_connections_per_task > 32) {
        return Result<void>::Err("ERR_SETTINGS_CONNECTIONS_RANGE", "Default connections per task must be between 1 and 32.");
    }
    if (Trim(m_settings_draft.default_download_dir).empty()) {
        return Result<void>::Err("ERR_SETTINGS_EMPTY_DIR", "Default download directory cannot be empty.");
    }
    for (unsigned char c : m_settings_draft.default_download_dir) {
        if (c < 0x20 || c == 0x7F) {
            return Result<void>::Err("ERR_SETTINGS_INVALID_DIR", "Default download directory contains invalid control characters.");
        }
    }
    if (m_settings_draft.speed_limit_bps > 0 && m_settings_draft.speed_limit_bps < 16384ULL) {
        return Result<void>::Err("ERR_SETTINGS_SPEED_TOO_LOW", "Bandwidth limit must be Unlimited (0) or at least 16 KiB/s.");
    }
    return Result<void>::Ok();
}

Result<void> DesktopView::ApplySettingsDraft() {
    if (!m_settings_validation_error.empty()) {
        m_last_feedback_message = "Validation error: " + m_settings_validation_error;
        return Result<void>::Err("ERR_SETTINGS_VALIDATION", m_settings_validation_error);
    }
    auto val = ValidateSettingsDraft();
    if (!val.ok()) {
        m_settings_validation_error = val.error->message;
        m_last_feedback_message = "Validation error: " + val.error->message;
        return val;
    }
    m_settings_draft.theme = "dark";
    auto save_res = ActionSaveSettings(m_settings_draft);
    if (save_res.ok()) {
        m_settings_validation_error.clear();
        m_last_feedback_message = "Settings applied and saved";
    }
    return save_res;
}

void DesktopView::CancelSettingsDraft() {
    m_settings_draft = m_controller.GetSettings();
    m_settings_draft.theme = "dark";
    m_settings_validation_error.clear();
    m_last_feedback_message = "Settings changes discarded";
}

std::vector<FileCategoryRuleView> DesktopView::GetSupportedCategoryRules() const {
    std::filesystem::path base_dir(
        m_settings_draft.default_download_dir.empty()
            ? m_controller.GetSettings().default_download_dir
            : m_settings_draft.default_download_dir);

    return {
        {"Video",      (base_dir / "Video").string(),      "mp4, mkv, webm, avi, mov, flv, ts, m4v, wmv"},
        {"Audio",      (base_dir / "Audio").string(),      "mp3, m4a, flac, wav, ogg, aac, opus"},
        {"Compressed", (base_dir / "Compressed").string(), "zip, rar, 7z, tar, gz, bz2, xz, tgz, zst"},
        {"Documents",  (base_dir / "Documents").string(),  "pdf, doc, docx, xls, xlsx, ppt, pptx, txt, md, epub, csv"},
        {"Programs",   (base_dir / "Programs").string(),   "exe, msi, iso, img, dmg, deb, rpm, apk, msix"},
        {"General",    base_dir.string(),                  "All other file types (saved directly to base folder)"}
    };
}

std::vector<SettingsItemViewModel> DesktopView::BuildAllSettingsItems(
    const AppSettings& draft,
    const AppSettings& committed,
    const AppSettings& defaults) const {

    auto br = GetBrowserIntegrationStatus();
    auto ab = GetAboutDialogView();
    const auto& cfg = m_controller.GetConfig();

    std::vector<SettingsItemViewModel> items;
    items.reserve(16);

    // 1. General
    items.push_back({
        "general.start_minimized",
        SettingsCategory::kGeneral,
        SettingsControlType::kToggleSwitch,
        "Start Minimized to System Tray",
        "Launch Flux silently in the Windows notification area without opening the main window on startup.",
        draft.start_minimized ? "Enabled" : "Disabled",
        defaults.start_minimized ? "Enabled" : "Disabled",
        draft.start_minimized != committed.start_minimized,
        false, false, false, true, ""
    });

    items.push_back({
        "general.close_to_tray",
        SettingsCategory::kGeneral,
        SettingsControlType::kToggleSwitch,
        "Close Main Window to System Tray",
        "Keep Flux running in the background and listening for browser downloads when closing the main window.",
        draft.close_to_tray ? "Enabled" : "Disabled",
        defaults.close_to_tray ? "Enabled" : "Disabled",
        draft.close_to_tray != committed.close_to_tray,
        false, false, false, true, ""
    });

    items.push_back({
        "general.reduced_motion",
        SettingsCategory::kGeneral,
        SettingsControlType::kToggleSwitch,
        "Reduce Cosmic Background Motion",
        "Suspend the slow star-field drift and render the static cosmic backdrop.",
        draft.reduced_motion ? "Enabled (Static Fallback)" : "Disabled (Subtle Drift)",
        defaults.reduced_motion ? "Enabled (Static Fallback)" : "Disabled (Subtle Drift)",
        draft.reduced_motion != committed.reduced_motion,
        false, false, false, true, ""
    });

    items.push_back({
        "general.theme",
        SettingsCategory::kGeneral,
        SettingsControlType::kReadOnlyStatus,
        "Interface Theme",
        "Flux uses a dedicated minimal cosmic dark palette across the desktop application and browser extension.",
        "Cosmic Dark (Active)",
        "Cosmic Dark (Active)",
        false, false, false, false, true, ""
    });

    // 2. Downloads
    std::string conc_err;
    if (draft.max_concurrent_tasks < 1 || draft.max_concurrent_tasks > 16) {
        conc_err = "Simultaneous downloads must be between 1 and 16.";
    }
    items.push_back({
        "downloads.max_concurrent_tasks",
        SettingsCategory::kDownloads,
        SettingsControlType::kNumericStepper,
        "Max Simultaneous Downloads",
        "Maximum number of active file and stream transfers running concurrently (1 to 16); extra tasks wait in the queue.",
        std::to_string(draft.max_concurrent_tasks) + " active task(s)",
        std::to_string(defaults.max_concurrent_tasks) + " active task(s)",
        draft.max_concurrent_tasks != committed.max_concurrent_tasks,
        false, false, false, true, conc_err
    });

    items.push_back({
        "downloads.conflict_policy",
        SettingsCategory::kDownloads,
        SettingsControlType::kReadOnlyStatus,
        "File Conflict & Part-File Safety",
        "In-progress transfers write to .openidm.part and atomically auto-rename on completion so existing files are never overwritten.",
        "Atomic Commit + Auto-Rename (file (1).ext)",
        "Atomic Commit + Auto-Rename (file (1).ext)",
        false, true, false, false, true, ""
    });

    items.push_back({
        "downloads.clear_history",
        SettingsCategory::kDownloads,
        SettingsControlType::kActionButton,
        "Clear Completed & Canceled History",
        "Remove completed and canceled transfer records from the queue list while preserving all downloaded files on disk.",
        "Clear History...",
        "",
        false, false, false, true, ComputeToolbarState().can_clear_history, ""
    });

    // 3. Save Locations
    std::string dir_err;
    if (Trim(draft.default_download_dir).empty()) {
        dir_err = "Default download directory cannot be empty.";
    }
    items.push_back({
        "locations.default_download_dir",
        SettingsCategory::kSaveLocations,
        SettingsControlType::kFolderSelector,
        "Default Download Directory",
        "Base destination directory for completed downloads. Missing folders are created automatically when saving.",
        draft.default_download_dir,
        defaults.default_download_dir,
        draft.default_download_dir != committed.default_download_dir,
        false, false, false, true, dir_err
    });

    items.push_back({
        "locations.category_rules",
        SettingsCategory::kSaveLocations,
        SettingsControlType::kCategoryRuleTable,
        "Automatic Category Subfolder Routing",
        "Flux automatically routes files into category subfolders (Video, Audio, Compressed, Documents, Programs) based on file extension.",
        "6 Category Rules Active",
        "6 Category Rules Active",
        false, false, false, false, true, ""
    });

    // 4. Connections
    std::string conn_err;
    if (draft.default_connections_per_task < 1 || draft.default_connections_per_task > 32) {
        conn_err = "Default connections per task must be between 1 and 32.";
    }
    items.push_back({
        "connections.default_connections_per_task",
        SettingsCategory::kConnections,
        SettingsControlType::kNumericStepper,
        "Default Connections per Task",
        "Maximum concurrent byte-range segments spawned per HTTP/HTTPS file transfer by the Brisk work-stealing engine (1 to 32).",
        std::to_string(draft.default_connections_per_task) + " connection(s)",
        std::to_string(defaults.default_connections_per_task) + " connection(s)",
        draft.default_connections_per_task != committed.default_connections_per_task,
        false, false, false, true, conn_err
    });

    items.push_back({
        "connections.stall_and_recovery",
        SettingsCategory::kConnections,
        SettingsControlType::kReadOnlyStatus,
        "Work-Stealing & Crash Recovery Parameters",
        "Dynamic segment splitting, hung-connection stall reset, and WAL crash-recovery byte rollback.",
        "Min Split: " + FormatBytes(cfg.min_split_bytes) +
            " | Stall Timeout: " + std::to_string(cfg.stall_timeout_ms) + " ms | Crash Rollback: 64.0 KiB",
        "Min Split: 128.0 KiB | Stall Timeout: 3000 ms | Crash Rollback: 64.0 KiB",
        false, true, false, false, true, ""
    });

    // 5. Speed Limits
    std::string speed_err;
    if (draft.speed_limit_bps > 0 && draft.speed_limit_bps < 16384ULL) {
        speed_err = "Bandwidth limit must be Unlimited (0) or at least 16 KiB/s.";
    }
    items.push_back({
        "speed.speed_limit_bps",
        SettingsCategory::kSpeedLimits,
        SettingsControlType::kBandwidthControl,
        "Global Bandwidth Limit",
        "Token-bucket transfer rate ceiling applied immediately across active downloads. Set to 0 for unlimited speed.",
        FormatBandwidthLimit(draft.speed_limit_bps),
        FormatBandwidthLimit(defaults.speed_limit_bps),
        draft.speed_limit_bps != committed.speed_limit_bps,
        false, false, false, true, speed_err
    });

    // 6. Browser Integration
    items.push_back({
        "browser.capture_enabled",
        SettingsCategory::kBrowserIntegration,
        SettingsControlType::kToggleSwitch,
        "Capture Downloads & Media from Browsers",
        "Automatically intercept eligible browser downloads and video streams sent by the Flux Browser Extension.",
        draft.browser_capture_enabled ? "Enabled" : "Paused",
        defaults.browser_capture_enabled ? "Enabled" : "Paused",
        draft.browser_capture_enabled != committed.browser_capture_enabled,
        false, false, false, true, ""
    });

    items.push_back({
        "browser.ipc_status",
        SettingsCategory::kBrowserIntegration,
        SettingsControlType::kReadOnlyStatus,
        "Native Messaging & Local IPC Bridge",
        "Authenticated Native Messaging host (com.flux.downloadmanager) connected to the per-user IPC endpoint.",
        std::string(br.ipc_server_running ? "Listening (" : "Offline (") + br.ipc_endpoint + ")",
        "Listening",
        false, false, false, false, true, ""
    });

    items.push_back({
        "browser.intercept_rules",
        SettingsCategory::kBrowserIntegration,
        SettingsControlType::kReadOnlyStatus,
        "Interception Rules & Alt-Key Bypass",
        "Hold Alt while clicking any link in your browser to bypass Flux and use the browser's built-in download manager.",
        "Min Fallback Size: " + FormatBytes(br.min_capture_size_bytes) + " | Alt-Click Bypass Enabled",
        "Min Fallback Size: 1.0 MiB | Alt-Click Bypass Enabled",
        false, true, false, false, true, ""
    });

    // 7. About & Diagnostics
    items.push_back({
        "about.version",
        SettingsCategory::kAboutAndDiagnostics,
        SettingsControlType::kReadOnlyStatus,
        "Application Version & License",
        ab.target_platform,
        ab.app_name + " v" + ab.version + " (" + ab.license + ")",
        ab.app_name + " v" + ab.version,
        false, false, false, false, true, ""
    });

    items.push_back({
        "about.workers",
        SettingsCategory::kAboutAndDiagnostics,
        SettingsControlType::kReadOnlyStatus,
        "Stream Engine & Media Muxer Workers",
        "Out-of-process HLS/DASH manifest prober/downloader and container muxer status.",
        std::string("N_m3u8DL-RE: ") + (br.nm3u8dl_available ? "Ready" : "Missing") +
            " | FFmpeg: " + (br.ffmpeg_available ? "Ready" : "Missing"),
        "N_m3u8DL-RE: Ready | FFmpeg: Ready",
        false, false, false, false, true, ""
    });

    items.push_back({
        "about.security",
        SettingsCategory::kAboutAndDiagnostics,
        SettingsControlType::kReadOnlyStatus,
        "Storage & Credential Security",
        "SQLite WAL database with DPAPI / AES-256-GCM encrypted session headers and automatic log redaction.",
        "State DB: " + cfg.state_db_path.filename().string() + " (Encrypted Auth + Redacted Logs)",
        "Encrypted Auth + Redacted Logs",
        false, true, false, false, true, ""
    });

    items.push_back({
        "about.restore_defaults",
        SettingsCategory::kAboutAndDiagnostics,
        SettingsControlType::kActionButton,
        "Restore Default Settings",
        "Reset concurrency, connection counts, speed limits, download directory, and tray preferences to factory defaults.",
        "Restore Defaults...",
        "",
        false, false, true, true, true, ""
    });

    return items;
}

SettingsCenterViewModel DesktopView::BuildSettingsCenterViewModel() const {
    SettingsCenterViewModel vm;
    vm.is_open = m_settings_open;
    vm.selected_category = m_settings_category;
    vm.search_query = m_settings_search_query;
    vm.show_advanced = m_settings_show_advanced;
    vm.committed_settings = m_controller.GetSettings();
    vm.draft_settings = m_settings_draft;
    vm.category_rules = GetSupportedCategoryRules();
    vm.browser_status = GetBrowserIntegrationStatus();
    vm.about_info = GetAboutDialogView();

    auto defaults = BuildDefaultAppSettings(m_controller);
    auto all_items = BuildAllSettingsItems(m_settings_draft, vm.committed_settings, defaults);

    const std::pair<SettingsCategory, const char*> cat_defs[] = {
        {SettingsCategory::kGeneral,            "Startup, system tray & cosmic motion"},
        {SettingsCategory::kDownloads,          "Simultaneous queue limits & history"},
        {SettingsCategory::kSaveLocations,      "Default directory & category folders"},
        {SettingsCategory::kConnections,        "Segmented HTTP connections & recovery"},
        {SettingsCategory::kSpeedLimits,        "Global token-bucket bandwidth ceiling"},
        {SettingsCategory::kBrowserIntegration, "Extension capture, IPC & bypass rules"},
        {SettingsCategory::kAboutAndDiagnostics,"Version, workers, security & reset"}
    };

    int sy = m_metrics.space_md;
    for (const auto& [cat, sub] : cat_defs) {
        SettingsCategorySummary cs;
        cs.category = cat;
        cs.title = SettingsCategoryToLabel(cat);
        cs.subtitle = sub;
        cs.selected = (cat == m_settings_category);
        cs.rect = {m_metrics.space_sm, sy, m_metrics.settings_sidebar_width - m_metrics.space_sm, sy + m_metrics.button_height};
        sy += m_metrics.button_height + m_metrics.space_xs;

        for (const auto& item : all_items) {
            if (item.category == cat) {
                ++cs.item_count;
                if (item.is_modified) {
                    ++cs.modified_count;
                    vm.has_unsaved_changes = true;
                }
            }
        }
        vm.categories.push_back(std::move(cs));
    }

    std::string q = ToLower(Trim(m_settings_search_query));
    for (auto& item : all_items) {
        if (!m_settings_show_advanced && item.is_advanced && q.empty()) {
            continue;
        }
        if (!q.empty()) {
            bool match = ToLower(item.label).find(q) != std::string::npos ||
                         ToLower(item.description).find(q) != std::string::npos ||
                         ToLower(item.key).find(q) != std::string::npos ||
                         ToLower(item.current_value_display).find(q) != std::string::npos ||
                         ToLower(SettingsCategoryToLabel(item.category)).find(q) != std::string::npos;
            if (!match) continue;
        } else if (item.category != m_settings_category) {
            continue;
        }
        if (!item.validation_error.empty() && vm.validation_summary.empty()) {
            vm.validation_summary = item.validation_error;
        }
        vm.visible_items.push_back(std::move(item));
    }

    if (!m_settings_validation_error.empty()) {
        vm.is_valid = false;
        vm.validation_summary = m_settings_validation_error;
    } else {
        auto val = ValidateSettingsDraft();
        vm.is_valid = val.ok();
        if (!val.ok()) vm.validation_summary = val.error->message;
    }

    if (!vm.is_valid) {
        vm.status_banner = "Validation Error: " + vm.validation_summary;
    } else if (vm.has_unsaved_changes) {
        vm.status_banner = "Unsaved changes in draft. Click Apply to persist settings.";
    } else {
        vm.status_banner = "All settings synchronized with persistent storage.";
    }

    return vm;
}

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
            OpenSettingsCenter(m_settings_category);
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
    m_settings_draft.max_concurrent_tasks = m_controller.GetMaxConcurrentTasks();
    m_last_feedback_message = "Max concurrent downloads set to " + std::to_string(m_controller.GetMaxConcurrentTasks());
}

void DesktopView::ActionSetSpeedLimit(uint64_t bps) {
    m_controller.SetGlobalSpeedLimit(bps);
    m_settings_draft.speed_limit_bps = bps;
    m_last_feedback_message = (bps == 0) ? "Speed limit set to Unlimited" : ("Speed limit set to " + FormatSpeed(bps));
}

void DesktopView::ActionSetBrowserCapture(bool enabled) {
    auto s = m_controller.GetSettings();
    s.browser_capture_enabled = enabled;
    m_controller.UpdateSettings(s);
    m_settings_draft.browser_capture_enabled = enabled;
    if (m_ipc_server) {
        m_ipc_server->SetCaptureEnabled(enabled);
    }
    m_last_feedback_message = enabled ? "Browser download capture enabled" : "Browser download capture paused";
}

Result<void> DesktopView::ActionSaveSettings(const AppSettings& settings) {
    AppSettings normalized = settings;
    normalized.theme = "dark";
    auto r = m_controller.UpdateSettings(normalized);
    if (r.ok()) {
        SetTheme("dark", false);
        m_settings_draft = m_controller.GetSettings();
        m_settings_validation_error.clear();
        if (m_ipc_server) {
            m_ipc_server->SetCaptureEnabled(m_settings_draft.browser_capture_enabled);
        }
        m_last_feedback_message = "Settings saved";
    }
    return r;
}

Result<AppSettings> DesktopView::ActionRestoreDefaultSettings() {
    auto r = m_controller.RestoreDefaultSettings();
    if (r.ok()) {
        SetTheme("dark", false);
        m_settings_draft = *r.value;
        m_settings_validation_error.clear();
        if (m_ipc_server) {
            m_ipc_server->SetCaptureEnabled(r.value->browser_capture_enabled);
        }
        m_last_feedback_message = "Restored default settings";
    }
    return r;
}

bool DesktopView::HandleKeyShortcut(const std::string& key_chord) {
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
    if (key_chord == "Escape" || key_chord == "VK_ESCAPE") {
        if (m_settings_open) {
            CloseSettingsCenter(true);
            return true;
        }
    }
    return false;
}

BrowserIntegrationStatusView DesktopView::GetBrowserIntegrationStatus() const {
    BrowserIntegrationStatusView v;
    v.ipc_server_running = (m_ipc_server != nullptr);
    v.ipc_endpoint = m_ipc_server ? m_ipc_server->GetEndpointPath() : ipc::IpcServer::DefaultSocketPath();
    v.native_host_id = "com.flux.downloadmanager";
    v.capture_enabled = m_controller.GetSettings().browser_capture_enabled;
    v.min_capture_size_bytes = 1048576ULL;
    v.intercepted_extensions = {
        "zip", "rar", "7z", "iso", "exe", "msi", "mp4", "mkv", "webm", "mp3", "flac", "pdf", "tar", "gz", "bin"
    };
    const auto& cfg = m_controller.GetConfig();
    std::error_code ec;
    v.nm3u8dl_path = cfg.nm3u8dl_binary_path.string();
    v.nm3u8dl_available = std::filesystem::exists(cfg.nm3u8dl_binary_path, ec);
    v.ffmpeg_path = cfg.ffmpeg_binary_path.string();
    v.ffmpeg_available = std::filesystem::exists(cfg.ffmpeg_binary_path, ec);
    v.supported_browsers = {
        "Google Chrome (MV3)",
        "Microsoft Edge (MV3)",
        "Brave Browser (MV3)",
        "Mozilla Firefox (MV3)"
    };
    return v;
}

AboutDialogView DesktopView::GetAboutDialogView() const {
    AboutDialogView a;
    const auto& cfg = m_controller.GetConfig();
    a.state_db_path = cfg.state_db_path.string();
    a.data_dir_path = cfg.state_db_path.parent_path().string();
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
    oss << " Flux Download Manager v0.3.0 — Cosmic Dark UI  |  Category: " << CategoryToLabel(m_selected_category)
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
// Native Windows 11 Win32 + GDI Double-Buffered Cosmic UI, Star-Field Backdrop,
// Multi-Column Task Table, Context Menus & Professional Settings Center Dialog
// -----------------------------------------------------------------------------
namespace {

constexpr UINT WM_OPENIDM_TRAYICON = WM_APP + 101;
constexpr UINT IDT_UI_REFRESH      = 1001;
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
    bool timer_running{false};
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

void DrawBorderRect(HDC hdc, const RECT& rc, ColorRgba col) {
    HBRUSH br = CreateSolidBrush(col.ToWin32ColorRef());
    FrameRect(hdc, &rc, br);
    DeleteObject(br);
}

void DrawCosmicStarFieldGdi(HDC hdc, const CosmicBackgroundState& bg_state) {
    for (const auto& star : bg_state.stars) {
        COLORREF cr = star.color.ToWin32ColorRef();
        if (star.size_px <= 1) {
            SetPixelV(hdc, star.x_px, star.y_px, cr);
        } else {
            RECT sr{star.x_px, star.y_px, star.x_px + star.size_px, star.y_px + star.size_px};
            FillRectSolid(hdc, sr, star.color);
            if (star.has_celestial_cross) {
                SetPixelV(hdc, star.x_px - 1, star.y_px, cr);
                SetPixelV(hdc, star.x_px + star.size_px, star.y_px, cr);
                SetPixelV(hdc, star.x_px, star.y_px - 1, cr);
                SetPixelV(hdc, star.x_px, star.y_px + star.size_px, cr);
            }
        }
    }
}

void ShowAddUrlQuickDialogWin32(HWND hwnd, DesktopView& view, DownloadController& controller) {
    std::string clip_url;
    if (OpenClipboard(hwnd)) {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData) {
            const wchar_t* wtxt = static_cast<const wchar_t*>(GlobalLock(hData));
            if (wtxt) {
                std::string candidate = Trim(WToUtf8(wtxt));
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

    const auto s = controller.GetSettings();
    std::wstring prompt = L"Queue new transfer in Flux?\n\n"
                          L"• Source URL: " + Utf8ToW(clip_url) + L"\n"
                          L"• Destination Folder: " + Utf8ToW(s.default_download_dir) + L"\n"
                          L"• Segmented Connections: " + std::to_wstring(s.default_connections_per_task) + L"\n"
                          L"• Global Speed Limit: " + Utf8ToW(DesktopView::FormatBandwidthLimit(s.speed_limit_bps));
    if (MessageBoxW(hwnd, prompt.c_str(), L"Flux — Confirm New Download", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
        AddDownloadDialogInput in;
        in.url = clip_url;
        in.save_dir = s.default_download_dir;
        in.max_connections = s.default_connections_per_task;
        in.start_immediately = true;
        view.SubmitAddDownloadDialog(in);
    }
}

// -----------------------------------------------------------------------------
// Interactive Win32 Professional Settings Center Modal Window
// -----------------------------------------------------------------------------
constexpr int IDC_SETTINGS_SEARCH = 4101;

struct SettingsActionHitRect {
    std::string key;
    std::string action; // "toggle", "dec", "inc", "browse", "preset_0", "preset_512k", "preset_2m", "preset_10m", "preset_50m", "reset_field", "button"
    RectI rect;
};

struct Win32SettingsDialogCtx {
    DesktopView* view{nullptr};
    DownloadController* controller{nullptr};
    HWND hwndParent{nullptr};
    HWND hwndDlg{nullptr};
    HWND hSearchEdit{nullptr};
    std::vector<RectI> category_rects;
    std::vector<SettingsActionHitRect> control_rects;
    RectI apply_btn_rect;
    RectI cancel_btn_rect;
    RectI restore_btn_rect;
    bool closed{false};
};

bool BrowseForFolderWin32(HWND hwndOwner, std::string& out_folder) {
    BROWSEINFOW bi{};
    bi.hwndOwner = hwndOwner;
    bi.lpszTitle = L"Select Default Download Directory for Flux";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
    PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&bi);
    if (!pidl) return false;
    wchar_t path_buf[MAX_PATH]{};
    bool ok = (SHGetPathFromIDListW(pidl, path_buf) != FALSE);
    CoTaskMemFree(pidl);
    if (ok && path_buf[0] != L'\0') {
        out_folder = WToUtf8(path_buf);
        return true;
    }
    return false;
}

void PaintSettingsDialogWin32(HWND hwnd, Win32SettingsDialogCtx& sctx) {
    PAINTSTRUCT ps{};
    HDC hdc_screen = BeginPaint(hwnd, &ps);

    RECT client_rc{};
    GetClientRect(hwnd, &client_rc);
    int w = client_rc.right - client_rc.left;
    int h = client_rc.bottom - client_rc.top;

    HDC hdc = CreateCompatibleDC(hdc_screen);
    HBITMAP hbm = CreateCompatibleBitmap(hdc_screen, std::max(1, w), std::max(1, h));
    HGDIOBJ old_bm = SelectObject(hdc, hbm);

    const auto& pal = sctx.view->GetPalette();
    const auto& m = sctx.view->GetMetrics();
    auto vm = sctx.view->BuildSettingsCenterViewModel();

    FillRectSolid(hdc, client_rc, pal.app_background);
    auto bg_stars = DesignSystem::ComputeCosmicBackground(w, h, m.dpi, sctx.view->GetCosmicElapsedMs(), vm.draft_settings.reduced_motion, false);
    DrawCosmicStarFieldGdi(hdc, bg_stars);
    SetBkMode(hdc, TRANSPARENT);

    HFONT hFontTitle = CreateFontW(-m.font_subtitle_px, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontBold = CreateFontW(-m.font_body_strong_px, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontBody = CreateFontW(-m.font_body_px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontCap = CreateFontW(-m.font_caption_px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HGDIOBJ old_font = SelectObject(hdc, hFontTitle);

    int header_h = DesignSystem::ScaleForDpi(52, m.dpi);
    int footer_h = DesignSystem::ScaleForDpi(54, m.dpi);
    int sidebar_w = m.settings_sidebar_width;

    // Top Header
    RECT hdr_rc{0, 0, w, header_h};
    FillRectSolid(hdc, hdr_rc, pal.surface_primary);
    RECT title_rc{m.space_lg, 0, w - DesignSystem::ScaleForDpi(240, m.dpi), header_h};
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    DrawTextW(hdc, L"Flux Configuration & Diagnostics Center", -1, &title_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Left Category Sidebar
    RECT side_rc{0, header_h + 1, sidebar_w, h - footer_h - 1};
    FillRectSolid(hdc, side_rc, pal.surface_primary);

    sctx.category_rects.clear();
    int cy = header_h + m.space_md;
    int cat_h = DesignSystem::ScaleForDpi(44, m.dpi);
    for (const auto& cat : vm.categories) {
        RectI crc{m.space_sm, cy, sidebar_w - m.space_sm, cy + cat_h};
        sctx.category_rects.push_back(crc);
        RECT wcrc = ToWinRect(crc);
        if (cat.selected) {
            FillRectSolid(hdc, wcrc, pal.surface_selected);
            RECT accent_bar{wcrc.left, wcrc.top + 6, wcrc.left + 3, wcrc.bottom - 6};
            FillRectSolid(hdc, accent_bar, pal.accent_cyan);
        }
        SelectObject(hdc, hFontBold);
        SetTextColor(hdc, (cat.selected ? pal.text_primary : pal.text_secondary).ToWin32ColorRef());
        std::string title_line = cat.title + (cat.modified_count > 0 ? " *" : "");
        RECT t_rc{wcrc.left + m.space_md, wcrc.top + 5, wcrc.right - m.space_sm, wcrc.top + 24};
        std::wstring wtitle = Utf8ToW(title_line);
        DrawTextW(hdc, wtitle.c_str(), -1, &t_rc, DT_LEFT | DT_SINGLELINE);

        SelectObject(hdc, hFontCap);
        SetTextColor(hdc, pal.text_muted.ToWin32ColorRef());
        RECT sub_rc{wcrc.left + m.space_md, wcrc.top + 23, wcrc.right - m.space_sm, wcrc.bottom - 4};
        std::wstring wsub = Utf8ToW(cat.subtitle);
        DrawTextW(hdc, wsub.c_str(), -1, &sub_rc, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
        cy += cat_h + m.space_xs;
    }

    // Main Content Area
    sctx.control_rects.clear();
    int content_left = sidebar_w + m.space_lg;
    int content_right = w - m.space_lg;
    int iy = header_h + m.space_md;

    // Category Title Banner
    SelectObject(hdc, hFontTitle);
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    std::string sec_heading = vm.search_query.empty()
        ? DesktopView::SettingsCategoryToLabel(vm.selected_category)
        : ("Search Results for \"" + vm.search_query + "\"");
    RECT sh_rc{content_left, iy, content_right, iy + 26};
    std::wstring wsh = Utf8ToW(sec_heading);
    DrawTextW(hdc, wsh.c_str(), -1, &sh_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    iy += 32;

    for (const auto& item : vm.visible_items) {
        int row_h = (item.control_type == SettingsControlType::kCategoryRuleTable)
            ? DesignSystem::ScaleForDpi(152, m.dpi)
            : DesignSystem::ScaleForDpi(62, m.dpi);
        if (iy + row_h > h - footer_h - m.space_sm) break;

        RECT row_rc{content_left, iy, content_right, iy + row_h};
        FillRectSolid(hdc, row_rc, pal.surface_primary);
        DrawBorderRect(hdc, row_rc, item.is_modified ? pal.accent_cyan : pal.border_subtle);

        SelectObject(hdc, hFontBold);
        SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
        std::string lbl = item.label + (item.is_modified ? "  [Modified]" : "");
        RECT l_rc{content_left + m.space_md, iy + 8, content_right - DesignSystem::ScaleForDpi(210, m.dpi), iy + 28};
        std::wstring wlbl = Utf8ToW(lbl);
        DrawTextW(hdc, wlbl.c_str(), -1, &l_rc, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);

        SelectObject(hdc, hFontCap);
        SetTextColor(hdc, (!item.validation_error.empty() ? pal.status_danger : pal.text_secondary).ToWin32ColorRef());
        std::string desc = !item.validation_error.empty() ? item.validation_error : item.description;
        RECT d_rc{content_left + m.space_md, iy + 28, content_right - DesignSystem::ScaleForDpi(210, m.dpi), iy + row_h - 6};
        std::wstring wdesc = Utf8ToW(desc);
        DrawTextW(hdc, wdesc.c_str(), -1, &d_rc, DT_LEFT | DT_WORDBREAK);

        int ctrl_right = content_right - m.space_md;
        int ctrl_y = iy + (row_h - DesignSystem::ScaleForDpi(28, m.dpi)) / 2;
        int btn_h = DesignSystem::ScaleForDpi(28, m.dpi);

        SelectObject(hdc, hFontBody);
        if (item.control_type == SettingsControlType::kToggleSwitch) {
            int sw_w = DesignSystem::ScaleForDpi(108, m.dpi);
            RectI sw_rc{ctrl_right - sw_w, ctrl_y, ctrl_right, ctrl_y + btn_h};
            sctx.control_rects.push_back({item.key, "toggle", sw_rc});
            bool on = (item.current_value_display.find("Enabled") == 0);
            RECT wsw = ToWinRect(sw_rc);
            FillRectSolid(hdc, wsw, on ? pal.accent_primary : pal.surface_elevated);
            DrawBorderRect(hdc, wsw, on ? pal.accent_cyan : pal.border_strong);
            SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
            std::wstring wval = Utf8ToW(item.current_value_display);
            DrawTextW(hdc, wval.c_str(), -1, &wsw, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        } else if (item.control_type == SettingsControlType::kNumericStepper) {
            int step_w = DesignSystem::ScaleForDpi(28, m.dpi);
            int val_w = DesignSystem::ScaleForDpi(110, m.dpi);
            RectI inc_rc{ctrl_right - step_w, ctrl_y, ctrl_right, ctrl_y + btn_h};
            RectI val_rc{inc_rc.left - val_w - 4, ctrl_y, inc_rc.left - 4, ctrl_y + btn_h};
            RectI dec_rc{val_rc.left - step_w - 4, ctrl_y, val_rc.left - 4, ctrl_y + btn_h};
            sctx.control_rects.push_back({item.key, "dec", dec_rc});
            sctx.control_rects.push_back({item.key, "inc", inc_rc});

            RECT wdec = ToWinRect(dec_rc), wval = ToWinRect(val_rc), winc = ToWinRect(inc_rc);
            FillRectSolid(hdc, wdec, pal.surface_elevated);
            DrawBorderRect(hdc, wdec, pal.border_strong);
            FillRectSolid(hdc, wval, pal.app_background);
            DrawBorderRect(hdc, wval, pal.border_subtle);
            FillRectSolid(hdc, winc, pal.surface_elevated);
            DrawBorderRect(hdc, winc, pal.border_strong);

            SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
            DrawTextW(hdc, L"-", -1, &wdec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            std::wstring wv = Utf8ToW(item.current_value_display);
            DrawTextW(hdc, wv.c_str(), -1, &wval, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawTextW(hdc, L"+", -1, &winc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        } else if (item.control_type == SettingsControlType::kFolderSelector) {
            int br_w = DesignSystem::ScaleForDpi(88, m.dpi);
            RectI br_rc{ctrl_right - br_w, ctrl_y, ctrl_right, ctrl_y + btn_h};
            sctx.control_rects.push_back({item.key, "browse", br_rc});
            RECT wbr = ToWinRect(br_rc);
            FillRectSolid(hdc, wbr, pal.surface_elevated);
            DrawBorderRect(hdc, wbr, pal.border_strong);
            SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
            DrawTextW(hdc, L"Browse...", -1, &wbr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            RECT path_rc{content_left + m.space_md, iy + row_h - 22, br_rc.left - m.space_sm, iy + row_h - 4};
            SelectObject(hdc, hFontCap);
            SetTextColor(hdc, pal.accent_cyan.ToWin32ColorRef());
            std::wstring wp = Utf8ToW("Path: " + item.current_value_display);
            DrawTextW(hdc, wp.c_str(), -1, &path_rc, DT_LEFT | DT_SINGLELINE | DT_PATH_ELLIPSIS);
        } else if (item.control_type == SettingsControlType::kBandwidthControl) {
            struct Preset { const char* act; const wchar_t* lbl; uint64_t bps; };
            const Preset presets[] = {
                {"preset_0",    L"Unlimited", 0ULL},
                {"preset_512k", L"512 KiB/s", 512ULL * 1024ULL},
                {"preset_2m",   L"2 MiB/s",   2ULL * 1048576ULL},
                {"preset_10m",  L"10 MiB/s",  10ULL * 1048576ULL},
                {"preset_50m",  L"50 MiB/s",  50ULL * 1048576ULL}
            };
            int pw = DesignSystem::ScaleForDpi(68, m.dpi);
            int px = ctrl_right;
            for (int pi = 4; pi >= 0; --pi) {
                RectI prc{px - pw, ctrl_y, px, ctrl_y + btn_h};
                sctx.control_rects.push_back({item.key, presets[pi].act, prc});
                RECT wprc = ToWinRect(prc);
                bool active = (vm.draft_settings.speed_limit_bps == presets[pi].bps);
                FillRectSolid(hdc, wprc, active ? pal.accent_primary : pal.surface_elevated);
                DrawBorderRect(hdc, wprc, active ? pal.accent_cyan : pal.border_strong);
                SelectObject(hdc, hFontCap);
                SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
                DrawTextW(hdc, presets[pi].lbl, -1, &wprc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                px -= (pw + 4);
            }
        } else if (item.control_type == SettingsControlType::kActionButton) {
            int ab_w = DesignSystem::ScaleForDpi(136, m.dpi);
            RectI ab_rc{ctrl_right - ab_w, ctrl_y, ctrl_right, ctrl_y + btn_h};
            sctx.control_rects.push_back({item.key, "button", ab_rc});
            RECT wab = ToWinRect(ab_rc);
            FillRectSolid(hdc, wab, item.is_dangerous ? ColorRgba{0x3B, 0x12, 0x19, 255} : pal.surface_elevated);
            DrawBorderRect(hdc, wab, item.is_dangerous ? pal.status_danger : pal.border_strong);
            SetTextColor(hdc, (item.enabled ? (item.is_dangerous ? pal.status_danger : pal.text_primary) : pal.text_muted).ToWin32ColorRef());
            std::wstring wact = Utf8ToW(item.current_value_display);
            DrawTextW(hdc, wact.c_str(), -1, &wab, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        } else if (item.control_type == SettingsControlType::kCategoryRuleTable) {
            SelectObject(hdc, hFontCap);
            int ry = iy + 48;
            for (const auto& rule : vm.category_rules) {
                RECT r_rc{content_left + m.space_md, ry, content_right - m.space_md, ry + 16};
                SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
                std::wstring wr = Utf8ToW("• " + rule.category_name + "  ->  " + rule.extensions_summary);
                DrawTextW(hdc, wr.c_str(), -1, &r_rc, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
                ry += 16;
            }
        } else {
            RECT val_rc{content_right - DesignSystem::ScaleForDpi(260, m.dpi), iy + 8, ctrl_right, iy + row_h - 8};
            SelectObject(hdc, hFontCap);
            SetTextColor(hdc, pal.accent_cyan.ToWin32ColorRef());
            std::wstring wval = Utf8ToW(item.current_value_display);
            DrawTextW(hdc, wval.c_str(), -1, &val_rc, DT_RIGHT | DT_VCENTER | DT_WORDBREAK);
        }

        iy += row_h + m.space_sm;
    }

    // Footer Bar
    RECT ftr_rc{0, h - footer_h, w, h};
    FillRectSolid(hdc, ftr_rc, pal.surface_primary);
    SelectObject(hdc, hFontCap);
    SetTextColor(hdc, (vm.is_valid ? pal.text_secondary : pal.status_danger).ToWin32ColorRef());
    RECT st_rc{m.space_lg, h - footer_h, w - DesignSystem::ScaleForDpi(320, m.dpi), h};
    std::wstring wst = Utf8ToW(vm.status_banner);
    DrawTextW(hdc, wst.c_str(), -1, &st_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

    int fbtn_w = DesignSystem::ScaleForDpi(96, m.dpi);
    int fbtn_h = DesignSystem::ScaleForDpi(32, m.dpi);
    int fbtn_y = h - (footer_h + fbtn_h) / 2;

    sctx.apply_btn_rect = {w - fbtn_w - m.space_lg, fbtn_y, w - m.space_lg, fbtn_y + fbtn_h};
    sctx.cancel_btn_rect = {sctx.apply_btn_rect.left - fbtn_w - m.space_sm, fbtn_y, sctx.apply_btn_rect.left - m.space_sm, fbtn_y + fbtn_h};
    int rbtn_w = DesignSystem::ScaleForDpi(126, m.dpi);
    sctx.restore_btn_rect = {sctx.cancel_btn_rect.left - rbtn_w - m.space_sm, fbtn_y, sctx.cancel_btn_rect.left - m.space_sm, fbtn_y + fbtn_h};

    SelectObject(hdc, hFontBold);
    RECT wres = ToWinRect(sctx.restore_btn_rect);
    FillRectSolid(hdc, wres, pal.surface_elevated);
    DrawBorderRect(hdc, wres, pal.border_strong);
    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
    DrawTextW(hdc, L"Restore Defaults", -1, &wres, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    RECT wcan = ToWinRect(sctx.cancel_btn_rect);
    FillRectSolid(hdc, wcan, pal.surface_elevated);
    DrawBorderRect(hdc, wcan, pal.border_strong);
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    DrawTextW(hdc, L"Close", -1, &wcan, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    RECT wapp = ToWinRect(sctx.apply_btn_rect);
    FillRectSolid(hdc, wapp, pal.accent_primary);
    DrawBorderRect(hdc, wapp, pal.accent_cyan);
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    DrawTextW(hdc, L"Apply & Save", -1, &wapp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, old_font);
    DeleteObject(hFontTitle);
    DeleteObject(hFontBold);
    DeleteObject(hFontBody);
    DeleteObject(hFontCap);

    BitBlt(hdc_screen, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    SelectObject(hdc, old_bm);
    DeleteObject(hbm);
    DeleteDC(hdc);
    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK FluxSettingsWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto* sctx = reinterpret_cast<Win32SettingsDialogCtx*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        sctx = static_cast<Win32SettingsDialogCtx*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(sctx));
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    if (!sctx || !sctx->view) return DefWindowProcW(hwnd, msg, wParam, lParam);

    switch (msg) {
        case WM_CREATE: {
            RECT rc{};
            GetClientRect(hwnd, &rc);
            const auto& m = sctx->view->GetMetrics();
            int sw = DesignSystem::ScaleForDpi(210, m.dpi);
            int sh = DesignSystem::ScaleForDpi(28, m.dpi);
            int sy = (DesignSystem::ScaleForDpi(52, m.dpi) - sh) / 2;
            sctx->hSearchEdit = CreateWindowExW(
                WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                rc.right - sw - m.space_lg, sy, sw, sh,
                hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_SETTINGS_SEARCH)),
                GetModuleHandleW(nullptr), nullptr);
            SendMessageW(sctx->hSearchEdit, EM_SETCUEBANNER, FALSE, reinterpret_cast<LPARAM>(L"Search settings..."));
            return 0;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_SETTINGS_SEARCH && HIWORD(wParam) == EN_CHANGE) {
                wchar_t buf[256]{};
                GetWindowTextW(sctx->hSearchEdit, buf, 255);
                sctx->view->SetSettingsSearchQuery(WToUtf8(buf));
                InvalidateRect(hwnd, nullptr, FALSE);
                return 0;
            }
            return 0;
        }
        case WM_LBUTTONDOWN: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            auto vm = sctx->view->BuildSettingsCenterViewModel();
            for (size_t i = 0; i < sctx->category_rects.size() && i < vm.categories.size(); ++i) {
                if (sctx->category_rects[i].Contains(x, y)) {
                    sctx->view->SetSettingsSearchQuery("");
                    if (sctx->hSearchEdit) SetWindowTextW(sctx->hSearchEdit, L"");
                    sctx->view->SelectSettingsCategory(vm.categories[i].category);
                    InvalidateRect(hwnd, nullptr, FALSE);
                    return 0;
                }
            }
            for (const auto& hit : sctx->control_rects) {
                if (!hit.rect.Contains(x, y)) continue;
                auto draft = sctx->view->GetSettingsDraft();
                if (hit.action == "toggle") {
                    if (hit.key == "general.start_minimized") draft.start_minimized = !draft.start_minimized;
                    else if (hit.key == "general.close_to_tray") draft.close_to_tray = !draft.close_to_tray;
                    else if (hit.key == "general.reduced_motion") draft.reduced_motion = !draft.reduced_motion;
                    else if (hit.key == "browser.capture_enabled") draft.browser_capture_enabled = !draft.browser_capture_enabled;
                    sctx->view->SetSettingsDraft(draft);
                } else if (hit.action == "dec" || hit.action == "inc") {
                    int delta = (hit.action == "inc") ? 1 : -1;
                    if (hit.key == "downloads.max_concurrent_tasks") {
                        int next = std::clamp(static_cast<int>(draft.max_concurrent_tasks) + delta, 1, 16);
                        draft.max_concurrent_tasks = static_cast<uint32_t>(next);
                    } else if (hit.key == "connections.default_connections_per_task") {
                        int next = std::clamp(static_cast<int>(draft.default_connections_per_task) + delta, 1, 32);
                        draft.default_connections_per_task = static_cast<uint32_t>(next);
                    }
                    sctx->view->SetSettingsDraft(draft);
                } else if (hit.action == "browse") {
                    std::string chosen;
                    if (BrowseForFolderWin32(hwnd, chosen)) {
                        sctx->view->SetSettingsDraftField("locations.default_download_dir", chosen);
                    }
                } else if (hit.action == "preset_0") {
                    sctx->view->SetSettingsDraftField("speed.speed_limit_bps", "0");
                } else if (hit.action == "preset_512k") {
                    sctx->view->SetSettingsDraftField("speed.speed_limit_bps", "512 KiB/s");
                } else if (hit.action == "preset_2m") {
                    sctx->view->SetSettingsDraftField("speed.speed_limit_bps", "2 MiB/s");
                } else if (hit.action == "preset_10m") {
                    sctx->view->SetSettingsDraftField("speed.speed_limit_bps", "10 MiB/s");
                } else if (hit.action == "preset_50m") {
                    sctx->view->SetSettingsDraftField("speed.speed_limit_bps", "50 MiB/s");
                } else if (hit.action == "button") {
                    if (hit.key == "downloads.clear_history") {
                        if (MessageBoxW(hwnd, L"Remove all completed and canceled entries from history? Downloaded files on disk will not be deleted.", L"Confirm Clear History", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                            sctx->view->ActionClearCompletedHistory();
                        }
                    } else if (hit.key == "about.restore_defaults") {
                        if (MessageBoxW(hwnd, L"Restore all Flux settings to factory defaults?", L"Confirm Restore Defaults", MB_YESNO | MB_ICONWARNING) == IDYES) {
                            sctx->view->ActionRestoreDefaultSettings();
                        }
                    }
                }
                InvalidateRect(hwnd, nullptr, FALSE);
                return 0;
            }
            if (sctx->restore_btn_rect.Contains(x, y)) {
                if (MessageBoxW(hwnd, L"Restore all Flux settings to factory defaults?", L"Confirm Restore Defaults", MB_YESNO | MB_ICONWARNING) == IDYES) {
                    sctx->view->ActionRestoreDefaultSettings();
                    InvalidateRect(hwnd, nullptr, FALSE);
                }
                return 0;
            }
            if (sctx->cancel_btn_rect.Contains(x, y)) {
                sctx->view->CloseSettingsCenter(true);
                DestroyWindow(hwnd);
                return 0;
            }
            if (sctx->apply_btn_rect.Contains(x, y)) {
                auto res = sctx->view->ApplySettingsDraft();
                if (!res.ok()) {
                    MessageBoxW(hwnd, Utf8ToW(res.error->message).c_str(), L"Flux — Settings Validation Error", MB_OK | MB_ICONERROR);
                } else {
                    sctx->view->CloseSettingsCenter(false);
                    DestroyWindow(hwnd);
                }
                return 0;
            }
            return 0;
        }
        case WM_KEYDOWN: {
            if (wParam == VK_ESCAPE) {
                sctx->view->CloseSettingsCenter(true);
                DestroyWindow(hwnd);
                return 0;
            }
            return 0;
        }
        case WM_PAINT: {
            PaintSettingsDialogWin32(hwnd, *sctx);
            return 0;
        }
        case WM_CLOSE: {
            sctx->view->CloseSettingsCenter(true);
            DestroyWindow(hwnd);
            return 0;
        }
        case WM_DESTROY: {
            sctx->closed = true;
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void ShowSettingsModalWin32(HWND hwndParent, DesktopView& view, DownloadController& controller, SettingsCategory initial_cat = SettingsCategory::kGeneral) {
    view.OpenSettingsCenter(initial_cat);

    HINSTANCE hInst = GetModuleHandleW(nullptr);
    static bool s_registered = false;
    if (!s_registered) {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = FluxSettingsWndProc;
        wc.hInstance = hInst;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
        wc.lpszClassName = L"Flux_SettingsCenterWindowClass";
        RegisterClassExW(&wc);
        s_registered = true;
    }

    const auto& m = view.GetMetrics();
    int dlg_w = DesignSystem::ScaleForDpi(820, m.dpi);
    int dlg_h = DesignSystem::ScaleForDpi(560, m.dpi);

    RECT parent_rc{};
    GetWindowRect(hwndParent, &parent_rc);
    int pos_x = parent_rc.left + ((parent_rc.right - parent_rc.left) - dlg_w) / 2;
    int pos_y = parent_rc.top + ((parent_rc.bottom - parent_rc.top) - dlg_h) / 2;

    Win32SettingsDialogCtx sctx{&view, &controller, hwndParent, nullptr, nullptr, {}, {}, {}, {}, {}, false};
    HWND hwndDlg = CreateWindowExW(
        WS_EX_DLGMODALFRAME,
        L"Flux_SettingsCenterWindowClass",
        L"Flux — Settings & Diagnostics",
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
        pos_x, pos_y, dlg_w, dlg_h,
        hwndParent, nullptr, hInst, &sctx);
    if (!hwndDlg) return;
    sctx.hwndDlg = hwndDlg;

    BOOL use_dark = TRUE;
    DwmSetWindowAttribute(hwndDlg, 20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */, &use_dark, sizeof(use_dark));

    EnableWindow(hwndParent, FALSE);
    ShowWindow(hwndDlg, SW_SHOW);
    UpdateWindow(hwndDlg);

    MSG msg{};
    while (!sctx.closed && GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    EnableWindow(hwndParent, TRUE);
    SetForegroundWindow(hwndParent);
    InvalidateRect(hwndParent, nullptr, FALSE);
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
    const auto& m = view.GetMetrics();
    auto geom = view.ComputeLayoutGeometry(0);

    // 0. Deep Space Charcoal Canvas + 3-Layer Cosmic Star-Field
    FillRectSolid(hdc, client_rc, pal.app_background);
    auto cosmic_bg = view.ComputeCosmicBackgroundState();
    DrawCosmicStarFieldGdi(hdc, cosmic_bg);
    SetBkMode(hdc, TRANSPARENT);

    HFONT hFontCap = CreateFontW(
        -m.font_caption_px, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontBody = CreateFontW(
        -m.font_body_px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontBold = CreateFontW(
        -m.font_body_strong_px, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HGDIOBJ old_font = SelectObject(hdc, hFontBold);

    // 1. Top Toolbar with cosmic navy atmosphere and restrained borders
    RECT toolbar_rc = ToWinRect(geom.toolbar_rect);
    FillRectSolid(hdc, toolbar_rc, pal.surface_primary);
    RECT toolbar_div{0, geom.toolbar_rect.bottom, w, geom.toolbar_rect.bottom + 1};
    FillRectSolid(hdc, toolbar_div, pal.border_subtle);

    for (size_t i = 0; i < geom.toolbar_buttons.size(); ++i) {
        const auto& btn = geom.toolbar_buttons[i];
        RECT brc = ToWinRect(btn.rect);
        bool hovered = (view.GetHoveredToolbarIndex() == static_cast<int>(i));
        ColorRgba bg = btn.is_primary
            ? (hovered ? pal.accent_hover : pal.accent_primary)
            : (hovered && btn.enabled ? pal.surface_hover : (btn.enabled ? pal.surface_elevated : pal.surface_primary));
        FillRectSolid(hdc, brc, bg);
        DrawBorderRect(hdc, brc, btn.is_primary ? pal.accent_cyan : (hovered && btn.enabled ? pal.border_focus : pal.border_strong));
        SetTextColor(hdc, (btn.enabled || btn.is_primary ? pal.text_primary : pal.text_muted).ToWin32ColorRef());
        std::wstring wlbl = Utf8ToW(btn.label);
        DrawTextW(hdc, wlbl.c_str(), -1, &brc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // 2. Left Navigation Sidebar with active cyan indicator bar & pill counts
    RECT sidebar_rc = ToWinRect(geom.sidebar_rect);
    FillRectSolid(hdc, sidebar_rc, pal.surface_primary);
    RECT side_div{geom.sidebar_rect.right, geom.sidebar_rect.top, geom.sidebar_rect.right + 1, geom.sidebar_rect.bottom};
    FillRectSolid(hdc, side_div, pal.border_subtle);

    for (size_t i = 0; i < geom.sidebar_items.size(); ++i) {
        const auto& item = geom.sidebar_items[i];
        RECT crc = ToWinRect(item.rect);
        if (item.selected) {
            FillRectSolid(hdc, crc, pal.surface_selected);
            RECT active_bar{crc.left, crc.top + 5, crc.left + 3, crc.bottom - 5};
            FillRectSolid(hdc, active_bar, pal.accent_cyan);
        } else if (view.GetHoveredSidebarIndex() == static_cast<int>(i)) {
            FillRectSolid(hdc, crc, pal.surface_hover);
        }

        SelectObject(hdc, item.selected ? hFontBold : hFontBody);
        SetTextColor(hdc, (item.selected ? pal.text_primary : pal.text_secondary).ToWin32ColorRef());
        RECT text_rc = crc;
        text_rc.left += m.space_md;
        text_rc.right -= DesignSystem::ScaleForDpi(44, m.dpi);
        std::wstring wlbl = Utf8ToW(item.label);
        DrawTextW(hdc, wlbl.c_str(), -1, &text_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Count pill badge
        RECT pill_rc{crc.right - DesignSystem::ScaleForDpi(38, m.dpi), crc.top + 6, crc.right - m.space_sm, crc.bottom - 6};
        FillRectSolid(hdc, pill_rc, item.selected ? pal.accent_primary : pal.surface_elevated);
        SelectObject(hdc, hFontCap);
        SetTextColor(hdc, (item.selected ? pal.text_primary : pal.text_secondary).ToWin32ColorRef());
        std::wstring wcnt = std::to_wstring(item.count);
        DrawTextW(hdc, wcnt.c_str(), -1, &pill_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // 3. Sortable Table Column Headers
    RECT header_rc = ToWinRect(geom.table_header_rect);
    FillRectSolid(hdc, header_rc, pal.cosmic_atmosphere);
    RECT hdr_div{geom.table_header_rect.left, geom.table_header_rect.bottom - 1, geom.table_header_rect.right, geom.table_header_rect.bottom};
    FillRectSolid(hdc, hdr_div, pal.border_subtle);

    SelectObject(hdc, hFontCap);
    for (const auto& col : geom.table_columns) {
        RECT crc = ToWinRect(col.rect);
        crc.left += m.space_sm;
        bool is_sorted = (col.sort_column == view.GetSortColumn() && col.title != "Actions");
        SetTextColor(hdc, (is_sorted ? pal.accent_cyan : pal.text_secondary).ToWin32ColorRef());
        std::string title = col.title;
        if (is_sorted) {
            title += view.IsSortAscending() ? " \xE2\x96\xB2" : " \xE2\x96\xBC";
        }
        std::wstring wtitle = Utf8ToW(title);
        DrawTextW(hdc, wtitle.c_str(), -1, &crc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // 4. Multi-Column Download Rows over the Cosmic Star-Field Canvas
    auto rows = view.BuildVisibleTaskRows(0, geom.task_rows.size());
    if (rows.empty()) {
        RECT empty_rc = ToWinRect(geom.table_body_rect);
        empty_rc.left += m.space_2xl;
        empty_rc.right -= m.space_2xl;
        SelectObject(hdc, hFontBody);
        SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
        std::wstring emsg = Utf8ToW(view.GetEmptyStateMessage());
        DrawTextW(hdc, emsg.c_str(), -1, &empty_rc, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
    } else {
        bool compact = view.IsCompactColumnsMode();
        for (size_t i = 0; i < rows.size() && i < geom.task_rows.size(); ++i) {
            const auto& r = rows[i];
            const auto& rl = geom.task_rows[i];
            RECT rrc = ToWinRect(rl.row_rect);
            if (r.selected) {
                FillRectSolid(hdc, rrc, pal.surface_selected);
                RECT sel_bar{rrc.left, rrc.top, rrc.left + 3, rrc.bottom};
                FillRectSolid(hdc, sel_bar, pal.accent_cyan);
            } else if (view.GetHoveredRowIndex() == static_cast<int>(i)) {
                FillRectSolid(hdc, rrc, pal.surface_hover);
            }
            RECT row_sep{rrc.left, rrc.bottom - 1, rrc.right, rrc.bottom};
            FillRectSolid(hdc, row_sep, pal.border_subtle);

            // Column 0: Filename
            if (!rl.cell_rects.empty()) {
                RECT fn_rc = ToWinRect(rl.cell_rects[0]);
                fn_rc.left += m.space_sm;
                fn_rc.right -= m.space_xs;
                SelectObject(hdc, hFontBold);
                SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
                std::wstring wfn = Utf8ToW(r.display_filename);
                DrawTextW(hdc, wfn.c_str(), -1, &fn_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
            }

            if (compact && rl.cell_rects.size() >= 6) {
                // Compact: [0]=Filename, [1]=Status, [2]=Progress, [3]=Size, [4]=Speed, [5]=Actions
                RECT st_rc = ToWinRect(rl.cell_rects[1]);
                st_rc.left += m.space_sm;
                SelectObject(hdc, hFontBody);
                SetTextColor(hdc, r.status_color.ToWin32ColorRef());
                std::wstring wst = Utf8ToW(r.status_text);
                DrawTextW(hdc, wst.c_str(), -1, &st_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

                RECT pr_rc = ToWinRect(rl.cell_rects[2]);
                pr_rc.left += m.space_sm;
                pr_rc.bottom -= DesignSystem::ScaleForDpi(12, m.dpi);
                SelectObject(hdc, hFontCap);
                SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
                std::wstring wpr = Utf8ToW(r.progress_text);
                DrawTextW(hdc, wpr.c_str(), -1, &pr_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

                RECT sz_rc = ToWinRect(rl.cell_rects[3]);
                sz_rc.left += m.space_sm;
                SelectObject(hdc, hFontBody);
                SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                std::wstring wsz = Utf8ToW(r.size_text);
                DrawTextW(hdc, wsz.c_str(), -1, &sz_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

                RECT sp_rc = ToWinRect(rl.cell_rects[4]);
                sp_rc.left += m.space_sm;
                SetTextColor(hdc, (r.speed_text != "—" ? pal.accent_cyan : pal.text_muted).ToWin32ColorRef());
                std::wstring wsp = Utf8ToW(r.speed_text);
                DrawTextW(hdc, wsp.c_str(), -1, &sp_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            } else if (!compact && rl.cell_rects.size() >= 8) {
                // Full: [0]=Filename, [1]=Type, [2]=Status, [3]=Progress, [4]=Size, [5]=Speed, [6]=ETA, [7]=Actions
                RECT ty_rc = ToWinRect(rl.cell_rects[1]);
                RECT badge_rc{ty_rc.left + m.space_xs, ty_rc.top + (m.table_row_height - DesignSystem::ScaleForDpi(20, m.dpi)) / 2,
                              ty_rc.right - m.space_sm, ty_rc.top + (m.table_row_height + DesignSystem::ScaleForDpi(20, m.dpi)) / 2};
                FillRectSolid(hdc, badge_rc, r.badge_color);
                SelectObject(hdc, hFontCap);
                SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
                std::wstring wty = Utf8ToW(r.file_type_badge);
                DrawTextW(hdc, wty.c_str(), -1, &badge_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                RECT st_rc = ToWinRect(rl.cell_rects[2]);
                RECT dot_rc{st_rc.left + m.space_sm, st_rc.top + (m.table_row_height - 6) / 2,
                            st_rc.left + m.space_sm + 6, st_rc.top + (m.table_row_height + 6) / 2};
                FillRectSolid(hdc, dot_rc, r.status_color);
                st_rc.left += m.space_sm + 12;
                SelectObject(hdc, hFontBody);
                SetTextColor(hdc, r.status_color.ToWin32ColorRef());
                std::wstring wst = Utf8ToW(r.status_text);
                DrawTextW(hdc, wst.c_str(), -1, &st_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

                RECT pr_rc = ToWinRect(rl.cell_rects[3]);
                pr_rc.left += m.space_sm;
                pr_rc.bottom -= DesignSystem::ScaleForDpi(12, m.dpi);
                SelectObject(hdc, hFontCap);
                SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
                std::wstring wpr = Utf8ToW(r.progress_text);
                DrawTextW(hdc, wpr.c_str(), -1, &pr_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

                RECT sz_rc = ToWinRect(rl.cell_rects[4]);
                sz_rc.left += m.space_sm;
                SelectObject(hdc, hFontBody);
                SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                std::wstring wsz = Utf8ToW(r.size_text);
                DrawTextW(hdc, wsz.c_str(), -1, &sz_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

                RECT sp_rc = ToWinRect(rl.cell_rects[5]);
                sp_rc.left += m.space_sm;
                SetTextColor(hdc, (r.speed_text != "—" ? pal.accent_cyan : pal.text_muted).ToWin32ColorRef());
                std::wstring wsp = Utf8ToW(r.speed_text);
                DrawTextW(hdc, wsp.c_str(), -1, &sp_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

                RECT eta_rc = ToWinRect(rl.cell_rects[6]);
                eta_rc.left += m.space_sm;
                SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
                std::wstring weta = Utf8ToW(r.eta_text);
                DrawTextW(hdc, weta.c_str(), -1, &eta_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }

            // Progress bar track & fill
            RECT ptrack = ToWinRect(rl.progress_bar_rect);
            FillRectSolid(hdc, ptrack, pal.progress_track);
            int fill_w = static_cast<int>(static_cast<double>(ptrack.right - ptrack.left) * r.progress_fraction);
            if (fill_w > 0) {
                RECT pfill = ptrack;
                pfill.right = ptrack.left + fill_w;
                ColorRgba bar_col = (r.task_state == TaskState::kCompleted)
                    ? pal.status_success
                    : (r.is_stream ? pal.badge_stream : pal.accent_primary);
                FillRectSolid(hdc, pfill, bar_col);
            }

            // Row inline action button
            RECT act_rc = ToWinRect(rl.action_button_rect);
            FillRectSolid(hdc, act_rc, pal.surface_elevated);
            DrawBorderRect(hdc, act_rc, pal.border_strong);
            SelectObject(hdc, hFontCap);
            SetTextColor(hdc, (r.primary_action_enabled ? pal.text_primary : pal.text_muted).ToWin32ColorRef());
            std::wstring wact = Utf8ToW(r.primary_action_label);
            DrawTextW(hdc, wact.c_str(), -1, &act_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    // Tooltip overlay if hovering over a truncated filename or control
    if (!view.GetActiveTooltip().empty()) {
        RECT tip_rc{geom.table_body_rect.left + m.space_md, geom.table_body_rect.top + 4,
                    geom.table_body_rect.right - m.space_md, geom.table_body_rect.top + 28};
        FillRectSolid(hdc, tip_rc, pal.surface_elevated);
        DrawBorderRect(hdc, tip_rc, pal.border_focus);
        SelectObject(hdc, hFontCap);
        SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
        std::wstring wtip = Utf8ToW("  " + view.GetActiveTooltip());
        DrawTextW(hdc, wtip.c_str(), -1, &tip_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }

    // 5. Bottom Transfer Inspector Panel
    RECT details_rc = ToWinRect(geom.details_panel_rect);
    FillRectSolid(hdc, details_rc, pal.surface_primary);
    RECT det_div{details_rc.left, details_rc.top - 1, details_rc.right, details_rc.top};
    FillRectSolid(hdc, det_div, pal.border_subtle);

    auto det = view.BuildSelectedTaskDetails();
    RECT dtext_rc = details_rc;
    dtext_rc.left += m.space_md;
    dtext_rc.top += m.space_sm;
    dtext_rc.right -= m.space_md;
    dtext_rc.bottom -= m.space_sm;
    SelectObject(hdc, hFontBody);
    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
    if (det.has_selection) {
        std::wstring dstr = Utf8ToW(
            "Selected Task: " + det.file_name + "  (" + det.size_summary + ")\n" +
            "Source URL: " + det.redacted_source_url + "\n" +
            "Destination: " + det.destination_path + "\n" +
            "Engine: " + det.engine_name + "  |  Format: " + det.stream_format + "\n" +
            "Tracks: " + det.selected_tracks_summary + "  |  Retries: " + std::to_string(det.retry_count) + "\n" +
            "Status / Diagnostics: " + det.error_explanation);
        DrawTextW(hdc, dstr.c_str(), -1, &dtext_rc, DT_LEFT | DT_TOP | DT_WORDBREAK);
    } else {
        DrawTextW(hdc, L"Select a download task to inspect its transfer details, tracks, and diagnostics.", -1, &dtext_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // 6. Status Bar
    RECT status_rc = ToWinRect(geom.status_bar_rect);
    FillRectSolid(hdc, status_rc, pal.surface_elevated);
    auto sb = view.BuildStatusBar();
    std::wstring sb_str = Utf8ToW("  " + sb.status_message + "   |   " + view.GetLastFeedbackMessage());
    SelectObject(hdc, hFontCap);
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    DrawTextW(hdc, sb_str.c_str(), -1, &status_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, old_font);
    DeleteObject(hFontCap);
    DeleteObject(hFontBody);
    DeleteObject(hFontBold);

    BitBlt(hdc_screen, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    SelectObject(hdc, old_bm);
    DeleteObject(hbm);
    DeleteDC(hdc);
    EndPaint(hwnd, &ps);
}

void UpdateWindowTimerStateWin32(Win32WindowContext& ctx) {
    bool should_run = ctx.view->IsWindowVisibleState();
    if (should_run && !ctx.timer_running && ctx.hwnd) {
        SetTimer(ctx.hwnd, IDT_UI_REFRESH, 100, nullptr);
        ctx.timer_running = true;
    } else if (!should_run && ctx.timer_running && ctx.hwnd) {
        KillTimer(ctx.hwnd, IDT_UI_REFRESH);
        ctx.timer_running = false;
    }
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
            auto geom = ctx->view->ComputeLayoutGeometry(0);
            ctx->hSearchEdit = CreateWindowExW(
                WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                geom.search_box_rect.left, geom.search_box_rect.top,
                geom.search_box_rect.width(), geom.search_box_rect.height(),
                hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_SEARCH_EDIT)),
                GetModuleHandleW(nullptr), nullptr);
            SendMessageW(ctx->hSearchEdit, EM_SETCUEBANNER, FALSE, reinterpret_cast<LPARAM>(L"Search filename or URL..."));
            return 0;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_SEARCH_EDIT && HIWORD(wParam) == EN_CHANGE) {
                wchar_t buf[512]{};
                GetWindowTextW(ctx->hSearchEdit, buf, 511);
                ctx->view->SetSearchQuery(WToUtf8(buf));
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
        case WM_SHOWWINDOW: {
            bool vis = (wParam != FALSE);
            ctx->view->SetWindowVisibility(vis, IsIconic(hwnd) != FALSE);
            UpdateWindowTimerStateWin32(*ctx);
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
                MoveWindow(ctx->hSearchEdit, geom.search_box_rect.left, geom.search_box_rect.top,
                           geom.search_box_rect.width(), geom.search_box_rect.height(), TRUE);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_SIZE: {
            if (wParam == SIZE_MINIMIZED) {
                ctx->view->SetWindowVisibility(true, true);
                UpdateWindowTimerStateWin32(*ctx);
                return 0;
            }
            ctx->view->SetWindowVisibility(IsWindowVisible(hwnd) != FALSE, false);
            UpdateWindowTimerStateWin32(*ctx);
            UINT dpi = GetDpiForWindow(hwnd);
            ctx->view->SetWindowDimensions(LOWORD(lParam), HIWORD(lParam), dpi > 0 ? dpi : 96);
            auto geom = ctx->view->ComputeLayoutGeometry(0);
            if (ctx->hSearchEdit) {
                MoveWindow(ctx->hSearchEdit, geom.search_box_rect.left, geom.search_box_rect.top,
                           geom.search_box_rect.width(), geom.search_box_rect.height(), TRUE);
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
            ctx->view->HandleMouseClick(x, y, msg == WM_LBUTTONDBLCLK, 0);

            if (hit.type == HitTargetType::kToolbarButton) {
                auto geom = ctx->view->ComputeLayoutGeometry(0);
                if (hit.index >= 0 && static_cast<size_t>(hit.index) < geom.toolbar_buttons.size()) {
                    auto aid = geom.toolbar_buttons[static_cast<size_t>(hit.index)].action_id;
                    if (aid == ToolbarActionId::kAddUrl) {
                        ShowAddUrlQuickDialogWin32(hwnd, *ctx->view, *ctx->controller);
                    } else if (aid == ToolbarActionId::kSettings) {
                        ShowSettingsModalWin32(hwnd, *ctx->view, *ctx->controller, SettingsCategory::kGeneral);
                    } else if (aid == ToolbarActionId::kOpenFolder) {
                        ctx->view->ActionOpenDestinationFolder(false);
                    }
                }
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
            AppendMenuW(hMenu, MF_STRING, IDM_CTX_BROWSER_STAT, L"Browser Integration & Worker Status...");
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
            } else if (cmd == IDM_CTX_SETTINGS) {
                ShowSettingsModalWin32(hwnd, *ctx->view, *ctx->controller, SettingsCategory::kGeneral);
            } else if (cmd == IDM_CTX_BROWSER_STAT) {
                ShowSettingsModalWin32(hwnd, *ctx->view, *ctx->controller, SettingsCategory::kBrowserIntegration);
            } else if (cmd == IDM_CTX_ABOUT) {
                ShowSettingsModalWin32(hwnd, *ctx->view, *ctx->controller, SettingsCategory::kAboutAndDiagnostics);
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_TIMER: {
            if (wParam == IDT_UI_REFRESH) {
                ctx->view->AdvanceCosmicAnimation(100);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }
        case (WM_APP + 105): {
            if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) {
                ShowWindow(hwnd, SW_RESTORE);
            }
            ctx->view->SetWindowVisibility(true, false);
            UpdateWindowTimerStateWin32(*ctx);
            SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            SetForegroundWindow(hwnd);
            if (ctx && ctx->view) {
                ctx->view->SelectFirstRow();
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_KEYDOWN: {
            bool ctrl_down = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            bool shift_down = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
            if (ctrl_down && wParam == 'N') {
                ShowAddUrlQuickDialogWin32(hwnd, *ctx->view, *ctx->controller);
            } else if (ctrl_down && wParam == 'F') {
                if (ctx->hSearchEdit) SetFocus(ctx->hSearchEdit);
            } else if (ctrl_down && wParam == VK_OEM_COMMA) {
                ShowSettingsModalWin32(hwnd, *ctx->view, *ctx->controller, SettingsCategory::kGeneral);
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
                    UpdateWindowTimerStateWin32(*ctx);
                } else if (cmd == IDM_TRAY_HIDE) {
                    ShowWindow(hwnd, SW_HIDE);
                    ctx->view->SetWindowVisibility(false, false);
                    UpdateWindowTimerStateWin32(*ctx);
                } else if (cmd == IDM_TRAY_PAUSE_ALL) ctx->controller->PauseAllTasks();
                else if (cmd == IDM_TRAY_RESUME_ALL) ctx->controller->ResumeAllTasks();
                else if (cmd == IDM_TRAY_EXIT) DestroyWindow(hwnd);
            } else if (lParam == WM_LBUTTONDBLCLK) {
                ShowWindow(hwnd, SW_RESTORE);
                ctx->view->SetWindowVisibility(true, false);
                UpdateWindowTimerStateWin32(*ctx);
                SetForegroundWindow(hwnd);
            }
            return 0;
        }
        case WM_CLOSE: {
            if (ctx->controller->GetSettings().close_to_tray && ctx->tray_added) {
                ShowWindow(hwnd, SW_HIDE);
                ctx->view->SetWindowVisibility(false, false);
                UpdateWindowTimerStateWin32(*ctx);
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
            if (ctx->timer_running) {
                KillTimer(hwnd, IDT_UI_REFRESH);
                ctx->timer_running = false;
            }
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

    // Honor Windows client-area animation preference if OS animations are disabled
    BOOL client_anim = TRUE;
    if (SystemParametersInfoW(SPI_GETCLIENTAREAANIMATION, 0, &client_anim, 0) && !client_anim) {
        auto s = m_controller.GetSettings();
        if (!s.reduced_motion) {
            s.reduced_motion = true;
            m_controller.UpdateSettings(s);
            m_settings_draft.reduced_motion = true;
        }
    }

    HINSTANCE hInst = GetModuleHandleW(nullptr);
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = OpenIdmWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    wc.lpszClassName = L"Flux_MainWindowClass";
    RegisterClassExW(&wc);

    Win32WindowContext ctx{this, &m_controller, nullptr, nullptr, {}, false, false};
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

    ctx.nid.cbSize = sizeof(NOTIFYICONDATAW);
    ctx.nid.hWnd = hwnd;
    ctx.nid.uID = 1;
    ctx.nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    ctx.nid.uCallbackMessage = WM_OPENIDM_TRAYICON;
    ctx.nid.hIcon = wc.hIcon;
    wcsncpy_s(ctx.nid.szTip, L"Flux Download Manager", _TRUNCATE);
    ctx.tray_added = (Shell_NotifyIconW(NIM_ADD, &ctx.nid) != FALSE);

    bool start_min = m_controller.GetSettings().start_minimized;
    SetWindowVisibility(!start_min, false);
    ShowWindow(hwnd, start_min ? SW_HIDE : SW_SHOWDEFAULT);
    UpdateWindowTimerStateWin32(ctx);
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
