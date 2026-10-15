#ifndef __SVPWM_H__
#define __SVPWM_H__

#define PIBY3 10923

typedef struct
{
 void(*calc)(void*);
 
  unsigned step_max;
 
  int alpha;
  unsigned sector;
 
  unsigned f;
  unsigned ul_max;
  unsigned udc;
  
  unsigned t1;
  unsigned tm;
  unsigned vect;
}SVPWM;

#define SVPWM_DEFAULTS {      \
 (void(*)(void*)) svpwm_calc, \
  839,                       \
  0,                         \
  0                          \
}

typedef SVPWM *SVPWM_Handle;
void svpwm_calc(SVPWM_Handle);
#endif