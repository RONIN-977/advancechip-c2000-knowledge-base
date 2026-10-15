/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  epg_ex7_generate_two_offset_clocks.c
*                         TITLE: EPG Generating Two Offset Clocks
*******************************************************************************************/


//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define CLK_GEN_PERIOD      100U-1U

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure GPIO pins 58, 54 for EPG OUT
    //
    Device_initGPIO();
    
    GPIO_setPinConfig(GPIO_58_OUTPUTXBAR1);
    GPIO_setDirectionMode(58, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(58, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(58, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(GPIO_54_OUTPUTXBAR2);
    GPIO_setDirectionMode(54, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(54, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(54, GPIO_QUAL_SYNC);

    //
    // Connect EPG Output to GPIO through xbar
    //
    XBAR_setOutputMuxConfig(OUTPUTXBAR_BASE, XBAR_OUTPUT1,
                            XBAR_OUT_MUX30_EPG1_OUT0);
    XBAR_setOutputMuxConfig(OUTPUTXBAR_BASE, XBAR_OUTPUT2,
                            XBAR_OUT_MUX31_EPG1_OUT1);
    XBAR_enableOutputMux(OUTPUTXBAR_BASE, XBAR_OUTPUT1, XBAR_MUX30);
    XBAR_enableOutputMux(OUTPUTXBAR_BASE, XBAR_OUTPUT2, XBAR_MUX31);

    //
    // Select Clock Generator output for EPG output of 0,1
    //
    EPG_selectEPGOutput(EPG1_BASE, EPG_OUT0, EPG_OUT_SEL_CLK);
    EPG_selectEPGOutput(EPG1_BASE, EPG_OUT1, EPG_OUT_SEL_CLK);

    //
    // Select Clock Generator Data output for EPG output of 0,1
    //
    EPG_selectClkOutput(EPG1_BASE, EPG_OUT0, EPG_CLKGEN0_CLKOUT0_DCLK);
    EPG_selectClkOutput(EPG1_BASE, EPG_OUT1, EPG_CLKGEN0_CLKOUT1_DCLK);

    //
    // Set Clock Generator 0 period to CLK_GEN_PERIOD
    //
    EPG_setClkGenPeriod(EPG1_BASE, EPG_CLKGEN0, CLK_GEN_PERIOD);

    //
    // Set offset of clock generator output 0 to 0
    //
    EPG_setClkGenOffset(EPG1_BASE, EPG_CLKGEN0, 0, 0);

    //
    // Set offset of clock generator output 1 to 25
    //
    EPG_setClkGenOffset(EPG1_BASE, EPG_CLKGEN0, 1, 25);

    //
    // Switching from BITBANG mode will disable the clock generation
    //
    //EPG_setSignalGenMode(EPG1_BASE, EPG_SIGGEN0, EPG_SIGGEN_MODE_SHIFT_RIGHT_ONCE);

    //
    // Enable EPG
    //
    EPG_enableGlobal(EPG1_BASE);

    while(1)
    {
        //
        // Delay
        //
        DEVICE_DELAY_US(1000000);
    }

}

//
// End of File
//
