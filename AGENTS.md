# AGENTS.md — Mandatory Instructions for AI Coding Agents

**Document Status:** Active Governance Policy  
**Last Reviewed:** 2026-10-16  
**Scope:** All AI coding agents, autonomous software engineers, and automated refactoring tools operating in this repository.

---

## 1. Project Objective

Your objective is to build and maintain **OpenIDM**, a native Windows 11 open-source download manager and browser media downloader. OpenIDM combines:
1. A native **C++20 / Win32** desktop application (`openidm.exe`) containing an in-process HTTP/HTTPS segmented download engine (`WinHTTP` + I/O Completion Ports) inspired by [Brisk](https://github.com/BrisklyDev/brisk)'s dynamic connection spawning and work-stealing range reuse algorithms.
2. A **Manifest V3 Browser Extension** (Chromium & Firefox) inspired by [Cat Catch](https://github.com/xifangczy/cat-catch) and [Brisk Browser Extension](https://github.com/BrisklyDev/brisk-browser-extension) that detects downloadable files, progressive videos, and HLS/DASH manifests.
3. A stateless **Native Messaging Host** (`openidm-nmh.exe`) that bridges browser `stdio` messages to `openidm.exe` over a per-user ACL-protected Windows Named Pipe.
4. Out-of-process worker integration with **[N_m3u8DL-RE](https://github.com/nilaoda/N_m3u8DL-RE)** and **[FFmpeg](https://ffmpeg.org/)** inside a Windows Job Object for HLS/DASH/MSS stream probing, segment downloading, and muxing.

---

## 2. Source of Truth for Architectural Decisions

1. **[docs/INDEX.md](docs/INDEX.md)** is the master routing table for all documentation.
2. **[docs/SYSTEM_ARCHITECTURE.md](docs/SYSTEM_ARCHITECTURE.md)** and **[docs/TECHNOLOGY_STACK.md](docs/TECHNOLOGY_STACK.md)** define the system structure and approved technology choices.
3. **[docs/adr/](docs/adr/)** (`0001` through `0008`) records binding architectural decisions.
4. **Never override an Accepted or Proposed ADR silently.** If implementation evidence proves an ADR must change, you must:
   - Document the empirical evidence in [docs/OPEN_QUESTIONS.md](docs/OPEN_QUESTIONS.md),
   - Update or supersede the relevant ADR in `docs/adr/`, and
   - Update [docs/SYSTEM_ARCHITECTURE.md](docs/SYSTEM_ARCHITECTURE.md) in the same change.

---

## 3. Mandatory Reading Order Before Editing Code

Before creating or modifying any file, inspect the repository state (`git status`, directory tree) and read the documents matching your task:

1. **Every Task:** Read `AGENTS.md` (this file), [docs/REPOSITORY_AUDIT.md](docs/REPOSITORY_AUDIT.md), and [docs/FEATURE_MATRIX.md](docs/FEATURE_MATRIX.md).
2. **Desktop UI, Core Controller, or Storage Tasks:** Read [docs/SYSTEM_ARCHITECTURE.md](docs/SYSTEM_ARCHITECTURE.md), [docs/DATA_MODEL_AND_STORAGE.md](docs/DATA_MODEL_AND_STORAGE.md), [docs/ERROR_HANDLING_AND_RECOVERY.md](docs/ERROR_HANDLING_AND_RECOVERY.md), and [docs/adr/0001-desktop-framework.md](docs/adr/0001-desktop-framework.md).
3. **HTTP Segmented Download Engine Tasks:** Read [docs/DOWNLOAD_ENGINE_DESIGN.md](docs/DOWNLOAD_ENGINE_DESIGN.md) and [docs/adr/0005-download-engine-integration.md](docs/adr/0005-download-engine-integration.md).
4. **HLS / DASH / Stream Pipeline Tasks:** Read [docs/STREAM_DOWNLOAD_PIPELINE.md](docs/STREAM_DOWNLOAD_PIPELINE.md), [docs/UPSTREAM_PROJECT_ANALYSIS.md](docs/UPSTREAM_PROJECT_ANALYSIS.md), and [docs/adr/0005-download-engine-integration.md](docs/adr/0005-download-engine-integration.md).
5. **Browser Extension or Media Detection Tasks:** Read [docs/BROWSER_EXTENSION.md](docs/BROWSER_EXTENSION.md), [docs/MEDIA_DETECTION.md](docs/MEDIA_DETECTION.md), and [docs/adr/0004-media-detection.md](docs/adr/0004-media-detection.md).
6. **Native Messaging or IPC Tasks:** Read [docs/NATIVE_MESSAGING_AND_IPC.md](docs/NATIVE_MESSAGING_AND_IPC.md), [docs/SECURITY_THREAT_MODEL.md](docs/SECURITY_THREAT_MODEL.md), and [docs/adr/0003-browser-communication.md](docs/adr/0003-browser-communication.md).
7. **Build, Installer, or Packaging Tasks:** Read [docs/WINDOWS_BUILD_AND_PACKAGING.md](docs/WINDOWS_BUILD_AND_PACKAGING.md), [docs/DEVELOPMENT_SETUP.md](docs/DEVELOPMENT_SETUP.md), and [docs/adr/0008-packaging-and-updates.md](docs/adr/0008-packaging-and-updates.md).

---

## 4. Rules for Inspecting Existing Implementations Before Creating New Ones

1. **Check the Actual Tree First:** Run directory and symbol searches before adding any class, module, or script. Do not assume a file exists because a design document proposes it, and do not assume a directory is empty without listing it.
2. **No Duplicate Abstractions:** Each subsystem (`core/`, `engine_http/`, `engine_stream/`, `ipc/`, `storage/`, `ui/`, `nmh/`, `extension/`) has a single designated location defined in [docs/SYSTEM_ARCHITECTURE.md](docs/SYSTEM_ARCHITECTURE.md).
3. **Avoid Unnecessary Rewrites:** Do not rewrite working modules, reformat unrelated files, or change build configurations outside the scope of your assigned milestone or issue.

---

## 5. Coding Standards and Naming Conventions

### 5.1 C++20 Desktop & Native Messaging Host (`src/`)
- **Language Standard:** ISO C++20 (`/std:c++20`, `/permissive-`, `/W4`, `/WX` on MSVC 19.38+ / Visual Studio 2022).
- **Unicode Everywhere:** Always define `UNICODE` and `_UNICODE`. Use `std::wstring` or `std::wstring_view` at Win32 API boundaries (`CreateFileW`, `WinHttpOpen`, `CreateProcessW`) and UTF-8 (`std::string`) internally for JSON, SQLite, and logs. Convert exclusively via `MultiByteToWideChar` / `WideCharToMultiByte` helper functions in `src/common/unicode.hpp`. Never use ANSI `*A` Win32 APIs.
- **RAII for OS Handles:** Never hold raw `HANDLE`, `HINTERNET`, `HKEY`, `HWND`, or `sqlite3*` pointers across scopes without an RAII wrapper (`wil::unique_handle`, `wil::unique_winhttp_hinternet`, or project-defined `UniqueHandle`).
- **Naming:**
  - Namespaces: `openidm::core`, `openidm::engine`, `openidm::ipc`, `openidm::storage`, `openidm::ui` (`snake_case`).
  - Types, Classes, Structs, Enums: `PascalCase` (e.g., `HttpDownloadEngine`, `SegmentWorker`, `TaskState`).
  - Functions & Methods: `PascalCase` for public interface methods (`StartTask`, `CancelTask`), `camelCase` for private helpers.
  - Variables & Parameters: `snake_case` (`byte_offset`, `retry_count`), member variables prefixed with `m_` (`m_task_id`, `m_pipe_handle`).
  - Constants & Enum Values: `kPascalCase` (`kMaxConnectionsPerTask`, `TaskState::kDownloading`).
- **Error Handling:** Use `std::expected<T, Error>` (or a backported `Result<T, Error>` struct in `src/common/result.hpp`) for recoverable I/O, network, and parsing errors. Do not throw C++ exceptions across module, thread, or Win32 callback (`WndProc` / IOCP) boundaries.

### 5.2 TypeScript / JavaScript Browser Extension (`extension/`)
- **Language Standard:** TypeScript 5.x (strict mode `"strict": true`) compiled to ES2022 modules for Manifest V3.
- **No Heavy UI Frameworks in Content Scripts:** In-page content scripts and video overlay buttons must use zero-dependency Vanilla TypeScript + Shadow DOM (`mode: "closed"`) so they never leak CSS/JS into host webpages or add more than 15 KB to page weight.
- **Naming:** `PascalCase` for interfaces/types, `camelCase` for functions and variables, `UPPER_SNAKE_CASE` for constants.

---

## 6. Requirements for Adding Dependencies

1. **Zero Unapproved Dependencies:** Do not add any NuGet, vcpkg, npm, or git submodule dependency unless it is already listed as **Approved** in [docs/TECHNOLOGY_STACK.md](docs/TECHNOLOGY_STACK.md).
2. **Justification Checklist:** If a new dependency is proposed, you must first document in [docs/TECHNOLOGY_STACK.md](docs/TECHNOLOGY_STACK.md) and [LICENSE_REVIEW.md](LICENSE_REVIEW.md):
   - Exact functionality required and why the Windows 11 OS API (`WinHTTP`, `BCrypt`, `DPAPI`, `Win32`, `Direct2D`) or standard library cannot provide it.
   - SPDX license identifier and compatibility with `GPL-3.0-or-later` ([docs/adr/0006-licensing-and-dependency-policy.md](docs/adr/0006-licensing-and-dependency-policy.md)).
   - Binary size impact (uncompressed and compressed) against [docs/PERFORMANCE_AND_SIZE_BUDGET.md](docs/PERFORMANCE_AND_SIZE_BUDGET.md).
   - Security track record, CVE history, and maintenance status.

---

## 7. Security Requirements (Non-Negotiable)

Every code change must enforce the controls in [docs/SECURITY_THREAT_MODEL.md](docs/SECURITY_THREAT_MODEL.md) and [docs/PRIVACY_AND_DATA_HANDLING.md](docs/PRIVACY_AND_DATA_HANDLING.md):

1. **No Arbitrary Command Execution:** The browser extension and IPC layer must **never** accept raw command-line strings, executable paths, or arbitrary `N_m3u8DL-RE` / `ffmpeg` flags from webpages or extension messages. The desktop app constructs all worker arguments from strongly typed, allowlisted fields.
2. **No Command-Line Credential Exposure:** Never pass `Cookie`, `Authorization`, or custom headers on the `N_m3u8DL-RE.exe` command line where any local process can read them via `NtQueryInformationProcess` / WMI. Always write sensitive headers and cookies to a temporary response/cookie file in a user-ACL-restricted directory (`%LOCALAPPDATA%\OpenIDM\tmp\<task_uuid>\`) and delete it immediately after worker exit.
3. **No Path Traversal:** Sanitize every server-provided filename (`Content-Disposition`), URL path segment, and webpage `<title>`. Strip path separators (`/`, `\`), drive letters (`C:`), NTFS alternate data streams (`:`), reserved DOS device names (`CON`, `PRN`, `AUX`, `NUL`, `COM1`–`COM9`, `LPT1`–`LPT9`), trailing dots/spaces, and control characters (`0x00`–`0x1F`). Verify via `GetFullPathNameW` that the resolved target path resides strictly inside the user-selected destination directory.
4. **No Unauthenticated Localhost HTTP Server:** Primary browser-to-desktop communication must use Native Messaging (`stdio`) bridged to an ACL-protected Windows Named Pipe (`\\.\pipe\OpenIDM_IPC_<UserSID>`).
5. **No Plaintext Credential Logging:** Redact `Cookie`, `Set-Cookie`, `Authorization`, `Proxy-Authorization`, and query parameters matching token patterns (`token`, `sig`, `signature`, `key`, `hdnea`, `Policy`, `Key-Pair-Id`) in all logs and crash dumps.
6. **Strict DRM Non-Circumvention:** Never implement, integrate, or document Widevine, PlayReady, or FairPlay CDM key extraction, license server proxy attacks, or unauthorized decryption key acquisition. If a manifest contains `#EXT-X-KEY:METHOD=SAMPLE-AES-CTR` or DASH `cenc:pssh` DRM signaling, fail fast with error code `ERR_STREAM_DRM_PROTECTED` and explain clearly to the user that DRM-protected streams are unsupported.

---

## 8. License Compliance Requirements

1. Follow [LICENSE_REVIEW.md](LICENSE_REVIEW.md) and [docs/adr/0006-licensing-and-dependency-policy.md](docs/adr/0006-licensing-and-dependency-policy.md).
2. Do not copy source files from [Brisk](https://github.com/BrisklyDev/brisk) or [Cat Catch](https://github.com/xifangczy/cat-catch) without preserving copyright notices and GPL-3.0 license headers.
3. Keep `N_m3u8DL-RE.exe` (MIT) and `ffmpeg.exe` (LGPL-2.1 / GPL-3.0) as separate executables invoked out-of-process, accompanied by their full license texts and source offer in `THIRD_PARTY_NOTICES.md`.

---

## 9. Testing, Build, and Reporting Expectations

1. **Deterministic Verification:** Every feature implementation must include corresponding unit or integration tests as specified in [docs/TEST_STRATEGY.md](docs/TEST_STRATEGY.md) and update [docs/TEST_MATRIX.md](docs/TEST_MATRIX.md) and [docs/FEATURE_MATRIX.md](docs/FEATURE_MATRIX.md).
2. **Documentation Synchronization Table:** When you modify a component or interface, update the corresponding documentation in the same commit/PR:

| Type of Change | Documents That MUST Be Updated |
| :--- | :--- |
| New or modified functional/non-functional requirement | `docs/PRODUCT_REQUIREMENTS.md`, `docs/FEATURE_MATRIX.md`, `docs/TEST_MATRIX.md` |
| Native Messaging or Named Pipe IPC schema change | `docs/NATIVE_MESSAGING_AND_IPC.md`, `docs/SECURITY_THREAT_MODEL.md` |
| SQLite schema or persistence change | `docs/DATA_MODEL_AND_STORAGE.md`, `docs/ERROR_HANDLING_AND_RECOVERY.md` |
| HTTP Download Engine state machine or segment algorithm change | `docs/DOWNLOAD_ENGINE_DESIGN.md` |
| `N_m3u8DL-RE` or `FFmpeg` invocation/flag change | `docs/STREAM_DOWNLOAD_PIPELINE.md`, `docs/SECURITY_THREAT_MODEL.md` |
| Browser extension permission, content script, or detector change | `docs/BROWSER_EXTENSION.md`, `docs/MEDIA_DETECTION.md`, `docs/PRIVACY_AND_DATA_HANDLING.md` |
| Compiler flags, CMakeLists, dependencies, or installer script change | `docs/TECHNOLOGY_STACK.md`, `docs/WINDOWS_BUILD_AND_PACKAGING.md`, `docs/DEVELOPMENT_SETUP.md`, `LICENSE_REVIEW.md` |

3. **Truthful Reporting (Zero Hallucination Rule):**
   - Never claim code was compiled, executed, tested, or benchmarked if you could not run the compiler or test runner (for example, when running in a Linux environment without MSVC/Windows SDK or without outbound internet access).
   - Explicitly separate **Verified by Execution**, **Verified by Static Inspection**, and **Unverified (Requires Windows 11 Host)** in every status report and pull request summary.
