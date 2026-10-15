

/****************************************************************
文件功能：变频驱动主函数
文件版本：V1.0
作者       ：
最新更新：2015-11-01
****************************************************************/

#include "CANcomm.h"
#include "MainInclude.h"
#include "SubPrgInclude.h"
#include "MotorInclude.h"
#include "param_lr.h"
#include "SPIcomm.h"

/***************************************************************
-------------------------主程序部分-----------------------------
****************************************************************///337hz
void main(void)
{
	Ulong m_BaseTime,m_DetaTime;
	Uint  Level500usCount;

   	InitSysCtrl();
   	InitInterrupt();
   	InitPeripherals();
   	InitForMotorApp();
   	InitForFunctionApp();

	EnableDog();
	SetInterruptEnable();
   	EINT;   							    
   	ERTM;   							    

   	Level500usCount = 0;
	m_BaseTime = GetTime();

  	while(1)							// Step 6. User Application function:
   	{
		m_DetaTime = m_BaseTime - GetTime();
		if(m_DetaTime >= C_TIME_05MS)
        {
		    Level500usCount ++;
			m_BaseTime -= C_TIME_05MS;

 			KickDog();
            Main05msMotor();
            Main05msFunction();    

			if((Level500usCount & 0x03) == 0)
			{
//GpioDataRegs.GPASET.bit.GPIO18 = 1;
			    gDSPActiveTime.Motor2MsBase = GetTime();
				Main2msMotor();
				gDSPActiveTime.Motor2Ms = gDSPActiveTime.Motor2MsBase - GetTime();
//GpioDataRegs.GPADAT.bit.GPIO18 = 0;
			}
			if((Level500usCount & 0x03) == 2)
			{
			    gDSPActiveTime.Function2MsBase = GetTime();
				Main2msFunction();
				//GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;
				CanTransmit2ms();
				gDSPActiveTime.Function2Ms = gDSPActiveTime.Function2MsBase - GetTime();
			}
        }

		Main0msMotor();
		Main0msFunction();
   	}
}

/***************************************************************
-----------------------中断程序部分-----------------------------
****************************************************************/

/***************************************************************
	EPWM的周期中断，
****************************************************************/
interrupt void EndOfADCISR(void)
{
    EALLOW;             //28035改为EALLOW保护
    ADC_CLEAR_INT_FLAG;
    EDIS;
	EINT;
//GpioDataRegs.GPASET.bit.GPIO18 = 1;

	gDSPActiveTime.ADCIntBase = GetTime();
    ADCOverInterrupt();
    CanRemoteAckDataFill();
    SPISendData();
    gDSPActiveTime.ADCInt = gDSPActiveTime.ADCIntBase - GetTime();

	DINT;
//GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;
    EALLOW;
    ADC_RESET_SEQUENCE;
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;	// Acknowledge this interrupt
    EDIS;
} 

/***************************************************************
	EPWM的下溢中断，
****************************************************************/
interrupt void ZeroOfEPWMISR(void)
{
   	EALLOW;
   	EPwm2Regs.ETCLR.bit.INT = 1;
   	EDIS;

	EINT;
	gDSPActiveTime.PWMIntBase = GetTime();
	PWMZeroInterrupt();
	gDSPActiveTime.PWMInt = gDSPActiveTime.PWMIntBase - GetTime();
	DINT;

   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge this interrupt
}

/***************************************************************
	EPWM的过流中断，对硬件过流信号处理
****************************************************************/
Uint16 OCcount = 0;
interrupt void OneShotTZOfEPWMISR(void)
{
    OCcount ++;
    if (EPwm2Regs.TZFLG.bit.OST == 1)
    {

        HardWareErrorDeal();                    //处理硬件故障－电机控制模块处理
    }

   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	// Acknowledge this interrupt
}
