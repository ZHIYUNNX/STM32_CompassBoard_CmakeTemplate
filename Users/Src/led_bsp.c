#include "led_bsp.h" 

void LED_Control(uint16_t Led_Pin, GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(GPIOB, Led_Pin, PinState);
}
