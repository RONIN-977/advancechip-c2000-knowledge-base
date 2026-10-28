
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

/*
void QueryDataPrep(void)
{
    int temp = 0;
    Uint16 utemp = 0;
    // 0 逆变器运行状态
    temp = RunState;
    ScilSendBuf[3] = (temp>>8);
    ScilSendBuf[4] = temp;
    // 1 当天在线时间:时
    temp = Online_Hour;
    ScilSendBuf[5] = (temp>>8);
    ScilSendBuf[6] = temp;
    // 2 当天在线时间:分
    temp = Online_Min;
    ScilSendBuf[7] = (temp>>8);
    ScilSendBuf[8] = temp;
    // 3 当天在线时间:秒
    temp = Online_Sec;
    ScilSendBuf[9] = (temp>>8);
    ScilSendBuf[10] = temp;

    if(f_err_trans == 1)
    {
        // 4 故障4
        utemp = Error3_shadow;
        ScilSendBuf[11] = (utemp>>8);
        ScilSendBuf[12] = utemp;
        // 5 故障1
        utemp = Error0_shadow;
        ScilSendBuf[13] = (utemp>>8);
        ScilSendBuf[14] = utemp;
        // 6 故障2
        utemp = Error1_shadow;
        ScilSendBuf[15] = (utemp>>8);
        ScilSendBuf[16] = utemp;
        // 7 故障3
        utemp = Error2_shadow;
        ScilSendBuf[17] = (utemp>>8);
        ScilSendBuf[18] = utemp;
    }
    else
    {
        // 4 故障4
        utemp = Error3.all;
        ScilSendBuf[11] = (utemp>>8);
        ScilSendBuf[12] = utemp;
        // 5 故障1
        utemp = (Error0.all & 0x0648);
        ScilSendBuf[13] = (utemp>>8);
        ScilSendBuf[14] = utemp;
        // 6 故障2
        utemp = (Error1.all & 0x0080);
        ScilSendBuf[15] = (utemp>>8);
        ScilSendBuf[16] = utemp;
        // 7 故障3
        utemp = Error2.all;
        ScilSendBuf[17] = (utemp>>8);
        ScilSendBuf[18] = utemp;
    }

    // 8 天最大功率
    temp = Pmax_today;
    ScilSendBuf[19] = (temp>>8);
    ScilSendBuf[20] = temp;
    // 9 保留
    temp = Reserve_9;
    ScilSendBuf[21] = (temp>>8);
    ScilSendBuf[22] = temp;
    // 10 A相电压
    temp = Uarms;
    ScilSendBuf[23] = (temp>>8);
    ScilSendBuf[24] = temp;
    // 11 B相电压
    temp = Ubrms;
    ScilSendBuf[25] = (temp>>8);
    ScilSendBuf[26] = temp;
    // 12 C相电压
    temp = Ucrms;
    ScilSendBuf[27] = (temp>>8);
    ScilSendBuf[28] = temp;
    // 13 A相电流
    temp = Iarms;
    ScilSendBuf[29] = (temp>>8);
    ScilSendBuf[30] = temp;
    // 14 B相电流
    temp = Ibrms;
    ScilSendBuf[31] = (temp>>8);
    ScilSendBuf[32] = temp;
    // 15 C相电流
    temp = Icrms;
    ScilSendBuf[33] = (temp>>8);
    ScilSendBuf[34] = temp;
    // 16 交流频率
    temp = Unet_freq;
    ScilSendBuf[35] = (temp>>8);
    ScilSendBuf[36] = temp;
    // 17 交流功率
    temp = Pac_active;
    ScilSendBuf[37] = (temp>>8);
    ScilSendBuf[38] = temp;
    // 18 无功功率,中小功率保留
    temp = Reserve_18;
    ScilSendBuf[39] = (temp>>8);
    ScilSendBuf[40] = temp;
    // 19 PV1功率
    temp = Ppv1;
    ScilSendBuf[41] = (temp>>8);
    ScilSendBuf[42] = temp;
    // 20 PV1电流
    temp = Ipv1;
    ScilSendBuf[43] = (temp>>8);
    ScilSendBuf[44] = temp;
    // 21 PV1电压
    temp = Upv1;
    ScilSendBuf[45] = (temp>>8);
    ScilSendBuf[46] = temp;
    // 22 天总发电量高16位
    temp = ((Eday>>16) & 0xFFFF);
    ScilSendBuf[47] = (temp>>8);
    ScilSendBuf[48] = temp;
    // 23 天总发电量低16位
    temp = (Eday & 0xFFFF);
    ScilSendBuf[49] = (temp>>8);
    ScilSendBuf[50] = temp;
    // 24 总发电量高16位
    temp = ((Etotal_int>>16) & 0xFFFF);
    ScilSendBuf[51] = (temp>>8);
    ScilSendBuf[52] = temp;
    // 25 总发电量低16位
    temp = (Etotal_int & 0xFFFF);
    ScilSendBuf[53] = (temp>>8);
    ScilSendBuf[54] = temp;
    // 26 逆变器交流温度
    temp = AveTac;
    ScilSendBuf[55] = (temp>>8);
    ScilSendBuf[56] = temp;
    // 27 保留
    temp = Reserve_27;
    ScilSendBuf[57] = (temp>>8);
    ScilSendBuf[58] = temp;
    // 28 二氧化碳排放量 兼容以前设备
    temp = Reduce_CO2;
    ScilSendBuf[59] = (temp>>8);
    ScilSendBuf[60] = temp;
    // 29 有功设定值
    temp = ApwrSetVal;
    ScilSendBuf[61] = (temp>>8);
    ScilSendBuf[62] = temp;
    // 30 无功设定值
    temp = RpwrSetVal;
    ScilSendBuf[63] = (temp>>8);
    ScilSendBuf[64] = temp;
    // 31 协议版本
    temp = ProtocolVer;
    ScilSendBuf[65] = (temp>>8);
    ScilSendBuf[66] = temp;
    // 32 设备型号
    temp = DeviceType;
    ScilSendBuf[67] = (temp>>8);
    ScilSendBuf[68] = temp;
    // 33 PV2直流功率
    temp = Ppv2;
    ScilSendBuf[69] = (temp>>8);
    ScilSendBuf[70] = temp;
    // 34 PV2直流电流
    temp = Ipv2;
    ScilSendBuf[71] = (temp>>8);
    ScilSendBuf[72] = temp;
    // 35 PV2直流电压
    temp = Upv2;
    ScilSendBuf[73] = (temp>>8);
    ScilSendBuf[74] = temp;
    // 36 二氧化碳减排量高字节
    temp = Reduce_CO2_H;
    ScilSendBuf[75] = (temp>>8);
    ScilSendBuf[76] = temp;
    // 37 二氧化碳减排量低字节
    temp = Reduce_CO2_L;
    ScilSendBuf[77] = (temp>>8);
    ScilSendBuf[78] = temp;
}*/
//测试代码
//int f_first_recved = 0;
//int ua_test = 0;
//extern int testzzc;
/*void QueryDataPrep_PC(void)
{
    int temp = 0;
    Uint16 utemp = 0;

    // 0
    temp = DataWatch2812[0];
    ScilSendBuf[3] = (temp>>8);
    ScilSendBuf[4] = temp;
    // 1
    temp = f_first_recved;
    //temp = DataWatch2812[1];
    ScilSendBuf[5] = (temp>>8);
    ScilSendBuf[6] = temp;
    // 2
    temp = ua_test;
    //temp = DataWatch2812[2];
    ScilSendBuf[7] = (temp>>8);
    ScilSendBuf[8] = temp;
    // 3
    temp = testzzc;
    //temp = DataWatch2812[3]
    ScilSendBuf[9] = (temp>>8);
    ScilSendBuf[10] = temp;
    // 4
    temp = DataWatch2812[4];
    ScilSendBuf[11] = (temp>>8);
    ScilSendBuf[12] = temp;
    // 5
    temp = DataWatch2812[5];
    ScilSendBuf[13] = (temp>>8);
    ScilSendBuf[14] = temp;
    // 6
    temp = DataWatch2812[6];
    ScilSendBuf[15] = (temp>>8);
    ScilSendBuf[16] = temp;
    // 7
    temp = Ug1_ave;
    //temp = DataWatch2812[7];
    ScilSendBuf[17] = (temp>>8);
    ScilSendBuf[18] = temp;
    // 8
    temp = Ug2_ave;
    //temp = DataWatch2812[8];
    ScilSendBuf[19] = (temp>>8);
    ScilSendBuf[20] = temp;

    // 9
    temp = ckfa_test;
    //temp = DataWatch2812[9];
    ScilSendBuf[21] = (temp>>8);
    ScilSendBuf[22] = temp;

    // 10
    utemp = Error2812.all;
    //temp = DataWatch2812[10];
    ScilSendBuf[23] = (utemp>>8);
    ScilSendBuf[24] = utemp;

    // 11
    temp = ckfa_switch_2812;
    //temp = DataWatch2812[11];
    ScilSendBuf[25] = (temp>>8);
    ScilSendBuf[26] = temp;

    //-------一下为固定传送的数据-------
    // 12 远程升级最小母线电压
    temp = Ubus_min;
    ScilSendBuf[27] = (temp>>8);
    ScilSendBuf[28] = temp;
    // 13 晶硅/薄膜选择
    temp = PV_type;
    ScilSendBuf[29] = (temp>>8);
    ScilSendBuf[30] = temp;
    // 14 远程起停状态
    temp = long_range_ctrl;
    ScilSendBuf[31] = (temp>>8);
    ScilSendBuf[32] = temp;
    // 15 机器类型
    temp = DeviceType;
    ScilSendBuf[33] = (temp>>8);
    ScilSendBuf[34] = temp;
    // 16 机器地址
    temp = Rs485Address;
    ScilSendBuf[35] = (temp>>8);
    ScilSendBuf[36] = temp;
    // 17 IV扫描
    temp = IV_switch;
    ScilSendBuf[37] = (temp>>8);
    ScilSendBuf[38] = temp;
    // 18 故障推送
    temp = f_err_trans;
    ScilSendBuf[39] = (temp>>8);
    ScilSendBuf[40] = temp;
    // 19 安规
    temp = Unet_limit;
    ScilSendBuf[41] = (temp>>8);
    ScilSendBuf[42] = temp;
    // 20 接地阻抗
    temp = Rgnd_val;
    ScilSendBuf[43] = (temp>>8);
    ScilSendBuf[44] = temp;
    // 21 漏电流
    temp = Ileak_ave;
    ScilSendBuf[45] = (temp>>8);
    ScilSendBuf[46] = temp;
    // 22 频率
    temp = Unet_freq;
    ScilSendBuf[47] = (temp>>8);
    ScilSendBuf[48] = temp;
    // 23 相序
    temp = DataWatch2812[23];
    ScilSendBuf[49] = (temp>>8);
    ScilSendBuf[50] = temp;

    //-------常规运行参数-------
    // 24 故障3
    utemp = Error3.all;
    ScilSendBuf[51] = (utemp>>8);
    ScilSendBuf[52] = utemp;
    // 25 故障2
    utemp = Error2.all;
    ScilSendBuf[53] = (utemp>>8);
    ScilSendBuf[54] = utemp;
    // 26 故障1
    utemp = Error1.all;
    ScilSendBuf[55] = (utemp>>8);
    ScilSendBuf[56] = utemp;
    // 27 故障0
    utemp = Error0.all;
    ScilSendBuf[57] = (utemp>>8);
    ScilSendBuf[58] = utemp;
    // 28 PV1电压
    temp = Upv1;
    ScilSendBuf[59] = (temp>>8);
    ScilSendBuf[60] = temp;
    // 29 PV1电流
    temp = Ipv1;
    ScilSendBuf[61] = (temp>>8);
    ScilSendBuf[62] = temp;
    // 30 PV2直流电压
    temp = Upv2;
    ScilSendBuf[63] = (temp>>8);
    ScilSendBuf[64] = temp;
    // 31 PV2直流电流
    temp = Ipv2;
    ScilSendBuf[65] = (temp>>8);
    ScilSendBuf[66] = temp;
    // 32 母线电压
    temp = Ubus;
    ScilSendBuf[67] = (temp>>8);
    ScilSendBuf[68] = temp;
    // 33 母线电压差
    temp = Ubus_delta;
    ScilSendBuf[69] = (temp>>8);
    ScilSendBuf[70] = temp;
    // 34 A相电压
    temp = Uarms;
    ScilSendBuf[71] = (temp>>8);
    ScilSendBuf[72] = temp;
    // 35 A相电流
    temp = Iarms;
    ScilSendBuf[73] = (temp>>8);
    ScilSendBuf[74] = temp;
    // 36 B相电压
    temp = Ubrms;
    ScilSendBuf[75] = (temp>>8);
    ScilSendBuf[76] = temp;
    // 37 B相电流
    temp = Ibrms;
    ScilSendBuf[77] = (temp>>8);
    ScilSendBuf[78] = temp;
    // 38 C相电压
    temp = Ucrms;
    ScilSendBuf[79] = (temp>>8);
    ScilSendBuf[80] = temp;
    // 39 C相电流
    temp = Icrms;
    ScilSendBuf[81] = (temp>>8);
    ScilSendBuf[82] = temp;
    // 40 逆变器直流温度
    temp = AveTdc;
    ScilSendBuf[83] = (temp>>8);
    ScilSendBuf[84] = temp;
    // 41 逆变器交流温度
    temp = AveTac;
    ScilSendBuf[85] = (temp>>8);
    ScilSendBuf[86] = temp;
    // 42 运行状态:F2812
    temp = RunState_2812;
    ScilSendBuf[87] = (temp>>8);
    ScilSendBuf[88] = temp;
    // 43 PV1功率
    temp = Ppv1;
    ScilSendBuf[89] = (temp>>8);
    ScilSendBuf[90] = temp;
    // 44 PV2功率
    temp = Ppv2;
    ScilSendBuf[91] = (temp>>8);
    ScilSendBuf[92] = temp;
    // 45 交流功率
    temp = Pac_active;
    ScilSendBuf[93] = (temp>>8);
    ScilSendBuf[94] = temp;
    // 46 无功功率
    temp = Pac_reactive;
    ScilSendBuf[95] = (temp>>8);
    ScilSendBuf[96] = temp;
    // 47 Eday_int
    temp = Eday_int;
    ScilSendBuf[97] = (temp>>8);
    ScilSendBuf[98] = temp;
    // 48 Eday_frac
    temp = Eday_frac;
    ScilSendBuf[99] = (temp>>8);
    ScilSendBuf[100] = temp;
    // 49 影子故障3
    utemp = Error3_shadow;
    ScilSendBuf[101] = (utemp>>8);
    ScilSendBuf[102] = utemp;
    // 50 影子故障2
    utemp = Error2_shadow;
    ScilSendBuf[103] = (utemp>>8);
    ScilSendBuf[104] = utemp;
    // 51 影子故障1
    utemp = Error1_shadow;
    ScilSendBuf[105] = (utemp>>8);
    ScilSendBuf[106] = utemp;
    // 52 影子故障0
    utemp = Error0_shadow;
    ScilSendBuf[107] = (utemp>>8);
    ScilSendBuf[108] = utemp;
    // 53 启动模式
    temp = f_start_mode;
    ScilSendBuf[109] = (temp>>8);
    ScilSendBuf[110] = temp;
}*/
