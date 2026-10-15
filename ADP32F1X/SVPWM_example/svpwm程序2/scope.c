
unsigned ua;
#include"svpwm.h"
extern SVPWM svpwm;

void scope(void)
{ 
  unsigned t1,tm,t0;
    t1=svpwm.t1;
    tm=svpwm.tm;
    t0=0xffff-t1-tm;
  switch(svpwm.vect)
  {
    case 1:ua=t0; break;
    case 3:ua=0;  break;
    case 2:ua=tm+t0; break;
    case 6:ua=t1+tm; break; 
    case 4:ua=0xffff; break;
    case 5:ua=t1;     break;
    default:  break;
   }
 
     

}