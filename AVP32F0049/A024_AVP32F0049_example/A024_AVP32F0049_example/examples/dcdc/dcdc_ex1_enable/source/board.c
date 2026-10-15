/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  Board Configurations.
*******************************************************************************************/

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	// DC-DC Initialization:
	// The procedure to enable the internal DC-DC regulator should be completed 
	// prior to other initialization functions. This is in order for the
	// DC-DC initialization to happen during low current operation. 
	// "DC_DC_init()" should be called before initialization functions like 
	// Device_init() or Device_enableAllPeripherals().

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	// DC-DC pinmux
	GPIO_setAnalogMode(22, GPIO_ANALOG_ENABLED);
	GPIO_setAnalogMode(23, GPIO_ANALOG_ENABLED);

}

//*****************************************************************************
//
// DCDC Configurations
//
//*****************************************************************************
void DC_DC_init(){
    //
    // myDCDC0 initialization
    //
    bool enable_status = ASysCtl_enableDCDC();
    if (!enable_status)
    {
        //
        // Error enabling the DC-DC module
        //
        ESTOP0;
    }
}
