#include "ita_control.h"

void Class_Controller::Init()
{
    // Slope_Angle.Init();
}

void Class_Controller::TIM_Calculate_PeriodElapsedCallback()
{
    switch(Motor.Get_Control_Mode())
    {
        case(Angle_Slope_Mode):
        {
            float tmp_motor_angle;
            float motor_now_angle = Motor.Get_Angle_Now();
            Slope_Angle.Set_Now_Real(motor_now_angle);
            Slope_Angle.Set_Target(Expect_Angle);
            Slope_Angle.TIM_Calculate_PeriodElapsedCallback();
            tmp_motor_angle = Slope_Angle.Get_Out();
            Motor.Set_Angle_Target(tmp_motor_angle);
        }
        break;
        default:
            break;
    }
}