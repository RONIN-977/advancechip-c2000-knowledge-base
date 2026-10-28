
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

#ifndef USER_INCLUDE_SCI_LIN_H_
#define USER_INCLUDE_SCI_LIN_H_

#define  ScilTx_Ready()   (SciaRegs.SCICTL2.bit.TXEMPTY == 1)
#define  ScilRx_Ready()   (SciaRegs.SCIRXST.bit.RXRDY == 1)
#define  ScilRead()       SciaRegs.SCIRXBUF.bit.RXDT
#define  ScilWrite(ch)   SciaRegs.SCITXBUF = (ch)
#define  ScilFrameErr()   (SciaRegs.SCIRXST.bit.FE == 1)
#define  ScilBreakErr()   (SciaRegs.SCIRXST.bit.BRKDT == 1)



#define  RBUF_SIZE_L      400
#define  SBUF_SIZE_L      400
#define  SWORD_NUM_L      ((int)(SBUF_SIZE_L/2-30))

#define  IDLE_L           0
#define  RX_L             1
#define  RX_7C_L          2
#define  RX_OVER_L        3
#define  PROCESS_L        4
#define  TX_L             5

extern int scil_rbyte,scil_sbyte;
extern int scil_deal_timer;
extern int scil_status;
extern int scil_rtimer;
extern long scil_wtimer;
extern int scil_rcnt;
extern int scil_rlen;
extern int scil_slen;
extern int scil_scnt;
extern int ScilRecvBuf[RBUF_SIZE_L+1];
extern int ScilSendBuf[SBUF_SIZE_L+1];

extern void ScilOverTimeDeal(void);
extern void ScilByteDeal(void);
extern void ScilSendStart(void);
extern void ScilComDelay(void);

#endif /* USER_INCLUDE_SCI_LIN_H_ */
