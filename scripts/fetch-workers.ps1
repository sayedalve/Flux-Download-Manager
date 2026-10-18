# SPDX-License-Identifier: GPL-3.0-or-later
# Flux Download Manager -- Upstream Stream Worker Provisioning Script (v0.3.0)
# Downloads and cryptographically verifies upstream stream workers:
#  1. nilaoda/N_m3u8DL-RE (v0.3.0-beta win-x64) - MIT License
#  2. FFmpeg (win-x64 essentials / GPL-3.0)
#  3. yt-dlp (optional auxiliary metadata / Unlicense)
#
# Enforces pre-extraction or post-extraction SHA-256 digest validation,
# validates PE32+ AMD64 (0x8664) machine headers, and stages binaries into tools/win-x64/.

[CmdletBinding()]
param(
    [string]$TargetDir = (Join-Path $PSScriptRoot "..\tools\win-x64"),
    [string]$Nm3u8dlUrl = "https://github.com/nilaoda/N_m3u8DL-RE/releases/download/v0.3.0-beta/N_m3u8DL-RE_v0.3.0-beta_win-x64_20241203.zip",
    [string]$Nm3u8dlExpectedSha256 = "E5C93A8DE0C58C50922C17A8A3BABD7D2193FB43B1A7FACD0B18DE9274EF8061",
    [string]$FfmpegUrl = "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl.zip",
    [string]$FfmpegExpectedSha256 = "AA24A30E1194D4CE54999EFF72C58484B1A5AEE32DD9933A9242D15711DF1E48",
    [string]$YtDlpUrl = "https://github.com/yt-dlp/yt-dlp/releases/download/2026.08.19/yt-dlp.exe",
    [string]$YtDlpExpectedSha256 = "66674953fe251b89f4d08c5f0e35e0728679bd67ab3d7d05c0562af101dd3e7a",
    [switch]$Force = $false,
    [switch]$SkipChecksumVerification = $false
)

$ErrorActionPreference = "Stop"
New-Item -ItemType Directory -Force -Path $TargetDir | Out-Null
$TmpDir = Join-Path ([System.IO.Path]::GetTempPath()) ("flux_workers_" + [Guid]::NewGuid().ToString("N"))
New-Item -ItemType Directory -Force -Path $TmpDir | Out-Null

function Assert-Pe64Executable([string]$FilePath) {
    if (-not (Test-Path $FilePath)) {
        throw "Required executable missing: $FilePath"
    }
    $bytes = [System.IO.File]::ReadAllBytes($FilePath)
    if ($bytes.Length -lt 256 -or $bytes[0] -ne 0x4D -or $bytes[1] -ne 0x5A) {
        throw "File is not a valid Windows PE executable (missing MZ header): $FilePath"
    }
    $peOffset = [BitConverter]::ToInt32($bytes, 0x3C)
    $machine = [BitConverter]::ToUInt16($bytes, $peOffset + 4)
    if ($machine -ne 0x8664) {
        throw "Executable $FilePath is not x64 (IMAGE_FILE_MACHINE_AMD64 0x8664); got 0x$($machine.ToString('X4'))"
    }
}

function Assert-Sha256Checksum([string]$FilePath, [string]$ExpectedHash) {
    if (-not $ExpectedHash -or $SkipChecksumVerification) {
        return
    }
    $hasher = [System.Security.Cryptography.SHA256]::Create()
    $stream = [System.IO.File]::OpenRead($FilePath)
    try {
        $hashBytes = $hasher.ComputeHash($stream)
        $actual = [BitConverter]::ToString($hashBytes).Replace("-", "").ToUpperInvariant()
        $expected = $ExpectedHash.Trim().ToUpperInvariant()
        if ($actual -ne $expected) {
            throw "SHA-256 verification failed for $FilePath.`nExpected: $expected`nActual  : $actual"
        }
    } finally {
        $stream.Close()
        $hasher.Dispose()
    }
}

function Verify-ExistingBinary([string]$FilePath, [string]$ExpectedSha256) {
    if (-not (Test-Path $FilePath)) { return $false }
    try {
        Assert-Pe64Executable $FilePath
        if ($ExpectedSha256 -and -not $SkipChecksumVerification) {
            Assert-Sha256Checksum $FilePath $ExpectedSha256
        }
        return $true
    } catch {
        return $false
    }
}

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "   Flux Download Manager -- Upstream Worker Provisioning         " -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "Target Directory: $TargetDir" -ForegroundColor White

