# SPDX-License-Identifier: GPL-3.0-or-later
# Package portable release archive for Flux Download Manager
$ErrorActionPreference = "Stop"
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$stage = Join-Path ([System.IO.Path]::GetTempPath()) ("Flux-v0.3.0-Windows-x64_" + [Guid]::NewGuid().ToString("N"))
New-Item -ItemType Directory -Force -Path $stage | Out-Null

Copy-Item (Join-Path $RepoRoot "build\win-x64\flux.exe") $stage\
Copy-Item (Join-Path $RepoRoot "build\win-x64\flux-nmh.exe") $stage\
Copy-Item (Join-Path $RepoRoot "README.md") $stage\
Copy-Item (Join-Path $RepoRoot "LICENSE_REVIEW.md") $stage\

New-Item -ItemType Directory -Force -Path (Join-Path $stage "tools\win-x64") | Out-Null
Copy-Item (Join-Path $RepoRoot "tools\win-x64\N_m3u8DL-RE.exe") (Join-Path $stage "tools\win-x64\")
Copy-Item (Join-Path $RepoRoot "tools\win-x64\ffmpeg.exe") (Join-Path $stage "tools\win-x64\")

Copy-Item -Recurse (Join-Path $RepoRoot "extension") (Join-Path $stage "extension")
Copy-Item -Recurse (Join-Path $RepoRoot "installer\manifests") (Join-Path $stage "manifests")

$distDir = Join-Path $RepoRoot "dist"
New-Item -ItemType Directory -Force -Path $distDir | Out-Null
$zipPath = Join-Path $distDir "Flux-v0.3.0-Windows-x64.zip"
if (Test-Path $zipPath) { Remove-Item -Force $zipPath }

Write-Host "Compressing Flux-v0.3.0-Windows-x64.zip..."
Compress-Archive -Path (Join-Path $stage "*") -DestinationPath $zipPath -CompressionLevel Optimal
Remove-Item -Recurse -Force $stage

$h = Get-FileHash $zipPath -Algorithm SHA256
$line = "$($h.Hash)  dist\Flux-v0.3.0-Windows-x64.zip"
Write-Host $line

# Append to SHA256SUMS.txt if not present
$sumsPath = Join-Path $distDir "SHA256SUMS.txt"
$existing = Get-Content $sumsPath -Raw
if ($existing -notmatch "Flux-v0.3.0-Windows-x64.zip") {
    Add-Content -Path $sumsPath -Value $line -Encoding utf8
}
Write-Host "Packaging completed successfully!"
