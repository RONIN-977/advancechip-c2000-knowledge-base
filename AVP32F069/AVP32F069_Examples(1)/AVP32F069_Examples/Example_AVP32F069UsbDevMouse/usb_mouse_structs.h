//###########################################################################
//
// FILE:   usb_mouse_structs.h
//
// TITLE:  USB mouse device structure definitions.
//
//###########################################################################

#ifndef __USB_MOUSE_STRUCTS_H__
#define __USB_MOUSE_STRUCTS_H__

extern unsigned long MouseHandler(void *pvCBData,
                                  unsigned long ulEvent,
                                  unsigned long ulMsgData,
                                  void *pvMsgData);

extern const tUSBDHIDMouseDevice g_sMouseDevice;

#endif // __USB_MOUSE_STRUCTS_H__
