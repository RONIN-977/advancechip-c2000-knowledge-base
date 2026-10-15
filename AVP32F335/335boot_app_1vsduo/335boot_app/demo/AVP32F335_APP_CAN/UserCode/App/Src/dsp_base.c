/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :dsp_base.c
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:时钟频率：外部30MHz晶振，系统时钟150MHz
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "dsp_base.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
*Input:
*Output:
-----------------------------------------------------------------------------*/
void DSP_Base_Init(void)
{
	//把需要在RAM运行的代码从FLASH复制到RAM区，该RAM区域是“ramfuncs”，
	//位于RAM_L0：0x008000，长度0x1000 word
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	
	//1.使能ADC校准
	//2.切换使用外部晶振
	//3.开启PLL，设置时钟倍频数，时钟分频数，使系统时钟达到10MHz*12/2=60MHz
	//4.设置低速外设时钟SYSCLKOUT/(XCLKOUTDIV*2)=15MHz
	//5.开启所需用到外设的时钟
	SysCtrl_Initialize();
	
	//设置FLASH的读等待周期，开启指令流水线模式
	Flash_Initialize();
	
	//禁止CPU中断
	DINT;
	DELAY_US(1000);
	
	//清空PIE寄存器
	PieCtrl_Initialize();
    IER = 0x0000;
    IFR = 0x0000;
	
	//初始化中断向量表
	PieVectTable_Initialize();
}

