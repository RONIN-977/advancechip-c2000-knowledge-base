
//-----------------------------------------------------------------------------
// Include

#include "MainInclude.h"

#pragma CODE_SECTION(Flash_Initialize, "ramfuncs");

// These are defined by the linker
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

//#define External_OSC            1
/************************************************************
初始化DSP系统，包括初始化时钟和配置寄存器
************************************************************/
void SysCtrl_Initialize(void)
{   
    DisableDog();           // Disable the watchdog

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // Enable ADC peripheral clock
    (*Device_cal)();
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; // Return ADC clock to original state
    EDIS;

    InitPll_Initialize(DSP28_PLLCR,DSP28_DIVSEL);      // Change 10MHz clock to 60MHz

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
    //Enable Flash Pipeline mode to improve performance
    //of code executed from Flash.
    FlashRegs.FOPT.bit.ENPIPE = 1;

    //Set the Paged Waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;

    //Set the Random Waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;

    //Set the Waitstate for the OTP
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;

    //                CAUTION
    //ONLY THE DEFAULT VALUE FOR THESE 2 REGISTERS SHOULD BE USED
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    EDIS;

    //Force a pipeline flush to ensure that the write to
    //the last register configured occurs before returning.

    asm(" RPT #7 || NOP");
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
//   PieVectTable.ADCINT1     = &ADC_Over_isr;        //ADC结束中断
//   PieVectTable.EPWM1_INT   = &EPWM1_zero_isr;      //下溢中断
//   PieVectTable.SCIRXINTA   = &SCI_RXD_isr;         //通讯接收中断
//   PieVectTable.SCITXINTA   = &SCI_TXD_isr;         //通讯发送中断
//   PieVectTable.TINT0       = &BSP_timer0ISR;       //定时器中断
//
//   PieVectTable.TINT2       = &BSP_timer2ISR;       //定时器中断
   EDIS;
}

/************************************************************
开启已经使用的中断
************************************************************/
void SetInterruptEnable(void)
{
   IER |= (M_INT1 | M_INT2 | M_INT3 |M_INT9);  //  Enable interrupts:
//   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;           //  ADC1INT
//   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;           //  EPWM1_INT
//   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;           //  Enable TINT0
//   // Enable SCIRX, SCITX interrupt...
//   PieCtrlRegs.PIEIER9.bit.INTx1 = 1;           // PIE Group 9, INT1
//   PieCtrlRegs.PIEIER9.bit.INTx2 = 1;           // PIE Group 9, INT2
   EINT;
   ERTM;
}

/************************************************************
初始化DSP的所有外设，为电机控制和接口做准备
************************************************************/
void Peripherals_Initialize(void)
{
    // Lin initialize
    Lin_Initialize();
    // Gpio initialize
    Gpio_Initialize();

    InitCpuTimers();
    //    ConfigCpuTimer(&CpuTimer0, DSP_CLOCK, 200000L); // 60MHz CPU, 200 ms Period
    //    StartCpuTimer0();                    //计时定时器中断
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

 // LOSPCP prescale register settings, normally it will be set to default values

    //GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // GPIO18 = XCLKOUT
    SysCtrlRegs.LOSPCP.all = SYSCTRL_LSCLK_PRESCALER;

 // XCLKOUT to SYSCLKOUT ratio.  By default XCLKOUT = 1/4 SYSCLKOUT
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;

 // Peripheral clock enables set for the selected peripherals.
 // If you are not using a peripheral leave the clock off
 // to save on power.
 //
 // Note: not all peripherals are available on all ADM32F03x derivates.
 // Refer to the datasheet for your particular device.
 //
 // This function is not written to be an example of efficient code.

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK   = 0;    // ADC
    SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 0;    // COMP1
    SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 0;    // COMP2
    SysCtrlRegs.PCLKCR3.bit.COMP3ENCLK = 0;    // COMP3
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;    // eCAP1
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0;    // eCAN-A
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;    // eQEP1
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;    // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 0;    // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 0;    // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;    // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;    // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;    // ePWM6
    SysCtrlRegs.PCLKCR1.bit.EPWM7ENCLK = 0;    // ePWM7
    SysCtrlRegs.PCLKCR0.bit.HRPWMENCLK = 0;    // HRPWM
    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0;     // I2C
    SysCtrlRegs.PCLKCR0.bit.LINAENCLK = 0;     // LIN-A
    SysCtrlRegs.PCLKCR3.bit.CLA1ENCLK = 0;     // CLA1
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;     // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0;     // SPI-A
    SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 0;     // SPI-B
    SysCtrlRegs.PCLKCR2.bit.HRCAP1ENCLK = 0;
    SysCtrlRegs.PCLKCR2.bit.HRCAP2ENCLK = 0;

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;     // Enable TBCLK within the ePWM

    EDIS;
}

