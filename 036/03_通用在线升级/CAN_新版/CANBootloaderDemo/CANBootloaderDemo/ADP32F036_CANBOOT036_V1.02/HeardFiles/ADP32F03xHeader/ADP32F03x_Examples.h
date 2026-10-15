//#############################################################################
//
// FILE:   ADP32F03x_Examples.h
//
// TITLE:  ADP32F03x example support definitions.
//
// Description:
// This file provides common device configuration macros, clock setup options,
// part number identifiers, and shared include files used by ADP32F03x
// example projects.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef ADP32F03X_EXAMPLES_H
#define ADP32F03X_EXAMPLES_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/

/* Device configuration defines */
#define DSP_CLOCK   60
#define CPU_TYPE    ADP32F036

//
// PLL and SYSCLKOUT Configuration
//

//#define DSP28_DIVSEL   0  // Enable /4 for SYSCLKOUT.
//#define DSP28_DIVSEL   1  // Disable /4 for SYSCLKOUT.
//#define DSP28_DIVSEL   2  // Enable /2 for SYSCLKOUT.
#define DSP28_DIVSEL   3    // Enable /1 for SYSCLKOUT.

//#define DSP28_PLLCR   12   // 60 MHz devices: (10 MHz * 12) / 2.
//#define DSP28_PLLCR   11
#define DSP28_PLLCR   10
//#define DSP28_PLLCR    9
//#define DSP28_PLLCR    8   // 40 MHz devices: (10 MHz * 8) / 2.
//#define DSP28_PLLCR    7
//#define DSP28_PLLCR    6
//#define DSP28_PLLCR    5
//#define DSP28_PLLCR    4
//#define DSP28_PLLCR    3
//#define DSP28_PLLCR    2
//#define DSP28_PLLCR    1
//#define DSP28_PLLCR    0   // PLL is bypassed in this mode.

//
// CPU Rate Selection
//
// Select one CPU rate definition that matches the final SYSCLKOUT value.
// The trailing L keeps the value in long-double form for the delay macro.
// Example: 60 MHz uses CPU_RATE = 16.667L.
//
#define CPU_RATE   10.000L   // 100 MHz CPU clock speed.
//#define CPU_RATE   16.667L  // 60 MHz CPU clock speed.
//#define CPU_RATE   20.000L  // 50 MHz CPU clock speed.
//#define CPU_RATE   25.000L  // 40 MHz CPU clock speed.
//#define CPU_RATE   33.333L  // 30 MHz CPU clock speed.
//#define CPU_RATE   41.667L  // 24 MHz CPU clock speed.
//#define CPU_RATE   50.000L  // 20 MHz CPU clock speed.
//#define CPU_RATE   66.667L  // 15 MHz CPU clock speed.
//#define CPU_RATE  100.000L  // 10 MHz CPU clock speed.

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/

/* Calibration and shared includes */
// Pointer to the factory calibration routine for the ADC and internal
// oscillators.
#define Device_cal (void (*)(void))0x3D7C80

#include <ADP32F03x_GlobalPrototypes.h>
#include <ADP32F03x_EPwm_defines.h>
#include <ADP32F03x_I2C_defines.h>
#include <ADP32F03x_Cla_Defines.h>

//
// Device Part Number Definitions
//
#define PARTNO_28030PAG   0xAA
#define PARTNO_28030PN    0xAB

#define PARTNO_28031PAG   0xAE
#define PARTNO_28031PN    0xAF

#define PARTNO_28032PAG   0xB2
#define PARTNO_28032PN    0xB3

#define PARTNO_28033PAG   0xB6
#define PARTNO_28033PN    0xB7

#define PARTNO_28034PAG   0xBA
#define PARTNO_28034PN    0xBB

#define PARTNO_28035PAG   0xBE
#define PARTNO_28035PN    0xBF

//
// Optional BIOS-Specific Includes
//
#ifndef DSP28_BIOS
#include <ADP32F03x_DefaultISR.h>
#endif

//
// Delay Macro
//
// DO NOT MODIFY THIS LINE.
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
