//###########################################################################
//工程演示说明
//1.本示例演示AVP32F069看门狗的基本使用方法,包括看门狗溢出后,软件中断和软件复位;
//2.本示例中正常情况下会定时喂狗,按键K1按下,执行长时间延时函数(共计约1s),不喂狗,AVP32F069看门狗将会产生溢出,
//  看门狗设置的溢出时间为839ms。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过18倍频后再2分频最终得到90MHZ的系统时钟;
//  3.3 然后是PIE中断向量表的初始化;
//  3.2 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个for循环, 对看门狗进行喂狗，还有按键扫描，当K1按下则不喂狗，LED慢闪。
// 4.观察实验现象
//4.1 将仿真器与开发板连接后，编译程序并下载，点击运行，LED1闪烁10次指示程序开始运行，后程序循环内运行，LED2一直闪烁；
//4.2  按键K1按下，LED2由灭变亮，AVP32F069复位，LED1继续闪烁，LED2恢复闪烁，
//###########################################################################

#include "AVP32F069_Project.h"
#include "string.h"

__interrupt void wakeint_isr(void);
// Global variables for this example
Uint32 WakeCount;
Uint32 LoopCount;
void (*run) (void);//PC跳转函数
#ifndef  DEBUG_EN
#define ENTRYPOINT 0x003f7ff6
#else
#define ENTRYPOINT 0x00
#endif

void main(void)
{


    // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
    InitSysCtrl();
    // 这里根据宏定义判断是使用哪种模式,如果没有定义DEBUG_EN则什么都不做,
    // 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
    // RAM里面运行的代码拷贝到flash运行
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

   // 第三步：初始化其他使用的外设
    EALLOW;
    //LED2/LED2初始化 设置为输出状态
    GpioDataRegs.GPASET.bit.GPIO25 = 1;         // Load the output latch
    GpioCtrlRegs.GPAMUX2.bit.GPIO25= 0;        // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;        // output
    GpioDataRegs.GPADAT.bit.GPIO25 = 0;        //led1 on
    GpioDataRegs.GPBCLEAR.bit.GPIO50= 1;       // Load the output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO50= 0;        // GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO50= 1;         // output
    GpioDataRegs.GPBDAT.bit.GPIO50 = 1;        //led2 off
    //K1 按键初始化 设置为输入
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;          // input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;        // XINT1 Synch to SYSCLKOUT only
    GpioCtrlRegs.GPAPUD.bit.GPIO20 =1;          //由于开发板外置了上拉电阻，禁用DSP内部上拉
    EDIS;
    //设置中断服务程序入口
    EALLOW;
    PieVectTable.WAKEINT = &wakeint_isr;
    EDIS;

    // 清除计计数器
   WakeCount = 0; // 中断计算器
   LoopCount = 0; // 程序循环计数器


   EALLOW;
   SysCtrlRegs.SCSR = BIT1; // 采用WDINT软件中断  WDENINT[1]=1，
   EDIS;

   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   //使能PID模块
   PieCtrlRegs.PIEIER1.bit.INTx8 = 1;   //使能中断组1的INT8
   IER |= M_INT1;                       // 使能中断组1
   EINT;                                // 使能全局中断

   //上电复位后运行指示灯  LED1闪烁10次
   int i;
   for(i=0;i<10;i++)
  {
      GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1;
      ServiceDog();
      DELAY_US(100000L);;//100MS
  }


   EALLOW;
   //设置看门狗溢出时间
   //WDPS[2:0] = 111 ， WDCLK = OSCCLK/512/64， (10000000/512/64/256)Hz ，即看门狗溢出时间为 839ms
   //看门狗选用10M时钟，固定512分频后，再根据 WDPS[2:0] 选择 分频数，这里再分配 64
   if(SysCtrlRegs.WDCR & 0x0080)//判断 WDFLAG 位，见P105 WDCR寄存器描述
  {
      //WDFLAG[7] 写1清除
      //WDCHK[5:3],固定写 b'101
      //WDPS[2:0] = b'1111
      //WDCR一次写入，否则可能在写期间引起复位
      SysCtrlRegs.WDCR = 0x00AF;
  }
  else
  {
      SysCtrlRegs.WDCR = 0x002F;//没有 WDFLAG ，故不清除WDFLAG
  }

   EDIS;
  //循环
   for(;;)
   {
      LoopCount++;
      if(LoopCount%100000 ==0)
      {
          GpioDataRegs.GPBTOGGLE.bit.GPIO50 = 1;
      }
       if(GpioDataRegs.GPADAT.bit.GPIO20 == 0) //按键K1按下时不喂狗 进入延时程序
       {
           DELAY_US(1000000L);;//1S
           GpioDataRegs.GPBTOGGLE.bit.GPIO50 = 1;
           DELAY_US(1000000L);;//1S

       }
       else //按键未按下时正常喂狗
       {
           ServiceDog();
       }
   }


}



__interrupt void wakeint_isr(void)
{
	WakeCount++;
    run = ( void(*)() ) ENTRYPOINT ;
    (*run)();
	// Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//===========================================================================
//2022.06.20 by gxq
//===========================================================================
