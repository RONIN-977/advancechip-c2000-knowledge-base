
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"
#include <stdio.h>
#include <Boot_ file_ encryption.h>
int timer_10ms = 0;
int timer_50ms = 0;
int timer_125ms = 0;
int timer_1s = 0;
int timer_pwrup_15s = 0;
int timer_start_20s = 0;
int cpu_reset_cnt = 0;

int f_parm2812_ok = 0;                     // 2812处读取数据标志
int f_first_addr = 0;
int f_first_rmt = 0;
int f_first_err = 0;
int f_first_mtype = 0;
int f_first_pvtype = 0;
int f_ckfa_in = 0;                   // 进入透传模式标志
int f_en_gudao = 0;                  // 孤岛模式标志位
int sci_err_set_cnt = 0,sci_err_clr_cnt = 0;
int f_ckfa_stop = 0;
int f_ckfa_sci = 0;
int ckfa_sci_cnt = 0;
int f_start_update = 0;
int CKFA_Wait_flg=0;
int CKFA_Wait_cnt=0;
//远程升级增加变量
char Up_download_flg[2]={0,0};
char Up_download_temp_flg[2]={0,0};

Uint16 Ckfa_state_28034[2] = {0,0};
Uint16 Update_state_28034[2] = {0,0};
Uint16 CKF_TEMP=0;
int Status_csm=0;
void Updownload(void);
void Ckfa_Wait(void);
int ProgramVer[18] =
{
    0x02,0x02,0x00,      //主控芯片
    0x03,0x00,0x01,      //从1控芯片
    0x03,0x00,0x01,      //从2控芯片
    0x02,0x00,0x01,      //CPLD芯片
    0x02,0x00,0x00,      //显示芯片
    0xff,0xff,0xff       //通信芯片
};
long ADD1=0x3ed65c;
Uint16 ADD1_data=0;
void main(void)
{
    //InitGpio();
    InitSysCtrl();

    InitPieCtrl();

    InitPieVectTable();

    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    EDIS;
    // 变量初始化
    UserVariableInit();

    E2promInitRead();

    if((Ckfa_state_28034[0]!=0x01)||(Ckfa_state_28034[1]!=0x02))
    {
      //  (*C28Update)();
        CKFA_Wait_flg=1;
    }
    else
    {
        CKFA_Wait_flg=0;
    }
    if((Update_state_28034[0]==0x01)&&(Update_state_28034[1]==0x02))
    {
        f_start_update = 1;
    }
    else
    {
        f_start_update = 0;
    }

    EALLOW;
    Flash_CPUScaleFactor = SCALE_FACTOR;
    EDIS;

    EALLOW;
    Flash_CallbackPtr = NULL;
    //Flash_CallbackPtr = &MyCallbackFunction;
    EDIS;

    // PIE中断使能
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT1;
    EINT;
    ERTM;
    // 启动CpuTimer0,打开看门狗
    StartCpuTimer0();
    EnableDog();
  //  ADD1_data=*(Uint16 *)ADD1;

    //IO_start_Tx=1;
    for(;;)
    {
//-------看门口标志-------
        if(f_ckfa_sci==0)
        {
            if(f_KickDogM>0)      //与50us中断配合，只要有一个地方出错就不能喂狗
            {
                f_KickDogM = 0;
                f_KickDogI = 1;
            }
        }
        //-------EEPROM存储与读取-------
        if(f_i2cw_stop==0)
        {
            if((eeprom_save_cmd==SAVE_E)&&(eeprom_len>0))
            {
                E2promParmSave(eeprom_start_addr,DateSaveBuf,eeprom_len);
            }
            else
            {
                if((eeprom_read_cmd!=NULL_E)&&(eeprom_len>0))
                {
                    E2promParmRead(eeprom_start_addr,DateSaveBuf,eeprom_len);
                }
            }
        }
        //-------10ms执行一次-------
        if(timer_10ms >= PRD_10MS)
        {
            timer_10ms = 0;
            Updownload();
        }

        //-------125ms执行一次-------
        if(timer_125ms >= PRD_125MS)
        {
            timer_125ms = 0;
            LedWork_NegVal();
        }

        //-------1s执行一次-------
        if(timer_1s >= PRD_1S)
        {
            timer_1s = 0;
            Ckfa_Wait();
        }

        //-------对外485通讯接收数据处理-------
        ScilRecvDataDeal();

        if(f_flash_erase==1)
        {
            if(f_erase_status!=ERASE_SUCCESS)
            {
                Status_csm = Example_CsmUnlock();
               if(Status_csm != STATUS_SUCCESS)
               {
                   Example_Error(Status_csm);
               }
                StopInterrupt();
                FlashEraseApi();
                StartInterrupt();
            }
            if(f_erase_status==ERASE_SUCCESS)
            {
                f_flash_program = 0;
                SendFeedBack(0x05);//7E 05 05 7D擦除成功
                ScilSendStart();

            }
            else if((f_erase_status==NO_ERASE)&&(f_start_update==0))
            {
                DateSaveBuf[0] = 0x0F;
                DateSaveBuf[1] = 0x0F;
                PageWrite(ADDR_CKFA_IN,DateSaveBuf,2);
                E2promDelay();
                SendFeedBack(0x06);//7E 06 06 7D未擦除
                ScilSendStart();
            }
            else
            {
                SendFeedBack(0x07);//擦除失败
                ScilSendStart();
            }
            f_flash_erase = 0;
        }

        if(f_flash_program==1)
        {
            f_erase_status=ERASE_SUCCESSED_PROGRAMING;//2021.07.15更改
            ScilComDelay();
            StopInterrupt();
            scil_status = PROCESS_L;
            FlashProgramApi();
            StartInterrupt();
            if(f_program_status==PROGRAM_SUCCESS)
            {
                if(f_last_row==1)
                {
                    f_last_row = 0;
                    f_start_update = 0;
                }
                f_flash_program = 0;
            }
            else if(f_program_status==PROGRAM_FAIL)
            {
                f_flash_program =2;
                //待修改,发生错误应当通知采集器退出
            }
            else if(f_program_status==PROGRAM_FAIL1)
            {
                f_flash_program=3;
            }
            else if(f_program_status==PROGRAM_FAIL2)
            {
                f_flash_program=4;
            }
            scil_status = IDLE_L;
            SciaRegs.SCICTL1.bit.RXENA=1;//打开接收

        }
    }
}

