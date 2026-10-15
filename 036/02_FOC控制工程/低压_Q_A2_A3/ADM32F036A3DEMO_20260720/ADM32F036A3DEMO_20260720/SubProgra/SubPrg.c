/****************************************************************
文件功能：主程序中调用的大的模块函数
文件版本：
最新更新：
	
****************************************************************/
#include "Main.h"
#include "SubPrgInclude.h"

/***************************************************************************
  Private Functions
 ***************************************************************************/
static Uint32 lcg_rand(void);


/****************************************************************
函数说明：反正切函数，该函数输入x，y，求得的反正切角度以及是4象限的角度
****************************************************************/
int16 atan(int16 x, int16 y)
{
	int16  result;
	int32 m_Input;

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
	m_Input = (((int32)y)<<16)/x;
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
	int32 m_Max,m_Min,m_Long;

	m_Max = ((int32)pid->Max)<<16;		//最大值
	m_Min = ((int32)pid->Min)<<16;		//最小值

	m_Long = pid->Total + (((int32)pid->KI * (int32)pid->Deta));//积分
	pid->Total = __IQsat(m_Long,m_Max,m_Min);

	m_Long = pid->Total + (((int32)pid->KP * (int32)pid->Deta));//比例
	pid->Out = __IQsat(m_Long,m_Max,m_Min);
}

void sort_min_mid(Uint16 a, Uint16 b, Uint16 c, Uint16 *min, Uint16 *mid) {
    // 三次比较完成排序，找到最小值和中间值
    if (a > b) { Uint16 t = a; a = b; b = t; }  // 确保 a <= b
    if (b > c) { Uint16 t = b; b = c; c = t; }  // 确保 b <= c
    if (a > b) { Uint16 t = a; a = b; b = t; }  // 再次确保 a <= b

    *min = a;  // 最小值
    *mid = b;  // 中间值
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
    if (time_acc->count_1s < 10000) {
        time_acc->count_1s++;
    }

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
static Uint32 lcg_seed = 0;

// 初始化随机种子
void init_random_freq(void) {
    lcg_seed = GetTime();  // 使用定时器计数值初始化种子
}

// 生成伪随机数（线性同余法）
static Uint32 lcg_rand(void) {
    lcg_seed = lcg_seed * 1103515245 + 12345;
    return (Uint32)(lcg_seed >> 16) & 0x7FFF;  // 返回15位随机数
}

// 生成指定范围内的随机频率
int16 random_frequency(int16 center, int16 amplitude) {
    // 生成随机值并映射到[0, 2*amplitude]范围内
    Uint32 random_val = lcg_rand() % (2 * amplitude + 1);

    // 计算最终频率值
    return (int16)(center - amplitude + random_val);
}
