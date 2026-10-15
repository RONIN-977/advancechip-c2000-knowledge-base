/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex18_aoc.c
*                         TITLE: CLB AOC Example.
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
    // Implement AND Gate between GPIO0 and GPIO1
    // Implement AOC Invert functionality turning the GATE to a NAND
    // Implement AOC Gate Control as an AND GATE on top of the logic
    // AOC Gate Control will use SW GATE register
    //
    initTILE1(myCLBForTILE1_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);

    //
    // Gate the NAND implemented
    // Switch the value of the GATE control to 'false' and the logic will
    // be gated.
    //
    CLB_writeSWGateControl(myCLBForTILE1_BASE, CLB_SW_GATE_CTRL4, true);

    while(1)
    {

        asm(" NOP");
    }
}

//
// End of File
//
