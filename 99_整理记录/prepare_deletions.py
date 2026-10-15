"""Create an auditable deletion plan for duplicates and disposable files."""
from pathlib import Path
import json, hashlib

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT/'99_整理记录'
srcs = {x['id']:x for x in json.loads((OUT/'来源索引.json').read_text(encoding='utf-8'))}
items = {
 'S032':('完全重复','保留 S031（随原完整资料包）'),
 'S039':('完全重复','保留 S029（随原完整方案包）'),
 'S040':('完全重复','保留 S027（随原完整方案包）'),
 'S078':('完全重复','保留 S080（采用规范文件名）'),
 'S084':('完全重复','保留 S088（随 v120 配套目录）'),
 'S102':('Office 临时锁文件','无有效文档内容'),
 'S103':('Office 临时锁文件','无有效文档内容'),
 'S112':('0 字节空备注','文件名线索已写入知识库冲突记录'),
 'S127':('0 字节空备注','文件名线索已写入知识库冲突记录'),
 'S128':('0 字节空备注','文件名线索已写入知识库冲突记录'),
 'S132':('完全重复且文件名误导','与 S116 内容完全相同，保留 AVP32F0039 名下的 S116'),
 'S147':('Office 临时锁文件','无有效文档内容'),
 'S153':('Office 临时锁文件','无有效文档内容'),
 'S164':('完全重复','保留 S170（明确标作 AVP32F335B 数据手册）'),
 'S194':('完全重复','保留 S092（归在对应型号目录）'),
}
plan=[]
for sid,(reason,keep) in items.items():
    s=srcs[sid]; p=ROOT/s['path']
    assert p.is_file() and p.resolve().is_relative_to(ROOT)
    assert hashlib.sha256(p.read_bytes()).hexdigest()==s['sha256']
    plan.append({'id':sid,'path':s['path'],'bytes':s['bytes'],'sha256':s['sha256'],'reason':reason,'retained':keep})
(OUT/'删除计划.json').write_text(json.dumps(plan,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps({'files':len(plan),'bytes':sum(x['bytes'] for x in plan)},ensure_ascii=False))
