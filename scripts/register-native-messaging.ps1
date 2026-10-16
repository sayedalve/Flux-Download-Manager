# SPDX-License-Identifier: GPL-3.0-or-later
# Flux Download Manager -- Native Messaging Host Per-User Registration Script
# Configures HKCU registry keys for Chrome, Edge, Brave, and Firefox without requiring admin rights.
# Supports portable mode by pointing directly to the local flux-nmh.exe executable.

param(
    [string]$ExtensionId = "",
    [string]$HostExePath = "",
    [switch]$Unregister = $false
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$RepoRoot = (Resolve-Path (Join-Path $ScriptDir "..")).Path

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "   Flux Download Manager -- Native Messaging Host Setup (HKCU)   " -ForegroundColor Cyan
Write-Host "=================================================================" -ForegroundColor Cyan

# 1. Locate flux-nmh.exe
if (-not $HostExePath) {
    $candidates = @(
        (Join-Path $RepoRoot "build\win-x64\Release\flux-nmh.exe"),
        (Join-Path $RepoRoot "build\win-x64\flux-nmh.exe"),
        (Join-Path $RepoRoot "dist\flux-nmh.exe"),
        (Join-Path $RepoRoot "flux-nmh.exe")
    )
    foreach ($cand in $candidates) {
        if (Test-Path $cand) {
            $HostExePath = (Resolve-Path $cand).Path
            break
        }
    }
}

if (-not $Unregister) {
    if (-not $HostExePath -or -not (Test-Path $HostExePath)) {
        Write-Error "flux-nmh.exe could not be found! Please build the solution first or specify -HostExePath."
        exit 1
    }
    Write-Host "[+] Found Native Host executable: $HostExePath" -ForegroundColor Green
}

# 2. Determine allowed origins
$allowedOrigins = @(
    "chrome-extension://aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa/",
    "chrome-extension://ngpampappnmepgilojfohadhhmbhlaek/",
    "chrome-extension://llbjbkhnmlidjebalopleeepgdfgcpec/"
)

if ($ExtensionId) {
    $cleanId = $ExtensionId.Trim()
    $formattedOrigin = "chrome-extension://$cleanId/"
    if ($allowedOrigins -notcontains $formattedOrigin) {
        $allowedOrigins += $formattedOrigin
    }
    Write-Host "[+] Added custom Extension ID origin: $formattedOrigin" -ForegroundColor Green
}

# 3. Create Manifest Directory in %LOCALAPPDATA%\Flux Download Manager\manifests
$manifestDir = Join-Path $env:LOCALAPPDATA "Flux Download Manager\manifests"
if (-not (Test-Path $manifestDir)) {
    New-Item -ItemType Directory -Force -Path $manifestDir | Out-Null
}

$chromiumManifestPath = Join-Path $manifestDir "com.flux.downloadmanager.chromium.json"
$firefoxManifestPath = Join-Path $manifestDir "com.flux.downloadmanager.firefox.json"

if ($Unregister) {
    Write-Host "`n==> Removing Native Messaging Registrations from HKCU..." -ForegroundColor Yellow
    $regPaths = @(
        "HKCU:\Software\Google\Chrome\NativeMessagingHosts\com.flux.downloadmanager",
        "HKCU:\Software\Microsoft\Edge\NativeMessagingHosts\com.flux.downloadmanager",
        "HKCU:\Software\BraveSoftware\Brave-Browser\NativeMessagingHosts\com.flux.downloadmanager",
        "HKCU:\Software\Mozilla\NativeMessagingHosts\com.flux.downloadmanager",
        "HKCU:\Software\Google\Chrome\NativeMessagingHosts\org.flux.native",
        "HKCU:\Software\Microsoft\Edge\NativeMessagingHosts\org.flux.native"
    )
    foreach ($rp in $regPaths) {
        if (Test-Path $rp) {
            Remove-Item -Path $rp -Recurse -Force
            Write-Host "  [-] Removed: $rp" -ForegroundColor Gray
        }
    }
    Write-Host "`nUnregistration complete." -ForegroundColor Green
    exit 0
}

# 4. Generate Chromium Manifest
$chromiumManifest = @{
    name = "com.flux.downloadmanager"
    description = "Flux Download Manager Native Messaging Host"
    path = $HostExePath
    type = "stdio"
    allowed_origins = $allowedOrigins
}
$chromiumJson = $chromiumManifest | ConvertTo-Json -Depth 5
Set-Content -Path $chromiumManifestPath -Value $chromiumJson -Encoding UTF8
Write-Host "[+] Wrote Chromium manifest: $chromiumManifestPath" -ForegroundColor Green

# 5. Generate Firefox Manifest
$firefoxManifest = @{
    name = "com.flux.downloadmanager"
    description = "Flux Download Manager Native Messaging Host"
    path = $HostExePath
    type = "stdio"
    allowed_extensions = @("flux@openidm.org", "flux@downloadmanager.org")
}
$firefoxJson = $firefoxManifest | ConvertTo-Json -Depth 5
Set-Content -Path $firefoxManifestPath -Value $firefoxJson -Encoding UTF8
Write-Host "[+] Wrote Firefox manifest: $firefoxManifestPath" -ForegroundColor Green

# 6. Register HKCU Keys for Browsers
Write-Host "`n==> Registering Browser Native Messaging Keys in HKCU..." -ForegroundColor Cyan

$targets = @(
    @{ Name = "Google Chrome"; Key = "HKCU:\Software\Google\Chrome\NativeMessagingHosts\com.flux.downloadmanager"; Manifest = $chromiumManifestPath },
    @{ Name = "Microsoft Edge"; Key = "HKCU:\Software\Microsoft\Edge\NativeMessagingHosts\com.flux.downloadmanager"; Manifest = $chromiumManifestPath },
    @{ Name = "Brave Browser"; Key = "HKCU:\Software\BraveSoftware\Brave-Browser\NativeMessagingHosts\com.flux.downloadmanager"; Manifest = $chromiumManifestPath },
    @{ Name = "Mozilla Firefox"; Key = "HKCU:\Software\Mozilla\NativeMessagingHosts\com.flux.downloadmanager"; Manifest = $firefoxManifestPath }
)

foreach ($t in $targets) {
    $parent = Split-Path -Parent $t.Key
    if (-not (Test-Path $parent)) {
        New-Item -ItemType Directory -Force -Path $parent | Out-Null
    }
    New-Item -ItemType Directory -Force -Path $t.Key | Out-Null
    Set-ItemProperty -Path $t.Key -Name "(Default)" -Value $t.Manifest
    Write-Host "  [OK] $($t.Name) -> $($t.Key)" -ForegroundColor Green
}

# Also register backwards-compatible alias org.flux.native
$aliasKey = "HKCU:\Software\Google\Chrome\NativeMessagingHosts\org.flux.native"
$aliasParent = Split-Path -Parent $aliasKey
if (-not (Test-Path $aliasParent)) { New-Item -ItemType Directory -Force -Path $aliasParent | Out-Null }
New-Item -ItemType Directory -Force -Path $aliasKey | Out-Null
Set-ItemProperty -Path $aliasKey -Name "(Default)" -Value $chromiumManifestPath

Write-Host "`n=================================================================" -ForegroundColor Cyan
Write-Host "  SUCCESS: Native Messaging Host Registered Successfully!       " -ForegroundColor Green
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "Next Steps for Browser Integration:" -ForegroundColor White
Write-Host "1. In Google Chrome or Microsoft Edge, navigate to: chrome://extensions or edge://extensions" -ForegroundColor Gray
Write-Host "2. Enable 'Developer mode' toggle in the top-right corner." -ForegroundColor Gray
Write-Host "3. Click 'Load unpacked' and select the folder: $RepoRoot\extension" -ForegroundColor Gray
Write-Host "4. If the browser generates a unique Extension ID (e.g., 'abcdef...'):" -ForegroundColor Gray
Write-Host "   Re-run this script with: .\scripts\register-native-messaging.ps1 -ExtensionId <your_id>" -ForegroundColor Yellow
Write-Host "5. Open Flux Download Manager (flux.exe) and verify that the extension popup displays 'Desktop Connected'." -ForegroundColor White
