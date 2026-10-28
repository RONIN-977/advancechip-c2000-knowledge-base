
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

union ERROR_F0_REG Error0;
union ERROR_F1_REG Error1;
union ERROR_F2_REG Error2;
union ERROR_F3_REG Error3;
union ERROR_2812_REG Error2812;
union STATE_2812_REG State2812;

int freq_err_cnt = 0;
int no_net_cnt = 0;
int ua_err_cnt = 0,ub_err_cnt = 0,uc_err_cnt = 0;
int upv_err_cnt = 0;
int dc_temph_cnt = 0,ac_temph_cnt = 0;
long no_invert_cnt = 0;
long io_err_cnt = 0;

int f_err_ed = 0;
int f_err_cnt = 0;

int ileak_300_cnt = 0,ileak_60_cnt = 0,ileak_30_cnt = 0;
int ileakh_clr_cnt = 0,ileakch_clr_cnt = 0;

long gnd_err_cnt = 0;
int State2812Cnt[16] = {0};
int Error2812Cnt[16] = {0};
int Err0ShadowCnt[16] = {0};
int Err1ShadowCnt[16] = {0};
int Err2ShadowCnt[16] = {0};
int Err3ShadowCnt[16] = {0};
