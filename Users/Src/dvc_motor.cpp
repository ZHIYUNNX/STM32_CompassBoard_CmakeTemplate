#include "dvc_motor.h"
#include "drv_pwm.h"

void Class_Motor_2208::Init(Struct_PWM_Channel __PWM_Output_Channel[3], I2C_HandleTypeDef * __phi2c, Struct_ADC_Manage_Object * __Handle_ADC_Manage_Object, uint8_t __Addr_7bit)
{
    Encoder.AS5600_Init(__phi2c);

    Handle_ADC_Manage_Object = __Handle_ADC_Manage_Object;
    Sampler_Torque.Init(Handle_ADC_Manage_Object, 2);
    

    Positive_FOC.Init(POSITIVE_TRANS, 12.0f);
    Inverse_FOC.Init(INVERSE_TRANS, 12.0f);

    for(uint8_t i = 0; i < 3; i++)
    {
        PWM_Output_Channel[i].Channel = __PWM_Output_Channel[i].Channel;
        PWM_Output_Channel[i].htim= __PWM_Output_Channel[i].htim;
        PWM_Init(&__PWM_Output_Channel[i]);
    }
    HAL_GPIO_WritePin(Motor_EN_GPIO_Port, Motor_EN_Pin, GPIO_PIN_SET);
    OpenLoop_Control(0, 0, 0);
    HAL_Delay(1000);
    Sampler_Torque.Sampler_Calibrate();
    Encoder.TIM_Encoder_ElapsedCallback();
    Theta_e_Calibrate();
    Encoder.TIM_Encoder_ElapsedCallback();
    Sampler_CallBack();
    
    
    

}

void Class_Motor_2208::OutPut()
{
    PWM_Set_Duty(&PWM_Output_Channel[2], Positive_FOC.Get_U_a()/Positive_FOC.Get_U_Max());
    PWM_Set_Duty(&PWM_Output_Channel[1], Positive_FOC.Get_U_b()/Positive_FOC.Get_U_Max());
    PWM_Set_Duty(&PWM_Output_Channel[0], Positive_FOC.Get_U_c()/Positive_FOC.Get_U_Max());
}

void Class_Motor_2208::OpenLoop_Control(float __U_q, float __U_d, float __Theta_e)
{
    Positive_FOC.Set_Theta_e(__Theta_e);
    Positive_FOC.Set_U_ref(__U_q, __U_d);
    Positive_FOC.TIM_FOC_CalculateElapsedCallback();
    OutPut();
}

void Class_Motor_2208::Update_Theta_e_Set()
{
    Theta_e_Set = normalizeAngle(Encoder.Get_Angle_Raw() * 7 - Theta_e_Zero);
}

void Class_Motor_2208::Update_Theta_e_Now()
{
    Theta_e_Now = normalizeAngle(Encoder.Get_Angle_Raw() * 7 - Theta_e_Now_Zero);
}

void Class_Motor_2208::Theta_e_Calibrate()
{
    Theta_e_Zero = 0;
    Theta_e_Now_Zero = 0;
    OpenLoop_Control(5.5f, 0, _3PI_2);
    HAL_Delay(1000);
    Encoder.TIM_Encoder_ElapsedCallback();
    Update_Theta_e_Set();
    Theta_e_Zero = Theta_e_Set;
    OpenLoop_Control(0, 0, _3PI_2);
    HAL_Delay(1000);
    // OpenLoop_Control(5.5f, 0, PI);
    // HAL_Delay(1000);
    // Encoder.TIM_Encoder_ElapsedCallback();
    // Update_Theta_e_Now();
    // Theta_e_Now_Zero = Theta_e_Now;
}   

void Class_Motor_2208::Sampler_CallBack()
{
    const float AmpGain = Sampler_Torque.Get_AmpGain();
    const float Shunt_Resistor = Sampler_Torque.Get_ShuntResister();
    static float i_1, i_2 = 0.0;
    float i_temp[3] = {0};
    i_temp[0] = -1 * (float)(((int16_t)Handle_ADC_Manage_Object->ADC_Data[0] - Sampler_Torque.Get_Offset()[0])* 3.3f)/ 4096 / AmpGain / Shunt_Resistor; // 假设电流传感器的增益为10
    i_temp[1] = -1 * (float)(((int16_t)Handle_ADC_Manage_Object->ADC_Data[1] - Sampler_Torque.Get_Offset()[1])* 3.3f)/ 4096 / AmpGain / Shunt_Resistor;
    i_1 = i_temp[1];//I_B
    i_2 = -i_temp[0]-i_temp[1];//I_A
    Update_Theta_e_Set();
    Inverse_FOC.Set_Theta_e(Theta_e_Set);
    Inverse_FOC.Set_U_Phase(i_2, i_1, 0);
    Inverse_FOC.TIM_FOC_CalculateElapsedCallback();
    Filter_Torque.Set_Now(Inverse_FOC.Get_U_q());
    Filter_Torque.TIM_Calculate_PeriodElapsedCallback();
    Torque_Now = Filter_Torque.Get_Out();
}

