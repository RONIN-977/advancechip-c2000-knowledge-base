/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                          http://www.advancechip.com/
*
*                         FILE:   clb_ex13_push_pull.c
*                         TITLE:  CLB PUSH PULL.
*******************************************************************************************/


#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"

__interrupt void clb1ISR(void);

#define FRC_LOAD     0x1


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

    Board_init();

    initTILE1(myTILE1_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    CLB_clearInterruptTag(myTILE1_BASE);

    while(1)
    {
        SysCtl_delay(20000000);
        HWREG(myTILE1_BASE + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(0)) = 500U;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(1)) = 1000U;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(2)) = 2000U;
        CLB_setGPREG(myTILE1_BASE, FRC_LOAD);
        CLB_setGPREG(myTILE1_BASE, 0);
        SysCtl_delay(20000000);
        HWREG(myTILE1_BASE + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(0)) = 1000U;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(1)) = 2000U;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(2)) = 4000U;
        CLB_setGPREG(myTILE1_BASE, FRC_LOAD);
        CLB_setGPREG(myTILE1_BASE, 0);
        asm(" NOP");
    }
}


__interrupt void clb1ISR(void)
{


    CLB_clearInterruptTag(myTILE1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

