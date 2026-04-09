#ifndef DRV_MOTOR_H_
#define DRV_MOTOR_H_


#include "drv_pwm.h"
#include "dvc_encoder.h"
#include "alg_foc.h"
#include "alg_pid.h"
#include "dvc_sampler.h"
#include "alg_slope.h"

typedef enum
{
    Torque_Openloop_Mode = 0,
    Torque_Closeloop_Mode,
    Omega_Openloop_Mode,
    Omega_Closeloop_Mode,
    Angle_Torque_Closeloop_Mode,
    Angle_Omega_Closeloop_Mode,
    Angle_Slope_Mode,
    NOT_READY,

}Enum_Motor_ControlMode;

class Class_Motor_2208
{
    public:
        void Init(Struct_PWM_Channel __PWM_Output_Channel[3], I2C_HandleTypeDef * __phi2c, Struct_ADC_Manage_Object * __Handle_ADC_Manage_Object, uint8_t __Addr_7bit = AS5600_ADDR_7BIT);//在dvc_motorTask.cpp中建立motorTask_Init函数并在其中调用该函数进行电机初始化 
        Class_Encoder_AS5600 Encoder;

        Class_FOC            Positive_FOC;
        Class_FOC            Inverse_FOC;

        Class_PID            PID_Torque;
        Class_PID            PID_Angle;
        Class_PID            PID_Omega;

        Class_Slope          Slope_Angle; 

        Struct_ADC_Manage_Object * Handle_ADC_Manage_Object;
        Class_Sampler              Sampler_Torque;

        Class_Filter_Second_Order_Butterworth Filter_Torque;

        inline void Set_Torque_Target(float __Torque_Target);
        inline void Set_Omega_Target(float __Omega_Target);
        inline void Set_Angle_Target(float __Angle_Target);
        inline void Set_Theta_e_Zero(float __Theta_e_Zero);

        inline float Get_Torque_Now();
        inline float Get_Omega_Now();
        inline float Get_Angle_Now();
        inline float Get_Theta_e_Zero();
        inline float Get_Theta_e_Set();
        inline float Get_Theta_e_Now();

        inline float Get_Torque_Target();
        inline float Get_Omega_Target();
        inline float Get_Angle_Target();

        inline Enum_Motor_ControlMode Get_Control_Mode();
        inline void Set_Control_Mode(Enum_Motor_ControlMode __Control_Mode);

        void Theta_e_Calibrate();

        void Sampler_CallBack();

        void OpenLoop_Control(float __U_q, float __U_d, float __Theta_e);

        void TIM_Motor_PeriodElapsedCallBack();
        
    protected:
        Struct_PWM_Channel PWM_Output_Channel[3];
        Enum_Motor_ControlMode Control_Mode;

        float Theta_e_Now = 0.0f;
        float Theta_e_Zero = 0.0f;
        float Theta_e_Set = 0.0f;
        float Theta_e_Now_Zero = 0.0f;
        

        float Torque_Now = 0.0f;
        float Angle_Now = 0.0f;
        float Omega_Now = 0.0f;

        float Torque_Target = 0.0f;
        float Angle_Target = 0.0f;
        float Omega_Target = 0.0f;

        void Update_Theta_e_Set();

        void Update_Theta_e_Now();

        void OutPut();
    
};


inline void Class_Motor_2208::Set_Torque_Target(float __Torque_Target)
{
    Torque_Target = __Torque_Target;
}      

inline void Class_Motor_2208::Set_Omega_Target(float __Omega_Target)
{
    Omega_Target = __Omega_Target;
}

inline void Class_Motor_2208::Set_Angle_Target(float __Angle_Target)
{
    Angle_Target = __Angle_Target;
}

inline void Class_Motor_2208::Set_Theta_e_Zero(float __Theta_e_Zero)
{
    Theta_e_Zero = __Theta_e_Zero;
}

inline void Class_Motor_2208::Set_Control_Mode(Enum_Motor_ControlMode __Control_Mode)
{
    Control_Mode = __Control_Mode;
}

inline float Class_Motor_2208::Get_Torque_Now()
{
    return Torque_Now;
}
inline float Class_Motor_2208::Get_Omega_Now()
{
    return Omega_Now;
}
inline float Class_Motor_2208::Get_Angle_Now()
{
    return Angle_Now;
}

inline float Class_Motor_2208::Get_Theta_e_Zero()
{
    return Theta_e_Zero;
}

inline float Class_Motor_2208::Get_Theta_e_Set()
{
    return Theta_e_Set;
}

inline float Class_Motor_2208::Get_Theta_e_Now()
{
    return Theta_e_Now;
}

inline float Class_Motor_2208::Get_Torque_Target()
{
    return Torque_Target;
}
inline float Class_Motor_2208::Get_Omega_Target()
{
    return Omega_Target;
}
inline float Class_Motor_2208::Get_Angle_Target()
{
    return Angle_Target;
}

inline Enum_Motor_ControlMode Class_Motor_2208::Get_Control_Mode()
{
    return Control_Mode;
}

#endif /* DRV_MOTOR_H_ */
