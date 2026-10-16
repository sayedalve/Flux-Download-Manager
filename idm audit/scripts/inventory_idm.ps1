# inventory_idm.ps1
# Recursively inventories the IDM installation: hashes, PE architecture, metadata, key strings

[CmdletBinding()]
param(
    [string]$IdmDir = "C:\Program Files (x86)\Internet Download Manager",
    [string]$OutputDir = "C:\Users\sayed\Downloads\IDM_RE_Investigation\inventory"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $IdmDir)) {
    Write-Error "IDM installation directory does not exist: $IdmDir"
    exit 1
}

if (-not (Test-Path -LiteralPath $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
}

function Get-PEArchitecture {
    param([string]$FilePath)
    try {
        $stream = [System.IO.File]::OpenRead($FilePath)
        $reader = New-Object System.IO.BinaryReader($stream)
        
        if ($stream.Length -lt 64) {
            $stream.Close()
            return "Non-PE (Too small)"
        }
        
        $mz = $reader.ReadUInt16()
        if ($mz -ne 0x5A4D) { # 'MZ'
            $stream.Close()
            return "Non-PE"
        }
        
        $stream.Seek(0x3C, [System.IO.SeekOrigin]::Begin) | Out-Null
        $peOffset = $reader.ReadUInt32()
        
        if ($peOffset + 6 -gt $stream.Length) {
            $stream.Close()
            return "Corrupted PE"
        }
        
        $stream.Seek($peOffset, [System.IO.SeekOrigin]::Begin) | Out-Null
        $peSig = $reader.ReadUInt32()
        if ($peSig -ne 0x00004550) { # 'PE\0\0'
            $stream.Close()
            return "Invalid PE Signature"
        }
        
        $machine = $reader.ReadUInt16()
        $stream.Close()
        
        switch ($machine) {
            0x014c { return "PE32 (x86 32-bit)" }
            0x8664 { return "PE32+ (x64 64-bit)" }
            0xAA64 { return "PE32+ (ARM64)" }
            0x0200 { return "PE32+ (Itanium)" }
            default { return ("Unknown PE (0x{0:X4})" -f $machine) }
        }
    }
    catch {
        return "Error reading PE"
    }
}

function Scan-InterestingStrings {
    param([string]$FilePath)
    $patterns = @(
        'video/mp4', 'video/webm', 'video/MP2T', 'application/dash\+xml', 'application/vnd\.apple\.mpegurl',
        'mpdatoidm', 'mpdtoidm', 'Download this video', 'com\.tonec\.idm', 'idmBroker', 'IDMMsgHost',
        'downlWithIDM', 'idmbrbtn', 'idmcchandler', 'IDMNetMon', 'IDMVMPrs', 'idmvconv', 'idmvs'
    )
    
    $found = [System.Collections.Generic.List[string]]::new()
    try {
        $bytes = [System.IO.File]::ReadAllBytes($FilePath)
        # Search ASCII strings
        $asciiText = [System.Text.Encoding]::ASCII.GetString($bytes)
        # Search Unicode UTF-16LE strings
        $unicodeText = [System.Text.Encoding]::Unicode.GetString($bytes)
        
        foreach ($pattern in $patterns) {
            if ($asciiText -match $pattern -or $unicodeText -match $pattern) {
                $found.Add($pattern)
            }
        }
    }
    catch {
        # File might be locked or unreadable
    }
    return ($found -join "; ")
}

Write-Host "Scanning IDM directory: $IdmDir"
$files = Get-ChildItem -LiteralPath $IdmDir -Recurse -File

$results = [System.Collections.Generic.List[PSCustomObject]]::new()

$count = 0
foreach ($file in $files) {
    $count++
    $relPath = $file.FullName.Substring($IdmDir.Length).TrimStart('\', '/')
    Write-Progress -Activity "Inventorying IDM files" -Status "$relPath ($count / $($files.Count))" -PercentComplete (($count / $files.Count) * 100)
    
    $sha256 = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
    $peArch = Get-PEArchitecture -FilePath $file.FullName
    
    $verInfo = [System.Diagnostics.FileVersionInfo]::GetVersionInfo($file.FullName)
    $interesting = ""
    if ($peArch -like "PE*") {
        $interesting = Scan-InterestingStrings -FilePath $file.FullName
    }

    $item = [PSCustomObject]@{
        RelativePath     = $relPath
        Name             = $file.Name
        Extension        = $file.Extension.ToLower()
        SizeBytes        = $file.Length
        SHA256           = $sha256
        Architecture     = $peArch
        FileDescription  = $verInfo.FileDescription
        FileVersion      = $verInfo.FileVersion
        ProductName      = $verInfo.ProductName
        ProductVersion   = $verInfo.ProductVersion
        CompanyName      = $verInfo.CompanyName
        LastWriteTimeUtc = $file.LastWriteTimeUtc.ToString("o")
        MatchedPatterns  = $interesting
    }
    $results.Add($item)
}

# Export outputs
$jsonPath = Join-Path $OutputDir "idm_files_inventory.json"
$csvPath  = Join-Path $OutputDir "idm_files_inventory.csv"
$mdPath   = Join-Path $OutputDir "idm_files_inventory.md"

$results | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath $jsonPath -Encoding utf8
$results | Export-Csv -LiteralPath $csvPath -NoTypeInformation -Encoding utf8

# Generate Markdown Summary
$sb = [System.Text.StringBuilder]::new()
[void]$sb.AppendLine("# IDM Installation Inventory")
[void]$sb.AppendLine("")
[void]$sb.AppendLine(('- **Source Directory**: `{0}`' -f $IdmDir))
[void]$sb.AppendLine(('- **Total Files**: {0}' -f $results.Count))
[void]$sb.AppendLine(('- **Scan Timestamp**: {0}' -f (Get-Date).ToString('yyyy-MM-dd HH:mm:ss UTC')))
[void]$sb.AppendLine("")
[void]$sb.AppendLine("## Summary by Architecture")
[void]$sb.AppendLine("")
$archGroups = $results | Group-Object Architecture
foreach ($g in $archGroups) {
    [void]$sb.AppendLine(('- **{0}**: {1} files' -f $g.Name, $g.Count))
}
[void]$sb.AppendLine("")
[void]$sb.AppendLine("## Key Executables and Dynamic Libraries")
[void]$sb.AppendLine("")
[void]$sb.AppendLine("| Relative Path | Size (bytes) | Arch | Description | File Version | Matched Signatures |")
[void]$sb.AppendLine("|---|---|---|---|---|---|")

$keyFiles = $results | Where-Object { $_.Architecture -like "PE*" -or $_.Extension -in ".crx", ".xpi", ".json", ".sys" } | Sort-Object RelativePath
foreach ($k in $keyFiles) {
    $desc = if ($k.FileDescription) { $k.FileDescription.Replace("|", "/") } else { "" }
    $ver  = if ($k.FileVersion) { $k.FileVersion.Replace("|", "/") } else { "" }
    $pat  = if ($k.MatchedPatterns) { $k.MatchedPatterns.Replace("|", "/") } else { "" }
    [void]$sb.AppendLine(('| `{0}` | {1} | {2} | {3} | {4} | {5} |' -f $k.RelativePath, $k.SizeBytes, $k.Architecture, $desc, $ver, $pat))
}

$sb.ToString() | Set-Content -LiteralPath $mdPath -Encoding utf8

Write-Host "Inventory completed successfully. Results saved to:"
Write-Host "  - JSON: $jsonPath"
Write-Host "  - CSV:  $csvPath"
Write-Host "  - MD:   $mdPath"
