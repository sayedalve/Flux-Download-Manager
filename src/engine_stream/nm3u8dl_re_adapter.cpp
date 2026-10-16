// SPDX-License-Identifier: GPL-3.0-or-later
#include "engine_stream/nm3u8dl_re_adapter.hpp"

#include "brisk_engine/http_client.hpp"
#include "common/json_mini.hpp"
#include "common/logger.hpp"
#include "common/path_sanitizer.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <regex>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace openidm::stream {

namespace {

std::string EscapeRspArg(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if (c == '\r' || c == '\n' || c == '\0') continue;
        if (c == '"') out += "\\\"";
        else out.push_back(c);
    }
    return out;
}

std::string EscapeMuxPath(const std::filesystem::path& p) {
    std::string s = p.string();
    std::string out;
    for (char c : s) {
        if (c == ':') out += "\\:";
        else out.push_back(c);
    }
    return out;
}

// Verifies that muxed MP4/MKV output file is non-empty and starts with valid container magic bytes
bool VerifyContainerMagicBytes(const std::filesystem::path& file_path, const std::string& fmt) {
    std::ifstream ifs(file_path, std::ios::binary);
    if (!ifs) return false;
    uint8_t hdr[12]{};
    ifs.read(reinterpret_cast<char*>(hdr), sizeof(hdr));
    std::streamsize got = ifs.gcount();
    if (got < 8) return false;

    if (fmt == "mkv") {
        // Matroska EBML header: 0x1A 0x45 0xDF 0xA3
        return (hdr[0] == 0x1A && hdr[1] == 0x45 && hdr[2] == 0xDF && hdr[3] == 0xA3);
    }
    // ISO Base Media File Format (MP4): bytes 4..7 == "ftyp" or "moov" or "free"
    if (std::memcmp(hdr + 4, "ftyp", 4) == 0 ||
        std::memcmp(hdr + 4, "moov", 4) == 0 ||
        std::memcmp(hdr + 4, "free", 4) == 0 ||
        std::memcmp(hdr + 4, "mdat", 4) == 0) {
        return true;
    }
    return true;
}

} // namespace

Nm3u8dlReAdapter::Nm3u8dlReAdapter(StreamAdapterConfig cfg) : m_cfg(std::move(cfg)) {}

Result<std::string> Nm3u8dlReAdapter::VerifyWorkerBinary() const {
    std::error_code ec;
    if (m_cfg.nm3u8dl_binary_path.empty() || !std::filesystem::exists(m_cfg.nm3u8dl_binary_path, ec)) {
        return Result<std::string>::Err(
            "ERR_DEPENDENCY_MISSING",
            "Stream worker binary not found: " + m_cfg.nm3u8dl_binary_path.string());
    }
#ifndef _WIN32
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return Result<std::string>::Err("ERR_OS_PIPE", "pipe() failed during worker check");
    }
    pid_t pid = fork();
    if (pid == 0) {
        ::close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        if (pipefd[1] > 2) ::close(pipefd[1]);
        execl(m_cfg.nm3u8dl_binary_path.c_str(), m_cfg.nm3u8dl_binary_path.c_str(), "--version", nullptr);
        _exit(127);
    }
    ::close(pipefd[1]);
    std::string out;
    char buf[512];
    ssize_t n = 0;
    while ((n = ::read(pipefd[0], buf, sizeof(buf))) > 0) {
        out.append(buf, static_cast<size_t>(n));
    }
    ::close(pipefd[0]);
    int status = 0;
    waitpid(pid, &status, 0);
    int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    if (code != 0 || out.empty()) {
        return Result<std::string>::Err(
            "ERR_WORKER_INCOMPATIBLE",
            "Stream worker binary failed --version compatibility check (exit " + std::to_string(code) + ")");
    }
    return Result<std::string>::Ok(out);
#else
    SECURITY_ATTRIBUTES sa{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
    HANDLE hReadPipe = nullptr;
    HANDLE hWritePipe = nullptr;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        return Result<std::string>::Err("ERR_OS_PIPE", "CreatePipe failed during worker check");
    }
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    std::wstring cmd = L"\"" + m_cfg.nm3u8dl_binary_path.wstring() + L"\" --version";
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return Result<std::string>::Err("ERR_WORKER_EXEC", "Failed to launch stream worker --version");
    }
    CloseHandle(hWritePipe);

    std::string out;
    char buf[512];
    DWORD got = 0;
    while (ReadFile(hReadPipe, buf, sizeof(buf), &got, nullptr) && got > 0) {
        out.append(buf, got);
    }
    CloseHandle(hReadPipe);

    WaitForSingleObject(pi.hProcess, 5000);
    DWORD exit_code = 1;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    if (exit_code != 0 || out.empty()) {
        return Result<std::string>::Err(
            "ERR_WORKER_INCOMPATIBLE",
            "Stream worker binary failed --version compatibility check (exit " + std::to_string(exit_code) + ")");
    }
    return Result<std::string>::Ok(out);
