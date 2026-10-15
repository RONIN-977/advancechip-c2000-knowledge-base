
/************************************************************************
 Project:             Automotive water pumps
 Filename:            LDF.c
 Partner Filename:    N/A
 Description:         Partner file of LDF.h
 Complier:            Code Composer Studio for ADM32F036, CCS Systems.
 CPU TYPE :           ADM32F036A2
*************************************************************************
 Copyright (c) 2024 **** Co., Ltd.
 All rights reserved.
*************************************************************************
*************************************************************************/

#include "Define.h"

///////////////////////////////////////////////////////////////////////////////////////
// Voltage Parameters
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MAX_DC_VOL_3_3V      =  480;        // 0.1V DC
#define  MAX_PEAK_CUR_3_3V           5000         // 0.01A

#define DEAD_TIME                    50                                                 // Dead time, 0.5uS
unsigned int DeadTimeInternal     =  ((long)DEAD_TIME * DSP_CLOCK) / 100;               // Dead time calculation

#define HARDWARE_OVER_CUR            4900//3600                                // Hardware overcurrent threshold 10.00A
#define HARDWARE_CBC_CUR             4900//3200                                // CBC current threshold, 8.00A
unsigned int OverCurPointInternal =  (((long)HARDWARE_OVER_CUR << 9) / MAX_PEAK_CUR_3_3V) + 512;   // Hardware comparison point value
unsigned int CBCCurPointInternal  =  (((long)HARDWARE_CBC_CUR  << 9) / MAX_PEAK_CUR_3_3V) + 512;   // CBC comparison point value

///////////////////////////////////////////////////////////////////////////////////////
// Motor Parameters
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MOTOR_RATED_CUR    =  120;

///////////////////////////////////////////////////////////////////////////////////////
// Frequency Reference and Acceleration/Deceleration Settings
///////////////////////////////////////////////////////////////////////////////////////
int RefSet                          =  100;     // 120.00Hz
unsigned int ACC1                   =  500;     // Acceleration time 5.0S
unsigned int ACC2                   =  200;     // Deceleration time 2.0S

///////////////////////////////////////////////////////////////////////////////////////
// PWM Parameters
///////////////////////////////////////////////////////////////////////////////////////
unsigned int PWM_FREQUENCY          =  160;     // 100 * 0.1kHz Switching frequency

//--------------------- Frequency Operating Range ----------------------/
unsigned int MAX_LIMIT_FREQ         =  4000;    // 300.00Hz
unsigned int MOTOR_RATED_FRE        =  2000;   //40000;

//--------------------- FOC Parameters (Normally not modified) ----------------------/
Uint PWMArrayData0[12] = {0,0,1,1,1,1,2,2,2,2,0,0};
Uint PWMArrayData1[12] = {1,1,0,0,2,2,1,1,0,0,2,2};
Uint PWMArrayData2[12] = {2,2,2,2,0,0,0,0,1,1,1,1};


