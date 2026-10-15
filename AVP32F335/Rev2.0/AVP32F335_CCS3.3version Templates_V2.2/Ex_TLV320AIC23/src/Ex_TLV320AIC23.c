/******************************************************************************
文件名			: Example_AVP33xMcBSP_DLB.c
作者        	: 湖南进芯电子科技有限公司
功能描述       	: McBSP例程
                  本例程实现的功能：首先通过SPI对音频芯片TLV320AC23进行配置实现
				  音频传输功能，然后通过Mcbspa将音频信号传输给音频芯片输出音频
				  信号。
创建时间       	: 06-25-2019
版本           	: V1.0
修订时间       	:
修订内容       	:
*******************************************************************************/

#include "AVP33x_Project.h"     // Device Headerfile and Examples Include File


const Uint16 LedCode[] = {0X7E7E,0X2929,0X2C2C,0X6666,0XA4A4,0XA0A0,0X3E3E,0X2020,0X2424};

unsigned int count;

int ADbuf[500];
int DA_wptr,DA_rptr,y;
int play_mode;

int temp1,temp2;

void delay(unsigned int);
void ldelay(unsigned int);
void WriteLED(unsigned int);
Uint16 Spi_TxReady(void);
Uint16 Spi_RxReady(void);
void InitSpi1(void);

void main(void)
{
    unsigned int temp,i;

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

// 步骤1：系统初始化。初始化PLL、看门狗、使能外设时钟等
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


// 步骤2：初始化GPIO
    // InitGpio();          // Skipped for this example
    InitMcbspaGpio();
    InitSpiGpio();


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   // 禁止CPU中断
    InitPieCtrl();          // 初始化PIE
    IER = 0x0000;           // 禁止CPU中断
    IFR = 0x0000;           // 清除CPU中断标志
    InitPieVectTable();     // 初始化PIE中断向量表

    EALLOW;
    PieVectTable.MXINTA = &ISRMcbspX;
    EDIS;


// 步骤4：初始化所有外设
    // InitPeripherals();   // Not required for this example
    InitSpi();              //初始化SPI
    temp = 0;
    count=0;
    for (i=0;i<9;i++)
    {
        WriteLED(i);
        ldelay(50);
    }

    InitSpi1();
    InitAic23();        //初始化AIC23 chip
    ldelay(10);
    InitMcbspa();
    ldelay(10);


// 步骤5：用户自己的代码

    // 使能McBSP的CPU级中断
    IER |= M_INT6;
    // 使能McBSP的PIE级中断
    PieCtrlRegs.PIEIER6.bit.INTx6 =1;

    EINT;               // 使能全局中断INTM
    ERTM;               // 使能全局实时中断DBGM

    play_mode = 0;
    McbspaRegs.DXR1.all = 20000;

    while(1)
    {
        for(temp=30000;temp>0;temp-=100)
        {
            for(i=0;i<2;i++)
            {
                y=temp1;
                delay(temp);
                y=-temp2;
                delay(temp);
            }
        }
    }

}

void WriteLED(unsigned int index)
{
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG == 1);   // 如果SPI发送缓冲区有数据，则一直等待
    SpiaRegs.SPITXBUF = (unsigned int)( LedCode[index]);
    delay(50000);
    GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;
    delay(1000);
    GpioDataRegs.GPASET.bit.GPIO24 = 1;
}

void ldelay(unsigned int k)
{
    while(k>0)
    {
        k--;
        delay(50000);
    }
}

void delay(unsigned int t)
{
    while(t>0)
        t--;
}
//////////////////////////////////////////////

interrupt void  ISRMcbspR(void)
{
    PieCtrlRegs.PIEACK.all = 0xFFFF;
    PieCtrlRegs.PIEIFR6.bit.INTx5 = 0;
    ERTM;

    ADbuf[0] = McbspaRegs.DRR1.all;
}

interrupt void  ISRMcbspX(void)
{
    PieCtrlRegs.PIEACK.all = 0xFFFF;
    PieCtrlRegs.PIEIFR6.bit.INTx6 =0;

    if(play_mode==0)
        McbspaRegs.DXR1.all = y;             //警报声
    else
        McbspaRegs.DXR1.all = McbspaRegs.DRR1.all;    //放音

    IER |= M_INT6;
    EINT;
    ERTM;
}

Uint16 Spi_TxReady(void)
{
    unsigned int i;
    if(SpiaRegs.SPISTS.bit.BUFFULL_FLAG == 1)   // SPI发送缓冲区有数据
    {
        i = 0;
    }
    else
    {
        i = 1;
    }
    return(i);
}

Uint16 Spi_RxReady(void)
{
    unsigned int i;
    if(SpiaRegs.SPISTS.bit.INT_FLAG == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return(i);
}

void InitSpi1(void)
{
    // Initialize SPI-A:

    SpiaRegs.SPICCR.all = 0x4f;     // 进入初始状态，数据在上升沿输出，自测禁止，16位数据模式

    SpiaRegs.SPICTL.all =0x06;      // 主动模式，禁止中断

    SpiaRegs.SPIBRR = 0x3B;         // 波特率 = LSPCLK/（SPIBRR+1）= 30/60 = 0.5M

    SpiaRegs.SPICCR.all = 0xcf;     // //退出初始状态

//  PieCtrl.PIEIER6.bit.INTx1 = 1;
//  PieCtrl.PIEIER6.bit.INTx2 = 1;
    //tbd...
    //tbd...
}

//===========================================================================
// No more.
//===========================================================================
