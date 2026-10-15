#ifndef F_COMMAND_H
#define F_COMMAND_H

#include "ADP32F03x_Device.h"

/***************************************************************************
  Public Variables
 ***************************************************************************/
#define CONST_SPEED                         0
#define ACC_SPEED                           1
#define DEC_SPEED                           2

#define TIME_UNIT_ACC_DEC_SPEED             10
#define FORWARD_DIR                         0
#define REVERSE_DIR                         1

struct RUN_CMD_BITS
{
    Uint16 common0:1;
    Uint16 common:1;
    Uint16 rsvd0:2;
    Uint16 rsvd1:2;
    Uint16 dir:1;
    Uint16 rsvd2:1;
    Uint16 freeStop:1;
    Uint16 rsvd3:1;
    Uint16 rsvd4:1;
    Uint16 rsvd5:1;
    Uint16 rsvd6:1;
    Uint16 rsvd7:3;
};

union RUN_CMD
{
    Uint16              all;
    struct RUN_CMD_BITS bit;
};

struct RUN_FLAG_BITS
{
    Uint16 run:1;
    Uint16 common:1;
    Uint16 rsvd0:3;
    Uint16 accDecStatus:2;
    Uint16 rsvd1:7;
    Uint16 rsvd2:2;
};

union RUN_FLAG
{
    Uint16               all;
    struct RUN_FLAG_BITS bit;
};

struct DSP_MAIN_COMMAND_BITS
{
    Uint16 run:1;
    Uint16 speedTrack:1;
    Uint16 stopBrake:1;
    Uint16 rsvd0:1;
    Uint16 rsvd1:2;
    Uint16 startBrake:1;
    Uint16 rsvd2:1;
    Uint16 rsvd3:1;
    Uint16 errorDealing:1;
    Uint16 accDecStatus:2;
    Uint16 rsvd4:2;
    Uint16 SecondPhase:1;
    Uint16 LastPhase:1;
};

union DSP_MAIN_COMMAND
{
    Uint16                       all;
    struct DSP_MAIN_COMMAND_BITS bit;
};

struct DSP_STATUS_BITS
{
    Uint16 rsvd0:2;
    Uint16 speedTrackEnd:1;
    Uint16 rsvd1:3;
    Uint16 uv:1;
    Uint16 rsvd2:5;
    Uint16 shortGndOver:1;
    Uint16 rsvd3:1;
    Uint16 runEnable:2;
};

union DSP_STATUS {
   Uint16                   all;
   struct DSP_STATUS_BITS   bit;
};

enum RUN_STATUS
{
    RUN_STATUS_WAIT,
    RUN_STATUS_START,
    RUN_STATUS_NORMAL,
    RUN_STATUS_STOP,
    RUN_STATUS_JOG,
    RUN_STATUS_SERVO,
    RUN_STATUS_TUNE,
    RUN_STATUS_DI_BRAKE,
    RUN_STATUS_LOSE_LOAD,
    RUN_STATUS_SHUT_DOWN
};

enum START_RUN_STATUS
{
    START_RUN_STATUS_SPEED_TRACK,
    START_RUN_STATUS_BRAKE,
    START_RUN_STATUS_HOLD_START_FRQ
};
#define START_RUN_STATUS_INIT           START_RUN_STATUS_SPEED_TRACK

enum STOP_RUN_STATUS
{
    STOP_RUN_STATUS_DEC_STOP,
    STOP_RUN_STATUS_WAIT_BRAKE,
    STOP_RUN_STATUS_BRAKE
};
#define STOP_RUN_STATUS_INIT            STOP_RUN_STATUS_DEC_STOP

typedef struct
{
    void (*calc)(void *);

    int32 maxValue;
    int32 aimValue;
    int32 curValue;

    Uint32 tickerAll;
    int32 remainder;
} LINE_CHANGE_STRUCT;


extern union RUN_CMD runCmd;
extern union RUN_FLAG runFlag;
extern union DSP_MAIN_COMMAND dspMainCmd;
extern union DSP_STATUS dspStatus;
extern enum RUN_STATUS runStatus;
extern LINE_CHANGE_STRUCT frqLine;


/***************************************************************************
  Public Functions
 ***************************************************************************/
void RunSrcDeal(void);
void SetAccTimes(int16 AccTimeIn001S);


#endif  // __F_COMMAND_H__
