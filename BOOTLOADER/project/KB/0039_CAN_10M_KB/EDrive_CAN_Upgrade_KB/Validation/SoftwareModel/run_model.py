from pathlib import Path
import subprocess, hashlib, json, os, re
here=Path(__file__).resolve().parent
source=Path(r'C:\Users\17879\Desktop\CHIP\BOOTLOADER\AVP32F0039Boot+APP_CAN_10M_KB\EDrive_CAN_Upgrade_KB\APP_KB\CAN_Upgrade')
root=here.parents[2]
zig=root/'tmp/kb0039-simulation/native/compiler/ziglang/zig.exe'
hashes={}
for name in ('kb_can_upgrade.c','boot_core.c','boot_core.h','kb_can_upgrade.h','app_partition.h','kb_can_upgrade_config.h','kb_customer_shutdown.c'):
    data=(source/name).read_bytes()
    hashes[name]={'delivery_sha256':hashlib.sha256(data).hexdigest()}
    if name=='kb_can_upgrade_config.h':
        data=data.replace(b'#define KB_UPGRADE_REQUEST_ADDR 0x0003F8UL',b'#define KB_UPGRADE_REQUEST_ADDR ((uintptr_t)request_words)')
    (here/name).write_bytes(data)
    hashes[name]['model_sha256']=hashlib.sha256(data).hexdigest()
    hashes[name]['unchanged']=hashes[name]['delivery_sha256']==hashes[name]['model_sha256']
shutdown=(here/'kb_customer_shutdown.c').read_text(encoding='utf8')
shutdown=re.sub(r'/\*.*?\*/|//[^\n]*','',shutdown,flags=re.S)
body=re.search(r'void\s+Customer_ShutdownForUpgrade\s*\(\s*void\s*\)\s*\{([^{}]*)\}',shutdown)
assert body and not body.group(1).strip(),'Delivery shutdown callback must have an empty body'
assert not re.search(r'EPwm|TZCTL|TZFRC',(here/'kb_can_upgrade.c').read_text(encoding='utf8')),'Adapter must not perform hardcoded PWM shutdown'
(here/'source_hashes.json').write_text(json.dumps(hashes,indent=2),encoding='utf8')
cmd=[str(zig),'cc','-O1','-Wall','-Wextra','-Werror','-I.','model.c','kb_can_upgrade.c','boot_core.c','-o','model.exe']
env=os.environ.copy()
env['ZIG_LOCAL_CACHE_DIR']=str(here/'zig-cache')
env['ZIG_GLOBAL_CACHE_DIR']=str(here/'zig-global-cache')
build=subprocess.run(cmd,cwd=here,capture_output=True,env=env)
(here/'build.log').write_bytes(build.stdout+build.stderr)
build.check_returncode()
cases=['enter','irq_disabled','info','data','bad_crc','out_of_order','partial_timeout','partial_200_boundary','tx_timeout_recovery','occupied_rx','occupied_tx','timer_wrap','wrong_id','extended_frame','bad_dlc']
results=[]
for case in cases:
    r=subprocess.run([str(here/'model.exe'),case],cwd=here,capture_output=True)
    result={'case':case,'pass':r.returncode==0,'returncode':r.returncode,'output':(r.stdout+r.stderr).decode('utf8',errors='replace')}
    results.append(result)
    print(result['output'].strip(),flush=True)
(here/'results.json').write_text(json.dumps({'passed':sum(x['pass'] for x in results),'total':len(results),'delivery_shutdown_body_empty':True,
    'adapter_has_no_hardcoded_pwm_shutdown':True,'callback_checks':'exactly once after final TX completion, before reset INTM disable and cookie writes; never on ignored/error/timeout paths',
    'cases':results},indent=2),encoding='utf8')
(here/'execution.log').write_text(''.join(x['output'] for x in results),encoding='utf8')
for name,entry in hashes.items():
    assert hashlib.sha256((source/name).read_bytes()).hexdigest()==entry['delivery_sha256'],name
assert all(x['pass'] for x in results)
