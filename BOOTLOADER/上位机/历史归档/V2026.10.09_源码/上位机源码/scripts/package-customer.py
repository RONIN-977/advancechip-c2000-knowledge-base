"""Package the existing customer runtime only; never copy the source project."""
from pathlib import Path
import hashlib
import json
import zipfile

source = Path(__file__).resolve().parents[1]
runtime = source.parent / '上位机用户版'
workspace = source.parent
report = json.loads((source / 'build/customer-check/ui-check.txt').read_text(encoding='utf-8-sig'))
assert report['SimulatedUpgradePaths'] == 9
assert report['PhysicalDeviceAccess'] == 'none'
for name in ['统一升级上位机.exe', 'bridge/x86/CanBridge32.exe', 'tools/ti/hex2000.exe',
             'drivers/zlg/x86/usbcan.dll', '使用说明.txt']:
    assert (runtime / name).is_file(), name
assert len(list((runtime / 'licenses').rglob('*LICENSE*'))) >= 4

source_files = [p for p in source.rglob('*') if p.is_file()
                and not set(p.relative_to(source).parts) & {'build', 'bin', 'obj', '.dotnet', '.git'}
                and p.name != 'SHA256.json']
manifest = {p.relative_to(source).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in sorted(source_files)}
(source / 'SHA256.json').write_text(json.dumps(manifest, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
source_hash = hashlib.sha256((source / 'SHA256.json').read_bytes()).hexdigest()
(runtime / '发布信息.txt').write_text(
    '统一升级上位机 用户版 2026-10-09，自定义 APP 起始地址\nWindows x64，自包含运行版。\n'
    '型号：AVP32F0039 / AVP32F335 / AVP32F0049 / ADP32F035 / AVP32F00157\n'
    f'发布界面模拟检查：{len(report["Checks"])} 项，9 条型号/通信路径通过。\n'
    '模拟检查不访问物理设备；本版自定义分区需使用客户配套 BOOT/APP 验证。\n'
    f'维护方源码清单 SHA256：{source_hash}\n', encoding='utf-8-sig')
(runtime / '使用说明.md').write_text('请阅读同目录的“使用说明.txt”，其中包含当前一键升级操作步骤。\n', encoding='utf-8-sig')
files = [p for p in runtime.rglob('*') if p.is_file() and p.name not in {'文件校验_SHA256.txt', '使用说明.md'}]
assert not any(p.suffix.lower() in {'.cs', '.csproj', '.sln', '.pdb', '.log'} for p in files)
checksums = {p.relative_to(runtime).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(files)}
checksum_file = runtime / '文件校验_SHA256.txt'
checksum_file.write_text(''.join(h+'  '+n+'\n' for n, h in checksums.items()), encoding='utf-8-sig')
archive = workspace / '统一升级上位机_用户版_20261009.zip'
prefix = '统一升级上位机_用户版/'
with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as z:
    for p in files + [checksum_file]: z.write(p, prefix+p.relative_to(runtime).as_posix())
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    for name, h in checksums.items(): assert hashlib.sha256(z.read(prefix+name)).hexdigest() == h
archive.with_suffix('.zip.sha256').write_text(hashlib.sha256(archive.read_bytes()).hexdigest()+'  '+archive.name+'\n', encoding='utf-8-sig')
print(f'PASS {archive} ({archive.stat().st_size} bytes); {len(checksums)} files verified')