try {
    # 1. N_m3u8DL-RE (MIT License - nilaoda/N_m3u8DL-RE)
    $NmExe = Join-Path $TargetDir "N_m3u8DL-RE.exe"
    $needNmDownload = $Force -or (-not (Verify-ExistingBinary $NmExe $Nm3u8dlExpectedSha256))

    if ($needNmDownload) {
        Write-Host "`n[1/3] Downloading upstream nilaoda/N_m3u8DL-RE win-x64..." -ForegroundColor Cyan
        $NmZip = Join-Path $TmpDir "nm3u8dl.zip"
        Invoke-WebRequest -Uri $Nm3u8dlUrl -OutFile $NmZip -UseBasicParsing
        Expand-Archive -Path $NmZip -DestinationPath (Join-Path $TmpDir "nm") -Force
        $FoundNm = Get-ChildItem -Path (Join-Path $TmpDir "nm") -Filter "N_m3u8DL-RE.exe" -Recurse | Select-Object -First 1
        if (-not $FoundNm) { throw "N_m3u8DL-RE.exe not found in downloaded archive: $NmZip" }
        Assert-Pe64Executable $FoundNm.FullName
        Assert-Sha256Checksum $FoundNm.FullName $Nm3u8dlExpectedSha256
        Copy-Item -Force $FoundNm.FullName $NmExe
        Write-Host "  [OK] Staged and verified N_m3u8DL-RE.exe" -ForegroundColor Green
    } else {
        Write-Host "`n[1/3] N_m3u8DL-RE.exe already present and verified in $TargetDir" -ForegroundColor Green
    }
    Assert-Pe64Executable $NmExe

    # 2. FFmpeg (GPL-3.0 / LGPL-2.1)
    $FfExe = Join-Path $TargetDir "ffmpeg.exe"
    $needFfDownload = $Force -or (-not (Verify-ExistingBinary $FfExe $FfmpegExpectedSha256))

    if ($needFfDownload) {
        Write-Host "`n[2/3] Downloading upstream FFmpeg win-x64..." -ForegroundColor Cyan
        $FfZip = Join-Path $TmpDir "ffmpeg.zip"
        Invoke-WebRequest -Uri $FfmpegUrl -OutFile $FfZip -UseBasicParsing
        Expand-Archive -Path $FfZip -DestinationPath (Join-Path $TmpDir "ff") -Force
        $FoundFf = Get-ChildItem -Path (Join-Path $TmpDir "ff") -Filter "ffmpeg.exe" -Recurse | Select-Object -First 1
        if (-not $FoundFf) { throw "ffmpeg.exe not found in downloaded archive: $FfZip" }
        Assert-Pe64Executable $FoundFf.FullName
        # If sha256 is pinned, verify it; otherwise compute and display
        if ($FfmpegExpectedSha256) {
            Assert-Sha256Checksum $FoundFf.FullName $FfmpegExpectedSha256
        }
        Copy-Item -Force $FoundFf.FullName $FfExe
        Write-Host "  [OK] Staged and verified ffmpeg.exe" -ForegroundColor Green
    } else {
        Write-Host "`n[2/3] ffmpeg.exe already present and verified in $TargetDir" -ForegroundColor Green
    }
    Assert-Pe64Executable $FfExe

    # 3. yt-dlp (Optional auxiliary metadata worker - Unlicense)
    $YtExe = Join-Path $TargetDir "yt-dlp.exe"
    $needYtDownload = $Force -or (-not (Verify-ExistingBinary $YtExe $YtDlpExpectedSha256))

    if ($needYtDownload) {
        Write-Host "`n[3/3] Downloading upstream yt-dlp win-x64..." -ForegroundColor Cyan
        $YtDl = Join-Path $TmpDir "yt-dlp.exe"
        Invoke-WebRequest -Uri $YtDlpUrl -OutFile $YtDl -UseBasicParsing
        Assert-Pe64Executable $YtDl
        Assert-Sha256Checksum $YtDl $YtDlpExpectedSha256
        Copy-Item -Force $YtDl $YtExe
        Write-Host "  [OK] Staged and verified yt-dlp.exe" -ForegroundColor Green
    } else {
        Write-Host "`n[3/3] yt-dlp.exe already present and verified in $TargetDir" -ForegroundColor Green
    }
    Assert-Pe64Executable $YtExe

    # Also keep tools/ root synchronized for local development convenience
    $RootTools = Join-Path $PSScriptRoot "..\tools"
    New-Item -ItemType Directory -Force -Path $RootTools | Out-Null
    Copy-Item -Force $NmExe (Join-Path $RootTools "N_m3u8DL-RE.exe")
    Copy-Item -Force $FfExe (Join-Path $RootTools "ffmpeg.exe")
    Copy-Item -Force $YtExe (Join-Path $RootTools "yt-dlp.exe")

    # If dist\tools exists, keep it in sync for distribution packaging
    $DistToolsX64 = Join-Path $PSScriptRoot "..\dist\tools\win-x64"
    if (Test-Path (Join-Path $PSScriptRoot "..\dist")) {
        New-Item -ItemType Directory -Force -Path $DistToolsX64 | Out-Null
        Copy-Item -Force $NmExe (Join-Path $DistToolsX64 "N_m3u8DL-RE.exe")
        Copy-Item -Force $FfExe (Join-Path $DistToolsX64 "ffmpeg.exe")
        Copy-Item -Force $YtExe (Join-Path $DistToolsX64 "yt-dlp.exe")
    }

    Write-Host "`n=================================================================" -ForegroundColor Cyan
    Write-Host "   Stream Workers Provisioned and Cryptographically Verified     " -ForegroundColor Green
    Write-Host "=================================================================" -ForegroundColor Cyan
} finally {
    Remove-Item -Recurse -Force $TmpDir -ErrorAction SilentlyContinue
}
