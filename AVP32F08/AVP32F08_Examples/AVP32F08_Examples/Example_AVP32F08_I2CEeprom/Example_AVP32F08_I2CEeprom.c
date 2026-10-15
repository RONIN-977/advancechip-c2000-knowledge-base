/******************************************************************************
文件名			: Ex_I2CEeprom.c
作者			: 湖南进芯电子科技有限公司
功能描述		: I2C通信例程
            	  本例程实现的功能：通过I2C向EEPROM写数据，然后再读出来。
                  变量定义：
               		- I2cMsgIn1：从EEPROM读到的数据
               		- I2cMsgOut1：通过I2C向EEPROM写入的数据

创建时间		: 07-25-2022
版本			: V1.0
修订时间 		:
修订内容 		:
*******************************************************************************/

#include "AVP32F08_Project.h" // Device Headerfile and Examples Include File

// Note: I2C Macros used in this example can be found in the
// DSP2833x_I2C_defines.h file

// Prototype statements for functions found within this file.
void   I2CA_Init(void);
Uint16 I2CA_WriteData(struct I2CMSG *msg);
Uint16 I2CA_ReadData(struct I2CMSG *msg);
__interrupt void i2c_int1a_isr(void);
void pass(void);
void fail(void);

#define I2C_SLAVE_ADDR        0x50
#define I2C_NUMBYTES          4
#define I2C_EEPROM_HIGH_ADDR  0x00
#define I2C_EEPROM_LOW_ADDR   0x30

// Global variables
// Two bytes will be used for the outgoing address,
// thus only setup 14 bytes maximum
struct I2CMSG I2cMsgOut1={I2C_MSGSTAT_SEND_WITHSTOP,
                          I2C_SLAVE_ADDR,
                          I2C_NUMBYTES,
                          I2C_EEPROM_HIGH_ADDR,
                          I2C_EEPROM_LOW_ADDR,
                          0x12,                   // Msg Byte 1
                          0x34,                   // Msg Byte 2
                          0x56,                   // Msg Byte 3
                          0x78,                   // Msg Byte 4
                          0x9A,                   // Msg Byte 5
                          0xBC,                   // Msg Byte 6
                          0xDE,                   // Msg Byte 7
                          0xF0,                   // Msg Byte 8
                          0x11,                   // Msg Byte 9
                          0x10,                   // Msg Byte 10
                          0x11,                   // Msg Byte 11
                          0x12,                   // Msg Byte 12
                          0x13,                   // Msg Byte 13
                          0x12};                  // Msg Byte 14


struct I2CMSG I2cMsgIn1={ I2C_MSGSTAT_SEND_NOSTOP,
                          I2C_SLAVE_ADDR,
                          I2C_NUMBYTES,
                          I2C_EEPROM_HIGH_ADDR,
                          I2C_EEPROM_LOW_ADDR};

struct I2CMSG *CurrentMsgPtr;				// Used in interrupts
Uint16 PassCount;
Uint16 FailCount;

