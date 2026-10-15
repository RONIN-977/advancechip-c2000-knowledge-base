/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  epg_ex1_generate_clocks.c
*                         TITLE: EPG Generating Synchronous Clocks
*******************************************************************************************/


//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define CLK_GEN_PERIOD      0U
#define CLK_GEN_OFFSET      0U
#define SIG_GEN_DATA_LENGTH 12U
#define SIG_GEN_DATA0_0_12  0b000001111110U
#define SIG_GEN_DATA0_16_27 (SIG_GEN_DATA0_0_12 << 2)


//
// Globals
//
uint32_t sigGenActiveData = 0;  //Data transform output

//
// Main
//
void main(void)
{
    uint32_t sigGenData = 0;

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
    // Select Signal Generator output for EPG output of 0,1
    //
    EPG_selectEPGOutput(EPG1_BASE, EPG_OUT0, EPG_OUT_SEL_SIG);
    EPG_selectEPGOutput(EPG1_BASE, EPG_OUT1, EPG_OUT_SEL_SIG);

    //
    // Select Signal Generator output 0,2 for EPG output 0,1
    // This makes sure that bit 0, bit 16 of the transformed data
    // are sent to the output when bit bang is disabled
    //
    EPG_selectSignalOutput(EPG1_BASE, EPG_OUT0, EPG_SIGGEN0_DATATRANOUT0);
    EPG_selectSignalOutput(EPG1_BASE, EPG_OUT1, EPG_SIGGEN0_DATATRANOUT2);

    //
    // Set Clock Generator 0 period to 0 which is divide by 1
    //
    EPG_setClkGenPeriod(EPG1_BASE, EPG_CLKGEN0, CLK_GEN_PERIOD);

    //
    // Set offset of clock generator output 0 to 0
    //
    EPG_setClkGenOffset(EPG1_BASE, EPG_CLKGEN0, 0, CLK_GEN_OFFSET);

    //
    // Set the source of signal generator 0 to clk generator output 0
    //
    EPG_selectSigGenClkSource(EPG1_BASE, EPG_SIGGEN0, EPG_CLKGEN0_CLKOUT0_DCLK);

    //
    // Set signal generator bit length to 12. When it completes
    // shifting of 12 bits, it loads the data0 and data1 registers to
    // to the active data register
    //
    EPG_setDataBitLen(EPG1_BASE, EPG_SIGGEN0, SIG_GEN_DATA_LENGTH);

    //
    // Set signal generator mode to right shift and repeat for 12 bits.
    //
    EPG_setSignalGenMode(EPG1_BASE, EPG_SIGGEN0,
                         EPG_SIGGEN_MODE_ROTATE_RIGHT_REPEAT);
    //
    // Set the first 12 bits of data for signal generator output 0 to
    // generator a 50% duty cycle clock with 1 cycle offset and next
    // 12 bits with 3 cycle offset
    //
    sigGenData = (SIG_GEN_DATA0_0_12 | (uint32_t)SIG_GEN_DATA0_16_27 << 16);
    EPG_setData0Word(EPG1_BASE, EPG_SIGGEN0, sigGenData);

    //
    // Enable EPG
    //
    EPG_enableGlobal(EPG1_BASE);

    //
    // Enable signal generator
    //
    EPG_enableSignalGen(EPG1_BASE, EPG_SIGGEN0);

    while(1)
    {
        sigGenActiveData = EPG_getData0ActiveReg(EPG1_BASE, EPG_SIGGEN0);
        //
        // Delay before reading the transformed data by signal generator
        //
        DEVICE_DELAY_US(1000000);
    }

}

//
// End of File
//
