/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  eqep_ex1_calculation.c
*                         TITLE: Position and Speed Measurement Using eQEP
*******************************************************************************************/

//
// Included Files
//
#include "eqep_ex2_calculation.h"

//
// Function to calculate the frequency of the input signal using both the unit
// timer and the quadrature capture units.
//
// For a more detailed explanation of the calculations, read the description at
// the top of this file.
//
void PosSpeed_calculate(PosSpeed_Object *p)
{
    int32_t temp;
    uint16_t pos16bVal, temp1;
    _iq temp2, newPosCnt, oldPosCnt;

    //
    // **** Position calculation - mechanical and electrical motor angle ****
    //
    // Get the motor direction: -1 = CCW/reverse, 1 = CW/forward
    //
    p->directionQEP = EQEP_getDirection(EQEP1_BASE);

    //
    // Capture position once per QA/QB period
    //
    pos16bVal = (uint16_t)EQEP_getPosition(EQEP1_BASE);

    //
    // Raw theta = current pos. + ang. offset from QA
    //
    p->thetaRaw = pos16bVal + p->calAngle;

    //
    // The following lines calculate
    //
    // p->thetaMech ~= QPOSCNT / mechScaler [current cnt/(total cnt in 1 rev)]
    //
    // where mechScaler = 4000 cnts/revolution
    //
    temp = (int32_t)p->thetaRaw * (int32_t)p->mechScaler;   // Q0 * Q26 = Q26
    temp &= 0x03FFF000;

    p->thetaMech = (int16_t)(temp >> 11);                    // Q26 -> Q15
    p->thetaMech &= 0x7FFF;

    //
    // The following lines calculate p->elec_mech
    //
    p->thetaElec = p->polePairs * p->thetaMech;               // Q0 * Q15 = Q15
    p->thetaElec &= 0x7FFF;

    //
    // Check for an index occurrence
    //
    if((EQEP_getInterruptStatus(EQEP1_BASE) & EQEP_INT_INDEX_EVNT_LATCH) != 0U)
    {
        EQEP_clearInterruptStatus(EQEP1_BASE, EQEP_INT_INDEX_EVNT_LATCH);
    }

    //
    // **** High Speed Calculation using QEP Position counter ****
    //
    // Check for unit timeout event
    //
    if((EQEP_getInterruptStatus(EQEP1_BASE) & EQEP_INT_UNIT_TIME_OUT) != 0)
    {
        //
        // The following lines calculate position:
        // (x2 - x1) / 4000 (position in 1 revolution)
        //

        pos16bVal = (uint16_t)EQEP_getPositionLatch(EQEP1_BASE);
        temp = (int32_t)pos16bVal * (int32_t)p->mechScaler; // Q0 * Q26 = Q26
        temp &= 0x03FFF000;

        temp = (int16_t)(temp >> 11);                        // Q26 -> Q15
        temp &= 0x7FFF;

        newPosCnt = _IQ15toIQ(temp);
        oldPosCnt = p->oldPos;

        //
        // POSCNT is counting down
        //
        if(p->directionQEP == -1)
        {
            if(newPosCnt > oldPosCnt)
            {
                //
                // x2 - x1 should be negative
                //
                temp2 = -(_IQ(1) - newPosCnt + oldPosCnt);
            }
            else
            {
                temp2 = newPosCnt -oldPosCnt;
            }
        }
        //
        // POSCNT is counting up
        //
        else if(p->directionQEP == 1)
        {
            if(newPosCnt < oldPosCnt)
            {
                temp2 = _IQ(1) + newPosCnt - oldPosCnt;
            }
            else
            {
                //
                // x2 - x1 should be positive
                //
                temp2 = newPosCnt - oldPosCnt;
            }
        }

        if(temp2 > _IQ(1))
        {
            p->speedFR = _IQ(1);
        }
        else if(temp2 < _IQ(-1))
        {
            p->speedFR = _IQ(-1);
        }
        else
        {
            p->speedFR = temp2;
        }

        //
        // Update the electrical angle
        //
        p->oldPos = newPosCnt;

        //
        // Change motor speed from pu value to rpm value (Q15 -> Q0)
        // Q0 = Q0*GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
        //
        p->speedRPMFR = _IQmpy(p->baseRPM, p->speedFR);

        //
        // Clear unit time out flag
        //
        EQEP_clearInterruptStatus(EQEP1_BASE, EQEP_INT_UNIT_TIME_OUT);
    }

    //
    // **** Low-speed computation using QEP capture counter ****
    //
    // Check for unit position event
    //
    if((EQEP_getStatus(EQEP1_BASE) & EQEP_STS_UNIT_POS_EVNT) != 0)
    {
        //
        // No Capture overflow
        //
        if((EQEP_getStatus(EQEP1_BASE) & EQEP_STS_CAP_OVRFLW_ERROR) == 0)
        {
            temp1 = (uint32_t)EQEP_getCapturePeriodLatch(EQEP1_BASE);
        }
        else
        {
            //
            // Capture overflow, saturate the result
            //
            temp1 = 0xFFFF;
        }

        //
        // p->speedPR = p->speedScaler / temp1
        //
        p->speedPR = _IQdiv(p->speedScaler, temp1);
        temp2 = p->speedPR;

        if(temp2 > _IQ(1))
        {
           p->speedPR = _IQ(1);
        }
        else
        {
           p->speedPR = temp2;
        }

        //
        // Convert p->speedPR to RPM
        //
        // Reverse direction = negative
        //
        if(p->directionQEP == -1)
        {
            //
            // Q0 = Q0 * GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
            //
            p->speedRPMPR = -_IQmpy(p->baseRPM, p->speedPR);
        }
        //
        // Forward direction = positive
        //
        else
        {
            //
            // Q0 = Q0 * GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
            //
            p->speedRPMPR = _IQmpy(p->baseRPM, p->speedPR);
        }

        //
        // Clear unit position event flag and overflow error flag
        //
        EQEP_clearStatus(EQEP1_BASE, (EQEP_STS_UNIT_POS_EVNT |
                                      EQEP_STS_CAP_OVRFLW_ERROR));
    }
}
