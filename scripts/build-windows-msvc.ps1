# SPDX-License-Identifier: GPL-3.0-or-later
# OpenIDM -- Reproducible Windows 11 x64 Build, PE32+ Verification, Test and Installer Pipeline
# Detects MSVC / Windows SDK / CMake / Inno Setup, compiles openidm.exe (WIN32 GUI subsystem),
# openidm-nmh.exe (CUI subsystem), and openidm_unit_tests.exe, validates PE32+ x64 headers,
# provisions stream workers, runs automated unit/extension/integration tests, and compiles
# the production installer dist/OpenIDM-Setup-x64-0.2.0.exe.

param(
    [switch]$SkipWorkerFetch = $false,
    [switch]$SkipTests = $false,
    [switch]$SkipInstaller = $false
)

$ErrorActionPreference = "Stop"
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$BuildDir = Join-Path $RepoRoot "build\win-x64"
$ToolsWinDir = Join-Path $RepoRoot "tools\win-x64"
$DistDir = Join-Path $RepoRoot "dist"

Write-Host "================================================================="
Write-Host "     OpenIDM v0.2.0 Windows 11 x64 Release Build Pipeline       "
Write-Host "================================================================="

# -----------------------------------------------------------------------------
# 1. Toolchain and Environment Detection
# -----------------------------------------------------------------------------
Write-Host "`n==> [1/8] Detecting Compiler, Windows SDK, CMake, and Build Tools..."

# 1.1 Locate CMake
$CMakeCmd = $null
if (Get-Command "cmake.exe" -ErrorAction SilentlyContinue) {
    $CMakeCmd = (Get-Command "cmake.exe").Source
} else {
    $vsCMakeCandidates = @(
        "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "C:\Program Files\CMake\bin\cmake.exe"
    )
    foreach ($cand in $vsCMakeCandidates) {
        if (Test-Path $cand) {
            $CMakeCmd = $cand
            $env:PATH = (Split-Path $cand) + ";" + $env:PATH
            break
        }
    }
}
if (-not $CMakeCmd) {
    throw "CMake not found! Install Visual Studio 2022/2026 C++ CMake tools or CMake 3.25+."
}
Write-Host "  [+] CMake: $CMakeCmd"

# 1.2 Locate Inno Setup 6 ISCC
$IsccCmd = $null
if (Get-Command "ISCC.exe" -ErrorAction SilentlyContinue) {
    $IsccCmd = (Get-Command "ISCC.exe").Source
} else {
    $isccCandidates = @(
        (Join-Path $env:LOCALAPPDATA "Programs\Inno Setup 6\ISCC.exe"),
        "C:\Program Files (x86)\Inno Setup 6\ISCC.exe",
        "C:\Program Files\Inno Setup 6\ISCC.exe"
    )
    foreach ($cand in $isccCandidates) {
        if (Test-Path $cand) {
            $IsccCmd = $cand
            break
        }
    }
}
if ($IsccCmd) {
    Write-Host "  [+] Inno Setup: $IsccCmd"
} else {
    Write-Warning "Inno Setup 6 (ISCC.exe) not found. Pass -SkipInstaller if installer compilation is not required."
}

# 1.3 Detect Visual Studio Generator
$ConfigureArgs = @("-S", $RepoRoot, "-B", $BuildDir, "-A", "x64", "-DCMAKE_BUILD_TYPE=Release")
if (-not (Test-Path (Join-Path $BuildDir "CMakeCache.txt"))) {
    $helpText = & $CMakeCmd --help
    if ($helpText -match '\*\s+(Visual Studio \d+ \d+)') {
        $Generator = $Matches[1]
        $ConfigureArgs += @("-G", $Generator)
        Write-Host "  [+] Generator: $Generator (x64)"
    }
} else {
    Write-Host "  [+] Using existing CMakeCache.txt configuration"
}

# -----------------------------------------------------------------------------
# 2. Configure CMake Build
# -----------------------------------------------------------------------------
Write-Host "`n==> [2/8] Configuring CMake Release build in $BuildDir..."
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
& $CMakeCmd @ConfigureArgs
if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed with exit code $LASTEXITCODE" }

# -----------------------------------------------------------------------------
# 3. Compile Windows x64 Targets
# -----------------------------------------------------------------------------
Write-Host "`n==> [3/8] Compiling Windows x64 targets (openidm, openidm-nmh, openidm_unit_tests)..."
& $CMakeCmd --build $BuildDir --config Release --target openidm openidm-nmh openidm_unit_tests sqlite3
if ($LASTEXITCODE -ne 0) { throw "CMake compilation failed with exit code $LASTEXITCODE" }

