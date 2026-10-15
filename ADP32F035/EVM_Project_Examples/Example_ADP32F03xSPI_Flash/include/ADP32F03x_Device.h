//###########################################################################
//
// FILE:   ADP32F03x_Device.h
//
// TITLE:  ADP32F03x Device Definitions.
//
//###########################################################################

#ifndef ADP32F03x_DEVICE_H
#define ADP32F03x_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#define   TARGET   1
//---------------------------------------------------------------------------
// User To Select Target Device:


#define   DSP32_ADP32F030PAG   0
#define   DSP32_ADP32F030PN    0

#define   DSP32_ADP32F031PAG   0
#define   DSP32_ADP32F031PN    0

#define   DSP32_ADP32F032PAG   0
#define   DSP32_ADP32F032PN    0

#define   DSP32_ADP32F033PAG   0
#define   DSP32_ADP32F033PN    0

#define   DSP32_ADP32F034PAG   0
#define   DSP32_ADP32F034PN    0

#define   DSP32_ADP32F035PAG   0
#define   DSP32_ADP32F035PN    TARGET


//---------------------------------------------------------------------------
// Common CPU Definitions:
//

extern cregister volatile unsigned int IFR;
extern cregister volatile unsigned int IER;

#define  EINT   asm(" clrc INTM")
#define  DINT   asm(" setc INTM")
#define  ERTM   asm(" clrc DBGM")
#define  DRTM   asm(" setc DBGM")
#define  EALLOW asm(" EALLOW")
#define  EDIS   asm(" EDIS")
#define  ESTOP0 asm(" ESTOP0")

#define M_INT1  0x0001
#define M_INT2  0x0002
#define M_INT3  0x0004
#define M_INT4  0x0008
#define M_INT5  0x0010
#define M_INT6  0x0020
#define M_INT7  0x0040
#define M_INT8  0x0080
#define M_INT9  0x0100
#define M_INT10 0x0200
#define M_INT11 0x0400
#define M_INT12 0x0800
#define M_INT13 0x1000
#define M_INT14 0x2000
#define M_DLOG  0x4000
#define M_RTOS  0x8000

#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

//---------------------------------------------------------------------------
// For Portability, User Is Recommended To Use Following Data Type Size
// Definitions For 16-bit and 32-Bit Signed/Unsigned Integers:
//

#ifndef DSP32_DATA_TYPES
#define DSP32_DATA_TYPES
typedef char            int8;
typedef unsigned char   Uint8;
typedef int             int16;
typedef long            int32;
typedef unsigned int    Uint16;
typedef unsigned long   Uint32;
typedef float           float32;
typedef long double     float64;
#endif


//---------------------------------------------------------------------------
// Include All Peripheral Header Files:
//

#include "ADP32F03x_Adc.h"                // ADC Registers
#include "ADP32F03x_BootVars.h"           // Boot ROM Variables
#include "ADP32F03x_DevEmu.h"             // Device Emulation Registers
#include "ADP32F03x_Cla.h"                // Control Law Accelerator Registers
#include "ADP32F03x_Comp.h"               // Comparator Registers
#include "ADP32F03x_CpuTimers.h"          // 32-bit CPU Timers
#include "ADP32F03x_ECan.h"               // Enhanced eCAN Registers
#include "ADP32F03x_ECap.h"               // Enhanced Capture
#include "ADP32F03x_EPwm.h"               // Enhanced PWM
#include "ADP32F03x_EQep.h"               // Enhanced QEP
#include "ADP32F03x_Gpio.h"               // General Purpose I/O Registers
#include "ADP32F03x_HRCap.h"              // High Resolution Capture
#include "ADP32F03x_I2c.h"                // I2C Registers
#include "ADP32F03x_Lin.h"                // LIN Registers
#include "ADP32F03x_NmiIntrupt.h"         // NMI Interrupt Registers
#include "ADP32F03x_PieCtrl.h"            // PIE Control Registers
#include "ADP32F03x_PieVect.h"            // PIE Vector Table
#include "ADP32F03x_Spi.h"                // SPI Registers
#include "ADP32F03x_Sci.h"                // SCI Registers
#include "ADP32F03x_SysCtrl.h"            // System Control/Power Modes
#include "ADP32F03x_XIntrupt.h"           // External Interrupts


#if (DSP32_ADP32F035PN||DSP32_ADP32F034PN||DSP32_ADP32F033PN||DSP32_ADP32F032PN||DSP32_ADP32F031PN||DSP232_ADP32F030PN)
#define DSP32_COMP1 1
#define DSP32_COMP2 1
#define DSP32_COMP3 1
#define DSP32_EPWM1 1
#define DSP32_EPWM2 1
#define DSP32_EPWM3 1
#define DSP32_EPWM4 1
#define DSP32_EPWM5 1
#define DSP32_EPWM6 1
#define DSP32_EPWM7 1
#define DSP32_ECAP1 1
#define DSP32_EQEP1 1
#define DSP32_ECANA 1
#define DSP32_HRCAP1 1
#define DSP32_HRCAP2 1
#define DSP32_SPIA  1
#define DSP32_SPIB  1
#define DSP32_SCIA  1
#define DSP32_I2CA  1
#define DSP32_LINA  1
#endif

#if (DSP32_ADP32F035PAG||DSP32_ADP32F034PAG||DSP32_ADP32F033PAG||DSP32_ADP32F032PAG||DSP32_ADP32F031PAG||DSP32_ADP32F030PAG)
#define DSP32_COMP1 1
#define DSP32_COMP2 1
#define DSP32_COMP3 1
#define DSP32_EPWM1 1
#define DSP32_EPWM2 1
#define DSP32_EPWM3 1
#define DSP32_EPWM4 1
#define DSP32_EPWM5 1
#define DSP32_EPWM6 1
#define DSP32_EPWM7 0
#define DSP32_ECAP1 1
#define DSP32_EQEP1 1
#define DSP32_ECANA 1
#define DSP32_HRCAP1 1
#define DSP32_HRCAP2 1
#define DSP32_SPIA  1
#define DSP32_SPIB  0
#define DSP32_SCIA  1
#define DSP32_I2CA  1
#define DSP32_LINA  1
#endif

// Timer definitions based on System Clock
// 60 MHz devices 
	#define      mSec0_5          30000           // 0.5 mS
	#define      mSec0_75         45000           // 0.75 mS
	#define      mSec1            60000           // 1.0 mS
	#define      mSec2            120000          // 2.0 mS
	#define      mSec5            300000          // 5.0 mS
	#define      mSec7_5          450000          // 7.5 mS
	#define      mSec10           600000          // 10 mS
	#define      mSec20           1200000         // 20 mS
	#define      mSec50           3000000         // 50 mS
	#define      mSec75           4500000         // 75 mS
	#define      mSec100          6000000         // 100 mS
	#define      mSec200          12000000        // 200 mS
	#define      mSec500          30000000        // 500 mS
	#define      mSec750          45000000        // 750 mS
	#define      mSec1000         60000000        // 1000 mS
	#define      mSec2000         120000000       // 2000 mS
	#define      mSec5000         300000000       // 5000 mS

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of ADP32F03x_DEVICE_H definition

//===========================================================================
// End of file.
//===========================================================================
