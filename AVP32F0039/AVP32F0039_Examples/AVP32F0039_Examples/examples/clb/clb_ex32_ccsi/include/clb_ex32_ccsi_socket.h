/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   clb_ex32_ccsi_socket.h
*                         TITLE:  Prototype definitions for CCSI socket.
*******************************************************************************************/

#ifndef CLB_EX32_CCSI_SOCKET_H_
#define CLB_EX32_CCSI_SOCKET_H_

#include "driverlib.h"

void CCSI_write(uint16_t headBytes, uint16_t *dataBytes, uint16_t length, unsigned int checkResponse);
void CCSI_read(uint16_t headBytes);
void CCSI_write_sync(uint16_t headBytes, uint16_t *dataBytes, uint16_t length);
void processReadData(uint16_t headBytes, uint16_t length, unsigned int compareCheckBit);

#endif /* CLB_EX32_CCSI_SOCKET_H_ */
