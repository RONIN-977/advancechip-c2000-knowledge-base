param([string]$CompilerRoot='C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS')
$ErrorActionPreference='Stop'
Push-Location -LiteralPath $PSScriptRoot
try {
$r='.'
$cc=Join-Path $CompilerRoot 'bin/cl2000.exe'
$work="$r/build/objects"; $out="$r/output"
New-Item -ItemType Directory -Path $work,$out -Force | Out-Null
$opts=@('-v28','-ml','-mt','--abi=eabi','--float_support=fpu32','--c99','-O0','-g','--gen_func_subsections=on','--define=CPU1','--define=_FLASH','--diag_warning=225','--emit_warnings_as_errors',"-I$r/Common","-I$CompilerRoot/include")
foreach($kind in @('Bootloader','Application')) {
    $objs=@()
    $shared=@(Get-ChildItem "$r/$kind/Common/*.c")+@(Get-ChildItem "$r/$kind/Device/source/*" -File)
    foreach($s in ($shared+@(Get-ChildItem "$r/$kind/*.c"))) {
        $obj="$work/$($kind)_$($s.BaseName).obj"
        & $cc @opts "-I$r/$kind" "-I$r/$kind/Common" "-I$r/$kind/Device/include" "-I$r/$kind/Device/driverlib" "-I$r/$kind/Device/FlashAPI" --compile_only (Resolve-Path -LiteralPath $s.FullName -Relative) "--output_file=$obj"
        if($LASTEXITCODE -ne 0){throw "Compile failed: $s"}; $objs+=$obj
    }
    $libs=@('-llibc.a','-ldriverlib.lib')
    if($kind -eq 'Bootloader'){$libs+='-lFAPI_F280015x_EABI_v2.00.10.lib'}
    & $cc -v28 --abi=eabi --float_support=fpu32 --run_linker --rom_model --entry_point=code_start --diag_suppress=10063 --stack_size=0x300 --reread_libs --warn_sections "-i$r/Common" "-i$r/$kind/Common" "-i$r/$kind/Device/lib" "-i$CompilerRoot/lib" @objs "$r/$kind/$($kind.ToLower()).cmd" @libs "--output_file=$out/AVP32F00157_$kind.out" "--map_file=$out/AVP32F00157_$kind.map"
    if($LASTEXITCODE -ne 0){throw "Link failed: $kind"}
}
& "$CompilerRoot/bin/hex2000.exe" --boot --sci8 --ascii "--outfile=$out/AVP32F00157_Application.txt" "$out/AVP32F00157_Application.out"
if($LASTEXITCODE -ne 0){throw 'APP export failed'}
Write-Output 'PASS BOOT/APP EABI build and SCI8 export'

} finally { Pop-Location }
