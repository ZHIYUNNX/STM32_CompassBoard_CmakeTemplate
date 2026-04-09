#ifndef __ALG_FOC_H
#define __ALG_FOC_H

#include "drv_math.h"


typedef enum
{

    POSITIVE_TRANS = 0,
    INVERSE_TRANS,

}FOC_TransMode_Enum;

class Class_FOC
{
public:
    void Init(FOC_TransMode_Enum __Trans_Mode, float __U_Max);
    inline void Set_U_ref(float __U_q, float __U_d);
    inline void Set_U_Phase(float __U_a, float __U_b, float __U_c);
    inline void Set_Theta_e(float __Theta_e);

    inline float Get_U_a();
    inline float Get_U_b();
    inline float Get_U_c();

    inline float Get_U_q();
    inline float Get_U_d();


    inline float Get_U_Max();

    void TIM_FOC_CalculateElapsedCallback();
    

protected:
    FOC_TransMode_Enum Trans_Mode;

    float U_Max = 12.0f;

    float Theta_e = 0.0f;

    float U_a = 0.0f;
    float U_b = 0.0f;
    float U_c = 0.0f;

    float U_alpha = 0.0f;
    float U_beta = 0.0f;

    float U_q = 0.0f;
    float U_d = 0.0f;

    void FOC_Trans_Positive();
    void FOC_Trans_Inverse();

};

inline void Class_FOC::Set_U_ref(float __U_q, float __U_d)
{
    U_q = __U_q;
    U_d = __U_d;
}
inline void Class_FOC::Set_U_Phase(float __U_a, float __U_b, float __U_c)
{
    U_a = __U_a;
    U_b = __U_b;
    U_c = __U_c;
}

inline void Class_FOC::Set_Theta_e(float __Theta_e)
{
    Theta_e = __Theta_e;
}

inline float Class_FOC::Get_U_a()
{
    return U_a;
}
inline float Class_FOC::Get_U_b()
{
    return U_b;
}
inline float Class_FOC::Get_U_c()
{
    return U_c;
}

inline float Class_FOC::Get_U_q()
{
    return U_q;
}
inline float Class_FOC::Get_U_d()
{
    return U_d;
}

inline float Class_FOC::Get_U_Max()
{
    return U_Max;
}



#endif /*ALG_FOC_H*/