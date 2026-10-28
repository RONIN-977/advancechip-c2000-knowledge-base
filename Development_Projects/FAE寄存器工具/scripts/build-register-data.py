"""Reproducible extraction of TI bitfield headers. Never infer undocumented reset/access values."""
import pathlib,re,json,hashlib,shutil,collections,argparse
ROOT=pathlib.Path(__file__).resolve().parents[1]
BASE=pathlib.Path(r'C:\ti\c2000\C2000Ware_6_00_01_00')
SOURCES=ROOT/'docs'/'sources'
# Literal translations of the TI header descriptions; original text retained for audit.
TERMS={
'Reserved':'保留位','Register':'寄存器','Registers':'寄存器','Control':'控制','Status':'状态','Configuration':'配置','Interrupt':'中断','Enable':'使能','Disable':'禁用','Flag':'标志','Clear':'清除','Force':'强制','Select':'选择','Selection':'选择','Clock':'时钟','Counter':'计数器','Count':'计数','Period':'周期','Prescale':'预分频','Prescaler':'预分频器','Divider':'分频器','Divide':'分频','Baud':'波特率','Rate':'速率','Bit':'位','Timing':'时序','Time':'时间','Segment':'时间段','Synchronization':'同步','Sync':'同步','Jump':'跳转','Width':'宽度','Receive':'接收','Received':'已接收','Receiver':'接收器','Transmit':'发送','Transmission':'发送','Transmitter':'发送器','Transmitted':'已发送','Data':'数据','Buffer':'缓冲','FIFO':'FIFO队列','Error':'错误','Errors':'错误','ErrorCode':'错误码','ErrorCount':'错误计数','Reset':'复位','Software':'软件','Hardware':'硬件','Mode':'模式','Input':'输入','Output':'输出','Direction':'方向','Value':'数值','High':'高位','Low':'低位','Lower':'低位','Upper':'高位','System':'系统','Peripheral':'外设','Source':'源','Stop':'停止','Start':'启动','Reload':'重装载','Compare':'比较','Comparator':'比较器','Action':'动作','Qualifier':'限定器','Trip':'跳闸','Zone':'区域','Dead':'死区','Band':'带','Rising':'上升沿','Falling':'下降沿','Edge':'边沿','Polarity':'极性','Channel':'通道','Pulse':'脉冲','Event':'事件','Trigger':'触发','Sample':'采样','Sampling':'采样','Acquisition':'采样保持','Window':'窗口','Conversion':'转换','Result':'结果','Resolution':'分辨率','Reference':'参考','Voltage':'电压','Power':'电源','Down':'下降','Up':'上升','Phase':'相位','Shadow':'影子','Load':'加载','Emulation':'仿真','Debug':'调试','Free':'自由运行','Soft':'软','Halt':'暂停','Bus':'总线','BusOn':'总线恢复','Off':'关闭','On':'开启','Automatic':'自动','Retransmission':'重发','Message':'报文','Object':'对象','Mailbox':'邮箱','Mask':'屏蔽','Acceptance':'接收验收','Identifier':'标识符','Extended':'扩展','Standard':'标准','Remote':'远程','Request':'请求','Acknowledge':'应答','Acknowledgement':'应答','Lost':'丢失','Arbitration':'仲裁','Length':'长度','Code':'编码','Function':'功能','Pin':'引脚','Pins':'引脚','Mux':'复用','Multiplexer':'多路选择','Pull':'上拉','Qualification':'输入限定','Lock':'锁定','Commit':'提交','Write':'写入','Read':'读取','Access':'访问','Protection':'保护','Protected':'受保护','Watchdog':'看门狗','Key':'密钥','Check':'检查','Checkbits':'检查位','Valid':'有效','Validation':'校验','Address':'地址','Parity':'奇偶校验','Byte':'字节','Word':'字','Bits':'位','Level':'电平','Line':'线路','Master':'主机','Slave':'从机','Busy':'忙碌','Ready':'就绪','Timeout':'超时','TimeOut':'超时','Overflow':'溢出','Underflow':'下溢','Full':'满','Empty':'空','Non':'非','Blocking':'阻塞','Break':'中断帧','Wake':'唤醒','Sleep':'睡眠','Priority':'优先级','Group':'组','Vector':'向量','Table':'表','CPU':'CPU','Timer':'定时器','CPU-Timer':'CPU定时器','CountDown':'倒计数','Divide-Down':'分频','Enable/Disable':'使能/禁用','Operating':'工作','Operation':'操作','Active':'活动','Inactive':'非活动','Digital':'数字','Analog':'模拟','Calibration':'校准','Trim':'修调','Test':'测试','Module':'模块','Frame':'帧','Format':'格式','Character':'字符','Char':'字符','Size':'长度','Delay':'延迟','Number':'数量','NumberOf':'数量','Numberof':'数量','NumberofBits':'位数','Current':'当前','Previous':'前次','Next':'下次','First':'首个','Last':'最后','LastError':'最近错误','Initialization':'初始化','CAN':'CAN','eCAN':'eCAN','I2C':'I2C','SCI':'SCI','SPI':'SPI','ADC':'ADC','GPIO':'GPIO','PWM':'PWM','ePWM':'ePWM','ePWMx':'ePWMx','Time-Base':'时基','Time-base':'时基','Timebase':'时基','Time-Out':'超时','Real-Time':'实时','Interrupts':'中断','Int':'中断','INT':'中断','Set':'置位','Enable.':'使能','Flag.':'标志','Counter.':'计数器','Idle':'空闲','Position':'位置','Capture':'捕获','Raw':'原始','Debounce':'消抖','Asynchronous':'异步','Synchronous':'同步','Signal':'信号','Signals':'信号','Frequency':'频率','Multiplier':'倍频','Oscillator':'振荡器','Crystal':'晶振','Missing':'丢失','Detect':'检测','Detection':'检测','Detected':'已检测','External':'外部','Internal':'内部','Enablement':'使能','Programmable':'可编程','All':'全部','Global':'全局','Prescaling':'预分频','Stopbit':'停止位','Startbit':'起始位','Framing':'帧格式','Overrun':'溢出','Priority':'优先级','SoftReset':'软件复位','SW':'软件','Start-of-Conversion':'转换启动','Start-of-conversion':'转换启动','Start of Conversion':'转换启动','SOC':'SOC转换启动','Analog-to-Digital':'模数转换','Pending':'待处理','Offset':'偏移','Force':'强制','Continuous':'连续','One-Shot':'单次','One-shot':'单次','Digital':'数字','Trip-zone':'跳闸保护','trip-zone':'跳闸保护','Cycle-by-cycle':'逐周期','Cycle':'周期','Blanking':'消隐','Latch':'锁存','Latched':'已锁存','Latched':'锁存','DBC':'死区计数','Half':'半','Disable':'禁用','Disablement':'禁用','Deadband':'死区','Dead-band':'死区','Dead-Band':'死区','Auto':'自动','Auto-Bus-On':'自动总线恢复','Time-Out':'超时','Overflow':'溢出','Polarity':'极性','Prescaled':'预分频后','Accumulation':'累加','Accumulator':'累加器','Limit':'限制','Maximum':'最大','Minimum':'最小','Qualified':'限定后','unqualified':'未限定','SyncOut':'同步输出','SyncIn':'同步输入','Time-stamp':'时间戳','Timestamp':'时间戳','TimeStamp':'时间戳','Timestamp':'时间戳','32-bit':'32位','16-bit':'16位','8-bit':'8位','Bit-Rate':'位速率','Bit-rate':'位速率','Bitrate':'位速率','baudrate':'波特率','Bit-timing':'位时序','Bit-Timing':'位时序','Clocking':'时钟','Divider':'分频器','Free/Soft':'自由/软暂停','Debug':'调试','Priority':'优先级','Filter':'滤波','Threshold':'阈值','IntEnable':'中断使能','IntFlag':'中断标志','IntClear':'中断清除','IntForce':'中断强制','Cnt':'计数','Res':'结果','Capture':'捕获','Shadow':'影子','Selects':'选择','Specifies':'指定','Determines':'决定','controls':'控制','reset':'复位','enable':'使能','disable':'禁用','interrupt':'中断','register':'寄存器','control':'控制','status':'状态','counter':'计数器','count':'计数','clock':'时钟','source':'来源','bits':'位','bit':'位','and':'及','or':'或','of':'的','for':'用于','to':'至','in':'中','on':'开启','off':'关闭','the':'','is':'为','from':'来自','with':'带','at':'在','a':'','an':'','This':'此','this':'此','not':'不','used':'使用','when':'当','each':'每个','only':'仅','If':'如果','if':'如果','into':'进入','as':'作为','by':'通过','mode':'模式','high':'高','low':'低','data':'数据','input':'输入','output':'输出','enable':'使能','select':'选择','stop':'停止','state':'状态','reserved':'保留位','Read-only':'只读','Write-only':'只写','Read/Write':'读写','value':'值','operation':'操作','Register.':'寄存器'}
PHRASES={
'Baud Rate Prescaler':'波特率预分频值','Bit Timing Configuration':'位时序配置','Time Segment':'时间段','Synchronization Jump Width':'重同步跳转宽度','Transmit Request Set':'发送请求置位','Transmit Request Reset':'发送请求清除','Receive Message Pending':'接收报文待处理','Message Lost':'报文丢失','Transmit Acknowledge':'发送应答','Module Control':'模块控制','Error and Status':'错误与状态','Control and Status':'控制与状态','Data Direction':'数据方向','General Purpose':'通用','General-Purpose':'通用','Time Base':'时基','Time-Base':'时基','Action-Qualifier':'动作限定','Dead-Band':'死区','Dead-band':'死区','Trip-Zone':'跳闸保护','Sample and Hold':'采样保持','Sample-and-hold':'采样保持','Sample Window':'采样窗口','Start of Conversion':'启动转换','Start Of Conversion':'启动转换','Start-of-Conversion':'启动转换','Start-of-conversion':'启动转换','Single-ended':'单端','Differential':'差分','PLL Status':'PLL状态','High-Speed':'高速','Low-Speed':'低速','High Speed':'高速','Low Speed':'低速','High-speed':'高速','Low-speed':'低速','Parity on/off':'奇偶校验开关','Interrupt line 1 Enable Disabled':'中断线路1使能（原头文件注释含Disabled，具体含义查TRM）'}
TERMS.update({"Temperature":"温度","sensor":"传感器","connection":"连接","generation":"产生","Inversion":"反相","Zero":"零","One":"单次","Shot":"触发","Type":"类型","Block":"模块","Post":"后置","Processing":"处理","Defines":"定义","Conditions":"条件","While":"期间","Wakeup":"唤醒","Pull-up":"上拉","Open-drain":"开漏","Toggle":"翻转","One-time":"一次性","Valley":"谷值","Crossing":"交越","Link":"链接","Up-count":"向上计数","Down-count":"向下计数","OneShot":"单次触发","Chopping":"斩波","Match":"匹配","Sign":"符号","Selected":"已选","Pre-scale":"预分频","Pre-scaler":"预分频器","Outpout":"输出","Invoked":"触发","Stamp":"戳","Swap":"交换","Extension":"扩展","Indicates":"表示","Indication":"指示","Options":"选项","Duty":"占空","Hold":"保持","Chop":"斩波","Inverted":"反相","Complementary":"互补","Phase-shift":"相移","Restart":"重新启动","Emulator":"仿真器","Standby":"待机","Idle":"空闲","Remainder":"余数","Additional":"额外","Suspend":"暂停","Suspend-Signal":"暂停信号","Divisor":"除数","Integer":"整数","Fractional":"小数","Locking":"锁定","Muxing":"复用","Pin-muxing":"引脚复用","Continuous":"连续","Dither":"抖动","Sets":"设置","Setting":"设置","Qualify":"限定","Log":"记录","Self-test":"自检","Stage":"级","Stages":"级","Balance":"平衡","Rollover":"回卷","Maintained":"保持","Disabled":"已禁用","Enabled":"已使能","Assert":"置有效","Deassert":"取消有效","Asserted":"已置有效","Happens":"发生","Occurs":"发生","Generating":"产生","Generate":"产生","Generates":"产生","Ignore":"忽略","Ignored":"忽略","Receive-buffer":"接收缓冲","Transmit-buffer":"发送缓冲","Reception":"接收","Latching":"锁存","Multiplexing":"复用","Half-cycle":"半周期","Sync-out":"同步输出","Tripzone":"跳闸保护","Zones":"区域","Event-trigger":"事件触发","Digital-compare":"数字比较","Zero-crossing":"过零","Internal/external":"内部/外部","Selection":"选择","Combinational":"组合逻辑","Combination":"组合","Destination":"目的","Destinations":"目的","Triggers":"触发","Flag/clear":"标志/清除","Bits":"位","VREFLO":"低参考电压","VREFHI":"高参考电压","Tracking":"跟踪","Gain":"增益","Result":"结果","ASysCtl":"模拟系统控制","TSNS":"温度传感器","Low-pass":"低通","Path":"路径","Positive":"正","Negative":"负","Filler":"填充","Wider":"更宽","LowTime":"低电平时间","HighTime":"高电平时间","Nonoverlap":"不重叠","Non-overlap":"不重叠","Store":"存储","Write-protect":"写保护","Load-on":"加载时机","Whether":"是否","Min":"最小","Max":"最大","Update":"更新","Exceed":"超过","Compare-based":"基于比较","Do":"执行","Done":"完成","Cycle-by-cycle":"逐周期","Retransmit":"重发","Retry":"重试","Programmable":"可编程","Length":"长度","Position":"位置","Samples":"采样点","Clocks":"时钟周期","Prescaler":"预分频器","Prescale":"预分频","Saturate":"饱和","Saturation":"饱和","Saturating":"饱和","Validity":"有效性","Enable/disable":"使能/禁用","Enable/Disable":"使能/禁用","Generated":"已产生","Resetting":"复位","Clamping":"钳位","Clamp":"钳位","Off-set":"偏移","Offest":"偏移","OffestTrim":"偏移修调","Multiplier":"倍频器","Predivider":"前置分频器","Divide-by":"分频比","Delay":"延迟","Repetition":"重复","Repeat":"重复","Strobe":"选通","Most":"最高","Least":"最低","Significant":"有效","Boundary":"边界","Filter":"滤波器","Disables":"禁用","Enables":"使能","Reflects":"反映","Contains":"包含","Content":"内容","Contents":"内容","Default":"默认","Auto-conversion":"自动转换","Acquistion":"采样保持","Acquistion-window":"采样保持窗口","Prescaling":"预分频","Decrement":"递减","Increment":"递增","Sync-select":"同步选择","Sync-input":"同步输入","Condition":"条件","Bitrate":"位速率","Direction":"方向","Clock-enable":"时钟使能","Adcint":"ADC中断","Config":"配置","Offset":"偏移","Latched":"锁存","Blanking-window":"消隐窗口","Lock-bit":"锁定位","test":"测试"})
TERMS_LOWER={k.lower():v for k,v in TERMS.items()}
def cn(text):
 text=re.sub(r'^\s*\d+(?::\d+)?\s*','',text).strip()
 text=re.sub(r'\s+',' ',text)
 if not text:return '资料未提供用途说明；保留原字段名供查阅。'
 orig=text
 for en,zh in sorted(PHRASES.items(),key=lambda x:-len(x[0])):text=text.replace(en,zh)
 text=re.sub(r'[A-Za-z][A-Za-z0-9]*(?:[-/][A-Za-z][A-Za-z0-9]*)*',lambda m:TERMS.get(m[0],TERMS_LOWER.get(m[0].lower(),m[0])),text)
 text=re.sub(r'\s+',' ',text).strip()
 return text

