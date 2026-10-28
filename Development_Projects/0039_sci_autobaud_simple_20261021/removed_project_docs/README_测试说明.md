# 0039_sci_autobaud 测试例程

- 芯片：进芯 AVP32F0039；CCS工具目标沿用TMS320F280039C。
- 外部20MHz晶振、CPU 120MHz、LSPCLK 30MHz；SCI-A GPIO28/RX、GPIO29/TX，初始9600、8N1。
- DriverLib直接初始化，无SysConfig。只有CPU1_FLASH配置。
- CCS12.7.1、C2000编译器22.6.1.LTS；driverlib头和库已包含，使用工程相对路径，不需要另装C2000Ware或SysConfig。建议解压到英文短路径。

## 测试

1. CCS导入0039_sci_autobaud，编译并将CPU1_FLASH/0039_sci_autobaud.out下载到Flash。核对实际板卡晶振、GPIO、电平、共地和仿真器；上位机TX接0039 RX，上位机RX接0039 TX。
2. 上电时先等待自动波特率锁定，上位机发送ASCII a（0x61）或A（0x41），之后开始持续输出hello，没有换行和NUL。
3. 正常收发约5秒后停止输出，上位机看到hello暂停，再发送a/A；锁定后恢复hello，再计时5秒。
4. 没有发送有效a/A时，TI的SCI_lockAutobaud会一直等待，不超时；等待期间主循环不接收业务数据也不发送hello。

接收数据存入rxBuffer[128]，rxWriteIndex指向下一写入位置，写满后回到0覆盖旧值；rxByteCount为累计数。C28x每个uint16_t数组元素只保存一个有效8位UART字节，不是C字符串。正常收发时收到a也只存数组，不触发校准。校准开始/成功时清RX FIFO，同步字符及紧随的数据不保证保留；先看到hello恢复再发业务数据。

Timer2仅用于5秒轮询，无中断。修改CALIBRATION_TICKS可调整间隔。测试程序不做接收错误恢复、协议握手或高低温容错；不要直接当作产品通信程序。

只在实际完成后记录软件构建结果；实板烧录、Flash冷启动、串口波形以及±10%/高低温适配需要另行测试。改变波特率应在hello暂停的校准窗口进行。

主C文件0039_sci_autobaud.c使用GBK编码，初始化、发送、接收及校准函数内已加入中文注释；CCS资源设置已指定该文件为GBK。
