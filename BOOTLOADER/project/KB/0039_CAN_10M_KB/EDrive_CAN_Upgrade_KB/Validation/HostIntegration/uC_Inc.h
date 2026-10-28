#ifndef MODEL_UC_INC_H
#define MODEL_UC_INC_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#define DEVICE_SYSCLK_FREQ 100000000UL
#define CANA_BASE 0U
#define CPUTIMER2_BASE 2U
#define SYSCTL_PERIPH_CLK_TIMER2 2U
#define CAN_O_IF2MCTL 0U
#define CAN_IF2MCTL_DLC_M 15U
#define CAN_MSG_FRAME_STD 0U
#define CAN_MSG_OBJ_TYPE_RX 1U
#define CAN_MSG_OBJ_TYPE_TX 2U
#define CAN_MSG_OBJ_USE_ID_FILTER 1U
#define CAN_MSG_OBJ_USE_EXT_FILTER 2U
#define CAN_MSG_OBJ_USE_DIR_FILTER 4U
#define CAN_MSG_OBJ_NO_FLAGS 0U
#define EALLOW do {} while (0)
#define EDIS do {} while (0)
typedef unsigned CAN_MsgFrameType;
extern volatile uint16_t request_words[4];
extern uint16_t model_dlc;
#define HWREGH(x) model_dlc
bool Interrupt_disableMaster(void);
void Interrupt_enableMaster(void);
uint32_t CAN_getValidMessageObjects(uint32_t);
void CAN_setupMessageObject(uint32_t,uint16_t,uint32_t,unsigned,unsigned,uint32_t,uint32_t,uint16_t);
uint32_t CAN_getTxRequests(uint32_t);
void CAN_sendMessage(uint32_t,uint16_t,uint16_t,const uint16_t *);
bool CAN_readMessageWithID(uint32_t,uint16_t,CAN_MsgFrameType *,uint32_t *,uint16_t *);
void SysCtl_enablePeripheral(unsigned);
void CPUTimer_stopTimer(unsigned);
void CPUTimer_disableInterrupt(unsigned);
void CPUTimer_setPreScaler(unsigned,unsigned);
void CPUTimer_setPeriod(unsigned,uint32_t);
void CPUTimer_reloadTimerCounter(unsigned);
void CPUTimer_startTimer(unsigned);
uint32_t CPUTimer_getTimerCount(unsigned);
void SysCtl_serviceWatchdog(void);
void SysCtl_delay(uint32_t);
void SysCtl_resetDevice(void);
#endif
