/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   i2cLib_FIFO_controller_target_interrupt.c
*                         TITLE:  C28x-I2C Library source file for FIFO interrupts
*******************************************************************************************/

#ifndef I2CLIB_FIFO_INTERRUPT_H
#define I2CLIB_FIFO_INTERRUPT_H

#include "device.h"

//
// Error messages for read and write functions
//
#define ERROR_BUS_BUSY              0x1000
#define ERROR_NACK_RECEIVED         0x2000
#define ERROR_ARBITRATION_LOST      0x3000
#define ERROR_STOP_NOT_READY        0x5555
#define SUCCESS                     0x0000

#define MAX_BUFFER_SIZE             64
#define I2C_FIFO_LEVEL              16

#define MAX_7_BIT_ADDRESS 127U
#define MAX_10_BIT_ADDRESS 1023U

#define MAX_I2C_IN_NETWORK 20


//
// Typedefs
//
struct I2CHandle
{
    uint32_t base;
    uint16_t TargetAddr;                  // target address tied to the message.
    uint32_t *pControlAddr;
    uint16_t NumOfAddrBytes;
    uint16_t *pTX_MsgBuffer;             // Pointer to TX message buffer
    uint16_t *pRX_MsgBuffer;             // Pointer to RX message buffer
    uint16_t NumOfDataBytes;             // Number of valid bytes in message.
    struct I2CHandle *currentHandlePtr;

    uint16_t numofSixteenByte;
    uint16_t remainingBytes;
};

uint16_t I2C_TransmitTargetAddress_ControlBytes(struct I2CHandle *I2C_Params);
uint16_t I2CBusScan(uint32_t base, uint16_t *pAvailableI2C_targets);

uint16_t I2C_ControllerTransmitter(struct I2CHandle *I2C_Params);
uint16_t I2C_ControllerReceiver(struct I2CHandle *I2C_Params);
uint16_t I2C_TargetTransmitter(struct I2CHandle *I2C_Params);
uint16_t I2C_TargetReceiver(struct I2CHandle *I2C_Params);

uint16_t checkBusStatus(uint32_t base);

uint16_t handleNACK(uint32_t base);

extern void handleI2C_ErrorCondition(struct I2CHandle *I2C_Params);
extern void Write_Read_TX_RX_FIFO(struct I2CHandle *I2C_Params);

extern uint16_t status;

extern struct I2CHandle *currentTargetPtr;

#endif


