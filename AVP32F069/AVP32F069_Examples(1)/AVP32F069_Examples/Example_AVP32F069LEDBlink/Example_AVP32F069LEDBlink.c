//###########################################################################
/*本实验演示了CPU定时器的配置和使用方法，AVP32F069有3个CPU定时器，
 * 分别是 Timer0，Timer1和Timer2，三个定时器相互独立。 本示例中定时器的最小定时单位为 1uS，
 * 在Timer0的定时中断中让LED2以500ms频率时间闪烁，定时时间不同，闪烁频率就不同，用户可以使
 * 用示波器观察 LED 的闪烁频率是否和定时频率一致。
*/
//###########################################################################
//#define DEBUG_EN  1


#include "AVP32F069_Project.h"
#include "string.h"

__interrupt void cpu_timer0_isr(void);

void main(void)
{

   // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
   // 该函数在AVP32F069_SysCtrl.c文件中
   InitSysCtrl();
#ifndef  DEBUG_EN
   memcpy(&RamfuncsRunStart , &RamfuncsLoadStart, (Uint32)RamfuncsLoadSize);
   InitFlash();
#endif
   // 第二步: 清除所有中断以及初始化中断向量表
   // 先禁止CPU中断
   DINT;
   InitPieCtrl();
 //禁止CPU的中断并清除所有相关的中断标志
   IER = 0x0000;
   IFR = 0x0000;
   //初始化中断向量表
   InitPieVectTable();
 // 第三步：设置中断服务程序入口
   EALLOW;  // 使能寄存器写保护
   PieVectTable.TINT0 = &cpu_timer0_isr; //设置timer0中断服务程序入口
   EDIS;    // 关闭寄存器写保护

// 第四步： 初始化外设
   InitCpuTimers();   // 该例程中只初始化了 Cpu 计时器
// 配置 CPU-Timer 0 每500ms中断一次
// 90MHz CPU主频  500ms周期 (单位us)
   ConfigCpuTimer(&CpuTimer0, 90, 500000);
   CpuTimer0Regs.TCR.bit.TSS = 0; //启动定时器

// 第五步：. 使能中断:

// 配置 GPIO50为输出
   EALLOW;
   GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;
   EDIS;


   IER |= M_INT1;//使能中断组1 -cup timer0所在中断组
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1; //使能timer0对应的中断组1中的中断7


   EINT;   // 使能全局中断
   ERTM;   // 使能实时中断

// 第六步：空循环
   for(;;);
}


__interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   GpioDataRegs.GPBTOGGLE.bit.GPIO50= 1; // 每间隔500ms翻转GPIO50

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;//确认收到中断 并响应同组 的其他中断程序
}


//===========================================================================

//===========================================================================
