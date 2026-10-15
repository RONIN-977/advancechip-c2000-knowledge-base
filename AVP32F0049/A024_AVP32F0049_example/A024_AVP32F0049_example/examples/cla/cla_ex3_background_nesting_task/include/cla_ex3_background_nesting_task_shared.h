/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  cla_ex3_background_nesting_task_shared.h
*                         TITLE: CLA ADC Sampling and Filtering with Buffering in a Background Task
*******************************************************************************************/

#ifndef _CLA_EX3_BACKGROUND_NESTING_TASK_H_
#define _CLA_EX3_BACKGROUND_NESTING_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif
//
// Included Files
//

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

//Background Task (C) Variables

//Common (C) Variables

//
// Function Prototypes
//
//CLA C Tasks defined in Cla1Tasks_C.cla
__attribute__((interrupt))  void Cla1Task1();
__attribute__((interrupt))  void Cla1Task2();
__attribute__((interrupt))  void Cla1Task3();
__attribute__((interrupt))  void Cla1Task4();
__attribute__((interrupt))  void Cla1Task5();
__attribute__((interrupt))  void Cla1Task6();
__attribute__((interrupt))  void Cla1Task7();
#if defined(__TMS320C28XX__)
__attribute__((interrupt))  void Cla1BackgroundTask();
#else
__attribute__((interrupt("background")))  void Cla1BackgroundTask();
#endif

#ifdef __cplusplus
}
#endif // extern "C"
#endif //_CLA_EX3_BACKGROUND_NESTING_TASK_H_

//
// End of File
//
