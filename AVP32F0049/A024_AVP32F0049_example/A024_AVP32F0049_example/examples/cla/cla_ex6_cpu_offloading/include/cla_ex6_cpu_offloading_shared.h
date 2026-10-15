/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  cla_ex6_cpu_offloading_shared.h
*                         TITLE: Optimal offloading of control algorithms to CLA
*******************************************************************************************/

#ifndef _CLA_EX6_CPUOFFLOADING_SHARED_H_
#define _CLA_EX6_CPUOFFLOADING_SHARED_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

#if run_loop1_cla == 1
#include "DCLCLA.h"
#else
#include "DCLF32.h"
#endif

//
// Structure definitions
//

typedef struct {
    uint16_t loop1;
    uint16_t loop2;
}reference;

typedef struct {
    uint16_t loop1;
    uint16_t loop2;
}sensed;

typedef struct {
    float32_t loop1;
    float32_t loop2;
}control_out;

//
// Defines
//
#define EPWM1_FREQ          1000000UL   // 1 MHz
#define EPWM4_FREQ          200000UL    // 200 KHz
#define EPWM5_FREQ          20000UL     // 20 KHz
#define EPWM1_PERIOD        (uint16_t)(DEVICE_SYSCLK_FREQ / (2 * EPWM1_FREQ))
#define EPWM4_PERIOD        (uint16_t)(DEVICE_SYSCLK_FREQ / EPWM4_FREQ)
#define EPWM5_PERIOD        (uint16_t)(DEVICE_SYSCLK_FREQ / EPWM5_FREQ)

//
// Globals
//
extern volatile reference ref_data;
extern volatile sensed sense_data;
extern volatile control_out control_out_data;
#if run_loop1_cla == 1
extern volatile DCL_PI_CLA pi_loop1;
#else
extern volatile DCL_PI pi_loop1;
#endif
extern volatile float32_t duty, duty_loop1, duty_loop2;

//
// Loop1 control task defined in a shared file so that can be used both
// by CPU and CLA in order avoid code duplication
//
static inline void loop1_task(void)
{
    //
    // Read the oversampled input captured by 4 SOCs and calculate
    // the average
    //
    sense_data.loop1 = (ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER0) +
                        ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER1) +
                         ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER2) +
                          ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER3)) >> 2;

//
// Run the parallel form PI controller
//
#if run_loop1_cla == 1
    control_out_data.loop1 = DCL_runPI_L4(&pi_loop1, (ref_data.loop1 / 4096.0f),
                                          (sense_data.loop1 / 4096.0f));
#else
    control_out_data.loop1 = DCL_runPI_C3(&pi_loop1, __divf32(ref_data.loop1, 4096.0f),
                                          __divf32(sense_data.loop1, 4096.0f));
#endif

    //
    // Calculate the duty contribution based on the controller output with 20% weightage
    //
    duty_loop1 = 0.8f * control_out_data.loop1;

    //
    // Computing final duty value by summing up the contribution of both loops
    //
    duty = duty_loop1 + duty_loop2;

    //
    // Check for upper(0.9) and lower(0.1) saturation values
    //
    duty = (duty > 0.9f) ? 0.9f : duty;
    duty = (duty < 0.1f) ? 0.1f : duty;

    //
    // Write to the COMPA register
    //
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A,
                                (uint16_t)((1.0f - duty) * EPWM1_PERIOD + 0.5f));
}

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
#endif //_CLA_EX6_CPUOFFLOADING_SHARED_H_

//
// End of File
//
