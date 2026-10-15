/***************************************************************************
 * HardwareSetup.c
 * ADM32F036 DSP系统初始化头文件
 * version : 1.0
 * Date    : 2025
   该文件提供对所有需要使用的外设初始化操作，主要包括：
    1、系统时钟初始化
    2、FLASH初始化
    4、PWM/ADC/LIN/SCI...
    5、所有GPIO口应用情况初始化
    6、
****************************************************************************/

#include "ADP32F03x_EPwm_defines.h"
#include "MotorInclude.h"
#include "SPIcomm.h"
#include "PreDriver.h"
#include "Parameter.h"
#include "PWMCommFunc.h"
#include "CommInterface.h"

#define Device_cal (void   (*)(void))0x3D7C80

/**修改FLASH寄存器的程序需要搬移到FLASH中执行**/
#pragma CODE_SECTION(InitFlash, "ramfuncs");
extern COPY_TABLE prginRAM;

#define LIN_EN_HIGH()       GpioDataRegs.GPASET.bit.GPIO9 = 1
#define LIN_EN_LOW()        GpioDataRegs.GPACLEAR.bit.GPIO9 = 1

#define LIN_EN_INIT()       {EALLOW;                             \
                             GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0; \
                             GpioCtrlRegs.GPADIR.bit.GPIO9  = 1; \
                             GpioDataRegs.GPASET.bit.GPIO9  = 1; \
                             EDIS;}

/***************************************************************************
  Private Functions
 ***************************************************************************/
static void InitFlash(void);
static void InitPieVectTable(void);
static void copy_prg(COPY_TABLE *tp);
static void DisableDog(void);
static void InitPll(Uint16 val);
static void InitPeripheralClocks(void);
static void InitPieCtrl(void);
static void InitSetGpio(void);
#if 0
static void SCI_Setup(void);
#endif
static void LIN_Setup(void);

/************************************************************
初始化DSP系统，包括初始化时钟和配置寄存器
************************************************************/
void InitSysCtrl()
{   
    DisableDog();			// Disable the watchdog

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

#ifndef LIN_UPDATE
   //TODO:由于BOOT跳转APP时关闭了总中断，LIN 中断子程序无法响应导致通信数据丢失。由于程序进入到
   //     APP初始化时主动将IFR清零，导致开启总中断时之前挂起的LIN中断请求无法响应.
   IFR = 0x0000;
#endif

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
#ifdef EXTINT_PWMGET
   PieVectTable.XINT1 = &Xint1_isr;                 //外部中断
#endif
   EDIS;
}

/************************************************************
开启已经使用的中断
************************************************************/
void SetInterruptEnable()
{
   IER |= (M_INT1 | M_INT2 | M_INT3 | M_INT5);	//  Enable interrupts:
   IER |= M_INT9;

   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;           //  ADC1INT
   PieCtrlRegs.PIEIER2.bit.INTx2 = 1;           // 	EPWM2_TZ
   PieCtrlRegs.PIEIER3.bit.INTx2 = 1;           // 	EPWM2_INT

   // Enable SCIRX, SCITX interrupt...
   PieCtrlRegs.PIEIER9.bit.INTx1 = 1;     		// PIE Group 9, INT1
   PieCtrlRegs.PIEIER9.bit.INTx2 = 1;     		// PIE Group 9, INT2
   PieCtrlRegs.PIEIER9.bit.INTx3 = 1;

#ifdef EXTINT_PWMGET
   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;           //Xint1_isr
#endif
}

/************************************************************
初始化DSP的所有外设，为电机控制和接口做准备
************************************************************/
void InitPeripherals(void)
{
   	InitSetGpio();

   	InitSetPWM();
   	InitSetAdc();

   	// Fix the initial power up false overcurrent fault, which occurs
   	// because the TZDC module is initialized before the comparator,
   	// causing a false overcurrent trip.
//   	ClearAllTZFlags();

#ifdef SPI_SCOPE_TEST
   	InitSpiIO();
   	InitSpi();
#endif

#if (ADM32F036_TYPE == ADM32F036_A3)
   	InitI2CIO();
   	PreDriverInit();
   	DriverClaerFault();
#endif

   	// Set up SCI function...
   	// SCI_Setup();
   	LIN_Setup();
   	InitCpuTimers();

#ifdef EXTINT_PWMGET
    PWMCommInit();
    StartCpuTimer0();                          //作为外部中断计数
#endif

    //ConfigCpuTimer(&CpuTimer0, DSP_CLOCK, 1000000L);//100MHz CPU, 1 millisecond Period
   	//StartCpuTimer0();					 //暂时没用的定时器中断
   	StartCpuTimer1();				     //作为主程序的时间基准
}

//---------------------------------------------------------------------------
// Move program from FLASH to RAM size < 65535 words
//---------------------------------------------------------------------------
extern MovePrgFrFlashToRam(int32 src_addr, int32 dst_addr, Uint16 size);
static void copy_prg(COPY_TABLE *tp)
{
	Uint16 size;
	COPY_RECORD *crp = &tp->recs[0];

	size = (Uint16)crp->size - 1;

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
static void DisableDog(void)
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
    // SysCtrlRegs.WDCR= 0x002A;    // 26ms
    // SysCtrlRegs.WDCR= 0x002B;    // 52
    // SysCtrlRegs.WDCR= 0x002C;    // 104
    // SysCtrlRegs.WDCR= 0x002D;    // 208
    SysCtrlRegs.WDCR= 0x002E;       // 419ms
    EDIS;
}

