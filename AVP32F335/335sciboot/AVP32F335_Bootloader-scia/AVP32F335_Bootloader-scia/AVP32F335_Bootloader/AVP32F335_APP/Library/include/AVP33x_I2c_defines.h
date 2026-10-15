/******************************************************************************
文件名                     : AVP33x_I2cExample.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x的I2C模块相关宏定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_I2C_DEFINES_H
#define AVP33x_I2C_DEFINES_H

//--------------------------------------------
// Defines
//--------------------------------------------

// 错误信息
#define I2C_ERROR               0xFFFF
#define I2C_ARB_LOST_ERROR      0x0001
#define I2C_NACK_ERROR          0x0002
#define I2C_BUS_BUSY_ERROR      0x1000
#define I2C_STP_NOT_READY_ERROR 0x5555
#define I2C_NO_FLAGS            0xAAAA
#define I2C_SUCCESS             0x0000

// 清除状态标志
#define I2C_CLR_AL_BIT          0x0001
#define I2C_CLR_NACK_BIT        0x0002
#define I2C_CLR_ARDY_BIT        0x0004
#define I2C_CLR_RRDY_BIT        0x0008
#define I2C_CLR_SCD_BIT         0x0020

// Interrupt Source Messages
#define I2C_NO_ISRC             0x0000
#define I2C_ARB_ISRC            0x0001
#define I2C_NACK_ISRC           0x0002
#define I2C_ARDY_ISRC           0x0003
#define I2C_RX_ISRC             0x0004
#define I2C_TX_ISRC             0x0005
#define I2C_SCD_ISRC            0x0006
#define I2C_AAS_ISRC            0x0007

// I2CMSG structure defines
#define I2C_NO_STOP  0
#define I2C_YES_STOP 1
#define I2C_RECEIVE  0
#define I2C_TRANSMIT 1
#define I2C_MAX_BUFFER_SIZE 16

// I2C Slave State defines
#define I2C_NOTSLAVE      0
#define I2C_ADDR_AS_SLAVE 1
#define I2C_ST_MSG_READY  2

// I2C Slave Receiver messages defines
#define I2C_SND_MSG1 1
#define I2C_SND_MSG2 2

// I2C State defines
#define I2C_IDLE               0
#define I2C_SLAVE_RECEIVER     1
#define I2C_SLAVE_TRANSMITTER  2
#define I2C_MASTER_RECEIVER    3
#define I2C_MASTER_TRANSMITTER 4

// I2C  Message Commands for I2CMSG struct
#define I2C_MSGSTAT_INACTIVE          0x0000
#define I2C_MSGSTAT_SEND_WITHSTOP     0x0010
#define I2C_MSGSTAT_WRITE_BUSY        0x0011
#define I2C_MSGSTAT_SEND_NOSTOP       0x0020
#define I2C_MSGSTAT_SEND_NOSTOP_BUSY  0x0021
#define I2C_MSGSTAT_RESTART           0x0022
#define I2C_MSGSTAT_READ_BUSY         0x0023

// Generic defines
#define I2C_TRUE  1
#define I2C_FALSE 0
#define I2C_YES   1
#define I2C_NO    0
#define I2C_DUMMY_BYTE 0


//--------------------------------------------
// Structures
//--------------------------------------------

// I2C Message Structure
struct I2CMSG {
  Uint16 MsgStatus;				// 消息状态:
  								//   I2C_MSGCMD_INACTIVE = 非激活状态，不能发送数据
  								//   I2C_MSGCMD_BUSY = 已经启动消息发送，等待停止信号
  								//   I2C_MSGCMD_SEND_WITHSTOP = 发送命令，作为主发送消息（包含停止位）
  								//   I2C_MSGCMD_SEND_NOSTOP = 发送命令，作为主发送消息（不包含停止位）
  								//   I2C_MSGCMD_RESTART = 发送重启命令，作为主接收消息（包含停止位）
  Uint16 SlaveAddress;			// 要发送的I2C从地址
  Uint16 NumOfBytes;			// 发送字节数
  Uint16 MemoryHighAddr;		// EEPROM数据访问地址高位
  Uint16 MemoryLowAddr;			// EEPROM数据访问地址低位
  Uint16 MsgBuffer[I2C_MAX_BUFFER_SIZE];	// 消息数据队列，最大数量有FIFO决定
};


#endif  // end of AVP33x_I2C_DEFINES_H definition

//===========================================================================
// End of file.
//===========================================================================
