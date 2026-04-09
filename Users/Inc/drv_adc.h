#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include "adc.h"

#define SAMPLE_BUFFER_SIZE 4

typedef struct
{
    ADC_HandleTypeDef *ADC_Handler;
    uint16_t ADC_Data[SAMPLE_BUFFER_SIZE];
    uint16_t Sample_Number;
}Struct_ADC_Manage_Object;

extern Struct_ADC_Manage_Object ADC1_Manage_Object;
extern Struct_ADC_Manage_Object ADC2_Manage_Object;
extern Struct_ADC_Manage_Object ADC3_Manage_Object;

void ADC_Init(ADC_HandleTypeDef *hadc, uint16_t __Sample_Number);

#endif /*__DRV_ADC_H*/
