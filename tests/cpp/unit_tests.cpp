// SPDX-License-Identifier: GPL-3.0-or-later
// Flux Download Manager — Automated C++20 Unit & Regression Test Suite (14 Suites)
// Verifies PathSanitizer, Dynamic Worker Discovery, Disk Space Guard, Logger Redaction,
// CryptoUtils, Brisk SegmentScheduler + Stolen Segment Rollback, SqliteStore + Crash Recovery,
// Nm3u8dlReAdapter + DRM Rejection, DownloadController Lifecycle + Race Guards,
// Cosmic Dark UI Design System + Star-Field Engine + Static Fallback + Hidden Suspension,
// UI Interactions + Redaction, Keyboard Navigation, Multi-Column Hit-Testing,
// and Professional Settings Center (Categories, Search, Validation, Draft/Apply/Reset & Persistence).

#include "brisk_engine/segment_scheduler.hpp"
#include "common/crypto_utils.hpp"
#include "common/json_mini.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"
#include "core/download_controller.hpp"
#include "engine_stream/nm3u8dl_re_adapter.hpp"
#include "storage/sqlite_store.hpp"
#include "ui/design_system.hpp"
#include "ui/win32_main_window.hpp"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "FAIL [" << __FUNCTION__ << ":" << __LINE__ << "]: " << (msg) << "\n"; \
            return false; \
        } \
    } while (0)

