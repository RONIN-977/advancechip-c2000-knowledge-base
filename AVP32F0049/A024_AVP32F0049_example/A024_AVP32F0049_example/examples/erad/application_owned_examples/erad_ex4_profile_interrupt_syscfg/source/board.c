/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  ERAD Profiling Interrupts Example
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
    ERAD_Counter_Config counter_params;
    ERAD_BusComp_Config buscomp_params;
    //
    // myERAD0_COUNTER1 initialization
    //
    counter_params.enable_stop = false;
    counter_params.enable_int = true;
    
    counter_params.event = ERAD_EVENT_NO_EVENT;
	counter_params.event_mode = ERAD_COUNTER_MODE_RISING_EDGE;
    counter_params.reference = 500;
    counter_params.rst_on_match = false;
	ERAD_configCounterInStartStopMode(myERAD0_COUNTER1_BASE, counter_params, ERAD_EVENT_HWBP1, ERAD_EVENT_HWBP2);
    //
    // myERAD0_COUNTER2 initialization
    //
    counter_params.enable_stop = false;
    counter_params.enable_int = false;
    
    counter_params.event = ERAD_EVENT_TIMER1_TINT1;
	counter_params.event_mode = ERAD_COUNTER_MODE_RISING_EDGE;
    counter_params.reference = 0;
    counter_params.rst_on_match = false;
	ERAD_configCounterInCountingMode(myERAD0_COUNTER2_BASE, counter_params);
    //
    // myERAD0_COUNTER3 initialization
    //
    counter_params.enable_stop = false;
    counter_params.enable_int = false;
    
    counter_params.event = ERAD_EVENT_HWBP1;
	counter_params.event_mode = ERAD_COUNTER_MODE_RISING_EDGE;
    counter_params.reference = 0;
    counter_params.rst_on_match = false;
	ERAD_configCounterInCountingMode(myERAD0_COUNTER3_BASE, counter_params);
    //
    // myERAD0_COUNTER4 initialization
    //
    counter_params.enable_stop = false;
    counter_params.enable_int = false;
    
    counter_params.event = ERAD_EVENT_NO_EVENT;
	counter_params.event_mode = ERAD_COUNTER_MODE_RISING_EDGE;
    counter_params.reference = 0;
    counter_params.rst_on_match = false;
	ERAD_configCounterInStartStopMode(myERAD0_COUNTER4_BASE, counter_params, ERAD_EVENT_TIMER1_TINT1, ERAD_EVENT_HWBP1);
    //
    // myERAD0_BUSCOMP1 initialization
    //
    buscomp_params.bus_sel = ERAD_BUSCOMP_BUS_VPC;
    buscomp_params.comp_mode = ERAD_BUSCOMP_COMPMODE_EQ;
    buscomp_params.mask = 0x0;
	buscomp_params.reference = (uint32_t)addr_cpuTimer1ISR;
    buscomp_params.enable_stop = false;
    buscomp_params.enable_int = false;
    ERAD_configBusComp(myERAD0_BUSCOMP1_BASE, buscomp_params);
    //
    // myERAD0_BUSCOMP2 initialization
    //
    buscomp_params.bus_sel = ERAD_BUSCOMP_BUS_DWAB;
    buscomp_params.comp_mode = ERAD_BUSCOMP_COMPMODE_EQ;
    buscomp_params.mask = 0x0;
	buscomp_params.reference = (uint32_t)addr_cpuTimer1IntCount;
    buscomp_params.enable_stop = false;
    buscomp_params.enable_int = false;
    ERAD_configBusComp(myERAD0_BUSCOMP2_BASE, buscomp_params);
    ERAD_enableModules(ERAD_INST_COUNTER1 | ERAD_INST_COUNTER2 | ERAD_INST_COUNTER3 | ERAD_INST_COUNTER4 | ERAD_INST_BUSCOMP1 | ERAD_INST_BUSCOMP2);
    
}
