/*
 * ECanFunc.h
 *
 */

#ifndef APL_INC_ECANFUNC_H_
#define APL_INC_ECANFUNC_H_

extern void canASendData(Uint32 data1,Uint32 data2,Uint16 mbnum);
extern void sCANFDDataTx(Uint16 *txbuf,Uint16 len);
extern Uint16 sCANFDTxStateGet(void);
extern void canfdSendDataP(Uint32 data1, Uint32 data2);
extern Uint16 sCANFDDataRx(Uint16 *buf);
extern void canfdSendDataS(Uint32 data1, Uint32 data2);
extern void canfdSendData_2(Uint16 data1);


#endif /* APL_INC_ECANFUNC_H_ */
