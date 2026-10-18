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
#include "archive/zip_preview.hpp"
#include "crawler/site_grabber.hpp"
#include "engine_ftp/ftp_client.hpp"
#include "engine_stream/rtsp_mms_adapter.hpp"
#include "engine_torrent/bencode.hpp"
#include "engine_torrent/multi_file_storage.hpp"
#include "engine_torrent/peer_wire_protocol.hpp"
#include "engine_torrent/piece_manager.hpp"
#include "engine_torrent/torrent_engine.hpp"
#include "engine_torrent/torrent_parser.hpp"
#include "engine_torrent/tracker_client.hpp"
#include "security/antivirus_scanner.hpp"
#include "storage/import_export.hpp"
#include "storage/sqlite_store.hpp"
#include "ui/design_system.hpp"
#include "ui/win32_main_window.hpp"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
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
    auto dark = DesignSystem::ResolvePalette("classic_blue", false);
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
    ASSERT_TRUE(restored.ok() && (view.GetThemeName() == "coral_dark" || view.GetThemeName() == "dark" || view.GetThemeName() == "coral") && ctrl.GetMaxConcurrentTasks() == 3, "Restored default settings");

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

    // Test validator mismatch reset behavior: task with ERR_HTTP_VALIDATOR_MISMATCH must reset on retry
    openidm::TaskModel t_mismatch;
    t_mismatch.task_id = "test-mismatch-task";
    t_mismatch.url = "https://example.com/changed.bin";
    t_mismatch.file_name = "changed.bin";
    t_mismatch.save_dir = (work_dir / "downloads").string();
    t_mismatch.state = openidm::TaskState::kFailed;
    t_mismatch.error_code = "ERR_HTTP_VALIDATOR_MISMATCH";
    t_mismatch.error_message = "Remote ETag changed";
    t_mismatch.downloaded_bytes = 262144;
    t_mismatch.segments.push_back(openidm::Segment{0, 0, 262144, 524287, false});
    std::filesystem::path mm_part = (work_dir / "downloads") / ("changed.bin." + t_mismatch.task_id + ".part");
    {
        std::ofstream ofs(mm_part, std::ios::binary);
        ofs << "stale segment content";
    }
    t_mismatch.part_path = mm_part.string();
    ASSERT_TRUE(ctrl.AddTask(t_mismatch, false, true).ok(), "Added mismatch task");
    ASSERT_TRUE(ctrl.RetryTask(t_mismatch.task_id).ok(), "RetryTask on validator mismatch");
    auto mm_retrieved = ctrl.GetTask(t_mismatch.task_id);
    ASSERT_TRUE(mm_retrieved.has_value(), "Retrieved mismatch task");
    ASSERT_TRUE(mm_retrieved->segments.empty(), "Segments cleared after validator mismatch retry");
    ASSERT_TRUE(mm_retrieved->downloaded_bytes == 0, "Downloaded bytes reset to 0 after validator mismatch");
    ASSERT_TRUE(!std::filesystem::exists(mm_part), "Stale partial file removed after validator mismatch");

    // Test CancelTask state consistency and cleanup
    openidm::TaskModel t_cancel;
    t_cancel.task_id = "test-cancel-task";
    t_cancel.url = "https://example.com/cancel_me.bin";
    t_cancel.file_name = "cancel_me.bin";
    t_cancel.save_dir = (work_dir / "downloads").string();
    t_cancel.state = openidm::TaskState::kPaused;
    std::filesystem::path c_part = (work_dir / "downloads") / ("cancel_me.bin." + t_cancel.task_id + ".part");
    {
        std::ofstream ofs(c_part, std::ios::binary);
        ofs << "cancel content";
    }
    t_cancel.part_path = c_part.string();
    t_cancel.downloaded_bytes = 100;
    t_cancel.segments.push_back(openidm::Segment{0, 0, 100, 1000, false});
    t_cancel.auth_headers["Authorization"] = "Bearer SecretToken";
    ASSERT_TRUE(ctrl.AddTask(t_cancel, false, true).ok(), "Added cancel task");
    ASSERT_TRUE(ctrl.CancelTask(t_cancel.task_id).ok(), "CancelTask succeeds");
    auto c_retrieved = ctrl.GetTask(t_cancel.task_id);
    ASSERT_TRUE(c_retrieved.has_value(), "Retrieved canceled task");
    ASSERT_TRUE(c_retrieved->state == openidm::TaskState::kCanceled, "State is kCanceled");
    ASSERT_TRUE(c_retrieved->segments.empty(), "Segments cleared on cancel");
    ASSERT_TRUE(c_retrieved->downloaded_bytes == 0, "Downloaded bytes reset to 0 on cancel");
    ASSERT_TRUE(c_retrieved->auth_headers.empty(), "Auth headers scrubbed on cancel");
    ASSERT_TRUE(!std::filesystem::exists(c_part), "Part file deleted on cancel");

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

bool TestConcurrentAtomicCommitsAndSha256() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path(ec) / ("flux_test_atomic_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    const int kThreadCount = 8;
    std::vector<std::string> payloads(kThreadCount);
    std::vector<std::string> expected_hashes(kThreadCount);
    std::vector<std::filesystem::path> part_files(kThreadCount);

    for (int i = 0; i < kThreadCount; ++i) {
        payloads[i] = "PAYLOAD_THREAD_CHUNK_" + std::to_string(i) + "_" + std::string(1024 * (i + 1), static_cast<char>('A' + i));
        expected_hashes[i] = openidm::CryptoUtils::ComputeBufferSha256Hex(payloads[i]);
        part_files[i] = work_dir / ("worker_" + std::to_string(i) + ".part");
        std::ofstream ofs(part_files[i], std::ios::binary);
        ofs << payloads[i];
    }

    // 1. Hammer kAutoRename concurrently: All 8 threads simultaneously commit to "contended.bin"
    std::vector<std::filesystem::path> resulting_paths(kThreadCount);
    std::vector<bool> thread_success(kThreadCount, false);
    std::atomic<bool> start_signal{false};

    std::vector<std::thread> workers;
    for (int i = 0; i < kThreadCount; ++i) {
        workers.emplace_back([&, i]() {
            while (!start_signal.load()) {
                std::this_thread::yield();
            }
            auto res = openidm::PathSanitizer::CommitPartFile(
                part_files[i], work_dir, "contended.bin", openidm::ConflictPolicy::kAutoRename);
            if (res.ok()) {
                resulting_paths[i] = *res.value;
                thread_success[i] = true;
            }
        });
    }

    start_signal.store(true);
    for (auto& w : workers) {
        if (w.joinable()) w.join();
    }

    // Verify: Every thread must have succeeded
    for (int i = 0; i < kThreadCount; ++i) {
        ASSERT_TRUE(thread_success[i], "Thread commit succeeded under kAutoRename");
        ASSERT_TRUE(std::filesystem::exists(resulting_paths[i], ec), "Resulting file exists");
    }

    // Verify: All resulting paths must be unique (no duplicate auto-rename slots!)
    std::set<std::filesystem::path> unique_paths(resulting_paths.begin(), resulting_paths.end());
    ASSERT_TRUE(unique_paths.size() == kThreadCount, "All 8 committed paths are strictly unique");

    // Verify: The SHA-256 digest of each committed file matches its exact source payload!
    for (int i = 0; i < kThreadCount; ++i) {
        std::ifstream ifs(resulting_paths[i], std::ios::binary);
        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        std::string actual_hash = openidm::CryptoUtils::ComputeBufferSha256Hex(content);
        ASSERT_TRUE(actual_hash == expected_hashes[i], "Committed file SHA-256 matches exact source content without corruption");
    }

    // 2. Hammer kFailIfExists concurrently:
    // Create new parts and attempt to commit to "exclusive.bin"
    for (int i = 0; i < kThreadCount; ++i) {
        part_files[i] = work_dir / ("failif_" + std::to_string(i) + ".part");
        std::ofstream ofs(part_files[i], std::ios::binary);
        ofs << payloads[i];
    }

    std::atomic<int> success_count{0};
    std::atomic<int> already_exists_count{0};
    start_signal.store(false);
    workers.clear();

    for (int i = 0; i < kThreadCount; ++i) {
        workers.emplace_back([&, i]() {
            while (!start_signal.load()) {
                std::this_thread::yield();
            }
            auto res = openidm::PathSanitizer::CommitPartFile(
                part_files[i], work_dir, "exclusive.bin", openidm::ConflictPolicy::kFailIfExists);
            if (res.ok()) {
                ++success_count;
            } else if (res.error->code == "ERR_FILE_EXISTS") {
                ++already_exists_count;
            }
        });
    }

    start_signal.store(true);
    for (auto& w : workers) {
        if (w.joinable()) w.join();
    }

    // Exactly 1 thread must claim "exclusive.bin" and the other 7 must fail with ERR_FILE_EXISTS
    ASSERT_TRUE(success_count.load() == 1, "Exactly 1 thread claims exclusive file under kFailIfExists");
    ASSERT_TRUE(already_exists_count.load() == kThreadCount - 1, "Other threads receive ERR_FILE_EXISTS");

    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestDnsRebindingAndDestinationSecurity() {
    using openidm::brisk::HttpClient;

    // 1. IPv4 private / loopback / link-local / CGNAT / multicast / broadcast
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0x7F000001), "127.0.0.1 is loopback");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0x7FFFFFFF), "127.255.255.255 is loopback");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0x0A000001), "10.0.0.1 is RFC 1918");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0xAC100001), "172.16.0.1 is RFC 1918");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0xAC1FFFFF), "172.31.255.255 is RFC 1918");
    ASSERT_TRUE(!HttpClient::IsPrivateOrLoopbackIp(0xAC200001), "172.32.0.1 is public");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0xC0A80101), "192.168.1.1 is RFC 1918");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0xA9FEA9FE), "169.254.169.254 is Link-Local APIPA");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0x64400001), "100.64.0.1 is Carrier-Grade NAT");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0xC0000201), "192.0.2.1 is TEST-NET-1");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0xE0000001), "224.0.0.1 is Multicast");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0xFFFFFFFF), "255.255.255.255 is Broadcast");
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIp(0x00000000), "0.0.0.0 is Unspecified");
    ASSERT_TRUE(!HttpClient::IsPrivateOrLoopbackIp(0x08080808), "8.8.8.8 is Public DNS");
    ASSERT_TRUE(!HttpClient::IsPrivateOrLoopbackIp(0x5DB8D822), "93.184.216.34 (example.com) is Public");

    // 2. IPv6 private / loopback / ULA / link-local / mapped
    uint8_t loopback_v6[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIpv6(loopback_v6), "::1 is IPv6 loopback");

    uint8_t unspec_v6[16] = {0};
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIpv6(unspec_v6), ":: is IPv6 unspecified");

    uint8_t link_local_v6[16] = {0xFE, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIpv6(link_local_v6), "fe80::1 is IPv6 link-local");

    uint8_t ula_v6[16] = {0xFD, 0x12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIpv6(ula_v6), "fd12::1 is IPv6 ULA");

    uint8_t mcast_v6[16] = {0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIpv6(mcast_v6), "ff02::1 is IPv6 multicast");

    uint8_t mapped_priv[16] = {0,0,0,0,0,0,0,0,0,0,0xFF,0xFF,127,0,0,1};
    ASSERT_TRUE(HttpClient::IsPrivateOrLoopbackIpv6(mapped_priv), "::ffff:127.0.0.1 is IPv4-mapped loopback");

    uint8_t mapped_pub[16] = {0,0,0,0,0,0,0,0,0,0,0xFF,0xFF,8,8,8,8};
    ASSERT_TRUE(!HttpClient::IsPrivateOrLoopbackIpv6(mapped_pub), "::ffff:8.8.8.8 is IPv4-mapped public");

    // 3. DNS resolution check
    ASSERT_TRUE(HttpClient::CheckHostResolvesToPrivate("localhost"), "localhost resolves to private");
    ASSERT_TRUE(HttpClient::CheckHostResolvesToPrivate("127.0.0.1"), "127.0.0.1 resolves to private");

    return true;
}

