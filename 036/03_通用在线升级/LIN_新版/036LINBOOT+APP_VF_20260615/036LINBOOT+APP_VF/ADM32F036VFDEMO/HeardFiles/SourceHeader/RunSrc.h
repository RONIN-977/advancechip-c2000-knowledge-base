#ifndef __F_COMMAND_H__
#define __F_COMMAND_H__

#include <ADP32F03x_Device.h>

#define FUNCCODE_accDecSpdCurve_LINE        0   // Linear acceleration and deceleration

//=====================================================================
//The main command word for performance transfer
//=====================================================================

struct MOTOR_MAIN_COMMAND_BITS
{                               // bits  description
    Uint16 run:1;               // 0,    1:run 0:stop
};
union MOTOR_MAIN_COMMAND
{
    Uint16                       all;
    struct MOTOR_MAIN_COMMAND_BITS bit;
};
extern union MOTOR_MAIN_COMMAND MotorMainCmd;
//=====================================================================

//-----------------------------------------------------
// runStatus£¬current operation status/step
//
enum RUN_STATUS
{
    RUN_STATUS_WAIT,        // Wait for start
    RUN_STATUS_START,       // Start
    RUN_STATUS_NORMAL,      // (Normal) operation
    RUN_STATUS_STOP,        // Stop
    RUN_STATUS_SHUT_DOWN    // shut down, turn off
};
extern enum RUN_STATUS RunMode;
//-----------------------------------------------------

// Linear change calculation: acceleration/deceleration, PID setpoint calculation
// Known that the change time from 0 to maximum value maxValue is tickerAll,
// remainder should be cleared for each new calculation, but the impact is small.

//
typedef struct
{
    void (*calc)(void *);       // Function pointer

    int32 MaxValue;             // Maximum value
    int32 AimValue;             // Target value
    int32 CurValue;             // Current value

    Uint32 TickerAll;           // Ticker from 0 to maximum value
    int32 Remainder;            // Remainder for delta calculation
} LINE_RAMP_STR;

#define LINE_CHANGE_STRTUCT_DEFALUTS       \
{                                          \
    (void (*)(void *))LineRampCalc       \
}
//-----------------------------------------------------
extern LINE_RAMP_STR FrequencyLineRamp;

// function

void RunLogicDeal(void);
void AccDecTimeCalc(void);
void LineRampCalc(LINE_RAMP_STR *p);
void AccAndDecFrqCalc(int32 accTime, int32 decTime, Uint16 AccRampmode);



#endif  // __F_COMMAND_H__

