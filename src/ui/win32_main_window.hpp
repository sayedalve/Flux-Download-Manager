// SPDX-License-Identifier: GPL-3.0-or-later
// Flux Download Manager — Windows 11 Cosmic Dark Desktop Interface, Star-Field Controller,
// Multi-Column Hit-Testing, Interaction Controller & Professional Settings Center
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/download_controller.hpp"
#include "ipc/ipc_server.hpp"
#include "ui/design_system.hpp"

namespace openidm::ui {

enum class SidebarCategory {
    kAllDownloads = 0,
    kInProgress,
    kQueued,
    kCompleted,
    kFailed,
    kVideoDownloads,
    kScheduled
};

enum class SortColumn {
    kCreatedAt = 0,
    kFileName,
    kDownloadType,
    kStatus,
    kProgress,
    kSize,
    kSpeed,
    kEta
};

enum class ToolbarActionId {
    kAddUrl = 0,
    kStart,
    kPause,
    kResume,
    kRetry,
    kCancel,
    kRemove,
    kClearHistory,
    kOpenFolder,
    kSettings
};

enum class SettingsCategory {
    kGeneral = 0,
    kDownloads,
    kSaveLocations,
    kConnections,
    kSpeedLimits,
    kBrowserIntegration,
    kAboutAndDiagnostics
};

enum class SettingsControlType {
    kToggleSwitch = 0,
    kNumericStepper,
    kFolderSelector,
    kBandwidthControl,
    kActionButton,
    kReadOnlyStatus,
    kCategoryRuleTable
};

enum class HitTargetType {
    kNone = 0,
    kToolbarButton,
    kSearchBox,
    kSidebarCategory,
    kTableHeaderColumn,
    kTaskRow,
    kTaskRowActionButton
};

struct HitTestResult {
    HitTargetType type{HitTargetType::kNone};
    int index{-1};
    std::string task_id;
    std::string tooltip_text;
};

struct ToolbarButtonLayout {
    ToolbarActionId action_id;
    std::string label;
    bool enabled{false};
    bool is_primary{false};
    RectI rect;
};

struct SidebarCategoryLayout {
    SidebarCategory category;
    std::string label;
    size_t count{0};
    bool selected{false};
    RectI rect;
};

struct TableColumnLayout {
    SortColumn sort_column;
    std::string title;
    RectI rect;
};

struct TaskRowLayout {
    std::string task_id;
    RectI row_rect;
    RectI filename_rect;
    RectI progress_bar_rect;
    RectI action_button_rect;
    std::vector<RectI> cell_rects;
};

struct WindowLayoutGeometry {
    RectI toolbar_rect;
    RectI search_box_rect;
    RectI sidebar_rect;
    RectI table_header_rect;
    RectI table_body_rect;
    RectI details_panel_rect;
    RectI status_bar_rect;
    std::vector<ToolbarButtonLayout> toolbar_buttons;
    std::vector<SidebarCategoryLayout> sidebar_items;
    std::vector<TableColumnLayout> table_columns;
    std::vector<TaskRowLayout> task_rows;
};

struct ToolbarState {
    bool can_add_url{true};
    bool can_start{false};
    bool can_pause{false};
    bool can_resume{false};
    bool can_retry{false};
    bool can_cancel{false};
    bool can_remove{false};
    bool can_delete_file{false};
    bool can_open_file{false};
    bool can_open_folder{false};
    bool can_clear_history{false};
    bool can_open_settings{true};
};

struct CategoryCounts {
    size_t all_downloads{0};
    size_t in_progress{0};
    size_t queued{0};
    size_t completed{0};
    size_t failed{0};
    size_t video_downloads{0};
    size_t scheduled{0};
};

struct FormattedTaskRow {
    std::string task_id;
    TaskState task_state{TaskState::kQueued};
    std::string display_filename;
    std::string full_filename_tooltip;
    bool is_filename_truncated{false};
    std::string file_type_badge;
    bool is_stream{false};
    std::string status_text;
    std::string status_tone; // "active", "probing", "paused", "queued", "completed", "failed", "canceled"
    ColorRgba status_color;
    ColorRgba badge_color;
    std::string progress_text;
    double progress_fraction{0.0};
    bool progress_indeterminate{false};
    std::string size_text;
    std::string speed_text;
    std::string eta_text;
    std::string primary_action_label;
    bool primary_action_enabled{true};
    bool selected{false};
};

struct TaskDetailsViewModel {
    bool has_selection{false};
    std::string task_id;
    std::string file_name;
    std::string redacted_source_url;
    std::string redacted_page_url;
    std::string destination_path;
    std::string size_summary;
    std::string progress_summary;
    std::string speed_summary;
    std::string created_time_str;
    std::string updated_time_str;
    std::string completed_time_str;
    uint32_t retry_count{0};
    std::string engine_name;
    std::string stream_format;
    std::string selected_tracks_summary;
    std::string redacted_headers_summary;
    std::string error_code;
    std::string error_explanation;
};

struct StatusBarViewModel {
    size_t active_downloads{0};
    uint64_t total_speed_bps{0};
    std::string formatted_total_speed;
    std::string formatted_speed_limit;
    size_t remaining_queued{0};
    bool browser_ipc_listening{false};
    bool browser_capture_enabled{true};
    bool workers_ready{false};
    bool cosmic_motion_active{true};
    std::string status_message;
};

struct AddDownloadDialogInput {
    std::string url;
    std::string custom_filename;
    std::string save_dir;
    uint32_t max_connections{8};
    bool start_immediately{true};
    bool scheduled{false};
    std::string schedule_time_local;
    bool is_stream{false};
    std::string video_filter{"best"};
    std::string audio_filter{"all"};
    std::string subtitle_filter{"all"};
    std::string output_format{"mp4"};
};

struct BrowserIntegrationStatusView {
    bool ipc_server_running{false};
    std::string ipc_endpoint;
    std::string native_host_id{"com.flux.downloadmanager"};
    bool capture_enabled{true};
    uint64_t min_capture_size_bytes{1048576};
    std::vector<std::string> intercepted_extensions;
    bool nm3u8dl_available{false};
    std::string nm3u8dl_path;
    bool ffmpeg_available{false};
    std::string ffmpeg_path;
    std::vector<std::string> supported_browsers;
};

struct AboutDialogView {
    std::string app_name{"Flux Download Manager"};
    std::string version{"0.3.0"};
    std::string license{"GPL-3.0-or-later"};
    std::string target_platform{"Windows 11 x64 (Native Win32 + Direct2D/GDI)"};
    std::string data_dir_path;
    std::string state_db_path;
    std::vector<std::string> upstream_attributions;
};

struct FileCategoryRuleView {
    std::string category_name;
    std::string subfolder_path;
    std::string extensions_summary;
};

struct SettingsItemViewModel {
    std::string key;
    SettingsCategory category{SettingsCategory::kGeneral};
    SettingsControlType control_type{SettingsControlType::kReadOnlyStatus};
    std::string label;
    std::string description;
    std::string current_value_display;
    std::string default_value_display;
    bool is_modified{false};
    bool is_advanced{false};
    bool is_dangerous{false};
    bool requires_confirmation{false};
    bool enabled{true};
    std::string validation_error;
};

struct SettingsCategorySummary {
    SettingsCategory category{SettingsCategory::kGeneral};
    std::string title;
    std::string subtitle;
    size_t item_count{0};
    size_t modified_count{0};
    bool selected{false};
    RectI rect;
};

struct SettingsCenterViewModel {
    bool is_open{false};
    SettingsCategory selected_category{SettingsCategory::kGeneral};
    std::string search_query;
    bool show_advanced{true};
    bool has_unsaved_changes{false};
    bool is_valid{true};
    std::string validation_summary;
    std::string status_banner;
    AppSettings draft_settings;
    AppSettings committed_settings;
    std::vector<SettingsCategorySummary> categories;
    std::vector<SettingsItemViewModel> visible_items;
    std::vector<FileCategoryRuleView> category_rules;
    BrowserIntegrationStatusView browser_status;
    AboutDialogView about_info;
};

class DesktopView {
public:
    explicit DesktopView(DownloadController& controller, ipc::IpcServer* ipc_server = nullptr);