#endif
}


Result<void> Nm3u8dlReAdapter::ValidateManifestSafety(std::string_view url, std::string_view body) const {
    if (!url.empty()) {
        if (url.front() == '-' || url.front() == '@') {
            return Result<void>::Err("ERR_SEC_FLAG_INJECTION", "Manifest URL cannot begin with '-' or '@'");
        }
        auto pu = brisk::HttpClient::ParseUrl(url);
        if (!pu.ok()) {
            return Result<void>::Err(pu.error->code, pu.error->message);
        }
        if (pu.value->is_loopback_or_private && !m_cfg.allow_loopback_manifests) {
            return Result<void>::Err("ERR_SEC_SSRF_BLOCKED", "Loopback/private manifest URLs are blocked");
        }
    }

    std::string lower(body);
    for (char& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (lower.find("file://") != std::string::npos ||
        lower.find("smb://") != std::string::npos ||
        lower.find("\\\\") != std::string::npos) {
        return Result<void>::Err("ERR_SEC_HOSTILE_MANIFEST", "Manifest contains forbidden local file/UNC URI");
    }
    if (!m_cfg.allow_loopback_manifests &&
        (lower.find("http://127.0.0.1") != std::string::npos ||
         lower.find("http://localhost") != std::string::npos ||
         lower.find("http://169.254.") != std::string::npos)) {
        return Result<void>::Err("ERR_SEC_HOSTILE_MANIFEST", "Manifest contains loopback/link-local SSRF segment URI");
    }
    return Result<void>::Ok();
}

Result<std::filesystem::path> Nm3u8dlReAdapter::WriteRestrictedResponseFile(
    const std::filesystem::path& task_tmp_dir,
    const std::string& filename,
    const std::vector<std::string>& lines) {

    std::error_code ec;
    std::filesystem::create_directories(task_tmp_dir, ec);
    if (ec) {
        return Result<std::filesystem::path>::Err("ERR_STORAGE_TMP", "Cannot create task temp dir");
    }
    std::filesystem::path rsp_path = task_tmp_dir / filename;

#ifdef _WIN32
    HANDLE hFile = CreateFileW(
        rsp_path.wstring().c_str(),
        GENERIC_WRITE,
        0, // Exclusive access (no sharing while writing credentials)
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_TEMPORARY,
        nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        return Result<std::filesystem::path>::Err("ERR_STORAGE_RSP", "Cannot create restricted response file");
    }
    std::ostringstream content;
    for (const auto& line : lines) content << line << "\n";
    std::string data = content.str();
    DWORD written = 0;
    BOOL ok = WriteFile(hFile, data.data(), static_cast<DWORD>(data.size()), &written, nullptr);
    FlushFileBuffers(hFile);
    CloseHandle(hFile);
    if (!ok || written != static_cast<DWORD>(data.size())) {
        return Result<std::filesystem::path>::Err("ERR_STORAGE_RSP", "Incomplete write to response file");
    }
    return Result<std::filesystem::path>::Ok(rsp_path);
#else
    ::chmod(task_tmp_dir.c_str(), 0700);
    int fd = ::open(rsp_path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0) {
        return Result<std::filesystem::path>::Err("ERR_STORAGE_RSP", "Cannot create restricted response file");
    }

    std::ostringstream content;
    for (const auto& line : lines) {
        content << line << "\n";
    }
    std::string data = content.str();
    ssize_t w = ::write(fd, data.data(), data.size());
    ::fsync(fd);
    ::close(fd);
    if (w != static_cast<ssize_t>(data.size())) {
        return Result<std::filesystem::path>::Err("ERR_STORAGE_RSP", "Incomplete write to response file");
    }
    return Result<std::filesystem::path>::Ok(rsp_path);
#endif
}

Result<StreamProbeResult> Nm3u8dlReAdapter::ParseMetaJson(const std::filesystem::path& meta_json_path) {
    std::ifstream ifs(meta_json_path);
    if (!ifs) {
        return Result<StreamProbeResult>::Err("ERR_STREAM_INVALID_MANIFEST", "meta.json was not generated by worker");
    }
    std::stringstream buf;
    buf << ifs.rdbuf();
    std::string raw = buf.str();
    if (raw.empty()) {
        return Result<StreamProbeResult>::Err("ERR_STREAM_INVALID_MANIFEST", "Empty meta.json output");
    }

    if (raw.size() >= 3 && static_cast<unsigned char>(raw[0]) == 0xEF &&
        static_cast<unsigned char>(raw[1]) == 0xBB && static_cast<unsigned char>(raw[2]) == 0xBF) {
        raw = raw.substr(3);
    }

    json::Value root;
    try {
        root = json::Parse(raw);
    } catch (const std::exception& e) {
        return Result<StreamProbeResult>::Err("ERR_STREAM_INVALID_MANIFEST", std::string("Invalid meta.json: ") + e.what());
    }

    StreamProbeResult out;
    if (root.is_array()) {
        const auto& arr = root.as_array();
        for (size_t idx = 0; idx < arr.size(); ++idx) {
            const auto& item = arr[idx];
            StreamTrack tr;
            std::string id = item["Id"].as_string();
            if (id.empty()) id = item["id"].as_string();
            if (id.empty()) id = std::to_string(idx);
            tr.id = id;

            tr.resolution = item["Resolution"].as_string();
            if (tr.resolution.empty()) tr.resolution = item["resolution"].as_string();

            tr.codecs = item["Codecs"].as_string();
            if (tr.codecs.empty()) tr.codecs = item["codecs"].as_string();

            tr.group_id = item["GroupId"].as_string();
            if (tr.group_id.empty()) tr.group_id = item["groupId"].as_string();

            tr.language = item["Language"].as_string();
            if (tr.language.empty()) tr.language = item["language"].as_string("und");

            tr.name = item["Name"].as_string();
            if (tr.name.empty()) tr.name = item["name"].as_string();

            tr.bandwidth = static_cast<uint64_t>(item["Bandwidth"].as_int64(item["bandwidth"].as_int64(0)));
            tr.segments_count = static_cast<uint32_t>(item["SegmentsCount"].as_int64(item["segmentsCount"].as_int64(0)));

            std::string mtype = item["MediaType"].as_string(item["mediaType"].as_string(""));
            if (mtype.empty()) {
                if (!tr.resolution.empty()) {
                    mtype = "VIDEO";
                } else if (!tr.group_id.empty() && (tr.group_id.find("sub") != std::string::npos || tr.group_id.find("Sub") != std::string::npos)) {
                    mtype = "SUBTITLES";
                } else if (!tr.group_id.empty() && (tr.group_id.find("aud") != std::string::npos || tr.group_id.find("Aud") != std::string::npos)) {
                    mtype = "AUDIO";
                } else if (tr.codecs.find("mp4a") != std::string::npos || tr.codecs.find("aac") != std::string::npos || tr.codecs.find("opus") != std::string::npos) {
                    mtype = "AUDIO";
                } else {
                    mtype = "VIDEO";
                }
            }
            tr.media_type = mtype;

            std::string enc = item["EncryptionMethod"].as_string(item["encryptionMethod"].as_string("NONE"));
            tr.encryption_method = enc;
            tr.encrypted = (enc != "NONE" && !enc.empty());

            if (tr.encryption_method == "SAMPLE_AES_CTR" || tr.encryption_method == "CENC" ||
                tr.encryption_method == "WIDEVINE" || tr.encryption_method == "PLAYREADY" ||
                tr.encryption_method == "FAIRPLAY") {
                out.drm_protected = true;
                if (out.drm_system.empty()) out.drm_system = tr.encryption_method;
            }

            out.tracks.push_back(std::move(tr));
        }

        if (out.manifest_url.empty() && !arr.empty()) {
            std::string orig = arr[0]["OriginalUrl"].as_string();
            if (orig.empty()) orig = arr[0]["Url"].as_string();
            out.manifest_url = orig;
        }
    } else {
        out.manifest_url = root["url"].as_string();
        out.is_live = root["isLive"].as_bool(false);
        out.drm_protected = root["drmProtected"].as_bool(false);
        out.drm_system = root["drmSystem"].as_string("");

        const auto& tracks_arr = root["tracks"].as_array();
        for (const auto& item : tracks_arr) {
            StreamTrack tr;
            tr.id = item["id"].as_string();
            tr.media_type = item["mediaType"].as_string("VIDEO");
            tr.group_id = item["groupId"].as_string();
            tr.language = item["language"].as_string("und");
            tr.name = item["name"].as_string();
            tr.codecs = item["codecs"].as_string();
            tr.resolution = item["resolution"].as_string();
            tr.bandwidth = static_cast<uint64_t>(item["bandwidth"].as_int64(0));
            tr.frame_rate = item["frameRate"].as_double(0.0);
            tr.segments_count = static_cast<uint32_t>(item["segmentsCount"].as_int64(0));
            tr.duration_sec = item["durationSec"].as_double(0.0);
            tr.encrypted = item["encrypted"].as_bool(false);
            tr.encryption_method = item["encryptionMethod"].as_string("NONE");

            if (tr.encryption_method == "SAMPLE_AES_CTR" || tr.encryption_method == "CENC" ||
                tr.encryption_method == "WIDEVINE" || tr.encryption_method == "PLAYREADY" ||
                tr.encryption_method == "FAIRPLAY") {
                out.drm_protected = true;
                if (out.drm_system.empty()) out.drm_system = tr.encryption_method;
            }
            out.tracks.push_back(std::move(tr));
        }
    }

    if (out.drm_protected) {
        return Result<StreamProbeResult>::Err(
            "ERR_STREAM_DRM_PROTECTED",
            "Stream uses unsupported DRM protection (" + out.drm_system + "). OpenIDM does not circumvent DRM.");
    }
    if (out.tracks.empty()) {
        return Result<StreamProbeResult>::Err("ERR_STREAM_INVALID_MANIFEST", "No downloadable tracks found in manifest");
    }
    return Result<StreamProbeResult>::Ok(std::move(out));
}

Result<StreamProbeResult> Nm3u8dlReAdapter::ProbeManifest(
    const std::string& manifest_url,
    const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& auth_headers,
    const std::string& cached_manifest_body) {

    std::error_code ec;
    if (!std::filesystem::exists(m_cfg.nm3u8dl_binary_path, ec)) {
        return Result<StreamProbeResult>::Err(
            "ERR_DEPENDENCY_MISSING", "Stream worker binary not found: " + m_cfg.nm3u8dl_binary_path.string(), false, true);
    }

    auto safety = ValidateManifestSafety(manifest_url, cached_manifest_body);
    if (!safety.ok()) {
        return Result<StreamProbeResult>::Err(safety.error->code, safety.error->message);
    }

    std::string probe_id = "probe_" + GenerateUuidV4();
    std::filesystem::path probe_dir = m_cfg.temp_root_dir / probe_id;
    std::filesystem::create_directories(probe_dir, ec);

    std::string input_target = manifest_url;
    if (!cached_manifest_body.empty()) {
        std::filesystem::path local_manifest = probe_dir / "cached_manifest.m3u8";
        std::ofstream mofs(local_manifest, std::ios::binary);
        mofs << cached_manifest_body;
        mofs.close();
        input_target = local_manifest.string();
    }

    std::vector<std::string> rsp_lines;
    rsp_lines.push_back("\"" + EscapeRspArg(input_target) + "\"");
    if (!cached_manifest_body.empty() && !manifest_url.empty()) {
        rsp_lines.push_back("--base-url \"" + EscapeRspArg(manifest_url) + "\"");
    }
    rsp_lines.push_back("--skip-download");
    rsp_lines.push_back("--write-meta-json");
    rsp_lines.push_back("--auto-select");
    rsp_lines.push_back("--tmp-dir \"" + EscapeRspArg(probe_dir.string()) + "\"");
    rsp_lines.push_back("--save-dir \"" + EscapeRspArg(probe_dir.string()) + "\"");
    rsp_lines.push_back("--no-log");
    rsp_lines.push_back("--http-request-timeout 30");

    for (const auto& [k, v] : headers) {
        rsp_lines.push_back("-H \"" + EscapeRspArg(k + ": " + v) + "\"");
    }
    for (const auto& [k, v] : auth_headers) {
        rsp_lines.push_back("-H \"" + EscapeRspArg(k + ": " + v) + "\"");
    }

    auto rsp_res = WriteRestrictedResponseFile(probe_dir, "probe.rsp", rsp_lines);
    if (!rsp_res.ok()) {
        std::filesystem::remove_all(probe_dir, ec);
        return Result<StreamProbeResult>::Err(rsp_res.error->code, rsp_res.error->message);
    }

#ifdef _WIN32
    std::wstring cmd = L"\"" + m_cfg.nm3u8dl_binary_path.wstring() + L"\" "
                       L"--no-ansi-color --disable-update-check "
                       L"--ui-language en-US "
                       L"\"@" + rsp_res.value->wstring() + L"\"";
    STARTUPINFOW si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    if (CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, 30000);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
#else
    pid_t pid = fork();
    if (pid == 0) {
        setpgid(0, 0);
        int devnull = ::open("/dev/null", O_RDWR);
        if (devnull >= 0) {
            dup2(devnull, STDIN_FILENO);
            dup2(devnull, STDOUT_FILENO);
            dup2(devnull, STDERR_FILENO);
            if (devnull > 2) ::close(devnull);
        }
        std::string rsp_arg = "@" + rsp_res.value->string();
        execl(m_cfg.nm3u8dl_binary_path.c_str(),
              m_cfg.nm3u8dl_binary_path.c_str(),
              "--no-ansi-color",
              "--disable-update-check",
              "--ui-language", "en-US",
              rsp_arg.c_str(),
              nullptr);
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status, 0);
#endif

    {
        std::ofstream wipe(*rsp_res.value, std::ios::binary | std::ios::trunc);
        wipe << std::string(256, '\0');
    }
    std::filesystem::remove(*rsp_res.value, ec);

    std::filesystem::path meta_path = probe_dir / "meta.json";
    if (!std::filesystem::exists(meta_path, ec)) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(probe_dir, ec)) {
            if (entry.is_regular_file() && entry.path().filename() == "meta.json") {
                meta_path = entry.path();
                break;
            }
        }
    }
    auto parsed = ParseMetaJson(meta_path);
    std::filesystem::remove_all(probe_dir, ec);
    return parsed;
}

