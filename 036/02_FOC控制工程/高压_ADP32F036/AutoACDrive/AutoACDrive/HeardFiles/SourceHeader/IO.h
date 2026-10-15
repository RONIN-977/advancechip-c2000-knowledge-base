#ifndef __F_IO_H__
#define __F_IO_H__


#include "FuncCode.h"

//=====================================================================
// DI端子定义
//
#define DI_TERMINAL_NUMBER          5   // DI端子数量
#define DI_FUNC_NUMBER              33  // DI的功能选择数量，[0, DI_FUNC_NUMBER-1]

#define DI_FUNC_NO_FUNC             0   // 0    无功能
#define DI_FUNC_ACC_DEC_TIME_SRC    16  // 16   加减速时间选择端子
#define DI_FUNC_COUNTER_TICKER_IN   23  // 23   计数器输入(DI5)
#define DI_FUNC_LENGTH_TICKER_IN    25  // 25   长度计数器输入(DI5)



// 两点(x1,y1), (x2,y2), 求第三点(x, y)的y
// 请确保: x1 < x2
typedef struct
{
    void (*calc)(void *);     // Pointer to calculation functon

    int32 x1;
    int32 y1;                 // (x1,y1)
    int32 x2;
    int32 y2;                 // (x2,y2)

    int32 x;                  // 需要求解点(x,y)的x, 输入
    int32 y;                  // 需要求解点(x,y)的y, 输出
} LINE_STRUCT;


void LineCalc(LINE_STRUCT *p);

#define LINE_STRTUCT_DEFALUTS       \
{                                   \
    (void (*)(void *))LineCalc      \
}


typedef struct
{
    void (*calc)(void *);         // Pointer to calculation functon

    int16 t;                      // 滤波时间

    int32 in;                     // 输入
    int32 out;                    // 输出

    int32 outOld;                 // 上一次的输出
    int32 remainder;              // 运算过程中的余数
} LowPassFilter;

void LpfCalc(LowPassFilter *p);

#define LPF_DEFALUTS            \
{                               \
    (void (*)(void *))LpfCalc   \
}
#endif // __F_IO_H__

