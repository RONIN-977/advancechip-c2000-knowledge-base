//###########################################################################
//
// FILE:   Example_281xEvSVPWM.c
//
// TITLE:  DSP281x Event Manager SVPWM Generation.
//
//###########################################################################


#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include 
#include <stdio.h>
#include <math.h>
float Ualfa[200],Ubeta[200];
int sector[200];
#define PI2 3.1415926*2
#define DETA  PI2/200 
#define INIA  PI2/360
int TP; 
float KP;
extern Uint16 Frequence;
extern Uint16 Voltage;
extern float theta;
// Prototype statements for functions found within this file.
void init_eva(void);

// Global counts used in this example


void main(void)
{
   float t;
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
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP281x_DefaultIsr.c.
// This function is found in DSP281x_PieVect.c.
   InitPieVectTable();
   PieVectTable.ADCINT = &adc_isr;
   PieVectTable.T1OFINT =&SVPWM;
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   init_eva();
   

// Step 5. User specific code, enable interrupts:

   DSP281x_ADCIO();
   t=(EvaRegs.T1CNT)/
   theta=INIA+PI2*Frequence*t;
   if (theta>PI2) theta-=PI2;
  // Just sit and loop forever:
  // PWM pins can be observed with a scope.	
  for(;;);

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
  EvaRegs.COMCONA.all = 0xA600; // Disable SVPWM mode,enable PWM output,下益/比较中断
  

} 
// function SVPWM(Uint16 Frequnce[i],Uint16 Voltage[i],int i)

interrupt void SVPWM() //实时计算各扇区矢量作用时间
{ int m=1,cmp1,cmp2,cmp3;
  int i,k=0;
  KP=(float)(Volatag/220);
  Ualfa[i]=KP*cos(theta);
  Ubeta[i]=KP*sin(theta);

 
  
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
	while(m==1)
	{ k=EvaRegs.EVAIFRA.all&0x0200;
	  if(k==0x0200) break; //if Timer1 build InterrupFlag,stop waiting
	}
  }
 }
}
void SECTOR()  //实时计算扇区
{    how t?
	int i;
	
	for (i=0;i<200;i++)
	{  theta=(INIA+PI2*Frequence*t)*360/PI2;//use angle value
		if(theta>=0&&theta<60) sector[i]=1;
		else if(theta>=60&&theta<120) sector[i]=2;
		else if (theta>=120&&theta<180) sector[i]=3;
		else if(theta>=180&&theta>240) sector[i]=4;
		else if (theta>=240&&theta>300) sector[i]=5;
		else if (theta>=300&&theta<360) sector[i]=6;
        else break;
	
	}
}