    // Layout, Theme, DPI & Hit-Testing Geometry
    void SetWindowDimensions(int width, int height, uint32_t dpi = 96);
    int GetWindowWidth() const { return m_window_width; }
    int GetWindowHeight() const { return m_window_height; }
    const LayoutMetrics& GetMetrics() const { return m_metrics; }
    const ThemePalette& GetPalette() const { return m_palette; }
    bool IsCompactColumnsMode() const { return m_window_width < m_metrics.compact_width_threshold; }
    std::vector<std::string> GetVisibleColumns() const;

    // Cosmic Star-Field Background State & Performance Lifecycle
    void SetWindowVisibility(bool visible, bool minimized = false);
    bool IsWindowVisibleState() const { return m_window_visible && !m_window_minimized; }
    bool IsCosmicAnimationActive() const;
    void AdvanceCosmicAnimation(int64_t delta_ms);
    int64_t GetCosmicElapsedMs() const { return m_cosmic_elapsed_ms; }
    CosmicBackgroundState ComputeCosmicBackgroundState(int64_t elapsed_ms = -1) const;

    WindowLayoutGeometry ComputeLayoutGeometry(size_t scroll_offset = 0) const;
    HitTestResult HitTestPoint(int x, int y, size_t scroll_offset = 0) const;
    bool HandleMouseClick(int x, int y, bool double_click = false, size_t scroll_offset = 0);
    HitTestResult HandleMouseMove(int x, int y, size_t scroll_offset = 0);
    const std::string& GetActiveTooltip() const { return m_active_tooltip; }
    int GetHoveredToolbarIndex() const { return m_hovered_toolbar_index; }
    int GetHoveredSidebarIndex() const { return m_hovered_sidebar_index; }
    int GetHoveredRowIndex() const { return m_hovered_row_index; }

