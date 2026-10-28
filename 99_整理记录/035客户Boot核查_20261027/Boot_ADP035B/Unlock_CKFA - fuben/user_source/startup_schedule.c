
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

long timer_no_invert = 0;                  // 时间到未逆变计时
int f_no_invert = 0;
int f_first_start = 0,f_first_stop = 0;    // 首次发送启动/停止命令
int f_key_ac_on = 0,f_key_dc_on = 0;       // 逆变启动标志,直流启动标志
int f_dc_ac_on = 0;
int ac_on_cnt = 0;
int long_range_ctrl = 0;
int f_gnd_detect = 0;
int f_start_mode = STOP_MODE;        // 调试助手调试模式标志位
int f_mode_sel_ok = 0;               // 是否判断完运行模式

int f_com_start_2812 = 0;
int f_gnd_test = GND_ERR;
int gnd_test_step = 0;
int gnd_test_times = 0;
int Ug_adc_ave[2] = {0,0};
int Ug1_ave = 0,Ug2_ave = 0;

int f_ack_boost = 0;
int ack_boost_cnt = 0;
int f_ileak_change = 0;
long ileak_change_cnt = 0;
