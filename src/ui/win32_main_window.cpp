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
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")
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

} // namespace

DesktopView::DesktopView(DownloadController& controller, ipc::IpcServer* ipc_server)
    : m_controller(controller),
      m_ipc_server(ipc_server),
      m_metrics(DesignSystem::ComputeMetrics(96)) {
    auto settings = m_controller.GetSettings();
    SetTheme(settings.theme, false);
}

void DesktopView::SetWindowDimensions(int width, int height, uint32_t dpi) {
    m_metrics = DesignSystem::ComputeMetrics(dpi);
    m_window_width = std::max(width, m_metrics.min_window_width);
    m_window_height = std::max(height, m_metrics.min_window_height);
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
            int cw = static_cast<int>(list_w * weights[i]);
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
            int cw = static_cast<int>(list_w * weights[i]);
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
        RectI prog_rc{list_left + m.space_md, ry + m.table_row_height - 9, list_left + DesignSystem::ScaleForDpi(320, m.dpi), ry + m.table_row_height - 4};
        RectI act_rc{w - action_btn_w - m.space_md, ry + (m.table_row_height - action_btn_h) / 2, w - m.space_md, ry + (m.table_row_height + action_btn_h) / 2};
        g.task_rows.push_back({r.task_id, row_rc, fn_rc, prog_rc, act_rc});
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

void DesktopView::SetTheme(std::string_view theme_name, bool os_prefers_light) {
    if (theme_name == "light" || theme_name == "system" || theme_name == "dark") {
        m_theme_name = std::string(theme_name);
    } else {
        m_theme_name = "dark";
    }
    m_palette = DesignSystem::ResolvePalette(m_theme_name, os_prefers_light);
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
        row.full_filename_tooltip = t.file_name;
        row.display_filename = TruncateFilenameGracefully(t.file_name, max_fn_chars);
        row.is_filename_truncated = (row.display_filename != t.file_name);
        row.is_stream = (t.task_type != TaskType::kHttpFile);

        if (t.task_type == TaskType::kStreamHls) row.file_type_badge = "HLS STREAM";
        else if (t.task_type == TaskType::kStreamDash) row.file_type_badge = "DASH STREAM";
        else if (t.task_type == TaskType::kYouTubeExtraction) row.file_type_badge = "YOUTUBE";
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
            return "No video or HLS/DASH stream downloads yet. Open a video page with the OpenIDM Browser Extension enabled.";
        case SidebarCategory::kScheduled:
            return "No scheduled tasks configured.";
        default:
            return "No tasks in this view.";
    }
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
            m_last_feedback_message = "Opened Settings Dialog";
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
    m_last_feedback_message = "Max concurrent downloads set to " + std::to_string(m_controller.GetMaxConcurrentTasks());
}

void DesktopView::ActionSetSpeedLimit(uint64_t bps) {
    m_controller.SetGlobalSpeedLimit(bps);
    m_last_feedback_message = (bps == 0) ? "Speed limit set to Unlimited" : ("Speed limit set to " + FormatSpeed(bps));
}

void DesktopView::ActionSetBrowserCapture(bool enabled) {
    auto s = m_controller.GetSettings();
    s.browser_capture_enabled = enabled;
    m_controller.UpdateSettings(s);
    if (m_ipc_server) {
        m_ipc_server->SetCaptureEnabled(enabled);
    }
    m_last_feedback_message = enabled ? "Browser download capture enabled" : "Browser download capture paused";
}

