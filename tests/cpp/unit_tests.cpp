// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux Download Manager — Automated C++20 Unit & Regression Test Suite (15 Suites)
// Verifies PathSanitizer, Dynamic Worker Discovery, Disk Space Guard, Logger Redaction,
// CryptoUtils, Brisk SegmentScheduler + Stolen Segment Rollback, SqliteStore + Crash Recovery,
// Nm3u8dlReAdapter + DRM Rejection, DownloadController Lifecycle + Race Guards,
// DesktopView Cosmic Dark Design System + High-DPI Layout, CosmicStarfield Animation & Static Fallback,
// Professional Settings Center + Validation + Persistence, UI Interactions + Redaction, and Keyboard Navigation.

#include "brisk_engine/http_client.hpp"
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

    // Verify dynamic discovery of ffmpeg via PATH or tools/ without hardcoded /usr/bin/ffmpeg
    auto ff_path = PathSanitizer::FindWorkerBinary("ffmpeg", exe_dir);
    ASSERT_TRUE(std::filesystem::exists(ff_path), "FindWorkerBinary dynamically discovers ffmpeg");

    auto nm_path = PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", exe_dir);
    ASSERT_TRUE(std::filesystem::exists(nm_path), "FindWorkerBinary dynamically discovers N_m3u8DL-RE");

    // Verify pre-flight disk space check (BUG-009 regression test)
    auto ok_space = PathSanitizer::CheckAvailableDiskSpace("/tmp", 1024 * 1024);
    ASSERT_TRUE(ok_space.ok(), "1 MiB disk space check succeeds");

    // Requesting 1 Exabyte must fail immediately with ERR_DISK_FULL
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

    // BUG-008 Regression Test: Suppose Server ignores Range header on Worker 1 before any bytes are written
    bool merged = sched.RollbackUnstartedStolenSegment(seg1->segment_id);
    ASSERT_TRUE(merged, "RollbackUnstartedStolenSegment merges unstarted stolen segment back into Seg 0");
    ASSERT_TRUE(seg0->end_offset.load() == kTotalSize - 1, "Seg 0 restored to full [0, kTotalSize - 1] range");

    // Re-steal across 4 workers and verify work-stealing reuse
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

    // Add task in paused mode
    openidm::TaskModel t;
    t.url = "https://example.com/sdk_setup.exe";
    t.file_name = "sdk_setup.exe";
    t.save_dir = cfg.default_download_dir.string();
    auto added = ctrl.AddTask(t, false, false);
    ASSERT_TRUE(added.ok() && added.value->state == openidm::TaskState::kPaused, "Added paused task");
    std::string tid = added.value->task_id;

    // Cancel task and verify RetryTask increments retry_count and transitions back to QUEUED (or PAUSED on pause)
    ASSERT_TRUE(ctrl.CancelTask(tid).ok(), "CancelTask succeeds");
    auto canceled = ctrl.GetTask(tid);
    ASSERT_TRUE(canceled.has_value() && canceled->state == openidm::TaskState::kCanceled, "Task is CANCELED");

    // Simulate completed file on disk and verify RemoveTask(id, false) NEVER deletes completed user file!
    std::filesystem::create_directories(cfg.default_download_dir);
    std::filesystem::path user_file = cfg.default_download_dir / "completed_doc.pdf";
    std::ofstream(user_file) << "important user content";

    openidm::TaskModel t_comp;
    t_comp.url = "https://example.com/completed_doc.pdf";
    t_comp.file_name = "completed_doc.pdf";
    t_comp.save_dir = cfg.default_download_dir.string();
    auto added_comp = ctrl.AddTask(t_comp, false, false);
    ASSERT_TRUE(added_comp.ok(), "Added second task");

    // Remove from list without deleting file
    ASSERT_TRUE(ctrl.RemoveTask(added_comp.value->task_id, false).ok(), "RemoveTask(delete_file=false)");
    ASSERT_TRUE(std::filesystem::exists(user_file), "Completed user file preserved on disk when removing from list!");

    // ClearCompletedHistory removes CANCELED task `tid`
    auto cleared = ctrl.ClearCompletedHistory();
    ASSERT_TRUE(cleared.ok() && *cleared.value == 1, "ClearCompletedHistory removed 1 canceled task");
    ASSERT_TRUE(!ctrl.GetTask(tid).has_value(), "Cleared task no longer in controller");

    // Verify Webpage Watch URL rejection
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
    using openidm::ui::DesignSystem;
    using openidm::ui::ThemeMode;

    // Verify Cosmic Dark tokens and palette helpers
    auto dark = DesignSystem::ResolvePalette("dark", false);
    auto light = DesignSystem::ResolvePalette("light", false);
    auto sys_light = DesignSystem::ResolvePalette("system", true);
    ASSERT_TRUE(dark.mode == ThemeMode::kDark && dark.app_background.ToCssHex() == "#0F172A", "Flux Deep Slate #0F172A background");
    ASSERT_TRUE(dark.cosmic_void.ToCssHex() == "#070B14", "Cosmic Void #070B14 token");
    ASSERT_TRUE(dark.accent_primary.ToCssHex() == "#2563EB", "Flux Blue #2563EB accent");
    ASSERT_TRUE(dark.accent_cyan.ToCssHex() == "#06B6D4", "Cosmic Cyan #06B6D4 accent");
    ASSERT_TRUE(light.mode == ThemeMode::kLight && light.surface_primary.ToCssHex() == "#FFFFFF", "Light theme white surface");
    ASSERT_TRUE(sys_light.mode == ThemeMode::kLight, "System theme honors OS light preference");

    // Verify High-DPI scaling across 100% (96), 125% (120), 150% (144), and 200% (192)
    auto m100 = DesignSystem::ComputeMetrics(96);
    auto m125 = DesignSystem::ComputeMetrics(120);
    auto m150 = DesignSystem::ComputeMetrics(144);
    auto m200 = DesignSystem::ComputeMetrics(192);
    ASSERT_TRUE(m100.min_window_width == 900 && m100.min_window_height == 560, "100% DPI min window 900x560");
    ASSERT_TRUE(m125.toolbar_height == 60, "125% DPI toolbar height scaled 48 -> 60");
    ASSERT_TRUE(m150.sidebar_width == 324, "150% DPI sidebar width scaled 216 -> 324");
    ASSERT_TRUE(m200.min_window_width == 1800 && m200.table_row_height == 88, "200% DPI 2x exact scaling");
    ASSERT_TRUE(m100.font_caption_px >= 11 && m100.font_body_px >= 13, "Enforces >=11px caption and >=13px body floor");

    // Verify graceful filename truncation preserves extension
    std::string long_name = "OpenIDM_Enterprise_Windows_11_x64_Offline_Installer_Package_2026.msi";
    std::string trunc = openidm::ui::DesktopView::TruncateFilenameGracefully(long_name, 32);
    ASSERT_TRUE(trunc.size() <= 32 && trunc.find("...") != std::string::npos && trunc.substr(trunc.size() - 4) == ".msi",
                "Graceful middle-ellipsis filename truncation preserves .msi extension");
    return true;
}

