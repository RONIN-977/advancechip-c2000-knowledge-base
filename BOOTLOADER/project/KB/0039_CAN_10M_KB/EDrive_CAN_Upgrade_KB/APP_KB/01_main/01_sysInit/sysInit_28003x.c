/*
 * SysInit_28002x.c
 *
 *  Created on: Feb 9, 2022
 *      Author: rand C
 */

#include "sysInit_28003x.h"

extern const u16 EXC_Carrier_Prd;
extern const u16 Exc_half_Duty;

void SetupGPIO(void);
void InitADC(void);
void InitEPwmResExc(void);
void InitCpuTimer(void);
void InitPeriherals(void);
void InitIsrCtrl(void);

void InitPIE(void);
void InitEPwmUVW(void);

void InitIsrCtrl(void){

//    InitPieCtrl();
//    IER = 0x0000;
//    IFR = 0x0000;

    Interrupt_initModule();
    InitPieVectTable();
//    ConfigCpuTimer(&CpuTimer0, 100, 0.0005 * 1000000);
    /* User Specific */
    EALLOW;

//    PieVectTable.ADCA1_INT = &ResAdcEndIsr;//123
//    PieVectTable.ADCC1_INT = &ResAdcFOCIsr;//123
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 0; /* KB_UPGRADE_TEST_IRQ: restore to 1 with original ISR */
    PieCtrlRegs.PIEIER1.bit.INTx3 = 0; /* KB_UPGRADE_TEST_IRQ: restore to 1 with original ISR */
//    PieVectTable.EPWM4_INT = &ResPwmIsr;
//    IER |= M_INT3;
//    PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
//    PieVectTable.EPWM1_TZ_INT = &EPWMTZ_IsrTask;//123
    IER |= (M_INT2);                            //  Enable interrupts:
    PieCtrlRegs.PIEIER2.bit.INTx1 = 0; /* KB_UPGRADE_TEST_IRQ: restore to 1 with original ISR */
    EDIS;
}

void InitPeriherals(void){

//    InitGpio();
    init_eCAN_A(20, 5, 4, 1, 2, 1);  // 500kbps
    //init_eCAN_A(40, 5, 4, 1, 2, 1);  // 250kbps
    Device_initGPIO();
    SetupGPIO();
    CAN_Config_Init();
    InitIsrCtrl();
#if  EnableABO
    CanaRegs.CAN_CTL.bit.ABO = 1;//index:20220503
#endif
    InitEPwmResExc();
    InitEPwmUVW();
    InitADC();
    InitCpuTimer();
    StartCpuTimer1();
    StartCpuTimer0();


}

