from pathlib import Path
import sys, re, json
root=Path(__file__).parent
for arg in sys.argv[1:]:
    if arg=='--list':
        for r in json.loads((root/'来源索引.json').read_text(encoding='utf-8')):
            print(r['id'],r.get('units'),r.get('chars'),r['path'])
    else:
        sid,_,pattern=arg.partition(':')
        sid,_,pages=sid.partition('@')
        p=root/'文本缓存'/f'{sid}.txt'
        data=p.read_text(encoding='utf-8')
        if pages:
            wanted=set()
            for part in pages.split(','):
                a,_,b=part.partition('-'); wanted.update(range(int(a),int(b or a)+1))
            blocks=re.split(r'(=== PDF页 \d+ ===)',data)
            data=blocks[0]+''.join(blocks[i]+blocks[i+1] for i in range(1,len(blocks),2) if int(re.search(r'\d+',blocks[i])[0]) in wanted)
        lines=data.splitlines()
        if pattern:
            indices=set(range(2))
            for i,line in enumerate(lines):
                if re.search(pattern,line,re.I): indices.update(range(max(0,i-4),min(len(lines),i+7)))
            lines=[line for i,line in enumerate(lines) if i in indices]
        print('\n'.join(l for l in lines if l.strip() and '....' not in l))
