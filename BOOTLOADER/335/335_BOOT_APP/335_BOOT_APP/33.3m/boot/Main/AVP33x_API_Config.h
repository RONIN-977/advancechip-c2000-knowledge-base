/******************************************************************************
文件名                     : AVP33x_API_Config.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : API 配置头文件
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef FLASH2833X_API_CONFIG_H
#define FLASH2833X_API_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Variables that can be configured by the user. 

/*-----------------------------------------------------------------------------
   1. Specify the device.
      Define the device to be programmed as "1" (no quotes).
      Define all other devices as "0" (no quotes).  
-----------------------------------------------------------------------------*/

#define FLASH_F28335   1
#define FLASH_F28334   0
#define FLASH_F28332   0

/*-----------------------------------------------------------------------------
   2. Specify the clock rate of the CPU (SYSCLKOUT) in nS.

      Take into account the input clock frequency and the PLL multiplier
      that your application will use.
 
      Use one of the values provided, or define your own.
      The trailing L is required tells the compiler to treat 
      the number as a 64-bit value.  

      Only one statement should be uncommented.

      Example:  CLKIN is a 30MHz crystal. 
 
                If the application will set PLLCR = 0xA then the CPU clock 
                will be 150Mhz (SYSCLKOUT = 150MHz).  

                In this case, the CPU_RATE will be 6.667L
                Uncomment the line:  #define CPU_RATE  6.667L   
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Flash2833x_API_Config.h
-----------------------------------------------------------------------------*/

#ifndef CPU_RATE
#define CPU_RATE (1000000000.0L / UPGRADE_CPU_HZ)
#endif
//#define CPU_RATE   10.000L   // for a 100MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   13.330L   // for a 75MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   20.000L   // for a 50MHz CPU clock speed  (SYSCLKOUT)
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

#endif // -- end FLASH2833X_API_CONFIG_H 
