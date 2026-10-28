# AVP32F0039 SCI 9600 运行中自动波特率例程

## 工程来源与工具

以用户提供的 `V12/sci_ex3_echoback` 为基础，保留TI例程的单C文件结构、注释分段、device启动文件和RAM/FLASH链接布局。原例程不修改。

- CCS 12.7.1，TI C2000 Compiler 22.6.1.LTS，SysConfig 1.23.1。
- 工程已包含 driverlib 头文件/库，以及 SysConfig 所需 C2000Ware 5.04 产品元数据；不用引用发送方电脑的 C2000Ware 安装目录。
- 客户仍须安装上述开发工具，并在 CCS 的 Products 中发现 SysConfig 1.23.1；这不是免安装编译器的软件包。
- 建议解压到英文、无空格短路径，CCS Import Existing CCS Projects 导入本目录。工程名 `sci_ex3_autobaud`。
- 保留4种原配置：CPU1_RAM、CPU1_FLASH、CPU1_LAUNCHXL_RAM、CPU1_LAUNCHXL_FLASH。选择与实际板卡对应的配置和仿真器。
- 沿用原 `device/device.h`：外部20MHz晶振、CPU 120MHz、LSPCLK 30MHz、Flash等待5。换晶振时应成套调整，不只改频率宏。
- SysConfig暂沿用原例程100PZ封装视图（TMS320F280039C作为寄存器/工具目标）。用于进芯0039其他封装时核对实际引脚，不以TI封装视图当作进芯引脚兼容证明。

## 功能与引脚

`c2000.syscfg` 配置 SCI-A，GPIO28 RX、GPIO29 TX，9600 bit/s、8N1、FIFO、轮询，不启用SCI中断。主程序调用 `Board_init()` 初始化，不能另写一套引脚/串口初始化覆盖它。

启动后直接按9600通信，不强制启动同步：

- 持续接收，保存到 `rxBuffer[128]`，写满后索引回0，覆盖旧值。每个uint16_t元素保存1个有效8位UART字节，不按包切分，不自动加字符串结束符。
- `rxWriteIndex` 指向下一个写入位置；`rxByteCount` 是累计字节数。写满后最旧数据位于rxWriteIndex，最新数据在其前一个位置。
- 持续发送 `hellohellohello...`，没有换行、NUL或额外应答。TX FIFO满时让出本轮，不阻塞等待，也不需要先收到数据才发送。
- 主循环必须频繁执行；接收FIFO深度有限，客户加入耗时逻辑后需重新评估丢字节风险。错误恢复会丢弃FIFO待处理字节并累计rxErrorCount。

## 客户只需完善的触发位置

主程序保留明确的调用条件：

```c
condition = SCI_autoBaudCondition();
if(condition && !previousCondition && (autoBaudState == AUTOBAUD_IDLE))
{
    SCI_autoBaudStart();
}
```

客户完善 `SCI_autoBaudCondition()`，只有双方已经约定进入同步窗口才返回true。示例默认读取 `autoBaudRequest`，方便调试：先置0让主循环运行至少一轮，再置1开始一次。条件持续为true不会重复检测；忙碌期间的新请求不排队。

这里采用与 `SCI_lockAutobaud()` 相同的寄存器序列，但拆为非阻塞的开始/服务过程，避免原函数无限等待。它不会主动校准对方MCU的时钟，只改变本机SCI分频。

## 必须遵守的同步时序

1. 客户协议先约定停止对端业务数据；触发条件返回true。
2. 本机停止继续填TX FIFO，等待FIFO和移位寄存器真正发完；此时 autoBaudState=1。
3. 清理旧接收/错误状态，启动自动检测；autoBaudState=2。**对端在此之后才发送 ASCII A（0x41）或a（0x61）**。
4. 检测成功清除同步字节，计数加1并恢复从h开始连续发送hello。对端看到hello恢复后才发送后续业务数据。
5. 从触发起1秒仍未成功，则关闭检测、恢复进入检测前的分频并清FIFO，计超时后恢复hello。超时恢复不代表已匹配对端；客户需重新安排同步。

本例没有额外READY/ACK协议；state变量只在本机可见，不会自动通知对端。客户可用双方约定的时序、额外GPIO等实现协调，不能假设对端天然知道state。

同步窗口内暂停业务收发，因此不能保证此时数据无损。不要把A紧接业务数据连发；成功处理会清RX FIFO，紧随同步字节的数据可能被丢弃。持续发送A的行为也需要客户约束停止时机。

## CCS观察变量

|变量|含义|
|---|---|
|rxBuffer / rxWriteIndex / rxByteCount|循环接收数组、下一写入位置、累计接收数|
|rxErrorCount|接收状态错误或FIFO溢出的恢复次数|
|autoBaudRequest|示例触发开关，0→1触发一次|
|autoBaudState|0正常收发，1等待TX排空，2检测同步字符|
|autoBaudLastResult|0无结果/本次进行中，1成功，2超时|
|autoBaudSuccessCount / autoBaudTimeoutCount|成功/超时累计数|
|autoBaudDivider|初始或最近成功锁定的SCI分频寄存器值|

CPU Timer2 专用于1秒超时，无中断，32位递减计数。不要在其他模块复用/重配Timer2；调试暂停时Timer2停止，超时应以连续运行时间判断。

## A/a与频偏边界

A/a是SCI硬件自动检测指定字符，不能通过替换本例比较常量改成其他字符。10%不是已知固定锁定上限；自动检测匹配实际位速率，但支持的绝对速率、分频范围、边沿质量和同步期间稳定性仍有限制。

本次软件生成和构建结果见交付附带验证说明；没有烧录或实板验证，不能宣称±10%全温、温漂突变、物理串口和Flash冷启动已通过。