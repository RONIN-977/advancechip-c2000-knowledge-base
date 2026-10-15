
#ifndef __F_FRQSRC_H__
#define __F_FRQSRC_H__

#include "ADP32F03x_Device.h"


extern int32 frq;
extern int32 frqTmp;
extern int32 frqAim;
extern int32 frqAimTmp;
extern int32 frqAimTmp0;
extern int32 frqCurAim;
extern Uint32 upperFrq;
extern Uint16 lowerFrq;
extern Uint32 maxFrq;
extern Uint16 upperTorque;
extern Uint16 frqSrc;


void FrqSrcDeal(void);
void UpdateFrqSetAim(void);
int32 UpdateMultiStepFrq(Uint16 step);

#endif // __F_FRQSRC_H__
