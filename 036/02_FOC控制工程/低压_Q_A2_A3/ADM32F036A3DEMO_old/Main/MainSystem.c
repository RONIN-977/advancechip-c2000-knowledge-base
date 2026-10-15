
/****************************************************************
文件功能：变频驱动主函数
文件版本：V1.0
作者        ：徐中领
最新更新：2024-10-01
****************************************************************/

#include "MainInclude.h"
#include "MotorInclude.h"
#include "SPIcomm.h"

#pragma DATA_SECTION(Bootflag, "BOOTFLAG");
const unsigned int Bootflag = 0xAA;
void CPU_TimerInit(void);
interrupt void Timer1_IsrHandler(void);
//#define DPS_ACTIVE_TIME_TEST    1
#ifdef DPS_ACTIVE_TIME_TEST
long ADCData[10];
#endif
/***************************************************************
-------------------------主程序部分-----------------------------
****************************************************************/
void main(void)
{
	Ulong m_BaseTime,m_DetaTime;
	Uint  Level500usCount;

   	InitSysCtrl();						// Step 1. Initialize System Control
   	InitInterrupt();					// Step 2. Initialize Interrupt service program:
   	InitPeripherals(); 					// Step 3. Initialize all the Device Peripherals:
//   	InitForMotorApp();					// Step 4. User specific code
//   	InitForFunctionApp();
//   	CPU_TimerInit();
//	//EnableDog();
//	SetInterruptEnable();				// Step 5. enable interrupts:
//	EINT;
//   	ERTM;
//
//   	Level500usCount = 0;
//	m_BaseTime = GetTime();
	EALLOW;
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
	GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
	GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;

    EDIS;

  	while(1)							// Step 6. User Application function:
   	{
//		m_DetaTime = m_BaseTime - GetTime();
//		if(m_DetaTime >= C_TIME_05MS)
//        {
//		    Level500usCount ++;
//			m_BaseTime -= C_TIME_05MS;
//
// 			KickDog();
// 			SystemLeve05msMotor();
// 			SystemLeve05msFunction();
//
//			if((Level500usCount & 0x03) == 0)
//			{
//			    gDSPActiveTime.Motor2MsBase = GetTime();
//				SystemLeve2msMotor();
//				gDSPActiveTime.Motor2Ms = gDSPActiveTime.Motor2MsBase - GetTime();
//#ifdef DPS_ACTIVE_TIME_TEST
//ADCData[0]++;
//if (ADCData[0] == 500) {
//    ADCData[0] = 0;
//    ADCData[1]++;
//}
//#endif
//			}
//			if((Level500usCount & 0x03) == 2)
//			{
//// GpioDataRegs.GPASET.bit.GPIO28  = 1;
//			    gDSPActiveTime.Function2MsBase = GetTime();
//				SystemLeve2msFunction();
//				gDSPActiveTime.Function2Ms = gDSPActiveTime.Function2MsBase - GetTime();
//// GpioDataRegs.GPACLEAR.bit.GPIO28  = 1;
//			}
//        }
//
//		SystemLeve0msMotor();
//		SystemLeve0msFunction();
//
//#ifdef DPS_ACTIVE_TIME_TEST
//		if (gDSPActiveTime.Motor2Ms > C_TIME_2MS) {
//		    gDSPActiveTime.Motor2msOverRunCnt++;
//		    if (gDSPActiveTime.Motor2msOverRunCnt > 10) {
//		        gDSPActiveTime.Motor2msOverRunFlag = 1;
//		    }
//		}
//
//        if (gDSPActiveTime.Function2Ms > C_TIME_2MS) {
//            gDSPActiveTime.Fun2msOverRunCnt++;
//            if (gDSPActiveTime.Fun2msOverRunCnt > 10) {
//                gDSPActiveTime.Fun2msOverRunFlag = 1;
//            }
//        }
//#endif


   	}
}



/***************************************************************
-----------------------中断程序部分-----------------------------
****************************************************************/

/***************************************************************
	EPWM的周期中断
    1: 单电阻FOC，可以屏蔽掉该中断
    AdcRegs.INTSEL1N2.all = 0x0020;// SOC0通道转换完成产生中断ADCINT1
    AdcRegs.INTSEL1N2.all = 0x0022;// SOC2通道转换完成产生中断ADCINT1
    AdcRegs.INTSEL1N2.all = 0x0000;// ADCINT1 中断屏蔽
****************************************************************/
interrupt void EndOfADCISR(void)
{
    EALLOW;
    ADC_CLEAR_INT_FLAG;
    EDIS;

    // Add Code here......

    EALLOW;
    ADC_RESET_SEQUENCE;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    EDIS;
} 

