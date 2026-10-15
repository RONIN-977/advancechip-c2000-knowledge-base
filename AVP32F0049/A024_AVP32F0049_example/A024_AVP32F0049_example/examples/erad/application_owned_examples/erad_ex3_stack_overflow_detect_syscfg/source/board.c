/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  ERAD HWBP Stack Overflow Detection.
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
	ERAD_init();

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
	

}

//*****************************************************************************
//
// ERAD Configurations
//
//*****************************************************************************
void ERAD_init(){
    //
	// myERAD0 initialization
    //
    ERAD_initModule(ERAD_OWNER_APPLICATION);
    ERAD_BusComp_Config buscomp_params;
    //
    // myERAD0_BUSCOMP1 initialization
    //
    buscomp_params.bus_sel = ERAD_BUSCOMP_BUS_DWAB;
    buscomp_params.comp_mode = ERAD_BUSCOMP_COMPMODE_EQ;
    buscomp_params.mask = 0x0;
	buscomp_params.reference = (uint32_t)address;
    buscomp_params.enable_stop = false;
    buscomp_params.enable_int = true;
    ERAD_configBusComp(myERAD0_BUSCOMP1_BASE, buscomp_params);
    ERAD_enableModules(ERAD_INST_BUSCOMP1);
    
}
