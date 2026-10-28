/*
 * canConfig.c
 *
 *  Created on: 2022Äê2ÔÂ21ÈÕ
 *      Author: 86176
 */
#include "canConfig.h"
CANSETTING_STRUCT CAN_Setting;
//void CANSetting_Init(void)
//{
//
//}
void CAN_Config_Init(void)
{

    MCU_CAN_Transmit_Task_Init();
    MCU_CAN_Receive_Task_Init();
}
/* System initialize for atomic system: '<Root>/CAN_Transmit_Task' */
void MCU_CAN_Transmit_Task_Init(void)
{

#if dbg_mode
  CAN_setupMessageObject(CANA_BASE, 1, Debugcan_MCU200, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);

  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit1' incorporates:
   *  Constant: '<S4>/Constant2'
   */
  CAN_setupMessageObject(CANA_BASE, 2, Debugcan_MCU23F, CAN_MSG_FRAME_STD,//201swj   0x23F
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);

  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, 3, Debugcan_MCU202, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);

  CAN_setupMessageObject(CANA_BASE, 4, Debugcan_MCU203, CAN_MSG_FRAME_STD,
      CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
  CAN_setupMessageObject(CANA_BASE, MCU_729_msg+1, Usercan_PhyRes729, CAN_MSG_FRAME_STD,
      CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
//  CAN_setupMessageObject(CANA_BASE, 5, Debugcan_MCU204, CAN_MSG_FRAME_STD,
//      CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
#else
  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit' incorporates:
   *  Constant: '<S4>/Constant1'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_130_msg+1, Usercan_MCU130, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);

  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit1' incorporates:
   *  Constant: '<S4>/Constant2'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_132_msg+1, Usercan_MCU132, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);

  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_181_msg+1, Usercan_MCU181, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_631_msg+1, Usercan_MCU631, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_111_msg+1, Usercan_MCU111, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_729_msg+1, Usercan_PhyRes729, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_112_msg+1, Usercan_MCU112, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_101_msg+1, Usercan_MCU101, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
  /* Start for S-Function (c280xcanxmt): '<S4>/CAN Transmit2' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  CAN_setupMessageObject(CANA_BASE, MCU_118_msg+1, Usercan_MCU118, CAN_MSG_FRAME_STD,
    CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
#endif
}
/* Output and update for atomic system: '<Root>/CAN_Receive_Task' */
void MCU_CAN_Receive_Task_Init(void)
{
    /* Start for S-Function (c280xcanrcv): '<S4>/CAN Receive2' */
      uint32_t ui32Flags;
      ui32Flags = CAN_MSG_OBJ_NO_FLAGS;
      CAN_setupMessageObject(CANA_BASE, DEBUG_102_msg+CanBoxOffset, Debugcan_VCU102, CAN_MSG_FRAME_STD,//100swj   0x102
                             CAN_MSG_OBJ_TYPE_RX, 0, ui32Flags, sizeof(unsigned char) * 8);
    /* Start for S-Function (c280xcanrcv): '<S4>/CAN Receive3' */
      CAN_setupMessageObject(CANA_BASE, VCU_118_msg+CanBoxOffset, Usercan_VCU118, CAN_MSG_FRAME_STD,
                             CAN_MSG_OBJ_TYPE_RX, 0, ui32Flags, sizeof(unsigned char) * 8);
    /* Start for S-Function (c280xcanrcv): '<S4>/CAN Receive4' */
      CAN_setupMessageObject(CANA_BASE, VCU_119_msg+CanBoxOffset, Usercan_VCU119, CAN_MSG_FRAME_STD,
                             CAN_MSG_OBJ_TYPE_RX, 0, ui32Flags, sizeof(unsigned char) * 8);
      /* Start for S-Function (c280xcanrcv): '<S4>/CAN Receive4' */
      CAN_setupMessageObject(CANA_BASE, VCU_126_msg+CanBoxOffset, Usercan_VCU126, CAN_MSG_FRAME_STD,
                             CAN_MSG_OBJ_TYPE_RX, 0, ui32Flags, sizeof(unsigned char) * 8);
      /* Start for S-Function (c280xcanrcv): '<S4>/CAN Receive4' */
      CAN_setupMessageObject(CANA_BASE, VCU_620_msg+CanBoxOffset, Usercan_VCU620, CAN_MSG_FRAME_STD,
                             CAN_MSG_OBJ_TYPE_RX, 0, ui32Flags, sizeof(unsigned char) * 8);

      CAN_setupMessageObject(CANA_BASE, UDS_721_msg+CanBoxOffset, Usercan_UDS721, CAN_MSG_FRAME_STD,
                             CAN_MSG_OBJ_TYPE_RX, 0, ui32Flags, sizeof(unsigned char) * 8);

      CAN_setupMessageObject(CANA_BASE, UDS_7df_msg+CanBoxOffset, Usercan_UDS7df, CAN_MSG_FRAME_STD,
                             CAN_MSG_OBJ_TYPE_RX, 0, ui32Flags, sizeof(unsigned char) * 8);
}

