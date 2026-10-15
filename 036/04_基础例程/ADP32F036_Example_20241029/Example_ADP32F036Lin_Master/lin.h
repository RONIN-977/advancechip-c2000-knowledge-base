#ifndef _LIN_H
#define _LIN_H

#define FramHead1          0x55   //帧头1
#define FramHead2          0xAA   //帧头2z
//#define Cmd                0X00   //命令字
//#define CmdAttrVal         0X00   //命令属性值
//#define Len                0X00   //数据长度
//#define Direction          0x00   //转向
//#define GradeVal           0X00   //档位信息
//#define CheckSum           0X00   //校验和

typedef struct _LIN_SCI_DATASTR
{
//    Uint16  FramHead1;//帧头1
//    Uint16  FramHead2;//帧头2
    Uint16  Cmd;     //命令字
    Uint16  CmdAttrVal;//命令属性值
    Uint16  Len;      //数据长度
    Uint16  Direction;//转向
    Uint16  GradeVal;//档位信息
    Uint16  CheckSum; //校验和
    Uint16  TxFlag;
    Uint16  RxFlag;

    Uint16  speed;
}LIN_SCI_DATASTR,*pLIN_SCI_DATASTR;


#endif
