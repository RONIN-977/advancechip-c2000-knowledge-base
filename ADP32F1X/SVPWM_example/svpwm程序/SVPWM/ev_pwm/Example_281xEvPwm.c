//###########################################################################
//
// FILE:   Example_281xEvSVPWM.c
//
// TITLE:  DSP281x Event Manager SVPWM Generation.
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File
#include "math.h"
float Ualfa[200],Ubeta[200];    //array Uout :Ud,Uq
int   sector[200];              //define the sector array
#define PI2 2*3.1415926         //MacroDefine 2pi  
#define DETA (PI2)/200          //Incream Angle Index
#define INIA 3.1415926/180      //the Start Angle
#define TP   1200               //Maro TMER1 TP=Tsvpwm/2,according to frequence 
#define KP    0.7               //dedine normalize Uout,between 0 to 1
// Prototype statements for functions found within this file.
void init_eva(void);
void calu();
void SECTOR();
void interrupt nothing();
// the main function
void main(void)
{ int i,k,cmp1,cmp2;
  float t1[200],t2[200];
  Uint16  anticlk[6]={0x1666,0x3666,0x2666,0x6666,0x4666,0x5666};
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO: 
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


// The shell ISR routines are found in DSP281x_DefaultIsr.c.
// This function is found in DSP281x_PieVect.c.
   InitPieVectTable();
   
//call init_eva()
   init_eva();
   
// Step 4. User specific code, enable interrupts:
   calu();
   SECTOR();
  // Just sit and loop forever:
  // PWM pins can be observed with a scope.	
  while(1)
  {for(i=0;i<200;i++)
  {
	  EvaRegs.ACTRA.all=anticlk[sector[i]-1];//Reload ACTRA,select Basic SVPWM
	  switch(sector[i])
	  { 
		  case 1: t1[i]=sqrt(3)*Ualfa[i]*TP/2;
			      t2[i]=(-sqrt(0.5)*Ualfa[i]+sqrt(2)*Ubeta[i])*TP;
                  
                  break;
          case  2:t1[i]=(-sqrt(6)*Ualfa[i]+sqrt(6)*Ubeta[i])*TP/2;
                  t2[i]=(sqrt(0.5)*Ualfa[i]+sqrt(0.5)*Ubeta[i])*TP;
                  
                  break;
          case  3:t1[i]=-sqrt(1.5)*Ubeta[i]*TP;
			      t2[i]=(sqrt(2)*Ualfa[i]-sqrt(0.5)*Ubeta[i])*TP;
                 
                 break;
          case  4:t1[i]=(-sqrt(1.5))*Ubeta[i]*TP;
			      t2[i]=(-sqrt(2)*Ualfa[i]+sqrt(0.5)*Ubeta[i])*TP;
                 
                 
                 break;
          case  5:t1[i]=(-sqrt(1.5)*Ualfa[i]+sqrt(1.5)*Ubeta[i])*TP;
                  t2[i]=(-sqrt(0.5)*Ualfa[i]-sqrt(0.5)*Ubeta[i])*TP;
                 
                 break;
          case  6:t1[i]=sqrt(0.75)*Ualfa[i]*TP;
				  t2[i]=(sqrt(0.5)*Ualfa[i]-sqrt(2)*Ubeta[i])*TP;
               
              
               break;
          default:break;
		  }
	  cmp1=(int)t1[i];
	  cmp2=(int)(t1[i]+t2[i]);
	  EvaRegs.CMPR1=cmp1;
	  EvaRegs.CMPR2=cmp2;//Relaod the CMPR1/2
	  if(i+k==0) EvaRegs.T1CON.all=EvaRegs.T1CON.all|0x0040;//Restart Timer1,only one
	  while(1){
		  k=EvaRegs.EVAIFRA.all&0x02000;
		  if(k==0x02000) break;} //if Timer3 build InteruptFlag,stop waiting
  }
  }


}

void init_eva()
{

// EVA Configure T1PWM, T2PWM, PWM1-PWM6 
// Initalize the timers
   // Initalize EVA Timer1 
   EvaRegs.T1PR = TP;       // Timer1 period:65535
   EvaRegs.T1CMPR = 0x3C00;     // Timer1 compare:15360
   EvaRegs.T1CNT = 0x0000;      // Timer1 counter:0
   // TMODE = continuous up/down:d12-11=10
   // Timer enable:d6=1
   // Timer compare enable:d1=1
   EvaRegs.T1CON.all = 0x1042;   

  // Setup T1PWM and T2PWM
  // Drive T1/T2 PWM by compare logic
  EvaRegs.GPTCONA.bit.TCMPOE = 1;
  // Polarity of GP Timer 1 Compare = Active low:01
  EvaRegs.GPTCONA.bit.T1PIN = 1;
  

  // Enable compare for PWM1-PWM6?
  EvaRegs.CMPR1 = 0x0C00;
  EvaRegs.CMPR2 = 0x3C00;
  EvaRegs.CMPR3 = 0xFC00;
    
  // Compare action control.  Action that takes place
  // on a cmpare event
  // output pin 1 CMPR1 - active high:01
  // output pin 2 CMPR1 - active low:10
  // output pin 3 CMPR2 - active high:01
  // output pin 4 CMPR2 - active low:10
  // output pin 5 CMPR3 - active high:01
  // output pin 6 CMPR3 - active low:10
  EvaRegs.ACTRA.all = 0x0666;  //D15=0:CCW
  EvaRegs.DBTCONA.all = 0x0000; // Disable deadband
  EvaRegs.COMCONA.all = 0xB600; //Enable SVPWM mode 
} 
//sub-function calu()
void calu()
{
	int i;
	for(i=0;i<200;i++)
	{
		Ualfa[i]=KP*cos(INIA+i*DETA);
		Ubeta[i]=KP*sin(INIA+i*DETA);
	}
}
//sub-function SECTOR()
void SECTOR()
{
	int i;
	float theta;
	for(i=0;i<=200;i++)
	{
		theta=(INIA+i*DETA)*180/3.1415926;//angle
		if(theta>=0&&theta<60) sector[i]=1;
		else if (theta>=60&&theta<120) sector[i]=2;
		else if (theta>=120&&theta<180) sector[i]=3;
		else if (theta>=180&&theta<240) sector[i]=4;
		else if (theta>=240&&theta<300) sector[i]=5;
		else if (theta>=300&&theta<360) sector[i]=6;
		else continue;
	}
}
//sub-function noting()
void interrupt nothing()
{
return ;
}
