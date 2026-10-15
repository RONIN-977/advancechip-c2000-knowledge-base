from pathlib import Path
import json,sys
import pypdfium2 as pdfium
root=Path(__file__).resolve().parents[1]
out=root/'99_整理记录'
sources=json.loads((out/'来源索引.json').read_text(encoding='utf-8'))
for arg in sys.argv[1:]:
    sid,_,pages=arg.partition('@')
    r=next(r for r in sources if r['id']==sid)
    pdf=pdfium.PdfDocument(str(root/r['path']))
    if not pages:
        parts=[f'来源: {r["path"]}\n来源编号: {sid}']
        for j in range(len(pdf)):
            pg=pdf[j]; tp=pg.get_textpage(); parts.append(f'=== PDF页 {j+1} ===\n'+tp.get_text_range()); tp.close(); pg.close()
        (out/r['cache']).write_text('\n\n'.join(parts),encoding='utf-8')
        r['chars']=sum(len(t) for t in parts);r['coverage']='全文提取'
        print(f'{sid} extracted {len(pdf)} pages')
    else:
        folder=out/'核对页面';folder.mkdir(exist_ok=True)
        for j in map(int,pages.split(',')):
            pg=pdf[j-1];pg.render(scale=1.5).to_pil().save(folder/f'{sid}_p{j}.png');pg.close()
            print(str(folder/f'{sid}_p{j}.png'))
    pdf.close()
(out/'来源索引.json').write_text(json.dumps(sources,ensure_ascii=False,indent=2),encoding='utf-8')
