from pathlib import Path
import re
import json
import xml.etree.ElementTree as ET

root=Path(__file__).parent
cmd=(root/'F28335.cmd').read_text(encoding='utf-8',errors='replace')
cmd=re.sub(r'/\*.*?\*/','',cmd,flags=re.S)
cmd=re.sub(r'//[^\n]*','',cmd)
memory=cmd.split('MEMORY',1)[1].split('SECTIONS',1)[0]
regions=[]
page=None
for line in memory.splitlines():
    match=re.search(r'PAGE\s+(\d+)\s*:',line)
    if match:page=int(match[1])
    match=re.search(r'(\w+)\s*:\s*origin\s*=\s*(0x[\da-fA-F]+),\s*length\s*=\s*(0x[\da-fA-F]+)',line)
    if match:
        start,size=int(match[2],16),int(match[3],16)
        regions.append(dict(name=match[1],page=page,start=start,end=start+size,size=size))
region_overlaps=[(a,b) for i,a in enumerate(regions) for b in regions[i+1:]
                 if max(a['start'],b['start'])<min(a['end'],b['end'])]

mapping=(root/'Ethercat_DSP_9252-HBI-402.map').read_text(errors='replace')
allocation=mapping.split('SECTION ALLOCATION MAP',1)[1].split('GLOBAL DATA SYMBOLS',1)[0]
pattern=r'^([^\s]+)\s+(?:\n\*\s*)?(\d+)\s+([0-9a-fA-F]{8})\s+([0-9a-fA-F]{8})([^\n]*)'
map_sections={}
for match in re.finditer(pattern,allocation,re.M):
    name,page,address,size,tail=match.groups()
    map_sections[name]=dict(page=int(page),load=int(address,16),words=int(size,16),tail=tail.strip())

coff=ET.parse(root/'369.ofd.xml').getroot().find('object_file/ti_coff')
comparisons=[]
loaded=[]
for s in coff.findall('section'):
    name=s.findtext('name')
    if name not in map_sections:continue
    actual=map_sections[name]
    size=int(s.findtext('raw_data_size','0'),16)//int(s.findtext('addr_size','1'),16)
    if size==0:
        continue
    load=int(s.findtext('virtual_addr','0'),16)
    result=dict(section=name,load=f'0x{load:X}',words=size,
        matches_map=load==actual['load'] and size==actual['words'])
    comparisons.append(result)
    if s.findtext('raw_data') and s.find('copy') is None and s.find('dummy') is None:
        loaded.append((name,load,load+size))
load_overlaps=[(a,b) for i,a in enumerate(loaded) for b in loaded[i+1:] if max(a[1],b[1])<min(a[2],b[2])]
result=dict(cmd_regions=regions,cmd_physical_overlap=region_overlaps,
    map_sections_count=len(map_sections),section_comparison=comparisons,
    actual_load_overlaps=load_overlaps)
(root/'CMD_MAP检查.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
print('CMD memory regions:',len(regions),'physical overlaps:',len(region_overlaps))
print('Compared sections:',len(comparisons),'mismatches:',[c for c in comparisons if not c['matches_map']])
print('Actual data load overlaps:',load_overlaps)
print('Main sections:',[c for c in comparisons if c['section'] in ['.text','.cinit','ramfuncs','.econst','.switch','IQmath','codestart','.stack','.ebss','.sysmem','.pardata','.PMSMdata']])
