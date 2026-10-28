from pathlib import Path
import base64
import csv
import hashlib
import json
import struct
import xml.etree.ElementTree as ET

root = Path(__file__).parent
out = (root / '369.out').read_bytes()
coff = ET.parse(root / '369.ofd.xml').getroot().find('object_file/ti_coff')
image = {}
sections = []
for sec in coff.findall('section'):
    raw = sec.findtext('raw_data')
    if not raw or sec.find('copy') is not None or sec.find('dummy') is not None:
        continue
    blob = base64.b64decode(raw)
    size = int(sec.findtext('raw_data_size'), 16)
    ptr = int(sec.findtext('file_offsets/raw_data_ptr'), 16)
    assert len(blob) == size and blob == out[ptr:ptr+size]
    load = int(sec.findtext('virtual_addr'), 16)
    if not 0x300000 <= load < 0x340000:
        continue
    assert len(blob) % 2 == 0
    words = struct.unpack('<' + 'H' * (len(blob)//2), blob)
    name = sec.findtext('name')
    sections.append(dict(name=name, load=hex(load), run=sec.findtext('physical_addr'), words=len(words), end=hex(load+len(words)-1)))
    for offset, value in enumerate(words):
        address = load + offset
        assert address not in image
        image[address] = (value, name)

results = {}
all_rows = []
for filename in ['ram1.dat', 'ram3.dat', 'ram.dat', 'ram2.dat']:
    lines = (root / filename).read_text().splitlines()
    header = lines[0].split()
    start, length = int(header[2], 16), int(header[4], 16)
    values = [int(v, 16) for v in lines[1:] if v.strip()]
    assert len(values) == length
    compared, mismatches, by_section = 0, 0, {}
    for offset, value in enumerate(values):
        address = start + offset
        if address not in image:
            continue
        expected, section = image[address]
        compared += 1
        info = by_section.setdefault(section, {'compared': 0, 'different': 0})
        info['compared'] += 1
        if value != expected:
            mismatches += 1
            info['different'] += 1
            all_rows.append(dict(File=filename, Address=f'0x{address:06X}', Section=section,
                OUT=f'0x{expected:04X}', DAT=f'0x{value:04X}', XOR=f'0x{expected^value:04X}'))
    results[filename] = dict(compared=compared, different=mismatches,
        unspecified=length-compared, by_section=by_section)

with (root / 'OUT_vs_DAT.csv').open('w', encoding='utf-8-sig', newline='') as handle:
    writer = csv.DictWriter(handle, fieldnames=['File','Address','Section','OUT','DAT','XOR'])
    writer.writeheader()
    writer.writerows(all_rows)
summary = dict(out_sha256=hashlib.sha256(out).hexdigest(), sections=sections, results=results)
(root / 'OUT_comparison.json').write_text(json.dumps(summary, indent=2), encoding='utf-8')
print(json.dumps(summary, indent=2))
