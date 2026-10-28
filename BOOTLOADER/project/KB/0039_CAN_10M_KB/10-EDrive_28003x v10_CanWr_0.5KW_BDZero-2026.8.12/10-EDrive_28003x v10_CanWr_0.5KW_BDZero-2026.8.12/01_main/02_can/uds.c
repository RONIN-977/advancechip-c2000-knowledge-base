/*
 * uds.c
 *
 *  Created on: 2022Äê3ÔÂ6ÈÕ
 *      Author: 
 */
#include "uds.h"
#include "debugCan.h"
#include "canConfig.h"

uds_Tp_Status_S uds_Tp_Status;
u8 tp_index = 0;
extern CAN_DATATYPE can_Tx;
void TpTx()
{
    u8 i = 1;
    unsigned char* ucTXMsgData;
	switch (uds_Tp_Status.StateMachine)
	{
	    case 0:break;
	    case 1:
	        tp_index = 1;
	        uds_Tp_Status.TimeoutCnt--;
	        if(uds_Tp_Status.TimeoutCnt == 0)
	        {
	            uds_Tp_Status.StateMachine = 0;
	        }
	        break;//wait flowContorl
	    case 2:
            can_Tx.ID = 0x721;
            can_Tx.Length = 8U;
	        can_Tx.Data[0] = (0x20 + tp_index);
	        tp_index++;
	        while((uds_Tp_Status.sendIndex < uds_Tp_Status.sendTotalNum)&&(i<=8))
	        {
	            can_Tx.Data[i++] = uds_Tp_Status.txdata[uds_Tp_Status.sendIndex++];
	        }
	        ucTXMsgData = (unsigned char*)can_Tx.Data;
	        CAN_sendMessage(CANA_BASE, MCU_729_msg+1, can_Tx.Length, (u16*)ucTXMsgData);
	        if(uds_Tp_Status.sendIndex >= uds_Tp_Status.sendTotalNum)
	        {
	            uds_Tp_Status.StateMachine = 0;
	            uds_Tp_Status.sendIndex = 0;
	            uds_Tp_Status.sendTotalNum = 0;
	        }
	        break;//send did
	    default:break;


	}
	
}


