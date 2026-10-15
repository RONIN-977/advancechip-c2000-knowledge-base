"""Run real RX/TX ISR and main-loop C against the documented non-FIFO TX model.
This models interrupt triggering; it does not emulate AVP335 peripherals or a board.
"""
from pathlib import Path
import argparse, hashlib, json, struct, subprocess, sys

parser = argparse.ArgumentParser()
parser.add_argument('--deps', type=Path)
parser.add_argument('--compiler', default=r'C:\ti\ccs1271\ccs\tools\compiler\ti-cgt-armllvm_3.2.2.LTS')
args = parser.parse_args()
if args.deps: sys.path.insert(0, str(args.deps.resolve()))
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_MEM_WRITE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC
from elftools.elf.elffile import ELFFile

root = Path(__file__).resolve().parent
build = root/'build/sci-runtime'
build.mkdir(parents=True, exist_ok=True)
source = (root/'../firmware/common/SciUpgrade.c').read_text(encoding='utf-8')

def function(name):
    start = source.index(name)
    # Include the return type and interrupt declaration on this line.
    start = source.rfind('\n', 0, start)+1
    opening = source.index('{', start)
    depth = 1; end = opening+1
    while depth:
        if source[end] == '{': depth += 1
        if source[end] == '}': depth -= 1
        end += 1
    return source[start:end]

runtime = '#include "SciRegisterStub.h"\n'+source.split('#ifndef UPGRADE_HOST_TEST\ninterrupt void SCI_RXD_isr', 1)[0]
runtime += '\n'+function('SCI_RXD_isr(void)')+'\n'+function('SCI_TXD_isr(void)')
runtime += '\n'+function('UpgradeJumpApp(void)')+'\n'+function('UpgradePendingAction(void)')+'\n'+function('RemoteModbus_MsgProcess(void)')
correct = 'SciaRegs.SCICTL2.bit.TXINTENA=1;\n            SciaRegs.SCITXBUF=RemoteModbus.SendBuf[0];'
assert correct in runtime
legacy = runtime.replace(correct, 'SciaRegs.SCITXBUF=RemoteModbus.SendBuf[0];\n            SciaRegs.SCICTL2.bit.TXINTENA=1;', 1)

def crc(data, poly):
    c = 0
    for b in data:
        c ^= b
        for _ in range(8): c = (c >> 1) ^ (poly if c & 1 else 0)
    return c

def request(command, sequence, payload=b''):
    data = bytearray([0xcc])+bytearray(struct.pack('<H',(11+len(payload))<<3))
    data += bytes([crc(data, 0x8c), 1])+struct.pack('<HH', command, sequence)
    data += payload
    return data+struct.pack('<H', crc(data, 0xa001))

