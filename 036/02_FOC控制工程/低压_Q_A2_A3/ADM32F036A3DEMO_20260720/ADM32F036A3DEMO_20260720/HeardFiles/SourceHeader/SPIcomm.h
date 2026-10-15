#ifndef SPICOMM_H
#define SPICOMM_H

#include "ADP32F03x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif

void InitSpi(void);
void InitSpiIO(void);
void SPISendData(void);

#ifdef __cplusplus
}
#endif

#endif /* SPICOMM_H */
