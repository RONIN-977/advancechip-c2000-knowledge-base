"""Verify the single-file customer directory from a relocated copy; never open hardware."""
from pathlib import Path
import argparse, hashlib, json, os, shutil, struct, subprocess

parser=argparse.ArgumentParser()
parser.add_argument('customer',type=Path)
parser.add_argument('scratch',type=Path)
parser.add_argument('results',type=Path)
args=parser.parse_args()
customer=args.customer.resolve(); scratch=args.scratch.resolve(); results=args.results.resolve()
assert not scratch.exists(), 'Use a fresh scratch directory to test clean bundle extraction'
scratch.mkdir(parents=True); results.mkdir(parents=True,exist_ok=True)
relocated=scratch/'独立客户目录'
shutil.copytree(customer,relocated)
extract=scratch/'内置运行时缓存'
environment=dict(os.environ,DOTNET_ROOT=str(scratch/'no-dotnet'),DOTNET_ROOT_X64=str(scratch/'no-dotnet'),
                 DOTNET_ROOT_X86=str(scratch/'no-dotnet'),DOTNET_MULTILEVEL_LOOKUP='0',
                 DOTNET_BUNDLE_EXTRACT_BASE_DIR=str(extract),
                 PATH=os.path.join(os.environ['SystemRoot'],'System32'))
app=relocated/'上位机'; exe=app/'统一升级上位机.exe'
def run(command,stdin=None):
    result=subprocess.run([str(x) for x in command],cwd=scratch,env=environment,input=stdin,
                          capture_output=True,timeout=90,creationflags=subprocess.CREATE_NO_WINDOW)
    if result.returncode:
        raise RuntimeError(result.stdout.decode(errors='replace')+result.stderr.decode(errors='replace'))
    return result
check=results/'customer-self-check.json'
run([exe,'--self-check',check,relocated])
report=json.loads(check.read_text(encoding='utf-8'))
assert Path(report['Runtime'])==exe or Path(report['Runtime']).is_relative_to(extract), report
for name in ['Driver','Converter']: assert Path(report[name]).is_relative_to(app), report
assert report['Avp335Bytes30MHz']==5650 and report['Avp335Bytes33MHz']==5650
assert report['Avp335UiSimulation']=='passed'
bridge=app/'bridge/x86/CanBridge32.exe'; driver=app/'drivers/zlg/x86/usbcan.dll'
response=run([bridge,'--check',driver])
(results/'customer-bridge-check.log').write_bytes(response.stdout+response.stderr)
response=run([bridge,'--serve',driver],b'')
assert response.stdout==struct.pack('<I',0x43414e31)
def machine(path):
    data=path.read_bytes(); offset=struct.unpack_from('<I',data,0x3c)[0]
    assert data[offset:offset+4]==b'PE\0\0'
    return struct.unpack_from('<H',data,offset+4)[0]
architectures={machine(exe),machine(bridge)}
assert architectures=={0x14c,0x8664}, architectures
assert any(extract.rglob('wpfgfx_cor3.dll')), 'WPF native library was not extracted from the bundle'
for chip in ['280039C','335']:
    command=[exe,'--snapshot',results/f'customer-{chip}.png',1080,800]
    if chip=='335': command.append(chip)
    run(command)
for line in (relocated/'文件校验_SHA256.txt').read_text(encoding='utf-8').splitlines():
    expected,name=line.split('  ',1)
    assert hashlib.sha256((relocated/name).read_bytes()).hexdigest().upper()==expected,name
summary=dict(singleFileRuntime='Self-contained x64/x86 single-file hosts; WPF native libraries extracted into fresh scratch directory',
             dependencyIsolation='Invalid DOTNET_ROOT paths; PATH contains Windows System32 only; customer folder relocated',
             outImport='0039c and both real 335 APP OUTs passed',
             canBridge='32-bit driver exports and EOF handshake passed; no device opened',
             ui='335 simulated upgrade, settings lock/chip clearing and both offscreen renders passed',
             customerHashes='all matched after runtime checks',
             hardware='No new-board or clean-Windows-VM test; user previously reported 30m board upgrade working')
(results/'customer-verification.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(summary,ensure_ascii=False,indent=2))
