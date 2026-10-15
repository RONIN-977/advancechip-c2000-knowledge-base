
//#############################################################################
//
// FILE:   ADP32F03x_XIntrupt.h
//
// TITLE:  ADP32F03x external interrupt register definitions.
//
// Description:
// This file defines the external interrupt register bitfields, unions, and
// structures for the ADP32F03x device support package.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef ADP32F03X_XINTRUPT_H
#define ADP32F03X_XINTRUPT_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/

//---------------------------------------------------------------------------

struct XINTCR_BITS {
    Uint16   ENABLE:1;    // 0      enable/disable
    Uint16   rsvd1:1;     // 1      reserved
    Uint16   POLARITY:2;  // 3:2    pos/neg, both triggered
    Uint16   rsvd2:12;    //15:4    reserved
};

union XINTCR_REG {
   Uint16               all;
   struct XINTCR_BITS   bit;
};

//---------------------------------------------------------------------------
// External Interrupt Register File:
//
struct XINTRUPT_REGS {
   union XINTCR_REG XINT1CR;
   union XINTCR_REG XINT2CR;
   union XINTCR_REG XINT3CR;
   Uint16           rsvd1[5];
   Uint16           XINT1CTR;
   Uint16           XINT2CTR;
   Uint16           XINT3CTR;
   Uint16           rsvd[5];
};

//---------------------------------------------------------------------------
// External Interrupt References & Function Declarations:
//
extern volatile struct XINTRUPT_REGS XIntruptRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

