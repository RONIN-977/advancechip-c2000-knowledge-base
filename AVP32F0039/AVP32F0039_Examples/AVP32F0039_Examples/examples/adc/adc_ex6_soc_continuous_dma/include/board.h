/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
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

//
// ANALOG -> myANALOGPinMux0 Pinmux
//

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
#define myADC0_BASE ADCA_BASE
#define myADC0_RESULT_BASE ADCARESULT_BASE
#define myADC0_SOC0 ADC_SOC_NUMBER0
#define myADC0_FORCE_SOC0 ADC_FORCE_SOC0
#define myADC0_SAMPLE_WINDOW_SOC0 75
#define myADC0_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM2_SOCA
#define myADC0_CHANNEL_SOC0 ADC_CH_ADCIN3
#define myADC0_SOC1 ADC_SOC_NUMBER1
#define myADC0_FORCE_SOC1 ADC_FORCE_SOC1
#define myADC0_SAMPLE_WINDOW_SOC1 75
#define myADC0_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC1 ADC_CH_ADCIN3
#define myADC0_SOC2 ADC_SOC_NUMBER2
#define myADC0_FORCE_SOC2 ADC_FORCE_SOC2
#define myADC0_SAMPLE_WINDOW_SOC2 75
#define myADC0_TRIGGER_SOURCE_SOC2 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC2 ADC_CH_ADCIN3
#define myADC0_SOC3 ADC_SOC_NUMBER3
#define myADC0_FORCE_SOC3 ADC_FORCE_SOC3
#define myADC0_SAMPLE_WINDOW_SOC3 75
#define myADC0_TRIGGER_SOURCE_SOC3 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC3 ADC_CH_ADCIN3
#define myADC0_SOC4 ADC_SOC_NUMBER4
#define myADC0_FORCE_SOC4 ADC_FORCE_SOC4
#define myADC0_SAMPLE_WINDOW_SOC4 75
#define myADC0_TRIGGER_SOURCE_SOC4 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC4 ADC_CH_ADCIN3
#define myADC0_SOC5 ADC_SOC_NUMBER5
#define myADC0_FORCE_SOC5 ADC_FORCE_SOC5
#define myADC0_SAMPLE_WINDOW_SOC5 75
#define myADC0_TRIGGER_SOURCE_SOC5 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC5 ADC_CH_ADCIN3
#define myADC0_SOC6 ADC_SOC_NUMBER6
#define myADC0_FORCE_SOC6 ADC_FORCE_SOC6
#define myADC0_SAMPLE_WINDOW_SOC6 75
#define myADC0_TRIGGER_SOURCE_SOC6 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC6 ADC_CH_ADCIN3
#define myADC0_SOC7 ADC_SOC_NUMBER7
#define myADC0_FORCE_SOC7 ADC_FORCE_SOC7
#define myADC0_SAMPLE_WINDOW_SOC7 75
#define myADC0_TRIGGER_SOURCE_SOC7 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC7 ADC_CH_ADCIN3
#define myADC0_SOC8 ADC_SOC_NUMBER8
#define myADC0_FORCE_SOC8 ADC_FORCE_SOC8
#define myADC0_SAMPLE_WINDOW_SOC8 75
#define myADC0_TRIGGER_SOURCE_SOC8 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC8 ADC_CH_ADCIN3
#define myADC0_SOC9 ADC_SOC_NUMBER9
#define myADC0_FORCE_SOC9 ADC_FORCE_SOC9
#define myADC0_SAMPLE_WINDOW_SOC9 75
#define myADC0_TRIGGER_SOURCE_SOC9 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC9 ADC_CH_ADCIN3
#define myADC0_SOC10 ADC_SOC_NUMBER10
#define myADC0_FORCE_SOC10 ADC_FORCE_SOC10
#define myADC0_SAMPLE_WINDOW_SOC10 75
#define myADC0_TRIGGER_SOURCE_SOC10 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC10 ADC_CH_ADCIN3
#define myADC0_SOC11 ADC_SOC_NUMBER11
#define myADC0_FORCE_SOC11 ADC_FORCE_SOC11
#define myADC0_SAMPLE_WINDOW_SOC11 75
#define myADC0_TRIGGER_SOURCE_SOC11 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC11 ADC_CH_ADCIN3
#define myADC0_SOC12 ADC_SOC_NUMBER12
#define myADC0_FORCE_SOC12 ADC_FORCE_SOC12
#define myADC0_SAMPLE_WINDOW_SOC12 75
#define myADC0_TRIGGER_SOURCE_SOC12 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC12 ADC_CH_ADCIN3
#define myADC0_SOC13 ADC_SOC_NUMBER13
#define myADC0_FORCE_SOC13 ADC_FORCE_SOC13
#define myADC0_SAMPLE_WINDOW_SOC13 75
#define myADC0_TRIGGER_SOURCE_SOC13 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC13 ADC_CH_ADCIN3
#define myADC0_SOC14 ADC_SOC_NUMBER14
#define myADC0_FORCE_SOC14 ADC_FORCE_SOC14
#define myADC0_SAMPLE_WINDOW_SOC14 75
#define myADC0_TRIGGER_SOURCE_SOC14 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC14 ADC_CH_ADCIN3
#define myADC0_SOC15 ADC_SOC_NUMBER15
#define myADC0_FORCE_SOC15 ADC_FORCE_SOC15
#define myADC0_SAMPLE_WINDOW_SOC15 75
#define myADC0_TRIGGER_SOURCE_SOC15 ADC_TRIGGER_SW_ONLY
#define myADC0_CHANNEL_SOC15 ADC_CH_ADCIN3
void myADC0_init();