/******************************************************************************
*Description:系统底层初始化完成后，使能总中断
-----------------------------------------------------------------------------*/
void DSP_INT_Enable(void)
{
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    EINT;
    ERTM;
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void SysCtrl_Initialize(void)
{
    WDogDisable();           // Disable the watchdog

    // Initialize the PLL control: PLLCR and DIVSEL   (30M*10)/2=150M
    // DSP28_PLLCR and DSP28_DIVSEL are defined in DSP2833x_Examples.h

    InitPll_Initialize(DSP28_PLLCR,DSP28_DIVSEL);   // Change 30MHz clock to 150MHz

    PeripheralClocks_Initialize();     // Initialize the peripheral clocks
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void Flash_Initialize(void)
{
    EALLOW;

    // 使能flash的管道模式，可改善代码在flash中运行的效果。
    FlashRegs.FOPT.bit.ENPIPE = 1;
    DELAY_US(10);

    // 设置flash运行的最小等待状态，请参考Advanchip最新的数据手册。
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

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void PieCtrl_Initialize(void)
{
    Uint16 * m_Point1,* m_Point2;
    Uint16   m_Index;

    DINT;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;  // Disable the PIE
    // Clear all PIEIER registers:
    m_Point1 = (Uint16 *)&PieCtrlRegs.PIEIER1.all;
    m_Point2 = (Uint16 *)&PieCtrlRegs.PIEIFR1.all;
    for(m_Index = 0;m_Index<12;m_Index++)
    {
        *(m_Point1++) = 0;
        *(m_Point2++) = 0;
    }
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void PieVectTable_Initialize(void)
{
    int16   i;
    PINT *pPieTable = (void *) &PieVectTable;

    EALLOW;
    for(i=0; i < 128; i++)
    {
        *pPieTable++ = rsvd_ISR;
    }
    EDIS;

    // Enable the PIE Vector Table
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
        *DestAddr++ = *SourceAddr++;
    }
}

void WDogService(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

void WDogDisable(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

void EnableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x002A;
    EDIS;
}

void PeripheralClocks_Initialize(void)
{
   EALLOW;
   // HISPCP/LOSPCP prescale register settings, normally it will be set to default values
   //These bits configure the high-speed peripheral clock (HSPCLK) rate relative to SYSCLKOUT:
   //If HISPCP(2) 1 0, HSPCLK = SYSCLKOUT/(HISPCP X 2)
   //If HISPCP = 0, HSPCLK = SYSCLKOUT
   //000 -- High speed clock = SYSCLKOUT/1
   //001 -- High speed clock = SYSCLKOUT/2 (reset default)
   //010 -- High speed clock = SYSCLKOUT/4
   //011 -- High speed clock = SYSCLKOUT/6
   //100 -- High speed clock = SYSCLKOUT/8
   //101 -- High speed clock = SYSCLKOUT/10
   //110 -- High speed clock = SYSCLKOUT/12
   //111 -- High speed clock = SYSCLKOUT/14
   SysCtrlRegs.HISPCP.all = SYSCTRL_HSCLK_PRESCALER; // 150/2=75MHz

   //These bits configure the low-speed peripheral clock (LSPCLK) rate relative to SYSCLKOUT:
   //If LOSPCP(2) 1 0, then LSPCLK = SYSCLKOUT/(LOSPCP X 2)
   //If LOSPCP = 0, then LSPCLK = SYSCLKOUT
   //000 -- Low speed clock = SYSCLKOUT/1
   //001 -- Low speed clock= SYSCLKOUT/2
   //010 -- Low speed clock= SYSCLKOUT/4 (reset default)
   //011 -- Low speed clock= SYSCLKOUT/6
   //100 -- Low speed clock= SYSCLKOUT/8
   //101 -- Low speed clock= SYSCLKOUT/10
   //110 -- Low speed clock= SYSCLKOUT/12
   //111 -- Low speed clock= SYSCLKOUT/14
   SysCtrlRegs.LOSPCP.all = SYSCTRL_LSCLK_PRESCALER;// 150/4=37.5MHz



   // XCLKOUT to SYSCLKOUT ratio.  By default XCLKOUT = 1/4 SYSCLKOUT
   // XTIMCLK = SYSCLKOUT/2
   XintfRegs.XINTCNF2.bit.XTIMCLK = 1;
   // XCLKOUT = XTIMCLK/2
   XintfRegs.XINTCNF2.bit.CLKMODE = 1;
   // Enable XCLKOUT
   XintfRegs.XINTCNF2.bit.CLKOFF = 0;



   // Peripheral clock enables set for the selected peripherals.
   // If you are not using a peripheral leave the clock off
   // to save on power.
   //
   // Note: not all peripherals are available on all 2833x derivates.
   // Refer to the datasheet for your particular device.

   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // TBCLK within the ePWM

   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    // ADC

   ADC_cal();

   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;   // I2C
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
   SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0;   // SCI-B
   SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 0;   // SCI-C
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0;   // SPI-A
   SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 0; // McBSP-A
   SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 0; // McBSP-B
   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;    // eCAN-A
   SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;    // eCAN-B

   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;  // ePWM1
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 0;  // ePWM2
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 0;  // ePWM3
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;  // ePWM4
   SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;  // ePWM5
   SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;  // ePWM6
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;  // eCAP1
   SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0;  // eCAP2
   SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;  // eCAP3
   SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;  // eCAP4
   SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 0;  // eCAP5
   SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 0;  // eCAP6
   SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;  // eQEP1
   SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;  // eQEP2

   SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 0; // CPU Timer 2
   SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 0;       // DMA Clock
   SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 0;     // XTIMCLK
   SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 0;    // GPIO input clock

   EDIS;
}

void InitPll_Initialize(Uint16 val, Uint16 divsel)
{
    // Make sure the PLL is not running in limp mode
    //advancechip 新增代码
    EALLOW;
    SysCtrlRegs.PLLSTS.all |= 0x0050;
    EDIS;

    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
        // Missing external clock has been detected
        // Replace this line with a call to an appropriate
        // SystemShutdown(); function.
        ESTOP0;
        //HALT_EMU();
    }


    // DIVSEL MUST be 0 before PLLCR can be changed from
    // 0x0000. It is set to 0 by an external reset XRSn
    // This puts us in 1/4
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }


    // Change the PLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != val)
    {
        // Before setting PLLCR turn off missing clock detect logic
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        DELAY_US(20000);

        // Optional: Wait for PLL to lock.
        // During this time the CPU will switch to OSCCLK/2 until
        // the PLL is stable.  Once the PLL is stable the CPU will
        // switch to the new PLL value.
        //
        // This time-to-lock is monitored by a PLL lock counter.
        //
        // Code is not required to sit and wait for the PLL to lock.
        // However, if the code does anything that is timing critical,
        // and requires the correct clock be locked, then it is best to
        // wait until this switching has completed.
        DisableDog();

        // Wait for the PLL lock bit to be set.
        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
        {
            // Uncomment to service the watchdog

        }

        //advancechip 需要屏蔽此处代码
//            EALLOW;
//            SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
//            EDIS;
    }


    //
    if((divsel == 1) || (divsel == 2))
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
        EDIS;
    }

    // NOTE: ONLY USE THIS SETTING IF PLL IS BYPASSED (I.E. PLLCR = 0) OR OFF
    // If switching to 1/1
    // * First go to 1/2 and let the power settle
    //   The time required will depend on the system, this is only an example
    // * Then switch to 1/1
    // 为什么要先2分频，再1分频？
    // 原因：逐步增加SYSCLKOUT频率，从而使电力阶段不必处理(接近瞬时的)电流需求峰值。
    // 如果您的电源级能够满足这种尖峰，而不会对电源电压产生任何不利影响，则此程序可能是不必要的。
    // 然而，经验表明，错开地增加频率是有益的。
    else if(divsel == 3)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;

        DELAY_US(50L);

        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
        EDIS;
    }
    else
    {
        //
    }
}












/* End of file */