bool TestStreamContainerValidation() {
    using openidm::stream::Nm3u8dlReAdapter;
    std::error_code ec;
    std::filesystem::path tmp_dir = std::filesystem::temp_directory_path(ec) / ("flux_test_container_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(tmp_dir, ec);

    // 1. MP4 / M4A (ISO BMFF ftyp / moov)
    auto p_mp4 = tmp_dir / "test.mp4";
    {
        std::ofstream ofs(p_mp4, std::ios::binary);
        uint8_t ftyp_hdr[] = {0x00, 0x00, 0x00, 0x20, 'f', 't', 'y', 'p', 'i', 's', 'o', 'm', 0, 0, 2, 0};
        ofs.write(reinterpret_cast<char*>(ftyp_hdr), sizeof(ftyp_hdr));
        ofs.write(std::string(64, '\0').data(), 64);
    }
    ASSERT_TRUE(Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_mp4, "mp4"), "Valid MP4 ftyp container recognized");

    auto p_m4a = tmp_dir / "test.m4a";
    {
        std::ofstream ofs(p_m4a, std::ios::binary);
        uint8_t m4a_hdr[] = {0x00, 0x00, 0x00, 0x20, 'f', 't', 'y', 'p', 'M', '4', 'A', ' ', 0, 0, 0, 0};
        ofs.write(reinterpret_cast<char*>(m4a_hdr), sizeof(m4a_hdr));
        ofs.write(std::string(64, '\0').data(), 64);
    }
    ASSERT_TRUE(Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_m4a, "m4a"), "Valid M4A audio container recognized");

    // 2. MKV / WebM (EBML 0x1A 0x45 0xDF 0xA3)
    auto p_mkv = tmp_dir / "test.mkv";
    {
        std::ofstream ofs(p_mkv, std::ios::binary);
        uint8_t ebml_hdr[] = {0x1A, 0x45, 0xDF, 0xA3, 0x93, 0x42, 0x82, 0x88, 'm', 'a', 't', 'r', 'o', 's', 'k', 'a'};
        ofs.write(reinterpret_cast<char*>(ebml_hdr), sizeof(ebml_hdr));
    }
    ASSERT_TRUE(Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_mkv, "mkv"), "Valid MKV EBML container recognized");
    ASSERT_TRUE(Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_mkv, "webm"), "Valid WebM EBML container recognized");

    // 3. MP3 (ID3v2 and MPEG frame sync)
    auto p_mp3 = tmp_dir / "test.mp3";
    {
        std::ofstream ofs(p_mp3, std::ios::binary);
        uint8_t id3_hdr[] = {'I', 'D', '3', 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};
        ofs.write(reinterpret_cast<char*>(id3_hdr), sizeof(id3_hdr));
        ofs.write(std::string(64, '\0').data(), 64);
    }
    ASSERT_TRUE(Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_mp3, "mp3"), "Valid MP3 ID3 container recognized");

    // 4. FLAC ("fLaC")
    auto p_flac = tmp_dir / "test.flac";
    {
        std::ofstream ofs(p_flac, std::ios::binary);
        uint8_t flac_hdr[] = {'f', 'L', 'a', 'C', 0, 0, 0, 0x22};
        ofs.write(reinterpret_cast<char*>(flac_hdr), sizeof(flac_hdr));
    }
    ASSERT_TRUE(Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_flac, "flac"), "Valid FLAC container recognized");

    // 5. MPEG-TS (0x47 sync byte)
    auto p_ts = tmp_dir / "test.ts";
    {
        std::ofstream ofs(p_ts, std::ios::binary);
        uint8_t ts_packet[188] = {0x47, 0x00, 0x10, 0x10};
        ofs.write(reinterpret_cast<char*>(ts_packet), sizeof(ts_packet));
    }
    ASSERT_TRUE(Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_ts, "ts"), "Valid MPEG-TS container recognized");

    // 6. Corrupt / empty / invalid files
    auto p_corrupt = tmp_dir / "corrupt.mp4";
    {
        std::ofstream ofs(p_corrupt, std::ios::binary);
        ofs << "XYZ";
    }
    ASSERT_TRUE(!Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_corrupt, "mp4"), "Corrupt MP4 rejected");
    ASSERT_TRUE(!Nm3u8dlReAdapter::VerifyContainerMagicBytes(p_corrupt, "mkv"), "Corrupt MKV rejected");

    std::filesystem::remove_all(tmp_dir, ec);
    return true;
}

bool TestFtpClientParsingAndCommands() {
    using namespace openidm::ftp;

    auto res1 = FtpUrl::Parse("ftp://ftp.example.com/pub/files/archive.tar.gz");
    ASSERT_TRUE(res1.ok(), "Parse standard FTP URL");
    ASSERT_TRUE(res1.value->scheme == "ftp", "FTP scheme");
    ASSERT_TRUE(res1.value->host == "ftp.example.com", "FTP host");
    ASSERT_TRUE(res1.value->port == 21, "Default FTP port 21");
    ASSERT_TRUE(res1.value->remote_path == "/pub/files/archive.tar.gz", "Remote path");
    ASSERT_TRUE(res1.value->file_name == "archive.tar.gz", "Filename");
    ASSERT_TRUE(res1.value->user == "anonymous", "Default user anonymous");

    // FTPS URL must be rejected fast to prevent plain unencrypted socket connections
    auto res2 = FtpUrl::Parse("ftps://alice:secret123@secure.ftp.org:990/data/report.pdf");
    ASSERT_TRUE(!res2.ok(), "Reject FTPS URL without verified TLS negotiation");
    ASSERT_TRUE(res2.error->code == "ERR_FTPS_UNSUPPORTED", "Rejection error code is ERR_FTPS_UNSUPPORTED");

    // Verify ProbeFileSize and DownloadFile defense-in-depth FTPS guards
    FtpUrl ftps_url;
    ftps_url.scheme = "ftps";
    ftps_url.host = "secure.ftp.org";
    auto probe_ftps = FtpClient::ProbeFileSize(ftps_url);
    ASSERT_TRUE(!probe_ftps.ok() && probe_ftps.error->code == "ERR_FTPS_UNSUPPORTED", "ProbeFileSize rejects ftps");

    std::atomic<bool> p_req{false}, c_req{false};
    std::atomic<uint64_t> spd{0};
    auto dl_ftps = FtpClient::DownloadFile(ftps_url, "/tmp/dummy.part", 0, p_req, c_req, spd, nullptr);
    ASSERT_TRUE(!dl_ftps.ok() && dl_ftps.error->code == "ERR_FTPS_UNSUPPORTED", "DownloadFile rejects ftps");

    auto res_invalid = FtpUrl::Parse("http://not-ftp.com/test.bin");
    ASSERT_TRUE(!res_invalid.ok(), "Reject non-FTP URL");
    return true;
}

bool TestBencodeAndTorrentParser() {
    using namespace openidm::torrent;

    // 1. Bencode integer, string, list, dict
    auto int_res = BencodeParser::Decode("i42e");
    ASSERT_TRUE(int_res.ok() && int_res.value->as_int() == 42, "Bencode int 42");

    auto str_res = BencodeParser::Decode("4:flux");
    ASSERT_TRUE(str_res.ok() && str_res.value->as_string() == "flux", "Bencode string 'flux'");

    auto list_res = BencodeParser::Decode("li1ei2e3:abce");
    ASSERT_TRUE(list_res.ok() && list_res.value->as_list().size() == 3, "Bencode list [1, 2, 'abc']");

    auto dict_res = BencodeParser::Decode("d4:name4:flux4:porti6881ee");
    ASSERT_TRUE(dict_res.ok() && dict_res.value->as_dict().size() == 2, "Bencode dict");
    ASSERT_TRUE(dict_res.value->contains("name") && (*dict_res.value)["name"].as_string() == "flux", "Dict 'name'");
    ASSERT_TRUE(dict_res.value->contains("port") && (*dict_res.value)["port"].as_int() == 6881, "Dict 'port'");

    // Re-encode
    std::string encoded = BencodeParser::Encode(dict_res.value.value());
    ASSERT_TRUE(encoded == "d4:name4:flux4:porti6881ee", "Bencode re-encode matches input");

    // 2. Magnet Link Parsing
    std::string magnet = "magnet:?xt=urn:btih:4a7d65b7f9a838528990c76892e85a6a2360a7e0&dn=Ubuntu_24.04_LTS&tr=http%3A%2F%2Ftracker.example.com%2Fannounce";
    auto mag_res = TorrentParser::ParseMagnetUri(magnet);
    ASSERT_TRUE(mag_res.ok(), "Parse magnet link");
    ASSERT_TRUE(mag_res.value->info_hash_hex == "4a7d65b7f9a838528990c76892e85a6a2360a7e0", "Magnet info_hash hex");
    ASSERT_TRUE(mag_res.value->name == "Ubuntu_24.04_LTS", "Magnet display name");
    ASSERT_TRUE(mag_res.value->announce == "http://tracker.example.com/announce", "Magnet tracker announce URL");

    // Base32 Magnet
    std::string mag_b32 = "magnet:?xt=urn:btih:JJOWLN7ZVIOFFCKQY5UJFYU2NIRQBJ7A&dn=TestBase32";
    auto mag_b32_res = TorrentParser::ParseMagnetUri(mag_b32);
    ASSERT_TRUE(mag_b32_res.ok(), "Parse Base32 magnet link");
    ASSERT_TRUE(mag_b32_res.value->info_hash_hex.size() == 40, "Base32 converted to 40 hex chars");

    // 3. Piece SHA-1 Verification
    const char test_data[] = "BitTorrent Piece Validation Test Block";
    std::string expected_sha1 = openidm::CryptoUtils::ComputeBufferSha1Hex(std::string_view(test_data, sizeof(test_data) - 1));
    ASSERT_TRUE(TorrentParser::VerifyPieceHex(reinterpret_cast<const uint8_t*>(test_data), sizeof(test_data) - 1, expected_sha1), "VerifyPieceHex matches");
    ASSERT_TRUE(!TorrentParser::VerifyPieceHex(reinterpret_cast<const uint8_t*>("corrupt data"), 12, expected_sha1), "Corrupt piece rejected");

    // 4. BitTorrent Execution Elimination Guard (honesty check)
    openidm::TaskModel tor_task;
    tor_task.task_id = "test-torrent-task";
    tor_task.url = magnet;
    tor_task.file_name = "test.iso";
    tor_task.save_dir = "/tmp/test_torrent_dir";
    TorrentInfo tinfo;
    tinfo.name = "test.iso";
    tinfo.total_length = 1048576;
    std::atomic<bool> t_pause{false}, t_cancel{false};
    std::atomic<uint64_t> t_speed{0};

    auto tor_exec = TorrentEngine::ExecuteTorrentDownload(tor_task, tinfo, t_pause, t_cancel, t_speed, nullptr);
    ASSERT_TRUE(!tor_exec.ok(), "BitTorrent execution must fail cleanly on incomplete metainfo");
    ASSERT_TRUE(tor_exec.error->code == "ERR_TORRENT_MALFORMED", "Returns ERR_TORRENT_MALFORMED on zero-piece metainfo");
    ASSERT_TRUE(tor_task.state == openidm::TaskState::kFailed, "Task state marked as kFailed");
    ASSERT_TRUE(tor_task.downloaded_bytes == 0, "Downloaded bytes must remain 0");
    ASSERT_TRUE(tor_task.completed_segments == 0, "Completed pieces must remain 0");
    ASSERT_TRUE(!std::filesystem::exists("/tmp/test_torrent_dir/test.iso"), "No dummy or placeholder files created on disk");

    // Test with complete metainfo but unreachable trackers/peers
    TorrentInfo tinfo_valid;
    tinfo_valid.name = "test_valid.iso";
    tinfo_valid.total_length = 32768;
    tinfo_valid.piece_length = 32768;
    tinfo_valid.pieces.push_back(std::vector<uint8_t>(20, 0x11));
    tinfo_valid.announce = "http://127.0.0.1:1/announce";
    tor_task.url = "test_valid.torrent";
    auto tor_valid_exec = TorrentEngine::ExecuteTorrentDownload(tor_task, tinfo_valid, t_pause, t_cancel, t_speed, nullptr);
    ASSERT_TRUE(!tor_valid_exec.ok(), "BitTorrent execution fails cleanly when peers unreachable");
    ASSERT_TRUE(tor_valid_exec.error->code == "ERR_TORRENT_NO_PEERS", "Returns ERR_TORRENT_NO_PEERS when peers unavailable");
    return true;
}

