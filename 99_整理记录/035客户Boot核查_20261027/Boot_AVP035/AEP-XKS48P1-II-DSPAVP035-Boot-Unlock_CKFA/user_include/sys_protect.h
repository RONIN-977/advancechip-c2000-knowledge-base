
#ifndef USER_INCLUDE_SYS_PROTECT_H_
#define USER_INCLUDE_SYS_PROTECT_H_

#define  U_NET_MIN         1300         // 电网电压保护上限
#define  U_PV_LOW1         2000         // 启动后PV低压故障
#define  U_PV_LOW2         2800         // 启动前PV低压故障
#define  U_PV_LOW3         3000         // PV清低压故障电压
#define  RGND_MAX          30           // 接地故障电阻上限

extern int freq_err_cnt;
extern int no_net_cnt;
extern int ua_err_cnt,ub_err_cnt,uc_err_cnt;
extern int upv_err_cnt;
extern int dc_temph_cnt,ac_temph_cnt;
extern long no_invert_cnt;
extern long io_err_cnt;
extern int f_err_ed;
extern int f_err_cnt;
extern int ileak_300_cnt,ileak_60_cnt,ileak_30_cnt;
extern int ileakh_clr_cnt,ileakch_clr_cnt;
extern long gnd_err_cnt;
extern int State2812Cnt[16];
extern int Error2812Cnt[16];
extern int Err0ShadowCnt[16];
extern int Err1ShadowCnt[16];
extern int Err2ShadowCnt[16];
extern int Err3ShadowCnt[16];

union  ERROR_2812_REG
{
    Uint16 all;
    struct
    {                                              // bits description
        Uint16 Phase_err            :1;            // 0 0x0001 相序故障
        Uint16 Ckfa_in_err          :1;            // 1 0x0002 进入远程升级状态
        Uint16 Temp_high            :1;            // 2 0x0004 温度高
        Uint16 Uac_peakh            :1;            // 3 0x0008 交流电压瞬时值过压

        Uint16 Iac_unbalanced       :1;            // 4 0x0010 三相电流失配
        Uint16 OverModu             :1;            // 5 0x0020 过调制                未使用
        Uint16 Ipv1_high            :1;            // 6 0x0040 PV1过流
        Uint16 Ipv2_high            :1;            // 7 0x0080 PV2过流

        Uint16 Hardware_err         :1;            // 8 0x0100 硬件保护故障      未使用
        Uint16 Ubus_unbalanced      :1;            // 9 0x0200 中点不平衡
        Uint16 Ubus_high            :1;            // 10 0x0400 母线电压高
        Uint16 Ubus_low             :1;            // 11 0x0800 母线电压低

        Uint16                      :1;            // 12 0x1000
        Uint16                      :1;            // 13 0x2000
        Uint16 Ubus_half_high       :1;            // 14 0x4000 半母线电压保护
        Uint16 Upv_low              :1;            // 15 0x8000 PV电压低            未使用
    }bit;
};

union  STATE_2812_REG
{
    Uint16 all;
    struct
    {                                           // bits description
        Uint16 Uac_rmsh             :1;            // 0 0x0001 交流过压
        Uint16 Uac_rmsl             :1;            // 1 0x0002 交流欠压
        Uint16 Freq_err             :1;            // 2 0x0004 频率异常
        Uint16 Mppt_overload        :1;            // 3 0x0008 Mppt过载

        Uint16 Reset_2812           :1;            // 4 0x0010 2812复位
        Uint16 OverModu             :1;            // 5 0x0020 过调制
        Uint16                      :1;            // 6 0x0040
        Uint16                      :1;            // 7 0x0080

        Uint16                      :1;            // 8 0x0100
        Uint16                      :1;            // 9 0x0200
        Uint16                      :1;            // 10 0x0400
        Uint16                      :1;            // 11 0x0800

        Uint16                      :1;            // 12 0x1000
        Uint16                      :1;            // 13 0x2000
        Uint16                      :1;            // 14 0x4000
        Uint16                      :1;            // 15 0x8000
    }bit;
};

union  ERROR_F0_REG
{
    Uint16 all;
    struct
    {                                    // bits description
        Uint16 Uac_rmsh_state     :1;    // 0  2812状态位     交流过压
        Uint16 Uac_rmsl_state     :1;    // 1  2812状态位     交流欠压
        Uint16 Freq_err_state     :1;    // 2  2812状态位     频率异常                 停机
        Uint16 Phase_error        :1;    // 3  2812故障位     相序故障                 停机

        Uint16 Mppt_over_state    :1;    // 4  2812状态位     MPPT过载
        Uint16 Ubus_halfh         :1;    // 5  2812故障位     半母线过压              停机
        Uint16 Dc_Temph           :1;    // 6  自行判断           直流温度高              停机
        Uint16 RunStateBit        :1;    // 7  自行判断           运行状态(未用)  待修改