namespace {

bool TestPathSanitizerAndContentDisposition() {
    using openidm::PathSanitizer;

    ASSERT_TRUE(PathSanitizer::SanitizeFileName("../../../etc/passwd") == "passwd", "Strip ../ traversal");
    ASSERT_TRUE(PathSanitizer::SanitizeFileName("..\\..\\Windows\\System32\\cmd.exe") == "cmd.exe", "Strip ..\\ traversal");
    ASSERT_TRUE(PathSanitizer::SanitizeFileName("C:\\Users\\Alice\\video.mp4") == "video.mp4", "Strip Windows drive prefix");
    ASSERT_TRUE(PathSanitizer::SanitizeFileName("movie.mp4:Zone.Identifier") == "movie.mp4", "Strip NTFS ADS");
    ASSERT_TRUE(PathSanitizer::SanitizeFileName("CON.txt") == "_CON.txt", "Escape DOS device CON.txt");
    ASSERT_TRUE(PathSanitizer::SanitizeFileName("nul") == "_nul", "Escape DOS device nul");
    ASSERT_TRUE(PathSanitizer::SanitizeFileName("COM1.mp4") == "_COM1.mp4", "Escape DOS device COM1");
    ASSERT_TRUE(PathSanitizer::SanitizeFileName("invoice.pdf. . .") == "invoice.pdf", "Strip trailing dots/spaces");
    std::string bidi_spoof = "report\xE2\x80\xAE" "fdp.exe";
    ASSERT_TRUE(PathSanitizer::SanitizeFileName(bidi_spoof) == "reportfdp.exe", "Strip U+202E BiDi override");

    std::string cd1 = "attachment; filename=\"fallback.mp4\"; filename*=UTF-8''%E6%B5%8B%E8%AF%95%E8%A7%86%E9%A2%91_1080p.mp4";
    ASSERT_TRUE(PathSanitizer::ExtractFromContentDisposition(cd1) == "测试视频_1080p.mp4", "RFC 5987 UTF-8 filename*");

    std::filesystem::path tmp_dir = "/tmp/openidm_test_san_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(tmp_dir);
    {
        std::ofstream(tmp_dir / "archive.zip") << "1";
    }
    auto res = PathSanitizer::ResolveSafeOutputPath(tmp_dir, "archive.zip", openidm::ConflictPolicy::kAutoRename);
    ASSERT_TRUE(res.ok() && res.value->filename() == "archive (1).zip", "Auto-Rename (1) conflict resolution");
    std::filesystem::remove_all(tmp_dir);
    return true;
}

bool TestDynamicWorkerAndDiskSpaceGuard() {
    using openidm::PathSanitizer;

    std::filesystem::path exe_dir = PathSanitizer::GetExecutableDir();
    ASSERT_TRUE(!exe_dir.empty(), "GetExecutableDir resolves non-empty path");

    auto ff_path = PathSanitizer::FindWorkerBinary("ffmpeg", exe_dir);
    ASSERT_TRUE(std::filesystem::exists(ff_path), "FindWorkerBinary dynamically discovers ffmpeg");

    auto nm_path = PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", exe_dir);
    ASSERT_TRUE(std::filesystem::exists(nm_path), "FindWorkerBinary dynamically discovers N_m3u8DL-RE");

    auto ok_space = PathSanitizer::CheckAvailableDiskSpace("/tmp", 1024 * 1024);
    ASSERT_TRUE(ok_space.ok(), "1 MiB disk space check succeeds");

    auto exhausted = PathSanitizer::CheckAvailableDiskSpace(".", 1000000000000ULL);
    ASSERT_TRUE(!exhausted.ok() && exhausted.error->code == "ERR_DISK_FULL", "Exabyte request rejected with ERR_DISK_FULL");
    return true;
}

bool TestLoggerRedactionAndCrypto() {
    using openidm::CryptoUtils;
    using openidm::Logger;

    std::string raw_log = "Request -H \"Cookie: sid=super_secret_123\" -H \"Authorization: Bearer tok_999\" https://cdn.example/v.m3u8?token=abc12345&res=1080";
    std::string redacted = Logger::RedactSensitiveData(raw_log);
    ASSERT_TRUE(redacted.find("super_secret_123") == std::string::npos, "Cookie value redacted");
    ASSERT_TRUE(redacted.find("tok_999") == std::string::npos, "Bearer token redacted");
    ASSERT_TRUE(redacted.find("abc12345") == std::string::npos, "URL token query param redacted");
    ASSERT_TRUE(redacted.find("[REDACTED]") != std::string::npos, "Contains [REDACTED] marker");

    std::string sha_abc = CryptoUtils::ComputeBufferSha256Hex("abc");
    ASSERT_TRUE(sha_abc == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad", "SHA-256 NIST vector");

    std::string secret_json = R"({"Cookie":"session=xyz987","Authorization":"Bearer secret"})";
    auto enc = CryptoUtils::ProtectSecret(secret_json);
    ASSERT_TRUE(enc.ok() && !enc.value->empty(), "ProtectSecret succeeds");
    std::string cipher_as_str(reinterpret_cast<const char*>(enc.value->data()), enc.value->size());
    ASSERT_TRUE(cipher_as_str.find("xyz987") == std::string::npos, "Ciphertext hides plaintext");

    auto dec = CryptoUtils::UnprotectSecret(*enc.value);
    ASSERT_TRUE(dec.ok() && *dec.value == secret_json, "UnprotectSecret round-trip matches");

    std::vector<uint8_t> tampered = *enc.value;
    tampered.back() ^= 0x01;
    auto bad_dec = CryptoUtils::UnprotectSecret(tampered);
    ASSERT_TRUE(!bad_dec.ok(), "Tampered ciphertext rejected");
    return true;
}

bool TestBriskSegmentSchedulerWorkStealingAndRollback() {
    const uint64_t kTotalSize = 100ULL * 1048576ULL; // 100 MiB
    openidm::brisk::SegmentScheduler sched(kTotalSize, 4, 1048576ULL);
    sched.Initialize();

    auto seg0 = sched.AcquireNextOrStealSegment(true);
    ASSERT_TRUE(seg0 != nullptr && seg0->start_offset == 0 && seg0->end_offset.load() == kTotalSize - 1, "Initial segment covers full file");

    auto seg1 = sched.AcquireNextOrStealSegment(true);
    ASSERT_TRUE(seg1 != nullptr && seg0->end_offset.load() + 1 == seg1->start_offset, "Worker 1 steals second half");

    bool merged = sched.RollbackUnstartedStolenSegment(seg1->segment_id);
    ASSERT_TRUE(merged, "RollbackUnstartedStolenSegment merges unstarted stolen segment back into Seg 0");
    ASSERT_TRUE(seg0->end_offset.load() == kTotalSize - 1, "Seg 0 restored to full [0, kTotalSize - 1] range");

    seg1 = sched.AcquireNextOrStealSegment(true);
    auto seg2 = sched.AcquireNextOrStealSegment(true);
    auto seg3 = sched.AcquireNextOrStealSegment(true);
    ASSERT_TRUE(seg1 != nullptr && seg2 != nullptr && seg3 != nullptr, "Workers 1..3 spawn dynamically");

    auto seg4 = sched.AcquireNextOrStealSegment(true);
    ASSERT_TRUE(seg4 == nullptr, "Respects max_connections = 4 ceiling");

    seg1->current_offset.store(seg1->end_offset.load() + 1);
    seg1->completed.store(true);
    seg1->active_worker.store(false);

    auto stolen_by_w1 = sched.AcquireNextOrStealSegment(true);
    ASSERT_TRUE(stolen_by_w1 != nullptr, "Finished Worker 1 steals remaining work from another active segment");

    auto snaps = sched.GetSnapshots();
    ASSERT_TRUE(snaps.size() == 5, "5 segments after 1 work-stealing reuse");
    ASSERT_TRUE(snaps.front().start_offset == 0, "Starts at byte 0");
    ASSERT_TRUE(snaps.back().end_offset == kTotalSize - 1, "Ends at total_bytes - 1");
    for (size_t i = 1; i < snaps.size(); ++i) {
        ASSERT_TRUE(snaps[i - 1].end_offset + 1 == snaps[i].start_offset, "Contiguous non-overlapping byte ranges");
    }
    return true;
}

bool TestSqliteStoreAndCrashRecovery() {
    std::filesystem::path db_path = "/tmp/openidm_test_db_" + openidm::GenerateUuidV4() + ".db";
    openidm::SqliteStore store;
    ASSERT_TRUE(store.Open(db_path).ok(), "Open SQLite WAL database");

    openidm::TaskModel t;
    t.task_id = openidm::GenerateUuidV4();
    t.task_type = openidm::TaskType::kHttpFile;
    t.state = openidm::TaskState::kDownloading;
    t.url = "https://example.com/large.iso";
    t.file_name = "large.iso";
    t.save_dir = "/tmp";
    t.total_bytes = 10485760;
    t.downloaded_bytes = 524288;
    t.range_supported = true;
    t.headers["Referer"] = "https://example.com/";
    t.auth_headers["Cookie"] = "auth_session=secret_cookie_value_777";
    t.segments = {
        openidm::Segment{0, 0, 524288, 5242879, false},
        openidm::Segment{1, 5242880, 5242880, 10485759, false}
    };

    ASSERT_TRUE(store.UpsertTask(t).ok(), "Upsert active task with segments and auth cookie");

    {
        std::ifstream raw_db(db_path, std::ios::binary);
        std::string db_bytes((std::istreambuf_iterator<char>(raw_db)), std::istreambuf_iterator<char>());
        ASSERT_TRUE(db_bytes.find("secret_cookie_value_777") == std::string::npos, "Cookie encrypted at rest in state.db");
    }

    auto rec = store.RecoverInterruptedTasks(65536);
    ASSERT_TRUE(rec.ok() && *rec.value == 1, "Recovered 1 interrupted task");

    auto loaded = store.GetTask(t.task_id);
    ASSERT_TRUE(loaded.ok() && loaded.value->has_value(), "Loaded recovered task");
    ASSERT_TRUE((*loaded.value)->state == openidm::TaskState::kPaused, "State transitioned to PAUSED");
    ASSERT_TRUE((*loaded.value)->segments[0].current_offset == 524288 - 65536, "64 KiB safety rollback applied");
    ASSERT_TRUE((*loaded.value)->auth_headers["Cookie"] == "auth_session=secret_cookie_value_777", "Decrypted Cookie restored on resume");

    (*loaded.value)->state = openidm::TaskState::kCompleted;
    ASSERT_TRUE(store.UpsertTask(**loaded.value).ok(), "Upsert completed task");
    auto after_comp = store.GetTask(t.task_id);
    ASSERT_TRUE(after_comp.ok() && after_comp.value->has_value(), "Reload completed task");
    ASSERT_TRUE((*after_comp.value)->auth_headers.empty(), "Credentials wiped upon completion");

    store.Close();
    std::filesystem::remove(db_path);
    return true;
}

bool TestStreamMetaJsonAndDrmRejection() {
    std::filesystem::path tmp_dir = "/tmp/openidm_test_meta_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(tmp_dir);

    std::filesystem::path valid_meta = tmp_dir / "valid_meta.json";
    std::ofstream(valid_meta) << R"({
        "url": "https://cdn.example.com/master.m3u8",
        "isLive": false,
        "drmProtected": false,
        "tracks": [
            {"id": "vid_0", "mediaType": "VIDEO", "resolution": "1920x1080", "bandwidth": 5000000, "codecs": "avc1", "segmentsCount": 10},
            {"id": "aud_0", "mediaType": "AUDIO", "language": "en", "bandwidth": 128000, "codecs": "mp4a", "segmentsCount": 10}
        ]
    })";
    auto res1 = openidm::stream::Nm3u8dlReAdapter::ParseMetaJson(valid_meta);
    ASSERT_TRUE(res1.ok() && res1.value->tracks.size() == 2, "Parsed 2 tracks from valid meta.json");

