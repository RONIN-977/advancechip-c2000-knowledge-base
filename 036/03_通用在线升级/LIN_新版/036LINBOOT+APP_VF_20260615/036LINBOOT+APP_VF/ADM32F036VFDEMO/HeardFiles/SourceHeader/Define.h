
/************************************************************
-----------------Public definition header file---------------
************************************************************/
#ifndef MAIN_DEFINE_H
#define MAIN_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
	                    DSP CLOCK
************************************************************/
#define    DSP_CLOCK100		    100		//100MHz CLOCK

#define     DSP_CLOCK           100     //100MHz CLOCK
#define     PWM_CLK_DIV         0

#define     C_TIME_05MS     (DSP_CLOCK*500L)    //0.5ms corresponding timer 1 count value
#define     C_TIME_2MS      (DSP_CLOCK*2000L)   //0.5ms corresponding timer 1 count value


typedef	long long 				llong;
typedef	unsigned int			Uint;
typedef	unsigned long			Ulong;
typedef	unsigned long long 		Ullong;


/************************************************************
  Flash waiting time
************************************************************/
#define  READ_RAND_FLASH_WAITE		3		//The waiting time during the execution in Flash
#define  READ_PAGE_FLASH_WAITE		3		//
#define  READ_OTP_WAITE				5		//The waiting time for reading data in the OTP (One-Time Password) system

#define CURRENT_SAMPLE_1SHUNT           1
#define CURRENT_SAMPLE_2SHUNT           2
#define CURRENT_SAMPLE_3SHUNT           3
#define CURRENT_SAMPLE_MOSFET           4

#define ADM32F036_A2                    1
#define ADM32F036_A3                    2


/************************************************************
    Motor Operation States and Control Modes Definition
************************************************************/
#define STATUS_UNDER_VOL        1       // Under voltage state
#define STATUS_STOP             3       // Stop state
#define STATUS_GET_PAR          2       // Parameter identification phase
#define STATUS_SPEED_CHECK      4       // Speed tracking phase
#define STATUS_RUN              5       // Running state
#define STATUS_SHORT_GND        6       // Ground short circuit detection phase
#define STATUS_BRAKE            7       // Braking phase for reverse wind start
#define OPEN_ACC                8       // Open-loop acceleration
#define HALF_OPEN_ACC           9       // Half open-loop acceleration

enum {
    PMSM_CONTROL_MODE_VF,
    PMSM_CONTROL_MODE_FOC
};

enum {
    AVR_MODE_DEFAULT,   // 0: Traditional method, fixed gRatio = m_Ratio / UDC.DC
    AVR_MODE_DEFAULT_1, // 1: Method 1, fixed gRatio = m_Ratio / UDC.DCFilter
    AVR_MODE_DEFAULT_2, // 2: Method 2, AVR disabled, gRatio = m_Ratio
    AVR_MODE_DEFAULT_3, // 3: Method 3, Combines method 0 and 2, mode 0 at startup, switches to mode 2 when modulation ratio exceeds 3600
};

enum {
    U_V_CUR_SEN,
    U_W_CUR_SEN,
    V_W_CUR_SEN
};

#define  NULL           0
#define  TRUE           1
#define  FALSE          0

#define C_INIT_PRD          5000
#define C_MAX_DB            100     //Initial dead zone size: 1.0 us

/************************************************************
	Shared structure definition
************************************************************/
struct MAIN_COMMAND_STR_DEF{
   Uint    Start:1;
   Uint    SpeedSearch:1;
   Uint    StopDC:1;
   Uint    TuneFull:1;
   Uint    VcSvc:1;
   Uint    VfVc:1;
   Uint    StartDC:1;
   Uint    TunePart:1;
   Uint    SpeedRev:1;
   Uint    ErrorOK:1;
   Uint    SpeedStatus:2;
   Uint    ShortGnd:2;
   Uint    SecondDCPhase:1;
   Uint    LastDCPhase:1;
};

union MAIN_COMMAND_UNION_DEF {
   Uint   all;
   struct MAIN_COMMAND_STR_DEF  	bit;
};

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/
