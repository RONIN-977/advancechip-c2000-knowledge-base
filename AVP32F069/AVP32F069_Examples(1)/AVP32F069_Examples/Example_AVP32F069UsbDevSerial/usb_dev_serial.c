//###########################################################################
/*
1.本示例演示了AVP32F069枚举为串口设备的功能。
2.代码执行流程：
2.1 首先是时钟的选择，这里我们选取的是外部时钟(20MHZ)，再通过4倍频得到80M的系统时钟，通过6倍频得到120MUSB工作时钟;
2.2 然后是PIE中断向量表的初始化；
2.3 接下来是一些外设的初的初始化；
2.4 usb模式的选择以及设置；
2.5 配置以及使能串口中断；
2.6 初始化USB发送、接收缓冲区；
2.7 初始化USB设备信息；
2.8 最后是一个while(1)空循环。
3.其他说明：
   USB程序中都是使用的外部时钟，例程中使用的20M晶振分别进行4倍频以及6倍频得到80M的CPU时钟频率以及120M的USB工作时钟。若使用10M晶振，需要修改函数SysCtrlInit中SysCtrlRegs.PLLCR.bit.DIV 和SysCtrlRegs.PLL2MULT.bit.PLL2MULT的初始值。
3.1 串口调试助手设置：任意波特率，保证两个串口设置一致即可。
4.观察实验现象：
4.1 将仿真器与开发板连接后，编译程序并下载并运行；
4.2 查看电脑设备管理器可以看到系统多一个串口显示，也可在串口助手中确认。
4.3 使用usbview查看设备厂商以及产品信息。
4.4 将一个USB串口连接开发板上的DB9端，可用串口助手进行数据收发测试。
*/
//#############################################################################

#include "AVP32F069_Project.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/usb.h"
#include "inc/hw_usb.h"
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "driverlib/usb_hal.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "usb_serial_structs.h"

//*****************************************************************************
//
// Flag indicating whether or not a Break condition is currently being sent.
//
//*****************************************************************************
static tBoolean g_bSendingBreak = false;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
	__asm(" ESTOP0");
}
#endif

//*****************************************************************************
//
// This function is called whenever serial data is received from the UART.
// It is passed the accumulated error flags from each character received in
// this interrupt and determines from them whether or not an interrupt
// notification to the host is required.
//
// If a notification is required and the control interrupt endpoint is idle,
// send the notification immediately.  If the endpoint is not idle, accumulate
// the errors in a global variable which will be checked on completion of the
// previous notification and used to send a second one if necessary.
//
//*****************************************************************************
static void
CheckForSerialStateChange(const tUSBDCDCDevice *psDevice, uint32_t lErrors)
{
    unsigned short usSerialState;

    //
    // Clear the USB serial state.  Since handshakes are being faked, always
    // set the TXCARRIER (DSR) and RXCARRIER (DCD) bits.
    //
    usSerialState = USB_CDC_SERIAL_STATE_TXCARRIER |
                    USB_CDC_SERIAL_STATE_RXCARRIER;

    //
    // Are any error bits set?
    //
    if(lErrors & (UART_RXST_BRKDT | UART_RXST_FE | UART_RXST_OE | UART_RXST_PE))
    {
        //
        // At least one error is being notified so translate from the hardware
        // error bits into the correct state markers for the USB notification.
        //
        if(lErrors & UART_RXST_OE)
        {
            usSerialState |= USB_CDC_SERIAL_STATE_OVERRUN;
        }

        if(lErrors & UART_RXST_PE)
        {
            usSerialState |= USB_CDC_SERIAL_STATE_PARITY;
        }

        if(lErrors & UART_RXST_FE)
        {
            usSerialState |= USB_CDC_SERIAL_STATE_FRAMING;
        }

        if(lErrors & UART_RXST_BRKDT)
        {
            usSerialState |= USB_CDC_SERIAL_STATE_BREAK;
        }

        //
        // Call the CDC driver to notify the state change.
        //
        USBDCDCSerialStateChange((void *)psDevice, usSerialState);
    }
}

