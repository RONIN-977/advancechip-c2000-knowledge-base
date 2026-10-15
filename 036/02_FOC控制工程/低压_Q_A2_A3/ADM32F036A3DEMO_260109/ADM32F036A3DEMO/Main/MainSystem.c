
/****************************************************************
文件功能：变频驱动主函数
文件版本：V1.0
作者
最新更新：
****************************************************************/

#include "MainInclude.h"
#include "MotorInclude.h"
#include "Parameter.h"
#include "SPIcomm.h"

#pragma DATA_SECTION(Bootflag, "BOOTFLAG");
const unsigned int Bootflag = 0xAA;

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
   	InitForMotorApp();					// Step 4. User specific code
   	InitForFunctionApp();
	EnableDog();
	SetInterruptEnable();				// Step 5. enable interrupts:
	EINT;
   	ERTM;
   	Level500usCount = 0;
	m_BaseTime = GetTime();
//GpioDataRegs.GPATOGGLE.bit.GPIO19  = 1;
  	while(1)							// Step 6. User Application function:
   	{
		m_DetaTime = m_BaseTime - GetTime();
		if(m_DetaTime >= C_TIME_05MS)
        {
		    Level500usCount ++;
			m_BaseTime -= C_TIME_05MS;

 			KickDog();
 			SystemLeve05msMotor();
 			SystemLeve05msFunction();
//GpioDataRegs.GPBTOGGLE.bit.GPIO34  = 1;

			if((Level500usCount & 0x03) == 0)
			{
			    gDSPActiveTime.Motor2MsBase = GetTime();
				SystemLeve2msMotor();
				gDSPActiveTime.Motor2Ms = gDSPActiveTime.Motor2MsBase - GetTime();
#ifdef DPS_ACTIVE_TIME_TEST
ADCData[0]++;
if (ADCData[0] == 500) {
    ADCData[0] = 0;
    ADCData[1]++;
}
#endif
			}
			if((Level500usCount & 0x03) == 2)
			{
//GpioDataRegs.GPBSET.bit.GPIO34  = 1;
			    gDSPActiveTime.Function2MsBase = GetTime();
				SystemLeve2msFunction();
				gDSPActiveTime.Function2Ms = gDSPActiveTime.Function2MsBase - GetTime();
//GpioDataRegs.GPBCLEAR.bit.GPIO34  = 1;
			}
        }

		SystemLeve0msMotor();
		SystemLeve0msFunction();

#ifdef DPS_ACTIVE_TIME_TEST
		if (gDSPActiveTime.Motor2Ms > C_TIME_2MS) {
		    gDSPActiveTime.Motor2msOverRunCnt++;
		    if (gDSPActiveTime.Motor2msOverRunCnt > 10) {
		        gDSPActiveTime.Motor2msOverRunFlag = 1;
		    }
		}

        if (gDSPActiveTime.Function2Ms > C_TIME_2MS) {
            gDSPActiveTime.Fun2msOverRunCnt++;
            if (gDSPActiveTime.Fun2msOverRunCnt > 10) {
                gDSPActiveTime.Fun2msOverRunFlag = 1;
            }
        }
#endif
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
#ifdef DPS_ACTIVE_TIME_TEST
ADCData[2]++;
if (ADCData[2] == 20000) {
    ADCData[2] = 0;
    ADCData[3]++;
}
#endif

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
GpioDataRegs.GPBSET.bit.GPIO32  = 1;
   	EPwm2Regs.ETCLR.bit.INT = 1;        // ETCLR 无需EALLOW 保护...

	EINT;
#ifdef DPS_ACTIVE_TIME_TEST
	gDSPActiveTime.PWMIntBase = GetTime();
ADCData[4]++;
if (ADCData[4] == 20000) {
    ADCData[4] = 0;
    ADCData[5]++;
}
#endif

    ADCOverInterrupt();

#ifdef SPI_SCOPE_TEST
    SPISendData();
#endif

#ifdef DPS_ACTIVE_TIME_TEST
    gDSPActiveTime.PWMInt = gDSPActiveTime.PWMIntBase - GetTime();
#endif
	DINT;

GpioDataRegs.GPBCLEAR.bit.GPIO32  = 1;
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
