/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                          http://www.advancechip.com/
*
*                         FILE:   clb_ex6_siggen.c
*                         TITLE:  CLB Signal Generator.
*******************************************************************************************/


#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"


__interrupt void clb2ISR(void);

// GPREG settings:
// bit 2 = enable
// bit 1 = ISR end flag
// bit 0 not used
uint32_t gpreg1 = 1;


bool error_low = false;
bool error_hi  = false;


void main(void)
{
    Device_init();
    Device_initGPIO();

    //
    // map interrupts
    //
    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_register(INT_CLB2, &clb2ISR);


    //
    // initialize CLB tiles
    //
    SysCtl_enablePeripheral((SysCtl_PeripheralPCLOCKCR)0x0011);
    SysCtl_enablePeripheral((SysCtl_PeripheralPCLOCKCR)0x0111);

    Board_init();

    initTILE1(myTILE1_BASE);
    initTILE2(myTILE2_BASE);

    //
    // enable interrupts
    //
    Interrupt_enable(INT_CLB2);
    EINT;

    //
    // enable CLB counters
    //
    HWREG(0x3100) = 0x6;
    HWREG(0x3500) = 0x6;
    CLB_setGPREG(myTILE1_BASE, gpreg1);


    while(1)
    {
        asm(" NOP");
    }
}



__interrupt void clb2ISR(void)
{
        asm(" ESTOP0");

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

//
// end of file //
//