//*****************************************************************************
//
// Read as many characters from the UART FIFO as possible and move them into
// the CDC transmit buffer.
//
// \return Returns UART error flags read during data reception.
//
//*****************************************************************************
static long
ReadUARTData(void)
{
    int32_t i32Char, i32Errors;
    uint8_t ui8Char;
    uint32_t ui32Space;

    //
    // Clear the error indicator.
    //
    i32Errors = 0;

    //
    // How much space is available in the buffer?
    //
    ui32Space = USBBufferSpaceAvailable(&g_sTxBuffer);

    //
    // Read data from the UART FIFO until there is none left or there is no
    // more space in the receive buffer.
    //
    while(ui32Space && UARTCharsAvail(UART0_BASE))
    {
        //
        // Read a character from the UART FIFO into the ring buffer if no
        // errors are reported.
        //
        i32Char = UARTCharGetNonBlocking(UART0_BASE);

        //
        // If the character did not contain any error notifications,
        // copy it to the output buffer.
        //
        if(!(i32Char & ~0xFF))
        {
            ui8Char = (unsigned char)(i32Char & 0xFF);
            USBBufferWrite(&g_sTxBuffer, &ui8Char, 1);

            //
            // Decrement the number of bytes the buffer can accept.
            //
            ui32Space--;
        }
        else
        {
            //
            // Update the error accumulator.
            //
            i32Errors |= i32Char;
        }
    }

    //
    // Pass back the accumulated error indicators.
    //
    return(i32Errors);
}

//*****************************************************************************
//
// Take as many bytes from the transmit buffer as there is space for and move
// them into the USB UART's transmit FIFO.
//
//*****************************************************************************
static void
USBUARTPrimeTransmit(void)
{
    uint32_t ui32Read;
    uint8_t ui8Char;

    //
    // If a break condition is currently being sent, don't receive any more
    // data.  Transmission will resume once the break is turned off.
    //
    if(g_bSendingBreak)
    {
        return;
    }

    //
    // If there is space in the UART FIFO, try to read some characters
    // from the receive buffer to fill it again.
    //
    while(UARTSpaceAvail(UART0_BASE))
    {
        //
        // Get a character from the buffer.
        //
        ui32Read = USBBufferRead(&g_sRxBuffer, &ui8Char, 1);

        //
        // Was a character read?
        //
        if(ui32Read)
        {
            //
            // Place the character in the UART transmit FIFO.
            //
            UARTCharPutNonBlocking(UART0_BASE, ui8Char);
        }
        else
        {
            //
            // There are no more characters so exit the function.
            //
            return;
        }
    }
}

//*****************************************************************************
//
// Interrupt handler for the UART TX which is being redirected via USB.
//
//*****************************************************************************
__interrupt void
USBUARTTXIntHandler(void)
{
    uint32_t ui32Ints;

    ui32Ints = UARTIntStatus(UART0_BASE, true);
    //
    // Handle transmit interrupts.
    //
    if(ui32Ints & UART_INT_TXRDY)
    {
        //
        // Move as many bytes as possible into the transmit FIFO.
        //
        USBUARTPrimeTransmit();

        //
        // If the output buffer is empty, turn off the transmit interrupt.
        //
        if(!USBBufferDataAvailable(&g_sRxBuffer))
        {
            UARTIntDisable(UART0_BASE, UART_INT_TXRDY);
        }
    }

    PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}

//*****************************************************************************
//
// Interrupt handler for the UART RX which is being redirected via USB.
//
//*****************************************************************************
__interrupt void
USBUARTRXIntHandler(void)
{
    uint32_t ui32Ints;

    ui32Ints = UARTIntStatus(UART0_BASE, true);
    //
    // Handle receive interrupts.
    //
    if(ui32Ints & UART_INT_RXRDY_BRKDT)
    {
        //
        // Read the UART's characters into the buffer.
        //
        ReadUARTData();

    }
    else if(ui32Ints & UART_INT_RXERR)
    {
        //
        //Notify Host of our error
        //
        CheckForSerialStateChange(&g_sCDCDevice, UARTRxErrorGet(UART0_BASE));

        //
        //Clear the error and continue
        //
        UARTRxErrorClear(UART0_BASE);
    }

    PieCtrlRegs.PIEACK.bit.ACK9 = 1;

}

