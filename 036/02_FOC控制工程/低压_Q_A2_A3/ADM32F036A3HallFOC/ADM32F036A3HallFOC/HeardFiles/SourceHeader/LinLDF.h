/*
 * Lin_Communication.h
 *
 *  Created on: 2024年11月25日
 *      Author: Administrator
 */

#ifndef HEARDFILES_SOURCEHEADER_LIN_COMMUNICATION_H_
#define HEARDFILES_SOURCEHEADER_LIN_COMMUNICATION_H_

#include "Define.h"
#include "DSP2803x_Device.h"

/*Lin通信结构体定义*/
typedef struct Lin_Communication
{
    /*主机发送数据*/
    Uint32 EcwpSpdReq;             //电机速度设置
    Uint32 EcwpSafeMode;           //模式设置
    Uint32 EcwpEna;                //电机使能
    /*主机接收数据*/
    Uint32 EcwpActSpd;             //电机速度
    Uint32 EcwpLoVolt;             //板子电压
    Uint32 EcwpBdT;                //板子温度
    Uint32 EcwpActCur;             //电机电流
    Uint32 EcwpOverCurErr;         //过流故障
    Uint32 EcwpDryRunErr;          //干转故障
    Uint32 EcwpVoltErr;            //电压故障
    Uint32 EcwpBlkErr;             //堵转故障
    Uint32 EcwpTempErr;            //过温度故障
    Uint32 EcwpRespErr;            //通信故障
}Lin_COMMUNICATION;


struct LINTD0 {                // bit    description
    Uint16  TD3:8;             // 7:0    Transmit Buffer 3
    Uint16  TD2:8;             // 15:8   Transmit Buffer 2
    Uint16  TD1:8;             // 23:16  Transmit Buffer 1
    Uint16  TD0:8;             // 31:24  Transmit Buffer 0
};


typedef union LIN_data0 {
   Uint32         all;
   struct LINTD0  bit;
} LIN_DATA0;


struct LINTD1 {                // bit    description
    Uint16  TD7:8;             // 7:0    Transmit Buffer 7
    Uint16  TD6:8;             // 15:8   Transmit Buffer 6
    Uint16  TD5:8;             // 23:16  Transmit Buffer 5
    Uint16  TD4:8;             // 31:24  Transmit Buffer 4
};

typedef union LIN_data1 {
   Uint32          all;
   struct LINTD1   bit;
} LIN_DATA1;

extern Lin_COMMUNICATION Lin_Communication;

extern void UpdateRecivedLinDataToVars(void);
extern void UpdateLinDataToSent(void);
extern void LinData_init(void);
extern void UpdateLinDataToVar(void);

extern unsigned short GetSetupFreComs(void);
extern unsigned short GetStartCMDComs(void);

extern Uint16 ReciveDataIDCorrect[8];
extern Uint16 ReciveDataIDInCorrect[8];
extern LIN_DATA0 SenddDataA;
extern LIN_DATA1 SenddDataB;

#endif /* HEARDFILES_SOURCEHEADER_LIN_COMMUNICATION_H_ */
