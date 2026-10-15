/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   SFO_V8.h
*******************************************************************************************/

#ifndef _SFO_V8_H
#define _SFO_V8_H

#ifdef __cplusplus
extern "C" {
#endif

//
// Defines
//
#define SFO_INCOMPLETE      0
#define SFO_COMPLETE        1
#define SFO_ERROR           2

#define PWM_CH_MAX 4  // Equals # of HRPWM modules in the device

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