//*****************************************************************************
//
// Set the communication parameters to use on the UART.
//
//*****************************************************************************
static tBoolean
SetLineCoding(tLineCoding *psLineCoding)
{
    uint32_t ui32Config;
    tBoolean bRetcode;

    //
    // Assume everything is OK until a problem is detected.
    //
    bRetcode = true;

    //
    // Word length.  For invalid values, the default is to set 8 bits per
    // character and return an error.
    //
    switch(psLineCoding->ui8Databits)
    {
    case 5:
    {
        ui32Config = UART_CONFIG_WLEN_5;
        break;
    }

    case 6:
    {
        ui32Config = UART_CONFIG_WLEN_6;
        break;
    }

    case 7:
    {
        ui32Config = UART_CONFIG_WLEN_7;
        break;
    }

    case 8:
    {
        ui32Config = UART_CONFIG_WLEN_8;
        break;
    }

    default:
    {
        ui32Config = UART_CONFIG_WLEN_8;
        bRetcode = false;
        break;
    }
    }

    //
    // Parity.  For any invalid values, set no parity and return an error.
    //
    switch(psLineCoding->ui8Parity)
    {
    case USB_CDC_PARITY_NONE:
    {
        ui32Config |= UART_CONFIG_PAR_NONE;
        break;
    }

    case USB_CDC_PARITY_ODD:
    {
        ui32Config |= UART_CONFIG_PAR_ODD;
        break;
    }

    case USB_CDC_PARITY_EVEN:
    {
        ui32Config |= UART_CONFIG_PAR_EVEN;
        break;
    }

    default:
    {
        ui32Config |= UART_CONFIG_PAR_NONE;
        bRetcode = false;
        break;
    }
    }

    //
    // Stop bits.  The hardware only supports 1 or 2 stop bits whereas CDC
    // allows the host to select 1.5 stop bits.  If passed 1.5 (or any other
    // invalid or unsupported value of ucStop, set up for 1 stop bit but return
    // an error in case the caller needs to Stall or otherwise report this back
    // to the host.
    //
    switch(psLineCoding->ui8Stop)
    {
        //
        // One stop bit requested.
        //
    case USB_CDC_STOP_BITS_1:
    {
        ui32Config |= UART_CONFIG_STOP_ONE;
        break;
    }

    //
    // Two stop bits requested.
    //
    case USB_CDC_STOP_BITS_2:
    {
        ui32Config |= UART_CONFIG_STOP_TWO;
        break;
    }

    //
    // Other cases are either invalid values of ucStop or values that are
    // not supported, so set 1 stop bit but return an error.
    //
    default:
    {
        ui32Config = UART_CONFIG_STOP_ONE;
        bRetcode = false;
        break;
    }
    }

    //
    // Set the UART mode appropriately.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED)/4,
    		            readusb32_t(&(psLineCoding->ui32Rate)), ui32Config);

    //
    // Let the caller know if a problem was encountered.
    //
    return(bRetcode);
}

//*****************************************************************************
//
// Get the communication parameters in use on the UART.
//
//*****************************************************************************
static void
GetLineCoding(tLineCoding *psLineCoding)
{
    uint32_t ui32Config;
    uint32_t ui32Rate;

    //
    // Get the current line coding set in the UART.
    //
    UARTConfigGetExpClk(UART0_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED)/4, &ui32Rate,
                        &ui32Config);
    writeusb32_t(&(psLineCoding->ui32Rate), ui32Rate);

    //
    // Translate the configuration word length field into the format expected
    // by the host.
    //
    switch(ui32Config & UART_CONFIG_WLEN_MASK)
    {
    case UART_CONFIG_WLEN_8:
    {
        psLineCoding->ui8Databits = 8;
        break;
    }

    case UART_CONFIG_WLEN_7:
    {
        psLineCoding->ui8Databits = 7;
        break;
    }

    case UART_CONFIG_WLEN_6:
    {
        psLineCoding->ui8Databits = 6;
        break;
    }

    case UART_CONFIG_WLEN_5:
    {
        psLineCoding->ui8Databits = 5;
        break;
    }
    }

    //
    // Translate the configuration parity field into the format expected
    // by the host.
    //
    switch(ui32Config & UART_CONFIG_PAR_MASK)
    {
    case UART_CONFIG_PAR_NONE:
    {
        psLineCoding->ui8Parity = USB_CDC_PARITY_NONE;
        break;
    }

    case UART_CONFIG_PAR_ODD:
    {
        psLineCoding->ui8Parity = USB_CDC_PARITY_ODD;
        break;
    }

    case UART_CONFIG_PAR_EVEN:
    {
        psLineCoding->ui8Parity = USB_CDC_PARITY_EVEN;
        break;
    }

    }

    //
    // Translate the configuration stop bits field into the format expected
    // by the host.
    //
    switch(ui32Config & UART_CONFIG_STOP_MASK)
    {
    case UART_CONFIG_STOP_ONE:
    {
        psLineCoding->ui8Stop = USB_CDC_STOP_BITS_1;
        break;
    }

    case UART_CONFIG_STOP_TWO:
    {
        psLineCoding->ui8Stop = USB_CDC_STOP_BITS_2;
        break;
    }
    }
}

