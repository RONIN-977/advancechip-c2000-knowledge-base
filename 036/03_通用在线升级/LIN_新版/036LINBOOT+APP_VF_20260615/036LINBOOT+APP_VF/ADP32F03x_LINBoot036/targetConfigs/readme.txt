Version Information
20250324
1: JumpApp_DoAction 函数修改，时间5S太长不符合实际产品需要，改成400MS
ReveiveBufferFrame(void)
函数中对RemoteModbus.nAppJumpFlag = 0;的操作提前，擦除FLASH 需要时间。
该时间太长，会影响实际应用
该时间太短，可能会导致APP运行时候调转到BOOT后，在没有执行到RemoteModbus.nAppJumpFlag = 0;时候计数到对应值调后又调转APP。


2：JumpApp_DoAction 函数修执行跳转前，DINT，关闭中断避免意外发生
3：原来版本可能存在上位机烧录时序错误导致没有烧录正确程序
在LINa_LEVEL_isr 函数中，增加RemoteModbus.nLinTxData0 = 0;RemoteModbus.nLinTxData1 = 0;
同时上位机也做了响应修改（V1.01)
4: 使用60MHZ，内部时钟，12倍频率，2分频率，可能是036 样品问题，时钟PLL 不稳定。发现的问题。
5: 删除原来一些IO 反转测试，注释部分做了一些调整...
6: EnableDrive 和DisableDrive 里面GPIO0 GPIO1 改成GPIO6 7

20260408
LinBoot
版本号：V1.01
使用内部晶振  100Mhz主频  Flash翻页等待参数335
lin通信使用自动波特率波特率
1. 在原有LinBoot基础上，调整了lin通信报错时将rxbuffer归0.
2. 新增0x72数据段的数据检验比对
对应上位机版本 拓邦1.0.5