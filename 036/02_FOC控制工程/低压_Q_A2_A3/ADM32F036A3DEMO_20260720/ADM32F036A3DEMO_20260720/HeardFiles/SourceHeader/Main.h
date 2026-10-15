#ifndef F_MAIN_H
#define F_MAIN_H

#include "ADP32F03x_Device.h"

#define     ABS_INT16(a) (((a) >= 0) ? (a) : (-(a)))
#define     ABS_INT32(a) (((a) >= 0L) ? (a) : (-(a)))
#define     MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define     MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#define     RUN_CTRL_PERIOD         2

/***************************************************************************
  Public Variables
 ***************************************************************************/
/* CpuTimer1 is used as the main time base. */
#define GetTime()               (CpuTimer1.RegsAddr->TIM.all)

enum POWER_ON_STATUS
{
    POWER_ON_WAIT,
    POWER_ON_CORE_OK,
    POWER_ON_FUNC_WAIT_OT
};

/*=============================================================================
 *  Shared variables
 *
 *  Only variables that are intentionally shared across modules should be
 *  declared here. Module-private variables must stay in their own .c files.
 *============================================================================*/
extern enum POWER_ON_STATUS powerOnStatus;
extern Uint16 frq2Core;
extern int32  ObserverMotorSpeedINOnePointHZ;
extern Uint16 errorCodeFromCore;

void InitSysCtrl(void);
void InitInterrupt(void);
void InitPeripherals(void);
void InitForMotorApp(void);

void InitForFunctionApp(void);
void CommsProtocalLayerTask2ms(void);
void SetInterruptEnable(void);
void EnableDog(void);
void KickDog(void);
void ResetDSP(void);

void ADP32F03x_usDelay(Uint32 Count);

void SystemLeve0msMotor(void);
void SystemLeve05msMotor(void);
void SystemLeve2msMotor(void);

void SystemLeve0msFunction(void);
void SystemLeve05msFunction(void);
void SystemLeve2msFunction(void);

/*=============================================================================
 *  Peripheral / communication / protection services
 *============================================================================*/
void ResetLINAndPreDrive(void);
void ADCOverInterrupt(void);
void HardWareErrorDeal(void);
void PreDriverTest(void);

/*=============================================================================
 *  Interrupt service routines
 *============================================================================*/
interrupt void cpu_timer0_isr(void);
interrupt void EndOfADCISR(void);
interrupt void OneShotTZOfEPWMISR(void);
interrupt void ZeroOfEPWMISR(void);
interrupt void NMI_OverUdc_isr(void);

#endif // __F_MAIN_H__

