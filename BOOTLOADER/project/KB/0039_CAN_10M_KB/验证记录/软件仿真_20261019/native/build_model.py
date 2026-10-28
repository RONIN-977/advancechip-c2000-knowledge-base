from pathlib import Path
import subprocess, hashlib, json
root=Path(r'C:\Users\17879\Desktop\CHIP\BOOTLOADER\AVP32F0039Boot+APP_CAN_10M_KB')
here=Path(__file__).resolve().parent
boot=root/'BOOT_KB'; app=root/'APP_KB'
io=(app/'src/boot_io.c').read_text(encoding='utf-8-sig')
start=io.index('static uint16_t can_wait_tx(')
end=io.index('/* Timer0 提供毫秒时基',start)
(here/'real_send.c').write_text('#include "driverlib.h"\n#include "board_config.h"\n#include "boot_io.h"\nstatic uint16_t can_transfer;\n'+io[start:end],encoding='utf-8')
files=[boot/'src/boot_core.c',app/'src/upgrade_entry.c',app/'src/boot_io.c']
(here/'source_hashes.json').write_text(json.dumps({str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},indent=2))
compiler=Path(r'C:\Users\17879\AppData\Local\Programs\Python\Python313\Lib\site-packages\ziglang\zig.exe')
import sys
if len(sys.argv)>1: compiler=Path(sys.argv[1])
cmd=[str(compiler),'cc','-shared','-O2','-Wall','-Wextra','-Werror','-I'+str(here),'-I'+str(app/'include'),str(here/'native_model.c'),str(here/'real_send.c'),str(boot/'src/boot_core.c'),str(app/'src/upgrade_entry.c'),'-o',str(here/'kb0039-native.dll')]
(here/'compile_command.json').write_text(json.dumps(cmd,indent=2))
subprocess.run(cmd,check=True)
