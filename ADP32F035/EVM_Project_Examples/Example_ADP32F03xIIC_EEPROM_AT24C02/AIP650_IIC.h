#ifndef AIP650_IIC_H_
#define AIP650_IIC_H_

#define IIC_NODE_NUM 2 //IIC节点数，2个，分别是 TM1650/AT24C02
#define IIC_RST_BIT        0X0020      //IIC 复位

extern char LigntVal;

//与AIP650有关的物理地址（需要右移动1位进行标准的IIC操作；标准IIC 的 LSB为读写位，高7位为物理地址位）
#define CMD_SEG  (0X48)>>1  //显示模式 ADDR

//数码管左边第一个定义为DIG1
#define DIG1  (0X6E)>>1 //DIG1 ADDR
#define DIG2  (0X6C)>>1 //DIG2 ADDR
#define DIG3  (0X6A)>>1 //DIG3 ADDR
#define DIG4  (0X68)>>1 //DIG4 ADDR

//SDA采用GPIO32
#define SDAMUX GpioCtrlRegs.GPBMUX1.bit.GPIO32
#define SDADIR GpioCtrlRegs.GPBDIR.bit.GPIO32
#define SDA_H()  GpioDataRegs.GPBSET.bit.GPIO32=1
#define SDA_L() GpioDataRegs.GPBCLEAR.bit.GPIO32 =1

//CLK采用GPIO33
#define CLKMUX GpioCtrlRegs.GPBMUX1.bit.GPIO33
#define CLKDIR GpioCtrlRegs.GPBDIR.bit.GPIO33
#define CLK_H()  GpioDataRegs.GPBSET.bit.GPIO33=1
#define CLK_L() GpioDataRegs.GPBCLEAR.bit.GPIO33 =1

// I2C Message Structure
typedef struct I2CSlaveMSG{
  Uint16 IntSRC[10];            //中断源            测试用
  Uint16 IntSRCCnt;             //中断源记录  测试用
  Uint16 MasterStatus;          //自行软件定义的IIC状态
  Uint16 SlavePHYAddress;       //IIC物理地址（硬件地址）
  Uint16 LogicAddr;             //类似于EEPROM，需要提供逻辑地址
  Uint16 Len;                   //操作数据的长度（不含物理地址）
  Uint16 MsgInBuffer[4]; //发送数组，最大4个深度
  Uint16 MsgOutBuffer[4];//接收数组，最大4个深度
  Uint16 IIC_TimerOUT;   //软件定义的超时变量，在1ms定时器中计数
}I2CSlaveMSG;


extern I2CSlaveMSG *PtrMsg[];
extern I2CSlaveMSG AIP650;
extern unsigned char SEG7Table[];

void InitI2C_Gpio(void);
void   I2CA_Init(void);
Uint16 I2CA_Tx_STOP(struct I2CSlaveMSG *msg);
Uint16 I2CA_Rxdata_STOP(struct I2CSlaveMSG *msg);
void softResetIIC_BUS(void);

void AIP650_Send(char addr,char data);

#endif // AIP650_IIC_H_
