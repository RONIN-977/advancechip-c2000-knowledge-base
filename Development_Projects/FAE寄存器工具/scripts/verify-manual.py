"""Independent structural and representative-source checks for the English pack.

This verifier never imports the manual extraction script and does not change Data.
It distinguishes usable images from completeness of the underlying source sections.
"""
import argparse
import collections
import hashlib
import json
import pathlib
import re
import sys

from PIL import Image, ImageStat

ROOT = pathlib.Path(__file__).resolve().parents[1]
SOURCES = ROOT / 'docs/sources'
CODES = {'035': 'sprui10', '335': 'sprui07', '0039': 'spruiw9', '0049': 'sprui33'}
REPRESENTATIVES = {
    'ADC': ('ADCCTL1', 'ADCTRL1'), 'CAN': ('CANBTC', 'CAN_BTR'),
    'GPIO': ('GPAMUX1',), 'I2C': ('I2CMDR',), 'PWM': ('TBCTL',),
    'SCI': ('SCICCR',), 'SPI': ('SPICCR',), '中断': ('PIECTRL',),
    '定时器': ('TCR',), '时钟': ('PLLCR', 'SYSPLLMULT'), '看门狗': ('WDCR',),
}
# Physical PDF pages independently located in the retained original-source text.
SOURCE_ANCHORS = {
    '035': {'ADCCTL1': (531, 531), 'CANBTC': (876, 876), 'GPAMUX1': (114, 114),
            'I2CMDR': (829, 829), 'TBCTL': (316, 316), 'SCICCR': (778, 778),
            'SPICCR': (742, 742), 'PIECTRL': (166, 166), 'TCR': (93, 94),
            'PLLCR': (69, 69), 'WDCR': (89, 89)},
    '335': {'ADCTRL1': (478, 478), 'CANBTC': (812, 813), 'GPAMUX1': (104, 105),
            'I2CMDR': (646, 646), 'TBCTL': (296, 297), 'SCICCR': (597, 597),
            'SPICCR': (564, 564), 'PIECTRL': (154, 154), 'TCR': (84, 84),
            'PLLCR': (72, 72), 'WDCR': (80, 81)},
    '0039': {'ADCCTL1': (1916, 1917), 'CAN_BTR': (2864, 2865), 'GPAMUX1': (1119, 1120),
             'I2CMDR': (2755, 2755), 'TBCTL': (2348, 2349), 'SCICCR': (2692, 2692),
             'SPICCR': (2649, 2649), 'PIECTRL': (439, 439), 'TCR': (257, 257),
             'SYSPLLMULT': (194, 194), 'WDCR': (519, 519)},
    '0049': {'ADCCTL1': (1601, 1601), 'CAN_BTR': (2519, 2520), 'GPAMUX1': (957, 958),
             'I2CMDR': (2408, 2408), 'TBCTL': (2008, 2009), 'SCICCR': (2348, 2348),
             'SPICCR': (2307, 2307), 'PIECTRL': (163, 163), 'TCR': (157, 157),
             'SYSPLLMULT': (281, 281), 'WDCR': (217, 217)},
}
FIELD_ANCHORS = {
    'ADCCTL1': ['INTPULSEPOS'], 'ADCTRL1': ['ACQ_PS', 'SEQ_CASC'],
    'CANBTC': ['BRPREG', 'TSEG2REG', 'SJWREG'], 'CAN_BTR': ['BRP', 'TSEG1', 'TSEG2', 'SJW'],
    'GPAMUX1': ['GPIO0', 'GPIO15'], 'I2CMDR': ['IRS', 'MST', 'STP'],
    'TBCTL': ['CTRMODE', 'HSPCLKDIV', 'CLKDIV'], 'SCICCR': ['STOPBITS', 'PARITYENA'],
    'SPICCR': ['SPICHAR', 'SPISWRESET'], 'PIECTRL': ['ENPIE'],
    'TCR': ['TIF', 'TIE', 'TSS'], 'PLLCR': ['DIV'], 'SYSPLLMULT': ['IMULT'],
    'WDCR': ['WDDIS', 'WDCHK', 'WDPS'],
}


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def norm(text):
    return re.sub(r'[^A-Z0-9]+', '', text.upper())


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', default=str(ROOT / 'artifacts/manual-verification.json'))
    parser.add_argument('--allow-incomplete', action='store_true')
    args = parser.parse_args()
    data = ROOT / 'Data/manual-content.json'
    if not data.exists():
        raise SystemExit('English manual pack not generated yet: ' + str(data))
    pack = json.loads(data.read_text(encoding='utf-8-sig'))
    registers = json.loads((ROOT / 'Data/registers.json').read_text(encoding='utf-8-sig'))['Registers']
    documents, mappings = pack['Documents'], pack['RegisterDocuments']
    errors, gaps = [], []
    coverage = collections.Counter()
    image_cache = {}
    expected_keys = {r['ChipId'] + '|' + r['Name'] for r in registers}
    missing = sorted(expected_keys - set(mappings))
    extra = sorted(set(mappings) - expected_keys)
    if missing:
        gaps.append({'Kind': 'MissingRegisterMappings', 'Count': len(missing), 'Items': missing})
    if extra:
        errors.append({'Kind': 'UnknownRegisterMappings', 'Items': extra})
    referenced = set()
    for register in registers:
        key = register['ChipId'] + '|' + register['Name']
        document_id = mappings.get(key)
        if document_id is None:
            continue
        if document_id not in documents:
            errors.append({'Kind': 'BrokenDocumentReference', 'Register': key, 'Document': document_id})
            continue
        doc = documents[document_id]
        referenced.add(document_id)
        coverage[(register['ChipId'], register['Peripheral'])] += 1
        if not doc.get('Pages'):
            gaps.append({'Kind': 'MissingRegisterImages', 'Register': key, 'Document': document_id})
        occupied = set()
        for field in doc.get('Fields', []):
            bits = set(range(field['Lsb'], field['Lsb'] + field['Width']))
            if not bits or min(bits) < 0 or max(bits) >= register['Width']:
                errors.append({'Kind': 'FieldOutsideRegister', 'Register': key, 'Field': field['Name'],
                               'Lsb': field['Lsb'], 'Width': field['Width'], 'RegisterWidth': register['Width']})
            if occupied & bits:
                errors.append({'Kind': 'OverlappingFields', 'Register': key, 'Field': field['Name']})
            occupied |= bits
            if not (field.get('Description', '').strip() or field.get('Values', '').strip()):
                gaps.append({'Kind': 'EmptyFieldExplanation', 'Register': key, 'Field': field['Name']})
        for old_field in register['Fields']:
            if re.match(r'rsvd|reserved', old_field['Name'], re.I):
                continue
            required_bits = set(range(old_field['Lsb'], old_field['Lsb'] + old_field['Width']))
            if not required_bits <= occupied:
                gaps.append({'Kind': 'UnexplainedNonreservedBits', 'Register': key, 'Field': old_field['Name'],
                             'Bits': sorted(required_bits - occupied)})
    for document_id in referenced:
        doc = documents[document_id]
        if not doc.get('SourceDocument') or not doc.get('SourceSection'):
            errors.append({'Kind': 'MissingProvenance', 'Document': document_id})
        if doc.get('FirstPage', 0) < 1 or doc.get('LastPage', 0) < doc.get('FirstPage', 0):
            errors.append({'Kind': 'InvalidPageRange', 'Document': document_id})
        if doc.get('LastPage', 0) - doc.get('FirstPage', 0) > 15:
            gaps.append({'Kind': 'OversizedRegisterSectionNeedsReview', 'Document': document_id,
                         'FirstPage': doc.get('FirstPage'), 'LastPage': doc.get('LastPage')})
        texts = [doc.get('Introduction', '')]
        texts += [field.get(k, '') for field in doc.get('Fields', [])
                  for k in ('Description', 'Values', 'Access', 'Reset')]
        if any(re.search('[\u4e00-\u9fff]', text) for text in texts):
            errors.append({'Kind': 'ChineseInEnglishBody', 'Document': document_id})
        if any(marker in '\n'.join(texts) for marker in ('TRM术语译注', '待核对', '未逐句', '尚未完成')):
            errors.append({'Kind': 'OldSoftwareAnnotationInBody', 'Document': document_id})
        for page in doc.get('Pages', []):
            relative = page['ImagePath']
            path = (ROOT / relative).resolve()
            if not path.is_relative_to(ROOT.resolve()):
                errors.append({'Kind': 'ImagePathOutsidePack', 'Path': relative})
                continue
            if relative not in image_cache:
                if not path.is_file():
                    image_cache[relative] = {'Present': False}
                else:
                    try:
                        with Image.open(path) as image:
                            gray = image.convert('L')
                            image_cache[relative] = {'Present': True, 'Width': image.width, 'Height': image.height,
                                                     'Stddev': ImageStat.Stat(gray).stddev[0], 'Sha256': sha(path)}
                    except Exception as exc:
                        image_cache[relative] = {'Present': False, 'Error': str(exc)}
            info = image_cache[relative]
            if not info['Present']:
                errors.append({'Kind': 'MissingOrUnreadableImage', 'Path': relative})
            elif info['Width'] < 100 or info['Height'] < 25 or info['Stddev'] < 0.5:
                errors.append({'Kind': 'BlankOrTinyImage', 'Path': relative, 'ImageInfo': info})
            if not (doc['FirstPage'] <= page['PageNumber'] <= doc['LastPage']):
                errors.append({'Kind': 'ImagePageOutsideSection', 'Document': document_id, 'Page': page['PageNumber']})

    samples = []
    for chip in CODES:
        for peripheral, names in REPRESENTATIVES.items():
            candidates = [r for r in registers if r['ChipId'] == chip and r['Peripheral'] == peripheral
                          and r['Name'].split('.')[-1] in names]
            if not candidates:
                errors.append({'Kind': 'MissingSampleRegister', 'Chip': chip, 'Peripheral': peripheral})
                continue
            register = candidates[0]
            short = register['Name'].split('.')[-1]
            key = chip + '|' + register['Name']
            document_id = mappings.get(key)
            sample = {'Register': key, 'Peripheral': peripheral, 'Document': document_id, 'Checks': []}
            if document_id not in documents:
                sample['Checks'].append('NO_DOCUMENT')
                samples.append(sample)
                continue
            doc = documents[document_id]
            lo, hi = SOURCE_ANCHORS[chip][short]
            if not (doc['FirstPage'] <= lo and doc['LastPage'] >= hi):
                errors.append({'Kind': 'RepresentativeSourceRangeMismatch', 'Register': key,
                               'Actual': [doc['FirstPage'], doc['LastPage']], 'ExpectedContains': [lo, hi]})
                sample['Checks'].append('SOURCE_RANGE_MISMATCH')
            names_in_doc = {norm(field['Name']) for field in doc.get('Fields', [])}
            for required in FIELD_ANCHORS[short]:
                if norm(required) not in names_in_doc:
                    gaps.append({'Kind': 'RepresentativeFieldMissing', 'Register': key, 'Field': required})
                    sample['Checks'].append('MISSING_FIELD:' + required)
            # A register outline/short header comment is not enough for interactive field help.
            for field in doc.get('Fields', []):
                if norm(field['Name']) not in {norm(n) for n in FIELD_ANCHORS[short]}:
                    continue
                text = field.get('Description', '') + '\n' + field.get('Values', '')
                if len(re.sub(r'\s+', ' ', text).strip()) < 30:
                    gaps.append({'Kind': 'RepresentativeFieldTooShortForFullManualText',
                                 'Register': key, 'Field': field['Name'], 'Text': text})
                    sample['Checks'].append('SHORT_FIELD:' + field['Name'])
                if field['Name'] in ('TIF', 'TIE', 'TSS', 'INTPULSEPOS', 'PARITYENA', 'SPISWRESET'):
                    if not (re.search(r'\b0h?\b', text, re.I) and re.search(r'\b1h?\b', text, re.I)):
                        gaps.append({'Kind': 'RepresentativeZeroOneDefinitionsMissing',
                                     'Register': key, 'Field': field['Name']})
                        sample['Checks'].append('MISSING_0_1:' + field['Name'])
            sample.update(FirstPage=doc['FirstPage'], LastPage=doc['LastPage'],
                          Fields=len(doc.get('Fields', [])), Images=len(doc.get('Pages', [])))
            if not sample['Checks']:
                sample['Checks'].append('STRUCTURE_AND_SOURCE_ANCHORS_PASS')
            samples.append(sample)

    report = {'Status': 'PASS' if not errors and not gaps else 'INCOMPLETE', 'PackSha256': sha(data),
              'ExpectedRegisters': len(registers), 'MappedRegisters': len(expected_keys & set(mappings)),
              'UniqueReferencedDocuments': len(referenced), 'ImagesChecked': len(image_cache),
              'Coverage': {f'{chip}/{module}': n for (chip, module), n in sorted(coverage.items())},
              'RepresentativeSamples': samples, 'Errors': errors, 'Gaps': gaps,
              'Boundary': 'Checks images for readability and source anchors; does not equate images with complete, correct semantic extraction.'}
    output = pathlib.Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding='utf-8')
    print(f'{report["Status"]}: {report["MappedRegisters"]}/{len(registers)} mappings; '
          f'{len(referenced)} documents; {len(image_cache)} images; '
          f'{len(errors)} errors; {len(gaps)} gaps; {len(samples)} representative samples')
    print(output)
    if (errors or gaps) and not args.allow_incomplete:
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
