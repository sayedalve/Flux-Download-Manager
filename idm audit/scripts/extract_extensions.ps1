# extract_extensions.ps1
# Extracts IDM browser extension packages (.crx and .xpi) and analyzes their source

param(
    [string]$IdmDir = "C:\Program Files (x86)\Internet Download Manager",
    [string]$OutputDir = "C:\Users\sayed\Downloads\IDM_RE_Investigation\extensions"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}

function Extract-CrxOrZip {
    param(
        [string]$SourceFile,
        [string]$DestFolder
    )
    
    if (-not (Test-Path -LiteralPath $DestFolder)) {
        New-Item -ItemType Directory -Path $DestFolder -Force | Out-Null
    }
    
    $bytes = [System.IO.File]::ReadAllBytes($SourceFile)
    
    # Check if standard ZIP (starts with PK)
    $zipOffset = -1
    for ($i = 0; $i -lt ($bytes.Length - 4); $i++) {
        if ($bytes[$i] -eq 0x50 -and $bytes[$i+1] -eq 0x4B -and $bytes[$i+2] -eq 0x03 -and $bytes[$i+3] -eq 0x04) {
            $zipOffset = $i
            break
        }
    }
    
    if ($zipOffset -lt 0) {
        Write-Warning "Could not find ZIP header in $SourceFile"
        return $false
    }
    
    Write-Host ("Extracting {0} (ZIP payload offset: {1})..." -f (Split-Path -Leaf $SourceFile), $zipOffset)
    $zipBytes = New-Object byte[] ($bytes.Length - $zipOffset)
    [System.Buffer]::BlockCopy($bytes, $zipOffset, $zipBytes, 0, $zipBytes.Length)
    
    $tempZip = Join-Path $DestFolder "package.zip"
    [System.IO.File]::WriteAllBytes($tempZip, $zipBytes)
    
    try {
        Expand-Archive -LiteralPath $tempZip -DestinationPath $DestFolder -Force
        Remove-Item -LiteralPath $tempZip -Force
        return $true
    } catch {
        Write-Warning ("Failed to unpack {0}: {1}" -f $tempZip, $_.Exception.Message)
        return $false
    }
}

$extFiles = @("IDMGCExt.crx", "IDMEdgeExt.crx", "IDMGCExt59.crx", "idmmzcc3.xpi")

$summary = [System.Collections.Generic.List[PSCustomObject]]::new()

foreach ($extName in $extFiles) {
    $src = Join-Path $IdmDir $extName
    if (-not (Test-Path -LiteralPath $src)) {
        Write-Warning ("File not found: {0}" -f $src)
        continue
    }
    
    $subDirName = [System.IO.Path]::GetFileNameWithoutExtension($extName)
    $dest = Join-Path $OutputDir $subDirName
    
    $ok = Extract-CrxOrZip -SourceFile $src -DestFolder $dest
    
    if ($ok) {
        $manifestPath = Join-Path $dest "manifest.json"
        $manifest = $null
        if (Test-Path -LiteralPath $manifestPath) {
            $manifest = Get-Content -LiteralPath $manifestPath -Raw -Encoding utf8 | ConvertFrom-Json
        }
        
        $allExtFiles = Get-ChildItem -LiteralPath $dest -Recurse -File
        
        $summary.Add([PSCustomObject]@{
            PackageName       = $extName
            TargetDir         = $dest
            ManifestVersion   = if ($manifest) { $manifest.manifest_version } else { "N/A" }
            ExtensionVersion  = if ($manifest) { $manifest.version } else { "N/A" }
            ExtensionName     = if ($manifest) { $manifest.name } else { "N/A" }
            Permissions       = if ($manifest) { ($manifest.permissions -join ", ") } else { "" }
            BackgroundScripts = if ($manifest -and $manifest.background) { ($manifest.background | ConvertTo-Json -Compress) } else { "" }
            ContentScripts    = if ($manifest -and $manifest.content_scripts) { ($manifest.content_scripts | ConvertTo-Json -Compress) } else { "" }
            FileCount         = $allExtFiles.Count
        })
    }
}

# Export Summary
$jsonOut = Join-Path $OutputDir "extracted_extensions_summary.json"
$summary | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $jsonOut -Encoding utf8

$mdOut = Join-Path $OutputDir "extensions_analysis.md"
$sb = [System.Text.StringBuilder]::new()
[void]$sb.AppendLine('# IDM Browser Extensions Extraction and Analysis')
[void]$sb.AppendLine('')
[void]$sb.AppendLine(('Extracted at: {0}' -f (Get-Date).ToString('yyyy-MM-dd HH:mm:ss UTC')))
[void]$sb.AppendLine('')
[void]$sb.AppendLine('| Package | Ext Name | Ext Version | Manifest Ver | Files | Permissions |')
[void]$sb.AppendLine('|---|---|---|---|---|---|')

foreach ($s in $summary) {
    [void]$sb.AppendLine(('| `{0}` | {1} | {2} | {3} | {4} | {5} |' -f $s.PackageName, $s.ExtensionName, $s.ExtensionVersion, $s.ManifestVersion, $s.FileCount, $s.Permissions))
}

[void]$sb.AppendLine('')
$sb.ToString() | Set-Content -LiteralPath $mdOut -Encoding utf8

Write-Host "Extension extraction complete. Summary saved to:"
Write-Host "  - JSON: $jsonOut"
Write-Host "  - MD:   $mdOut"
