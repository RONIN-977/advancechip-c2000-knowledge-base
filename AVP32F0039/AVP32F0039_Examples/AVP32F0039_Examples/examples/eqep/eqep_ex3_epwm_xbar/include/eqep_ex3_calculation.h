/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   eqep_ex3_calculation.h
*                         TITLE:  Frequency Measurement Using eQEP
*******************************************************************************************/

#ifndef EQEP_EX3_CALCULATION_H
#define EQEP_EX3_CALCULATION_H

//
// Included Files
//
#include "IQmathLib.h"
#include "driverlib.h"

//
// Typedefs
//
typedef struct
{
    uint32_t freqScalerPR;  // Parameter: Scaler converting 1/N cycles to a
                            // GLOBAL_Q freq (Q0) - independently with global Q
    uint32_t freqScalerFR;  // Parameter: Scaler converting 1/N cycles to a
                            // GLOBAL_Q freq (Q0) - independently with global Q
    uint32_t baseFreq;      // Parameter: Maximum freq

    _iq freqPR;             // Output: Freq in per-unit using capture unit
    int32_t freqHzPR;       // Output: Freq in Hz, measured using Capture unit
    uint32_t oldPos;

    _iq freqFR;             // Output: Freq in per-unit using position counter
    int32_t freqHzFR;       // Output: Freq in Hz, measured using Capture unit
} FreqCal_Object;

typedef FreqCal_Object *FreqCal_Handle;

//
// Function Prototypes
//
void FreqCal_calculate(FreqCal_Handle, uint32_t*);

#endif  // EQEP_EX3_CALCULATION_H

//
// End of File
//

