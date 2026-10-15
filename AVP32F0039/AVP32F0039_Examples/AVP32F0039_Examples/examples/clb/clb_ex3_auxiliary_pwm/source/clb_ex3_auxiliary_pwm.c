/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex3_auxiliary_pwm.c
*                         TITLE: CLB Auxilary PWM
*******************************************************************************************/



#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"

__interrupt void clb1ISR(void);

#define ENABLE_AUX_PWM      1
volatile uint16_t dutyValue=200;

void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_register(INT_CLB1, &clb1ISR);
    Interrupt_enable(INT_CLB1);

	SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB1);

    Board_init();

    initTILE1(myTILE1_BASE);

    CLB_setGPREG(myTILE1_BASE, ENABLE_AUX_PWM);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    CLB_clearInterruptTag(myTILE1_BASE);


    while(1)
    {
        asm(" NOP");
    }
}

__interrupt void clb1ISR(void)
{
    GPIO_togglePin(myGPIO0);      // Debug: Used to determine if interrupt is being set

    //
    // Change duty cycle of PWM signal at the end of a period
    //
    if (dutyValue == 150)
    {
        dutyValue = 100U;
    }
    else
    {
        dutyValue = 150U;
    }

    //
    // Load new duty cycle value in HLC Registers
    //
    CLB_setHLCRegisters(myTILE1_BASE, dutyValue, 0, 0, 0);

    CLB_clearInterruptTag(myTILE1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

