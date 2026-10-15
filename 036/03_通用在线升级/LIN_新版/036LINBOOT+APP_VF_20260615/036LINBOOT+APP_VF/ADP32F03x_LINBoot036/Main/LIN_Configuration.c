//-----------------------------------------------------------------------------
#include "MainInclude.h"

static Uint16 sdataA[8];
static Uint16 rdataA[8];
static Uint32 LinL0IntVect;
volatile Uint16 LinRxFlag = 0;
extern Uint16 testERnum;

void Lin_Initialize(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.LINAENCLK = 1;  // LINACLKEN

    // ADM036系列配置
    GpioCtrlRegs.GPAPUD.bit.GPIO14   = 0;  // Enable pull-up for GPIO14 (LIN TX)
    GpioCtrlRegs.GPAPUD.bit.GPIO15   = 0;  // Enable pull-up for GPIO15 (LIN RX)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 2;  // Asynch input GPIO15 (LINRXA)
    GpioCtrlRegs.GPAMUX1.bit.GPIO14  = 2;  // Configure GPIO14 for LIN TX operation
    GpioCtrlRegs.GPAMUX1.bit.GPIO15  = 2;  // Configure GPIO15 for LIN RX operation
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 2;

    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;    // LINEN
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
    GpioDataRegs.GPASET.bit.GPIO1= 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;
    GpioDataRegs.GPASET.bit.GPIO17  = 1; // LIN_EN
    EDIS;

    // 配置中断接收
    EALLOW;
    PieVectTable.LIN0INTA = &LINa_LEVEL_isr;
    EDIS;

    IER |= (M_INT9);                     // Enable interrupts
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
    EINT;
    ERTM;

    EALLOW;
    LinaRegs.SCIGCR0.bit.RESET = 0;      // Into reset
    LinaRegs.SCIGCR0.bit.RESET = 1;      // Out of reset

    LinaRegs.SCIGCR1.bit.SWnRST = 0;     // Into software reset

    LinaRegs.SCIGCR1.bit.CLK_MASTER = 0; // Slave mode
    LinaRegs.SCIGCR1.bit.LINMODE   = 1;  // LIN mode enabled
    LinaRegs.SCIGCR1.bit.COMMMODE  = 0;  // ID4/ID5 not used for length control
    LinaRegs.SCIGCR1.bit.PARITYENA = 0;  // No parity check
    LinaRegs.SCIGCR1.bit.ADAPT     = 1;//0;  // Fixed baudrate
    LinaRegs.SCIGCR1.bit.MBUFMODE  = 1;  // Buffered mode
    LinaRegs.SCIGCR1.bit.CTYPE     = 1;  // LIN 2.0 enhanced checksum
    LinaRegs.SCIGCR1.bit.HGENCTRL  = 1;  // ID filtering using ID-SlaveTask byte
    LinaRegs.SCIGCR1.bit.STOPEXTFRAME = 0;
    LinaRegs.SCIGCR1.bit.LOOPBACK  = 0;  // Normal mode
    LinaRegs.SCIGCR1.bit.CONT      = 1;  // Continue on suspend
    LinaRegs.SCIGCR1.bit.RXENA     = 1;  // Enable RX
    LinaRegs.SCIGCR1.bit.TXENA     = 1;  // Enable TX

    LinaRegs.SCIGCR2.bit.CC = 1;         // Compare checksum
    LinaRegs.SCIGCR2.bit.SC = 1;         // Send checksum

    LinaRegs.SCIFORMAT.bit.LENGTH = 7;   // Buffer depth = 8 bytes

    LinaRegs.BRSR.bit.SCI_LIN_PSL = 161; // 100M-19200 baud rate
    LinaRegs.BRSR.bit.M = 12;
    LinaRegs.MBRSR = 2500;               // 20kHz (max autobaud rate)//(50M/20K)



    LinaRegs.LINMASK.bit.RXIDMASK = 0xFF;
    LinaRegs.LINMASK.bit.TXIDMASK = 0xFF;

    LinaRegs.LINCOMP.bit.SBREAK = 0;
    LinaRegs.LINCOMP.bit.SDEL   = 0;

    LinaRegs.SCICLEARINTLVL.all = 0xFFFFFFFF;
    LinaRegs.SCISETINT.bit.SETRXINT = 1;
    LinaRegs.SCISETINT.bit.SETIDINT = 1;

    LinaRegs.SCISETINTLVL.bit.SETRXINTOVO = 0;
    LinaRegs.SCISETINTLVL.bit.SETIDINTLVL = 0;

    LinaRegs.SCIGCR1.bit.SWnRST = 1;     // Out of software reset
    EDIS;
}

interrupt void LINa_LEVEL_isr(void)
{
    LinL0IntVect = LinaRegs.SCIINTVECT0.all;

    if (LinL0IntVect == ID)
    {
        if (LinaRegs.LINID.bit.RECEIVEDID == 0x64)  // ID:0x24
        {
            LinaRegs.SCIFORMAT.bit.LENGTH = 7;
            LinaRegs.LINTD1.all = RemoteModbus.nLinTxData1;
            LinaRegs.LINTD0.all = RemoteModbus.nLinTxData0;
            RemoteModbus.TxBufferIndex = 1;

            // Fix fast programming bug (handshake issue)
            RemoteModbus.nLinTxData0 = 0;
            RemoteModbus.nLinTxData1 = 0;//testERnum;
        }
        else
        {
            LinaRegs.SCIFORMAT.bit.LENGTH = 7;
        }
    }
    else if (LinL0IntVect == Receive)
    {
        if (LinaRegs.LINID.bit.RECEIVEDID == 0xA3)  // ID:0x23
        {
            rdataA[0] = LinaRegs.LINRD0.bit.RD0;
            rdataA[1] = LinaRegs.LINRD0.bit.RD1;
            rdataA[2] = LinaRegs.LINRD0.bit.RD2;
            rdataA[3] = LinaRegs.LINRD0.bit.RD3;
            rdataA[4] = LinaRegs.LINRD1.bit.RD4;
            rdataA[5] = LinaRegs.LINRD1.bit.RD5;
            rdataA[6] = LinaRegs.LINRD1.bit.RD6;
            rdataA[7] = LinaRegs.LINRD1.bit.RD7;

            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex]     = rdataA[0] & 0xFF;
            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex + 1] = rdataA[1] & 0xFF;
            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex + 2] = rdataA[2] & 0xFF;
            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex + 3] = rdataA[3] & 0xFF;
            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex + 4] = rdataA[4] & 0xFF;
            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex + 5] = rdataA[5] & 0xFF;
            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex + 6] = rdataA[6] & 0xFF;
            RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex + 7] = rdataA[7] & 0xFF;

            if (RemoteModbus.ReceiveBuf[0] == 0xCC)
            {
                RemoteModbus.RxBufferIndex += 8;
                LinRxFlag = 1;
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
    else
    {
        // No action
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