#define myADC1_BASE ADCC_BASE
#define myADC1_RESULT_BASE ADCCRESULT_BASE
#define myADC1_SOC0 ADC_SOC_NUMBER0
#define myADC1_FORCE_SOC0 ADC_FORCE_SOC0
#define myADC1_SAMPLE_WINDOW_SOC0 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM2_SOCA
#define myADC1_CHANNEL_SOC0 ADC_CH_ADCIN3
#define myADC1_SOC1 ADC_SOC_NUMBER1
#define myADC1_FORCE_SOC1 ADC_FORCE_SOC1
#define myADC1_SAMPLE_WINDOW_SOC1 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC1 ADC_CH_ADCIN3
#define myADC1_SOC2 ADC_SOC_NUMBER2
#define myADC1_FORCE_SOC2 ADC_FORCE_SOC2
#define myADC1_SAMPLE_WINDOW_SOC2 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC2 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC2 ADC_CH_ADCIN3
#define myADC1_SOC3 ADC_SOC_NUMBER3
#define myADC1_FORCE_SOC3 ADC_FORCE_SOC3
#define myADC1_SAMPLE_WINDOW_SOC3 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC3 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC3 ADC_CH_ADCIN3
#define myADC1_SOC4 ADC_SOC_NUMBER4
#define myADC1_FORCE_SOC4 ADC_FORCE_SOC4
#define myADC1_SAMPLE_WINDOW_SOC4 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC4 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC4 ADC_CH_ADCIN3
#define myADC1_SOC5 ADC_SOC_NUMBER5
#define myADC1_FORCE_SOC5 ADC_FORCE_SOC5
#define myADC1_SAMPLE_WINDOW_SOC5 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC5 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC5 ADC_CH_ADCIN3
#define myADC1_SOC6 ADC_SOC_NUMBER6
#define myADC1_FORCE_SOC6 ADC_FORCE_SOC6
#define myADC1_SAMPLE_WINDOW_SOC6 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC6 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC6 ADC_CH_ADCIN3
#define myADC1_SOC7 ADC_SOC_NUMBER7
#define myADC1_FORCE_SOC7 ADC_FORCE_SOC7
#define myADC1_SAMPLE_WINDOW_SOC7 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC7 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC7 ADC_CH_ADCIN3
#define myADC1_SOC8 ADC_SOC_NUMBER8
#define myADC1_FORCE_SOC8 ADC_FORCE_SOC8
#define myADC1_SAMPLE_WINDOW_SOC8 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC8 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC8 ADC_CH_ADCIN3
#define myADC1_SOC9 ADC_SOC_NUMBER9
#define myADC1_FORCE_SOC9 ADC_FORCE_SOC9
#define myADC1_SAMPLE_WINDOW_SOC9 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC9 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC9 ADC_CH_ADCIN3
#define myADC1_SOC10 ADC_SOC_NUMBER10
#define myADC1_FORCE_SOC10 ADC_FORCE_SOC10
#define myADC1_SAMPLE_WINDOW_SOC10 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC10 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC10 ADC_CH_ADCIN3
#define myADC1_SOC11 ADC_SOC_NUMBER11
#define myADC1_FORCE_SOC11 ADC_FORCE_SOC11
#define myADC1_SAMPLE_WINDOW_SOC11 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC11 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC11 ADC_CH_ADCIN3
#define myADC1_SOC12 ADC_SOC_NUMBER12
#define myADC1_FORCE_SOC12 ADC_FORCE_SOC12
#define myADC1_SAMPLE_WINDOW_SOC12 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC12 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC12 ADC_CH_ADCIN3
#define myADC1_SOC13 ADC_SOC_NUMBER13
#define myADC1_FORCE_SOC13 ADC_FORCE_SOC13
#define myADC1_SAMPLE_WINDOW_SOC13 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC13 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC13 ADC_CH_ADCIN3
#define myADC1_SOC14 ADC_SOC_NUMBER14
#define myADC1_FORCE_SOC14 ADC_FORCE_SOC14
#define myADC1_SAMPLE_WINDOW_SOC14 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC14 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC14 ADC_CH_ADCIN3
#define myADC1_SOC15 ADC_SOC_NUMBER15
#define myADC1_FORCE_SOC15 ADC_FORCE_SOC15
#define myADC1_SAMPLE_WINDOW_SOC15 66.66666666666667
#define myADC1_TRIGGER_SOURCE_SOC15 ADC_TRIGGER_SW_ONLY
#define myADC1_CHANNEL_SOC15 ADC_CH_ADCIN3
void myADC1_init();


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
extern __interrupt void adcA1ISR(void);

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
