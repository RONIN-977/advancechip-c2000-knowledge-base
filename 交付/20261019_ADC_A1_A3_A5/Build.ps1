param([string]$CompilerRoot='C:/ti/ti-cgt-c2000_20.2.0.LTS')
$ErrorActionPreference='Stop'
Set-Location -LiteralPath (Join-Path $PSScriptRoot '编译工程/Ex_ADC')
$cc=Join-Path $CompilerRoot 'bin/cl2000.exe'
foreach($config in @('Debug','Release')) {
 $dir='Build_'+$config
 if(Test-Path -LiteralPath $dir){$dir += '_' + (Get-Date -Format 'HHmmss')}
 New-Item -ItemType Directory -Path $dir | Out-Null
 $log=Join-Path $dir 'build.log'
 $flags=@('-v28','-ml','--abi=coffabi','--include_path=include','--define=LARGE_MODEL',('--obj_directory='+$dir))
 if($config -eq 'Debug'){$flags+=@('-g','--define=_DEBUG')}else{$flags+='-O3'}
 foreach($src in Get-ChildItem -LiteralPath src -File | Where-Object {$_.Extension -in '.c','.asm'}) {
  $result=& $cc @flags '--compile_only' ('src/'+$src.Name) 2>&1
  $result | Out-File -LiteralPath $log -Append -Encoding utf8
  if($LASTEXITCODE -ne 0){throw "Compile failed: $($src.Name); see $log"}
 }
 $objs=@(Get-ChildItem -LiteralPath $dir -Filter *.obj | ForEach-Object {$dir+'/'+$_.Name})
 $libs=@(Get-ChildItem -LiteralPath lib -Filter *.lib | ForEach-Object {'lib/'+$_.Name})
 $link=@('-v28','-ml','--abi=coffabi','--run_linker','--rom_model',('--map_file='+$dir+'/Ex_ADC.map'),('--output_file='+$dir+'/Ex_ADC.out'),'--warn_sections','--reread_libs',('--search_path='+$CompilerRoot+'/lib'))
 if($config -eq 'Debug'){$link+='--stack_size=0x400'}
 $result=& $cc @link @objs 'cmd/AVP335.cmd' 'cmd/AVP33x_Headers_nonBIOS.cmd' @libs '--library=rts2800_ml.lib' 2>&1
 $result | Out-File -LiteralPath $log -Append -Encoding utf8
 if($LASTEXITCODE -ne 0){throw "Link failed: $config; see $log"}
 Write-Output "$config build passed"
}