void SetupGPIO(void){
    EALLOW;

    // res INH
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;   // Enable pullup on GPIO8
    GpioDataRegs.GPASET.bit.GPIO8 = 1;   // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  // GPIO8 = GPIO8
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;   // GPIO8 = output

    // res CTRL
//    GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;   // Enable pullup on GPIO39
//    GpioDataRegs.GPBSET.bit.GPIO39 = 1;   // Load output latch
//    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;  // GPIO39 = GPIO39
//    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;   // GPIO39 = output

    // beep; used for testing cpu load
//    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pullup on GPIO32
//    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3; // Select input qualification type: Async
//    InputXbarRegs.INPUT1SELECT = 0x20;    // GPIO32 = TZ1

    // beep; used for testing cpu load
//    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;    // Enable pullup on GPIO7
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;  // Select input qualification type: Async
//    InputXbarRegs.INPUT1SELECT = 0x07;    // GPIO7 = TZ1

    // beep; used for testing cpu load
    GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;    // Enable pullup on GPIO39
    GpioCtrlRegs.GPBQSEL1.bit.GPIO39 = 3;  // Select input qualification type: Async
    InputXbarRegs.INPUT1SELECT = 0x27;     // GPIO39 = TZ1

    // res indicator light or buzzer
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;   // Enable pullup on GPIO10
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1; // Load output latch 0
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;  // GPIO10 = GPIO10
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;   // GPIO10 = output
    
    // test
    //GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;   // Enable pullup on GPIO25
    //GpioDataRegs.GPACLEAR.bit.GPIO25 = 1; // Load output latch 0
    //GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;  // GPIO25 = GPIO25
    //GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;   // GPIO25 = output
    
    // Open cover protection
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;   // Enable pullup on GPIO29
    GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 1; // Select input qualification type: Qualification (3 samples)
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;  // GPIO29 = GPIO29
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 0;   // GPIO29 = input
    
    // START
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;   // Enable pullup on GPIO28
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 1; // Select input qualification type: Qualification (3 samples)
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;  // GPIO28 = GPIO28
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;   // GPIO28 = input
    
    // ECO
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;   // Enable pullup on GPIO11
    GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 1; // Select input qualification type: Qualification (3 samples)
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;  // GPIO11 = GPIO11
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 0;   // GPIO11 = input
    
    // REVERSE
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   // Enable pullup on GPIO33
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 1; // Select input qualification type: Qualification (3 samples)
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;  // GPIO33 = GPIO33
    GpioCtrlRegs.GPBDIR.bit.GPIO33 = 0;   // GPIO33 = input
    
    // FORWARD
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pullup on GPIO16
    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 1; // Select input qualification type: Qualification (3 samples)
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;  // GPIO16 = GPIO16
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 0;   // GPIO16 = input
    
    // BREAK
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pullup on GPIO17
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 1; // Select input qualification type: Qualification (3 samples)
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;  // GPIO17 = GPIO17
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;   // GPIO17 = input

//    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;   // GPIO32 = output
//    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;   // GPIO24 = output
//    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;   // GPIO25 = output
//    GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;   // GPIO26 = output
//    GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;   // GPIO27 = output

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pullup on GPIO8
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;    // Enable pullup on GPIO9
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;   // Enable pullup on GPIO14
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;   // Enable pullup on GPIO15
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;   // Enable pullup on GPIO22
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;   // Enable pullup on GPIO23
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;   // Enable pullup on GPIO24
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;   // Enable pullup on GPIO25
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;   // Enable pullup on GPIO26
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;   // Enable pullup on GPIO27
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;   // Enable pullup on GPIO30
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;   // Enable pullup on GPIO31
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pullup on GPIO32
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;   // Enable pullup on GPIO34
    GpioCtrlRegs.GPBPUD.bit.GPIO40 = 0;   // Enable pullup on GPIO40
    GpioCtrlRegs.GPBPUD.bit.GPIO41 = 0;   // Enable pullup on GPIO41
    GpioCtrlRegs.GPBPUD.bit.GPIO42 = 0;   // Enable pullup on GPIO42
    GpioCtrlRegs.GPBPUD.bit.GPIO43 = 0;   // Enable pullup on GPIO43
    GpioCtrlRegs.GPBPUD.bit.GPIO44 = 0;   // Enable pullup on GPIO44
    GpioCtrlRegs.GPBPUD.bit.GPIO45 = 0;   // Enable pullup on GPIO45
    GpioCtrlRegs.GPBPUD.bit.GPIO46 = 0;   // Enable pullup on GPIO46

    GpioCtrlRegs.GPAPUD.all = 0x007F;
    GpioCtrlRegs.GPBPUD.all = 0x0000;

    EDIS;
}

void InitEPwmResExc(void){

    EALLOW;

    // CLK
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;  //BIT2 TBCLKSYNC
    CpuSysRegs.PCLKCR2.bit.EPWM4     = 1;  //BIT0 ePWM4
    // set Gpio6 as epwm4A
    GpioCtrlRegs.GPAPUD.bit.GPIO6   = 1;    // Disable pull-up
    GpioCtrlRegs.GPAGMUX1.bit.GPIO6 = 0;    // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 01;   // Configure GPIO6 as EPWM4A

    /* Setup TBCLK */
    EPwm4Regs.TBCTL.bit.CTRMODE = 0x0;
//    EPwm4Regs.TBPRD = EXC_Carrier_Prd;//0x0271;//123
    EPwm4Regs.TBCTL.bit.PHSEN = 0x0;
    EPwm4Regs.TBCTR = 0x0000;               /* Clear counter */
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0x0;    /* Clock ratio to SYSCLKOUT */
    EPwm4Regs.TBCTL.bit.CLKDIV = 0x0;
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = 0x00;

//    EPwm4Regs.CMPA.bit.CMPA = Exc_half_Duty;//0x139;//123

    EPwm4Regs.AQCTLA.bit.ZRO = 0x2;
    EPwm4Regs.AQCTLA.bit.CAU = 0x1;

//    EPwm4Regs.ETSEL.bit.INTSEL = 1;
//    EPwm4Regs.ETSEL.bit.INTEN = 1;
//    EPwm4Regs.ETPS.bit.INTPRD = 0x1;

    EPwm4Regs.ETSEL.bit.SOCAEN = 1;
    EPwm4Regs.ETSEL.bit.SOCASEL = 1;
    EPwm4Regs.ETPS.bit.SOCAPRD = 1;

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

    EDIS;
}

