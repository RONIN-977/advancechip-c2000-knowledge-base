//###########################################################################
//! 本例程为串口自发自收测试，将开发板上SCIA的TX/RX 短接
//! 在串口助手上可收到00-ff的数据
//###########################################################################

#include "AVP32F069_Project.h"
#include "string.h"

void scia_loopback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void error();
__interrupt void scia_rx_isr(void);
__interrupt void scia_tx_isr(void);


Uint16 LoopCount;
Uint16 ErrorCount;

void main(void)
{
    Uint16 SendChar;
    Uint16 ReceivedChar;

// 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
// 该函数在AVP32F069_SysCtrl.c文件中
	InitSysCtrl();
#ifndef  DEBUG_EN
   memcpy(&RamfuncsRunStart , &RamfuncsLoadStart, (Uint32)RamfuncsLoadSize);
   InitFlash();
#endif
 // 第二步:配置GPIO
   InitSciGpio();

   // 第三步: 清除所有中断以及初始化中断向量表
   // 先禁止CPU中断
    DINT;
    InitPieCtrl();  //给PIE寄存器赋初值
    IER = 0x0000;//禁止CPU的中断并清楚所有相关的中断标志
    IFR = 0x0000;
    InitPieVectTable();//初始化中断向量表
    EnableInterrupts(); //使能中断

    LoopCount = 0;
    ErrorCount = 0;

    scia_fifo_init();	   // 初始化 SCI FIFO
    scia_loopback_init();

    // 发送起始字符0
    SendChar = 0;

// 发送00-ff数据

	for(;;)
    {
       scia_xmit(SendChar);
       while(SciaRegs.SCIFFRX.bit.RXFFST !=1) { } // 等待RRDY/RXFFST =1

       //判断接收到的数据
       ReceivedChar = SciaRegs.SCIRXBUF.all;
       if(ReceivedChar != SendChar) error();

       // 下一个数据加1
       SendChar++;
       // Limit the character to 8-bits
       SendChar &= 0x00FF;
       LoopCount++;
    }

}



void error()
{

      ErrorCount++;
//   __asm("     ESTOP0");  // Uncomment to stop the test here
//    for (;;);

}

// 波特率：9600 停止位：1  数据位：8 校验位 无
void scia_loopback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	SciaRegs.SCICCR.all =0x0007;   // 1停止位 无校验


	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all =0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
    SciaRegs.SCIHBAUD    =0x0001;
    SciaRegs.SCILBAUD    =0x0024;
	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
}

//发送一个数据
void scia_xmit(int a)
{
    SciaRegs.SCITXBUF=a;
}

// 初始化 SCI FIFO
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x2044;
    SciaRegs.SCIFFCT.all=0x0;

}






//===========================================================================

//===========================================================================

