param([string]$Dotnet = 'dotnet', [string]$Avp335Root = (Join-Path $PSScriptRoot '..\..\..\..\AVP32F335\335SCI统一升级_V201'), [string]$PackageCache)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$env:DOTNET_CLI_HOME = Join-Path $root '.dotnet'
$env:DOTNET_CLI_TELEMETRY_OPTOUT = '1'
if ($PackageCache) { $env:NUGET_PACKAGES = $PackageCache }
$output = Join-Path $root 'build\portable\C2000-Updater'
& $Dotnet publish "$root\host\Updater.Desktop\Updater.Desktop.csproj" -c Release -r win-x64 --self-contained true --source https://api.nuget.org/v3/index.json --configfile "$root\NuGet.Config" -o $output
if ($LASTEXITCODE -ne 0) { throw 'Desktop publish failed' }
& $Dotnet publish "$root\host\CanBridge32\CanBridge32.csproj" -c Release -r win-x86 --self-contained true --source https://api.nuget.org/v3/index.json --configfile "$root\NuGet.Config" -o "$output\bridge\x86"
if ($LASTEXITCODE -ne 0) { throw 'Bridge publish failed' }
Get-ChildItem -LiteralPath "$root\assets" | Copy-Item -Destination $output -Recurse -Force
New-Item -ItemType Directory -Path "$output\firmware" -Force | Out-Null
Copy-Item -LiteralPath "$root\build\firmware\application.out" -Destination "$output\firmware" -Force
Copy-Item -LiteralPath "$root\docs\portable-readme.md" -Destination "$output\使用说明.md" -Force
foreach ($crystal in @('30MHz', '33.3MHz')) {
    $destination = Join-Path $output "firmware\335\$crystal"
    New-Item -ItemType Directory -Path $destination -Force | Out-Null
    foreach ($kind in @('BOOT', 'APP')) {
        Copy-Item -Path "$Avp335Root\build\$crystal\$kind\*.out", "$Avp335Root\build\$crystal\$kind\*.map" -Destination $destination -Force
    }
}
Copy-Item -LiteralPath "$root\docs\335-sci.md" -Destination "$output\335接线与首次烧录.md" -Force
Compress-Archive -LiteralPath $output -DestinationPath "$root\build\portable\C2000-Updater-Portable-win-x64.zip" -Force
Write-Output "Portable application: $output"
