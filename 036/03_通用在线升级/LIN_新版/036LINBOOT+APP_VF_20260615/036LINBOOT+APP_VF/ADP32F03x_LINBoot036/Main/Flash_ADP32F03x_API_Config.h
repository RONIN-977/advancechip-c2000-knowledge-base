//###########################################################################
//		         PICCOLO B 
//###########################################################################
// FILE:  Flash_ADP32F03x_API_Config.h
//
// TITLE: ADP32F03x Flash Algo's - User Settings
//
// NOTE:  This file contains user defined settings that
//        are used by the ADP32F03x Flash APIs.
//
//###########################################################################

#ifndef FLASH_ADP32F03x_API_CONFIG_H
#define FLASH_ADP32F03x_API_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Variables that can be configured by the user. 

/*-----------------------------------------------------------------------------
   1. Specify the device.
      Define the device to be programmed as "1" (no quotes).
      Define all other devices as "0" (no quotes).  
-----------------------------------------------------------------------------*/

#define FLASH_F2803x   1

#define FLASH_F28035   1
#define FLASH_F28034   0
#define FLASH_F28033   0
#define FLASH_F28032   0
 
#define   FLASH_ADDR_BASE                  0x3E8000
#define   FLASH_FLAG1                      ( *(Uint16*)(FLASH_ADDR_BASE + 1) )
#define   FLASH_FLAG2                      ( *(Uint16*)(FLASH_ADDR_BASE + 2) )
#define   FLASH_FLAG3                      ( *(Uint16*)(FLASH_ADDR_BASE + 3) )
#define   FLASH_FLAG4                      ( *(Uint16*)(FLASH_ADDR_BASE + 4) )
#define   FLASH_FLAG_ADDR                  (FLASH_ADDR_BASE + 1)
#define   FLASH_DATA_ADDR                  (FLASH_ADDR_BASE + 10)

#define   FlashReadData(ADDR)              (*(Uint16 *)(FLASH_DATA_ADDR+(ADDR)))

//#define   FLASH_DATA_ADDR                  0x3E8010
//#define   FLASH_FLAG_ADDR                  0x3E8001
/*-----------------------------------------------------------------------------
   2. Specify the clock rate of the CPU (SYSCLKOUT) in nS.

      Take into account the input clock frequency and the PLL multiplier
      that your application will use.
 
      Use one of the values provided, or define your own.
      The trailing L is required tells the compiler to treat 
      the number as a 64-bit value.  

      Only one statement should be uncommented.

      Example:  CLKIN is 10MHz. 
 
                If the application will set PLLCR = 0xC and the DIVSEL to /2
                then the CPU clock will be 60Mhz (SYSCLKOUT = 60MHz).  

                In this case, the CPU_RATE will be 16.667L
                Uncomment the line:  #define CPU_RATE  16.667L   
-----------------------------------------------------------------------------*/

#define CPU_RATE   10.000L //16.667L   // for a 60MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   20.000L   // for a 50MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   25.000L   // for a 40MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   33.333L   // for a 30MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   41.667L   // for a 24MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   50.000L   // for a 20MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   66.667L   // for a 15MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE  100.000L   // for a 10MHz CPU clock speed  (SYSCLKOUT)

//----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// **** DO NOT modify the code below this line ****
//-----------------------------------------------------------------------------
#define SCALE_FACTOR  1048576.0L*( (200L/CPU_RATE) )  // IQ20


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // -- end FLASH_ADP32F03x_API_CONFIG_H 
