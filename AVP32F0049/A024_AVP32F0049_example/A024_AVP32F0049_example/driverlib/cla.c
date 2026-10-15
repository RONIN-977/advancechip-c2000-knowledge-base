/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   cla.c
*                         TITLE:  CLA Driver Implementation File
*******************************************************************************************/

#include "cla.h"

//*****************************************************************************
//
// CLA_setTriggerSource()
//
//*****************************************************************************
void
CLA_setTriggerSource(CLA_TaskNumber taskNumber, CLA_Trigger trigger)
{
    uint32_t srcSelReg;
    uint32_t shiftVal;

    //
    // Calculate the shift value for the specified task.
    //
    shiftVal = ((uint32_t)taskNumber * SYSCTL_CLA1TASKSRCSEL1_TASK2_S) % 32U;

    //
    // Calculate the register address for the specified task.
    //
    if(taskNumber <= CLA_TASK_4)
    {
        //
        // Tasks 1-4
        //
        srcSelReg = (uint32_t)DMACLASRCSEL_BASE + SYSCTL_O_CLA1TASKSRCSEL1;
    }
    else
    {
        //
        // Tasks 5-8
        //
        srcSelReg = (uint32_t)DMACLASRCSEL_BASE + SYSCTL_O_CLA1TASKSRCSEL2;
    }

    EALLOW;

    //
    // Write trigger selection to the appropriate register.
    //
    HWREG(srcSelReg) &= ~((uint32_t)SYSCTL_CLA1TASKSRCSEL1_TASK1_M
                           << shiftVal);
    HWREG(srcSelReg) = HWREG(srcSelReg) | ((uint32_t)trigger << shiftVal);

    EDIS;
}