    void SetTheme(std::string_view theme_name, bool os_prefers_light = false);
    std::string GetThemeName() const { return m_theme_name; }

    static std::string FormatBytes(uint64_t bytes);
    static std::string FormatSpeed(uint64_t bps);
    static std::string FormatBandwidthLimit(uint64_t bps);
    static Result<uint64_t> ParseBandwidthLimitInput(std::string_view text);
    static std::string FormatEta(uint64_t remaining_bytes, uint64_t speed_bps);
    static std::string TruncateFilenameGracefully(std::string_view filename, size_t max_chars = 38);

    // Sidebar Category Filtering, Sorting, Search & Selection
    void SelectCategory(SidebarCategory category);
    SidebarCategory GetSelectedCategory() const { return m_selected_category; }
    static std::string CategoryToLabel(SidebarCategory category);
    CategoryCounts ComputeCategoryCounts() const;

    void SetSearchQuery(std::string query);
    const std::string& GetSearchQuery() const { return m_search_query; }

    void SetSort(SortColumn column, bool ascending);
    void ToggleSortColumn(SortColumn column);
    SortColumn GetSortColumn() const { return m_sort_column; }
    bool IsSortAscending() const { return m_sort_ascending; }

    void SelectTask(const std::string& task_id);
    std::string GetSelectedTaskId() const { return m_selected_task_id; }
    void SelectNextRow();
    void SelectPrevRow();
    void SelectFirstRow();
    void SelectLastRow();

    // Computed View Models
    std::vector<TaskModel> GetFilteredAndSortedTasks() const;
    std::vector<FormattedTaskRow> BuildVisibleTaskRows(size_t scroll_offset = 0, size_t max_visible_rows = 100) const;
    ToolbarState ComputeToolbarState() const;
    TaskDetailsViewModel BuildSelectedTaskDetails() const;
    StatusBarViewModel BuildStatusBar() const;
    std::string GetEmptyStateMessage() const;

    // Professional Download Manager Settings Center
    void OpenSettingsCenter(SettingsCategory initial_category = SettingsCategory::kGeneral);
    void CloseSettingsCenter(bool discard_unsaved = true);
    bool IsSettingsCenterOpen() const { return m_settings_open; }
    void SelectSettingsCategory(SettingsCategory category);
    SettingsCategory GetSelectedSettingsCategory() const { return m_settings_category; }
    static std::string SettingsCategoryToLabel(SettingsCategory category);
    void SetSettingsSearchQuery(std::string query);
    const std::string& GetSettingsSearchQuery() const { return m_settings_search_query; }
    void SetShowAdvancedSettings(bool show_advanced);
    bool GetShowAdvancedSettings() const { return m_settings_show_advanced; }

