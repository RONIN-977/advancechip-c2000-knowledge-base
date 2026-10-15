/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex19_aoc_release_control.c
*                         TITLE: CLB AOC Release Control Example.
*******************************************************************************************/



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

    Board_init();

    //
    // Implement Pass through GPIO0
    // Implement AOC does not invert
    // Implement AOC Gate Control is in bypass
    // AOC Release Control will be in CLEAR mode with SW RLS control
    //
    initTILE1(myCLBForTILE1_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);


    while(1)
    {
        DEVICE_DELAY_US(5000000);

        //
        // After this function executes, the state of the FLOP in the AOC
        // is set to HIGH.
        //
        CLB_writeSWReleaseControl(myCLBForTILE1_BASE, CLB_SW_RLS_CTRL4, true);

        //
        // Now that the output is set to HIGH, a rising edge in the CLB_OUT4
        // signal will cause a CLEAR of the output.
        //

        GPIO_writePin(myGPIO1Output, 1);

        DEVICE_DELAY_US(5000000);

        //
        // Clear the release signal, making it ready for the next write to SW
        // RLS register to cause a RISING EDGE.
        //
        CLB_writeSWReleaseControl(myCLBForTILE1_BASE, CLB_SW_RLS_CTRL4, false);
        GPIO_writePin(myGPIO1Output, 0);
    }
}

//
// End of File
//
