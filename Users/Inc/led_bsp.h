#ifndef __LED_BSP_H__
#define __LED_BSP_H__
#include "main.h"

#define ON     GPIO_PIN_RESET
#define OFF    GPIO_PIN_SET


void LED_Control(uint16_t Led_Pin, GPIO_PinState PinState);

#endif /* __LED_BSP_H__ */
