#ifndef SCI_UPGRADE_H
#define SCI_UPGRADE_H
typedef struct {
    volatile Uint16 RxBufferIndex, TxBufferIndex, RecvState;
    Uint16 ReceiveBuf[160], SendBuf[48], TransmitLength;
    Uint16 nAppJumpFlag, apiReady, pendingAction, sessionActive;
    Uint32 firmwareBytes, receivedBytes, nextPacket;
} TRemoteModbus;
extern TRemoteModbus RemoteModbus;
void Sci_Initialize(void);
void SCI_Boot(void);
void RemoteModbus_Initialize(void);
void RemoteModbus_MsgProcess(void);
void UpgradePendingAction(void);
void UpgradeJumpApp(void);
void UpgradeWatchdog(void);
#endif
