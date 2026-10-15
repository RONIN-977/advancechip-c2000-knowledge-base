/***************************************************************************
 该文件提供对所有需要使用的外设初始化操作，主要包括：
1、系统时钟初始化
2、搬移部分程序到RAM中运行
3、FLASH初始化
4、为需要使用的外设提供时钟
5、所有GPIO口应用情况初始化
6、初始化定时器
****************************************************************************
注意:

  此处只包含性能部分的初始化,功能模块的初始化在性能初始化完成后执行，调试时
  首先要判断此处的初始化结果，有没有被功能的初始化意外修改
***************************************************************************/
 
#include "DSP2803x_EPwm_defines.h"
#include "MotorInclude.h"
#include "SPIcomm.h"
#include "PreDriver.h"
#include "Parameter.h"

#define Device_cal (void   (*)(void))0x3D7C80

/**修改FLASH寄存器的程序需要搬移到FLASH中执行**/
#pragma CODE_SECTION(InitFlash, "ramfuncs");
extern COPY_TABLE prginRAM;

void 	copy_prg(COPY_TABLE *tp);
void   	DisableDog(void);   
void 	InitPll(Uint val);
void	InitFlash(void);
void	InitPeripheralClocks(void);

void 	InitPieCtrl(void);
void 	InitPieVectTable(void);
void 	InitSetGpio(void);
void    InitSetPWM();
void    InitSetAdc(void);
void    SCI_Setup(void);
void    LIN_Setup(void);
void InitXint1Gpio(void);

interrupt void Xint1_isr(void);
extern  interrupt void SCI_RXD_isr(void);
extern  interrupt void SCI_TXD_isr(void);
extern  interrupt void LINA_LEVEL_isr(void);



/************************************************************
初始化DSP系统，包括初始化时钟和配置寄存器
************************************************************/
void InitSysCtrl()
{   
    DisableDog();			// Disable the watchdog

    // *IMPORTANT*
    // The Device_cal function, which copies the ADC & oscillator calibration values
    // from TI reserved OTP into the appropriate trim registers, occurs automatically
    // in the Boot ROM. If the boot ROM code is bypassed during the debug process, the
    // following function MUST be called for the ADC and oscillators to function according
    // to specification. The clocks to the ADC MUST be enabled before calling this
    // function.
    // See the device data manual and/or the ADC Reference
    // Manual for more information.
	EALLOW;
	//SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // Enable ADC peripheral clock
	//(*Device_cal)();
	//SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; // Return ADC clock to original state
	EDIS;

    // Select Internal Oscillator 1 as Clock Source (default), and turn off all unused clocks to
    // conserve power.
    // Initialize the PLL control: PLLCR and CLKINDIV
    // DSP28_PLLCR and DSP28_CLKINDIV are defined in DSP2803x_Examples.h
	InitPll(DSP_CLOCK/10);

    copy_prg(&prginRAM);		// Move the program from FLASH to RAM
    InitFlash();				// Initializes the Flash Control registers

    InitPeripheralClocks();		// Initialize the peripheral clocks
}

/************************************************************
初始化中断服务程序
************************************************************/
void InitInterrupt()
{
   DINT;							//Clear all interrupts and initialize PIE vector table:
   InitPieCtrl();  					//Disable PIE
   IER = 0x0000;
   IFR = 0x0000;

   InitPieVectTable();				//Enable PIE

   EALLOW;  						//设置用户服务程序

   PieVectTable.ADCINT1     = &EndOfADCISR;		    //ADC结束中断
   PieVectTable.EPWM2_TZINT = &OneShotTZOfEPWMISR;	//过流中断
   PieVectTable.EPWM2_INT 	= &ZeroOfEPWMISR;		//下溢中断

#ifndef LIN_CONTROL_MODE
   PieVectTable.SCIRXINTA 	= &SCI_RXD_isr; 		//通讯接收中断
   PieVectTable.SCITXINTA 	= &SCI_TXD_isr; 		//通讯发送中断
#endif
   PieVectTable.LIN0INTA    = &LINA_LEVEL_isr;
   PieVectTable.XINT1       = &Xint1_isr;
   EDIS;
}

/************************************************************
开启已经使用的中断
************************************************************/
void SetInterruptEnable()
{
   IER |= (M_INT1 | M_INT2 | M_INT3 | M_INT5);	//  Enable interrupts:
   IER |= M_INT9;
   IER |= M_INT4;

   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;           //  ADC1INT
   PieCtrlRegs.PIEIER2.bit.INTx2 = 1;           // 	EPWM1_TZ
   PieCtrlRegs.PIEIER3.bit.INTx2 = 1;           // 	EPWM1_INT

   // Enable SCIRX, SCITX interrupt...
   PieCtrlRegs.PIEIER9.bit.INTx1 = 1;     		// PIE Group 9, INT1
   PieCtrlRegs.PIEIER9.bit.INTx2 = 1;     		// PIE Group 9, INT2
   PieCtrlRegs.PIEIER9.bit.INTx3 = 1;

   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          //  XINT1
}

/************************************************************
初始化DSP的所有外设，为电机控制和接口做准备
************************************************************/
void InitPeripherals(void)
{
   	InitSetGpio();
   	InitSetPWM();
   	InitSetAdc();

#ifdef SPI_SCOPE_TEST
   	InitSpiIO();
   	InitSpi();
#endif

#if (ADM32F036_TYPE == ADM32F036_A3)
   	InitI2CIO();
   	InitI2C();
   	PreDriverInit();
   	DriverClaerFault();
#endif

   	// Set up SCI function...
   	// SCI_Setup();
   	LIN_Setup();
   	InitXint1Gpio();
   	InitCpuTimers();

   	//ConfigCpuTimer(&CpuTimer0, DSP_CLOCK, 1000000L);//100MHz CPU, 1 millisecond Period
   	//StartCpuTimer0();					 //暂时没用的定时器中断
   	StartCpuTimer1();				     //作为主程序的时间基准
}

