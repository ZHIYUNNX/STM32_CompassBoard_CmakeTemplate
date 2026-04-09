#ifndef ITA_CONTROL_H
#define ITA_CONTROL_H

#include "alg_slope.h"
#include "dvc_motor.h"

class Class_Controller
{
public:
    void Init();

    Class_Motor_2208 Motor;
    Class_Slope Slope_Angle;

    inline float Get_Expect_Angle();
    inline void Set_Expect_Angle(float __Expect_Angle);
    
    void TIM_Calculate_PeriodElapsedCallback();
protected:
    float Expect_Angle;
    
};


inline float Class_Controller::Get_Expect_Angle()
{
    return Expect_Angle;
}

inline void Class_Controller::Set_Expect_Angle(float __Expect_Angle)
{
    Expect_Angle = __Expect_Angle;
}
#endif /*ITA_CONTROL_H*/