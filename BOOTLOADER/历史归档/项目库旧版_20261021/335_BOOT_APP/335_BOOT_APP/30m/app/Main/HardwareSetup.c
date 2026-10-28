//#############################################################################

/*<FH>*************************************************************************
* Copyright (c) :  2021-2031, Advancechip Co., Ltd. All Rights Reserved
* Original Author: pei.shen

* FileName: HardwareSetup.c
* Current Version: V1.0
* Date: 21-Aug-2021

* ABSTRACT:  the device Peripheral Initialize
* DESCRIPTION:

* Revise:
;   Last rev by:    ***
;   Last rev date:  ***
;   Last rev desc.: ***
**<FH>************************************************************************/

//-----------------------------------------------------------------------------
// Include

#include "MainInclude.h"


#pragma CODE_SECTION(Flash_Initialize, "ramfuncs");
#pragma CODE_SECTION(UpgradePrepareClock, "ramfuncs");
static void UpgradePrepareClock(void)
{
    EALLOW;
    FlashRegs.FBANKWAIT.bit.PAGEWAIT=5;
    FlashRegs.FBANKWAIT.bit.RANDWAIT=5;
    FlashRegs.FOTPWAIT.bit.OTPWAIT=8;
    EDIS;
    asm(" RPT #7 || NOP");
}


// These are defined by the linker
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;


/************************************************************
初始化DSP系统，包括初始化时钟和配置寄存器
************************************************************/
void SysCtrl_Initialize(void)
{   
    WDogDisable();           // Disable the watchdog

    // Initialize the PLL control: PLLCR and DIVSEL   (30M*10)/2=150M
    // DSP28_PLLCR and DSP28_DIVSEL are defined in DSP2833x_Examples.h

    InitPll_Initialize(UPGRADE_PLL,UPGRADE_DIVSEL);   // Change 30MHz clock to 150MHz

    PeripheralClocks_Initialize();     // Initialize the peripheral clocks
}

//---------------------------------------------------------------------------
// This function initializes the Flash Control registers
//                  CAUTION
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results
//---------------------------------------------------------------------------
void Flash_Initialize(void)
{
    EALLOW;

    // 使能flash的管道模式，可改善代码在flash中运行的效果。
    FlashRegs.FOPT.bit.ENPIPE = 1;

    // 设置flash运行的最小等待状态，请参考Advanchip最新的数据手册。
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = UPGRADE_FLASH_WAIT;
    FlashRegs.FBANKWAIT.bit.RANDWAIT = UPGRADE_FLASH_WAIT;
    FlashRegs.FOTPWAIT.bit.OTPWAIT = UPGRADE_OTP_WAIT;

    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;     // 默认设置，请勿更改
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;   // 默认设置，请勿更改

    EDIS;

    asm(" RPT #7 || NOP");                  // 等待，确保上面配置参数写成功

}

//--------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
//初始化的时候首先把所有的中断服务程序 都初始化为默认服务程序
//---------------------------------------------------------------------------
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

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    //
    while(SourceAddr < SourceEndAddr)
    {
        //
        *DestAddr++ = *SourceAddr++;
    }
}


/************************************************************
初始化中断服务程序
************************************************************/
void Interrupt_Initialize(void)
{
   EALLOW;                                          //设置用户服务程序
//   PieVectTable.ADCINT      = &ADC_Over_isr;        //ADC结束中断
//   PieVectTable.EPWM1_INT   = &EPWM1_zero_isr;      //下溢中断
//   PieVectTable.SCIRXINTB   = &SCI_RXD_isr;         //通讯接收中断
//   PieVectTable.SCITXINTB   = &SCI_TXD_isr;         //通讯发送中断
//   PieVectTable.TINT0       = &BSP_timer0ISR;       //定时器中断
   EDIS;
}

/************************************************************
开启已经使用的中断
************************************************************/
void SetInterruptEnable(void)
{
   IER |= (M_INT1 | M_INT2 | M_INT3 | M_INT9);  //  Enable interrupts:
   PieCtrlRegs.PIEIER1.bit.INTx6 = 1;           //  ADCINT
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;           //  EPWM1_INT
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;           //  Enable TINT0
//   // Enable SCIRX, SCITX interrupt...
   PieCtrlRegs.PIEIER9.bit.INTx1 = 1;           // PIE Group 9, INT1
   PieCtrlRegs.PIEIER9.bit.INTx2 = 1;           // PIE Group 9, INT2
   EINT;
   ERTM;
}