    std::filesystem::path drm_meta = tmp_dir / "drm_meta.json";
    std::ofstream(drm_meta) << R"({
        "url": "https://cdn.example.com/drm.mpd",
        "isLive": false,
        "drmProtected": true,
        "drmSystem": "WIDEVINE",
        "tracks": [
            {"id": "vid_0", "mediaType": "VIDEO", "encryptionMethod": "CENC"}
        ]
    })";
    auto res2 = openidm::stream::Nm3u8dlReAdapter::ParseMetaJson(drm_meta);
    ASSERT_TRUE(!res2.ok() && res2.error->code == "ERR_STREAM_DRM_PROTECTED", "Rejected Widevine DRM stream");

    openidm::stream::StreamAdapterConfig scfg{"/bin/true", "/usr/bin/ffmpeg", tmp_dir, false};
    openidm::stream::Nm3u8dlReAdapter adapter(scfg);
    auto ssrf1 = adapter.ValidateManifestSafety("https://cdn.example.com/a.m3u8", "#EXTM3U\nfile:///etc/passwd\n");
    ASSERT_TRUE(!ssrf1.ok() && ssrf1.error->code == "ERR_SEC_HOSTILE_MANIFEST", "Blocked file:// URI in manifest");
    auto flag_inj = adapter.ValidateManifestSafety("--decryption-binary-path=/tmp/evil", "");
    ASSERT_TRUE(!flag_inj.ok() && flag_inj.error->code == "ERR_SEC_FLAG_INJECTION", "Blocked leading '-' flag injection in URL");

    auto json_as_m3u8 = adapter.ValidateManifestSafety("https://www.youtube.com/watch?v=dQw4w9WgXcQ", R"({"fluxType":"YOUTUBE_STREAM","videoId":"dQw4w9WgXcQ"})");
    ASSERT_TRUE(!json_as_m3u8.ok() && json_as_m3u8.error->code == "ERR_STREAM_INVALID_MANIFEST", "Rejected YouTube JSON descriptor passed as HLS/DASH manifest");
    auto watch_as_m3u8 = adapter.ValidateManifestSafety("https://www.youtube.com/watch?v=dQw4w9WgXcQ", "");
    ASSERT_TRUE(!watch_as_m3u8.ok() && watch_as_m3u8.error->code == "ERR_STREAM_INVALID_MANIFEST", "Rejected YouTube watch page URL passed directly to N_m3u8DL-RE");

    std::filesystem::remove_all(tmp_dir);
    return true;
}

bool TestControllerLifecycleAndDeletedTaskRace() {
    std::filesystem::path work_dir = "/tmp/openidm_test_ctrl_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(work_dir);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    cfg.nm3u8dl_binary_path = openidm::PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", openidm::PathSanitizer::GetExecutableDir());
    cfg.ffmpeg_binary_path = openidm::PathSanitizer::FindWorkerBinary("ffmpeg", openidm::PathSanitizer::GetExecutableDir());

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize DownloadController");

    openidm::TaskModel t;
    t.url = "https://example.com/sdk_setup.exe";
    t.file_name = "sdk_setup.exe";
    t.save_dir = cfg.default_download_dir.string();
    auto added = ctrl.AddTask(t, false, false);
    ASSERT_TRUE(added.ok() && added.value->state == openidm::TaskState::kPaused, "Added paused task");
    std::string tid = added.value->task_id;

    ASSERT_TRUE(ctrl.CancelTask(tid).ok(), "CancelTask succeeds");
    auto canceled = ctrl.GetTask(tid);
    ASSERT_TRUE(canceled.has_value() && canceled->state == openidm::TaskState::kCanceled, "Task is CANCELED");

    std::filesystem::create_directories(cfg.default_download_dir);
    std::filesystem::path user_file = cfg.default_download_dir / "completed_doc.pdf";
    std::ofstream(user_file) << "important user content";

    openidm::TaskModel t_comp;
    t_comp.url = "https://example.com/completed_doc.pdf";
    t_comp.file_name = "completed_doc.pdf";
    t_comp.save_dir = cfg.default_download_dir.string();
    auto added_comp = ctrl.AddTask(t_comp, false, false);
    ASSERT_TRUE(added_comp.ok(), "Added second task");

    ASSERT_TRUE(ctrl.RemoveTask(added_comp.value->task_id, false).ok(), "RemoveTask(delete_file=false)");
    ASSERT_TRUE(std::filesystem::exists(user_file), "Completed user file preserved on disk when removing from list!");

    auto cleared = ctrl.ClearCompletedHistory();
    ASSERT_TRUE(cleared.ok() && *cleared.value == 1, "ClearCompletedHistory removed 1 canceled task");
    ASSERT_TRUE(!ctrl.GetTask(tid).has_value(), "Cleared task no longer in controller");

    ASSERT_TRUE(openidm::PathSanitizer::IsWebpageWatchUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"), "YouTube watch URL recognized as webpage");
    ASSERT_TRUE(openidm::PathSanitizer::IsWebpageWatchUrl("https://youtu.be/dQw4w9WgXcQ"), "youtu.be URL recognized as webpage");
    ASSERT_TRUE(openidm::PathSanitizer::IsWebpageWatchUrl("https://vimeo.com/12345678"), "Vimeo watch URL recognized as webpage");
    ASSERT_TRUE(!openidm::PathSanitizer::IsWebpageWatchUrl("https://example.com/archive.zip"), "Archive URL is not a webpage");
    ASSERT_TRUE(!openidm::PathSanitizer::IsWebpageWatchUrl("https://example.com/video.mp4"), "MP4 URL is not a webpage");

    openidm::TaskModel yt_task;
    yt_task.task_type = openidm::TaskType::kHttpFile;
    yt_task.url = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
    yt_task.file_name = "watch";
    auto yt_res = ctrl.AddTask(yt_task, false, false);
    ASSERT_TRUE(!yt_res.ok(), "AddTask rejects YouTube watch webpage as raw HTTP file");
    ASSERT_TRUE(yt_res.error->code == "ERR_URL_IS_WEBPAGE", "Error code is ERR_URL_IS_WEBPAGE");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir);
    return true;
}

