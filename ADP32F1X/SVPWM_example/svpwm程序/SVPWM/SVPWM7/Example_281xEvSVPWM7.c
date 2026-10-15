//###########################################################################
//
// FILE:   Example_281xEvSVPWM7.c
//
// TITLE:  DSP281x Event Manager SVPWM Generation.
//
//###########################################################################


#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File
#include <math.h>
#define  PI2 2*3.1415926        //Macro 2pi
#define  DETA  PI2/200          // Increment Angle
#define  INIA  PI2/360
#define  TP    1200             // Timer1 period=Tsvpwm/2
#define  KP    0.7              //Normalize Uout:between 0 and 1,according with VVVF cuvre
float Ualfa[200],Ubeta[200];
int sector[200];    
// Prototype statements for functions found within this file.
void init_eva(void);
void init_evb(void);
void calu();
void SECTOR();
void interrupt_nothing();


void main(void)
{
 int i,m=1,k;
 Uint16 cmp1,cmp2,cmp3;
 float t1[200],t2[200],t0[200];

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO: 
// This example function is found in the DSP281x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example  

// Initialize only GPAMUX and GPBMUX for this test
   EALLOW;
   // Enable PWM pins
   GpioMuxRegs.GPAMUX.all = 0x00FF; // EVA PWM 1-6  pins
   
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
// The shell ISR routines are found in DSP281x_DefaultIsr.c.
// This function is found in DSP281x_PieVect.c.
   InitPieVectTable();
   
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   init_eva();
  
// Step 5. User specific code, enable interrupts:
   calu();
   SECTOR();
  // Just sit and loop forever:
  // PWM pins can be observed with a scope.	
 while(1)
 {for(i=0;i<200;i++)
    {
     switch(sector[i])
         {case 1:t1[i]=sqrt(3)*Ualfa[i]*TP/2;
                 t2[i]=(-sqrt(0.5)*Ualfa[i]+sqrt(2)*Ubeta[i])*TP;
                 t0[i]=(TP-t1[i]-t2[i])/2;
                 cmp1=(int)t0[i];
				 cmp2=(int)(t0[i]+t1[i]);
				 cmp3=(int)(t0[i]+t1[i]+t2[i]);
                 break;
         case 2:t1[i]=(-sqrt(6)*Ualfa[i]+sqrt(6)*Ubeta[i])*TP/2;
                t2[i]=(sqrt(0.5)*Ualfa[i]+sqrt(0.5)*Ubeta[i])*TP;
				t0[i]=(TP-t1[i]-t2[i])/2;
                cmp1=(int)(t0[i]+t2[i]);
				cmp2=(int)(t0[i]);
				cmp3=(int)(t0[i]+t1[i]+t2[i]);
                break;
        case 3:t1[i]=-sqrt(1.5)*Ubeta[i]*TP;
			   t2[i]=(sqrt(2)*Ualfa[i]-sqrt(0.5)*Ubeta[i])*TP;
               t0[i]=(TP-t1[i]-t2[i])/2;
               cmp1=(int)(t0[i]+t1[i]+t2[i]);
			   cmp2=(int)(t0[i]);
			   cmp3=(int)(t0[i]+t1[i]);
               break;
        case 4:t1[i]=(-sqrt(1.5)*Ubeta[i])*TP;
			   t2[i]=(-sqrt(2)*Ualfa[i]+sqrt(0.5)*Ubeta[i])*TP;
               t0[i]=(TP-t1[i]-t2[i])/2;
               cmp1=(int)(t0[i]+t1[i]+t2[i]);
			   cmp2=(int)(t0[i]+t2[i]);
			   cmp3=(int)(t0[i]);
               break;
       case 5:t1[i]=(-sqrt(1.5)*Ualfa[i]+sqrt(1.5)*Ubeta[i])*TP;
              t2[i]=(-sqrt(0.5)*Ualfa[i]-sqrt(0.5)*Ubeta[i])*TP;
		      t0[i]=(TP-t1[i]-t2[i])/2;
              cmp1=(int)(t0[i]+t1[i]);
			  cmp2=(int)(t1[i]+t2[i]+t0[i]);
			  cmp3=(int)(t0[i]);
              break;
       case 6:t1[i]=sqrt(0.75)*Ualfa[i]*TP;
		      t2[i]=(sqrt(0.5)*Ualfa[i]-sqrt(2)*Ubeta[i])*TP;
              t0[i]=(TP-t1[i]-t2[i])/2;
              cmp1=(int)(t0[i]);
			  cmp2=(int)(t0[i]+t1[i]+t2[i]);
			  cmp3=(int)(t0[i]+t2[i]);
              break;
       }
  
    EvaRegs.CMPR1=cmp1;//Reload CMPR1/2/3
	EvaRegs.CMPR2=cmp2;
	EvaRegs.CMPR3=cmp3;
    if((i+k)==0) EvaRegs.T1CON.all=EvaRegs.T1CON.all&0x0200;//start Timer1,only one
	while(1)
	{ k=EvaRegs.EVAIFRA.all&0x0200;
	  if(k==0x0200) break; //if Timer1 build InterrupFlag,stop waiting
	}
  }
 }
}
void init_eva()
{

// EVA Configure T1PWM, T2PWM, PWM1-PWM6 
// Initalize the timers
   // Initalize EVA Timer1 
   EvaRegs.T1PR = 0xFFFF;       // Timer1 period:65535
   EvaRegs.T1CMPR = 0x3C00;     // Timer1 compare:15360
   EvaRegs.T1CNT = 0x0000;      // Timer1 counter:0
   // TMODE = continuous up/down
   // Timer enable
   // Timer compare enable
   EvaRegs.T1CON.all = 0x1042;   


  // Initalize EVA Timer2 
  EvaRegs.T2PR = 0x0FFF;       // Timer2 period:4095
  EvaRegs.T2CMPR = 0x03C0;     // Timer2 compare:960
  EvaRegs.T2CNT = 0x0000;      // Timer2 counter:0
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

  // Enable compare for PWM1-PWM6?
  EvaRegs.CMPR1 = 0x0C00;//:960
  EvaRegs.CMPR2 = 0x3C00;//:15360
  EvaRegs.CMPR3 = 0xFC00;//:64512
    
  // Compare action control.  Action that takes place
  // on a cmpare event
  // output pin 1 CMPR1 - active high:10
  // output pin 2 CMPR1 - active low:01
  // output pin 3 CMPR2 - active high:10
  // output pin 4 CMPR2 - active low:01
  // output pin 5 CMPR3 - active high:10
  // output pin 6 CMPR3 - active low:01
  EvaRegs.ACTRA.all = 0x0666;
  EvaRegs.DBTCONA.all = 0x0000; // Disable deadband
  EvaRegs.COMCONA.all = 0xA600; // Disable SVPWM mode
  } 


//sub-function calu()
void calu()
{
  int  i;
  for(i=0;i<200;i++)
   {
    Ualfa[i]=KP*cos(INIA+i*DETA);
    Ubeta[i]=KP*sin(INIA+i*DETA);
   }
}
//function:interrupt nothing()
void interrupt_nothing()
{
	return;
}
// sub-function SECTOR()
void SECTOR()
{
	int i;
	float theta;
	for (i=0;i<200;i++)
	{  theta=(INIA+i*DETA)*360/PI2;//use angle value
		if(theta>=0&&theta<60) sector[i]=1;
		else if(theta>=60&&theta<120) sector[i]=2;
		else if (theta>=120&&theta<180) sector[i]=3;
		else if(theta>=180&&theta>240) sector[i]=4;
		else if (theta>=240&&theta>300) sector[i]=5;
		else if (theta>=300&&theta<360) sector[i]=6;
        else break;
	
	}
}
