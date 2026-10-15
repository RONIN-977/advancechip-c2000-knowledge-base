//###########################################################################

//!  该例程通过串口A测试接收以及回传数据
//!  通讯波特率为9600
//!  监测变量
//!  - LoopCount, 发送字节个数
//!  - ErrorCount  错误个数
//!


//###########################################################################

#include "AVP32F069_Project.h"
#include "string.h"

void scia_echoback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);


Uint16 LoopCount;
Uint16 ErrorCount;

void main(void)
{

    Uint16 ReceivedChar;
    char *msg;

    // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
    // 该函数在AVP32F069_SysCtrl.c文件中
   InitSysCtrl();
#ifndef  DEBUG_EN
   memcpy(&RamfuncsRunStart , &RamfuncsLoadStart, (Uint32)RamfuncsLoadSize);
   InitFlash();
#endif
// 第二步:配置GPIO
   InitSciaGpio();
// 第三步: 清除所有中断以及初始化中断向量表
// 先禁止CPU中断
    DINT;
    InitPieCtrl();  //给PIE寄存器赋初值
    IER = 0x0000;//禁止CPU的中断并清楚所有相关的中断标志
    IFR = 0x0000;
    InitPieVectTable();//初始化中断向量表

    LoopCount = 0;
    ErrorCount = 0;

    scia_fifo_init();	   // 初始化 SCI FIFO
    scia_echoback_init();

    msg = "\r\n\n\nHello World!\0";
    scia_msg(msg);

    msg = "\r\nYou will enter a character, and the DSP will echo it back! \n\0";
    scia_msg(msg);

	for(;;)
    {
       msg = "\r\nEnter a character: \0";
       scia_msg(msg);

       // Wait for inc character
       while(SciaRegs.SCIFFRX.bit.RXFFST !=1) { } // wait for XRDY =1 for empty state

       // Get character
       ReceivedChar = SciaRegs.SCIRXBUF.all;

       // Echo character back
       msg = "  You sent: \0";
       scia_msg(msg);
       scia_xmit(ReceivedChar);

       LoopCount++;
    }

}

// Test 1,SCIA  DLB, 8-bit word, baud rate 0x0103, default, 1 STOP bit, no parity
void scia_echoback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE

	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;

	SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 22.5MHz (90 MHz SYSCLK).
    SciaRegs.SCILBAUD    =0x0024;

	SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

// 发送一个字符
void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    SciaRegs.SCITXBUF=a;

}

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

// 初始化SCI FIFO
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x2044;
    SciaRegs.SCIFFCT.all=0x0;

}

//===========================================================================
//===========================================================================