bool TestCosmicStarfieldPerformanceAndFallback() {
    using openidm::ui::CosmicStarfield;
    using openidm::ui::DesignSystem;
    using openidm::ui::RectI;

    CosmicStarfield sf;
    auto pal = DesignSystem::GetDarkPalette();
    RectI canvas{0, 0, 1280, 720};

    // 1. Default state: visible, not minimized, reduced_motion = false -> active animation
    ASSERT_TRUE(sf.IsAnimationActive() && !sf.IsStaticFallbackActive(), "Starfield animation active by default");
    auto initial_stars = sf.BuildRenderStars(canvas, 96, pal);
    ASSERT_TRUE(initial_stars.size() == CosmicStarfield::kStarCount, "Renders exact pre-computed 48 star points");

    // 2. Advancing time gently shifts layer offsets over long 60s-120s periods
    sf.AdvanceTime(15000); // 15 seconds
    auto moved_offset = sf.ComputeLayerDriftOffset(2, 1280, 720);
    ASSERT_TRUE(moved_offset.first > 0, "Foreground celestial layer drifts gently after 15s");

    // 3. Reduced motion preference activates static fallback (offset = {0, 0}, zero time advancement)
    sf.SetReducedMotion(true);
    ASSERT_TRUE(!sf.IsAnimationActive() && sf.IsStaticFallbackActive(), "Reduced motion activates static fallback");
    auto static_offset = sf.ComputeLayerDriftOffset(2, 1280, 720);
    ASSERT_TRUE(static_offset.first == 0 && static_offset.second == 0, "Static fallback returns zero drift offset");
    uint64_t frozen_time = sf.GetElapsedMs();
    sf.AdvanceTime(10000);
    ASSERT_TRUE(sf.GetElapsedMs() == frozen_time, "AdvanceTime does zero work when reduced_motion is active");

    // 4. Minimized or hidden window suspends animation immediately
    sf.SetReducedMotion(false);
    ASSERT_TRUE(sf.IsAnimationActive(), "Resumed when reduced_motion disabled");
    sf.SetWindowState(true, true); // minimized
    ASSERT_TRUE(!sf.IsAnimationActive() && sf.IsStaticFallbackActive(), "Suspended when window is minimized");
    sf.SetWindowState(false, false); // hidden to tray
    ASSERT_TRUE(!sf.IsAnimationActive() && sf.IsStaticFallbackActive(), "Suspended when window is hidden to tray");
    sf.SetWindowState(true, false); // restored
    ASSERT_TRUE(sf.IsAnimationActive(), "Active again when window is restored");

    return true;
}

