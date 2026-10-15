#ifndef SEGSCAN_H
#define SEGSCAN_H

#include "AVP32F069_Project.h"

#define SEGHexBLK       16      //高优先级显示
#define SCANTIMER 5             //定义扫描的频率，单位ms
#define KeyNUM 4

typedef enum
{
  KEY1 = 0,
  KEY2 = 1,
  KEY3 = 2,
  KEY4 = 3
} keySlt;

typedef struct KeyScan{
  Uint16  pressDEF;  //  需要按下识别的次数
  Uint16  pressTick; //  按下次数时基计数
  Uint16  ClickCNT;  //  已经按下
}KeyScan;

typedef struct Scan{
  Uint16         scanTick;    //扫描时间
  unsigned char  showRam[4];  //数码管显示的内容,最低层显示的内容，  [0] -->SEG4
  float          showNum;     //数码管显示的数字, 可以是浮点数也可以是整数，这里的最大范围 0.999 到 9999
  char           showhex[4];  // 显示优先级高于showNum;范围 0 --- 0x0F
  keySlt         nowScanKey;  //当前发送的按键
  KeyScan        key[KeyNUM];     //KEY1,KEY2,KEY3,KEY4
}Scan; 

extern char *keyStr[];
extern Scan Running;
extern char poitPosition;//存放小数点的位置
extern float ftestData;

void segGetShow(Scan * PtrSegTask);
void SegScanTask(Scan * PtrSegTask);

#endif //SEGSCAN_H