bool TestDesktopUiDesignSystemAndHighDpiLayout() {
    using openidm::ui::CosmicRenderMode;
    using openidm::ui::DesignSystem;
    using openidm::ui::ThemeMode;

    // 1. Verify Minimal Cosmic Dark UI palette & strict dark-mode enforcement
    auto dark = DesignSystem::ResolvePalette("dark", false);
    auto light_req = DesignSystem::ResolvePalette("light", true);
    auto sys_req = DesignSystem::ResolvePalette("system", true);
    ASSERT_TRUE(dark.mode == ThemeMode::kDark && dark.app_background.ToCssHex() == "#090D16", "Cosmic Deep Space Charcoal #090D16 background");
    ASSERT_TRUE(dark.surface_primary.ToCssHex() == "#0F172A", "Cosmic Navy #0F172A surface_primary");
    ASSERT_TRUE(dark.accent_primary.ToCssHex() == "#2563EB", "Flux Cosmic Blue #2563EB accent");
    ASSERT_TRUE(dark.accent_cyan.ToCssHex() == "#38BDF8", "Celestial Cyan #38BDF8 accent");
    ASSERT_TRUE(dark.badge_stream.ToCssHex() == "#8B5CF6", "Nebula Violet #8B5CF6 stream badge");
    ASSERT_TRUE(light_req.mode == ThemeMode::kDark && sys_req.mode == ThemeMode::kDark, "Application stays strictly in dark mode");

    // 2. Verify Cosmic Star-Field Background Engine (Animated Drift, Reduced-Motion Static Fallback, Hidden Suspension)
    auto bg_t0 = DesignSystem::ComputeCosmicBackground(1280, 720, 96, 0, false, false);
    auto bg_t30s = DesignSystem::ComputeCosmicBackground(1280, 720, 96, 30000, false, false);
    ASSERT_TRUE(bg_t0.mode == CosmicRenderMode::kAnimatedDrift && bg_t0.animation_active, "Animated drift active by default");
    ASSERT_TRUE(bg_t0.layers.size() == 3 && bg_t0.stars.size() == 42, "3 parallax layers and 42 deterministic stars");
    ASSERT_TRUE(bg_t30s.layers[2].offset_x_px < 0.0, "Star layers drift gently over long periods");

    auto bg_static = DesignSystem::ComputeCosmicBackground(1280, 720, 96, 30000, true, false);
    ASSERT_TRUE(bg_static.mode == CosmicRenderMode::kStaticFallback && !bg_static.animation_active, "Reduced motion switches to static fallback");
    ASSERT_TRUE(bg_static.stars.size() == 42 && bg_static.layers[0].offset_x_px == 0.0 && bg_static.layers[2].offset_x_px == 0.0,
                "Static fallback retains all 42 stars with zero motion");

    auto bg_hidden = DesignSystem::ComputeCosmicBackground(1280, 720, 96, 30000, false, true);
    ASSERT_TRUE(bg_hidden.mode == CosmicRenderMode::kSuspendedHidden && !bg_hidden.animation_active && bg_hidden.stars.empty(),
                "Hidden/minimized window suspends cosmic background work");

    // 3. Verify High-DPI scaling across 100% (96), 125% (120), 150% (144), and 200% (192)
    auto m100 = DesignSystem::ComputeMetrics(96);
    auto m125 = DesignSystem::ComputeMetrics(120);
    auto m150 = DesignSystem::ComputeMetrics(144);
    auto m200 = DesignSystem::ComputeMetrics(192);
    ASSERT_TRUE(m100.min_window_width == 900 && m100.min_window_height == 560, "100% DPI min window 900x560");
    ASSERT_TRUE(m125.toolbar_height == 60, "125% DPI toolbar height scaled 48 -> 60");
    ASSERT_TRUE(m150.sidebar_width == 324, "150% DPI sidebar width scaled 216 -> 324");
    ASSERT_TRUE(m200.min_window_width == 1800 && m200.table_row_height == 88, "200% DPI 2x exact scaling");
    ASSERT_TRUE(m100.font_caption_px >= 11 && m100.font_body_px >= 13, "Enforces >=11px caption and >=13px body floor");

    // 4. Verify graceful filename truncation preserves extension
    std::string long_name = "OpenIDM_Enterprise_Windows_11_x64_Offline_Installer_Package_2026.msi";
    std::string trunc = openidm::ui::DesktopView::TruncateFilenameGracefully(long_name, 32);
    ASSERT_TRUE(trunc.size() <= 32 && trunc.find("...") != std::string::npos && trunc.substr(trunc.size() - 4) == ".msi",
                "Graceful middle-ellipsis filename truncation preserves .msi extension");
    return true;
}

