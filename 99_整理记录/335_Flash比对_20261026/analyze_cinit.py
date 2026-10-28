from pathlib import Path
import json
import xml.etree.ElementTree as ET

root = Path(__file__).parent
coff = ET.parse(root/'369.ofd.xml').getroot().find('object_file/ti_coff')
symbols = []
for s in coff.findall('.//symbol'):
    value = s.findtext('value')
    if s.findtext('kind') == 'defined' and value and s.findtext('name', '').startswith('_'):
        symbols.append((int(value,16),s.findtext('name'),s.findtext('section'),s.findtext('storage_type')))

def near(address, section=None):
    items = [s for s in symbols if s[0] <= address and (section is None or s[2]==section)]
    return max(items, default=None)

data = {}
for name in ['ram1','ram','ram2']:
    words = [int(line,16) for line in (root/f'{name}.dat').read_text().splitlines()[1:]]
    data[name] = words

records = {}
result = {}
for name, words in data.items():
    p, limit = 0x31f024-0x300000, 0x32272c-0x300000
    recs=[]
    while p<limit:
        start=p
        raw_count=words[p];p+=1
        count=raw_count if raw_count<0x8000 else 0x10000-raw_count
        if count==0:
            break
        dest=words[p];p+=1
        if raw_count>=0x8000:
            dest|=words[p]<<16;p+=1
        if p+count>limit:
            raise ValueError((name,hex(p+0x300000),count))
        recs.append(dict(header=start+0x300000,source=p+0x300000,dest=dest,count=count))
        p+=count
    records[name]=recs
    result[name]=dict(records=len(recs),terminator=hex(p-1+0x300000),next_word=hex(p+0x300000))

good=records['ram1']
assert good==records['ram']==records['ram2']
affected=[]
for name in ['ram','ram2']:
    for i,(a,b) in enumerate(zip(data['ram1'],data[name])):
        addr=i+0x300000
        if a==b or not 0x31f024<=addr<0x32272c:continue
        rec=next(r for r in good if r['source']<=addr<r['source']+r['count'])
        dest=rec['dest']+addr-rec['source']
        affected.append(dict(file=name,flash=f'0x{addr:06X}',ram=f'0x{dest:06X}',
            good=f'0x{a:04X}',bad=f'0x{b:04X}',record={k:hex(v) for k,v in rec.items()},
            nearest_symbol=near(dest,'.pardata')))
result['affected']=affected
result['text_function_near_difference']=near(0x3082ae,'.text')
(root/'cinit_analysis.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
print(json.dumps({k:v for k,v in result.items() if k!='affected'},indent=2))
print('Affected first/last per file:')
for name in ['ram','ram2']:
    subset=[a for a in affected if a['file']==name]
    print(json.dumps([subset[0],subset[-1]],indent=2))
print('Records covering affected data:')
print(json.dumps([r for r in good if any(r['source']<=int(a['flash'],16)<r['source']+r['count'] for a in affected)],indent=2))
