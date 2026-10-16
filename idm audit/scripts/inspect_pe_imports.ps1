# inspect_pe_imports.ps1
# Extracts exported functions and string references related to IPC and video handling from key IDM binaries

param(
    [string]$IdmDir = "C:\Program Files (x86)\Internet Download Manager",
    [string]$OutputDir = "C:\Users\sayed\Downloads\IDM_RE_Investigation\inventory"
)

$ErrorActionPreference = "Stop"

$targets = @("IDMMsgHost.exe", "idmBroker.exe", "idmbrbtn.dll", "idmcchandler7.dll", "IDMVMPrs.dll")

$ipcKeywords = @(
    "CreateNamedPipe", "ConnectNamedPipe", "CallNamedPipe", "WaitNamedPipe",
    "CreateFileMapping", "OpenFileMapping", "MapViewOfFile",
    "SendMessage", "PostMessage", "FindWindow", "RegisterWindowMessage", "WM_COPYDATA",
    "socket", "connect", "bind", "listen", "accept", "WSASocket",
    "CreateProcess", "ShellExecute", "WinHttp", "InternetOpen",
    "pipe", "IDMan", "tonec", "Download this video", "video", "m3u8", "mpd"
)

$results = [System.Collections.Generic.List[PSCustomObject]]::new()

foreach ($t in $targets) {
    $filePath = Join-Path $IdmDir $t
    if (-not (Test-Path -LiteralPath $filePath)) { continue }
    
    $bytes = [System.IO.File]::ReadAllBytes($filePath)
    $ascii = [System.Text.Encoding]::ASCII.GetString($bytes)
    $unicode = [System.Text.Encoding]::Unicode.GetString($bytes)
    
    $foundMatches = [System.Collections.Generic.List[string]]::new()
    foreach ($kw in $ipcKeywords) {
        if ($ascii -match [regex]::Escape($kw) -or $unicode -match [regex]::Escape($kw)) {
            $foundMatches.Add($kw)
        }
    }
    
    # Also extract printable strings >= 6 chars matching pipe, idm, window, msg, video
    $regex = [regex]'[a-zA-Z0-9_\-\.\:\/\\\#]{6,}'
    $asciiMatches = $regex.Matches($ascii) | ForEach-Object { $_.Value } | Where-Object {
        $_ -match 'pipe|idm|tonec|msg|hook|video|downl|broker|cchandler|wm_|shared'
    } | Select-Object -Unique -First 40

    $results.Add([PSCustomObject]@{
        BinaryName   = $t
        SizeBytes    = $bytes.Length
        IpcKeywords  = ($foundMatches -join ", ")
        SampleStrings = ($asciiMatches -join " | ")
    })
}

$outPath = Join-Path $OutputDir "ipc_and_strings_quickscan.json"
$results | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath $outPath -Encoding utf8

$mdPath = Join-Path $OutputDir "ipc_and_strings_quickscan.md"
$sb = [System.Text.StringBuilder]::new()
[void]$sb.AppendLine('# Key Binaries IPC & String Quick Scan')
[void]$sb.AppendLine('')
foreach ($r in $results) {
    [void]$sb.AppendLine(('## `{0}` ({1} bytes)' -f $r.BinaryName, $r.SizeBytes))
    [void]$sb.AppendLine(('- **Matched IPC / API Keywords**: {0}' -f $r.IpcKeywords))
    [void]$sb.AppendLine('- **Notable Strings**:')
    [void]$sb.AppendLine('```')
    [void]$sb.AppendLine(($r.SampleStrings -replace ' \| ', "`n"))
    [void]$sb.AppendLine('```')
    [void]$sb.AppendLine('')
}
$sb.ToString() | Set-Content -LiteralPath $mdPath -Encoding utf8

Write-Host "Quick scan saved to: $mdPath"