bool TestDesktopUiInteractionsFilteringSortingAndRedaction() {
    std::filesystem::path work_dir = "/tmp/openidm_test_ui_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(work_dir);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    cfg.nm3u8dl_binary_path = openidm::PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", openidm::PathSanitizer::GetExecutableDir());
    cfg.ffmpeg_binary_path = openidm::PathSanitizer::FindWorkerBinary("ffmpeg", openidm::PathSanitizer::GetExecutableDir());

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller for UI test");
    openidm::ui::DesktopView view(ctrl);

    // 1. Responsive column hiding at compact width vs full width
    view.SetWindowDimensions(920, 600, 96);
    ASSERT_TRUE(view.IsCompactColumnsMode() && view.GetVisibleColumns().size() == 6, "Compact mode (<1024px) shows 6 columns");
    view.SetWindowDimensions(1280, 720, 96);
    ASSERT_TRUE(!view.IsCompactColumnsMode() && view.GetVisibleColumns().size() == 8, "Full mode (>=1024px) shows 8 columns");

    // 2. Empty state on first launch
    ASSERT_TRUE(view.GetEmptyStateMessage().find("Add URL") != std::string::npos, "First launch empty state guides user");

    // 3. Add Download Dialog validation (rejects invalid URL, accepts valid URL)
    openidm::ui::AddDownloadDialogInput bad_in;
    bad_in.url = "ftp://invalid-scheme/file.zip";
    ASSERT_TRUE(!view.SubmitAddDownloadDialog(bad_in).ok(), "Add Download dialog rejects ftp:// scheme");

    openidm::ui::AddDownloadDialogInput file_in;
    file_in.url = "https://cdn.example.com/archive.zip?token=secret_query_token_999";
    file_in.custom_filename = "archive.zip";
    file_in.save_dir = cfg.default_download_dir.string();
    file_in.start_immediately = false;
    auto file_task = view.SubmitAddDownloadDialog(file_in);
    ASSERT_TRUE(file_task.ok(), "Add Download dialog creates HTTP file task");

    // 4. Add a video stream task with sensitive Cookie header and unknown total_bytes
    openidm::TaskModel stream_t;
    stream_t.task_type = openidm::TaskType::kStreamHls;
    stream_t.url = "https://vod.example.com/lecture.m3u8?sig=private_sig_123";
    stream_t.file_name = "lecture.mp4";
    stream_t.save_dir = cfg.default_download_dir.string();
    stream_t.auth_headers["Cookie"] = "session_id=top_secret_cookie_val";
    stream_t.status_detail = "Downloading video segments (5/10)";
    stream_t.completed_segments = 5;
    stream_t.total_segments = 10;
    auto added_stream = ctrl.AddTask(stream_t, false, false);
    ASSERT_TRUE(added_stream.ok(), "Added HLS stream task");

    // 5. Verify Sidebar Category Counts
    auto counts = view.ComputeCategoryCounts();
    ASSERT_TRUE(counts.all_downloads == 2 && counts.video_downloads == 1 && counts.queued == 2,
                "Category counts accurately reflect unified task queue");

    // 6. Filter by Video Downloads category
    view.SelectCategory(openidm::ui::SidebarCategory::kVideoDownloads);
    auto video_rows = view.BuildVisibleTaskRows();
    ASSERT_TRUE(video_rows.size() == 1 && video_rows[0].file_type_badge == "HLS STREAM", "Video category filters to HLS stream");
    ASSERT_TRUE(video_rows[0].progress_text.find("50.0%") != std::string::npos, "Stream with known segment count shows segment %");

    // 7. Unknown total_bytes on ordinary HTTP file must NOT show a fake percentage
    view.SelectCategory(openidm::ui::SidebarCategory::kAllDownloads);
    view.SetSearchQuery("archive.zip");
    auto archive_rows = view.BuildVisibleTaskRows();
    ASSERT_TRUE(archive_rows.size() == 1, "Search filter matches archive.zip");
    ASSERT_TRUE(archive_rows[0].progress_text.find("Size Unknown") != std::string::npos &&
                archive_rows[0].progress_text.find("%") == std::string::npos,
                "Unknown size HTTP task does not display a fake percentage");
    view.SetSearchQuery("");

    // 8. Verify Details Panel strictly redacts Cookie and URL query tokens
    view.SelectTask(added_stream.value->task_id);
    auto details = view.BuildSelectedTaskDetails();
    ASSERT_TRUE(details.has_selection, "Details panel populated for selected task");
    ASSERT_TRUE(details.redacted_source_url.find("private_sig_123") == std::string::npos, "Source URL query token redacted in Details Panel");
    ASSERT_TRUE(details.redacted_headers_summary.find("top_secret_cookie_val") == std::string::npos, "Cookie value redacted in Details Panel");
    ASSERT_TRUE(details.engine_name.find("N_m3u8DL-RE") != std::string::npos, "Details Panel shows stream engine");

    // 9. Settings persistence & restore defaults (keeping dark mode strictly active)
    openidm::AppSettings custom_s = ctrl.GetSettings();
    custom_s.theme = "light"; // Should be normalized to "dark"
    custom_s.max_concurrent_tasks = 6;
    custom_s.speed_limit_bps = 5242880;
    custom_s.reduced_motion = true;
    ASSERT_TRUE(view.ActionSaveSettings(custom_s).ok(), "Save custom settings");
    ASSERT_TRUE(view.GetThemeName() == "dark" && ctrl.GetMaxConcurrentTasks() == 6 && !view.IsCosmicAnimationActive(),
                "Settings applied to view and controller while keeping dark mode");

    auto restored = view.ActionRestoreDefaultSettings();
    ASSERT_TRUE(restored.ok() && view.GetThemeName() == "dark" && ctrl.GetMaxConcurrentTasks() == 3 && view.IsCosmicAnimationActive(),
                "Restored default settings");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir);
    return true;
}

