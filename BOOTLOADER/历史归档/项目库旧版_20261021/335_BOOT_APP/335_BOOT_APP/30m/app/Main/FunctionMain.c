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

CPU_TIME_STRUCT         gCpuTime;


Uint16 NumberCtr;

void LED_blinky(void)
{
    NumberCtr++;
    if(NumberCtr >= 250)
    {
       NumberCtr = 0;
       GpioDataRegs.GPATOGGLE.bit.GPIO27 = 1;
    }
}


void Main0msFunction(void)
{
    UpgradePendingAction();
}


void Main05msFunction(void)
{

}




void Main2msFunction(void)
{
    LED_blinky();

//    RemoteModbus_MsgProcess();
}
