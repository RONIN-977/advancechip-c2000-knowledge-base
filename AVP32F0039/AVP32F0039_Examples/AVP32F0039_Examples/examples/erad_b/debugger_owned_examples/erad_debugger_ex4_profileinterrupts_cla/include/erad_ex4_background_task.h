/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   erad_ex4_background_task.c
*                         TITLE:  CLA ADC Sampling and Filtering with Buffering in a Background Task
*******************************************************************************************/

#ifndef _ERAD_EX4_BACKGROUND_TASK_H_
#define _ERAD_EX4_BACKGROUND_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif
//
// Included Files
//

//
// Defines
//
#define BUFFER_SIZE     1024 // NOTE: keep to a power of 2
#define FILTER_ORDER    28U
#define FILTER_LENGTH   (FILTER_ORDER + 1U)


//
// Globals
//
//Task 1 (C) Variables
extern const float coeffs[FILTER_LENGTH];
extern float filter_out;

//Task 2 (C) Variables

//Task 3 (C) Variables

//Task 4 (C) Variables

//Task 5 (C) Variables

//Task 6 (C) Variables

//Task 7 (C) Variables

//Background Task (C) Variables
extern volatile float buffer[BUFFER_SIZE];

//Common (C) Variables
extern float D[FILTER_LENGTH];

//
// Function Prototypes
//
//CLA C Tasks defined in erad_ex4_background_task_cla.cla
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
#endif //_ERAD_EX4_BACKGROUND_TASK_H_

//
// End of File
//
