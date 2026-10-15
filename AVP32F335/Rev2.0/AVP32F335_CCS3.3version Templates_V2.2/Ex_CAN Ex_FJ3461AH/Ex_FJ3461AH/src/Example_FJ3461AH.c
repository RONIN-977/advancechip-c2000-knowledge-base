/******************************************************************************
文件名: 	Example_FJ3461AH.c
作者: 		湖南进芯电子科技有限公司
功能描述:	CPU定时器例程：
            本例实现的功能是：配置CPU定时器0、1、2定时1s，定时完成进入各自的中断，每次进入定时器中断，计数值加1.
创建时间: 	06-25-2019
版本:       V1.0
修订时间:
修订内容:
*******************************************************************************/

#include "AVP33x_Project.h"     // Device Headerfile and Examples Include File
#include "AIP650.h"

Uint32 Systimerticker = 0;
Uint16 timer = 9999;

// CPU定时器的中断服务程序声明
__interrupt void cpu_timer0_isr(void);

void main(void)
{

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

   
// 步骤1：系统初始化。设置PLL、看门狗、使能外设时钟等。
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();
	InitGpio();
	


// 步骤2：初始化GPIO
    // InitGpio();          // Skipped for this example


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
    InitCpuTimers();         // 为本例程的用到的CPU定时器进行初始化

    #if (CPU_FRQ_150MHZ)
    // Configure CPU-Timer 0 to interrupt every 100 usecond:
    // 150MHz CPU Freq, 100 usecond Period

        ConfigCpuTimer(&CpuTimer0, 150, 100000);
    #endif

    #if (CPU_FRQ_120MHZ)
    // Configure CPU-Timer 0 to interrupt every 100 usecond:
    // 120MHz CPU Freq, 100 usecond Period

        ConfigCpuTimer(&CpuTimer0, 120, 100000);

    #endif

   CpuTimer0Regs.TCR.all = 0x4000;  // 使能CPU-Timer0中断，并启动CPU-Timer0


// 步骤5：用户自己的代码

    // 使能CPU定时器0的CPU级中断
    IER |= M_INT1;      // CPU-Timer 0


    // 使能CPU-Timer 0的PIE级中断
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    EINT;               // 使能全局中断INTM
    ERTM;               // 使能全局实时中断DBGM

	DisplayEnable();

    while(1)
	{

	 DataDisplay(timer);
	 
	}
}

__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
	
    Systimerticker++;
	if(Systimerticker>=10)
	{
	    Systimerticker = 0;
	    timer = timer-1;
		if(timer<=0)
		{
           timer = 9999;
		}
	}
	

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


//===========================================================================
// No more.
//===========================================================================
