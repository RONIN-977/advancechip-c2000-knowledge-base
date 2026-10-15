from pathlib import Path
import shutil, zipfile, hashlib, json, re
base=Path(r'C:\Users\17879\Desktop\CHIP\BOOTLOADER')
work=Path(__file__).resolve().parent
models=('ADP32F035','AVP32F00157')
for model in models:
    package=base/(model+'Boot+APP_20261009')
    backup=work/'backup'/package.name
    backup.parent.mkdir(parents=True,exist_ok=True)
    if not backup.exists():
        shutil.copytree(package,backup)
        shutil.copy2(base/(package.name+'.zip'),backup.parent/(package.name+'.zip'))
    stage=work/'staging'/package.name
    assert not stage.exists(),stage
    shutil.copytree(package,stage,ignore=shutil.ignore_patterns('Debug','Release','build','__pycache__'))
    root=stage/model
    for kind in ('Bootloader','Application'):
        project=root/kind
        shutil.copytree(root/'Common',project/'Common',ignore=shutil.ignore_patterns('app_partition.h'))
        shutil.copytree(root/'Device',project/'Device')
        if model=='ADP32F035' and kind=='Bootloader':
            (project/'Device/lib').mkdir(exist_ok=True)
            shutil.copy2(root/'2803x_FlashAPI_BootROMSymbols.lib',project/'Device/lib')
        p=project/'.project'
        text=p.read_text(encoding='utf-8')
        text=re.sub(r'<linkedResources>.*?</linkedResources>', '<linkedResources>\n\t\t<link><name>app_partition.h</name><type>1</type><locationURI>PARENT-1-PROJECT_LOC/Common/app_partition.h</locationURI></link>\n\t</linkedResources>',text,flags=re.S)
        p.write_text(text,encoding='utf-8')
        p=project/'.cproject'; text=p.read_text(encoding='utf-8')
        text=text.replace('25.11.1.LTS','22.6.1.LTS')
        text=text.replace('${PROJECT_ROOT}/../Device','${PROJECT_ROOT}/Device')
        text=text.replace('value="${PROJECT_ROOT}/../Common"','value="${PROJECT_ROOT}/Common"/><listOptionValue builtIn="false" value="${PROJECT_ROOT}/../Common"')
        text=text.replace('value="${PROJECT_ROOT}/.."','value="${PROJECT_ROOT}/Device/lib"')
        text=re.sub(r'excluding="[^"]*"', 'excluding="Debug|Release|Device/lib"',text)
        p.write_text(text,encoding='utf-8')
        p=project/'.ccsproject'; text=p.read_text(encoding='utf-8').replace('25.11.1.LTS','22.6.1.LTS'); p.write_text(text,encoding='utf-8')
    # The only model-wide dependency retained by the projects is the address header.
    for p in (root/'Common').iterdir():
        if p.name!='app_partition.h':
            assert p.is_file() and p.resolve().is_relative_to(stage.resolve())
            p.unlink()
    old=root/'Device'; assert old.resolve().is_relative_to(stage.resolve()); shutil.rmtree(old)
    old=root/'2803x_FlashAPI_BootROMSymbols.lib'
    if old.exists():old.unlink()
    p=root/'build.ps1'; text=p.read_text(encoding='utf-8')
    text=text.replace('C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-c2000_25.11.1.LTS','C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS')
    if model=='ADP32F035':
        text=text.replace('$root/2803x_FlashAPI_BootROMSymbols.lib','$root/Bootloader/Device/lib/2803x_FlashAPI_BootROMSymbols.lib')
        text=text.replace('"--include_path=$root/Device/include",','')
        text=re.sub(r'^\$shared = .*\n','',text,flags=re.M)
        text=text.replace("    $objects = @()", "    $objects = @()\n    $shared = @(Get-ChildItem -LiteralPath \"$root/$kind/Common\" -Filter '*.c') + @(Get-ChildItem -LiteralPath \"$root/$kind/Device/source\" -File | Where-Object Extension -in '.c','.asm')")
        text=text.replace('"--include_path=$root/$kind"','"--include_path=$root/$kind" "--include_path=$root/$kind/Common" "--include_path=$root/$kind/Device/include"')
        text=text.replace('"--search_path=$root"','"--search_path=$root/$kind/Common" "--search_path=$root/$kind/Device/lib"')
        text=text.replace('$root/Device/DSP2803x_Headers_nonBIOS.cmd','$root/$kind/Device/DSP2803x_Headers_nonBIOS.cmd')
    else:
        text=text.replace('"-I$r/Device/include","-I$r/Device/driverlib","-I$r/Device/FlashAPI",','')
        text=re.sub(r'^\$shared=.*\n','',text,flags=re.M)
        text=text.replace('    $objs=@()', '    $objs=@()\n    $shared=@(Get-ChildItem "$r/$kind/Common/*.c")+@(Get-ChildItem "$r/$kind/Device/source/*" -File)')
        text=text.replace('"-I$r/$kind"','"-I$r/$kind" "-I$r/$kind/Common" "-I$r/$kind/Device/include" "-I$r/$kind/Device/driverlib" "-I$r/$kind/Device/FlashAPI"')
        text=text.replace('"-i$r/Device/lib"','"-i$r/$kind/Common" "-i$r/$kind/Device/lib"')
    p.write_text(text,encoding='utf-8-sig')
    for p in (root/'tests').glob('*'):
        if p.suffix not in ('.ps1','.py'):continue
        text=p.read_text(encoding='utf-8')
        text=text.replace('$r/Common/','$r/Application/Common/').replace('"$r/Common"','"$r/Application/Common" "-I$r/Common"')
        text=text.replace('"$r/Device/include"','"$r/Application/Device/include"')
        text=text.replace("'-I',\"$r/Application/Common\" \"-I$r/Common\"", "'-I',\"$r/Application/Common\",'-I',\"$r/Common\"")
        text=text.replace("'-I'+str(root/'Common'),", "'-I'+str(root/'Common'),'-I'+str(root/'Bootloader/Common'),")
        p.write_text(text,encoding='utf-8-sig' if p.suffix=='.ps1' else 'utf-8')
    p=root/'tools/verify.py'; text=p.read_text(encoding='utf-8')
    text=text.replace("root / 'Common/partition_config.h'", "root / 'Bootloader/Common/partition_config.h'")
    text=text.replace("root / '2803x_FlashAPI_BootROMSymbols.lib'", "root / 'Bootloader/Device/lib/2803x_FlashAPI_BootROMSymbols.lib'")
    text=text.replace("    assert any(e.text.endswith('/boot_io.c') for e in links)\n    assert any(e.text.endswith('/sci_port.c') for e in links)", "    assert len(links)==1 and links[0].text=='PARENT-1-PROJECT_LOC/Common/app_partition.h'\n    for name in ('boot_io.c','sci_port.c','boot_core.c','update_service.c'):\n        assert (r/kind/'Common'/name).is_file()")
    p.write_text(text,encoding='utf-8')
    p=root/'tools/package.py'; text=p.read_text(encoding='utf-8')
    text=re.sub(r"archive=root/'[^']+'", "archive=root.parent.with_name(root.parent.name+'.zip')",text)
    p.write_text(text,encoding='utf-8')
    print('STAGED',model,'one shared address header; local Common/Device; compiler 22.6.1')
