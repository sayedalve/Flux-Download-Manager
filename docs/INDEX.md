# OpenIDM Engineering Documentation Index

**Document Status:** Active Navigation Map (`v0.2.0`)  
**Last Reviewed:** 2026-10-16  
**Scope:** Complete guide to the 47-document OpenIDM engineering knowledge base for AI coding agents and human developers.

---

## 1. Recommended Reading Order for New Coding Agents

1. **[../README.md](../README.md)** — High-level overview, `v0.2.0` architecture, test verification summary, and quick start commands.
2. **[../AGENTS.md](../AGENTS.md)** — Mandatory rules, coding conventions, security invariants, and documentation update requirements.
3. **[PRODUCT_COMPLETION_AUDIT.md](PRODUCT_COMPLETION_AUDIT.md)** — Complete Phase 1 audit of the `v0.1.0` release gaps and prioritized bug table.
4. **[BUG_FIX_LOG.md](BUG_FIX_LOG.md)** — Root causes, implemented code changes, and regression tests for `BUG-001` through `BUG-014`.
5. **[UI_DESIGN_SYSTEM.md](UI_DESIGN_SYSTEM.md)** — Windows 11 Dark/Light/System design tokens, 4px grid, High-DPI scaling, control states, and accessibility.
6. **[RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md)** — Final `26 / 26` automated test results, baseline vs. post-fix benchmarks, and verification classifications.
7. **[SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md)** & **[SOURCE_MAPPING.md](SOURCE_MAPPING.md)** — End-to-end component boundaries and upstream mapping to Brisk, Cat Catch, and N_m3u8DL-RE.

---

## 2. Complete File Inventory (47 Markdown Documents)

### 2.1 Root Governance & Licensing Files
- [../README.md](../README.md) — Project overview, build commands, and test summary.
- [../AGENTS.md](../AGENTS.md) — Operating instructions and guardrails for AI coding agents.
- [../CONTRIBUTING.md](../CONTRIBUTING.md) — Development workflow, commit conventions, and PR gates.
- [../LICENSE_REVIEW.md](../LICENSE_REVIEW.md) — Upstream license audit and `GPL-3.0-or-later` compliance strategy.

### 2.2 Product Completion, UI & Release Verification (`docs/`)
- [PRODUCT_COMPLETION_AUDIT.md](PRODUCT_COMPLETION_AUDIT.md) — Phase 1 source code audit and prioritized defect catalog.
- [UI_DESIGN_SYSTEM.md](UI_DESIGN_SYSTEM.md) — Desktop UI design system, palettes, typography, DPI math, and dialogs.
- [BUG_FIX_LOG.md](BUG_FIX_LOG.md) — Detailed bug fix and regression test log (`BUG-001`–`BUG-014`).
- [RELEASE_READINESS_REPORT.md](RELEASE_READINESS_REPORT.md) — Final test results, benchmark comparisons, and platform verification classification.
- [SOURCE_MAPPING.md](SOURCE_MAPPING.md) — Exact file-by-file upstream source mapping for Brisk, Cat Catch, and N_m3u8DL-RE.
- [PHASE_REPORTS.md](PHASE_REPORTS.md) — Implementation and verification reports across all 10 build phases.

### 2.3 Core Product & Architecture Specifications (`docs/`)
- [INDEX.md](INDEX.md) — Documentation navigation map (this file).
- [PROJECT_BRIEF.md](PROJECT_BRIEF.md) — Problem statement, target users, MVP scope, non-goals, and success metrics.
- [PRODUCT_REQUIREMENTS.md](PRODUCT_REQUIREMENTS.md) — Traceable functional (`FR-001`–`FR-028`) and non-functional (`NFR-001`–`NFR-016`) requirements.
- [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md) — Component architecture, process boundaries, interfaces, and Mermaid diagrams.
- [TECHNOLOGY_STACK.md](TECHNOLOGY_STACK.md) — Detailed desktop framework evaluation and selected stack.
- [REPOSITORY_AUDIT.md](REPOSITORY_AUDIT.md) — Repository state and build artifact inventory.
- [UPSTREAM_PROJECT_ANALYSIS.md](UPSTREAM_PROJECT_ANALYSIS.md) — Deep dive into Brisk, Brisk Browser Extension, Cat Catch, and N_m3u8DL-RE.
- [FEATURE_MATRIX.md](FEATURE_MATRIX.md) — Requirement-to-module traceability and verification status matrix.
- [TECHNICAL_RISK_REGISTER.md](TECHNICAL_RISK_REGISTER.md) — Engineering risk catalog with mitigations and closure criteria.
- [GLOSSARY.md](GLOSSARY.md) — Canonical definitions of domain terms used across all documents.

