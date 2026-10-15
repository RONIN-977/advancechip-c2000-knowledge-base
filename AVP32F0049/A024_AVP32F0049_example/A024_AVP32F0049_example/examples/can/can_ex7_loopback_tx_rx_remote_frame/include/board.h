/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
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
// CANB -> myCAN0 Pinmux
//
//
// CANB_RX - GPIO Settings
//
#define GPIO_PIN_CANB_RX 10
#define myCAN0_CANRX_GPIO 10
#define myCAN0_CANRX_PIN_CONFIG GPIO_10_CANB_RX
//
// CANB_TX - GPIO Settings
//
#define GPIO_PIN_CANB_TX 12
#define myCAN0_CANTX_GPIO 12
#define myCAN0_CANTX_PIN_CONFIG GPIO_12_CANB_TX

//*****************************************************************************
//
// CAN Configurations
//
//*****************************************************************************
#define myCAN0_BASE CANB_BASE

#define myCAN0_MessageObj1_ID 5
#define myCAN0_MessageObj2_ID 5
#define myCAN0_MessageObj3_ID 7
void myCAN0_init();


//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CAN_init();
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
