from pathlib import Path
import subprocess,shutil,sys,re,json,zipfile,hashlib
work=Path(__file__).resolve().parent
base=Path(r'C:\Users\17879\Desktop\CHIP\BOOTLOADER')
assert (work/'desktop_build_pass.json').is_file()
hexexe=r'C:\ti\ccs1271\ccs\tools\compiler\ti-cgt-c2000_22.6.1.LTS\bin\hex2000.exe'
log=(work/'logs/desktop_full_build.log').read_text(encoding='utf-8')
assert '0 out of 4 projects have errors' in log
assert len(re.findall(r"Warnings for project '[^']+' \(0\)",log))==4
for model,start,example in [('ADP32F035','0x3E8000','0x3EA000'),('AVP32F00157','0x088000','0x08A000')]:
    root=base/(model+'Boot+APP_20261009')/model
    for kind in ('Bootloader','Application'):
        for suffix in ('.out','.map'):
            shutil.copy2(root/kind/'Debug'/(model+'_'+kind+suffix),root/'output')
        maptext=(root/'output'/(model+'_'+kind+'.map')).read_text()
        match=re.search(r'^\s*(?:0|1|abs)\s+([0-9a-fA-F]{8})\s+code_start\s*$',maptext,re.M)
        assert match
        expected=int(start,16) if kind=='Application' else (0x3f7ff6 if model=='ADP32F035' else 0x80000)
        assert int(match[1],16)==expected
    result=subprocess.run([hexexe,'--boot','--sci8','--ascii','--outfile=output/'+model+'_Application.txt','output/'+model+'_Application.out'],cwd=root,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    assert result.returncode==0,result.stdout
    result=subprocess.run([sys.executable,str(root/'tools/verify.py')],capture_output=True,text=True)
    print(model,result.stdout.strip(),flush=True)
    if result.returncode:print(result.stderr);sys.exit(result.returncode)
    shutil.copy2(work/'logs/desktop_full_build.log',root/'output/ccs1271-full-build-20261009.log')
    for name in ('command_build','relocated_address_build'):
        shutil.copy2(work/'logs'/(model+'_'+name+'.log'),root/'output'/('ccs1271-'+name+'-20261009.log'))
    (root/'output/CCS12.7.1_验证记录.md').write_text(f'''# 2026-10-09 独立工程版验证

- 实际交付目录：`{root}`。
- 使用本机 `C:/ti/ccs1271/ccs/eclipse/eclipsec.exe`，CCS 12.7.1；C2000 编译器 22.6.1.LTS。
- 两型号共 4 个工程实际导入全新 CCS 工作区，Debug full build：4/4 通过，0 错误，0 警告。原路径中文目录直接编译通过。
- 本型号默认地址 `{start}`；output OUT/MAP 从本次原路径 CCS Debug 构建复制，APP SCI8 TXT 用同版本 hex2000 导出。
- 本型号整体复制到独立验证目录，只修改共享头文件一行至 `{example}`，BOOT/APP 均通过同编译器构建；入口地址、Flash 边界及 RAM 函数/API 位置校验通过。交付版本已保留默认地址。
- 原版 Common、Device 中所有源码/头文件/库以及各工程根目录 C/H/ASM/CMD 与备份逐文件按字节比对一致；只调整目录、工程引用、编译器版本、构建/测试/校验/打包路径和说明。
- 每个工程仅一个外部链接：`PARENT-1-PROJECT_LOC/Common/app_partition.h`；其余源码和库为工程目录内实体文件。
- 验证旧有链接检查脚本，035 ROM API 库哈希及绝对入口保持一致，00157 Flash API/RAM 搬运范围通过。
- 本次没有重新运行历史上位机协议仿真或进行烧录、SCI/CAN 实板升级测试。

详细全量编译日志：`ccs1271-full-build-20261009.log`。
原始工程和同名 ZIP 备份：`{work/'backup'}`。
''',encoding='utf-8')
    result=subprocess.run([sys.executable,str(root/'tools/package.py')],capture_output=True,text=True)
    print(result.stdout.strip(),flush=True)
    if result.returncode:print(result.stderr);sys.exit(result.returncode)
    archive=base/(model+'Boot+APP_20261009.zip')
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert len([n for n in z.namelist() if n.endswith('/app_partition.h')])==1
        assert not any('/Debug/' in n or '/build/' in n for n in z.namelist())
        for kind in ('Bootloader','Application'):
            assert any(n.startswith(model+'/'+kind+'/Common/') and n.endswith('.c') for n in z.namelist())
            assert any(n.startswith(model+'/'+kind+'/Device/') for n in z.namelist())
        manifest=json.loads(z.read(model+'/SHA256.json'))
        for name,digest in manifest.items():
            assert hashlib.sha256((root/name).read_bytes()).hexdigest()==digest
            assert hashlib.sha256(z.read(model+'/'+name)).hexdigest()==digest
    print('DELIVERED ZIP',archive,'files',len(manifest),'bytes',archive.stat().st_size,flush=True)
print('PASS original paths, default OUT/MAP/TXT, one address header, portable local sources/libraries, ZIP integrity and SHA256',flush=True)
