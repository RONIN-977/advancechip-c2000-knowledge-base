#ifndef MOCK_DRIVERLIB_H
#define MOCK_DRIVERLIB_H
#include <stdint.h>
#include <stdbool.h>
#define CANA_BASE 0U
#define CAN_O_IF2MCTL 0U
#define CAN_IF2MCTL_DLC_M 15U
typedef uint16_t CAN_MsgFrameType;
#define CAN_MSG_FRAME_STD 0U
extern uint16_t mock_dlc;
#define HWREGH(address) mock_dlc
bool CAN_readMessageWithID(uint32_t base,uint32_t obj,CAN_MsgFrameType *type,uint32_t *id,uint16_t *data);
uint32_t CAN_getTxRequests(uint32_t base);
void CAN_sendMessage_updateDLC(uint32_t base,uint32_t obj,uint16_t dlc,const uint16_t *data);
#endif
