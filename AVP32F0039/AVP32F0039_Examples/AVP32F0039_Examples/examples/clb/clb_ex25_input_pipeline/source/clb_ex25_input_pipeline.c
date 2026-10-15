/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex25_input_pipeline.c
*                         TITLE: CLB INPUT Pipeline Mode
*******************************************************************************************/



//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "clb_config.h"

#define INPUT_WITHOUT_PIPELINE_MODE CLB_IN1
#define INPUT_WITH_PIPELINE_MODE    CLB_IN2

//
// Main
//
void main(void)
{
    uint32_t delayBetweenInputs = 0;

    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();


    Board_init();

    //
    // The tile configuration is:
    // LUT0 is only high when -> IN1 is high and IN0 is LOW
    // LUT0 output (when high) enables the COUNTER0 to count up
    //
    initTILE1(myCLBForTILE1_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Set both inputs to capture the delay between them in the counter value
    //
    CLB_setGPREG(myCLBForTILE1_BASE,
                 (1 << INPUT_WITH_PIPELINE_MODE) |
                 (1 << INPUT_WITHOUT_PIPELINE_MODE));

    //
    // Read C0 to get the counter value reflecting the delay between the INPUTs
    //
    delayBetweenInputs = CLB_getRegister(myCLBForTILE1_BASE, CLB_REG_CTR_C0);

    //
    // The input WITH pipeline mode enabled with capture the counter
    // one cycle later than the input WITHOUT pipeline mode enabled
    //
    if (delayBetweenInputs != 1)
    {
        //
        // Error, the captured counter value is not 1 cycle
        //
        ESTOP0;
        while(1);
    }

    //
    // Success the counter value captured delay between pipeline mode enabled input
    // and without pipeline mode enabled input
    //
    ESTOP0;

    while(1)
    {

    }
}


//
// End of File
//
