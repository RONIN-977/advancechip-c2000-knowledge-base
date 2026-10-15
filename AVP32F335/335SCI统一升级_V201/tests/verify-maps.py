from pathlib import Path
import argparse, hashlib, json, re, struct, subprocess
parser=argparse.ArgumentParser()
parser.add_argument('--compiler',type=Path,default=Path(r'C:\ti\ti-cgt-c2000_20.2.0.LTS'))
args=parser.parse_args()
root=Path(__file__).resolve().parents[1]
expected='2FD4073BE476E48247CD9CB29D0D9FF11A20777D08E21202253156FCDC864831'
assert hashlib.sha256((root/'firmware/AVP32F335_Bootloader/Library/lib/Flash28335_API_V210.lib').read_bytes()).hexdigest().upper()==expected
report=[]
for crystal in ['30MHz','33.3MHz']:
    for kind in ['BOOT','APP']:
        stem=root/'build'/crystal/kind/f'AVP32F335_{kind}_{crystal}'
        text=Path(str(stem)+'.map').read_text(encoding='ascii')
        # Preserve dots in the crystal label when appending the extension.
        binary=Path(str(stem)+'.out').read_bytes()
        address=0x33fff6 if kind=='BOOT' else 0x300000
        assert f'ENTRY POINT SYMBOL: "code_start"  address: {address:08x}' in text
        assert struct.unpack_from('<I',binary,38)[0]==address
        def symbol(name):
            match=re.search(r'^0\s+([0-9a-f]{8})\s+'+re.escape(name)+r'\s*$',text,re.M)
            assert match,name
            return int(match[1],16)
        ram=symbol('_RamfuncsRunStart'); assert 0x8000<=ram<0xa000
        if kind=='BOOT':
            api=symbol('_Flash28_API_RunStart'); assert api==0x8000
            for name in ['_Flash28335_Erase','_Flash28335_Program','_Flash28335_Verify','_UpgradeWatchdog']:
                assert 0x8000<=symbol(name)<0x9000
            dis=subprocess.run([str(args.compiler/'bin/dis2000.exe'),'-q','-e',stem.name+'.out'],
                               cwd=stem.parent,capture_output=True,check=True).stdout.decode('ascii',errors='replace')
            callback=dis.split('_UpgradeWatchdog:',1)[1].split('LRETR',1)[0]
            save=re.search(r'PUSH\s+ST1',callback); restore=re.search(r'POP\s+ST1',callback)
            assert save and restore and save.start()<callback.index('EALLOW')<restore.start()
            assert 'EDIS' not in callback, 'Flash callback must preserve caller EALLOW/interrupt state'
        else:
            assert re.search(r'BOOTFLAG\s+0\s+00337ffe\s+00000001',text)
            assert re.search(r'FLASHAPP\s+00300002\s+00037ffc',text)
        report.append(dict(crystal=crystal,kind=kind,entry=f'0x{address:06X}',ramfuncs=f'0x{ram:06X}',outSha256=hashlib.sha256(binary).hexdigest().upper()))
destination=root/'verification'; destination.mkdir(exist_ok=True)
(destination/'map-verification.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
print('PASS four COFF/MAP entry points, APP/metadata bounds, Flash API and watchdog RAM addresses, B-library hash; compiled callback preserves ST1')
