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

std::string TrimView(std::string_view sv) {
    while (!sv.empty() && (sv.front() == ' ' || sv.front() == '\t' || sv.front() == '\r' || sv.front() == '\n')) {
        sv.remove_prefix(1);
    }
    while (!sv.empty() && (sv.back() == ' ' || sv.back() == '\t' || sv.back() == '\r' || sv.back() == '\n')) {
        sv.remove_suffix(1);
    }
    return std::string(sv);
}

std::string ToLowerStr(std::string_view sv) {
    std::string s(sv);
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string StripGooglevideoRangeParams(const std::string& url) {
    if (url.find("googlevideo.com") == std::string::npos && url.find("/videoplayback") == std::string::npos) {
        return url;
    }
    auto qpos = url.find('?');
    if (qpos == std::string::npos) return url;
    std::string base = url.substr(0, qpos);
    std::string query = url.substr(qpos + 1);
    std::ostringstream kept;
    bool first = true;
    std::istringstream qss(query);
    std::string pair;
    while (std::getline(qss, pair, '&')) {
        if (pair.empty()) continue;
        auto eq = pair.find('=');
        std::string key = (eq == std::string::npos) ? pair : pair.substr(0, eq);
        if (key == "range" || key == "rn" || key == "rbuf" || key == "ump" || key == "srfvp" || key == "alr" || key == "cpn") {
            continue;
        }
        if (!first) kept << "&";
        kept << pair;
        first = false;
    }
    std::string qs = kept.str();
    return qs.empty() ? base : (base + "?" + qs);
}

std::string ExtractBalancedJsonObject(const std::string& s, size_t start_pos) {
    size_t open_brace = s.find('{', start_pos);
    if (open_brace == std::string::npos) return "";
    int depth = 0;
    bool in_str = false;
    bool escaped = false;
    for (size_t i = open_brace; i < s.size(); ++i) {
        char c = s[i];
        if (in_str) {
            if (escaped) {
                escaped = false;
            } else if (c == '\\') {
                escaped = true;
            } else if (c == '"') {
                in_str = false;
            }
        } else {
            if (c == '"') {
                in_str = true;
            } else if (c == '{') {
                ++depth;
            } else if (c == '}') {
                --depth;
                if (depth == 0) {
                    return s.substr(open_brace, i - open_brace + 1);
                }
            }
        }
    }
    return "";
}

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
} // namespace

bool Nm3u8dlReAdapter::VerifyContainerMagicBytes(const std::filesystem::path& file_path, const std::string& fmt) {
    std::ifstream ifs(file_path, std::ios::binary);
    if (!ifs) return false;
    uint8_t hdr[16]{};
    ifs.read(reinterpret_cast<char*>(hdr), sizeof(hdr));
    std::streamsize got = ifs.gcount();
    if (got < 4) return false;

    std::string f = ToLowerStr(fmt);

    // Matroska / WebM EBML header: 0x1A 0x45 0xDF 0xA3
    if (f == "mkv" || f == "webm") {
        return (got >= 4 && hdr[0] == 0x1A && hdr[1] == 0x45 && hdr[2] == 0xDF && hdr[3] == 0xA3);
    }
    // ISO Base Media File Format (MP4, M4A, MOV): bytes 4..7 == "ftyp", "moov", "free", "mdat"
    if (f == "mp4" || f == "m4a" || f == "mov") {
        if (got >= 8 &&
            (std::memcmp(hdr + 4, "ftyp", 4) == 0 ||
             std::memcmp(hdr + 4, "moov", 4) == 0 ||
             std::memcmp(hdr + 4, "free", 4) == 0 ||
             std::memcmp(hdr + 4, "mdat", 4) == 0)) {
            return true;
        }
        return false;
    }
    // MP3: ID3 header ("ID3") or MPEG audio frame sync (0xFF followed by 0xE0..0xFF)
    if (f == "mp3") {
        if (got >= 3 && hdr[0] == 'I' && hdr[1] == 'D' && hdr[2] == '3') return true;
        if (got >= 2 && hdr[0] == 0xFF && (hdr[1] & 0xE0) == 0xE0) return true;
        return false;
    }
    // AAC: ADTS frame sync (0xFF followed by 0xF0..0xFF)
    if (f == "aac") {
        if (got >= 2 && hdr[0] == 0xFF && (hdr[1] & 0xF0) == 0xF0) return true;
        return false;
    }
    // FLAC: "fLaC" (0x66 0x4C 0x61 0x43)
    if (f == "flac") {
        return (got >= 4 && hdr[0] == 'f' && hdr[1] == 'L' && hdr[2] == 'a' && hdr[3] == 'C');
    }
    // Ogg / Opus: "OggS" (0x4F 0x67 0x67 0x53)
    if (f == "ogg" || f == "opus") {
        return (got >= 4 && hdr[0] == 'O' && hdr[1] == 'g' && hdr[2] == 'g' && hdr[3] == 'S');
    }
    // MPEG-TS: 0x47 sync byte
    if (f == "ts") {
        return (got >= 1 && hdr[0] == 0x47);
    }
    // Fallback for other media container formats: require at least 8 valid bytes
    return got >= 8;
}