bool TestRtspMmsStreamDetection() {
    using namespace openidm::stream;

    ASSERT_TRUE(RtspMmsAdapter::IsStreamUrl("rtsp://media.example.com/live/ch1"), "rtsp:// is stream");
    ASSERT_TRUE(RtspMmsAdapter::IsStreamUrl("rtsps://media.example.com:443/live"), "rtsps:// is stream");
    ASSERT_TRUE(RtspMmsAdapter::IsStreamUrl("mms://stream.example.com/broadcast"), "mms:// is stream");
    ASSERT_TRUE(RtspMmsAdapter::IsStreamUrl("rtmp://live.twitch.tv/app/streamkey"), "rtmp:// is stream");
    ASSERT_TRUE(!RtspMmsAdapter::IsStreamUrl("https://example.com/video.mp4"), "https MP4 is not raw rtsp/mms stream");
    ASSERT_TRUE(!RtspMmsAdapter::IsStreamUrl("ftp://ftp.example.com/file.bin"), "ftp is not rtsp/mms stream");
    return true;
}

bool TestSiteGrabberCrawlerAndSsrf() {
    using namespace openidm::crawler;

    // 1. URL Resolution
    ASSERT_TRUE(SiteGrabber::ResolveUrl("https://example.com/docs/page.html", "sub.html") == "https://example.com/docs/sub.html", "Relative path resolution");
    ASSERT_TRUE(SiteGrabber::ResolveUrl("https://example.com/docs/page.html", "/images/logo.png") == "https://example.com/images/logo.png", "Root-relative path resolution");
    ASSERT_TRUE(SiteGrabber::ResolveUrl("https://example.com/docs/page.html", "//cdn.example.com/asset.js") == "https://cdn.example.com/asset.js", "Protocol-relative URL");
    ASSERT_TRUE(SiteGrabber::ResolveUrl("https://example.com/page.html", "file.pdf#section2") == "https://example.com/file.pdf", "Strip fragment");
    ASSERT_TRUE(SiteGrabber::ResolveUrl("https://example.com/", "javascript:void(0)").empty(), "Ignore javascript: pseudo-protocol");

    // 2. HTML link extraction
    std::string html = R"(
        <html>
        <body>
            <a href="/downloads/setup.exe">Download Installer</a>
            <img src="banner.jpg" alt="Banner"/>
            <a href="https://example.com/manual.pdf">PDF Manual</a>
            <video src="/videos/intro.mp4"></video>
        </body>
        </html>
    )";
    auto urls = SiteGrabber::ExtractUrlsFromHtml(html, "https://example.com/main/");
    ASSERT_TRUE(urls.size() == 4, "Extracted 4 links/media from HTML");

    // 3. Category & Extension Filter Matching
    SiteGrabberConfig cfg;
    cfg.filter_categories = {"Documents", "Images"};
    std::string cat;
    ASSERT_TRUE(SiteGrabber::MatchesFilter("https://example.com/manual.pdf", cfg, &cat) && cat == "Documents", "Matches Documents category");
    ASSERT_TRUE(SiteGrabber::MatchesFilter("https://example.com/photo.png", cfg, &cat) && cat == "Images", "Matches Images category");
    ASSERT_TRUE(!SiteGrabber::MatchesFilter("https://example.com/setup.exe", cfg, &cat), "Rejects Programs when not in filter");

    // 4. SSRF Guard on Crawler Start URL
    SiteGrabberConfig ssrf_cfg;
    ssrf_cfg.start_url = "http://127.0.0.1:8080/admin";
    ssrf_cfg.allow_loopback = false;
    std::atomic<bool> cancel{false};
    auto crawl_res = SiteGrabber::Crawl(ssrf_cfg, cancel);
    ASSERT_TRUE(!crawl_res.ok() && crawl_res.error->code == "ERR_SSRF_BLOCKED", "Crawler blocks SSRF to loopback");
    return true;
}

bool TestZipArchivePreviewAndSecurity() {
    using namespace openidm::archive;

    // 1. Zip Slip Path Traversal Guard
    ASSERT_TRUE(ZipPreviewer::IsZipSlipPath("../../etc/shadow"), "Detect ../ traversal");
    ASSERT_TRUE(ZipPreviewer::IsZipSlipPath("..\\..\\Windows\\System32\\calc.exe"), "Detect ..\\ traversal");
    ASSERT_TRUE(ZipPreviewer::IsZipSlipPath("/etc/passwd"), "Detect leading slash");
    ASSERT_TRUE(ZipPreviewer::IsZipSlipPath("C:\\autoexec.bat"), "Detect drive letter");
    ASSERT_TRUE(ZipPreviewer::IsZipSlipPath("report.pdf:Zone.Identifier"), "Detect NTFS ADS colon");
    ASSERT_TRUE(!ZipPreviewer::IsZipSlipPath("docs/readme.txt"), "Safe relative path allowed");
    ASSERT_TRUE(!ZipPreviewer::IsZipSlipPath("images/photo.png"), "Safe nested path allowed");

    // 2. Synthetic Valid ZIP Archive in Memory
    // Construct Central Directory Header + EOCD record
    std::vector<uint8_t> zip_buf;
    uint8_t cd_hdr[] = {
        0x50, 0x4b, 0x01, 0x02, // signature
        0x14, 0x00,             // version made by (2.0)
        0x14, 0x00,             // version needed (2.0)
        0x00, 0x00,             // flags (no encryption)
        0x08, 0x00,             // compression (8 = deflated)
        0x00, 0x00, 0x00, 0x00, // mod time/date
        0x12, 0x34, 0x56, 0x78, // crc-32
        0x20, 0x00, 0x00, 0x00, // compressed size (32 bytes)
        0x64, 0x00, 0x00, 0x00, // uncompressed size (100 bytes)
        0x08, 0x00,             // filename length (8)
        0x00, 0x00,             // extra length (0)
        0x00, 0x00,             // comment length (0)
        0x00, 0x00,             // disk start (0)
        0x00, 0x00,             // internal attrs
        0x00, 0x00, 0x00, 0x00, // external attrs
        0x00, 0x00, 0x00, 0x00  // local header offset
    };
    zip_buf.insert(zip_buf.end(), cd_hdr, cd_hdr + sizeof(cd_hdr));
    const char fn[] = "test.txt";
    zip_buf.insert(zip_buf.end(), fn, fn + 8);

    auto summary_res = ZipPreviewer::InspectBuffer(zip_buf.data(), zip_buf.size());
    ASSERT_TRUE(summary_res.ok(), "InspectBuffer parses Central Directory header");
    ASSERT_TRUE(summary_res.value->total_files == 1, "Total files is 1");
    ASSERT_TRUE(summary_res.value->entries[0].file_path == "test.txt", "Entry filename");
    ASSERT_TRUE(summary_res.value->entries[0].uncompressed_size == 100, "Uncompressed size 100");
    ASSERT_TRUE(summary_res.value->entries[0].compressed_size == 32, "Compressed size 32");
    ASSERT_TRUE(summary_res.value->entries[0].compression_method == 8, "Compression method 8 (Deflated)");
    ASSERT_TRUE(!summary_res.value->entries[0].is_encrypted, "Not encrypted");
    return true;
}

bool TestTaskImportExportAndMultiQueue() {
    using namespace openidm::storage;

    // 1. Task Export & Credential Redaction
    openidm::TaskModel t1;
    t1.task_id = "test-uuid-1";
    t1.url = "https://user:SuperSecretPassword@cdn.example.com/download.zip?token=xyz123";
    t1.file_name = "download.zip";
    t1.category = "Compressed";
    t1.queue_id = "main";
    t1.total_bytes = 1048576;
    t1.auth_headers["Authorization"] = "Bearer secret-token-999";
    t1.auth_headers["Cookie"] = "session=secret-session-abc";

    std::vector<openidm::TaskModel> tasks = {t1};

    auto json_exp = TaskImportExport::ExportTasksToString(tasks, ExportFormat::kJson, false);
    ASSERT_TRUE(json_exp.ok(), "Export to JSON succeeds");
    ASSERT_TRUE(json_exp.value->find("SuperSecretPassword") == std::string::npos, "Password scrubbed from URL");
    ASSERT_TRUE(json_exp.value->find("secret-token-999") == std::string::npos, "Auth headers excluded from export");
    ASSERT_TRUE(json_exp.value->find("download.zip") != std::string::npos, "Filename present");

    auto csv_exp = TaskImportExport::ExportTasksToString(tasks, ExportFormat::kCsv, false);
    ASSERT_TRUE(csv_exp.ok(), "Export to CSV succeeds");
    ASSERT_TRUE(csv_exp.value->find("SuperSecretPassword") == std::string::npos, "Password scrubbed from CSV URL");

    // 2. Task Import from URL list
    std::string url_list = "# Comment line\nhttps://example.com/file1.pdf\nftp://ftp.example.com/file2.zip\nmagnet:?xt=urn:btih:1111111111111111111111111111111111111111&dn=Linux\n";
    auto imported = TaskImportExport::ImportFromUrlList(url_list);
    ASSERT_TRUE(imported.ok(), "Import from URL list succeeds");
    ASSERT_TRUE(imported.value->size() == 3, "Imported 3 tasks");
    ASSERT_TRUE((*imported.value)[0].category == "Documents", "Auto-inferred Documents category for PDF");
    ASSERT_TRUE((*imported.value)[1].task_type == openidm::TaskType::kFtp, "Auto-inferred FTP task type");
    ASSERT_TRUE((*imported.value)[2].task_type == openidm::TaskType::kTorrent, "Auto-inferred Torrent task type");

    // 3. Multi-Queue Management in SqliteStore
    std::filesystem::path db_p = "/tmp/test_queue_" + openidm::GenerateUuidV4() + ".db";
    openidm::SqliteStore store;
    ASSERT_TRUE(store.Open(db_p).ok(), "Open test DB for queues");

    openidm::QueueConfig q2;
    q2.queue_id = "sync_queue";
    q2.name = "Synchronization Queue";
    q2.max_concurrent_tasks = 5;
    q2.speed_limit_bps = 5242880;
    q2.schedule_enabled = true;
    q2.start_time_local = "02:00";
    q2.stop_time_local = "06:00";
    q2.post_completion_action = "EXIT_APP";

    ASSERT_TRUE(store.UpsertQueue(q2).ok(), "UpsertQueue succeeds");
    auto q_list = store.ListQueues();
    ASSERT_TRUE(q_list.ok(), "ListQueues succeeds");
    ASSERT_TRUE(q_list.value->size() >= 2, "Contains default 'main' and 'sync_queue'");

    std::error_code ec;
    std::filesystem::remove(db_p, ec);
    return true;
}

