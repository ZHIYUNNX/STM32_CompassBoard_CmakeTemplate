#include "dvc_motorTask.h"
#include "dvc_encoder.h"
#include "FreeRTOS.h"
#include "dvc_motor.h"
#include "task.h"
#include "tim.h"
#include "alg_filter.h"
#include "drv_adc.h"

extern uint8_t disp_mode;

Class_Encoder_AS5600 Encoder_0;
Class_Encoder_AS5600 Encoder_1;

Class_Motor_2208 Motor_0;

Class_Filter_Second_Order_Butterworth Filter_Test;

void Task_Motor_2208_Init(void * param)
{
    Struct_PWM_Channel PWM_Channel[3] = {
        {&htim3, TIM_CHANNEL_1},
        {&htim3, TIM_CHANNEL_2},
        {&htim3, TIM_CHANNEL_3}
    };
    ADC_Init(&hadc1, 2);
    
    Motor_0.Set_Control_Mode(NOT_READY);
    Motor_0.Filter_Torque.Init(1000, 0.0005);
    Motor_0.Init(PWM_Channel, &hi2c1, &ADC1_Manage_Object, AS5600_ADDR_7BIT);
    Motor_0.PID_Torque.Init(20.0f, 88.0f, 1.5f,0.0f, 5.0f, 6.0f, 0.001f, 0.0f, 0.0f, 0.0f, 0.0f);
    Motor_0.PID_Omega.Init(0.001f, 0.03f, 0.0f, 0.0f, 0.30f, 0.3f, 0.001f, 0.0f, 0.0f, 0.0f, 0.0f);
    // Motor_0.PID_Angle.Init(0.005f, 3.0f, 0.0f, 0.0f, 50.0f, 60.0f, 0.001f, 0.0f, 0.0f, 0.0f, 6.28f);
    Motor_0.PID_Angle.Init(0.005f, 3.0f, 0.0f, 0.0f, 0.3f, 0.3f, 0.001f, 0.0f, 0.0f, 0.0f, 6.28f);
    // 0.025 2.0 0.0 角度力矩环
    // 0.01  2.0 0.0 角度速度力矩环
    Motor_0.Encoder.Filter_Omega.Init(1000, 0.001);
    

    
}

void Task_Motor_2208_Test(void * param)
{
    static int32_t cnt = 0;
    while(1)
    {
        if(disp_mode == 4)
        {
            cnt++;
            if(cnt >= 0 && cnt < 1000)
            {
                Motor_0.Positive_FOC.Set_U_ref(0.006 * cnt,0);
            }
            else if(cnt >= 1000 && cnt < 3000)
            {
                Motor_0.Positive_FOC.Set_U_ref(0.006 * (2000 - cnt),0);
            }
            else if(cnt >= 3000 && cnt < 4000)
            {
                Motor_0.Positive_FOC.Set_U_ref(0.006 * (cnt - 4000),0);
            }
            else
            {
                cnt = 0;
            }
        }
        Motor_0.TIM_Motor_PeriodElapsedCallBack();
        vTaskDelay(1);
    }
}


void Task_EncoderTest_Init(void * param)
{
    Encoder_0.AS5600_Init(&hi2c1, AS5600_ADDR_7BIT);
    Encoder_1.AS5600_Init(&hi2c2, AS5600_ADDR_7BIT);
}

void Task_EncoderTest(void * param)
{
    while(1)
    {
        Encoder_0.TIM_Encoder_ElapsedCallback();
        Encoder_1.TIM_Encoder_ElapsedCallback();
        // usart_printf(&huart2, "Pos: %d, Angle: %f\n", Encoder_0.Get_Pos(), Encoder_0.Get_Angle());
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void Task_EncoderDisp_Init()
{
    Filter_Test.Init(1000, 2);
}

void Task_EncoderDisp(void * param)
{
    while(1)
    {
        switch(disp_mode)
        {
            case 0:
            {
                break;
            }
            case 1:
            {
                usart_printf(&huart1, "%f,%f\n",Motor_0.Get_Torque_Now(),Motor_0.Get_Torque_Target());

            }
            break;
            case 2:
            {
                usart_printf(&huart1, "%f,%f\n",Motor_0.Get_Angle_Target(),Motor_0.Get_Angle_Now());

            }
            break;
            case 3:
            {
                usart_printf(&huart1, "%f,%f\n",Motor_0.Get_Omega_Now(),Motor_0.Get_Omega_Target());

            }
            break;
            case 4:
            {
                usart_printf(&huart1, "%f,%f\n",Motor_0.Get_Torque_Now(),Motor_0.Get_Omega_Now());

            }
            break;
            // default:
            // {
            //     break;
            // }
        }
    }
}
