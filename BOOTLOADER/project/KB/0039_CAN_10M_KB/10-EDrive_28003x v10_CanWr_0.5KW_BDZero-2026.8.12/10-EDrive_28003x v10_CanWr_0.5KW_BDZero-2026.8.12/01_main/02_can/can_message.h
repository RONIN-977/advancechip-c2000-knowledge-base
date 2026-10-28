/*
 * can_message.h
 *
 *  Created on: 2022Äê3ÔÂ6ÈÕ
 *      Author: 86176
 */

#ifndef USER_DEFINE_CAN_MESSAGE_H_
#define USER_DEFINE_CAN_MESSAGE_H_
#include <DataTypeDef.h>

/*
  The CAN_DATATYPE structure has been structured so that it is tightly packed.
  Size of structure (without timestamp field) :16 bytes
  Size of structure (with timestamp field)    :24 bytes
*/
typedef struct
{

    /* Is Extended frame */
    u8 Extended;

    /* Length */
    u8 Length;

    /* RTR */
    u8 Remote;

    /* Error */
    u8 Error;

    /* CAN ID */
    u32 ID;

    /* Data field */
    u8 Data[8];

}  CAN_MESSAGE;

typedef CAN_MESSAGE CAN_DATATYPE;

#endif /* 00_SYS280025_USER_DEFINE_CAN_MESSAGE_H_ */
