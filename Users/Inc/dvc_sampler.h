#ifndef __DVC_SAMPLER_H
#define __DVC_SAMPLER_H

#include "drv_adc.h"
#include "alg_filter.h"

typedef void (*Sampler_PeriodCalculateCallBack)(void);

class Class_Sampler
{
public:
    void Init(Struct_ADC_Manage_Object * __Handle_ADC_Manage_Object, uint16_t __Sample_Number, float __AmpGain = 50.0f, float __Shunt_Resistor = 0.01f);

    Class_Filter_Second_Order_Butterworth Filter_Sampler;

    Sampler_PeriodCalculateCallBack PeriodCalculateCallBack;

    

    inline int32_t * Get_Offset();

    inline float Get_AmpGain();

    inline float Get_ShuntResister();

    void Sampler_Calibrate();
    void TIM_Sampler_PeriodCalculateCallBack();

protected:
    float AmpGain = 50.0f;

    float Shunt_Resistor = 0.01f;

    Struct_ADC_Manage_Object * Handle_ADC_Manage_Object;

    uint16_t Sample_Number;

    int32_t Offset[SAMPLE_BUFFER_SIZE] = {0};

};

inline int32_t * Class_Sampler::Get_Offset()
{
    return Offset;
}

inline float Class_Sampler::Get_AmpGain()
{
    return AmpGain;
}

inline float Class_Sampler::Get_ShuntResister()
{
    return Shunt_Resistor;
}


#endif /* __DVC_SAMPLER_H */