/***************************************************************
    取代EPWM的ADC 中断，使用下溢中断
    1：清除中断标志，可以使下一个中断进来
    2：中断嵌套，设置可以被其他中断打断（比如SCI 等执行时间非常短的中断）
    3：执行FOC 算法
    4：关中断
    5：Acknowledge this interrupt

    2&4 是可以选择项目，供设计师参考！！！
****************************************************************/
interrupt void ZeroOfEPWMISR(void)
{
//GpioDataRegs.GPASET.bit.GPIO16  = 1;
//    EALLOW;
   	EPwm2Regs.ETCLR.bit.INT = 1;        // ETCLR 无需EALLOW 保护...
//   	EDIS;

	EINT;
#ifdef DPS_ACTIVE_TIME_TEST
	gDSPActiveTime.PWMIntBase = GetTime();
ADCData[4]++;
if (ADCData[4] == 16000) {
    ADCData[4] = 0;
    ADCData[5]++;
}
#endif

    ADCOverInterrupt();
    //SPISendData();

#ifdef DPS_ACTIVE_TIME_TEST
    gDSPActiveTime.PWMInt = gDSPActiveTime.PWMIntBase - GetTime();
#endif
	DINT;

//GpioDataRegs.GPACLEAR.bit.GPIO16  = 1;
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge this interrupt
}

/***************************************************************
    EPWM的过流中断
         对硬件过流信号处理：

    注意：
    逻辑是：先封锁（硬件已经完成，大概200NS），然后进该中断（执行报错，清除标志位等），
    也可以屏蔽此中断，在外面采用查询的方式，不影响保护
****************************************************************/
interrupt void OneShotTZOfEPWMISR(void)
{
	HardWareErrorDeal();					// 处理硬件故障－电机控制模块处理
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	// Acknowledge this interrupt
}

static void CpuTimer_IntHandlerConfig(void)
{
    EALLOW;
    PieVectTable.TINT1 = &Timer1_IsrHandler;
    EDIS;
}

static void CpuTimer_CpuIntEn(void)
{
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;      // Enable the PIE block

    // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
    // which is connected to CPU-Timer 1, and CPU int 14, which is connected
    // to CPU-Timer 2:
    IER |= M_INT13;

    // Enable TINT0 in the PIE: Group 1 interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM                                    //使能全局中断
}

void CPU_TimerInit(void)
{
    CpuTimer_IntHandlerConfig();
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer1, 100, 500000);
    //本示例中我们使能并启动了三个定时器(Timer0-2),如果不想用某个定时器,
    //注释掉对应的使能代码即可
    CpuTimer1Regs.TCR.bit.TSS = 0;      // 启动定时器(TSS = 0)
    CpuTimer_CpuIntEn();
}

interrupt void Timer1_IsrHandler(void)
{
    if(CpuTimer1.InterruptCount <= 8)
    {
        EALLOW;
        // 配置GPIO15为XINT1输入
        XIntruptRegs.XINT1CR.bit.ENABLE = 0; // 禁止XINT1中断

        // 1. GPIO14 设置为高阻输入（接收模式）
        GpioCtrlRegs.GPADIR.bit.GPIO14 = 0; // 输入模式
        //GpioDataRegs.GPASET.bit.GPIO14 = 1;
        GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
        GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1; // 禁用上拉（高阻）

        // 2. GPIO15 配置为 XINT1 输入（已有）
        GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;
        GpioCtrlRegs.GPADIR.bit.GPIO15 = 0; // 输入模式
        GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0; // 使能上拉（高阻）

        GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 15;
        XIntruptRegs.XINT1CR.bit.POLARITY = 3;
        GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 2;//0;   // 仅与SYSCLKOUT同步

        XIntruptRegs.XINT1CR.bit.ENABLE = 1;
        GpioDataRegs.GPASET.bit.GPIO9 = 1; // GPIO9使能

        EDIS;

    }
    else if(CpuTimer1.InterruptCount == 9)
    {
        EALLOW;
        // 1. 禁用 GPIO15 中断
        XIntruptRegs.XINT1CR.bit.ENABLE = 0; // 禁用XINT1中断

        // 2. GPIO15 设置为浮空输入
        GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;// GPIO
        GpioCtrlRegs.GPADIR.bit.GPIO15 = 0; // Input
        GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1; // 禁用上拉（浮空）

        // 3. GPIO14 设置为输出模式
        GpioCtrlRegs.GPADIR.bit.GPIO14 = 1; // 改为输出

        GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
        GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0; // 使能上拉
        GpioDataRegs.GPACLEAR.bit.GPIO14 = 1; // 设输出锁存器

        GpioDataRegs.GPASET.bit.GPIO9 = 1; // GPIO9使能

        EDIS;

    }

    CpuTimer1.InterruptCount++;
    if(CpuTimer1.InterruptCount == 10)
    {
        CpuTimer1.InterruptCount = 0;
    }

    PieCtrlRegs.PIEACK.bit.ACK1 = 1;

}

