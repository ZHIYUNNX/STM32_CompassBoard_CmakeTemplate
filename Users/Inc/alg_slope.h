#ifndef ALG_SLOPE_H
#define ALG_SLOPE_H


#include"drv_math.h"

class Class_Slope;

typedef enum
{
    SLOPE_TARGET_FIRST = 0,
    SLOPE_REAL_FIRST,

}Enum_Slope_FirstMode;

class Class_Slope
{
public:
    void Init(float __Increase_Step, float __Decrease_Step, Enum_Slope_FirstMode __Slope_FirstMode = SLOPE_TARGET_FIRST);
    
    inline void Set_Increase(float __Increase_Step);

    inline void Set_Decrease(float __Decrease_Step);

    inline void Set_Target(float __Target);

    inline void Set_Now_Real(float __Now_Real);

    inline float Get_Out();

    void TIM_Calculate_PeriodElapsedCallback();
    
protected:

    Enum_Slope_FirstMode Slope_FirstMode = SLOPE_TARGET_FIRST;

    float Increase_Step = 0.0f;
    float Decrease_Step = 0.0f;

    float Target = 0.0f;
    float Now_Real = 0.0f;
    float Now_Planning = 0.0f;

    float Out = 0.0f;
};

inline float Class_Slope::Get_Out()
{
    return (Out);
}

inline void Class_Slope::Set_Increase(float __Increase_Step)
{
    Increase_Step = __Increase_Step;
}

inline void Class_Slope::Set_Decrease(float __Decrease_Step)
{
    Decrease_Step = __Decrease_Step;
}

inline void Class_Slope::Set_Target(float __Target)
{
    Target = __Target;
}

inline void Class_Slope::Set_Now_Real(float __Now_Real)
{
    Now_Real = __Now_Real;
}



#endif
