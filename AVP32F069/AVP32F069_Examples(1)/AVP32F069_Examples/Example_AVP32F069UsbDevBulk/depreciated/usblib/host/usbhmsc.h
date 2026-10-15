//#############################################################################
// FILE: usbhmsc.h
// TITLE: Definitions for the USB MSC host driver
//#############################################################################
// $TI Release: F2806x C/C++ Header Files and Peripheral Examples V151 $
// $Release Date: February  2, 2016 $
// $Copyright: Copyright (C) 2011-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//#############################################################################

#ifndef __USBHMSC_H__
#define __USBHMSC_H__

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
//! \addtogroup usblib_host_class
//! @{
//
//*****************************************************************************

typedef struct tUSBHMSCInstance tUSBHMSCInstance;

//*****************************************************************************
//
// These defines are the the events that will be passed in the \e ui32Event
// parameter of the callback from the driver.
//
//*****************************************************************************
#define MSC_EVENT_OPEN          1
#define MSC_EVENT_CLOSE         2

//*****************************************************************************
//
// The prototype for the USB MSC host driver callback function.
//
//*****************************************************************************
typedef void (*tUSBHMSCCallback)(tUSBHMSCInstance *psMSCInstance,
                                 uint32_t ui32Event,
                                 void *pvEventData);

//*****************************************************************************
//
// Prototypes for the USB MSC host driver APIs.
//
//*****************************************************************************
extern tUSBHMSCInstance * USBHMSCDriveOpen(uint32_t ui32Drive,
                                           tUSBHMSCCallback pfnCallback);
extern void USBHMSCDriveClose(tUSBHMSCInstance *psMSCInstance);
extern int32_t USBHMSCDriveReady(tUSBHMSCInstance *psMSCInstance);
extern int32_t USBHMSCBlockRead(tUSBHMSCInstance *psMSCInstance,
                                uint32_t ui32LBA, uint8_t *pui8Data,
                                uint32_t ui32NumBlocks);
extern int32_t USBHMSCBlockWrite(tUSBHMSCInstance *psMSCInstance,
                                 uint32_t ui32LBA, uint8_t *pui8Data,
                                 uint32_t ui32NumBlocks);

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

#endif // __USBHMSC_H__
