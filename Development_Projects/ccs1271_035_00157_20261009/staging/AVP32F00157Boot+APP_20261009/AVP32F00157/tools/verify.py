"""Check linked C28x WORD addresses, ECC boundaries and portable CCS links."""
from pathlib import Path
import re,sys,xml.etree.ElementTree as ET,hashlib
r=Path(__file__).resolve().parents[1]
app_start=int(re.search(r'^#define\s+APP_START_ADDRESS\s+(0x[0-9A-Fa-f]+)',
                       (r/'Common/app_partition.h').read_text(),re.M)[1],16)
for kind in ('Bootloader','Application'):
    for name in ('.project','.cproject','.ccsproject'): ET.parse(r/kind/name)
    links=ET.parse(r/kind/'.project').findall('.//locationURI')
    for e in links:
        assert e.text.startswith('PARENT-1-PROJECT_LOC/')
        assert (r/e.text.split('/',1)[1]).is_file(),e.text
    assert len(links)==1 and links[0].text=='PARENT-1-PROJECT_LOC/Common/app_partition.h'
    for name in ('boot_io.c','sci_port.c','boot_core.c','update_service.c'):
        assert (r/kind/'Common'/name).is_file()
    path=r/'output'/('AVP32F00157_'+kind+'.map')
    if kind=='Bootloader' and len(sys.argv)>1:path=Path(sys.argv[1])
    s=path.read_text()
    def symbol(name):
        m=re.search(r'^\s*(?:0|1|abs)\s+([0-9a-fA-F]{8})\s+'+re.escape(name)+r'\s*$',s,re.M)
        assert m,name
        return int(m[1],16)
    lo,hi=(0x80000,app_start-0x400) if kind=='Bootloader' else (app_start,0x9fff0)
    assert symbol('code_start')==lo
    n=symbol('RamfuncsLoadSize'); run=symbol('RamfuncsRunStart'); load=symbol('RamfuncsLoadStart')
    assert 0<n and 0x8000<=run and run+n<=0xa000 and lo<=load and load+n<=hi
    if kind=='Bootloader':
        for name in ['Fapi_initializeAPI','Fapi_issueProgrammingCommand','Fapi_issueAsyncCommandWithAddress',
                     'Fapi_checkFsmForReady','Fapi_getFsmStatus','Fapi_setupBankSectorEnable','Fapi_doVerify','Fapi_doBlankCheck','boot_flash_init']:
            assert 0x8000<=symbol(name)<0xa000,name
        assert 'FAPI_F280015x_EABI_v2.00.10.lib' in s
        assert 'BootROMSymbols' not in s and 'Flash2803x_API' not in s
    cmd=(r/kind/(kind.lower()+'.cmd')).read_text()
    assert 'origin = 0x0003F8, length = 0x000004' in cmd
    assert 'origin = 0x000128, length = 0x0002D0' in cmd
    assert 'length = 0x0003F8' in cmd and 'length = 0x001FF8' in cmd
print('PASS CCS sources, entry points, RAM API/copy ranges, reserved RAM and Flash boundaries')
