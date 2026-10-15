//#############################################################################
/*<FH>*************************************************************************
* Copyright (c) :  2021-2031, Advancechip Co., Ltd. All Rights Reserved
* Original Author: pei.shen

* FileName: FunctionMain.c
* Current Version: V1.0
* Date: 21-Aug-2021

* ABSTRACT:  the function
* DESCRIPTION:

* Revise:
;   Last rev by:    ***
;   Last rev date:  ***
;   Last rev desc.: ***
**<FH>************************************************************************/

//-----------------------------------------------------------------------------
// Include

#include "MainInclude.h"

CPU_TIME_STRUCT   gCpuTime;

static void SelectMode_Boot(void);
static void JumpApp_DoAction(void);

void LED_blinky(void)
{
    int  tmp=0;
    gCpuTime.nLEDSumCnt ++;
    if(gCpuTime.nLEDSumCnt >= 250)
    {
       gCpuTime.nLEDSumCnt = 0;
       GpioDataRegs.GPATOGGLE.bit.GPIO24 = 1;
       tmp++;
       tmp++;
       GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1;
       tmp++;
       tmp++;
       GpioDataRegs.GPATOGGLE.bit.GPIO27 = 1;
       tmp++;
       tmp++;
       GpioDataRegs.GPATOGGLE.bit.GPIO26 = 1;
    }
}


void SelectMode_Boot(void)
{
    if(gCpuTime.nBootMode == 0)    //SCI引导
    {
        SCI_Boot();
    }else if(gCpuTime.nBootMode == 2)    //CAN引导
    {

    }
    else{

    }
}


void Main05msFunction(void)
{
     SelectMode_Boot();
}


// 从Boot跳转到APP实现方法
// asm("    MOVL XAR7, #0x300000"); //0x300000是APP程序的入口地址
// asm("    LB *XAR7");

void JumpApp_DoAction(void)
{
    //当nBootFlag == 0xAA 表示已经有APP程序，不需要升级直接跳APP。
    //在延时跳转期间接收到上位机的升级指令nBootFlag = 0,Boot程序进入升级流程。

    gCpuTime.nBootFlag = *(Uint16 *)Boot_FlagAdd;

    if((gCpuTime.nBootFlag == 0xAA)&&(RemoteModbus.nAppJumpFlag == 1))
    {
        gCpuTime.nBootSumCnt++;

        if(gCpuTime.nBootSumCnt >= 1000)
        {
           gCpuTime.nBootSumCnt = 0;
           (*Jump_App)();
        }
    }
}


void Main2msFunction(void)
{
    LED_blinky();

    JumpApp_DoAction();
}
