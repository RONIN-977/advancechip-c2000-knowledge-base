param(
    [string]$C2000Ware = 'C:\ti\c2000\C2000Ware_6_00_01_00',
    [string]$Compiler = 'C:\ti\ccs1271\ccs\tools\compiler\ti-cgt-c2000_22.6.1.LTS\bin\cl2000.exe'
)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$fixtureRoot = Join-Path $projectRoot 'artifacts\ccs'
$compilerRoot = Split-Path -Parent (Split-Path -Parent $Compiler)
if (-not (Test-Path -LiteralPath $Compiler)) { throw "C28x compiler missing: $Compiler" }
if (-not (Test-Path -LiteralPath $C2000Ware)) { throw "C2000Ware missing: $C2000Ware" }
$files = @(Get-ChildItem -LiteralPath $fixtureRoot -Filter '*.c')
if ($files.Count -eq 0) { throw 'Run Fae.Tests first to export generated C fixtures.' }
$results = @()
foreach ($file in $files) {
    $chipId = $file.BaseName.Split('_')[1]
    $family = switch ($chipId) { '035' { 'f2803x' } '335' { 'f2833x' } '0039' { 'f28003x' } '0049' { 'f28004x' } }
    $driverLib = $file.BaseName.Contains('_driverlib_')
    $argsList = @('--silicon_version=28', '--abi=eabi', '--c99', '--compile_only', '--display_error_number', "--include_path=$compilerRoot\include", '--obj_directory=.')
    if ($driverLib) {
        $argsList += "--include_path=$C2000Ware\driverlib\$family\driverlib"
        $argsList += "--include_path=$C2000Ware\device_support\$family\common\include"
    } else {
        $argsList += "--include_path=$C2000Ware\device_support\$family\headers\include"
    }
    if ($chipId -ne '035') { $argsList += '--float_support=fpu32' }
    $argsList += $file.Name
    # TI compiler 22.6 does not reliably accept Chinese source paths in argv.
    # Keep cwd at the fixture folder and pass ASCII relative source/output names.
    Push-Location -LiteralPath $fixtureRoot
    try {
        $compilerOutput = & $Compiler @argsList 2>&1 | Out-String
        $ok = $LASTEXITCODE -eq 0
    } finally { Pop-Location }
    $results += [pscustomobject]@{ File=$file.Name; SourceSha256=(Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash; Pass=$ok; Output=$compilerOutput.Trim() }
}
$report = [pscustomobject]@{
    GeneratedOn=(Get-Date -Format o)
    C2000Ware=$C2000Ware
    Compiler=$Compiler
    CompilerSha256=(Get-FileHash -LiteralPath $Compiler -Algorithm SHA256).Hash
    Passed=@($results | Where-Object Pass).Count
    Total=$results.Count
    Results=$results
    Validation='Compile-only of generated timing snippets. No linking, flashing, or board verification.'
}
$report | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $fixtureRoot 'compile-results.json') -Encoding UTF8
$results | Where-Object { -not $_.Pass } | Format-List
Write-Output "C28x compile: $($report.Passed)/$($report.Total) passed; $fixtureRoot\compile-results.json"
if ($report.Passed -ne $report.Total) { throw 'Some generated C snippets failed compilation.' }
