#include "alg_foc.h"
#include "drv_math.h"

void Class_FOC::Init(FOC_TransMode_Enum __Trans_Mode, float __U_Max)
{
    Trans_Mode = __Trans_Mode;
    U_Max = __U_Max;
}

void Class_FOC::FOC_Trans_Positive()
{
    Math_Constrain(&U_q, -U_Max / 2, U_Max / 2);

    float ct = arm_cos_f32(Theta_e);
    float st = arm_sin_f32(Theta_e);

    U_alpha = -U_q * st + U_d * ct;
    U_beta = U_q * ct + U_d * st;

    U_a = U_alpha + U_Max / 2;
    U_b = (_sqrt3 * U_beta - U_alpha) / 2 + U_Max / 2;
    U_c = (-_sqrt3 * U_beta - U_alpha) / 2 + U_Max / 2;
}

void Class_FOC::FOC_Trans_Inverse()
{
    float ct = arm_cos_f32(Theta_e);
    float st = arm_sin_f32(Theta_e);
    U_alpha = U_a;
    U_beta = _1_sqrt3 * U_a + _2_sqrt3 * U_b;
    U_q = U_beta * ct - U_alpha * st;
}

void Class_FOC::TIM_FOC_CalculateElapsedCallback()
{
    Theta_e = normalizeAngle(Theta_e);
    switch(Trans_Mode)
    {
        case POSITIVE_TRANS:
        {
            FOC_Trans_Positive();
        }
        break;
        case INVERSE_TRANS:
        {
            FOC_Trans_Inverse();
        }
        break;
        default:
        {
            U_a = 0.0f;
            U_b = 0.0f;
            U_c = 0.0f;
            U_q = 0.0f;
            U_d = 0.0f;
        }
        break;
    }
}