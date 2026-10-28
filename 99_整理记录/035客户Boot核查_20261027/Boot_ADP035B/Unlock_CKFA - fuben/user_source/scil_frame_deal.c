
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"
int Scil_rCrcL_PC = 0;
int Scil_rSum_PC = 0;
int Scil_rCrcH_Mod = 0;
int Scil_rCrcL_Mod = 0;
int Rs485Address = 0x01;              //逆变器地址
int ckfa_switch = 0,ckfa_switch_2812 = 0;
int coff_cal_cmd = 0;
int coff_cal_val = 0;
int Rgnd_val = 0;

int AveTac = 0,AveTdc = 0;              // 直流温度 交流温度
int ApwrSetVal = 0,RpwrSetVal = 0;      // 有功无功设置数值
int Unet_limit = 270;                   // 交流安规电压
int f_err_trans = 0;
int Ubus_min = 0;
int PV_type = 0;
int RunState = 0,RunState_2812 = 0;
int Online_Hour = 0,Online_Min = 0,Online_Sec = 0;
Uint16 Error0_shadow = 0,Error1_shadow = 0,Error2_shadow = 0,Error3_shadow = 0;
int Pmax_today = 0;
int Reserve_9 = 0,Reserve_18 = 0,Reserve_27 = 0;
int ProtocolVer = 0x0100;
int DeviceType = 0;
int Reduce_CO2 = 0,Reduce_CO2_H = 0,Reduce_CO2_L = 0;
long Reduce_CO2_temp = 0;
int Uarms = 0,Ubrms = 0,Ucrms = 0;
int Iarms = 0,Ibrms = 0,Icrms = 0;
int f_code_flg = 0;
int f_flash_erase = 0;
int f_flash_program = 0;
int f_last_row = 0;
int32 RowNumber = 0,RowNumberOld = 0;
Uint32 recv_word_cnt = 0,recv_word_len = 0;
Uint32 AddressHigh = 0,AddressAbs = 0;
Uint32 AddressAbsTemp = 0,AddressAbsPredict = 0;
Uint16 FlashData[1400] = {0};