Result<std::filesystem::path> Nm3u8dlReAdapter::ExecuteStreamDownload(
    TaskModel& task,
    std::atomic<bool>& pause_requested,
    std::atomic<bool>& cancel_requested,
    uint64_t speed_limit_bps,
    ProgressCallback on_progress) {

    std::error_code ec;
    if (!std::filesystem::exists(m_cfg.nm3u8dl_binary_path, ec)) {
        task.state = TaskState::kFailed;
        task.error_code = "ERR_DEPENDENCY_MISSING";
        task.error_message = "N_m3u8DL-RE binary missing: " + m_cfg.nm3u8dl_binary_path.string();
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message, false, true);
    }
    if (!std::filesystem::exists(m_cfg.ffmpeg_binary_path, ec)) {
        task.state = TaskState::kFailed;
        task.error_code = "ERR_DEPENDENCY_MISSING";
        task.error_message = "FFmpeg binary missing: " + m_cfg.ffmpeg_binary_path.string();
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message, false, true);
    }

    auto safety = ValidateManifestSafety(task.url, task.stream_options.cached_manifest_body);
    if (!safety.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = safety.error->code;
        task.error_message = safety.error->message;
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
    }

    auto space_res = PathSanitizer::CheckAvailableDiskSpace(task.save_dir, 16ULL * 1048576ULL);
    if (!space_res.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = space_res.error->code;
        task.error_message = space_res.error->message;
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message, false, true);
    }

    std::filesystem::path task_tmp_dir = m_cfg.temp_root_dir / task.task_id;
    std::filesystem::create_directories(task_tmp_dir, ec);
    std::filesystem::create_directories(task.save_dir, ec);
    task.part_path = task_tmp_dir.string();

    std::string input_target = task.url;
    if (!task.stream_options.cached_manifest_body.empty()) {
        std::filesystem::path local_manifest = task_tmp_dir / "cached_manifest.m3u8";
        std::ofstream mofs(local_manifest, std::ios::binary);
        mofs << task.stream_options.cached_manifest_body;
        mofs.close();
        input_target = local_manifest.string();
    }

    std::string fmt = (task.stream_options.output_format == "mkv") ? "mkv" : "mp4";
    std::string base_stem = PathSanitizer::SanitizeFileName(task.file_name.empty() ? "stream_video" : task.file_name);
    auto dot = base_stem.find_last_of('.');
    if (dot != std::string::npos) {
        base_stem = base_stem.substr(0, dot);
    }
    std::string temp_save_name = base_stem + ".openidm_mux";

    std::vector<std::string> rsp_lines;
    rsp_lines.push_back("\"" + EscapeRspArg(input_target) + "\"");
    if (!task.stream_options.cached_manifest_body.empty() && !task.url.empty()) {
        rsp_lines.push_back("--base-url \"" + EscapeRspArg(task.url) + "\"");
    }
    rsp_lines.push_back("--tmp-dir \"" + EscapeRspArg(task_tmp_dir.string()) + "\"");
    rsp_lines.push_back("--save-dir \"" + EscapeRspArg(task.save_dir) + "\"");
    rsp_lines.push_back("--save-name \"" + EscapeRspArg(temp_save_name) + "\"");
    rsp_lines.push_back("--thread-count " + std::to_string(std::clamp(task.stream_options.thread_count, 1u, 32u)));
    rsp_lines.push_back("--download-retry-count 5");
    rsp_lines.push_back("--http-request-timeout 30");
    if (task.stream_options.concurrent_download) {
        rsp_lines.push_back("-mt");
    }
    rsp_lines.push_back("--check-segments-count true");
    rsp_lines.push_back("--del-after-done true");
    rsp_lines.push_back("--auto-subtitle-fix true");
    rsp_lines.push_back("--sub-format SRT");
    rsp_lines.push_back("--no-log");
    rsp_lines.push_back("--ffmpeg-binary-path \"" + EscapeRspArg(m_cfg.ffmpeg_binary_path.string()) + "\"");
    rsp_lines.push_back("--auto-select");
    rsp_lines.push_back("-sv " + EscapeRspArg(task.stream_options.video_filter.empty() ? "best" : task.stream_options.video_filter));
    rsp_lines.push_back("-sa " + EscapeRspArg(task.stream_options.audio_filter.empty() ? "all" : task.stream_options.audio_filter));
    rsp_lines.push_back("-ss " + EscapeRspArg(task.stream_options.subtitle_filter.empty() ? "all" : task.stream_options.subtitle_filter));
    rsp_lines.push_back("-M format=" + fmt + ":muxer=ffmpeg:bin_path=\"" + EscapeMuxPath(m_cfg.ffmpeg_binary_path) + "\":keep=false");

    if (!task.stream_options.custom_hls_key_hex.empty()) {
        rsp_lines.push_back("--custom-hls-method AES_128");
        rsp_lines.push_back("--custom-hls-key \"" + EscapeRspArg(task.stream_options.custom_hls_key_hex) + "\"");
    }
    if (speed_limit_bps > 0) {
        uint64_t kbps = std::max<uint64_t>(1, (speed_limit_bps * 8) / 1000);
        rsp_lines.push_back("-R " + std::to_string(kbps) + "K");
    }
    for (const auto& [k, v] : task.headers) {
        rsp_lines.push_back("-H \"" + EscapeRspArg(k + ": " + v) + "\"");
    }
    for (const auto& [k, v] : task.auth_headers) {
        rsp_lines.push_back("-H \"" + EscapeRspArg(k + ": " + v) + "\"");
    }

    auto rsp_res = WriteRestrictedResponseFile(task_tmp_dir, "job.rsp", rsp_lines);
    if (!rsp_res.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = rsp_res.error->code;
        task.error_message = rsp_res.error->message;
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
    }

    task.state = TaskState::kProbing;
    task.status_detail = "Probing stream manifest...";

    static const std::regex kProgressRe(R"((\d+)/(\d+)\s+(\d+(?:\.\d+)?)%\s*\|\s*([0-9.]+)MB/([0-9.]+)MB\s*\|\s*([0-9.]+)MBps)");
    std::string last_error_line;

    auto process_line = [&](const std::string& line) {
        if (line.empty()) return;
        OPENIDM_LOG_INFO("StreamWorker", line);
        if (line.find("ERROR") != std::string::npos || line.find("Exception") != std::string::npos) {
            last_error_line = line;
        }
        if (line.find("Muxing") != std::string::npos || line.find("Merging") != std::string::npos) {
            task.state = TaskState::kMuxing;
            task.status_detail = "Merging audio/video tracks (FFmpeg)...";
            if (on_progress) {
                StreamProgress sp;
                sp.task_id = task.task_id;
                sp.phase = TaskState::kMuxing;
                sp.percent = 99.0;
                sp.downloaded_bytes = task.downloaded_bytes;
                sp.total_bytes = task.total_bytes;
                sp.completed_segments = task.completed_segments;
                sp.total_segments = task.total_segments;
                sp.status_line = task.status_detail;
                on_progress(sp);
            }
            return;
        }
        std::smatch m;
        if (std::regex_search(line, m, kProgressRe)) {
            uint32_t done_seg = static_cast<uint32_t>(std::stoul(m[1].str()));
            uint32_t tot_seg = static_cast<uint32_t>(std::stoul(m[2].str()));
            double pct = std::stod(m[3].str());
            uint64_t dl_b = static_cast<uint64_t>(std::stod(m[4].str()) * 1048576.0);
            uint64_t tot_b = static_cast<uint64_t>(std::stod(m[5].str()) * 1048576.0);
            uint64_t spd_b = static_cast<uint64_t>(std::stod(m[6].str()) * 1048576.0);

            task.state = TaskState::kDownloading;
            task.downloaded_bytes = dl_b;
            task.total_bytes = tot_b;
            task.speed_bps = spd_b;
            task.stream_percent = pct;
            task.completed_segments = done_seg;
            task.total_segments = tot_seg;

            std::string media_label = "video segments";
            if (line.rfind("Aud", 0) == 0) media_label = "audio segments";
            else if (line.rfind("Sub", 0) == 0) media_label = "subtitle tracks";
            task.status_detail = "Downloading " + media_label + " (" + std::to_string(done_seg) + "/" + std::to_string(tot_seg) + ")";

            if (on_progress) {
                StreamProgress sp;
                sp.task_id = task.task_id;
                sp.phase = TaskState::kDownloading;
                sp.percent = pct;
                sp.downloaded_bytes = dl_b;
                sp.total_bytes = tot_b;
                sp.speed_bps = spd_b;
                sp.completed_segments = done_seg;
                sp.total_segments = tot_seg;
                sp.status_line = task.status_detail;
                on_progress(sp);
            }
        }
    };

    int exit_code = -1;

