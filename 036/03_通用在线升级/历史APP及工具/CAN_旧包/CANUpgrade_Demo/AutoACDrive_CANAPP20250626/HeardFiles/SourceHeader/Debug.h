#ifndef __F_DEBUG_H__
#define __F_DEBUG_H__

#include "FuncCode.h"

#ifdef FUNC_DEBUG_ONLY              // 不是仅调试功能，在CCS的build option中定义的宏
#define FUNC_DEBUG          1        // 功能部分的调试
#else
#define FUNC_DEBUG          0
#endif

//#if FUNC_DEBUG && (!defined(_lint))
#if 1
#define LOCALF
#define LOCALD extern
#else
#define LOCALF static
#define LOCALD static
#endif

//=====================================================================
// DSP 频率: 60MHz/100MHz. 晶振是20MHz
#if (DSP_CLOCK == 100)
#define TIME_10US       1000UL
#define TIME_50US       5000UL
#define TIME_100US      10000UL // 0.1ms对应的定时器计数值（定时器1 10ns单位）
#define TIME_1MS        100000UL
#elif (DSP_CLOCK == 60)
#define TIME_10US       600UL
#define TIME_50US       3000UL
#define TIME_100US      6000UL  // 0.1ms对应的定时器计数值（定时器1 100/6ns单位）
#define TIME_1MS        60000UL
#endif
//=====================================================================

// dest的bit1 = source的bit2
#define BitAssign(source, bit1, dest, bit2)     \
{                                               \
    if ((source) & (0x1UL << (bit1)))             \
        (dest) |= (0x1UL << (bit2));              \
    else                                        \
        (dest) &= (~(0x1UL << (bit2)));           \
}

Uint16 qsqrt(Uint32 data);

#ifdef FUNC_DEBUG_ONLY
#endif


#endif  // #ifndef __F_DEBUG_H__



