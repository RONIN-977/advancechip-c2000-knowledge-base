//#############################################################################
//
// FILE:   Boot_main.c
//
// TITLE:  CAN bootloader main entry.
//
// Description:
// This file contains the bootloader main entry, CAN transceiver enable helper,
// and the update trigger loop used to hand off into the upgrade process.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include <ADP32F03x_Project.h>
#include "CANFunc.h"

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;

extern Uint16 CanUpgradeFinishFlag;

/*============================================================================*/
/*                           FUNCTION DECLARATIONS                            */
/*============================================================================*/
void InitECan(void);
extern Uint32 UpdateProcess(void);

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Enable the external CAN transceiver.
//!
//! This function configures the standby control pin used to place the CAN
//! transceiver into its active operating mode.
//!
//! \return None.
//
//*****************************************************************************
void CANTransceiverEnable(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1; // Enable the CAN transceiver
    EDIS;
}

//*****************************************************************************
//
//! Run the CAN bootloader main loop.
//!
//! This function initializes the system, flash, interrupt controller, and CAN
//! mailbox resources before entering the firmware update polling loop.
//!
//! \return None.
//
//*****************************************************************************
void main(void)
{ 
    // Copy the ramfuncs section from its flash load address to the RAM run
    // address before any RAM-resident routine is called.
    memcpy((Uint16 *)&RamfuncsRunStart,
            (Uint16 *)&RamfuncsLoadStart, 
            (unsigned long)&RamfuncsLoadSize);

    // Initialize System Control:
    InitSysCtrl();

    // Initialize GPIO.
    InitECanGpio();
    CANTransceiverEnable();

    // Clear all interrupts and initialize the PIE vector table.
    DINT;
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags.
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with shell interrupt pointers.
    InitPieVectTable();
    InitFlash();

    // Initialize eCAN-A module
    InitECana(); 
    CanTransmit_MailBoxInit(2, 0x1E1E1E1E, 1, 8);
    CanReceive_MailBoxInit(15, 0x1F1F1F1F, 1, 800);

    CanMailBox_MsgData_Struct bootMsg;

    bootMsg.mMailBox_DataL = 0xAA00AA00;
    bootMsg.mMailBox_DataH = 0xAA00AA00;

    while(1)
    {
        if((ECanaMboxes.MBOX15.MDL.all == 0x00AA00AA)
            &&(ECanaMboxes.MBOX15.MDH.all == 0x00AA00AA))
        {
            CanMsgDataClr(15);
            CanTranMsg(2, &bootMsg);
            UpdateProcess();
        }

        if(CanUpgradeFinishFlag == 1) // CAN upgrade completed
        {
            // Trigger a soft reset
            // Startup assembly checks the flag and branches accordingly.
            EnableDog();
            while(1);
        }
    }
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

