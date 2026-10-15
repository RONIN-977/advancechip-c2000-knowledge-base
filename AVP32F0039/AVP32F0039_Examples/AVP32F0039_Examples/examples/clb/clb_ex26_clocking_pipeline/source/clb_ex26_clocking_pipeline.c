/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex26_clocking_pipeline.c
*                         TITLE: CLB Clocking and PIPELINE Mode
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
    //
    // The counter value for a CLB without PIPELINE Mode Enabled
    //
    uint32_t counterValueWithoutPipelineMode = 0;

    //
    // The counter value for a CLB with PIPELINE Mode Enabled
    //
    uint32_t counterValueWitPipelineMode = 0;

    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();


    Board_init();

    //
    // The tile configuration is:
    // TILE1:
    // PIPELINE MODE
    // COUNTER 0 configured to match at 62,500,000 counts
    // FSM to toggle at COUNTER0 match
    // CLB OUTPUT XBAR will show the output on GPIO
    //
    // COUNTER 2 will count to 64 then trigger an ADD-5
    // With PIPELINE enabled, counter will use previous counter value
    //
    initTILE1(myCLBForTILE1_BASE);

    //
    // The tile configuration is:
    // TILE2:
    // NO PIPELINE
    // COUNTER 2 will count to 64 then trigger an ADD-5
    // With PIPELINE enabled, counter will use current counter value
    //
    initTILE2(myCLBForTILE2_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);
    CLB_enableCLB(myCLBForTILE2_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    DEVICE_DELAY_US(1000000);

    counterValueWithoutPipelineMode = CLB_getRegister(myCLBForTILE2_BASE, CLB_REG_CTR_C2);
    counterValueWitPipelineMode = CLB_getRegister(myCLBForTILE1_BASE, CLB_REG_CTR_C2);

    if (counterValueWithoutPipelineMode != counterValueWitPipelineMode + 1)
    {
        ESTOP0;

        //
        // Error with pipeline, the value should be 1 less
        //
        while(1);
    }

    //
    // Success
    //
    ESTOP0;

    //
    // Keep running and view CLB OUTPUT XBAR GPIO
    //
    while(1)
    {

    }
}


//
// End of File
//