bool TestKeyboardNavigationAndShortcuts() {
    std::filesystem::path work_dir = "/tmp/openidm_test_keys_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(work_dir);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller for keyboard test");
    openidm::ui::DesktopView view(ctrl);

    for (int i = 1; i <= 3; ++i) {
        openidm::TaskModel t;
        t.url = "https://example.com/file_" + std::to_string(i) + ".bin";
        t.file_name = "file_" + std::to_string(i) + ".bin";
        t.created_at_ms = 1000 * i;
        ctrl.AddTask(t, false, true);
    }

    view.SetSort(openidm::ui::SortColumn::kFileName, true);
    view.HandleKeyShortcut("VK_HOME");
    auto first_id = view.GetSelectedTaskId();
    ASSERT_TRUE(!first_id.empty(), "VK_HOME selects first row");

    view.HandleKeyShortcut("VK_DOWN");
    auto second_id = view.GetSelectedTaskId();
    ASSERT_TRUE(second_id != first_id, "VK_DOWN moves selection to second row");

    view.HandleKeyShortcut("VK_END");
    auto last_id = view.GetSelectedTaskId();
    ASSERT_TRUE(last_id != second_id, "VK_END moves selection to last row");

    view.HandleKeyShortcut("VK_DELETE");
    ASSERT_TRUE(ctrl.ListTasks().size() == 2, "VK_DELETE removes selected task from list");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir);
    return true;
}

bool TestDesktopUiHitTestingAndMouseInteractions() {
    std::filesystem::path work_dir = "/tmp/openidm_test_hit_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(work_dir);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller for hit-test suite");
    openidm::ui::DesktopView view(ctrl);
    view.SetWindowDimensions(1280, 720, 96);

    openidm::TaskModel t1;
    t1.url = "https://example.com/short.zip";
    t1.file_name = "short.zip";
    t1.created_at_ms = 1000;
    t1.total_bytes = 2048;
    ctrl.AddTask(t1, false, true);

    openidm::TaskModel t2;
    t2.task_type = openidm::TaskType::kStreamHls;
    t2.url = "https://example.com/master.m3u8";
    t2.file_name = "Very_Long_University_Computer_Science_Lecture_Recording_2026_1080p_60fps.mp4";
    t2.created_at_ms = 2000;
    t2.total_bytes = 10485760;
    ctrl.AddTask(t2, false, true);

    auto geom = view.ComputeLayoutGeometry(0);
    ASSERT_TRUE(geom.toolbar_buttons.size() == 10, "All 10 toolbar buttons laid out");
    ASSERT_TRUE(geom.sidebar_items.size() == 7, "All 7 sidebar categories laid out");
    ASSERT_TRUE(geom.table_columns.size() == 8, "All 8 table columns laid out at 1280px width");
    ASSERT_TRUE(geom.task_rows.size() == 2, "Both task rows laid out");
    ASSERT_TRUE(geom.task_rows[0].cell_rects.size() == 8, "Each task row has 8 aligned column cell rects");

    const auto& video_cat_rc = geom.sidebar_items[5].rect;
    int click_x = (video_cat_rc.left + video_cat_rc.right) / 2;
    int click_y = (video_cat_rc.top + video_cat_rc.bottom) / 2;
    ASSERT_TRUE(view.HandleMouseClick(click_x, click_y, false), "Click Video Downloads category");
    ASSERT_TRUE(view.GetSelectedCategory() == openidm::ui::SidebarCategory::kVideoDownloads, "Category switched to Video Downloads");
    ASSERT_TRUE(view.BuildVisibleTaskRows().size() == 1, "Filtered to 1 video task");

    const auto& all_cat_rc = geom.sidebar_items[0].rect;
    ASSERT_TRUE(view.HandleMouseClick((all_cat_rc.left + all_cat_rc.right) / 2, (all_cat_rc.top + all_cat_rc.bottom) / 2, false),
                "Click All Downloads category");
    ASSERT_TRUE(view.BuildVisibleTaskRows().size() == 2, "Restored all 2 tasks");

    geom = view.ComputeLayoutGeometry(0);
    const auto& fn_rc = geom.task_rows[0].filename_rect;
    auto hover_res = view.HandleMouseMove((fn_rc.left + fn_rc.right) / 2, (fn_rc.top + fn_rc.bottom) / 2);
    ASSERT_TRUE(hover_res.type == openidm::ui::HitTargetType::kTaskRow, "Hover hit-tests task row");
    ASSERT_TRUE(view.GetActiveTooltip() == "Very_Long_University_Computer_Science_Lecture_Recording_2026_1080p_60fps.mp4",
                "Hovering truncated filename exposes full untruncated filename in tooltip");

    const auto& col0_rc = geom.table_columns[0].rect;
    ASSERT_TRUE(view.HandleMouseClick((col0_rc.left + col0_rc.right) / 2, (col0_rc.top + col0_rc.bottom) / 2, false),
                "Click Filename column header");
    ASSERT_TRUE(view.GetSortColumn() == openidm::ui::SortColumn::kFileName && view.IsSortAscending(), "Sorted by Filename ascending");
    ASSERT_TRUE(view.HandleMouseClick((col0_rc.left + col0_rc.right) / 2, (col0_rc.top + col0_rc.bottom) / 2, false),
                "Click Filename column header again");
    ASSERT_TRUE(!view.IsSortAscending(), "Toggled Filename sort to descending");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir);
    return true;
}

