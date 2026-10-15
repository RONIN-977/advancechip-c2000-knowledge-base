/******************************************************************************
文件名                     : AVP33x_Device.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP32F335设备定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_DEVICE_H
#define AVP33x_DEVICE_H


#ifdef __cplusplus
extern "C" {
#endif


#define   TARGET   1
//---------------------------------------------------------------------------
// 用户选择目标文件

#define   DSP28_28335   TARGET    // Selects '28335/'28235
#define   DSP28_28334   0         // Selects '28334/'28234
#define   DSP28_28332   0         // Selects '28332/'28232


//---------------------------------------------------------------------------
// 同样 CPU宏定义:
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

#define M_INT1      0x0001
#define M_INT2      0x0002
#define M_INT3      0x0004
#define M_INT4      0x0008
#define M_INT5      0x0010
#define M_INT6      0x0020
#define M_INT7      0x0040
#define M_INT8      0x0080
#define M_INT9      0x0100
#define M_INT10     0x0200
#define M_INT11     0x0400
#define M_INT12     0x0800
#define M_INT13     0x1000
#define M_INT14     0x2000
#define M_DLOG      0x4000
#define M_RTOS      0x8000

#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000


//---------------------------------------------------------------------------
// 为便于移植，建议用户对16位和32位有符号/无符号整数使用以下数据类型大小定义:
//

#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef int                 int16;
typedef long                int32;
typedef long long           int64;
typedef unsigned int        Uint16;
typedef unsigned long       Uint32;
typedef unsigned long long  Uint64;
typedef float               float32;
typedef long double         float64;
#endif


//---------------------------------------------------------------------------
// 包含所有外设及其他配置头文件:
//

#include "AVP33x_Adc.h"               // ADC寄存器
#include "AVP33x_DevEmu.h"            // 仿真控制寄存器
#include "AVP33x_CpuTimers.h"         // 32位CPU定时器
#include "AVP33x_ECan.h"              // eCAN寄存器
#include "AVP33x_ECap.h"              // 增强型捕获寄存器
#include "AVP33x_DMA.h"               // DMA寄存器
#include "AVP33x_EPwm.h"              // 增强型PWM
#include "AVP33x_EQep.h"              // 增强型QEP
#include "AVP33x_Gpio.h"              // 通用IO控制寄存器
#include "AVP33x_I2c.h"               // I2C寄存器
#include "AVP33x_Mcbsp.h"             // McBSP寄存器
#include "AVP33x_PieCtrl.h"           // PIE控制寄存器
#include "AVP33x_PieVect.h"           // PIE向量表
#include "AVP33x_Spi.h"               // SPI寄存器
#include "AVP33x_Sci.h"               // SCI寄存器
#include "AVP33x_SysCtrl.h"           // 系统控制
#include "AVP33x_XIntrupt.h"          // 外部中断
#include "AVP33x_Xintf.h"             // XINTF外部接口
#include "AVP33x_AIC23.h"             // AIC23音频模块头文件

#if DSP28_28335
#define DSP28_EPWM1  1
#define DSP28_EPWM2  1
#define DSP28_EPWM3  1
#define DSP28_EPWM4  1
#define DSP28_EPWM5  1
#define DSP28_EPWM6  1
#define DSP28_ECAP1  1
#define DSP28_ECAP2  1
#define DSP28_ECAP3  1
#define DSP28_ECAP4  1
#define DSP28_ECAP5  1
#define DSP28_ECAP6  1
#define DSP28_EQEP1  1
#define DSP28_EQEP2  1
#define DSP28_ECANA  1
#define AVP335_ECANB  1
#define DSP28_MCBSPA 1
#define DSP28_MCBSPB 1
#define DSP28_SPIA   1
#define DSP28_SCIA   1
#define DSP28_SCIB   1
#define DSP28_SCIC   1
#define DSP28_I2CA   1
#endif  // end DSP28_28335

#if DSP28_28334
#define DSP28_EPWM1  1
#define DSP28_EPWM2  1
#define DSP28_EPWM3  1
#define DSP28_EPWM4  1
#define DSP28_EPWM5  1
#define DSP28_EPWM6  1
#define DSP28_ECAP1  1
#define DSP28_ECAP2  1
#define DSP28_ECAP3  1
#define DSP28_ECAP4  1
#define DSP28_ECAP5  0
#define DSP28_ECAP6  0
#define DSP28_EQEP1  1
#define DSP28_EQEP2  1
#define DSP28_ECANA  1
#define AVP335_ECANB  1
#define DSP28_MCBSPA 1
#define DSP28_MCBSPB 1
#define DSP28_SPIA   1
#define DSP28_SCIA   1
#define DSP28_SCIB   1
#define DSP28_SCIC   1
#define DSP28_I2CA   1
#endif  // end DSP28_28334

#if DSP28_28332
#define DSP28_EPWM1  1
#define DSP28_EPWM2  1
#define DSP28_EPWM3  1
#define DSP28_EPWM4  1
#define DSP28_EPWM5  1
#define DSP28_EPWM6  1
#define DSP28_ECAP1  1
#define DSP28_ECAP2  1
#define DSP28_ECAP3  1
#define DSP28_ECAP4  1
#define DSP28_ECAP5  0
#define DSP28_ECAP6  0
#define DSP28_EQEP1  1
#define DSP28_EQEP2  1
#define DSP28_ECANA  1
#define AVP335_ECANB  1
#define DSP28_MCBSPA 1
#define DSP28_MCBSPB 0
#define DSP28_SPIA   1
#define DSP28_SCIA   1
#define DSP28_SCIB   1
#define DSP28_SCIC   0
#define DSP28_I2CA   1
#endif  // end DSP28_28332

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_DEVICE_H definition


//===========================================================================
// End of file.
//===========================================================================
