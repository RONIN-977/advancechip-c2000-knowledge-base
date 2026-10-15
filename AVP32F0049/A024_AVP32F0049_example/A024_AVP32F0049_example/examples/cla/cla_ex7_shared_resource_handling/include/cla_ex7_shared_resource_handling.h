/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  cla_ex7_shared_resource_handling.h
*                         TITLE: Handling shared resources across C28x and CLA
*******************************************************************************************/

#ifndef _CLA_EX7_SHARED_RESOURCE_HANDLING__H_
#define _CLA_EX7_SHARED_RESOURCE_HANDLING__H_

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
#define EPWM4_FREQ          10000UL    // 10 KHz
#define EPWM5_FREQ          100000UL   // 100 KHz
#define EPWM4_PERIOD        (uint16_t)(DEVICE_SYSCLK_FREQ / EPWM4_FREQ)
#define EPWM5_PERIOD        (uint16_t)(DEVICE_SYSCLK_FREQ / EPWM5_FREQ)

//CLA C Tasks defined in Cla1Tasks_C.cla
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
#endif //_CLA_EX7_SHARED_RESOURCE_HANDLING__H_

//
// End of File
//
