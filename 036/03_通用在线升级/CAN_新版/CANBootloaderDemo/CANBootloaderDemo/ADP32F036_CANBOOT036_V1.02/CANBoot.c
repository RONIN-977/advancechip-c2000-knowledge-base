//#############################################################################
//
// FILE:   CANBoot.c
//
// TITLE:  CAN bootloader programming functions.
//
// Description:
// This file implements the CAN bootloader programming flow, flash callback
// helpers, and transport data handling used during firmware update.
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
#include <ADP32F03x_Device.h>
#include <FlashADP32F03x_API_Library.h>
#include "CANFunc.h"

/*============================================================================*/
/*                              TYPEDEFS & ENUMS                              */
/*============================================================================*/
typedef struct {
     Uint16 *StartAddr;
     Uint16 *EndAddr;
} SECTOR;

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
#define OTP_START_ADDR  0x3D7800
#define OTP_END_ADDR    0x3D7BFF
#define OTP_ENTRY_POINT   0x3D7800
#define FLASH_START_ADDR  0x3E8000
#define FLASH_END_ADDR    0x3F7FFF
#define FLASH_ENTRY_POINT 0x3F7FF6
#define RAM_ENTRY_POINT   0x000000
#define EIGHT_BIT_HEADER   0x08AA
#define PROG_BUFFER_LENGTH 0x80  //0x100

/*============================================================================*/
/*                          GLOBAL DATA DEFINITIONS                           */
/*============================================================================*/
FLASH_ST FlashStatus;
Uint16 progBuf[PROG_BUFFER_LENGTH]; //Programming Buffer
Uint32 CANBoot_CallbackCounter = 0; // Just increment a counter in the callback function

Uint16 CanUpgradeFinishFlag = 0; // CAN upgrade completion flag

/* Linker-provided RAM relocation symbols */
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/


//*****************************************************************************
//
//! Copy a block of code or data into RAM.
//!
//! \param SourceAddr points to the first source word.
//! \param SourceEndAddr points one word past the end of the source range.
//! \param DestAddr points to the destination buffer in RAM.
//!
//! This function copies a contiguous word range, typically to move routines
//! from flash into RAM before execution.
//!
//! \return None.
//
//*****************************************************************************
void CANBoot_MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
       *DestAddr++ = *SourceAddr++;
    }
    return;
}

//*****************************************************************************
//
//! Run the flash API callback routine.
//!
//! This function executes from RAM and provides a lightweight callback hook
//! while flash erase or program operations are in progress.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(CANBoot_CallbackFunction,"ramfuncs");
void CANBoot_CallbackFunction(void)
{
    // Toggle pin, service external watchdog etc
    CANBoot_CallbackCounter++;
    asm("    NOP");
}

//*****************************************************************************
//
//! Halt bootloader execution after a flash or transport error.
//!
//! \param Status is the flash API or bootloader status code associated with
//! the failure.
//!
//! This function stops execution in a debug-friendly state when the bootloader
//! detects an unrecoverable error.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(CANBoot_Error,"ramfuncs");
void CANBoot_Error(Uint16 Status)
{
    //TODO it returns a serial error and reboot device
    asm("    ESTOP0");
    asm("    SB 0, UNC");
   // asm("    NOP");
}

//*****************************************************************************
//
//! Read one 16-bit word from the CAN receive mailbox.
//!
//! This function fetches two bytes from the CAN transport stream and combines
//! them into one 16-bit value in LSB-first order.
//!
//! \return The next 16-bit word received from the host.
//
//*****************************************************************************
#pragma CODE_SECTION(CANBoot_GetWordData,"ramfuncs");
Uint16 CANBoot_GetWordData(void)
{
    Uint16 wordData;
    Uint16 byteData;

    wordData = 0x0000;
    byteData = 0x0000;

    while(ECanaRegs.CANRMP.all == 0) { }

    // Fetch the LSB
    wordData =  (Uint16) ECanaMboxes.MBOX15.MDL.byte.BYTE0;   // LS byte

    // Fetch the MSB
    byteData =  (Uint16)ECanaMboxes.MBOX15.MDL.byte.BYTE1;    // MS byte

    // form the wordData from the MSB:LSB
    wordData |= (byteData << 8);

    ECanaRegs.CANRMP.all = 0xFFFFFFFF;

    return wordData;

}

