/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   sdfm_ex2_filter_sync_claread_shared.h
*                         TITLE:  SDFM filter sync CLA
*******************************************************************************************/
#ifndef _CLA_SDFM_FILTER_SYNCH_SHARED_H_
#define _CLA_SDFM_FILTER_SYNCH_SHARED_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// Defines
//
#define MAX_SAMPLES          1024

//
// Globals
//
extern int16_t  filter1Result[MAX_SAMPLES];
extern int16_t  filter2Result[MAX_SAMPLES];
extern int16_t  filter3Result[MAX_SAMPLES];
extern int16_t  filter4Result[MAX_SAMPLES];

//
// The following are symbols defined in the CLA code
// Including them in the shared header file makes them
// .global and the main CPU can make use of them.
//
//

//
// CLA C Tasks
//
__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();

__interrupt void cla1Isr1();
__interrupt void cla1Isr2();
__interrupt void cla1Isr3();
__interrupt void cla1Isr4();
__interrupt void cla1Isr5();
__interrupt void cla1Isr6();
__interrupt void cla1Isr7();
__interrupt void cla1Isr8();

//
// Linker command variables
//
extern uint32_t Cla1funcsLoadEnd;
extern uint32_t Cla1ProgRunStart;
extern uint32_t Cla1ProgLoadStart;
extern uint32_t Cla1ProgLoadSize;
extern uint32_t Cla1ConstLoadStart;
extern uint32_t Cla1ConstLoadEnd;
extern uint32_t Cla1ConstRunStart;
extern uint32_t Cla1ConstLoadSize;

#ifdef __cplusplus
}
#endif // extern "C"

#endif //end of _CLA_SDFM_FILTER_SYNCH_SHARED_H_ definition

//
// End of file
//
