/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   interrupt_ex1_external.c
*                         TITLE:  External Interrupt test program.
*******************************************************************************************/
//
// Included Files
//
#include "device.h"
#include "driverlib.h"

//
// Defines
//

// Qualification period at 6 samples in microseconds
#define DELAY   (6.0 * 510.0 * 1000000.0 * (1.0 / DEVICE_SYSCLK_FREQ))


//
// Globals
//
volatile uint32_t xint1Count;
volatile uint32_t xint2Count;
volatile uint32_t loopCount;

//
// Function Prototypes
//
interrupt void xint1ISR(void);
interrupt void xint2ISR(void);

//
// Main
//
void main(void)
{
    uint32_t tempX1Count;
    uint32_t tempX2Count;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups
    //
    //Device_initGPIO(); // Skipped for this example

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR)
    //
    Interrupt_initVectorTable();

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    Interrupt_register(INT_XINT1, &xint1ISR);
    Interrupt_register(INT_XINT2, &xint2ISR);

    //
    // User specific code, enable interrupts:
    //
    // Clear the counters
    //
    xint1Count = 0; // Count XINT1 interrupts
    xint2Count = 0; // Count XINT2 interrupts
    loopCount = 0;  // Count times through idle loop

    //
    // Enable XINT1 and XINT2 in the PIE: Group 1 interrupt 4 & 5
    // Enable INT1 which is connected to WAKEINT:
    //
    Interrupt_enable(INT_XINT1);
    Interrupt_enable(INT_XINT2);

    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // GPIO10 & GPIO11 are outputs, start GPIO10 high and GPIO11 low
    //

    //
    // Load the output latch
    //
    GPIO_writePin(10, 1);
    GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);           // output

    //
    // Load the output latch
    //
    GPIO_writePin(11, 0);
    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);           // output

    //
    // GPIO0 and GPIO1 are configured as inputs
    //
    GPIO_setDirectionMode(0, GPIO_DIR_MODE_IN);             // input

    //
    // XINT1 Synch to SYSCLKOUT only
    //
    GPIO_setQualificationMode(0, GPIO_QUAL_SYNC);
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_IN);             // input

    //
    // XINT2 Qual using 6 samples
    //
    GPIO_setQualificationMode(1, GPIO_QUAL_6SAMPLE);

    //
    // Set qualification period for GPIO0 to GPIO7
    // Each sampling window is 510*SYSCLKOUT
    //
    GPIO_setQualificationPeriod(3, 510);

    //
    // GPIO0 is mapped to XINT1, GPIO1 is mapped to XINT2
    //
    GPIO_setInterruptPin(0, GPIO_INT_XINT1);
    GPIO_setInterruptPin(1, GPIO_INT_XINT2);

    //
    // Configure falling edge trigger for XINT1
    //
    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_FALLING_EDGE);

    //
    // Configure rising edge trigger for XINT2
    //
    GPIO_setInterruptType(GPIO_INT_XINT2, GPIO_INT_TYPE_RISING_EDGE);

    //
    // Enable XINT1 and XINT2
    //
    GPIO_enableInterrupt(GPIO_INT_XINT1);         // Enable XINT1
    GPIO_enableInterrupt(GPIO_INT_XINT2);         // Enable XINT2

    //
    // GPIO16 will go low inside each interrupt.  Monitor this on a scope
    //
    GPIO_setDirectionMode(16, GPIO_DIR_MODE_OUT);  // output


    //
    // IDLE loop:
    //
    for(;;)
    {
        tempX1Count = xint1Count;
        tempX2Count = xint2Count;

        //
        // Set GPIO16
        //
        GPIO_writePin(16, 1);

        //
        // Lower GPIO10, trigger XINT1
        //
        GPIO_clearPortPins(GPIO_PORT_A, GPIO_GPADIR_GPIO10);
        while(xint1Count == tempX1Count) {}

        //
        // Set GPIO16
        //
        GPIO_writePin(16, 1);

        //
        // Wait for Qual period
        //
        DEVICE_DELAY_US(DELAY);

        //
        // Raise GPIO11, trigger XINT2
        //
        GPIO_writePin(11, 1);
        while(xint2Count == tempX2Count) {}

        //
        // Check that the counts were incremented properly and get ready
        // to start over.
        //
        if((xint1Count == tempX1Count + 1) && (xint2Count == tempX2Count + 1))
        {
            loopCount++;

            //
            // Raise GPIO10
            //
            GPIO_writePin(10, 1);

            //
            // Lower GPIO11
            //
            GPIO_writePin(11, 0);
        }
        else
        {
            ESTOP0;
        }
    }
}

//
// xint1ISR - External Interrupt 1 ISR
//
interrupt void xint1ISR(void)
{
    GPIO_writePin(16, 0); // GPIO16 is lowered
    xint1Count++;

    //
    // Acknowledge this interrupt to get more from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// xint2ISR - External Interrupt 2 ISR
//
interrupt void xint2ISR(void)
{
    GPIO_writePin(16, 0); // GPIO16 is lowered
    xint2Count++;

    //
    // Acknowledge this interrupt to get more from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// End of file
//
