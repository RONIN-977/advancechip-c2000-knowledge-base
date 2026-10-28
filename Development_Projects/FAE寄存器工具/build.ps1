param([string]$Dotnet, [string]$Packages, [switch]$SkipTests, [switch]$Publish, [switch]$Offline)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$repo = Split-Path (Split-Path $root -Parent) -Parent
if (-not $Dotnet) {
    $localSdk = Join-Path $repo 'tmp\dotnet-sdk\dotnet.exe'
    if (Test-Path -LiteralPath $localSdk) { $Dotnet = $localSdk } else { $Dotnet = 'dotnet' }
}
if (-not $Packages) {
    $localPackages = Join-Path $repo 'tmp\nuget'
    if (Test-Path -LiteralPath $localPackages) { $Packages = $localPackages }
}
$env:DOTNET_CLI_HOME = Join-Path $root '.dotnet'
$env:DOTNET_CLI_TELEMETRY_OPTOUT = '1'
$env:DOTNET_SKIP_FIRST_TIME_EXPERIENCE = '1'
$env:DOTNET_GENERATE_ASPNET_CERTIFICATE = 'false'
$env:DOTNET_NOLOGO = '1'
if ($Packages) { $env:NUGET_PACKAGES = $Packages }
$desktop = Join-Path $root 'src\Fae.Desktop\Fae.Desktop.csproj'
$nugetConfig = Join-Path $root 'NuGet.Config'
if ($Offline) {
    New-Item -ItemType Directory -Path $env:DOTNET_CLI_HOME -Force | Out-Null
    $nugetConfig = Join-Path $env:DOTNET_CLI_HOME 'NuGet.Offline.Config'
    '<configuration><packageSources><clear /></packageSources></configuration>' | Set-Content -LiteralPath $nugetConfig -Encoding utf8
}
& $Dotnet build $desktop -c Release --configfile $nugetConfig --ignore-failed-sources
if ($LASTEXITCODE -ne 0) { throw 'Desktop build failed' }
if (-not $SkipTests) {
    & $Dotnet build "$root\tests\Fae.Tests\Fae.Tests.csproj" -c Release --configfile $nugetConfig --ignore-failed-sources
    if ($LASTEXITCODE -ne 0) { throw 'Test build failed' }
    & $Dotnet "$root\tests\Fae.Tests\bin\Release\net10.0\Fae.Tests.dll" "$root\artifacts\ccs"
    if ($LASTEXITCODE -ne 0) { throw 'Core tests failed' }
}
if ($Publish) {
    $publishDir = Join-Path $root 'artifacts\publish'
    & $Dotnet publish $desktop -c Release -r win-x64 --self-contained true -p:PublishSingleFile=false -p:PublishTrimmed=false -o $publishDir --configfile $nugetConfig --ignore-failed-sources
    if ($LASTEXITCODE -ne 0) { throw 'Publish failed' }
    $dataDir = Join-Path $publishDir 'Data'
    New-Item -ItemType Directory -Path $dataDir -Force | Out-Null
    Copy-Item -LiteralPath "$root\Data\registers.json" -Destination $dataDir -Force
    $runtimeConfig = Get-Content -LiteralPath (Join-Path $publishDir 'FAE寄存器工具.runtimeconfig.json') -Raw | ConvertFrom-Json
    foreach ($framework in $runtimeConfig.runtimeOptions.includedFrameworks) {
        $packageId = if ($framework.name -eq 'Microsoft.WindowsDesktop.App') { 'microsoft.windowsdesktop.app.runtime.win-x64' } else { 'microsoft.netcore.app.runtime.win-x64' }
        if ($env:NUGET_PACKAGES) {
            $packageRoot = Join-Path $env:NUGET_PACKAGES "$packageId\$($framework.version)"
            $licenseDir = Join-Path $publishDir "licenses\$packageId"
            New-Item -ItemType Directory -Path $licenseDir -Force | Out-Null
            Get-ChildItem -LiteralPath $packageRoot -File -ErrorAction SilentlyContinue | Where-Object Name -Match '^(LICENSE|THIRD.PARTY)' | Copy-Item -Destination $licenseDir -Force
        }
    }
    Write-Output "Published: $publishDir"
}
