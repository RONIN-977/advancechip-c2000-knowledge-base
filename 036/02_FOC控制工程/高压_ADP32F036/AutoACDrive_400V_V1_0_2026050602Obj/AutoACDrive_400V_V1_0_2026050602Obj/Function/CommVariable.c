#include "Define.h"
#include "Main.h"
#include "MotorInclude.h"
#include "param_lr.h"
#include "Interface.h"
#include "CANcomm.h"
#include "Parameter.h"

unsigned int RealTimeDataSendFlag = 0;

unsigned int reserve;
const unsigned int  *VarAddr[28] =
{
    &RunSignal,                          //0  运行命令
    (unsigned int *)&RefSet,             //1  使能系数状态数据上传
    &reserve,                            //2  保留
    &pm_control_mode,                    //3  控制方式(VF/矢量)
    &LOW_SPEED_KP,                       //4  低速段速度环Kp
    &LOW_SPEED_KI,                       //5  低速段速度环Ki
    &HIGH_SPEED_KP_GAIN,                 //6  高速段速度环Kp
    (unsigned int *)&DCStart_flag,       //7  启动方式
    &HIGH_SPEED_KI_GAIN,                 //8  高速段速度环Ki
    &D_CUR_KP,                           //9  d轴电流环Kp
    &D_CUR_KI,                           //10 d轴电流环Ki
    &FILED_WEEKING_MODE,                 //11 弱磁模式
    &Q_CUR_KP,                           //12 q轴电流环Kp
    &Q_CUR_KI,                           //13 q轴电流环Ki
    &FIELD_WEAK_REF,                     //14 弱磁点
    &reserve,                            //15 控制模式(速度模式/转矩模式)
    &F_CHANGING_POINT_1_PI,              //16 频率切换点1
    &F_CHANGING_POINT_2_PI,              //17 频率切换点2
    &SOFT_OC_POINT,                      //18 软件过流点
    &reserve,                            //19 目标转矩
    &OVER_DC_VOL_POINT,                  //20 过压点
    &UNDER_DC_VOL_POINT,                 //21 欠压点
    &MOTOR_RATED_VOL,                    //22 额定电压
    &TORQUE_CUR_LIMIT,                   //23 限流点
    &reserve,                            //24 电流采样系数
    &reserve,                            //25 电压采样系数
    &MOTOR_RATED_CUR,                     //26 额定电流
    &RealTimeDataSendFlag                //27 实时数据监控使能
};

