#include "TestPlatform.h"
#define interrupt
#define PIEACK_GROUP9 0x100
typedef union {
    Uint16 all;
    struct { Uint16 RXENA:1, TXENA:1, reserved:3, SWRESET:1; } bit;
} TEST_SCICTL1;
typedef union {
    Uint16 all;
    struct { Uint16 TXINTENA:1, RXBKINTENA:1, reserved:4, TXEMPTY:1, TXRDY:1; } bit;
} TEST_SCICTL2;
typedef union {
    Uint16 all;
    struct { Uint16 reserved:7, RXERROR:1; } bit;
} TEST_SCIRXST;
typedef union {
    Uint16 all;
    struct { Uint16 reserved:6, TXFFINTCLR:1; } bit;
} TEST_SCIFFTX;
typedef union {
    Uint16 all;
    struct { Uint16 reserved:6, RXFFINTCLR:1, reserved2:7, RXFFOVRCLR:1; } bit;
} TEST_SCIFFRX;
typedef struct {
    union { Uint16 all; } SCIRXBUF;
    TEST_SCIRXST SCIRXST;
    TEST_SCICTL1 SCICTL1;
    TEST_SCICTL2 SCICTL2;
    Uint16 SCITXBUF;
    TEST_SCIFFTX SCIFFTX;
    TEST_SCIFFRX SCIFFRX;
} TEST_SCI;
volatile TEST_SCI SciaRegs;
struct { union { Uint16 all; } PIEACK; } PieCtrlRegs;
#define EALLOW
#define EDIS
struct { Uint16 WDCR; } SysCtrlRegs;
Uint32 TestJumpAddress;
static void jump(Uint32 address) { TestJumpAddress=address; }
void UpgradePendingAction(void);