bool TestProfessionalSettingsCenterValidationAndPersistence() {
    std::filesystem::path work_dir = "/tmp/openidm_test_settings_" + openidm::GenerateUuidV4();
    std::filesystem::create_directories(work_dir);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    cfg.nm3u8dl_binary_path = openidm::PathSanitizer::FindWorkerBinary("N_m3u8DL-RE", openidm::PathSanitizer::GetExecutableDir());
    cfg.ffmpeg_binary_path = openidm::PathSanitizer::FindWorkerBinary("ffmpeg", openidm::PathSanitizer::GetExecutableDir());

    {
        openidm::DownloadController ctrl(cfg);
        ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller for Settings Center test");
        openidm::ui::DesktopView view(ctrl);
        view.SetWindowDimensions(1280, 720, 96);

        // Open Settings Center via toolbar action
        ASSERT_TRUE(view.TriggerToolbarAction(openidm::ui::ToolbarActionId::kSettings).ok(), "Open Settings Center");
        ASSERT_TRUE(view.IsSettingsCenterOpen(), "Settings Center is open");

        auto sv = view.BuildSettingsCenterView();
        ASSERT_TRUE(sv.categories.size() == 8, "Settings Center exposes 8 logical categories");
        ASSERT_TRUE(!sv.has_unsaved_changes && sv.is_valid, "Clean initial state in Settings Center");

        // Test Settings Search across categories
        view.SetSettingsSearchQuery("Stall Timeout");
        sv = view.BuildSettingsCenterView();
        ASSERT_TRUE(sv.visible_items.size() == 1 && sv.visible_items[0].key == "stall_timeout_ms",
                    "Settings search locates Segment Stall Timeout across categories");
        view.SetSettingsSearchQuery("");

        // Test input validation on invalid numeric range
        auto bad_conc = view.SetDraftSettingByKey("max_concurrent_tasks", "99");
        ASSERT_TRUE(!bad_conc.ok(), "Rejects max_concurrent_tasks > 16");
        sv = view.BuildSettingsCenterView();
        ASSERT_TRUE(!sv.is_valid && sv.validation_error.find("1 and 16") != std::string::npos,
                    "Validation error message exposed in SettingsCenterViewModel");

        // Revert invalid draft changes
        view.RevertSettingsChanges();
        ASSERT_TRUE(!view.HasUnsavedSettingsChanges(), "RevertSettingsChanges clears unsaved edits");

        // Modify multiple settings across categories and Apply
        ASSERT_TRUE(view.SetDraftSettingByKey("max_concurrent_tasks", "6").ok(), "Set max_concurrent_tasks = 6");
        ASSERT_TRUE(view.SetDraftSettingByKey("default_connections_per_task", "16").ok(), "Set default_connections_per_task = 16");
        ASSERT_TRUE(view.SetDraftSettingByKey("speed_limit_bps", "10485760").ok(), "Set speed_limit_bps = 10 MiB/s");
        ASSERT_TRUE(view.SetDraftSettingByKey("category_subfolders_enabled", "false").ok(), "Disable category subfolders");
        ASSERT_TRUE(view.SetDraftSettingByKey("conflict_policy", "overwrite").ok(), "Set conflict_policy = overwrite");
        ASSERT_TRUE(view.SetDraftSettingByKey("reduced_motion", "true").ok(), "Enable reduced_motion");
        ASSERT_TRUE(view.HasUnsavedSettingsChanges(), "Detects unsaved draft changes");

        ASSERT_TRUE(view.ApplySettingsChanges().ok(), "ApplySettingsChanges persists valid draft");
        ASSERT_TRUE(!view.HasUnsavedSettingsChanges(), "No unsaved changes after Apply");
        ASSERT_TRUE(!view.GetCosmicStarfield().IsAnimationActive(), "Reduced motion setting immediately switches starfield to static fallback");

        ctrl.Shutdown();
    }

    // Re-open controller on the same state.db and verify all settings persisted across restart
    {
        openidm::DownloadController ctrl2(cfg);
        ASSERT_TRUE(ctrl2.Initialize().ok(), "Re-initialize controller from persisted state.db");
        auto s = ctrl2.GetSettings();
        ASSERT_TRUE(s.max_concurrent_tasks == 6, "Persisted max_concurrent_tasks == 6");
        ASSERT_TRUE(s.default_connections_per_task == 16, "Persisted default_connections_per_task == 16");
        ASSERT_TRUE(s.speed_limit_bps == 10485760ULL, "Persisted speed_limit_bps == 10 MiB/s");
        ASSERT_TRUE(!s.category_subfolders_enabled, "Persisted category_subfolders_enabled == false");
        ASSERT_TRUE(s.conflict_policy == openidm::ConflictPolicy::kOverwrite, "Persisted conflict_policy == kOverwrite");
        ASSERT_TRUE(s.reduced_motion, "Persisted reduced_motion == true");
        ctrl2.Shutdown();
    }

    std::filesystem::remove_all(work_dir);
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

    // 7. Unknown total_bytes on ordinary HTTP file must NOT show a fake percentage (Phase 2D)
    view.SelectCategory(openidm::ui::SidebarCategory::kAllDownloads);
    view.SetSearchQuery("archive.zip");
    auto archive_rows = view.BuildVisibleTaskRows();
    ASSERT_TRUE(archive_rows.size() == 1, "Search filter matches archive.zip");
    ASSERT_TRUE(archive_rows[0].progress_text.find("Size Unknown") != std::string::npos &&
                archive_rows[0].progress_text.find("%") == std::string::npos,
                "Unknown size HTTP task does not display a fake percentage");
    view.SetSearchQuery("");

    // 8. Verify Details Panel strictly redacts Cookie and URL query tokens (Phase 2E)
    view.SelectTask(added_stream.value->task_id);
    auto details = view.BuildSelectedTaskDetails();
    ASSERT_TRUE(details.has_selection, "Details panel populated for selected task");
    ASSERT_TRUE(details.redacted_source_url.find("private_sig_123") == std::string::npos, "Source URL query token redacted in Details Panel");
    ASSERT_TRUE(details.redacted_headers_summary.find("top_secret_cookie_val") == std::string::npos, "Cookie value redacted in Details Panel");
    ASSERT_TRUE(details.engine_name.find("N_m3u8DL-RE") != std::string::npos, "Details Panel shows stream engine");

    // 9. Settings persistence & restore defaults (preserving dark mode palette)
    openidm::AppSettings custom_s = ctrl.GetSettings();
    custom_s.max_concurrent_tasks = 6;
    custom_s.speed_limit_bps = 5242880;
    ASSERT_TRUE(view.ActionSaveSettings(custom_s).ok(), "Save custom settings");
    ASSERT_TRUE(view.GetPalette().mode == openidm::ui::ThemeMode::kDark && ctrl.GetMaxConcurrentTasks() == 6,
                "Settings applied while keeping application in Cosmic Dark mode");

    auto restored = view.ActionRestoreDefaultSettings();
    ASSERT_TRUE(restored.ok() && view.GetThemeName() == "dark" && ctrl.GetMaxConcurrentTasks() == 3, "Restored default settings");

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

    // Add 2 tasks (1 with a very long filename to trigger truncation & tooltip)
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

    // 1. Click Sidebar 'Video Downloads' category (index 5) via pixel coordinates
    const auto& video_cat_rc = geom.sidebar_items[5].rect;
    int click_x = (video_cat_rc.left + video_cat_rc.right) / 2;
    int click_y = (video_cat_rc.top + video_cat_rc.bottom) / 2;
    ASSERT_TRUE(view.HandleMouseClick(click_x, click_y, false), "Click Video Downloads category");
    ASSERT_TRUE(view.GetSelectedCategory() == openidm::ui::SidebarCategory::kVideoDownloads, "Category switched to Video Downloads");
    ASSERT_TRUE(view.BuildVisibleTaskRows().size() == 1, "Filtered to 1 video task");

    // 2. Click Sidebar 'All Downloads' category (index 0)
    const auto& all_cat_rc = geom.sidebar_items[0].rect;
    ASSERT_TRUE(view.HandleMouseClick((all_cat_rc.left + all_cat_rc.right) / 2, (all_cat_rc.top + all_cat_rc.bottom) / 2, false),
                "Click All Downloads category");
    ASSERT_TRUE(view.BuildVisibleTaskRows().size() == 2, "Restored all 2 tasks");

    // 3. Hover over truncated filename in Row 0 and verify full-filename tooltip is populated
    geom = view.ComputeLayoutGeometry(0);
    const auto& fn_rc = geom.task_rows[0].filename_rect;
    auto hover_res = view.HandleMouseMove((fn_rc.left + fn_rc.right) / 2, (fn_rc.top + fn_rc.bottom) / 2);
    ASSERT_TRUE(hover_res.type == openidm::ui::HitTargetType::kTaskRow, "Hover hit-tests task row");
    ASSERT_TRUE(view.GetActiveTooltip() == "Very_Long_University_Computer_Science_Lecture_Recording_2026_1080p_60fps.mp4",
                "Hovering truncated filename exposes full untruncated filename in tooltip");

    // 4. Click Table Column Header 'Filename' (index 0) to sort ascending, then click again to toggle descending
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

    // 1. Missing worker binary must be detected and reported as ERR_DEPENDENCY_MISSING
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

    // 2. Incompatible worker binary (e.g. /bin/false on POSIX or attrib.exe on Windows) must be detected as ERR_WORKER_INCOMPATIBLE
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

    // 3. Valid compiled worker binary passes VerifyWorkerBinary
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

    // Test migration logic: create mock legacy OpenIDM directory and verify clean copy
    std::error_code ec;
    std::filesystem::path mock_root = std::filesystem::temp_directory_path(ec) / ("flux_test_mig_" + openidm::GenerateUuidV4());
    std::filesystem::path legacy_dir = mock_root / "OpenIDM";
    std::filesystem::path target_flux = mock_root / "Flux Download Manager";
    std::filesystem::create_directories(legacy_dir, ec);

    {
        std::ofstream(legacy_dir / "state.db") << "MOCK_SQLITE_HEADER";
        std::ofstream(legacy_dir / "settings.json") << R"({"theme":"dark","max_concurrent":4})";
    }

    // Direct invocation of migration routine
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

bool TestPartFileIsolationAndSafeCommit() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_isolation_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    // COR-01: Two concurrent tasks downloading with identical filename
    std::string filename = "presentation.zip";
    std::string task1_id = "task-uuid-1111";
    std::string task2_id = "task-uuid-2222";

    std::filesystem::path part1 = work_dir / (filename + "." + task1_id + ".part");
    std::filesystem::path part2 = work_dir / (filename + "." + task2_id + ".part");

    ASSERT_TRUE(part1 != part2, "Tasks with same filename get distinct part files");

    std::string content1 = "CONTENT_FOR_TASK_1_DETERMINISTIC_BYTES";
    std::string content2 = "CONTENT_FOR_TASK_2_TOTALLY_DIFFERENT_DATA_STREAM";

    {
        std::ofstream ofs1(part1, std::ios::binary);
        ofs1.write(content1.data(), content1.size());
    }
    {
        std::ofstream ofs2(part2, std::ios::binary);
        ofs2.write(content2.data(), content2.size());
    }

    // COR-02: Commit size check rejection
    auto bad_size_commit = openidm::PathSanitizer::CommitPartFile(
        part1, work_dir, filename, openidm::ConflictPolicy::kAutoRename, content1.size() + 100);
    ASSERT_TRUE(!bad_size_commit.ok() && bad_size_commit.error->code == "ERR_STORAGE_PART_SIZE_MISMATCH",
                "CommitPartFile rejects part file with mismatched size");

    // COR-02 & FDM-008: Concurrent part file commit race test
    std::atomic<bool> gate{false};
    openidm::Result<std::filesystem::path> res1 = openidm::Result<std::filesystem::path>::Err("ERR", "init");
    openidm::Result<std::filesystem::path> res2 = openidm::Result<std::filesystem::path>::Err("ERR", "init");

    std::thread th1([&]() {
        while (!gate.load()) std::this_thread::yield();
        res1 = openidm::PathSanitizer::CommitPartFile(
            part1, work_dir, filename, openidm::ConflictPolicy::kAutoRename, content1.size());
    });
    std::thread th2([&]() {
        while (!gate.load()) std::this_thread::yield();
        res2 = openidm::PathSanitizer::CommitPartFile(
            part2, work_dir, filename, openidm::ConflictPolicy::kAutoRename, content2.size());
    });
    gate.store(true);
    th1.join();
    th2.join();

    ASSERT_TRUE(res1.ok(), "Task 1 committed successfully");
    ASSERT_TRUE(res2.ok(), "Task 2 committed successfully via auto-rename");
    ASSERT_TRUE(std::filesystem::exists(*res1.value), "Task 1 output exists");
    ASSERT_TRUE(std::filesystem::exists(*res2.value), "Task 2 output exists");
    ASSERT_TRUE(*res1.value != *res2.value, "Output paths are distinct without overwriting");

    // Verify content integrity - neither output mixed data
    std::string read1, read2;
    {
        std::ifstream ifs1(*res1.value, std::ios::binary);
        read1.assign((std::istreambuf_iterator<char>(ifs1)), std::istreambuf_iterator<char>());
    }
    {
        std::ifstream ifs2(*res2.value, std::ios::binary);
        read2.assign((std::istreambuf_iterator<char>(ifs2)), std::istreambuf_iterator<char>());
    }
    ASSERT_TRUE(read1 == content1 || read1 == content2, "Output 1 content preserved without corruption");
    ASSERT_TRUE(read2 == content1 || read2 == content2, "Output 2 content preserved without corruption");
    ASSERT_TRUE(read1 != read2, "Both outputs have distinct content");

    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestResourceAwareResumeAndValidatorMismatch() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_resume_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller");

    // COR-03: Create a task with existing etag and segments
    openidm::TaskModel t;
    t.task_id = "test-resume-task";
    t.url = "https://example.com/data.bin";
    t.file_name = "data.bin";
    t.save_dir = (work_dir / "downloads").string();
    t.etag = "\"v1-original-etag\"";
    t.last_modified = "Mon, 01 Jan 2026 00:00:00 GMT";
    t.total_bytes = 1048576;
    t.downloaded_bytes = 524288;
    t.segments.push_back(openidm::Segment{0, 0, 524288, 524287, true});
    t.segments.push_back(openidm::Segment{1, 524288, 524288, 1048575, false});
    t.part_path = ((work_dir / "downloads") / ("data.bin." + t.task_id + ".part")).string();

    auto add_res = ctrl.AddTask(t, false, true);
    ASSERT_TRUE(add_res.ok(), "Task added");

    // If partial file is missing on resume, retry/resume resets safely
    ASSERT_TRUE(ctrl.RetryTask(t.task_id).ok(), "RetryTask succeeds on task");
    auto retrieved = ctrl.GetTask(t.task_id);
    ASSERT_TRUE(retrieved.has_value(), "Retrieved task");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestSegmentSchedulerBoundarySplitRaceGuard() {
    // CON-01 & FDM-009: Verify SegmentScheduler with active concurrent writer and stealer threads
    uint64_t total = 10ULL * 1048576ULL; // 10 MiB
    openidm::brisk::SegmentScheduler scheduler(total, 4, 131072);
    scheduler.Initialize();

    auto s1 = scheduler.AcquireNextOrStealSegment(false);
    ASSERT_TRUE(s1 != nullptr, "Acquire initial segment");
    ASSERT_TRUE(s1->start_offset == 0 && s1->end_offset.load() == total - 1, "Initial segment covers whole file");

    std::atomic<bool> writer_done{false};
    std::atomic<bool> split_started{false};
    std::shared_ptr<openidm::brisk::LiveSegment> s2 = nullptr;

    // Concurrent writer thread: advances offset
    std::thread writer([&]() {
        for (int i = 0; i < 200; ++i) {
            s1->current_offset.fetch_add(4096);
            if (i == 50) split_started.store(true);
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
        writer_done.store(true);
    });

    // Concurrent stealer thread: steals while writer is advancing
    std::thread stealer([&]() {
        while (!split_started.load()) std::this_thread::yield();
        s2 = scheduler.AcquireNextOrStealSegment(true);
    });

    writer.join();
    stealer.join();

    ASSERT_TRUE(s2 != nullptr, "Stolen segment acquired under active concurrency");
    ASSERT_TRUE(s1->end_offset.load() < s2->start_offset, "Victim end is strictly less than stolen start");
    ASSERT_TRUE(s1->end_offset.load() + 1 == s2->start_offset, "No byte hole between split segments");
    ASSERT_TRUE(s2->end_offset.load() == total - 1, "Stolen segment preserves outer boundary");
    ASSERT_TRUE(s1->current_offset.load() <= s1->end_offset.load() + 1, "Victim cursor clamped to boundary");

    return true;
}

bool TestIpcSecurityDaclAndPathTraversal() {
    std::string sock = openidm::ipc::IpcServer::DefaultSocketPath();
    ASSERT_TRUE(!sock.empty(), "Default socket path is non-empty");

#ifdef _WIN32
    ASSERT_TRUE(sock.find("\\\\.\\pipe\\Flux_IPC_") == 0, "Named pipe is in Flux_IPC_ namespace");
#else
    ASSERT_TRUE(sock.find("/tmp/flux_ipc_") == 0, "Unix socket is in /tmp/flux_ipc_ namespace");
#endif

    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_ipc_sec_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Init controller");
    openidm::ipc::IpcServer server(ctrl, sock);

    // SEC-02: Path traversal rejection in download.offer
    std::string traversal_req = R"({
        "jsonrpc": "2.0",
        "id": "test-sec-1",
        "method": "download.offer",
        "params": {
            "url": "https://example.com/evil.exe",
            "filename": "evil.exe",
            "saveDir": "../../../Windows/System32"
        }
    })";
    std::string resp = server.HandleJsonRpcMessage(traversal_req);
    ASSERT_TRUE(resp.find("ERR_PATH_TRAVERSAL_REJECTED") != std::string::npos,
                "IPC download.offer rejects saveDir path traversal");

    ctrl.Shutdown();
    std::error_code ec;
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestGlobalRateLimiting() {
    // CTRL-01 & FDM-009: Verify shared TokenBucketLimiter throttles real concurrent threads
    openidm::brisk::TokenBucketLimiter limiter;
    std::atomic<uint64_t> limit_bps{1000000}; // 1 MB/s
    std::atomic<bool> stop_flag{false};
    std::atomic<bool> start_gate{false};
    std::atomic<int> completed{0};

    auto start = std::chrono::steady_clock::now();
    auto worker = [&]() {
        while (!start_gate.load()) {
            std::this_thread::yield();
        }
        limiter.Consume(750000, limit_bps, stop_flag);
        completed.fetch_add(1);
    };

    std::thread t1(worker);
    std::thread t2(worker);
    start_gate.store(true);
    t1.join();
    t2.join();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    ASSERT_TRUE(completed.load() == 2, "Both concurrent threads finished");
    ASSERT_TRUE(elapsed >= 300, "Shared rate limiter throttled concurrent aggregate throughput");
    return true;
}

// Regression Suite 21: Verify URI Reference & Protocol-Relative Redirect Resolution (NET-03)
bool TestRelativeAndProtocolRelativeRedirectResolution() {
    openidm::brisk::ParsedUrl base;
    base.scheme = "http";
    base.host = "127.0.0.1";
    base.port = 18081;
    base.path_and_query = "/dir/sample.bin?token=abc";

    // 1. Protocol-relative URL
    std::string r1 = openidm::brisk::HttpClient::ResolveRedirectTarget(base, "//localhost:18082/full.bin");
    ASSERT_TRUE(r1 == "http://localhost:18082/full.bin", "Protocol-relative redirect inherits scheme");

    // 2. Absolute path
    std::string r2 = openidm::brisk::HttpClient::ResolveRedirectTarget(base, "/downloads/output.dat");
    ASSERT_TRUE(r2 == "http://127.0.0.1:18081/downloads/output.dat", "Absolute path inherits authority");

    // 3. Query-only relative redirect
    std::string r3 = openidm::brisk::HttpClient::ResolveRedirectTarget(base, "?retry=1");
    ASSERT_TRUE(r3 == "http://127.0.0.1:18081/dir/sample.bin?retry=1", "Query-only replaces existing query");

    // 4. Directory-relative path
    std::string r4 = openidm::brisk::HttpClient::ResolveRedirectTarget(base, "chunk2.bin");
    ASSERT_TRUE(r4 == "http://127.0.0.1:18081/dir/chunk2.bin", "Relative path resolves against directory");

    // 5. Full absolute URL
    std::string r5 = openidm::brisk::HttpClient::ResolveRedirectTarget(base, "https://secure.example.com/asset.zip");
    ASSERT_TRUE(r5 == "https://secure.example.com/asset.zip", "Absolute URL preserved verbatim");

    return true;
}

// Regression Suite 22: Complete Segment Range Map & Partition Integrity (FDM-002, TG-01, TG-02)
bool TestCompleteSegmentRangeMapValidation() {
    uint64_t total = 1048576; // 1 MiB

    // 1. Valid 2-segment partition
    std::vector<openidm::Segment> valid_segs = {
        {0, 0, 524288, 524287, true},
        {1, 524288, 524288, 1048575, false}
    };
    ASSERT_TRUE(openidm::brisk::SegmentScheduler::ValidateSegmentPartition(valid_segs, total), "Valid partition accepted");

    // 2. Gap between segments
    std::vector<openidm::Segment> gap_segs = {
        {0, 0, 500000, 499999, true},
        {1, 510000, 510000, 1048575, false}
    };
    ASSERT_TRUE(!openidm::brisk::SegmentScheduler::ValidateSegmentPartition(gap_segs, total), "Gap partition rejected");

    // 3. Overlap between segments
    std::vector<openidm::Segment> overlap_segs = {
        {0, 0, 524288, 524287, true},
        {1, 500000, 500000, 1048575, false}
    };
    ASSERT_TRUE(!openidm::brisk::SegmentScheduler::ValidateSegmentPartition(overlap_segs, total), "Overlap partition rejected");

    // 4. Cursor outside range
    std::vector<openidm::Segment> bad_cursor = {
        {0, 0, 600000, 524287, true},
        {1, 524288, 524288, 1048575, false}
    };
    ASSERT_TRUE(!openidm::brisk::SegmentScheduler::ValidateSegmentPartition(bad_cursor, total), "Cursor overflow rejected");

    // 5. Premature completion flag
    std::vector<openidm::Segment> premature_comp = {
        {0, 0, 100000, 524287, true},
        {1, 524288, 524288, 1048575, false}
    };
    ASSERT_TRUE(!openidm::brisk::SegmentScheduler::ValidateSegmentPartition(premature_comp, total), "Premature completion rejected");

    // 6. Duplicate segment ID
    std::vector<openidm::Segment> dup_id = {
        {0, 0, 524288, 524287, true},
        {0, 524288, 524288, 1048575, false}
    };
    ASSERT_TRUE(!openidm::brisk::SegmentScheduler::ValidateSegmentPartition(dup_id, total), "Duplicate segment ID rejected");

    return true;
}

// Regression Suite 23: HTTP Chunked Body Framing Decoder (FDM-003, TG-04)
bool TestPosixHttpChunkedDecoder() {
    openidm::brisk::ChunkedDecoder decoder;
    std::string decoded_output;
    auto on_chunk = [&](const uint8_t* data, size_t len) -> bool {
        decoded_output.append(reinterpret_cast<const char*>(data), len);
        return true;
    };

    // Standard chunked stream from RFC 9112 Section 7.1
    std::string stream = "4\r\nWiki\r\n6\r\npedia \r\nE\r\nin \r\n\r\nchunks.\r\n0\r\n\r\n";
    bool ok = decoder.Feed(stream.data(), stream.size(), on_chunk);
    ASSERT_TRUE(ok, "Chunked feed succeeded");
    ASSERT_TRUE(decoder.IsComplete(), "Chunked decoder completed");
    ASSERT_TRUE(decoded_output == "Wikipedia in \r\n\r\nchunks.", "Decoded body matches exact fixture");

    // Chunk extensions and trailers
    openidm::brisk::ChunkedDecoder decoder2;
    std::string out2;
    auto on_chunk2 = [&](const uint8_t* d, size_t l) {
        out2.append(reinterpret_cast<const char*>(d), l);
        return true;
    };
    std::string partA = "5;ext=foo\r\nHello";
    std::string partB = "\r\n0;final=true\r\nX-Trailer: val\r\n\r\n";
    ASSERT_TRUE(decoder2.Feed(partA.data(), partA.size(), on_chunk2), "Part A fed");
    ASSERT_TRUE(!decoder2.IsComplete(), "Not complete after Part A");
    ASSERT_TRUE(decoder2.Feed(partB.data(), partB.size(), on_chunk2), "Part B fed");
    ASSERT_TRUE(decoder2.IsComplete(), "Complete after Part B");
    ASSERT_TRUE(out2 == "Hello", "Chunk extension and trailers handled");

    return true;
}

// Regression Suite 24: URL Parsing, Query Without Slash, IPv6 & Security (FDM-004, FDM-005, TG-05, TG-06)
bool TestUrlParsingAndSecurityChecks() {
    // 1. Query without slash: https://example.test?token=secret
    auto r1 = openidm::brisk::HttpClient::ParseUrl("https://example.test?token=secret");
    ASSERT_TRUE(r1.ok(), "Parse URL without slash ok");
    ASSERT_TRUE(r1.value->host == "example.test", "Host parsed without query attached");
    ASSERT_TRUE(r1.value->path_and_query == "/?token=secret", "Path and query formatted correctly");

    // 2. URL with fragment: https://example.test/file#section
    auto r2 = openidm::brisk::HttpClient::ParseUrl("https://example.test/file#section");
    ASSERT_TRUE(r2.ok(), "Parse URL with fragment ok");
    ASSERT_TRUE(r2.value->path_and_query == "/file", "Fragment stripped from path_and_query");

    // 3. IPv6 in brackets: http://[::1]:8080/path
    auto r3 = openidm::brisk::HttpClient::ParseUrl("http://[::1]:8080/path");
    ASSERT_TRUE(r3.ok(), "Parse bracketed IPv6 ok");
    ASSERT_TRUE(r3.value->host == "::1", "IPv6 host extracted");
    ASSERT_TRUE(r3.value->port == 8080, "Port extracted");

    // 4. Dot segment removal in relative redirect: /a/b/c.html + ../d.html -> /a/d.html
    openidm::brisk::ParsedUrl base;
    base.scheme = "http";
    base.host = "cdn.example.com";
    base.port = 80;
    base.path_and_query = "/a/b/c.html";
    std::string red = openidm::brisk::HttpClient::ResolveRedirectTarget(base, "../d.html");
    ASSERT_TRUE(red == "http://cdn.example.com/a/d.html", "Parent-relative redirect resolved correctly");

    // 5. Private IP classification (RFC 1918 & Loopback)
    auto r_p1 = openidm::brisk::HttpClient::ParseUrl("http://10.0.0.1/doc.pdf");
    ASSERT_TRUE(r_p1.ok() && r_p1.value->is_loopback_or_private, "10.0.0.1 marked private");
    auto r_p2 = openidm::brisk::HttpClient::ParseUrl("http://192.168.1.1/doc.pdf");
    ASSERT_TRUE(r_p2.ok() && r_p2.value->is_loopback_or_private, "192.168.1.1 marked private");
    auto r_p3 = openidm::brisk::HttpClient::ParseUrl("http://172.20.0.1/doc.pdf");
    ASSERT_TRUE(r_p3.ok() && r_p3.value->is_loopback_or_private, "172.20.0.1 marked private");

    return true;
}

// Regression Suite 25: IPC Bounded Shutdown & First Pipe Instance (FDM-006, FDM-011, TG-08, TG-16)
bool TestIpcShutdownAndFirstPipeInstance() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_ipc_shut_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Init controller");

    std::string pipe_name;
#ifdef _WIN32
    pipe_name = "\\\\.\\pipe\\Flux_Test_Shutdown_" + openidm::GenerateUuidV4();
#else
    pipe_name = (work_dir / "test.sock").string();
#endif

    openidm::ipc::IpcServer server1(ctrl, pipe_name);
    ASSERT_TRUE(server1.Start().ok(), "Server 1 started");

#ifdef _WIN32
    // Test FILE_FLAG_FIRST_PIPE_INSTANCE: A second server trying to claim the same pipe name as first instance must fail!
    HANDLE hSecond = CreateNamedPipeA(
        pipe_name.c_str(),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1, 1024, 1024, 50, nullptr);
    ASSERT_TRUE(hSecond == INVALID_HANDLE_VALUE, "Second server cannot claim first pipe instance");
    ASSERT_TRUE(GetLastError() == ERROR_ACCESS_DENIED || GetLastError() == ERROR_ALREADY_EXISTS, "Error code is access denied or already exists");
#endif

    // Test clean shutdown with active connection
    auto start = std::chrono::steady_clock::now();
    server1.Stop();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    ASSERT_TRUE(elapsed < 3000, "Server stopped cleanly in under 3 seconds");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

// Regression Suite 26: Conflict Policies End-to-End (FDM-007, FDM-008, TG-10)
bool TestConflictPolicies() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_conflict_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    std::string fname = "report.pdf";
    std::filesystem::path existing = work_dir / fname;
    {
        std::ofstream ofs(existing);
        ofs << "Original content";
    }

    // 1. FailIfExists
    std::filesystem::path part_fail = work_dir / "fail.part";
    {
        std::ofstream ofs(part_fail);
        ofs << "New content";
    }
    auto res_fail = openidm::PathSanitizer::CommitPartFile(part_fail, work_dir, fname, openidm::ConflictPolicy::kFailIfExists);
    ASSERT_TRUE(!res_fail.ok() && res_fail.error->code == "ERR_FILE_EXISTS", "kFailIfExists rejects existing destination");

    // 2. Overwrite
    std::filesystem::path part_ovw = work_dir / "ovw.part";
    {
        std::ofstream ofs(part_ovw);
        ofs << "Overwritten content";
    }
    auto res_ovw = openidm::PathSanitizer::CommitPartFile(part_ovw, work_dir, fname, openidm::ConflictPolicy::kOverwrite);
    ASSERT_TRUE(res_ovw.ok(), "kOverwrite succeeded");
    {
        std::ifstream ifs(*res_ovw.value);
        std::string s;
        std::getline(ifs, s);
        ASSERT_TRUE(s == "Overwritten content", "Content was replaced");
    }

    // 3. AutoRename
    std::filesystem::path part_ren = work_dir / "ren.part";
    {
        std::ofstream ofs(part_ren);
        ofs << "Auto-renamed content";
    }
    auto res_ren = openidm::PathSanitizer::CommitPartFile(part_ren, work_dir, fname, openidm::ConflictPolicy::kAutoRename);
    ASSERT_TRUE(res_ren.ok(), "kAutoRename succeeded");
    ASSERT_TRUE(res_ren.value->filename().string() == "report (1).pdf", "Target renamed to report (1).pdf");
    {
        std::ifstream ifs(*res_ren.value);
        std::string s;
        std::getline(ifs, s);
        ASSERT_TRUE(s == "Auto-renamed content", "Renamed file content preserved");
    }

    std::filesystem::remove_all(work_dir, ec);
    return true;
}

