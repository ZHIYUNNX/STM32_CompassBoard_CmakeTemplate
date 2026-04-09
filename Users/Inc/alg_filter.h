#ifndef __ALG_FILTER_H
#define __ALG_FILTER_H

#include "drv_math.h"

class Class_Filter_Second_Order_Butterworth
{
public:
    void Init(float __Fs, float __Fca);
    void TIM_Calculate_PeriodElapsedCallback();
    inline void Set_Now(float __Now);
    inline float Get_Out();

protected:
    
    float Fs;
    float Fca;
    float Fcd;

    float a0;
    float a1;
    float a2;

    float b0;
    float b1;
    float b2;

    float c;

    float Now;

    float x0;
    float x1;
    float x2;
    float y1;
    float y2;

    float y;

    float Out;

};

inline void Class_Filter_Second_Order_Butterworth::Set_Now(float __Now)
{
    Now = __Now;
}

inline float Class_Filter_Second_Order_Butterworth::Get_Out()
{
    return Out;
}

#endif /*__ALG_FILTER_H*/