namespace {

int RunExternalFfmpegMux(const std::filesystem::path& ffmpeg_bin, const std::vector<std::string>& args) {
#ifdef _WIN32
    auto to_wide = [](std::string_view sv) {
        if (sv.empty()) return std::wstring();
        int sz = MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), nullptr, 0);
        std::wstring w(sz, 0);
        MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), w.data(), sz);
        return w;
    };
    std::wostringstream cmd;
    cmd << L"\"" << ffmpeg_bin.wstring() << L"\"";
    for (const auto& a : args) {
        cmd << L" \"" << to_wide(a) << L"\"";
    }
    std::wstring wcmd = cmd.str();
    STARTUPINFOW si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(nullptr, wcmd.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        return -1;
    }
    WaitForSingleObject(pi.hProcess, 60000);
    DWORD exit_code = 1;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return static_cast<int>(exit_code);
#else
    pid_t pid = fork();
    if (pid == 0) {
        int devnull = ::open("/dev/null", O_RDWR);
        if (devnull >= 0) {
            dup2(devnull, STDIN_FILENO);
            dup2(devnull, STDOUT_FILENO);
            dup2(devnull, STDERR_FILENO);
            if (devnull > 2) ::close(devnull);
        }
        std::string bin_s = ffmpeg_bin.string();
        std::vector<char*> argv_ptrs;
        argv_ptrs.push_back(const_cast<char*>(bin_s.c_str()));
        for (const auto& a : args) {
            argv_ptrs.push_back(const_cast<char*>(a.c_str()));
        }
        argv_ptrs.push_back(nullptr);
        execvp(bin_s.c_str(), argv_ptrs.data());
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
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

    // Defect B Fix: Reject JSON descriptors or non-manifest bodies passed as cached_manifest_body
    std::string trimmed = TrimView(body);
    if (!trimmed.empty()) {
        if (trimmed.front() == '{' || trimmed.front() == '[') {
            return Result<void>::Err(
                "ERR_STREAM_INVALID_MANIFEST",
                "Nm3u8dlReAdapter expects actual HLS (#EXTM3U) or DASH (<MPD) manifest text, not a JSON descriptor");
        }
        bool is_hls = (trimmed.rfind("#EXTM3U", 0) == 0 ||
                       trimmed.find("#EXT-X-STREAM-INF") != std::string::npos ||
                       trimmed.find("#EXT-X-TARGETDURATION") != std::string::npos);
        bool is_dash = (trimmed.find("<MPD") != std::string::npos);
        bool is_mss = (trimmed.find("<SmoothStreamingMedia") != std::string::npos);
        if (!is_hls && !is_dash && !is_mss) {
            return Result<void>::Err(
                "ERR_STREAM_INVALID_MANIFEST",
                "Cached manifest body is not a valid HLS (#EXTM3U) or MPEG-DASH (<MPD) manifest");
        }
    } else if (!url.empty() && PathSanitizer::IsWebpageWatchUrl(url)) {
        return Result<void>::Err(
            "ERR_STREAM_INVALID_MANIFEST",
            "Webpage watch URL cannot be passed directly to N_m3u8DL-RE as an HLS/DASH manifest");
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
            "Stream uses unsupported DRM protection (" + out.drm_system + "). Flux does not circumvent DRM.");
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
        bool is_dash = (cached_manifest_body.find("<MPD") != std::string::npos);
        std::filesystem::path local_manifest = probe_dir / (is_dash ? "cached_manifest.mpd" : "cached_manifest.m3u8");
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
    if (task.url.find("googlevideo.com") != std::string::npos ||
        task.url.find("youtube.com") != std::string::npos ||
        task.url.find("youtu.be") != std::string::npos ||
        task.page_url.find("youtube.com") != std::string::npos ||
        task.stream_options.cached_manifest_body.find("YOUTUBE") != std::string::npos ||
        (!task.stream_options.cached_manifest_body.empty() &&
         (task.stream_options.cached_manifest_body.front() == '{' ||
          task.stream_options.cached_manifest_body.front() == '['))) {
        return ExecuteYouTubeExtraction(task, pause_requested, cancel_requested, speed_limit_bps, on_progress);
    }

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
        bool is_dash = (task.task_type == TaskType::kStreamDash ||
                        task.stream_options.cached_manifest_body.find("<MPD") != std::string::npos);
        std::filesystem::path local_manifest = task_tmp_dir / (is_dash ? "cached_manifest.mpd" : "cached_manifest.m3u8");
        std::ofstream mofs(local_manifest, std::ios::binary);
        mofs << task.stream_options.cached_manifest_body;
        mofs.close();
        input_target = local_manifest.string();
    }

    std::string req_fmt = ToLowerStr(task.stream_options.output_format);
    std::string fmt = "mp4";
    if (req_fmt == "mkv" || req_fmt == "webm" || req_fmt == "m4a" || req_fmt == "mp3" || req_fmt == "ts") {
        fmt = req_fmt;
    }
    std::string base_stem = PathSanitizer::SanitizeFileName(task.file_name.empty() ? "stream_video" : task.file_name);
    auto dot = base_stem.find_last_of('.');
    if (dot != std::string::npos) {
        base_stem = base_stem.substr(0, dot);
    }
    std::string temp_save_name = base_stem + ".flux_mux";

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

    bool is_audio_only = (fmt == "m4a" || fmt == "mp3" || task.stream_options.video_filter == "none");
    bool is_video_only = (task.stream_options.audio_filter == "none");

    if (is_audio_only) {
        rsp_lines.push_back("--drop-video");
        rsp_lines.push_back("-sa " + EscapeRspArg(task.stream_options.audio_filter.empty() || task.stream_options.audio_filter == "all" ? "best" : task.stream_options.audio_filter));
    } else if (is_video_only) {
        rsp_lines.push_back("--drop-audio");
        rsp_lines.push_back("-sv " + EscapeRspArg(task.stream_options.video_filter.empty() ? "best" : task.stream_options.video_filter));
    } else {
        rsp_lines.push_back("--auto-select");
        rsp_lines.push_back("-sv " + EscapeRspArg(task.stream_options.video_filter.empty() ? "best" : task.stream_options.video_filter));
        rsp_lines.push_back("-sa " + EscapeRspArg(task.stream_options.audio_filter.empty() ? "all" : task.stream_options.audio_filter));
    }
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
        std::filesystem::remove_all(task_tmp_dir, ec);
        std::filesystem::remove(muxed_output, ec);
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
    auto commit_res = PathSanitizer::CommitPartFile(muxed_output, task.save_dir, desired_final_name, task.conflict_policy);
    std::filesystem::remove_all(task_tmp_dir, ec);

    if (!commit_res.ok()) {
        std::filesystem::remove(muxed_output, ec);
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

// -----------------------------------------------------------------------------
// Phase 3: Explicit YouTube / Site Extraction & Adaptive Track Muxing (Defect B)
// -----------------------------------------------------------------------------
Result<std::filesystem::path> Nm3u8dlReAdapter::ExecuteYouTubeExtraction(
    TaskModel& task,
    std::atomic<bool>& pause_requested,
    std::atomic<bool>& cancel_requested,
    uint64_t speed_limit_bps,
    ProgressCallback on_progress) {

    std::error_code ec;
    if (!std::filesystem::exists(m_cfg.ffmpeg_binary_path, ec)) {
        task.state = TaskState::kFailed;
        task.error_code = "ERR_DEPENDENCY_MISSING";
        task.error_message = "FFmpeg binary missing: " + m_cfg.ffmpeg_binary_path.string();
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message, false, true);
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

    std::string fmt = (task.stream_options.output_format == "mkv") ? "mkv" : "mp4";
    std::string base_stem = PathSanitizer::SanitizeFileName(task.file_name.empty() ? "youtube_video" : task.file_name);
    auto dot = base_stem.find_last_of('.');
    if (dot != std::string::npos) {
        base_stem = base_stem.substr(0, dot);
    }
    std::filesystem::path muxed_output = std::filesystem::path(task.save_dir) / (base_stem + ".flux_yt_mux." + fmt);

    task.state = TaskState::kProbing;
    task.status_detail = "Extracting YouTube stream metadata...";
    if (on_progress) {
        StreamProgress sp;
        sp.task_id = task.task_id;
        sp.phase = TaskState::kProbing;
        sp.percent = 0.0;
        sp.status_line = task.status_detail;
        on_progress(sp);
    }

    std::string watch_url = task.url;
    std::string video_url;
    std::string audio_url;
    std::string hls_manifest_url;
    std::string dash_manifest_url;
    bool requires_cipher = false;
    bool requires_audio_mux = false;

    std::string req_filter = ToLowerStr(TrimView(task.stream_options.video_filter));
    bool wants_audio_only = (req_filter == "audio" || req_filter == "audio-only");
    int target_height = 0;
    {
        std::smatch hm;
        static const std::regex kFilterHeightRe(R"((\d{3,4})p?)");
        if (!wants_audio_only && req_filter != "best" && std::regex_search(req_filter, hm, kFilterHeightRe)) {
            target_height = std::stoi(hm[1].str());
        }
    }

    // 1. Parse structured manifest body if provided as JSON
    if (!task.stream_options.cached_manifest_body.empty() &&
        (task.stream_options.cached_manifest_body.front() == '{' || task.stream_options.cached_manifest_body.front() == '[')) {
        try {
            auto root = json::Parse(task.stream_options.cached_manifest_body);
            if (root.is_object()) {
                if (root["qualities"].is_array() && !req_filter.empty() && req_filter != "best") {
                    for (const auto& q : root["qualities"].as_array()) {
                        std::string qid = ToLowerStr(q["id"].as_string());
                        std::string qres = ToLowerStr(q["resolution"].as_string());
                        int qh = static_cast<int>(q["height"].as_int64(0));
                        if (qid == req_filter || qres == req_filter || (target_height > 0 && qh == target_height) ||
                            (wants_audio_only && q["isAudioOnly"].as_bool(false))) {
                            std::string qv = StripGooglevideoRangeParams(q["videoUrl"].as_string());
                            std::string qa = StripGooglevideoRangeParams(q["pairedAudioUrl"].as_string());
                            if (!qv.empty()) {
                                video_url = qv;
                                audio_url = qa;
                                requires_audio_mux = !audio_url.empty();
                                requires_cipher = q["requiresCipherExtraction"].as_bool(false);
                            }
                            break;
                        }
                    }
                }
                if (video_url.empty()) video_url = StripGooglevideoRangeParams(root["videoUrl"].as_string());
                if (audio_url.empty()) audio_url = StripGooglevideoRangeParams(root["audioUrl"].as_string());
                if (watch_url.empty()) watch_url = root["watchUrl"].as_string();
                hls_manifest_url = root["hlsManifestUrl"].as_string();
                dash_manifest_url = root["dashManifestUrl"].as_string();
                if (root["requiresCipherExtraction"].as_bool(false) && video_url.empty()) requires_cipher = true;
                if (root["requiresAudioMux"].as_bool(false) && !wants_audio_only) requires_audio_mux = true;
            }
        } catch (...) {}
    }

    if (video_url.empty() && task.url.find("googlevideo.com") != std::string::npos) {
        video_url = StripGooglevideoRangeParams(task.url);
    }

    if (!video_url.empty() && (video_url == watch_url || PathSanitizer::IsWebpageWatchUrl(video_url))) {
        if (watch_url.empty()) watch_url = video_url;
        video_url.clear();
    }

    std::map<std::string, std::string> combined_headers = task.headers;
    for (const auto& [k, v] : task.auth_headers) {
        combined_headers[k] = v;
    }
    if (combined_headers.find("User-Agent") == combined_headers.end()) {
        combined_headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36";
    }
    if (combined_headers.find("Referer") == combined_headers.end()) {
        combined_headers["Referer"] = (!watch_url.empty() && watch_url.find("youtube.com") != std::string::npos) ? watch_url : "https://www.youtube.com/";
    }

    // 2. If watch_url is a webpage or player endpoint and video_url is not yet resolved, inspect page/player JSON
    auto extract_from_player_json = [&](const json::Value& root) -> Result<void> {
        if (!root.is_object()) {
            return Result<void>::Err("ERR_YOUTUBE_PARSE", "Invalid YouTube player response object");
        }
        const auto& play_status = root["playabilityStatus"];
        if (play_status.is_object()) {
            std::string st = play_status["status"].as_string("OK");
            if (st != "OK" && !root["streamingData"].is_object()) {
                std::string reason = play_status["reason"].as_string("YouTube stream status: " + st);
                return Result<void>::Err("ERR_YOUTUBE_UNPLAYABLE", reason);
            }
        }
        const auto& sdata = root["streamingData"];
        if (!sdata.is_object()) {
            return Result<void>::Err("ERR_YOUTUBE_NO_STREAMS", "No streamingData found in YouTube player response");
        }

        if (hls_manifest_url.empty()) hls_manifest_url = sdata["hlsManifestUrl"].as_string();
        if (dash_manifest_url.empty()) dash_manifest_url = sdata["dashManifestUrl"].as_string();

        uint64_t best_v_bw = 0;
        uint64_t matched_v_bw = 0;
        std::string matched_v_url;
        uint64_t best_a_bw = 0;
        bool saw_cipher = false;
        bool saw_drm = false;

        if (sdata["adaptiveFormats"].is_array()) {
            for (const auto& item : sdata["adaptiveFormats"].as_array()) {
                if (item["drmFamilies"].is_array()) {
                    saw_drm = true;
                    continue;
                }
                std::string u = item["url"].as_string();
                if (u.empty()) {
                    if (!item["signatureCipher"].as_string().empty() || !item["cipher"].as_string().empty()) {
                        saw_cipher = true;
                    }
                    continue;
                }
                std::string mime = ToLowerStr(item["mimeType"].as_string());
                uint64_t bw = static_cast<uint64_t>(item["bitrate"].as_int64(item["averageBitrate"].as_int64(500000)));
                int h = static_cast<int>(item["height"].as_int64(0));
                std::string qlabel = ToLowerStr(item["qualityLabel"].as_string());
                if (mime.rfind("video/", 0) == 0) {
                    if (bw >= best_v_bw) {
                        best_v_bw = bw;
                        video_url = StripGooglevideoRangeParams(u);
                    }
                    if (target_height > 0 && (h == target_height || qlabel == req_filter || qlabel.rfind(std::to_string(target_height) + "p", 0) == 0)) {
                        if (bw >= matched_v_bw) {
                            matched_v_bw = bw;
                            matched_v_url = StripGooglevideoRangeParams(u);
                        }
                    }
                } else if (mime.rfind("audio/", 0) == 0 && bw >= best_a_bw) {
                    best_a_bw = bw;
                    audio_url = StripGooglevideoRangeParams(u);
                }
            }
        }

        if (wants_audio_only && !audio_url.empty()) {
            video_url = audio_url;
            audio_url.clear();
            requires_audio_mux = false;
            requires_cipher = false;
            return Result<void>::Ok();
        }

        if (!matched_v_url.empty()) {
            video_url = matched_v_url;
        }

        if (!video_url.empty() && !audio_url.empty()) {
            requires_audio_mux = true;
            requires_cipher = false;
            return Result<void>::Ok();
        }

        // If adaptive video had no paired audio, check progressive formats (combined audio+video)
        std::string best_prog_url;
        uint64_t best_prog_bw = 0;
        if (sdata["formats"].is_array()) {
            for (const auto& item : sdata["formats"].as_array()) {
                if (item["drmFamilies"].is_array()) {
                    saw_drm = true;
                    continue;
                }
                std::string u = item["url"].as_string();
                if (u.empty()) {
                    if (!item["signatureCipher"].as_string().empty() || !item["cipher"].as_string().empty()) {
                        saw_cipher = true;
                    }
                    continue;
                }
                uint64_t bw = static_cast<uint64_t>(item["bitrate"].as_int64(1000000));
                if (bw >= best_prog_bw || best_prog_url.empty()) {
                    best_prog_bw = bw;
                    best_prog_url = StripGooglevideoRangeParams(u);
                }
            }
        }

        if (!best_prog_url.empty()) {
            video_url = best_prog_url;
            audio_url.clear();
            requires_audio_mux = false;
            requires_cipher = false;
            return Result<void>::Ok();
        }

        if (saw_drm && video_url.empty()) {
            return Result<void>::Err("ERR_STREAM_DRM_PROTECTED", "YouTube stream uses unsupported DRM protection");
        }
        if (saw_cipher) {
            requires_cipher = true;
        }
        return Result<void>::Ok();
    };

    if (video_url.empty() && !watch_url.empty() && hls_manifest_url.empty() && dash_manifest_url.empty()) {
        std::string page_body;
        auto get_res = brisk::HttpClient::PerformGet(
            watch_url,
            combined_headers,
            std::nullopt,
            std::nullopt,
            20000,
            m_cfg.allow_loopback_manifests,
            cancel_requested,
            [](const brisk::HttpResponseMeta&) { return true; },
            [&](const uint8_t* data, size_t len) {
                if (page_body.size() + len > 8 * 1048576) return false;
                page_body.append(reinterpret_cast<const char*>(data), len);
                return true;
            });
        if (get_res.ok() && !page_body.empty()) {
            std::string trimmed_page = TrimView(page_body);
            if (!trimmed_page.empty() && trimmed_page.front() == '{') {
                try {
                    auto parsed_j = json::Parse(trimmed_page);
                    auto ex_res = extract_from_player_json(parsed_j);
                    if (!ex_res.ok()) {
                        std::filesystem::remove_all(task_tmp_dir, ec);
                        task.state = TaskState::kFailed;
                        task.error_code = ex_res.error->code;
                        task.error_message = ex_res.error->message;
                        task.status_detail = task.error_message;
                        return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
                    }
                } catch (...) {}
            } else {
                auto yt_pos = page_body.find("ytInitialPlayerResponse");
                if (yt_pos != std::string::npos) {
                    std::string json_str = ExtractBalancedJsonObject(page_body, yt_pos);
                    if (!json_str.empty()) {
                        try {
                            auto parsed_j = json::Parse(json_str);
                            auto ex_res = extract_from_player_json(parsed_j);
                            if (!ex_res.ok()) {
                                std::filesystem::remove_all(task_tmp_dir, ec);
                                task.state = TaskState::kFailed;
                                task.error_code = ex_res.error->code;
                                task.error_message = ex_res.error->message;
                                task.status_detail = task.error_message;
                                return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
                            }
                        } catch (...) {}
                    }
                }
            }
        }
    }

    // 3. If an actual HLS or DASH manifest URL was discovered in streamingData, delegate to N_m3u8DL-RE
    if (video_url.empty() && (!hls_manifest_url.empty() || !dash_manifest_url.empty())) {
        TaskModel delegated = task;
        if (!hls_manifest_url.empty()) {
            delegated.url = hls_manifest_url;
            delegated.task_type = TaskType::kStreamHls;
        } else {
            delegated.url = dash_manifest_url;
            delegated.task_type = TaskType::kStreamDash;
        }
        delegated.stream_options.cached_manifest_body.clear();
        auto res = ExecuteStreamDownload(delegated, pause_requested, cancel_requested, speed_limit_bps, on_progress);
        task.state = delegated.state;
        task.file_name = delegated.file_name;
        task.downloaded_bytes = delegated.downloaded_bytes;
        task.total_bytes = delegated.total_bytes;
        task.stream_percent = delegated.stream_percent;
        task.status_detail = delegated.status_detail;
        task.error_code = delegated.error_code;
        task.error_message = delegated.error_message;
        task.completed_at_ms = delegated.completed_at_ms;
        return res;
    }

    // 4. If cipher extraction or site extraction is required (or video_url is still empty), try external yt-dlp worker
    std::filesystem::path ytdlp_bin = PathSanitizer::FindWorkerBinary("yt-dlp", PathSanitizer::GetExecutableDir());
    if ((video_url.empty() || requires_cipher) && std::filesystem::exists(ytdlp_bin, ec)) {
        task.state = TaskState::kDownloading;
        task.status_detail = "Extracting & downloading via yt-dlp...";

        std::string out_tmpl = muxed_output.string();
        std::string ytdlp_fmt = "bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4]/best";
        if (wants_audio_only) {
            ytdlp_fmt = "bestaudio[ext=m4a]/bestaudio";
        } else if (target_height > 0) {
            std::string hs = std::to_string(target_height);
            ytdlp_fmt = "bestvideo[height<=" + hs + "][ext=mp4]+bestaudio[ext=m4a]/best[height<=" + hs + "][ext=mp4]/best[height<=" + hs + "]";
        }
        std::vector<std::string> yt_args = {
            "--newline",
            "--no-playlist",
            "--no-warnings",
            "--ffmpeg-location", m_cfg.ffmpeg_binary_path.string(),
            "-f", ytdlp_fmt,
            "--merge-output-format", fmt,
            "-o", out_tmpl,
            watch_url
        };

        static const std::regex kYtDlpProgRe(R"(\[download\]\s+(\d+(?:\.\d+)?)%\s+of\s+~?\s*([0-9.]+)(KiB|MiB|GiB))");
        std::string last_yt_err;
        auto parse_yt_line = [&](const std::string& line) {
            if (line.empty()) return;
            OPENIDM_LOG_INFO("YtDlpWorker", line);
            if (line.find("ERROR:") != std::string::npos) {
                last_yt_err = line;
            }
            if (line.find("[Merger]") != std::string::npos || line.find("Merging formats") != std::string::npos) {
                task.state = TaskState::kMuxing;
                task.status_detail = "Merging audio/video tracks (FFmpeg)...";
                if (on_progress) {
                    StreamProgress sp;
                    sp.task_id = task.task_id;
                    sp.phase = TaskState::kMuxing;
                    sp.percent = 99.0;
                    sp.downloaded_bytes = task.downloaded_bytes;
                    sp.total_bytes = task.total_bytes;
                    sp.status_line = task.status_detail;
                    on_progress(sp);
                }
                return;
            }
            std::smatch m;
            if (std::regex_search(line, m, kYtDlpProgRe)) {
                double pct = std::stod(m[1].str());
                double sz_val = std::stod(m[2].str());
                std::string unit = m[3].str();
                double mult = 1048576.0;
                if (unit == "KiB") mult = 1024.0;
                else if (unit == "GiB") mult = 1073741824.0;
                uint64_t tot_b = static_cast<uint64_t>(sz_val * mult);
                uint64_t dl_b = static_cast<uint64_t>((pct / 100.0) * tot_b);

                task.state = TaskState::kDownloading;
                task.stream_percent = pct;
                task.downloaded_bytes = dl_b;
                task.total_bytes = tot_b;
                task.status_detail = "Downloading YouTube stream (" + std::to_string(static_cast<int>(pct)) + "%)";
                if (on_progress) {
                    StreamProgress sp;
                    sp.task_id = task.task_id;
                    sp.phase = TaskState::kDownloading;
                    sp.percent = pct;
                    sp.downloaded_bytes = dl_b;
                    sp.total_bytes = tot_b;
                    sp.status_line = task.status_detail;
                    on_progress(sp);
                }
            }
        };

        int yt_exit = -1;
#ifndef _WIN32
        int pipefd[2];
        if (pipe(pipefd) == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                setpgid(0, 0);
                ::close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                dup2(pipefd[1], STDERR_FILENO);
                if (pipefd[1] > 2) ::close(pipefd[1]);
                std::string bin_s = ytdlp_bin.string();
                std::vector<char*> argv_ptrs;
                argv_ptrs.push_back(const_cast<char*>(bin_s.c_str()));
                for (const auto& a : yt_args) argv_ptrs.push_back(const_cast<char*>(a.c_str()));
                argv_ptrs.push_back(nullptr);
                execvp(bin_s.c_str(), argv_ptrs.data());
                _exit(127);
            }
            ::close(pipefd[1]);
            std::string line_buf;
            char rbuf[4096];
            ssize_t n = 0;
            while ((n = ::read(pipefd[0], rbuf, sizeof(rbuf))) > 0) {
                line_buf.append(rbuf, static_cast<size_t>(n));
                size_t nl;
                while ((nl = line_buf.find_first_of("\r\n")) != std::string::npos) {
                    parse_yt_line(line_buf.substr(0, nl));
                    line_buf.erase(0, nl + 1);
                }
            }
            if (!line_buf.empty()) parse_yt_line(line_buf);
            ::close(pipefd[0]);
            int st = 0;
            waitpid(pid, &st, 0);
            yt_exit = WIFEXITED(st) ? WEXITSTATUS(st) : -1;
        }
#else
        auto to_wide = [](std::string_view sv) {
            if (sv.empty()) return std::wstring();
            int sz = MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), nullptr, 0);
            std::wstring w(sz, 0);
            MultiByteToWideChar(CP_UTF8, 0, sv.data(), static_cast<int>(sv.size()), w.data(), sz);
            return w;
        };
        SECURITY_ATTRIBUTES sa{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
        HANDLE hReadPipe = nullptr, hWritePipe = nullptr;
        if (CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
            SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);
            STARTUPINFOW si{};
            si.cb = sizeof(si);
            si.dwFlags = STARTF_USESTDHANDLES;
            si.hStdOutput = hWritePipe;
            si.hStdError = hWritePipe;
            si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

            std::wostringstream wcmd_ss;
            wcmd_ss << L"\"" << ytdlp_bin.wstring() << L"\"";
            for (const auto& a : yt_args) wcmd_ss << L" \"" << to_wide(a) << L"\"";
            std::wstring wcmd = wcmd_ss.str();
            PROCESS_INFORMATION pi{};
            if (CreateProcessW(nullptr, wcmd.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
                CloseHandle(hWritePipe);
                std::string line_buf;
                char rbuf[4096];
                DWORD got = 0;
                while (ReadFile(hReadPipe, rbuf, sizeof(rbuf), &got, nullptr) && got > 0) {
                    line_buf.append(rbuf, got);
                    size_t nl;
                    while ((nl = line_buf.find_first_of("\r\n")) != std::string::npos) {
                        parse_yt_line(line_buf.substr(0, nl));
                        line_buf.erase(0, nl + 1);
                    }
                }
                if (!line_buf.empty()) parse_yt_line(line_buf);
                WaitForSingleObject(pi.hProcess, 60000);
                DWORD wcode = 1;
                GetExitCodeProcess(pi.hProcess, &wcode);
                yt_exit = static_cast<int>(wcode);
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
            } else {
                CloseHandle(hWritePipe);
            }
            CloseHandle(hReadPipe);
        }
#endif
        if (yt_exit == 0 && std::filesystem::exists(muxed_output, ec) &&
            std::filesystem::file_size(muxed_output, ec) > 0 &&
            VerifyContainerMagicBytes(muxed_output, fmt)) {
            uint64_t final_size = std::filesystem::file_size(muxed_output, ec);
            task.downloaded_bytes = final_size;
            task.total_bytes = final_size;
            task.stream_percent = 100.0;
            std::string desired_final_name = base_stem + "." + fmt;
            auto commit_res = PathSanitizer::CommitPartFile(muxed_output, task.save_dir, desired_final_name, task.conflict_policy);
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

        if (video_url.empty()) {
            std::filesystem::remove_all(task_tmp_dir, ec);
            task.state = TaskState::kFailed;
            task.error_code = "ERR_YOUTUBE_EXTRACTION_FAILED";
            task.error_message = last_yt_err.empty() ? "yt-dlp failed to extract playable YouTube stream" : last_yt_err;
            task.status_detail = task.error_message;
            return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
        }
    }

    // 5. Validate resolved video_url & audio_url requirements
    if (video_url.empty()) {
        std::filesystem::remove_all(task_tmp_dir, ec);
        task.state = TaskState::kFailed;
        task.error_code = requires_cipher ? "ERR_YOUTUBE_EXTRACTOR_REQUIRED" : "ERR_YOUTUBE_NO_PLAYABLE_STREAM";
        task.error_message = requires_cipher
            ? "YouTube stream uses signatureCipher encryption and requires yt-dlp (install yt-dlp in tools/ or PATH)"
            : "No playable video stream URL could be resolved from YouTube player response";
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
    }

    if (requires_audio_mux && audio_url.empty()) {
        std::filesystem::remove_all(task_tmp_dir, ec);
        task.state = TaskState::kFailed;
        task.error_code = "ERR_YOUTUBE_INCOMPLETE_ADAPTIVE";
        task.error_message = "Adaptive YouTube video stream has no paired audio track; refusing to save incomplete silent video";
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
    }

    // 6. Download video track (and paired audio track if adaptive) and mux via FFmpeg
    std::filesystem::path video_tmp = task_tmp_dir / "yt_video.mp4";
    std::filesystem::path audio_tmp = task_tmp_dir / "yt_audio.m4a";
    bool has_separate_audio = (!audio_url.empty() && audio_url != video_url);
    uint32_t total_tracks = has_separate_audio ? 2u : 1u;
    uint64_t cumulative_bytes = 0;

    auto download_track = [&](const std::string& track_url, const std::filesystem::path& dest_path, uint32_t track_idx, const std::string& label) -> Result<void> {
        std::ofstream ofs(dest_path, std::ios::binary | std::ios::trunc);
        if (!ofs) {
            return Result<void>::Err("ERR_STORAGE_WRITE", "Failed to open temporary track file: " + dest_path.string());
        }
        uint64_t track_total = 0;
        uint64_t track_dl = 0;
        auto res = brisk::HttpClient::PerformGet(
            track_url,
            combined_headers,
            std::nullopt,
            std::nullopt,
            30000,
            m_cfg.allow_loopback_manifests,
            cancel_requested,
            [&](const brisk::HttpResponseMeta& meta) -> bool {
                if (meta.status_code != 200 && meta.status_code != 206) {
                    return false;
                }
                if (meta.content_length > 0) {
                    track_total = meta.content_length;
                }
                return true;
            },
            [&](const uint8_t* data, size_t len) -> bool {
                if (pause_requested.load() || cancel_requested.load()) return false;
                ofs.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(len));
                if (!ofs) return false;
                track_dl += len;
                cumulative_bytes += len;

                double track_pct = (track_total > 0) ? ((static_cast<double>(track_dl) * 100.0) / static_cast<double>(track_total)) : 50.0;
                double overall_pct = ((static_cast<double>(track_idx - 1) * 100.0) + track_pct) / static_cast<double>(total_tracks);

                task.state = TaskState::kDownloading;
                task.downloaded_bytes = cumulative_bytes;
                if (track_total > 0) {
                    task.total_bytes = std::max(task.total_bytes, cumulative_bytes);
                }
                task.stream_percent = std::clamp(overall_pct, 0.0, 98.0);
                task.completed_segments = track_idx - 1;
                task.total_segments = total_tracks;
                task.status_detail = "Downloading YouTube " + label + " (" + std::to_string(track_idx) + "/" + std::to_string(total_tracks) + ")";

                if (on_progress) {
                    StreamProgress sp;
                    sp.task_id = task.task_id;
                    sp.phase = TaskState::kDownloading;
                    sp.percent = task.stream_percent;
                    sp.downloaded_bytes = task.downloaded_bytes;
                    sp.total_bytes = task.total_bytes;
                    sp.completed_segments = task.completed_segments;
                    sp.total_segments = task.total_segments;
                    sp.status_line = task.status_detail;
                    on_progress(sp);
                }
                return true;
            });
        ofs.close();
        if (!res.ok()) {
            return Result<void>::Err(res.error->code, "Failed to download YouTube " + label + ": " + res.error->message);
        }
        if (std::filesystem::file_size(dest_path, ec) == 0) {
            return Result<void>::Err("ERR_YOUTUBE_EMPTY_TRACK", "Downloaded YouTube " + label + " is empty (0 bytes)");
        }
        return Result<void>::Ok();
    };

    auto v_res = download_track(video_url, video_tmp, 1, "video track");
    if (!v_res.ok()) {
        std::filesystem::remove_all(task_tmp_dir, ec);
        task.state = cancel_requested.load() ? TaskState::kCanceled : (pause_requested.load() ? TaskState::kPaused : TaskState::kFailed);
        task.error_code = v_res.error->code;
        task.error_message = v_res.error->message;
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
    }

    if (has_separate_audio) {
        auto a_res = download_track(audio_url, audio_tmp, 2, "audio track");
        if (!a_res.ok()) {
            std::filesystem::remove_all(task_tmp_dir, ec);
            task.state = cancel_requested.load() ? TaskState::kCanceled : (pause_requested.load() ? TaskState::kPaused : TaskState::kFailed);
            task.error_code = a_res.error->code;
            task.error_message = a_res.error->message;
            task.status_detail = task.error_message;
            return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
        }
    }

    // 7. Mux via FFmpeg into clean output container
    task.state = TaskState::kMuxing;
    task.completed_segments = total_tracks;
    task.total_segments = total_tracks;
    task.status_detail = "Merging YouTube audio/video tracks (FFmpeg)...";
    if (on_progress) {
        StreamProgress sp;
        sp.task_id = task.task_id;
        sp.phase = TaskState::kMuxing;
        sp.percent = 99.0;
        sp.downloaded_bytes = task.downloaded_bytes;
        sp.total_bytes = task.downloaded_bytes;
        sp.completed_segments = total_tracks;
        sp.total_segments = total_tracks;
        sp.status_line = task.status_detail;
        on_progress(sp);
    }

    std::vector<std::string> ff_args = {"-y", "-loglevel", "error", "-i", video_tmp.string()};
    if (has_separate_audio) {
        ff_args.push_back("-i");
        ff_args.push_back(audio_tmp.string());
        ff_args.insert(ff_args.end(), {"-map", "0:v:0", "-map", "1:a:0"});
    }
    ff_args.insert(ff_args.end(), {"-c:v", "copy", "-c:a", "copy", muxed_output.string()});

    int mux_rc = RunExternalFfmpegMux(m_cfg.ffmpeg_binary_path, ff_args);
    if (mux_rc != 0 || !std::filesystem::exists(muxed_output, ec) ||
        std::filesystem::file_size(muxed_output, ec) == 0 ||
        !VerifyContainerMagicBytes(muxed_output, fmt)) {
        std::filesystem::remove_all(task_tmp_dir, ec);
        std::filesystem::remove(muxed_output, ec);
        task.state = TaskState::kFailed;
        task.error_code = "ERR_YOUTUBE_MUX_FAILED";
        task.error_message = "FFmpeg failed to mux YouTube tracks (exit code " + std::to_string(mux_rc) + ")";
        task.status_detail = task.error_message;
        return Result<std::filesystem::path>::Err(task.error_code, task.error_message);
    }

    task.state = TaskState::kVerifying;
    task.status_detail = "Verifying output container...";
    uint64_t final_size = std::filesystem::file_size(muxed_output, ec);
    task.downloaded_bytes = final_size;
    task.total_bytes = final_size;
    task.stream_percent = 100.0;

    std::string desired_final_name = base_stem + "." + fmt;
    auto commit_res = PathSanitizer::CommitPartFile(muxed_output, task.save_dir, desired_final_name, task.conflict_policy);
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
