"""Build English register documents directly from the bundled TI PDFs.

The cropped originals are authoritative. Structured field text is an aid to
selection, not a replacement for a table which contains diagrams or footnotes.
"""
import argparse
import collections
import hashlib
import json
import pathlib
import re
import sys
import unicodedata

import pypdfium2 as pdfium
from pypdf import PdfReader

ROOT = pathlib.Path(__file__).resolve().parents[1]
SOURCES = ROOT / 'docs/sources'
MANUALS = {'035': 'sprui10', '335': 'sprui07', '0039': 'spruiw9', '0049': 'sprui33'}
FIRST_BODY = {'sprui10': 31, 'sprui07': 38, 'spruiw9': 89, 'sprui33': 73}
ACCESS = {'只读': 'R', '只写': 'W', '读写': 'R/W', '读；写1清零': 'R/W1C',
          '读写；硬件自动清零': 'R/W1S', '只写；读回0': 'W (reads as 0)',
          '所有模式可读；初始化且EALLOW时可写': 'RWPI', '读写（INIT=1且CCE=1时配置）': 'R/W'}


def load(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8')


def clean(text):
    text = text.replace('\r\n', '\n').replace('\r', '\n')
    text = text.replace('\u2002', ' ').replace('\u2003', ' ').replace('\u00ad', '')
    lines = text.splitlines()
    result = []
    for line in lines:
        if re.search(r'www\.ti\.com|Copyright ©|Submit Document|SPRUI(?:07|10|33|W9)|Texas Instruments Incorporated', line, re.I):
            continue
        if re.match(r'^\d+\s+TMS320', line):
            continue
        result.append(line)
    return '\n'.join(result).strip()


def text_cache(code, pdf):
    path = ROOT / f'docs/manual-audit-text-{code}.json'
    if path.exists():
        return load(path)
    pages = {}
    for index in range(len(pdf)):
        page = pdf[index]
        tp = page.get_textpage()
        pages[str(index + 1)] = tp.get_text_range().replace('\r\n', '\n')
        tp.close(); page.close()
        if index % 250 == 0:
            print(f'{code}: text {index + 1}/{len(pdf)}', flush=True)
    save(path, pages)
    return pages


def line_top(pdf, page_number, offset):
    page = pdf[page_number - 1]
    tp = page.get_textpage()
    text = tp.get_text_range().replace('\r\n', '\n')
    # PDFium character positions include CRLF; resolve by matching the source line.
    line = text[offset:text.find('\n', offset) if '\n' in text[offset:] else len(text)].strip()
    original = tp.get_text_range()
    at = original.find(line)
    if at < 0: at = 0
    for i in range(at, min(at + max(1, len(line)), tp.count_chars())):
        box = tp.get_charbox(i)
        if box[3] > box[1]:
            value = page.get_height() - box[3] - 4
            tp.close(); page.close()
            return max(35, min(735, value))
    tp.close(); page.close()
    return 45


def headings(code, pdf, pages):
    result = []
    for number, text in pages.items():
        number = int(number)
        if number < FIRST_BODY[code]: continue
        for m in re.finditer(r'^(\d+(?:\.\d+){1,6})\s+([^\n]+)', text, re.M):
            title = m.group(2).strip()
            if len(title) < 3 or re.match(r'\d|Reserved\b', title): continue
            result.append(dict(Page=number, Offset=m.start(), Number=m.group(1), Title=title,
                               Kind='section', Top=None))
    # Old TRMs have some headings rendered as separated text objects. Their PDF
    # bookmarks supply the exact heading and page, then text resolves its location.
    reader = PdfReader(SOURCES / f'{code}.pdf')
    def walk(items):
        for item in items:
            if isinstance(item, list):
                walk(item); continue
            m = re.match(r'^(\d+(?:\.\d+)*)\s+(.+)', unicodedata.normalize('NFKC', item.title))
            if not m: continue
            try: page_number = reader.get_destination_page_number(item) + 1
            except Exception: continue
            if page_number < FIRST_BODY[code]: continue
            if any(h['Page'] == page_number and h['Number'] == m.group(1) for h in result): continue
            text = pages.get(str(page_number), '')
            at = text.find(m.group(1) + ' ')
            if at < 0:
                match = re.search(r'^' + re.escape(m.group(1)) + r'\s', text, re.M)
                at = match.start() if match else 0
            result.append(dict(Page=page_number, Offset=at, Number=m.group(1), Title=m.group(2),
                               Kind='section', Top=None))
    walk(reader.outline)
    result.sort(key=lambda h: (h['Page'], h['Offset'], len(h['Number'])))
    for index, h in enumerate(result):
        depth = h['Number'].count('.')
        next_h = next((n for n in result[index + 1:] if n['Number'].count('.') <= depth), None)
        h['EndPage'] = next_h['Page'] if next_h else h['Page'] + 1
        h['EndOffset'] = next_h['Offset'] if next_h else 0
    return result


def aliases(register):
    name = register['Name'].split('.')[-1]
    names = {name}
    if register['ChipId'] in ('035', '335'):
        if re.fullmatch(r'LAM\d+', name): names.add('CANLAM')
        if re.fullmatch(r'MOTO\d+', name): names.add('MOTO')
        if re.fullmatch(r'MOTS\d+', name): names.add('MOTS')
        if name in ('MDL', 'MDH'): names.add('CAN' + name)
        if register['Peripheral'] == '定时器': names.update(['TIMERx' + name, 'TIMERx' + name + 'H'])
        if name.startswith('ADCRESULT'): names.add('ADCRESULTx')
        if re.fullmatch(r'ADCSOC\d+CTL', name): names.add('ADCSOCxCTL')
        if re.fullmatch(r'INTSEL\d+N\d+', name): names.add('INTSELxNy')
        if re.fullmatch(r'XINT\d+CR', name): names.add('XINTnCR')
        if re.fullmatch(r'XINT\d+CTR', name): names.add('XINTnCTR')
        if re.fullmatch(r'PIE(?:IER|IFR)\d+', name): names.add(re.sub(r'\d+$', 'x', name))
    return names


def token_match(title, names):
    return any(re.search(r'(?<![A-Za-z0-9_])' + re.escape(n) + r'(?![A-Za-z0-9_])', title, re.I) for n in names)


def candidate_sections(register, sections, pages):
    names = aliases(register)
    short = register['Name'].split('.')[-1]
    found = []
    for h in sections:
        if not token_match(h['Title'], names): continue
        if 'register' not in h['Title'].lower() and '(' not in h['Title'] and 'mask' not in h['Title'].lower():
            continue
        span = h['EndPage'] - h['Page']
        if span < 0 or span > 10: continue
        score = 40 + (20 if re.search(r'^' + re.escape(short) + r'\s+Register', h['Title'], re.I) else 0)
        score += 5 if 'register' in h['Title'].lower() else 0
        score -= span
        found.append((score, h))
    return found


def virtual_sections(register, sections, pages):
    names = aliases(register)
    candidates = []
    # Exact Figure/Table captions identify old-style register blocks without
    # numbered per-register sections. Each block ends at the next register figure.
    for number, text in pages.items():
        if int(number) < FIRST_BODY[MANUALS[register['ChipId']]]: continue
        markers = list(re.finditer(r'^(Figure|Table)\s+[\d-]+\.\s*([^\n]+)', text, re.M))
        for m in markers:
            title = m[2]
            if not token_match(title, names): continue
            if not re.search(r'Register|Bit Definitions|Bit Description|Masks', title, re.I): continue
            page = int(number)
            # A figure is preferred over its later field-description table.
            if m[1] == 'Table' and any(x['Page'] == page and token_match(x['Title'], names) for x in candidates): continue
            end_page = page
            end_offset = len(text)
            for j in range(page, min(page + 7, len(pages)) + 1):
                content = pages.get(str(j), '')
                start = m.end() if j == page else 0
                boundaries = []
                for n in re.finditer(r'^(?:Figure\s+[\d-]+\.\s*([^\n]+)|(\d+(?:\.\d+){1,6})\s+([^\n]+))', content[start:], re.M):
                    caption = n[1] or n[3]
                    if token_match(caption, names): continue
                    if n[1] and not re.search(r'Register|Registers|Layout|Bit Definitions', caption, re.I): continue
                    boundaries.append(start + n.start())
                if boundaries:
                    end_page = j; end_offset = min(boundaries); break
                end_page = j; end_offset = len(content)
            candidates.append(dict(Page=page, Offset=m.start(), Number='', Title=title, Kind='figure', Top=None,
                                   EndPage=end_page, EndOffset=end_offset))
    return [(25 - (c['EndPage'] - c['Page']), c) for c in candidates]


def body_for(h, pages):
    blocks = []
    for page in range(h['Page'], h['EndPage'] + 1):
        text = pages.get(str(page), '')
        start = h['Offset'] if page == h['Page'] else 0
        end = h['EndOffset'] if page == h['EndPage'] else len(text)
        blocks.append(clean(text[start:end]))
    return '\n\n'.join(x for x in blocks if x)


def english_field(field, body):
    names = [field['Name']]
    if re.match(r'rsvd', field['Name'], re.I): names.append('Reserved')
    if re.match(r'GPIO\d+', field['Name']): names += ['GPIOx', 'GPIO']
    bits = str(field['Lsb']) if field['Width'] == 1 else f'{field["Lsb"] + field["Width"] - 1}[-:]{field["Lsb"]}'
    matches = []
    for name in names:
        pattern = r'(?m)^' + bits + r'\s+' + re.escape(name) + r'\b[^\n]*'
        for m in re.finditer(pattern, body, re.I):
            following = body[m.start():]
            end = re.search(r'\n\d+(?:[-:]\d+)?\s+[A-Z_][A-Z_0-9]+\b', following[m.end() - m.start():])
            length = m.end() - m.start() + end.start() if end else len(following)
            text = following[:length].strip()
            if len(text) > 15: matches.append(text)
    text = max(matches, key=len) if matches else ''
    if not text:
        # The preceding version retained the exact source-table text alongside
        # its translation. Reuse only that English evidence, never translated values.
        notes = field.get('Notes', '')
        if '原文字段表：' in notes:
            text = notes.split('原文字段表：', 1)[1].split('；关键字段', 1)[0]
        elif notes.startswith('原文：'):
            text = notes[3:].split('；', 1)[0]
    text = clean(text)
    text = re.sub(r'[\u4e00-\u9fff]+[^\n]*', '', text).strip()
    access, reset = '', ''
    row = re.match(r'^\d+(?:[-:]\d+)?\s+\w+\s+((?:R|W)[A-Z0-9/-]*)\s+([A-Fa-f0-9]+h|[xX-])\s*(.*)', text, re.S)
    if row: access, reset, text = row.groups()
    if not access: access = ACCESS.get(field.get('Access', ''), '')
    if not reset and re.fullmatch(r'[A-Fa-f0-9]+h?', field.get('Reset', '')): reset = field['Reset']
    if re.match(r'rsvd', field['Name'], re.I) and not text: text = 'Reserved'
    return dict(Name=field['Name'], Lsb=field['Lsb'], Width=field['Width'], Description=text, Values='', Access=access, Reset=reset)


def render_pages(pdf, h, code, doc_id, image_root):
    images = []
    for number in range(h['Page'], h['EndPage'] + 1):
        if number == h['EndPage'] and h['EndOffset'] == 0: continue
        page = pdf[number - 1]
        width, height = page.get_size()
        top = line_top(pdf, number, h['Offset']) if number == h['Page'] else 42
        bottom = line_top(pdf, number, h['EndOffset']) - 2 if number == h['EndPage'] and h['EndOffset'] else height - 50
        if bottom <= top + 8:
            page.close(); continue
        scale = 1.5
        bitmap = page.render(scale=scale)
        image = bitmap.to_pil().crop((int(48 * scale), int(top * scale), int((width - 38) * scale), int(bottom * scale)))
        name = f'{doc_id}-{number}.png'
        path = image_root / code / name
        path.parent.mkdir(parents=True, exist_ok=True)
        image.save(path, optimize=True)
        images.append(dict(ImagePath=str(path.relative_to(ROOT)).replace('\\', '/'), PageNumber=number))
        bitmap.close(); page.close()
    return images


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--no-render', action='store_true')
    parser.add_argument('--chips', default='035,335,0039,0049')
    args = parser.parse_args()
    registers = load(ROOT / 'Data/registers.json')['Registers']
    documents, mapping, audit = {}, {}, []
    image_root = ROOT / 'Data/manual-images'
    for chip in args.chips.split(','):
        code = MANUALS[chip]
        pdf = pdfium.PdfDocument(SOURCES / f'{code}.pdf')
        pages = text_cache(code, pdf)
        sections = headings(code, pdf, pages)
        chip_regs = [r for r in registers if r['ChipId'] == chip]
        unique = {}
        for r in chip_regs:
            alias_key = (r['Peripheral'], tuple(sorted(aliases(r))), tuple((f['Name'], f['Lsb'], f['Width']) for f in r['Fields']))
            unique.setdefault(alias_key, []).append(r)
        print(chip, 'unique', len(unique), flush=True)
        for index, instances in enumerate(unique.values()):
            register = instances[0]
            candidates = candidate_sections(register, sections, pages)
            if not candidates: candidates = virtual_sections(register, sections, pages)
            if not candidates:
                audit.extend(dict(Chip=chip, Name=r['Name'], Status='unmatched') for r in instances)
                continue
            candidates.sort(key=lambda x: (-x[0], x[1]['Page'], x[1]['Offset']))
            h = candidates[0][1]
            body = body_for(h, pages)
            if not body.strip(): continue
            key = code + '-' + hashlib.sha256((str(h['Page']) + ':' + str(h['Offset']) + ':' + register['Name'].split('.')[-1]).encode()).hexdigest()[:12]
            intro = re.split(r'\n(?:Figure|Table)\s+\d', body, maxsplit=1)[0]
            fields = [english_field(f, body) for f in register['Fields']]
            pictures = [] if args.no_render else render_pages(pdf, h, code, key, image_root)
            documents[key] = dict(Name=register['Name'].split('.')[-1], Introduction=intro, Fields=fields, Pages=pictures,
                                  SourceDocument=code, SourceSection=h['Number'], FirstPage=h['Page'], LastPage=h['EndPage'])
            for instance in instances:
                mapping[chip + '|' + instance['Name']] = key
                audit.append(dict(Chip=chip, Name=instance['Name'], Status='matched', Page=h['Page'], LastPage=h['EndPage'], Title=h['Title'], Kind=h['Kind']))
            if index % 40 == 0:
                print(chip, index + 1, '/', len(unique), 'mapped', len(mapping), flush=True)
                save(ROOT / 'Data/manual-content.json', dict(Version='2.0.0', Documents=documents, RegisterDocuments=mapping))
        pdf.close()
        save(ROOT / 'Data/manual-content.json', dict(Version='2.0.0', Documents=documents, RegisterDocuments=mapping))
        save(ROOT / 'docs/manual-audit-coverage.json', audit)
    print('DONE', len(documents), 'documents;', len(mapping), '/', len(registers), 'instances', flush=True)


if __name__ == '__main__':
    main()
