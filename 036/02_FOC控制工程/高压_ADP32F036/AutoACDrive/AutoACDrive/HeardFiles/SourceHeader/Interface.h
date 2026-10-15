
#ifndef __F_INTERFACE_H__
#define __F_INTERFACE_H__

#define  CORE_TO_FUNC_DATA_NUM	    12	        // 性能部分传递给功能部分的参数个数
#define  FUNC_TO_CORE_DATA_NUM	    26+22		// 功能部分传递给性能部分的参数个数

extern unsigned int gSendToMotorDataBuff[FUNC_TO_CORE_DATA_NUM];
extern int16 gSendToFunctionDataBuff[CORE_TO_FUNC_DATA_NUM];


void InitForFunctionApp(void);          // 功能码初始化
void Main0msFunction(void);             // 功能处理函数, 0ms
void Main05msFunction(void);            // 功能处理函数, 0.5ms
void Main2msFunction(void);             // 功能处理函数, 2ms


#endif // __F_INTERFACE_H__
