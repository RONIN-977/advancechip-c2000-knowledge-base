/******************************* Copyright(C) *********************************************
*
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   dac.c
*                         TITLE:  C28x DAC driver.
*******************************************************************************************/

#include "dac.h"

//*****************************************************************************
//
// DAC_tuneOffsetTrim()
//
//*****************************************************************************

void
DAC_tuneOffsetTrim(uint32_t base, float32_t referenceVoltage)
{
    uint16_t oldOffsetTrim;
    float32_t newOffsetTrim;

    //
    // Check the arguments.
    //
    ASSERT(DAC_isBaseValid(base));
    ASSERT(referenceVoltage > 0U);

    //
    // Get the sign-extended offset trim value
    //
    oldOffsetTrim = (HWREGH(base + DAC_O_TRIM) & DAC_TRIM_OFFSET_TRIM_M);
    oldOffsetTrim = ((oldOffsetTrim & (uint16_t)DAC_REG_BYTE_MASK) ^
                    (uint16_t)0x80) - (uint16_t)0x80;

    //
    // Calculate new offset trim value if DAC is operating at a reference
    // voltage other than 2.5v.
    //
    newOffsetTrim = ((float32_t)(2.5 / referenceVoltage) *
                     (int16_t)oldOffsetTrim);

    //
    // Check if the new offset trim value is valid
    //
    ASSERT(((int16_t)newOffsetTrim > -129) && ((int16_t)newOffsetTrim < 128));

    //
    // Set the new offset trim value
    //
    EALLOW;
    HWREGH(base + DAC_O_TRIM) = (HWREGH(base + DAC_O_TRIM) &
                                 ~DAC_TRIM_OFFSET_TRIM_M) |
                                 (int16_t)newOffsetTrim;

    EDIS;

}

