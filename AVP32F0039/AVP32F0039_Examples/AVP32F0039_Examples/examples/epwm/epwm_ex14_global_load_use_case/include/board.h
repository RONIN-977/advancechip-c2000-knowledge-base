/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*******************************************************************************************/

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// ANALOG -> myANALOGPinMux0 Pinmux
//

//
// EPWM1 -> myEPWM1 Pinmux
//
//
// EPWM1_A - GPIO Settings
//
#define GPIO_PIN_EPWM1_A 0
#define myEPWM1_EPWMA_GPIO 0
#define myEPWM1_EPWMA_PIN_CONFIG GPIO_0_EPWM1_A
//
// EPWM1_B - GPIO Settings
//
#define GPIO_PIN_EPWM1_B 1
#define myEPWM1_EPWMB_GPIO 1
#define myEPWM1_EPWMB_PIN_CONFIG GPIO_1_EPWM1_B

//
// EPWM2 -> myEPWM2 Pinmux
//
//
// EPWM2_A - GPIO Settings
//
#define GPIO_PIN_EPWM2_A 2
#define myEPWM2_EPWMA_GPIO 2
#define myEPWM2_EPWMA_PIN_CONFIG GPIO_2_EPWM2_A
//
// EPWM2_B - GPIO Settings
//
#define GPIO_PIN_EPWM2_B 3
#define myEPWM2_EPWMB_GPIO 3
#define myEPWM2_EPWMB_PIN_CONFIG GPIO_3_EPWM2_B

//
// EPWM3 -> myEPWM3 Pinmux
//
//
// EPWM3_A - GPIO Settings
//
#define GPIO_PIN_EPWM3_A 4
#define myEPWM3_EPWMA_GPIO 4
#define myEPWM3_EPWMA_PIN_CONFIG GPIO_4_EPWM3_A
//
// EPWM3_B - GPIO Settings
//
#define GPIO_PIN_EPWM3_B 5
#define myEPWM3_EPWMB_GPIO 5
#define myEPWM3_EPWMB_PIN_CONFIG GPIO_5_EPWM3_B
//
// GPIO6 - GPIO Settings
//
#define myGPIO6_GPIO_PIN_CONFIG GPIO_6_GPIO6

//*****************************************************************************
//
// CMPSS Configurations
//
//*****************************************************************************
#define myCMPSS1_BASE CMPSS1_BASE
#define myCMPSS1_HIGH_COMP_BASE CMPSS1_BASE    
#define myCMPSS1_LOW_COMP_BASE CMPSS1_BASE    
void myCMPSS1_init();

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define myEPWM1_BASE EPWM1_BASE
#define myEPWM1_TBPRD 125
#define myEPWM1_COUNTER_MODE EPWM_COUNTER_MODE_UP_DOWN
#define myEPWM1_TBPHS 0
#define myEPWM1_CMPA 69
#define myEPWM1_CMPB 69
#define myEPWM1_CMPC 0
#define myEPWM1_CMPD 0
#define myEPWM1_DBRED 20
#define myEPWM1_DBFED 20
#define myEPWM1_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM1_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM1_INTERRUPT_SOURCE EPWM_INT_TBCTR_ZERO
void myEPWM1_init();
#define myEPWM2_BASE EPWM2_BASE
#define myEPWM2_TBPRD 125
#define myEPWM2_COUNTER_MODE EPWM_COUNTER_MODE_UP_DOWN
#define myEPWM2_TBPHS 42
#define myEPWM2_CMPA 69
#define myEPWM2_CMPB 69
#define myEPWM2_CMPC 0
#define myEPWM2_CMPD 0
#define myEPWM2_DBRED 20
#define myEPWM2_DBFED 20
#define myEPWM2_TZA_ACTION EPWM_TZ_ACTION_LOW
#define myEPWM2_TZB_ACTION EPWM_TZ_ACTION_LOW
#define myEPWM2_CBC_SOURCES EPWM_TZ_SIGNAL_DCAEVT2
#define myEPWM2_TZ_INTERRUPT_SOURCES EPWM_TZ_INTERRUPT_CBC
#define myEPWM2_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED
void myEPWM2_init();
#define myEPWM3_BASE EPWM3_BASE
#define myEPWM3_TBPRD 125
#define myEPWM3_COUNTER_MODE EPWM_COUNTER_MODE_UP_DOWN
#define myEPWM3_TBPHS 83
#define myEPWM3_CMPA 69
#define myEPWM3_CMPB 69
#define myEPWM3_CMPC 0
#define myEPWM3_CMPD 0
#define myEPWM3_DBRED 20
#define myEPWM3_DBFED 20
#define myEPWM3_TZA_ACTION EPWM_TZ_ACTION_LOW
#define myEPWM3_TZB_ACTION EPWM_TZ_ACTION_LOW
#define myEPWM3_OSHT_SOURCES EPWM_TZ_SIGNAL_OSHT1
#define myEPWM3_TZ_INTERRUPT_SOURCES EPWM_TZ_INTERRUPT_OST
#define myEPWM3_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED
void myEPWM3_init();

//*****************************************************************************
//
// EPWMXBAR Configurations
//
//*****************************************************************************
void myEPWMXBAR4_init();
#define myEPWMXBAR4 XBAR_TRIP4
#define myEPWMXBAR4_ENABLED_MUXES (XBAR_MUX00)

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define myGPIO6 6
void myGPIO6_init();

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
#define myINPUTXBARINPUT0_SOURCE 6
#define myINPUTXBARINPUT0_INPUT XBAR_INPUT1
void myINPUTXBARINPUT0_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myEPWM1
#define INT_myEPWM1 INT_EPWM1
#define INT_myEPWM1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP3
extern __interrupt void INT_myEPWM1_ISR(void);

// Interrupt Settings for INT_myEPWM2_TZ
#define INT_myEPWM2_TZ INT_EPWM2_TZ
#define INT_myEPWM2_TZ_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP2
extern __interrupt void INT_myEPWM2_TZ_ISR(void);

// Interrupt Settings for INT_myEPWM3_TZ
#define INT_myEPWM3_TZ INT_EPWM3_TZ
#define INT_myEPWM3_TZ_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP2
extern __interrupt void INT_myEPWM3_TZ_ISR(void);

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CMPSS_init();
void	EPWM_init();
void	EPWMXBAR_init();
void	GPIO_init();
void	INPUTXBAR_init();
void	INTERRUPT_init();
void	SYNC_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
