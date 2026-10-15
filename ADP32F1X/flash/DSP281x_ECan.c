// TI File $Revision: /main/5 $
// Checkin $Date: May 1, 2007   10:01:41 $
//###########################################################################
//
// FILE:    DSP281x_ECan.c
//
// TITLE:   DSP281x Enhanced CAN Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "DSP281x_Device.h"     // DSP28 Headerfile Include File
#include "DSP281x_Examples.h"   // DSP28 Examples Include File

//---------------------------------------------------------------------------
// InitECan:
//---------------------------------------------------------------------------
// This function initializes the eCAN module to a known state.
//


void InitECan(void)
{
    EALLOW;
         ECanaRegs.CANTIOC.bit.TXFUNC = 1;  //配置eCAN的TX、RX分别为发送、接收脚
       ECanaRegs.CANRIOC.bit.RXFUNC = 1;

         ECanaRegs.CANMC.bit.STM=0;    //配置CAN为正常模式
       ECanaRegs.CANMC.bit.SCB=1;    //=1:HECC模式,0-31邮箱可用;=0:SCC模式,0-15邮箱可用
       ECanaRegs.CANMC.bit.DBO=1;    //首先接受或发送数据的低位字节

       //RTR位在复位后状态不定，因此在程序进行初始化的时候必须对该位赋值
       ECanaMboxes.MBOX0.MSGCTRL.bit.RTR = 0;           //无远程帧请求
       ECanaMboxes.MBOX16.MSGCTRL.bit.RTR = 0;

       //信息控制寄存器指定了要发送的字节数和发送优先级
      //CAN初始化时应将其所有位初始化为0
       ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
           ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;



       ECanaRegs.CANTA.all       =0xFFFFFFFF;            //发送应答寄存器写1清零
      ECanaRegs.CANRMP.all      =0xFFFFFFFF;            //接收消息挂起寄存器写1清零        
        ECanaRegs.CANGIF0.all     =0xFFFFFFFF;      //清除所有中断标志位
       ECanaRegs.CANGIF1.all     =0xFFFFFFFF;
       //------------------------------------
        ECanaRegs.CANGIM.all      =0x00000001;      //中断0(ECAN0INT)使能
       ECanaRegs.CANMIM.all      =0x00010000;      //16号邮箱中断使能
       ECanaRegs.CANMIL.all      =0x00000000;      //在ECAN0INT(中断0)产生邮箱中断
      //------------------------------------
       //配置位定时参数
       ECanaRegs.CANMC.bit.CCR   =1;               //CCR=1,改变配置请求
       while(ECanaRegs.CANES.bit.CCE!=1){};   //等待CCE置位,CPU必须等待CCE=1后才能对CANBTC寄存器进行操作

        ECanaRegs.CANBTC.bit.SAM=0;                //can模块采样3个值并进行多数表决
        ECanaRegs.CANBTC.bit.BRPREG = 9;             //CAN总线波特率设为500Kbps
        ECanaRegs.CANBTC.bit.TSEG2REG = 2;
        ECanaRegs.CANBTC.bit.TSEG1REG = 10;

           ECanaRegs.CANMC.bit.CCR   =0;              //配置CCR=0,CPU请求正常模式
        while(ECanaRegs.CANES.bit.CCE!=0){};       //等待CCE清零，若被清零则表明CAN模块已经配置完毕
        ECanaRegs.CANME.all=0;                        //屏蔽所有邮箱,在对MSGIDs写操作之前
        EDIS;

}

/******************************************************/
/* Bit configuration parameters for 150 MHz SYSCLKOUT */
/******************************************************/
/*

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 15, for a 80% SP:
--------------------------------------------------------------
BT = 15, TSEG1(reg) = 10, TSEG2(reg) = 2, Sampling Point = 80%
--------------------------------------------------------------
1   Mbps : BRP(reg)+1 = 10      : CAN clock = 15 MHz
500 kbps : BRP(reg)+1 = 20      : CAN clock = 7.5 MHz
250 kbps : BRP(reg)+1 = 40      : CAN clock = 3.75 MHz
125 kbps : BRP(reg)+1 = 80      : CAN clock = 1.875 MHz
100 kbps : BRP(reg)+1 = 100     : CAN clock = 1.5 MHz
50  kbps : BRP(reg)+1 = 200     : CAN clock = 0.75 MHz

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 10, for a 80% SP:
-------------------------------------------------------------
BT = 10, TSEG1(reg) = 6, TSEG2(reg) = 1, Sampling Point = 80%
-------------------------------------------------------------
1   Mbps : BRP(reg)+1 = 15  : CAN clock = 10 MHz
500 kbps : BRP(reg)+1 = 30  : CAN clock = 5 MHz
250 kbps : BRP(reg)+1 = 60  : CAN clock = 2.5 MHz
125 kbps : BRP(reg)+1 = 120 : CAN clock = 1.25 MHz
100 kbps : BRP(reg)+1 = 150 : CAN clock = 1 MHz
50  kbps : BRP(reg)+1 = 300 : CAN clock = 0.5 MHz

The table below shows how to achieve different sampling points with a BT of 15:
---------------------------------------------------------------------
Achieving desired SP by changing TSEG1(reg) & TSEG2(reg) with BT = 15
---------------------------------------------------------------------

TSEG1(reg) = 10, TSEG2(reg) = 2, SP = 80%
TSEG1(reg) = 9, TSEG2(reg) = 3, SP = 73%
TSEG1(reg) = 8, TSEG2(reg) = 4, SP = 66%
TSEG1(reg) = 7, TSEG2(reg) = 5, SP = 60%

Note: BRP(reg), TSEG1(reg) & TSEG2(reg) indicate the actual value that is written
 into the bit fields of the CAN control registers. These values are increased by 1
 by the CAN module when these registers are accessed.

*/