### 2.4 Architecture Decision Records (`docs/adr/`)
- [adr/0001-desktop-framework.md](adr/0001-desktop-framework.md) — Selection of C++20 with Native Win32 API.
- [adr/0002-process-and-module-boundaries.md](adr/0002-process-and-module-boundaries.md) — Process separation between `openidm`, `openidm-nmh`, `N_m3u8DL-RE`, and `ffmpeg`.
- [adr/0003-browser-communication.md](adr/0003-browser-communication.md) — Selection of Native Messaging + ACL Named Pipe / UDS IPC.
- [adr/0004-media-detection.md](adr/0004-media-detection.md) — Multi-layered browser media detection and strict boundary between discovery and downloading.
- [adr/0005-download-engine-integration.md](adr/0005-download-engine-integration.md) — Native C++ adaptation of Brisk's HTTP algorithms + subprocess wrapper for `N_m3u8DL-RE`.
- [adr/0006-licensing-and-dependency-policy.md](adr/0006-licensing-and-dependency-policy.md) — `GPL-3.0-or-later` project licensing and third-party dependency rules.
- [adr/0007-configuration-and-persistence.md](adr/0007-configuration-and-persistence.md) — Embedded SQLite WAL persistence + DPAPI/AES-256-GCM encryption for transient secrets.
- [adr/0008-packaging-and-updates.md](adr/0008-packaging-and-updates.md) — Inno Setup installer, Native Messaging registration, and update verification.

### 2.5 Subsystem & Operational Specifications (`docs/`)
- [DOWNLOAD_ENGINE_DESIGN.md](DOWNLOAD_ENGINE_DESIGN.md) — Ordinary HTTP/HTTPS segmented download engine specification.
- [MEDIA_DETECTION.md](MEDIA_DETECTION.md) — Browser media sniffing, DOM observation, manifest deduplication, and confidence scoring.
- [STREAM_DOWNLOAD_PIPELINE.md](STREAM_DOWNLOAD_PIPELINE.md) — HLS/DASH/MSS probing, track selection, segment downloading, and muxing pipeline.
- [BROWSER_EXTENSION.md](BROWSER_EXTENSION.md) — Manifest V3 browser extension architecture, Shadow DOM overlay, and cross-browser compatibility.
- [NATIVE_MESSAGING_AND_IPC.md](NATIVE_MESSAGING_AND_IPC.md) — Wire protocol specification between browser extension, `openidm-nmh`, and `openidm`.
- [SECURITY_THREAT_MODEL.md](SECURITY_THREAT_MODEL.md) — STRIDE threat model, attack paths, mitigations, and security test cases.
- [PRIVACY_AND_DATA_HANDLING.md](PRIVACY_AND_DATA_HANDLING.md) — Data classification, DPAPI protection, log redaction, and retention policies.
- [DATA_MODEL_AND_STORAGE.md](DATA_MODEL_AND_STORAGE.md) — SQLite DDL schema, task state machine, crash recovery, and atomic file replacement.
- [ERROR_HANDLING_AND_RECOVERY.md](ERROR_HANDLING_AND_RECOVERY.md) — Canonical error taxonomy, retry policies, and user recovery flows.
- [WINDOWS_BUILD_AND_PACKAGING.md](WINDOWS_BUILD_AND_PACKAGING.md) — MSVC/CMake build pipeline, binary size breakdown, and Inno Setup packaging.
- [PERFORMANCE_AND_SIZE_BUDGET.md](PERFORMANCE_AND_SIZE_BUDGET.md) — Measured size, memory, startup, and throughput budgets.
- [TEST_STRATEGY.md](TEST_STRATEGY.md) — Unit, deterministic local test server, browser E2E, security, and installer test strategy.
- [TEST_MATRIX.md](TEST_MATRIX.md) — Requirement-to-test-case mapping and current execution status.
- [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) — Step-by-step developer workstation setup for Windows 11 and Linux.
- [RELEASE_AND_UPDATE_PROCESS.md](RELEASE_AND_UPDATE_PROCESS.md) — Semantic versioning, reproducible release builds, signing, and rollback procedures.
- [ROADMAP.md](ROADMAP.md) — Phased implementation milestones (`M0` through `M5`) with exit criteria.
- [KNOWN_LIMITATIONS.md](KNOWN_LIMITATIONS.md) — Browser MV3 restrictions, unsupported DRM/MSE streams, and host toolchain boundaries.
- [OPEN_QUESTIONS.md](OPEN_QUESTIONS.md) — Resolved and remaining empirical questions.
- [DOCUMENTATION_AUDIT.md](DOCUMENTATION_AUDIT.md) — Quality validation report for the documentation suite.
