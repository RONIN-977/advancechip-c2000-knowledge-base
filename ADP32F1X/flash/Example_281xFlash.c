// TI File $Revision: /main/2 $
// Checkin $Date: July 31, 2009   17:58:40 $
//###########################################################################
//
// FILE:   DSP28_281xFlash.c
//
// TITLE:  DSP281x Flash example.
//
// ASSUMPTIONS:
//
//          This program requires the DSP281x V1.00 header files.  
//          As supplied, this project is configured for "boot to Flash" operation.   
//
//          This example runs the EV Timer Period example from the 
//          Flash.  
//
//          1) Build the project
//          2) Flash the .out file into the device.  This program will fit on 
//             an F2812 or an F2810
//          3) Set the hardware jumpers to boot to Flash 
//          4) Use the included GEL file to load the project, symbols 
//             defined within the project and the variables into the watch
//             window.   
//
//          Steps that were taken to convert the EV example from RAM 
//          to Flash execution:
//
//          - Change the linker cmd file to reflect the flash memory map.
//            This example uses the DSP281x F2812.cmd file.
//          - Make sure any initialized sections are mapped to Flash.  
//            In SDFlash utility this can be checked by the View->Coff/Hex
//            status utility. Any section marked as "load" should be
//            allocated to Flash.
//          - Make sure there is a branch instruction from the entry to Flash
//            at 0x3F7FF6 to the beginning of code execution. This example
//            uses the DSP281x_CodeStartBranch.asm file to accomplish this.
//          - Set boot mode Jumpers to "boot to Flash"
//          - For best performance from the flash, modify the waitstates
//            and enable the flash pipeline as shown in this example.
//            Note: any code that manipulates the flash waitstate and pipeline
//            control must be run from RAM. Thus these functions are located
//            in their own memory section called ramfuncs.
// 
// DESCRIPTION:
//
//          This program sets up EVA Timer 1, EVA Timer 2, EVB Timer 3
//          and EVB Timer 4 to fire an interrupt on a period overflow.  
//          A count is kept each time each interrupt passes through
//          the interrupt service routine. 
//
//          EVA Timer 1 has the shortest period while EVB Timer4 has the
//          longest period.
//
//          ISR Locations:
//                eva_timer1_isr  executed from RAM, puts flash in sleep mode
//                eva_timer2_isr  executed from RAM, puts flash in standby mode
//                evb_timer3_isr  executed from RAM, puts flash in sleep mode
//                                              and later moves it to standby
//                evb_timer4_isr  executed from FLASH
//        
//          Watch Variables:
//
//                EvaTimer1InterruptCount;
//                EvaTimer2InterruptCount;
//                EvbTimer3InterruptCount;
//                EvbTimer4InterruptCount;
//
//###########################################################################
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File



// Prototype statements for functions found within this file.
interrupt void adc_isr(void);
void init_eva(void);
void init_evb(void);
void scia_echoback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);

void scib_echoback_init(void);
void scib_fifo_init(void);
void scib_xmit(int a);
void scib_msg(char *msg);

void InitECan(void);

// Global variables used in this example
Uint16 LoopCount;
Uint16 ConversionCount;
Uint16 Voltage1[10];
Uint16 Voltage2[10];

#define TXCOUNT  1000    //设定CAN发送次数
int TXcounter ;                   //发送次数
unsigned long int  i;
struct        ECAN_REGS ECanaShadow;   //影子寄存器


extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;



