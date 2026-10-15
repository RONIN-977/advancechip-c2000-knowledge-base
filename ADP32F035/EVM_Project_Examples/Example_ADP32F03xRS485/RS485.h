#ifndef RS485_H_
#define RS485_H_

#include "BspInc.h"
//GPIO27 ’∑¢øÿ÷∆IO
#define TEN485MUX   GpioCtrlRegs.GPAMUX2.bit.GPIO27
#define TEN485DIR   GpioCtrlRegs.GPADIR.bit.GPIO27
#define TEN485ON()  GpioDataRegs.GPASET.bit.GPIO27 =1
#define TEN485OFF() GpioDataRegs.GPACLEAR.bit.GPIO27=1

void Init485EN(void);
void start485Send(void);
void wait485Send(void);
void handle485RxFIFO(Scan * PtrSegTask);
void handleScanKey485(Scan * PtrSegTask);

#endif //RS485_H_
