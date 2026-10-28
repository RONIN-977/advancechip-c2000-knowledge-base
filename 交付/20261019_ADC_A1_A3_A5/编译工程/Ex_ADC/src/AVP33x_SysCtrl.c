/******************************************************************************
文件名                     : AVP33x_SysCtrl.c
作者                         : 湖南进芯电子科技有限公司
功能描述                 : 系统初始化
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // Headerfile Include File
#include "AVP33x_Examples.h"   // Examples Include File

// 将flash初始化代码搬至RAM运行
#pragma CODE_SECTION(InitFlash, "ramfuncs");
// 将PLL初始化代码搬至RAM运行
#pragma CODE_SECTION(InitPll, "ramfuncs");

//---------------------------------------------------------------------------
// InitSysCtrl:
//---------------------------------------------------------------------------
// 系统初始化函数。
// - 禁止看门狗
// - 设置PLL
// - 设置高速外设始终和低速外设始终
// - 使能外设时钟
void InitSysCtrl(void)
{
    // 禁止看门狗
    DisableDog();

    // 初始化PLL
    InitPll(DSP28_PLLCR,DSP28_DIVSEL);  // 外部时钟30MHz，30MHz*10/2 = 150MHz

    // 初始化外设时钟
    InitPeripheralClocks();
}

//---------------------------------------------------------------------------
// InitFlash:
//---------------------------------------------------------------------------
// flash初始化函数：将flash的寄存器设置为一个固定的状态。
// 注意！本函数必须在RAM中运行，否则会出现意料不到的结果。
void InitFlash(void)
{
    EALLOW;

    // 使能flash的管道模式，可改善代码在flash中运行的效果。
    FlashRegs.FOPT.bit.ENPIPE = 1;

    // 设置flash运行的最小等待状态，请参考最新的数据手册。
#if CPU_FRQ_150MHZ

    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5;   // 设置页等待
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 5;   // 设置随机访问等待
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 8;     // 设置OTP等待

#endif

#if CPU_FRQ_120MHZ

    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;   // 设置页等待
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;   // 设置随机访问等待
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;     // 设置OTP等待

#endif

    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;     // 默认设置，请勿更改
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;   // 默认设置，请勿更改

    EDIS;

    asm(" RPT #7 || NOP");                  // 等待，确保上面配置参数写成功
}

//---------------------------------------------------------------------------
// ServiceDog:
//---------------------------------------------------------------------------
// 看门狗喂狗函数。
void ServiceDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//---------------------------------------------------------------------------
// DisableDog:
//---------------------------------------------------------------------------
// 禁止看门狗函数。
void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

//---------------------------------------------------------------------------
// InitPll:
//---------------------------------------------------------------------------
// 初始化PLL函数
void InitPll(Uint16 val, Uint16 divsel)
{

//    EALLOW;
//    SysCtrlRegs.PLLSTS.all |= 0x0050;
//    EDIS;
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
        asm("        ESTOP0");
    }

    // 重新设置PLLCR寄存器之前要确保DIVSEL位为0
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }

    // 配置PLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != val)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        DisableDog();

        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
        {
            // 如果看门狗被使能，则要打开下面这条喂狗语句
            // ServiceDog();
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        EDIS;
    }

    // 设置分频系数为1或2
    if((divsel == 1)||(divsel == 2))
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
        EDIS;
    }

    // 设置分频系数为3。需要先设置为2分频，然后等待电压建立，最后才能设置为3分频。
    if(divsel == 3)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
        DELAY_US(50L);
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
        EDIS;
    }
}

//--------------------------------------------------------------------------
// InitPeripheralClocks:
//---------------------------------------------------------------------------
// 初始化外设时钟函数。
// 首先，设置高速和低速外设时钟的预分频系数；
// 然后，使能要用到的外设时钟。（为了降低功耗，没用到的外设建议将其外设时钟关掉）
// 需要注意的是：如果哪个外设时钟未使能，则相应的外设寄存器是无法读写的。
void InitPeripheralClocks(void)
{
    EALLOW;

    // 高/低速外设时钟预分频系数设置，一般建议设为如下默认值
    SysCtrlRegs.HISPCP.all = 0x0001;
    SysCtrlRegs.LOSPCP.all = 0x0002;

    // 设置XINTF总线时钟
    XintfRegs.XINTCNF2.bit.XTIMCLK = 1;         // 总线时钟，XTIMCLK = SYSCLKOUT/2
    XintfRegs.XINTCNF2.bit.CLKMODE = 1;         // 时钟输出，XCLKOUT = XTIMCLK/2
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;          // 禁止总线时钟输出


    // This function is not written to be an example of efficient code.

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;       // ADC

	// ADC校准(由于产品批次问题，此处需用以下两条代码替代原来的ADC_cal()函数)
	AdcRegs.ADCREFSEL.all = *(Uint16 *)0x380083;  // load gain
	AdcRegs.ADCOFFTRIM.all = *(Uint16 *)0x380085; // load offset


    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;      // I2C
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;      // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;      // SCI-B
    SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;      // SCI-C
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;      // SPI-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 1;    // McBSP-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 1;    // McBSP-B
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;       // eCAN-A
    SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=1;       // eCAN-B

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;     // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;     // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;     // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;     // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;     // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1;     // ePWM6
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;      // Enable TBCLK within the ePWM

    SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 1;     // eCAP3
    SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 1;     // eCAP4
    SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 1;     // eCAP5
    SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 1;     // eCAP6
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;     // eCAP1
    SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;     // eCAP2
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;     // eQEP1
    SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1;     // eQEP2

    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2

    SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;       // DMA Clock
    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;     // XTIMCLK
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO input clock

    EDIS;
}

//---------------------------------------------------------------------------
// CsmUnlock:
//---------------------------------------------------------------------------
// 密码解锁函数。用户必须将所设置的密码替换下列的0xFFFF。 解锁成功返回1.
#define STATUS_FAIL          0
#define STATUS_SUCCESS       1

Uint16 CsmUnlock()
{
    volatile Uint16 temp;

    // 用户将自荷柚玫拿苈胩钊胂铝屑拇嫫髦�
    EALLOW;
    CsmRegs.KEY0 = 0xFFFF;
    CsmRegs.KEY1 = 0xFFFF;
    CsmRegs.KEY2 = 0xFFFF;
    CsmRegs.KEY3 = 0xFFFF;
    CsmRegs.KEY4 = 0xFFFF;
    CsmRegs.KEY5 = 0xFFFF;
    CsmRegs.KEY6 = 0xFFFF;
    CsmRegs.KEY7 = 0xFFFF;
    EDIS;

    // 对键入能码进行虚拟读取，如果匹配则解锁成功
    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;

    if (CsmRegs.CSMSCR.bit.SECURE == 0)
        return STATUS_SUCCESS;
    else
        return STATUS_FAIL;
}

//===========================================================================
// End of file.
//===========================================================================
