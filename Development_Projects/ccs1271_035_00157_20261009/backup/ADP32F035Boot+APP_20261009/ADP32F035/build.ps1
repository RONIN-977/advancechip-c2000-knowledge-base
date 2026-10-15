param(
    [string]$CompilerRoot = 'C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-c2000_25.11.1.LTS',
    [switch]$NoFifo,
    [switch]$EchoTest
)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$compiler = Join-Path $CompilerRoot 'bin/cl2000.exe'
$expected = '0F01A9C5F2BBF4D5DDCD710B586DCE7354097F2F808FD099E252253AD28656FC'
if ((Get-FileHash -LiteralPath "$root/2803x_FlashAPI_BootROMSymbols.lib").Hash -ne $expected) {
    throw 'AVP Flash API library differs from the supplied/validated library. Verify its origin before updating the hash.'
}
$variant = if ($EchoTest) { 'echo' } elseif ($NoFifo) { 'no-fifo' } else { 'normal' }
$work = Join-Path $root "build/$variant"
$out = if ($variant -eq 'normal') { Join-Path $root 'output' } else { Join-Path $work 'output' }
New-Item -ItemType Directory -Path $work,$out -Force | Out-Null
$options = @('-v28','-ml','-mt','--abi=coffabi','--c99','-O0','--symdebug:dwarf','--gen_func_subsections=on','--diag_warning=225','--display_error_number','--emit_warnings_as_errors',"--include_path=$root/Common","--include_path=$root/Device/include","--include_path=$CompilerRoot/include")
if ($NoFifo) { $options += '--define=BOOT_SCI_USE_FIFO=0' }
if ($EchoTest) { $options += '--define=BOOT_SCI_ECHO_TEST' }
$shared = @(Get-ChildItem -LiteralPath "$root/Common" -Filter '*.c') + @(Get-ChildItem -LiteralPath "$root/Device/source" -File | Where-Object Extension -in '.c','.asm')
foreach ($kind in @('Bootloader','Application')) {
    $objects = @()
    $sources = $shared + @(Get-ChildItem -LiteralPath "$root/$kind" -Filter '*.c')
    foreach ($source in $sources) {
        $obj = Join-Path $work ($kind+'_'+$source.BaseName+'.obj')
        & $compiler @options "--include_path=$root/$kind" --compile_only $source.FullName "--output_file=$obj"
        if ($LASTEXITCODE -ne 0) { throw "Compile failed: $source" }
        $objects += $obj
    }
    $name = 'ADP32F035_'+$kind
    $libs = @('--library=libc.a')
    if ($kind -eq 'Bootloader') { $libs += '--library=2803x_FlashAPI_BootROMSymbols.lib' }
    & $compiler -v28 --abi=coffabi --run_linker --rom_model --entry_point=code_start --diag_suppress=10063 --stack_size=0x300 --reread_libs --warn_sections "--search_path=$root/Common" "--search_path=$root" "--search_path=$CompilerRoot/lib" @objects "$root/$kind/$($kind.ToLower()).cmd" "$root/Device/DSP2803x_Headers_nonBIOS.cmd" @libs "--output_file=$out/$name.out" "--map_file=$out/$name.map"
    if ($LASTEXITCODE -ne 0) { throw "Link failed: $kind" }
}
& "$CompilerRoot/bin/hex2000.exe" --boot --sci8 --ascii "--outfile=$out/ADP32F035_Application.txt" "$out/ADP32F035_Application.out"
if ($LASTEXITCODE -ne 0) { throw 'APP export failed' }
Write-Output "PASS ADP32F035 $variant : supplied Flash library, BOOT/APP + SCI8 built in $out"
