#ifndef  PRINTF_UART_H
#define  PRINTF_UART_H

#include "ADP32F03x_Project.h"
#include <stdio.h>
#include <file.h>

#define SCIBaudRate   115200L
#define TIMEROUTSCI (Uint32)10*(SYSCLK/SCIBaudRate) //估算的等待超时时间，请根据实际修改

#define RTU_TIMEROUT 5 //ms

#define SCI_FIFO_LEN  1 //定义DSP串口FIFO深度

#define UartRxLEN 20  //接收缓存长度
#define UartTxLEN 20  //发送缓存长度


typedef struct Uart_Type{
	union
	  {
	    Uint16 All;
	    struct{
	            Uint16  UartRevFlag           :1;  //接收到数据标志
	            Uint16  rFifoDataflag          :1;  //接收内存非空
	    		}Status_Bits;
	  	}Mark_Para;
	char rxData[UartRxLEN];				 //接收缓存
	Uint16 rxReadIndex;                         //接收FIFO写入索引
	Uint16 rxWriteIndex;                        //接收FIFO读出索引

	Uint16 timerOut;                            //串口通信 ，判断超时 计数
}
Uart_Msg; 

extern Uart_Msg SCI_Msg;

//--------------------------------------------------------------------
void handleRxFIFO(void);
void SCI_Init(void);
void scia_xmit(int a);
void sciPutString(char Enter1,char *s, char Enter2);

void open_uart_debug (void);
int printf(const char* str, ...); 

//---------------------------------------------------------------------
int my_open(const char *path, unsigned flags, int fno);
int my_close(int fno);
int my_read(int fno, char *buffer, unsigned count);
int my_write(int fno, const char *buffer, unsigned count);
off_t my_lseek(int fno, off_t offset, int origin);
int my_unlink(const char *path);
int my_rename(const char *old_name, const char *new_name);


#endif //PRINTF_UART_H


