#include "pnl.h"
#include "math.h"
void pnl_calc(PNL_Handle p)
{
  unsigned rulmax;
  
  if (p->rf>=F_QMAX)
      p->f=0x7fff;
  else 
      p->f =(unsigned)( ((unsigned long)p->rf<<15)/F_QMAX);
  if (p->rudc>=U_QMAX)
      p->udc=0x7fff;
  else 
      p->udc =(unsigned)( ((unsigned long)p->rudc<<15)/U_QMAX); 
  rulmax=(unsigned)( ((unsigned long)p->rul*46341)>>15);    
  if (rulmax>=U_QMAX)
     p->ul_max=0x7fff;
  else
     p->ul_max=(unsigned)( ((unsigned long)rulmax<<15)/U_QMAX);
}