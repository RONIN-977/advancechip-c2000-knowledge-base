//#############################################################################
/*<FH>*************************************************************************
* Copyright (c) :  2021-2031, Advancechip Co., Ltd. All Rights Reserved
* Original Author: pei.shen

* FileName: MainInclude.h
* Current Version: V1.0
* Date: 21-Aug-2021

* ABSTRACT:  the main include
* DESCRIPTION:

* Revise:
;   Last rev by:    ***
;   Last rev date:  ***
;   Last rev desc.: ***
**<FH>************************************************************************/

#ifndef MAIN_INCLUDE_H
#define MAIN_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include

#include "ADP32F03x_Project.h"
#include "RemoteModbus.h"

#include "string.h"
//#include "RemoteModbus.h"


#define Gpio_EnableRS485RD()     GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1;
#define Gpio_EnableRS485SD()     GpioDataRegs.GPBSET.bit.GPIO49= 1;


#define Jump_Boot        (void  (*)(void)) 0x3F7FF6    //0x3F7FF6是Boot程序的入口地址



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



#define ARR_SIZE(a)     (sizeof(a)/sizeof(a[0]))
typedef int             BOOL;
#define TRUE            1
#define FALSE           0


#define EnableDrive()                           \
                                                \
    EALLOW;                                     \
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;         \
    EDIS;                                       \

#define DisableDrive()                          \
                                                \
    EALLOW;                                     \
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;         \
    EDIS;                                       \
//-----------------------------------------------------------------------------
// Constants and Macro Definitions

#define PIE_VECTTABLE_ADCINT    PieVectTable.ADCINT1                    //ADC中断向量
#define ADC_CLEAR_INT_FLAG      AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1      //Clear INT SEQ1 bit//AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1    //清除ADC模块的中断标志
#define ADC_RESET_SEQUENCE      AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1        //AdcRegs.SOCPRICTL.bit.RRPOINTER = 0x20  //reset the sequence
#define ADC_START_CONVERSION    AdcRegs.ADCSOCFRC1.all = 0xFFFF         //软件启动AD
#define ADC_END_CONVERSIN       AdcRegs.ADCINTFLG.bit.ADCINT1           //AD转换完成标志位

//#define External_OSC          0
#define External_10M_OSC      1     // 1-- 10M   0 -- 20M

#define DSP_CLOCK60   1
//#define DSP_CLOCK150  1


#ifdef      DSP_CLOCK150
    #define     DSP_CLOCK           150     //150MHz时钟
    #define     PWM_CLK_DIV         1
#elif       DSP_CLOCK60
    #define     DSP_CLOCK           60      //60MHz时钟
    #define     PWM_CLK_DIV         0
#else
    #define     DSP_CLOCK           40      //40MHz时钟
    #define     PWM_CLK_DIV         0
#endif

#define     C_TIME_05MS     (DSP_CLOCK*500L)    //0.5ms对应的定时器1计数值


/************************************************************
    基本函数定义和引用
************************************************************/
#define  GetTime()  (CpuTimer1.RegsAddr->TIM.all)

/************************************************************
    引用函数说明
************************************************************/
extern void InitDevice(void);
extern void InitAppData(void);
extern void SysCtrl_Initialize(void);
extern void Flash_Initialize(void);
extern void PieCtrl_Initialize(void);
extern void PieVectTable_Initialize(void);
extern void Interrupt_Initialize(void);
extern void Peripherals_Initialize(void);
extern void SetInterruptEnable(void);
extern void EnableDog(void);
extern void WDogDisable(void);
extern void WDogService(void);
extern void InitPll_Initialize(Uint16 val, Uint16 divsel);
extern void PeripheralClocks_Initialize(void);

//peripherals
extern void Gpio_Initialize(void);

extern void Main2msFunction(void);
extern void Main05msFunction(void);
extern void Main0msFunction(void);


//以下是中断程序说明
extern interrupt void ADC_Over_isr(void);
extern interrupt void SCI_RXD_isr(void);
extern interrupt void SCI_TXD_isr(void);
extern interrupt void EPWM1_zero_isr(void);
extern interrupt void BSP_timer0ISR(void);

extern interrupt void LINa_LEVEL_isr(void);
extern void Lin_Initialize(void);

extern void CommsProtocalLayerTask2ms(void);
//extern void Sci_Initialize(void);

//#define Modbus_Rx_Enable        GpioDataRegs.GPADAT.bit.GPIO27 = 0
//#define Modbus_Tx_Enable        GpioDataRegs.GPADAT.bit.GPIO27 = 1

typedef struct CPU_TIME_STRUCT_DEF {
    Uint32    Function2MsBase;
    Uint16    Function2Ms;        //功能部分2摸索执行时间
    Uint16    flash_IAP_flag;
    Uint16    IAP_Delay;
}CPU_TIME_STRUCT;   //统计模块执行时间的数据结构


extern CPU_TIME_STRUCT  gCpuTime;

/************************************************************
	引用结构说明
************************************************************/




#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================
