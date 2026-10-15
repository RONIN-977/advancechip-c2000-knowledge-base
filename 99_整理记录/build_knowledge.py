"""Render the knowledge base with links to the post-organization locations."""
from pathlib import Path
import json, re, os, csv

ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'99_整理记录'
KB=ROOT/'00_替代与差异知识库'
TPL=OUT/'知识库模板'
sources=json.loads((OUT/'来源索引.json').read_text(encoding='utf-8'))
src={x['id']:x for x in sources}
moves={x['id']:x['to'] for x in json.loads((OUT/'分类移动计划.json').read_text(encoding='utf-8'))}
deletions={x['id']:x for x in json.loads((OUT/'删除计划.json').read_text(encoding='utf-8'))}
chip036_moves=sorted(json.loads((OUT/'036_路径迁移.json').read_text(encoding='utf-8')),key=lambda x:len(x['from']),reverse=True)

def esc_label(s): return s.replace('[','\\[').replace(']','\\]').replace('|','\\|')
def current(sid):
    path=moves.get(sid,src[sid]['path'])
    for item in chip036_moves:
        if path==item['from'] or path.startswith(item['from']+'/'):
            return item['to']+path[len(item['from']):]
    return path
def link(sid):
    if sid in deletions:
        return f"{sid}（已删除：{deletions[sid]['reason']}；{deletions[sid]['retained']}）"
    rel=Path(os.path.relpath(ROOT/current(sid),KB)).as_posix()
    return f"[{sid}：{esc_label(Path(current(sid)).name)}](<{rel}>)"

KB.mkdir(exist_ok=True)
for p in TPL.glob('*.md'):
    text=p.read_text(encoding='utf-8')
    text=re.sub(r'\{\{(S\d{3})\}\}',lambda m:link(m.group(1)),text)
    if re.search(r'\{\{S\d{3}\}\}',text): raise RuntimeError(f'未替换来源: {p}')
    (KB/p.name).write_text(text,encoding='utf-8')

lines=['# 原始资料索引','',
 '本索引记录整理前发现的 238 个 PDF、Word、Excel、PowerPoint 与顶层 txt。路径链接已经更新为整理后位置；删除项保留审计信息。文本提取状态只表示可检索程度，不表示逐页审查完成。','',
 '| ID | 当前资料 | 状态 | 提取范围 | 大小 |', '|---|---|---|---|---:|']
for s in sources:
    sid=s['id']; size=f"{s['bytes']:,}"
    if sid in deletions:
        d=deletions[sid]
        item=esc_label(Path(s['path']).name)
        state=f"已删除：{d['reason']}；{d['retained']}"
    else:
        item=link(sid); state='已分类移动' if sid in moves else '保留在成套/工程目录'
    coverage=s.get('coverage') or ('提取失败：'+s.get('error','未知'))
    lines.append(f"| {sid} | {item} | {state} | {esc_label(str(coverage))} | {size} |")
(KB/'04_原始资料索引.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')

del_lines=['# 删除记录','',
 '原始文档整理阶段只删除内容完全相同的重复副本、Office 临时锁文件和 0 字节文件。036 程序的后续清理另见下文。','',
 '| ID | 原路径 | 原因 | 保留内容 | SHA-256 |', '|---|---|---|---|---|']
for d in deletions.values():
    del_lines.append(f"| {d['id']} | `{d['path']}` | {d['reason']} | {d['retained']} | `{d['sha256']}` |")
with (OUT/'036_删除文件清单.csv').open(encoding='utf-8-sig',newline='') as f:
    chip036_deleted=list(csv.DictReader(f))
del_lines.extend(['', '## 036 程序重分类清理', '',
 f'按用户确认，删除旧 LIN/CAN BOOT 工程及旧 LIN BOOT 压缩包；另删除 9 个与完整例程包的非 Debug 源码哈希相同的旧例程目录，以及待核实旧工程内的 CCS/浏览器工作区缓存。共记录 {len(chip036_deleted)} 个被删文件的路径、大小、SHA-256、原因及保留对应项。完整明细：[036_删除文件清单.csv](<../99_整理记录/036_删除文件清单.csv>)。旧 APP、上位机及待核实工程源码仍保留。'])
