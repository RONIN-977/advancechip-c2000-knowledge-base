/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   clapromcrc_ex1_cpuinterrupt.c
*                         TITLE:  Background CLA Program ROM CRC computation with interrupt.
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "clapromcrc_ex1_crctable.h"

#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern uint16_t RamfuncsLoadStart;
extern uint16_t RamfuncsLoadSize;
extern uint16_t RamfuncsRunStart;
#endif

//
// Globals
//
bool pass = false;

//
// Function Prototypes
//
__interrupt void clapromcrcISR(void);

//
// Main
//
void main(void)
{
	volatile uint32_t currentAddress = 0x0000;
	volatile bool runStatus = false;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();

    //
    // Disable global interrupts.
    //
    DINT;

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts. 
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Map the ISR to the CLAPROMCRC interrupt.
    //
    Interrupt_register(INT_CLA1PROMCRC, clapromcrcISR);

    //
    // Enable the wake interrupt in the PIE: Group 7 interrupt 15.
    //
    Interrupt_enable(INT_CLA1PROMCRC);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Set the emulation mode of the CLAPROMCRC to free run.
    //
    CLAPROMCRC_setEmulationMode(CLA1PROMCRC_BASE, CLAPROMCRC_MODE_FREE);

    //
    // Set the block size to 96KB for the CRC.
    //
    CLAPROMCRC_setBlockSize(CLA1PROMCRC_BASE, 96);

    //
    // Set the start adress to 0x0000 according to the start address of
    // program ROM of the CLA memory map.
    //
    CLAPROMCRC_setStartAddress(CLA1PROMCRC_BASE, 0x0000);

    //
    // Set the seed of the CRC to 0x00000000.
    //
    CLAPROMCRC_setSeed(CLA1PROMCRC_BASE, 0x00000000);

    //
    // Set the golden CRC value of the entire 96KB program ROM.
    //
    CLAPROMCRC_setGoldenCRC(CLA1PROMCRC_BASE, clapromcrcTable[96]);

    //
    // Enable the DONE interrupt of the CLAPROMCRC.
    //
    CLAPROMCRC_enableDoneInterrupt(CLA1PROMCRC_BASE);

    //
    // Start the CRC calculation.
    //
    CLAPROMCRC_start(CLA1PROMCRC_BASE);

    //
    // Set runStatus to true;
    //
    runStatus = true;

    //
    // Loop Forever
    //
    while(1)
    {
    	//
    	// Check the run status and current address of the CRC calculation.
    	//
    	runStatus = CLAPROMCRC_getRunStatus(CLA1PROMCRC_BASE);
    	currentAddress = CLAPROMCRC_getCurrentAddress(CLA1PROMCRC_BASE);
    }
}

//
// clapromcrcISR - DONE interrupt from CLAPROMCRC.
//
__interrupt void
clapromcrcISR(void)
{
    //
    // Check the status of the CRC calculation and set the global pass
    // variable.
    //
    if(CLAPROMCRC_checkStatus(CLA1PROMCRC_BASE))
    {
    	pass = true;
    }

    //
    // Clear the CLAPROMCRC interrupt flags.
    //
    CLAPROMCRC_clearInterruptFlag(CLA1PROMCRC_BASE, CLAPROMCRC_CRCDONE_FLG);
    CLAPROMCRC_clearInterruptFlag(CLA1PROMCRC_BASE, CLAPROMCRC_INT_FLG);

    //
    // Acknowledge the PIE interrupt.
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
}

//
// End of File
//
