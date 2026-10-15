/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex1_combinatorial_logic.c
*                         TITLE: CLB Combinational Logic
*******************************************************************************************/




#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"

void initCLB1(void);


uint32_t mode = 0;


void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

	SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB1);

    Board_init();

    initTILE1(myTILE1_BASE);

    CLB_setGPREG(myTILE1_BASE, mode);

    CLB_setOutputMask(myTILE1_BASE, 0x5, true);

    Interrupt_enable(INT_EPWM1);
    EINT;

    while(1)
    {
        asm(" NOP");
    }
}


__interrupt void INT_myEPWM1_ISR(void)
{
    EPWM_clearEventTriggerInterruptFlag(myEPWM1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
    CLB_setGPREG(myTILE1_BASE, mode & 3UL);
    asm(" NOP");
}
