#ifndef KB_CAN_BOARD_H
#define KB_CAN_BOARD_H
/* 两个工程各自保存一份；可运行交付根目录 Select-CAN-Pins.ps1 同时切换。 */
#define KB_CAN_PIN_PAIR 12U
#if KB_CAN_PIN_PAIR == 12U
#define KB_CAN_RX_GPIO 12U
#define KB_CAN_TX_GPIO 13U
#define KB_CAN_RX_CONFIG GPIO_12_CANA_RX
#define KB_CAN_TX_CONFIG GPIO_13_CANA_TX
#elif KB_CAN_PIN_PAIR == 30U
#define KB_CAN_RX_GPIO 30U
#define KB_CAN_TX_GPIO 31U
#define KB_CAN_RX_CONFIG GPIO_30_CANA_RX
#define KB_CAN_TX_CONFIG GPIO_31_CANA_TX
#else
#error KB_CAN_PIN_PAIR must be 12 or 30
#endif
#endif
