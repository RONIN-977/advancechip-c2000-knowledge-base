/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   SFO_V8.h
*                         TITLE:  HRPWM Period Control.
*******************************************************************************************/

#ifndef _SFO_V8_H
#define _SFO_V8_H

#ifdef __cplusplus
extern "C" {
#endif

//
// Defines
//
#define PWM_CH_MAX 8  // Equals # of HRPWM modules in the device

#define SFO_INCOMPLETE      0
#define SFO_COMPLETE        1
#define SFO_ERROR           2

//
// Function Prototypes
//
int SFO(void);     // SFO V8  Calibration Function

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // End: Multiple include Guard
//
// End of file
//