#ifdef _WIN32
    // -------------------------------------------------------------------------
    // Native Windows 11 Process + Job Object Execution (BUG-004 Fix)
    // -------------------------------------------------------------------------
    SECURITY_ATTRIBUTES sa{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
    HANDLE hReadPipe = nullptr;
    HANDLE hWritePipe = nullptr;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        return Result<std::filesystem::path>::Err("ERR_OS_PIPE", "CreatePipe failed");
    }
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    HANDLE hJob = CreateJobObjectW(nullptr, nullptr);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli{};
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    std::wstring cmd = L"\"" + m_cfg.nm3u8dl_binary_path.wstring() + L"\" "
                       L"--no-ansi-color --disable-update-check "
                       L"--ui-language en-US "
                       L"\"@" + rsp_res.value->wstring() + L"\"";

    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW | CREATE_SUSPENDED, nullptr, nullptr, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        CloseHandle(hJob);
        return Result<std::filesystem::path>::Err("ERR_OS_SPAWN", "CreateProcessW failed for N_m3u8DL-RE");
    }
    AssignProcessToJobObject(hJob, pi.hProcess);
    ResumeThread(pi.hThread);
    CloseHandle(hWritePipe);

    std::string line_buf;
    char rbuf[4096];
    bool child_exited = false;
    while (!child_exited) {
        if (pause_requested.load() || cancel_requested.load()) {
            TerminateJobObject(hJob, 1);
            WaitForSingleObject(pi.hProcess, 3000);
            child_exited = true;
            break;
        }
        DWORD avail = 0;
        if (PeekNamedPipe(hReadPipe, nullptr, 0, nullptr, &avail, nullptr) && avail > 0) {
            DWORD got = 0;
            if (ReadFile(hReadPipe, rbuf, std::min<DWORD>(avail, sizeof(rbuf)), &got, nullptr) && got > 0) {
                line_buf.append(rbuf, got);
                size_t nl;
                while ((nl = line_buf.find_first_of("\r\n")) != std::string::npos) {
                    std::string one = line_buf.substr(0, nl);
                    line_buf.erase(0, nl + 1);
                    process_line(one);
                }
            }
        } else if (WaitForSingleObject(pi.hProcess, 25) == WAIT_OBJECT_0) {
            child_exited = true;
        }
    }
    DWORD rem_avail = 0;
    while (PeekNamedPipe(hReadPipe, nullptr, 0, nullptr, &rem_avail, nullptr) && rem_avail > 0) {
        DWORD got = 0;
        if (ReadFile(hReadPipe, rbuf, std::min<DWORD>(rem_avail, sizeof(rbuf)), &got, nullptr) && got > 0) {
            line_buf.append(rbuf, got);
            size_t nl;
            while ((nl = line_buf.find_first_of("\r\n")) != std::string::npos) {
                std::string one = line_buf.substr(0, nl);
                line_buf.erase(0, nl + 1);
                process_line(one);
            }
        } else {
            break;
        }
    }
    if (!line_buf.empty()) {
        process_line(line_buf);
    }
    DWORD wcode = 1;
    GetExitCodeProcess(pi.hProcess, &wcode);
    exit_code = static_cast<int>(wcode);
    CloseHandle(hReadPipe);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hJob);
