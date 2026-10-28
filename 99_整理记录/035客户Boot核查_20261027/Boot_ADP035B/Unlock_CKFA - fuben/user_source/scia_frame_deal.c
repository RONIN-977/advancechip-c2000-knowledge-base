#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

int system_power = 0;
int Ubus = 0,Ubus_delta = 0;
int Unet_daxis = 0;
int Unet_freq = 0;
int Upv1 = 0,Ipv1 = 0,Upv2 = 0,Ipv2 = 0;
int Ppv1 = 0,Ppv2 = 0;
int Pac_active = 0,Pac_reactive = 0;
int IV_switch = 0;
int Unet_max = 2800;
long Etotal_int = 0,Etotal_frac = 0;
long Eday = 0,Eday_int = 0,Eday_frac = 0;

int MachineSn[8] = {0};     // »úÆ÷ÐòÁÐºÅ
int DataWatch2812[30] = {0};

void scil_cmd_cpy(void)
{
    ScilSendBuf[0] = ScilRecvBuf[0];
    ScilSendBuf[1] = ScilRecvBuf[1];
    ScilSendBuf[2] = ScilRecvBuf[2];
    ScilSendBuf[3] = ScilRecvBuf[3];
}
