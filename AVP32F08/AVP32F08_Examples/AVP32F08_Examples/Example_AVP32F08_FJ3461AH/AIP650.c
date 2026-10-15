/******************************************************************************
文件名: 	AIP650.c
作者: 		湖南进芯电子科技有限公司
功能描述:	AIP650驱动模块
创建时间: 	07-24-2019
版本:       V1.0
修订时间:
修订内容:
*******************************************************************************/

#include "AVP32F08_Project.h"     // DSP281x Headerfile Include File
#include "AIP650.h"

/********共阴极数码管***********/
Uint16  segList[16] ={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71
};
/****************共阳极*****************
uint8_t SEG_List[16] ={
0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e
};************************************/

void WriteByte(Uint16 data);
void WriteWord(Uint16 data);

//---------------------------------------------------------------------------
// DataDisplay:
// 数据显示函数
//
void DataDisplay(Uint16 data)
{
    Uint16 temp, thousands_digit, hundreds_digit, tens_digit, ones_digit;

    temp = data;
    thousands_digit = temp / 1000;
    hundreds_digit= (temp % 1000) / 100;
    tens_digit = (temp % 100) / 10;
    ones_digit = temp % 10;

    if(temp >= 1000)
    {
        WriteWord(0x6800 | segList[thousands_digit]);
        WriteWord(0x6A00 | segList[hundreds_digit]);
        WriteWord(0x6C00 | segList[tens_digit]);
        WriteWord(0x6E00 | segList[ones_digit]);
    }
    else if(temp >= 100)
    {
        WriteWord(0x6800);
        WriteWord(0x6A00 | segList[hundreds_digit]);
        WriteWord(0x6C00 | segList[tens_digit]);
        WriteWord(0x6E00 | segList[ones_digit]);
    }
    else if(temp >= 10)
    {
        WriteWord(0x6800);
        WriteWord(0x6A00);
        WriteWord(0x6C00 | segList[tens_digit]);
        WriteWord(0x6E00 | segList[ones_digit]);
    }
    else
    {
        WriteWord(0x6800);
        WriteWord(0x6A00);
        WriteWord(0x6C00);
        WriteWord(0x6E00 | segList[ones_digit]);
    }
}

//---------------------------------------------------------------------------
// DisplayEnable:
// 数显使能函数
// 使能数显，并设置显示亮度。bit4~bit7表示亮度值，1、2、3、4、5、6、7、0依次表示1~8级亮度。
//
void DisplayEnable(void)
{
    WriteWord(0x4801);      // 8段显示 1级亮度
}

//---------------------------------------------------------------------------
// Display:
// 给4个8段数显一个初始显示状态
//
void Display(void)
{
    WriteWord(0x6800 | segList[0]);     // 写DIG0的8段显示数据，‘1’亮，‘0’灭
    WriteWord(0x6A00 | segList[1]);     // 写DIG1的8段显示数据，‘1’亮，‘0’灭
    WriteWord(0x6C00 | segList[2]);     // 写DIG2的8段显示数据，‘1’亮，‘0’灭
    WriteWord(0x6E00 | segList[3]);     // 写DIG3的8段显示数据，‘1’亮，‘0’灭
}

//---------------------------------------------------------------------------
// WriteWord:
// 写16-bit的字数据
//
void WriteWord(Uint16 data)
{
    CLK_H
	CLK_H

    DATA_H
	DATA_H

    DATA_L
    DATA_L                 
    DATA_L
    DATA_L                 //开始

    WriteByte(data>>8);     //先传递高八位
    WriteByte(data);        //后传递低八位

    CLK_L
    CLK_L
	CLK_L
    CLK_L

    DATA_L                  //结束
    DATA_L
	DATA_L
	DATA_L

    CLK_H
    CLK_H
	CLK_H
    CLK_H

    DATA_H
    DATA_H
	DATA_H
    DATA_H
}

//---------------------------------------------------------------------------
// WriteByte:
// 写8-bit的字节数据
//
void WriteByte(Uint16 data)
{
    Uint16 i=0;
    Uint16 temp;
    temp = data;
    for(i = 7; i > 0; i--)  // 发送数据的格式为先发高位再发低位
    {
        CLK_L
        CLK_L
		CLK_L
        CLK_L
        if((temp >> i)&0x01)
		{
            DATA_H
			DATA_H
	    }
        else
		{
            DATA_L
			DATA_L
	    }
        CLK_H
        CLK_H
		CLK_H
        CLK_H
    }
    CLK_L
    CLK_L
	CLK_L
    CLK_L
    if((temp >> i)&0x01)
	{
        DATA_H
		DATA_H
	}
    else
	{
        DATA_L
		DATA_L
    }
    CLK_H
    CLK_H
	CLK_H
    CLK_H

    CLK_L
    CLK_L
	CLK_L
    CLK_L

    CLK_H
    CLK_H  
    CLK_H
    CLK_H   //ACK
}

//===========================================================================
// No more.
//===========================================================================
