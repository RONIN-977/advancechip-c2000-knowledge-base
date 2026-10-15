//###########################################################################
//
// FILE:    AVP32F069_BootVars.h
//
// TITLE:   AVP32F069 Boot Variable Definitions.
//
// NOTES:
//

//###########################################################################

#ifndef AVP32F069_BOOT_VARS_H
#define AVP32F069_BOOT_VARS_H

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

#endif  // end of AVP32F069_BOOT_VARS_H definition

//===========================================================================
// End of file.
//===========================================================================

