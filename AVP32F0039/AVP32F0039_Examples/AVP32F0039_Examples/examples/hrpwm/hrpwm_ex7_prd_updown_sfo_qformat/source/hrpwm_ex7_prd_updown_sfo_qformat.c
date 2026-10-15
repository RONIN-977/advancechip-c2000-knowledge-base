/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   hrpwm_ex7_prd_updown_sfo_qformat.c
*                         TITLE:  HRPWM SFO V8 High-Resolution Period (Up-Down Count) example
*******************************************************************************************/
//! This example modifies the MEP control registers to show edge displacement
//! for high-resolution period with ePWM in Up-Down count mode
//! due to the HRPWM control extension of the respective ePWM module.
//!
//! To run this example:
//! -# Run this example at maximum SYSCLKOUT
//! -# Activate Real time mode
//! -# Run the code
//!
//! \b External \b Connections \n
//!  - Monitor ePWM1/2 A/B pins on an oscilloscope.
//!
//! \b Watch \b Variables \n
//!  - updateFine - Set to 1 use HRPWM capabilities and observe in fine MEP
//!                 steps(default)
//!                 Set to 0 to disable HRPWM capabilities and observe in
//!                 coarse SYSCLKOUT cycle steps
//!
//
//#############################################################################
//
// Included Files
//
#include "sfo_v8.h"
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Defines
//

// # of PWM channels
#define PWM_CH            2
#define STATUS_SUCCESS    1
#define STATUS_FAIL       0

//
// Globals
//
uint16_t updateFine, periodFine, status;

//
// Global variable used by the SFO library
// Result can be used for all HRPWM channels
// This variable is also copied to HRMSTEP
// register by SFO(0) function.
//
int MEP_ScaleFactor = 0;

//
// Used by SFO library (ePWM[0] is a dummy value that isn't used)
//
volatile uint32_t ePWM[PWM_CH + 1] = {0, EPWM1_BASE, EPWM2_BASE};



//
// Function Prototypes
//
void error(void);

//
// Main
//
void main(void)
{
    uint16_t i;
    uint32_t base;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pull-ups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Setup example variables
    //
    updateFine = 1;
    periodFine = 0;
    status = SFO_INCOMPLETE;

    //
    // Calling SFO() updates the HRMSTEP register with calibrated
    // MEP_ScaleFactor. HRMSTEP must be populated with a scale factor
    // value prior to enabling high resolution period control.
    //
    while(status == SFO_INCOMPLETE)
    {
        status = SFO();
        if(status == SFO_ERROR)
        {
            //
            // SFO function returns 2 if an error occurs & # of MEP
            // steps/coarse step exceeds maximum of 255.
            //
            error();
        }
    }

    //
    // Configure ePWM1 and ePWM2 GPIOs
    //
    Board_init();

    //
    // Enable TBCLK within EPWM.
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    for(;;)
    {
        //
        // Sweep periodFine as a Q16 number from 0.2 - 0.999
        //
        for(periodFine = 0x3333; periodFine < 0xFFBF; periodFine++)
        {
            if(updateFine)
            {
                //
                // Because auto-conversion is enabled, the desired
                // fractional period must be written directly to the
                // TBPRDHR (or TBPRDHRM) register in Q16 format
                // (lower 8-bits are ignored)
                //
                // EPwm1Regs.TBPRDHR = periodFine;
                //
                // The hardware will automatically scale
                // the fractional period by the MEP_ScaleFactor
                // in the HRMSTEP register (which is updated
                // by the SFO calibration software).
                //
                // Hardware conversion:
                // MEP delay movement = ((TBPRDHR(15:0) >> 8) *  HRMSTEP(7:0) +
                //                       0x80) >> 8
                //
                for(i = 0; i < PWM_CH; i++)
                {
                    base = EPWM1_BASE + (i * 0x100U);

                    //
                    // Write fractional period value in Q16 format
                    //
                    HWREGH(base + HRPWM_O_TBPRDHR) = periodFine;
                }
            }
            else
            {
                //
                // No high-resolution movement on TBPRDHR.
                //
                for(i = 0; i < PWM_CH; i++)
                {
                    HWREGH(base + HRPWM_O_TBPRDHR) = 0U;
                }
            }

            //
            // Call the scale factor optimizer lib function SFO(0)
            // periodically to track for any change due to temp/voltage.
            // This function generates MEP_ScaleFactor by running the
            // MEP calibration module in the HRPWM logic. This scale
            // factor can be used for all HRPWM channels. HRMSTEP
            // register is automatically updated by the SFO function.
            // In background, MEP calibration module continuously updates
            // MEP_ScaleFactor
            //
            status = SFO();

            if(status == SFO_ERROR)
            {
                //
                // SFO function returns 2 if an error occurs & # of
                // MEP steps/coarse step exceeds maximum of 255.
                //
                error();
            }
        }
    }
}

// error - Halt debugger when error occurs
//
void error (void)
{
    //
    // Stop here and handle error
    //
    ESTOP0;
}

//
// End of file
//