void main(void)
{
// Step 5. User specific code, enable interrupts:

// Copy time critical code and Flash setup code to RAM
// This includes the following ISR functions: EvaTimer1(), EvaTimer2()
// EvbTimer3 and and InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
// symbols are created by the linker. Refer to the F2812.cmd file.
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
   InitFlash();

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();
   
   EALLOW;
   SysCtrlRegs.HISPCP.all = 0x3;  // HSPCLK = SYSCLKOUT/6
   EDIS;

// Step 2. Initalize GPIO: 
// This example function is found in the DSP281x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example  
   EALLOW;
   // Enable PWM pins
   GpioMuxRegs.GPAMUX.all = 0x00FF; // EVA PWM 1-6  pins
   GpioMuxRegs.GPBMUX.all = 0x00FF; // EVB PWM 7-12 pins
   EDIS;

   EALLOW;
   GpioMuxRegs.GPFMUX.all=0x0030;   // Select GPIOs to be Scia pins
                                    // Port F MUX - x000 0000 0011 0000
   GpioMuxRegs.GPGMUX.all=0x0030;   // Select GPIOs to be Scib pins
                                    // Port G MUX - x000 0000 0011 0000
   GpioMuxRegs.GPFMUX.bit.CANTXA_GPIOF6 = 1;  //配置GPIOF6为CANTX
   GpioMuxRegs.GPFMUX.bit.CANRXA_GPIOF7 = 1;  //配置GPIOF7为CANRX
   EDIS;

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.  
// This function is found in the DSP281x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt 
// Service Routines (ISR).  
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP281x_DefaultIsr.c.
// This function is found in DSP281x_PieVect.c.
   InitPieVectTable();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.  	
   EALLOW;  // This is needed to write to EALLOW protected register
   PieVectTable.ADCINT = &adc_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   InitAdc();  // For this example, init the ADC

   //init_eva();
   //init_evb();

   scia_fifo_init();
   scib_fifo_init();// Initialize the SCI FIFO
   scia_echoback_init();  // Initalize SCI for echoback
   scib_echoback_init();

   InitECan();

     
   char *msg;
   LoopCount = 0;
   ConversionCount = 0;
   TXcounter=0;
 

   PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
   IER |= M_INT1; // Enable CPU Interrupt 1
   EINT;          // Enable Global interrupt INTM
   ERTM;          // Enable Global realtime interrupt DBGM

   // Enable global Interrupts and higher priority real-time debug events:

   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Configure ADC
   AdcRegs.ADCMAXCONV.all = 0x0003;       // Setup 2 conv's on SEQ1
   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // Setup ADCINA3 as 1st SEQ1 conv.
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // Setup ADCINA2 as 2nd SEQ1 conv.
   AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;
   AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;
   AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1 = 1;  // Enable EVASOC to start SEQ1
   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)

// Configure EVA
// Assumes EVA Clock is already enabled in InitSysCtrl();
   EvaRegs.T1CMPR = 0x0080;               // Setup T1 compare value
   EvaRegs.T1PR = 0xFFFF;                 // Setup period register
   EvaRegs.GPTCONA.bit.T1TOADC = 1;       // Enable EVASOC in EVA
   EvaRegs.T1CON.all = 0x1042;            // Enable timer 1 compare (upcount mode)

   //-----------------------------配置CAN邮箱----------------------------
//配置发送邮箱MBOX0的信息标识符寄存器(MSGID,11位标识符)
   ECanaMboxes.MBOX0.MSGID.all = 0x5C600000;   //0号邮箱标识符
//配置邮箱0为发送
   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD0 = 0;      //邮箱0为发送
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
//配置发送邮箱0的数据长度为8个字节
   ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
//将待发送的数据写入发送邮箱MBOX0 的RAM//
   ECanaMboxes.MBOX0.MDL.all = 0x9555AAA0;
   ECanaMboxes.MBOX0.MDH.all = 0x89ABCDEF;
//使能邮箱0
   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME0 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;

// Step 6. IDLE loop. Just sit and loop forever:	
   while(1)
   {
       msg = "\r\nABCDEFG\0";
       scia_msg(msg);
       DELAY_US(1000);
       msg = " 1234567\0";
       scib_msg(msg);
       DELAY_US(1000);
       /*for (i=0;i<TXCOUNT;i++)
                         {
                        ECanaShadow.CANTRS.all = 0;
                        ECanaShadow.CANTRS.bit.TRS0 = 1;      //CPU对CANTRS[0]置位开始邮箱0的发送
                      ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

                        while(ECanaRegs.CANTA.bit.TA0 == 0){} //等待TA0置位.置位表示发送成功
                    ECanaShadow.CANTA.all=0;
                           ECanaShadow.CANTA.bit.TA0=1;          //清除TA0
                           ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
                          TXcounter++;                          //发送次数
                }
                       TXcounter=0;
                       i=0;*/
       LoopCount++;
   }

} 
 

interrupt void  adc_isr(void)
{

  Voltage1[ConversionCount] = AdcRegs.ADCRESULT0 >>4;
  Voltage2[ConversionCount] = AdcRegs.ADCRESULT1 >>4;

  // If 40 conversions have been logged, start over
  if(ConversionCount == 9)
  {
     ConversionCount = 0;
  }
  else ConversionCount++;

  // Reinitialize for next ADC sequence
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE

  return;
}

