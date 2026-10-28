"""Run with Python 3 after build.ps1. Addresses are C28x word addresses."""
from pathlib import Path
import hashlib
import re
import xml.etree.ElementTree as ET
import sys

root = Path(__file__).resolve().parents[1]
config = (root / 'Bootloader/Common/partition_config.h').read_text()
def setting(name):
    if name == 'PART_APP_START':
        return int(re.search(r'^#define\s+APP_START_ADDRESS\s+(0x[0-9A-Fa-f]+)',
                            (root/'Common/app_partition.h').read_text(), re.M)[1],16)
    return int(re.search(r'^#define\s+' + name + r'\s+(0x[0-9A-Fa-f]+)', config, re.M)[1], 16)
app_start, app_end, metadata, boot_start = [setting(n) for n in
    ('PART_APP_START', 'PART_APP_END', 'PART_METADATA', 'PART_BOOT_START')]
def region(project, name):
    text=(root/project/(project.lower()+'.cmd')).read_text()
    m=re.search(r'\b'+name+r'\s*:\s*origin\s*=\s*([^,\n]+),\s*length\s*=\s*([^\n]+)', text)
    assert m, name
    def value(expression):
        for macro in ('PART_APP_START','PART_APP_END','PART_BOOT_START'):
            expression=expression.replace(macro,str(setting(macro)))
        assert re.fullmatch(r'[0-9a-fA-FxX()+*/ \t-]+',expression),expression
        return int(eval(expression,{'__builtins__':{}},{}))
    return tuple(value(v) for v in m.groups())
assert region('Application','BEGIN') == (app_start,2), 'APP codestart disagrees with partition_config.h'
assert region('Application','APPFLASH') == (app_start+2,app_end-app_start-2), 'APP linker bounds disagree'
assert region('Bootloader','BOOTFLASH') == (boot_start,0x3f7f80-boot_start), 'BOOT linker bounds disagree'
assert region('Bootloader','BEGIN') == (0x3f7ff6,2), 'BOOT reset entry must stay fixed'
assert app_start < app_end <= metadata and metadata+0x2000 <= boot_start
assert hashlib.sha256((root / 'Bootloader/Device/lib/2803x_FlashAPI_BootROMSymbols.lib').read_bytes()).hexdigest() == '0f01a9c5f2bbf4d5ddcd710b586dce7354097f2f808fd099e252253ad28656fc'
for project in ('Bootloader', 'Application'):
    for name in ('.project', '.cproject', '.ccsproject'):
        ET.parse(root / project / name)
    for link in ET.parse(root / project / '.project').findall('.//locationURI'):
        assert link.text.startswith('PARENT-1-PROJECT_LOC/')
        assert (root / link.text.split('/', 1)[1]).is_file(), link.text
    content = (root / project / (project.lower() + '.cmd')).read_text()
    assert 'origin = 0x0003F8, length = 0x000004' in content
    assert 'length = 0x0003A8' in content
    text = (root / 'output' / ('ADP32F035_' + project + '.map')).read_text()
    if project == 'Bootloader':
        if len(sys.argv) > 1:
            text = Path(sys.argv[1]).read_text()
        def addr(symbol):
            match = re.search(r'^\s*(?:0|1|abs)\s+([0-9a-fA-F]{8})\s+_' + re.escape(symbol) + r'\s*$', text, re.M)
            assert match, symbol
            return int(match[1], 16)
        for symbol, expected in {'Flash2803x_Program': 0x3ff8d2, 'Flash2803x_Erase': 0x3ff9b3,
                                 'Flash2803x_Verify': 0x3ffdd5, 'Flash2803x_APIVersionHex': 0x3ffec1,
                                 'Flash_CPUScaleFactor': 0xd04, 'Flash_CallbackPtr': 0xd02}.items():
            assert addr(symbol) == expected, symbol
        assert 0x8000 <= addr('RamfuncsRunStart') < 0x9000
        for prefix in ('Ramfuncs',):
            assert addr(prefix + 'LoadSize') > 0, 'Empty RAM section: ' + prefix
            assert addr(prefix + 'RunStart') + addr(prefix + 'LoadSize') <= 0x9000
            assert boot_start <= addr(prefix + 'LoadStart') < 0x3f7f80
        for helper in ('flash_range_mask', 'flash_write_allowed', 'flash_begin_plan', 'api_erase', 'begin_image'):
            assert '(ramfuncs:_' + helper + ')' in text, 'Not in RAM section: ' + helper
        assert 'Flash2803x_API_V100.lib' not in text, 'External algorithm linked unexpectedly'
print('PASS partition/linker consistency, ROM library hash, CCS resources, reserved RAM, exact ROM API/data addresses and all RAM callers')