    const AppSettings& GetSettingsDraft() const { return m_settings_draft; }
    void SetSettingsDraft(const AppSettings& draft);
    Result<void> SetSettingsDraftField(std::string_view key, std::string_view value_str);
    Result<void> ResetSettingsDraftField(std::string_view key);
    Result<void> ValidateSettingsDraft() const;
    Result<void> ApplySettingsDraft();
    void CancelSettingsDraft();
    std::vector<FileCategoryRuleView> GetSupportedCategoryRules() const;
    SettingsCenterViewModel BuildSettingsCenterViewModel() const;

    // Interactive Actions & Dialog Controllers
    Result<TaskModel> SubmitAddDownloadDialog(const AddDownloadDialogInput& input);
    Result<stream::StreamProbeResult> ProbeStreamForDialog(
        const std::string& manifest_url,
        const std::map<std::string, std::string>& headers = {},
        const std::map<std::string, std::string>& auth_headers = {});
    Result<TaskModel> ConfirmStreamSelectionDialog(
        const stream::StreamProbeResult& probe_result,
        const std::string& page_title,
        const std::string& save_dir,
        const StreamSelectionOptions& chosen_options,
        const std::map<std::string, std::string>& headers = {},
        const std::map<std::string, std::string>& auth_headers = {});

    Result<void> TriggerToolbarAction(ToolbarActionId action_id);
    Result<void> TriggerRowPrimaryAction(const std::string& task_id);

    Result<void> ActionStartSelected();
    Result<void> ActionPauseSelected();
    Result<void> ActionResumeSelected();
    Result<void> ActionRetrySelected();
    Result<void> ActionCancelSelected();
    Result<void> ActionRemoveSelectedFromList();
    Result<void> ActionDeleteSelectedWithFile();
    Result<int> ActionClearCompletedHistory();
    Result<std::filesystem::path> ActionOpenCompletedFile(bool dry_run = false);
    Result<std::filesystem::path> ActionOpenDestinationFolder(bool dry_run = false);

    void ActionSetConcurrentLimit(uint32_t max_tasks);
    void ActionSetSpeedLimit(uint64_t bps);
    void ActionSetBrowserCapture(bool enabled);
    Result<void> ActionSaveSettings(const AppSettings& settings);
    Result<AppSettings> ActionRestoreDefaultSettings();

    bool HandleKeyShortcut(const std::string& key_chord);

    BrowserIntegrationStatusView GetBrowserIntegrationStatus() const;
    AboutDialogView GetAboutDialogView() const;
    const std::string& GetLastFeedbackMessage() const { return m_last_feedback_message; }

    std::string RenderQueueTableText() const;
    std::string RenderFullDesktopSnapshotText() const;

    int RunApplicationLoop(bool headless_exit_immediately = false);

#ifdef _WIN32
    int RunWin32MessageLoop();
#endif

private:
    bool MatchesCategory(const TaskModel& t, SidebarCategory cat) const;
    bool MatchesSearch(const TaskModel& t) const;
    static AppSettings BuildDefaultAppSettings(const DownloadController& controller);
    std::vector<SettingsItemViewModel> BuildAllSettingsItems(
        const AppSettings& draft,
        const AppSettings& committed,
        const AppSettings& defaults) const;

    DownloadController& m_controller;
    ipc::IpcServer* m_ipc_server{nullptr};

    int m_window_width{1180};
    int m_window_height{720};
    LayoutMetrics m_metrics;
    std::string m_theme_name{"dark"};
    ThemePalette m_palette;

    bool m_window_visible{true};
    bool m_window_minimized{false};
    int64_t m_cosmic_elapsed_ms{0};

    SidebarCategory m_selected_category{SidebarCategory::kAllDownloads};
    SortColumn m_sort_column{SortColumn::kCreatedAt};
    bool m_sort_ascending{false};
    std::string m_search_query;
    std::string m_selected_task_id;
    std::string m_last_feedback_message{"Ready"};

    bool m_settings_open{false};
    SettingsCategory m_settings_category{SettingsCategory::kGeneral};
    std::string m_settings_search_query;
    bool m_settings_show_advanced{true};
    AppSettings m_settings_draft;
    std::string m_settings_validation_error;

    int m_hovered_toolbar_index{-1};
    int m_hovered_sidebar_index{-1};
    int m_hovered_row_index{-1};
    std::string m_active_tooltip;
};

} // namespace openidm::ui
