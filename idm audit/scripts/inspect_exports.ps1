# inspect_exports.ps1
# Inspect PE export directory for DLLs

[CmdletBinding()]
param(
    [string[]]$Files = @(
        "C:\Users\sayed\Downloads\IDM_RE_Investigation\binaries\IDMVMPrs.dll",
        "C:\Users\sayed\Downloads\IDM_RE_Investigation\binaries\idmbrbtn.dll",
        "C:\Users\sayed\Downloads\IDM_RE_Investigation\binaries\idmcchandler7.dll",
        "C:\Users\sayed\Downloads\IDM_RE_Investigation\binaries\IDMNetMon.dll"
    )
)

function Get-PEExports([string]$filePath) {
    $bytes = [System.IO.File]::ReadAllBytes($filePath)
    if ($bytes.Length -lt 64) { return @() }
    
    # Check MZ header
    if ($bytes[0] -ne 0x4D -or $bytes[1] -ne 0x5A) { return @() }
    
    $e_lfanew = [BitConverter]::ToInt32($bytes, 0x3C)
    if ($e_lfanew -lt 0 -or $e_lfanew + 0x100 -gt $bytes.Length) { return @() }
    
    # Check PE signature
    if ($bytes[$e_lfanew] -ne 0x50 -or $bytes[$e_lfanew+1] -ne 0x45) { return @() }
    
    $optHeaderOffset = $e_lfanew + 0x18
    $exportDirOffset = if ($magic -eq 0x10B) { $optHeaderOffset + 0x60 } else { $optHeaderOffset + 0x70 }
    
    $exportRva  = [BitConverter]::ToUInt32($bytes, $exportDirOffset)
    $exportSize = [BitConverter]::ToUInt32($bytes, $exportDirOffset + 4)
    if ($exportRva -eq 0 -or $exportSize -eq 0) { return @() }
    
    # Section headers to map RVA to file offset
    $numSections = [BitConverter]::ToUInt16($bytes, $e_lfanew + 6)
    $optHeaderSize = [BitConverter]::ToUInt16($bytes, $e_lfanew + 0x14)
    $secTableOffset = $e_lfanew + 0x18 + $optHeaderSize
    
    $sections = @()
    for ($i = 0; $i -lt $numSections; $i++) {
        $secOffset = $secTableOffset + ($i * 40)
        $secName = [System.Text.Encoding]::ASCII.GetString($bytes, $secOffset, 8).TrimEnd("`0")
        $vSize   = [BitConverter]::ToUInt32($bytes, $secOffset + 8)
        $vAddr   = [BitConverter]::ToUInt32($bytes, $secOffset + 12)
        $rSize   = [BitConverter]::ToUInt32($bytes, $secOffset + 16)
        $rOffset = [BitConverter]::ToUInt32($bytes, $secOffset + 20)
        $sections += [PSCustomObject]@{
            Name = $secName; VirtualAddress = $vAddr; VirtualSize = $vSize; RawOffset = $rOffset; RawSize = $rSize
        }
    }
    
    function RvaToOffset($rva) {
        foreach ($sec in $sections) {
            if ($rva -ge $sec.VirtualAddress -and $rva -lt ($sec.VirtualAddress + $sec.VirtualSize)) {
                return $sec.RawOffset + ($rva - $sec.VirtualAddress)
            }
        }
        return $null
    }
    
    $expFileOffset = RvaToOffset $exportRva
    if ($null -eq $expFileOffset) { return @() }
    
    $numNames   = [BitConverter]::ToUInt32($bytes, $expFileOffset + 0x18)
    $funcRva    = [BitConverter]::ToUInt32($bytes, $expFileOffset + 0x1C)
    $nameRva    = [BitConverter]::ToUInt32($bytes, $expFileOffset + 0x20)
    $ordRva     = [BitConverter]::ToUInt32($bytes, $expFileOffset + 0x24)
    
    $nameTableOffset = RvaToOffset $nameRva
    $ordTableOffset  = RvaToOffset $ordRva
    $funcTableOffset = RvaToOffset $funcRva
    
    $exports = [System.Collections.Generic.List[PSCustomObject]]::new()
    for ($i = 0; $i -lt $numNames; $i++) {
        $curNameRva = [BitConverter]::ToUInt32($bytes, $nameTableOffset + ($i * 4))
        $curNameOffset = RvaToOffset $curNameRva
        if ($null -ne $curNameOffset) {
            $end = $curNameOffset
            while ($end -lt $bytes.Length -and $bytes[$end] -ne 0) { $end++ }
            $name = [System.Text.Encoding]::ASCII.GetString($bytes, $curNameOffset, $end - $curNameOffset)
            
            $ordinal = [BitConverter]::ToUInt16($bytes, $ordTableOffset + ($i * 2))
            $funcAddr = [BitConverter]::ToUInt32($bytes, $funcTableOffset + ($ordinal * 4))
            
            $exports.Add([PSCustomObject]@{
                Name    = $name
                Ordinal = $ordinal
                RVA     = ('0x{0:X8}' -f $funcAddr)
            })
        }
    }
    return $exports
}

foreach ($f in $Files) {
    if (Test-Path -LiteralPath $f) {
        $exps = Get-PEExports $f
        Write-Host ('=== Exports for {0} (Count: {1}) ===' -f (Split-Path $f -Leaf), $exps.Count)
        $exps | Format-Table -AutoSize
    }
}
