param([switch]$Test, [string]$Dotnet = 'dotnet', [string]$Avp335Root)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$env:DOTNET_CLI_HOME = Join-Path $root '.dotnet'
$env:DOTNET_CLI_TELEMETRY_OPTOUT = '1'
& $Dotnet build "$root\host\Updater.Desktop\Updater.Desktop.csproj" -c Release --configfile "$root\NuGet.Config"
if ($LASTEXITCODE -ne 0) { throw 'Desktop build failed' }
$desktop = "$root\host\Updater.Desktop\bin\Release\net10.0-windows"
Get-ChildItem -LiteralPath "$root\assets" | Copy-Item -Destination $desktop -Recurse -Force
if ($Test) {
    # Discovery tests use the same bundled converter as the application.
    $testOutput = "$root\tests\Updater.Tests\bin\Release\net10.0"
    New-Item -ItemType Directory -Path $testOutput -Force | Out-Null
    Copy-Item -LiteralPath "$root\assets\tools" -Destination $testOutput -Recurse -Force
    & $Dotnet build "$root\tests\Updater.Tests\Updater.Tests.csproj" -c Release --configfile "$root\NuGet.Config"
    if ($LASTEXITCODE -ne 0) { throw 'Test build failed' }
    $testArguments = @("$root", "$root\assets\tools\ti\hex2000.exe")
    if ($Avp335Root) { $testArguments += $Avp335Root }
    & $Dotnet "$testOutput\Updater.Tests.dll" @testArguments
    if ($LASTEXITCODE -ne 0) { throw 'Tests failed' }
}
Write-Output "Application: $desktop\Updater.Desktop.exe"
