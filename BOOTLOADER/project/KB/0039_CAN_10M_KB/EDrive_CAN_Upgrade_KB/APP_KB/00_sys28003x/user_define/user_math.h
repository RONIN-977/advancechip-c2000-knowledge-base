/*
 * user_math.h
 *
 *  Created on: Feb 15, 2022
 *      Author: rand C
 */

#ifndef USER_MATH_H_
#define USER_MATH_H_

#define GLOBAL_Q    16

#include "IQmathLib.h"
#include "DataTypeDef.h"

extern int  absl(long int);
extern int  qsin(int);
extern int  qatan(long int);
extern int  qatan2(int x, int y);
extern u16  qsqrt(Ulong);

#define     Half_PI         16384
#define     PI_INT          32767
#define     Deg120          21845
#define     Deg2INT     (182.04444F)
#define     INT2Deg     (5.4932E-3F)//(6.9387E-3F)//

#define  u_Max(A,B)    (A > B)?A:B
#define  u_Min(A,B)    (A < B)?A:B

#define  u_Sign(A)    (A>0)?1:(-1)

#endif /* 00_SYS280025_MATH_LIB_USER_MATH_H_ */
