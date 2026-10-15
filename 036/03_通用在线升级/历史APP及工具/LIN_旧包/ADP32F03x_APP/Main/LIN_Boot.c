//#############################################################################
/*<FH>*************************************************************************
* Copyright (c) :  2021-2031, Advancechip Co., Ltd. All Rights Reserved
* Original Author: pei.shen

* FileName: SCI_Boot.c
* Current Version: V1.0
* Date: 21-Aug-2021

* ABSTRACT:  the SCI Function Initialize, the device is AVP32F335
* DESCRIPTION:

* Revise:
;   Last rev by:    ***
;   Last rev date:  ***
;   Last rev desc.: ***
**<FH>************************************************************************/

//-----------------------------------------------------------------------------
// Include

#include "MainInclude.h"

void Lin_Initialize(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.LINAENCLK    = 1;  // LINA
#if 1 //035
      GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO22 (LIN TX)
      GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;     // Enable pull-up for GPIO23 (LIN RX)
      GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO23 (LINRXA)
      GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;   // Configure GPIO19 for LIN TX operation  (3-Enable,0-Disable)
      GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;
#else //036

      //水泵板
      GpioCtrlRegs.GPAPUD.bit.GPIO14   = 0;  // Enable pull-up for GPIO18 (LIN TX)
      GpioCtrlRegs.GPAPUD.bit.GPIO15   = 0;  // Enable pull-up for GPIO19 (LIN RX)
      GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 2;  // Asynch input GPIO19 (LINRXA)
      GpioCtrlRegs.GPAMUX1.bit.GPIO14  = 2;  // Configure GPIO18 for LIN TX operation (2-Enable,0-Disable)
      GpioCtrlRegs.GPAMUX1.bit.GPIO15  = 2;  // Configure GPIO19 for LIN RX operation (2-Enable,0-Disable)
      GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 2;

      GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;
      GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;
      GpioDataRegs.GPADAT.bit.GPIO16 = 1;

/*
      //开发板
      GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;     // Enable pull-up for GPIO18 (LIN TX)
      GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;     // Enable pull-up for GPIO19 (LIN RX)
      GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (LINRXA)
      GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for LIN TX operation (2-Enable,0-Disable)
      GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for LIN RX operation (2-Enable,0-Disable)

      GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
      GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
      GpioDataRegs.GPASET.bit.GPIO3 = 1;
*/
#endif
      EDIS;

      //配置中断接收
      EALLOW;
      PieVectTable.LIN0INTA    = &LINa_LEVEL_isr;
      EDIS;

      IER |= (M_INT9);  //  Enable interrupts:
      PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
      EINT;
      ERTM;

      EALLOW;
      LinaRegs.SCIGCR0.bit.RESET = 0;       // Into reset
      LinaRegs.SCIGCR0.bit.RESET = 1;       // Out of reset

      LinaRegs.SCIGCR1.bit.SWnRST = 0;      // Into software reset

      LinaRegs.SCIGCR1.bit.CLK_MASTER = 0;  // Enable The node in slave mode
      LinaRegs.SCIGCR1.bit.LINMODE = 1;     // LIN mode is enabled
      LinaRegs.SCIGCR1.bit.COMMMODE = 0;    // ID4 and ID5 are not used for length control.
      LinaRegs.SCIGCR1.bit.PARITYENA = 0;   // No Parity Check, ID-parity verification is Unenabled
      LinaRegs.SCIGCR1.bit.ADAPT = 1;       // Automatic baudrate adjustment is disabled
      LinaRegs.SCIGCR1.bit.MBUFMODE = 1;    // Buffered Mode
      LinaRegs.SCIGCR1.bit.CTYPE = 1;       // LIN 2.0 Enhanced checksum used
      LinaRegs.SCIGCR1.bit.HGENCTRL = 1;    // ID filtering using ID-SlaveTask byte
      LinaRegs.SCIGCR1.bit.STOPEXTFRAME = 0;// Stop extended frame communication.
      LinaRegs.SCIGCR1.bit.LOOPBACK = 0;    // External Loopback 正常模式，非自测模式
      LinaRegs.SCIGCR1.bit.CONT = 1;        // Continue on Suspend
      LinaRegs.SCIGCR1.bit.RXENA = 1;       // Enable RX
      LinaRegs.SCIGCR1.bit.TXENA = 1;       // Enable TX

      LinaRegs.SCIGCR2.bit.CC = 1;          // compare checksum
      LinaRegs.SCIGCR2.bit.SC = 1;          // send checksum

      LinaRegs.SCIFORMAT.bit.LENGTH = 7;   // （LENGTH+1）个字节深度的BUFFER，
                                           // 意味着ADP32F035的 LIN-SCI接收到（LENGTH+1） 个字节之后，进入中断服务函数???

      // 从机自动识别主机波特率
      LinaRegs.BRSR.bit.SCI_LIN_PSL = 194;  // 9600 Baud Rate
      LinaRegs.BRSR.bit.M = 0;              // 11
      LinaRegs.MBRSR = 92;                  // 20kHz (max autobaud rate)


      LinaRegs.LINMASK.bit.RXIDMASK = 0xFF;
      LinaRegs.LINMASK.bit.TXIDMASK = 0xFF; // When HGENCTRL is set to 1, this field must be set to 0xFF

      LinaRegs.LINCOMP.bit.SBREAK = 0;      // TSYNBRK = 13Tbit + SBREAK x Tbit
      LinaRegs.LINCOMP.bit.SDEL = 0;        // TSDEL = (SDEL + 1)Tbit

      //Set interrupt priority
      LinaRegs.SCICLEARINTLVL.all = 0xFFFFFFFF;     // Set Int level of all interrupts to LVL 0 设置中断级别

      LinaRegs.SCISETINT.bit.SETRXINT = 1;          // Enable RX interrupt
      LinaRegs.SCISETINT.bit.SETIDINT = 1;          // Enable ID interrupt

      // LIN mode only. This bit is set when there has been an inconsistent Synch Field error detected by
      // the Synchronizer during Header reception. See section 1.5.2.3, "Header Reception and Adaptive
      // Baudrate" , for more information. The Inconsistent Synch Field Error flag is cleared by reading the
      // corresponding interrupt offset in the SCIINTVECT0/1 register,
      LinaRegs.SCIFLR.bit.PE  = 1;             // 24     Parity Error Flag
      LinaRegs.SCIFLR.bit.OE  = 1;             // 25     Overrun Error Flag
      LinaRegs.SCIFLR.bit.FE  = 1;             // 26     Framing Error Flag
      LinaRegs.SCIFLR.bit.NRE = 1;             // 27     No-Response Error Flag (LIN only)
      LinaRegs.SCIFLR.bit.ISFE= 1;             // 28     Inconsistent Synch Field Error Flag (LIN only)
      LinaRegs.SCIFLR.bit.CE  = 1;             // 29     Checksum Error Flag (LIN only)
      LinaRegs.SCIFLR.bit.PBE = 1;             // 30     Physical Bus Error Flag (LIN only)
      LinaRegs.SCIFLR.bit.BE  = 1;             // 31     Bit Error Flag (LIN only)

      LinaRegs.SCISETINTLVL.bit.SETRXINTOVO = 0;
      LinaRegs.SCISETINTLVL.bit.SETIDINTLVL = 0;    // 清除中断级别

      LinaRegs.SCIGCR1.bit.SWnRST = 1;              // bring out of software reset
      EDIS;
}

