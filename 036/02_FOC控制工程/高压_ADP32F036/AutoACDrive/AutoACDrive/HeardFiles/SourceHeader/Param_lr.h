
/************************************************************
------------------该文件是主程序的头文件---------------------
************************************************************/
#ifndef PARAM_LR_H
#define PARAM_LR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Define.h"
#include <HeardFiles/ADP32F03xHeader/ADP32F03x_Device.h>

extern int pm_ki_speed;
extern int pm_kp_speed;
extern long pm_ki_d;
extern long pm_ki_q;
extern long pm_kp_q;
extern long pm_kp_d;
extern long pm_r;
extern unsigned long l_ld;
extern unsigned long l_lq;

extern long pm_ref_id;
extern long pm_ref_iq;
extern unsigned int pm_control_mode;

extern int pm_curr_limitL;
extern int pm_curr_limitH;
extern long pm_coef_d0;
extern long pm_coef_d1;
extern long pm_coef_d2;
extern long pm_coef_q0;
extern long pm_coef_q1;
extern long pm_coef_q2;
extern long pm_coef_alfa;
extern long pm_coef_beta;
extern long pm_coef_bem;
extern long pm_ki_d_est;
extern long pm_ki_q_est;
extern long pm_kp_q_est;
extern long pm_kp_d_est;
extern long  pm_est_lpf_omg;
extern long  pm_est_lpf_omg1;
extern unsigned int l_disable_dbc;
extern unsigned int l_step;
extern unsigned int	 l_orient_angel;
extern int l_orient_currl;
extern int l_orient_currh;

extern int pm_omg_show;
extern int pm_ud_show;
extern int pm_uq_show;

extern int pm_power_show;
extern int pm_check_ip_flag;
extern unsigned int  l_cal_l_curr;
extern int pm_ud;
extern int pm_uq;
extern long pm_est_omg, pm_est_omg_1;
extern unsigned int pm_bem_kf;

extern long pm_est_angel;
extern int pm_kp_speed_app;//Q0
extern int pm_ki_speed_app;
extern int cof_uwMaxThetaPerCntTcPu;

extern Uint DCBrakeCur;
extern Uint TorqueCurrentLimit;
extern Uint OverspeedScaling;
extern Uint SpeedFeedbackFilter;
extern Uint SFLowSpeed;
extern Uint MinCurrentLowSpeed;
extern Uint SpecailFactor;
extern Uint SpeedMeasureFactor1;
extern Uint SpeedMeasureFactor2;
extern Uint StartPresetCurrent;
extern Uint LowLimitFrequency;
extern Uint FieldWeakMode;
extern Uint FieldWeakCurScale;
extern Uint FieldWeakScale;
extern Uint FieldWeakRef;
extern Uint IPDCurrentScaling;
extern Uint IPDAutoDectScaling;

extern int pm_min_curr;
extern int pm_speed_lpf_k;
extern int pm_speed_lpf_k1;
extern long pm_ref_speed;
extern long pm_integral_speed;


extern void get_lr_isr();
extern void ResetAdcANDPwm();
extern void pm_control_isr();
extern void UpdatePMSMControlVariable();
extern void CalParToFunction(void);
extern void calc_curr_pi_coef();

extern long GetCurrentIPD(int SectionData);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================

