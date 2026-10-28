"""Validate the shipped register pack independently of its extraction script."""
import collections
import hashlib
import json
import pathlib
import re
import sys

root = pathlib.Path(__file__).resolve().parents[1]
pack = json.loads((root / 'Data/registers.json').read_text(encoding='utf-8-sig'))
registers = pack['Registers']
assert pack['Version'] and pack['GeneratedOn']
assert len(registers) > 1000, 'Register pack unexpectedly small'
keys = set()
counts = collections.Counter()
fields = checked_access = checked_reset = 0
for register in registers:
    key = (register['ChipId'], register['Name'])
    assert key not in keys, ('Duplicate register', key)
    keys.add(key)
    counts[(register['ChipId'], register['Peripheral'])] += 1
    address = int(register['Address'], 16)
    assert 0 <= address <= 0xFFFFFFFF, key
    assert register['Width'] in (16, 32, 64), (key, register['Width'])
    assert register['Source'], ('Missing source', key)
    occupied = set()
    for field in register['Fields']:
        bits = set(range(field['Lsb'], field['Lsb'] + field['Width']))
        assert bits and max(bits) < register['Width'] and min(bits) >= 0, (key, field)
        assert not occupied & bits, ('Overlapping fields', key, field['Name'])
        occupied |= bits
        assert field['Description'] and field['Access'] and field['Reset'], (key, field['Name'])
        fields += 1
        checked_access += not any(x in field['Access'] for x in ('待核对', '暂无', '未提供'))
        checked_reset += not any(x in field['Reset'] for x in ('待核对', '暂无', '未提供'))

expected_groups = {'时钟', 'GPIO', 'CAN', 'SCI', 'SPI', 'I2C', 'PWM', 'ADC', '中断', '定时器', '看门狗'}
for chip in ('035', '335', '0039', '0049'):
    actual_groups = {group for (item_chip, group) in counts if item_chip == chip}
    assert expected_groups <= actual_groups, (chip, 'Missing peripherals', expected_groups - actual_groups)

# Independent anchors from TI register maps and bit-field tables.
anchors = [
    ('035', 'ECanaRegs.CANBTC', 0x6016, 'BRPREG', 16, 8),
    ('335', 'ECanaRegs.CANBTC', 0x6016, 'TSEG2REG', 0, 3),
    ('335', 'EPwm1Regs.TBCTL', 0x6800, 'HSPCLKDIV', 7, 3),
    ('0039', 'CanaRegs.CAN_BTR', 0x4800C, 'BRPE', 16, 4),
    ('0049', 'CanaRegs.CAN_BTR', 0x4800C, 'SJW', 6, 2),
]
for chip, name, address, field_name, lsb, width in anchors:
    register = next(r for r in registers if r['ChipId'] == chip and r['Name'] == name)
    assert int(register['Address'], 16) == address, ('Map anchor', chip, name, register['Address'])
    field = next(f for f in register['Fields'] if f['Name'] == field_name)
    assert (field['Lsb'], field['Width']) == (lsb, width), ('Field anchor', chip, name, field_name)

source_checks = 0
manifest_path = root / 'docs/sources/manifest.json'
if manifest_path.exists():
    manifest = json.loads(manifest_path.read_text(encoding='utf-8-sig'))
    if isinstance(manifest, list):
        for item in manifest:
            if 'Path' not in item or 'Sha256' not in item:
                continue
            path = root / item['Path']
            assert path.is_file(), ('Source missing', item['Path'])
            assert hashlib.sha256(path.read_bytes()).hexdigest().lower() == item['Sha256'].lower(), ('Source hash mismatch', item['Path'])
            source_checks += 1

result = dict(Status='PASS', Registers=len(registers), Fields=fields,
              AccessDocumented=checked_access, ResetDocumented=checked_reset,
              SourceFilesVerified=source_checks,
              Coverage={f'{chip}/{group}': count for (chip, group), count in sorted(counts.items())})
print(json.dumps(result, ensure_ascii=False, indent=2))
if len(sys.argv) > 1:
    output = pathlib.Path(sys.argv[1])
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
