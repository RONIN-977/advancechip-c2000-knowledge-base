#ifndef DELAY_H_H_
#define	DELAY_H_H_

#include "ADP32F03x_Project.h"

#define Delay_US()		DELAY_US(1)
#define Delay_nUS(n)	DELAY_US(n)

void Delay_nMS(Uint16 nms);

#endif