//---------------------------------------------------------------------------
// Move program from FLASH to RAM size < 65535 words
//---------------------------------------------------------------------------
extern MovePrgFrFlashToRam(long src_addr, long dst_addr, Uint16 size);
void copy_prg(COPY_TABLE *tp)
{
	Uint size;
	COPY_RECORD *crp = &tp->recs[0];

	size = (Uint)crp->size - 1;

	MovePrgFrFlashToRam(crp->src_addr, crp->dst_addr, size);
}

//---------------------------------------------------------------------------
// This function initializes the Flash Control registers
//                   CAUTION 
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results
//---------------------------------------------------------------------------
void InitFlash(void)
{
   EALLOW;

   //Enable Flash Pipeline mode to improve performance of code executed from Flash.
   //FlashRegs.FOPT.bit.ENPIPE = 1;
   FlashRegs.FOPT.all = 0x01;
   //Set the Random Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.RANDWAIT = READ_RAND_FLASH_WAITE;
   //Set the Paged Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = READ_PAGE_FLASH_WAITE;
   //Set the Waitstate for the OTP
   FlashRegs.FOTPWAIT.bit.OTPWAIT = READ_OTP_WAITE;
   
   //Set number of cycles to transition from sleep to standby
   //FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;       
   //Set number of cycles to transition from standby to active
   //FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;   
   EDIS;

   asm(" RPT #7 || NOP");	
}	

//---------------------------------------------------------------------------
// This function resets the watchdog timer.
//---------------------------------------------------------------------------
void KickDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

void ResetDSP(void)
{
    EALLOW;
    SysCtrlRegs.WDCR = 0x0002;
    EDIS;
}

//---------------------------------------------------------------------------
// This function disables the watchdog timer.
//---------------------------------------------------------------------------
void DisableDog(void)
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

//---------------------------------------------------------------------------
// This function initializes the PLLCR register.
//---------------------------------------------------------------------------
void InitPll(Uint16 val)
{
    // select Osc source: external crystal-ocs, and turn off other osc-source
#ifndef INTER_OSC
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 0;     // Turn on XTALOSC
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
	SysCtrlRegs.CLKCTL.bit.INTOSC1OFF 	= 	0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL	=	0;  	// Clk Src = INTOSC1
	SysCtrlRegs.CLKCTL.bit.XCLKINOFF	=	1;     	// Turn off XCLKIN
	SysCtrlRegs.CLKCTL.bit.XTALOSCOFF	=	1;    	// Turn off XTALOSC
	SysCtrlRegs.CLKCTL.bit.INTOSC2OFF	=	1;    	// Turn off INTOSC2
    EDIS;
#endif

    // configure the PLL
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)	// Make sure the PLL is not running in limp mode
    {
        // DIVSEL MUST be 0 before PLLCR can be changed from
        // 0x0000. It is set to 0 by an external reset XRSn
        // This puts us in 1/4
        if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
        {
            EALLOW;
            SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
            EDIS;
        }

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
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;     // turn off missing clock detect
        EDIS;

        EALLOW;
#ifndef INTER_OSC
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;      // configure PLLSTS.DIVSEL = 2  (default)
#else
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;      // configure PLLSTS.DIVSEL = 2  (default)
#endif
        EDIS;
    }
}

//--------------------------------------------------------------------------
// This function initializes the clocks to the peripheral modules.
//---------------------------------------------------------------------------
void InitPeripheralClocks(void)
{
   EALLOW;

   //SysCtrlRegs.HISPCP.all = 0x0001;			//50MHz for ADC / 30MHz
   SysCtrlRegs.LOSPCP.all = 0x0002;				//25MHz for SCI and SPI / 15MHz

   //SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;		//Set ClockOut Pin = 100MHz/60MHZ
   SysCtrlRegs.XCLK.all = 0x02;
   
   SysCtrlRegs.PCLKCR0.bit.HRPWMENCLK = 0;      // 0     Enable low speed clk to HRPWM
   SysCtrlRegs.PCLKCR0.bit.LINAENCLK  = 1;      // 1     Enable low speed clk to LIN-A
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC  = 1;      // 2     ETWPM Module TBCLK enable/sync
   SysCtrlRegs.PCLKCR0.bit.ADCENCLK   = 1;      // 3     Enable high speed clk to ADC
   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK  = 0;      // 4     Enable SYSCLKOUT to I2C-A
                                                // 7:5   reserved
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK  = 1;      // 8     Enable low speed clk to SPI-A
   SysCtrlRegs.PCLKCR0.bit.SPIBENCLK  = 0;      // 9     Enable low speed clk to SPI-B
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK  = 1;      // 10    Enable low speed clk to SCI-A
                                                // 13:11 reserved
   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;      // 14    Enable system clk to eCAN-A
                                                // 15    reserved

   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 0;   // 0     Enable SYSCLKOUT to EPWM1
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;   // 1     Enable SYSCLKOUT to EPWM2
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;   // 2     Enable SYSCLKOUT to EPWM3
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;   // 3     Enable SYSCLKOUT to EPWM4
   SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;   // 4     Enable SYSCLKOUT to EPWM5
   SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;   // 5     Enable SYSCLKOUT to EPWM6
   SysCtrlRegs.PCLKCR1.bit.EPWM7ENCLK = 0;   // 6     Enable SYSCLKOUT to EPWM7
                                             // 7     reserved
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;   // 8     Enable SYSCLKOUT to ECAP1
                                             // 13:9  reserved
   SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;   // 14    Enable SYSCLKOUT to EQEP1
                                             // 15    reserved

   SysCtrlRegs.PCLKCR3.all = 0x6701;
   SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1;
   EDIS;
}

