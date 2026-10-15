
//#############################################################################
//
// FILE:   ADP32F03x_DevEmu.h
//
// TITLE:  ADP32F03x device emulation register definitions.
//
// Description:
// This file defines the device emulation register bitfields and structures
// used by the ADP32F03x device support package.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef ADP32F03X_DEV_EMU_H
#define ADP32F03X_DEV_EMU_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/

//---------------------------------------------------------------------------
// Device Emulation Register Bit Definitions:
//
// Device Configuration Register Bit Definitions
struct DEVICECNF_BITS  {     // bits  description
   Uint16 rsvd1:3;           // 2:0   reserved
   Uint16 VMAPS:1;           // 3     VMAP Status
   Uint16 rsvd2:1;           // 4     reserved
   Uint16 XRSn:1;            // 5     XRSn Signal Status
   Uint16 rsvd3:10;          // 15:6  reserved
   Uint16 rsvd4:3;           // 18:16 reserved
   Uint16 ENPROT:1;          // 19    Enable/Disable pipeline protection
   Uint16 rsvd5:7;           // 26:21 reserved
   Uint16 TRSTn:1;           // 27    Status of TRSTn signal
   Uint16 PINOUTSELSTS:1;    // 28    Status of PINOUSTSEL pin
   Uint16 PINOUTSELPUD:1;    // 29    PINOUTSEL pin pull-up disable
   Uint16 rsvd6:2;           // 31:30 reserved
};

union DEVICECNF_REG {
   Uint32                 all;
   struct DEVICECNF_BITS  bit;
};

// CLASSID
struct CLASSID_BITS   {  // bits  description
   Uint16 CLASSNO:8;     // 7:0   Class Number
   Uint16 PARTTYPE:8;    // 15:8  Part Type
};

union CLASSID_REG {
   Uint16               all;
   struct CLASSID_BITS  bit;
};


struct DEV_EMU_REGS {
   union DEVICECNF_REG DEVICECNF;  // Device Configuration
   union CLASSID_REG   CLASSID;    // Class ID
   Uint16              REVID;      // Device ID
};

// PARTID
struct PARTID_BITS   {  // bits  description
   Uint16 PARTNO:8;     // 7:0   Part Number
   Uint16 PARTTYPE:8;   // 15:8  Part Type
};

union PARTID_REG {
   Uint16               all;
   struct PARTID_BITS   bit;
};

struct PARTID_REGS {
   union PARTID_REG PARTID; // Part ID
};


//---------------------------------------------------------------------------
// Device Emulation Register References & Function Declarations:
//
extern volatile struct DEV_EMU_REGS DevEmuRegs;
extern volatile struct PARTID_REGS PartIdRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