def parse_file(path):
 text=path.read_text(errors='replace')
 defs={}
 for m in re.finditer(r'\b(struct|union)\s+(\w+)\s*\{(.*?)\};',text,re.S):
  kind,name,body=m.groups();members=[]
  for ln in body.splitlines():
   code,_,comment=ln.partition('//');code=code.strip()
   if not code:continue
   bm=re.fullmatch(r'(?:Uint|uint|bp_)(16|32|64)(?:_t)?\s+(\w+)\s*:\s*(\d+)\s*;',code)
   if bm:members.append(('bits',bm[2],int(bm[3]),comment.strip()));continue
   mm=re.fullmatch(r'(?:(struct|union)\s+)?([A-Za-z_]\w*)\s+(\w+)\s*(?:\[\s*(\d+)\s*\])?\s*;',code)
   if mm:members.append(('member',mm[3],mm[2],int(mm[4] or 1),comment.strip()))
  defs[name]=(kind,members,path)
 return defs

def size(typ,defs,cache):
 if typ in cache:return cache[typ]
 prim={'Uint16':1,'uint16_t':1,'bp_16':1,'Uint32':2,'uint32_t':2,'bp_32':2,'Uint64':4,'uint64_t':4}
 if typ in prim:return prim[typ]
 if typ not in defs:raise ValueError('unknown type '+typ)
 kind,members,_=defs[typ]
 if members and members[0][0]=='bits':n=(sum(m[2] for m in members)+15)//16
 elif kind=='union':n=max(size(m[2],defs,cache)*m[3] for m in members if m[0]=='member')
 else:
  n=0
  for m in members:
   if m[0]!='member':continue
   s=size(m[2],defs,cache);align=min(s,2)
   # Arrays of Uint16 remain word aligned, 32-bit registers align to even word.
   if align==2 and n%2:n+=1
   n+=s*m[3]
 cache[typ]=n;return n

