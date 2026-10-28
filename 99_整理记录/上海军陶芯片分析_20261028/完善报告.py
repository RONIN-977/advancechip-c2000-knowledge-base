from pathlib import Path
from zipfile import ZipFile
from copy import deepcopy
from hashlib import sha256
from lxml import etree as E
from PIL import Image
import json

BASE = Path(__file__).resolve().parent
SOURCE = Path('C:/Users/17879/Desktop/芯片检验/保威芯片分析-上海军陶.docx')
IMAGE = Path('C:/Users/17879/AppData/Local/Temp/codex-clipboard-2739d7a5-a179-4c60-a175-5d98473deae4.png')
OUTPUT = BASE / '保威芯片分析-上海军陶_完善版.docx'
NS = {'w': 'http://schemas.openxmlformats.org/wordprocessingml/2006/main',
      'wp': 'http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing',
      'a': 'http://schemas.openxmlformats.org/drawingml/2006/main'}
def tag(n): return '{' + NS['w'] + '}' + n
def prop(parent, name, **attrs):
    node = parent.find('w:' + name, NS)
    if node is None: node = E.SubElement(parent, tag(name))
    for key, value in attrs.items(): node.set(tag(key), str(value))
    return node
def replace_text(p, text):
    runs = p.findall('w:r', NS)
    exemplar = next((r for r in runs if r.find('w:rPr', NS) is not None), None)
    rp = deepcopy(exemplar.find('w:rPr', NS)) if exemplar is not None else None
    for child in list(p):
        if child.tag != tag('pPr'): p.remove(child)
    r = E.SubElement(p, tag('r'))
    if rp is not None: r.append(rp)
    t = E.SubElement(r, tag('t')); t.text = text
    return p

with ZipFile(SOURCE) as z:
    original = {n: z.read(n) for n in z.namelist()}
root = E.fromstring(original['word/document.xml'])
body = root.find('w:body', NS)
ps = body.findall('w:p', NS)
replacements = {
    8: '本次分析对象为上海军陶送检的 1 颗 ADP32F035QN56S 芯片，样品标识为“上海军陶035A-56”。客户反馈老化测试 NG，发现芯片 3.3V 短路。依据本次提供的引脚对地阻抗记录，对 VDD、VDDIO、VDDA 及 GPIO28 相关通路进行分析，测量数据如下：',
    10: '测量结果显示：VDDIO（PIN26、PIN50）对地阻抗分别为 3.3Ω、7.3Ω，GPIO28（PIN28）为 3.6Ω，均呈欧姆级低阻，异常主要集中在 VDDIO 供电域及 GPIO28 相关通路，与客户反馈的 3.3V 短路现象相符。',
    12: '本次样品 VDD（PIN3、PIN38、PIN52）对地阻抗分别为 48kΩ、48kΩ、17.5kΩ，VDDA（PIN14）为 5.69MΩ，未见与 VDDIO 相同的欧姆级对地低阻。PIN52 与另外两路 VDD 读数存在差异，建议在相同仪表、极性、量程及稳定时间下复测，并与同型号良品对比；现有记录不足以判定 VDD 阻抗是否下降，也不能仅据此确认内核和模拟电路功能正常。',
    13: '综合判定：样品 VDDIO 供电域及 GPIO28 相关通路存在对地低阻异常，可导致 IO 供电异常，影响芯片正常运行。电源或 IO 引脚遭受过压、浪涌、静电等电应力是后续排查方向，但仅凭阻抗数据，尚不能确定损伤入口、具体内部失效器件及最终根因。建议重点核查板端 3.3V 供电、上掉电时序及 GPIO28 外围连接，结合同型号良品对照和老化工况下的电源、IO 波形进一步确认。',
}
for i, text in replacements.items(): replace_text(ps[i], text)

# Preserve the source's title, logo, sample photograph, styles and page geometry.
# Compact only the summary table's blank paragraphs and excessive minimum heights.
table = body.find('w:tbl', NS)
rows = table.findall('w:tr', NS)
# Reduce the existing photograph proportionally so the complete summary table
# remains inside the first page's lower margin.
for node in table.findall('.//wp:extent', NS) + table.findall('.//a:xfrm/a:ext', NS):
    node.set('cx', str(round(int(node.get('cx')) * 0.82)))
    node.set('cy', str(round(int(node.get('cy')) * 0.82)))
