# 0039 CAN 升级客户初始化适配说明
本次修改 APP_CAN_ONLY，使升级模块复用客户 CAN 初始化，独立使用 Timer2 计时。已恢复原版 dispatch：有效请求成功应答后直接复位进入 BOOT，不再调用客户安全停机接口。邮箱号与客户系统接口仍需填写。当前为初步移植，未编译、未上板。

## 本次修改范围

原 APP、BOOT 以及 Desktop\kuobo 下的客户文件保持不变。APP_CAN_ONLY 修改前已完整压缩备份，备份 ZIP 位于它的同级目录。旧版编译产物与 PASS 记录已归档到验证记录目录，不能证明本次代码可运行。

| 文件 | 已经修改的内容 |
| --- | --- |
| src/main.c | 改为客户系统初始化、业务后台任务及升级调用模板；移除 LED 和示例心跳。 |
| src/boot_io.c | 删除重复 CAN 模块初始化；新增升级消息对象配置，使用独立 Timer2 1MHz 毫秒时基。 |
| src/upgrade_entry.c | 初始化 Timer2 和协议状态；对象号改用宏；恢复原版成功应答后复位流程。 |
| include/board_config.h | 引脚参数改为 GPIO12 RX、GPIO13 TX；速率暂记 250k；新增待填消息对象号及编译提示。 |
| include/customer_upgrade_port.h | 只保留客户系统初始化与后台任务接口；已删除客户毫秒和安全停机接口声明。 |
| include/boot_io.h 和 upgrade_entry.h | 更新初始化接口及调用要求。 |
| application.cmd | 新增 K10 注释，提示客户链接分区整合；未改原示例内存布局。 |

boot_core.c 和 boot_core.h 与修改前备份逐字节一致。协议校验、CAN 分片格式、请求标记值和复位流程未改变。代码中的 K01 至 K10 标记用于查找后续调整位置。

# 初始化与主循环接入
## 客户 main 中的调用顺序

```text
客户原系统初始化（PLL、GPIO、CAN、定时器、中断）
    ↓
客户 CAN_Config_Init() 完成
    ↓
boot_can_objects_init();
upgrade_entry_init();
    ↓
while (1) 中执行客户后台任务及 upgrade_entry_poll();
```

当前 main.c 用 Customer_SystemInit() 包装客户初始化，并用 Customer_BackgroundTask() 表示客户后台任务。这两个接口尚未实现。正式合并时可以直接沿用客户原 main 和原调用顺序，仅加入两处升级初始化与循环内轮询，不必复制模板 main.c。

## 不再照搬的初始化

upgrade_entry_init() 不再调用旧 boot_ports_init()、boot_timers_init()，现在调用 boot_timer2_init()、bl_init() 和 bl_can_reset()。不复位 CAN、不改业务波特率或引脚、不重配 Timer0/1；仅占用 Timer2。客户初始化完成后调用一次，之后不能再停止、重载或重配 Timer2。

## 消息对象配置位置

boot_can_objects_init() 只创建升级 RX/TX 对象，必须在客户 CAN_Config_Init() 之后执行。之后若业务重新调用 CAN_initModule() 或改写这些对象，将破坏升级通道。若客户把对象配置统一管理，也可将两段 CAN_setupMessageObject 合并进 CAN_Config_Init，只保留一次配置。

## 保留两种写法

客户寄存器代码和升级 driverlib 调用可以并存。需核对目标器件头文件、库版本、寄存器地址和时钟配置相互一致；无需仅为统一风格重写客户初始化。当前 APP 接收仍是轮询方式，不新增 CAN 接收中断。