int f_parameter = 0;
int Etotal_set[8] = {0};
int parm_set_temp[80] = {0};
int parm_set_val[80] = {0};
int Scil_passthrough_flg=0;
int SCIRerr_cnt=0;
void ScilRecvDataDeal(void)
{
    Uint16 i = 0;
    int hex_check = 0;
    static int Up_downloadReady_flg=0;
    if(scil_status == RX_OVER_L)
    {
        ScilComDelay();
        if(scil_rlen > 2)
        {
       /*  #ifdef CKFA_Master
            if((ScilRecvBuf[0]==0x00)&&(ScilRecvBuf[1]==0xff)&&(ScilRecvBuf[2]==0x00)
               &&(ScilRecvBuf[3]==0xff)&&(ScilRecvBuf[4]==0x01)&&(ScilRecvBuf[5]==0x04)
               &&(ScilRecvBuf[6]==0x24)&&(ScilRecvBuf[7]==0x6C))//MASTER芯片远程升级
         #else
            if((ScilRecvBuf[0]==0x00)&&(ScilRecvBuf[1]==0xff)&&(ScilRecvBuf[2]==0x00)
              &&(ScilRecvBuf[3]==0xff)&&(ScilRecvBuf[4]==0x01)&&(ScilRecvBuf[5]==0x05)
              &&(ScilRecvBuf[6]==0xE5)&&(ScilRecvBuf[7]==0xAC))//Slave芯片远程升级
        #endif*/
#ifdef CKFA_Master
        if(((ScilRecvBuf[0]==0x00)&&(ScilRecvBuf[1]==0xfe)&&(ScilRecvBuf[2]==0x00)
           &&(ScilRecvBuf[3]==0xfe)&&(ScilRecvBuf[4]==0x01)&&(ScilRecvBuf[5]==0x04)
           &&(ScilRecvBuf[6]==0x48)&&(ScilRecvBuf[7]==0x6C))||//MASTER芯片远程升级
                ((ScilRecvBuf[0]==0x00)&&(ScilRecvBuf[1]==0xff)&&(ScilRecvBuf[2]==0x00)
               &&(ScilRecvBuf[3]==0xff)&&(ScilRecvBuf[4]==0x01)&&(ScilRecvBuf[5]==0x04)
               &&(ScilRecvBuf[6]==0x24)&&(ScilRecvBuf[7]==0x6C)))
     #else
        if(((ScilRecvBuf[0]==0x00)&&(ScilRecvBuf[1]==0xfe)&&(ScilRecvBuf[2]==0x00)
          &&(ScilRecvBuf[3]==0xfe)&&(ScilRecvBuf[4]==0x01)&&(ScilRecvBuf[5]==0x05)
          &&(ScilRecvBuf[6]==0x89)&&(ScilRecvBuf[7]==0xAC))||//Slave芯片远程升级
                ((ScilRecvBuf[0]==0x00)&&(ScilRecvBuf[1]==0xff)&&(ScilRecvBuf[2]==0x00)
              &&(ScilRecvBuf[3]==0xff)&&(ScilRecvBuf[4]==0x01)&&(ScilRecvBuf[5]==0x05)
              &&(ScilRecvBuf[6]==0xE5)&&(ScilRecvBuf[7]==0xAC)))
    #endif
                {
                if(CKFA_Wait_flg==1)//等待中接收到远程升级指令
                {
                    CKFA_Wait_cnt=0;
                    Up_downloadReady_flg=0;
                    ckfa_switch=1;
                    Up_download_flg[0]=0x01;
                    Up_download_flg[1]=0x02;
                }
                else
                {
                    for(i=0;i<8;i++)
                    {
                        ScilSendBuf[i]=ScilRecvBuf[i];
                    }
                    scil_slen = 8;
                    // 最后打开传送模式
                    scil_status = TX_L;
                    Scil_passthrough_flg=1;
                    Up_downloadReady_flg=1;
                }
            }
           else if((ScilRecvBuf[0]==0x00)&&(ScilRecvBuf[1]==0x00)&&(ScilRecvBuf[2]==0x00)
               &&(ScilRecvBuf[3]==0x33)&&(ScilRecvBuf[4]==0x01)&&(ScilRecvBuf[5]==0x00)
               &&(ScilRecvBuf[6]==0x34))           //退出升级
            {
               if(Up_downloadReady_flg==0)
               {
                   scil_status = IDLE_L;
                   SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
                return;
               }
                if((f_start_update==0)&&(eeprom_save_cmd==NULL_E)&&(eeprom_read_cmd==NULL_E)&&(f_ckfa_stop==0))
                {
                    DateSaveBuf[0] = 0x0F;
                    DateSaveBuf[1] = 0x0F;
                    DateSaveBuf[2] = 0x0F;
                    DateSaveBuf[3] = 0x0F;
                    eeprom_len = 4;
                    eeprom_start_addr = ADDR_CKFA_FLAG;
                    eeprom_save_cmd = SAVE_E;
                    eeprom_read_cmd = UPD_OUT_E;
                }
                else
                {
                   // RS485RxEn();
                    scil_status = IDLE_L;
                    SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
                }
            }
            else if(f_code_flg==1)
            {
                f_code_flg=0;
                if(Up_downloadReady_flg==0)
                {
                    scil_status = IDLE_L;
                    SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
                 return;
                }

                    hex_check = HexSumCheck(ScilRecvBuf,scil_rlen-1);
                    if((hex_check == ScilRecvBuf[scil_rlen-1]))
                    {
                        if (ScilRecvBuf[2]==0xFD)//7E  00 00 FD sum 7D 擦除FLASH
                        {
                            DateSaveBuf[0] = 0x01;
                            DateSaveBuf[1] = 0x02;
                            eeprom_len = 2;
                            eeprom_start_addr = ADDR_CKFA_IN;
                            eeprom_save_cmd = SAVE_E;
                            eeprom_read_cmd = UPD_IN_E;
                            RowNumberOld = -1;
                            recv_word_cnt = 0;
                            scil_status = PROCESS_L;
                        }
                       if(f_flash_program==0)
                        {
                               if((f_erase_status==ERASE_SUCCESS)||(f_erase_status==ERASE_SUCCESSED_PROGRAMING))
                               {
                                  if(scil_rlen==ScilRecvBuf[2]+7)
                                  {
                                    RowNumber = (((int32)ScilRecvBuf[0])<<8) + (int32)ScilRecvBuf[1];
                                    if(RowNumber == (RowNumberOld+1))
                                    {
                                        RowNumberOld = RowNumber;

                                        if(ScilRecvBuf[5]==0x04)//地址
                                        {
                                            AddressHigh = (Uint32)ScilRecvBuf[7];
                                            SendFeedBack(0x01);
                                            ScilSendStart();
                                        }
                                        else if(ScilRecvBuf[5]==0x01)
                                        {
                                            recv_word_len = recv_word_cnt;
                                            recv_word_cnt = 0;
                                            f_flash_program = 1;
                                            f_last_row = 1;
                                            SendFeedBack(0x01);
                                            ScilSendStart();
                                        }
                                        else if(ScilRecvBuf[5]==0x00)
                                        {
                                            AddressAbsTemp = (AddressHigh<<16)+(((Uint32)ScilRecvBuf[3])<<8)+((Uint32)ScilRecvBuf[4]);

                                            if(recv_word_cnt==0)
                                            {
                                                AddressAbs = AddressAbsTemp;
                                            }
                                            else
                                            {
                                                //判断地址是否连续
                                                if(AddressAbsPredict != AddressAbsTemp)
                                                {
                                                    AddressAbsPredict = AddressAbsTemp;
                                                    recv_word_len = recv_word_cnt;
                                                    recv_word_cnt = 0;
                                                    f_flash_program = 1;
                                                    RowNumberOld = RowNumberOld - 1;   //待理解
                                                    SendFeedBack(0x00);
                                                    ScilSendStart();
                                                }
                                            }

                                            if(f_flash_program != 1)
                                            {
                                                //预测下一行的起始地址
                                                AddressAbsPredict = AddressAbsTemp+(((Uint32)ScilRecvBuf[2])>>1);

                                                for(i=3;((i<<1)<(scil_rlen-1));i++)
                                                {

                                                    FlashData[recv_word_cnt] = (((Uint16)ScilRecvBuf[(i<<1)])<<8)+(Uint16)ScilRecvBuf[(i<<1)+1];
                                                 if(FlashData[recv_word_cnt]==0xFFFF)
                                                 {
                                                     SCIRerr_cnt++;
                                                     if(SCIRerr_cnt>5)
                                                     {
                                                         SCIRerr_cnt=0;
                                                         SendFeedBack(0x03);
                                                         ScilSendStart();
                                                     }

                                                 }
                                                 else
                                                 {
                                                     SCIRerr_cnt=0;
                                                 }
                                                 recv_word_cnt++;
                                                }
                                                if(recv_word_cnt>1023)
                                                {
                                                    recv_word_len = recv_word_cnt;
                                                    recv_word_cnt = 0;
                                                    f_flash_program = 1;
                                                }
                                                SendFeedBack(0x01);
                                                ScilSendStart();
                                            }
                                        }
                                        else
                                        {
                                            RowNumberOld-=1;
                                            //待写代码,如果发送的数据命令不正确该怎么处理
                                        }
                                    }
                                    else if(RowNumber == RowNumberOld)    //行数错误
                                    {
                                        SendFeedBack(0x01);
                                        ScilSendStart();
                                    }
                                    else                                  //行数严重错误
                                    {
                                        SendFeedBack(0x99);
                                        ScilSendStart();
                                    }
                                  }
                                  else
                                  {
                                      SendFeedBack(0x09);
                                      ScilSendStart();
                                  }
                                }
                                else
                                {
                              //      SendFeedBack(0x06);//7E 06 06 7D未擦除
                              //      ScilSendStart();
                                }
                    }
                   else if(f_flash_program==1)
                  {
                      SendFeedBack(0x02);
                      ScilSendStart();
                  }
                  else if(f_flash_program==2)
                  {
                      SendFeedBack(0x04);
                      ScilSendStart();
                  }
                  else if(f_flash_program==3)
                  {
                      SendFeedBack(0x07);
                      ScilSendStart();
                  }
                  else if(f_flash_program==4)
                  {
                      SendFeedBack(0x08);
                      ScilSendStart();
                  }

                }
                else
                {
                    SendFeedBack(0x00);
                    ScilSendStart();
                }

            }
            else    //if not (scil_rlen > 4)
            {
               // RS485RxEn();
                scil_status = IDLE_L;
                SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
            }
        }
        else    //if not (scil_rlen > 4)
        {
           // RS485RxEn();
            scil_status = IDLE_L;
            SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
        }
    }
}


