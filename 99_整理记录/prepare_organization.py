"""Build a reviewed, file-only move plan. Does not move original materials."""
from pathlib import Path
import json, hashlib

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / '99_整理记录'
sources = json.loads((OUT/'来源索引.json').read_text(encoding='utf-8'))
by_id = {s['id']:s for s in sources}
assignments = {}
# Deleted separately: exact duplicate loose copies, empty filename-only notes, and Office lock files.
DELETE_IDS = {'S032','S039','S040','S078','S084','S102','S103','S112','S127','S128','S132','S147','S153','S164','S194'}

def assign(category, numbers):
    for n in numbers:
        sid = f'S{n:03d}'
        assert sid not in assignments, sid
        assignments[sid] = category

assign('01_数据手册', [8,16,19,20,21,24,25,36,58,64,72,76,77,79,81,82,83,93,121,135,138,139,140,148,151,155,156,170,175,192])
assign('02_替代与移植差异', [33,34,50,57,65,69,70,71,73,74,75,78,80,89,90,94,100,114,117,118,120,124,125,126,134,136,141,142,150,157,168,169,179,182,185,186,187,188,189])
assign('03_勘误与应用注意', [1,22,51,59,63,66,67,112,127,128,143,144,158,173,174,177,184])
assign('04_硬件与开发板', [6,7,11,12,13,14,15,18,23,26,32,37,39,52,53,54,55,111,116,119,129,131,132,133,137,145,154,172,190,191])
assign('05_烧录与开发说明', [35,38,56,113,115,122,130,167,171,176,178])
assign('06_TI参考资料', [49,84,91,92,98,99,123,149,181,183])
assign('07_应用方案', [40,44,45,46])
assign('08_培训资料', [180])

# General loose documents: keep cross-family material together; rehome model-specific items.
general = {
194:'文档/01_选型与跨型号对比/TI原厂参考',195:'文档/01_选型与跨型号对比/TI原厂参考',
196:'AVP32F0049/06_TI参考资料',197:'文档/01_选型与跨型号对比/TI原厂参考',
198:'文档/02_培训与产品介绍',199:'文档/01_选型与跨型号对比',200:'文档/01_选型与跨型号对比',
201:'文档/02_培训与产品介绍',202:'036/03_勘误与应用注意',
203:'文档/06_其他系列/ADP16F03',204:'036/ADP32F036/02_替代与移植差异/来自原文档目录',
205:'036/ADP32F036/02_替代与移植差异/来自原文档目录',206:'036/ADP32F036/05_烧录与开发说明/来自原文档目录',
208:'文档/06_其他系列/AVP32F075',209:'文档/01_选型与跨型号对比',
210:'文档/04_外设参考',211:'文档/04_外设参考',212:'文档/04_外设参考',
213:'文档/03_烧录与调试',214:'文档/06_其他系列/ADP16F01',215:'文档/03_烧录与调试',
216:'文档/04_外设参考',217:'文档/04_外设参考',218:'文档/01_选型与跨型号对比',
219:'文档/03_烧录与调试',220:'文档/03_烧录与调试',221:'文档/03_烧录与调试',
222:'文档/01_选型与跨型号对比/TI原厂参考',223:'文档/03_烧录与调试',
224:'文档/03_烧录与调试',225:'文档/02_培训与产品介绍',226:'文档/04_外设参考',
227:'文档/05_经验记录',228:'文档/01_选型与跨型号对比',229:'文档/01_选型与跨型号对比',
230:'文档/01_选型与跨型号对比',231:'文档/03_烧录与调试',232:'ADP32F035/02_替代与移植差异',
233:'文档/06_其他系列/ADP16F03',234:'文档/02_培训与产品介绍',
235:'文档/01_选型与跨型号对比',236:'文档/01_选型与跨型号对比',237:'文档/01_选型与跨型号对比',
238:'文档/03_烧录与调试',193:'文档/05_经验记录',
}
plan = []
for s in sources:
    if s['id'] in DELETE_IDS:
        continue
    old = Path(s['path'])
    if s['id'] in assignments:
        # Only loose files, never move files out of engineering or library bundles.
        assert len(old.parts)==2 or (old.parts[0]=='036' and len(old.parts)==3), s['path']
        parent = old.parent
        if s['id']=='S150': parent=Path('AVP32F035')
        new = parent / assignments[s['id']] / old.name
    elif int(s['id'][1:]) in general:
        new = Path(general[int(s['id'][1:])]) / old.name
    else:
        continue
    src = ROOT/old
    dst = ROOT/new
    assert src.is_file(), str(src)
    assert not dst.exists(), str(dst)
    assert src.resolve().is_relative_to(ROOT) and dst.resolve().is_relative_to(ROOT)
    assert hashlib.sha256(src.read_bytes()).hexdigest()==s['sha256'], s['id']
    plan.append({'id':s['id'],'from':old.as_posix(),'to':new.as_posix(),'bytes':s['bytes'],'sha256':s['sha256']})
assert len({p['to'].lower() for p in plan})==len(plan)
(OUT/'分类移动计划.json').write_text(json.dumps(plan,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps({'original_files':len(json.loads((OUT/'整理前文件清单.json').read_text(encoding='utf-8'))),'document_candidates':len(sources),'planned_file_moves':len(plan),'original_bytes_to_move':sum(p['bytes'] for p in plan)},ensure_ascii=False))
