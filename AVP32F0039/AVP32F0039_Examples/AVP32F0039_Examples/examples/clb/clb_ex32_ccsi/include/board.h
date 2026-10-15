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
// CLB_OUTPUTXBAR7 -> CLB_SCLK_1 Pinmux
//
//
// CLB_OUTPUTXBAR7 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR7 1
#define CLB_SCLK_1_CLBOUTPUTXBAR_GPIO 1
#define CLB_SCLK_1_CLBOUTPUTXBAR_PIN_CONFIG GPIO_1_CLB_OUTPUTXBAR7

//
// CLB_OUTPUTXBAR8 -> CLB_SCLKX2_1 Pinmux
//
//
// CLB_OUTPUTXBAR8 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR8 0
#define CLB_SCLKX2_1_CLBOUTPUTXBAR_GPIO 0
#define CLB_SCLKX2_1_CLBOUTPUTXBAR_PIN_CONFIG GPIO_0_CLB_OUTPUTXBAR8

//
// CLB_OUTPUTXBAR5 -> CLB_SOUT_1 Pinmux
//
//
// CLB_OUTPUTXBAR5 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR5 8
#define CLB_SOUT_1_CLBOUTPUTXBAR_GPIO 8
#define CLB_SOUT_1_CLBOUTPUTXBAR_PIN_CONFIG GPIO_8_CLB_OUTPUTXBAR5

//
// EPWM1 -> EPWM_CLK Pinmux
//
//
// EPWM1_A - GPIO Settings
//
#define GPIO_PIN_EPWM1_A 30
#define EPWM_CLK_EPWMA_GPIO 30
#define EPWM_CLK_EPWMA_PIN_CONFIG GPIO_30_EPWM1_A
//
// EPWM1_B - GPIO Settings
//
#define GPIO_PIN_EPWM1_B 31
#define EPWM_CLK_EPWMB_GPIO 31
#define EPWM_CLK_EPWMB_PIN_CONFIG GPIO_31_EPWM1_B
//
// GPIO17 - GPIO Settings
//
#define CLB_SIN_1_GPIO_PIN_CONFIG GPIO_17_GPIO17

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define CCSI1_RX_TILE_BASE CLB1_BASE
void CCSI1_RX_TILE_init();
#define CCSI1_TX_TILE_BASE CLB2_BASE
void CCSI1_TX_TILE_init();

//*****************************************************************************
//
// CLBINPUTXBAR Configurations
//
//*****************************************************************************
#define CLBINPUTXBAR_CLB_SIN_1_SOURCE 17
#define CLBINPUTXBAR_CLB_SIN_1_INPUT XBAR_INPUT1
void CLBINPUTXBAR_CLB_SIN_1_init();
#define CLBINPUTXBAR_CLB_SCLKX2_SOURCE 0
#define CLBINPUTXBAR_CLB_SCLKX2_INPUT XBAR_INPUT2
void CLBINPUTXBAR_CLB_SCLKX2_init();

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_SCLK_1_init();
#define CLB_SCLK_1 XBAR_OUTPUT7
#define CLB_SCLK_1_ENABLED_MUXES (XBAR_MUX09)
void CLB_SCLKX2_1_init();
#define CLB_SCLKX2_1 XBAR_OUTPUT8
#define CLB_SCLKX2_1_ENABLED_MUXES (XBAR_MUX08)
void CLB_SOUT_1_init();
#define CLB_SOUT_1 XBAR_OUTPUT5
#define CLB_SOUT_1_ENABLED_MUXES (XBAR_MUX12)

//*****************************************************************************
//
// CLBXBAR Configurations
//
//*****************************************************************************
void myCLBXBAR_TRANSFER_START_SYNC_init();
#define myCLBXBAR_TRANSFER_START_SYNC XBAR_AUXSIG0

#define myCLBXBAR_TRANSFER_START_SYNC_ENABLED_MUXES (XBAR_MUX07)

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define EPWM_CLK_BASE EPWM1_BASE
#define EPWM_CLK_TBPRD 0
#define EPWM_CLK_COUNTER_MODE EPWM_COUNTER_MODE_STOP_FREEZE
#define EPWM_CLK_TBPHS 0
#define EPWM_CLK_CMPA 0
#define EPWM_CLK_CMPB 0
#define EPWM_CLK_CMPC 0
#define EPWM_CLK_CMPD 0
#define EPWM_CLK_DBRED 0
#define EPWM_CLK_DBFED 0
#define EPWM_CLK_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define EPWM_CLK_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define EPWM_CLK_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define CLB_SIN_1 17
void CLB_SIN_1_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_CCSI1_RX_TILE
#define INT_CCSI1_RX_TILE INT_CLB1
#define INT_CCSI1_RX_TILE_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_CCSI1_RX_TILE_ISR(void);

// Interrupt Settings for INT_CCSI1_TX_TILE
#define INT_CCSI1_TX_TILE INT_CLB2
#define INT_CCSI1_TX_TILE_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_CCSI1_TX_TILE_ISR(void);

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
void	CLB_init();
void	CLB_INPUTXBAR_init();
void	CLB_OUTPUTXBAR_init();
void	CLBXBAR_init();
void	EPWM_init();
void	GPIO_init();
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
