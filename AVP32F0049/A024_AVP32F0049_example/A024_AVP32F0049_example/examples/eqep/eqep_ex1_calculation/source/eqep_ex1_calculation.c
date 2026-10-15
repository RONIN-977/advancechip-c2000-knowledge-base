/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   eqep_ex1_calculation.c
*                         TITLE:  Frequency Measurement Using eQEP
*******************************************************************************************/
//
// Included Files
//
#include "eqep_ex1_calculation.h"

//
// FreqCal_calculate - Function to calculate the frequency of the input signal using both the unit
// timer and the quadrature capture units.
//
// For a more detailed explanation of the calculations, read the description at
// the top of this file.
//
void
FreqCal_calculate(FreqCal_Object *p)
{
    uint32_t temp;
    _iq newPosCnt, oldPosCnt;

    //
    // **** Frequency calculation using eQEP position counter ****
    //
    //  Check for unit time out event
    //
    if((EQEP_getInterruptStatus(EQEP1_BASE) & EQEP_INT_UNIT_TIME_OUT) != 0)
    {
        //
        // Get latched POSCNT value
        //
        newPosCnt = EQEP_getPositionLatch(EQEP1_BASE);
        oldPosCnt = p->oldPos;

        if(newPosCnt > oldPosCnt)
        {
            //
            // x2 - x1 in v = (x2 - x1) / T equation
            //
            temp = newPosCnt - oldPosCnt;
        }
        else
        {
            temp = (0xFFFFFFFF - oldPosCnt) + newPosCnt;
        }

        //
        // p->freqFR = (x2 - x1) / (T * 10kHz)
        //
        p->freqFR = _IQdiv(temp, p->freqScalerFR);
        temp=p->freqFR;

        //
        // Is freq greater than max freq (10kHz for this example)?
        //
        if(temp >= _IQ(1))
        {
            p->freqFR = _IQ(1);
        }
        else
        {
            p->freqFR = temp;
        }

        //
        // Q0 = Q0 * GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
        // p->freqHzFR = (p->freqFR) * 10kHz = (x2 - x1) / T
        //
        p->freqHzFR = _IQmpy(p->baseFreq,p->freqFR);

        //
        // Update old position counter value and clear unit time out flag
        //
        p->oldPos = newPosCnt;
        EQEP_clearInterruptStatus(EQEP1_BASE, EQEP_INT_UNIT_TIME_OUT);
    }

    //
    // **** Frequency calculation using eQEP capture counter ****
    //
    // Check for unit position event
    //
    if((EQEP_getStatus(EQEP1_BASE) & EQEP_STS_UNIT_POS_EVNT) != 0)
    {
        //
        // No capture overflow
        //
        if((EQEP_getStatus(EQEP1_BASE) & EQEP_STS_CAP_OVRFLW_ERROR) == 0)
        {
            temp = (uint32_t)EQEP_getCapturePeriodLatch(EQEP1_BASE);
        }
        else
        {
            //
            // Capture overflow, saturate the result
            //
            temp = 0xFFFF;
        }

        //
        // p->freqPR = X / [(t2 - t1) * 10kHz]
        //
        p->freqPR = _IQdiv(p->freqScalerPR, temp);
        temp = p->freqPR;

        if(temp > _IQ(1))
        {
            p->freqPR = _IQ(1);
        }
        else
        {
            p->freqPR = temp;
        }

        //
        // Q0 = Q0 * GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
        // p->freqHzPR = (p->freqPR) * 10kHz = X / (t2 - t1)
        //
        p->freqHzPR = _IQmpy(p->baseFreq, p->freqPR);

        //
        // Clear unit position event flag and overflow error flag
        //
        EQEP_clearStatus(EQEP1_BASE, (EQEP_STS_UNIT_POS_EVNT |
                                      EQEP_STS_CAP_OVRFLW_ERROR));
    }
}

//
// End of File
//

