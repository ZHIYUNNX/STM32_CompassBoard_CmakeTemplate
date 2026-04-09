#include "dvc_sampler.h"

void Class_Sampler::Init(Struct_ADC_Manage_Object * __Handle_ADC_Manage_Object, uint16_t __Sample_Number, float __AmpGain, float __Shunt_Resistor)
{
    Handle_ADC_Manage_Object = __Handle_ADC_Manage_Object;
    Sample_Number = __Sample_Number;
    AmpGain = __AmpGain;
    Shunt_Resistor = __Shunt_Resistor;
    // Filter_Sampler.Init(1000, 2);
}

void Class_Sampler::Sampler_Calibrate()
{
    ADC_Init(Handle_ADC_Manage_Object->ADC_Handler, Sample_Number);
    const int calibration_rounds = 1000;
    for(int i = 0; i < calibration_rounds; i++)
    {
        for(int j = 0; j < Handle_ADC_Manage_Object->Sample_Number; j++)
        {
            Offset[j] += Handle_ADC_Manage_Object->ADC_Data[j];
            HAL_Delay(1);
        }
    }
    for(int j = 0; j < Handle_ADC_Manage_Object->Sample_Number; j++)
    {
        Offset[j] /= calibration_rounds;
    }
}

void Class_Sampler::TIM_Sampler_PeriodCalculateCallBack()
{
    PeriodCalculateCallBack();
}