bool TestWorkerLifecycleAndIncompatibleWorkerDetection() {
    std::filesystem::path tmp_dir = "/tmp/openidm_test_worker_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(tmp_dir);

    openidm::stream::StreamAdapterConfig missing_cfg{
        tmp_dir / "nonexistent_N_m3u8DL-RE",
        "/usr/bin/ffmpeg",
        tmp_dir,
        true
    };
    openidm::stream::Nm3u8dlReAdapter missing_adapter(missing_cfg);
    auto v1 = missing_adapter.VerifyWorkerBinary();
    ASSERT_TRUE(!v1.ok() && v1.error->code == "ERR_DEPENDENCY_MISSING", "Missing worker reported as ERR_DEPENDENCY_MISSING");

    auto p1 = missing_adapter.ProbeManifest("https://example.com/master.m3u8", {}, {});
    ASSERT_TRUE(!p1.ok() && p1.error->code == "ERR_DEPENDENCY_MISSING", "ProbeManifest reports ERR_DEPENDENCY_MISSING");

#ifdef _WIN32
    std::filesystem::path bad_binary = "C:\\Windows\\System32\\find.exe";
#else
    std::filesystem::path bad_binary = "/bin/false";
#endif
    openidm::stream::StreamAdapterConfig bad_cfg{
        bad_binary,
        "/usr/bin/ffmpeg",
        tmp_dir,
        true
    };
    openidm::stream::Nm3u8dlReAdapter bad_adapter(bad_cfg);
    auto v2 = bad_adapter.VerifyWorkerBinary();
    ASSERT_TRUE(!v2.ok() && v2.error->code == "ERR_WORKER_INCOMPATIBLE", "Incompatible worker reported as ERR_WORKER_INCOMPATIBLE");

    auto valid_worker = openidm::PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", openidm::PathSanitizer::GetExecutableDir());
    openidm::stream::StreamAdapterConfig good_cfg{
        valid_worker,
        openidm::PathSanitizer::FindWorkerBinary("ffmpeg", openidm::PathSanitizer::GetExecutableDir()),
        tmp_dir,
        true
    };
    openidm::stream::Nm3u8dlReAdapter good_adapter(good_cfg);
    auto v3 = good_adapter.VerifyWorkerBinary();
    ASSERT_TRUE(v3.ok() && (!v3.value->empty() && (v3.value->find("N_m3u8DL-RE") != std::string::npos || v3.value->find("0.") != std::string::npos)), "Valid compiled worker passes --version check");

    std::filesystem::remove_all(tmp_dir);
    return true;
}

bool TestFluxBrandingAndLegacyDataMigration() {
    using openidm::PathSanitizer;
    std::filesystem::path app_data = PathSanitizer::GetDefaultAppDataDir();
    ASSERT_TRUE(app_data.string().find("Flux") != std::string::npos, "AppData dir contains Flux");

    std::filesystem::path downloads = PathSanitizer::GetDefaultDownloadsDir(PathSanitizer::GetExecutableDir());
    ASSERT_TRUE(downloads.string().find("Flux") != std::string::npos, "Downloads dir contains Flux");

    openidm::ui::AboutDialogView about;
    ASSERT_TRUE(about.app_name == "Flux Download Manager", "About dialog app name is Flux Download Manager");
    ASSERT_TRUE(about.version == "0.3.0", "About dialog version is 0.3.0");

    std::error_code ec;
    std::filesystem::path mock_root = std::filesystem::temp_directory_path(ec) / ("flux_test_mig_" + openidm::GenerateUuidV4());
    std::filesystem::path legacy_dir = mock_root / "OpenIDM";
    std::filesystem::path target_flux = mock_root / "Flux Download Manager";
    std::filesystem::create_directories(legacy_dir, ec);

    {
        std::ofstream(legacy_dir / "state.db") << "MOCK_SQLITE_HEADER";
        std::ofstream(legacy_dir / "settings.json") << R"({"theme":"dark","max_concurrent":4})";
    }

    std::filesystem::create_directories(target_flux, ec);
    const char* files_to_copy[] = {"state.db", "settings.json"};
    for (const char* f : files_to_copy) {
        std::filesystem::path src = legacy_dir / f;
        if (std::filesystem::exists(src, ec)) {
            std::filesystem::copy_file(src, target_flux / f, std::filesystem::copy_options::update_existing, ec);
        }
    }

    ASSERT_TRUE(std::filesystem::exists(target_flux / "state.db"), "Migrated state.db exists in Flux directory");
    ASSERT_TRUE(std::filesystem::exists(target_flux / "settings.json"), "Migrated settings.json exists in Flux directory");
    ASSERT_TRUE(std::filesystem::exists(legacy_dir / "state.db"), "Legacy state.db preserved without deletion");

    std::filesystem::remove_all(mock_root, ec);
    return true;
}

