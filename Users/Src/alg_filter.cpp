#include "alg_filter.h"

void Class_Filter_Second_Order_Butterworth::Init(float __Fs, float __Fca)
{
    Fs = __Fs;
    Fca = __Fca;

    float wc = Fca * 2 * PI;
    float wc_2 = wc * wc;
    
    c = 1 + _sqrt2 * wc + wc_2;

    a1 = (2 * wc_2 - 2) / c;
    a2 = (1 - _sqrt2 * wc + wc_2) / c;

    b0 = wc_2 / c;
    b1 = 2 * b0;
    b2 = b0;

}


void Class_Filter_Second_Order_Butterworth::TIM_Calculate_PeriodElapsedCallback()
{
    y = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    x0 = Now;
    x2 = x1;
    x1 = x0;

    y2 = y1;
    y1 = y;
    Out = y;
}