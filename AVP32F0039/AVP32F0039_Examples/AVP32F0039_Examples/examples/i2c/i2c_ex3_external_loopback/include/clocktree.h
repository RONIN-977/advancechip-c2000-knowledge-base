/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   clockTree.h
*                         TITLE:  Setups device clocking for examples.
*******************************************************************************************/
#ifndef CLOCKTREE_H
#define CLOCKTREE_H

//*****************************************************************************
//
// Summary of SYSPLL related clock configuration
//
//*****************************************************************************
//
// Input Clock to SYSPLL (OSCCLK)	= 10 MHz    (INTOSC2 provides OSCCLK)
//
//##### SYSPLL ENABLED #####
//
// PLLRAWCLK				= 240 MHz   (Output of SYSPLL if enabled) 
// PLLSYSCLK				= 120 MHz
// CPUCLK					= 120 MHz
// SYSCLK					= 120 MHz
// LSPCLK					= 60 MHz 
	
//*****************************************************************************
//
// Macro definitions used in device.c (SYSPLL / LSPCLK)
//
//*****************************************************************************
//
//	Input Clock to SYSPLL (OSCCLK) = INTOSC2 = 10 MHz
//
#define DEVICE_OSCSRC_FREQ          10000000U
//
// Define to pass to SysCtl_setClock(). Will configure the clock as follows:
// SYSPLL ENABLED
// SYSCLK = 120 MHz = 10 MHz (OSCCLK) * 24 (IMULT) / (1 (REFDIV) * 1 (ODIV) * 2 (SYSCLKDIVSEL))
#define DEVICE_SYSCLK_FREQ          ((DEVICE_OSCSRC_FREQ * 24) / (1 * 1 * 2))
//
#define DEVICE_SETCLOCK_CFG         (SYSCTL_OSCSRC_OSC2  | SYSCTL_IMULT(24) | \
									 SYSCTL_REFDIV(1) | SYSCTL_ODIV(1)| \
									 SYSCTL_SYSDIV(2) | SYSCTL_PLL_ENABLE | \
									 SYSCTL_DCC_BASE_0)
									 

//
// Define to pass to SysCtl_setLowSpeedClock().
// Low Speed Clock (LSPCLK) = 120 MHz / 2 = 60 MHz
//
#define DEVICE_LSPCLK_CFG  			SYSCTL_LSPCLK_PRESCALE_2

#define DEVICE_LSPCLK_FREQ          (DEVICE_SYSCLK_FREQ / 2)

//*****************************************************************************
//
// CPUCLK Domain (120 MHz)
//
//*****************************************************************************
// FPU
// TMU
// Flash
//	
//*****************************************************************************
//
// SYSCLK Domain (120 MHz)
//
//*****************************************************************************
// ePIE
// CLA
// GPIO
// MxRAMs
// LSxRAMs
// GSxRAMs
// BootROM
// MessageRAMs
// DCSM
// SystemControl
// WD
// XINT
//	

/////////////////////	
// Gated CPU1 SYSCLK
/////////////////////
// CPUTIMERs
// CLB
// ECAP
// EQEP
// EPWM
// HRCAL
// PMBUS
// LIN
// FSI
// SDFM
// EPG
// AES
// I2C
// ADC
// CMPSS
// GPDAC
// CAN
// MCAN
// HIC
// DCC
// HWBIST
// BGCRC
// ERAD
//

//*****************************************************************************
//
// Gated Peripheral SYSCLK Domain (120 MHz) 
//
//*****************************************************************************
// CPUTIMERs
// CLB
// ECAP
// EQEP
// EPWM
// HRCAL
// PMBUS
// LIN
// FSI
// SDFM
// EPG
// AES
// I2C
// ADC
// CMPSS
// GPDAC
// CAN
// MCAN
// HIC
// DCC
// HWBIST
// BGCRC
// ERAD
//	

//*****************************************************************************
//
// Gated LSPCLK Domain (60 MHz) 
//
//*****************************************************************************
// SCI
// SPI

#endif // CLOCKTREE_H

