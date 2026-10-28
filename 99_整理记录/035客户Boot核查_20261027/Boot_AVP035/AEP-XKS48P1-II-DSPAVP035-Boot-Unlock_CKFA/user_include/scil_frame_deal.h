
#include "F2806x_Device.h"     // DSP2803x Headerfile Include File
#include "F2806x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

#ifndef USER_INCLUDE_SCI_LIN_FRAME_H_
#define USER_INCLUDE_SCI_LIN_FRAME_H_

extern int Scil_rCrcL_PC;
extern int Scil_rSum_PC;
extern int Scil_rCrcH_Mod;
extern int Scil_rCrcL_Mod;
extern int Rs485Address;
extern int ckfa_switch,ckfa_switch_2812;
extern int coff_cal_cmd;
extern int coff_cal_val;
extern int Rgnd_val;

extern int AveTac,AveTdc;
extern int ApwrSetVal,RpwrSetVal;
extern int Unet_limit;
extern int f_err_trans;
extern int Ubus_min;
extern int PV_type;
extern int RunState,RunState_2812;
extern int Online_Hour,Online_Min,Online_Sec;
extern Uint16 Error0_shadow,Error1_shadow,Error2_shadow,Error3_shadow;
extern int Pmax_today;
extern int Reserve_9,Reserve_18,Reserve_27;
extern int ProtocolVer;
extern int DeviceType;
extern int Reduce_CO2,Reduce_CO2_H,Reduce_CO2_L;
extern long Reduce_CO2_temp;
extern int Uarms,Ubrms,Ucrms;
extern int Iarms,Ibrms,Icrms;

extern int f_flash_erase;
extern int f_code_flg;
extern int f_flash_program;
extern int f_last_row;
extern int32 RowNumber,RowNumberOld;
extern Uint32 recv_word_cnt,recv_word_len;
extern Uint32 AddressHigh,AddressAbs;
extern Uint32 AddressAbsTemp,AddressAbsPredict;
extern Uint16 FlashData[];
extern int Scil_passthrough_flg;
extern int f_parameter;
extern int Etotal_set[8];
extern int parm_set_temp[80];
extern int parm_set_val[80];

extern int CrcCheckCalc(int data_buf[], int data_length);
extern void CrcSumCheck(int data_buf[], int data_length);
extern int SumCheck(int data_buf[], int data_length);
extern int HexSumCheck(int data_buf[], int data_length);
extern void ScilRecvDataDeal(void);
extern void SendFeedBack(int feedback_cmd);
extern void scil_cmd_cpy(void);

#endif /* USER_INCLUDE_SCI_LIN_FRAME_H_ */
