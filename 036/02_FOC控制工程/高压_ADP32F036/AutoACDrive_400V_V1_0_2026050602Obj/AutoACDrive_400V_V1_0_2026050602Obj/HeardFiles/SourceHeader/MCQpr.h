/*
 * MCQpr.h
 *
 *  Created on: 2026Äê2ÔÂ28ÈÕ
 *      Author: Advancechip
 */

#ifndef HEARDFILES_SOURCEHEADER_MCQPR_H_
#define HEARDFILES_SOURCEHEADER_MCQPR_H_

#include "MainInclude.h"
#include "Parameter.h"
#include "SubPrgInclude.h"

typedef struct
{
    Uint16   QprEnable;
    long     e_k;
    long     e_k1;
    long     e_k2;
    long     v_k;
    long     v_k1;
    long     v_k2;
    long     v_Deta;

    long     Kr;
    long     Wc;
    long     Ws;
    long     OmegaPer;

    long     Wc_Ts_4;
    long     Kr_Wc_Ts_4;

    long     Den;
    long     b0;
    long     b2;
    long     a1;
    long     a2;
}Qpr_Handle_t;

extern Qpr_Handle_t  Qpr_q;
extern Qpr_Handle_t  Qpr_d;

void QPRParameterCal(Qpr_Handle_t *pHandle);
long QPR_Control(Qpr_Handle_t *pHandle,long error, long omega);
void QPRReset( Qpr_Handle_t *pHandle );

#endif /* HEARDFILES_SOURCEHEADER_MCQPR_H_ */
