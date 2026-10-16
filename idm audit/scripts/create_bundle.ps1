# create_bundle.ps1
# Creates the final IDM_RE_Results_Bundle.zip package

[CmdletBinding()]
param(
    [string]$Workspace = "C:\Users\sayed\Downloads\IDM_RE_Investigation",
    [string]$ZipOutput = "C:\Users\sayed\Downloads\IDM_RE_Investigation\IDM_RE_Results_Bundle.zip"
)

$ErrorActionPreference = "Stop"

Write-Host ('=== Preparing IDM Reverse Engineering Results Bundle ===')

if (Test-Path -LiteralPath $ZipOutput) {
    Remove-Item -LiteralPath $ZipOutput -Force
    Write-Host ('Removed existing bundle: {0}' -f $ZipOutput)
}

$stagingDir = Join-Path $Workspace "staging_bundle"
if (Test-Path -LiteralPath $stagingDir) {
    Remove-Item -LiteralPath $stagingDir -Recurse -Force
}
New-Item -ItemType Directory -Path $stagingDir -Force | Out-Null

$itemsToCopy = @(
    "README.md",
    "inventory",
    "registry",
    "extensions",
    "reports",
    "scripts"
)

foreach ($item in $itemsToCopy) {
    $src = Join-Path $Workspace $item
    if (Test-Path -LiteralPath $src) {
        Copy-Item -Path $src -Destination (Join-Path $stagingDir $item) -Recurse -Force
        Write-Host ('Included: {0}' -f $item)
    } else {
        Write-Warning ('Item not found: {0}' -f $src)
    }
}

# Include Ghidra reports, logs, scripts, and decompiled code (exclude heavy projects cache)
$ghidraStaging = Join-Path $stagingDir "ghidra"
New-Item -ItemType Directory -Path $ghidraStaging -Force | Out-Null

$ghidraSubdirs = @("reports", "scripts", "decompiled")
foreach ($sub in $ghidraSubdirs) {
    $src = Join-Path $Workspace ('ghidra\{0}' -f $sub)
    if (Test-Path -LiteralPath $src) {
        Copy-Item -Path $src -Destination (Join-Path $ghidraStaging $sub) -Recurse -Force
        Write-Host ('Included Ghidra: {0}' -f $sub)
    }
}

Write-Host ('Creating archive at: {0}' -f $ZipOutput)
Compress-Archive -Path ('{0}\*' -f $stagingDir) -DestinationPath $ZipOutput -CompressionLevel Optimal

# Cleanup staging directory
Remove-Item -LiteralPath $stagingDir -Recurse -Force

if (Test-Path -LiteralPath $ZipOutput) {
    $zipItem = Get-Item -LiteralPath $ZipOutput
    Write-Host ('=== Bundle Created Successfully ===')
    Write-Host ('Path: {0}' -f $zipItem.FullName)
    Write-Host ('Size: {0:N0} bytes ({1:N2} MB)' -f $zipItem.Length, ($zipItem.Length / 1MB))
} else {
    Write-Error ('Failed to create archive at {0}' -f $ZipOutput)
}
