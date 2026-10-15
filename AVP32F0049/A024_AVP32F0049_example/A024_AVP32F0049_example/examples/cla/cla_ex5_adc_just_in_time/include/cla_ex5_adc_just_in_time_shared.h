/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  cla_ex5_adc_just_in_time_shared.h
*                         TITLE: Just-in-time ADC sampling with CLA
*******************************************************************************************/

#ifndef _CLA_EX5_ADCJUSTINTIME_SHARED_H_
#define _CLA_EX5_ADCJUSTINTIME_SHARED_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define EPWM1_FREQ          1000000UL   // 1 MHz
#define EPWM1_PERIOD        (uint16_t)(DEVICE_SYSCLK_FREQ / (EPWM1_FREQ))

//
// Globals
//
//Task 1 (C) Variables

//Task 2 (C) Variables

//Task 3 (C) Variables

//Task 4 (C) Variables

//Task 5 (C) Variables

//Task 6 (C) Variables

//Task 7 (C) Variables

//Task 8 (C) Variables

//Common (C) Variables


//
// Function Prototypes
//
__attribute__((interrupt))  void Cla1Task1();
__attribute__((interrupt))  void Cla1Task2();
__attribute__((interrupt))  void Cla1Task3();
__attribute__((interrupt))  void Cla1Task4();
__attribute__((interrupt))  void Cla1Task5();
__attribute__((interrupt))  void Cla1Task6();
__attribute__((interrupt))  void Cla1Task7();
__attribute__((interrupt))  void Cla1Task8();

#ifdef __cplusplus
}
#endif // extern "C"
#endif //_CLA_EX5_ADCJUSTINTIME_SHARED_H_

//
// End of File
//