//*****************************************************************************
//
//! Read one 32-bit value from the CAN transport stream.
//!
//! This function assembles two sequential 16-bit words into one 32-bit value.
//!
//! \return The next 32-bit value received from the host.
//
//*****************************************************************************
#pragma CODE_SECTION(CANBoot_GetLongData,"ramfuncs");
Uint32 CANBoot_GetLongData(void)
{
    Uint32 longData;

    // Fetch the upper 1/2 of the 32-bit value
    longData = ( (Uint32)CANBoot_GetWordData() << 16);

    // Fetch the lower 1/2 of the 32-bit value
    longData |= (Uint32)CANBoot_GetWordData();

    return longData;
}

//*****************************************************************************
//
//! Read and discard the reserved header words.
//!
//! This function consumes the eight reserved words in the incoming boot image
//! header so later payload parsing begins at the correct offset.
//!
//! \return None.
//
//*****************************************************************************
//#pragma CODE_SECTION(CANBoot_ReadReservedFn,"ramfuncs");
void CANBoot_ReadReservedFn(void)
{
    Uint16 i;
    // Read and discard the 8 reserved words.
    for(i = 1; i <= 8; i++)
    {
        CANBoot_GetWordData();
    }
    return;
}

//*****************************************************************************
//
//! Erase the application flash sectors.
//!
//! This function copies the RAM functions, unlocks the code security module,
//! erases the configured application flash sectors, and then enables the
//! watchdog before stopping in a loop.
//!
//! \return None.
//
//*****************************************************************************
//#pragma CODE_SECTION(AppFlashErase,"ramfuncs");
void AppFlashErase(void)
{
    DisableDog();
    DINT;

    memcpy((Uint16 *)&RamfuncsRunStart,(Uint16 *)&RamfuncsLoadStart, (unsigned long)&RamfuncsLoadSize);

    CsmUnlock();

    Flash_Erase((SECTORB | SECTORC | SECTORD | SECTORE | SECTORF | SECTORG | SECTORH), &FlashStatus);

    EnableDog();
    while(1);
}

//*****************************************************************************
//
//! Copy incoming CAN boot data blocks into flash.
//!
//! This function reads block headers and payload words from the transport
//! stream, buffers the data, programs the destination flash ranges, and sets
//! the upgrade completion flag when all blocks are written.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(CANBoot_CopyData,"ramfuncs");
void CANBoot_CopyData(void)
{
  Uint16  i,j;
  Uint16  Status;
  Uint16  WordData;
  struct HEADER {
    Uint16 BlockSize;
    Uint32 DestAddr;
    Uint32 ProgBuffAddr;
  } BlockHeader;
  // Get the size in words of the first block
  BlockHeader.BlockSize = CANBoot_GetWordData();

  // While the block size is > 0 copy the data
  // to the DestAddr.  There is no error checking
  // as it is assumed the DestAddr is a valid
  // memory location
  while(BlockHeader.BlockSize != (Uint16)0x0000)
  {
     if(BlockHeader.BlockSize > PROG_BUFFER_LENGTH)
     {
         //Block is to big to fit into our buffer so we must program it in chunks
         BlockHeader.DestAddr = CANBoot_GetLongData();
         //Program as many full buffers as possible
         for(j = 0; j < (BlockHeader.BlockSize / PROG_BUFFER_LENGTH); j++)
         {
             BlockHeader.ProgBuffAddr = (Uint32)progBuf;
             for(i = 1; i <= PROG_BUFFER_LENGTH; i++)
             {
                 WordData = CANBoot_GetWordData();
                 *(Uint16 *)BlockHeader.ProgBuffAddr++ = WordData;
             }
             Status = Flash_Program((Uint16 *) BlockHeader.DestAddr, (Uint16 *)progBuf, PROG_BUFFER_LENGTH, &FlashStatus);
             if(Status != STATUS_SUCCESS)
             {
                 CANBoot_Error(Status);
             }
             BlockHeader.DestAddr += PROG_BUFFER_LENGTH;
         }

         if(0 != (BlockHeader.BlockSize % PROG_BUFFER_LENGTH))
         {
             //Program the leftovers
             BlockHeader.ProgBuffAddr = (Uint32)progBuf;
             for(i = 1; i <= (BlockHeader.BlockSize % PROG_BUFFER_LENGTH); i++)
             {
                 WordData = CANBoot_GetWordData();
                 *(Uint16 *)BlockHeader.ProgBuffAddr++ = WordData;
             }
             Status = Flash_Program((Uint16 *) BlockHeader.DestAddr, (Uint16 *)progBuf, (BlockHeader.BlockSize % PROG_BUFFER_LENGTH), &FlashStatus);
             if(Status != STATUS_SUCCESS)
             {
                 CANBoot_Error(Status);
             }
         }
     }
     else
     {
         //Block will fit into our buffer so we'll program it all at once
         BlockHeader.DestAddr = CANBoot_GetLongData();
         BlockHeader.ProgBuffAddr = (Uint32)progBuf;
         for(i = 1; i <= BlockHeader.BlockSize; i++)
         {
             WordData = CANBoot_GetWordData();
             *(Uint16 *)BlockHeader.ProgBuffAddr++ = WordData;
         }
         Status = Flash_Program((Uint16 *) BlockHeader.DestAddr, (Uint16 *)progBuf, BlockHeader.BlockSize, &FlashStatus);
         if(Status != STATUS_SUCCESS)
         {
           CANBoot_Error(Status);
         }

     }

     // Get the size of the next block
     BlockHeader.BlockSize = CANBoot_GetWordData();
  }
  CanUpgradeFinishFlag = 1;
}

