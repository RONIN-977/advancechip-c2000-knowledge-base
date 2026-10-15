from pathlib import Path
import shutil,hashlib,json,subprocess,sys
work=Path(__file__).resolve().parent
base=Path(r'C:\Users\17879\Desktop\CHIP\BOOTLOADER')
assert (work/'verification.json').is_file()
for model in ('ADP32F035','AVP32F00157'):
    package=model+'Boot+APP_20261009'
    stage=work/'staging'/package/model
    backup=work/'backup'/package/model
    target=base/package/model
    assert target.resolve().is_relative_to(base.resolve()) and target.name==model
    # Check all original maintained files against the backup before writing.
    for p in backup.rglob('*'):
        if not p.is_file():continue
        if set(p.relative_to(backup).parts)&{'Debug','Release','build','__pycache__'}:continue
        q=target/p.relative_to(backup)
        assert q.is_file() and q.read_bytes()==p.read_bytes(),('Original changed since backup',q)
    retired=work/'retired'/model; retired.mkdir(parents=True,exist_ok=True)
    old=target/'Device'
    assert old.resolve().is_relative_to(target.resolve()) and not (retired/'Device').exists()
    shutil.move(str(old),str(retired/'Device'))
    oldcommon=retired/'Common'; oldcommon.mkdir()
    for p in (target/'Common').iterdir():
        if p.name=='app_partition.h':continue
        assert p.is_file() and p.resolve().is_relative_to(target.resolve())
        shutil.move(str(p),str(oldcommon/p.name))
    lib=target/'2803x_FlashAPI_BootROMSymbols.lib'
    if lib.exists():shutil.move(str(lib),str(retired/lib.name))
    shutil.copytree(stage,target,dirs_exist_ok=True,ignore=shutil.ignore_patterns('Debug','Release','build','__pycache__'))
    for kind in ('Bootloader','Application'):
        for p in (stage/kind).rglob('*'):
            if p.is_file() and not set(p.relative_to(stage/kind).parts)&{'Debug','Release','build','__pycache__'}:
                assert p.read_bytes()==(target/kind/p.relative_to(stage/kind)).read_bytes()
    assert len(list(target.rglob('app_partition.h')))==1
    print('PUBLISHED',target,flush=True)
# Import the actual desktop deliverables into another clean CCS workspace.
exe=r'C:\ti\ccs1271\ccs\eclipse\eclipsec.exe'
for model in ('ADP32F035','AVP32F00157'):
    root=base/(model+'Boot+APP_20261009')/model
    for kind in ('Bootloader','Application'):
        args=[exe,'-noSplash','-data',str(work/'ccs_workspace_desktop'),'-application','com.ti.ccstudio.apps.projectImport','-ccs.location',str(root/kind)]
        result=subprocess.run(args,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=180)
        text=result.stdout.decode('utf-8',errors='replace')
        (work/'logs'/(model+'_'+kind+'_desktop_import.log')).write_text(text,encoding='utf-8')
        print('DESKTOP IMPORT',model,kind,result.returncode,flush=True)
        if result.returncode: print(text,flush=True); sys.exit(result.returncode)
args=[exe,'-noSplash','-data',str(work/'ccs_workspace_desktop'),'-application','com.ti.ccstudio.apps.projectBuild','-ccs.workspace','-ccs.configuration','Debug','-ccs.buildType','full','-ccs.listProblems']
result=subprocess.run(args,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=240)
text=result.stdout.decode('utf-8',errors='replace')
(work/'logs/desktop_full_build.log').write_text(text,encoding='utf-8')
print('DESKTOP FULL BUILD',result.returncode,flush=True)
print(text[-2200:],flush=True)
if result.returncode:sys.exit(result.returncode)
assert text.count('Finished building target:')==4
(work/'desktop_build_pass.json').write_text(json.dumps({'CCS':'12.7.1','compiler':'22.6.1.LTS','four_desktop_projects_full_build':'PASS'},indent=2),encoding='utf-8')
