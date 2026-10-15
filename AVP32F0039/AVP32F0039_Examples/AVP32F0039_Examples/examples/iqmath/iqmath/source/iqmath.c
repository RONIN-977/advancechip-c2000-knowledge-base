/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  IQmath_main.c
*                         TITLE: IQmath SysConfig Example
*******************************************************************************************/


//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

//
// Main
//
void main(void)
{
    //
    // CPU Initialization
    //
    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();
	Board_init();

	//
	// Convert floating-point to fixed-point (_iq) value
	// NOTE: Default N data type value for _iq is _iq24
	//
    _iq param0 = _IQ(9.0);
    _iq param1 = _IQ(3.0);

    //
    // Perform arithmetic (multiplication and division)
    //
    _iq resultMult = _IQmpy(param0, param1);
    _iq resultDivi = _IQdiv(param0, param1);

    //
    // Convert results and parameters to floating point to verify results
    // and proper conversion
    //
    float32_t floatParam0 = _IQtoF(param0);
    float32_t floatParam1 = _IQtoF(param1);
    float32_t floatResultMult = _IQtoF(resultMult);
    float32_t floatResultDivi = _IQtoF(resultDivi);

	while(1);
}

//
// End of File
//
