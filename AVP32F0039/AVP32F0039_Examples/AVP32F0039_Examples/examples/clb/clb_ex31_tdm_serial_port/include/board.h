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
// CLB_OUTPUTXBAR6 -> CLB_FSYNC_OUT Pinmux
//
//
// CLB_OUTPUTXBAR6 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR6 4
#define CLB_FSYNC_OUT_CLBOUTPUTXBAR_GPIO 4
#define CLB_FSYNC_OUT_CLBOUTPUTXBAR_PIN_CONFIG GPIO_4_CLB_OUTPUTXBAR6

//
// CLB_OUTPUTXBAR8 -> CLB_DATA1_OUT Pinmux
//
//
// CLB_OUTPUTXBAR8 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR8 6
#define CLB_DATA1_OUT_CLBOUTPUTXBAR_GPIO 6
#define CLB_DATA1_OUT_CLBOUTPUTXBAR_PIN_CONFIG GPIO_6_CLB_OUTPUTXBAR8
//
// GPIO1 - GPIO Settings
//
#define CLB_BCLK_IN_GPIO_PIN_CONFIG GPIO_1_GPIO1
//
// GPIO0 - GPIO Settings
//
#define CLB_FSYNC_IN_GPIO_PIN_CONFIG GPIO_0_GPIO0
//
// GPIO2 - GPIO Settings
//
#define CLB_DATA1_IN_GPIO_PIN_CONFIG GPIO_2_GPIO2

//
// OUTPUTXBAR3 -> CLB_BCLK_OUT Pinmux
//
//
// OUTPUTXBAR3 - GPIO Settings
//
#define GPIO_PIN_OUTPUTXBAR3 5
#define CLB_BCLK_OUT_OUTPUTXBAR_GPIO 5
#define CLB_BCLK_OUT_OUTPUTXBAR_PIN_CONFIG GPIO_5_OUTPUTXBAR3

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define myCLBForTILE1_BASE CLB1_BASE
void myCLBForTILE1_init();

//*****************************************************************************
//
// CLBINPUTXBAR Configurations
//
//*****************************************************************************
#define CLBINPUTXBAR_BCLK_IN_SOURCE 1
#define CLBINPUTXBAR_BCLK_IN_INPUT XBAR_INPUT1
void CLBINPUTXBAR_BCLK_IN_init();
#define CLBINPUTXBAR_FSYNC_IN_SOURCE 0
#define CLBINPUTXBAR_FSYNC_IN_INPUT XBAR_INPUT2
void CLBINPUTXBAR_FSYNC_IN_init();
#define CLBINPUTXBAR_DATA1_IN_SOURCE 2
#define CLBINPUTXBAR_DATA1_IN_INPUT XBAR_INPUT3
void CLBINPUTXBAR_DATA1_IN_init();

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_FSYNC_OUT_init();
#define CLB_FSYNC_OUT XBAR_OUTPUT6
#define CLB_FSYNC_OUT_ENABLED_MUXES (XBAR_MUX04)
void CLB_DATA1_OUT_init();
#define CLB_DATA1_OUT XBAR_OUTPUT8
#define CLB_DATA1_OUT_ENABLED_MUXES (XBAR_MUX00)

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define CLB_BCLK_IN 1
void CLB_BCLK_IN_init();
#define CLB_FSYNC_IN 0
void CLB_FSYNC_IN_init();
#define CLB_DATA1_IN 2
void CLB_DATA1_IN_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myCLBForTILE1
#define INT_myCLBForTILE1 INT_CLB1
#define INT_myCLBForTILE1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void clbISR(void);

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_BCLK_OUT_init();
#define CLB_BCLK_OUT XBAR_OUTPUT3
#define CLB_BCLK_OUT_ENABLED_MUXES (XBAR_MUX03)

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CLB_init();
void	CLB_INPUTXBAR_init();
void	CLB_OUTPUTXBAR_init();
void	GPIO_init();
void	INTERRUPT_init();
void	OUTPUTXBAR_init();
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
