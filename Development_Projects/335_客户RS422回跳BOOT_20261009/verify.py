from pathlib import Path
import hashlib
import json
import subprocess
import shutil

root = Path(__file__).resolve().parent
verify = root / 'verification'
verify.mkdir(exist_ok=True)
headers = Path('BOOTLOADER/335_BOOT_APP/335_BOOT_APP/30m/app/Library/include').resolve()
stubs = verify / 'stubs'
stubs.mkdir(exist_ok=True)
(stubs / 'common.h').write_text('''#include "AVP33x_Device.h"
#define TURE 1U
#define VMC_BUFF_LEN 32U
#define CCDL_BUFF_LEN 32U
''')
(stubs / 'Init_2833x.h').write_text('void InitSciaGpio(void);\nvoid InitScibGpio(void);\n')
(stubs / 'gpio.h').write_text('void SCI_EN_Gpio_Init(void);\n')
(stubs / 'ccdl.h').write_text('void InitCCDLSCI(void);\nvoid InitVMCSCI(void);\n')
compiler = r'C:\ti\ti-cgt-c2000_20.2.0.LTS\bin\cl2000.exe'
shutil.copytree(headers, verify / 'device_headers', dirs_exist_ok=True)
args = [compiler, '--silicon_version=28', '--float_support=fpu32', '--abi=coffabi',
        '--display_error_number', '--compile_only', '--include_path=device_headers',
        '--include_path=..', '--include_path=stubs',
        '--include_path=C:/ti/ti-cgt-c2000_20.2.0.LTS/include',
        '--obj_directory=.', '../rs422.c']
r = subprocess.run(args, capture_output=True, cwd=verify)
(verify / 'compile.log').write_bytes(r.stdout + r.stderr)
assert r.returncode == 0, (r.stdout + r.stderr).decode(errors='replace')

original = (root / 'original/rs422.c').read_bytes().decode('gbk')
modified = (root / 'rs422.c').read_bytes().decode('gbk')
def function(source, name):
    start = source.index('void ' + name + '(')
    brace = source.index('{', start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]

checks = []
for name in ['scib_init', 'RS422_Init', 'scia_xmit', 'scib_xmit', 'serial_RXB_isr']:
    assert function(original, name) == function(modified, name), name
    checks.append(name + ': unchanged')
reset = '\r\n    /* Reset added monitor only; preserve customer SCI settings. */\r\n    rs422_boot_pending = 0U;\r\n    rs422_boot_count = 0U;\r\n'
assert function(original, 'scia_init') == function(modified, 'scia_init').replace(reset, '')
checks.append('SCI-A baud, 8E1, FIFO, GPIO and vectors: unchanged')
added_rx = '    Uint16 raw;\r\n\r\n    /* Read once and preserve the original raw business RX value. */\r\n    raw = SciaRegs.SCIRXBUF.all;\r\n    VMC_RX_BUF[VMC_RX_CNT] = raw;\r\n    RS422_BootFeed(raw);'
restored = function(modified, 'serial_RXA_isr').replace(added_rx, '    VMC_RX_BUF[VMC_RX_CNT] = SciaRegs.SCIRXBUF.all;')
assert restored == function(original, 'serial_RXA_isr')
checks.append('SCI-A original business RX body preserved; hardware read once')

def crc(data, polynomial):
    value = 0
    for byte in data:
        value ^= byte
        for _ in range(8):
            value = (value >> 1) ^ (polynomial if value & 1 else 0)
    return value

frame = bytearray.fromhex('CC 58 00')
frame.append(crc(frame, 0x8c))
frame.extend(bytes.fromhex('00 72 00 00 00'))
frame.extend(crc(frame, 0xa001).to_bytes(2, 'little'))
sample = frame.hex(' ').upper()
result = {'target_compile': 'passed (customer dependencies replaced by declaration stubs)',
          'checks': checks, 'request_hex': sample,
          'sha256': {name: hashlib.sha256((root / name).read_bytes()).hexdigest()
                     for name in ['rs422.c', 'rs422.h']},
          'hardware': 'not tested', 'full_customer_project': 'not provided'}
(verify / 'result.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
print(json.dumps(result, ensure_ascii=False, indent=2))