void Class_Motor_2208::TIM_Motor_PeriodElapsedCallBack()
{
    Encoder.TIM_Encoder_ElapsedCallback();
    Omega_Now = Encoder.Get_Omega();
    Angle_Now = Encoder.Get_Angle();
    // Update_Theta_e_Set();
    Sampler_CallBack();//ADC定时回调
    
    switch(Control_Mode)
    {
        
        case Torque_Openloop_Mode:
        {
            Positive_FOC.Set_Theta_e(Theta_e_Set);
            Positive_FOC.TIM_FOC_CalculateElapsedCallback();
            OutPut();
            
        }   
        break;
        case Torque_Closeloop_Mode:
        {
            Positive_FOC.Set_Theta_e(Theta_e_Set);
            PID_Torque.Set_Now(Torque_Now);
            PID_Torque.Set_Target(Torque_Target);
            PID_Torque.TIM_Adjust_PeriodElapsedCallback();
            Positive_FOC.Set_U_ref(PID_Torque.Get_Out(), 0.0f);
            Positive_FOC.TIM_FOC_CalculateElapsedCallback();
            OutPut();
        }   
        break;
        case Omega_Closeloop_Mode:
        {
            PID_Omega.Set_Now(Omega_Now);
            PID_Omega.Set_Target(Omega_Target);
            PID_Omega.TIM_Adjust_PeriodElapsedCallback();
            Torque_Target = PID_Omega.Get_Out();
            Positive_FOC.Set_Theta_e(Theta_e_Set);
            PID_Torque.Set_Now(Torque_Now);
            PID_Torque.Set_Target(Torque_Target);
            PID_Torque.TIM_Adjust_PeriodElapsedCallback();
            Positive_FOC.Set_U_ref(PID_Torque.Get_Out(), 0.0f);
            Positive_FOC.TIM_FOC_CalculateElapsedCallback();
            OutPut();
        }   
        break;
        case Angle_Torque_Closeloop_Mode:
        {
            PID_Angle.Set_Now(Angle_Now);
            PID_Angle.Set_Target(Angle_Target);
            PID_Angle.TIM_Adjust_PeriodElapsedCallback();
            Torque_Target = PID_Angle.Get_Out();
            Positive_FOC.Set_Theta_e(Theta_e_Set);
            PID_Torque.Set_Now(Torque_Now);
            PID_Torque.Set_Target(Torque_Target);
            PID_Torque.TIM_Adjust_PeriodElapsedCallback();
            Positive_FOC.Set_U_ref(PID_Torque.Get_Out(), 0.0f);
            Positive_FOC.TIM_FOC_CalculateElapsedCallback();
            OutPut();
        }
        break;
        case Angle_Omega_Closeloop_Mode:
        {
            PID_Angle.Set_Now(Angle_Now);
            PID_Angle.Set_Target(Angle_Target);
            PID_Angle.TIM_Adjust_PeriodElapsedCallback();
            Omega_Target = PID_Angle.Get_Out();
            PID_Omega.Set_Now(Omega_Now);
            PID_Omega.Set_Target(Omega_Target);
            PID_Omega.TIM_Adjust_PeriodElapsedCallback();
            Torque_Target = PID_Omega.Get_Out();
            PID_Torque.Set_Now(Torque_Now);
            PID_Torque.Set_Target(Torque_Target);
            PID_Torque.TIM_Adjust_PeriodElapsedCallback();
            Positive_FOC.Set_Theta_e(Theta_e_Set);
            Positive_FOC.Set_U_ref(PID_Torque.Get_Out(), 0.0f);
            Positive_FOC.TIM_FOC_CalculateElapsedCallback();
            OutPut();
        }
        break;
        case NOT_READY:
        {
            OpenLoop_Control(0, 0, Angle_Target);
            
        }
        break;
        // default:
        // {
        //     OpenLoop_Control(0, 0, Angle_Target);
        //     break;
        // }
        
    }

    
}


