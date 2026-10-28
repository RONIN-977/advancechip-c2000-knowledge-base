from pathlib import Path
import subprocess, hashlib, json, os, shutil, sys
here=Path(__file__).resolve().parent
root=here.parents[2]
delivery=Path(r'C:\Users\17879\Desktop\CHIP\BOOTLOADER\AVP32F0039Boot+APP_CAN_10M_KB\EDrive_CAN_Upgrade_KB')
app=delivery/'APP_KB';boot=delivery/'BOOT_KB'
host=Path(r'C:\Users\17879\Desktop\CHIP\BOOTLOADER\上位机源码\上位机源码\host\Updater.Core')
firmware=app/'Debug/EDrive_CAN_Application_KB.out'
hashes={}
def copy(src,name=None):
    target=here/(name or src.name);target.parent.mkdir(parents=True,exist_ok=True)
    data=src.read_bytes();target.write_bytes(data)
    hashes[str(src)]={'sha256':hashlib.sha256(data).hexdigest(),'model_file':str(target.relative_to(here))}
    return target
for name in ('kb_can_upgrade.c','kb_can_upgrade.h','kb_can_upgrade_config.h','kb_customer_shutdown.c','app_partition.h','boot_core.h'):
    copy(app/'CAN_Upgrade'/name)
copy(boot/'src/boot_core.c')
assert (boot/'src/boot_core.c').read_bytes()==(app/'CAN_Upgrade/boot_core.c').read_bytes()
assert (boot/'include/app_partition.h').read_bytes()==(app/'CAN_Upgrade/app_partition.h').read_bytes()
config=here/'kb_can_upgrade_config.h'
config.write_bytes(config.read_bytes().replace(b'#define KB_UPGRADE_REQUEST_ADDR 0x0003F8UL',b'#define KB_UPGRADE_REQUEST_ADDR ((uintptr_t)request_words)'))
copy(boot/'include/board_config.h');copy(boot/'include/kb_can_board.h');copy(boot/'include/boot_io.h')
stub=(here.parent/'upgrade-model/uC_Inc.h').read_bytes()
(here/'uC_Inc.h').write_bytes(stub)
(here/'driverlib.h').write_text('#include "uC_Inc.h"\nvoid CAN_sendMessage_updateDLC(uint32_t,uint32_t,uint16_t,const uint16_t*);\n')
io=(boot/'src/boot_io.c').read_bytes()
a=io.index(b'static uint16_t can_wait_tx(');b=io.index('/* Timer0 提供毫秒时基'.encode(),a)
extracted=io[a:b]
(here/'real_boot_send.c').write_bytes(b'#include "driverlib.h"\n#include "board_config.h"\n#include "boot_io.h"\nstatic uint16_t can_transfer;\n'+extracted)
hashes[str(boot/'src/boot_io.c')]={'sha256':hashlib.sha256(io).hexdigest(),'extracted_span_sha256':hashlib.sha256(extracted).hexdigest()}
host_comparison={}
for src in host.glob('*.cs'):
    copy(src,'host-core/'+src.name)
    prior=root/'tmp/kb0039-simulation/host-core'/src.name
    host_comparison[src.name]={'same_as_previous_snapshot':prior.exists() and prior.read_bytes()==src.read_bytes(),'sha256':hashlib.sha256(src.read_bytes()).hexdigest()}
(here/'source_hashes.json').write_text(json.dumps(hashes,indent=2),encoding='utf8')
(here/'host_source_comparison.json').write_text(json.dumps({'source_directory':str(host),'files':host_comparison},indent=2),encoding='utf8')
env=os.environ.copy();env['ZIG_LOCAL_CACHE_DIR']=str(here/'zig-cache');env['ZIG_GLOBAL_CACHE_DIR']=str(here/'zig-global-cache')
env['DOTNET_CLI_HOME']=str(here/'dotnet-home');env['DOTNET_CLI_TELEMETRY_OPTOUT']='1';env['DOTNET_SKIP_FIRST_TIME_EXPERIENCE']='1'
env['NUGET_PACKAGES']=str(here/'nuget-packages');env['TMP']=env['TEMP']=str(here/'runtime-temp');(here/'runtime-temp').mkdir(exist_ok=True)
zig=root/'tmp/kb0039-simulation/native/compiler/ziglang/zig.exe'
cmd=[str(zig),'cc','-shared','-O1','-Wall','-Wextra','-Werror','-finstrument-functions','-I.','native_model.c','kb_can_upgrade.c','kb_customer_shutdown.c','boot_core.c','real_boot_send.c','-o','integration.dll']
q=subprocess.run(cmd,cwd=here,env=env,capture_output=True);(here/'native-build.log').write_bytes(q.stdout+q.stderr);q.check_returncode()
dotnet=root/'tmp/dotnet-sdk/dotnet.exe'
q=subprocess.run([str(dotnet),'build','HostIntegration.csproj','--configfile','NuGet.Config','-o','bin'],cwd=here,env=env,capture_output=True)
(here/'host-build.log').write_bytes(q.stdout+q.stderr);q.check_returncode()
hashes[str(firmware)]={'sha256':hashlib.sha256(firmware.read_bytes()).hexdigest()}
(here/'source_hashes.json').write_text(json.dumps(hashes,indent=2),encoding='utf8')
hex=Path(r'C:\ti\ccs1271\ccs\tools\compiler\ti-cgt-c2000_22.6.1.LTS\bin\hex2000.exe')
results=[]
for scenario in ['full_update','lost_ack','cancel_recover','wrong_partition','app_ack_timeout','flash_fault_1','flash_fault_2','flash_fault_3','flash_fault_4']:
    resultfile=here/(scenario+'.json')
    cmd=[str(dotnet),str(here/'bin/HostIntegration.dll'),str(here/'integration.dll'),str(firmware),str(hex),scenario,str(resultfile)]
    q=subprocess.run(cmd,cwd=here,env=env,capture_output=True)
    (here/(scenario+'.log')).write_bytes(q.stdout+q.stderr)
    results.append({'case':scenario,'pass':q.returncode==0,'exit':q.returncode,'result':json.loads(resultfile.read_text()) if resultfile.exists() else None})
    print(scenario,q.returncode,flush=True)
(here/'results.json').write_text(json.dumps({'passed':sum(x['pass'] for x in results),'total':len(results),'cases':results},indent=2),encoding='utf8')
for name,entry in hashes.items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==entry['sha256'],name
assert all(x['pass'] for x in results)
