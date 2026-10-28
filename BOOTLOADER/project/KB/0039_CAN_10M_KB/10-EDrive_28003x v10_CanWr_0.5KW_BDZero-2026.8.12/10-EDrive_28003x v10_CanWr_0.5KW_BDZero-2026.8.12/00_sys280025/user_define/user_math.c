/*
 * user_math.c
 *
 *  Created on: Feb 16, 2022
 *      Author: rand C
 */

#include "user_math.h"

#pragma CODE_SECTION(qatan2, ".TI.ramfunc");

int qatan2(int x, int y)
{
    int  result;
    long m_Input;

    if(x == 0)
    {
        if(y < 0)
        {
            return(-16384);
        }
        else
        {
            return(16384);
        }
    }
    m_Input = (((long)y)<<16)/x;
    result = qatan(m_Input);
    if(x < 0)
    {
        result += 32768;
    }
    return result;
}