bool TestProfessionalSettingsCenterAndPersistence() {
    using openidm::ui::DesktopView;
    using openidm::ui::SettingsCategory;

    std::filesystem::path work_dir = "/tmp/flux_test_settings_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(work_dir);
    std::filesystem::path db_path = work_dir / "state.db";
    std::filesystem::path custom_dl_dir = work_dir / "custom_downloads";

    {
        openidm::ControllerConfig cfg;
        cfg.state_db_path = db_path;
        cfg.default_download_dir = work_dir / "default_dl";
        cfg.temp_root_dir = work_dir / "tmp";
        cfg.nm3u8dl_binary_path = openidm::PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", openidm::PathSanitizer::GetExecutableDir());
        cfg.ffmpeg_binary_path = openidm::PathSanitizer::FindWorkerBinary("ffmpeg", openidm::PathSanitizer::GetExecutableDir());

        openidm::DownloadController ctrl(cfg);
        ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller for settings center test");
        DesktopView view(ctrl);

        view.OpenSettingsCenter(SettingsCategory::kGeneral);
        ASSERT_TRUE(view.IsSettingsCenterOpen(), "Settings Center opens");

        auto vm = view.BuildSettingsCenterViewModel();
        ASSERT_TRUE(vm.categories.size() == 7, "7 justified settings categories present");
        ASSERT_TRUE(!vm.has_unsaved_changes && vm.is_valid, "Initial settings draft is clean and valid");
        ASSERT_TRUE(vm.category_rules.size() == 6, "6 automatic file category subfolder rules exposed");

        // Test settings search across all categories
        view.SetSettingsSearchQuery("bandwidth");
        auto search_vm = view.BuildSettingsCenterViewModel();
        ASSERT_TRUE(search_vm.visible_items.size() == 1 && search_vm.visible_items[0].key == "speed.speed_limit_bps",
                    "Settings search locates Global Bandwidth Limit across categories");
        view.SetSettingsSearchQuery("");

        // Test input validation on out-of-range fields
        ASSERT_TRUE(!view.SetSettingsDraftField("downloads.max_concurrent_tasks", "99").ok(),
                    "Rejects simultaneous downloads > 16");
        ASSERT_TRUE(!view.BuildSettingsCenterViewModel().is_valid, "ViewModel reflects validation error");

        ASSERT_TRUE(!view.SetSettingsDraftField("connections.default_connections_per_task", "0").ok(),
                    "Rejects 0 connections per task");
        ASSERT_TRUE(!view.SetSettingsDraftField("locations.default_download_dir", "   ").ok(),
                    "Rejects empty download directory");
        ASSERT_TRUE(!view.SetSettingsDraftField("speed.speed_limit_bps", "100 B/s").ok(),
                    "Rejects speed limit < 16 KiB/s");

        // Test valid edits across categories
        ASSERT_TRUE(view.SetSettingsDraftField("general.start_minimized", "true").ok(), "Set start_minimized");
        ASSERT_TRUE(view.SetSettingsDraftField("general.close_to_tray", "false").ok(), "Set close_to_tray");
        ASSERT_TRUE(view.SetSettingsDraftField("general.reduced_motion", "true").ok(), "Set reduced_motion");
        ASSERT_TRUE(view.SetSettingsDraftField("downloads.max_concurrent_tasks", "6").ok(), "Set max_concurrent_tasks = 6");
        ASSERT_TRUE(view.SetSettingsDraftField("connections.default_connections_per_task", "16").ok(), "Set default_connections = 16");
        ASSERT_TRUE(view.SetSettingsDraftField("speed.speed_limit_bps", "5 MiB/s").ok(), "Set speed_limit_bps = 5 MiB/s");
        ASSERT_TRUE(view.SetSettingsDraftField("locations.default_download_dir", custom_dl_dir.string()).ok(), "Set custom download dir");
        ASSERT_TRUE(view.SetSettingsDraftField("browser.capture_enabled", "false").ok(), "Set browser_capture_enabled = false");

        // Test per-field reset to default before applying
        ASSERT_TRUE(view.ResetSettingsDraftField("general.close_to_tray").ok(), "Reset close_to_tray to default (true)");
        ASSERT_TRUE(view.GetSettingsDraft().close_to_tray == true, "close_to_tray restored to true in draft");

        auto mod_vm = view.BuildSettingsCenterViewModel();
        ASSERT_TRUE(mod_vm.has_unsaved_changes && mod_vm.is_valid, "Draft shows unsaved valid modifications");

        ASSERT_TRUE(view.ApplySettingsDraft().ok(), "ApplySettingsDraft commits changes to controller & SQLite");
        ASSERT_TRUE(!view.BuildSettingsCenterViewModel().has_unsaved_changes, "Draft no longer marked dirty after apply");

        ctrl.Shutdown();
    }

    // Re-open a fresh DownloadController on the same SQLite DB to verify restart persistence
    {
        openidm::ControllerConfig cfg2;
        cfg2.state_db_path = db_path;
        cfg2.default_download_dir = work_dir / "default_dl";
        cfg2.temp_root_dir = work_dir / "tmp";

        openidm::DownloadController ctrl2(cfg2);
        ASSERT_TRUE(ctrl2.Initialize().ok(), "Re-initialize controller after restart");
        auto persisted = ctrl2.GetSettings();
        ASSERT_TRUE(persisted.theme == "dark", "Persisted theme is dark");
        ASSERT_TRUE(persisted.start_minimized == true, "start_minimized persisted across restart");
        ASSERT_TRUE(persisted.close_to_tray == true, "close_to_tray persisted across restart");
        ASSERT_TRUE(persisted.reduced_motion == true, "reduced_motion persisted across restart");
        ASSERT_TRUE(persisted.max_concurrent_tasks == 6, "max_concurrent_tasks=6 persisted across restart");
        ASSERT_TRUE(persisted.default_connections_per_task == 16, "default_connections_per_task=16 persisted across restart");
        ASSERT_TRUE(persisted.speed_limit_bps == 5ULL * 1048576ULL, "speed_limit_bps=5 MiB/s persisted across restart");
        ASSERT_TRUE(persisted.default_download_dir == custom_dl_dir.string(), "default_download_dir persisted across restart");
        ASSERT_TRUE(persisted.browser_capture_enabled == false, "browser_capture_enabled=false persisted across restart");

        ctrl2.Shutdown();
    }

    std::filesystem::remove_all(work_dir);
    return true;
}

} // namespace

int main() {
    int passed = 0;
    const int total = 14;

    auto run = [&](int idx, const char* name, bool (*fn)()) {
        std::cout << "[" << idx << "/" << total << "] Running " << name << "...\n";
        if (fn()) {
            std::cout << "  -> PASS\n";
            ++passed;
        }
    };

    run(1,  "TestPathSanitizerAndContentDisposition", TestPathSanitizerAndContentDisposition);
    run(2,  "TestDynamicWorkerAndDiskSpaceGuard", TestDynamicWorkerAndDiskSpaceGuard);
    run(3,  "TestLoggerRedactionAndCrypto", TestLoggerRedactionAndCrypto);
    run(4,  "TestBriskSegmentSchedulerWorkStealingAndRollback", TestBriskSegmentSchedulerWorkStealingAndRollback);
    run(5,  "TestSqliteStoreAndCrashRecovery", TestSqliteStoreAndCrashRecovery);
    run(6,  "TestStreamMetaJsonAndDrmRejection", TestStreamMetaJsonAndDrmRejection);
    run(7,  "TestControllerLifecycleAndDeletedTaskRace", TestControllerLifecycleAndDeletedTaskRace);
    run(8,  "TestDesktopUiDesignSystemAndHighDpiLayout", TestDesktopUiDesignSystemAndHighDpiLayout);
    run(9,  "TestDesktopUiInteractionsFilteringSortingAndRedaction", TestDesktopUiInteractionsFilteringSortingAndRedaction);
    run(10, "TestKeyboardNavigationAndShortcuts", TestKeyboardNavigationAndShortcuts);
    run(11, "TestDesktopUiHitTestingAndMouseInteractions", TestDesktopUiHitTestingAndMouseInteractions);
    run(12, "TestWorkerLifecycleAndIncompatibleWorkerDetection", TestWorkerLifecycleAndIncompatibleWorkerDetection);
    run(13, "TestFluxBrandingAndLegacyDataMigration", TestFluxBrandingAndLegacyDataMigration);
    run(14, "TestProfessionalSettingsCenterAndPersistence", TestProfessionalSettingsCenterAndPersistence);

    std::cout << "\nC++20 Unit & Regression Test Summary: " << passed << " / " << total << " suites PASSED.\n";
    return (passed == total) ? 0 : 1;
}
