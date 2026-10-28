param([switch]$IntOsc, [string]$CompilerRoot = 'C:\ti\ccs2100\ccs\tools\compiler\ti-cgt-c2000_25.11.1.LTS')
$fw = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../../../F280049_Bootloader'))
& (Join-Path $fw 'build.ps1') -IntOsc:$IntOsc -CompilerRoot $CompilerRoot
