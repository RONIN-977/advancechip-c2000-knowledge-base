/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :main.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "ADP32F03x_Project.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    TICK_10     (10)//单位ms
#define    TICK_100    (100)//单位ms

#define    CHECK_TASK_0()    (Task.tick.ms_1  > 0? 1 : 0)
#define    CHECK_TASK_1()    (Task.tick.ms_10  >= TICK_10? 1 : 0)
#define    CHECK_TASK_2()    (Task.tick.ms_100 >= TICK_100? 1 : 0)

#define    TICK0_SET()      Task.tick.ms_1   = 1
#define    TICK12_SET()     Task.tick.ms_10++; Task.tick.ms_100++

#define    TICK0_CLEAR()    Task.tick.ms_1   = 0
#define    TICK1_CLEAR()    Task.tick.ms_10  = 0
#define    TICK2_CLEAR()    Task.tick.ms_100 = 0

#define    SYSTICK_SET()    Task.systickcnt++;
#define    GET_SYSTICKCNT()    Task.systickcnt


/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef struct
{
    union
    {
        struct
        {
            Uint16 ms_1  :2;
            Uint16 ms_10 :4;
            Uint16 ms_100:10;
        }tick;
        Uint16 data;
    };
    Uint32 systickcnt;
}TASK_T;

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/
extern TASK_T Task;

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/
static inline void Task_Init(void)
{
    Task.tick.ms_1   = 0;
    Task.tick.ms_10  = 0;
    Task.tick.ms_100 = 0;
    Task.systickcnt  = 0;
}

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/




















#endif
