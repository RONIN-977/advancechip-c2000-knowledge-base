"""Execute the real portable BOOT protocol/MD5 C code in ARM emulation.
Flash/register hardware is stubbed; this is not an AVP335 peripheral or board test.
Requires: pip install unicorn==2.1.4 pyelftools==0.32
"""
from pathlib import Path
import argparse, hashlib, json, struct, subprocess, sys
parser=argparse.ArgumentParser()
parser.add_argument('--deps', type=Path)
parser.add_argument('--serve', action='store_true', help='Exchange encoded frames with the C# integration test over stdin/stdout')
parser.add_argument('--compiler', default=r'C:\ti\ccs1271\ccs\tools\compiler\ti-cgt-armllvm_3.2.2.LTS')
args=parser.parse_args()
if args.deps: sys.path.insert(0,str(args.deps.resolve()))
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
from elftools.elf.elffile import ELFFile
root=Path(__file__).resolve().parent
out=root/'build'; out.mkdir(exist_ok=True)
cl=Path(args.compiler)/'bin/tiarmclang.exe'
# Compile the actual initialization function too; do not force apiReady in tests.
hardware=(root/'../firmware/AVP32F335_Bootloader/Main/HardwareSetup.c').read_bytes().decode('gbk')
start=hardware.index('void InitFlashAPI(void)')
opening=hardware.index('{',start); end=opening+1; depth=1
while depth:
    if hardware[end]=='{': depth+=1
    if hardware[end]=='}': depth-=1
    end+=1
(out/'FlashInit.c').write_text('#include "FlashInitStub.h"\n'+hardware[start:end],encoding='utf-8')
command=[str(cl),'-mcpu=cortex-m3','-mthumb','-O1','-DUPGRADE_HOST_TEST=1','-DUPGRADE_IS_BOOT=1','-I.',
         '../firmware/common/SciUpgrade.c','../firmware/common/UpgradeMd5.c','FlashStub.c','build/FlashInit.c',
         '-Wl,--entry_point=UpgradeHandleFrame','-Wl,--ram_model','-Wl,--unused_section_elimination=off','-Wl,--stack_size=4096',
         '-Wl,--map_file=build/test.map','test-memory.cmd','-o','build/test.out']
