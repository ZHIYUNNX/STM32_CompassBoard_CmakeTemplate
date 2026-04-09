#include "drv_pwm.h"




void PWM_Init(Struct_PWM_Channel * PWM_Channel)
{
    HAL_TIM_PWM_Start(PWM_Channel->htim,PWM_Channel->Channel);
}

void PWM_Set_Duty(Struct_PWM_Channel * PWM_Channel, float duty)
{
    uint16_t compare = 0;
    compare = (uint16_t)(duty * (PWM_Channel->htim->Init.Period + 1));
    __HAL_TIM_SET_COMPARE(PWM_Channel->htim,PWM_Channel->Channel, compare);
}
       