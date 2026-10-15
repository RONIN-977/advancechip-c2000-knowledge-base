/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :dsp_base.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef DSP_BASE_H_
#define DSP_BASE_H_
/******************************************************************************
Includes
******************************************************************************/
#include "AVP33x_Project.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void DSP_Base_Init(void);
void DSP_INT_Enable(void);
void SysCtrl_Initialize(void);
void Flash_Initialize(void);
void PieCtrl_Initialize(void);
void PieVectTable_Initialize(void);
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
void InitPll_Initialize(Uint16 val, Uint16 divsel);
void InitFlashAPI(void);
void WDogService(void);
void WDogDisable(void);
void EnableDog(void);
void PeripheralClocks_Initialize(void);

















#endif