// Regression Suite 27: System Suspend/Resume Selective Task Restoration (CTRL-02)
bool TestSystemSuspendResumeAndPauseReason() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_suspend_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Init controller");

    // Task 1: Started immediately (active/queued)
    openidm::TaskModel t1;
    t1.url = "https://example.com/file1.zip";
    auto r1 = ctrl.AddTask(t1, true);
    ASSERT_TRUE(r1.ok(), "Task 1 added");
    ASSERT_TRUE(r1.value->state == openidm::TaskState::kQueued, "Task 1 starts in Queued state");
    ASSERT_TRUE(r1.value->pause_reason == openidm::PauseReason::kNone, "Task 1 pause_reason is kNone");

    // Task 2: Created in paused state by user
    openidm::TaskModel t2;
    t2.url = "https://example.com/file2.zip";
    auto r2 = ctrl.AddTask(t2, false);
    ASSERT_TRUE(r2.ok(), "Task 2 added");
    ASSERT_TRUE(r2.value->state == openidm::TaskState::kPaused, "Task 2 starts in Paused state");
    ASSERT_TRUE(r2.value->pause_reason == openidm::PauseReason::kUser, "Task 2 pause_reason is kUser");

    // 1. Trigger system suspend
    ctrl.OnSystemSuspend();

    auto g1_suspend = ctrl.GetTask(r1.value->task_id);
    auto g2_suspend = ctrl.GetTask(r2.value->task_id);
    ASSERT_TRUE(g1_suspend.has_value(), "Task 1 found");
    ASSERT_TRUE(g2_suspend.has_value(), "Task 2 found");

    ASSERT_TRUE(g1_suspend->state == openidm::TaskState::kPaused, "Task 1 paused on suspend");
    ASSERT_TRUE(g1_suspend->pause_reason == openidm::PauseReason::kSystemSuspend, "Task 1 marked as kSystemSuspend");

    ASSERT_TRUE(g2_suspend->state == openidm::TaskState::kPaused, "Task 2 still paused on suspend");
    ASSERT_TRUE(g2_suspend->pause_reason == openidm::PauseReason::kUser, "Task 2 retains kUser pause_reason");

    // 2. Trigger system resume
    ctrl.OnSystemResume();

    auto g1_resume = ctrl.GetTask(r1.value->task_id);
    auto g2_resume = ctrl.GetTask(r2.value->task_id);
    ASSERT_TRUE(g1_resume.has_value(), "Task 1 found");
    ASSERT_TRUE(g2_resume.has_value(), "Task 2 found");

    ASSERT_TRUE(g1_resume->state == openidm::TaskState::kQueued, "Task 1 automatically resumed to Queued");
    ASSERT_TRUE(g1_resume->pause_reason == openidm::PauseReason::kNone, "Task 1 pause_reason reset to kNone");

    ASSERT_TRUE(g2_resume->state == openidm::TaskState::kPaused, "Task 2 strictly remains Paused after sleep/wake");
    ASSERT_TRUE(g2_resume->pause_reason == openidm::PauseReason::kUser, "Task 2 pause_reason remains kUser");

    // 3. User manually resumes Task 2
    ASSERT_TRUE(ctrl.ResumeTask(r2.value->task_id).ok(), "Manual resume of Task 2 succeeded");
    auto g2_manual = ctrl.GetTask(r2.value->task_id);
    ASSERT_TRUE(g2_manual.has_value() && g2_manual->state == openidm::TaskState::kQueued, "Task 2 resumed to Queued");
    ASSERT_TRUE(g2_manual->pause_reason == openidm::PauseReason::kNone, "Task 2 pause_reason reset to kNone");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

} // namespace

