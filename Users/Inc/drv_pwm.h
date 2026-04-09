#ifndef __DRV_PWM_H
#define __DRV_PWM_H

#ifdef __cplusplus
extern "C" {
#endif  

#include "tim.h"

typedef struct
{
    TIM_HandleTypeDef * htim;
    uint32_t Channel;
    
}Struct_PWM_Channel;

void PWM_Init(Struct_PWM_Channel * PWM_Channel);

void PWM_Set_Duty(Struct_PWM_Channel * PWM_Channel, float duty);

#ifdef __cplusplus
}
#endif  

#endif/*__DRV_PWM_H*/