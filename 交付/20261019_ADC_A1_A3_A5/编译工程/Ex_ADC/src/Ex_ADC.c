/******************************************************************************
文件名                     : Example_AVP33xAdcSeqModeTest.c
作者                         : 湖南进芯电子科技有限公司
功能描述                 : ADC排序模式测试例程：
                                       本例实现的功能是：将通道A1、A3、A5设置为连续顺序采样模式，分别保存采样结果。
                                       分别在ADCINA1、ADCINA3、ADCINA5输入待测模拟电压。
             SampleTableA1、SampleTableA3、SampleTableA5 - 各通道AD转换结果。
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Project.h"     // Device Headerfile and Examples Include File

// ADC模块参数宏定义
#if (CPU_FRQ_150MHZ)            // Default - 150 MHz SYSCLKOUT
    #define ADC_MODCLK 0x3      // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_120MHZ)
    #define ADC_MODCLK 0x2      // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 120/(2*2)   = 30.0 MHz
#endif
#define ADC_CKPS   0x1          // ADC模块时钟  = HSPCLK/2 = 30MHz/2 = 15.0 MHz
#define ADC_SHCLK  0xf          // 采样保持窗口宽度 = 16个ADC时钟周期
#define AVG        1000         // 每通道保存点数
#define ZOFFSET    0x00         // 平均零偏
#define BUF_SIZE   1000         // 采样数组大小

void scia_echoback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);
void PowerUpReset(void);

Uint16 LoopCount;
Uint16 ErrorCount;
Uint16 SampleTableA1[BUF_SIZE];  // ADCINA1采样结果
Uint16 SampleTableA3[BUF_SIZE];  // ADCINA3采样结果
Uint16 SampleTableA5[BUF_SIZE];  // ADCINA5采样结果

main()
{
//    PowerUpReset();
    Uint16 i;
    Uint16 ReceivedChar;
    char *msg;

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。


// 步骤1：系统初始化
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();

    // 为本例程设置时钟
    EALLOW;
    SysCtrlRegs.HISPCP.all = ADC_MODCLK;				// HSPCLK = SYSCLKOUT/ADC_MODCLK
    EDIS;


// 步骤2：初始化GPIO
    // InitGpio();          							// Skipped for this example


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   							// 禁止CPU中断
    InitPieCtrl();          							// 初始化PIE
    IER = 0x0000;           							// 禁止CPU中断
    IFR = 0x0000;           							// 清除CPU中断标志
    InitPieVectTable();     							// 初始化PIE中断向量表


// 步骤4：初始化所有外设
    // InitPeripherals();   							// Not required for this example
    InitAdc();             	 							// 为本例程初始化ADC模块

    // 为本例程制定ADC模块配置
    AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK; 			// 设置采样窗口宽度
    AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;			// 设置ADC时钟
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;       			// 0：双序列模式；1：级联模式
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;                // 顺序采样
    AdcRegs.ADCMAXCONV.all = 2;             			// 3个通道采样，转换次数减1
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x01; 			// 第1路ADCINA1
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x03;           // 第2路ADCINA3
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x05;           // 第3路ADCINA5
    AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       			// 0：启停采样模式；1：连续采样模式


// 步骤5：用户自己的代码
    for (i=0; i<BUF_SIZE; i++)              			// 三通道数组初始化清零
    {
        SampleTableA1[i] = 0;
        SampleTableA3[i] = 0;
        SampleTableA5[i] = 0;
    }

    AdcRegs.ADCTRL2.all = 0x2000;           			// 启动排序器SEQ1

    for(;;)                                 			// 将AD结果分别保存到三通道数组
    {
        for (i=0; i<AVG; i++)
        {
            while (AdcRegs.ADCST.bit.INT_SEQ1== 0) {}   // 通过查询ADC中断标志来等待ADC转换完成
            AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;         // 清除AD中断标志
            SampleTableA1[i] = (AdcRegs.ADCRESULT0 >> 4); // ADCINA1
            SampleTableA3[i] = (AdcRegs.ADCRESULT1 >> 4); // ADCINA3
            SampleTableA5[i] = (AdcRegs.ADCRESULT2 >> 4); // ADCINA5
        }
    }
}
void PowerUpReset(void)
{
    asm(" RPT #0x1FFF || NOP ");
    Uint16 Temp_WD = 0;
    if((SysCtrlRegs.WDCR & 0x80) == 0)
    {
        EALLOW;
        Temp_WD = SysCtrlRegs.SCSR;
        SysCtrlRegs.SCSR = (Temp_WD & 0xFE);
        SysCtrlRegs.WDCR = 0x00B8;
        EDIS;
        while(1);
    }
    else
    {
        EALLOW;
        SysCtrlRegs.WDCR = 0x00E8;
        EDIS;
    }
}


//===========================================================================
// No more.
//===========================================================================
