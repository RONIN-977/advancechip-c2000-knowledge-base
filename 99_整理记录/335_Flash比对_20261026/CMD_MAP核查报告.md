# 客户F28335.cmd与MAP核查

日期：2026-10-26。用户提供D盘F28335.cmd、Ethercat_DSP_9252-HBI-402.map，已保留副本、SHA256并与此前369.out/OFD结果核对。只作静态检查，未修改CMD或客户工程、未重新编译烧录。

## 结论

**当前CMD/MAP未发现能够解释低概率局部连续两字错位的直接配置错误，不建议先改CMD。** 这不等于已经排除实际烧录插件的工作区、缓存或加载边界处理问题；这些须核对CCS6现场实际算法，而不能只由应用CMD得出。

## MAP与OUT对应性

- MAP显示链接器v6.2.5、链接时间2026-10-26 13:32:02，输出名Ethercat_DSP_9252-HBI-402.out，入口_c_int00=0x31ED94。
- 自动检查64个非空、可对应的输出段，其LOAD地址和字数均与369.out一致；空.pinit在MAP/OUT中的无效起点不同不参与地址冲突判断。
- 独立子代理检查2485个共有C_EXT符号，地址全部一致。证明布局对应，不将其扩大为两文件已通过完整二进制哈希对应。
- 实际含原始内容的装载段没有LOAD地址相互重叠。

| 段 | LOAD范围（字地址，含端点） | RUN | 说明 |
|---|---|---|---|
| .text | 0x300000–0x31F023 | 同LOAD | 代码 |
| .cinit | 0x31F024–0x32272B | 同LOAD | C启动初始化表 |
| ramfuncs | 0x32272C–0x32274A | 0x8000–0x801E | Flash存放、运行前复制到RAM |
| IQmath | 0x330000–0x3300A6 | 同LOAD | 正确落在物理Flash B |
| .econst | 0x338000–0x33957A | 同LOAD | 常量 |
| .switch | 0x33957C–0x3395E7 | 同LOAD | 跳转表 |
| codestart | 0x33FFF6–0x33FFF7 | 同LOAD | 启动跳转 |

RamfuncsLoadEnd=0x32274B是尾后地址，减去LoadStart=0x32272C为0x1F字，与段长度一致。仅从链接信息不能验证客户源代码的复制函数参数；本次未将其列为Flash镜像错位原因。

## MEMORY范围与PAGE检查

去掉注释后，客户CMD实际定义26个MEMORY区域，数值地址范围无相互重叠；未重复定义高低RAM镜像别名。

| CMD名称 | 实际范围 | 判断 |
|---|---|---|
| RAML0（PAGE0） | 0x8000–0x9FFF | 合并物理L0/L1，未与PAGE1 RAM重叠 |
| RAML2（PAGE1） | 0xA000–0xB1FF | 跨物理块，边界与下一段衔接，无重叠 |
| RAML3（PAGE1） | 0xB200–0xEFFF | 合并连续内部数据RAM，与上段衔接 |
| RAML5（PAGE1） | 0xF000–0xF4FF | 在物理L7内部划分 |
| RAML7（PAGE1） | 0xF500–0xF9FF | 与上一段相邻，未重叠 |
| FLASHH（PAGE0） | 0x300000–0x32FFFF | 合并物理Flash H至C；标签不限制只能使用H扇区 |
| FLASHC（PAGE0） | 0x330000–0x337FFF | 实际物理Flash B，名称不准确但地址合法 |
| FLASHA（PAGE0） | 0x338000–0x33FF7F | 后方预留CSM、启动和密码区域 |

跨连续物理块合并并非错误；链接器按origin/length分配，不能因名称仍叫RAML0或FLASHH就判定越界。没有理由仅因这些标签修改客户布局。

MAP中额外的CSM_PWL在PAGE0/PAGE1均出现，来源包括外设头文件的映射：PAGE0的csmpasswds没有实际载荷，PAGE1的CsmPwlFile为UNINITIALIZED，当前OUT未向密码地址生成两份初始化写入。它是需保持清楚的物理别名，不是本次0x3082AE、0x321FC8、0x322310错位的证据。本次不修改安全区定义。

## RAM实际分配及烧录工作区边界

| 段 | MAP实际范围 | 剩余/边界 |
|---|---|---|
| .stack | 0x400–0x6FF | RAMM1剩0x100字；静态分配不越界，实际栈峰值未验证 |
| .ebss | 0xA000–0xAA26 | 与.sysmem不重叠 |
| .sysmem | 0xAA28–0xB027 | RAML2分配合计0x1027/0x1200字，无越界 |
| .pardata | 0xB200–0xEB13 | RAML3分配0x3914/0x3E00字，无越界 |
| .PMSMdata | 0xF000–0xF2E1 | 位于对应0x500字RAM区以内 |

上述段在OUT中均无原始装载内容。尤其_ApplicationObjDic由Flash中的.cinit在客户启动时写入RAM，不能将其等同于CCS下载阶段直接把RAM数据覆盖进烧录算法工作区。ramfuncs也明确从Flash装载，而不是本次OUT直接装载RAM。

实际烧录算法当然也占用RAM，但“客户运行时需要某片RAM”不等于“烧录期间一定冲突”；需证明存在同时写入或算法运行中其他动作覆盖。本次没有该证据。

## 与异常地址的关系

- ram2代码异常0x3082AE–0x3083EF位于.text内部，MAP指向cia402appl.obj的.text。
- ram、ram2的两段初始化异常均位于cia402appl.obj的.cinit（0x320C39–0x3225C1）内部；对应_ApplicationObjDic的初始化内容。
- 三段起止都不是这些输出段的起止边界。未见LOAD/RUN交界、段越界或两段装载覆盖能自然解释“两字错位”。不因此排除烧录工具自身分块边界异常。

后续主线保持CCS6/XDS100v2加载与实际进芯Flash算法核对；Full Verify/UniFlash Verify Image用于拦截和验证，不作为写入根因已解决的证明。

可复算文件：check_cmd_map.py、CMD_MAP检查.json。原件SHA256：F28335.cmd=`32D0911D9E2E4807E375575DB6BF82E4F25A612B97256F4F9E41BACB11ACBD54`；MAP=`77CB7FD0C4091B428DCC6B798D6789244071F4E8E21698390977DCDF385D5DA1`。
