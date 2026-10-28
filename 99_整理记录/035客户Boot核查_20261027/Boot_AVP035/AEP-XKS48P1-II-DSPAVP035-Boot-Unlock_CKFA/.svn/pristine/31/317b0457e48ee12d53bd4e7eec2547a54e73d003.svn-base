
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"
extern int f_code_flg;
int scil_rbyte = 0,scil_sbyte = 0;
int scil_deal_timer = 0;
int scil_status = IDLE_L;         // 重要! SCIL接收发送状态
int scil_rtimer = 0;              // 重要! SCIL帧数据接收完成计时
long scil_wtimer = 0;              // 重要! SCIL长时间未处于接收状态计数
int scil_rcnt = 0;
int scil_rlen = 0;
int scil_slen = 0;
int scil_scnt = 0;

int ScilRecvBuf[RBUF_SIZE_L+1] = {0};
int ScilSendBuf[SBUF_SIZE_L+1] = {0};

#pragma CODE_SECTION(ScilOverTimeDeal, "ramfuncs");
void ScilOverTimeDeal(void)
{
    // SCIL定时处理
         // 长时间未放开接收处理
        if(scil_status != IDLE_L)
        {
            scil_wtimer++;
            if(scil_wtimer > 180000)              // 5ms*1800 = 9s
            {
                scil_wtimer = 0;
                scil_status = IDLE_L;
                SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
                //RS485RxEn();
            }
        }
        else
        {
            scil_wtimer = 0;
        }
}


#pragma CODE_SECTION(ScilByteDeal, "ramfuncs");
void ScilByteDeal(void)
{
    static int ScilRx_NULL_cnt=0;
    if(ScilFrameErr() || ScilBreakErr())
    {
        // 初始化SCIL寄存器
  //      InitLin();
        InitSci();

        // 初始化SCIL状态
        if((scil_status==RX_L)||(scil_status==RX_7C_L))
           scil_status = IDLE_L;
      //  SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
        scil_wtimer = 0;
        //RS485RxEn();
    }
    else
    {
        if(ScilRx_Ready())
        {
            ScilRx_NULL_cnt=0;
            scil_rbyte = (ScilRead() & 0xFF);
            switch(scil_status)
            {
                case IDLE_L:
                    if(scil_rbyte == 0x7e)
                    {
                        scil_rcnt = 0;
                        scil_rlen = 0;
                        scil_status = RX_L;
                        f_code_flg=1;
                    }
                    else if((scil_rbyte == 0x00))
                    {
                        scil_rcnt=0;
                        ScilRecvBuf[scil_rcnt] = scil_rbyte;
                        scil_rcnt++;
                        scil_status = RX_L;
                    }
                    break;
                case RX_L:
                    if(scil_rbyte == 0x7c)
                    {
                        scil_status = RX_7C_L;
                    }
                    else if(scil_rbyte == 0x7d)
                    {
                        //RS485TxEn();
                        scil_status = RX_OVER_L;
                        scil_rlen = scil_rcnt;
                        SciaRegs.SCICTL1.bit.RXENA=0;//关闭接收
                    }
                    else
                    {
                        if(scil_rcnt < RBUF_SIZE_L)
                        {
                            ScilRecvBuf[scil_rcnt] = scil_rbyte;
                            scil_rcnt++;
                        }
                        else
                        {
                            scil_status = IDLE_L;
                        }
                    }
                    break;
                case RX_7C_L:
                    scil_status = RX_L;
                    if(scil_rcnt < RBUF_SIZE_L)
                    {
                        ScilRecvBuf[scil_rcnt] = (scil_rbyte ^ 0x20);
                        scil_rcnt++;
                    }
                    else
                    {
                        scil_status = IDLE_L;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
          if(ScilRx_NULL_cnt<100)
              ScilRx_NULL_cnt++;
          else if(ScilRx_NULL_cnt==100)
          {
              ScilRx_NULL_cnt++;
              scil_status = RX_OVER_L;
              SciaRegs.SCICTL1.bit.RXENA=0;//关闭接收
              scil_rlen = scil_rcnt;
          }
        }
        if(scil_status == TX_L)
        {
            if(ScilTx_Ready())
            {
                if(scil_scnt < scil_slen)
                {
                    ScilSendBuf[scil_scnt] = ScilSendBuf[scil_scnt] & 0xFF;
                    if((ScilSendBuf[scil_scnt]>=0x7c) && (ScilSendBuf[scil_scnt]<=0x7e))
                    {
                        ScilSendBuf[scil_scnt] ^= 0x20;
                        ScilWrite(0x7c);
                    }
                    else
                    {
                        ScilWrite(ScilSendBuf[scil_scnt]);
                        scil_scnt++;
                    }
                }
                else if((scil_scnt==scil_slen)&&(Scil_passthrough_flg==0))
                {
                    ScilWrite(0x7d);
                    scil_scnt++;
                }
                else
                {
                    Scil_passthrough_flg=0;
                    scil_status = IDLE_L;
                    SciaRegs.SCICTL1.bit.RXENA=1;//打开接收
                   // RS485RxEn();
                    scil_scnt = 0;
                }
            }
        }
    }
}

#pragma CODE_SECTION(ScilSendStart, "ramfuncs");
void ScilSendStart(void)
{
    int scil_send_sum = 0;
    if(scil_slen > 1)

    {
        scil_send_sum = SumCheck(ScilSendBuf,scil_slen-1);
        ScilSendBuf[scil_slen-1] = scil_send_sum;
        scil_scnt = 0;
        ScilWrite(0x7e);
        // 最后打开传送模式
        scil_status = TX_L;
    }
}

#pragma CODE_SECTION(ScilComDelay, "ramfuncs");
void ScilComDelay(void)
{
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
    DELAY_US(4000L);
    ServiceDog();
}


