
#ifndef __F_PARAMETER_H__
#define __F_PARAMETER_H__

extern unsigned int MAX_DC_VOL_3_3V;


extern unsigned int OverCurPointInternal;   // Hardware overcurrent comparison value
extern unsigned int CBCCurPointInternal;    // CBC overcurrent comparison value

extern unsigned int DeadTimeInternal;


///////////////////////////////////////////////////////////////////////////////////////
// Motor parameter settings
///////////////////////////////////////////////////////////////////////////////////////

extern unsigned int MOTOR_RATED_CUR;


///////////////////////////////////////////////////////////////////////////////////////
// Control parameter settings
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned int PWM_FREQUENCY;     // 100 * 0.1kHz PWM frequency


//--------------------- Frequency range parameter----------------------/
extern unsigned int MAX_LIMIT_FREQ;   // 300.00Hz
extern unsigned int MOTOR_RATED_FRE;


extern int RefSet;
extern unsigned int ACC1;
extern unsigned int ACC2;

extern unsigned int AUTO_RESET_CNT;
extern unsigned int ThetaCoff;
extern unsigned int AVR_Mode;


#define CURRENT_SAMPLE_TYPE             CURRENT_SAMPLE_1SHUNT
#define ADM32F036_TYPE                  ADM32F036_A3

#endif // __F_PARAMETER_H__