//--------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
void InitPieCtrl(void)
{
	Uint * m_Point1,* m_Point2;
	Uint   m_Index;

    DINT;    
    
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	// Disable the PIE
	// Clear all PIEIER registers:
	m_Point1 = (Uint *)&PieCtrlRegs.PIEIER1.all;
	m_Point2 = (Uint *)&PieCtrlRegs.PIEIFR1.all;
	for(m_Index = 0;m_Index<12;m_Index++)
	{
		*(m_Point1++) = 0;
		*(m_Point2++) = 0;
	}
}	

/************************************************************
所有误操作的中断处理
************************************************************/
int gErrorIntCnt;
interrupt void rsvd_ISR(void)      
{
	gErrorIntCnt++;				//监控是否有错误进入中断的情况
}

//--------------------------------------------------------------------------
//初始化的时候首先把所有的中断服务程序 都初始化为默认服务程序
//---------------------------------------------------------------------------
void InitPieVectTable(void)
{
	int16	i;
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

//---------------------------------------------------------------------------
// Set GPIO 口的复用、上拉下拉、同步
//---------------------------------------------------------------------------
void InitSetGpio(void)
{
    EALLOW;

    // 2802x 变频冰箱控制板, 硬件设置...
    ////////////////////////////////////

    // STEP1:
    // GPIO口上拉选择, 例如 Relay 是高电平有效则不需要上拉，其他全部上拉...
    GpioCtrlRegs.GPAPUD.all = 0x00000000L;
    GpioCtrlRegs.GPBPUD.all = 0x00000000L;

    // STEP2:
    // GPIO口功能选择：设置使用的外设, QFN56
    // IO                                   PIN                    FUNCTION             DIR
    // B2/COMP1B/AI10                       5                       ADC                 X
    // B4/COMP2B/AI12                       6                       ADC                 X
    // B6/COMP3B/AI14                       7                       ADC                 X
    // GPIO34/CMPOUT2/CMPOUT3               8                       JTAG                X
    // GPIO35/TDI                           9                       JTAG                X
    // GPIO36/TMS                           10                      JTAG                X
    // GPIO37/TDO                           11                      JTAG                X
    // GPIO38/TCK                           12                      JTAG                X
    // GPIO18/SPICLKA/LINTXA/XCLKOUT        13                      SPICLKA             X
    // GPIO19/SPISTEA/XCLKIN/LINRXA/HRCAP   14                      SPISTEA             X
    // GPIO17/SOMIA/TZ3                     15                      SOMIA               X
    // GPIO16/SIMOA/TZ2                     16                      SIMOA               X
    // GPIO1/EPWM1B/COMP1OUT                17                      NOT USED            X
    // GPIO0/EPWM1A/CANRX/CANFDRX           18                      NOT USED            X
    // GPIO32/EPWMSYNC/CANTX/CANFDTX        19                      TEST                OUT
    // GPIO9/HRCAP                          26                      LIN_ENABLE          OUT
    // A6/COMP3A/AI6                        49                      ADC
    // A4/COMP2A/AI4                        50                      ADC
    // A2/COMP1A/AI2                        53                      ADC
    ///////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    // GPIO02                               30                      EPWM2A              OUT
    // GPIO03                               32                      EPWM2B              OUT
    // GPIO04                               33                      EPWM3A              OUT
    // GPIO05                               35                      EPWM3B              OUT
    // GPIO06                               36                      EPWM4A              OUT
    // GPIO07                               38                      EPWM4B              OUT
    ///////////////////////////////////////////////////////////////////////////////////////
    GpioCtrlRegs.GPAMUX1.all = 0x00000000L;
    GpioCtrlRegs.GPAMUX2.all = 0x00000000L;
    GpioCtrlRegs.GPBMUX1.all = 0x00000000L;

    // STEP3:
    // GPIO数据先初始化为需要的值
    GpioDataRegs.GPADAT.all = 0x00000000L;
    GpioDataRegs.GPBDAT.all = 0x00000000L;

    // GPIO口方向选择1：复位为输入
    // 0100 0000 0000 0100 0000 0000 0111 1111
    GpioCtrlRegs.GPADIR.all = 0x00;
    GpioCtrlRegs.GPBDIR.all = 0x00;
    GpioCtrlRegs.GPADIR.bit.GPIO2  = 1;     // EPWM2A
    GpioCtrlRegs.GPADIR.bit.GPIO3  = 1;     // EPWM2B
    GpioCtrlRegs.GPADIR.bit.GPIO4  = 1;     // EPWM3A
    GpioCtrlRegs.GPADIR.bit.GPIO5  = 1;     // EPWM3B
    GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;     // EPWM4A
    GpioCtrlRegs.GPADIR.bit.GPIO7  = 1;     // EPWM4B

    // 高有效，初始化低电平，对于低有效的需取反
    GpioDataRegs.GPACLEAR.bit.GPIO2  = 1;   // FORCE LOW
    GpioDataRegs.GPACLEAR.bit.GPIO3  = 1;   // FORCE LOW
    GpioDataRegs.GPACLEAR.bit.GPIO4  = 1;   // FORCE LOW
    GpioDataRegs.GPACLEAR.bit.GPIO5  = 1;   // FORCE LOW
    GpioDataRegs.GPACLEAR.bit.GPIO6  = 1;   // FORCE LOW
    GpioDataRegs.GPACLEAR.bit.GPIO7  = 1;   // FORCE LOW

    // STEP4:
    // GPIO口同步选择
    // 配置为异步的IO有：
    //      TZ1 GPIO12       --- 过流中断接口
    //      SCIRXDA GPIO28   --- 烧写SCI boot load
    // 配置为3 sample的有
    //      TZ2 GPIO16       --- CBC
    // 其他均为默认
    // GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;
    // GpioCtrlRegs.GPAQSEL1.bit.GPIO28 = 3;
    // GpioCtrlRegs.GPAQSEL1.bit.GPIO16 = 1;
    GpioCtrlRegs.GPAQSEL1.all = 0x03000000L;
    GpioCtrlRegs.GPAQSEL2.all = 0x03000001L;
    GpioCtrlRegs.GPBQSEL1.all = 0x0000000FL;        // GPIO32-GPIO34 Synch to SYSCLKOUT

    //GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;
    // LIN ENABLE
    // A3 -- GPIO09
    // A2 -- GPIO01
#if (ADM32F036_TYPE == ADM32F036_A3)
//    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
//    GpioCtrlRegs.GPADIR.bit.GPIO9  = 1;
//    GpioDataRegs.GPASET.bit.GPIO9  = 1; //LIN_EN--
#else
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO1  = 1;
    GpioDataRegs.GPASET.bit.GPIO1  = 1; //LIN_EN--
#endif

    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0x0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;

    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0x0; // HALL A
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0x0;  // HALL B
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0x0;  // HALL C
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;

    EDIS;
}


//---------------------------------------------------------------------------
// Set EPWM for motor Control
//---------------------------------------------------------------------------
void InitSetPWM(void)
{
	EALLOW;

	/////////////PWM1//////////////
	//Set the Time-Base (TB) Module
	EPwm2Regs.TBPRD = C_INIT_PRD;

	EPwm2Regs.TBPHS.all = 0;
	EPwm2Regs.TBCTL.all = 0x2012;
	//EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;
	//EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;	//定时器下溢产生同步信号
	//EPwm2Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;
	//Set the Counter-compare (CC) Module
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
	EPwm2Regs.CMPCTL.all = 0x0000;
#else
	EPwm2Regs.CMPCTL.all = 0x0100;
#endif

	//EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPA.half.CMPA = C_INIT_PRD/2;
	EPwm2Regs.CMPB = C_INIT_PRD/2;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef CUT_DOWN_VER_HARDWARE
	// Set the Action-qualifier (AQ) Module
	// 1001 0110, that is
	// CAD = 10, ePWMxA high when decreasing
	// CAU = 01, ePWMxA low when increasing
	// PRD = 00, nothing
	// ZRO = 00, nothing
	////////////////////////////////////////
	EPwm2Regs.AQCTLA.all = 0x0810;//0x0090;
#else
	EPwm2Regs.AQCTLA.all = 0x0420;//0x0060;
#endif
#else
#ifndef CUT_DOWN_VER_HARDWARE
    // Set the Action-qualifier (AQ) Module
    // 1001 0110, that is
    // CAD = 10, ePWMxA high when decreasing
    // CAU = 01, ePWMxA low when increasing
    // PRD = 00, nothing
    // ZRO = 00, nothing
    ////////////////////////////////////////
    EPwm2Regs.AQCTLA.all = 0x0090;
#else
    EPwm2Regs.AQCTLA.all = 0x0060;
#endif
#endif

#ifndef CUT_DOWN_VER_HARDWARE
	EPwm2Regs.DBCTL.all = 0x0007;
#else
	EPwm2Regs.DBCTL.all = 0x000B;
#endif

	//EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	//EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm2Regs.DBFED = C_MAX_DB;
	EPwm2Regs.DBRED = C_MAX_DB;

	// Set the PWM-chopper (PC) Module
	// Clear CBC,One-shot,and interrupt flag
	EPwm2Regs.TZCLR.all = 0x07;

    // DCAEVT1 as one-shot
    // Set the Trip-zone (TZ) Module
    // EPwm2Regs.DCACTL.bit.EVT1SRCSEL = 0;             // DCAEVT1 Source Is DCAEVT1 Signal
    // EPwm2Regs.DCACTL.bit.EVT1SYNCE = 1;              // DCAEVT1 SYNC, Enable
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;          // DCAEVT1 Source Is DCAEVT1 Signal
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT1 Force Synchronization Signal Select: Asynchronous Signal

    EPwm2Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;        // Digital Compare Output A Event 1 Selection:
                                                        // DCAH = low, DCAL = don't care
    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;  // Digital Compare A Low Input Select: COMP1OUT input
    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
                                                        // TZ1,TZ3 ASLO OK as DCAL don't care

    EPwm2Regs.TZSEL.bit.DCAEVT1 = 1;                    // Enable DCAEVT1 as one-shot-trip source for this ePWM module
    EPwm2Regs.TZSEL.bit.OSHT2 = 1;                      // Enable TZ2 as a one-shot trip source for this ePWM module

    // DCAEVT2 as CBC
    EPwm2Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;          // DCAEVT2 Source Is DCAEVT1 Signal
    EPwm2Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT2 Force Synchronization Signal Select: Asynchronous Signal

    EPwm2Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCBH_LOW;        // Digital Compare Output A Event 2 Selection:
                                                        // DCAH = low, DCAL = don't care
    EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;  // Digital Compare A Low Input Select: COMP2OUT input
    EPwm2Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
                                                        // TZ1,TZ3 ASLO OK as DCAL don't care

    EPwm2Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCAEVT2 as one-shot-trip source for this ePWM module

    //EPwm2Regs.TZSEL.bit.OSHT1 = TZ_ENABLE;	//过流信号对PWM1的封锁
	//EPwm2Regs.TZSEL.bit.CBC3 = TZ_ENABLE;		//EPWM1的逐波限流
    //Force to EPMAX,BX to high state...
	//EPwm2Regs.TZCTL.bit.TZA = 1;
	//EPwm2Regs.TZCTL.bit.TZB = 1;
#ifndef CUT_DOWN_VER_HARDWARE
	EPwm2Regs.TZCTL.all = 0x0005;
#else
	EPwm2Regs.TZCTL.all = 0x00A0;
#endif

	// EPwm2Regs.TZEINT.bit.OST = TZ_ENABLE;		//启用过流中断
	// Digital Comparator Output A Event 1 Interrupt Enable
	EPwm2Regs.TZEINT.all = 0x0004;

#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
	EPwm2Regs.ETPS.all = 0x0101;
#endif
    EPwm2Regs.ETPS.bit.INTPRD = 1;
	//EPwm2Regs.ETPS.bit.INTPRD = 1;
	//EPwm2Regs.ETPS.bit.SOCAPRD = 1;				//每一事件启动一次AD

	/////////////PWM2//////////////
	//Set the Time-Base (TB) Module
	EPwm3Regs.TBPRD = C_INIT_PRD;
	EPwm3Regs.TBPHS.all = 0;
	EPwm3Regs.TBCTL.all = 0x2006;

	//EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	//EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;	//以PWM1同步信号为输出同步信号
	//EPwm3Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;
	//Set the Counter-compare (CC) Module
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    EPwm3Regs.CMPCTL.all = 0x0000;
#else
    EPwm3Regs.CMPCTL.all = 0x0100;
#endif

	//EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPA.half.CMPA = C_INIT_PRD/2;

	//Set the Action-qualifier (AQ) Module
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef CUT_DOWN_VER_HARDWARE
    // Set the Action-qualifier (AQ) Module
    // 1001 0110, that is
    // CAD = 10, ePWMxA high when decreasing
    // CAU = 01, ePWMxA low when increasing
    // PRD = 00, nothing
    // ZRO = 00, nothing
    ////////////////////////////////////////
    EPwm3Regs.AQCTLA.all = 0x0810;//0x0090;
#else
    EPwm3Regs.AQCTLA.all = 0x0420;//0x0060;
#endif
#else
#ifndef CUT_DOWN_VER_HARDWARE
    // Set the Action-qualifier (AQ) Module
    // 1001 0110, that is
    // CAD = 10, ePWMxA high when decreasing
    // CAU = 01, ePWMxA low when increasing
    // PRD = 00, nothing
    // ZRO = 00, nothing
    ////////////////////////////////////////
    EPwm3Regs.AQCTLA.all = 0x0090;
#else
    EPwm3Regs.AQCTLA.all = 0x0060;
#endif
#endif

#ifndef CUT_DOWN_VER_HARDWARE
	//EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
	//EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	//Set the Dead-Band Generator (DB) Module
	EPwm3Regs.DBCTL.all = 0x0007;
#else
	EPwm3Regs.DBCTL.all = 0x000B;
#endif

	//EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	//EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm3Regs.DBFED = C_MAX_DB;
	EPwm3Regs.DBRED = C_MAX_DB;

    //DCAEVT1 as one-shot
    //Set the Trip-zone (TZ) Module
    // EPwm3Regs.DCACTL.bit.EVT1SYNCE = 0;//1;
    EPwm3Regs.DCACTL.bit.EVT1SRCSEL = 0;

    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;
    EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;

    EPwm3Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;

    EPwm3Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;
    EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm3Regs.TZSEL.bit.DCAEVT1 = 1;
    EPwm3Regs.TZSEL.bit.OSHT2 = 1;

    // DCAEVT2 as CBC
    EPwm3Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;          // DCAEVT2 Source Is DCAEVT1 Signal
    EPwm3Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT2 Force Synchronization Signal Select: Asynchronous Signal

    EPwm3Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCAH_LOW;        // Digital Compare Output A Event 2 Selection:
                                                        // DCAH = low, DCAL = don't care
    EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;  // Digital Compare A Low Input Select: COMP2OUT input
    EPwm3Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
                                                        // TZ1,TZ3 ASLO OK as DCAL don't care

    EPwm3Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCAEVT2 as one-shot-trip source for this ePWM module

#ifndef CUT_DOWN_VER_HARDWARE
    EPwm3Regs.TZCTL.all = 0x0005;
#elsed
    EPwm3Regs.TZCTL.all = 0x00A0;
#endif

	//EPwm3Regs.TZCTL.bit.TZA = 1;
	//EPwm3Regs.TZCTL.bit.TZB = 1;
	//Set the Event-trigger (ET) Module


	/////////////PWM3//////////////
	//Set the Time-Base (TB) Module
	EPwm4Regs.TBPRD = C_INIT_PRD;
	EPwm4Regs.TBPHS.all = 0;
	EPwm4Regs.TBCTL.all = 0x2006;

	//EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	//EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;	//不产生同步信号
	//EPwm4Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;
	//Set the Counter-compare (CC) Module
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    EPwm4Regs.CMPCTL.all = 0x0000;
#else
    EPwm4Regs.CMPCTL.all = 0x0100;
#endif

	//EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm4Regs.CMPA.half.CMPA = C_INIT_PRD/2;

	//Set the Action-qualifier (AQ) Module
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef CUT_DOWN_VER_HARDWARE
    // Set the Action-qualifier (AQ) Module
    // 1001 0110, that is
    // CAD = 10, ePWMxA high when decreasing
    // CAU = 01, ePWMxA low when increasing
    // PRD = 00, nothing
    // ZRO = 00, nothing
    ////////////////////////////////////////
    EPwm4Regs.AQCTLA.all = 0x0810;//0x0090;
#else
    EPwm4Regs.AQCTLA.all = 0x0420;//0x0060;
#endif
#else
#ifndef CUT_DOWN_VER_HARDWARE
    // Set the Action-qualifier (AQ) Module
    // 1001 0110, that is
    // CAD = 10, ePWMxA high when decreasing
    // CAU = 01, ePWMxA low when increasing
    // PRD = 00, nothing
    // ZRO = 00, nothing
    ////////////////////////////////////////
    EPwm4Regs.AQCTLA.all = 0x0090;
#else
    EPwm4Regs.AQCTLA.all = 0x0060;
#endif
#endif

#ifndef CUT_DOWN_VER_HARDWARE
	//EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
	//EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	//Set the Dead-Band Generator (DB) Module
	EPwm4Regs.DBCTL.all = 0x0007;
#else
	EPwm4Regs.DBCTL.all = 0x000B;
#endif

	//EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	//EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm4Regs.DBFED = C_MAX_DB;
	EPwm4Regs.DBRED = C_MAX_DB;

    // DCAEVT1 as one-shot
    // Set the Trip-zone (TZ) Module
    // EPwm4Regs.DCACTL.bit.EVT1SYNCE = 0;//1;
    EPwm4Regs.DCACTL.bit.EVT1SRCSEL = 0;

    EPwm4Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;
    EPwm4Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;

    EPwm4Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;

    EPwm4Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;
    EPwm4Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm4Regs.TZSEL.bit.DCAEVT1 = 1;
    EPwm4Regs.TZSEL.bit.OSHT2 = 1;

    // DCAEVT2 as CBC
    EPwm4Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;          // DCAEVT2 Source Is DCAEVT1 Signal
    EPwm4Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT2 Force Synchronization Signal Select: Asynchronous Signal

    EPwm4Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCAH_LOW;        // Digital Compare Output A Event 2 Selection:
                                                        // DCAH = low, DCAL = don't care
    EPwm4Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;  // Digital Compare A Low Input Select: COMP2OUT input
    EPwm4Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
                                                        // TZ1,TZ3 ASLO OK as DCAL don't care

    EPwm4Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCAEVT2 as one-shot-trip source for this ePWM module

#ifndef CUT_DOWN_VER_HARDWARE
    EPwm4Regs.TZCTL.all = 0x0005;
#else
    EPwm4Regs.TZCTL.all = 0x00A0;
#endif

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    /////////////PWM5//////////////
    //Set the Time-Base (TB) Module
    EPwm5Regs.TBPRD = C_INIT_PRD;
    EPwm5Regs.TBPHS.all = 0;
    EPwm5Regs.TBCTL.all = 0x2006;

    EPwm5Regs.CMPA.half.CMPA = C_INIT_PRD/2;
    EPwm5Regs.CMPB = C_INIT_PRD/2;

    EPwm5Regs.ETSEL.all = 0xFC00;   //CPMA上升沿触发\CPMB下降沿触发
    EPwm5Regs.ETPS.all = 0x1100;

    // 开下溢中断
    EPwm2Regs.ETCLR.bit.INT = 1;
    EPwm2Regs.ETSEL.all = 0x0009;
#else
    EPwm2Regs.ETCLR.bit.INT         = 1;        // 首先清除中断标志
    EPwm2Regs.ETSEL.bit.SOCAEN      = 1;        // Enable SOCA
    EPwm2Regs.ETSEL.bit.SOCASEL     = 1;        // 波谷触发...
    EPwm2Regs.ETSEL.bit.INTEN       = 1;
    EPwm2Regs.ETSEL.bit.INTSEL      = 1;
#endif

	EDIS;
}

void SCI_Setup(void)
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up for GPIO29 (SCITXDA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;   // Configure GPIO28 for SCIRXDA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;   // Configure GPIO29 for SCITXDA operation
    EDIS;

	// SCI Register setup... ...
    // 0000 0111
	SciaRegs.SCICCR.all =0x0007;   			// 1 stop bit,  No loopback
											// No parity,8 char bits,
											// async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  			// enable TX, RX, internal SCICLK,
											// Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;

	// Baud rate, 60MHZ/4 = 15MHz
	// BRR		Baudrate
	// 780.25	2.4
	//////////////////////////////////
	SciaRegs.SCIHBAUD = 0x03;
	SciaRegs.SCILBAUD = 0x0C;

	SciaRegs.SCICTL1.all =0x0023;     		// Relinquish SCI from Reset
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}

void LIN_Setup(void)
{
      EALLOW;
#if 0 //035
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO22 (LIN TX)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;     // Enable pull-up for GPIO23 (LIN RX)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO23 (LINRXA)
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;   // Configure GPIO19 for LIN TX operation  (3-Enable,0-Disable)
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;
#else //036
    GpioCtrlRegs.GPAPUD.bit.GPIO14   = 0;  // Enable pull-up for GPIO18 (LIN TX)
    GpioCtrlRegs.GPAPUD.bit.GPIO15   = 0;  // Enable pull-up for GPIO19 (LIN RX)
    GpioCtrlRegs.GPAMUX1.bit.GPIO14  = 2;  // Configure GPIO18 for LIN TX operation (2-Enable,0-Disable)
    GpioCtrlRegs.GPAMUX1.bit.GPIO15  = 2;  // Configure GPIO19 for LIN RX operation (2-Enable,0-Disable)

    // GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;    // Asynch input GPIO19 (LINRXA)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15  = 2;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 5;      //130--not ok

    // FAN GPIO-LIN-ENABLE
//    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
//    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
//    GpioDataRegs.GPADAT.bit.GPIO1 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;
    GpioDataRegs.GPADAT.bit.GPIO17 = 1;
#endif
    EDIS;

    EALLOW;
    LinaRegs.SCIGCR0.bit.RESET = 0;       // Into reset
    LinaRegs.SCIGCR0.bit.RESET = 1;       // Out of reset

    LinaRegs.SCIGCR1.bit.SWnRST = 0;      // Into software reset

    LinaRegs.SCIGCR1.bit.CLK_MASTER = 0;  // Enable The node in slave mode
    LinaRegs.SCIGCR1.bit.LINMODE = 1;     // LIN mode is enabled
    LinaRegs.SCIGCR1.bit.COMMMODE = 0;    // ID4 and ID5 are not used for length control.
    LinaRegs.SCIGCR1.bit.PARITYENA = 0;   // No Parity Check, ID-parity verification is Unenabled
    LinaRegs.SCIGCR1.bit.ADAPT = 1;       // Automatic baudrate adjustment is disabled
    LinaRegs.SCIGCR1.bit.MBUFMODE = 1;    // Buffered Mode
    LinaRegs.SCIGCR1.bit.CTYPE = 1;       // LIN 2.0 Enhanced checksum used
    LinaRegs.SCIGCR1.bit.HGENCTRL = 1;    // ID filtering using ID-SlaveTask byte
    LinaRegs.SCIGCR1.bit.STOPEXTFRAME = 0;// Stop extended frame communication.
    LinaRegs.SCIGCR1.bit.LOOPBACK = 0;    // External Loopback 正常模式，非自测模式
    LinaRegs.SCIGCR1.bit.CONT = 1;        // Continue on Suspend
    LinaRegs.SCIGCR1.bit.RXENA = 1;       // Enable RX
    LinaRegs.SCIGCR1.bit.TXENA = 1;       // Enable TX

    LinaRegs.SCIGCR2.bit.CC = 1;          // compare checksum
    LinaRegs.SCIGCR2.bit.SC = 1;          // send checksum

    LinaRegs.SCIFORMAT.bit.LENGTH = 7;    // （LENGTH+1）个字节深度的BUFFER，
                                          // 意味着ADP32F035的 LIN-SCI接收到（LENGTH+1） 个字节之后，进入中断服务函数???

    // 从机自动识别主机波特率
    LinaRegs.BRSR.bit.SCI_LIN_PSL = 194;  // 9600 Baud Rate
    LinaRegs.BRSR.bit.M = 0;              // 11
    LinaRegs.MBRSR = 92;                  // 20kHz (max autobaud rate)


    LinaRegs.LINMASK.bit.RXIDMASK = 0xFF;
    LinaRegs.LINMASK.bit.TXIDMASK = 0xFF; // When HGENCTRL is set to 1, this field must be set to 0xFF

    LinaRegs.LINCOMP.bit.SBREAK = 0;      // TSYNBRK = 13Tbit + SBREAK x Tbit
    LinaRegs.LINCOMP.bit.SDEL = 0;        // TSDEL = (SDEL + 1)Tbit

    //Set interrupt priority
    LinaRegs.SCICLEARINTLVL.all = 0xFFFFFFFF;     // Set Int level of all interrupts to LVL 0 设置中断级别

    LinaRegs.SCISETINT.bit.SETRXINT = 1;          // Enable RX interrupt
    LinaRegs.SCISETINT.bit.SETIDINT = 1;          // Enable ID interrupt

    // LIN mode only. This bit is set when there has been an inconsistent Synch Field error detected by
    // the Synchronizer during Header reception. See section 1.5.2.3, "Header Reception and Adaptive
    // Baudrate" , for more information. The Inconsistent Synch Field Error flag is cleared by reading the
    // corresponding interrupt offset in the SCIINTVECT0/1 register,
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    LinaRegs.SCISETINT.bit.SETPEINT = 1;          // 24     Set Parity Interrupt
    LinaRegs.SCISETINT.bit.SETOEINT = 1;          // 25     Set Overrun-Error Interrupt
    LinaRegs.SCISETINT.bit.SETFEINT = 1;          // 26     Set Framing-Error Interrupt
    LinaRegs.SCISETINT.bit.SETNREINT = 1;         // 27     Set No-Response-Error Interrupt (LIN only)
    LinaRegs.SCISETINT.bit.SETISFEINT = 1;        // 28     Set Inconsistent-Synch-Field-Error Interrupt (LIN only)
    LinaRegs.SCISETINT.bit.SETCEINT = 1;          // 29     Set Checksum-error Interrupt (LIN only)
    LinaRegs.SCISETINT.bit.SETPBEINT = 1;         // 30     Set Physical Bus Error Interrupt (LIN only)
    LinaRegs.SCISETINT.bit.SETBEINT = 1;          // 31     Set Bit Error Interrupt (LIN only)

    LinaRegs.SCISETINTLVL.bit.SETRXINTOVO = 0;
    LinaRegs.SCISETINTLVL.bit.SETIDINTLVL = 0;    // 清除中断级别

    LinaRegs.SCIGCR1.bit.SWnRST = 1;              // bring out of software reset
    EDIS;
}

void InitXint1Gpio(void)
{
    //Config GPIO17 As Xint1
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO9   = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO9 = 0;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 9;
    XIntruptRegs.XINT1CR.bit.POLARITY = 1;
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;
    EDIS;
}

/************************************************************
函数输入:无
调用位置:主循环之前
调用条件:无
函数功能:初始化DSP的AD采样模块
************************************************************/
extern void DSP28x_usDelay(Uint32 Count);
void InitSetAdc(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    EDIS;

    asm(" RPT #28 || NOP");                 // 延时
    asm(" RPT #28 || NOP");

    EALLOW;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    PagOpaRegs.PAGOPACTL.bit.OPAEN = 1;     // 内部运放配置
#else
    PagOpaRegs.PAGOPACTL.bit.OPAEN = 1;     // 内部运放配置

    PagOpaRegs.PAGOPACTL.bit.PGA1_GAIN = 4; // 16倍
    PagOpaRegs.PAGOPACTL.bit.PGA2_GAIN = 4;
    PagOpaRegs.PAGOPACTL.bit.PGA3_GAIN = 4;

    PagOpaRegs.PAGOPACTL.bit.PGA1EN = 1;
    PagOpaRegs.PAGOPACTL.bit.PGA2EN = 1;
    PagOpaRegs.PAGOPACTL.bit.PGA3EN = 1;
#endif
    EDIS;

    EALLOW;
    Comp1Regs.DACVAL.bit.DACVAL = OverCurPointInternal;
                                        // 硬件过流比较器配置   2.5V  1024->3V, about 685, 5*1.4142A = 7.1
                                        // 512 * (7.1 / 21) + 512 = 685
                                        // 实测680 OK，690 NOT OK！！！！
                                        // 如果设置10APEAK 过流点，则 10 / 21)*512+512 = 755
                                        // 如果设置10ARMS 过流点，则 10*1.4142 / 21)*512+512 = 856
                                        ///////////////////////////////////////////////////////////////
    Comp1Regs.COMPCTL.bit.SYNCSEL       = 0;  // Asynchronous version of Comparator output is passed
    Comp1Regs.COMPCTL.bit.CMPINV        = 1;  // Inverted output of comparator is passed
    Comp1Regs.COMPCTL.bit.COMPSOURCE    = 0;  // input of comparator connected to internal DAC
    Comp1Regs.COMPCTL.bit.QUALSEL       = 10; // Qualification Period for synchronized output of the comparator
    Comp1Regs.COMPCTL.bit.COMPDACEN     = 1;  // Comparator/DAC Enable

    Comp2Regs.DACVAL.bit.DACVAL = CBCCurPointInternal;
                                        // 硬件过流比较器配置   2.5V  1024->3V, about 685, 5*1.4142A = 7.1
                                        // 512 * (7.1 / 21) + 512 = 685
                                        // 实测680 OK，690 NOT OK！！！！
                                        // 如果设置10APEAK 过流点，则 10 / 21)*512+512 = 755
                                        // 如果设置10ARMS 过流点，则 10*1.4142 / 21)*512+512 = 856
                                        ///////////////////////////////////////////////////////////////
    Comp2Regs.COMPCTL.bit.SYNCSEL       = 0;  // Asynchronous version of Comparator output is passed
    Comp2Regs.COMPCTL.bit.CMPINV        = 1;  // Inverted output of comparator is passed
    Comp2Regs.COMPCTL.bit.COMPSOURCE    = 0;  // input of comparator connected to internal DAC
    Comp2Regs.COMPCTL.bit.QUALSEL       = 10; // Qualification Period for synchronized output of the comparator
    Comp2Regs.COMPCTL.bit.COMPDACEN     = 1;  // Comparator/DAC Enable
    EDIS;

    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power  ADC BG
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power  reference
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power  ADC
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
    EDIS;

    // asm(" RPT #28 || NOP");
    DSP28x_usDelay(20000L);                 // 1ms

    //----------- 以下开始设置ADC的控制寄存器、转换通道选择寄存器等 ------------------
    EALLOW;
    // AdcRegs.INTSEL1N2.all     = 0x0020;   // SOC0通道转换完成产生中断ADCINT1
    // AdcRegs.INTSEL1N2.all     = 0x0022;   // SOC2通道转换完成产生中断ADCINT1
    AdcRegs.ADCSAMPLEMODE.all = 0;           // 采样模式为0: 顺序采样(非同时采样)
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;     // ADCINT1 trips after AdcResults latch
    AdcRegs.ADCCTL1.bit.TEMPCONV    = 1;

    AdcRegs.ADCCTL2.bit.CLKDIV2EN = 1;
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    // SOCx Trigger Select
    // 036 -- 低压Demo 板
    // B1 -- VDC
    // B3 -- IDC
    // A3 -- ADC VSP
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCA;
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;//ADCTRIG_EPWM4_SOCB;
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC6CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC7CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;//ADC_TRIG_SOURCE;

    //Acquisition Pulse Size
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC6CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC7CTL.bit.ACQPS  = ACQPS_SYS_CLKS;

    //SOCx Channel Select
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S2;
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S2;
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S6;//ADC_PIN_S12;//ADC_PIN_S12;//13;//ADC_PIN_S0;

    // A2 FAN V1.1 Single-shunt
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S5;//ADC_PIN_S6; // Internal Temperature Sensor
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S7; // A1
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S5; // A2

    // A2 FAN V1.0 Two-shunt
    // AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S10; // Internal Temperature Sensor
    // AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S5; // A1
    // AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S7; // A2


    AdcRegs.ADCSOC6CTL.bit.CHSEL  = ADC_PIN_S5; // A0
    AdcRegs.ADCSOC7CTL.bit.CHSEL  = ADC_PIN_S5; // IBUS

    // following is 034 64PIN 配置
    //////////////////////////////
    //    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S6;
    //    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S6;
    //    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S4;
#else
    AdcRegs.INTSEL1N2.all     = 0x0024;

    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADCTRIG_EPWM4_SOCB;
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;

    //Acquisition Pulse Size
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_SYS_CLKS;

    // SOCx Channel Select
    // Dual Motor Demo
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S3; // IU
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S7; // IV
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S12;// ADC_PIN_S12;//13;//ADC_PIN_S0;
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S6; // vdc
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S5; // Internal Temperature Sensor
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S3; // A2
#endif

    ADC_CLEAR_INT_FLAG;

    EDIS;
}

//===========================================================================
// End of file.
//===========================================================================
