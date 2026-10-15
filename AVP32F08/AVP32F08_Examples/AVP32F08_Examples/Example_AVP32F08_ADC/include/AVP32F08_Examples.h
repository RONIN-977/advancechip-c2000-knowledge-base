/******************************************************************************
文件名                     : AVP32F08_Examples.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP32F335例程相关定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP32F08_EXAMPLES_H
#define AVP32F08_EXAMPLES_H


#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
      PLL控制寄存器(PLLCR)和分频系数(DIVSEL)值宏定义
-----------------------------------------------------------------------------*/
//#define DSP28_DIVSEL  0       // 4分频
//#define DSP28_DIVSEL  1       // 4分频
#define DSP28_DIVSEL    2       // 2分频
//#define DSP28_DIVSEL  3       // 1分频

#define DSP28_PLLCR     10
//#define DSP28_PLLCR    9
//#define DSP28_PLLCR    8
//#define DSP28_PLLCR    7
//#define DSP28_PLLCR    6
//#define DSP28_PLLCR    5
//#define DSP28_PLLCR    4
//#define DSP28_PLLCR    3
//#define DSP28_PLLCR    2
//#define DSP28_PLLCR    1
//#define DSP28_PLLCR    0      // PLL为旁路
//----------------------------------------------------------------------------


/*-----------------------------------------------------------------------------
主频时钟周期宏定义，单位：ns。
        要注意上面所配置的时钟频率和倍频系数。
        可使用下面所提供的值之一，或定义自己所需的值。
        尾随的L是必需的，它告诉编译器将数字视为64位值。
        只能留一个宏定义不被注释。

   eg1:(主频150MHz)
       CLKIN为30MHz晶振。
                     在上面的步骤中设置PLLCR = 0xA可得到150MHz的主频时钟（即SYSCLKOUT = 150MHz）。
                     在下面的宏定义选择时，应选择6.667L（即去掉#define CPU_RATE  6.667L这句宏定义的注释）。

   eg2:(主频100MHz)
       CLKIN为20MHz晶振。
                     在上面的步骤中设置PLLCR = 0xA可得到100MHz的主频时钟（即SYSCLKOUT = 100MHz）。
                     在下面的宏定义选择时，应选择10.000L（即去掉#define CPU_RATE  10.000L这句宏定义的注释）。

-----------------------------------------------------------------------------*/
#define CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    7.143L   // for a 140MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE    8.333L   // for a 120MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   10.000L   // for a 100MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   13.330L   // for a 75MHz CPU clock speed (SYSCLKOUT)
//#define CPU_RATE   20.000L   // for a 50MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   33.333L   // for a 30MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   41.667L   // for a 24MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   50.000L   // for a 20MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   66.667L   // for a 15MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE  100.000L   // for a 10MHz CPU clock speed  (SYSCLKOUT)

//----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
        芯片决定了CPU主频，用户不需要在这里做任何改变。
-----------------------------------------------------------------------------*/
#if DSP28_28332                   // DSP28_28332 device only
  #define CPU_FRQ_120MHZ    1
  #define CPU_FRQ_150MHZ    0
#else
  #define CPU_FRQ_120MHZ    0
  #define CPU_FRQ_150MHZ    1
#endif


//---------------------------------------------------------------------------
// 包含例程头文件:
//

#include "AVP32F08_GlobalPrototypes.h"      // .c文件中的全局函数原型

#include "AVP32F08_EPwm_defines.h"          // PWM例程中用到的宏定义头文件
#include "AVP32F08_Dma_defines.h"           // DMA例程中用到的宏定义头文件
#include "AVP32F08_I2c_defines.h"           // I2C例程中用到的宏定义头文件

#define PARTNO_28335  0xEF
#define PARTNO_28334  0xEE
#define PARTNO_28332  0xED
#define PARTNO_28235  0xE8
#define PARTNO_28234  0xE7
#define PARTNO_28232  0xE6


// 包含不与DSP/BIOS一起使用的头文件
#ifndef DSP28_BIOS
#include "AVP32F08_DefaultIsr.h"
#endif


// 请勿修改这条语句
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP32F08_EXAMPLES_H definition


//===========================================================================
// End of file.
//===========================================================================
