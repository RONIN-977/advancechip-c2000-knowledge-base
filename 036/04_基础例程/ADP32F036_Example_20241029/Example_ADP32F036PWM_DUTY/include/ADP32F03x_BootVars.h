//###########################################################################
//
// FILE:    ADP32F03x_BootVars.h
//
// TITLE:   ADP32F03x Boot Variable Definitions.
//
// NOTES:
//
//###########################################################################

#ifndef ADP32F03x_BOOT_VARS_H
#define ADP32F03x_BOOT_VARS_H

#ifdef __cplusplus
extern "C" {
#endif



//---------------------------------------------------------------------------
// External Boot ROM variable definitions:
//
extern Uint16 EmuKey;
extern Uint16 EmuBMode;
extern Uint32 Flash_CPUScaleFactor;
extern void (*Flash_CallbackPtr) (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of ADP32F03x_BOOT_VARS_H definition

//===========================================================================
// End of file.
//===========================================================================

