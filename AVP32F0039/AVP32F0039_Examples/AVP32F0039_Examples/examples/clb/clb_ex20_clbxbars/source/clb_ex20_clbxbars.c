/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex20_clbxbars.c
*                         TITLE: CLB XBARs.
*******************************************************************************************/



//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "clb_config.h"

//
// Main
//
void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    //
    // Initialize pinmux, and sysconfig peripherals
    //
    Board_init();

    //
    //
    //
    initTILE1(myCLBForTILE1_BASE);
    initTILE2(myCLBForTILE2_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);
    CLB_enableCLB(myCLBForTILE2_BASE);


    while(1)
    {
        asm(" NOP");
    }
}

//
// End of File
//
