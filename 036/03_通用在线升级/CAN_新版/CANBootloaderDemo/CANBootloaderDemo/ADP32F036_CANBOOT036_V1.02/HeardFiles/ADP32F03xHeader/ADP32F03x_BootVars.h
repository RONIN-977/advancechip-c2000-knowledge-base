
//#############################################################################
//
// FILE:   ADP32F03x_BootVars.h
//
// TITLE:  ADP32F03x boot variable declarations.
//
// Description:
// This file declares boot ROM variables and callback pointers used by the
// ADP32F03x boot and flash programming flow.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef ADP32F03X_BOOT_VARS_H
#define ADP32F03X_BOOT_VARS_H

#ifdef __cplusplus
extern "C" {
#endif



/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/

/* External Boot ROM variable definitions */
extern Uint16 EmuKey;
extern Uint16 EmuBMode;
extern Uint32 Flash_CPUScaleFactor;
extern void (*Flash_CallbackPtr) (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

