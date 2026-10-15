from pathlib import Path
import re,sys
root=Path(__file__).parent/'文本缓存'
for arg in sys.argv[1:]:
    sid,pat=arg.split(':',1)
    data=(root/f'{sid}.txt').read_text(encoding='utf-8')
    print(sid)
    blocks=re.split(r'=== PDF页 (\d+) ===',data)
    shown=0
    for i in range(1,len(blocks),2):
        txt=blocks[i+1]
        match=re.search(pat,txt,re.I)
        if match and '....' not in txt[max(0,match.start()-60):match.end()+60]:
            print('PDF页 '+blocks[i]+': '+re.sub(r'\s+',' ',txt[max(0,match.start()-100):match.end()+500]))
            shown+=1
            if shown>=8: break
