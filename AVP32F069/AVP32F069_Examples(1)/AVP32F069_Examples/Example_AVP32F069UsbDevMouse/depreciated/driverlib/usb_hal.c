//###########################################################################
//
// FILE:   usb_hal.c
//
// TITLE:  Wrapper for interrupt functions and USB support pins.
//
//###########################################################################
// $TI Release: F2806x C/C++ Header Files and Peripheral Examples V151 $
// $Release Date: February  2, 2016 $
// $Copyright: Copyright (C) 2011-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "AVP32F069_Device.h"
#include "AVP32F069_Examples.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_usb.h"
#include "driverlib/usb_hal.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usblibpriv.h"
#include "usblib/device/usbdevice.h"
#include "usblib/host/usbhost.h"
#include "usblib/host/usbhostpriv.h"
#include "usblib/usblibpriv.h"

//*****************************************************************************
//
//! \addtogroup c2000_specific
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Enables USB related GPIOs to perform their USB function.
//
//*****************************************************************************

void USBGPIOEnable(void)
{
    EALLOW;
    GpioCtrlRegs.GPACTRL2.bit.USB0IOEN = 1;

    //GpioCtrlRegs.GPBLOCK.all = 0x00000000;
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO42 = 1; //USBDM GPIO 27
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO43 = 1; //USBDP GPIO 26
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO27 = 1; //USBDM GPIO 27
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO26 = 1; //USBDP GPIO 26

    //VBUS
    //GpioCtrlRegs.GPBDIR.bit.GPIO46 = 0; // GPIO 39
    //ID
    //GpioCtrlRegs.GPBDIR.bit.GPIO47 = 0; // GPIO 2

    //GpioCtrlRegs.GPDGMUX2.bit.GPIO120 = 3;
    //GpioCtrlRegs.GPDMUX2.bit.GPIO120 = 3;
    //GpioCtrlRegs.GPDGMUX2.bit.GPIO121 = 3;
    //GpioCtrlRegs.GPDMUX2.bit.GPIO121 = 3;
    EDIS;
}

//*****************************************************************************
//
//! Disables USB related GPIOs from performing their USB function.
//
//*****************************************************************************
void USBGPIODisable(void)
{
    EALLOW;
    GpioCtrlRegs.GPACTRL2.bit.USB0IOEN = 0;

    //GpioCtrlRegs.GPBLOCK.all = 0x00000000;
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO42 = 0;
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO43 = 0;
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO27 = 0;
    //GpioCtrlRegs.GPBAMSEL.bit.GPIO26 = 0;

    //GpioCtrlRegs.GPDGMUX2.bit.GPIO120 = 0;
    //GpioCtrlRegs.GPDMUX2.bit.GPIO120 = 0;
    //GpioCtrlRegs.GPDGMUX2.bit.GPIO121 = 0;
    //GpioCtrlRegs.GPDMUX2.bit.GPIO121 = 0;
    EDIS;
}

//*****************************************************************************
//
//! Wrapper function to implement mS based delay for USB functions
//
//*****************************************************************************
void USBDelay(uint32_t ui32Delay)
{
    DELAY_US(ui32Delay*1000);
}

//*****************************************************************************
//
//! Device interrupt service routine wrapper to make ISR compatible with
//! C2000 PIE controller.
//
//*****************************************************************************

__interrupt void
f28x_USB0DeviceIntHandler(void)
{
    USB0DeviceIntHandler();
    PieCtrlRegs.PIEACK.all |= 0x010;
}
//*****************************************************************************
//
//! Host interrupt service routine wrapper to make ISR compatible with
//! C2000 PIE controller.
//
//*****************************************************************************
__interrupt void
f28x_USB0HostIntHandler(void)
{
    USB0HostIntHandler();
    PieCtrlRegs.PIEACK.all |= 0x010;
}

//*****************************************************************************
//
//! Dual mode interrupt service routine wrapper to make ISR compatible with
//! C2000 PIE controller.
//
//*****************************************************************************
__interrupt void
f28x_USB0DualModeIntHandler(void)
{
    USB0DualModeIntHandler();
    PieCtrlRegs.PIEACK.all |= 0x010;
}

//*****************************************************************************
//
// Close the c2000_specific Doxygen group.
//! @}
//
//*****************************************************************************

