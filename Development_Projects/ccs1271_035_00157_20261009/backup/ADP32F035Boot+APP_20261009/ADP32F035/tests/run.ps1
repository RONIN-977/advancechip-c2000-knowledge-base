param([string]$NativeCompiler='C:/ADVCHIP/UDS_OriCode/.test-tools/ziglang/zig.exe')
$ErrorActionPreference='Stop'
$r=Split-Path -Parent $PSScriptRoot
$out=Join-Path $r 'build/tests'
New-Item -ItemType Directory -Path $out -Force | Out-Null
$includes=@('-I',"$r/tests/mock",'-I',"$r/Common",'-I',"$r/Device/include")
foreach ($fifo in @(1,0)) {
    & $NativeCompiler cc -O0 -Wall -Wextra -Werror @includes "-DBOOT_SCI_USE_FIFO=$fifo" "$r/tests/sci_test.c" "$r/Common/sci_port.c" -o "$out/sci-$fifo.exe"
    if ($LASTEXITCODE -ne 0) { throw 'SCI test compile failed' }
    & "$out/sci-$fifo.exe"
    if ($LASTEXITCODE -ne 0) { throw 'SCI tests failed' }
}
& $NativeCompiler cc -O0 -Wall -Wextra -Werror @includes "$r/tests/service_test.c" "$r/Common/update_service.c" "$r/Common/boot_core.c" -o "$out/service.exe"
if ($LASTEXITCODE -ne 0) { throw 'APP service test compile failed' }
& "$out/service.exe"
if ($LASTEXITCODE -ne 0) { throw 'APP service tests failed' }