void init_eva()
{

// EVA Configure T1PWM, T2PWM, PWM1-PWM6
// Initalize the timers
   // Initalize EVA Timer1
   EvaRegs.T1PR = 0xFFFF;       // Timer1 period
   EvaRegs.T1CMPR = 0x3C00;     // Timer1 compare
   EvaRegs.T1CNT = 0x0000;      // Timer1 counter
   // TMODE = continuous up/down
   // Timer enable
   // Timer compare enable
   EvaRegs.T1CON.all = 0x1042;


  // Initalize EVA Timer2
  EvaRegs.T2PR = 0xFFFF;       // Timer2 period
  EvaRegs.T2CMPR = 0x3C00;     // Timer2 compare
  EvaRegs.T2CNT = 0x0000;      // Timer2 counter
  // TMODE = continuous up/down
  // Timer enable
  // Timer compare enable
  EvaRegs.T2CON.all = 0x1042;


  // Setup T1PWM and T2PWM
  // Drive T1/T2 PWM by compare logic
  EvaRegs.GPTCONA.bit.TCMPOE = 1;
  // Polarity of GP Timer 1 Compare = Active low
  EvaRegs.GPTCONA.bit.T1PIN = 1;
  // Polarity of GP Timer 2 Compare = Active high
  EvaRegs.GPTCONA.bit.T2PIN = 2;

  // Enable compare for PWM1-PWM6
  EvaRegs.CMPR1 = 0x0C00;
  EvaRegs.CMPR2 = 0x3C00;
  EvaRegs.CMPR3 = 0xFC00;

  // Compare action control.  Action that takes place
  // on a cmpare event
  // output pin 1 CMPR1 - active high
  // output pin 2 CMPR1 - active low
  // output pin 3 CMPR2 - active high
  // output pin 4 CMPR2 - active low
  // output pin 5 CMPR3 - active high
  // output pin 6 CMPR3 - active low
  EvaRegs.ACTRA.all = 0x0666;
  EvaRegs.DBTCONA.all = 0x0000; // Disable deadband
  EvaRegs.COMCONA.all = 0xA600;


}

void init_evb()
{

// EVB Configure T3PWM, T4PWM and PWM7-PWM12
// Step 1 - Initialize the Timers

  // Initialize EVB Timer3
  // Timer3 controls T3PWM and PWM7-12
  EvbRegs.T3PR = 0xFFFF;       // Timer3 period
  EvbRegs.T3CMPR = 0x3C00;     // Timer3 compare
  EvbRegs.T3CNT = 0x0000;      // Timer3 counter
  // TMODE = continuous up/down
  // Timer enable
  // Timer compare enable
  EvbRegs.T3CON.all = 0x1042;

  // Initialize EVB Timer4
  // Timer4 controls T4PWM
  EvbRegs.T4PR = 0xFFFF;       // Timer4 period
  EvbRegs.T4CMPR = 0x3C00;     // Timer4 compare
  EvbRegs.T4CNT = 0x0000;      // Timer4 counter
  // TMODE = continuous up/down
  // Timer enable
  // Timer compare enable
  EvbRegs.T4CON.all = 0x1042;

  // Setup T3PWM and T4PWM
  // Drive T3/T4 PWM by compare logic
  EvbRegs.GPTCONB.bit.TCMPOE = 1;
  // Polarity of GP Timer 3 Compare = Active low
  EvbRegs.GPTCONB.bit.T3PIN = 1;
  // Polarity of GP Timer 4 Compare = Active high
  EvbRegs.GPTCONB.bit.T4PIN = 2;

  // Enable compare for PWM7-PWM12
  EvbRegs.CMPR4 = 0x0C00;
  EvbRegs.CMPR5 = 0x3C00;
  EvbRegs.CMPR6 = 0xFC00;

  // Compare action control.  Action that takes place
  // on a cmpare event
  // output pin 1 CMPR4 - active high
  // output pin 2 CMPR4 - active low
  // output pin 3 CMPR5 - active high
  // output pin 4 CMPR5 - active low
  // output pin 5 CMPR6 - active high
  // output pin 6 CMPR6 - active low
  EvbRegs.ACTRB.all = 0x0666;
  EvbRegs.DBTCONB.all = 0x0000; // Disable deadband
  EvbRegs.COMCONB.all = 0xA600;
}

// Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void scia_echoback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

    SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all =0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA =1;
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;

    SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
    SciaRegs.SCILBAUD    =0x00E7;

    SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

void scib_echoback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

    ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.all =0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA =1;
    ScibRegs.SCICTL2.bit.RXBKINTENA =1;

    ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
    ScibRegs.SCILBAUD    =0x00E7;

    ScibRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

// Transmit a character from the SCI
void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    SciaRegs.SCITXBUF=a;

}

void scib_xmit(int a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0) {}
    ScibRegs.SCITXBUF=a;

}

void scia_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}

void scib_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scib_xmit(msg[i]);
        i++;
    }
}

// Initalize the SCI FIFO
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x204f;
    SciaRegs.SCIFFCT.all=0x0;

}

void scib_fifo_init()
{
    ScibRegs.SCIFFTX.all=0xE040;
    ScibRegs.SCIFFRX.all=0x204f;
    ScibRegs.SCIFFCT.all=0x0;

}

//===========================================================================
// No more.
//===========================================================================
