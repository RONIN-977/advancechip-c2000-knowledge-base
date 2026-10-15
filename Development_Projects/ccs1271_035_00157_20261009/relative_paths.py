from pathlib import Path
import re
work=Path(__file__).resolve().parent
for model in ('ADP32F035','AVP32F00157'):
    root=work/'staging'/(model+'Boot+APP_20261009')/model
    for kind in ('Bootloader','Application'):
        p=root/kind/'.cproject'; text=p.read_text(encoding='utf-8')
        text=text.replace('${PROJECT_ROOT}/../Common','../../Common').replace('${PROJECT_ROOT}/','../').replace('${PROJECT_ROOT}','..')
        p.write_text(text,encoding='utf-8')
    p=root/'build.ps1'; text=p.read_text(encoding='utf-8-sig')
    if model=='ADP32F035':
        text=text.replace('$root = $PSScriptRoot', "Push-Location -LiteralPath $PSScriptRoot\ntry {\n$root = '.'")
        text=text.replace('--compile_only $source.FullName', '--compile_only (Resolve-Path -LiteralPath $source.FullName -Relative)')
    else:
        text=text.replace('$r=$PSScriptRoot', "Push-Location -LiteralPath $PSScriptRoot\ntry {\n$r='.'")
        text=text.replace('--compile_only $s.FullName', '--compile_only (Resolve-Path -LiteralPath $s.FullName -Relative)')
    text+='\n} finally { Pop-Location }\n'
    p.write_text(text,encoding='utf-8-sig')
# use a fresh CCS workspace after the include-path correction
p=work/'ccs_build.py'; text=p.read_text(encoding='utf-8-sig').replace("work/'ccs_workspace'","work/'ccs_workspace_relative'")
p.write_text(text,encoding='utf-8')
print('Relative paths configured for CCS and command-line builds')
