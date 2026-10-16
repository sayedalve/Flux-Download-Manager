# inspect_registry_nm.ps1
# Read-only inspection of Native Messaging registry keys and manifest JSON files

param(
    [string]$OutputDir = "C:\Users\sayed\Downloads\IDM_RE_Investigation\registry"
)

$ErrorActionPreference = "Continue"

if (-not (Test-Path -LiteralPath $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}

$regPaths = @(
    "HKCU:\Software\Google\Chrome\NativeMessagingHosts",
    "HKLM:\Software\Google\Chrome\NativeMessagingHosts",
    "HKLM:\Software\Wow6432Node\Google\Chrome\NativeMessagingHosts",
    "HKCU:\Software\Microsoft\Edge\NativeMessagingHosts",
    "HKLM:\Software\Microsoft\Edge\NativeMessagingHosts",
    "HKLM:\Software\Wow6432Node\Microsoft\Edge\NativeMessagingHosts",
    "HKCU:\Software\Mozilla\NativeMessagingHosts",
    "HKLM:\Software\Mozilla\NativeMessagingHosts",
    "HKLM:\Software\Wow6432Node\Mozilla\NativeMessagingHosts"
)

$manifestsFound = [System.Collections.Generic.List[PSCustomObject]]::new()

foreach ($rp in $regPaths) {
    if (Test-Path -LiteralPath $rp) {
        Write-Host "Checking: $rp"
        $subkeys = Get-ChildItem -LiteralPath $rp
        foreach ($sub in $subkeys) {
            $name = $sub.PSChildName
            $val = (Get-ItemProperty -LiteralPath $sub.PSPath).'(default)'
            Write-Host "  Found registration: $name -> $val"
            
            $manifestContent = $null
            $manifestObj = $null
            $execPath = $null
            $execExists = $false
            $execSha256 = $null
            
            if ($val -and (Test-Path -LiteralPath $val)) {
                try {
                    $manifestContent = Get-Content -LiteralPath $val -Raw -Encoding utf8
                    $manifestObj = $manifestContent | ConvertFrom-Json
                    $execPath = $manifestObj.path
                    if ($execPath -and (Test-Path -LiteralPath $execPath)) {
                        $execExists = $true
                        $execSha256 = (Get-FileHash -LiteralPath $execPath -Algorithm SHA256).Hash
                    }
                }
                catch {
                    Write-Warning "Could not read or parse manifest: $val"
                }
            }
            
            $manifestsFound.Add([PSCustomObject]@{
                RegistryRoot     = $rp
                HostName         = $name
                ManifestPath     = $val
                ManifestExists   = (Test-Path -LiteralPath $val)
                ExecutablePath   = $execPath
                ExecutableExists = $execExists
                ExecutableSHA256 = $execSha256
                ManifestContent  = $manifestObj
            })
        }
    }
}

# Also inspect the manifest files in IDM installation directory directly
$idmDir = "C:\Program Files (x86)\Internet Download Manager"
$idmJsonFiles = Get-ChildItem -LiteralPath $idmDir -Filter "*.json" -File
foreach ($jf in $idmJsonFiles) {
    try {
        $raw = Get-Content -LiteralPath $jf.FullName -Raw -Encoding utf8
        $obj = $raw | ConvertFrom-Json
        $copyTarget = Join-Path $OutputDir $jf.Name
        Copy-Item -LiteralPath $jf.FullName -Destination $copyTarget -Force
        
        $manifestsFound.Add([PSCustomObject]@{
            RegistryRoot     = "Direct IDM Folder Inspection"
            HostName         = $obj.name
            ManifestPath     = $jf.FullName
            ManifestExists   = $true
            ExecutablePath   = $obj.path
            ExecutableExists = (Test-Path -LiteralPath $obj.path)
            ExecutableSHA256 = if (Test-Path -LiteralPath $obj.path) { (Get-FileHash -LiteralPath $obj.path -Algorithm SHA256).Hash } else { $null }
            ManifestContent  = $obj
        })
    } catch {
        Write-Warning "Could not parse IDM json: $($jf.FullName)"
    }
}

# Export JSON
$jsonOut = Join-Path $OutputDir "native_messaging_registrations.json"
$manifestsFound | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $jsonOut -Encoding utf8

# Export Markdown
$mdOut = Join-Path $OutputDir "native_messaging_report.md"
$sb = [System.Text.StringBuilder]::new()
[void]$sb.AppendLine('# Native Messaging Registrations & Host Manifests')
[void]$sb.AppendLine('')
[void]$sb.AppendLine(('Report generated at: {0}' -f (Get-Date).ToString('yyyy-MM-dd HH:mm:ss UTC')))
[void]$sb.AppendLine('')
[void]$sb.AppendLine('| Browser / Hive | Host Name | Manifest Path | Executable Target | Exists |')
[void]$sb.AppendLine('|---|---|---|---|---|')

foreach ($m in $manifestsFound) {
    [void]$sb.AppendLine(('| `{0}` | `{1}` | `{2}` | `{3}` | {4} |' -f $m.RegistryRoot, $m.HostName, $m.ManifestPath, $m.ExecutablePath, $m.ExecutableExists))
}

[void]$sb.AppendLine('')
[void]$sb.AppendLine('## Detailed Manifest Contents')
[void]$sb.AppendLine('')
foreach ($m in $manifestsFound) {
    [void]$sb.AppendLine(('### Host: `{0}` ({1})' -f $m.HostName, $m.RegistryRoot))
    [void]$sb.AppendLine('')
    [void]$sb.AppendLine(('- **Manifest Path**: `{0}`' -f $m.ManifestPath))
    [void]$sb.AppendLine(('- **Binary Target**: `{0}` (SHA-256: `{1}`)' -f $m.ExecutablePath, $m.ExecutableSHA256))
    [void]$sb.AppendLine('```json')
    [void]$sb.AppendLine(($m.ManifestContent | ConvertTo-Json -Depth 5))
    [void]$sb.AppendLine('```')
    [void]$sb.AppendLine('')
}

$sb.ToString() | Set-Content -LiteralPath $mdOut -Encoding utf8

Write-Host "Inspection complete. Saved to:"
Write-Host "  - JSON: $jsonOut"
Write-Host "  - MD:   $mdOut"
