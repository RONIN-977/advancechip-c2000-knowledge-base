#include "F2806x_Device.h"     // DSP2803x Headerfile Include File
#include "F2806x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"



/*----------E2prom存储注意事项----------/
1.相关变量不允许在定时中断中赋值,以免造成存储指令打断冲突,导致存储数据错误.
2.一条指令未存储完成之前,不允许改变上述变量.
/-----------------------------------*/

Uint16 eeprom_len = 0;
Uint16 eeprom_start_addr = 0;
Uint16 eeprom_save_cmd = NULL_E;
Uint16 eeprom_read_cmd = NULL_E;
Uint16 DateSaveBuf[128] = {0};

#pragma CODE_SECTION(E2promParmSave, "ramfuncs");
void E2promParmSave(Uint16 start_addr,Uint16 *save_buf,Uint16 save_length)
{
    static Uint16 save_cnt = 0;
    static Uint16 save_addr = 0;

    if((save_cnt+32)<save_length)
    {
        save_addr = start_addr + save_cnt;
        PageWrite(save_addr,&save_buf[save_cnt],32);
        save_cnt += 32;
    }
    else
    {
        save_addr = start_addr + save_cnt;
        PageWrite(save_addr,&save_buf[save_cnt],(save_length-save_cnt));
        save_cnt = 0;
        save_addr = 0;
        eeprom_save_cmd = NULL_E;
    }
}


#pragma CODE_SECTION(E2promParmRead, "ramfuncs");
void E2promParmRead(Uint16 start_addr,Uint16 *read_buf,Uint16 read_length)
{
    Uint16 i = 0;
    static Uint16 read_cnt = 0;
    static Uint16 read_addr = 0;

    if((read_cnt+32)<read_length)
    {
        read_addr = start_addr + read_cnt;
        PageRead(read_addr,&read_buf[read_cnt],32);
        read_cnt += 32;
    }
    else
    {
        read_addr = start_addr + read_cnt;
        PageRead(read_addr,&read_buf[read_cnt],(read_length-read_cnt));
        read_cnt = 0;
        read_addr = 0;
        switch(eeprom_read_cmd)
        {
            case READ_E:
                scil_cmd_cpy();
                ScilSendBuf[4] = (start_addr>>8)&0xFF;
                ScilSendBuf[5] = start_addr&0xFF;
                ScilSendBuf[6] = (read_length>>8)&0xFF;
                ScilSendBuf[7] = read_length&0xFF;
                for(i=0;i<read_length;i++)
                {
                    ScilSendBuf[i+8] = read_buf[i];
                }
                scil_slen = read_length + 10;
                ScilSendStart();
            break;
            case UPD_IN_E:
                if((read_buf[0]==0x01)&&(read_buf[1]==0x02))
                {
                    f_flash_program=0;
                    f_flash_erase = 1;
                    RowNumberOld = -1;
                    recv_word_cnt = 0;
                }
            break;
            case UPD_OUT_E:
                if((read_buf[0]!=0x01)||(read_buf[1]!=0x02))
                {
                  //  scil_cmd_cpy();
        /*            for(i=0;i<8;i++)
                    {
                        ScilSendBuf[i]=ScilRecvBuf[i];
                    }
                    scil_slen = 8;
                    ScilSendStart();*/
                    f_ckfa_stop = 1;
                }
            break;
            default:
            break;
        }
        eeprom_read_cmd = NULL_E;
    }
}

#pragma CODE_SECTION(E2promInitRead, "ramfuncs");
void E2promInitRead(void)
{
    int i = 0;
    int f_read_ok = 0;
    int temp = 0;
    Uint16 read_temp[32];

    while(f_read_ok==0)
    {
        f_read_ok = 1;
        //读取远程升级状态
        PageRead(ADDR_CKFA_FLAG,read_temp,2);
        for(i=0;i<2;i++)
        {
            if(Ckfa_state_28034[i]!=read_temp[i])
            {
                Ckfa_state_28034[i] = read_temp[i];
                f_read_ok=0;
            }
        }
        //读取用户程序状态
        PageRead(ADDR_CKFA_IN,read_temp,2);
        for(i=0;i<2;i++)
        {
            if(Update_state_28034[i]!=read_temp[i])
            {
                Update_state_28034[i] = read_temp[i];
                f_read_ok=0;
            }
        }
        //读取零漂系数
        PageRead(ADDR_ZERO_CALI,read_temp,32);
        for(i=0;i<16;i++)
        {
            temp = (read_temp[i<<1]<<8)+read_temp[(i<<1)+1];
            if(AdcZeroOffset[i]!=temp)
            {
                AdcZeroOffset[i] = temp;
                f_read_ok=0;
            }
        }
        //读取校准系数
        PageRead(ADDR_COEF_ADJ,read_temp,32);
        for(i=0;i<16;i++)
        {
            temp = (read_temp[i<<1]<<8)+read_temp[(i<<1)+1];
            if(AdcCoefAdjust[i]!=temp)
            {
                AdcCoefAdjust[i] = temp;
                f_read_ok=0;
            }
        }
    }
}



