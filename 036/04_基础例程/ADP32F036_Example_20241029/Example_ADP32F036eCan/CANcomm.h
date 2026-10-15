#ifndef CANCOMM_H_
#define CANCOMM_H_

#include "OnChip_Driver_Inc.h"

#define CANSENDNOWAIT   //CAN 发送不等待发送完毕 TA 标志 (默认测试程序选用)
#define CANRxLEN 20

typedef struct CAN_Rev{
	union
	  {
	    Uint16 All;
	    struct{
	            Uint16  RevFlag                  :1;  //接收到数据标志
	            Uint16  HWOVFlag                 :1;  //DSP硬件接收数据溢出标志
	            Uint16  rFifoDataflag            :1;  //接收内存非空
	            Uint16  rFifoFullFlag             :1;  //接收内存溢出

	    		}Status_Bits;
	  	}Mark_Para;
	Uint16 rxData[CANRxLEN][4];				     //接收缓存, [0]放较低位数据
	Uint16 rxReadIndex;                         //接收FIFO写入索引
	Uint16 rxWriteIndex;                        //接收FIFO读出索引
}RCAN_Msg;

extern RCAN_Msg CANRxMsg[];
extern RCAN_Msg *PtrCanRx;



typedef struct CAN_Tx{
	Uint16 DA;
	Uint16 DB;
	Uint16 DC;
	Uint16 DD;

	Uint16 MBXnbr;                      //指定发送的邮箱
	Uint16 RevMBXnID ;                  //指定发送的ID号,取值范围 0x00 到 0x3FF , 11bit 标准帧ID
}TCAN_Msg;

extern TCAN_Msg  CANTxMsg[];
extern TCAN_Msg *PtrCanTx;


void CAN_Init_Interrupt(void);
void canMailboxInit(void);
void InitECana_1(void);

void CANInit(void);
void CAN_Send(TCAN_Msg *CANTx);
void handleCANRxFIFO(void);

#endif // CANCOMM_H_
