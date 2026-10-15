//-----------------------------------------------------------------------------
// Include
#include "MainInclude.h"

static Uint16 Lin_error = 0;
Uint16 testERnum;
/***************************************************************
-------------------------�����򲿷�-----------------------------
****************************************************************/
void main(void)
{
    Uint32 m_BaseTime, m_DetaTime;
    Uint16 m_LoopFlag;

    // Initialize the Device
    InitDevice();
    // Initialize Controller application Data
    InitAppData();
    // Initialize Flash API
    InitFlashAPI();
    // Enable the watchdog
    EnableDog();

    m_LoopFlag = 0;
    m_BaseTime = GetTime();
//  DisableDog();

    while (1)       // User Application function:
    {
        m_DetaTime = m_BaseTime - GetTime();
        if (m_DetaTime >= C_TIME_05MS)
        {
            ServiceDog();
            m_LoopFlag++;

            m_BaseTime -= C_TIME_05MS;

            Main05msFunction();
            if ((m_LoopFlag & 0x03) == 0)
            {
                gCpuTime.Function2MsBase = GetTime();
                Main2msFunction();
                gCpuTime.Function2Ms = gCpuTime.Function2MsBase - GetTime();
            }
        }

        Lin_error = (Uint16)((LinaRegs.SCIFLR.all >> 24) & 0xFF);
        if (Lin_error > 0)
        {
            EALLOW;
            LinaRegs.SCIGCR1.bit.SWnRST = 0;
            LinaRegs.SCIFLR.bit.BE   = 1;
            LinaRegs.SCIFLR.bit.CE   = 1;
            LinaRegs.SCIFLR.bit.PBE  = 1;
            LinaRegs.SCIFLR.bit.ISFE = 1;
            LinaRegs.SCIFLR.bit.NRE  = 1;
            LinaRegs.SCIFLR.bit.FE   = 1;
            LinaRegs.SCIFLR.bit.OE   = 1;
            LinaRegs.SCIFLR.bit.PE   = 1;
            LinaRegs.SCIGCR1.bit.SWnRST = 1;
            EDIS;
            RemoteModbus.nRecivedErrorFlag = 1;
            testERnum++;
        }
    }
}