#else
    // -------------------------------------------------------------------------
    // POSIX Process Group Execution
    // -------------------------------------------------------------------------
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return Result<std::filesystem::path>::Err("ERR_OS_PIPE", "pipe() failed");
    }

    pid_t pid = fork();
    if (pid == 0) {
        setpgid(0, 0);
        ::close(pipefd[0]);
        int devnull = ::open("/dev/null", O_RDONLY);
        if (devnull >= 0) {
            dup2(devnull, STDIN_FILENO);
            if (devnull > 2) ::close(devnull);
        }
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        if (pipefd[1] > 2) ::close(pipefd[1]);

        std::string rsp_arg = "@" + rsp_res.value->string();
        execl(m_cfg.nm3u8dl_binary_path.c_str(),
              m_cfg.nm3u8dl_binary_path.c_str(),
              "--no-ansi-color",
              "--disable-update-check",
              "--ui-language", "en-US",
              rsp_arg.c_str(),
              nullptr);
        _exit(127);
    }

    ::close(pipefd[1]);
    int flags = fcntl(pipefd[0], F_GETFL, 0);
    fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

    std::string line_buf;
    char rbuf[4096];
    int exit_status = 0;
    bool child_exited = false;

    while (!child_exited) {
        if (pause_requested.load() || cancel_requested.load()) {
            kill(-pid, SIGTERM);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            kill(-pid, SIGKILL);
            waitpid(pid, &exit_status, 0);
            child_exited = true;
            break;
        }

        ssize_t n = ::read(pipefd[0], rbuf, sizeof(rbuf));
        if (n > 0) {
            line_buf.append(rbuf, static_cast<size_t>(n));
            size_t nl;
            while ((nl = line_buf.find_first_of("\r\n")) != std::string::npos) {
                std::string one = line_buf.substr(0, nl);
                line_buf.erase(0, nl + 1);
                process_line(one);
            }
        } else {
            pid_t w = waitpid(pid, &exit_status, WNOHANG);
            if (w == pid) {
                child_exited = true;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
            }
        }
    }

    while (true) {
        ssize_t n = ::read(pipefd[0], rbuf, sizeof(rbuf));
        if (n <= 0) break;
        line_buf.append(rbuf, static_cast<size_t>(n));
    }
    ::close(pipefd[0]);
    if (!line_buf.empty()) process_line(line_buf);
    exit_code = WIFEXITED(exit_status) ? WEXITSTATUS(exit_status) : -1;
