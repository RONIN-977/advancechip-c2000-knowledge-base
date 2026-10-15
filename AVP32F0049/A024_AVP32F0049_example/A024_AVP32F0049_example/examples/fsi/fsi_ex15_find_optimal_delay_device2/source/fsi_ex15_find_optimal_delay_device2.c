/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                  FILE:  fsi_find_optimal_delay_device2.c
*                  TITLE: FSI Find Optimal Skew Compensation Setting For FSIRX (Device 2)
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include <fsi_optimal_delay.h>
//
// Main
//
void main(void)
{
    volatile FSIExecutionPoint exePoint;
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // NOTE: External loopback, Modify GPIO settings as per setup
    //
	GPIO_setPinConfig(DEVICE_GPIO_CFG_FSI_TXCLK);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_FSI_TX0);
	GPIO_setPinConfig(DEVICE_GPIO_CFG_FSI_TX1);

    GPIO_setPinConfig(DEVICE_GPIO_CFG_FSI_RXCLK);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_FSI_RX0);
	GPIO_setPinConfig(DEVICE_GPIO_CFG_FSI_RX1);

    //
    // Set RX GPIO to be asynchronous
    // (pass through without delay)
    // Default setting is to have 2 SYS_CLK cycles delay
    //
	GPIO_setQualificationMode(DEVICE_GPIO_PIN_FSI_RX1, GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_FSI_RX0, GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_FSI_RXCLK, GPIO_QUAL_ASYNC);

    // Wait for Device 1 to calibrate Device 2
    exePoint = FSI_calibrateExecutionPoint(FSIRXA_BASE, FSITXA_BASE, FSI_DATA_WIDTH_2_LANE, 16U);

    // Device 2 transmits to calibrate Device 1
    FSI_transmitToCalibrate(FSIRXA_BASE, FSITXA_BASE, 2U);

    ESTOP0;
    while(1);
}