void InitADC(void){

    //SetVREF(ADC_ADCA, ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADC_ADCA, ADC_EXTERNAL, ADC_VREF3P3);

    EALLOW;

    // AdcA
    AdcaRegs.ADCCTL2.bit.PRESCALE = 2;
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    // AdcC
    AdccRegs.ADCCTL2.bit.PRESCALE = 2;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    DELAY_US(1200);

    // OSIN A6
    AdcaRegs.ADCSOC0CTL.bit.CHSEL   = 6;      // SOC0 will convert pin A6
    AdcaRegs.ADCSOC0CTL.bit.ACQPS   = 19;     // Sample window is 20 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 11;     // Trigger on ePWM4 SOCA

    // OCOS C6
    AdccRegs.ADCSOC0CTL.bit.CHSEL   = 6;      // SOC0 will convert pin C6
    AdccRegs.ADCSOC0CTL.bit.ACQPS   = 19;     // Sample window is 20 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 11;     // Trigger on ePWM4 SOCA

    // OEXC A15
    AdcaRegs.ADCSOC1CTL.bit.CHSEL   = 15;     // SOC1 will convert pin A15
    AdcaRegs.ADCSOC1CTL.bit.ACQPS   = 19;     // Sample window is 20 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 11;     // Trigger on ePWM4 SOCA
    
    // IV
    //AdccRegs.ADCSOC1CTL.bit.CHSEL   = 0;      // SOC1 will convert pin C0
    //AdccRegs.ADCSOC1CTL.bit.ACQPS   = 40;     // Sample window is 20 SYSCLK cycles
    //AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 11;     // Trigger on ePWM4 SOCA

    // Udc C1
    AdccRegs.ADCSOC1CTL.bit.CHSEL   = 1;      // SOC1 will convert pin C1
    AdccRegs.ADCSOC1CTL.bit.ACQPS   = 19;     // Sample window is 20 SYSCLK cycles
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 11;     // Trigger on ePWM4 SOCA
    
    // IU A0
    AdcaRegs.ADCSOC2CTL.bit.CHSEL   = 0;      // SOC2 will convert pin A0
    AdcaRegs.ADCSOC2CTL.bit.ACQPS   = 19;     // Sample window is 20 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5;      // Trigger on ePWM1 SOCA
    
    // IV C0
    AdccRegs.ADCSOC2CTL.bit.CHSEL   = 0;      // SOC2 will convert pin C0
    AdccRegs.ADCSOC2CTL.bit.ACQPS   = 19;     // Sample window is 20 SYSCLK cycles
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 5;      // Trigger on ePWM1 SOCA
    
    // Accelerator1 C2
    AdccRegs.ADCSOC3CTL.bit.CHSEL = 2;        // SOC3 will convert pin C2
    AdccRegs.ADCSOC3CTL.bit.ACQPS = 19;       // Set SOC3 S/H Window to 20 ADC Clock Cycles
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = 5;      // Trigger on ePWM1 SOCA
    
    // Accelerator2 A1
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 1;        // Set SOC3 channel select to A1
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = 19;       // Set SOC3 S/H Window to 20 ADC Clock Cycles
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 5;      // Trigger on ePWM1 SOCA

    // IGBTtemp A2
    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 2;        // Set SOC4 channel select to A2
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = 19;       // Set SOC4 S/H Window to 20 ADC Clock Cycles
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = 5;      // Trigger on ePWM1 SOCA
    
    // V12dc C3
    AdccRegs.ADCSOC4CTL.bit.CHSEL = 3;        // Set SOC4 channel select to C3
    AdccRegs.ADCSOC4CTL.bit.ACQPS = 19;       // Set SOC4 S/H Window to 20 ADC Clock Cycles
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = 5;      // Trigger on ePWM1 SOCA
    
    // Motortemp A14
    AdcaRegs.ADCSOC5CTL.bit.CHSEL = 14;       // Set SOC5 channel select to A14
    AdcaRegs.ADCSOC5CTL.bit.ACQPS = 19;       // Set SOC5 S/H Window to 20 ADC Clock Cycles
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 5;      // Trigger on ePWM1 SOCA
    
    //
    AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 2; // SOC0-SOC1 are high priority, SOC2-SOC15 are in round robin mode
    AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 2; // SOC0-SOC1 are high priority, SOC2-SOC15 are in round robin mode

    // Interrupt: adca soc1
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    AdcaRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;           //end of SOC1 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E   = 1;           //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;           //make sure INT1 flag is cleared
    // Interrupt: adcc soc2
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    AdccRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;
    AdccRegs.ADCINTSEL1N2.bit.INT1E   = 1;          /* Enabled/Disable ADCINT1 interrupt*/
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 2;          /* Setup EOC2 to trigger ADCINT1*/
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          //make sure INT1 flag is cleared

    //
    //PieVectTable.ADCA1_INT = &ResAdcEndIsr;

    EDIS;
}

