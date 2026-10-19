# ADR-0007: Configuration, Task Persistence, and Credential Storage

- **Date:** 2026-10-16
- **Status:** Proposed
- **Deciders:** Senior Windows Systems Engineer, Application Security Engineer
- **Related Documents:** [../DATA_MODEL_AND_STORAGE.md](../DATA_MODEL_AND_STORAGE.md), [../PRIVACY_AND_DATA_HANDLING.md](../PRIVACY_AND_DATA_HANDLING.md), [../ERROR_HANDLING_AND_RECOVERY.md](../ERROR_HANDLING_AND_RECOVERY.md)

---

## Status

**Proposed** (Baseline Storage Decision for Milestone 1).

## Context

OpenIDM must persist:
1. **Task Metadata & Lifecycle State:** URL, final destination path, file size, `ETag`/`Last-Modified` validators, category, queue ID, creation/completion timestamps, and error status across thousands of historical tasks.
2. **High-Frequency Segment Progress:** For every active segmented HTTP download (up to 32 segments per task), the exact `start_offset`, `current_offset`, and `end_offset` must be checkpointed frequently so a power outage, BSOD, or `TerminateProcess` loses minimal downloaded bytes and never corrupts the state file.
3. **Transient Authentication Headers:** Resuming a paused download from an authenticated session requires retaining the `Referer`, `User-Agent`, and (while the task is incomplete) `Cookie` or `Authorization` headers, without storing session cookies in plaintext on disk.
4. **User Preferences & Queues:** Concurrency limits, speed limits, category paths, and scheduled queue definitions.

## Decision

1. **Primary Storage Engine:** Use an embedded **SQLite 3** database (`%LOCALAPPDATA%\OpenIDM\state.db`) compiled from the official amalgamation (`sqlite3.c`) directly into `openidm.exe`.
2. **Concurrency & Crash-Safety Configuration:**
   - Open `state.db` on a dedicated serializer thread with:
     ```sql
     PRAGMA journal_mode = WAL;
     PRAGMA synchronous = NORMAL;
     PRAGMA foreign_keys = ON;
     PRAGMA busy_timeout = 5000;
     ```
   - Batch segment byte-offset checkpoints into a single SQLite transaction committed every **1,000 ms** (or immediately upon segment split, segment completion, task pause, or application shutdown).
3. **Credential Protection at Rest (DPAPI):**
   - Never store `Cookie`, `Authorization`, or `Proxy-Authorization` headers in plaintext columns.
   - Encrypt sensitive header JSON blobs using the Windows Data Protection API (**`CryptProtectData`** with `CRYPTPROTECT_UI_FORBIDDEN`, scoped to the current user logon credential) and store the ciphertext `BLOB` in `tasks.encrypted_auth_blob`.
   - **Automatic Ephemeral Wipe:** As soon as a task transitions to `Completed` (or is deleted), set `encrypted_auth_blob = NULL` and overwrite any temporary `@headers.rsp` / `cookies.txt` files before unlinking.
4. **Partial & Temporary File Layout:**
   - Ordinary HTTP downloads write directly to `<destination_dir>\<filename>.openidm.part` (pre-allocated via `SetFileInformationByHandle` `FileEndOfFileInfo`) so the final atomic rename (`FileRenameInfoEx`) is an instant same-volume metadata operation.
   - Stream downloads use an isolated per-task directory `%LOCALAPPDATA%\OpenIDM\tmp\<task_uuid>\` (created with a restrictive per-user DACL) for `N_m3u8DL-RE` segment assembly and response files, moving only the final muxed `.mp4`/`.mkv` to `<destination_dir>`.

## Alternatives Considered

1. **Flat JSON or INI Files (`settings.json`, `tasks.json`) (`Rejected`):**
   - *Why Rejected:* Rewriting a multi-megabyte `tasks.json` file every second during active multi-segment downloads causes high disk write amplification, SSD wear, and risks total state loss if the process crashes mid-write (even with write-temp-and-rename, serializing 5,000 tasks + 64 active segments every second wastes CPU).
2. **Windows Registry for Task or Settings Storage (`Rejected`):**
   - *Why Rejected:* Registry keys are inappropriate for relational task/segment state and make clean uninstallation and backup harder. Registry usage is restricted strictly to the OS-required Native Messaging Host registration keys (`HKCU\Software\...\NativeMessagingHosts\org.openidm.native`) and optional `HKCU\Software\Microsoft\Windows\CurrentVersion\Run` auto-start entry.
3. **Hive / Isar (Used by Upstream Brisk) (`Rejected`):**
   - *Why Rejected:* Dart-specific NoSQL stores; SQLite 3 is the gold standard for C/C++ embedded ACID storage (`< 1 MB` binary footprint, Public Domain).

## Rationale

SQLite 3 in `WAL` mode provides ACID transactions, sub-millisecond batched offset updates, and structured schema migrations (`PRAGMA user_version`), while Windows DPAPI provides zero-prompt, OS-backed encryption for transient session headers.

## Consequences

- **Positive:** Crash-safe resume within 1 second of progress (`NFR-005`), instant UI filtering/sorting over 10,000+ tasks via indexed SQL queries, and zero plaintext credentials on disk (`NFR-009`).
- **Negative:** DPAPI blobs cannot be decrypted if `state.db` is copied to a different Windows machine (which is a desirable security property for session cookies; tasks migrated to another PC simply fall back to unauthenticated resume or prompt for URL refresh).

## Risks & Mitigations

- **Risk:** Pre-allocating a 50 GB file on a FAT32/exFAT USB drive takes several seconds because FAT filesystems do not support NTFS sparse files or valid-data-length fast extension.
- **Mitigation:** Detect filesystem capabilities via `GetVolumeInformationW` (`FILE_SUPPORTS_SPARSE_FILES`) and handle slow allocation asynchronously on the disk worker thread with a `"Pre-allocating disk space..."` UI status indicator.
