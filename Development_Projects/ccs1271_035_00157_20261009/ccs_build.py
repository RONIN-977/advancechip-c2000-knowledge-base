from pathlib import Path
import subprocess,sys,xml.etree.ElementTree as ET,json
work=Path(__file__).resolve().parent
exe=r'C:\ti\ccs1271\ccs\eclipse\eclipsec.exe'
ws=work/'ccs_workspace_relative'
logs=work/'logs'; logs.mkdir(exist_ok=True)
for model in ('ADP32F035','AVP32F00157'):
    root=work/'staging'/(model+'Boot+APP_20261009')/model
    for kind in ('Bootloader','Application'):
        p=root/kind
        for name in ('.project','.cproject','.ccsproject'):ET.parse(p/name)
        args=[exe,'-noSplash','-data',str(ws),'-application','com.ti.ccstudio.apps.projectImport','-ccs.location',str(p),'-ccs.autoBuild']
        result=subprocess.run(args,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=240)
        text=result.stdout.decode('utf-8',errors='replace')
        (logs/(model+'_'+kind+'_ccs_import_build.log')).write_text(text,encoding='utf-8')
        print(model,kind,'CCS return',result.returncode,flush=True)
        print(text[-6500:] if result.returncode else text[-850:],flush=True)
        if result.returncode:sys.exit(result.returncode)
print('PASS 4 fresh CCS 12.7.1 imports/builds',flush=True)
