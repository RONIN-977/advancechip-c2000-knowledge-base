
#ifndef HEARDFILES_SOURCEHEADER_MCQPR_H
#define HEARDFILES_SOURCEHEADER_MCQPR_H

#include "Main.h"
#include "Parameter.h"
#include "SubPrgInclude.h"

typedef struct
{
    Uint16   QprEnable;
    int32     e_k;
    int32     e_k1;
    int32     e_k2;
    int32     v_k;
    int32     v_k1;
    int32     v_k2;
    int32     v_Deta;
    int32     Kr;
    int32     Wc;
    int32     Ws;
    int32     OmegaPer;
    int32     Wc_Ts_4;
    int32     Kr_Wc_Ts_4;
    int32     Den;
    int32     b0;
    int32     b2;
    int32     a1;
    int32     a2;
}Qpr_Handle_t;

typedef struct OL_STRUCT_DEF{
    int32    Ts;
    Uint16  McSpdState;
    Uint16  OlToClose;
    int32    OL_Running;

    //To Close
    int32    ToClose_Iqref;
    int32    ToClose_Idref;
    int32    ToClose_Vd;
    int32    ToClose_Vq;

    // OL Out
    int32    Curr;
    int32    CurrReal;
    int32    FreqReal;

    int32    OutCurrSet;
    int32    OutOmgSet;
    Uint16  OL_OutPhase;
    Uint16  Phase;
}OL_STRUCT;

/***************************************************************************
  Public Variables
 ***************************************************************************/
extern Qpr_Handle_t  Qpr_q;
extern Qpr_Handle_t  Qpr_d;
extern OL_STRUCT     OL;

/***************************************************************************
  Public Functions
 ***************************************************************************/
void  QPRParameterCal(Qpr_Handle_t *pHandle);
void  OL_ParamReset(void);
void  FadeOverVarsForOL2CL(void);
void  VolAngleCalation1();
int32 QPR_Control(Qpr_Handle_t *pHandle,int32 error, int32 omega);


#endif /* HEARDFILES_SOURCEHEADER_MCQPR_H_ */
