//###########################################################################
//
// FILE:    Example_281xMCBSP__SPI_FFDLB.c
//
// TITLE:   DSP281x Device McBSP in SPI mode Digital Loop Back program.
//
// ASSUMPTIONS:
//
//          This program requires the DSP281x header files.
//          As supplied, this project is configured for "boot to H0" operation.
//
//          Other then boot mode pin configuration, no other hardware configuration
//          is required.
//
// DESCRIPTION:
//
//          Digital loopback tests for the McBSP peripheral in SPI master mode.
//
//          Three different serial word sizes can be tested.
//
//          Before compiling this project:
//          * Select the serial word size (8/16/32) by using
//            the #define statements at the beginning of the code.
//
//          * Select the FIFO level by using the #define statement
//            at the beginning of the code.
//
//          This example does not use interrupts.  Instead a polling method
//          is used to check the level of the receive FIFO.  The incoming
//          data is checked for accuracy.  If an error is found the error()
//          function is called and execution stops.
//
//           McBSP Signals      SPI equivalent
//           -------------------------------------
//           MCLKX              SPICLK  (master)
//           MFSX               SPISTE  (master)
//           MDX                SPISIMO
//           MCLKR              SPICLK  (slave - not used for this example)
//           MFSR               SPISTE  (slave - not used for this example)
//           MDR                SPISOMI (not used for this example)
//
// This program will execute and transmit words until terminated by the user.
//
// By default for the McBSP examples, the McBSP sample rate generator (SRG) input
// clock frequency is LSPCLK (150E6/4) assuming SYSCLKOUT = 150 MHz.
// If while testing, the SRG input frequency
// is changed, the #define MCBSP_SRG_FREQ  (150E6/4) in the Mcbsp.c file must
// also be updated accordingly.  This define is used to determine the Mcbsp initialization
// delay after the SRG is enabled, which must be at least 2 SRG clock cycles.
//
//    Watch Variables:
//          sdata1
//          sdata2
//          rdata1
//          rdata2
//          rdata1_point
//          rdata2_point
//
//###########################################################################
//
// Original Author: S.S.
//
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################



#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Define the level of the FIFO 1-16
#define FIFO_LEVEL 1

// Prototype statements for functions found within this file.

void init_mcbsp_spi(void);
void mcbsp_xmit(int a, int b);
void mcbsp_fifo_init(void);
void error(void);

// Global data for this example
Uint16 sdata1 = 0x000;    // Sent Data
Uint16 rdata1 = 0x000;    // Recieved Data

Uint16 sdata2 = 0x000;    // Sent Data
Uint16 rdata2 = 0x000;    // Recieved Data

Uint16 rdata1_point;
Uint16 rdata2_point;


void main(void)
{

   Uint16 i;


// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP281x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
// For this example, only enable the GPIO for McBSP
   InitMcbspGpio();                  // Select GPIOs to be McBSP pins
                                     // Port F MUX - x011 1111 0000 0000

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP281x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP281x_DefaultIsr.c.
// This function is found in DSP281x_PieVect.c.
   InitPieVectTable();

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals();     // Not required for this example
   mcbsp_fifo_init();     // Initialize the Mcbsp FIFO
   init_mcbsp_spi();      // Initalize the Mcbsp in spi master loopback mode

// Step 5. User specific code, enable interrupts:


          sdata1 = 0x0000;
          sdata2 = 0xFFFF;
          rdata1_point = sdata1;
          rdata2_point = sdata2;
          for(;;)
          {
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 mcbsp_xmit(sdata1,sdata2);
                 sdata1++;
                 sdata2--;
             }
             while(McbspaRegs.MFFRX.bit.RXFFST != FIFO_LEVEL ) { }  // Check for receive
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 rdata2 = McbspaRegs.DRR2.all;
                 rdata1 = McbspaRegs.DRR1.all;
                 if(rdata1 != rdata1_point) error();
                 if(rdata2 != rdata2_point) error();
                 rdata1_point++;
                 rdata2_point--;
             }
             asm("    nop");                                    // Good place for a breakpoint

           }


}



// Some Useful local functions

void error(void)
{
    asm("     ESTOP0");  // test failed!! Stop!
    for (;;);
}

void init_mcbsp_spi()
{
    // McBSP register settings in SPI master mode for Digital loop back
    McbspaRegs.SPCR2.all=0x0000;        // Reset FS generator, sample rate generator & transmitter
    McbspaRegs.SPCR1.all=0x0000;        // Reset Receiver, Right justify word, Digital loopback dis.
    McbspaRegs.PCR.all=0x0F08;          //(CLKXM=CLKRM=FSXM=FSRM= 1, FSXP = 1)
    McbspaRegs.SPCR1.bit.DLB = 1;
    McbspaRegs.SPCR1.bit.CLKSTP = 2;    // Together with CLKXP/CLKRP determines clocking scheme
    McbspaRegs.PCR.bit.CLKXP = 0;       // CPOL = 0, CPHA = 0 rising edge no delay
    McbspaRegs.PCR.bit.CLKRP = 0;
    McbspaRegs.RCR2.bit.RDATDLY=01;     // FSX setup time 1 in master mode. 0 for slave mode (Receive)
    McbspaRegs.XCR2.bit.XDATDLY=01;     // FSX setup time 1 in master mode. 0 for slave mode (Transmit)

    McbspaRegs.RCR1.bit.RWDLEN1=5;      // 32-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=5;      // 32-bit word

    McbspaRegs.SRGR2.all=0x2000;        // CLKSM=1, FPER = 1 CLKG periods
    McbspaRegs.SRGR1.all= 0x000F;       // Frame Width = 1 CLKG period, CLKGDV=16

    McbspaRegs.SPCR2.bit.GRST=1;        // Enable the sample rate generator
    delay_loop();                       // Wait at least 2 SRG clock cycles
    McbspaRegs.SPCR2.bit.XRST=1;        // Release TX from Reset
    McbspaRegs.SPCR1.bit.RRST=1;        // Release RX from Reset
    McbspaRegs.SPCR2.bit.FRST=1;        // Frame Sync Generator reset
}


void mcbsp_xmit(int a, int b)
{
    McbspaRegs.DXR2.all=b;
    McbspaRegs.DXR1.all=a;
}

void mcbsp_fifo_init()
{

    McbspaRegs.MFFTX.all=0x0000;
    McbspaRegs.MFFRX.all=0x001F;
    McbspaRegs.MFFCT.all=0x0;
    McbspaRegs.MFFINT.all=0x0;
    McbspaRegs.MFFST.all=0x0;
    McbspaRegs.MFFTX.bit.MFFENA=1;         // Enable FIFO
    McbspaRegs.MFFTX.bit.TXFIFO_RESET=1;   // Enable Transmit channel
    McbspaRegs.MFFRX.bit.RXFIFO_RESET=1;   // Enable Receive channel


}



//===========================================================================
// No more.
//===========================================================================