/************************************************************
所有误操作的中断处理
************************************************************/
int gErrorIntCnt;
interrupt void rsvd_ISR(void)
{
    gErrorIntCnt++;             //监控是否有错误进入中断的情况
}

void InitAppData(void)
{
    //ECanMgr_Initialize();
    memset(&gCpuTime, 0, sizeof(gCpuTime));
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
    // Interrupt_Initialize();

    //Step7:Peripheral Initialize.
    Peripherals_Initialize();
}

//---------------------------------------------------------------------------
// This function initializes the PLLCR register.
//---------------------------------------------------------------------------
void InitPll_Initialize(Uint16 val, Uint16 divsel)
{
    // select Osc source: external crystal-ocs, and turn off other osc-source
#ifdef External_OSC
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 0;     // Turn on XTALOSC
    DELAY_US(1000L);
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF  = 1;     // Turn off XCLKIN
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0;  // Switch to external clock
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // Switch from INTOSC1 to INTOSC2/ext clk
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // Switch Watchdog Clk Src to external clock
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;     // Turn off INTOSC2
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;     // Turn off INTOSC1
    EDIS;
#else
    // Switch to Internal Oscillator 1 and turn off all other clock
    // sources to minimize power consumption
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF   =   0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL =   0;      // Clk Src = INTOSC1
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF    =   1;      // Turn off XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF   =   1;      // Turn off XTALOSC
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF   =   1;      // Turn off INTOSC2
    EDIS;
#endif

       volatile Uint16 iVol;

       // Make sure the PLL is not running in limp mode
       if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
       {
          EALLOW;
          // OSCCLKSRC1 failure detected. PLL running in limp mode.
          // Re-enable missing clock logic.
          SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
          EDIS;
          // Replace this line with a call to an appropriate
          // SystemShutdown(); function.
          __asm("        ESTOP0");     // Uncomment for debugging purposes
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

          EALLOW;
          // Before setting PLLCR turn off missing clock detect logic
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

          // Wait for the PLL lock bit to be set.

          // The watchdog should be disabled before this loop, or fed within
          // the loop via ServiceDog().

          // Uncomment to disable the watchdog
          DisableDog();

          while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
          {
              // Uncomment to service the watchdog
              // ServiceDog();
          }

          EALLOW;
          SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
          EDIS;
        }

        // If switching to 1/2
        if((divsel == 1)||(divsel == 2))
        {
            EALLOW;
            SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
            EDIS;
        }

        // If switching to 1/1
        // * First go to 1/2 and let the power settle
        //   The time required will depend on the system, this is only an example
        // * Then switch to 1/1
        if(divsel == 3)
        {
            EALLOW;
            SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
            DELAY_US(50L);
            SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
            EDIS;
        }
}

//---------------------------------------------------------------------------
// Set GPIO 口的复用、上拉下拉、同步
//---------------------------------------------------------------------------
void Gpio_Initialize(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO2  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO3  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO4  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO5  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO7  = 1;

    GpioDataRegs.GPADAT.bit.GPIO2  = 0;
    GpioDataRegs.GPADAT.bit.GPIO3  = 0;
    GpioDataRegs.GPADAT.bit.GPIO4  = 0;
    GpioDataRegs.GPADAT.bit.GPIO5  = 0;
    GpioDataRegs.GPADAT.bit.GPIO6  = 0;
    GpioDataRegs.GPADAT.bit.GPIO7  = 0;

//    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;
//    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;
//    GpioCtrlRegs.GPADIR.bit.GPIO16  = 1;
//    GpioCtrlRegs.GPADIR.bit.GPIO17  = 1;
//    GpioDataRegs.GPADAT.bit.GPIO16  = 0;
//    GpioDataRegs.GPADAT.bit.GPIO17  = 1;

//    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  //CLKOUT

    EDIS;
}	


void InitFlashAPI(void)
{
    Uint16 Status;

    MemCopy((Uint16*)&Flash28_API_LoadStart, (Uint16*)&Flash28_API_LoadEnd, (Uint16*)&Flash28_API_RunStart);

    EALLOW;
    Flash_CPUScaleFactor = SCALE_FACTOR;
    Flash_CallbackPtr = 0;
    EDIS;

    Status = CsmUnlock();
    if (Status != STATUS_SUCCESS)
    {
        //
    }
}

//===========================================================================
// End of file.
//===========================================================================
