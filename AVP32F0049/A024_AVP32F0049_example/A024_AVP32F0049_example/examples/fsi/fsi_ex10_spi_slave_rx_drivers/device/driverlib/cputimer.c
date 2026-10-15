/******************************* Copyright(C) *********************************************
*
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   cputimer.c
*                         TITLE:  C28x CPU timer Driver
*******************************************************************************************/

#include "cputimer.h"

//*****************************************************************************
//
// CPUTimer_setEmulationMode
//
//*****************************************************************************
void CPUTimer_setEmulationMode(uint32_t base, CPUTimer_EmulationMode mode)
{
    ASSERT(CPUTimer_isBaseValid(base));
    //
    // Write to FREE_SOFT bits of register TCR
    //
      HWREGH(base + CPUTIMER_O_TCR) =
            (HWREGH(base + CPUTIMER_O_TCR) &
            ~(CPUTIMER_TCR_FREE | CPUTIMER_TCR_SOFT)) |
            (uint16_t)mode;
}