//*****************************************************************************
//
// This function sets or clears a break condition on the redirected UART RX
// line.  A break is started when the function is called with \e bSend set to
// \b true and persists until the function is called again with \e bSend set
// to \b false.
//
//*****************************************************************************
static void
SendBreak(tBoolean bSend)
{
    //
    //C28x SCI cannot send break conditions
    //
    return;
}

//*****************************************************************************
//
// Handles CDC driver notifications related to control and setup of the device.
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ui32Event identifies the notification event.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the CDC driver to perform control-related
// operations on behalf of the USB host.  These functions include setting
// and querying the serial communication parameters, setting handshake line
// states and sending break conditions.
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned long
ControlHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,
               void *pvMsgData)
{
    //
    // Which event was sent?
    //
    switch(ui32Event)
    {
        //
        // The host has connected.
        //
    case USB_EVENT_CONNECTED:
    {
        //
        // Flush the buffers.
        //
        USBBufferFlush(&g_sTxBuffer);
        USBBufferFlush(&g_sRxBuffer);

        break;
    }

    //
    // The host has disconnected.
    //
    case USB_EVENT_DISCONNECTED:
    {
        break;
    }

    //
    // Return the current serial communication parameters.
    //
    case USBD_CDC_EVENT_GET_LINE_CODING:
    {
        GetLineCoding(pvMsgData);
        break;
    }

    //
    // Set the current serial communication parameters.
    //
    case USBD_CDC_EVENT_SET_LINE_CODING:
    {
        SetLineCoding(pvMsgData);
        break;
    }

    //
    // Set the current serial communication parameters.
    //
    case USBD_CDC_EVENT_SET_CONTROL_LINE_STATE:
    {
        break;
    }

    //
    // Send a break condition on the serial line.
    //
    case USBD_CDC_EVENT_SEND_BREAK:
    {
        SendBreak(true);
        break;
    }

    //
    // Clear the break condition on the serial line.
    //
    case USBD_CDC_EVENT_CLEAR_BREAK:
    {
        SendBreak(false);
        break;
    }

    //
    // Ignore SUSPEND and RESUME for now.
    //
    case USB_EVENT_SUSPEND:
    case USB_EVENT_RESUME:
    {
        break;
    }

    //
    // Other events can be safely ignored.
    //
    default:
    {
        break;
    }
    }

    return(0);
}

//*****************************************************************************
//
// Handles CDC driver notifications related to the transmit channel (data to
// the USB host).
//
// \param ui32CBData is the client-supplied callback pointer for this channel.
// \param ui32Event identifies the notification event.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the CDC driver to notify us of any events
// related to operation of the transmit data channel (the IN channel carrying
// data to the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned long
TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,
          void *pvMsgData)
{
    //
    // Which event was sent?
    //
    switch(ui32Event)
    {
    case USB_EVENT_TX_COMPLETE:
    {
        //
        // There is nothing to do here since it is handled by the
        // USBBuffer.
        //
        break;
    }

    //
    // Other events can be safely ignored.
    //
    default:
    {
        break;
    }
    }

    return(0);
}

//*****************************************************************************
//
// Handles CDC driver notifications related to the receive channel (data from
// the USB host).
//
// \param ui32CBData is the client-supplied callback data value for this channel.
// \param ui32Event identifies the notification event.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the CDC driver to notify us of any events
// related to operation of the receive data channel (the OUT channel carrying
// data from the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
unsigned long
RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,
          void *pvMsgData)
{
    uint32_t ui32Count;

    //
    // Which event was sent?
    //
    switch(ui32Event)
    {
        //
        // A new packet has been received.
        //
    case USB_EVENT_RX_AVAILABLE:
    {
        //
        // Feed some characters into the UART TX FIFO and enable the
        // interrupt.
        //
        USBUARTPrimeTransmit();
        UARTIntEnable(UART0_BASE, UART_INT_TXRDY);
        break;
    }

    //
    // This is a request for how much unprocessed data is still waiting to
    // be processed.  Return 0 if the UART is currently idle or 1 if it is
    // in the process of transmitting something.  The actual number of
    // bytes in the UART FIFO is not important here, merely whether or
    // not everything previously sent to us has been transmitted.
    //
    case USB_EVENT_DATA_REMAINING:
    {
        //
        // Get the number of bytes in the buffer and add 1 if some data
        // still has to clear the transmitter.
        //
        ui32Count = UARTBusy(UART0_BASE) ? 1 : 0;
        return(ui32Count);
    }

    //
    // This is a request for a buffer into which the next packet can be
    // read.  This mode of receiving data is not supported so let the
    // driver know by returning 0.  The CDC driver should not be sending
    // this message but this is included just for illustration and
    // completeness.
    //
    case USB_EVENT_REQUEST_BUFFER:
    {
        return(0);
    }

    //
    // Other events can be safely ignored.
    //
    default:
    {
        break;
    }
    }

    return(0);
}


