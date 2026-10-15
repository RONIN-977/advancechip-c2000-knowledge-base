
#include "LF2407REGS.h"
#include "math.h"
#include "svpwm.h"
SVPWM svpwm=SVPWM_DEFAULTS;
#include "ev.h"
EV ev=EV_DEFAULTS;
#include "pnl.h"
extern PNL pnl;
void scope(void);


void init_reg(void)
{
 *WSGR=0X0000;
 *IMR=0X0042;
 *IFR=0X0ffff;
 *SCSR1=0X00ad;
 *XINT1CR=*XINT2CR=0X8006;
 *WDCR=0X0068;
 *WDKEY=0X0055;
 *WDKEY=0X00aa;
 *MCRA=0X3ffb;
 *MCRB=0Xfedd;
 *MCRC=0X0ffe;
 *PADATDIR=0;
 *PBDATDIR=0;
 *PCDATDIR=0X2020;
 *PDDATDIR=0;
 *PEDATDIR=0;
 *PFDATDIR=0X4000;
}

interrupt void pwm_int2(void)
{
  if (*PIVR==0x0029) *EVAIFRA=(unsigned int)1<<9;
  else return;
  
  svpwm.f=pnl.f;
  svpwm.ul_max=pnl.ul_max;
  svpwm.udc=pnl.udc;
  svpwm.calc(&svpwm);
  
  ev.vect=svpwm.vect;
  ev.t1=svpwm.t1;
  ev.tm=svpwm.tm;
  ev.pwm(&ev);
  scope();
 }
 

void ev_init (EV_Handle p)
{
  *GPTCONA=*GPTCONB=0;
  *T1CON=*T2CON=*T3CON=*T4CON=0;
  *T1PR=*T2PR=*T3PR=*T4PR=0;
  *T1CNT=*T2CNT=*T3CNT=*T4CNT=0;
  *T1CMPR=*T2CMPR=*T3CMPR=*T4CMPR=0;
  
  *COMCONA=*COMCONB=0;
  *ACTRA=*ACTRB=0;
  *DBTCONA=*DBTCONB=0;
  *CMPR1=*CMPR2=*CMPR3=*CMPR4=*CMPR5=*CMPR6=0;
  *EVAIMRA=*EVAIMRB=*EVAIMRC=*EVBIMRA=*EVBIMRB=*EVBIMRC=0;
  *EVAIFRA=*EVAIFRB=*EVAIFRC=*EVBIFRA=*EVBIFRB=*EVBIFRC=0XFFFF;
  
  *T1PR=TCBY2;
  *T1CON=0X0840;
  
  *ACTRA =0X0666;

  *DBTCONA =0X0af4;

  *EVAIMRA=0X0200;
}

void ev_pwm(EV_Handle p)
{
  *ACTRA=(*ACTRA&0X8FFF)|(p->vect<<12);
  *CMPR1=((unsigned long)p->t1*TCBY2)>>16;
  *CMPR2=((unsigned long)(p->t1+p->tm)*TCBY2)>>16;
  
}
 
 
 
void ev_open(EV_Handle p)
{
  *COMCONA=0X9200;
 // asm ("setc xf");
}

void ev_close(EV_Handle p)
{
  *COMCONA=0X9000;
  //asm ("clrc xf");
}


