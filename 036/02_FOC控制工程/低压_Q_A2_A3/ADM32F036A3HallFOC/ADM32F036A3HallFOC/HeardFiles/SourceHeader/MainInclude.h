/************************************************************
------------------该文件是主程序的头文件---------------------
************************************************************/
#ifndef MAIN_INCLUDE_H
#define MAIN_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Define.h"
#include "DSP2803x_Device.h"

/************************************************************
	基本函数定义和引用
************************************************************/
#define  GetTime() 	(CpuTimer1.RegsAddr->TIM.all)

//extern Uint gSendToMotorDataBuff[];
extern unsigned int gSendToMotorDataBuff1[];
extern int gSendToFunctionDataBuff[];
extern unsigned int gSendToFunctionDataBuff1[];

/************************************************************
	常数定义
************************************************************/


/************************************************************
	引用函数说明
************************************************************/
extern void InitSysCtrl(void);   
extern void InitInterrupt(void);   
extern void InitPeripherals(void);   
extern void InitForMotorApp(void);
extern void InitForFunctionApp(void);   
extern void SetInterruptEnable(void);


extern void ADCOverInterrupt(void);
extern void HardWareErrorDeal(void);

extern void SystemLeve2msMotor(void);
extern void SystemLeve2msFunction(void);
extern void SystemLeve05msMotor(void);
extern void SystemLeve05msFunction(void);
extern void SystemLeve0msMotor(void);
extern void SystemLeve0msFunction(void);

extern void EnableDog(void);
extern void DisableDog(void);
extern void KickDog(void);
extern void ResetDSP(void);

extern void PreDriverTest(void);

//以下是中断程序说明
extern interrupt void cpu_timer0_isr(void);
extern interrupt void EndOfADCISR(void);
extern interrupt void OneShotTZOfEPWMISR(void);
extern interrupt void ZeroOfEPWMISR(void);
extern interrupt void NMI_OverUdc_isr(void);

/************************************************************
	引用结构说明
************************************************************/
extern int gDebugFlag;
#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================