void SysCtrlInit(void)
{

    EALLOW;
    //Disable Watchdog
    SysCtrlRegs.WDCR = 0x68;

    //Setup Clock
    //20MHz ->PLL->80MHz->C28
    //      ->PLL2->120MHz->USB
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;

    while(SysCtrlRegs.PLLSTS.bit.MCLKSTS);
    SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
    SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
    SysCtrlRegs.PLLCR.bit.DIV = 4;       //开发板外部晶振为10M的话这里需要设置成8
    while(!SysCtrlRegs.PLLSTS.bit.PLLLOCKS);
    SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
    SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;

    SysCtrlRegs.PLL2CTL.bit.PLL2CLKSRCSEL = 2;
    SysCtrlRegs.PLL2CTL.bit.PLL2EN = 1;
    SysCtrlRegs.PLL2MULT.bit.PLL2MULT = 6; //开发板外部晶振为10M的话这里需要设置成12
    while(!SysCtrlRegs.PLL2STS.bit.PLL2LOCKS);

    //Enable USB Clock
    SysCtrlRegs.PCLKCR3.bit.USB0ENCLK = 1;
    //Enable PHY
    GpioCtrlRegs.GPACTRL2.bit.USB0IOEN = 1;

    EDIS;
}

//*****************************************************************************
//
// This is the main application entry function.
//
//*****************************************************************************
int
main(void)
{
	//
    // Section .ebss was initialized to 0x0000 in
	// F2806x_CodeStartBranch_ebss_init.asm. This was to ensure that all global
	// variables of the usblib are initialized to 0x0000 on the F2806x.
	//

    //
    // Set the clocking to run from the PLL
    //
    SysCtrlInit();
    InitPieCtrl();
    InitPieVectTable();

    //
    // Enable Device Mode
    //
    EALLOW;
    HWREG(USBMODESEL) = USBMODESEL_DEV;
    HWREG(USB0_BASE + USB_O_GPCS) = USBGPCS_DEV;
    EDIS;

    //
    // Enable the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Enable and configure the UART RX and TX pins
    //
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;
    EDIS;

    //
    // Register interrupt handlers
    //
    IntRegister(INT_SCITXINTA, USBUARTTXIntHandler);
    IntRegister(INT_SCIRXINTA, USBUARTRXIntHandler);

    //
    // Configure the required pins for USB operation.
    //
    USBGPIOEnable();
    USBIntRegister(USB0_BASE, f28x_USB0DeviceIntHandler);

    //
    // Set the default UART configuration.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED)/4, 115200,
                        UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE |
                        UART_CONFIG_STOP_ONE);
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);

    //
    // Configure and enable UART interrupts.
    //
    UARTIntClear(UART0_BASE, UARTIntStatus(UART0_BASE, false));
    UARTIntEnable(UART0_BASE, (UART_INT_RXERR | UART_INT_RXRDY_BRKDT | UART_INT_TXRDY ));

    //
    // Initialize the transmit and receive buffers.
    //
    USBBufferInit(&g_sTxBuffer);
    USBBufferInit(&g_sRxBuffer);

    //
    // Set the USB stack mode to Device mode with VBUS monitoring.
    //
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    //
    // Pass the device information to the USB library and place the device
    // on the bus.
    //
    USBDCDCInit(0, &g_sCDCDevice);

    //
    // Enable interrupts now that the application is ready to start.
    //
    IntEnable(INT_SCITXINTA);
    IntEnable(INT_SCIRXINTA);

    IntMasterEnable();

    //
    // Main application loop.
    //
    while(1)
    {
    }
}
