from pathlib import Path
import subprocess
import sys

root=Path(__file__).resolve().parents[1]
cc=sys.argv[1] if len(sys.argv)>1 else 'C:/ADVCHIP/UDS_OriCode/.test-tools/ziglang/zig.exe'
out=root/'build/tests'
out.mkdir(parents=True,exist_ok=True)
base=[cc,'cc','-O0','-Wall','-Wextra','-Werror','-I'+str(root/'Common'),'-I'+str(root/'Bootloader/Common'),
      '-I'+str(root/'Bootloader'),str(root/'tests/flash_layout_test.c')]
valid=[{}, {'PART_APP_START':'0x3EA000UL'}, {'PART_APP_START':'0x3F0000UL'},
       {'PART_APP_END':'0x3F0000UL'},
       {'PART_APP_END':'0x3F0000UL','PART_METADATA':'0x3F0000UL','PART_BOOT_START':'0x3F2000UL'},
       {'PART_APP_END':'0x3F4000UL','PART_METADATA':'0x3F4000UL','PART_BOOT_START':'0x3F6000UL'}]
for i,defines in enumerate(valid):
    exe=out/f'partition-{i}.exe'
    subprocess.run(base+[f'-D{k}={v}' for k,v in defines.items()]+['-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
invalid=[{'PART_APP_START':'0x3EA004UL'}, {'PART_APP_END':'0x3F1FFCUL'},
         {'PART_APP_START':'0x3E6000UL'}, {'PART_APP_START':'0x3F2000UL'},
         {'PART_METADATA':'0x3F0000UL'}, {'PART_METADATA':'0x3F4000UL'},
         {'PART_BOOT_START':'0x3F8000UL'}, {'PART_APP_END':'0xFFFFFFFFUL'}]
for defines in invalid:
    result=subprocess.run(base+[f'-D{k}={v}' for k,v in defines.items()]+['-c','-o',str(out/'invalid.obj')],capture_output=True)
    assert result.returncode!=0 and b'error:' in result.stderr and b'FileNotFound' not in result.stderr,defines
print('PASS 6 layouts, all 4-WORD image lengths per layout, physical masks, write limits; 8 invalid configurations rejected at compile time')
