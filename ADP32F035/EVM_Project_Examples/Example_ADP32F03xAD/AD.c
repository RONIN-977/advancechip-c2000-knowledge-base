#include "ADP32F03x_Project.h"
#include "BspInc.h"

//ADC

//AdcRegs.ADCSOCFRC1.all = 0X000F; //软件触发SOC0--SOC3采样

float ad_ave = 0.0;

interrupt void adc_isr(void);

//AD初始化
void ADC_Config(void)
{

   InitAdc();  // 先调用官方AD默认的配置函数初始化（必须）

   EALLOW;
   PieVectTable.ADCINT3 = &adc_isr;// AD中断函数映射
   EDIS;

   PieCtrlRegs.PIEIER10.bit.INTx3 = 1;	// Enable INT 10.3 in the PIE
   IER |= M_INT10; 						// Enable CPU Interrupt 10

    // Configure ADC
	EALLOW;
    //	AdcRegs.ADCCTL2.bit.CLKDIV2EN = 0; //ADCclock = CPUclock
	AdcRegs.ADCCTL1.bit.ADCREFSEL	= 0;    //REF Int
	AdcRegs.ADCCTL1.bit.INTPULSEPOS	= 1;	//ADCINT1 trips after AdcResults latch //--> P39 HIKE,EOCx产生来自源选择
	AdcRegs.INTSEL3N4.bit.INT3E     = 1;	//Enabled ADCINT3
	AdcRegs.INTSEL3N4.bit.INT3CONT  = 0;	//Disable ADCINT3 Continuous mode，single conversion mode

	//HIKE,ADCINTx EOC Source Select, 即哪个 EOCx 触发中断
	AdcRegs.INTSEL3N4.bit.INT3SEL	= 3;	//setup EOC3 to trigger ADCINT1 to fire , EOC3 is trigger for ADCINTx


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC勘误表上指出，SOC通道第一个采样的数据不正确，从第二个开始为正确的数据，故这里SOC0 SOC1数据不做处理

//	//P17 P35 同步采样的话，两个SOC的CHSEL设置一样
//	//Simultaneous sampling enable for SOC2/SOC3
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0=1;	//SOC0/1 Simultaneous Sampling Enable	,SOC0蚐OC1组合工作
//	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 1;	//SOC0 (ADCINA1/ADCINB1 pair)，配置模拟通道连接
//	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 1;	//SOC1 (ADCINA1/ADCINB1 pair)，配置模拟通道连接
//
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN2=1;	//SOC2/3 Simultaneous Sampling Enable	,SOC2蚐OC3组合工作
//	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 1;	//SOC2 (ADCINA1/ADCINB1 pair)，配置模拟通道连接
//	AdcRegs.ADCSOC3CTL.bit.CHSEL 	= 1;	//SOC3 (ADCINA1/ADCINB1 pair)，配置模拟通道连接


//SOC 连续采样，赋值与通道连接如下
//0-7为A0到A7
//8-16为B0到B7
	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 15;	//set SOC0 channel select to ADCINB7
	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 15;	//set SOC1 channel select to ADCINB7
	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 15;	//set SOC2 channel select to ADCINB7
	AdcRegs.ADCSOC3CTL.bit.CHSEL 	= 15;	//set SOC3 channel select to ADCINB7

//-->HIKE ,AD选择触发源,软件触发 P34
	AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 0x00;
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL 	= 0x00; //ADCTRIG0- Software only
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL 	= 0x00;
	AdcRegs.ADCSOC3CTL.bit.TRIGSEL 	= 0x00;

	AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;
	AdcRegs.ADCSOC1CTL.bit.ACQPS 	= 6;	//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC2CTL.bit.ACQPS 	= 6;
	AdcRegs.ADCSOC3CTL.bit.ACQPS 	= 6;

	EDIS;
}

//AD 中断处理函数
interrupt void  adc_isr(void)
{
	EALLOW;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;   // Acknowledge interrupt to PIE
	EDIS;

	AdcRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;		//Clear ADCINT1 flag reinitialize for next SOC

	ad_ave = (float)((AdcResult.ADCRESULT0+AdcResult.ADCRESULT1+AdcResult.ADCRESULT2+AdcResult.ADCRESULT3)*3.3/4095.0/4.0);
	//串口打印AD的数据，
	//中断里不建议用printf函数，该例程只作为演示使用
	printf("\r\nADP32F035片上AD试验例程-->INB7 ADC Result is:%0.3fV(平均值)",ad_ave);

	Running.showNum = ad_ave;//初始化数码管显示内容
}
