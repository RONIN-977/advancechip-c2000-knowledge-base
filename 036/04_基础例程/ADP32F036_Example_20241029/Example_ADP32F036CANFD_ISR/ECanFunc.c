/*
 * ECanFunc.c
 *
 *  Created on: 2022Äê9ÔÂ26ÈÕ
 *      Author: liaohaitao
 */


#include "ADP32F03x_Device.h"        // DSP2803x Headerfile
#include "ECanFunc.h"

void canASendData(Uint32 data1,Uint32 data2,Uint16 mbnum)
{
    volatile struct MBOX *mboxp;

    mboxp = &ECanaMboxes.MBOX0;
    mboxp = mboxp + mbnum;

    mboxp->MDL.all = data1;
    mboxp->MDH.all = data2;

    ECanaRegs.CANTRS.all |= (0x00000001<<mbnum);
}

void sCANFDDataTx(Uint16 *txbuf,Uint16 len)
{
    Uint16 i;
    if(len > 64)
    {
        asm("  ESTOP0");
        return;
    }
    if(len <= 8)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = len;
    }
    else if(len == 12)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 9;
    }
    else if(len == 16)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 10;
    }
    else if(len == 20)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 11;
    }
    else if(len == 24)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 12;
    }
    else if(len == 32)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 13;
    }
    else if(len == 48)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 14;
    }
    else if(len == 64)
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 15;
    }
    else
    {
        CanfdRegs.TBUF.RIDST.bit.DLC = 0;
    }

    for(i=0; i<(len/2); i++)
    {
        CanfdRegs.TBUF.DATA[i] = txbuf[i];
    }

    CanfdRegs.CFG_STAT.bit.TPE = 1;         // start transmit
    CanfdRegs.CFG_STAT.bit.TSALL = 0;
}

Uint16 sCANFDTxStateGet(void)
{
    if(CanfdRegs.RTINTFE.bit.TPIF == 1)
    {
       CanfdRegs.RTINTFE.bit.TPIF = 1;
       return 1;
    }
    else
    {
        return 0;
    }
}

void canfdSendDataP(Uint32 data1, Uint32 data2)
{
    CanfdRegs.TBUF.DATA[0] = data1;
    CanfdRegs.TBUF.DATA[1] = data1>>16;
    CanfdRegs.TBUF.DATA[2] = data2;       // transmit data
    CanfdRegs.TBUF.DATA[3] = data2>>16;

    CanfdRegs.CFG_STAT.bit.TPE = 1;         // start transmit
//    CanfdRegs.CFG_STAT.bit.TSONE = 0;
    CanfdRegs.CFG_STAT.bit.TSALL = 0;

    while(CanfdRegs.RTINTFE.bit.TPIF == 0);     // wait for tansmit finish
    CanfdRegs.RTINTFE.bit.TPIF = 1;
}

Uint16 sCANFDDataRx(Uint16 *buf)
{
    Uint16 cnt,rxbytes;

    if(CanfdRegs.TCTRL.bit.RSTAT > 0)
    {
        cnt = CanfdRegs.RBUF.RIDST.bit.DLC;
        if(cnt <= 8)
        {
            rxbytes = cnt;
        }
        else if(cnt == 9)
        {
            rxbytes = 12;
        }
        else if(cnt == 10)
        {
            rxbytes = 16;
        }
        else if(cnt == 11)
        {
            rxbytes = 20;
        }
        else if(cnt == 12)
        {
            rxbytes = 24;
        }
        else if(cnt == 13)
        {
            rxbytes = 32;
        }
        else if(cnt == 14)
        {
            rxbytes = 48;
        }
        else if(cnt == 15)
        {
            rxbytes = 64;
        }
        for(cnt=0; cnt < (rxbytes/2); cnt++)
        {
            buf[cnt] = CanfdRegs.RBUF.DATA[cnt];
        }
        CanfdRegs.TCTRL.bit.RREL = 1;
        return rxbytes;
    }
    else
    {
        return 0;
    }
}

void canfdSendDataS(Uint32 data1, Uint32 data2)
{
    CanfdRegs.TBUF.DATA[0] = data1;
    CanfdRegs.TBUF.DATA[1] = data2;       // transmit data

    CanfdRegs.CFG_STAT.bit.TPE = 1;         // start transmit
    CanfdRegs.CFG_STAT.bit.TSONE = 0;
    CanfdRegs.CFG_STAT.bit.TSALL = 1;

    while(CanfdRegs.RTINTFE.bit.TSIF == 0);     // wait for tansmit finish
}

void canfdSendData_2(Uint16 data1)
{
    CanfdRegs.TBUF.DATA[0] = data1;

    CanfdRegs.CFG_STAT.bit.TPE = 1;         // start transmit

    while(CanfdRegs.RTINTFE.bit.TPIF == 0);     // wait for tansmit finish
}

