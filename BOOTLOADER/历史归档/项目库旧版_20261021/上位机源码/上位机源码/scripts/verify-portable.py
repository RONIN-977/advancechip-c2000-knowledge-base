"""Read-only portable self-check, bridge load/EOF handshake and offscreen layouts."""
import argparse, json, os, struct, subprocess
from pathlib import Path
parser=argparse.ArgumentParser()
parser.add_argument('portable',type=Path)
parser.add_argument('results',type=Path)
args=parser.parse_args()
portable=args.portable.resolve(); results=args.results.resolve(); results.mkdir(parents=True,exist_ok=True)
environment=dict(os.environ, DOTNET_ROOT=str(results/'no-installed-runtime'), DOTNET_ROOT_X86=str(results/'no-installed-runtime'), DOTNET_ROOT_X64=str(results/'no-installed-runtime'))
exe=portable/'Updater.Desktop.exe'
def run(command):
    return subprocess.run([str(x) for x in command],env=environment,capture_output=True,timeout=45,creationflags=subprocess.CREATE_NO_WINDOW)
result=run([exe,'--self-check',results/'portable-self-check.json'])
if result.returncode: raise SystemExit((results/'portable-self-check.json').read_text(encoding='utf-8'))
report=json.loads((results/'portable-self-check.json').read_text(encoding='utf-8'))
for field in ['Runtime','Driver','Converter']: assert Path(report[field]).is_relative_to(portable), report
bridge=portable/'bridge/x86/CanBridge32.exe'; dll=portable/'drivers/zlg/x86/usbcan.dll'
result=run([bridge,'--check',dll]); assert result.returncode==0,result.stderr
(results/'bridge-check.log').write_bytes(result.stdout+result.stderr)
result=subprocess.run([str(bridge),'--serve',str(dll)],input=b'',env=environment,capture_output=True,timeout=15,creationflags=subprocess.CREATE_NO_WINDOW)
assert result.returncode==0 and result.stdout==struct.pack('<I',0x43414e31),(result.returncode,result.stdout,result.stderr)
for chip in ['280039C','335']:
    for width,height in [(1080,800),(640,480)]:
        command=[exe,'--snapshot',results/f'ui-{chip}-{width}x{height}.png',width,height]
        if chip=='335': command.append('335')
        result=run(command); assert result.returncode==0,result.stderr
print('PASS portable self-check: local x64 runtime, both OUT variants, UI simulation, settings lock and chip clearing')
print('PASS x86 CAN driver exports and EOF bridge handshake; no device opened')
print('PASS four offscreen layout renders')
