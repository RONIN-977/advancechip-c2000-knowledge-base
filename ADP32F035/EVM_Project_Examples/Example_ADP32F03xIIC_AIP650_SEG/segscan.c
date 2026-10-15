#include "OnChip_Driver_Inc.h"
#include "segscan.h"
#include "BspInc.h"
#include <math.h>

char *keyStr[]={"K1","K2","K3","K4"};
char poitPosition=0;//存放小数点的位置
float ftestData = 0.01;//数码管内容的变化量
char keyValCode[4] = {1,2,3,4};
char keyVal;
Scan Running;


// 数码管显示内容获取
// 先计算显示数字，后计算显示字母,
// 显示字母优先级高于显示数字
void segGetShow(Scan * PtrSegTask)
{
    float tempfloatdata;
    float pointData=0;   //小数部分
    int iterData=0;      //正数部分
    char i;
//-------------------------------------showNum转化为showRam显示---------------------------------------------------------
    tempfloatdata = PtrSegTask ->showNum;
    //这里限幅显示
    if(tempfloatdata  > 9999) tempfloatdata  = 9999;
    else if(tempfloatdata < -999)  tempfloatdata = -999;
    //显示正数
    if(tempfloatdata >=0)
    {
                //获取小数点
        if(tempfloatdata >=1000)
            poitPosition = 0;
        else if(tempfloatdata >=100)
            poitPosition = 1;
        else if(tempfloatdata >=10)
            poitPosition = 2;
        else if(tempfloatdata >0)
            poitPosition = 3;

        //清空显示
        PtrSegTask ->showRam[3] = 0x00;
        PtrSegTask ->showRam[2] = 0x00;
        PtrSegTask ->showRam[1] = 0x00;
        PtrSegTask ->showRam[0] = 0x00;

        pointData = (pow(10,poitPosition) * (tempfloatdata) );
        iterData = (float)pointData;
        //获取显示的内容，整数与小数
        for(i=0;i< 4;i++)
        {
            PtrSegTask ->showRam[i] = SEG7Table[iterData%10];
            iterData = iterData/10;
        }
        //添加小数点
        PtrSegTask ->showRam[poitPosition] |= 0x80;// 共阴极数码管|=0x80; 共阳极数码管 &=0x7F
    }
    //显示负数
    else
    {
        //获取小数点
        if(tempfloatdata <= (-100))
            poitPosition = 0;
        else if(tempfloatdata <=(-10))
            poitPosition = 1;
        else if(tempfloatdata <0)
            poitPosition = 2;

        PtrSegTask ->showRam[3] = 0x40;//显示负号 共阴极数码管，负号为 0x40；共阳极为0xBF
        PtrSegTask ->showRam[2] = 0x00;
        PtrSegTask ->showRam[1] = 0x00;
        PtrSegTask ->showRam[0] = 0x00;

        pointData = (pow(10,poitPosition) * (-(tempfloatdata) ));
        iterData = (float)pointData;
        //获取显示的内容，整数与小数
        for(i=0;i< 3;i++)
        {
            PtrSegTask ->showRam[i] = SEG7Table[iterData%10];
            iterData = iterData/10;
        }
        //添加小数点
        PtrSegTask ->showRam[poitPosition] |= 0x80;  //共阴极数码管|=0x80; 共阳极数码管 &=0x7F

    }

    //showhex强制显示，高优先级显示
    //左侧第一个数码管，若为负数，显示的数字依次向右移动一位
    if(PtrSegTask ->showhex[3]!= SEGHexBLK)
    {
        PtrSegTask ->showRam[0] = PtrSegTask ->showhex[0]!=SEGHexBLK?SEG7Table[PtrSegTask ->showhex[0]]:PtrSegTask ->showRam[1];
        PtrSegTask ->showRam[1] = PtrSegTask ->showhex[1]!=SEGHexBLK?SEG7Table[PtrSegTask ->showhex[1]]:PtrSegTask ->showRam[2];
        PtrSegTask ->showRam[2] = PtrSegTask ->showhex[2]!=SEGHexBLK?SEG7Table[PtrSegTask ->showhex[2]]:PtrSegTask ->showRam[3];
        PtrSegTask ->showRam[3] = SEG7Table[PtrSegTask ->showhex[3]];
    }
    else//showhex显示的优先级高于showRam
    {
        PtrSegTask ->showRam[0] = PtrSegTask ->showhex[0]!=SEGHexBLK?SEG7Table[PtrSegTask ->showhex[0]]:PtrSegTask ->showRam[0];
        PtrSegTask ->showRam[1] = PtrSegTask ->showhex[1]!=SEGHexBLK?SEG7Table[PtrSegTask ->showhex[1]]:PtrSegTask ->showRam[1];
        PtrSegTask ->showRam[2] = PtrSegTask ->showhex[2]!=SEGHexBLK?SEG7Table[PtrSegTask ->showhex[2]]:PtrSegTask ->showRam[2];
        PtrSegTask ->showRam[3] = PtrSegTask ->showhex[3]!=SEGHexBLK?SEG7Table[PtrSegTask ->showhex[3]]:PtrSegTask ->showRam[3];
    }

}

