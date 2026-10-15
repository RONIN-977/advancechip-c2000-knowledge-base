//#############################################################################
// FILE: usbhhidkeyboard.h
// TITLE: This file holds the application interfaces for USB
//#############################################################################
// $TI Release: F2806x C/C++ Header Files and Peripheral Examples V151 $
// $Release Date: February  2, 2016 $
// $Copyright: Copyright (C) 2011-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//#############################################################################

#ifndef __USBHHIDKEYBOARD_H__
#define __USBHHIDKEYBOARD_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \addtogroup usblib_host_device
//! @{
//
//*****************************************************************************

typedef struct tUSBHKeyboard tUSBHKeyboard;

//*****************************************************************************
//
// The prototype for the host USB Keyboard driver callback function.
//
//*****************************************************************************
typedef void (*tUSBHIDKeyboardCallback)(tUSBHKeyboard *psKbInstance,
                                        uint32_t ui32Event,
                                        uint32_t ui32MsgParam,
                                        void *pvMsgData);

extern tUSBHKeyboard * USBHKeyboardOpen(tUSBHIDKeyboardCallback pfnCallback,
                                        uint8_t *pui8Buffer,
                                        uint32_t ui32BufferSize);
extern uint32_t USBHKeyboardClose(tUSBHKeyboard *psKbInstance);
extern uint32_t USBHKeyboardInit(tUSBHKeyboard *psKbInstance);
extern uint32_t USBHKeyboardModifierSet(tUSBHKeyboard *psKbInstance,
                                        uint32_t ui32Modifiers);
extern uint32_t USBHKeyboardPollRateSet(tUSBHKeyboard *psKbInstance,
                                        uint32_t ui32PollRate);
extern uint32_t USBHKeyboardUsageToChar(tUSBHKeyboard *psKbInstance,
                                        const tHIDKeyboardUsageTable *psTable,
                                        uint8_t ui8UsageID);

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
