

#ifndef CANCOMM_H_
#define CANCOMM_H_

#include <HeardFiles/ADP32F03xHeader/ADP32F03x_Device.h>
#include <HeardFiles/ADP32F03xHeader/ADP32F03x_ECan.h>

#define   CAN_BAUD_500K
//#define   CAN_BAUD_250K

#define   MAILBOX_LENGTH  (sizeof(struct MBOX))
#define   CANRECV_BOXDATA_LENGTH  8
#define   CANRxLEN 20

typedef struct CAN_Rev{
	union {
	    Uint16 All;
	    struct{
	            Uint16  RevFlag                  :1;  //接收到数据标志
	            Uint16  HWOVFlag                 :1;  //DSP硬件接收数据溢出标志
	            Uint16  rFifoDataflag            :1;  //接收内存非空
	            Uint16  rFifoFullFlag            :1;  //接收内存溢出
	    	}Status_Bits;
	}Mark_Para;
	Uint16 rxData[CANRxLEN][4];				    //接收缓存, [0]放较低位数据
	Uint16 rxReadIndex;                         //接收FIFO写入索引
	Uint16 rxWriteIndex;                        //接收FIFO读出索引
}RCAN_Msg;

extern RCAN_Msg CANRxMsg[];
extern RCAN_Msg *PtrCanRx;

typedef  struct  __CanMailBox_MsgData_Struct {
	Uint32   mMailBox_DataL;
	Uint32   mMailBox_DataH;
}CanMailBox_MsgData_Struct;


typedef  struct __CanMsgData_Entry {
	Uint32 mMsgId;
	Uint32 mMsgCtrl;
	Uint32 CanDataLow;
	Uint32 CanDataHigh;
}CanMsgData_Entry;

typedef struct __CanRecv_MsgData_Struct {
	int  mHeader;
	int  mTailer;
	int  mCount;
	CanMsgData_Entry mCanMsg_Entry[CANRECV_BOXDATA_LENGTH];
}CanRecv_MsgData_Struct;

typedef union
{
    Uint32 all;
    struct
    {                           // bit  description
        Uint32  Mailbox0:1;     // 0   RMP for Mailbox 0
        Uint32  Mailbox1:1;     // 1   RMP for Mailbox 1
        Uint32  Mailbox2:1;     // 2   RMP for Mailbox 2
        Uint32  Mailbox3:1;     // 3   RMP for Mailbox 3
        Uint32  Mailbox4:1;     // 4   RMP for Mailbox 4
        Uint32  Mailbox5:1;     // 5   RMP for Mailbox 5
        Uint32  Mailbox6:1;     // 6   RMP for Mailbox 6
        Uint32  Mailbox7:1;     // 7   RMP for Mailbox 7
        Uint32  Mailbox8:1;     // 8   RMP for Mailbox 8
        Uint32  Mailbox9:1;     // 9   RMP for Mailbox 9
        Uint32  Mailbox10:1;    // 10  RMP for Mailbox 10
        Uint32  Mailbox11:1;    // 11  RMP for Mailbox 11
        Uint32  Mailbox12:1;    // 12  RMP for Mailbox 12
        Uint32  Mailbox13:1;    // 13  RMP for Mailbox 13
        Uint32  Mailbox14:1;    // 14  RMP for Mailbox 14
        Uint32  Mailbox15:1;    // 15  RMP for Mailbox 15
        Uint32  Mailbox16:1;    // 16  RMP for Mailbox 16
        Uint32  Mailbox17:1;    // 17  RMP for Mailbox 17
        Uint32  Mailbox18:1;    // 18  RMP for Mailbox 18
        Uint32  Mailbox19:1;    // 19  RMP for Mailbox 19
        Uint32  Mailbox20:1;    // 20  RMP for Mailbox 20
        Uint32  Mailbox21:1;    // 21  RMP for Mailbox 21
        Uint32  Mailbox22:1;    // 22  RMP for Mailbox 22
        Uint32  Mailbox23:1;    // 23  RMP for Mailbox 23
        Uint32  Mailbox24:1;    // 24  RMP for Mailbox 24
        Uint32  Mailbox25:1;    // 25  RMP for Mailbox 25
        Uint32  Mailbox26:1;    // 26  RMP for Mailbox 26
        Uint32  Mailbox27:1;    // 27  RMP for Mailbox 27
        Uint32  Mailbox28:1;    // 28  RMP for Mailbox 28
        Uint32  Mailbox29:1;    // 29  RMP for Mailbox 29
        Uint32  Mailbox30:1;    // 30  RMP for Mailbox 30
        Uint32  Mailbox31:1;    // 31  RMP for Mailbox 31
    }bit;
}Mailbox_ReadyBits;

