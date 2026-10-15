#ifndef AT24C02_H_
#define AT24C02_H_

#include "AIP650_IIC.h"


#define EE_PAGE_SIZE   8 //AT24C02 每页8个字节
#define EE_PAGE_NUM    32//AT24C02 一共有32页
#define EE_MEM_BYTE    256//总的容量，单位byte

#define AT24CO2_TIMER_OUT 50 //ms为单位
#define IIC_RST_BIT 0X0020      //IIC 复位


// Status Flags
#define I2C_AL_BIT          0x0001
#define I2C_NACK_BIT        0x0002
#define I2C_ARDY_BIT        0x0004
#define I2C_RRDY_BIT        0x0008
#define I2C_SCD_BIT         0x0020


// I2C State defines
#define IIC_IDLE               0
#define IIC_WRITE              1
#define IIC_READ_STEP1         2
#define IIC_READ_STEP2         3
#define IIC_READ_STEP3         4
#define IIC_BUS_BUSY           5
#define IIC_BUS_ERROR          6


#define EE_COMM_ID     0xA2 //宏定义EEPROM串口操作的地址

//串口接收到数据的位置定义
#define EE_ID_I   0
#define EE_CMD_I  1
#define EE_ADDR_I 2
#define EE_LEN_I  3
#define EE_DATA_START 4


extern I2CSlaveMSG AT24C02Msg;

Uint16 I2CA_Txdata_NoSTOP(struct I2CSlaveMSG *msg);
Uint16 I2CA_Rxdata_STOP(struct I2CSlaveMSG *msg);
Uint16 I2CA_Txdata_STOP(struct I2CSlaveMSG *msg);
unsigned char handleCommEEprom(struct I2CSlaveMSG *msg);


#endif /* AT24C02_H_ */
