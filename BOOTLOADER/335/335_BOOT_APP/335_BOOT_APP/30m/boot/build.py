"""Build this standalone project with the locally installed TI C2000 compiler."""
import argparse
import subprocess
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--compiler', default=r'C:\ti\ti-cgt-c2000_20.2.0.LTS')
args = parser.parse_args()
compiler = Path(args.compiler)
cl = compiler/'bin/cl2000.exe'
if not cl.exists():
    raise SystemExit(f'Missing compiler: {cl}')
crystal = 30000000
label = '30MHz'
kind = 'BOOT'
project = ROOT
outdir = ROOT/'Debug'
objects = outdir/'obj'
objects.mkdir(parents=True, exist_ok=True)
log = outdir/'build.log'
flags = ['-v28', '-ml', '-mt', '--float_support=fpu32', '-g',
         f'--define=BOARD_OSC_HZ={crystal}UL',
         '--include_path=Main', '--include_path=Library/include',
         f'--include_path={compiler/"include"}', '--display_error_number']
def run(command):
    result = subprocess.run([str(cl), *command], cwd=project, capture_output=True)
    with log.open('ab') as f:
        f.write(result.stdout + result.stderr)
    if result.returncode:
        print((result.stdout+result.stderr).decode('gbk', errors='replace'))
        raise SystemExit(f'Build failed: {label}/{kind}; see {log}')
log.write_bytes(b'')
sources = sorted((project/'Main').glob('*.c')) + sorted((project/'Library/src').glob('*.c')) + sorted((project/'Library/src').glob('*.asm'))
for source in sources:
    run([*flags, '--compile_only', f'--obj_directory={os.path.relpath(objects,project)}', str(source.relative_to(project))])
name = f'AVP32F335_{kind}_{label}'
cmd = project/'Library/cmd'/('AVP32F335_Boot.cmd' if kind == 'BOOT' else 'AVP32F335_App.cmd')
link = [*flags, '--run_linker', '--search_path=Library/lib', f'--search_path={compiler/"lib"}', '--rom_model', '--entry_point=code_start', '--stack_size=0x300', '--warn_sections',
        f'--output_file={os.path.relpath(outdir/(name+".out"),project)}', f'--map_file={os.path.relpath(outdir/(name+".map"),project)}',
        *[os.path.relpath(obj,project) for obj in sorted(objects.glob('*.obj'))], str(cmd.relative_to(project)), 'Library/cmd/AVP33x_Headers_nonBIOS.cmd',
        '--search_path=Library/lib', f'--search_path={compiler/"lib"}', '--library=libc.a']
if kind == 'BOOT':
    link.append('Library/lib/Flash28335_API_V210.lib')
run(link)
print(f'Built {label}/{kind}: {outdir/(name+".out")}')
