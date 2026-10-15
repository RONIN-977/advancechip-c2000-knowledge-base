/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  fsi_ex2_loopback_clacontrol_cla_shared.h
*                         TITLE: CLA code to control FSI data frame transfers
*******************************************************************************************/

#ifndef _FSI_EX2_LOOPBACK_CLACONTROL_CLA_SHARED_H_
#define _FSI_EX2_LOOPBACK_CLACONTROL_CLA_SHARED_H_

#ifdef __cplusplus
extern "C" {
#endif
//
// Included Files
//

//
// Defines
//

//
// Globals, User can modify these parameters as per usecase
//

//
// Globals, these are not config parameters, user are not required to edit them
//

extern volatile uint32_t fsiTxInt1Received;
extern volatile uint32_t fsiRxInt1Received;
extern uint32_t txTimeOutCntr, rxTimeOutCntr;

//
// Function Prototypes
//

//
// Globals
//
//Task 1 (C) Variables

//Task 2 (C) Variables

//Task 3 (C) Variables

//Task 4 (C) Variables

//Task 5 (C) Variables

//Task 6 (C) Variables

//Task 7 (C) Variables

//Task 8 (C) Variables

//Common (C) Variables

//
// Function Prototypes
//
//CLA C Tasks defined in fsi_ex2_loopback_clacontrol_cla.cla
__attribute__((interrupt))  void Cla1Task1();
__attribute__((interrupt))  void Cla1Task2();
__attribute__((interrupt))  void Cla1Task3();
__attribute__((interrupt))  void Cla1Task4();
__attribute__((interrupt))  void Cla1Task5();
__attribute__((interrupt))  void Cla1Task6();
__attribute__((interrupt))  void Cla1Task7();
__attribute__((interrupt))  void Cla1Task8();
#if defined(__TMS320C28XX__)
__attribute__((interrupt))  void Cla1Task8();
#else
__attribute__((interrupt("background")))  void Cla1Task8();
#endif

#ifdef __cplusplus
}
#endif // extern "C"
#endif //_FSI_EX2_LOOPBACK_CLACONTROL_CLA_SHARED_H_

//
// End of File
//