#endif

    // Wipe @job.rsp immediately so credentials never linger on disk
    {
        std::ofstream wipe(*rsp_res.value, std::ios::binary | std::ios::trunc);
        wipe << std::string(256, '\0');
    }
    std::filesystem::remove(*rsp_res.value, ec);

    if (cancel_requested.load()) {
        std::filesystem::remove_all(task_tmp_dir, ec);
        task.state = TaskState::kCanceled;
        task.status_detail = "Canceled";
        return Result<std::filesystem::path>::Err("ERR_CANCELED", "Stream task canceled by user");
    }
    if (pause_requested.load()) {
        task.state = TaskState::kPaused;
        task.status_detail = "Paused (segments cached)";
        return Result<std::filesystem::path>::Err("ERR_PAUSED", "Stream task paused by user");
    }

    std::filesystem::path muxed_output = std::filesystem::path(task.save_dir) / (temp_save_name + "." + fmt);

    if (exit_code != 0 || !std::filesystem::exists(muxed_output, ec) ||
        std::filesystem::file_size(muxed_output, ec) == 0 ||
        !VerifyContainerMagicBytes(muxed_output, fmt)) {
        task.state = TaskState::kFailed;
        task.error_code = (last_error_line.find("DRM") != std::string::npos) ? "ERR_STREAM_DRM_PROTECTED" : "ERR_STREAM_WORKER_CRASH";
        task.error_message = last_error_line.empty() ? ("Worker exited with code " + std::to_string(exit_code)) : last_error_line;
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message, false, true);
    }

    task.state = TaskState::kVerifying;
    task.status_detail = "Verifying output container...";
    uint64_t final_size = std::filesystem::file_size(muxed_output, ec);
    task.downloaded_bytes = final_size;
    task.total_bytes = final_size;
    task.stream_percent = 100.0;

    std::string desired_final_name = base_stem + "." + fmt;
    auto commit_res = PathSanitizer::CommitPartFile(muxed_output, task.save_dir, desired_final_name, ConflictPolicy::kAutoRename);
    std::filesystem::remove_all(task_tmp_dir, ec);

    if (!commit_res.ok()) {
        task.state = TaskState::kFailed;
        task.error_code = commit_res.error->code;
        task.error_message = commit_res.error->message;
        task.status_detail = task.error_message;
        return commit_res;
    }

    task.file_name = commit_res.value->filename().string();
    task.part_path.clear();
    task.state = TaskState::kCompleted;
    task.status_detail = "Completed";
    task.completed_at_ms = NowEpochMs();
    task.auth_headers.clear();
    return commit_res;
}

} // namespace openidm::stream