class Sci:
    def __init__(self, code, label):
        path = build/(label+'.c'); path.write_text(code, encoding='utf-8')
        command = [str(Path(args.compiler)/'bin/tiarmclang.exe'), '-mcpu=cortex-m3', '-mthumb', '-O1', '-DUPGRADE_HOST_TEST=1',
                   '-DUPGRADE_IS_BOOT='+('0' if label=='app' else '1'),
                   '-I.', '-I../firmware/common', str(path.relative_to(root)),
                   '../firmware/common/UpgradeMd5.c', 'FlashStub.c',
                   '-Wl,--entry_point=RemoteModbus_MsgProcess', '-Wl,--ram_model',
                   '-Wl,--unused_section_elimination=off', '-Wl,--stack_size=4096',
                   'test-memory.cmd', '-o', str((build/(label+'.out')).relative_to(root))]
        r = subprocess.run(command, cwd=root, capture_output=True)
        (build/(label+'-compile.log')).write_bytes(r.stdout+r.stderr)
        assert r.returncode == 0, (r.stdout+r.stderr).decode(errors='replace')
        with (build/(label+'.out')).open('rb') as f:
            elf = ELFFile(f)
            self.symbols = {s.name: s['st_value'] for s in elf.get_section_by_name('.symtab').iter_symbols()}
            segments = [(s['p_vaddr'], s.data()) for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        self.cpu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
        self.cpu.mem_map(0x10000, 0x30000); self.cpu.mem_map(0x20000000, 0x400000)
        for address, data in segments: self.cpu.mem_write(address, data)
        self.wire = []; self.enabled = False; self.pending = False
        self.sci = self.symbols['SciaRegs']
        self.cpu.hook_add(UC_HOOK_MEM_WRITE, self.write, begin=self.sci, end=self.sci+13)
        self.call('RemoteModbus_Initialize')
        self.cpu.mem_write(self.sci+6, struct.pack('<H', 0xc2))

    def write(self, cpu, access, address, size, value, user):
        if address == self.sci+6: self.enabled = bool(value & 1)
        if address == self.sci+8:
            self.wire.append(value & 255)
            # Enabling TXINTENA AFTER this write does not generate the first IRQ.
            self.pending = self.enabled

    def call(self, name):
        self.cpu.reg_write(UC_ARM_REG_SP, 0x203ff000)
        self.cpu.reg_write(UC_ARM_REG_LR, 0x30001)
        self.cpu.emu_start(self.symbols[name] | 1, 0x30000, count=150000000)
        assert self.cpu.reg_read(UC_ARM_REG_PC)==0x30000, 'C execution budget exceeded: '+name
        return self.cpu.reg_read(UC_ARM_REG_R0)

    def exchange(self, data):
        self.wire.clear()
        for b in data:
            self.cpu.mem_write(self.sci, struct.pack('<H', b)); self.call('SCI_RXD_isr')
        self.call('RemoteModbus_MsgProcess')
        interrupts = 0
        while self.pending:
            self.pending = False; self.call('SCI_TXD_isr'); interrupts += 1
            assert interrupts <= 48
        self.jump_before_drain = self.cpu.mem_read(self.symbols['TestJumpAddress'], 4)
        self.cpu.mem_write(self.sci+6, struct.pack('<H', 0xc2 if not self.enabled else 0xc3))
        self.call('RemoteModbus_MsgProcess')
        return bytes(self.wire)

old = Sci(legacy, 'legacy')
assert old.exchange(request(1, 1)) == b'\xcc'
print('PASS: legacy compiled C reproduces one-byte CC response', flush=True)
fixed = Sci(runtime, 'fixed')
for command, sequence, length in [(1, 1, 37), (1, 2, 37), (1, 2, 37), (0x71, 3, 14)]:
    reply = fixed.exchange(request(command, sequence))
    assert len(reply) == length and reply[0] == 0xcc and reply[4] == 0x89
    assert struct.unpack_from('<HH', reply, 5) == (command, sequence)
    assert crc(reply[:3], 0x8c) == reply[3]
    assert crc(reply[:-2], 0xa001) == struct.unpack_from('<H', reply, len(reply)-2)[0]
    if command == 1: assert struct.unpack_from('<I', reply, 27)[0] == 201
    # Receiver is re-armed and TX interrupt disabled after each complete reply.
    assert struct.unpack('<HHH', fixed.cpu.mem_read(fixed.symbols['RemoteModbus'], 6)) == (0, 0, 0)
    assert not fixed.enabled
bad = bytearray(request(1, 4)); bad[-1] ^= 1
assert fixed.exchange(bad) == b''
print('PASS: full 37-byte version ACK, next request, duplicate ACK, state ACK and bad CRC', flush=True)
fixed.call('TestReady')
firmware=bytes(range(132))
assert fixed.exchange(request(0x70, 5, struct.pack('<I',len(firmware))))[9]==0
for index, chunk in enumerate([firmware[:128],firmware[128:]]):
    packet=request(0x73,6+index,struct.pack('<IH',index,len(chunk))+chunk)
    ack=fixed.exchange(packet)
    assert ack[9]==0 and fixed.exchange(packet)==ack
assert fixed.exchange(request(0x74,8,hashlib.md5(firmware).digest()))[9]==0
assert fixed.cpu.mem_read(fixed.symbols['fakeFlash'],len(firmware))==firmware
assert fixed.cpu.mem_read(fixed.symbols['fakeFlash']+0x6fffc,2)==b'\xaa\x00'
assert fixed.exchange(request(0x75,9))[9]==0
assert fixed.jump_before_drain==bytes(4)
assert struct.unpack('<I',fixed.cpu.mem_read(fixed.symbols['TestJumpAddress'],4))[0]==0x300000
app=Sci(runtime,'app')
reply=app.exchange(request(0x72,1))
assert reply[4]==0x81 and reply[9]==0 and app.jump_before_drain==bytes(4)
assert struct.unpack('<I',app.cpu.mem_read(app.symbols['TestJumpAddress'],4))[0]==0x33fff6
print('PASS: RX/TX full upgrade, short last packet, duplicate packet, MD5, flag; BOOT/APP jumps deferred until ACK drain',flush=True)
report = dict(legacy='one byte CC reproduced', fixed='full ACK and RX rearm passed',
              fullFlow='RX/TX upgrade and MD5 commit passed; actual pending-action logic drains ACK before intercepted APP/BOOT jumps',
              hardware='not tested', model='SPRUI07 SCICTL2 non-FIFO first TX interrupt; Flash and jump are stubs')
(root/'../verification/sci-runtime-tests.json').write_text(json.dumps(report, indent=2), encoding='utf-8')
