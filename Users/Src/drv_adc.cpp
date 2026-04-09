#include "drv_adc.h"

Struct_ADC_Manage_Object ADC1_Manage_Object = {0};
Struct_ADC_Manage_Object ADC2_Manage_Object = {0};
Struct_ADC_Manage_Object ADC3_Manage_Object = {0};

void ADC_Init(ADC_HandleTypeDef *hadc, uint16_t __Sample_Number)
{
    if (hadc->Instance == ADC1)
    {
        ADC1_Manage_Object.ADC_Handler = hadc;
        ADC1_Manage_Object.Sample_Number = __Sample_Number;
        HAL_ADCEx_Calibration_Start(hadc);
        HAL_ADC_Start_DMA(hadc, (uint32_t *) &ADC1_Manage_Object.ADC_Data, __Sample_Number);
    }
    else if (hadc->Instance == ADC2)
    {
        ADC2_Manage_Object.ADC_Handler = hadc;
        ADC2_Manage_Object.Sample_Number = __Sample_Number;
        HAL_ADCEx_Calibration_Start(hadc);
        HAL_ADC_Start_DMA(hadc, (uint32_t *) &ADC2_Manage_Object.ADC_Data, __Sample_Number);
    }
    else if (hadc->Instance == ADC3)
    {
        ADC3_Manage_Object.ADC_Handler = hadc;
        ADC3_Manage_Object.Sample_Number = __Sample_Number;
        HAL_ADCEx_Calibration_Start(hadc);
        HAL_ADC_Start_DMA(hadc, (uint32_t *) &ADC3_Manage_Object.ADC_Data, __Sample_Number);
    }
}