(KB/'06_删除记录.md').write_text('\n'.join(del_lines)+'\n',encoding='utf-8')

kb_readme='''# 进芯替代 TI C2000 资料知识库

建议从 [01_型号替代总表](<01_型号替代总表.md>) 开始，再按项目查看 [02_差异学习笔记](<02_差异学习笔记.md>) 和 [03_资料冲突与待核实](<03_资料冲突与待核实.md>)。资料是否已经覆盖某型号见 [05_学习进度](<05_学习进度.md>)；程序专项见 [335 CAN 在线升级](<08_程序学习_335_CAN在线升级.md>)、[335 Rev2.0 例程](<09_程序学习_335_Rev2.0例程.md>)、[ADP035 UART/CAN 升级与例程](<10_程序学习_ADP035_升级与例程.md>)、[0039 升级与例程](<11_程序学习_0039_升级与例程.md>)、[0049 升级与例程](<12_程序学习_0049_升级与例程.md>)、[069 例程与升级缺口](<13_程序学习_069_例程与升级资料缺口.md>)和 [036 FOC 与在线升级](<14_程序学习_036_FOC与在线升级.md>)；集中确认结果和剩余跟踪项见 [07_集中确认结果](<07_待集中确认问题.md>)；所有原始资料和提取范围见 [04_原始资料索引](<04_原始资料索引.md>)，精简记录见 [06_删除记录](<06_删除记录.md>)。

分类目录的统一含义：`01_数据手册`、`02_替代与移植差异`、`03_勘误与应用注意`、`04_硬件与开发板`、`05_烧录与开发说明`、`06_TI参考资料`、`07_应用方案`、`08_培训资料`。不是每个型号都需要全部目录。

各型号资料仍按型号查找；036 已单独按[器件、FOC、在线升级、例程和硬件工具](<../036/README.md>)重分类，成套工程内部结构保留。带版本号的手册全部保留，不能只看文件名日期判断有效性；存在冲突的结论已集中标注。
'''
(KB/'README.md').write_text(kb_readme,encoding='utf-8')

root_readme='''# 进芯替代 TI C2000 系列资料

本资料库已按“型号 → 资料用途”整理。学习结论入口：[00_替代与差异知识库/README.md](<00_替代与差异知识库/README.md>)。

- TI 型号反查进芯：[型号替代总表](<00_替代与差异知识库/01_型号替代总表.md>)
- 硬件、软件和版本差异：[差异学习笔记](<00_替代与差异知识库/02_差异学习笔记.md>)
- 不能直接照用的冲突资料：[资料冲突与待核实](<00_替代与差异知识库/03_资料冲突与待核实.md>)
- 已学与待补资料：[型号学习进度](<00_替代与差异知识库/05_学习进度.md>)

程序专项已完成 [335 CAN 在线升级静态学习](<00_替代与差异知识库/08_程序学习_335_CAN在线升级.md>)、[335 Rev2.0 开发板例程梳理](<00_替代与差异知识库/09_程序学习_335_Rev2.0例程.md>)、[ADP035 UART/CAN 升级与例程](<00_替代与差异知识库/10_程序学习_ADP035_升级与例程.md>)、[0039 升级与例程](<00_替代与差异知识库/11_程序学习_0039_升级与例程.md>)、[0049 升级与例程](<00_替代与差异知识库/12_程序学习_0049_升级与例程.md>)、[069 例程与升级缺口](<00_替代与差异知识库/13_程序学习_069_例程与升级资料缺口.md>)和 [036 FOC 与在线升级](<00_替代与差异知识库/14_程序学习_036_FOC与在线升级.md>)；[036 目录导航](<036/README.md>)列出新版 BOOT 的通用范围和手工同步清单。保留工程的源码未修改。整理审计、原文件清单、哈希、文本缓存和执行记录在 `99_整理记录`。
'''
(ROOT/'README.md').write_text(root_readme,encoding='utf-8')
print(json.dumps({'knowledge_files':len(list(KB.glob('*.md'))),'unresolved_placeholders':sum(len(re.findall(r'\{\{S\d{3}\}\}',p.read_text(encoding='utf-8'))) for p in KB.glob('*.md'))},ensure_ascii=False))
