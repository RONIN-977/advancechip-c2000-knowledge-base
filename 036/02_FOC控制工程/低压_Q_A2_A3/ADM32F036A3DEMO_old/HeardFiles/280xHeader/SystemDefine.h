/************************************************************
 28035版本定义系统中使用的宏和常量
            
************************************************************/
#ifndef MAIN_DEFINE_28035_H
#define MAIN_DEFINE_28035_H

#ifdef __cplusplus
extern "C" {
#endif
#include "DSP2803x_Device.h"



/************************************************************
*************************************************************
	28035版本，性能软件版本号的维护
***********************************************************/
//#define SOFT_VERSION		99		// 28035ver
// 0.52 update on xxxx
// 0.54 update on 2010、4、26 (自测第二版本)
// 0.56 update on 2010、5、17 (380驱动部分提交测试版本)
// 0.58 update on 2010, 6, 21 (380驱动第一轮测试完修改后的版本)
// 0.60 update on 2010, 6, 30 (第一轮验证程序2)
// 0.66 update on 2010、8、25 (中试之后)
// 0.68 sizeD验证

// 0.69 update 2010、11、15 加入SVC 优化程序
// 0.70 update 2010、12、22 修改VF振荡抑制
                         //修改了测试问题


// 非标记录
// 60001非标，将55kw/220V 改制机型对应到132kw/380V (通用是110kw/380V)
// 60002非标，将55kw/220V 改制机型对应到132kw/380V (通用是110kw/380V)
// 60003非标，将3.7kw/220V改制机型对应到7.5kw/380V (通用是5.5kw/380V) 2010/11/11
        

/************************************************************
************************************************************/

// // 定义DSP芯片和时钟
//#define    TMS320F2801		1		                            //2801芯片
//#define    TMS320F2802		2		                            //2802芯片
//#define      TMS320F2808		8		                            //2808芯片
//#define      TMS320F28035   3                               //28035芯片

#define    	 DSP_CLOCK100			100		                        //100MHz时钟
//#define      DSP_CLOCK80        80                                  // 80MHz
//#define    DSP_CLOCK60			60		                     //60MHz时钟

#ifdef		 DSP_CLOCK100
	#define    	DSP_CLOCK			100		                //100MHz时钟
	#define		PWM_CLK_DIV			0
	#define     PWM_CLOCK           100                     //PWM模块时钟周期 
	#define     DBTIME_1140V         50                      //1140V死区时间
    #define     DCTIME_1140V         14                      //1140V死区补偿
#endif
#ifdef      DSP_CLOCK80
	#define    	DSP_CLOCK			80		                //80MHz时钟
	#define		PWM_CLK_DIV			0
	#define     PWM_CLOCK           80                      //PWM模块时钟周期   
#endif
#ifdef      DSP_CLOCK60
	#define    	DSP_CLOCK			60		                //60MHz时钟
	#define		PWM_CLK_DIV			0
	#define     PWM_CLOCK           60                      //PWM模块时钟周期   
    #define     DBTIME_1140V         30                     //1140V死区时间
    #define     DCTIME_1140V         8                     //1140V死区补偿
#endif

#define     PWM_CLOCK_HALF      (PWM_CLOCK/2)               //PWM时钟除以2
#define  	C_TIME_05MS         (DSP_CLOCK*500L)	        //0.5ms对应的定时器1计数值
#define  	C_TIME_20MS         (DSP_CLOCK*2000L)
#define     C_TIME_045MS        (DSP_CLOCK*450L)            //0.45ms对应的定时器1计数值

#define  READ_RAND_FLASH_WAITE		3		                //Flash中运行的等待时间
#define  READ_PAGE_FLASH_WAITE		3		                //
#define  READ_OTP_WAITE				8		                //OTP中读数据的等待时间

/************************************************************
与芯片相关的寄存器定义
************************************************************/

// // 与芯片相关的寄存器定义
#define PIE_VECTTABLE_ADCINT    PieVectTable.ADCINT1                    //ADC中断向量
#define ADC_CLEAR_INT_FLAG      AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1    //清除ADC模块的中断标志
#define ADC_RESET_SEQUENCE      AdcRegs.SOCPRICTL.bit.RRPOINTER = 0x20  //reset the sequence
#define ADC_START_CONVERSION   	AdcRegs.ADCSOCFRC1.all = 0xFFFF 		//软件启动AD
#define ADC_END_CONVERSIN       AdcRegs.ADCINTFLG.bit.ADCINT1           //AD转换完成标志位

#define  ADC_VOLTAGE_08         ( 8L*65535/33)  // AD输入0.8V对应的采样值   28035AD范围是0-3.3V
#define  ADC_VOLTAGE_10         (10L*65535/33)  // AD输入1.0V对应的采样值
#define  ADC_VOLTAGE_20         (20L*65535/33)  // AD输入2.0V对应的采样值
#define  ADC_VOLTAGE_25         (25L*65535/33)  // AD输入2.5V对应的采样值

#define PL_INPUT_HIGH           (PL_VOE_PROTECT < ADC_VOLTAGE_08)       //输入缺相信号电平判断

#define DisConnectRelay()   	GpioDataRegs.GPACLEAR.bit.GPIO19   = 1
#define ConnectRelay()    	    GpioDataRegs.GPASET.bit.GPIO19 = 1    //上电缓冲继电器控制
/*
#define EnableDrive()    	    				\
	EALLOW;										\
	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;			\
	GpioDataRegs.GPASET.bit.GPIO30  = 1;
	EDIS;

#define DisableDrive()      	                \
	EALLOW;										\
	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;			\
	GpioDataRegs.GPACLEAR.bit.GPIO30  = 1;
	EDIS;
*/

#define GetOverUdcFlag()      GpioDataRegs.AIODAT.bit.AIO2 == 0       // 28035通过io口做硬件过压

// //旋转变压器用到的宏定义
#define RT_SAMPLE_START         (GpioDataRegs.AIOCLEAR.bit.AIO10 = 1)      // 旋变信号开始采样
#define RT_SAMPLE_END           (GpioDataRegs.AIOSET.bit.AIO10 = 1)        // 旋变信号采样完成

#define ROTOR_TRANS_RDVEL	    GpioDataRegs.GPADAT.bit.GPIO20
#define ROTOR_TRANS_SO		    GpioDataRegs.GPADAT.bit.GPIO21
#define ROTOR_TRANS_RD	        GpioDataRegs.GPADAT.bit.GPIO23
#define ROTOR_TRANS_SCLK	    GpioDataRegs.AIODAT.bit.AIO12

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif  // end of definition