void InitCpuTimer(void){

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;
    CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
    EDIS;

    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
    CpuTimer1Regs.TPR.all  = 0;
    CpuTimer1Regs.TPRH.all = 0;

    CpuTimer1Regs.TCR.bit.TSS = 1;

    CpuTimer1Regs.TCR.bit.TRB = 1;

    CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;

    CpuTimer0Regs.TCR.bit.TSS = 1;

    CpuTimer0Regs.TCR.bit.TRB = 1;
}
#define gPWM10K  5000
#define DEFAULT_DEAD_TIME  200
void InitEPwmUVW(void){
    EALLOW;

    // CLK
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;  //BIT2 TBCLKSYNC
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;/* Enable clock to ePWM */
    CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
    //    GpioCtrlRegs.GPAPUD.all |= 0x3F;    // Disable pull-up
    //    GpioCtrlRegs.GPAGMUX1.all &=0xFFFFFFC0; // Configure GPIO0~5 as EPWM1~3
    //    GpioCtrlRegs.GPAMUX1.all |= 0x3F;   // Configure GPIO0~5 as EPWM1~3
    // set Gpio0 as epwm1A
    GpioCtrlRegs.GPAPUD.bit.GPIO0   = 1;    // Disable pull-up
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;    // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 01;   // Configure GPIO0 as EPWM1A
    // set Gpio1 as epwm1B
    GpioCtrlRegs.GPAPUD.bit.GPIO1   = 1;    // Disable pull-up
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;    // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 01;   // Configure GPIO1 as EPWM1B
    // set Gpio2 as epwm2A
    GpioCtrlRegs.GPAPUD.bit.GPIO2   = 1;    // Disable pull-up
    GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;    // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 01;   // Configure GPIO0 as EPWM1A
    // set Gpio3 as epwm2B
    GpioCtrlRegs.GPAPUD.bit.GPIO3   = 1;    // Disable pull-up
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;    // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 01;   // Configure GPIO3 as EPWM2B
    // set Gpio4 as epwm3A
    GpioCtrlRegs.GPAPUD.bit.GPIO4   = 1;    // Disable pull-up
    GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;    // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 01;   // Configure GPIO4 as EPWM3A
    // set Gpio5 as epwm3B
    GpioCtrlRegs.GPAPUD.bit.GPIO5   = 1;    // Disable pull-up
    GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;    // Configure GPIO5 as EPWM3B
    GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 01;   // Configure GPIO5 as EPWM3B

    /////////////ePWM1//////////////
    //Set the Time-Base (TB) Module
    EPwm1Regs.TBPRD     = gPWM10K;
    EPwm1Regs.TBPHS.all = 0;
    EPwm1Regs.TBCTL.all = 0xE012;
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 0;
    //Set the Counter-compare (CC) Module
    EPwm1Regs.CMPCTL.all = 0x0100;                   //?锟斤拷?锟斤拷o锟斤拷??锟斤拷?锟斤拷???锟斤拷锟斤拷??cmp?锟斤拷;cmpb锟斤拷?锟斤拷锟斤拷??锟斤拷锟斤拷?锟戒é锟斤拷D锟斤拷
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;//CC_CTR_ZERO
    EPwm1Regs.CMPA.bit.CMPA = gPWM10K/2;
//    EPwm1Regs.CMPB.bit.CMPB = 1200;
    //Set the Action-qualifier (AQ) Module
    EPwm1Regs.AQCTLA.all = 0x0090;
    //Set the Trip-zone (TZ) Module
    EPwm1Regs.TZCTL.all = 0x000A;
    EPwm1Regs.TZSEL.bit.OSHT1 = 1;
    EPwm1Regs.TZCLR.all = 0x07;

    EPwm1Regs.TZEINT.bit.OST  = 1;
    //Set the Dead-Band Generator (DB) Module
    EPwm1Regs.AQSFRC.all  = 0x0;
    EPwm1Regs.AQCSFRC.all = 0x0;
    EPwm1Regs.DBCTL.all = 0x000B;
    EPwm1Regs.DBFED.bit.DBFED = DEFAULT_DEAD_TIME;
    EPwm1Regs.DBRED.bit.DBRED = DEFAULT_DEAD_TIME;

    EPwm1Regs.TBCTL.bit.SWFSYNC = 1;
    //Set the Event-trigger (ET) Module
//    EPwm1Regs.ETCLR.bit.SOCA = 1;
//    EPwm1Regs.ETCLR.bit.INT  = 1;
//    EPwm1Regs.ETPS.bit.SOCAPRD = 1;
////    EPwm1Regs.ETPS.bit.INTPRD = 1;
//    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
////    EPwm1Regs.ETSEL.bit.SOCASEL = 0x001;
//    EPwm1Regs.ETSEL.bit.SOCASEL = 0x011; // double sampling

    EPwm1Regs.ETCLR.bit.SOCA = 1;               //首先清除ADC采样标志位
    EPwm1Regs.ETCLR.bit.INT  = 1;
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;
//    EPwm1Regs.ETPS.bit.INTPRD = 1;
    EPwm1Regs.ETSEL.all = 0x0B00;

    /////////////ePWM2//////////////
    //Set the Time-Base (TB) Module
    EPwm2Regs.TBPRD = gPWM10K;
    EPwm2Regs.TBPHS.all = 0;
    EPwm2Regs.TBCTL.all = 0xE006;
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 0;

    EPwm2Regs.CMPCTL.all = 0x0100;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;//CC_CTR_ZERO
    EPwm2Regs.CMPA.bit.CMPA = gPWM10K/2;
    //Set the Action-qualifier (AQ) Module
    EPwm2Regs.AQCTLA.all = 0x0090;
    //Set the Trip-zone (TZ) Module
    EPwm2Regs.TZCTL.all = 0x000A;
    EPwm2Regs.TZSEL.bit.OSHT1 = 1;
    EPwm2Regs.TZCLR.all = 0x07;
    //Set the Dead-Band Generator (DB) Module
    EPwm2Regs.AQSFRC.all  = 0x0;
    EPwm2Regs.AQCSFRC.all = 0x0;
    EPwm2Regs.DBCTL.all = 0x000B;
    EPwm2Regs.DBFED.bit.DBFED = DEFAULT_DEAD_TIME;
    EPwm2Regs.DBRED.bit.DBRED = DEFAULT_DEAD_TIME;
    EPwm2Regs.TBCTL.bit.SWFSYNC = 1;

    /////////////ePWM3//////////////
    //Set the Time-Base (TB) Module
    EPwm3Regs.TBPRD = gPWM10K;
    EPwm3Regs.TBPHS.all = 0;
    EPwm3Regs.TBCTL.all = 0xE036;
    //Set the Counter-compare (CC) Module
    EPwm3Regs.CMPCTL.all = 0x0100;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;//CC_CTR_ZERO
//    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
    EPwm3Regs.CMPA.bit.CMPA = gPWM10K/2;
    //Set the Action-qualifier (AQ) Module
    EPwm3Regs.AQCTLA.all = 0x0090;
    //Set the Trip-zone (TZ) Module
    EPwm3Regs.TZCTL.all = 0x000A;
    EPwm3Regs.TZSEL.bit.OSHT1 = 1;
    EPwm3Regs.TZCLR.all = 0x07;
    //Set the Dead-Band Generator (DB) Module
    EPwm3Regs.AQSFRC.all  = 0x0;
    EPwm3Regs.AQCSFRC.all = 0x00;
    EPwm3Regs.DBCTL.all = 0x000B;
    EPwm3Regs.DBFED.bit.DBFED = DEFAULT_DEAD_TIME;
    EPwm3Regs.DBRED.bit.DBRED = DEFAULT_DEAD_TIME;
    EPwm3Regs.TBCTL.bit.SWFSYNC = 1;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;  //BIT2 TBCLKSYNC
    EDIS;
}