for idx, tr in enumerate(rows):
    trpr = tr.find('w:trPr', NS)
    if trpr is None: trpr = E.SubElement(tr, tag('trPr'))
    prop(trpr, 'trHeight', val=[500, 420, 2300, 680, 1300][idx], hRule='atLeast')
    prop(trpr, 'cantSplit')
    for cell in tr.findall('w:tc', NS):
        tcpr = cell.find('w:tcPr', NS)
        if tcpr is None: tcpr = E.Element(tag('tcPr')); cell.insert(0, tcpr)
        prop(tcpr, 'vAlign', val='center')
        margins = prop(tcpr, 'tcMar')
        for side, size in [('top', 70), ('bottom', 70), ('left', 100), ('right', 100)]:
            prop(margins, side, w=size, type='dxa')
        cps = cell.findall('w:p', NS)
        for p in cps:
            if not ''.join(p.xpath('.//w:t/text()', namespaces=NS)).strip() and not p.findall('.//w:drawing', NS):
                if len(cell.findall('w:p', NS)) > 1: cell.remove(p)
        for p in cell.findall('w:p', NS):
            pp = p.find('w:pPr', NS)
            if pp is None: pp = E.Element(tag('pPr')); p.insert(0, pp)
            prop(pp, 'spacing', before=60, after=60, line=280, lineRule='auto')
            ind = prop(pp, 'ind', left=0, right=0, firstLine=0)
            for attr in ['firstLineChars', 'leftChars', 'rightChars', 'hanging', 'hangingChars']:
                ind.attrib.pop(tag(attr), None)
            prop(pp, 'jc', val='center' if idx == 0 else 'left')
            prop(pp, 'keepNext', val=0)
            prop(pp, 'keepLines', val=0)

def cell_text(row, col, text):
    cell = rows[row].findall('w:tc', NS)[col]
    p = cell.find('w:p', NS)
    replace_text(p, text)
    for other in cell.findall('w:p', NS)[1:]:
        if not other.findall('.//w:drawing', NS): cell.remove(other)
cell_text(3, 0, '失效模式')
cell_text(3, 1, 'VDDIO 及 GPIO28 对地呈欧姆级低阻，影响芯片正常运行。')
cell_text(4, 0, '失效机理')
cell_text(4, 1, '异常集中在 VDDIO 供电域及 GPIO28 相关通路，具体内部损伤机理及根因待进一步确认。')

# Keep the analysis together on the next page, following the template's section order.
for i in [7, 11]:
    pp = ps[i].find('w:pPr', NS)
    if pp is None: pp = E.Element(tag('pPr')); ps[i].insert(0, pp)
    prop(pp, 'keepNext', val=1)
if ps[7].find('w:pPr', NS) is not None:
    prop(ps[7].find('w:pPr', NS), 'pageBreakBefore', val=1)
for i in [8, 10, 12, 13]:
    pp = ps[i].find('w:pPr', NS)
    prop(pp, 'spacing', before=0, after=120, line=300, lineRule='auto')
    prop(pp, 'keepNext', val=0)
    prop(pp, 'keepLines', val=0)

# Replace the old customer's impedance image without redrawing or changing the supplied data.
iw, ih = Image.open(IMAGE).size
cx = 3474720  # 3.8 inches, readable within the original text width
cy = round(cx * ih / iw)
for node in ps[9].findall('.//wp:extent', NS) + ps[9].findall('.//a:xfrm/a:ext', NS):
    node.set('cx', str(cx)); node.set('cy', str(cy))
for node in ps[9].findall('.//wp:docPr', NS):
    node.set('descr', '上海军陶035A-56 芯片引脚对地阻抗实测记录')

payload = E.tostring(root, encoding='UTF-8', xml_declaration=True, standalone=True)
with ZipFile(SOURCE) as zin, ZipFile(OUTPUT, 'w') as zout:
    for info in zin.infolist():
        data = payload if info.filename == 'word/document.xml' else IMAGE.read_bytes() if info.filename == 'word/media/image3.png' else zin.read(info.filename)
        zout.writestr(info, data)
with ZipFile(OUTPUT) as z:
    changed = [n for n, data in original.items() if z.read(n) != data]
assert changed == ['word/document.xml', 'word/media/image3.png'] or set(changed) == {'word/document.xml', 'word/media/image3.png'}, changed
for stale in ['GPIO6', 'GPIO22', 'ADP32F034QP80S', 'PIN36', 'PIN70', '永久导通']:
    assert stale not in ''.join(root.xpath('//w:t/text()', namespaces=NS)), stale
record = {'source': str(SOURCE), 'source_sha256': sha256(SOURCE.read_bytes()).hexdigest(), 'output': str(OUTPUT), 'output_sha256': sha256(OUTPUT.read_bytes()).hexdigest(), 'changed_parts': changed, 'image_sha256': sha256(IMAGE.read_bytes()).hexdigest()}
(BASE / '核对记录.json').write_text(json.dumps(record, ensure_ascii=False, indent=2), encoding='utf-8')
print(json.dumps(record, ensure_ascii=False, indent=2))
