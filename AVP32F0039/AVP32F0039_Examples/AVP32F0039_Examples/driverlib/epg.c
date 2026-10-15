/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   epg.c
*                         TITLE:  C28x EPG driver.
*******************************************************************************************/

#include "epg.h"

//
// Select EPG Data Output
//
void EPG_selectEPGDataOut(uint32_t muxbase, EPG_DataIn dataIn,
                          EPG_DataOutSel dataOutSel)
{
    ASSERT(EPG_isMUXBaseValid(muxbase));

    if((uint16_t)dataIn <= 31U)
    {
        HWREG(muxbase + EPG_O_MXSEL0) = ((HWREG(muxbase + EPG_O_MXSEL0) &
                                         ~(1U << (uint32_t)dataIn)) |
                                         ((uint32_t)dataOutSel <<
                                          (uint32_t)dataIn));
    }
}
