#define DISABLE_INTS()	  asm ("  setc  intm")
#define ENABLE_INTS()	  asm ("  clrc  intm")
#include "pnl.h"
  PNL pnl=PNL_DEFAULTS;
#include "ev.h"
 extern EV ev;
void init_reg(void);
//void rtmon_init(void);
#include "sysvecs.h"


void main(void)
{
//rtmon_init();  //ÊµÊ±·ÂÕæ
DISABLE_INTS();
init_reg();
ev.init(&ev);
ENABLE_INTS();
pnl.run=1;

 for(;;)
  { 
    pnl.calc(&pnl);
  
   if(pnl.run)
    ev.open(&ev);
   else
    ev.close(&ev);
  }//end of for(;;)
  
}//end of main()

//void rtmon_init(void)
//{
 // asm ("  CALL MON_RT_CNFG");
//}
