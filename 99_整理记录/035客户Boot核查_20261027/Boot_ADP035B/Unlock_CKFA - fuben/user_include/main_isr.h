
#ifndef USER_INCLUDE_MAIN_ISR_H_
#define USER_INCLUDE_MAIN_ISR_H_

// 时间相关的宏定义置于此
#define  PRD_5MS           100             //50us*100 = 5ms
#define  PRD_10MS          200             //50us*200 = 10ms
#define  PRD_50MS          1000            //50us*1000 = 50ms
#define  PRD_125MS         2500            //50us*2500 = 125ms
#define  PRD_250MS         5000            //50us*5000 = 250ms
#define  PRD_500MS         10000           //50us*10000 = 500ms
#define  PRD_1S            20000           //50us*20000 = 1s
#define  PRD_1S5           30000           //50us*30000 = 1.5s
#define  PRD_10S           200000          //50us*200000 = 10s
#define  RECV_ERR_CLR      300             // 接收故障清零计数 10ms*300 = 3s
#define  ERR_FLAG_CLR      20000           // 定时器中判断故障清零 50us*20000 = 1s
#define  ERR_SHADOW_CLR    14400           // 50ms*20 = 1s,50ms*14400 = 720sec = 12min
#define  T0_ERR_CLR        60000           // 定时器中判断故障清零 50us*60000 = 3s
#define  NO_INVERT_CNT     1200000         // 时间长未并网

extern int f_KickDogM,f_KickDogI;                    //喂狗变量
extern int looptimes;                                //定时器中断循环变量
extern int ckfa_test;
extern int ileak_rms_cnt,ileak_ave_cnt;
extern int Ileak_rms,Ileak_ave,Ileak_ave_z1;
extern long Ileak_rms_temp;
extern long Ileak_rms_sum;
extern long Ileak_ave_sum;
extern interrupt void cpu_timer0_isr(void);

#endif /* USER_INCLUDE_MAIN_ISR_H_ */
