#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#define ENTRYPOINT 0x003f7ff6


Uint16 WatchDogInit_HWRST(void);
Uint16 WatchDogInit_SWIsrRST(void);

#endif //WATCHDOG_H_
