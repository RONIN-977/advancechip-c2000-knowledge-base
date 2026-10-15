//###########################################################################
//
// FILE:   usb_keyb_structs.h
//
// TITLE:  USB keyboard device structure definitions.
//
//###########################################################################

#ifndef __USB_KEYB_STRUCTS_H__
#define __USB_KEYB_STRUCTS_H__

extern unsigned long KeyboardHandler(void *pvCBData,
                                     unsigned long ulEvent,
                                     unsigned long ulMsgData,
                                     void *pvMsgData);

extern tUSBDHIDKeyboardDevice g_sKeyboardDevice;

#endif // __USB_KEYB_STRUCTS_H__
