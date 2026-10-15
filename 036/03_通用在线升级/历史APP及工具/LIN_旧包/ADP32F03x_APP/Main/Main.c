//#############################################################################
/*<FH>*************************************************************************
* Copyright (c) :  2021-2031, Advancechip Co., Ltd. All Rights Reserved
* Original Author: pei.shen

* FileName: main.c
* Current Version: V1.0
* Date: 21-Aug-2021

* ABSTRACT:  the main Function Initialize, the device is AVP32F335
* DESCRIPTION:

* Revise:
;   Last rev by:    ***
;   Last rev date:  ***
;   Last rev desc.: ***
**<FH>************************************************************************/

//-----------------------------------------------------------------------------
// Include

#include "MainInclude.h"

#pragma DATA_SECTION(Bootflag, "BOOTFLAG");
const unsigned int Bootflag = 0xAA;

Uint16 error1 = 0;
Uint16 testERnum = 0;
/***************************************************************
-------------------------主程序部分-----------------------------
****************************************************************/
void main(void)
{
    Uint32 m_BaseTime,m_DetaTime;
	Uint16 m_LoopFlag;

    //Initialize the Device
    InitDevice();

    //Initialize Controller application Data
    InitAppData();

    //Enable the watchdo
    EnableDog();

    //Enable interrupts.
    //SetInterruptEnable();

	m_LoopFlag = 0;
	m_BaseTime = GetTime();

  	while(1)       // Step 6. User Application function:
   	{
		m_DetaTime = m_BaseTime - GetTime();
		if(m_DetaTime >= C_TIME_05MS)
        {
			m_LoopFlag ++;			
			m_BaseTime -= C_TIME_05MS;

			WDogService();
 			Main05msFunction();

			if((m_LoopFlag & 0x03) == 0)
			{
				gCpuTime.Function2MsBase = GetTime();
				Main2msFunction();
				gCpuTime.Function2Ms = gCpuTime.Function2MsBase - GetTime();
			}
        }
		Main0msFunction();

		error1 = ((LinaRegs.SCIFLR.all & 0xFF000000)>>24);
        if(error1 > 0)
        {
            LinaRegs.SCIFLR.bit.BE = 1;
            LinaRegs.SCIFLR.bit.PBE = 1;
            LinaRegs.SCIFLR.bit.CE = 1;
            LinaRegs.SCIFLR.bit.ISFE = 1;
            LinaRegs.SCIFLR.bit.NRE = 1;
            LinaRegs.SCIFLR.bit.FE = 1;
            LinaRegs.SCIFLR.bit.OE = 1;
            LinaRegs.SCIFLR.bit.PE = 1;
            testERnum++;
        }
   	}
}

