/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                          http://www.advancechip.com/
*
*                         FILE:   clb_ex7_state_machine.c
*                         TITLE:  CLB State Machine.
*******************************************************************************************/


#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"


#define UPDATE_PWM_COMPLETED_TAG    1
#define TRIGGER_PWM_UPDATE_SHIFT    2
volatile uint16_t canUpdate = 1;

volatile uint32_t clbPwmPeriod = 2000;
volatile uint32_t clbPwmDuty = 1000;
volatile uint16_t clbPwmUpdateNow = 0;

__interrupt void clb1ISR(void);
void updateClbPwm(uint32_t period, uint32_t duty);

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
        if (clbPwmUpdateNow)
        {
            updateClbPwm(clbPwmPeriod, clbPwmDuty);
            clbPwmUpdateNow = 0;
        }
        asm(" NOP");
    }
}

__interrupt void clb1ISR(void)
{
    uint16_t tag = CLB_getInterruptTag(myTILE1_BASE);
    if (tag == UPDATE_PWM_COMPLETED_TAG)
    {
        canUpdate = 1;
        CLB_setGPREG(myTILE1_BASE, 0);
    }

    CLB_clearInterruptTag(myTILE1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

void updateClbPwm(uint32_t period, uint32_t duty)
{
    if (canUpdate)
    {
        canUpdate = 0;
        CLB_writeInterface(myTILE1_BASE, CLB_ADDR_COUNTER_1_LOAD, period);
        HWREG(myTILE1_BASE + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(0)) = period;
        HWREG(myTILE1_BASE + CLB_DATAEXCH + CLB_O_PULL(1)) = duty;
        CLB_setGPREG(myTILE1_BASE, 1 << TRIGGER_PWM_UPDATE_SHIFT);
    }
}
