/******************************************************************************
文件名			: Example_AVP33xCpuTimerBeep.c
作者			: 湖南进芯电子科技有限公司
功能描述 		: CPU定时器例程：
                  本例实现的功能是：通过定时器来设置蜂鸣器鸣叫的频率。
创建时间      	: 06-25-2019
版本         	: V1.0
修订时间       	:
修订内容      	:
*******************************************************************************/

#include "AVP33x_Project.h"     // Device Headerfile and Examples Include File


__interrupt void cpu_timer0_isr(void);  // CPU定时器0的中断服务程序声明

void main(void)
{
// 步骤1：系统初始化。设置PLL、看门狗、使能外设时钟等。
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


// 步骤2：初始化GPIO
    // InitGpio();          // Skipped for this example
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;    // 将GPIO设置为通用IO引脚
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;     // 设置为输出引脚
    EDIS;


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   // 禁止CPU中断
    InitPieCtrl();          // 初始化PIE
    IER = 0x0000;           // 禁止CPU中断
    IFR = 0x0000;           // 清除CPU中断标志
    InitPieVectTable();     // 初始化PIE中断向量表

    // 本例程中用的中断的中断向量重新映射
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    EDIS;


// 步骤4：初始化所有外设
    // InitPeripherals();   // Not required for this example
    InitCpuTimers();        // 为本例程的用到的CPU定时器进行初始化

    #if (CPU_FRQ_150MHZ)
        ConfigCpuTimer(&CpuTimer0, 150, 1000000);   // 1000000配置定时周期，单位：us。这里定时器周期设置为1s。
    #endif

    #if (CPU_FRQ_120MHZ)
        ConfigCpuTimer(&CpuTimer0, 120, 1000000);
    #endif

   CpuTimer0Regs.TCR.all = 0x4000;  // 使能CPU-Timer0中断，并启动CPU-Timer0


// 步骤5：用户自己的代码

    // 使能CPU定时器0的CPU级中断
    IER |= M_INT1;      // CPU-Timer 0

    // 使能CPU-Timer 0的PIE级中断
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    EINT;               // 使能全局中断INTM
    ERTM;               // 使能全局实时中断DBGM


// 步骤6：空循环 （可选）
    for(;;);
}

__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    GpioDataRegs.GPATOGGLE.bit.GPIO17 = 1;      // IO口翻转

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;     // 响应其他同组中断
}

//===========================================================================
// No more.
//===========================================================================
