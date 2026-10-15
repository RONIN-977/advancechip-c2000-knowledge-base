/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  Board Configurations.
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

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
#define myADC0_BASE ADCA_BASE
#define myADC0_RESULT_BASE ADCARESULT_BASE
#define myADC0_SOC7 ADC_SOC_NUMBER7
#define myADC0_FORCE_SOC7 ADC_FORCE_SOC7
#define myADC0_SAMPLE_WINDOW_SOC7 80
#define myADC0_TRIGGER_SOURCE_SOC7 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC7 ADC_CH_ADCIN0
#define myADC0_SOC8 ADC_SOC_NUMBER8
#define myADC0_FORCE_SOC8 ADC_FORCE_SOC8
#define myADC0_SAMPLE_WINDOW_SOC8 80
#define myADC0_TRIGGER_SOURCE_SOC8 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC8 ADC_CH_ADCIN1
#define myADC0_SOC9 ADC_SOC_NUMBER9
#define myADC0_FORCE_SOC9 ADC_FORCE_SOC9
#define myADC0_SAMPLE_WINDOW_SOC9 80
#define myADC0_TRIGGER_SOURCE_SOC9 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC9 ADC_CH_ADCIN2
#define myADC0_SOC10 ADC_SOC_NUMBER10
#define myADC0_FORCE_SOC10 ADC_FORCE_SOC10
#define myADC0_SAMPLE_WINDOW_SOC10 80
#define myADC0_TRIGGER_SOURCE_SOC10 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC10 ADC_CH_ADCIN0
#define myADC0_SOC11 ADC_SOC_NUMBER11
#define myADC0_FORCE_SOC11 ADC_FORCE_SOC11
#define myADC0_SAMPLE_WINDOW_SOC11 80
#define myADC0_TRIGGER_SOURCE_SOC11 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC11 ADC_CH_ADCIN1
#define myADC0_SOC12 ADC_SOC_NUMBER12
#define myADC0_FORCE_SOC12 ADC_FORCE_SOC12
#define myADC0_SAMPLE_WINDOW_SOC12 80
#define myADC0_TRIGGER_SOURCE_SOC12 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC12 ADC_CH_ADCIN3
#define myADC0_SOC13 ADC_SOC_NUMBER13
#define myADC0_FORCE_SOC13 ADC_FORCE_SOC13
#define myADC0_SAMPLE_WINDOW_SOC13 80
#define myADC0_TRIGGER_SOURCE_SOC13 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC13 ADC_CH_ADCIN0
#define myADC0_SOC14 ADC_SOC_NUMBER14
#define myADC0_FORCE_SOC14 ADC_FORCE_SOC14
#define myADC0_SAMPLE_WINDOW_SOC14 80
#define myADC0_TRIGGER_SOURCE_SOC14 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC14 ADC_CH_ADCIN1
#define myADC0_SOC15 ADC_SOC_NUMBER15
#define myADC0_FORCE_SOC15 ADC_FORCE_SOC15
#define myADC0_SAMPLE_WINDOW_SOC15 80
#define myADC0_TRIGGER_SOURCE_SOC15 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC15 ADC_CH_ADCIN4
void myADC0_init();


//*****************************************************************************
//
// ASYSCTL Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myADC0_1
#define INT_myADC0_1 INT_ADCA1
#define INT_myADC0_1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP1
extern __interrupt void adcABurstISR(void);

// Interrupt Settings for INT_myADC0_2
#define INT_myADC0_2 INT_ADCA2
#define INT_myADC0_2_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP10
extern __interrupt void adcABurstISR(void);

// Interrupt Settings for INT_myADC0_3
#define INT_myADC0_3 INT_ADCA3
#define INT_myADC0_3_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP10
extern __interrupt void adcABurstISR(void);

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	ADC_init();
void	ASYSCTL_init();
void	INTERRUPT_init();
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