void main(void)
{
    Uint16 Error;
    Uint16 i;

    CurrentMsgPtr = &I2cMsgOut1;

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

// 步骤1：系统初始化。初始化PLL、看门狗、使能外设时钟等
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


// 步骤2：初始化GPIO
    // InitGpio();          // Skipped for this example
    InitI2CGpio();


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   // 禁止CPU中断
    InitPieCtrl();          // 初始化PIE
    IER = 0x0000;           // 禁止CPU中断
    IFR = 0x0000;           // 清除CPU中断标志
    InitPieVectTable();     // 初始化PIE中断向量表

    // I2C中断向量映射
    EALLOW;
    PieVectTable.I2CINT1A = &i2c_int1a_isr;
    EDIS;


// 步骤4：初始化所有外设
    // InitPeripherals();   // Not required for this example
    I2CA_Init();            // 初始化I2C


// 步骤5：用户自己的代码

    // 清计数变量
    PassCount = 0;
    FailCount = 0;

    // 清接收消息数组
    for (i = 0; i < I2C_MAX_BUFFER_SIZE; i++)
    {
        I2cMsgIn1.MsgBuffer[i] = 0x0000;
    }

    // 使能I2C的PIE级中断
    PieCtrlRegs.PIEIER8.bit.INTx1 = 1;

    // 使能I2C的CPU级中断
    IER |= M_INT8;
    EINT;

    // 主循环
    for(;;)
    {
        //////////////////////////////////
        //		往EEPROM写入数据 		//
        //////////////////////////////////

        // 检查待发送消息状态，看是否应该发送。在变量定义时已将发送消息被初始化为以一个停止位结束的。
        if(I2cMsgOut1.MsgStatus == I2C_MSGSTAT_SEND_WITHSTOP)
        {
            Error = I2CA_WriteData(&I2cMsgOut1);
            // 如果通信正确启动，则消息状态设置为忙，并更新中断服务程序中的CurrentMsgPtr变量；否则，不
            // 执行任何操作，然后进入下一个循环。
            if (Error == I2C_SUCCESS)
            {
                CurrentMsgPtr = &I2cMsgOut1;
                I2cMsgOut1.MsgStatus = I2C_MSGSTAT_WRITE_BUSY;
            }
        }

        ///////////////////////////////////
        //		从EEPROM中读数据		 //
        ///////////////////////////////////

        // 检查发送消息状态，如果状态不是非激活状态，则跳过读取部分。
        if (I2cMsgOut1.MsgStatus == I2C_MSGSTAT_INACTIVE)
        {
            // 检查接收消息状态
            if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
            {
                // 设置EEPROM地址
                while(I2CA_ReadData(&I2cMsgIn1) != I2C_SUCCESS)
                {
                    // 这里可以设置一个计数器来中断无限的whlie循环，EEPROM将在执行写操作时发送一个NACK。
                    // 即使此时写入消息完成，EEPROM仍可能忙于写数据，因此需要尝试多次。
                }
                // 更新当前的消息指针和消息状态
                CurrentMsgPtr = &I2cMsgIn1;
                I2cMsgIn1.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
            }

            // 一旦设置好了EEPROM的内部数据访问地址，发送一个重启消息来读取EEPROM中的数据，用一个停止位来结束
            // 通信过程。变量MsgStatus在中断服务程序中更新。
            else if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_RESTART)
            {
                // 读EEPROM数据
                while(I2CA_ReadData(&I2cMsgIn1) != I2C_SUCCESS)
                {
                    // 这里可以设置一个计数器来中断无限的whlie循环
                }
                // 更新当前的消息指针和消息状态
                CurrentMsgPtr = &I2cMsgIn1;
                I2cMsgIn1.MsgStatus = I2C_MSGSTAT_READ_BUSY;
            }
        }
    }
}

// I2C初始化函数
void I2CA_Init(void)
{
    // Initialize I2C
    I2caRegs.I2CSAR = 0x0050;       // 设置I2C从地址

    #if (CPU_FRQ_150MHZ)
        I2caRegs.I2CPSC.all = 14;   // 设置预分频系数  - 150/(14+1) = 10MHz
    #endif
    #if (CPU_FRQ_100MHZ)            // For 100 MHz SYSCLKOUT
        I2caRegs.I2CPSC.all = 9;    // Prescaler - need 7-12 Mhz on module clk (100/10 = 10MHz)
    #endif

    I2caRegs.I2CCLKL = 10;			// I2C时钟输出细分频设置。注意: 不能为0
    I2caRegs.I2CCLKH = 5;			//
    I2caRegs.I2CIER.all = 0x24;		// 使能停止信号中断和寄存器准备就绪中断

    I2caRegs.I2CMDR.all = 0x0020;	// 退出复位状态，暂停时停止I2C

    I2caRegs.I2CFFTX.all = 0x6000;	// 使能FIFO模式和发送FIFO，禁止发送FIFO中断
    I2caRegs.I2CFFRX.all = 0x2040;	// 使能接收FIFO，禁止接收FIFO中断，清除接收FIFO中断标志

    return;
}

// I2C发送数据函数
Uint16 I2CA_WriteData(struct I2CMSG *msg)
{
    Uint16 i;

    // 确保前一次通信的停止状态位被清除。如果在启动新消息前未检测该位，则I2C可能会混淆。停止位的清除放到SCD位置位之后了。
    if (I2caRegs.I2CMDR.bit.STP == 1)
    {
        return I2C_STP_NOT_READY_ERROR;
    }

    // 设置I2C从地址
    I2caRegs.I2CSAR = msg->SlaveAddress;

    // 检测I2C总线是否繁忙
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
        return I2C_BUS_BUSY_ERROR;
    }

    // 设置发送的字节数（MsgBuffer + Address）
    I2caRegs.I2CCNT = msg->NumOfBytes+2;

    // 设置访问地址
    I2caRegs.I2CDXR = msg->MemoryHighAddr;
    I2caRegs.I2CDXR = msg->MemoryLowAddr;

    for (i=0; i<msg->NumOfBytes; i++)
    {
        I2caRegs.I2CDXR = *(msg->MsgBuffer+i);
    }

    // 主发送器启动发送数据
    I2caRegs.I2CMDR.all = 0x6E20;       // I2C内部数据计数器自减到0时发送一个停止位，I2C主模式，发送模式，7位地址模式，非循环模式，屏蔽自测模式，使能I2C模块，其实信号无需延长，数据长度位8位

    return I2C_SUCCESS;
}