extern  CanRecv_MsgData_Struct     CanRecv_MsgData_Buffer;
extern  CanMailBox_MsgData_Struct  CanMailBox_MsgData;
extern  Mailbox_ReadyBits          CanMailBoxReadyFlag;

extern const unsigned int  *VarAddr[];
extern unsigned int RealTimeDataSendFlag;
extern unsigned int RunSignal;


// 接收报文byte0~3
struct  CAN_R_MDL_BITS {   // bits  description
   Uint16      ECC_R_BYTE3:8;        // BYTE3
   Uint16      ECC_R_BYTE2:8;        // BYTE2
   Uint16      ECC_R_BYTE1:8;        // BYTE1
   Uint16      ECC_R_BYTE0:8;        // BYTE0
};

/* Allow access to the bit fields or entire data */
union CAN_R_MDL {
   Uint32                         all;
   struct CAN_R_MDL_BITS  bit;
};

// 接收报文byte4~7
struct  CAN_R_MDH_BITS {   // bits  description
   Uint16      ECC_R_BYTE7:16;        // BYTE7
   Uint16      ECC_R_BYTE6:16;        // BYTE6
   Uint16      ECC_R_BYTE5:16;        // BYTE5
   Uint16      ECC_R_BYTE4:16;        // BYTE4
};

/* Allow access to the bit fields or entire data */
union CAN_R_MDH {
   Uint32                         all;
   struct CAN_R_MDH_BITS  bit;
};

// EAS发给ECC的状态报文格式
struct  CAN_T_MDL_BITS {
   Uint16      CAN_T_BYTE3:8;           // BYTE3
   Uint16      CAN_T_BYTE2:8;           // BYTE3
   Uint16      CAN_T_BYTE1:8;           // BYTE1
   Uint16      CAN_T_BYTE0:8;           // BYTE0
};

/* Allow access to the bit fields or entire data */
union CAN_T_MDL {
   Uint32                 all;
   struct CAN_T_MDL_BITS  bit;
};

struct  CAN_T_MDH_BITS {       // bits  description
   Uint16      CAN_T_BYTE7:8;     // BYTE7
   Uint16      CAN_T_BYTE6:8;     // BYTE6
   Uint16      CAN_T_BYTE5:8;     // BYTE5
   Uint16      CAN_T_BYTE4:8;     // BYTE4
};

/* Allow access to the bit fields or entire data */
union CAN_T_MDH {
   Uint32                         all;
   struct CAN_T_MDH_BITS  bit;
};
typedef enum {
    RUN_MODE_CAN,
    RUN_MODE_PWM,
} RUN_MODE_CHANGE;
extern RUN_MODE_CHANGE RunModeChange;

typedef struct  ECAP_VALUE_STRUC {       // bits  description
    Uint32      pwm_t1;
    Uint32      pwm_t2;
    Uint32      pwm_t3;
    Uint32      pwm_period;
    Uint32      pwm_high;
    Uint32      pwm_duty;
    Uint32      pwm_duty_buff;
    Uint32      rotate_speed;
    Uint32      ElecFreq;
}ECAP_VALUE;
extern ECAP_VALUE Ecap_PwmValve;

extern void Ecap_Handle(void);

extern void CANInit(void);
extern  void  Init_CanMsgData(void);
extern  void  CanTransmit_MailBoxInit(Uint16  mailNo, Uint32 filterNo, Uint16 filterType, Uint16 msgNum);
extern  void  CanReceive_MailBoxInit(Uint16  mailNo, Uint32 filterNo, Uint16 filterType, Uint32 mTimout);
extern  void  CanTransmit_MsgData(Uint16 mChanNo, Uint16 msgNum, Uint16 flag, CanMailBox_MsgData_Struct *msgData);
extern  void  CanDevice_Reset(void);
extern  CanMsgData_Entry *CanMsgData_Pop(void);
extern  void  CanMsgData_Push(struct  MBOX *msgBoxData);
extern  void  HandleCanRecveMsg(void);
extern  void  CAN_Init_Interrupt(void);


extern void CanTransmit2ms(void);
extern void Can_tx_data(void);
extern  void  CanRemoteAckDataFill(void);

#endif /* CANCOMM_H_ */
