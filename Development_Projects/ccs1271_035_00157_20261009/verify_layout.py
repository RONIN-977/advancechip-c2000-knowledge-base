from pathlib import Path
import shutil,subprocess,sys,hashlib,json,xml.etree.ElementTree as ET,re
work=Path(__file__).resolve().parent
results=[]
for model,new_start in [('ADP32F035','0x3EA000'),('AVP32F00157','0x08A000')]:
    root=work/'staging'/(model+'Boot+APP_20261009')/model
    original=work/'backup'/(model+'Boot+APP_20261009')/model
    for kind in ('Bootloader','Application'):
        for p in (original/'Common').rglob('*'):
            if p.is_file() and p.name!='app_partition.h':assert p.read_bytes()==(root/kind/'Common'/p.relative_to(original/'Common')).read_bytes(),p
        for p in (original/'Device').rglob('*'):
            if p.is_file():assert p.read_bytes()==(root/kind/'Device'/p.relative_to(original/'Device')).read_bytes(),p
        for p in (original/kind).iterdir():
            if p.suffix in ('.c','.h','.asm','.cmd'):assert p.read_bytes()==(root/kind/p.name).read_bytes(),p
        links=ET.parse(root/kind/'.project').findall('.//locationURI')
        assert len(links)==1 and links[0].text=='PARENT-1-PROJECT_LOC/Common/app_partition.h'
        text=(root/kind/'.cproject').read_text()
        assert '25.11' not in text and '../Device' in text and '../../Common' in text
        assert not (root/kind/'Common/app_partition.h').exists()
    assert len(list(root.rglob('app_partition.h')))==1
    assert (root/'Common/app_partition.h').read_bytes()==(original/'Common/app_partition.h').read_bytes()
    if model=='ADP32F035':assert (original/'2803x_FlashAPI_BootROMSymbols.lib').read_bytes()==(root/'Bootloader/Device/lib/2803x_FlashAPI_BootROMSymbols.lib').read_bytes()
    result=subprocess.run(['pwsh','-NoProfile','-ExecutionPolicy','Bypass','-File',str(root/'build.ps1')],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=180)
    log=result.stdout.decode('utf-8',errors='replace'); (work/'logs'/(model+'_command_build.log')).write_text(log,encoding='utf-8')
    print(model,'default CLI build',result.returncode,log[-700:],flush=True)
    if result.returncode:sys.exit(result.returncode)
    result=subprocess.run([sys.executable,str(root/'tools/verify.py')],capture_output=True,text=True)
    print(result.stdout,result.stderr,flush=True)
    if result.returncode:sys.exit(result.returncode)
    relocated=work/'relocation'/model
    shutil.copytree(root,relocated,ignore=shutil.ignore_patterns('Debug','Release','build','__pycache__'))
    header=relocated/'Common/app_partition.h'; text=header.read_text(); text=re.sub(r'(#define\s+APP_START_ADDRESS\s+)0x[0-9A-Fa-f]+',r'\g<1>'+new_start,text); header.write_text(text,encoding='utf-8')
    result=subprocess.run(['pwsh','-NoProfile','-ExecutionPolicy','Bypass','-File',str(relocated/'build.ps1')],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=180)
    log=result.stdout.decode('utf-8',errors='replace'); (work/'logs'/(model+'_relocated_address_build.log')).write_text(log,encoding='utf-8')
    print(model,'relocated changed address build',result.returncode,log[-500:],flush=True)
    if result.returncode:sys.exit(result.returncode)
    result=subprocess.run([sys.executable,str(relocated/'tools/verify.py')],capture_output=True,text=True)
    print(result.stdout,result.stderr,flush=True)
    if result.returncode:sys.exit(result.returncode)
    results.append({'model':model,'default_build':'PASS','relocated_address':new_start,'relocated_build':'PASS','original_source_and_library_bytes':'UNCHANGED','shared_address_headers':1})
(work/'verification.json').write_text(json.dumps(results,ensure_ascii=False,indent=2),encoding='utf-8')
print('PASS source/library identity, default and relocated layout checks',flush=True)