Uint16 sdataA[8];
Uint16 rdataA[8];
Uint32 LinL0IntVect;
Uint16 LinRxFlag = 0;
Uint16 error = 0;
Uint32 txdata = 0;
interrupt void LINa_LEVEL_isr(void)
{
    LinL0IntVect = LinaRegs.SCIINTVECT0.all;
    if(LinL0IntVect == 4)
    {
        if(LinaRegs.LINID.bit.RECEIVEDID == 0x64) //ID:0x24
        {
//            Delay_nUS(400);
            LinaRegs.SCIFORMAT.bit.LENGTH = 7;//3;
            LinaRegs.LINTD1.all = 0;//RemoteModbus.nLinTxData1;
            LinaRegs.LINTD0.all = 0;//RemoteModbus.nLinTxData0;
            RemoteModbus.TxBufferIndex = 1;
        }
        else
        {
            LinaRegs.SCIFORMAT.bit.LENGTH = 7;
        }
    }
    else if(LinL0IntVect == 11)
    {
        if(LinaRegs.LINID.bit.RECEIVEDID == 0xA3) //ID:0x23
        {
            rdataA[0] = LinaRegs.LINRD0.bit.RD0;
            rdataA[1] = LinaRegs.LINRD0.bit.RD1;
            rdataA[2] = LinaRegs.LINRD0.bit.RD2;
            rdataA[3] = LinaRegs.LINRD0.bit.RD3;
            rdataA[4] = LinaRegs.LINRD1.bit.RD4;
            rdataA[5] = LinaRegs.LINRD1.bit.RD5;
            rdataA[6] = LinaRegs.LINRD1.bit.RD6;
            rdataA[7] = LinaRegs.LINRD1.bit.RD7;

            RemoteModbus.ReceiveBuf[0] = rdataA[0] & 0xFF;
            RemoteModbus.ReceiveBuf[1] = rdataA[1] & 0xFF;
            RemoteModbus.ReceiveBuf[2] = rdataA[2] & 0xFF;
            RemoteModbus.ReceiveBuf[3] = rdataA[3] & 0xFF;
            RemoteModbus.ReceiveBuf[4] = rdataA[4] & 0xFF;
            RemoteModbus.ReceiveBuf[5] = rdataA[5] & 0xFF;
            RemoteModbus.ReceiveBuf[6] = rdataA[6] & 0xFF;
            RemoteModbus.ReceiveBuf[7] = rdataA[7] & 0xFF;

            if(RemoteModbus.ReceiveBuf[0] == 0xCC)
            {
                RemoteModbus.RxBufferIndex+=8;
                LinRxFlag= 1;
            }
            else
            {
                RemoteModbus.RxBufferIndex = 0;
            }

        }
        else
        {
            sdataA[0] = LinaRegs.LINRD0.bit.RD0;
            sdataA[1] = LinaRegs.LINRD0.bit.RD1;
            sdataA[2] = LinaRegs.LINRD0.bit.RD2;
            sdataA[3] = LinaRegs.LINRD0.bit.RD3;
            sdataA[4] = LinaRegs.LINRD1.bit.RD4;
            sdataA[5] = LinaRegs.LINRD1.bit.RD5;
            sdataA[6] = LinaRegs.LINRD1.bit.RD6;
            sdataA[7] = LinaRegs.LINRD1.bit.RD7;
        }
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
