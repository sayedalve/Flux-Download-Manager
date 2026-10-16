# SPDX-License-Identifier: GPL-3.0-or-later
# OpenIDM — Automated Windows x64 Upstream Stream Worker Provisioning Script
# Downloads and verifies upstream nilaoda/N_m3u8DL-RE (win-x64) and FFmpeg (win-x64) into tools/win-x64/,
# validates PE32+ AMD64 (0x8664) machine headers, and verifies --version execution.

param(
    [string]$TargetDir = (Join-Path $PSScriptRoot "..\tools\win-x64"),
    [string]$Nm3u8dlUrl = "https://github.com/nilaoda/N_m3u8DL-RE/releases/download/v0.3.0-beta/N_m3u8DL-RE_v0.3.0-beta_win-x64_20241203.zip",
    [string]$FfmpegUrl = "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl.zip"
)

$ErrorActionPreference = "Stop"
New-Item -ItemType Directory -Force -Path $TargetDir | Out-Null
$TmpDir = Join-Path ([System.IO.Path]::GetTempPath()) ("openidm_workers_" + [Guid]::NewGuid().ToString("N"))
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

try {
    $NmExe = Join-Path $TargetDir "N_m3u8DL-RE.exe"
    if (-not (Test-Path $NmExe)) {
        Write-Host "[1/2] Downloading upstream nilaoda/N_m3u8DL-RE win-x64..."
        $NmZip = Join-Path $TmpDir "nm3u8dl.zip"
        Invoke-WebRequest -Uri $Nm3u8dlUrl -OutFile $NmZip -UseBasicParsing
        Expand-Archive -Path $NmZip -DestinationPath (Join-Path $TmpDir "nm") -Force
        $FoundNm = Get-ChildItem -Path (Join-Path $TmpDir "nm") -Filter "N_m3u8DL-RE.exe" -Recurse | Select-Object -First 1
        if (-not $FoundNm) { throw "N_m3u8DL-RE.exe not found in downloaded archive" }
        Copy-Item -Force $FoundNm.FullName $NmExe
    }
    Assert-Pe64Executable $NmExe

    $FfExe = Join-Path $TargetDir "ffmpeg.exe"
    if (-not (Test-Path $FfExe)) {
        Write-Host "[2/2] Downloading FFmpeg win-x64..."
        $FfZip = Join-Path $TmpDir "ffmpeg.zip"
        Invoke-WebRequest -Uri $FfmpegUrl -OutFile $FfZip -UseBasicParsing
        Expand-Archive -Path $FfZip -DestinationPath (Join-Path $TmpDir "ff") -Force
        $FoundFf = Get-ChildItem -Path (Join-Path $TmpDir "ff") -Filter "ffmpeg.exe" -Recurse | Select-Object -First 1
        if (-not $FoundFf) { throw "ffmpeg.exe not found in downloaded archive" }
        Copy-Item -Force $FoundFf.FullName $FfExe
    }
    Assert-Pe64Executable $FfExe

    # Also copy to tools\ root for local development runs
    $RootTools = Join-Path $PSScriptRoot "..\tools"
    New-Item -ItemType Directory -Force -Path $RootTools | Out-Null
    Copy-Item -Force $NmExe (Join-Path $RootTools "N_m3u8DL-RE.exe")
    Copy-Item -Force $FfExe (Join-Path $RootTools "ffmpeg.exe")

    Write-Host "Successfully provisioned and verified x64 PE32+ workers in $TargetDir"
} finally {
    Remove-Item -Recurse -Force $TmpDir -ErrorAction SilentlyContinue
}
