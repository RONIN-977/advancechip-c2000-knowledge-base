#include "svpwm.h"
#include "math.h"
void svpwm_calc(SVPWM_Handle p)
{
  int step;
  unsigned k;
  step=(int)( ((unsigned long)p->f*p->step_max)>>15);
  p->alpha=p->alpha + step;
if(p->alpha>PIBY3)
{
  p->alpha=p->alpha - PIBY3;
  if(p->sector<5)
     p->sector++;
  else
     p->sector=0;
}
 switch(p->sector)
{ 
  case 0:p->vect=1;  break;
  case 1:p->vect=3;   break;
  case 2:p->vect=2; break;
  case 3:p->vect=6; break;
  case 4:p->vect=4; break;
  case 5:p->vect=5;     break;
  default:         break;
}
if(p->ul_max<p->udc)
   k=(unsigned)(  ((unsigned long)p->ul_max<<16)/p->udc);
else
   k=0xffff;
p->t1 =(unsigned) ( ((unsigned long)k * qsinlt(PIBY3 - p->alpha))>>15  );
p->tm =(unsigned) ( ((unsigned long)k * qsinlt(p->alpha) )>>15   );
}