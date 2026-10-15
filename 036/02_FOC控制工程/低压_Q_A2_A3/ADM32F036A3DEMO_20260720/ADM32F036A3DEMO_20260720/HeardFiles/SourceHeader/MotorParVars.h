#ifndef MOTOR_VAR_INCLUDE_H
#define MOTOR_VAR_INCLUDE_H

#include "Define.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 *  Boot / System Control
 *============================================================================*/
#define Jump_Boot()   ((void (*)(void))0x3F7FF6)()

/*=============================================================================
 *  System Error Codes
 *============================================================================*/
typedef enum
{
    ERROR_NONE = 0,
    ERROR_OC_HARDWARE        = 12,
    ERROR_OV_ACC_SPEED       = 13,
    ERROR_UV                 = 14,
    ERROR_LOSE_PHASE_OUTPUT  = 15,
    ERROR_OT_IGBT            = 16,
    ERROR_CURRENT_SAMPLE     = 17,
    ERROR_TUNE               = 18,
    ERROR_STALL              = 20,
    ERROR_NTC                = 21,
    ERROR_NO_LOAD            = 22,
    ERROR_MOTOR_SHORT_TO_GND = 23,
    ERROR_IPD_NO_MOTOR       = 32,
    ERROR_OC_SOFTWARE        = 33,
    ERROR_CBC                = 41,
    ERROR_ERROR_KE           = 44,
    ERROR_LIN                = 55,
    ERROR_DRIVER             = 66

} MOTOR_ERROR_CODE;

/*=============================================================================
 *  PWM Hardware Calculation
 *============================================================================*/
#define T_PRD   ((Uint16)(((Uint32)DSP_CLOCK60 * 5000U) / PWM_FREQUENCY))

#define T_CC1   (0U)
#define T_CC2   (0U)
#define T_CC3   (0U)

#define EPWM1_CMPB   ((Uint16)(T_PRD - 300U))
#define EPWM3_CMPB   ((Uint16)((T_PRD >> 1U) - TMIN - TBEFORE))

/*=============================================================================
 *  ADC Configuration
 *============================================================================*/
#define ADC_TRIG_SOURCE   13U
#define ACQPS_SYS_CLKS    8U

#define ADC_PIN_S0   0U
#define ADC_PIN_S1   1U
#define ADC_PIN_S2   2U
#define ADC_PIN_S3   3U
#define ADC_PIN_S4   4U
#define ADC_PIN_S5   5U
#define ADC_PIN_S6   6U
#define ADC_PIN_S7   7U
#define ADC_PIN_S8   8U
#define ADC_PIN_S9   9U
#define ADC_PIN_S10  10U
#define ADC_PIN_S11  11U
#define ADC_PIN_S12  12U
#define ADC_PIN_S13  13U
#define ADC_PIN_S14  14U
#define ADC_PIN_S15  15U

/*=============================================================================
 *  ADC Trigger Sources
 *============================================================================*/
#define ADCTRIG_SOFT        0U
#define ADCTRIG_EPWM1_SOCA  5U
#define ADCTRIG_EPWM1_SOCB  6U
#define ADCTRIG_EPWM2_SOCA  7U
#define ADCTRIG_EPWM2_SOCB  8U
#define ADCTRIG_EPWM3_SOCA  9U
#define ADCTRIG_EPWM3_SOCB  10U
#define ADCTRIG_EPWM4_SOCA  11U
#define ADCTRIG_EPWM4_SOCB  12U
#define ADCTRIG_EPWM5_SOCA  13U
#define ADCTRIG_EPWM5_SOCB  14U
#define ADCTRIG_EPWM6_SOCA  15U
#define ADCTRIG_EPWM6_SOCB  16U
#define ADCTRIG_EPWM7_SOCA  17U
#define ADCTRIG_EPWM7_SOCB  18U

/*=============================================================================
 *  PIE Mapping
 *============================================================================*/
#define PIE_VECTTABLE_ADCINT   PieVectTable.ADCINT1
#define ADC_CLEAR_INT_FLAG      AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1    //
#define ADC_RESET_SEQUENCE      AdcRegs.SOCPRICTL.bit.RRPOINTER = 0x20  //reset the sequence
#define ADC_START_CONVERSION    AdcRegs.ADCSOCFRC1.all = 0xFFFF         //        AD
#define ADC_END_CONVERSIN       AdcRegs.ADCINTFLG.bit.ADCINT1           //

/************************************************************
         DSP  ADC    ? GPIO      ? GPIO     
************************************************************/
#define DisConnectRelay()           GpioDataRegs.GPACLEAR.bit.GPIO8  = 1
#define ConnectRelay()              GpioDataRegs.GPASET.bit.GPIO8    = 1    //
#define DRIVE_ENABLE()              GpioDataRegs.GPASET.bit.GPIO31  = 1
#define DRIVE_DISABLE()             GpioDataRegs.GPACLEAR.bit.GPIO31  = 1   //DRRIVER_EN

#define EnableDrive()                           \
                                                \
    EALLOW;                                     \
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;         \
    EDIS;                                       \

#define DisableDrive()                          \
                                                \
    EALLOW;                                     \
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;         \
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;         \
    EDIS;                                       \

/************************************************************
  зр?  ???       
************************************************************/
#define ADC_GND             (AdcResult.ADCRESULT3<<4)
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define ADC_IV              (AdcResult.ADCRESULT0)
#define ADC_IV1             (AdcResult.ADCRESULT1)
#define ADC_UDC             (AdcResult.ADCRESULT2<<4)
#else
#define ADC_IU              (AdcResult.ADCRESULT0<<4)
#define ADC_IV              (AdcResult.ADCRESULT1<<4)
#define ADC_IW              (AdcResult.ADCRESULT0<<4)
#define ADC_UDC             (AdcResult.ADCRESULT2<<4)
#endif

#define ADCRESULT_UU        (AdcResult.ADCRESULT5)
#define ADCRESULT_UV        (AdcResult.ADCRESULT4)
#define ADCRESULT_UW        (AdcResult.ADCRESULT3)
#define ADC_VSP             (AdcResult.ADCRESULT6)
#define ADC_TEMP            (AdcResult.ADCRESULT6)
#define ADC_DSP_TEMP        (AdcResult.ADCRESULT6)
#define ADC_IBUS            (AdcResult.ADCRESULT7)

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_INCLUDE_H */
