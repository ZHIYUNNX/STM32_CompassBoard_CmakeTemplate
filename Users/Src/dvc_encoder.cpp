#include "dvc_encoder.h"

void Class_Encoder_AS5600::AS5600_Init(I2C_HandleTypeDef * __phi2c, uint8_t __Addr_7bit)
{
    Addr = __Addr_7bit << 1;
    I2C_Config.phi2c = __phi2c;
    AS5600_ReadAgle_Raw();
    Pre_Pos = Angle;
    Pre_Angle = Pos;
    Filter_Omega.Init(1000, 0.01);
}

void Class_Encoder_AS5600::AS5600_ReadAgle_Raw()
{
    uint8_t data_buffer[2];
    if(HAL_I2C_Mem_Read(I2C_Config.phi2c, Addr, AS5600_REG_ANGLE_HIGH, I2C_MEMADD_SIZE_8BIT, data_buffer, 2, HAL_MAX_DELAY)==HAL_OK)
    {
        Pre_Pos_Raw = Pos_Raw;
        Pos_Raw = (uint16_t)((uint16_t)data_buffer[0]<<8 | data_buffer[1]);
    }
    else
    {
        return;
    }
}

void Class_Encoder_AS5600::TIM_Encoder_ElapsedCallback()
{
    AS5600_ReadAgle_Raw();
    int16_t delta_Pos = Pos_Raw - Pre_Pos_Raw;

    if(Pos_Raw >= 0)
    {
        switch(DIR)
        {
            case 1:
            {
                if(delta_Pos < -2048)
                {
                    round_count++;
                }
                else if(delta_Pos > 2048)
                {
                    round_count--;
                }
            }
            break;
            case -1:
            {
                if(delta_Pos < -2048)
                {
                    round_count--;
                }
                else if(delta_Pos > 2048)
                {
                    round_count++;
                }
            }
            break;
            default:
            {
                break;
            }
        }
        
    }
    else
    {
        return;
    }
    
    Pos = Pos_Raw + round_count*4096;
    Angle_Raw = DIR * Pos_Raw * 2 * PI / AS5600_RESOLUTION;
    Angle = Angle_Raw + round_count*2*PI;
    Omega_Raw = (Angle - Pre_Angle) * 1000; // rad/s, since this function is called every 1ms
    Pre_Pos = Pos;
    Pre_Angle = Angle;
    Filter_Omega.Set_Now(Omega_Raw);
    Filter_Omega.TIM_Calculate_PeriodElapsedCallback();
    Omega = Filter_Omega.Get_Out();
}