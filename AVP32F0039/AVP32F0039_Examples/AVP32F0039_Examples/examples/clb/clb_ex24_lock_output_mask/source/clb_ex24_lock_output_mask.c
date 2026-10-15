/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex24_lock_output_mask.c
*                         TITLE: CLB Lock Output Mask
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
    uint32_t outputMask = 0;

    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();


    Board_init();

    //
    // The tile configuration is not important for this example
    //
    initTILE1(myCLBForTILE1_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    DEVICE_DELAY_US(1000);
    ESTOP0;

    outputMask = HWREG(myCLBForTILE1_BASE + CLB_LOGICCTL + CLB_O_OUT_EN);
    if (outputMask != 1U << 4U){
        //
        // Output Mask was not set for OUT4
        //
        ESTOP0;
        while(1);
    }

    //
    // Output Mask was set for OUT4
    // let's try to change it while it's locked
    //
    CLB_setOutputMask(myCLBForTILE1_BASE, CLB_OUTPUT_04, false);

    outputMask = HWREG(myCLBForTILE1_BASE + CLB_LOGICCTL + CLB_O_OUT_EN);
    if (outputMask != 1U << 4U){
        //
        // Output Mask was change even though it was locked
        //
        ESTOP0;
        while(1);
    }

    ESTOP0;

    //
    // Output Mask was set for OUT4
    // Enable the writes to OUT_EN then change the mask
    //
    CLB_enableOutputMaskUpdates(myCLBForTILE1_BASE);
    CLB_setOutputMask(myCLBForTILE1_BASE, CLB_OUTPUT_04, false);

    outputMask = HWREG(myCLBForTILE1_BASE + CLB_LOGICCTL + CLB_O_OUT_EN);
    if (outputMask == 1U << 4U){
        //
        // Output Mask was change even though it was locked
        //
        ESTOP0;
        while(1);
    }

    //
    // Success, the lock was diabled and the update took place on the
    // OUT_EN register
    //
    ESTOP0;

    while(1)
    {

    }
}


//
// End of File
//
