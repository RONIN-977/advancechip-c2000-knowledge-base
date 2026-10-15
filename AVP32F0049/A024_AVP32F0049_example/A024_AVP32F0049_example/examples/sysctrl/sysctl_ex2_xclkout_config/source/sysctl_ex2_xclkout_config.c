/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   sysctl_ex2_xclkout_config.c
*                         TITLE:  XCLKOUT Configuration
*******************************************************************************************/
//! This example demonstrates how to configure the XCLKOUT pin for observing
//! internal clocks through an external pin, for debugging and testing purposes.
//!
//! In this example, we are using INTOSC1 as the XCLKOUT clock source and
//! configuring the divider as 8.
//! Expected frequency of XCLKOUT = (INTOSC1 freq)/8 = 10/8 = 1.25MHz
//!
//! View the XCLKOUT on GPIO16 using an oscilloscope.
//

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Select the clock source for XCLKOUT. Refer to the enum SysCtl_ClockOut
    // for other clock sources.
    //
    SysCtl_selectClockOutSource(SYSCTL_CLOCKOUT_INTOSC1);

    //
    // Configure the XCLOUT clock divider. The divider can be /1, /2, /4 or /8.
    //
    SysCtl_setXClk(SYSCTL_XCLKOUT_DIV_8);

    //
    // Configure GPIO16 as XCLKOUT pin. Note that GPIO18 also can be used as
    // XCLKOUT pin
    //
    GPIO_setPinConfig(GPIO_16_XCLKOUT);
    // GPIO_setPinConfig(GPIO_18_XCLKOUT);

    //
    // Loop Forever. The Clock can be viewed on the configured GPIO pin.
    //
    while(1);
}

//
// End of File
//
