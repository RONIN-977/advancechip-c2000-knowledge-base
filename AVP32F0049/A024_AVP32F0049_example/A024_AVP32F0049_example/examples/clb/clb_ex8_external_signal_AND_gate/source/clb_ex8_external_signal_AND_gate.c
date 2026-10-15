/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                          http://www.advancechip.com/
*
*                         FILE:   clb_ex8_external_signal_AND_gate.c
*                         TITLE:  CLB External Signal AND Gate.
*******************************************************************************************/


#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"


void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    //
    // Enabling CLB1
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB1);

	Board_init();

    initTILE1(myCLBTILE1_BASE);
    CLB_enableCLB(myCLBTILE1_BASE);


    while(1)
    {

        asm(" NOP");
    }
}