// 读EEPROM数据函数
Uint16 I2CA_ReadData(struct I2CMSG *msg)
{
    // 确保前一次通信的停止状态位被清除。模块清除该位的动作被延迟到SCD位置位之后，如果在启动新消息前未检测该位，则I2C可能会混淆。
    if (I2caRegs.I2CMDR.bit.STP == 1)
    {
        return I2C_STP_NOT_READY_ERROR;
    }

    // 设置I2C从地址
    I2caRegs.I2CSAR = msg->SlaveAddress;

    if(msg->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
    {
        // 检测I2C总线是否繁忙
        if (I2caRegs.I2CSTR.bit.BB == 1)
        {
            return I2C_BUS_BUSY_ERROR;
        }
        I2caRegs.I2CCNT = 2;                    // 设置要发送的字节数
        I2caRegs.I2CDXR = msg->MemoryHighAddr;
        I2caRegs.I2CDXR = msg->MemoryLowAddr;
        I2caRegs.I2CMDR.all = 0x2620;           // 主模式，发送模式，7位地址模式，非循环模式屏蔽自测模式，使能I2C模块，其实信号无需延长，数据长度位8位
    }
    else if(msg->MsgStatus == I2C_MSGSTAT_RESTART)
    {
        I2caRegs.I2CCNT = msg->NumOfBytes;	    // 设置需要读取的字节数
        I2caRegs.I2CMDR.all = 0x2C20;			// I2C内部数据计数器自减到0时发送一个停止位，主模式，接收模式，7位地址模式，非循环模式，屏蔽自测模式，使能I2C模块，其实信号无需延长，数据长度位8位
    }

    return I2C_SUCCESS;
}

// I2C中断服务程序
__interrupt void i2c_int1a_isr(void)            // I2C-A
{
    Uint16 IntSource, i;

    // 确认中断源
    IntSource = I2caRegs.I2CISRC.all;

    // 如果中断源为检测到停止信号
    if(IntSource == I2C_SCD_ISRC)
    {
        // 如果刚完成的消息为写数据消息，则将消息状态复位到非激活状态
        if (CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_WRITE_BUSY)
        {
            CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_INACTIVE;
        }
        else
        {
            // 如果在读EEPROM的配置地址时收到一个NACK信号，在下面的寄存器访问就绪中断源代码中将产生一个停止条件。
            // 在这里接收到停止条件后，将消息状态设置为重试，用户可能希望在生成错误之前限制重试次数。
            if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
            {
                CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP;
            }
            // 如果刚完成的消息是读取EEPROM数据，则将消息重置为非激活状态，并从FIFO中读取数据。
            else if (CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_READ_BUSY)
            {
                CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_INACTIVE;
                for(i=0; i < I2C_NUMBYTES; i++)
                {
                    CurrentMsgPtr->MsgBuffer[i] = I2caRegs.I2CDRR;      // 读I2C的FIFO中的数据
                }

                // 检查接收到的数据是否和发出去的数据是一致的
                for(i=0; i < I2C_NUMBYTES; i++)
                {
                    if(I2cMsgIn1.MsgBuffer[i] == I2cMsgOut1.MsgBuffer[i])
                    {
                        PassCount++;
                    }
                    else
                    {
                        FailCount++;
                    }
                }
                if(PassCount == I2C_NUMBYTES)
                {
                    pass();
                }
                else
                {
                    fail();
                }

            }
        }
    }

    // 如果中断源为寄存器访问准备就绪。
    // 这个中断主要用于确定读取数据通信时的EEPROM地址设置部分何时完成。由于没有命令停止位，这个中断源标志告诉我们什么时候
    // 发送了消息。如果接收到一个NACK信号，则清除NACK信号并发送一个停止命令；否则，转到通信的读数据部分。
    else if(IntSource == I2C_ARDY_ISRC)
    {
        if(I2caRegs.I2CSTR.bit.NACK == 1)
        {
            I2caRegs.I2CMDR.bit.STP = 1;            // 发生停止信号
            I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT; // 清NACK标志
        }
        else if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
        {
            CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_RESTART;
        }
    }

    else
    {
        //
        __asm("   ESTOP0");
    }

    // 响应其他同组中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

// 对EEPROM写和读都正确函数
void pass()
{
    __asm("   ESTOP0");
    for(;;);
}

// 对EEPROM写读不正确函数
void fail()
{
    __asm("   ESTOP0");
    for(;;);
}

//===========================================================================
// No more.
//===========================================================================
