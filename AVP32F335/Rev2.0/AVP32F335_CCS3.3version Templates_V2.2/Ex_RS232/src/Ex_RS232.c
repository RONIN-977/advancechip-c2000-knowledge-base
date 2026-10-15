/******************************************************************************
文件名			: Ex_Ex_RS232.c
作者			: 湖南进芯电子科技有限公司
功能描述		: 232通信例程
             	  本例程实现的功能：与上位机进行232通信。将上位机发来的数据回传
                  至上位机。首先，在上位机打开一个串口调试助手；然后给DSP开发板
                  上电，DSP会往上位机发送一串字符要用户发送一个字符；最后用户通
                  过串口调试助手发送一个字符，DSP会将该字符回传到上位机在串口调
                  试助手的接收区显示出来。
             	  SCIA管脚定义：
                    - SCITXDB:GPIO14
                    - SCIRXDB:GPIO15
                  通过232串口工具将SCIA和与上位机连接，通信波特率9600bit/s。

创建时间  		: 06-25-2019
版本      		: V1.0
修订时间 		:
修订内容 		:
*******************************************************************************/

#include "AVP33x_Project.h" // Device Headerfile and Examples Include File

// 函数声明
void scia_echoback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);

//  本例程中用到的全局变量定义
Uint16 LoopCount;
Uint16 ErrorCount;

void main(void)
{
    Uint16 ReceivedChar;
    char *msg;

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

// 步骤1：系统初始化。初始化PLL、看门狗、使能外设时钟等
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


// 步骤2：初始化GPIO
    // InitGpio();          // Skipped for this example
	InitSciaGpio();


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   // 禁止CPU中断
    InitPieCtrl();          // 初始化PIE
    IER = 0x0000;           // 禁止CPU中断
    IFR = 0x0000;           // 清除CPU中断标志
    InitPieVectTable();     // 初始化PIE中断向量表


// 步骤4：初始化所有外设
    // InitPeripherals();   // Not required for this example


// 步骤5：用户自己的代码

    LoopCount = 0;
    ErrorCount = 0;

    scia_fifo_init();	    // 初始化SCI FIFO
    scia_echoback_init();   // 初始化SCI回传

    msg = "\r\n\n\nHello World!\0";
    scia_msg(msg);

    msg = "\r\nYou will enter a character, and the DSP will echo it back! \n\0";
    scia_msg(msg);

	for(;;)
    {
	    msg = "\r\nEnter a character: \0";
	    scia_msg(msg);

	    // 等待SCIA接收到数据
	    while(SciaRegs.SCIFFRX.bit.RXFFST !=1) { }  // wait for XRDY =1 for empty state

	    // 读SCIA接收缓冲区数据时
	    ReceivedChar = SciaRegs.SCIRXBUF.all;

	    // 将接收到的数据回传至上位机
	    msg = "  You sent: \0";
	    scia_msg(msg);
	    scia_xmit(ReceivedChar);

	    LoopCount++;
    }
}

// SCIA初始化函数
void scia_echoback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	SciaRegs.SCICCR.all = 0x0007;       // 1位停止位、无奇偶校验、屏蔽自测模式、数据长度8位
 	SciaRegs.SCICTL1.all = 0x0003;      // 使能SCIA的发送和接收功能，复位SCIA
 	SciaRegs.SCICTL2.all = 0x0003;      // 使能SCIA的接收和发送中断
 	SciaRegs.SCICTL2.bit.TXINTENA = 1;
 	SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
	#if (CPU_FRQ_150MHZ)
 	SciaRegs.SCIHBAUD = 0x0001;   		// 9600 baud @LSPCLK = 37.5MHz.
 	SciaRegs.SCILBAUD = 0x00E7;
	#endif
	#if (CPU_FRQ_100MHZ)
 	SciaRegs.SCIHBAUD = 0x0001;     	// 9600 baud @LSPCLK = 20MHz.
 	SciaRegs.SCILBAUD = 0x0044;
	#endif
 	SciaRegs.SCICTL1.all = 0x0023;      // 使能SCIA的发送和接收功能
}

// SCIA发送一个字符函数
void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    SciaRegs.SCITXBUF=a;
}

// SCIA消息发送函数
void scia_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}

// SCI FIFO初始化函数
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all = 0xE040;      // 使能SCIA的发送FIFO，并清除FIFO的中断标志，禁止发送FIFO中断
    SciaRegs.SCIFFRX.all = 0x204f;      // 使能SCIA的接收FIFO，并清除FIFO的中断标志，禁止接收FIFO中断
    SciaRegs.SCIFFCT.all = 0x0;         // 禁止波特率自动检测，FIFO发送延时为0
}

//===========================================================================
// No more.
//===========================================================================
