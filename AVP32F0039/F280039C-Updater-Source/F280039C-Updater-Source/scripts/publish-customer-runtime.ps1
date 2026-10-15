param([string]$Dotnet = 'dotnet', [string]$OutputDirectory, [string]$PackageCache)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
if (!$OutputDirectory) { $OutputDirectory = Join-Path $projectRoot 'build\customer-runtime' }
$stage = Join-Path $projectRoot 'build\customer-publish'
$env:DOTNET_CLI_HOME = Join-Path $projectRoot '.dotnet'
$env:DOTNET_CLI_TELEMETRY_OPTOUT = '1'
if ($PackageCache) { $env:NUGET_PACKAGES = $PackageCache }
foreach ($item in @(@('Updater.Desktop','win-x64','desktop'), @('CanBridge32','win-x86','bridge'))) {
    # No trimming. The optional single-file analyzer is disabled so an offline SDK
    # with cached runtime packs does not need the additional ILLink package.
    & $Dotnet publish (Join-Path $projectRoot ('host\'+$item[0]+'\'+$item[0]+'.csproj')) `
        -c Release -r $item[1] --self-contained true -p:PublishSingleFile=true `
        -p:EnableSingleFileAnalyzer=false -p:IncludeNativeLibrariesForSelfExtract=true `
        -p:EnableCompressionInSingleFile=true -p:PublishTrimmed=false -p:DebugType=None -p:DebugSymbols=false `
        --source https://api.nuget.org/v3/index.json --configfile (Join-Path $projectRoot 'NuGet.Config') `
        -o (Join-Path $stage $item[2])
    if ($LASTEXITCODE -ne 0) { throw ('Publish failed: '+$item[0]) }
}
New-Item -ItemType Directory -Path (Join-Path $OutputDirectory 'bridge\x86') -Force | Out-Null
Copy-Item -LiteralPath (Join-Path $stage 'desktop\Updater.Desktop.exe') -Destination (Join-Path $OutputDirectory '统一升级上位机.exe') -Force
Copy-Item -LiteralPath (Join-Path $stage 'bridge\CanBridge32.exe') -Destination (Join-Path $OutputDirectory 'bridge\x86\CanBridge32.exe') -Force
Get-ChildItem -LiteralPath (Join-Path $projectRoot 'assets') | Copy-Item -Destination $OutputDirectory -Recurse -Force
Write-Output ('Runtime directory: '+$OutputDirectory)
Write-Output 'Place customer firmware alongside this directory, and retain the accompanying Microsoft runtime license notices.'
