/****************************************************************
文件功能：主程序中调用的大的模块函数
文件版本：
最新更新：

****************************************************************/
#include "MainInclude.h"
#include "SubPrgInclude.h"

/****************************************************************
函数说明：反正切函数，该函数输入x，y，求得的反正切角度以及是4象限的角度
****************************************************************/
int atan(int x, int y)
{
	int  result;
	long m_Input;

	if(x == 0)
	{
		if(y < 0)
		{
			return(-16384);
		}
		else
		{
			return(16384);
		}
	}
	m_Input = (((long)y)<<16)/x;
	result = qatan(m_Input);
	if(x < 0)
	{
		result += 32768;
	}
	return result;
}

/****************************************************************
函数说明：PID函数(暂时不考虑D增益的作用)
****************************************************************/
void PID(PID_STRUCT * pid)
{
	long m_Max,m_Min,m_Long;

	m_Max = ((long)pid->Max)<<16;		//最大值
	m_Min = ((long)pid->Min)<<16;		//最小值

	m_Long = pid->Total + (((long)pid->KI * (long)pid->Deta));//积分
	pid->Total = __IQsat(m_Long,m_Max,m_Min);

	m_Long = pid->Total + (((long)pid->KP * (long)pid->Deta));//比例
	pid->Out = __IQsat(m_Long,m_Max,m_Min);
}


/****************************************************************
函数说明：PID函数(暂时不考虑D增益的作用)
输入偏差为int型变量
输出结果为long型变量。右移16位得到需要的结果
比例增益在pid内部倍左移 4位
****************************************************************/
void PID_MD(PID_STRUCT_MD * pid)
{
	long m_Max,m_Min,m_Out,m_OutKp,m_OutKi;
    long vMax = 0x7FFFFFFF;

    SETOVM;
 	m_Max = ((long)pid->Max)<<16;						// 最大值
	m_Min = ((long)pid->Min)<<16;						// 最小值

	m_OutKp = (long)pid->KP * (long)pid->Deta;			// 比例
	m_OutKp = __IQsat(m_OutKp, (vMax>> (4+ pid->QP)), -(vMax>> (4+ pid->QP))); //保证使用算术右移
    m_OutKp = m_OutKp << (4+ pid->QP);

 	m_OutKi = (long)pid->KI * (long)pid->Deta;// 积分
    m_OutKi = __IQsat(m_OutKi, (vMax >> pid->QI), -(vMax >> pid->QI));
    m_OutKi = m_OutKi << pid->QI;

    // 饱和情况下的去饱和处理
    if((m_OutKp > m_Max) && (pid->Total > 0))
    {
        pid->Total -= (pid->Total>>8) + 1;  //加1去掉滤波静差
        m_OutKi = 0;
    }
    else if((m_OutKp < m_Min) && (pid->Total < 0))
    {
        pid->Total -= (pid->Total>>8) - 1;
        m_OutKi = 0;
    }
	pid->Total += m_OutKi;
	m_Out       = pid->Total + m_OutKp;
 	pid->Out    = __IQsat(m_Out,m_Max,m_Min);
    pid->Total  = __IQsat(pid->Total,m_Max,m_Min);

    CLROVM;
}

// 优化版本，减少嵌套层次
void UpdateTimeAccumulator(TIME_STRUCT* time_acc)
{
    // 2ms累计
    time_acc->_ms_counter += 2;

    // 10ms处理
    if (time_acc->_ms_counter < 10) {
        return;  // 未达到10ms，直接返回
    }

    time_acc->_ms_counter -= 10;
    time_acc->count_10ms++;

    // 1秒处理
    time_acc->_s_counter++;
    if (time_acc->_s_counter < 100) {
        return;
    }

    time_acc->_s_counter = 0;
    time_acc->count_1s++;

    // 1分钟处理
    time_acc->_min_counter++;
    if (time_acc->_min_counter < 60) {
        return;
    }

    time_acc->_min_counter = 0;
    time_acc->count_1min++;

    // 1小时处理
    if (time_acc->count_1min >= 60)
    {
        time_acc->count_10ms = 0;
        time_acc->count_1min = 0;

        // 防止溢出
        if (time_acc->count_1hour < 0xFFFFFFFF) {
            time_acc->count_1hour++;
        }
    }
}

// 重置时间累计器
void ResetTimeAccumulator(TIME_STRUCT* time_acc)
{
    time_acc->count_10ms = 0;
    time_acc->count_1s = 0;
    time_acc->count_1min = 0;
    time_acc->count_1hour = 0;
    time_acc->_ms_counter = 0;
    time_acc->_s_counter = 0;
    time_acc->_min_counter = 0;
}

// 精简的线性同余伪随机数生成器
static unsigned long lcg_seed = 0;

// 初始化随机种子
void init_random_freq(void) {
    lcg_seed = GetTime();  // 使用定时器计数值初始化种子
}

// 生成伪随机数（线性同余法）
static unsigned long lcg_rand(void) {
    lcg_seed = lcg_seed * 1103515245 + 12345;
    return (unsigned long)(lcg_seed >> 16) & 0x7FFF;  // 返回15位随机数
}

// 生成指定范围内的随机频率
int random_frequency(int center, int amplitude) {
    // 生成随机值并映射到[0, 2*amplitude]范围内
    unsigned long random_val = lcg_rand() % (2 * amplitude + 1);

    // 计算最终频率值
    return (int)(center - amplitude + random_val);
}
