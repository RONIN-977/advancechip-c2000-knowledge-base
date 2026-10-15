/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  empty.c
*                         TITLE: CLB Empty Project
*******************************************************************************************/


#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"



void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();


    initTILE1(CLB1_BASE);
    CLB_enableCLB(CLB1_BASE);

    while(1)
    {

        asm(" NOP");
    }
}

