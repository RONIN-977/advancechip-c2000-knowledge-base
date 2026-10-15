#include "ADP32F03x_Project.h"
#include "BspInc.h"

//ADC

//AdcRegs.ADCSOCFRC1.all = 0X000F; //软件触发SOC0--SOC3采样

float ad_ave = 0.0;

interrupt void adc_isr(void);

//AD初始化
void ADC_Config(void)
{
        EALLOW;
        SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
        (*Device_cal)();
        EDIS;

        // asm(" RPT #100 || NOP");              // 延时1us, 可以删除

        EALLOW;
        AnalogRegs.ANEN.bit.OPAEN = 0;     // 内部运放配置
        EDIS;

        EALLOW;
        AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power  ADC BG
        AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power  reference
        AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power  ADC
        AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
        EDIS;

        ADP32F03x_usDelay(20000L);                 // 20ms

        //----------- 以下开始设置ADC的控制寄存器、转换通道选择寄存器等 ------------------
        EALLOW;
        AdcRegs.ADCSAMPLEMODE.all = 0;           // 采样模式为0: 顺序采样(非同时采样)
        AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;     // ADCINT1 trips after AdcResults latch
        AdcRegs.ADCCTL1.bit.TEMPCONV    = 1;

        AdcRegs.ADCCTL2.bit.CLKDIV2EN = 1;
        AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;

    //#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
        // SOCx Trigger Select
        // 036 -- 低压Demo 板
        // B1 -- VDC
        // B3 -- IDC
        // A3 -- ADC VSP
        AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = 0;
        AdcRegs.ADCSOC1CTL.bit.TRIGSEL  = 0;//ADCTRIG_EPWM4_SOCB;
        AdcRegs.ADCSOC2CTL.bit.TRIGSEL  = 0;//ADC_TRIG_SOURCE;
        AdcRegs.ADCSOC3CTL.bit.TRIGSEL  = 0;//ADC_TRIG_SOURCE;

        //Acquisition Pulse Size
        AdcRegs.ADCSOC0CTL.bit.ACQPS  = 8;
        AdcRegs.ADCSOC1CTL.bit.ACQPS  = 8;
        AdcRegs.ADCSOC2CTL.bit.ACQPS  = 8;
        AdcRegs.ADCSOC3CTL.bit.ACQPS  = 8;


        AdcRegs.ADCSOC0CTL.bit.CHSEL  = 9;//9;//2;
        AdcRegs.ADCSOC1CTL.bit.CHSEL  = 9;//9;//2;
        AdcRegs.ADCSOC2CTL.bit.CHSEL  = 6;//6;//12;//ADC_PIN_S12;//13;//ADC_PIN_S0;


        AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

        EDIS;
}

//AD 中断处理函数
//interrupt void  adc_isr(void)
//{
//	EALLOW;
//	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;   // Acknowledge interrupt to PIE
//	EDIS;
//
//	AdcRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;		//Clear ADCINT1 flag reinitialize for next SOC
//
//	ad_ave = (float)((AdcResult.ADCRESULT0+AdcResult.ADCRESULT1+AdcResult.ADCRESULT2+AdcResult.ADCRESULT3)*3.0/4095.0/4.0);
//
//	//串口打印AD的数据，
//	//中断里不建议用printf函数，该例程只作为演示使用
//	printf("\r\nADP32F036片上OPA试验例程-->OPA放大后的电压值 = :%0.3fV(平均值)",ad_ave);
//

//}