def fields(typ,defs):
 if typ not in defs:return []
 kind,members,_=defs[typ]
 if kind=='union':
  b=next((m[2] for m in members if m[0]=='member' and m[1]=='bit'),None)
  return fields(b,defs) if b else []
 result=[];bit=0
 for m in members:
  if m[0]!='bits':continue
  _,name,width,desc=m
  result.append(dict(Name=name,Lsb=bit,Width=width,Access='待核对TRM',Reset='待核对TRM',Description=cn(desc),Values='头文件未给出编码取值，请查对应TRM。',Notes='保留位不得用于配置。' if re.match(r'rsvd|reserved',name,re.I) else '原文：'+desc))
  bit+=width
 return result

CHIPS=[('035','f2803x','DSP2803x'),('335','f2833x','DSP2833x'),('0039','f28003x','f28003x'),('0049','f28004x','f28004x')]
MODULES={'adc':'ADC','ecan':'CAN','can':'CAN','epwm':'PWM','gpio':'GPIO','i2c':'I2C','sci':'SCI','spi':'SPI','sysctrl':'时钟','piectrl':'中断','xintrupt':'中断','xint':'中断','nmiintrupt':'中断','cputimers':'定时器','cputimer':'定时器','wwd':'看门狗'}
def main():
 allregs=[];manifest=[];missing=[]
 for chip,family,prefix in CHIPS:
  local=SOURCES/family;local.mkdir(parents=True,exist_ok=True)
  hdrroot=BASE/'device_support'/family/'headers'
  # Cache only inputs consumed; allow rebuilding from the bundled files on another machine.
  if hdrroot.exists():
   for h in (hdrroot/'include').glob('*.h'):
    suffix=h.stem[len(prefix)+1:].lower()
    if suffix in MODULES:shutil.copy2(h,local/h.name)
   for sub,pat in [('cmd','*non*'),('source','*lobal*')]:
    for f in (hdrroot/sub).glob(pat):
     if f.suffix.lower() in ('.c','.cmd'):shutil.copy2(f,local/f.name)
  # Byte-peripheral CAN structs have a special C28x compiler layout. Never infer their addresses with ordinary sizeof rules.
  if chip in ('0039','0049'):
   hw=BASE/'driverlib'/family/'driverlib'/'inc'/'hw_can.h'
   if hw.exists():shutil.copy2(hw,local/'hw_can.h')
  can_offsets={}
  hwcopy=local/'hw_can.h'
  if hwcopy.exists():
   can_offsets={name:int(value,16) for name,value in re.findall(r'#define\s+CAN_O_(\w+)\s+0x([0-9A-Fa-f]+)U',hwcopy.read_text())}
  defs={};hmods={} 
  for h in local.glob('*.h'):
   suffix=h.stem[len(prefix)+1:].lower();hmods[h.name]=MODULES.get(suffix,'其他');defs.update(parse_file(h))
  cmds=list(local.glob('*.cmd'));globals_=list(local.glob('*.c'))
  cmd='\n'.join(f.read_text(errors='replace') for f in cmds)
  glob='\n'.join(f.read_text(errors='replace') for f in globals_)
  bases={k:int(v,16) for k,v in re.findall(r'(\w+)\s*:\s*origin\s*=\s*0x([0-9a-fA-F]+)',cmd,re.I)}
  sections=dict(re.findall(r'(\w+)\s*:\s*>\s*(\w+)',cmd))
  pragmas=dict(re.findall(r'#pragma\s+DATA_SECTION\(\s*(\w+)\s*,\s*"(\w+)"',glob))
  instances=re.findall(r'volatile\s+struct\s+(\w+)\s+(\w+)\s*;',glob)
  cache={}
  for typ,inst in instances:
   if typ not in defs:continue
   source=defs[typ][2];module=hmods.get(source.name,'其他')
   if module=='其他':continue
   section=pragmas.get(inst);region=sections.get(section);base=bases.get(region)
   if base is None:missing.append(f'{chip}: missing base {typ}/{inst}/{section}');continue
   def emit(t,stem,addr):
    off=0
    for mem in defs[t][1]:
     if mem[0]!='member':continue
     _,name,mt,count,comment=mem
     try:words=size(mt,defs,cache)
     except ValueError as e:missing.append(str(e));break
     if min(words,2)==2 and off%2:off+=1
     for i in range(count):
      nm=name+(f'[{i}]' if count>1 else '')
      if re.match(r'rsvd|reserved',name,re.I):continue
      a=addr+off+i*words
      if module=='CAN' and chip in ('0039','0049'):
       suffix=name.removeprefix('CAN_')
       if suffix not in can_offsets:raise ValueError('Missing official CAN offset '+name)
       a=base+can_offsets[suffix]
      if mt in defs and defs[mt][0]=='struct' and not mt.endswith('_BITS'):
       emit(mt,stem+'.'+nm,a);continue
      ff=fields(mt,defs)
      if not ff:
       ff=[dict(Name=name,Lsb=0,Width=words*16,Access='待核对TRM',Reset='待核对TRM',Description=cn(comment),Values='原始数值字段，具体有效范围请查TRM。',Notes='原文：'+comment)]
      special=''
      if chip in ('035','335') and name in ('SCSR','WDCNTR','WDKEY','WDCR'):grp='看门狗'
      elif typ=='WD_REGS':grp='看门狗'
      else:grp=module
      entry=dict(ChipId=chip,Peripheral=grp,Name=stem+'.'+nm,Address=f'0x{a:06X}',Width=words*16,Description=cn(comment),Keywords=' '.join([grp,typ,inst,name,comment,'通信 接收 发送 波特率' if grp in ('CAN','SCI','SPI','I2C') else '脉宽 频率 周期 占空比' if grp=='PWM' else '模数 采样 电压' if grp=='ADC' else '']),Source=f'C2000Ware 6.00.01.00；docs/sources/{family}/{source.name}；结构 {typ} / 字段 {name}；基址 {region} 来自 {cmds[0].name}',Notes='地址单位：16位字（C28x），非字节。基于TI寄存器头文件；尚未完成进芯实板验证。'+special+(' DCAN地址直接采用同目录hw_can.h中CAN_O_*偏移，避免byte_peripheral特殊布局误算。' if chip in ('0039','0049') and module=='CAN' else ''),Fields=ff)
      allregs.append(entry)
     off+=words*count
   emit(typ,inst,base)
  for f in sorted(local.iterdir()):
   manifest.append(dict(Path=str(f.relative_to(ROOT)).replace('\\','/'),Sha256=hashlib.sha256(f.read_bytes()).hexdigest(),Origin='Texas Instruments C2000Ware_6_00_01_00/device_support/'+family+'/headers',Bytes=f.stat().st_size))
 # Validate shape before releasing.
 for r in allregs:
  occupied=set()
  for f in r['Fields']:
   bits=set(range(f['Lsb'],f['Lsb']+f['Width']))
   assert not bits&occupied,(r['Name'],f['Name'])
   assert max(bits)<r['Width'],(r['Name'],f['Name'])
   occupied|=bits
 out=ROOT/'Data';out.mkdir(exist_ok=True)
 (out/'registers.json').write_text(json.dumps(dict(Version='1.0.0',GeneratedOn='2026-10-22',Registers=allregs),ensure_ascii=False,indent=2),encoding='utf-8')
 (SOURCES/'manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
 (SOURCES/'extraction-notes.json').write_text(json.dumps(missing,ensure_ascii=False,indent=2),encoding='utf-8')
 counts=collections.Counter((r['ChipId'],r['Peripheral']) for r in allregs)
 print('registers',len(allregs),'fields',sum(len(r['Fields']) for r in allregs));print(counts);print('missing',missing)
# main invocation appears at end to permit enrichment helpers

# PDF extraction uses only official downloaded manuals; page numbers are physical PDF pages.
def extract_manual(pdf):
 from pypdf import PdfReader
 reader=PdfReader(pdf);name=pathlib.Path(pdf).stem
 ranges={'sprui10':[(0,901)],'sprui07':[(0,841)],'sprui33':[(150,665),(948,1040),(1575,1650),(2003,2200),(2304,2440),(2509,2570)],'spruiw9':[(155,710),(1108,1240),(1890,2000),(2343,2560),(2646,2780),(2854,2913)]}[name]
 pages={}
 for lo,hi in ranges:
  for i in range(lo,min(hi,len(reader.pages))):
   pages[str(i+1)]=reader.pages[i].extract_text()
 dest=pathlib.Path(pdf).with_suffix('.pages.json');dest.write_text(json.dumps(pages,ensure_ascii=False),encoding='utf-8')
 print(name,len(pages),flush=True)

MANUALS={'035':('sprui10','SPRUI10A / 2022-06'),'335':('sprui07','SPRUI07 / 2020-03'),'0039':('spruiw9','SPRUIW9C / 2024-03'),'0049':('sprui33','SPRUI33H / 2024-06')}
ACCESS={'R':'只读','W':'只写','R/W':'读写','R/W1C':'读；写1清零','R/W1S':'读；写1置位','R/W1S/W1C':'特殊置位/清除','R/W1SC':'读写；硬件自动清零','R/W1CS':'读写；写1清除/置位（见原文）','R/W0C':'读；写0清零','R/WP':'读写（受保护）','RWPI':'所有模式可读；初始化且EALLOW时可写','RWPO':'所有模式可读；操作模式可写','R/W/1C':'读写；写1清零','R/WC':'读写；清除行为见原文','R/WHS':'读写；硬件置位','R/WC-1':'读写；清除行为见原文','R/WSC':'读写；硬件自动清零','R/W1C/W1S':'写1清除/置位','R-0':'只读','RC':'读清零','R/C':'读清零','R/W1S/W1C':'写1置位/清除'}

def manual_tables(code):
 src=SOURCES/(code+'.pages.json')
 if not src.exists():return {}
 pages=json.loads(src.read_text(encoding='utf-8'));tables=collections.defaultdict(list)
 for page,txt in pages.items():
  # Do not parse prose sections: only register field-description tables.
  starts=list(re.finditer(r'Table\s+[\d-]+\.\s+([^\n]+?)Field Descriptions(?:\s*\(continued\))?\s*\n',txt))
  for k,m in enumerate(starts):
   title=m[1].strip();reg=''
   a=re.search(r'\(([A-Za-z][A-Za-z0-9_]+)\)',title)
   if a:reg=a[1]
   else:
    a=re.match(r'([A-Za-z][A-Za-z0-9_]+)\s+Register',title)
    if a:reg=a[1]
   if not reg:continue
   block=txt[m.end():starts[k+1].start() if k+1<len(starts) else len(txt)]
   # Drop footer and following section/figure material; retain continuation rows.
   block=re.split(r'\n(?:www\.ti\.com|Copyright|Submit Document|[A-Z][A-Za-z -]+ www\.ti\.com|\d+\.\d+(?:\.\d+)*\s)',block)[0]
   rows=list(re.finditer(r'^(\d+(?:[-:]\d+)?)\s+([A-Za-z_]\w*)(?:\.[\d:]+)?\s+(.*)$',block,re.M))
   for i,row in enumerate(rows):
    bitstr,name,first=row.groups()
    # Value rows start with a number and then English words; demand a known field form later by exact bit/name match.
    if name in ('The','This','No','All','Any','CPU','Stop','Start','Read','Write','When','In','Do','If','For','Reserved') and name!='Reserved':continue
    content=first+'\n'+block[row.end():rows[i+1].start() if i+1<len(rows) else len(block)]
    content=re.sub(r'\nReset type:.*?(?=\n|$)','',content).strip()
    parts=re.split('[-:]',bitstr);hi=int(parts[0]);lo=int(parts[-1]);width=hi-lo+1
    if hi>63 or width<1:continue
    access=reset=''
    am=re.match(r'((?:R|W)[A-Z0-9/]*)(?:\s+)([0-9A-Fa-f]+h|[xX-])\s*(.*)',content,re.S)
    if am:
     access,reset,content=am.groups()
    tables[reg].append(dict(Name=name,Lsb=lo,Width=width,Access=access,Reset=reset,Text=content,Page=int(page),Title=title))
 return tables

# Human-reviewed descriptions and encodings for frequently adjusted fields.
# All applied only when the field name, register name and TI family match.
COMMON={
('TBCTL','CTRMODE'):('时基计数模式。','0：向上计数；1：向下计数；2：上下计数；3：停止/冻结。','读写','3'),
('TBCTL','PHSEN'):('相位加载使能。','0：禁用相位加载；1：收到同步事件时将TBPHS载入TBCTR。','读写','0'),
('TBCTL','PRDLD'):('周期寄存器的影子加载模式。','0：使用影子寄存器，计数器归零时装载；1：直接写活动寄存器。','读写','0'),
('TBCTL','CLKDIV'):('时基低速预分频选择。','编码0/1/2/3/4/5/6/7分别对应分频1/2/4/8/16/32/64/128。','读写','0'),
('TBCTL','HSPCLKDIV'):('时基高速预分频选择。','编码0/1/2/3/4/5/6/7分别对应分频1/2/4/6/8/10/12/14。','读写','1'),
('TBCTL','SWFSYNC'):('软件强制同步。','写1产生软件同步脉冲；硬件清零。','读写；硬件自动清零','0'),
('TBCTL','PHSDIR'):('上下计数模式下，同步载入相位后的计数方向。','0：同步后向下计数；1：同步后向上计数。','读写','0'),
('TCR','TSS'):('CPU定时器停止控制。','0：运行；1：停止。','读写','1'),
('TCR','TRB'):('定时器重新装载。','写1将周期值载入计数器，并重新装载预分频计数器；读取始终为0。','只写；读回0','0'),
('TCR','TIE'):('CPU定时器中断使能。','0：禁用；1：使能。','读写','0'),
('TCR','TIF'):('CPU定时器溢出标志。','0：尚未减到零；1：已减到零。写1清除，写0无作用。','读；写1清零','0'),
('SCICCR','SCICHAR'):('SCI每个字符的数据位数。','编码0至7分别对应1至8位数据。','读写','0'),
('SCICCR','STOPBITS'):('SCI停止位数。','0：1个停止位；1：2个停止位。','读写','0'),
('SCICCR','PARITYENA'):('SCI奇偶校验使能。','0：禁用校验；1：使能校验。','读写','0'),
('SCICCR','PARITY'):('SCI奇偶校验方式，仅PARITYENA=1时有效。','0：奇校验；1：偶校验。','读写','0'),
('SCICTL1','RXENA'):('SCI接收使能。','0：禁用接收；1：使能接收。','读写','0'),
('SCICTL1','TXENA'):('SCI发送使能。','0：禁用发送；1：使能发送。','读写','0'),
('SCICTL1','SWRESET'):('SCI软件复位。','0：保持SCI复位；1：释放复位、允许工作。','读写','0'),
('SPICCR','SPICHAR'):('SPI每个字符的数据位数。','编码0至15分别对应1至16位。','读写','0'),
('SPICCR','CLKPOLARITY'):('SPI时钟极性。','0：空闲低电平；1：空闲高电平。采样边沿还取决于CLK_PHASE。','读写','0'),
('SPICCR','SPISWRESET'):('SPI软件复位。','0：保持SPI复位；1：释放复位。','读写','0'),
('SPICTL','MASTER_SLAVE'):('SPI主从模式选择。','0：从机；1：主机。','读写','0'),
('SPICTL','TALK'):('SPI发送输出使能。','0：禁止发送引脚驱动；1：允许发送。','读写','0'),
('ADCCTL1','INTPULSEPOS'):('ADC中断脉冲相对转换过程的位置。','0：采样窗口结束时；1：转换结束时。','读写','0'),
('ADCCTL1','ADCPWDNZ'):('ADC模拟电路电源控制。','0：掉电；1：上电。上电后须满足手册规定的稳定时间。','读写','0'),
('ADCCTL1','ADCBSY'):('ADC忙碌状态。','0：可接受下一通道采样；1：ADC正在处理转换。','只读','0'),
('ADCCTL1','TEMPCONV'):('将ADCINA5连接到内部温度传感器。','0：禁用内部温度传感器连接；1：连接内部温度传感器。','读写','0'),
('ADCCTL1','VREFLOCONV'):('将ADCINB5连接到VREFLO。','0：使用外部ADCINB5；1：内部连接至VREFLO。','读写','0'),
('ADCCTL1','ADCREFSEL'):('ADC参考电压来源选择。','0：内部参考；1：外部参考。','读写','0'),
('ADCSOC0CTL','ACQPS'):('SOC0采样保持窗口长度。','采样窗口长度为ACQPS+1个采样时钟周期；必须核对该型号时钟定义及最小采样时间，不能跨系列套用。','读写','0'),
('ADCSOC0CTL','CHSEL'):('SOC0模拟输入通道选择。','编码与通道对应关系随具体器件和封装变化，必须核对本型号TRM通道表。','读写','0'),
('PIECTRL','ENPIE'):('PIE中断控制器使能。','0：禁用PIE；1：使能PIE。CPU全局中断和IER仍需单独配置。','读写','0'),
('WDCR','WDPS'):('看门狗预分频选择。','编码0/1：除1；2：除2；3：除4；4：除8；5：除16；6：除32；7：除64。','读写','0'),
('WDCR','WDCHK'):('看门狗控制寄存器写入校验位。','写WDCR时必须写入二进制101；其他值可能立即触发复位。','读写','0'),
('WDCR','WDDIS'):('看门狗禁用控制。','0：使能看门狗；1：禁用看门狗，能否禁用还受SCSR.WDOVERRIDE约束。','读写','1'),
('WDKEY','WDKEY'):('看门狗喂狗密钥。','按顺序写0x55和0xAA服务看门狗；不可写入不正确序列。','只写','0'),
('SCSR','WDENINT'):('看门狗超时响应选择。','0：复位模式；1：中断模式。','读写','0'),
}
ECAN={
'BRPREG':('CAN位时间量子的预分频编码。','实际分频BRP=BRPREG+1。TI基线不允许BRP=1，故BRPREG应至少为1；CAN时钟=SYSCLKOUT/2。'),
'TSEG1REG':('CAN时间段1编码，包括传播段和相位缓冲段1。','TSEG1=TSEG1REG+1 TQ；TSEG1须不小于TSEG2和IPT。'),
'TSEG2REG':('CAN时间段2编码，即相位缓冲段2。','TSEG2=TSEG2REG+1 TQ；TSEG2须不小于IPT，并不大于TSEG1。BRP>1时IPT=2 TQ。'),
'SJWREG':('重同步跳转宽度编码。','SJW=SJWREG+1 TQ；SJW不得大于TSEG2，且最大4 TQ。'),
'SAM':('CAN总线电平采样次数。','0：单次采样；1：3次采样多数表决。3次采样仅允许BRP>4。')}
DCAN={
'BRP':('CAN波特率分频值的低6位。','实际分频=BRP+64×BRPE+1。'),
'BRPE':('CAN波特率分频值的高4位。','与BRP拼接得到10位分频编码，实际分频为编码+1。'),
'TSEG1':('CAN时间段1编码。','实际TSEG1=TSEG1+1 TQ；合法编程值1至15，即实际2至16 TQ。'),
'TSEG2':('CAN时间段2编码。','实际TSEG2=TSEG2+1 TQ，范围1至8 TQ。'),
'SJW':('CAN重同步跳转宽度编码。','实际SJW=SJW+1 TQ，范围1至4 TQ，不得大于TSEG2。')}

def enrich():
 path=ROOT/'Data/registers.json';pack=json.loads(path.read_text(encoding='utf-8'))
 indexes={c:manual_tables(code) for c,(code,version) in MANUALS.items()}
 stats=collections.Counter();samples=[]
 for r in pack['Registers']:
  chip=r['ChipId'];short=r['Name'].split('.')[-1];code,version=MANUALS[chip]
  names=[short]
  if r['Peripheral']=='CAN' and chip in ('0039','0049'):names.insert(0,'CAN_'+short)
  if r['Peripheral']=='定时器':names+=['TIMERx'+short]
  if chip=='035' and re.fullmatch(r'ADCSOC\d+CTL',short):names+=['ADCSOCxCTL']
  tables=indexes[chip];rows=[]
  for n in names:rows.extend(tables.get(n,[]))
  pages=set()
  for f in r['Fields']:
   candidates=[x for x in rows if x['Lsb']==f['Lsb'] and x['Width']==f['Width'] and (x['Name'].lower()==f['Name'].lower() or (re.match(r'rsvd',f['Name'],re.I) and x['Name'].upper()=='RESERVED'))]
   if candidates:
    x=max(candidates,key=lambda x:(bool(x['Access']),len(x['Text'])))
    pages.add(x['Page']);stats['trm_fields']+=1
    if x['Access']:
     f['Access']=ACCESS.get(x['Access'],'原文访问类型：'+x['Access']);f['Reset']=x['Reset'];stats['access_reset']+=1
    # Exact full English evidence always retained; machine terminology rendering is not marked reviewed.
    translated=cn(x['Text'])
    f['Values']='TRM术语译注（未逐句人工复核）：'+translated
    f['Notes']+='；'+version+f' PDF第{x["Page"]}页；原文字段表：'+x['Text']
   reviewed=None
   # ADC SOC acquisition fields have identical encodings across SOC instances, not all legacy devices have same width.
   key=short.removeprefix('CAN_') if r['Peripheral']=='CAN' else short
   if re.fullmatch(r'ADCSOC\d+CTL',key):key='ADCSOC0CTL'
   if (key,f['Name']) in COMMON:reviewed=COMMON[(key,f['Name'])]
   if short=='CANBTC' and f['Name'].upper() in ECAN:
    desc,vals=ECAN[f['Name'].upper()];reviewed=(desc,vals,'所有模式可读；初始化且EALLOW时可写','0')
   if r['Peripheral']=='CAN' and key=='BTR' and f['Name'] in DCAN:
    desc,vals=DCAN[f['Name']];reviewed=(desc,vals,'读写（INIT=1且CCE=1时配置）','1h' if f['Name']=='BRP' else '2h' if f['Name']=='TSEG2' else '3h' if f['Name']=='TSEG1' else '0h')
   if reviewed:
    f['Description'],f['Values'],f['Access'],f['Reset']=reviewed
    f['Notes']+='；关键字段中文说明已按'+version+'对应字段表核对。';stats['reviewed_fields']+=1
   if not reviewed:f['Notes']+='；中文按原文术语整理，尚未完成逐句人工复核。'
   if re.match(r'rsvd',f['Name'],re.I):
    f['Description']='保留位；不要用于用户配置。'
    f['Values']='按该寄存器手册要求保留；不得假定保留位都可写0。'
   if r['Peripheral']=='GPIO' and re.match(r'GPIO\d+',f['Name']):
    if short.endswith('DIR'):f['Values']='0：输入；1：输出。';f['Description']=f['Name']+'引脚方向。'
    elif short.endswith('PUD'):f['Values']='0：使能内部上拉；1：禁用内部上拉。';f['Description']=f['Name']+'上拉禁用控制。'
    elif 'QSEL' in short:f['Values']='0：与系统时钟同步；1：3点采样限定；2：6点采样限定；3：异步输入。';f['Description']=f['Name']+'输入限定方式。'
    elif short.endswith('SET'):f['Values']='写1置位对应输出；写0无作用。';f['Description']=f['Name']+'输出置位。'
    elif short.endswith('CLEAR'):f['Values']='写1清除对应输出；写0无作用。';f['Description']=f['Name']+'输出清除。'
    elif short.endswith('TOGGLE'):f['Values']='写1翻转对应输出；写0无作用。';f['Description']=f['Name']+'输出翻转。'
   # Unsupported field semantics remain explicit instead of silently treating every enable as same encoding.
   if not re.search('[\u4e00-\u9fff]',f['Description']):f['Description']='中文释义待整理；原字段说明：'+f['Description']
  if pages:r['Source']+='；'+version+'，docs/sources/'+code+'.pdf，PDF页 '+','.join(map(str,sorted(pages)))
  else:r['Source']+='；配套TRM '+version+'：docs/sources/'+code+'.pdf（本条尚未建立字段页定位）'
  if chip=='335' and re.match('ADCRESULT',short):
   addr=int(r['Address'],16)
   if 0x7108<=addr<=0x7117:r['Notes']+=' ADC结果为12位左对齐，换算前右移4位；SPRUI07 PDF第490页。'
   elif 0xB00<=addr<=0xB0F:r['Notes']+=' ADC结果镜像为12位右对齐，直接取低12位换算，无需右移；SPRUI07 PDF第490页。'
  # More precise register names for common plain-value registers.
  if short=='TBPRD':r['Description']='PWM时基周期寄存器，决定PWM周期。';r['Fields'][0]['Description']='时基周期计数值。';r['Fields'][0]['Values']='向上/向下计数：频率=TBCLK/(TBPRD+1)；上下计数：频率=TBCLK/(2×TBPRD)。'
  if short=='SCIHBAUD':r['Description']='SCI波特率分频值高8位。'
  if short=='SCILBAUD':r['Description']='SCI波特率分频值低8位。'
  if short=='CANBTC':r['Description']='eCAN波特率位时序配置寄存器。';r['Keywords']+=' 波特率 分频 采样点 位时序'
  if short=='CAN_BTR' and r['Peripheral']=='CAN':r['Description']='DCAN波特率位时序配置寄存器。';r['Keywords']+=' 波特率 分频 采样点 位时序'
 path.write_text(json.dumps(pack,ensure_ascii=False,indent=2),encoding='utf-8')
 (SOURCES/'enrichment-stats.json').write_text(json.dumps(dict(stats),ensure_ascii=False,indent=2),encoding='utf-8')
 manifest=json.loads((SOURCES/'manifest.json').read_text(encoding='utf-8'))
 for code,version in MANUALS.values():
  for f in [SOURCES/(code+'.pdf'),SOURCES/(code+'.pages.json')]:
   if f.exists():manifest.append(dict(Path=str(f.relative_to(ROOT)).replace('\\','/'),Sha256=hashlib.sha256(f.read_bytes()).hexdigest(),Origin='https://www.ti.com/lit/pdf/'+code,Version=version,Bytes=f.stat().st_size))
 (SOURCES/'manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
 print('TRM enrichment',dict(stats))

if __name__=='__main__':
 main()
 enrich()