//*****************************************************************************
//
//! Initialize the CAN-A port for bootloader communication.
//!
//! This function enables the CAN clock, configures the CAN GPIO pins, resets
//! the mailbox control state, and programs the CAN timing and receive mailbox.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(CANBoot_Init,"ramfuncs");
void CANBoot_Init(void)
{
    struct ECAN_REGS ECanaShadow;

    EALLOW;

    // Enable the CAN clock.
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;

    // Configure the eCAN-A GPIO pins.
    // GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1; // GPIO30 is CANRXA
    // GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1; // GPIO31 is CANTXA
    GpioCtrlRegs.GPAMUX2.all |= 0x50000000;

    // Enable the internal pull-ups for the CAN pins.
    // GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;
    // GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;
    GpioCtrlRegs.GPAPUD.all &= 0x3FFFFFFF;

    // Select asynchronous qualification for the CAN RX pin.
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;

    // Route the GPIO pins to the eCAN peripheral.
    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

    // Clear all Message Control Register bits before using mailbox 15.
    // Some MSGCTRL bits power up in an unknown state, including reserved bits.
    ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;

    // Clear all receive-message-pending and global interrupt flag bits.
    // These bits are reset to zero, but clear them again as a precaution.
    ECanaRegs.CANRMP.all = 0xFFFFFFFF;


    // Clear all interrupt flag bits.
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    // Configure the eCANA bit timing registers.
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );       // Wait for CCE bit to be set..

    ECanaShadow.CANBTC.all = 0;

#ifdef CAN_BAUD_500K
    ECanaShadow.CANBTC.bit.BRPREG = 4;
    ECanaShadow.CANBTC.bit.SJWREG = 3;
    ECanaShadow.CANBTC.bit.TSEG2REG = 3;
    ECanaShadow.CANBTC.bit.TSEG1REG = 14;
#endif

    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
    {
       ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 );       // Wait for CCE bit to be  cleared..


    // Disable all mailboxes before programming the message identifiers.
    ECanaRegs.CANME.all = 0;     // Required before writing the MSGIDs

    // Program the receive identifier for mailbox 15.
    //ECanaMboxes.MBOX1.MSGID.all = 0x00040000;  // Standard ID of 1, Acceptance mask disabled
    ECanaMboxes.MBOX15.MSGID.bit.IDE = 1;
    ECanaMboxes.MBOX15.MSGID.bit.STDMSGID = (0x1F1F1F1F >> 18) & 0x7FF;
    ECanaMboxes.MBOX15.MSGID.bit.EXTMSGID_H = (0x1F1F1F1F >> 16) & 0x3;
    ECanaMboxes.MBOX15.MSGID.bit.EXTMSGID_L = (0x1F1F1F1F & 0xFFFF);

    // Configure mailbox 15 as a receive mailbox.
    ECanaRegs.CANMD.all = 0x8000;

    // Enable mailbox 15 and clear any pending receive flags.
    ECanaRegs.CANME.all = 0x8000;

    ECanaRegs.CANRMP.all = 0xFFFFFFFF;
    EDIS;

    return;
}