$BuiltDir = $BuildDir
if (Test-Path (Join-Path $BuildDir "Release\openidm.exe")) {
    $BuiltDir = Join-Path $BuildDir "Release"
}

# Copy targets to canonical locations expected by installer and test suites
$Targets = @("openidm.exe", "openidm-nmh.exe", "openidm_unit_tests.exe")
foreach ($t in $Targets) {
    $srcPath = Join-Path $BuiltDir $t
    Copy-Item -Force $srcPath (Join-Path $BuildDir $t)
    Copy-Item -Force $srcPath (Join-Path $RepoRoot "build\$t")
}

# -----------------------------------------------------------------------------
# 4. Verify PE32+ Headers and Subsystems
# -----------------------------------------------------------------------------
Write-Host "`n==> [4/8] Verifying PE32+ AMD64 signatures and subsystem types..."

function Verify-PeBinary([string]$FilePath, [int]$ExpectedSubsystem, [string]$SubsystemName) {
    if (-not (Test-Path $FilePath)) {
        throw "Verification failed: missing binary $FilePath"
    }
    $bytes = [System.IO.File]::ReadAllBytes($FilePath)
    if ($bytes.Length -lt 256 -or $bytes[0] -ne 0x4D -or $bytes[1] -ne 0x5A) {
        throw "Invalid MZ DOS signature in $FilePath"
    }
    $peOffset = [BitConverter]::ToInt32($bytes, 0x3C)
    $peSig = [BitConverter]::ToUInt32($bytes, $peOffset)
    if ($peSig -ne 0x00004550) {
        throw "Invalid PE signature in $FilePath"
    }
    $machine = [BitConverter]::ToUInt16($bytes, $peOffset + 4)
    if ($machine -ne 0x8664) {
        throw "Binary is not x64 AMD64: $FilePath"
    }
    $optOffset = $peOffset + 24
    $magic = [BitConverter]::ToUInt16($bytes, $optOffset)
    if ($magic -ne 0x20B) {
        throw "Binary is not PE32+: $FilePath"
    }
    $subsystem = [BitConverter]::ToUInt16($bytes, $optOffset + 68)
    if ($subsystem -ne $ExpectedSubsystem) {
        throw "Binary subsystem mismatch in $FilePath"
    }
    $sizeKb = [Math]::Round($bytes.Length / 1024.0, 1)
    Write-Host "  [+] Verified $FilePath -> AMD64 PE32+ Subsystem=$subsystem ($SubsystemName), Size=${sizeKb} KB"
}

Verify-PeBinary (Join-Path $BuildDir "openidm.exe") 2 "IMAGE_SUBSYSTEM_WINDOWS_GUI"
Verify-PeBinary (Join-Path $BuildDir "openidm-nmh.exe") 3 "IMAGE_SUBSYSTEM_WINDOWS_CUI"
Verify-PeBinary (Join-Path $BuildDir "openidm_unit_tests.exe") 3 "IMAGE_SUBSYSTEM_WINDOWS_CUI"

# -----------------------------------------------------------------------------
# 5. Provision and Verify Upstream Stream Workers
# -----------------------------------------------------------------------------
Write-Host "`n==> [5/8] Provisioning and verifying upstream stream workers..."
New-Item -ItemType Directory -Force -Path $ToolsWinDir | Out-Null
$NmExe = Join-Path $ToolsWinDir "N_m3u8DL-RE.exe"
$FfExe = Join-Path $ToolsWinDir "ffmpeg.exe"

if (-not $SkipWorkerFetch -and (-not (Test-Path $NmExe) -or -not (Test-Path $FfExe))) {
    & (Join-Path $PSScriptRoot "fetch-workers.ps1") -TargetDir $ToolsWinDir
}

if (-not (Test-Path $NmExe)) {
    throw "Upstream N_m3u8DL-RE.exe missing from $ToolsWinDir!"
}
if (-not (Test-Path $FfExe)) {
    throw "FFmpeg.exe missing from $ToolsWinDir!"
}

Verify-PeBinary $NmExe 3 "IMAGE_SUBSYSTEM_WINDOWS_CUI"
Verify-PeBinary $FfExe 3 "IMAGE_SUBSYSTEM_WINDOWS_CUI"

