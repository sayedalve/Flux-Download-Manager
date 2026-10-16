# test_ast.ps1
# Validates PowerShell scripts using [System.Management.Automation.Language.Parser]
param(
    [Parameter(Mandatory=$true)]
    [string]$ScriptPath
)

if (-not (Test-Path -LiteralPath $ScriptPath)) {
    Write-Error "File not found: $ScriptPath"
    exit 1
}

$tokens = $null
$errors = $null
$ast = [System.Management.Automation.Language.Parser]::ParseFile($ScriptPath, [ref]$tokens, [ref]$errors)

if ($errors.Count -eq 0) {
    Write-Host "[AST VALID] $ScriptPath has no syntax errors." -ForegroundColor Green
    exit 0
} else {
    Write-Host "[AST ERROR] Found $($errors.Count) error(s) in $ScriptPath" -ForegroundColor Red
    foreach ($err in $errors) {
        Write-Host "  Line $($err.Extent.StartLineNumber), Col $($err.Extent.StartColumnNumber): $($err.Message)" -ForegroundColor Yellow
    }
    exit 1
}