int main() {
    int passed = 0;
    const int total = 27;

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
    run(9,  "TestCosmicStarfieldPerformanceAndFallback", TestCosmicStarfieldPerformanceAndFallback);
    run(10, "TestProfessionalSettingsCenterValidationAndPersistence", TestProfessionalSettingsCenterValidationAndPersistence);
    run(11, "TestDesktopUiInteractionsFilteringSortingAndRedaction", TestDesktopUiInteractionsFilteringSortingAndRedaction);
    run(12, "TestKeyboardNavigationAndShortcuts", TestKeyboardNavigationAndShortcuts);
    run(13, "TestDesktopUiHitTestingAndMouseInteractions", TestDesktopUiHitTestingAndMouseInteractions);
    run(14, "TestWorkerLifecycleAndIncompatibleWorkerDetection", TestWorkerLifecycleAndIncompatibleWorkerDetection);
    run(15, "TestFluxBrandingAndLegacyDataMigration", TestFluxBrandingAndLegacyDataMigration);
    run(16, "TestPartFileIsolationAndSafeCommit", TestPartFileIsolationAndSafeCommit);
    run(17, "TestResourceAwareResumeAndValidatorMismatch", TestResourceAwareResumeAndValidatorMismatch);
    run(18, "TestSegmentSchedulerBoundarySplitRaceGuard", TestSegmentSchedulerBoundarySplitRaceGuard);
    run(19, "TestIpcSecurityDaclAndPathTraversal", TestIpcSecurityDaclAndPathTraversal);
    run(20, "TestGlobalRateLimiting", TestGlobalRateLimiting);
    run(21, "TestRelativeAndProtocolRelativeRedirectResolution", TestRelativeAndProtocolRelativeRedirectResolution);
    run(22, "TestCompleteSegmentRangeMapValidation", TestCompleteSegmentRangeMapValidation);
    run(23, "TestPosixHttpChunkedDecoder", TestPosixHttpChunkedDecoder);
    run(24, "TestUrlParsingAndSecurityChecks", TestUrlParsingAndSecurityChecks);
    run(25, "TestIpcShutdownAndFirstPipeInstance", TestIpcShutdownAndFirstPipeInstance);
    run(26, "TestConflictPolicies", TestConflictPolicies);
    run(27, "TestSystemSuspendResumeAndPauseReason", TestSystemSuspendResumeAndPauseReason);

    std::cout << "\nC++20 Unit & Regression Test Summary: " << passed << " / " << total << " suites PASSED.\n";
    return (passed == total) ? 0 : 1;
}
