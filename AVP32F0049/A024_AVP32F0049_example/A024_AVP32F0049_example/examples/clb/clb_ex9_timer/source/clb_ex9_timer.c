/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                          http://www.advancechip.com/
*
*                         FILE:   clb_ex9_timer.c
*                         TITLE:  CLB Timer.
*******************************************************************************************/


#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"

__interrupt void clb1ISR(void);

#define RESET_TIMER     1
#define ENABLE_TIMER    2
#define COUNTUP_MODE    4


void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_register(INT_CLB1, &clb1ISR);
    Interrupt_enable(INT_CLB1);

    //
    // Enabling CLB1
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB1);

    initTILE1(myTILE1_BASE);
    Board_init();

    CLB_setGPREG(myTILE1_BASE, ENABLE_TIMER | COUNTUP_MODE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    CLB_clearInterruptTag(myTILE1_BASE);


    while(1)
    {
        SysCtl_delay(20000000);
        // Change Timer Value to 2000
        CLB_configCounterLoadMatch(myTILE1_BASE, CLB_CTR0, 0, 2000, 0);
        SysCtl_delay(20000000);
        // Change Timer Value to 1000
        CLB_configCounterLoadMatch(myTILE1_BASE, CLB_CTR0, 0, 1000, 0);
        asm(" NOP");
    }
}


__interrupt void clb1ISR(void)
{
    GPIO_togglePin(myGPIO0);
    CLB_clearInterruptTag(myTILE1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