// 数码管扫描函数
// 定时器提供时基
void SegScanTask(Scan * PtrSegTask)
{
      static float temp =0 ;
      static char hexNum[4]={0};
      if(PtrSegTask ->scanTick > SCANTIMER)
      {
         PtrSegTask ->scanTick =0;
         //数码管显示处理--------------------------------------------
         //若显示内容不同，则刷新显示
        if((temp!=PtrSegTask ->showNum)
                || (hexNum[0]!=PtrSegTask ->showhex[0])|| (hexNum[1]!=PtrSegTask ->showhex[1])
                || (hexNum[2]!=PtrSegTask ->showhex[2])|| (hexNum[3]!=PtrSegTask ->showhex[3])
                )
        {
            temp = PtrSegTask ->showNum;
            hexNum[0]=PtrSegTask ->showhex[0];
            hexNum[1]=PtrSegTask ->showhex[1];
            hexNum[2]=PtrSegTask ->showhex[2];
            hexNum[3]=PtrSegTask ->showhex[3];

            segGetShow(PtrSegTask);
            AIP650_Send(DIG1,PtrSegTask ->showRam[0]);  //GID1
            AIP650_Send(DIG2,PtrSegTask ->showRam[1]);  //GID2
            AIP650_Send(DIG3,PtrSegTask ->showRam[2]);  //GID3
            AIP650_Send(DIG4,PtrSegTask ->showRam[3]);  //GID4
        }
        //扫描按键
        //KEY1 = 0
        //KEY2 = 1
        //KEY3 = 2
        //KEY4 = 3
        if(PtrSegTask ->nowScanKey >= KEY4)
        {
            PtrSegTask ->nowScanKey =KEY1;
        }
        else
        {
            PtrSegTask ->nowScanKey +=1 ;
        }

         keyVal=0xff;//先初始化一个无效的按键值，再做以下按键值的返回

         if(PtrSegTask ->nowScanKey == KEY1)
         {
            if(rK1_DAT()==0)
            {
               keyVal = 0x01;
            }
         }
         else if(PtrSegTask ->nowScanKey == KEY2)
         {
            if(rK2_DAT()==0)
            {
               keyVal = 0x02;
            }

         }
         else if(PtrSegTask ->nowScanKey == KEY3)
         {
            if(rK3_DAT()==0)
            {
               keyVal = 0x03;
            }
         }
         else
         {
            if(rK4_DAT()==0)
            {
               keyVal = 0x04;
            }
         }

         if(keyVal == keyValCode[PtrSegTask ->nowScanKey])//判断是否按下
         {
            PtrSegTask ->key[PtrSegTask ->nowScanKey].pressTick++;

            if(PtrSegTask ->key[PtrSegTask ->nowScanKey].pressTick >= PtrSegTask ->key[PtrSegTask ->nowScanKey].pressDEF)
            {
               PtrSegTask ->key[PtrSegTask ->nowScanKey].ClickCNT = 1;
            }
         }
         else
         {
             PtrSegTask ->key[PtrSegTask ->nowScanKey].pressTick = 0;
         }
      }

}