/*******************************************************************************
Peripherals_Initialize - The following function initializes the peripherals to a default state.

// It calls each of the peripherals default initialization functions.
// This function should be executed at boot time or on a soft reset.

Paramters:


Precondition:
-
-
Postcondition:
-
-

*******************************************************************************/
void Peripherals_Initialize(void)
{

    // Initialize ADC Peripheral To default State:
  //  ADC_Initialize();

    // Initialize ePWM Peripherals To default State:
  //  ePWM_Initialize();

    // Initialize Gpio Peripherals To default State:
    Gpio_Initialize();

    // Initialize SCI Peripherals To default State:
    Sci_Initialize();

    // Initialize IIC Peripherals To default State:
    //I2C_Initialize();

    // Initialize eCAN Peripheral To default State:
    //ECan_Initialize();

    // Initialize SPI Peripherals To default State:
    //Spi_Initialize();

    // Initialize LED Peripherals To default State:
   // SegmentLED_Initialize();

    InitCpuTimers();
    CpuTimer1Regs.TPR.all=0; CpuTimer1Regs.TPRH.all=0;
    CpuTimer1Regs.TCR.bit.TRB=1;
//    ConfigCpuTimer(&CpuTimer0, DSP_CLOCK, 100L); // 60MHz CPU, 100 us Period = 10K     17us = 60K
//    StartCpuTimer0();                  //暂时没用的定时器中断
    StartCpuTimer1();                    //作为主程序的时间基准
}

//---------------------------------------------------------------------------
// This function resets the watchdog timer.
//---------------------------------------------------------------------------
void WDogService(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//---------------------------------------------------------------------------
// This function disables the watchdog timer.
//---------------------------------------------------------------------------

void WDogDisable(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}


//---------------------------------------------------------------------------
// This function Enables the watchdog timer.
//---------------------------------------------------------------------------
void EnableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x002A;
    EDIS;
}

//--------------------------------------------------------------------------
// This function initializes the clocks to the peripheral modules.
//---------------------------------------------------------------------------
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

/************************************************************
所有误操作的中断处理
************************************************************/
int gErrorIntCnt;
interrupt void ISR_ILLEGAL(void)
{
    gErrorIntCnt++;             //监控是否有错误进入中断的情况
}

void InitAppData(void)
{
    //ECanMgr_Initialize();
    RemoteModbus_Initialize();

}

/************************************************************
 DSP芯片底层初始化
************************************************************/
void InitDevice(void)
{
    //Step1:Initialize System Control registers,PLL,WatchDog,Clocks to default states.
    //Global Disable all Interrupts
    MemCopy((Uint16*)&RamfuncsLoadStart, (Uint16*)&RamfuncsLoadEnd, (Uint16*)&RamfuncsRunStart);
    UpgradePrepareClock();
    SysCtrl_Initialize();

    //Step2:Flash Initialize.
    Flash_Initialize();

    //Step3:Pie control registers Initialize.
    PieCtrl_Initialize();

    //Step4:Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    //Step5:Pie Vect Initialize
    PieVectTable_Initialize();

    //Step6:Interrupt Initialize.
    Interrupt_Initialize();

    //Step7:Peripheral Initialize.
//    Peripherals_Initialize();
}

//---------------------------------------------------------------------------
// This function initializes the PLLCR register.
//---------------------------------------------------------------------------
void InitPll_Initialize(Uint16 val, Uint16 divsel)
{
    // Make sure the PLL is not running in limp mode
        //advancechip 新增代码
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
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
            EALLOW;
            SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
            EDIS;
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


//---------------------------------------------------------------------------
// Set GPIO 口的复用、上拉下拉、同步
//---------------------------------------------------------------------------
void Gpio_Initialize(void)
{
       EALLOW;

       //LED DS3
       GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;
       GpioDataRegs.GPASET.bit.GPIO27 = 1;

       //LED DS4
       GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;
       GpioDataRegs.GPASET.bit.GPIO26 = 1;

       //LED DS5
       GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
       GpioDataRegs.GPASET.bit.GPIO25 = 1;


       //LED DS6
       GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
       GpioDataRegs.GPASET.bit.GPIO24 = 1;

       //GPIO43
       GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;
       GpioCtrlRegs.GPBDIR.bit.GPIO43 = 1;
       GpioDataRegs.GPBSET.bit.GPIO43 = 1;

       //GPIO65
       GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 0;
       GpioCtrlRegs.GPCDIR.bit.GPIO65 = 1;
       GpioDataRegs.GPCSET.bit.GPIO65= 1;
//
//
//       //58 GPIO
//       GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
//       GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;
//       GpioDataRegs.GPBSET.bit.GPIO58 = 1;

//       //485 GPIO
//       GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
//       GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
//       GpioDataRegs.GPASET.bit.GPIO24 = 1;

       //AIP650 GPIO
//       GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;
//       GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
//       GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;
//       GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;
//       GpioCtrlRegs.GPBPUD.bit.GPIO35 = 1;
//       GpioCtrlRegs.GPAPUD.bit.GPIO30 = 1;

       EDIS;
}	

//===========================================================================
// End of file.
//===========================================================================
