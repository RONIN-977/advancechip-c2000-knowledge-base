/******************************************************************************
文件名: 	AIP650.h
作者: 		湖南进芯电子科技有限公司
功能描述:	AIP650驱动模块申明
创建时间: 	07-24-2019
版本:       V1.0
修订时间:
修订内容:
*******************************************************************************/
#ifndef AIP650_H
#define AIP650_H


#ifdef __cplusplus
extern "C" {
#endif


#define CLK_H   GpioDataRegs.GPBSET.bit.GPIO35 = 1;     // 数码管时钟高
#define CLK_L   GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;   // 数码管时钟低
#define DATA_H  GpioDataRegs.GPASET.bit.GPIO30 = 1;     // 数码管数据高
#define DATA_L  GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;   // 数码管数据低

extern void DataDisplay(Uint16 data);                   // 数显函数
extern void DisplayEnable(void);                        // 数显使能函数
extern void Display(void);                              // 数显初始显示设置函数


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of AIP650_H definition

//===========================================================================
// No more.
//===========================================================================