//---------------------------------------------------------------------------
// This function initializes the PLLCR register.
//---------------------------------------------------------------------------
static void InitPll(Uint16 val)
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
static void InitPeripheralClocks(void)
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
   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK  = 1;      // 4     Enable SYSCLKOUT to I2C-A
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
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;   // 8     Enable SYSCLKOUT to ECAP1
                                             // 13:9  reserved
   SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;   // 14    Enable SYSCLKOUT to EQEP1
                                             // 15    reserved

   // 0110 0111 0000 0001
   // SysCtrlRegs.PCLKCR3.all = 0x6701;
   SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK     = 1;
   SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK     = 1;
   SysCtrlRegs.PCLKCR3.bit.COMP3ENCLK     = 0;
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1;
   SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 0;
   SysCtrlRegs.PCLKCR3.bit.rsvd2          = 4;
   SysCtrlRegs.PCLKCR3.bit.CLA1ENCLK      = 1;
   EDIS;
}

//--------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
static void InitPieCtrl(void)
{
	Uint16 * m_Point1,* m_Point2;
	Uint16   m_Index;

    DINT;    
    
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	// Disable the PIE
	// Clear all PIEIER registers:
	m_Point1 = (Uint16 *)&PieCtrlRegs.PIEIER1.all;
	m_Point2 = (Uint16 *)&PieCtrlRegs.PIEIFR1.all;
	for(m_Index = 0;m_Index<12;m_Index++)
	{
		*(m_Point1++) = 0;
		*(m_Point2++) = 0;
	}
}	

