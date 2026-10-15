/******************************************************************************
文件名                     : AVP32F08_Adc.c
作者                         : 湖南进芯电子科技有限公司
功能描述                 : ADC初始化及相关函数
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP32F08_Device.h"      // AVP32F08 Headerfile Include File
#include "AVP32F08_Examples.h"    // AVP32F08 Examples Include File

#define ADC_usDELAY  5000L

//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// 初始化ADC控制寄存器
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count);

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;   // 使能ADC时钟
    
    // ADC校准(由于产品批次问题，此处需用以下两条代码替代原来的ADC_cal()函数)
	AdcRegs.ADCREFSEL.all = *(Uint16 *)0x380083;  // load gain
	AdcRegs.ADCOFFTRIM.all = *(Uint16 *)0x380085; // load offset
    EDIS;

    AdcRegs.ADCTRL1.bit.LDO_EN = 1;     	// ADC模块LDO上电

    // ADC上电前，需要使能ADC时钟，然后给带隙上电、参考电路上电、ADC内核上电。
    // 配置上电后，至少需要延时5ms才能进行正常的AD采样和转换，以预留足够的时间给模拟电路上电和基准的建立
    // 为确保DELAY_US()函数的延时精确性，需要配置正确的CPU_RATE（在AVP32F08_Examples.h文件中设置）
    AdcRegs.ADCTRL3.all = 0x00E0;  			// 给带隙、参考电路上电
    DELAY_US(ADC_usDELAY);         			// 延时
}

//===========================================================================
// End of file.
//===========================================================================
