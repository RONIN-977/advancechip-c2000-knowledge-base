
 #include <HeardFiles/ADP32F03xHeader/ADP32F03x_EPwm_defines.h>
#include "MotorInclude.h"
#include "CANcomm.h"
#include "SPIcomm.h"
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
#ifndef INTER_OSC
	InitPll(DSP_CLOCK/20);		// Change 20MHz clock to 100MHz or 60MHz
#else
	InitPll(DSP_CLOCK/10);
#endif

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

   PieVectTable.ADCINT1     = &EndOfADCISR;		//ADC结束中断
   PieVectTable.EPWM2_TZINT = &OneShotTZOfEPWMISR;		//过流中断
   PieVectTable.EPWM2_INT 	= &ZeroOfEPWMISR;		//下溢中断

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
}

/************************************************************
初始化DSP的所有外设，为电机控制和接口做准备
************************************************************/
void InitPeripherals(void)
{
    InitSetGpio();
    //InitSpiIO();
    InitSetPWM();
    InitSetAdc();
    CANInit();
    //InitSpi();
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
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;      // configure PLLSTS.DIVSEL = 2  (default)
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

   //SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    	// ADC
   //SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;   	// I2C

   //SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;     // SPI-A
   //SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 0;     // SPI-B
   //SysCtrlRegs.PCLKCR0.bit.SPICENCLK = 0;     // SPI-C
   //SysCtrlRegs.PCLKCR0.bit.SPIDENCLK = 0;     // SPI-D

   //SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;     // SCI-A
   //SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0;     // SCI-B

   //SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0;    // eCAN-A
   //SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;    // eCAN-B   

   //SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // Enable TBCLK within the ePWM

   // 2802x 变频冰箱板，禁止I2C,禁止SPI... ...
   // 0000 0100 0000 1100
   SysCtrlRegs.PCLKCR0.all = 0x451C;
   //SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;    // eCAN-A
   //SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;  // eCAP1
   //SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;  // eCAP2
   //SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;  // eCAP3
   //SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;  // eCAP4
   
   //SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;  // ePWM1
   //SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;  // ePWM2
   //SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;  // ePWM3
   //SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;  // ePWM4
   //SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;  // ePWM5
   //SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;  // ePWM6
  
   //SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;  // eQEP1
   //SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;  // eQEP2

   // 0000 0001 0000 0111
   SysCtrlRegs.PCLKCR1.all = 0x410F;
   SysCtrlRegs.PCLKCR3.all = 0x6706;          // COMP2 AND COMP3 0110

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
	// GPIO口上拉选择, 以前是Relay 是高电平有效，其他全部上拉...
	// PWM1~6禁止上拉, EPWM2, EPWM3,EPWM4....
	GpioCtrlRegs.GPAPUD.all = 0x000000FCL;
	GpioCtrlRegs.GPBPUD.all = 0x0;			 
		
	// STEP2:
	// GPIO口功能选择：设置使用的外设
	// GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO0 as EPWM1A
	// GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO1 as EPWM1B
	// GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
	// GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
	// GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
	// GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
	// GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1(OC)
	// GpioCtrlRegs.GPAMUX1.bit.GPIO28 = 1;  // Configure GPIO28 as TZ2(OC)
	// EPWM默认设置为 GIPIO 口。因为EnableDrive 里面会设置...
	GpioCtrlRegs.GPAMUX1.all = 0x00000000L;

	//GPAMUX2, SPI SMO
	GpioCtrlRegs.GPAMUX2.all = 0x00030000L;

	// STEP3:
	// GPIO数据先初始化为需要的值
	// GPIO6     Realy   O
    // GPIO16    HALL    I
    // GPIO17    HALL    I
    // GPIO18    LED     O
    // GPIO30    Enable  O
    // GPIO31    Save    I
    // GPIO34    HALL    I
	//
#ifdef POSTIVE_DRIVE_LOGIC
    GpioDataRegs.GPADAT.all = 0x00000000L;
#else
    GpioDataRegs.GPADAT.all = 0x0000003FL;
#endif
	GpioDataRegs.GPBDAT.all = 0x00000000L;
	
	// GPIO口方向选择1：复位为输入
    //
	GpioCtrlRegs.GPADIR.all = 0x00000000L;
    GpioCtrlRegs.GPADIR.bit.GPIO2  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO3  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO4  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO5  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO7  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;  //RELAY
	GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;  //RELAY


	GpioCtrlRegs.GPBDIR.all = 0x00;

	// STEP4:
	// GPIO口同步选择
	// 配置为异步的IO有：
	// 		TZ1 GPIO12       --- 逐波限流中断接口
	//      TZ2 GPIO28       --- 过流中断接口CBC
	// 配置为3 sample的有
	//
	// 其他均为默认
	// GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;      
	// GpioCtrlRegs.GPAQSEL1.bit.GPIO28 = 3;      

	GpioCtrlRegs.GPAQSEL1.all = 0x00000000L;
	GpioCtrlRegs.GPAQSEL2.all = 0x03000000L;
	GpioCtrlRegs.GPBQSEL1.all = 0x0000000FL;    	// GPIO32-GPIO34 Synch to SYSCLKOUT

	GpioCtrlRegs.GPADIR.bit.GPIO9  = 1;
	GpioDataRegs.GPADAT.bit.GPIO9 = 1;
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
#ifndef POSTIVE_DRIVE_LOGIC
	EPwm2Regs.CMPB = EPwm2Regs.TBPRD - gADC.DelayApply;
#else
	EPwm2Regs.CMPB = gADC.DelayApply;
#endif

	EPwm2Regs.TBPHS.all = 0;
	EPwm2Regs.TBCTL.all = 0x2012;

	//EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;
	//EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;	//定时器下溢产生同步信号
	//EPwm2Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;
	//Set the Counter-compare (CC) Module
	EPwm2Regs.CMPCTL.all = 0x0100;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = 1;

	//EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPA.half.CMPA = C_INIT_PRD/2;

#ifndef POSTIVE_DRIVE_LOGIC
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

#ifndef POSTIVE_DRIVE_LOGIC
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
	EPwm2Regs.TZCLR.all = 0x0F;

    // TZ1 as one-shot, and TZ2 as CBC
    // Set the Trip-zone (TZ) Module
    //  EPwm1Regs.TZSEL.all = 0x0104;//??why is 0x0102

	//OC
    // DCAEVT1 as one-shot
    // Set the Trip-zone (TZ) Module
    // EPwm2Regs.DCACTL.bit.EVT1SRCSEL = 0;             // DCAEVT1 Source Is DCAEVT1 Signal
    // EPwm2Regs.DCACTL.bit.EVT1SYNCE = 1;              // DCAEVT1 SYNC, Enable
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;          // DCAEVT1 Source Is DCAEVT1 Signal
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT1 Force Synchronization Signal Select: Asynchronous Signal

    EPwm2Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;        // Digital Compare Output A Event 1 Selection:
                                                        // DCAH = low, DCAL = don't care
    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP2OUT;  // Digital Compare A Low Input Select: COMP1OUT input
    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
                                                        // TZ1,TZ3 ASLO OK as DCAL don't care
    EPwm2Regs.TZSEL.bit.DCAEVT1 = 1;                    // Enable DCAEVT1 as one-shot-trip source for this ePWM module
    EPwm2Regs.TZSEL.bit.OSHT2 = 1;                      // Enable TZ2 as a one-shot trip source for this ePWM module

    // DCAEVT2 as CBC
//    EPwm2Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;          // DCAEVT2 Source Is DCAEVT1 Signal
//    EPwm2Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT2 Force Synchronization Signal Select: Asynchronous Signal
//
//    EPwm2Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCBH_LOW;        // Digital Compare Output A Event 2 Selection:
//                                                        // DCAH = low, DCAL = don't care
//    EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP3OUT;  // Digital Compare A Low Input Select: COMP2OUT input
//    EPwm2Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
//                                                        // TZ1,TZ3 ASLO OK as DCAL don't care
//    EPwm2Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCAEVT2 as one-shot-trip source for this ePWM module

    //EPwm2Regs.TZSEL.all = 0x0200;

	//EPwm2Regs.TZSEL.bit.OSHT1 = TZ_ENABLE;	//过流信号对PWM1的封锁
	//EPwm2Regs.TZSEL.bit.CBC3 = TZ_ENABLE;		//EPWM1的逐波限流
    //Force to EPMAX,BX to high state...
	//EPwm2Regs.TZCTL.bit.TZA = 1;
	//EPwm2Regs.TZCTL.bit.TZB = 1;
#ifndef POSTIVE_DRIVE_LOGIC
	EPwm2Regs.TZCTL.all = 0x0005;
#else
	EPwm2Regs.TZCTL.all = 0x00AA;
#endif

	// EPwm2Regs.TZEINT.bit.OST = TZ_ENABLE;		//启用过流中断
	// Digital Comparator Output A Event 1 Interrupt Enable
	EPwm2Regs.TZEINT.all = 0x0004;

#ifndef POSTIVE_DRIVE_LOGIC
	//Set the Event-trigger (ET) Module
	EPwm2Regs.ETCLR.bit.INT = 1;				//首先清除中断标志

	// 0000 1111 0000 1001
	// EPWMxSOCB disabled.
	// Enable EPWMxSOCA pulse.
	// Enable event: time-base counter equal to CMPB when the timer is decrementing.
	////////////////////////////////////////////////////////////////////////////////
	EPwm2Regs.ETSEL.all = 0x0F09;
	//EPwm2Regs.ETSEL.bit.INTEN = 1;
	//EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
	//EPwm2Regs.ETSEL.bit.SOCAEN = 1;
	//EPwm2Regs.ETSEL.bit.SOCASEL = ET_CTRD_CMPB;	//COMPAR_B的下降沿启动ADC
#else
	EPwm2Regs.ETCLR.bit.INT 		= 1;		// 首先清除中断标志
	
	// 2,6,7 都不对，只有1可以！4 较差，其他不行...
	// 分析下，1 是Enable event time-base counter equal to zero.
	// 也就是说波谷触发ADC。。。
	// 选择2，6，7 即波峰，CMPAR_B上升，下降都不行
	///////////////////////////////////////////////////////////
	EPwm2Regs.ETSEL.bit.SOCAEN 		= 1;     	// Enable SOCA
	EPwm2Regs.ETSEL.bit.SOCASEL 	= 1;        // 波谷触发...

	// 注意，这里中断触发的时机可能有问题。之前是波峰触发ADC采样，波谷进行ADC 中断。
	// 现在改成波谷ADC 采样，波峰采样？
	// 这样或许应该保留之前的中断结构。设置CMPB 很小。然后
	EPwm2Regs.ETSEL.bit.INTEN  = 1;
	EPwm2Regs.ETSEL.bit.INTSEL = 1;
#endif

	EPwm2Regs.ETPS.all = 0x0101;
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
	EPwm3Regs.CMPCTL.all = 0x0100;

	//EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPA.half.CMPA = C_INIT_PRD/2;

	//Set the Action-qualifier (AQ) Module
#ifndef POSTIVE_DRIVE_LOGIC
	//Set the Action-qualifier (AQ) Module
	EPwm3Regs.AQCTLA.all = 0x0090;
#else
	EPwm3Regs.AQCTLA.all = 0x0060;
#endif

#ifndef POSTIVE_DRIVE_LOGIC
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

    //Set the PWM-chopper (PC) Module
    //Set the Trip-zone (TZ) Module
	EPwm3Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;          // DCAEVT1 Source Is DCAEVT1 Signal
	EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT1 Force Synchronization Signal Select: Asynchronous Signal

	EPwm3Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;        // Digital Compare Output A Event 1 Selection:
                                                        // DCAH = low, DCAL = don't care
	EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP2OUT;  // Digital Compare A Low Input Select: COMP1OUT input
	EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
                                                        // TZ1,TZ3 ASLO OK as DCAL don't care
	EPwm3Regs.TZSEL.bit.DCAEVT1 = 1;                    // Enable DCAEVT1 as one-shot-trip source for this ePWM module
	EPwm3Regs.TZSEL.bit.OSHT2 = 1;                      // Enable TZ2 as a one-shot trip source for this ePWM module

    // DCAEVT2 as CBC
//	EPwm3Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;          // DCAEVT2 Source Is DCAEVT1 Signal
//	EPwm3Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT2 Force Synchronization Signal Select: Asynchronous Signal
//
//	EPwm3Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCBH_LOW;        // Digital Compare Output A Event 2 Selection:
//                                                        // DCAH = low, DCAL = don't care
//	EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP3OUT;  // Digital Compare A Low Input Select: COMP2OUT input
//	EPwm3Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
//                                                        // TZ1,TZ3 ASLO OK as DCAL don't care
//	EPwm3Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCAEVT2 as one-shot-trip source for this ePWM module

	//EPwm3Regs.TZSEL.all = 0x0100;

#ifndef POSTIVE_DRIVE_LOGIC
	EPwm3Regs.TZCTL.all = 0x0005;
#else
	EPwm3Regs.TZCTL.all = 0x00AA;
#endif

	//EPwm3Regs.TZCTL.bit.TZA = 1;
	//EPwm3Regs.TZCTL.bit.TZB = 1;
	//Set the Event-trigger (ET) Module

	/////////////PWM3//////////////
	//Set the Time-Base (TB) Module
	EPwm4Regs.TBPRD = C_INIT_PRD;
	EPwm4Regs.TBPHS.all = 0;
	EPwm4Regs.TBCTL.all = 0x2036;

	//EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	//EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;	//不产生同步信号
	//EPwm4Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;
	//Set the Counter-compare (CC) Module
	EPwm4Regs.CMPCTL.all = 0x0100;

	//EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm4Regs.CMPA.half.CMPA = C_INIT_PRD/2;

	//Set the Action-qualifier (AQ) Module
#ifndef POSTIVE_DRIVE_LOGIC
	//Set the Action-qualifier (AQ) Module
	EPwm4Regs.AQCTLA.all = 0x0090;
#else
	EPwm4Regs.AQCTLA.all = 0x0060;
#endif

#ifndef POSTIVE_DRIVE_LOGIC
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

    //Set the PWM-chopper (PC) Module
    //Set the Trip-zone (TZ) Module
    EPwm4Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;          // DCAEVT1 Source Is DCAEVT1 Signal
    EPwm4Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT1 Force Synchronization Signal Select: Asynchronous Signal

    EPwm4Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;        // Digital Compare Output A Event 1 Selection:
                                                        // DCAH = low, DCAL = don't care
    EPwm4Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP2OUT;  // Digital Compare A Low Input Select: COMP1OUT input
    EPwm4Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
                                                        // TZ1,TZ3 ASLO OK as DCAL don't care
    EPwm4Regs.TZSEL.bit.DCAEVT1 = 1;                    // Enable DCAEVT1 as one-shot-trip source for this ePWM module
    EPwm4Regs.TZSEL.bit.OSHT2 = 1;                      // Enable TZ2 as a one-shot trip source for this ePWM module

    // DCAEVT2 as CBC
//    EPwm4Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;          // DCAEVT2 Source Is DCAEVT1 Signal
//    EPwm4Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC; // DCAEVT2 Force Synchronization Signal Select: Asynchronous Signal
//
//    EPwm4Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCBH_LOW;        // Digital Compare Output A Event 2 Selection:
//                                                        // DCAH = low, DCAL = don't care
//    EPwm4Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP3OUT;  // Digital Compare A Low Input Select: COMP2OUT input
//    EPwm4Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;       // Digital Compare B Low Input Select: TZ2
//                                                        // TZ1,TZ3 ASLO OK as DCAL don't care
//    EPwm4Regs.TZSEL.bit.DCBEVT2 = 1;                    // Enable DCAEVT2 as one-shot-trip source for this ePWM module
    //EPwm4Regs.TZSEL.all = 0x0100;
#ifndef POSTIVE_DRIVE_LOGIC
	EPwm4Regs.TZCTL.all = 0x0005;
#else
	EPwm4Regs.TZCTL.all = 0x00AA;
#endif

	// make sure the CMPB be loading immediate
	// PWM_setShadowMode_CmpB(obj->pwmHandle[cnt],PWM_ShadowMode_Immediate);
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = 1;

	//Set the Event-trigger (ET) Module
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

    asm(" RPT #50 || NOP");                 // 延时
    EALLOW;
    PagOpaRegs.PAGOPACTL.bit.OPAEN = 1;     // 内部运放配置

//    PagOpaRegs.PAGOPACTL.bit.PGA1_GAIN = 4; // 16倍
//    PagOpaRegs.PAGOPACTL.bit.PGA2_GAIN = 4;
//    PagOpaRegs.PAGOPACTL.bit.PGA3_GAIN = 4;
//
//    PagOpaRegs.PAGOPACTL.bit.PGA1EN = 1;
//    PagOpaRegs.PAGOPACTL.bit.PGA2EN = 1;
//    PagOpaRegs.PAGOPACTL.bit.PGA3EN = 1;
    EDIS;

    //OC
    EALLOW;
    Comp2Regs.DACVAL.bit.DACVAL = OverCurPointInternal;
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

    //CBC
    Comp3Regs.DACVAL.bit.DACVAL = CBCCurPointInternal;
                                        // 硬件过流比较器配置   2.5V  1024->3V, about 685, 5*1.4142A = 7.1
                                        // 512 * (7.1 / 21) + 512 = 685
                                        // 实测680 OK，690 NOT OK！！！！
                                        // 如果设置10APEAK 过流点，则 10 / 21)*512+512 = 755
                                        // 如果设置10ARMS 过流点，则 10*1.4142 / 21)*512+512 = 856
                                        ///////////////////////////////////////////////////////////////
    Comp3Regs.COMPCTL.bit.SYNCSEL       = 0;  // Asynchronous version of Comparator output is passed
    Comp3Regs.COMPCTL.bit.CMPINV        = 1;  // Inverted output of comparator is passed
    Comp3Regs.COMPCTL.bit.COMPSOURCE    = 0;  // input of comparator connected to internal DAC
    Comp3Regs.COMPCTL.bit.QUALSEL       = 10; // Qualification Period for synchronized output of the comparator
    Comp3Regs.COMPCTL.bit.COMPDACEN     = 0;  // Comparator/DAC Enable
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
    // AdcRegs.INTSEL1N2.all  = 0x0020;      // SOC0通道转换完成产生中断ADCINT1
    AdcRegs.INTSEL1N2.all     = 0x0024;      // SOC2通道转换完成产生中断ADCINT1
    AdcRegs.ADCSAMPLEMODE.all = 0;           // 采样模式为0: 顺序采样(非同时采样)
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;     // ADCINT1 trips after AdcResults latch

    AdcRegs.ADCCTL2.bit.CLKDIV2EN = 1;
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;

    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;   // ADCTRIG7 – ePWM2, ADCSOCA
                                                            // ETSEL[SOCASEL] = 6, CTRU=CMPB
                                                            // ETSEL[SOCAEN ] = 1, Enable SOCA
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;   // ADCTRIG7 – ePWM2, ADCSOCA
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;   // ADCTRIG7 – ePWM2, ADCSOCA
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;   // ADCTRIG7 – ePWM2, ADCSOCA
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;   // ADCTRIG7 – ePWM2, ADCSOCA
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;   // ADCTRIG7 – ePWM2, ADCSOCA

    // Acquisition Pulse Size
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = 8;//4;//8;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = 8;//4;//8;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = 8;//4;//8;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = 8;//4;//8;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = 8;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = 8;

    // SOCx Channel Select
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S3;  //
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S3;  // ADCINA0-IU
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S5;  // ADCINA1-IV
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S7;  // ADCINA3-IW
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S11; // ADCINA7-UDC
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S2;  // ADCINB1-TMP

    ADC_CLEAR_INT_FLAG;

    EDIS;
}

//===========================================================================
// End of file.
//===========================================================================
