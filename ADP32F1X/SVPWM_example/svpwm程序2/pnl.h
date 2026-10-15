#ifndef __PNL_H__
#define __PNL_H__

#define U_QMAX 1024
#define F_QMAX 128

typedef struct 
{ 
 void(*calc)(void*);
  unsigned rf;
  unsigned rul;
  unsigned rudc;
  unsigned run;

  unsigned f;
  unsigned ul_max;
  unsigned udc;
}PNL;

#define PNL_DEFAULTS {     \
 (void(*)(void*))pnl_calc,\
  50,                     \
  180,                   \
  310,                   \
  0  ,                    \
}
typedef PNL *PNL_Handle;
void pnl_calc(PNL_Handle);
#endif