#pragma CODE_SECTION(SendFeedBack, "ramfuncs");
void SendFeedBack(int feedback_cmd)
{
    int temp=0;
    if((feedback_cmd==1)||(feedback_cmd==0))
    {
        ScilSendBuf[0] = ScilRecvBuf[0];
        ScilSendBuf[1] = ScilRecvBuf[1];
        ScilSendBuf[2] = 0x01;
        ScilSendBuf[3] = 0x00;
        ScilSendBuf[4] = feedback_cmd;
        scil_slen = 6;

    }
    else if(feedback_cmd==0x99)
    {
        ScilSendBuf[0] = ((RowNumberOld>>8) & 0xFF);
        ScilSendBuf[1] = (RowNumberOld & 0xFF);
        ScilSendBuf[2] = 0x01;
        ScilSendBuf[3] = 0x00;
        ScilSendBuf[4] = 0x01;
        scil_slen = 6;
    }
    else
    {
        ScilSendBuf[0] = (feedback_cmd & 0xFF);
        scil_slen = 2;
    }
    if((ScilSendBuf[0]==0x00)&&
       (ScilSendBuf[1]==0x54)&&
       (ScilSendBuf[2]==0x01)&&
       (ScilSendBuf[3]==0x00)&&
       (ScilSendBuf[4]==0x00))
    {
        temp=1;
    }
}
