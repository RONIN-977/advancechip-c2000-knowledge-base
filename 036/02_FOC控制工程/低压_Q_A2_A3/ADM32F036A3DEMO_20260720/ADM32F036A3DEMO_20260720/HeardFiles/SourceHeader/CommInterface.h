#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H

#include "ADP32F03x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif

void CommsProtocalLayerTask2ms(void);

interrupt void LINA_LEVEL_isr(void);

#ifndef LIN_CONTROL_MODE
interrupt void SCI_TXD_isr(void);
interrupt void SCI_RXD_isr(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* COMM_INTERFACE_H */