        Uint16 Temph_state_error  :1;    // 8  2812故障位     高温停机                  停机
        Uint16 Sci_error          :1;    // 9  自行判断           通讯故障                  停机
        Uint16 Iac_unbalanced     :1;    // 10 2812故障位     三相电流不平衡       停机
        Uint16 Reset_2812_error   :1;    // 11 2812状态位     2812复位                停机

        Uint16 Ac_Temph           :1;    // 12 自行判断            交流过温                  停机
        Uint16 NoNet              :1;    // 13 自行判断            无市电                      停机
        Uint16 Mcpu_reset_error   :1;    // 14 自行判断            28034复位             停机
        Uint16 Time_to_no_invert  :1;    // 15 自行判断            时间到未逆变           停机
    }bit;
};

union  ERROR_F1_REG
{
    Uint16 all;
    struct
    {                                    // bits description
        Uint16 Ckfa_in_error      :1;    // 0  自行判断          进入远程升级状态       停机
        Uint16 Hardware_error     :1;    // 1  2812故障位    硬件保护                      停机
        Uint16 IO_key_error       :1;    // 2  自行判断          IO口导致的停机          停机
        Uint16 Uac_peak_high      :1;    // 3  2812故障位    交流瞬时值过压           不停机,仅作为状态

        Uint16 Ckfa_in_state      :1;    // 4  2812故障位    2812远程升级停机     不停机,仅作为状态位
        Uint16                    :1;    // 5
        Uint16                    :1;    // 6
        Uint16 Ground_error       :1;    // 7  自行判断    接地故障                      停机

        Uint16                    :1;    // 8
        Uint16                    :1;    // 9
        Uint16                    :1;    // 10
        Uint16                    :1;    // 11

        Uint16                    :1;    // 12
        Uint16 Overmodu_state     :1;    // 13 2812状态位   过调制
        Uint16                    :1;    // 14
        Uint16                    :1;    // 15
    }bit;
};

union  ERROR_F2_REG
{
    Uint16 all;
    struct
    {                                    // bits description
        Uint16                    :1;    // 0
        Uint16                    :1;    // 1
        Uint16 Ipv1_high          :1;    // 2  2812故障位      第一路过流       停机
        Uint16 Ubus_high          :1;    // 3  2812故障位      母线过压           停机

        Uint16 Freq_high          :1;    // 4  自行判断             过频                  停机
        Uint16 Freq_low           :1;    // 5  自行判断             欠频                  停机
        Uint16                    :1;    // 6
        Uint16                    :1;    // 7

        Uint16                    :1;    // 8
        Uint16 Uc_low             :1;    // 9  2812峰值+自行判断    C相欠压     停机
        Uint16 Ub_low             :1;    // 10 2812峰值+自行判断    B相欠压     停机
        Uint16 Ua_low             :1;    // 11 2812峰值+自行判断    A相欠压     停机

        Uint16 Uc_high            :1;    // 12 2812峰值+自行判断    C相过压     停机
        Uint16 Ub_high            :1;    // 13 2812峰值+自行判断    B相过压     停机
        Uint16 Ua_high            :1;    // 14 2812峰值+自行判断    A相过压     停机
        Uint16                    :1;    // 15
    } bit;
};

union  ERROR_F3_REG
{
    Uint16 all;
    struct
    {                                    // bits description
        Uint16                    :1;    // 0
        Uint16                    :1;    // 1
        Uint16                    :1;    // 2
        Uint16                    :1;    // 3

        Uint16 Upv_low            :1;    // 4  自行判断           直流欠压                   停机
        Uint16                    :1;    // 5
        Uint16 Ubus_low           :1;    // 6  2812故障位     母线欠压                   停机
        Uint16 Ubus_unbalanced    :1;    // 7  2812故障位     母线不平衡               停机

        Uint16                    :1;    // 8
        Uint16 Ipv2_high          :1;    // 9  2812故障位      第二路过流               停机
        Uint16                    :1;    // 10
        Uint16                    :1;    // 11

        Uint16 Ileak_change       :1;    // 12 自行判断            漏电流变化快            停机
        Uint16 Ileak_high         :1;    // 13 自行判断            漏电流过大                停机
        Uint16                    :1;    // 14
        Uint16                    :1;    // 15
    } bit;
};

extern union ERROR_F0_REG Error0;
extern union ERROR_F1_REG Error1;
extern union ERROR_F2_REG Error2;
extern union ERROR_F3_REG Error3;
extern union ERROR_2812_REG Error2812;
extern union STATE_2812_REG State2812;

#endif /* USER_INCLUDE_SYS_PROTECT_H_ */
