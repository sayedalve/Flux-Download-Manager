# launch_ghidra_analysis.ps1
# Standalone PowerShell launcher for Ghidra headless analysis on IDM binaries

[CmdletBinding()]
param(
    [string[]]$Targets = @("IDMMsgHost.exe", "idmbrbtn.dll", "idmcchandler7.dll"),
    [string]$IdmDir = "C:\Program Files (x86)\Internet Download Manager",
    [string]$GhidraHome = "C:\Users\sayed\Downloads\ghidra\ghidra_12.1.4_PUBLIC",
    [string]$JavaHome = "C:\Program Files\Eclipse Adoptium\jdk-25.0.4.101-hotspot",
    [string]$Workspace = "C:\Users\sayed\Downloads\IDM_RE_Investigation"
)

$ErrorActionPreference = "Stop"

$projectDir = Join-Path $Workspace "ghidra\projects"
$scriptDir  = Join-Path $Workspace "ghidra\scripts"
$reportDir  = Join-Path $Workspace "ghidra\reports"
$scriptName = "IDMDeepAnalysisScript.java"

if (-not (Test-Path -LiteralPath $projectDir)) { New-Item -ItemType Directory -Path $projectDir -Force | Out-Null }
if (-not (Test-Path -LiteralPath $reportDir))  { New-Item -ItemType Directory -Path $reportDir -Force | Out-Null }

$headlessBat = Join-Path $GhidraHome "support\analyzeHeadless.bat"
if (-not (Test-Path -LiteralPath $headlessBat)) {
    Write-Error ('Ghidra headless script not found at: {0}' -f $headlessBat)
    exit 1
}

if (-not (Test-Path -LiteralPath $JavaHome)) {
    Write-Error ('Java home directory not found: {0}' -f $JavaHome)
    exit 1
}

# Set JAVA_HOME and PATH in current process
$env:JAVA_HOME = $JavaHome
$env:PATH = ('{0}\bin;{1}' -f $JavaHome, $env:PATH)

Write-Host ('Using Ghidra at: {0}' -f $GhidraHome)
Write-Host ('Using Java at:   {0}' -f $JavaHome)

$binariesDir = Join-Path $Workspace "binaries"
if (-not (Test-Path -LiteralPath $binariesDir)) { New-Item -ItemType Directory -Path $binariesDir -Force | Out-Null }

$results = [System.Collections.Generic.List[PSCustomObject]]::new()

foreach ($targetName in $Targets) {
    $binaryPath = Join-Path $binariesDir $targetName
    if (-not (Test-Path -LiteralPath $binaryPath)) {
        $binaryPath = Join-Path $IdmDir $targetName
    }
    if (-not (Test-Path -LiteralPath $binaryPath)) {
        Write-Warning ('Target binary not found: {0}' -f $binaryPath)
        continue
    }

    $projectName = ('proj_{0}' -f ($targetName -replace '\.', '_'))
    $logFile = Join-Path $reportDir ('{0}_headless.log' -f $targetName)

    Write-Host ('------------------------------------------------------------')
    Write-Host ('Launching Ghidra Headless on: {0}' -f $targetName)
    Write-Host ('  Source:  {0}' -f $binaryPath)
    Write-Host ('  Project: {0} in {1}' -f $projectName, $projectDir)
    Write-Host ('  Script:  {0}' -f $scriptName)
    Write-Host ('  Log:     {0}' -f $logFile)

    $sw = [System.Diagnostics.Stopwatch]::StartNew()

    $cmdArgs = @(
        ('"{0}"' -f $projectDir),
        $projectName,
        "-import", ('"{0}"' -f $binaryPath),
        "-overwrite",
        "-scriptPath", ('"{0}"' -f $scriptDir),
        "-preScript", "OptimizeAnalyzers.java",
        "-postScript", $scriptName,
        "-log", ('"{0}"' -f $logFile)
    )

    try {
        $process = Start-Process -FilePath $headlessBat -ArgumentList $cmdArgs -NoNewWindow -Wait -PassThru
        $exitCode = $process.ExitCode
    } catch {
        Write-Error ('Failed to start Ghidra process: {0}' -f $_.Exception.Message)
        $exitCode = -1
    }

    $sw.Stop()
    $durationSec = [Math]::Round($sw.Elapsed.TotalSeconds, 2)

    Write-Host ('Finished {0}: ExitCode={1}, Elapsed={2}s' -f $targetName, $exitCode, $durationSec)

    $mdReport = Join-Path $reportDir ('{0}_ghidra_report.md' -f $targetName)
    $reportGenerated = Test-Path -LiteralPath $mdReport

    $results.Add([PSCustomObject]@{
        TargetBinary    = $targetName
        ExitCode        = $exitCode
        DurationSeconds = $durationSec
        LogFile         = $logFile
        ReportGenerated = $reportGenerated
        ReportPath      = if ($reportGenerated) { $mdReport } else { "None" }
    })
}

$summaryJson = Join-Path $reportDir "ghidra_runs_summary.json"
$results | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath $summaryJson -Encoding utf8

Write-Host "============================================================"
Write-Host "All Ghidra runs completed. Summary:"
$results | Format-Table -AutoSize
