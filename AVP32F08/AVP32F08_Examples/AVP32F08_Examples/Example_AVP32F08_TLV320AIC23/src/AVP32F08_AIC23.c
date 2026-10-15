/******************************************************************************
文件名					: AVP32F08_AIC23.c
作者                	: 湖南进芯电子科技有限公司
功能描述              	: AIC23音频模块初始化及函数定义
创建时间              	: 06-25-2019
版本                  	: V1.0
修订时间               	:
修订内容              	:
*******************************************************************************/

#include	"AVP32F08_Device.h"

Uint16	volume;

void InitAic23()
{ 
	WriteAic23(RESET,0);                // 复位AIC23寄存器
	WriteAic23(RESET,0);
	WriteAic23(RESET,0);
	WriteAic23(RESET,0);
	WriteAic23(RESET,0);
	WriteAic23(RESET,0);	
	WriteAic23(RESET,0);
	WriteAic23(RESET,0);
	WriteAic23(RESET,0);
	delay(100);	
	WriteAic23(D_INTERFACE_ACT,0x001);  // 激活数字接口
	WriteAic23(POWER_CON,0);            // 全部功能模块上电
	WriteAic23(SAMPLE_RATE,0x022);      // 时钟不分频，输入输出时钟都为44.1k
//	write_AIC23(SAMPLE_RATE,0x062);     // 22.05k
//	WriteAic23(SAMPLE_RATE,0x02e);      // 8.021k
    
	// 左右麦克输入音量控制
// 	WriteAic23(L_LINE_VOLUME,0x0117);   // 左右声道同时更新，左声道输入音量控制为0dB
//	WriteAic23(R_LINE_VOLUME,0x0117);   // 左右声道同时更新，右声道输入音量控制为0dB
	WriteAic23(L_LINE_VOLUME,0x011F);   // 左右声道同时更新，左声道输入音量控制为12dB
    WriteAic23(R_LINE_VOLUME,0x011F);   // 左右声道同时更新，右声道输入音量控制为12dB
   
	// 左右声道耳机输出音量控制
	volume = 98;
	WriteAic23(L_HEADPHONE_VOLUME,0x0180+volume);   // 设置左声道耳机音量
	WriteAic23(R_HEADPHONE_VOLUME,0x0180+volume);   // 设置右声道耳机音量
   
//	write_AIC23(A_AUDIO_PATH,0x009);    //bypass DAC OFF
//	write_AIC23(A_AUDIO_PATH,0x014);    //MIC ADC DAC ON  0DB
//	write_AIC23(A_AUDIO_PATH,0x1fc);    //bypass MIC ADC DAC ON 0DB sidetone
	WriteAic23(A_AUDIO_PATH,0x07d);     // 旁白-9dB，使能旁白，选择DAC，使能滤波，ADC输入为麦克风输入，麦克不静音，麦克风增强20dB
//	write_AIC23(A_AUDIO_PATH,0x011);    //LINE ADC DAC ON
   
	WriteAic23(D_AUDIO_PATH,0x04);      // 禁止DAC静音，时钟44.1kHz，禁止ADC高通滤波
	WriteAic23(D_AUDIO_INTERFACE,0x043);// 数字音频接口：主模式，左右声道不互换，有声道打开，帧信号为高，输入数据字长为16位，DSP格式，帧同步信号后接2个数据字
	WriteAic23(D_AUDIO_INTERFACE,0x043);
	WriteAic23(D_AUDIO_INTERFACE,0x043);
	WriteAic23(D_INTERFACE_ACT,0x001);  // 激活该音频接口
	WriteAic23(D_INTERFACE_ACT,0x001);
}
		
void WriteAic23(Uint16  addr,Uint16  dat)
{
	int flag;
	Uint16 Temp;

	Temp = addr<<9;
	Temp = Temp|dat;

	flag = 0;
	while(flag != 1)
	{
		flag = Spi_TxReady();
	}

	delay(5000);
	if(Spi_TxReady() == 1)
	    SpiaRegs.SPITXBUF = Temp;

	delay(5000);

	if(Spi_TxReady() == 1)
	    SpiaRegs.SPITXBUF = Temp;
}

//===========================================================================
// End of file.
//===========================================================================

