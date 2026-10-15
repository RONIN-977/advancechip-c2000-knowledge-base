/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                          http://www.advancechip.com/
*
*                         FILE:   clb_ex21_clockprescalar_nmi.c
*                         TITLE:  CLB Clock Prescalar and NMI
*******************************************************************************************/


//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "clb_config.h"


__interrupt void clb1ISR(void);
__interrupt void clb1NMIISR(void);

//
// Main
//
void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_register(INT_CLB1, &clb1ISR);
    Interrupt_register(INT_NMI, &clb1NMIISR);
    Interrupt_enable(INT_CLB1);
    Interrupt_enable(INT_NMI);

    SysCtl_enableNMIGlobalInterrupt();


    Board_init();

    //
    // Clock prescalar as IN0
    // HLC will generate interrupt on IN0 rising edge
    //
    initTILE1(myCLBForTILE1_BASE);

    CLB_enableCLB(myCLBForTILE1_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    while(1)
    {

    }
}


__interrupt void clb1ISR(void)
{
    CLB_clearInterruptTag(CLB1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

__interrupt void clb1NMIISR(void)
{

    if (SYSCTL_NMI_CLBNMI & SysCtl_getNMIFlagStatus())
    {
        //
        // CLB NMI
        //
        GPIO_togglePin(myGPIOOutput0);

    }
    SysCtl_clearAllNMIFlags();
}

//
// End of File
//
