/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   hrpwm_ex6_duty_sfo_qformat.c
*                         TITLE:  HRPWM SFO V8 High-Resolution Period
*******************************************************************************************/
//! This example modifies the MEP control registers to show edge displacement
//! for high-resolution period with ePWM in Up count mode
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
//!  - status - Example run status
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
#include "board.h"

//
// Defines
//

//
// # of PWM channels
//
#define PWM_CH            2
#define STATUS_SUCCESS    1
#define STATUS_FAIL       0

//
// 1 = Turn auto-conversion ON
// 0 = Turn auto-conversion OFF
//
#define AUTOCONVERT       1

//
// Globals
//
uint16_t updateFine;
uint16_t dutyFine;
uint16_t status;
uint16_t regValCMPA;
uint16_t regValCMPAHR;
uint16_t regValCMPB;
uint16_t regValCMPBHR;

//
// Global variable used by the SFO library
// Result can be used for all HRPWM channels
// This variable is also copied to HRMSTEP
// register by SFO() function.
//
int MEP_ScaleFactor;

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
    uint32_t temp, temp1, base;
    uint16_t periodVal;

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
    dutyFine = 0;
    status = SFO_INCOMPLETE;

    //
    // Calling SFO() updates the HRMSTEP register with calibrated
    // MEP_ScaleFactor. HRMSTEP must be populated with a scale factor value
    // prior to enabling high resolution period control.
    //
    while(status == SFO_INCOMPLETE)
    {
        status = SFO();
        if(status == SFO_ERROR)
        {
            error();   // SFO function returns 2 if an error occurs & # of MEP
        }              // steps/coarse step exceeds maximum of 255.
    }


    //
    // Configure ePWM1 and ePWM2
    //
    Board_init();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    while(1)
    {
        //
        // Sweep dutyFine as a Q15 number from 0.2 - 0.999
        //
        for(dutyFine = 0x2300U; dutyFine < 0x7000U; dutyFine++)  
        {
            if(updateFine)
            {
                /*
                // All below calculation apply for CMPB as well
                // regValCMPA , regValCMPAHR are calculated as Q0.
                // Since dutyFine is a Q15 number, and the period is Q0
                // the product is Q15. So to store as a Q0, we shift right
                // 15 bits.
                regValCMPA = ((long)dutyFine * periodVal) >> 15U;

                // This next step is to obtain the remainder which was
                // truncated during our 15 bit shift above.
                // Compute the whole value, and then subtract regValCMPA
                // shifted LEFT 15 bits:
                temp = ((long)dutyFine * periodVal) ;
                temp = temp - ((long)regValCMPA << 15);

                // If auto-conversion is disabled, the following step can be
                // skipped. If autoconversion is enabled, the SFO function will
                // write the MEP_ScaleFactor to the HRMSTEP register and the
                // hardware will automatically scale the remainder in the
                // CMPAHR register by the MEP_ScaleFactor.
                // Because the remainder calculated above (temp) is in Q15
                // format, it must be shifted left by 1 to convert to Q16
                // format for the hardware to properly convert.
                regValCMPAHR = temp << 1;

                // If auto-conversion is enabled, the following step is
                // performed automatically in hardware and can be skipped
                // This obtains the MEP count in digits, from
                // 0,1, .... MEP_Scalefactor.
                // 0x0080 (0.5 in Q8) is converted to 0.5 in Q15 by shifting
                // left 7. This is added to fractional duty*MEP_SF product in
                // order to round the decimal portion of the product up to the
                // next integer if the decimal portion is >=0.5.
                //
                // Once again since this is Q15
                // convert to Q0 by shifting:
                regValCMPAHR = (temp * MEP_ScaleFactor + (0x0080 << 7)) >> 15;

                // If auto-conversion is enabled, the following step is
                // performed automatically in hardware and can be skipped
                // Add the offset and rounding
                regValCMPAHR += 0x0080;

                // Write the values to the registers as one 32-bit
                // or two 16-bits
                HWREG(base + HRPWM_O_CMPA) = ((long)regValCMPA) << 16U |
                                               regValCMPAHR;
                HWREG(base + HRPWM_O_CMPB) = ((long)regValCMPA) << 16U |
                                               regValCMPBHR;
                */

                //
                // All the above operations may be condensed into
                // the following form:
                // EPWM1 calculations
                //
                for(i = 0; i < PWM_CH; i++)
                {
                    base = ePWM[i+1];
                    periodVal = EPWM_getTimeBasePeriod(base);
                    regValCMPA = ((long)dutyFine * periodVal) >> 15U;
                    regValCMPB = ((long)dutyFine * periodVal) >> 15U;
                    temp = ((long)dutyFine * periodVal) ;
                    temp1 = ((long)dutyFine * periodVal) ;
                    temp = temp - ((long)regValCMPA << 15U);
                    temp1 = temp1 - ((long)regValCMPB << 15U);

                   #if(AUTOCONVERT)
                    regValCMPAHR = temp << 1U; // convert to Q16
                    regValCMPBHR = temp << 1U; // convert to Q16
                   #else

                    regValCMPAHR = ((temp * MEP_ScaleFactor) +
                                    (0x0080U << 7U)) >> 15U;
                    regValCMPAHR = regValCMPAHR << 8;
                    regValCMPBHR = ((temp1 * MEP_ScaleFactor) +
                                    (0x0080U << 7U)) >> 15U;
                    regValCMPBHR = regValCMPBHR << 8U;
                   #endif

                   //
                   // Example for a 32 bit write to CMPA:CMPAHR
                   //
                   HWREG(base + HRPWM_O_CMPA) = (((long)regValCMPA) << 16U) |
                                                regValCMPAHR;

                   //
                   // Example for a 32 bit write to CMPB:CMPBHR
                   //
                   HWREG(base + HRPWM_O_CMPB) = (((long)regValCMPB) << 16U) |
                                                regValCMPBHR;
                }
            }
            else
            {
                //
                // regValCMPA is calculated as a Q0.
                // Since dutyFine is a Q15 number, and the period is Q0
                // the product is Q15. So to store as a Q0, we shift right
                // 15 bits.
                //
                for(i = 0; i < PWM_CH; i++)
                {
                    base = ePWM[i+1];
                    periodVal = EPWM_getTimeBasePeriod(base);
                    regValCMPA = ((long)dutyFine * periodVal);
                    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_A,
                                                regValCMPA);
                    regValCMPB = ((long)dutyFine * periodVal);
                    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_B,
                                                regValCMPB);
                }
            }

            //
            // Call the scale factor optimizer lib function SFO()
            // periodically to track for any change due to temp/voltage.
            // This function generates MEP_ScaleFactor by running the
            // MEP calibration module in the HRPWM logic. This scale
            // factor can be used for all HRPWM channels. The SFO()
            // function also updates the HRMSTEP register with the
            // scale factor value.
            //
            status = SFO(); // in background, MEP calibration module
                            // continuously updates MEP_ScaleFactor

            if(status == SFO_ERROR)
            {
                error();   // SFO function returns 2 if an error occurs & #
                           // of MEP steps/coarse step exceeds maximum of 255.
            }
        } // end dutyFine for loop
    } // end infinite for loop
}
//
// error - Halt debugger when called
//
void error(void)
{
    ESTOP0;         // Stop here and handle error
}

//
// End of file
//