/************************************************************
所有误操作的中断处理
************************************************************/
int16 gErrorIntCnt;
#pragma CODE_SECTION(rsvd_ISR, "ramfuncs");
interrupt void rsvd_ISR(void)
{
    gErrorIntCnt++;             //监控是否有错误进入中断的情况
    EALLOW;
    SysCtrlRegs.WDCR= 0x0028;
    EDIS;
    while(1);
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
static void InitSetGpio(void)
{
    EALLOW;

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
    GpioCtrlRegs.GPAQSEL1.all = 0x03000000L;
    GpioCtrlRegs.GPAQSEL2.all = 0x03000001L;
    GpioCtrlRegs.GPBQSEL1.all = 0x0000000FL;        // GPIO32-GPIO34 Synch to SYSCLKOUT

    // For test. GPIO32 is used for Current point trigger
    //////////////////////////////////////////
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0x0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;

#ifdef TESTCLK
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0x3;
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;
#endif
    EDIS;

    // LIN ENABLE
    // A3 -- GPIO09
    // A2 -- GPIO01
    LIN_EN_INIT();
}

#if 1
void InitSetPWM(void)
{
    EALLOW;

    /***********************************************************************
     * ePWM2
     **********************************************************************/
    /* Configure the Time-Base (TB) module. */
    EPwm2Regs.TBPRD                     = C_INIT_PRD;
    EPwm2Regs.TBPHS.half.TBPHSHR        = 0;
    EPwm2Regs.TBPHS.half.TBPHS          = 0;

#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm2Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UP;
#else
    EPwm2Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UPDOWN;
#endif
    EPwm2Regs.TBCTL.bit.PHSEN           = TB_DISABLE;
    EPwm2Regs.TBCTL.bit.PRDLD           = TB_SHADOW;
    EPwm2Regs.TBCTL.bit.SYNCOSEL        = TB_CTR_ZERO;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV       = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV          = TB_DIV1;
    EPwm2Regs.TBCTL.bit.PHSDIR          = TB_UP;
    EPwm2Regs.TBCTL.bit.FREE_SOFT       = 0;

    /*
     * Configure the Counter-Compare (CC) module.
     * For the original 0x0100 case, bit 8 is SHDWAFULL, which is a
     * read-only status bit. Do not write SHDWAFULL/SHDWBFULL here.
     */
    EPwm2Regs.CMPCTL.bit.LOADAMODE      = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE      = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.SHDWAMODE      = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE      = CC_SHADOW;

    EPwm2Regs.CMPA.half.CMPA            = C_INIT_PRD / 2;
    EPwm2Regs.CMPB                      = C_INIT_PRD / 2;

    /* Configure the Action-Qualifier (AQ) module for output A. */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: AQCTLA = 0x0810. */
    EPwm2Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CAU            = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD            = AQ_SET;
#else
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm2Regs.AQCTLA.bit.CAU            = AQ_SET;        /* Set on CMPA up-count event.   */
    EPwm2Regs.AQCTLA.bit.CBU            = AQ_CLEAR;      /* Clear on CMPB up-count event. */
#else
    /* Original value: AQCTLA = 0x0420. */
    EPwm2Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CAU            = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAD            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD            = AQ_CLEAR;
#endif
#endif
#else
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: AQCTLA = 0x0090. */
    EPwm2Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CAU            = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD            = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD            = AQ_NO_ACTION;
#else
    /* Original value: AQCTLA = 0x0060. */
    EPwm2Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CAU            = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAD            = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD            = AQ_NO_ACTION;
#endif
#endif

    /* Configure the Dead-Band (DB) module. */
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: DBCTL = 0x0007. */
    EPwm2Regs.DBCTL.bit.OUT_MODE        = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL          = DB_ACTV_LOC;
    EPwm2Regs.DBCTL.bit.IN_MODE         = DBA_ALL;
#else
    /* Original value: DBCTL = 0x000B. */
    EPwm2Regs.DBCTL.bit.OUT_MODE        = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL          = DB_ACTV_HIC;
    EPwm2Regs.DBCTL.bit.IN_MODE         = DBA_ALL;
#endif
    EPwm2Regs.DBCTL.bit.HALFCYCLE       = 0;    /* Original DBCTL bit15 = 0: full-cycle clocking. */

    EPwm2Regs.DBFED                     = DeadTimeInternal;
    EPwm2Regs.DBRED                     = DeadTimeInternal;

    /* Clear Trip-Zone interrupt, CBC, and one-shot flags. */
    EPwm2Regs.TZCLR.bit.INT             = 1;
    EPwm2Regs.TZCLR.bit.CBC             = 1;
    EPwm2Regs.TZCLR.bit.OST             = 1;

    /* Configure DCAEVT1 as a one-shot trip source. */
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm2Regs.TZDCSEL.bit.DCAEVT1       = TZ_DCAH_LOW;
    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;
    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;

    EPwm2Regs.TZSEL.bit.DCAEVT1         = 1;
    EPwm2Regs.TZSEL.bit.OSHT2           = 1;

    /* Configure DCBEVT2 as a cycle-by-cycle trip source. */
    EPwm2Regs.DCBCTL.bit.EVT2SRCSEL     = DC_EVT2;
    EPwm2Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm2Regs.TZDCSEL.bit.DCBEVT2       = TZ_DCBH_LOW;
    EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;
    EPwm2Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;

    EPwm2Regs.TZSEL.bit.DCBEVT2         = 1;

    /* Configure Trip-Zone output actions. */
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: TZCTL = 0x0005. */
    EPwm2Regs.TZCTL.bit.TZA             = TZ_FORCE_HI;
    EPwm2Regs.TZCTL.bit.TZB             = TZ_FORCE_HI;
    EPwm2Regs.TZCTL.bit.DCAEVT1         = TZ_HIZ;
    EPwm2Regs.TZCTL.bit.DCAEVT2         = TZ_HIZ;
    EPwm2Regs.TZCTL.bit.DCBEVT1         = TZ_HIZ;
    EPwm2Regs.TZCTL.bit.DCBEVT2         = TZ_HIZ;
#else
    /* Original value: TZCTL = 0x00A0. */
    EPwm2Regs.TZCTL.bit.TZA             = TZ_HIZ;
    EPwm2Regs.TZCTL.bit.TZB             = TZ_HIZ;
    EPwm2Regs.TZCTL.bit.DCAEVT1         = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.DCAEVT2         = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.DCBEVT1         = TZ_HIZ;
    EPwm2Regs.TZCTL.bit.DCBEVT2         = TZ_HIZ;
#endif

    /* Original value: TZEINT = 0x0004. */
    EPwm2Regs.TZEINT.bit.CBC            = 0;
    EPwm2Regs.TZEINT.bit.OST            = 1;
    EPwm2Regs.TZEINT.bit.DCAEVT1        = 0;
    EPwm2Regs.TZEINT.bit.DCAEVT2        = 0;
    EPwm2Regs.TZEINT.bit.DCBEVT1        = 0;
    EPwm2Regs.TZEINT.bit.DCBEVT2        = 0;

#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
    /*
     * Original value: ETPS = 0x0101.
     * INTCNT/SOCACNT/SOCBCNT are read-only counters and are not assigned.
     */
    EPwm2Regs.ETPS.bit.INTPRD           = ET_1ST;
    EPwm2Regs.ETPS.bit.SOCAPRD          = ET_1ST;
    EPwm2Regs.ETPS.bit.SOCBPRD          = ET_DISABLE;
#endif
    EPwm2Regs.ETPS.bit.INTPRD           = ET_1ST;

    /***********************************************************************
     * ePWM3
     **********************************************************************/
    /* Configure the Time-Base (TB) module. */
    EPwm3Regs.TBPRD                     = C_INIT_PRD;
    EPwm3Regs.TBPHS.half.TBPHSHR        = 0;
    EPwm3Regs.TBPHS.half.TBPHS          = 0;

#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm3Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UP;
#else
    EPwm3Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UPDOWN;
#endif
    EPwm3Regs.TBCTL.bit.PHSEN           = TB_ENABLE;
    EPwm3Regs.TBCTL.bit.PRDLD           = TB_SHADOW;
    EPwm3Regs.TBCTL.bit.SYNCOSEL        = TB_SYNC_IN;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV       = TB_DIV1;
    EPwm3Regs.TBCTL.bit.CLKDIV          = TB_DIV1;
    EPwm3Regs.TBCTL.bit.PHSDIR          = TB_UP;
    EPwm3Regs.TBCTL.bit.FREE_SOFT       = 0;

    /*
     * Configure the Counter-Compare (CC) module.
     * For the original 0x0100 case, bit 8 is SHDWAFULL, which is a
     * read-only status bit. Do not write SHDWAFULL/SHDWBFULL here.
     */
    EPwm3Regs.CMPCTL.bit.LOADAMODE      = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE      = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.SHDWAMODE      = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE      = CC_SHADOW;

    EPwm3Regs.CMPA.half.CMPA            = C_INIT_PRD / 2;

    /* Configure the Action-Qualifier (AQ) module for output A. */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: AQCTLA = 0x0810. */
    EPwm3Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAU            = AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.CAD            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD            = AQ_SET;
#else
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm3Regs.AQCTLA.bit.CAU            = AQ_SET;        /* Set on CMPA up-count event.   */
    EPwm3Regs.AQCTLA.bit.CBU            = AQ_CLEAR;      /* Clear on CMPB up-count event. */
#else
    /* Original value: AQCTLA = 0x0420. */
    EPwm3Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAU            = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CAD            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD            = AQ_CLEAR;
#endif
#endif
#else
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: AQCTLA = 0x0090. */
    EPwm3Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAU            = AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.CAD            = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD            = AQ_NO_ACTION;
#else
    /* Original value: AQCTLA = 0x0060. */
    EPwm3Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAU            = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CAD            = AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD            = AQ_NO_ACTION;
#endif
#endif

    /* Configure the Dead-Band (DB) module. */
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: DBCTL = 0x0007. */
    EPwm3Regs.DBCTL.bit.OUT_MODE        = DB_FULL_ENABLE;
    EPwm3Regs.DBCTL.bit.POLSEL          = DB_ACTV_LOC;
    EPwm3Regs.DBCTL.bit.IN_MODE         = DBA_ALL;
#else
    /* Original value: DBCTL = 0x000B. */
    EPwm3Regs.DBCTL.bit.OUT_MODE        = DB_FULL_ENABLE;
    EPwm3Regs.DBCTL.bit.POLSEL          = DB_ACTV_HIC;
    EPwm3Regs.DBCTL.bit.IN_MODE         = DBA_ALL;
#endif
    EPwm3Regs.DBCTL.bit.HALFCYCLE       = 0;    /* Original DBCTL bit15 = 0: full-cycle clocking. */

    EPwm3Regs.DBFED                     = DeadTimeInternal;
    EPwm3Regs.DBRED                     = DeadTimeInternal;

    /* Clear Trip-Zone interrupt, CBC, and one-shot flags. */
    EPwm3Regs.TZCLR.bit.INT             = 1;
    EPwm3Regs.TZCLR.bit.CBC             = 1;
    EPwm3Regs.TZCLR.bit.OST             = 1;

    /* Configure DCAEVT1 as a one-shot trip source. */
    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;
    EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;

    EPwm3Regs.TZDCSEL.bit.DCAEVT1       = TZ_DCAH_LOW;

    EPwm3Regs.DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm3Regs.TZSEL.bit.DCAEVT1         = 1;
    EPwm3Regs.TZSEL.bit.OSHT2           = 1;

    /* Configure DCBEVT2 as a cycle-by-cycle trip source. */
    EPwm3Regs.DCBCTL.bit.EVT2SRCSEL     = DC_EVT2;
    EPwm3Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm3Regs.TZDCSEL.bit.DCBEVT2       = TZ_DCAH_LOW;   /* Preserve original selection value. */
    EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;
    EPwm3Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;

    EPwm3Regs.TZSEL.bit.DCBEVT2         = 1;

    /* Configure Trip-Zone output actions. */
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: TZCTL = 0x0005. */
    EPwm3Regs.TZCTL.bit.TZA             = TZ_FORCE_HI;
    EPwm3Regs.TZCTL.bit.TZB             = TZ_FORCE_HI;
    EPwm3Regs.TZCTL.bit.DCAEVT1         = TZ_HIZ;
    EPwm3Regs.TZCTL.bit.DCAEVT2         = TZ_HIZ;
    EPwm3Regs.TZCTL.bit.DCBEVT1         = TZ_HIZ;
    EPwm3Regs.TZCTL.bit.DCBEVT2         = TZ_HIZ;
#else
    /* Original value: TZCTL = 0x00A0. */
    EPwm3Regs.TZCTL.bit.TZA             = TZ_HIZ;
    EPwm3Regs.TZCTL.bit.TZB             = TZ_HIZ;
    EPwm3Regs.TZCTL.bit.DCAEVT1         = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.DCAEVT2         = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.DCBEVT1         = TZ_HIZ;
    EPwm3Regs.TZCTL.bit.DCBEVT2         = TZ_HIZ;
#endif

    /***********************************************************************
     * ePWM4
     **********************************************************************/
    /* Configure the Time-Base (TB) module. */
    EPwm4Regs.TBPRD                     = C_INIT_PRD;
    EPwm4Regs.TBPHS.half.TBPHSHR        = 0;
    EPwm4Regs.TBPHS.half.TBPHS          = 0;

    /* Original values: TBCTL = 0x2004 / 0x2006. */
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm4Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UP;
#else
    EPwm4Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UPDOWN;
#endif
    EPwm4Regs.TBCTL.bit.PHSEN           = TB_ENABLE;
    EPwm4Regs.TBCTL.bit.PRDLD           = TB_SHADOW;
    EPwm4Regs.TBCTL.bit.SYNCOSEL        = TB_SYNC_IN;
    EPwm4Regs.TBCTL.bit.SWFSYNC         = 0;
    EPwm4Regs.TBCTL.bit.HSPCLKDIV       = TB_DIV1;
    EPwm4Regs.TBCTL.bit.CLKDIV          = TB_DIV1;
    EPwm4Regs.TBCTL.bit.PHSDIR          = TB_UP;
    EPwm4Regs.TBCTL.bit.FREE_SOFT       = 0;

    /*
     * Configure the Counter-Compare (CC) module.
     * For the original 0x0100 case, bit 8 is SHDWAFULL, which is a
     * read-only status bit. Do not write SHDWAFULL/SHDWBFULL here.
     */
    EPwm4Regs.CMPCTL.bit.LOADAMODE      = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.LOADBMODE      = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.SHDWAMODE      = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE      = CC_SHADOW;

    EPwm4Regs.CMPA.half.CMPA            = C_INIT_PRD / 2;

    /* Configure the Action-Qualifier (AQ) module for output A. */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: AQCTLA = 0x0810. */
    EPwm4Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CAU            = AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CAD            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBD            = AQ_SET;
#else
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm4Regs.AQCTLA.bit.CAU            = AQ_SET;        /* Set on CMPA up-count event.   */
    EPwm4Regs.AQCTLA.bit.CBU            = AQ_CLEAR;      /* Clear on CMPB up-count event. */
#else
    /* Original value: AQCTLA = 0x0420. */
    EPwm4Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CAU            = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CAD            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBD            = AQ_CLEAR;
#endif
#endif
#else
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: AQCTLA = 0x0090. */
    EPwm4Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CAU            = AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CAD            = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBD            = AQ_NO_ACTION;
#else
    /* Original value: AQCTLA = 0x0060. */
    EPwm4Regs.AQCTLA.bit.ZRO            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.PRD            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CAU            = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CAD            = AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CBU            = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBD            = AQ_NO_ACTION;
#endif
#endif

    /* Configure the Dead-Band (DB) module. */
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: DBCTL = 0x0007. */
    EPwm4Regs.DBCTL.bit.OUT_MODE        = DB_FULL_ENABLE;
    EPwm4Regs.DBCTL.bit.POLSEL          = DB_ACTV_LOC;
    EPwm4Regs.DBCTL.bit.IN_MODE         = DBA_ALL;
#else
    /* Original value: DBCTL = 0x000B. */
    EPwm4Regs.DBCTL.bit.OUT_MODE        = DB_FULL_ENABLE;
    EPwm4Regs.DBCTL.bit.POLSEL          = DB_ACTV_HIC;
    EPwm4Regs.DBCTL.bit.IN_MODE         = DBA_ALL;
#endif
    EPwm4Regs.DBCTL.bit.HALFCYCLE       = 0;    /* Original DBCTL bit15 = 0: full-cycle clocking. */

    EPwm4Regs.DBFED                     = DeadTimeInternal;
    EPwm4Regs.DBRED                     = DeadTimeInternal;

    /* Clear Trip-Zone interrupt, CBC, and one-shot flags. */
    EPwm4Regs.TZCLR.bit.INT             = 1;
    EPwm4Regs.TZCLR.bit.CBC             = 1;
    EPwm4Regs.TZCLR.bit.OST             = 1;

    /* Configure DCAEVT1 as a one-shot trip source. */
    EPwm4Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;
    EPwm4Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;

    EPwm4Regs.TZDCSEL.bit.DCAEVT1       = TZ_DCAH_LOW;

    EPwm4Regs.DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm4Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm4Regs.TZSEL.bit.DCAEVT1         = 1;
    EPwm4Regs.TZSEL.bit.OSHT2           = 1;

    /* Configure DCBEVT2 as a cycle-by-cycle trip source. */
    EPwm4Regs.DCBCTL.bit.EVT2SRCSEL     = DC_EVT2;
    EPwm4Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm4Regs.TZDCSEL.bit.DCBEVT2       = TZ_DCAH_LOW;   /* Preserve original selection value. */
    EPwm4Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;
    EPwm4Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;

    EPwm4Regs.TZSEL.bit.DCBEVT2         = 1;

    /* Configure Trip-Zone output actions. */
#ifndef POSTIVE_DRIVE_LOGIC
    /* Original value: TZCTL = 0x0005. */
    EPwm4Regs.TZCTL.bit.TZA             = TZ_FORCE_HI;
    EPwm4Regs.TZCTL.bit.TZB             = TZ_FORCE_HI;
    EPwm4Regs.TZCTL.bit.DCAEVT1         = TZ_HIZ;
    EPwm4Regs.TZCTL.bit.DCAEVT2         = TZ_HIZ;
    EPwm4Regs.TZCTL.bit.DCBEVT1         = TZ_HIZ;
    EPwm4Regs.TZCTL.bit.DCBEVT2         = TZ_HIZ;
#else
    /* Original value: TZCTL = 0x00A0. */
    EPwm4Regs.TZCTL.bit.TZA             = TZ_HIZ;
    EPwm4Regs.TZCTL.bit.TZB             = TZ_HIZ;
    EPwm4Regs.TZCTL.bit.DCAEVT1         = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.DCAEVT2         = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.DCBEVT1         = TZ_HIZ;
    EPwm4Regs.TZCTL.bit.DCBEVT2         = TZ_HIZ;
#endif

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    /***********************************************************************
     * ePWM5
     **********************************************************************/
    /* Configure the Time-Base (TB) module. */
    EPwm5Regs.TBPRD                     = C_INIT_PRD;
    EPwm5Regs.TBPHS.half.TBPHSHR        = 0;
    EPwm5Regs.TBPHS.half.TBPHS          = 0;

    /* Original values: TBCTL = 0x2004 / 0x2006. */
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm5Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UP;
#else
    EPwm5Regs.TBCTL.bit.CTRMODE         = TB_COUNT_UPDOWN;
#endif
    EPwm5Regs.TBCTL.bit.PHSEN           = TB_ENABLE;
    EPwm5Regs.TBCTL.bit.PRDLD           = TB_SHADOW;
    EPwm5Regs.TBCTL.bit.SYNCOSEL        = TB_SYNC_IN;
    EPwm5Regs.TBCTL.bit.SWFSYNC         = 0;
    EPwm5Regs.TBCTL.bit.HSPCLKDIV       = TB_DIV1;
    EPwm5Regs.TBCTL.bit.CLKDIV          = TB_DIV1;
    EPwm5Regs.TBCTL.bit.PHSDIR          = TB_UP;
    EPwm5Regs.TBCTL.bit.FREE_SOFT       = 0;

    EPwm5Regs.CMPA.half.CMPA            = C_INIT_PRD / 2;
    EPwm5Regs.CMPB                      = C_INIT_PRD / 2;

    /* Configure SOCA/SOCB trigger sources for single-shunt sampling. */
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    /* Original value: ETSEL = 0xEC00. */
    EPwm5Regs.ETSEL.bit.INTSEL          = 0;
    EPwm5Regs.ETSEL.bit.INTEN           = 0;
    EPwm5Regs.ETSEL.bit.SOCASEL         = ET_CTRU_CMPA;
    EPwm5Regs.ETSEL.bit.SOCAEN          = 1;
    EPwm5Regs.ETSEL.bit.SOCBSEL         = ET_CTRU_CMPB;
    EPwm5Regs.ETSEL.bit.SOCBEN          = 1;
#else
    /* Original value: ETSEL = 0xFC00. */
    EPwm5Regs.ETSEL.bit.INTSEL          = 0;
    EPwm5Regs.ETSEL.bit.INTEN           = 0;
    EPwm5Regs.ETSEL.bit.SOCASEL         = ET_CTRU_CMPA;
    EPwm5Regs.ETSEL.bit.SOCAEN          = 1;
    EPwm5Regs.ETSEL.bit.SOCBSEL         = ET_CTRD_CMPB;
    EPwm5Regs.ETSEL.bit.SOCBEN          = 1;
#endif

    /*
     * Original value: ETPS = 0x1100.
     * INTCNT/SOCACNT/SOCBCNT are read-only counters and are not assigned.
     */
    EPwm5Regs.ETPS.bit.INTPRD           = ET_DISABLE;
    EPwm5Regs.ETPS.bit.SOCAPRD          = ET_1ST;
    EPwm5Regs.ETPS.bit.SOCBPRD          = ET_1ST;

    /* Clear the ePWM2 interrupt flag and enable ePWM2 interrupt only. */
    EPwm2Regs.ETCLR.bit.INT             = 1;

    /* Original value: EPwm2 ETSEL = 0x0009. */
    EPwm2Regs.ETSEL.bit.INTSEL          = ET_CTR_ZERO;
    EPwm2Regs.ETSEL.bit.INTEN           = 1;
    EPwm2Regs.ETSEL.bit.SOCASEL         = 0;
    EPwm2Regs.ETSEL.bit.SOCAEN          = 0;
    EPwm2Regs.ETSEL.bit.SOCBSEL         = 0;
    EPwm2Regs.ETSEL.bit.SOCBEN          = 0;
#else
    /* Clear the ePWM2 interrupt flag and enable SOCA/interrupt generation. */
    EPwm2Regs.ETCLR.bit.INT             = 1;
    EPwm2Regs.ETSEL.bit.SOCAEN          = 1;
    EPwm2Regs.ETSEL.bit.SOCASEL         = ET_CTR_ZERO;
    EPwm2Regs.ETSEL.bit.INTEN           = 1;
    EPwm2Regs.ETSEL.bit.INTSEL          = ET_CTR_ZERO;
#endif

    EDIS;
}
#else
//---------------------------------------------------------------------------
// Set EPWM for motor Control
//---------------------------------------------------------------------------
void InitSetPWM(void)
{
	EALLOW;

	/////////////PWM2//////////////
	//Set the Time-Base (TB) Module
	EPwm2Regs.TBPRD = C_INIT_PRD;

	EPwm2Regs.TBPHS.all = 0;
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
	EPwm2Regs.TBCTL.all = 0x2010;
#else
	EPwm2Regs.TBCTL.all = 0x2012;
#endif

	EPwm2Regs.CMPCTL.all = 0x0000;
	EPwm2Regs.CMPA.half.CMPA = C_INIT_PRD/2;
	EPwm2Regs.CMPB = C_INIT_PRD/2;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef POSTIVE_DRIVE_LOGIC
	EPwm2Regs.AQCTLA.all = 0x0810;//0x0090;
#else
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm2Regs.AQCTLA.bit.CAU = 2;   // UP equal HIGH  //adjust
    EPwm2Regs.AQCTLA.bit.CBU = 1;   // up equal LOW   //adjust
#else
    EPwm2Regs.AQCTLA.all = 0x0420;//0x0060;
#endif
#endif
#else
#ifndef POSTIVE_DRIVE_LOGIC
    EPwm2Regs.AQCTLA.all = 0x0090;
#else
    EPwm2Regs.AQCTLA.all = 0x0060;
#endif
#endif

#ifndef POSTIVE_DRIVE_LOGIC
	EPwm2Regs.DBCTL.all = 0x0007;
#else
	EPwm2Regs.DBCTL.all = 0x000B;
#endif

	EPwm2Regs.DBFED = DeadTimeInternal;
	EPwm2Regs.DBRED = DeadTimeInternal;

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

    EPwm2Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCBEVT2 as Cycle-by-Cycle (CBC) trip source

#ifndef POSTIVE_DRIVE_LOGIC
	EPwm2Regs.TZCTL.all = 0x0005;
#else
	EPwm2Regs.TZCTL.all = 0x00A0;
#endif

	EPwm2Regs.TZEINT.all = 0x0004;

#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
	EPwm2Regs.ETPS.all = 0x0101;
#endif
    EPwm2Regs.ETPS.bit.INTPRD = 1;

	/////////////PWM3//////////////
	//Set the Time-Base (TB) Module
	EPwm3Regs.TBPRD = C_INIT_PRD;
	EPwm3Regs.TBPHS.all = 0;

#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
	EPwm3Regs.TBCTL.all = 0x2004;
#else
	EPwm3Regs.TBCTL.all = 0x2006;
#endif

    EPwm3Regs.CMPCTL.all = 0x0000;
	EPwm3Regs.CMPA.half.CMPA = C_INIT_PRD/2;

	//Set the Action-qualifier (AQ) Module
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef POSTIVE_DRIVE_LOGIC
    EPwm3Regs.AQCTLA.all = 0x0810;//0x0090;
#else
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm3Regs.AQCTLA.bit.CAU = 2;   // UP equal HIGH  //adjust
    EPwm3Regs.AQCTLA.bit.CBU = 1;   // up equal LOW   //adjust
#else
    EPwm3Regs.AQCTLA.all = 0x0420;//0x0060;
#endif
#endif
#else
#ifndef POSTIVE_DRIVE_LOGIC
    EPwm3Regs.AQCTLA.all = 0x0090;
#else
    EPwm3Regs.AQCTLA.all = 0x0060;
#endif
#endif

#ifndef POSTIVE_DRIVE_LOGIC
	EPwm3Regs.DBCTL.all = 0x0007;
#else
	EPwm3Regs.DBCTL.all = 0x000B;
#endif

	EPwm3Regs.DBFED = DeadTimeInternal;
	EPwm3Regs.DBRED = DeadTimeInternal;

    // EPwm3Regs.DCACTL.bit.EVT1SRCSEL = 0;
	EPwm3Regs.TZCLR.all = 0x07;

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

    EPwm3Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCBEVT2 as Cycle-by-Cycle (CBC) trip source

#ifndef POSTIVE_DRIVE_LOGIC
    EPwm3Regs.TZCTL.all = 0x0005;
#else
    EPwm3Regs.TZCTL.all = 0x00A0;
#endif

	/////////////PWM4//////////////
	//Set the Time-Base (TB) Module
	EPwm4Regs.TBPRD = C_INIT_PRD;
	EPwm4Regs.TBPHS.all = 0;

#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm4Regs.TBCTL.all = 0x2004;
#else
    EPwm4Regs.TBCTL.all = 0x2006;
#endif

    EPwm4Regs.CMPCTL.all = 0x0000;
	EPwm4Regs.CMPA.half.CMPA = C_INIT_PRD/2;

	//Set the Action-qualifier (AQ) Module
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef POSTIVE_DRIVE_LOGIC
    EPwm4Regs.AQCTLA.all = 0x0810;//0x0090;
#else
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm4Regs.AQCTLA.bit.CAU = 2;   // UP equal HIGH  //adjust
    EPwm4Regs.AQCTLA.bit.CBU = 1;   // up equal LOW   //adjust
#else
    EPwm4Regs.AQCTLA.all = 0x0420;//0x0060;
#endif
#endif
#else
#ifndef POSTIVE_DRIVE_LOGIC
    EPwm4Regs.AQCTLA.all = 0x0090;
#else
    EPwm4Regs.AQCTLA.all = 0x0060;
#endif
#endif

#ifndef POSTIVE_DRIVE_LOGIC
	EPwm4Regs.DBCTL.all = 0x0007;
#else
	EPwm4Regs.DBCTL.all = 0x000B;
#endif

	EPwm4Regs.DBFED = DeadTimeInternal;
	EPwm4Regs.DBRED = DeadTimeInternal;//C_MAX_DB;

    // DCAEVT1 as one-shot
    // EPwm4Regs.DCACTL.bit.EVT1SRCSEL = 0;
	EPwm4Regs.TZCLR.all = 0x07;

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

    EPwm4Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCBEVT2 as Cycle-by-Cycle (CBC) trip source

#ifndef POSTIVE_DRIVE_LOGIC
    EPwm4Regs.TZCTL.all = 0x0005;
#else
    EPwm4Regs.TZCTL.all = 0x00A0;
#endif

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    /////////////PWM5//////////////
    //Set the Time-Base (TB) Module
    EPwm5Regs.TBPRD = C_INIT_PRD;
    EPwm5Regs.TBPHS.all = 0;

#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm5Regs.TBCTL.all = 0x2004;
#else
    EPwm5Regs.TBCTL.all = 0x2006;
#endif

    EPwm5Regs.CMPA.half.CMPA = C_INIT_PRD/2;
    EPwm5Regs.CMPB = C_INIT_PRD/2;

#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    EPwm5Regs.ETSEL.all = 0xEC00;   //CPMA, CPMB上升沿触发
#else
    EPwm5Regs.ETSEL.all = 0xFC00;   //CPMA上升沿触发\CPMB下降沿触发
#endif
    // EPwm5Regs.ETSEL.all = 0xFC00;   //CPMA上升沿触发\CPMB下降沿触发
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
#endif

#if 0
static void SCI_Setup(void)
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
#endif

static void LIN_Setup(void)
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

/************************************************************
函数输入:无
调用位置:主循环之前
调用条件:无
函数功能:初始化DSP的AD采样模块
************************************************************/
extern void ADP32F03x_usDelay(Uint32 Count);
void InitSetAdc(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    EDIS;

    // asm(" RPT #100 || NOP");              // 延时1us, 可以删除

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
    ADP32F03x_usDelay(20000L);                 // 20ms

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

    // SOCx Channel Select
    // A3_BLDC_V25/11/11
    // S9,S9,S6
    // A3. FAN_V1.1
    //     UVW---A3A1A0
    //     IDC---A2
    //     VDC---B4
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S2;//9;//2;
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S2;//9;//2;
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S12;//6;//12;//ADC_PIN_S12;//13;//ADC_PIN_S0;

//    // Following 9 9 6 is BLDC A3 DEMO
//    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S9;
//    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S9;
//    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S6;

    // A3 FAN V1.1 two-shunt
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S10;// w
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S14;// v
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S4; // U

    // A2 FAN V1.1 Single-shunt
    // AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S6; // Internal Temperature Sensor
    // AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S7; // A1
    // AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S5; // A2

    // A2 FAN V1.0 Two-shunt
    // AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S10; // Internal Temperature Sensor
    // AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S5;  // A1
    // AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S7;  // A2

    // A3 BLDC A3 DEMO
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S10; // B2
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S4;  // A4
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S12; // B4

    AdcRegs.ADCSOC6CTL.bit.CHSEL  = ADC_PIN_S5;     // A5
    AdcRegs.ADCSOC7CTL.bit.CHSEL  = ADC_PIN_S4;     // IBUS

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
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S0; // IV
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S6; // ADC_PIN_S12;//13;//ADC_PIN_S0;

    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S10; // U
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S4;  // V
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S12; // W
#endif

    ADC_CLEAR_INT_FLAG;

    EDIS;
}

void ResetLINAndPreDrive(void)
{
    LIN_EN_INIT();
    PreDriverInit();
    DriverClaerFault();
}
#pragma CODE_SECTION(EnterSleepMode, "ramfuncs");
void EnterSleepMode(void)
{
    // 注意：一般逻辑是接收到休眠唤醒命令后要休眠，进来此函数之前已经要停机！
    // 确保外面没有停机此处再加上！
    DisableDrive();

    DINT;

    LIN_EN_LOW();    /* LEN_EN = 0; */

    asm(" RPT #50 || NOP");

    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;   /* TX = 0;  */
    EDIS;

    ADP32F03x_usDelay(20000L);              /* 2.5ms    */

    EALLOW;
    SysCtrlRegs.WDCR = 0x0000;
    EDIS;

    /* 防止看门狗复位不成功，程序继续运行 */
    //LIN_EN_HIGH();      /* LEN_EN = 1; */
}

//===========================================================================
// End of file.
//===========================================================================