# Copy workers to tools root for local multi-process resolution
$RootTools = Join-Path $RepoRoot "tools"
New-Item -ItemType Directory -Force -Path $RootTools | Out-Null
Copy-Item -Force $NmExe (Join-Path $RootTools "N_m3u8DL-RE.exe")
Copy-Item -Force $FfExe (Join-Path $RootTools "ffmpeg.exe")

# -----------------------------------------------------------------------------
# 6. Test Suite Execution
# -----------------------------------------------------------------------------
if (-not $SkipTests) {
    Write-Host "`n==> [6/8] Executing Test Suites..."

    # 6.1 C++ Unit Tests
    Write-Host "  --> [A] C++20 Unit and Regression Tests (12 Suites)..."
    & (Join-Path $BuildDir "openidm_unit_tests.exe")
    if ($LASTEXITCODE -ne 0) { throw "C++20 unit test suite failed!" }

    # 6.2 Browser Extension Tests
    if (Get-Command "node" -ErrorAction SilentlyContinue) {
        Write-Host "  --> [B] Browser Extension Media Detection Tests..."
        node (Join-Path $RepoRoot "tests\test_extension_detection.js")
        if ($LASTEXITCODE -ne 0) { throw "Browser extension test suite failed!" }
    } else {
        Write-Warning "node.exe not found in PATH -- skipped extension tests."
    }

    # 6.3 End-to-End Integration Tests
    if (Get-Command "python" -ErrorAction SilentlyContinue) {
        Write-Host "  --> [C] End-to-End Integration, Stream and Benchmark Suite..."
        python (Join-Path $RepoRoot "scripts\run-benchmarks.py")
        if ($LASTEXITCODE -ne 0) { throw "Integration and benchmark suite failed!" }
    } else {
        Write-Warning "python.exe not found in PATH -- skipped integration tests."
    }
} else {
    Write-Host "`n==> [6/8] Tests skipped as requested (-SkipTests)."
}

# -----------------------------------------------------------------------------
# 7. Compile Inno Setup Production Installer
# -----------------------------------------------------------------------------
if (-not $SkipInstaller) {
    Write-Host "`n==> [7/8] Compiling Windows 11 x64 Installer via Inno Setup 6..."
    if (-not $IsccCmd) {
        throw "ISCC.exe (Inno Setup 6) is required to build the release installer."
    }
    New-Item -ItemType Directory -Force -Path $DistDir | Out-Null
    & $IsccCmd (Join-Path $RepoRoot "installer\openidm.iss")
    if ($LASTEXITCODE -ne 0) { throw "Inno Setup installer compilation failed!" }

    $InstallerPath = Join-Path $DistDir "OpenIDM-Setup-x64-0.2.0.exe"
    if (-not (Test-Path $InstallerPath)) {
        throw "Installer output not found at $InstallerPath"
    }
    $instItem = Get-Item $InstallerPath
    $instSizeMb = [Math]::Round(($instItem.Length / 1MB), 2)
    Write-Host "  [+] Compiled installer: $InstallerPath ($instSizeMb MB)"
} else {
    Write-Host "`n==> [7/8] Installer compilation skipped as requested (-SkipInstaller)."
}

# -----------------------------------------------------------------------------
# 8. Checksum Generation and Final Verification
# -----------------------------------------------------------------------------
Write-Host "`n==> [8/8] Generating Release Checksums..."
$CheckFiles = @(
    (Join-Path $BuildDir "openidm.exe"),
    (Join-Path $BuildDir "openidm-nmh.exe"),
    (Join-Path $ToolsWinDir "N_m3u8DL-RE.exe"),
    (Join-Path $ToolsWinDir "ffmpeg.exe")
)
if (Test-Path (Join-Path $DistDir "OpenIDM-Setup-x64-0.2.0.exe")) {
    $CheckFiles += (Join-Path $DistDir "OpenIDM-Setup-x64-0.2.0.exe")
}

$ChecksumReport = Join-Path $DistDir "SHA256SUMS.txt"
$hashLines = @()
foreach ($f in $CheckFiles) {
    if (Test-Path $f) {
        $h = Get-FileHash -Algorithm SHA256 $f
        $rel = $f
        if ($f.StartsWith($RepoRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
            $rel = $f.Substring($RepoRoot.Length).TrimStart('\', '/')
        }
        $line = "$($h.Hash)  $rel"
        $hashLines += $line
        Write-Host "  $line"
    }
}
$hashLines | Out-File -Encoding utf8 -FilePath $ChecksumReport

Write-Host "`n================================================================="
Write-Host "     Windows 11 x64 Build & Verification Completed Successfully  "
Write-Host "=================================================================`n"
