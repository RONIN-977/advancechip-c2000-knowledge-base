# 客户0039 CAN在线升级移植版

以用户自行 //123 屏蔽后的客户工程为基础另建。原客户目录、原 APP_KB 和原 BOOT_KB 未改，不使用之前的 Framework 或 BuildOnly 工程。

## 已实现

- 客户 main 只增加一个头文件和一处 `KB_CAN_UpgradeTask();`，保留原初始化、主循环、喂狗、时间差及 //123 屏蔽状态。
- 升级初始化、Timer2时基、CAN轮询、应答分片和复位都集中在该函数；协议重组/CRC复用原 APP_KB 的 boot_core.c/h。
- 客户停机已抽为 `CAN_Upgrade/kb_customer_shutdown.c` 内的空函数 `Customer_ShutdownForUpgrade()`，由客户实现；升级任务不再硬编码PWM/GPIO停机。
- APP只接受 ENTER_BOOT：完整应答发完，再写RAM标记并通过看门狗复位进入BOOT。APP不接受下载/擦写，不新增SCI。
- APP：10MHz晶振、100MHz主频；Timer2独占、1MHz自由计数、不启用Timer2中断。客户Timer0/1不重配。
- BOOT：10MHz晶振、120MHz主频保持原版。新BOOT修改配套分区/引脚，必须与本APP配套使用。
- 客户 canConfig.c/h 原文不变，原邮箱、ID、位时序及500k参数保留。只新增升级RX31/TX32；ID为0x601/0x581、节点1。
- 寄存器头、配套定义/系统支持源、driverlib、Flash API均采用003x；新CMD替换客户旧CMD，启用003x硬件寄存器映射。

## 工程与入口

| 目录 | CCS工程名 | 用途 |
|---|---|---|
| APP_KB | EDrive_CAN_Application_KB | 客户程序 + 单函数返回BOOT入口 |
| BOOT_KB | F280039C_Bootloader_EDrive_KB | 配套驻留BOOT，负责擦写/校验/启动APP |
| Validation | 编译/预处理/哈希/软件模型记录 | 验证证据，不是CCS工程 |

CCS12.7.1，TI C2000 22.6.1.LTS，EABI，Debug。两个工程均可独立导入；各自保存分区头和引脚头，不依赖共享源码文件。

## 引脚选择

默认12/13用于客户现场。30/31仅按用户要求用于开发板测试：客户GPIO30原有FO故障输入，不能在客户现场直接改成CAN。

在本目录打开PowerShell：

```powershell
.\Select-CAN-Pins.ps1 -Pair 30   # 开发板：RX30/TX31
.\Select-CAN-Pins.ps1 -Pair 12   # 客户现场：RX12/TX13
```

脚本只同步两份 `kb_can_board.h` 的 `KB_CAN_PIN_PAIR`。每次切换必须 Rebuild 两个工程，并使用同一引脚选择的BOOT和APP。当前交付源码及 Debug/OUT 已恢复12/13；30/31的编译日志单独保留。

## 实板测试顺序

1. 选择引脚并重新编译两工程，晶振须为10MHz，确认硬件CAN收发器和引脚接线对应。
2. 首次用CCS/JTAG烧录新 `BOOT_KB/Debug/F280039C_Bootloader_EDrive_KB.out`。不要使用旧目录中APP入口为0x090000的BOOT来配本版。
3. 用上位机选择0039、500000、节点1及正确CAN通道，**先把“APP起始地址”填为0x087000，再选择固件**（默认0x090000不匹配，会被拒绝）。选择新 `APP_KB/Debug/EDrive_CAN_Application_KB.out`，通过BOOT完整升级并提交镜像后复位。只用JTAG单独下载APP不会自动生成BOOT有效镜像元数据。
4. APP运行时再发送升级操作的ENTER_BOOT请求，观察ACK和复位进入BOOT，再执行后续下载。APP只处理进入BOOT命令，运行在APP时单独“读取信息”不会由本入口返回信息；读取/下载由BOOT响应。
5. 若无法进入，先在调试器观察 `kb_upgrade_status`，再查引脚、波特率、节点和31/32邮箱占用。状态3表示发现升级邮箱被占用，本函数没有覆盖客户邮箱。

## 验证结论

12/13、30/31两种配置的APP与BOOT均完成CCS全量编译链接，0错误。APP保留21条原客户/缺失目录警告，BOOT保留1条指定code_start入口的警告。APP过去的未映射寄存器段警告已经消除。

实际移植函数与原协议C源码的软件模型15/15通过：最后应答帧完成前不复位、坏CRC/乱序/超时拒绝、发送超时恢复、中断状态保持、占用邮箱不覆盖、Timer2回绕。源码静态/预处理和MAP另已核对。

以上不是实板验收。没有连接开发板烧录，没有验证物理CAN/晶振/Flash时序/复位RAM保持，也未恢复客户缺失电机算法。具体修改、恢复客户代码时的联动点见《移植与修改说明.md》；详细记录见《编译与验证记录.md》。

## 后续空停机回调及上位机联测

停机接口已改为Customer_ShutdownForUpgrade空函数，客户填写文件为APP_KB/CAN_Upgrade/kb_customer_shutdown.c；Task不再硬编码PWM/GPIO停机。空回调版入口模型15/15、上位机Core完整下载软件联测9/9通过；详情见《空停机接口与上位机联测复核.md》。上位机APP起始地址须0x087000。尚未实板验收，也不对缺失客户代码恢复后的零影响作无依据保证。
