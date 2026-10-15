//#############################################################################
// FILE: usbhscsi.h
// TITLE: Definitions for the USB host SCSI layer
//#############################################################################
// $TI Release: F2806x C/C++ Header Files and Peripheral Examples V151 $
// $Release Date: February  2, 2016 $
// $Copyright: Copyright (C) 2011-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//#############################################################################

#ifndef __USBHSCSI_H__
#define __USBHSCSI_H__

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

//*****************************************************************************
//
// Prototypes for the APIs exported by the USB SCSI layer.
//
//*****************************************************************************
extern uint32_t USBHSCSIInquiry(uint32_t ui32InPipe, uint32_t ui32OutPipe,
                                uint8_t *pui8Buffer, uint32_t *pui32Size);
extern uint32_t USBHSCSIReadCapacity(uint32_t ui32InPipe, uint32_t ui32OutPipe,
                                     uint8_t *pui8Data, uint32_t *pui32Size);
extern uint32_t USBHSCSIReadCapacities(uint32_t ui32InPipe,
                                       uint32_t ui32OutPipe, uint8_t *pui8Data,
                                       uint32_t *pui32Size);
extern uint32_t USBHSCSIModeSense6(uint32_t ui32InPipe, uint32_t ui32OutPipe,
                                   uint32_t ui32Flags, uint8_t *pui8Data,
                                   uint32_t *pui32Size);
extern uint32_t USBHSCSITestUnitReady(uint32_t ui32InPipe,
                                      uint32_t ui32OutPipe);
extern uint32_t USBHSCSIRequestSense(uint32_t ui32InPipe, uint32_t ui32OutPipe,
                                     uint8_t *pui8Data, uint32_t *pui32Size);
extern uint32_t USBHSCSIRead10(uint32_t ui32InPipe, uint32_t ui32OutPipe,
                               uint32_t ui32LBA, uint8_t *pui8Data,
                               uint32_t *pui32Size, uint32_t ui32NumBlocks);
extern uint32_t USBHSCSIWrite10(uint32_t ui32InPipe, uint32_t ui32OutPipe,
                                uint32_t ui32LBA, uint8_t *pui8Data,
                                uint32_t *pui32Size, uint32_t ui32NumBlocks);

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

#endif // __USBHSCSI_H__
