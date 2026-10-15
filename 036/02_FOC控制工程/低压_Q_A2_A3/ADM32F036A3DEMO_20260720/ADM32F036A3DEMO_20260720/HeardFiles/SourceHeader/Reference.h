
/******************************************************************************
  * @file    Reference.h
  * @brief   Frequency source processing interface.
  * @note    This file declares public variables and functions used for
  *          frequency command processing, frequency limit handling, and
  *          power limit control.
  ******************************************************************************
  *****************************************************************************/
#ifndef F_FRQSRC_H
#define F_FRQSRC_H

#include "ADP32F03x_Device.h"

/***************************************************************************
  Public Variables
 ***************************************************************************/
extern int32 frq;
extern int32 frqTmp;
extern int32 frqAim;
extern int32 frqAimTmp;
extern int32 frqAimTmp0;
extern int32 frqCurAim;
extern Uint16 lowerFrq;
extern Uint32 maxFrq;
extern Uint16 upperTorque;

/***************************************************************************
  Public Functions
 ***************************************************************************/
void FrqSrcDeal(void);
void UpdateFrqSetAim(void);

#endif // __F_FRQSRC_H__
