/******************************************************************************
�ļ���                     : AVP33x_Examples.h
����                         : ���Ͻ�о���ӿƼ����޹�˾
��������                 : AVP32F335������ض���
����ʱ��                 : 06-25-2019
�汾                         : V1.0
�޶�ʱ��                 :
�޶�����                 :
*******************************************************************************/

#ifndef AVP33x_EXAMPLES_H
#define AVP33x_EXAMPLES_H


#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
      PLL���ƼĴ���(PLLCR)�ͷ�Ƶϵ��(DIVSEL)ֵ�궨��
-----------------------------------------------------------------------------*/
//#define DSP28_DIVSEL  0       // 4��Ƶ
//#define DSP28_DIVSEL  1       // 4��Ƶ
#define DSP28_DIVSEL    2       // 2��Ƶ
//#define DSP28_DIVSEL  3       // 1��Ƶ

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
//#define DSP28_PLLCR    0      // PLLΪ��·
//----------------------------------------------------------------------------


// The system clock output frequency.
#define SYSCTRL_SYSCLKOUT_FREQ      (150000000L)
#define SYSCLKOUT_MHZ               (150)
#define TBCLK_MHZ                   (SYSCLKOUT_MHZ/2)
#define DBTIME_US                   3
#define DBTIME_2US                  2


//Timer 0 HeartBeat
#define    SW_T0_FREQ_KHZ           1



// The high speed peripheral clock prescaler register value. Div by 2 (default).
// NOTE: If register value is changed to 0, SYSCTRL_HSCLK MUST be changed due to
// divide by zero.
#define SYSCTRL_HSCLK_PRESCALER (unsigned int)(0x0001)

// The high speed peripheral clock frequency.   ---150/2=75MHz
#define SYSCTRL_HSCLK           (unsigned long)(SYSCTRL_SYSCLKOUT_FREQ / (SYSCTRL_HSCLK_PRESCALER * 2))


// The low speed peripheral clock prescaler register value. Div by 4 (default).
// NOTE: If register value is changed to 0, SYSCTRL_LSCLK MUST be changed due to
// divide by zero.
#define SYSCTRL_LSCLK_PRESCALER (unsigned int)(0x0002)

// The low speed peripheral clock frequency.---150/4=37.5MHz
#define SYSCTRL_LSCLK           (unsigned long)(SYSCTRL_SYSCLKOUT_FREQ /(SYSCTRL_LSCLK_PRESCALER * 2))




/*-----------------------------------------------------------------------------
��Ƶʱ�����ں궨�壬��λ��ns��
        Ҫע�����������õ�ʱ��Ƶ�ʺͱ�Ƶϵ����
        ��ʹ���������ṩ��ֵ֮һ�������Լ������ֵ��
        β���L�Ǳ���ģ������߱�������������Ϊ64λֵ��
        ֻ����һ���궨�岻��ע�͡�

   eg1:(��Ƶ150MHz)
       CLKINΪ30MHz����
                     ������Ĳ���������PLLCR = 0xA�ɵõ�150MHz����Ƶʱ�ӣ���SYSCLKOUT = 150MHz����
                     ������ĺ궨��ѡ��ʱ��Ӧѡ��6.667L����ȥ��#define CPU_RATE  6.667L���궨���ע�ͣ���

   eg2:(��Ƶ100MHz)
       CLKINΪ20MHz����
                     ������Ĳ���������PLLCR = 0xA�ɵõ�100MHz����Ƶʱ�ӣ���SYSCLKOUT = 100MHz����
                     ������ĺ궨��ѡ��ʱ��Ӧѡ��10.000L����ȥ��#define CPU_RATE  10.000L���궨���ע�ͣ���

-----------------------------------------------------------------------------*/
#define CPU_RATE    16.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
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
        оƬ������CPU��Ƶ���û�����Ҫ���������κθı䡣
-----------------------------------------------------------------------------*/
#if DSP28_28332                   // DSP28_28332 device only
  #define CPU_FRQ_120MHZ    1
  #define CPU_FRQ_150MHZ    0
#else
  #define CPU_FRQ_120MHZ    0
  #define CPU_FRQ_150MHZ    1
#endif


//---------------------------------------------------------------------------
// ��������ͷ�ļ�:
//

#include "AVP33x_GlobalPrototypes.h"      // .c�ļ��е�ȫ�ֺ���ԭ��

#include "AVP33x_EPwm_defines.h"          // PWM�������õ��ĺ궨��ͷ�ļ�
#include "AVP33x_Dma_defines.h"           // DMA�������õ��ĺ궨��ͷ�ļ�
#include "AVP33x_I2c_defines.h"           // I2C�������õ��ĺ궨��ͷ�ļ�

#define PARTNO_28335  0xEF
#define PARTNO_28334  0xEE
#define PARTNO_28332  0xED
#define PARTNO_28235  0xE8
#define PARTNO_28234  0xE7
#define PARTNO_28232  0xE6


// ��������DSP/BIOSһ��ʹ�õ�ͷ�ļ�
#ifndef DSP28_BIOS
#include "AVP33x_DefaultIsr.h"
#endif


// �����޸��������
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_EXAMPLES_H definition


//===========================================================================
// End of file.
//===========================================================================
