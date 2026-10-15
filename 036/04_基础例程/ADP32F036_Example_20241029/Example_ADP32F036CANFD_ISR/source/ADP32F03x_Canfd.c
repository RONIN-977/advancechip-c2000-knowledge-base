//###########################################################################
//
// FILE:    DSP2803x_Canfd.c
//
// TITLE:   DSP2803x Enhanced CANFD Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2803x C/C++ Header Files and Peripheral Examples V130 $
// $Release Date: May  8, 2015 $
// $Copyright: Copyright (C) 2009-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "ADP32F03x_Device.h"     // DSP28 Headerfile Include File
#include "ADP32F03x_Examples.h"   // DSP28 Examples Include File

//---------------------------------------------------------------------------
// InitCanfd:
//---------------------------------------------------------------------------
// This function initializes the eCAN module to a known state.
//
Uint16 rdata;
void InitCanfd(void)
{
    CanfdRegs.CFG_STAT.bit.RESET = 1;

    CanfdRegs.CANCFG.bit.CAN_CLK_SEL = 1;   // 0:sysclk;1:pllclk
    // 500KHZ@120MHzCANHD_CLK
    CanfdRegs.S_CFG.bit.S_PRESC = 9;
    CanfdRegs.S_SEG.bit.S_Seg_1 = 17;
    CanfdRegs.S_SEG.bit.S_Seg_2 = 4;
    //    CanfdRegs.S_CFG.bit.S_SJW = 0;
    // 2MHZ@120MHzCANHD_CLK
    CanfdRegs.F_CFG.bit.F_PRESC = 3;
    CanfdRegs.F_SEG.bit.F_Seg_1 = 9;
    CanfdRegs.F_SEG.bit.F_Seg_2 = 3;
    //    CanfdRegs.F_CFG.bit.F_SJW = 0;

    CanfdRegs.CFG_STAT.bit.RESET = 0;

    CanfdRegs.CFG_STAT.bit.TBSEL = 0;       // select PTB
    CanfdRegs.CFG_STAT.bit.TPE = 0;         // no transfer to PTB
    CanfdRegs.CFG_STAT.bit.TSONE = 0;
    CanfdRegs.CFG_STAT.bit.TSALL = 0;
    //CanfdRegs.CFG_STAT.bit.LBME = 1;      //外回环使能
    // CanfdRegs.CFG_STAT.bit.LBMI = 1;      // 0:disable internal loopback mode ; 1: enable internal loopback mode

    CanfdRegs.TBUF.RID0.bit.ID0 = 0x123;    // standard frame ID
    CanfdRegs.TBUF.RID0.bit.ID1 = 0x0;
    CanfdRegs.TBUF.RID1.bit.ID = 0;
    CanfdRegs.TBUF.RID1.bit.ESI = 0;

    CanfdRegs.TBUF.RIDST.bit.IDE = 0;        // 0: standard frame ID; 1: extended frame ID
    CanfdRegs.TBUF.RIDST.bit.RTR = 0;        // 0: data frame; 1: remote frame(for can)
    CanfdRegs.TBUF.RIDST.bit.FDF_EDL = 1;    // 0: can 2.0; 1:can_fd
    CanfdRegs.TBUF.RIDST.bit.BRS = 0;        //

    CanfdRegs.TBUF.RIDST.bit.KOER = 0;

    //CanfdRegs.RTINTFE.bit.TPIE = 0;         // enable interrupt

    CanfdRegs.RBUF.RID0.bit.ID0 = 0x15A;    // standard frame ID
    CanfdRegs.RBUF.RID0.bit.ID1 = 0x0;
    CanfdRegs.RBUF.RID1.bit.ID = 0;
    CanfdRegs.RBUF.RID1.bit.ESI = 0;

    CanfdRegs.RBUF.RIDST.bit.IDE = 0;        // 0: standard frame ID; 1: extended frame ID
    CanfdRegs.RBUF.RIDST.bit.RTR = 0;        // 0: data frame; 1: remote frame(for can)
    CanfdRegs.RBUF.RIDST.bit.FDF_EDL = 0;    // 0: can 2.0; 1:can_fd
    CanfdRegs.RBUF.RIDST.bit.BRS = 0;        //

    CanfdRegs.RBUF.RIDST.bit.KOER = 0;

   // CanfdRegs.TCTRL.bit.SACK = 1; //自动应答
}

void InitCanfdGpio(void)
{
//    EALLOW;
//    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;     // CANFD_Rx
//    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 1;     // CANFD_Tx
//
//    AnalogRegs.PIN_MUX_FLASH.bit.canfd_gpio_mux = 5;   //GPIO0 CANFDRXD ; GPIO32 CANFDTXD;
//    EDIS;

    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;     // CANFD_Rx
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 1;     // CANFD_Tx
    AnalogRegs.PIN_MUX_FLASH.bit.canfd_gpio_mux = 0x5;   //GPIO0 CANFDRXD ; GPIO32 CANFDTXD;
    EDIS;
}


/* Note: Bit timing parameters must be chosen based on the network parameters such as
   the sampling point desired and the propagation delay of the network. The propagation
   delay is a function of length of the cable, delay introduced by the
   transceivers and opto/galvanic-isolators (if any).

   The parameters used in this file must be changed taking into account the above mentioned
   factors in order to arrive at the bit-timing parameters suitable for a network.
*/

//===========================================================================
// End of file.
//===========================================================================