Result<void> DesktopView::ActionSaveSettings(const AppSettings& settings) {
    auto r = m_controller.UpdateSettings(settings);
    if (r.ok()) {
        SetTheme(settings.theme, false);
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
        SetTheme(r.value->theme, false);
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
    oss << " Flux Download Manager v0.3.0 — Unified Download Manager  |  Category: " << CategoryToLabel(m_selected_category)
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
// Native Windows 11 Win32 + GDI Double-Buffered Window, Hit-Testing, Context Menus & Dialogs
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
constexpr int IDM_CTX_BROWSER_STAT = 3010;
constexpr int IDM_CTX_ABOUT        = 3011;

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

void ShowAddUrlQuickDialogWin32(HWND hwnd, DesktopView& view, DownloadController& controller) {
    // Checks clipboard for http:// or https:// URL to pre-populate the Add Download confirmation dialog
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

    std::wstring prompt = L"Add download from clipboard URL?\n\nURL: " + Utf8ToW(clip_url) +
                          L"\nDestination: " + Utf8ToW(controller.GetSettings().default_download_dir);
    if (MessageBoxW(hwnd, prompt.c_str(), L"Flux — Add Download", MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
        AddDownloadDialogInput in;
        in.url = clip_url;
        in.save_dir = controller.GetSettings().default_download_dir;
        in.max_connections = controller.GetSettings().default_connections_per_task;
        in.start_immediately = true;
        view.SubmitAddDownloadDialog(in);
    }
}

void ShowSettingsModalWin32(HWND hwnd, DesktopView& view, DownloadController& controller) {
    auto s = controller.GetSettings();
    std::wstring msg = L"Current Flux Settings:\n\n"
                       L"• Theme: " + Utf8ToW(s.theme) + L" (Click 'Yes' to toggle Dark / Light theme)\n"
                       L"• Max Concurrent Downloads: " + std::to_wstring(s.max_concurrent_tasks) + L"\n"
                       L"• Default Connections per Task: " + std::to_wstring(s.default_connections_per_task) + L"\n"
                       L"• Default Download Folder: " + Utf8ToW(s.default_download_dir) + L"\n"
                       L"• Browser Capture: " + (s.browser_capture_enabled ? L"Enabled" : L"Paused") + L"\n"
                       L"• Close Window to System Tray: " + (s.close_to_tray ? L"Yes" : L"No") + L"\n\n"
                       L"Click [Yes] to switch Theme, [No] to Restore Default Settings, or [Cancel] to close.";
    int r = MessageBoxW(hwnd, msg.c_str(), L"Flux — Settings", MB_YESNOCANCEL | MB_ICONINFORMATION);
    if (r == IDYES) {
        s.theme = (s.theme == "dark") ? "light" : "dark";
        view.ActionSaveSettings(s);
    } else if (r == IDNO) {
        view.ActionRestoreDefaultSettings();
    }
}

void ShowBrowserStatusModalWin32(HWND hwnd, DesktopView& view) {
    auto st = view.GetBrowserIntegrationStatus();
    std::wstring msg = L"Browser Integration & Stream Worker Status:\n\n"
                       L"• Local IPC Server: " + std::wstring(st.ipc_server_running ? L"Listening" : L"Stopped") + L"\n"
                       L"• IPC Endpoint: " + Utf8ToW(st.ipc_endpoint) + L"\n"
                       L"• Browser Capture: " + std::wstring(st.capture_enabled ? L"Enabled" : L"Disabled") + L"\n"
                       L"• Stream Worker (N_m3u8DL-RE): " + std::wstring(st.nm3u8dl_available ? L"Ready (" : L"Missing (") + Utf8ToW(st.nm3u8dl_path) + L")\n"
                       L"• Media Muxer (FFmpeg): " + std::wstring(st.ffmpeg_available ? L"Ready (" : L"Missing (") + Utf8ToW(st.ffmpeg_path) + L")";
    MessageBoxW(hwnd, msg.c_str(), L"Flux — Browser Integration Status", MB_OK | MB_ICONINFORMATION);
}

void ShowAboutModalWin32(HWND hwnd, DesktopView& view) {
    auto ab = view.GetAboutDialogView();
    std::ostringstream oss;
    oss << ab.app_name << " v" << ab.version << "\n"
        << "License: " << ab.license << "\n"
        << "Platform: " << ab.target_platform << "\n\n"
        << "Integrated Upstream Projects:\n";
    for (const auto& attr : ab.upstream_attributions) {
        oss << "• " << attr << "\n";
    }
    MessageBoxW(hwnd, Utf8ToW(oss.str()).c_str(), L"About Flux Download Manager", MB_OK | MB_ICONINFORMATION);
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

    FillRectSolid(hdc, client_rc, pal.app_background);
    SetBkMode(hdc, TRANSPARENT);

    HFONT hFontBody = CreateFontW(
        -m.font_body_px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HFONT hFontBold = CreateFontW(
        -m.font_body_strong_px, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    HGDIOBJ old_font = SelectObject(hdc, hFontBold);

    // 1. Top Toolbar using exact geometry from ComputeLayoutGeometry()
    RECT toolbar_rc = ToWinRect(geom.toolbar_rect);
    FillRectSolid(hdc, toolbar_rc, pal.surface_primary);
    for (size_t i = 0; i < geom.toolbar_buttons.size(); ++i) {
        const auto& btn = geom.toolbar_buttons[i];
        RECT brc = ToWinRect(btn.rect);
        bool hovered = (view.GetHoveredToolbarIndex() == static_cast<int>(i));
        ColorRgba bg = btn.is_primary
            ? (hovered ? pal.accent_hover : pal.accent_primary)
            : (hovered && btn.enabled ? pal.surface_hover : pal.surface_elevated);
        FillRectSolid(hdc, brc, bg);
        SetTextColor(hdc, (btn.enabled || btn.is_primary ? pal.text_primary : pal.text_muted).ToWin32ColorRef());
        std::wstring wlbl = Utf8ToW(btn.label);
        DrawTextW(hdc, wlbl.c_str(), -1, &brc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // 2. Left Navigation Sidebar
    RECT sidebar_rc = ToWinRect(geom.sidebar_rect);
    FillRectSolid(hdc, sidebar_rc, pal.surface_primary);
    SelectObject(hdc, hFontBody);
    for (size_t i = 0; i < geom.sidebar_items.size(); ++i) {
        const auto& item = geom.sidebar_items[i];
        RECT crc = ToWinRect(item.rect);
        if (item.selected) {
            FillRectSolid(hdc, crc, pal.surface_selected);
        } else if (view.GetHoveredSidebarIndex() == static_cast<int>(i)) {
            FillRectSolid(hdc, crc, pal.surface_hover);
        }
        SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
        std::wstring lbl = Utf8ToW(item.label + " (" + std::to_string(item.count) + ")");
        RECT text_rc = crc;
        text_rc.left += m.space_sm;
        DrawTextW(hdc, lbl.c_str(), -1, &text_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // 3. Sortable Table Column Headers
    RECT header_rc = ToWinRect(geom.table_header_rect);
    FillRectSolid(hdc, header_rc, pal.surface_primary);
    SelectObject(hdc, hFontBold);
    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
    for (const auto& col : geom.table_columns) {
        RECT crc = ToWinRect(col.rect);
        crc.left += m.space_sm;
        std::string title = col.title;
        if (col.sort_column == view.GetSortColumn() && col.title != "Actions") {
            title += view.IsSortAscending() ? " [^]" : " [v]";
        }
        std::wstring wtitle = Utf8ToW(title);
        DrawTextW(hdc, wtitle.c_str(), -1, &crc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // 4. Main Task List & Progress Bars
    SelectObject(hdc, hFontBody);
    auto rows = view.BuildVisibleTaskRows(0, geom.task_rows.size());
    if (rows.empty()) {
        RECT empty_rc = ToWinRect(geom.table_body_rect);
        empty_rc.left += m.space_xl;
        empty_rc.right -= m.space_xl;
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

            SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
            std::wstring row_line = Utf8ToW(
                "[" + r.file_type_badge + "]  " + r.display_filename + "   |   " +
                r.status_text + "   |   " + r.progress_text + "   |   " +
                r.size_text + "   |   " + r.speed_text);
            RECT trc = rrc;
            trc.left += m.space_sm;
            trc.right = rl.action_button_rect.left - m.space_sm;
            trc.bottom -= 10;
            DrawTextW(hdc, row_line.c_str(), -1, &trc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            // Progress bar track & fill
            RECT ptrack = ToWinRect(rl.progress_bar_rect);
            FillRectSolid(hdc, ptrack, pal.progress_track);
            int fill_w = static_cast<int>(static_cast<double>(ptrack.right - ptrack.left) * r.progress_fraction);
            if (fill_w > 0) {
                RECT pfill = ptrack;
                pfill.right = ptrack.left + fill_w;
                FillRectSolid(hdc, pfill, r.is_stream ? pal.badge_stream : pal.accent_primary);
            }

            // Row inline action button
            RECT act_rc = ToWinRect(rl.action_button_rect);
            FillRectSolid(hdc, act_rc, pal.surface_elevated);
            std::wstring wact = Utf8ToW(r.primary_action_label);
            DrawTextW(hdc, wact.c_str(), -1, &act_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    // Tooltip overlay if hovering over a truncated filename
    if (!view.GetActiveTooltip().empty()) {
        RECT tip_rc{geom.table_body_rect.left + m.space_md, geom.table_body_rect.top + 4,
                    geom.table_body_rect.right - m.space_md, geom.table_body_rect.top + 28};
        FillRectSolid(hdc, tip_rc, pal.surface_elevated);
        SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
        std::wstring wtip = Utf8ToW("  Full Name: " + view.GetActiveTooltip());
        DrawTextW(hdc, wtip.c_str(), -1, &tip_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // 5. Bottom Details Panel
    RECT details_rc = ToWinRect(geom.details_panel_rect);
    FillRectSolid(hdc, details_rc, pal.surface_primary);
    auto det = view.BuildSelectedTaskDetails();
    RECT dtext_rc = details_rc;
    dtext_rc.left += m.space_md;
    dtext_rc.top += m.space_sm;
    dtext_rc.right -= m.space_md;
    dtext_rc.bottom -= m.space_sm;
    SetTextColor(hdc, pal.text_secondary.ToWin32ColorRef());
    if (det.has_selection) {
        std::wstring dstr = Utf8ToW(
            "Selected Task: " + det.file_name + "\n" +
            "Source URL: " + det.redacted_source_url + "\n" +
            "Destination: " + det.destination_path + "\n" +
            "Engine: " + det.engine_name + "  |  Format: " + det.stream_format + "\n" +
            "Tracks: " + det.selected_tracks_summary + "  |  Retries: " + std::to_string(det.retry_count) + "\n" +
            "Status / Error: " + det.error_explanation);
        DrawTextW(hdc, dstr.c_str(), -1, &dtext_rc, DT_LEFT | DT_TOP | DT_WORDBREAK);
    } else {
        DrawTextW(hdc, L"Select a download task to inspect its transfer details, tracks, and diagnostics.", -1, &dtext_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    // 6. Status Bar
    RECT status_rc = ToWinRect(geom.status_bar_rect);
    FillRectSolid(hdc, status_rc, pal.surface_elevated);
    auto sb = view.BuildStatusBar();
    std::wstring sb_str = Utf8ToW("  " + sb.status_message + "   |   " + view.GetLastFeedbackMessage());
    SetTextColor(hdc, pal.text_primary.ToWin32ColorRef());
    DrawTextW(hdc, sb_str.c_str(), -1, &status_rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, old_font);
    DeleteObject(hFontBody);
    DeleteObject(hFontBold);

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
                        ShowSettingsModalWin32(hwnd, *ctx->view, *ctx->controller);
                        BOOL use_dark = (ctx->view->GetPalette().mode == ThemeMode::kDark) ? TRUE : FALSE;
                        DwmSetWindowAttribute(hwnd, 20, &use_dark, sizeof(use_dark));
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
            } else if (cmd == IDM_CTX_BROWSER_STAT) ShowBrowserStatusModalWin32(hwnd, *ctx->view);
            else if (cmd == IDM_CTX_ABOUT) ShowAboutModalWin32(hwnd, *ctx->view);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_TIMER: {
            if (wParam == IDT_UI_REFRESH) {
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }
        case (WM_APP + 105): {
            if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) {
                ShowWindow(hwnd, SW_RESTORE);
            }
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
                ShowSettingsModalWin32(hwnd, *ctx->view, *ctx->controller);
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
                if (cmd == IDM_TRAY_SHOW) ShowWindow(hwnd, SW_RESTORE);
                else if (cmd == IDM_TRAY_HIDE) ShowWindow(hwnd, SW_HIDE);
                else if (cmd == IDM_TRAY_PAUSE_ALL) ctx->controller->PauseAllTasks();
                else if (cmd == IDM_TRAY_RESUME_ALL) ctx->controller->ResumeAllTasks();
                else if (cmd == IDM_TRAY_EXIT) DestroyWindow(hwnd);
            } else if (lParam == WM_LBUTTONDBLCLK) {
                ShowWindow(hwnd, SW_RESTORE);
                SetForegroundWindow(hwnd);
            }
            return 0;
        }
        case WM_CLOSE: {
            if (ctx->controller->GetSettings().close_to_tray && ctx->tray_added) {
                ShowWindow(hwnd, SW_HIDE);
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
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = OpenIdmWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
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

    BOOL use_dark = (m_palette.mode == ThemeMode::kDark) ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd, 20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */, &use_dark, sizeof(use_dark));

    ctx.nid.cbSize = sizeof(NOTIFYICONDATAW);
    ctx.nid.hWnd = hwnd;
    ctx.nid.uID = 1;
    ctx.nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    ctx.nid.uCallbackMessage = WM_OPENIDM_TRAYICON;
    ctx.nid.hIcon = wc.hIcon;
    wcsncpy_s(ctx.nid.szTip, L"Flux Download Manager", _TRUNCATE);
    ctx.tray_added = (Shell_NotifyIconW(NIM_ADD, &ctx.nid) != FALSE);

    SetTimer(hwnd, IDT_UI_REFRESH, 100, nullptr);
    ShowWindow(hwnd, m_controller.GetSettings().start_minimized ? SW_HIDE : SW_SHOWDEFAULT);
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
