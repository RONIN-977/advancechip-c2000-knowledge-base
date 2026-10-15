/**************************************************************************************************
 *          文件：      RS422.h
 *          功能：      can通讯函数定义
 *          公司：      鑫航
 *          作者：      龙耀祖
 *          修改：      龙耀祖
 *          版本：      V1.0
 *          说明：     本模块采用 28335
 *          说明：    RS422的SCIb、SCIC采用FIFO进行通讯函数
 *          占用空间：
 *          修改记录：  2025.02.27
 **************************************************************************************************/


#ifndef RS422_H_
#define RS422_H_

#include "common.h"
#include "Init_2833x.h"


#define LOST_RXFRAME_SIZE      100           /*  丢失帧尺寸   */





void scia_xmit(Uint16 a);
void scib_xmit(Uint16 a);
void scia_init(Uint32 baud);
void scib_init(Uint32 baud);
void RS422_Init(Uint32 baud);


typedef struct RS422_err
{
    Uint16 NORefresh;                      /*  未刷新故障   */
    Uint16 SumCheck;                       /*  校验和故障   */
    Uint16 Communication_Falg;             /*  通讯故障   */

} RS422_err;




typedef struct StSCI
{
    /*与VMC的通讯状态包*/
    Uint16 RxVMC422Fault;   //422错误
    Uint16 NoRxVMCDataFault;   //未接收VMC发送的数据块
    Uint16 RxVMCChecksumFault;   //校验和错误
    Uint16 RxVMCDataNoUpdata;  //接收VMC数据块未更新
    Uint16 RxVMCLengthFault;  //接收VMC数据块长度错误
    Uint16 OppRxVMCFault;   //对侧阀接收上位机通讯错误
    Uint16 RxCCDLFault;      //本侧阀接收对侧阀通讯错误

    /*与对侧刹车阀的通讯状态包*/
    Uint16 RxCCDL422Fault; //422错误
    Uint16 NoRxCCDLDataFault; //未接收对侧阀发送的数据块
    Uint16 RxCCDLLengthFault; //接收对侧阀数据块长度错误
    Uint16 RxCCDLDataNoUpdata;  //接收对侧阀数据块未更新
    Uint16 RxCCDLChecksumFault;  //校验和错误
//    Uint16 LocRxVMCFalut;  //本侧阀接收上位机通讯错误
//    Uint16 OppRxVMCFault;  //对侧阀接收上位机通讯错误
}StSCI;





#endif /* RS485_H_ */