//*****************************************************************************
//
//! Execute the main CAN bootloader sequence.
//!
//! This function prepares the flash API callback, erases the application
//! sectors, validates the incoming boot header, receives the image payload,
//! and returns the entry address for the downloaded application.
//!
//! \return The entry address of the downloaded application image.
//
//*****************************************************************************
#pragma CODE_SECTION(CANBoot,"ramfuncs");
Uint32 CANBoot(void)
{
    Uint32 EntryAddr;
    Uint32 Status;
    EntryAddr = 0;
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS == 1)
    {
      for(;;);
    }

    EALLOW;
    Flash_CPUScaleFactor = SCALE_FACTOR;
    Flash_CallbackPtr = &CANBoot_CallbackFunction;
    EDIS;

    Status = Flash_Erase((SECTORB | SECTORC | SECTORD | SECTORE | SECTORF | SECTORG | SECTORH), &FlashStatus);

    if(Status != STATUS_SUCCESS)
    {
      CANBoot_Error(Status);
    }

    Status = CANBoot_GetWordData();
    if (Status != EIGHT_BIT_HEADER)
       return FLASH_ENTRY_POINT;

    // Receive the upgrade image.
    CANBoot_ReadReservedFn();

    EntryAddr = CANBoot_GetLongData();

    CANBoot_CopyData();

    return EntryAddr;
}

//*****************************************************************************
//
//! Initialize the PLL divider settings.
//!
//! \param val is the PLL multiplier value written to the PLL control register.
//! \param divsel is the requested system clock divider selection.
//!
//! This function updates the PLL configuration after checking limp mode and
//! waiting for the PLL to lock when a divider change is required.
//!
//! \return None.
//
//*****************************************************************************

void InitPll(Uint16 val, Uint16 divsel)
{
    volatile Uint16 iVol;

    // Make sure the PLL is not running in limp mode
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
      EALLOW;
      // OSCCLKSRC1 failure detected. PLL running in limp mode.
      // Re-enable missing clock logic.
      SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
      EDIS;
      // Replace this line with a call to an appropriate
      // SystemShutdown(); function.
      __asm("        ESTOP0");     // Uncomment for debugging purposes
    }

    // DIVSEL MUST be 0 before PLLCR can be changed from
    // 0x0000. It is set to 0 by an external reset XRSn
    // This puts us in 1/4
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
    {
       EALLOW;
       SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
       EDIS;
    }

    // Change the PLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != val)
    {

      EALLOW;
      // Before setting PLLCR turn off missing clock detect logic
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
      SysCtrlRegs.PLLCR.bit.DIV = val;
      EDIS;

      // Optional: Wait for PLL to lock.
      // During this time the CPU will switch to OSCCLK/2 until
      // the PLL is stable.  Once the PLL is stable the CPU will
      // switch to the new PLL value.
      //
      // This time-to-lock is monitored by a PLL lock counter.
      //
      // Code is not required to sit and wait for the PLL to lock.
      // However, if the code does anything that is timing critical,
      // and requires the correct clock be locked, then it is best to
      // wait until this switching has completed.

      // Wait for the PLL lock bit to be set.

      // The watchdog should be disabled before this loop, or fed within
      // the loop via ServiceDog().

      // Uncomment to disable the watchdog
      DisableDog();

      while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
      {
          // Uncomment to service the watchdog
          // ServiceDog();
      }

      EALLOW;
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
      EDIS;
    }

    // If switching to 1/2
    if((divsel == 1)||(divsel == 2))
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
        EDIS;
    }

    // If switching to 1/1
    // * First go to 1/2 and let the power settle
    //   The time required will depend on the system, this is only an example
    // * Then switch to 1/1
    if(divsel == 3)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
        DELAY_US(50L);
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
        EDIS;
    }
}

//*****************************************************************************
//
//! Run the RAM-based firmware update flow.
//!
//! This function disables interrupts and the watchdog, initializes the CAN
//! boot interface, and then executes the main CAN bootloader routine.
//!
//! \return The entry address returned by the CAN bootloader.
//
//*****************************************************************************
Uint32 UpdateProcess(void)
{
    __disable_interrupts();  // Disable all interrupts
    DisableDog();

    DELAY_US(10000);
    CANBoot_Init();
    return CANBoot();       // Execute the RAM-resident boot routine
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

