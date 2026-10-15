//###########################################################################
/*
 在本实验中，通过操作DSP的GPIO，用了三种不同的方式来改变GPIO的状态来实现LED的闪烁，
 针对DSP控制的GPIO寄存器进行操作，它们分别是：GpioCtrlRegs寄存器；GpioDataRegs寄存器。
以下示例说明取自工程演示的Example_AVP32F069GpioToggle.c中。
工程示例说明：
1.本示例演示GPIO的操作，LED1/2/3/4每隔500ms闪烁一次；
2.本示例中使用了3种方式来实现GPIO状态翻转，包含3个不同的例程，通过宏定义来选择执行例程，
每个例程通过不同方式切换GPIO状态，第一个通过写值到GPIO DATA 寄存器，第二个例程通过写值到SET/CLEAR寄存器，
第三个通过写值到TOGGLE寄存器。更多 GPIO方面的知识请参考GPIO阐述部分的数据手册。
3.代码执行流程:
3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过19倍频后再2分频最终得到90MHZ的系统时钟
3.2 然后是PIE中断向量表的初始化
3.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作；
3.4 最后是一个循环实现LED状态翻转，通过宏定义来选择执行哪个例程。
4.观察实验现象
4.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到LED1/2/3/4以一定的频率闪烁；

*/
//###########################################################################

#include "AVP32F069_Project.h"
#include "string.h"
// 选择运行例程，只允许设置一个例程为1，其他设置为0
#define EXAMPLE1 1  // 使用DATA 寄存器翻转 I/O
#define EXAMPLE2 0  // 使用SET/CLEAR寄存器翻转 I/O
#define EXAMPLE3 0 // 使用 TOGGLE寄存器翻转 I/O
//#define DEBUG_EN  1

void delay_loop(void);
void Gpio_select(void);
void Gpio_example1(void);
void Gpio_example2(void);
void Gpio_example3(void);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
       *DestAddr++ = *SourceAddr++;
    }
    return;
}

void main(void)
{

  // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
  // 该函数在AVP32F069_SysCtrl.c文件中
#ifndef  DEBUG_EN
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
   InitFlash();
#endif
   InitSysCtrl();

// 第二步:配置GPIO
   Gpio_select();

// 第三步: 清除所有中断以及初始化中断向量表
// 先禁止CPU中断
   DINT;

   InitPieCtrl();  //给PIE寄存器赋初值
   IER = 0x0000;//禁止CPU的中断并清楚所有相关的中断标志
   IFR = 0x0000;
   InitPieVectTable();//初始化中断向量表


#if EXAMPLE1
    // 该例程使用DATA 寄存器翻转 I/O
    Gpio_example1();

#endif  // - EXAMPLE1

#if EXAMPLE2

    // 该例程使用SET/CLEAR 寄存器翻转 I/O
    Gpio_example2();
    
#endif

#if EXAMPLE3

    //该例程使用TOGGLE 寄存器翻转 I/O
    Gpio_example3();
    
#endif

}

/*void delay_loop()
{
    short      i;
    for (i = 0; i < 1000; i++) {}
}*/

void Gpio_example1(void)
{ 
   // Example 1:
   // 使用DATA 寄存器翻转 I/O

   for(;;)
   {   
       GpioDataRegs.GPADAT.bit.GPIO25 = 1; //led1 off
       GpioDataRegs.GPBDAT.bit.GPIO50 = 1; //led2 off
       GpioDataRegs.GPADAT.bit.GPIO12 = 1; //led3 off
       GpioDataRegs.GPBDAT.bit.GPIO51 = 1; //led4 off
       DELAY_US(500000L);
       GpioDataRegs.GPADAT.bit.GPIO25 = 0; //led1 on
       GpioDataRegs.GPBDAT.bit.GPIO50 = 0; //led2 on
       GpioDataRegs.GPADAT.bit.GPIO12 = 0; //led3 on
       GpioDataRegs.GPBDAT.bit.GPIO51 = 0; //led4 on
       DELAY_US(500000L);

    }
}

void Gpio_example2(void)
{ 
   // Example 2:
   // 使用 SET/CLEAR  寄存器翻转 I/O
   for(;;)
   {    
   

        GpioDataRegs.GPASET.bit.GPIO25 = 1;
        GpioDataRegs.GPASET.bit.GPIO12 = 1;
        GpioDataRegs.GPBSET.bit.GPIO50 = 1;
        GpioDataRegs.GPBSET.bit.GPIO51 = 1;
        DELAY_US(500000L);
        GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;
        GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
        GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;
        GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;

        DELAY_US(500000L);
   
    }
}

void Gpio_example3(void)
{ 
   // Example 2:
   // 使用TOGGLE 寄存器翻转 I/O

    GpioDataRegs.GPASET.bit.GPIO25 = 1;
    GpioDataRegs.GPASET.bit.GPIO12 = 1;
    GpioDataRegs.GPBSET.bit.GPIO50 = 1;
    GpioDataRegs.GPBSET.bit.GPIO51 = 1;
   // 使用TOGGLE寄存器反转引脚状态
   // 置1翻转
   // 置0则不翻转

   for(;;)
   {    
        GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1;
        GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;
        GpioDataRegs.GPBTOGGLE.bit.GPIO50 = 1;
        GpioDataRegs.GPBTOGGLE.bit.GPIO51 = 1;
        DELAY_US(500000L);
    }
}

void Gpio_select(void)
{

   
    EALLOW;
    /*GpioCtrlRegs.GPAMUX1.all = 0x00000000;
    GpioCtrlRegs.GPAMUX2.all = 0x00000000;
    GpioCtrlRegs.GPBMUX1.all = 0x00000000;
    GpioCtrlRegs.GPBMUX2.all = 0x00000000;
    GpioCtrlRegs.GPADIR.all = 0xCFFFFFFF;   // All outputs
   // GpioCtrlRegs.GPBDIR.all = 0x0000000F;   // All outputs
    GpioCtrlRegs.GPBDIR.all = 0x7FC1FFF;*/
    //管脚复用设置为普通IO
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0; //led1
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0; //led2
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  //led3
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0; //led4
    //设置为输出模式
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;

    EDIS;
     
}     
//===========================================================================
// 2022.06.07 by  gxq
//===========================================================================