result=subprocess.run(command,cwd=root,capture_output=True)
(out/'compile.log').write_bytes(result.stdout+result.stderr)
if result.returncode: raise SystemExit((result.stdout+result.stderr).decode(errors='replace'))
with (out/'test.out').open('rb') as f:
    elf=ELFFile(f)
    segments=[(s['p_vaddr'],s.data()) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
    symbols={s.name:s['st_value'] for s in elf.get_section_by_name('.symtab').iter_symbols()}
def cells(data): return struct.pack('<'+'H'*len(data),*data)
def crc(data,poly):
    value=0
    for b in data:
        value^=b
        for _ in range(8): value=(value>>1)^ (poly if value&1 else 0)
    return value
def frame(cmd,seq,payload=b''):
    raw=bytearray([0xcc])+bytearray(struct.pack('<H',(11+len(payload))<<3))
    raw+=bytes([crc(raw,0x8c),1])+struct.pack('<HH',cmd,seq)+payload
    return raw+struct.pack('<H',crc(raw,0xa001))
class Boot:
    def __init__(self):
        self.cpu=Uc(UC_ARCH_ARM,UC_MODE_THUMB)
        self.cpu.mem_map(0x10000,0x30000); self.cpu.mem_map(0x20000000,0x400000)
        for addr,data in segments: self.cpu.mem_write(addr,data)
        self.cpu.mem_write(symbols['fakeFlash'],b'\xff'*0x70000)
        self.call('RemoteModbus_Initialize'); self.call('InitFlashAPI')
        self.receive=self.call('TestReceive'); self.reply=self.call('TestReply')
    def call(self,name,*values):
        for register,value in zip([UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2],values): self.cpu.reg_write(register,value)
        self.cpu.reg_write(UC_ARM_REG_SP,0x203ff000); self.cpu.reg_write(UC_ARM_REG_LR,0x30001)
        self.cpu.emu_start(symbols[name]|1,0x30000,count=150000000)
        assert self.cpu.reg_read(UC_ARM_REG_PC)==0x30000, 'C execution budget exceeded: '+name
        return self.cpu.reg_read(UC_ARM_REG_R0)
    def value(self,name): return struct.unpack('<I',self.cpu.mem_read(symbols[name],4))[0]
    def fault(self,name): self.cpu.mem_write(symbols[name],b'\x01\x00')
    def flag(self): return struct.unpack('<H',self.cpu.mem_read(symbols['fakeFlash']+0x6fffc,2))[0]
    def send(self,cmd,seq,payload=b''):
        raw=self.exchange(frame(cmd,seq,payload))
        assert raw[:1]==b'\xcc' and raw[4]==0x89 and struct.unpack_from('<HH',raw,5)==(cmd,seq)
        assert crc(raw[:-2],0xa001)==struct.unpack_from('<H',raw,len(raw)-2)[0]
        return raw[9:-2]
    def exchange(self,wire):
        self.cpu.mem_write(self.receive,cells(wire))
        # RxBufferIndex is the first word of TRemoteModbus.
        self.cpu.mem_write(symbols['RemoteModbus'],struct.pack('<H',len(wire)))
        self.call('UpgradeHandleFrame'); count=self.call('TestReplyLength')
        raw=bytes(struct.unpack('<'+'H'*count,self.cpu.mem_read(self.reply,count*2)))
        return raw
    def digest(self,data):
        self.cpu.mem_write(0x20100000,cells(data))
        self.call('TestDigest',0x20100000,len(data),0x20200000)
        return bytes(struct.unpack('<16H',self.cpu.mem_read(0x20200000,32)))
if args.serve:
    boot=Boot()
    print('READY',flush=True)
    for line in sys.stdin:
        line=line.strip()
        if line=='stats':
            print(json.dumps(dict(erase=boot.value('eraseCount'), program=boot.value('programCount'),
                                  commit=boot.value('commitCount'), flag=boot.flag(), action=boot.call('TestAction'))),flush=True)
        else:
            print(boot.exchange(bytes.fromhex(line)).hex(),flush=True)
    sys.exit(0)
passed=0
def test(name,body):
    global passed
    body(); passed+=1; print('PASS C firmware '+name,flush=True)

def flash_init():
    b=Boot()
    assert b.call('TestApiReady')==1, 'CsmUnlock success (1) must enable Flash API'
    assert b.send(0x70,1,struct.pack('<I',4))[0]==0 and b.value('eraseCount')==2
    b=Boot(); b.cpu.mem_write(symbols['TestCsmResult'],b'\x00\x00'); b.call('InitFlashAPI')
    assert b.call('TestApiReady')==0
    assert b.send(0x70,1,struct.pack('<I',4))[0]==1 and b.value('eraseCount')==0

test('real InitFlashAPI accepts CSM success=1; locked CSM refuses erase',flash_init)
def md5_vectors():
    b=Boot()
    for value in [b'',b'a',b'abc',b'message digest',b'123456789'*7,bytes(range(256))*3]:
        assert b.digest(value)==hashlib.md5(value).digest()
test('MD5 standard vectors and multi-block input',md5_vectors)
def versions():
    b=Boot(); info=b.send(1,1); assert len(info)==26 and struct.unpack_from('<I',info,18)[0]==201
    assert b.send(0x71,2)[0]==1 and b.call('TestHold')==0
test('version 201, original CRC frames and BOOT hold',versions)
def full(size):
    b=Boot(); data=bytes((i*31)&255 for i in range(size)); payload=struct.pack('<I',size)
    assert b.send(0x70,1,payload)==bytes([0,128,0]); assert b.value('firstErase')==2
    assert b.send(0x70,1,payload)==bytes([0,128,0]) and b.value('eraseCount')==2
    seq=2
    for offset in range(0,size,128):
        chunk=data[offset:offset+128]; p=struct.pack('<IH',offset//128,len(chunk))+chunk
        reply=b.send(0x73,seq,p); assert reply==b'\0'+struct.pack('<I',offset//128)
        before=b.value('programCount'); assert b.send(0x73,seq,p)==reply and b.value('programCount')==before
        seq+=1
    assert b.flag()==0xffff
    assert bytes(b.cpu.mem_read(symbols['fakeFlash'],size))==data
    digest=hashlib.md5(data).digest(); assert b.send(0x74,seq,digest)==b'\0'
    assert b.flag()==0xaa and b.value('commitCount')==1
    assert b.send(0x74,seq,digest)==b'\0' and b.value('commitCount')==1
test('short final packet, duplicate begin/data/finish',lambda:full(4100))
test('448 KB minus 4 boundary, fixed 128-byte stride, MD5 commit',lambda:full(458748))
def errors():
    for fault in ['failErase','failProgram','failVerify']:
        b=Boot(); b.fault(fault); result=b.send(0x70,1,struct.pack('<I',128))
        if fault=='failErase': assert result[0]!=0
        else: assert b.send(0x73,2,struct.pack('<IH',0,128)+bytes(128))[0]!=0
        assert b.value('commitCount')==0 and b.call('TestHold')==0
    b=Boot()
    for size in [0,3,458750,0xffffffff]: assert b.send(0x70,1,struct.pack('<I',size))[0]!=0
    assert b.value('eraseCount')==0
test('erase/program/verify errors and 32-bit bounds',errors)
def invalid_finish():
    b=Boot(); b.send(0x70,1,struct.pack('<I',132))
    assert b.send(0x74,2,bytes(16))[0]!=0
    assert b.send(0x73,3,struct.pack('<IH',1,128)+bytes(128))[0]!=0
    assert b.send(0x73,4,struct.pack('<IH',0,128)+bytes(128))[0]==0
    assert b.send(0x73,5,struct.pack('<IH',1,4)+bytes(4))[0]==0
    assert b.send(0x74,6,bytes(16))[0]==3 and b.flag()==0xffff
    assert b.send(0x75,7)[0]!=0
    assert b.send(0x70,8,struct.pack('<I',4))[0]==0
test('order/length checks, missing data, MD5 failure and restart',invalid_finish)
def invalidate_first():
    b=Boot(); b.cpu.mem_write(symbols['fakeFlash']+0x6fffc,b'\xaa\x00'); b.fault('failErase')
    assert b.send(0x70,1,struct.pack('<I',4))[0]!=0
    assert b.flag()==0 and b.value('firstErase')==2 and b.value('commitCount')==0
test('old valid marker invalidated before a failed erase',invalidate_first)
print(f'All {passed} firmware C tests passed.')