#pragma CODE_SECTION(StopInterrupt, "ramfuncs");
void StopInterrupt(void)
{
    DisableDog();
    StopCpuTimer0();
    PieCtrlRegs.PIEIER1.bit.INTx7 = 0;
    IER &= (~M_INT1);
    DINT;
    DRTM;
    DELAY_US(1000L);
}

#pragma CODE_SECTION(StartInterrupt, "ramfuncs");
void StartInterrupt(void)
{
    DELAY_US(500L);
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT1;
    EINT;
    ERTM;
    StartCpuTimer0();
    DELAY_US(500L);
    EnableDog();
}
void Ckfa_Wait(void)
{
    if(CKFA_Wait_flg==1)
    {
        CKFA_Wait_cnt++;
        if(CKFA_Wait_cnt==20)
        {
             Boot_encryption();
            StopInterrupt();
            (*C28Update)();
        }
    }
    else
    {
        CKFA_Wait_cnt=0;
    }
}
void Updownload(void)
{
    static int delay_cnt=0;
    if((ckfa_switch ==1)&&(Up_download_flg[0]==1)&&(Up_download_flg[1]==2))
    {
        delay_cnt++;
        if(delay_cnt==1)
        {
           PageWrite(5001,Up_download_flg,2);
         //   void PageRead(unsigned int add,char *recv,unsigned int length)
        }
        else if(delay_cnt==15)
        {
            PageRead(5001,Up_download_temp_flg,2);

        }
        else if(delay_cnt==20)
        {
            if((Up_download_temp_flg[0]==Up_download_flg[0])&&(Up_download_temp_flg[1]==Up_download_flg[1]))
            {
                f_ckfa_stop = 1;
            }
            else
            {
                delay_cnt=0;
            }
        }

    }
}
#pragma CODE_SECTION(Example_Error,"ramfuncs");
void Example_Error(Uint16 Status)
{
    //
    // Error code will be in the AL register.
    //
    asm("    ESTOP0");
    asm("    SB 0, UNC");
}
