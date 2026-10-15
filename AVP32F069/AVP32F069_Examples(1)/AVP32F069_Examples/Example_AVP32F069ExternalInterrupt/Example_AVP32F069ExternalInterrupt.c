//###########################################################################
//工程示例说明
//1.本示例演示AVP32F069的外中断配置和使用方法。AVP32F069一共有3个外中断，这三个外中断的
// 的中断触发源可配置为GPIO0-GPIO31之间的任意一个；本示例中，将GPIO20作为XINT1的中断触发沿,
// GPIO21作为XINT2的中断触发沿，GPIO20/21与开发板上的按键K1\K2相连接，如果按键K1\K2按下，
// 一个下降沿将触发进入中断xint1_isr\xint2_isr，在中断中，我们将让LED1/LED2的状态进行翻转输出。
//2.外中断XINT可以配置为上升沿触发，下降沿触，双边触发(上升沿和下降沿都触发)。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过19倍频后再2分频最终得到90MHZ的系统时钟
//  3.3 然后是PIE中断向量表的初始化
//  3.2 接下来是一些外设的初的初始化,如：LED初始化，外中断初始化
//  3.4 最后是一个for循环,这里我们什么都不做，LED的状态反正将在外中断执行程序中实现。
//4.其他说明
//  4.1 注意XINT1-XINT3只能分配从GPIO0-GPIO31之间的引脚，如果用户自行设计硬件时，欲使用外中断
//      功能的引脚必须安排在这之间。
//5.观察实验现象
//  5.1 将仿真器与开发板连接后,编译程序并下载,点击运行;
//  5.2 按下K1\K2后可以看到LED1\LED2的状态会翻转一次，若之前是亮的，那么会熄，反之亦然。
//  5.3 用户可以更改DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。
//    2022/06/16 by gxq
//###########################################################################

#include "AVP32F069_Project.h"
#include "string.h"

__interrupt void xint1_isr(void);
__interrupt void xint2_isr(void);

// Global variables for this example
volatile Uint32 Xint1Count;
volatile Uint32 Xint2Count;
Uint32 LoopCount;

#define DELAY (CPU_RATE/1000*6*510)  //Qual period at 6 samples

void main(void)
{


   // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
   InitSysCtrl();
#ifndef  DEBUG_EN
   memcpy(&RamfuncsRunStart , &RamfuncsLoadStart, (Uint32)RamfuncsLoadSize);
   InitFlash();
#endif
   // 第二步: 清除所有中断标志并初始化PIE中断向量表
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   EALLOW;
   PieVectTable.XINT1 = &xint1_isr; //设置中断服务程序入口
   PieVectTable.XINT2 = &xint2_isr;
   EDIS;


// 第三步.配置gpio:



// GPIO20 and GPIO21 作为外部中断输入
   EALLOW;
   GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;         // GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;          // 设置为输入
   GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;
   GpioCtrlRegs.GPAPUD.bit.GPIO20 =1;           //由于开发板外置了上拉电阻，禁用DSP内部上拉

   GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;         // GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;          // 设置为输入

   GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0;
   GpioCtrlRegs.GPAPUD.bit.GPIO21 =1;           //由于开发板外置了上拉电阻，禁用DSP内部上拉
   EDIS;

   //设置GPIO25/GPIO50为输出
    EALLOW;
    GpioDataRegs.GPASET.bit.GPIO25 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO25= 0;        // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;        // output
    GpioDataRegs.GPADAT.bit.GPIO25 = 0;        //led1 on

    GpioDataRegs.GPBCLEAR.bit.GPIO50= 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO50= 0;       // GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO50= 1;        // output
    GpioDataRegs.GPBDAT.bit.GPIO50 = 0;       //led2 on
    EDIS;



// GPIO0 为 XINT1, GPIO1 为XINT2
   EALLOW;
   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 20;
   GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 21;
   EDIS;

// 配置 XINT1、 XINT2
   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // 下降沿中断
   XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // 下降沿中断

// 使能 XINT1、 and XINT2
   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // 使能 XINT1
   XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // 使能 XINT2

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // 使能外设中断模块
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // 使能外设中断组1的INT4
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // 使能外设中断组1的INT5
    IER |= M_INT1;                              // 使能中断组1
    EINT;                                       // 使能全局中断
    ERTM;                                       // 使能实时中断

   for(;;)
   {

   }


}

//外部中断1服务程序
__interrupt void xint1_isr(void)
{
    GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1; //翻转LED1
	Xint1Count++;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //响应同组的其他中断
}
//外部中断2服务程序
__interrupt void xint2_isr(void)
{
    GpioDataRegs.GPBTOGGLE.bit.GPIO50 = 1; //翻转LED2
	Xint2Count++;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;//响应同组的其他中断
}

//===========================================================================

//===========================================================================
