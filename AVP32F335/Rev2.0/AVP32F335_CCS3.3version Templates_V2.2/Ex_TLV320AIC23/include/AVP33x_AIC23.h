/******************************************************************************
文件名                     : AVP33x_AIC23.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AIC23音频模块函数声明和宏定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_AIC23_H
#define AVP33x_AIC23_H


#ifdef __cplusplus
extern "C" {
#endif

#define SPCR10_VAL     0x0000
#define SPCR20_VAL     0x0220
//2 words per frame
#define RCR10_VAL      0x0140
#define RCR20_VAL      0x04
//2 words per frame
#define XCR10_VAL      0x0140
#define XCR20_VAL      0x04
#define PCR0_VAL       0x01

#define   VOL1     0x81
#define   VOL2     0x82

#define   L_LINE_VOLUME        0x00
#define   R_LINE_VOLUME        0x01
#define   L_HEADPHONE_VOLUME   0x02
#define   R_HEADPHONE_VOLUME   0x03
#define   A_AUDIO_PATH         0x04
#define   D_AUDIO_PATH         0x05
#define   POWER_CON            0x06
#define   D_AUDIO_INTERFACE    0x07
#define   SAMPLE_RATE          0x08
#define   D_INTERFACE_ACT      0x09
#define   RESET                0x0f

extern void delay(unsigned int k);
extern void InitAic23(void);
extern void WriteAic23(Uint16 addr, Uint16 dat);
interrupt void ISRMcbspR(void);
interrupt void ISRMcbspX(void);
extern Uint16 Spi_TxReady(void);
extern Uint16 Spi_RxReady(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_AIC23_H definition

//===========================================================================
// End of file.
//===========================================================================