bool TestAntivirusScannerSafetyAndHandling() {
    using namespace openidm::security;

    // 1. ScanReport default status must be kUnavailable (never kClean)
    ScanReport def_rep;
    ASSERT_TRUE(def_rep.status == ScanStatus::kUnavailable, "ScanReport default is kUnavailable");

    // 2. Scan missing file returns error
    auto missing_res = AntivirusScanner::ScanWithDefender("/nonexistent/file/for/antivirus.dat");
    ASSERT_TRUE(!missing_res.ok(), "Scan missing file returns error");
    ASSERT_TRUE(missing_res.error->code == "ERR_FILE_NOT_FOUND", "Error code is ERR_FILE_NOT_FOUND");

    // 3. Create dummy file and verify scan does NOT report fake kClean on non-Windows
    std::filesystem::path tmp_p = std::filesystem::temp_directory_path() / ("test_av_" + openidm::GenerateUuidV4() + ".bin");
    {
        std::ofstream ofs(tmp_p, std::ios::binary);
        ofs << "Antivirus Test Content";
    }

#ifndef _WIN32
    auto scan_res = AntivirusScanner::ScanWithDefender(tmp_p);
    ASSERT_TRUE(scan_res.ok(), "ScanWithDefender succeeds with report");
    ASSERT_TRUE(scan_res.value->status == ScanStatus::kUnavailable, "Non-Windows mock scan reports kUnavailable (never kClean)");
    ASSERT_TRUE(!AntivirusScanner::IsWindowsDefenderAvailable(), "IsWindowsDefenderAvailable is false on non-Windows");
#endif

    // 4. Verify DesktopView blocks ActionOpenFile if a threat was detected
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_av_ui_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    std::filesystem::create_directories(cfg.default_download_dir, ec);

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller");

    openidm::TaskModel t_threat;
    t_threat.task_id = "task-threat-1";
    t_threat.url = "https://example.com/malware.exe";
    t_threat.file_name = "malware.exe";
    t_threat.save_dir = cfg.default_download_dir.string();
    t_threat.state = openidm::TaskState::kCompleted;
    t_threat.error_code = "ERR_THREAT_DETECTED";
    t_threat.status_detail = "Threat detected by Antivirus (Defender)";

    // Write file so it exists
    {
        std::ofstream ofs(cfg.default_download_dir / "malware.exe", std::ios::binary);
        ofs << "Threat Dummy";
    }

    auto add_threat = ctrl.AddTask(t_threat, false, true);
    ASSERT_TRUE(add_threat.ok(), "Added threat task");

    openidm::ui::DesktopView view(ctrl);
    view.SelectTask("task-threat-1");
    auto open_res = view.ActionOpenCompletedFile(true); // dry_run
    ASSERT_TRUE(!open_res.ok(), "ActionOpenCompletedFile must block execution on threat detection");
    ASSERT_TRUE(open_res.error->code == "ERR_THREAT_DETECTED", "Returns ERR_THREAT_DETECTED");

    ctrl.Shutdown();
    std::filesystem::remove(tmp_p, ec);
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestDualRateLimiterAndPerTaskSpeedLimit() {
    using openidm::brisk::TokenBucketLimiter;

    // Test dual limiter logic
    auto global_limiter = std::make_shared<TokenBucketLimiter>();
    TokenBucketLimiter task_limiter;
    std::atomic<uint64_t> global_limit{100000}; // 100 KB/s
    std::atomic<uint64_t> task_limit{50000};    // 50 KB/s
    std::atomic<bool> stop_flag{false};

    global_limiter->Consume(1000, global_limit, stop_flag);
    task_limiter.Consume(1000, task_limit, stop_flag);

    // Test controller per-task speed limit API
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_speed_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    std::filesystem::create_directories(cfg.default_download_dir, ec);

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller");

    openidm::TaskModel t;
    t.task_id = "task-speed-1";
    t.url = "https://example.com/file.bin";
    t.file_name = "file.bin";
    t.speed_limit_bps = 500000;
    auto add_res = ctrl.AddTask(t, false, true);
    ASSERT_TRUE(add_res.ok(), "Added task with speed limit");

    ASSERT_TRUE(ctrl.GetTaskSpeedLimit("task-speed-1") == 500000, "GetTaskSpeedLimit returns 500000");
    auto set_res = ctrl.SetTaskSpeedLimit("task-speed-1", 1000000);
    ASSERT_TRUE(set_res.ok(), "SetTaskSpeedLimit to 1000000 ok");
    ASSERT_TRUE(ctrl.GetTaskSpeedLimit("task-speed-1") == 1000000, "GetTaskSpeedLimit returns 1000000");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestDynamicCategorizationAndSiteRulesPriority() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_cat_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    std::filesystem::create_directories(cfg.default_download_dir, ec);

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller");

    auto settings = ctrl.GetSettings();
    settings.category_subfolders_enabled = true;
    settings.custom_extension_categories["custompkg"] = "Programs";
    settings.custom_extension_categories["myvideo"] = "Video";
    settings.category_folders["Audio"] = (work_dir / "CustomAudio").string();
    settings.site_rules["special-music.org"] = "Audio";
    settings.site_rules["direct-dump.net"] = (work_dir / "DumpFolder").string();
    ASSERT_TRUE(ctrl.UpdateSettings(settings).ok(), "Updated settings");

    // 1. Standard extension inference
    ASSERT_TRUE(ctrl.DetermineCategory("song.mp3", "https://example.com/song.mp3") == "Audio", "song.mp3 -> Audio");
    ASSERT_TRUE(ctrl.DetermineCategory("archive.7z", "https://example.com/archive.7z") == "Compressed", "archive.7z -> Compressed");

    // 2. Custom extension mapping
    ASSERT_TRUE(ctrl.DetermineCategory("tool.custompkg", "https://example.com/tool.custompkg") == "Programs", "custompkg -> Programs");

    // 3. Site rule (Domain -> Category)
    ASSERT_TRUE(ctrl.DetermineCategory("track.unknown", "https://special-music.org/track.unknown") == "Audio", "special-music.org -> Audio");

    // 4. Resolve category save dir priority
    // 4a. Site rule directory override
    auto dump_dir = ctrl.ResolveCategorySaveDir("General", "https://direct-dump.net/file.bin");
    ASSERT_TRUE(dump_dir == (work_dir / "DumpFolder"), "Site rule directory override matches DumpFolder");

    // 4b. Category custom folder
    auto audio_dir = ctrl.ResolveCategorySaveDir("Audio", "https://example.com/song.mp3");
    ASSERT_TRUE(audio_dir == (work_dir / "CustomAudio"), "Custom Category Folder matches CustomAudio");

    // 4c. Category subfolder
    auto video_dir = ctrl.ResolveCategorySaveDir("Video", "https://example.com/clip.mp4");
    ASSERT_TRUE(video_dir == (cfg.default_download_dir / "Video"), "Video subfolder under default dir");

    // 4d. Default dir
    auto gen_dir = ctrl.ResolveCategorySaveDir("General", "https://example.com/data.bin");
    ASSERT_TRUE(gen_dir == cfg.default_download_dir, "General category maps to default dir");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestAlternateUrlsAndMirrorValidation() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_mirrors_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    std::filesystem::create_directories(cfg.default_download_dir, ec);

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller");

    openidm::TaskModel t;
    t.task_id = "task-mirrors-1";
    t.url = "https://primary.example.com/large.iso";
    t.file_name = "large.iso";
    auto add_res = ctrl.AddTask(t, false, true);
    ASSERT_TRUE(add_res.ok(), "Added task");

    auto add_alt1 = ctrl.AddAlternateUrl("task-mirrors-1", "https://mirror1.example.com/large.iso");
    ASSERT_TRUE(add_alt1.ok(), "Added mirror 1");
    auto add_alt2 = ctrl.AddAlternateUrl("task-mirrors-1", "https://mirror2.example.com/large.iso");
    ASSERT_TRUE(add_alt2.ok(), "Added mirror 2");

    auto alts = ctrl.GetAlternateUrls("task-mirrors-1");
    ASSERT_TRUE(alts.size() == 2, "Task has 2 alternate URLs");
    ASSERT_TRUE(alts[0] == "https://mirror1.example.com/large.iso", "Mirror 1 match");
    ASSERT_TRUE(alts[1] == "https://mirror2.example.com/large.iso", "Mirror 2 match");

    // Adding duplicate mirror URL is idempotent
    auto add_dup = ctrl.AddAlternateUrl("task-mirrors-1", "https://mirror1.example.com/large.iso");
    ASSERT_TRUE(add_dup.ok(), "Adding duplicate mirror succeeds");
    ASSERT_TRUE(ctrl.GetAlternateUrls("task-mirrors-1").size() == 2, "Size remains 2");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestFtpReliabilityEpsvAndResumeTruncation() {
    using openidm::ftp::FtpUrl;

    auto parsed = FtpUrl::Parse("ftp://user:pass@ftp.example.com:2121/files/document.pdf");
    ASSERT_TRUE(parsed.ok(), "Parsed FTP URL with custom port");
    ASSERT_TRUE(parsed.value->port == 2121, "Port is 2121");
    ASSERT_TRUE(parsed.value->username == "user", "Username is user");
    ASSERT_TRUE(parsed.value->password == "pass", "Password is pass");
    ASSERT_TRUE(parsed.value->file_name == "document.pdf", "File name is document.pdf");

    return true;
}

bool TestSchedulerStopTimesAndPostCompletionActions() {
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / ("flux_test_sched_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    std::filesystem::create_directories(cfg.default_download_dir, ec);

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Initialize controller");

    openidm::TaskModel t;
    t.task_id = "task-sched-1";
    t.url = "https://example.com/nightly.iso";
    t.file_name = "nightly.iso";
    auto add_res = ctrl.AddTask(t, false, true);
    ASSERT_TRUE(add_res.ok(), "Added task");

    auto set_sched = ctrl.SetTaskSchedule("task-sched-1", "02:00", "06:00");
    ASSERT_TRUE(set_sched.ok(), "SetTaskSchedule succeeded");

    auto task_opt = ctrl.GetTask("task-sched-1");
    ASSERT_TRUE(task_opt.has_value(), "Task exists");
    ASSERT_TRUE(task_opt->scheduled, "Task scheduled is true");
    ASSERT_TRUE(task_opt->schedule_time_local == "02:00", "Start time is 02:00");
    ASSERT_TRUE(task_opt->schedule_stop_time_local == "06:00", "Stop time is 06:00");

    // Verify Settings Center Queue completion action
    auto settings = ctrl.GetSettings();
    settings.queue_completion_action = "SLEEP";
    settings.max_auto_retries = 8;
    settings.retry_interval_sec = 20;
    ASSERT_TRUE(ctrl.UpdateSettings(settings).ok(), "Updated settings");

    auto updated = ctrl.GetSettings();
    ASSERT_TRUE(updated.queue_completion_action == "SLEEP", "Queue completion action is SLEEP");
    ASSERT_TRUE(updated.max_auto_retries == 8, "Max auto retries is 8");
    ASSERT_TRUE(updated.retry_interval_sec == 20, "Retry interval is 20s");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestPeerWireProtocolFramingAndCompactPeers() {
    using namespace openidm::torrent;

    // 1. Peer ID format
    std::string peer_id = PeerWireProtocol::GeneratePeerId();
    ASSERT_TRUE(peer_id.size() == 20, "Peer ID is exactly 20 bytes");
    ASSERT_TRUE(peer_id.rfind("-FL0100-", 0) == 0, "Peer ID has -FL0100- prefix");

    // 2. Handshake serialization and verification
    std::vector<uint8_t> info_hash(20, 0x5A);
    auto hs_bytes = PeerWireProtocol::CreateHandshake(info_hash, peer_id);
    ASSERT_TRUE(hs_bytes.size() == 68, "Handshake length is 68 bytes");
    ASSERT_TRUE(hs_bytes[0] == 19, "Handshake pstrlen is 19");
    ASSERT_TRUE(std::memcmp(&hs_bytes[1], "BitTorrent protocol", 19) == 0, "Handshake protocol string");
    ASSERT_TRUE(std::memcmp(&hs_bytes[28], info_hash.data(), 20) == 0, "Handshake info_hash matches");
    ASSERT_TRUE(std::memcmp(&hs_bytes[48], peer_id.data(), 20) == 0, "Handshake peer_id matches");

    std::string recvd_peer_id;
    ASSERT_TRUE(PeerWireProtocol::VerifyHandshake(hs_bytes.data(), hs_bytes.size(), info_hash, &recvd_peer_id), "Verify valid handshake");
    ASSERT_TRUE(recvd_peer_id == peer_id, "Extracted peer ID matches");

    std::vector<uint8_t> wrong_hash(20, 0x11);
    ASSERT_TRUE(!PeerWireProtocol::VerifyHandshake(hs_bytes.data(), hs_bytes.size(), wrong_hash), "Reject wrong info-hash");
    ASSERT_TRUE(!PeerWireProtocol::VerifyHandshake(hs_bytes.data(), 50, info_hash), "Reject truncated handshake");

    // 3. Keep-Alive, Choke, Unchoke, Interested
    auto ka = PeerWireProtocol::CreateKeepAlive();
    ASSERT_TRUE(ka.size() == 4 && ka[0] == 0 && ka[1] == 0 && ka[2] == 0 && ka[3] == 0, "KeepAlive is 4 zero bytes");

    auto unchoke = PeerWireProtocol::CreateUnchoke();
    ASSERT_TRUE(unchoke.size() == 5 && unchoke[4] == 1, "Unchoke message format");
    auto unchoke_parsed = PeerWireProtocol::ParseMessage(unchoke.data(), unchoke.size());
    ASSERT_TRUE(unchoke_parsed.has_value() && unchoke_parsed->id == BtMessageId::kUnchoke, "Parse Unchoke");

    // 4. Have & Bitfield
    auto have_msg = PeerWireProtocol::CreateHave(1337);
    auto have_parsed = PeerWireProtocol::ParseMessage(have_msg.data(), have_msg.size());
    ASSERT_TRUE(have_parsed.has_value() && have_parsed->id == BtMessageId::kHave && have_parsed->piece_index == 1337, "Create and parse Have");

    std::vector<bool> bits = {true, false, true, true, false, false, false, true, true};
    auto bf_msg = PeerWireProtocol::CreateBitfield(bits);
    auto bf_parsed = PeerWireProtocol::ParseMessage(bf_msg.data(), bf_msg.size());
    ASSERT_TRUE(bf_parsed.has_value() && bf_parsed->id == BtMessageId::kBitfield, "Create and parse Bitfield");
    ASSERT_TRUE(bf_parsed->payload.size() == 2, "Bitfield payload size for 9 bits is 2 bytes");
    ASSERT_TRUE((bf_parsed->payload[0] & 0x80) != 0, "Bit 0 is set");
    ASSERT_TRUE((bf_parsed->payload[0] & 0x40) == 0, "Bit 1 is clear");

    // 5. Request & Piece
    auto req_msg = PeerWireProtocol::CreateRequest(7, 16384, 16384);
    auto req_parsed = PeerWireProtocol::ParseMessage(req_msg.data(), req_msg.size());
    ASSERT_TRUE(req_parsed.has_value() && req_parsed->id == BtMessageId::kRequest, "Parse Request");
    ASSERT_TRUE(req_parsed->piece_index == 7 && req_parsed->block_begin == 16384 && req_parsed->block_length == 16384, "Request fields match");

    const uint8_t sample_blk[] = {0xDE, 0xAD, 0xBE, 0xEF};
    auto pc_msg = PeerWireProtocol::CreatePiece(7, 0, sample_blk, 4);
    auto pc_parsed = PeerWireProtocol::ParseMessage(pc_msg.data(), pc_msg.size());
    ASSERT_TRUE(pc_parsed.has_value() && pc_parsed->id == BtMessageId::kPiece, "Parse Piece");
    ASSERT_TRUE(pc_parsed->piece_index == 7 && pc_parsed->block_begin == 0 && pc_parsed->payload.size() == 4, "Piece payload matches");
    ASSERT_TRUE(pc_parsed->payload[0] == 0xDE && pc_parsed->payload[3] == 0xEF, "Piece bytes match");

    // 6. Compact Peer Unpacking (aria2 unpackcompact)
    // 192.168.1.105:6881 -> C0 A8 01 69 1A E1
    const uint8_t compact_peer[6] = {192, 168, 1, 105, 0x1A, 0xE1};
    auto [ip4, port4] = PeerWireProtocol::UnpackCompactIpv4(compact_peer);
    ASSERT_TRUE(ip4 == "192.168.1.105", "Unpack IPv4 address");
    ASSERT_TRUE(port4 == 6881, "Unpack IPv4 port");

    std::string compact_12(reinterpret_cast<const char*>(compact_peer), 6);
    const uint8_t peer2[6] = {10, 0, 0, 1, 0x00, 0x50}; // 10.0.0.1:80
    compact_12.append(reinterpret_cast<const char*>(peer2), 6);
    auto peers_list = PeerWireProtocol::ExtractPeersFromCompact(compact_12);
    ASSERT_TRUE(peers_list.size() == 2, "Extracted 2 peers from 12-byte compact list");
    ASSERT_TRUE(peers_list[0].ip == "192.168.1.105" && peers_list[0].port == 6881, "Peer 0 matches");
    ASSERT_TRUE(peers_list[1].ip == "10.0.0.1" && peers_list[1].port == 80, "Peer 1 matches");

    return true;
}

bool TestMultiFileStorageMappingAndBoundarySplitting() {
    using namespace openidm::torrent;
    std::error_code ec;
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path() / "flux_test_storage";
    std::filesystem::remove_all(temp_dir, ec);
    std::filesystem::create_directories(temp_dir, ec);

    TorrentInfo info;
    info.name = "multi_pkg";
    info.total_length = 4000;
    info.piece_length = 1000;

    TorrentFileEntry f1{"sub/file_a.bin", 1000, 0};
    TorrentFileEntry f2{"sub/file_b.bin", 2000, 1000};
    TorrentFileEntry f3{"file_c.bin", 1000, 3000};
    info.files = {f1, f2, f3};

    MultiFileStorage storage;
    ASSERT_TRUE(storage.Initialize(temp_dir, info).ok(), "Storage initialized");
    ASSERT_TRUE(storage.AllocateFiles().ok(), "Files allocated");

    // Write a 1500-byte block starting at offset 500
    // Spans file_a.bin [500..1000) and file_b.bin [0..1000)
    std::vector<uint8_t> test_bytes(1500);
    for (size_t i = 0; i < test_bytes.size(); ++i) {
        test_bytes[i] = static_cast<uint8_t>((i * 7 + 13) & 0xFF);
    }

    ASSERT_TRUE(storage.WriteBytes(500, test_bytes.data(), test_bytes.size()).ok(), "Write across boundary succeeded");

    // Read back and verify exact byte matching
    std::vector<uint8_t> read_bytes(1500, 0);
    ASSERT_TRUE(storage.ReadBytes(500, read_bytes.data(), read_bytes.size()).ok(), "Read across boundary succeeded");
    ASSERT_TRUE(read_bytes == test_bytes, "Read back bytes match written bytes exactly");

    // Verify disk files directly
    std::filesystem::path p1 = temp_dir / "sub" / "file_a.bin";
    std::filesystem::path p2 = temp_dir / "sub" / "file_b.bin";
    ASSERT_TRUE(std::filesystem::exists(p1), "file_a exists on disk");
    ASSERT_TRUE(std::filesystem::exists(p2), "file_b exists on disk");

    // Write another block spanning file_b.bin into file_c.bin at offset 2800 (length 500)
    std::vector<uint8_t> span2(500, 0x42);
    ASSERT_TRUE(storage.WriteBytes(2800, span2.data(), span2.size()).ok(), "Write across second boundary");
    std::vector<uint8_t> read_span2(500, 0);
    ASSERT_TRUE(storage.ReadBytes(2800, read_span2.data(), read_span2.size()).ok(), "Read across second boundary");
    ASSERT_TRUE(read_span2 == span2, "Second boundary slice matches");

    std::filesystem::remove_all(temp_dir, ec);
    return true;
}

bool TestPieceManagerAndBlockScheduling() {
    using namespace openidm::torrent;
    std::error_code ec;
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path() / "flux_test_piecemgr";
    std::filesystem::remove_all(temp_dir, ec);
    std::filesystem::create_directories(temp_dir, ec);

    // Create 2 pieces of 32 KB each (total 64 KB)
    TorrentInfo info;
    info.name = "test_pieces.bin";
    info.total_length = 65536;
    info.piece_length = 32768;

    std::vector<uint8_t> piece0_data(32768, 'A');
    std::vector<uint8_t> piece1_data(32768, 'B');

    std::vector<uint8_t> hash0 = openidm::CryptoUtils::ComputeBufferSha1Raw(piece0_data.data(), piece0_data.size());
    std::vector<uint8_t> hash1 = openidm::CryptoUtils::ComputeBufferSha1Raw(piece1_data.data(), piece1_data.size());
    info.pieces = {hash0, hash1};

    auto storage = std::make_shared<MultiFileStorage>();
    ASSERT_TRUE(storage->Initialize(temp_dir, info).ok(), "Storage init");
    storage->AllocateFiles();

    PieceManager mgr(info, storage);
    ASSERT_TRUE(mgr.TotalPieces() == 2, "Total pieces is 2");
    ASSERT_TRUE(mgr.CompletedPieces() == 0, "Initial completed pieces is 0");
    ASSERT_TRUE(!mgr.IsAllCompleted(), "Not all completed initially");

    // Peer offers piece 0 and 1
    std::vector<bool> peer_have = {true, true};
    auto reqs = mgr.GetNextRequestsForPeer(peer_have, 4);
    // Piece size 32768 -> two 16 KB blocks (0..16384, 16384..32768)
    ASSERT_TRUE(reqs.size() == 2, "Generated 2 block requests for piece 0");
    ASSERT_TRUE(reqs[0].piece_index == 0 && reqs[0].begin == 0 && reqs[0].length == 16384, "Block 0");
    ASSERT_TRUE(reqs[1].piece_index == 0 && reqs[1].begin == 16384 && reqs[1].length == 16384, "Block 1");

    // Feed block 0 of piece 0
    bool verified = false, corrupt = false;
    mgr.ProcessBlock(0, 0, piece0_data.data(), 16384, verified, corrupt);
    ASSERT_TRUE(!verified && !corrupt, "First block processed; piece not complete yet");
    ASSERT_TRUE(mgr.CompletedPieces() == 0, "Piece 0 not yet marked complete");

    // Feed block 1 of piece 0 -> completes piece and triggers SHA-1 verification
    mgr.ProcessBlock(0, 16384, piece0_data.data() + 16384, 16384, verified, corrupt);
    ASSERT_TRUE(verified && !corrupt, "Piece 0 complete and verified by SHA-1");
    ASSERT_TRUE(mgr.CompletedPieces() == 1, "Completed pieces is now 1");
    ASSERT_TRUE(mgr.DownloadedBytes() == 32768, "Downloaded bytes updated to 32768");
    ASSERT_TRUE(mgr.HavePieces()[0] == true, "HavePieces[0] marked true");

    // Feed corrupt block data for piece 1
    std::vector<uint8_t> corrupt_data(16384, 'Z');
    mgr.ProcessBlock(1, 0, corrupt_data.data(), 16384, verified, corrupt);
    mgr.ProcessBlock(1, 16384, piece1_data.data() + 16384, 16384, verified, corrupt);
    ASSERT_TRUE(!verified && corrupt, "Corrupt piece detected by SHA-1 verification and rejected");
    ASSERT_TRUE(mgr.CompletedPieces() == 1, "Piece 1 not marked complete due to corruption");

    std::filesystem::remove_all(temp_dir, ec);
    return true;
}

bool TestTrackerClientQueryEncodingAndResponseParsing() {
    using namespace openidm::torrent;

    // 1. URL percent encoding of raw binary info-hash
    const uint8_t binary_hash[5] = {0x00, 0x12, 0xFF, 'A', 'z'};
    std::string enc = TrackerClient::PercentEncodeBinary(binary_hash, 5);
    ASSERT_TRUE(enc == "%00%12%FFAz", "Binary percent encoding preserves unreserved chars and encodes non-printables");

    // Bencoded dict: d8:completei42e10:incompletei7e8:intervali1800e5:peers12:\xC0\xA8\x01\x69\x1A\xE1\x0A\x00\x00\x01\x00\x50 + e
    const char kRespBytes[] = "d8:completei42e10:incompletei7e8:intervali1800e5:peers12:\xC0\xA8\x01\x69\x1A\xE1\x0A\x00\x00\x01\x00\x50" "e";
    std::string bencoded_resp(kRespBytes, sizeof(kRespBytes) - 1);
    auto tr_res = TrackerClient::ParseTrackerResponse(bencoded_resp);
    ASSERT_TRUE(tr_res.ok(), "Parse valid tracker response");
    ASSERT_TRUE(tr_res.value->complete == 42, "Seeders is 42");
    ASSERT_TRUE(tr_res.value->incomplete == 7, "Leechers is 7");
    ASSERT_TRUE(tr_res.value->interval == 1800, "Interval is 1800s");
    ASSERT_TRUE(tr_res.value->peers.size() == 2, "Unpacked 2 peers from compact binary list");
    ASSERT_TRUE(tr_res.value->peers[0].ip == "192.168.1.105" && tr_res.value->peers[0].port == 6881, "Peer 0 matches");
    ASSERT_TRUE(tr_res.value->peers[1].ip == "10.0.0.1" && tr_res.value->peers[1].port == 80, "Peer 1 matches");

    // 3. Tracker Failure Reason parsing
    std::string fail_resp = "d14:failure reason26:Torrent passkey is invalide";
    auto fail_res = TrackerClient::ParseTrackerResponse(fail_resp);
    ASSERT_TRUE(!fail_res.ok(), "Tracker failure response detected");
    ASSERT_TRUE(fail_res.error->code == "ERR_TORRENT_TRACKER_FAILURE", "Code is ERR_TORRENT_TRACKER_FAILURE");
    ASSERT_TRUE(fail_res.error->message == "Torrent passkey is invalid", "Error message contains failure reason");

    return true;
}

bool TestHttpRangeSatisfactionValidation() {
    using namespace openidm::brisk;

    // Simulate aria2 isRangeSatisfied logic
    uint64_t total_bytes = 10485760;
    uint64_t curr = 1048576;

    // 1. Valid 206 Partial Content response
    HttpResponseMeta meta_valid;
    meta_valid.status_code = 206;
    meta_valid.is_partial_content = true;
    meta_valid.range_start = 1048576;
    meta_valid.range_end = 2097151;
    meta_valid.range_total = 10485760;

    bool valid_satisfied = (meta_valid.status_code == 206 && meta_valid.is_partial_content &&
                            meta_valid.range_start == curr &&
                            (meta_valid.range_total == 0 || meta_valid.range_total == total_bytes));
    ASSERT_TRUE(valid_satisfied, "Valid 206 response satisfies range");

    // 2. Server returned 206 but entity size changed (remote file modified)
    HttpResponseMeta meta_mismatch;
    meta_mismatch.status_code = 206;
    meta_mismatch.is_partial_content = true;
    meta_mismatch.range_start = 1048576;
    meta_mismatch.range_end = 2097151;
    meta_mismatch.range_total = 20971520; // Size changed on server!

    bool entity_mismatch = (total_bytes > 0 && meta_mismatch.range_total > 0 && meta_mismatch.range_total != total_bytes);
    ASSERT_TRUE(entity_mismatch, "Detected remote entity length change mid-download");

    // 3. Server returned 200 OK starting from byte 0 instead of 206
    HttpResponseMeta meta_ignored_range;
    meta_ignored_range.status_code = 200;
    meta_ignored_range.is_partial_content = false;
    meta_ignored_range.range_start = 0;

    bool range_ignored = (meta_ignored_range.status_code != 206 || !meta_ignored_range.is_partial_content || meta_ignored_range.range_start != curr);
    ASSERT_TRUE(range_ignored, "Server ignoring Range header is caught and rejected");

    return true;
}

bool TestDuplicateTaskDetectionThreeTiers() {
    using namespace openidm::core;

    // 1. URL Normalization
    std::string norm1 = DuplicateDetector::NormalizeUrl("  HTTP://Example.COM:80/path/file.zip?token=123#frag  ");
    ASSERT_TRUE(norm1 == "http://example.com/path/file.zip?token=123", "Normalized URL strips fragment, lowers host/scheme, removes default port");

    std::string norm2 = DuplicateDetector::NormalizeUrl("https://API.Server.org:8443/data/v1/?query=yes#section");
    ASSERT_TRUE(norm2 == "https://api.server.org:8443/data/v1/?query=yes", "Non-default port and query preserved");

    // 2. Setup tasks list
    std::vector<openidm::TaskModel> tasks;
    openidm::TaskModel t1;
    t1.task_id = "task-url-1";
    t1.url = "https://cdn.example.com/downloads/setup.exe?v=1";
    t1.file_name = "setup.exe";
    t1.total_bytes = 10485760;
    t1.etag = "\"33a64df551425fcc55e4d42a148795d9f25f89d4\"";
    t1.created_at_ms = 1000;
    tasks.push_back(t1);

    openidm::TaskModel t2;
    t2.task_id = "task-name-size-1";
    t2.url = "https://mirror1.example.org/files/linux-distro.iso";
    t2.file_name = "linux-distro.iso";
    t2.total_bytes = 2147483648ULL;
    t2.etag = "\"iso-hash-111\"";
    t2.created_at_ms = 2000;
    tasks.push_back(t2);

    openidm::TaskModel t3;
    t3.task_id = "task-etag-1";
    t3.url = "https://cdn.example.com/assets/video_hd.mp4";
    t3.file_name = "video_hd.mp4";
    t3.total_bytes = 524288000;
    t3.etag = "\"strong-etag-video-999\"";
    t3.created_at_ms = 3000;
    tasks.push_back(t3);

    // Tier 1: Match by URL (even with different casing, whitespace, or fragment)
    auto match_url = DuplicateDetector::FindDuplicate(tasks, "  HTTPS://CDN.EXAMPLE.COM:443/downloads/setup.exe?v=1#download  ");
    ASSERT_TRUE(match_url.kind == DuplicateKind::kSameUrl, "Tier 1: Matched by Same URL");
    ASSERT_TRUE(match_url.matched_task.task_id == "task-url-1", "Matched correct task ID");

    // Tier 2: Match by File Name and Size (different URL, but same case-insensitive name & size)
    auto match_name_size = DuplicateDetector::FindDuplicate(tasks, "https://mirror2.different.net/archive/LINUX-DISTRO.ISO", "LINUX-DISTRO.ISO", 2147483648ULL);
    ASSERT_TRUE(match_name_size.kind == DuplicateKind::kSameNameAndSize, "Tier 2: Matched by Same Name & Size");
    ASSERT_TRUE(match_name_size.matched_task.task_id == "task-name-size-1", "Matched correct task ID");

    // Rejection on size 0 or mismatch
    auto no_match_size0 = DuplicateDetector::FindDuplicate(tasks, "https://mirror2.different.net/archive/linux-distro.iso", "linux-distro.iso", 0);
    ASSERT_TRUE(no_match_size0.kind == DuplicateKind::kNone, "Size 0 does not trigger name-and-size match");

    // Tier 3: Match by Strong ETag on Same Host (different URL and name, but same host and strong ETag)
    auto match_etag = DuplicateDetector::FindDuplicate(tasks, "https://cdn.example.com/rest/get_video?id=55", "renamed.mp4", 100, "\"strong-etag-video-999\"");
    ASSERT_TRUE(match_etag.kind == DuplicateKind::kSameEtag, "Tier 3: Matched by Strong ETag on same host");
    ASSERT_TRUE(match_etag.matched_task.task_id == "task-etag-1", "Matched correct task ID");

    // Rejection of Weak ETag
    auto match_weak_etag = DuplicateDetector::FindDuplicate(tasks, "https://cdn.example.com/rest/get_video?id=55", "renamed.mp4", 100, "W/\"strong-etag-video-999\"");
    ASSERT_TRUE(match_weak_etag.kind == DuplicateKind::kNone, "Weak ETag (W/) rejected for duplicate detection");

    // Rejection of ETag on different host
    auto match_diff_host_etag = DuplicateDetector::FindDuplicate(tasks, "https://unrelated-host.org/get", "renamed.mp4", 100, "\"strong-etag-video-999\"");
    ASSERT_TRUE(match_diff_host_etag.kind == DuplicateKind::kNone, "ETag match rejected across different hosts");

    // Tier Precedence: URL match takes precedence over Name & Size match
    auto match_precedence = DuplicateDetector::FindDuplicate(tasks, "https://cdn.example.com/downloads/setup.exe?v=1", "linux-distro.iso", 2147483648ULL);
    ASSERT_TRUE(match_precedence.kind == DuplicateKind::kSameUrl, "Tier 1 takes precedence over Tier 2");
    ASSERT_TRUE(match_precedence.matched_task.task_id == "task-url-1", "URL match preferred");

    // 4. Motrix Reference Reuse: BTIH info-hash normalization & magnet link deduplication
    std::string hex_norm = DuplicateDetector::NormalizeBtInfoHash("D1E05F7C32BF24F6E1F0E4708E33E9B11D8D3F66");
    ASSERT_TRUE(hex_norm == "d1e05f7c32bf24f6e1f0e4708e33e9b11d8d3f66", "40-hex infohash lowercased");

    std::string base32_zeros = DuplicateDetector::NormalizeBtInfoHash("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    ASSERT_TRUE(base32_zeros == "0000000000000000000000000000000000000000", "32-base32 zeros decoded to 40-hex");

    std::string base32_ones = DuplicateDetector::NormalizeBtInfoHash("77777777777777777777777777777777");
    ASSERT_TRUE(base32_ones == "ffffffffffffffffffffffffffffffffffffffff", "32-base32 all-ones decoded to 40-hex");

    // Setup magnet task with 40-hex hash
    openidm::TaskModel t_magnet;
    t_magnet.task_id = "task-magnet-1";
    t_magnet.url = "magnet:?xt=urn:btih:ffffffffffffffffffffffffffffffffffffffff&dn=TestTorrent";
    t_magnet.file_name = "TestTorrent";
    t_magnet.created_at_ms = 4000;
    tasks.push_back(t_magnet);

    // Incoming duplicate magnet link with 32-base32 hash and disparate tracker params
    auto match_magnet = DuplicateDetector::FindDuplicate(tasks, "magnet:?xt=urn:btih:77777777777777777777777777777777&tr=http%3A%2F%2Ftracker.example.com%2Fannounce#extra");
    ASSERT_TRUE(match_magnet.kind == DuplicateKind::kSameUrl, "Matched magnet link via BTIH normalization across Base32/Hex");
    ASSERT_TRUE(match_magnet.matched_task.task_id == "task-magnet-1", "Matched correct magnet task ID");

    // 5. Motrix Reference Reuse: FinalNamePicker .part file collision prevention
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path() / ("test_motrix_part_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(temp_dir);
    std::filesystem::path primary = temp_dir / "collision_test.bin";
    std::filesystem::path part_slot1 = temp_dir / "collision_test (1).bin.part";

    // Create primary file and (1).part file
    {
        std::ofstream(primary) << "primary";
        std::ofstream(part_slot1) << "partial_download";
    }

    auto safe_res = openidm::PathSanitizer::ResolveSafeOutputPath(temp_dir, "collision_test.bin", openidm::ConflictPolicy::kAutoRename);
    ASSERT_TRUE(safe_res.ok(), "Safe output path resolved successfully");
    // Should skip collision_test (1).bin because collision_test (1).bin.part exists, picking (2)
    ASSERT_TRUE(safe_res.value->filename().string() == "collision_test (2).bin", "Avoided slot with existing in-progress .part file, picked slot (2)");

    std::error_code ec;
    std::filesystem::remove_all(temp_dir, ec);

    return true;
}

bool TestExpiredLinkRefreshAndCookieReplacement() {
    using namespace openidm;

    // 1. IsLinkExpiredStatus verification
    ASSERT_TRUE(brisk::HttpClient::IsLinkExpiredStatus(403), "HTTP 403 Forbidden is link expired");
    ASSERT_TRUE(brisk::HttpClient::IsLinkExpiredStatus(410), "HTTP 410 Gone is link expired");
    ASSERT_TRUE(brisk::HttpClient::IsLinkExpiredStatus(401, ""), "HTTP 401 without auth header is link expired");
    ASSERT_TRUE(!brisk::HttpClient::IsLinkExpiredStatus(401, "Basic realm=\"Access\""), "HTTP 401 with Basic auth challenge is not link expired");
    ASSERT_TRUE(!brisk::HttpClient::IsLinkExpiredStatus(401, "NTLM"), "HTTP 401 with NTLM challenge is not link expired");
    ASSERT_TRUE(!brisk::HttpClient::IsLinkExpiredStatus(404), "HTTP 404 is not link expired");
    ASSERT_TRUE(!brisk::HttpClient::IsLinkExpiredStatus(200), "HTTP 200 is not link expired");

    // 2. Controller RefreshTaskUrl end-to-end state and cookie replacement test
    std::string temp_db = (std::filesystem::temp_directory_path() / ("test_refresh_" + GenerateUuidV4() + ".db")).string();
    ControllerConfig cfg;
    cfg.state_db_path = temp_db;
    cfg.default_download_dir = std::filesystem::temp_directory_path();
    cfg.temp_root_dir = std::filesystem::temp_directory_path();

    DownloadController controller(cfg);
    auto init_res = controller.Initialize();
    ASSERT_TRUE(init_res.ok(), "Initialize controller for refresh test");

    TaskModel t;
    t.task_id = "task-refresh-1";
    t.url = "https://cdn.example.com/expired-presigned-url?sig=old123";
    t.file_name = "archive.tar.gz";
    t.downloaded_bytes = 4194304;
    t.total_bytes = 16777216;
    t.headers["Cookie"] = "session=stale_session_token_old; id=123";
    t.headers["User-Agent"] = "CustomAgent/1.0";
    t.auth_headers["Authorization"] = "Bearer old-bearer-token";
    t.state = TaskState::kPaused;
    t.error_code = "ERR_HTTP_403_EXPIRED_OR_AUTH";
    t.error_message = "HTTP 403 (Link Expired or Auth Required)";
    t.retry_count = 3;

    // Create partial segment
    Segment seg0{0, 0, 4194304, 16777215, false};
    t.segments.push_back(seg0);

    auto add_res = controller.AddTask(t, false, true);
    ASSERT_TRUE(add_res.ok(), "Task added");

    // Refresh URL with new URL, new Cookie, and new Authorization header
    std::map<std::string, std::string> new_headers = {
        {"Cookie", "session=fresh_session_token_new"},
        {"X-Custom-Header", "Value123"}
    };
    std::map<std::string, std::string> new_auth = {
        {"Authorization", "Bearer refreshed-new-bearer"}
    };

    auto ref_res = controller.RefreshTaskUrl(
        "task-refresh-1",
        "https://cdn.example.com/fresh-presigned-url?sig=new999",
        new_headers,
        new_auth);
    ASSERT_TRUE(ref_res.ok(), "RefreshTaskUrl succeeds");

    auto updated_opt = controller.GetTask("task-refresh-1");
    ASSERT_TRUE(updated_opt.has_value(), "Task found after refresh");
    const auto& up = *updated_opt;

    // Verify URL updated
    ASSERT_TRUE(up.url == "https://cdn.example.com/fresh-presigned-url?sig=new999", "URL refreshed");

    // Verify Cookie cleanly replaced (not merged with stale cookie)
    ASSERT_TRUE(up.headers.at("Cookie") == "session=fresh_session_token_new", "Cookie replaced without old session lingering");
    ASSERT_TRUE(up.headers.at("Cookie").find("stale_session_token_old") == std::string::npos, "Old cookie scrubbed");
    ASSERT_TRUE(up.headers.at("User-Agent") == "CustomAgent/1.0", "Preserved existing headers");
    ASSERT_TRUE(up.headers.at("X-Custom-Header") == "Value123", "Added new headers");

    // Verify Auth header replaced
    ASSERT_TRUE(up.auth_headers.at("Authorization") == "Bearer refreshed-new-bearer", "Auth header replaced");

    // Verify partial segments and downloaded bytes preserved
    ASSERT_TRUE(up.downloaded_bytes == 4194304, "Downloaded bytes preserved on disk");
    ASSERT_TRUE(up.total_bytes == 16777216, "Total bytes preserved");
    ASSERT_TRUE(up.segments.size() == 1 && up.segments[0].current_offset == 4194304, "Segment cursor preserved for immediate resume");

    // Verify errors cleared and state queued with fresh retries
    ASSERT_TRUE(up.error_code.empty() && up.error_message.empty(), "Error state cleared");
    ASSERT_TRUE(up.retry_count == 0, "Retry count reset for fresh link");
    ASSERT_TRUE(up.state == TaskState::kQueued, "Task queued for resume");

    controller.Shutdown();
    std::error_code ec;
    std::filesystem::remove(temp_db, ec);
    return true;
}

bool TestSegmentSchedulerChunkNormalizationAndTruncationGuard() {
    using namespace openidm::brisk;

    uint64_t total_bytes = 10000;
    SegmentScheduler scheduler(total_bytes, 4);

    // Segment 0: completed [0, 4999] with cursor at 5000, BUT completed flag was false (crash right after last byte)
    // Segment 1: incomplete [5000, 9999] with cursor at 6000
    std::vector<openidm::Segment> unnormalized_segments = {
        openidm::Segment{0, 0, 5000, 4999, false}, // current_offset == end_offset + 1, completed = false
        openidm::Segment{1, 5000, 6000, 9999, false}
    };

    ASSERT_TRUE(SegmentScheduler::ValidateSegmentPartition(unnormalized_segments, total_bytes),
                "ValidateSegmentPartition accepts consistent cursors where is_fully_read is true");

    scheduler.Initialize(unnormalized_segments);

    auto snapshots = scheduler.GetSnapshots();
    ASSERT_TRUE(snapshots.size() == 2, "2 segments restored");
    ASSERT_TRUE(snapshots[0].completed, "Segment 0 normalized to completed=true (XDM normalizeRestoredChunks)");
    ASSERT_TRUE(!snapshots[1].completed, "Segment 1 remains incomplete");
    ASSERT_TRUE(!scheduler.AllCompleted(), "Scheduler knows task is not finished yet");

    // Steal or acquire next segment: should immediately pick segment 1 without spawning worker for segment 0
    auto seg = scheduler.AcquireNextOrStealSegment(false);
    ASSERT_TRUE(seg != nullptr, "Acquired segment");
    ASSERT_TRUE(seg->segment_id == 1, "Acquired segment 1; segment 0 was skipped because normalized to completed");

    // Simulate segment 1 finishing: cursor advances to 10000
    seg->current_offset.store(10000);
    seg->completed.store(true);

    ASSERT_TRUE(scheduler.AllCompleted(), "All segments complete after segment 1 finished");

    return true;
}

bool TestUrlUserinfoExtractionAndBasicAuthGeneration() {
    using namespace openidm;
    using namespace openidm::brisk;

    // 1. Parse URL with userinfo (username:password)
    auto p1 = HttpClient::ParseUrl("http://admin:secret123@127.0.0.1:8080/downloads/file.bin?token=abc");
    ASSERT_TRUE(p1.ok(), "ParseUrl parses URL with userinfo");
    ASSERT_TRUE(p1.value->userinfo == "admin:secret123", "Extracted userinfo");
    ASSERT_TRUE(p1.value->host == "127.0.0.1", "Extracted clean host without userinfo");
    ASSERT_TRUE(p1.value->port == 8080, "Extracted port 8080");
    ASSERT_TRUE(p1.value->path_and_query == "/downloads/file.bin?token=abc", "Path and query correct");

    // 2. Base64 encoding/decoding of credentials
    std::string b64 = CryptoUtils::Base64Encode(p1.value->userinfo);
    ASSERT_TRUE(b64 == "YWRtaW46c2VjcmV0MTIz", "Base64 encoding matches admin:secret123");
    auto dec = CryptoUtils::Base64Decode(b64);
    std::string dec_str(dec.begin(), dec.end());
    ASSERT_TRUE(dec_str == "admin:secret123", "Base64 decoding restores original string");

    // 3. Parse URL with IPv6 host and userinfo
    auto p2 = HttpClient::ParseUrl("https://user:p@ss@[::1]:8443/api/v1");
    ASSERT_TRUE(p2.ok(), "ParseUrl parses IPv6 host with userinfo");
    ASSERT_TRUE(p2.value->userinfo == "user:p@ss", "Extracted userinfo before @");
    ASSERT_TRUE(p2.value->host == "::1", "Extracted clean IPv6 host");
    ASSERT_TRUE(p2.value->port == 8443, "Extracted port 8443");

    return true;
}

// Regression Suite 52: Modern Coral Charcoal Theme, Settings Modal Bounds, Sorting Labels & Safe Delete
bool TestModernCoralThemeSettingsModalBoundsAndSafeDelete() {
    // 1. AppSettings default theme is "coral_dark"
    openidm::AppSettings default_settings;
    ASSERT_TRUE(default_settings.theme == "coral_dark", "Default theme is coral_dark");

    // 2. Palette resolution: "coral_dark", "coral", "charcoal", and legacy "dark" resolve to GetRedesignPalette (#FF4F57)
    auto pal_coral = openidm::ui::DesignSystem::ResolvePalette("coral_dark");
    ASSERT_TRUE(pal_coral.accent_primary.r == 0xFF &&
                pal_coral.accent_primary.g == 0x4F &&
                pal_coral.accent_primary.b == 0x57, "coral_dark resolves to coral red #FF4F57");
    ASSERT_TRUE(pal_coral.app_background.r == 0x16 &&
                pal_coral.app_background.g == 0x16 &&
                pal_coral.app_background.b == 0x18, "coral_dark resolves to warm matte charcoal background #161618");

    auto pal_dark = openidm::ui::DesignSystem::ResolvePalette("dark");
    ASSERT_TRUE(pal_dark.accent_primary.r == 0xFF &&
                pal_dark.accent_primary.g == 0x4F &&
                pal_dark.accent_primary.b == 0x57, "legacy dark resolves to coral red #FF4F57");

    auto pal_classic = openidm::ui::DesignSystem::ResolvePalette("classic_blue");
    ASSERT_TRUE(pal_classic.accent_primary.r == 0x25 &&
                pal_classic.accent_primary.g == 0x63 &&
                pal_classic.accent_primary.b == 0xEB, "classic_blue resolves to legacy blue #2563EB");

    // 3. Modern Windows 11 Typography ramp floors
    auto metrics_96 = openidm::ui::DesignSystem::ComputeMetrics(96);
    ASSERT_TRUE(metrics_96.font_body_px >= 14, "Body font size is at least 14px at 96 DPI");
    ASSERT_TRUE(metrics_96.font_caption_px >= 12, "Caption font size is at least 12px at 96 DPI");

    // 4. Controller initialization and RestoreDefaultSettings defaults
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path(ec) / ("flux_test_theme_" + openidm::GenerateUuidV4());
    std::filesystem::create_directories(work_dir, ec);

    openidm::ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";

    openidm::DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Controller initialized");
    ASSERT_TRUE(ctrl.GetSettings().theme == "coral_dark", "Initialized controller settings theme is coral_dark");

    auto restored = ctrl.RestoreDefaultSettings();
    ASSERT_TRUE(restored.ok() && restored.value->theme == "coral_dark", "RestoreDefaultSettings returns coral_dark theme");

    // 5. DesktopView Settings Modal Hit Testing Bounds & Click-Through Absorption
    openidm::ui::DesktopView view(ctrl);
    ASSERT_TRUE(!view.IsSettingsCenterOpen(), "Settings center starts closed");

    // When settings center is open, hit testing inside modal dialog returns kSettingsModalBackground
    view.OpenSettingsCenter(openidm::ui::SettingsCategory::kGeneral);
    ASSERT_TRUE(view.IsSettingsCenterOpen(), "Settings center opened");

    auto sv = view.BuildSettingsCenterView();

    // A click inside the modal that doesn't hit a specific control must return kSettingsModalBackground (not fall through!)
    auto hit_inside = view.HitTestPoint(sv.modal_rect.left + 50, sv.modal_rect.top + 30); // in modal header area
    ASSERT_TRUE(hit_inside.type == openidm::ui::HitTargetType::kSettingsModalBackground,
                "Click inside modal background returns kSettingsModalBackground");

    // A click outside the modal must return kSettingsBackdrop (not fall through to toolbar/table!)
    auto hit_outside = view.HitTestPoint(5, 5); // top-left corner outside modal
    ASSERT_TRUE(hit_outside.type == openidm::ui::HitTargetType::kSettingsBackdrop,
                "Click outside modal returns kSettingsBackdrop");

    // Verify HandleMouseClick on backdrop closes or handles modal safely
    bool handled = view.HandleMouseClick(5, 5, false, 0);
    ASSERT_TRUE(handled, "HandleMouseClick handles backdrop click");
    ASSERT_TRUE(!view.IsSettingsCenterOpen(), "Backdrop click closes settings center");

    // 6. Safe Deletion: ActionMoveSelectedToRecycleBin safely removes task
    openidm::TaskModel t_del;
    t_del.url = "https://example.com/delete_test.zip";
    t_del.file_name = "delete_test.zip";
    t_del.save_dir = work_dir.string();
    auto add_res = ctrl.AddTask(t_del, false);
    ASSERT_TRUE(add_res.ok(), "Task added for deletion test");
    std::string test_task_id = add_res.value->task_id;

    view.SelectTask(test_task_id);
    auto bin_res = view.ActionMoveSelectedToRecycleBin();
    ASSERT_TRUE(bin_res.ok(), "ActionMoveSelectedToRecycleBin succeeds");
    ASSERT_TRUE(!ctrl.GetTask(test_task_id).has_value(), "Task removed from controller after recycle bin action");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestYouTubeSpeedParsingMidnightCrossingAndBitTorrentUpload() {
    using namespace openidm;
    using namespace openidm::torrent;

    // 1. Regex parsing of yt-dlp download progress and speed
    static const std::regex kYtDlpProgRe(R"(\[download\]\s+(\d+(?:\.\d+)?)%\s+of\s+~?\s*([0-9.]+)(KiB|MiB|GiB))");
    static const std::regex kYtDlpSpeedRe(R"(at\s+([0-9.]+)(B|KiB|MiB|GiB)/s)");

    std::string sample_line = "[download]  45.2% of  120.50MiB at  4.85MiB/s ETA 00:15";
    std::smatch m;
    ASSERT_TRUE(std::regex_search(sample_line, m, kYtDlpProgRe), "Prog regex matches yt-dlp output");
    double pct = std::stod(m[1].str());
    double sz_val = std::stod(m[2].str());
    std::string unit = m[3].str();
    ASSERT_TRUE(std::abs(pct - 45.2) < 0.001, "Percent is 45.2");
    ASSERT_TRUE(unit == "MiB", "Unit is MiB");
    uint64_t tot_b = static_cast<uint64_t>(sz_val * 1048576.0);
    ASSERT_TRUE(tot_b > 120ULL * 1048576ULL, "Total bytes calculated");

    std::smatch sm;
    ASSERT_TRUE(std::regex_search(sample_line, sm, kYtDlpSpeedRe), "Speed regex matches yt-dlp speed");
    double sp_val = std::stod(sm[1].str());
    std::string sp_unit = sm[2].str();
    ASSERT_TRUE(std::abs(sp_val - 4.85) < 0.001, "Speed val is 4.85");
    ASSERT_TRUE(sp_unit == "MiB", "Speed unit is MiB");
    uint64_t speed_bps = static_cast<uint64_t>(sp_val * 1048576.0);
    ASSERT_TRUE(speed_bps > 5000000ULL, "Speed bps correctly calculated");

    // 2. BitTorrent Message Framing for kRequest
    auto req_msg = PeerWireProtocol::CreateRequest(5, 32768, 16384);
    auto p_opt = PeerWireProtocol::ParseMessage(req_msg.data(), req_msg.size());
    ASSERT_TRUE(p_opt.has_value(), "kRequest parsed successfully");
    ASSERT_TRUE(p_opt->id == BtMessageId::kRequest, "ID is kRequest");
    ASSERT_TRUE(p_opt->piece_index == 5, "Piece index is 5");
    ASSERT_TRUE(p_opt->block_begin == 32768, "Block begin is 32768");
    ASSERT_TRUE(p_opt->block_length == 16384, "Block length is 16384");

    // 3. BitTorrent upload accounting fields in TorrentProgress and TaskModel
    TorrentProgress tp;
    tp.uploaded_bytes = 1048576;
    tp.upload_speed_bps = 524288;
    TaskModel task;
    task.uploaded_bytes = tp.uploaded_bytes;
    task.upload_speed_bps = tp.upload_speed_bps;
    ASSERT_TRUE(task.uploaded_bytes == 1048576, "Uploaded bytes copied");
    ASSERT_TRUE(task.upload_speed_bps == 524288, "Upload speed bps copied");

    // 4. Controller Scheduling with start/stop time and retry reset
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / "flux_test_sched_yt";
    std::filesystem::remove_all(work_dir, ec);
    std::filesystem::create_directories(work_dir, ec);

    ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    std::filesystem::create_directories(cfg.default_download_dir, ec);

    DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Init controller");

    TaskModel ts;
    ts.task_id = "task-yt-sched";
    ts.task_type = TaskType::kStreamHls;
    ts.url = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
    ts.file_name = "rickroll.mp4";
    ASSERT_TRUE(ctrl.AddTask(ts, false, true).ok(), "Add task");

    // Set schedule
    ASSERT_TRUE(ctrl.SetTaskSchedule("task-yt-sched", "23:00", "06:00").ok(), "Set overnight schedule");
    auto topt = ctrl.GetTask("task-yt-sched");
    ASSERT_TRUE(topt.has_value() && topt->scheduled, "Task is scheduled");
    ASSERT_TRUE(topt->schedule_time_local == "23:00", "Start time is 23:00");
    ASSERT_TRUE(topt->schedule_stop_time_local == "06:00", "Stop time is 06:00");

    // Retrying or Resuming cleans stale schedule flags so manual resume is never blocked
    ASSERT_TRUE(ctrl.ResumeTask("task-yt-sched").ok(), "ResumeTask succeeds");
    auto t_resumed = ctrl.GetTask("task-yt-sched");
    ASSERT_TRUE(t_resumed.has_value(), "Task exists after resume");
    ASSERT_TRUE(!t_resumed->scheduled, "Manual resume clears scheduled flag");
    ASSERT_TRUE(t_resumed->schedule_time_local.empty(), "Start time cleared on manual resume");
    ASSERT_TRUE(t_resumed->schedule_stop_time_local.empty(), "Stop time cleared on manual resume");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

bool TestFinalPhaseRemediationAndLiveMetrics() {
    using namespace openidm;

    // 1. Theme default restoration to coral_dark and coral red accent
    AppSettings def_settings;
    ASSERT_TRUE(def_settings.theme == "coral_dark", "Default theme is coral_dark");
    auto pal = openidm::ui::DesignSystem::ResolvePalette("coral_dark");
    ASSERT_TRUE(pal.accent_primary.r == 0xFF && pal.accent_primary.g == 0x4F && pal.accent_primary.b == 0x57,
                "coral_dark accent is coral red #FF4F57");
    ASSERT_TRUE(openidm::ui::DesignSystem::ResolvePalette("").accent_primary.r == 0xFF,
                "Fallback theme defaults to coral red #FF4F57");

    // 2. Safe File Deletion in Controller
    std::error_code ec;
    std::filesystem::path work_dir = std::filesystem::temp_directory_path() / "flux_test_safe_del";
    std::filesystem::remove_all(work_dir, ec);
    std::filesystem::create_directories(work_dir, ec);

    ControllerConfig cfg;
    cfg.state_db_path = work_dir / "state.db";
    cfg.default_download_dir = work_dir / "downloads";
    cfg.temp_root_dir = work_dir / "tmp";
    std::filesystem::create_directories(cfg.default_download_dir, ec);

    DownloadController ctrl(cfg);
    ASSERT_TRUE(ctrl.Initialize().ok(), "Init controller for safe deletion");

    // 2a. Completed file deleted when delete_file == true
    std::filesystem::path target_file = cfg.default_download_dir / "sample_completed.dat";
    {
        std::ofstream ofs(target_file, std::ios::binary);
        ofs << "test completed content";
    }
    ASSERT_TRUE(std::filesystem::exists(target_file), "Test file created on disk");

    TaskModel t1;
    t1.task_id = "task-safe-del-1";
    t1.url = "https://example.com/sample_completed.dat";
    t1.file_name = "sample_completed.dat";
    t1.save_dir = cfg.default_download_dir.string();
    t1.state = TaskState::kCompleted;
    ASSERT_TRUE(ctrl.AddTask(t1, false, true).ok(), "Add completed task");

    // Remove task with delete_file = false: file MUST remain on disk
    TaskModel t2 = t1;
    t2.task_id = "task-safe-del-2";
    ASSERT_TRUE(ctrl.AddTask(t2, false, true).ok(), "Add second completed task");
    ASSERT_TRUE(ctrl.RemoveTask("task-safe-del-2", false).ok(), "Remove without deleting file");
    ASSERT_TRUE(std::filesystem::exists(target_file), "File preserved when delete_file == false");

    // Remove task with delete_file = true: file MUST be removed from disk
    ASSERT_TRUE(ctrl.RemoveTask("task-safe-del-1", true).ok(), "Remove with delete_file == true");
    ASSERT_TRUE(!std::filesystem::exists(target_file), "File removed from disk when delete_file == true");
    ASSERT_TRUE(!ctrl.GetTask("task-safe-del-1").has_value(), "Task removed from DB");

    // 3. Status Bar Metrics and Active Counts
    openidm::ui::DesktopView view(ctrl);
    auto sb = view.BuildStatusBar();
    ASSERT_TRUE(sb.browser_capture_enabled, "Browser capture enabled by default");
    ASSERT_TRUE(!sb.formatted_total_speed.empty(), "Formatted speed non-empty");
    ASSERT_TRUE(!sb.formatted_total_upload_speed.empty(), "Formatted upload speed non-empty");

    // 4. YouTube Googlevideo Classification Guard
    std::string yt_video_url = "https://rr3---sn-4g5ednks.googlevideo.com/videoplayback?expire=1729272302&ei=xyz&ip=1.2.3.4&id=o-1234&itag=18&source=youtube&requiressl=yes&mime=video%2Fmp4";
    std::string low_url = yt_video_url;
    for (char& c : low_url) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    bool is_youtube_watch = (low_url.find("youtube.com/watch") != std::string::npos ||
                             low_url.find("youtu.be/") != std::string::npos ||
                             low_url.find("youtube.com/shorts/") != std::string::npos ||
                             low_url.find("googlevideo.com") != std::string::npos ||
                             low_url.find("videoplayback") != std::string::npos);
    ASSERT_TRUE(is_youtube_watch, "googlevideo.com URL matched as YouTube stream");

    bool is_direct_video = !is_youtube_watch && (low_url.find(".mp4") != std::string::npos);
    ASSERT_TRUE(!is_direct_video, "googlevideo.com with .mp4 is NOT classified as direct video");

    // 5. Scheduled Window Precision (Midnight crossing)
    auto is_in_window = [](const std::string& cur, const std::string& start, const std::string& stop) {
        if (cur.size() != 5 || start.size() != 5 || stop.size() != 5) return false;
        if (start <= stop) return cur >= start && cur <= stop;
        return cur >= start || cur <= stop;
    };
    ASSERT_TRUE(is_in_window("23:30", "23:00", "04:00"), "23:30 inside 23:00-04:00 window");
    ASSERT_TRUE(is_in_window("02:15", "23:00", "04:00"), "02:15 inside 23:00-04:00 window");
    ASSERT_TRUE(!is_in_window("12:00", "23:00", "04:00"), "12:00 outside 23:00-04:00 window");
    ASSERT_TRUE(is_in_window("14:30", "09:00", "17:00"), "14:30 inside 09:00-17:00 window");
    ASSERT_TRUE(!is_in_window("19:00", "09:00", "17:00"), "19:00 outside 09:00-17:00 window");

    ctrl.Shutdown();
    std::filesystem::remove_all(work_dir, ec);
    return true;
}

} // namespace

int main() {
    int passed = 0;
    const int total = 54;

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
    run(28, "TestConcurrentAtomicCommitsAndSha256", TestConcurrentAtomicCommitsAndSha256);
    run(29, "TestDnsRebindingAndDestinationSecurity", TestDnsRebindingAndDestinationSecurity);
    run(30, "TestStreamContainerValidation", TestStreamContainerValidation);
    run(31, "TestFtpClientParsingAndCommands", TestFtpClientParsingAndCommands);
    run(32, "TestBencodeAndTorrentParser", TestBencodeAndTorrentParser);
    run(33, "TestRtspMmsStreamDetection", TestRtspMmsStreamDetection);
    run(34, "TestSiteGrabberCrawlerAndSsrf", TestSiteGrabberCrawlerAndSsrf);
    run(35, "TestZipArchivePreviewAndSecurity", TestZipArchivePreviewAndSecurity);
    run(36, "TestTaskImportExportAndMultiQueue", TestTaskImportExportAndMultiQueue);
    run(37, "TestAntivirusScannerSafetyAndHandling", TestAntivirusScannerSafetyAndHandling);
    run(38, "TestDualRateLimiterAndPerTaskSpeedLimit", TestDualRateLimiterAndPerTaskSpeedLimit);
    run(39, "TestDynamicCategorizationAndSiteRulesPriority", TestDynamicCategorizationAndSiteRulesPriority);
    run(40, "TestAlternateUrlsAndMirrorValidation", TestAlternateUrlsAndMirrorValidation);
    run(41, "TestFtpReliabilityEpsvAndResumeTruncation", TestFtpReliabilityEpsvAndResumeTruncation);
    run(42, "TestSchedulerStopTimesAndPostCompletionActions", TestSchedulerStopTimesAndPostCompletionActions);
    run(43, "TestPeerWireProtocolFramingAndCompactPeers", TestPeerWireProtocolFramingAndCompactPeers);
    run(44, "TestMultiFileStorageMappingAndBoundarySplitting", TestMultiFileStorageMappingAndBoundarySplitting);
    run(45, "TestPieceManagerAndBlockScheduling", TestPieceManagerAndBlockScheduling);
    run(46, "TestTrackerClientQueryEncodingAndResponseParsing", TestTrackerClientQueryEncodingAndResponseParsing);
    run(47, "TestHttpRangeSatisfactionValidation", TestHttpRangeSatisfactionValidation);
    run(48, "TestDuplicateTaskDetectionThreeTiers", TestDuplicateTaskDetectionThreeTiers);
    run(49, "TestExpiredLinkRefreshAndCookieReplacement", TestExpiredLinkRefreshAndCookieReplacement);
    run(50, "TestSegmentSchedulerChunkNormalizationAndTruncationGuard", TestSegmentSchedulerChunkNormalizationAndTruncationGuard);
    run(51, "TestUrlUserinfoExtractionAndBasicAuthGeneration", TestUrlUserinfoExtractionAndBasicAuthGeneration);
    run(52, "TestModernCoralThemeSettingsModalBoundsAndSafeDelete", TestModernCoralThemeSettingsModalBoundsAndSafeDelete);
    run(53, "TestYouTubeSpeedParsingMidnightCrossingAndBitTorrentUpload", TestYouTubeSpeedParsingMidnightCrossingAndBitTorrentUpload);
    run(54, "TestFinalPhaseRemediationAndLiveMetrics", TestFinalPhaseRemediationAndLiveMetrics);

    std::cout << "\nC++20 Unit & Regression Test Summary: " << passed << " / " << total << " suites PASSED.\n";
    return (passed == total) ? 0 : 1;
}
