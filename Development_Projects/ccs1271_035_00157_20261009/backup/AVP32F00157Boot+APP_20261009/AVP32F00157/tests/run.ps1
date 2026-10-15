param([string]$NativeCompiler='C:/ADVCHIP/UDS_OriCode/.test-tools/ziglang/zig.exe', [string]$HostRoot)
$ErrorActionPreference='Stop'
$r=Split-Path -Parent $PSScriptRoot
if (!$HostRoot) { $HostRoot=Join-Path (Split-Path -Parent $r) '上位机源码' }
$HostRoot=(Resolve-Path -LiteralPath $HostRoot).Path
$out="$r/build/tests"
New-Item -ItemType Directory -Path $out -Force | Out-Null
& $NativeCompiler cc -O0 -Wall -Wextra -Werror -I "$r/tests/mock" -I "$r/Common" "$r/tests/service_test.c" "$r/Common/update_service.c" "$r/Common/boot_core.c" -o "$out/service.exe"
if($LASTEXITCODE -ne 0){throw 'Service compile failed'}
& "$out/service.exe"
if($LASTEXITCODE -ne 0){throw 'Service test failed'}
& $NativeCompiler cc -shared -O2 -Wall -Wextra -Werror -I "$r/Common" "$r/tests/native_model.c" "$r/Common/boot_core.c" -o "$out/native-model.dll"
if($LASTEXITCODE -ne 0){throw 'Native model compile failed'}
$env:DOTNET_CLI_HOME="$HostRoot/.dotnet"
dotnet build "$HostRoot/tests/Updater.Tests/Updater.Tests.csproj" -c Release --configfile "$HostRoot/NuGet.Config"
if($LASTEXITCODE -ne 0){throw 'Host tests compile failed'}
dotnet "$HostRoot/tests/Updater.Tests/bin/Release/net10.0/Updater.Tests.dll" --f2800157 "$HostRoot" "$HostRoot/assets/tools/ti/hex2000.exe" "$out/native-model.dll" "$r/output"
if($LASTEXITCODE -ne 0){throw 'F2800157 protocol tests failed'}
