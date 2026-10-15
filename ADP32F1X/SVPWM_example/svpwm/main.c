#include "DSP281x_Device.h"     
#include "math.h"
#include "float.h"
float     ualfa[200],ubeta[200];
int       sector[20];
#define  PI2  2*3.1415926
#define  DETA  PI2/200
#define  INIA  3.1415926
#define  TP    1200
#define  KP    0.7

void ConfigureGpio(void);
void InitSystem(void);
void calu(void);
void SECTOR(void);
void init_evb(void);

void main(void)
{
 int anticlk[6]={0x1666,0x1666,0x1666,0x1666,0x1666,0x1666};
 int i,k=0,cmp1,cmp2;
 float x,y,z;
 InitSystem();
 ConfigureGpio();
 DINT;
 InitPieCtrl();
 IER = 0x0000;
 IFR = 0x0000;
 InitPieVectTable();
 init_evb();
 calu();
 SECTOR();
 while(1)
  {
   for(i=0;i<200;i++)
   {
    EvbRegs.ACTRB.all=anticlk[sector[i]];//重新装配ACTRB
    x=ubeta[i];
    y=(1.732051*ualfa[i]+ubeta[i])/2;
    z=(-1.732051*ualfa[i]+ubeta[i])/2;
    switch (sector[i])
    {
    case 0:
      cmp1=(int)(-z*TP),cmp2=(int)(x*TP);
      break;
    case 1:
      cmp1=(int)(y*TP),cmp2=(int)(z*TP);
      break;
    case 2:
      cmp1=(int)(x*TP),cmp2=(int)(-y*TP);
      break;
    case 3:
      cmp1=(int)(z*TP),cmp2=(int)(-x*TP);
      break;
    case 4:
      cmp1=(int)(-y*TP),cmp2=(int)(-z*TP);
      break;
    case 5:
      cmp1=(int)(-x*TP),cmp2=(int)(y*TP);
      break;
    default:
      break;
     }
   EvbRegs.CMPR4=cmp1;
   EvbRegs.CMPR5=cmp1+cmp2;
   if(i+k==0)
   EvbRegs.T3CON.bit.TENABLE=1; //启动定时器，且只启动一次
   while(1)
     {
      k=EvbRegs.EVBIFRA.bit.T3UFINT;
      if(k==1)
      break;
     }  
   }
  }
}
void InitSystem(void)
{
 EALLOW;
 SysCtrlRegs.WDCR=0x00E8;//禁止看门狗模块
 SysCtrlRegs.PLLCR.bit.DIV=10;//将CPU的PLL倍频系数设为5
 SysCtrlRegs.HISPCP.all=0x1;  //将高速时钟的预定标器设置成除2模式
 SysCtrlRegs.LOSPCP.all=0x2;  //将低速时钟的预定标器设置成除4模式
 SysCtrlRegs.PCLKCR.bit.EVAENCLK=0;//使能EVA模块
 SysCtrlRegs.PCLKCR.bit.EVBENCLK=1;//禁止EVB模块
 SysCtrlRegs.PCLKCR.bit.SCIAENCLK=0;//使能SCIA模块
 SysCtrlRegs.PCLKCR.bit.SCIBENCLK=0;//禁止SCIB模块
 SysCtrlRegs.PCLKCR.bit.MCBSPENCLK=0;//禁止MCBSP模块
 SysCtrlRegs.PCLKCR.bit.SPIENCLK=0;//禁止SPI模块
 SysCtrlRegs.PCLKCR.bit.ECANENCLK=0;//禁止ECAN模块
 SysCtrlRegs.PCLKCR.bit.ADCENCLK=0;//使能ADCA模块
 EDIS;
}
void ConfigureGpio(void)
{
 EALLOW;
 GpioMuxRegs.GPAMUX.all=0X0000;//
 GpioMuxRegs.GPBMUX.all=0X00FF;//EVB PWM 7-12  pins
 GpioMuxRegs.GPDMUX.all=0X0000;//GPIOD配置为通用IO口
 GpioMuxRegs.GPEMUX.all=0X0000;//GPIOE配置为通用IO口
 GpioMuxRegs.GPFMUX.all=0X0000;//GPIOF4配置为通用IO口
 GpioMuxRegs.GPGMUX.all=0X0000;//GPIOG配置为通用IO口
 EDIS;
}
void init_evb(void)
{

  EvbRegs.T3PR = TP;            
  EvbRegs.T3CNT = 0x0000;     
 
  EvbRegs.T3CON.all = 0x0802;     
    
  EvbRegs.ACTRB.all = 0x0666;
  EvbRegs.DBTCONB.all = 0x0000; 
  EvbRegs.COMCONB.all = 0x9200;
  EvbRegs.EVBIMRA.all=0;
  EvbRegs.EVBIFRA.all=1;
}
void calu(void)
{
 int i;
 for(i=0;i<200;i++)
 {
  ualfa[i]=KP*cos(INIA+i*DETA);
  ubeta[i]=KP*sin(INIA+i*DETA);
 }
}
void SECTOR(void)
{
 int i,a ,b,c;
 float vref1,vref2,vref3;
 for(i=0;i<200;i++)
 {
  vref1=ubeta[i];
  vref2=(-ubeta[i]+ualfa[i]*1.732051)/2;
  vref3=(-1.732051*ualfa[i]-ubeta[i])/2;
  
  if(vref1>0)
  a=1;
  else
  a=0;
  if(vref2>0)
  b=1;
  else
  b=0;
  if(vref3>0)
  c=1;
  else
  c=0;
  a=4*c+2*b+a;
  switch(a)
  {
   case 1:
     sector[i]=1;
     break;
   case 2:
     sector[i]=5;
     break;
   case 3:
     sector[i]=0;
     break;
   case 4:
     sector[i]=3;
     break;
   case 5:
     sector[i]=2;
     break;
   case 6:
     sector[i]=4;
     break;
   default:
     break;
  }
 }
}