# 后续调整位置
| 标记 | 定位与后续操作 |
| --- | --- |
| K01 | board_config.h：暂记 BOOT_CAN_BAUD=250000。确认客户实际速率后更新记录并同步 BOOT、上位机；该宏不会改变客户 APP 的寄存器配置。 |
| K02 | board_config.h：BOOT_CAN_RX_OBJ 和 BOOT_CAN_TX_OBJ 目前均为 0。填入客户未使用、互不相同的对象号 1 至 32；接收、发送和 TX 完成掩码会自动跟随宏。未填时 #error 提示，不要仅删提示。 |
| K03 | 客户 PLL、CAN 时钟及 vendor/device/device.h：核对真实 CPU/CAN 时钟，延时常量要匹配。 |
| K04 | main.c 和 customer_upgrade_port.h：接入客户原系统初始化及后台任务。正式合并原 main 时可取消这两个模板包装调用。 |
| K05 | boot_io.c 的 boot_timer2_init 与 boot_millis：独立使用 Timer2，以系统时钟经分频得到 1MHz；确认 Timer2 空闲且 DEVICE_SYSCLK_FREQ 匹配真实 CPU 时钟。不再需要客户毫秒接口。 |
| K06 | upgrade_entry.c 的 dispatch：已恢复原版，有效请求与成功应答后直接调用 boot_request_reset；客户停机回调已删除。 |
| K07 | boot_io.c 的 boot_can_objects_init：确认对象不冲突、标准帧过滤和升级 ID。当前请求 0x601、应答 0x581。 |
| K08 | upgrade_entry.c 的 upgrade_entry_init：初始化 Timer2 和协议状态，在客户设备与升级对象初始化后只调用一次。 |
| K09 | upgrade_entry.c 的 upgrade_entry_poll：确认客户 ISR 或统一收帧程序不会抢读升级 RX 对象；必要时改接客户收帧队列。 |
| K10 | application.cmd 和配套 BOOT：整合应用入口、Flash 范围和 RAM 标记预留，保留客户控制算法、CLA、DMA 等段。 |

修改对象号后，初始化、CAN_readMessageWithID、CAN_sendMessage_updateDLC 和 TX 请求掩码均使用同一组宏，无需分别寻找硬编码 1、2。

# 时基与配套验证
## 独立 Timer2 时基

本次独立使用 CPU Timer2。boot_timer2_init 使能外设时钟，关闭计数器中断，选择 SYSCLK 且时钟源预分频为 1，再将计数器预分频设置为 DEVICE_SYSCLK_FREQ/1000000-1，周期为 0xFFFFFFFF。boot_millis 读取 Timer2，保留原版差值与余数累计换算。Timer0/1 配置保持不变。

can_wait_tx() 的等待上限仍为 500ms。Timer2 是硬件计数器，等待发送时仍可读出经过时间，不依赖客户毫秒中断。确认 Timer2 未被其他代码使用、运行中不改时钟、不重载计数器；两次计时读取间隔须小于一次回绕周期，约 71.6 分钟。

## 时钟与 CAN 引脚

客户 init_eCAN_A(40,5,4,1,2,1) 选择系统时钟作为 CAN 输入。按配置计算，速率为 fCAN/[40×(1+5+4)]：100MHz 对应 250k，120MHz 对应 300k。客户目录没有 PLL 配置，250k 尚待核实。GPIO12 RX、GPIO13 TX 已写入参数；客户 CAN RX 异步采样设置仍需确认，必要时在客户 GPIO 配置中补 GPAQSEL1.bit.GPIO12=3。

## 原版升级复位流程

dispatch 保留原版逻辑：bl_handle 校验请求，count 非零、应答状态为 BL_OK 且 boot_send_reply 成功后，调用 boot_request_reset 写 RAM 升级标记并触发复位。当前没有客户安全停机或禁止重新启动回调。Customer_PrepareForUpgrade 声明和调用均已删除。

## BOOT 和链接文件

本次没有修改 BOOT。配套 BOOT 必须使用客户板 GPIO12/13、确认后的速率和相同升级 ID。客户 APP 入口应为 0x090000，Flash 使用范围为 [0x090000,0x0AF000)，按该配套 BOOT 整合；预留 RAM 字地址 0x3F8 至 0x3FB，启动、自检和数据段不得提前覆盖。

## 继续完成移植所需材料

补充 CAN_Config_Init、CAN 收发处理、PLL 与时钟配置，并确认 Timer2 空闲。然后填写对象号与客户系统接口、整合客户链接文件，再进行编译和上板验证。当前完成源码静态核对和原文件完整性检查，没有执行完整 CCS 编译、烧录或实板 CAN 升级测试。
