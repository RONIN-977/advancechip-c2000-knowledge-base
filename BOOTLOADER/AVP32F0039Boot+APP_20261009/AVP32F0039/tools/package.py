"""Package the shared-header BOOT/APP delivery, without historical copies."""
from pathlib import Path
import zipfile,hashlib,json
root=Path(__file__).resolve().parents[1]
archive=root/'AVP32F0039Boot+APP_单头文件配置_20261009.zip'
allowed=set(['BOOT', 'APP']+['Common','Device','docs','output','tests','tools','README.md','单头文件修改说明.md','build.ps1','2803x_FlashAPI_BootROMSymbols.lib'])
files=[]
for p in root.rglob('*'):
    if not p.is_file():continue
    rel=p.relative_to(root)
    if rel.parts[0] not in allowed:continue
    if set(rel.parts)&{'build','Debug','Release','__pycache__','.git','.settings','原版固件'}:continue
    if p.suffix.lower() in {'.zip','.obj','.dll','.exe','.pyc'}:continue
    files.append(p)
manifest={p.relative_to(root).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(files)}
(root/'SHA256.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in files+[root/'SHA256.json']:z.write(p,'AVP32F0039'+'/'+p.relative_to(root).as_posix())
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    for name,h in manifest.items():assert hashlib.sha256(z.read('AVP32F0039'+'/'+name)).hexdigest()==h
    assert len([n for n in z.namelist() if n.endswith('/app_partition.h')])==1
    for kind in ['BOOT', 'APP']:
        for meta in ['.project','.cproject','.ccsproject']:assert 'AVP32F0039'+'/'+kind+'/'+meta in z.namelist()
print('PASS '+str(archive)+'; '+str(len(manifest))+' files, one shared app_partition.h')
