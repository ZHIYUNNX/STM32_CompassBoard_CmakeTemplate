#ifndef __DVC_AS5600_H
#define __DVC_AS5600_H


#include "stm32f1xx_hal.h"
#include "alg_filter.h"
#include "i2c.h"

#ifndef PI 
#define PI					3.14159265358979f
#endif



#define AS5600_ADDR_7BIT 0x36
#define AS5600_REG_ANGLE_HIGH 0x0C
#define AS5600_REG_ANGLE_LOW 0x0D
#define AS5600_RESOLUTION 4096.0f

typedef struct
{  
    I2C_HandleTypeDef * phi2c;

}Struct_I2C;

class Class_Encoder_AS5600
{
public:
    void AS5600_Init(I2C_HandleTypeDef * __phi2c, uint8_t __Addr_7bit = AS5600_ADDR_7BIT);
    inline int16_t Get_Pos_Raw();
    inline int Get_Round();
    inline int Get_Pos();
    inline float Get_Angle();
    inline float Get_Angle_Raw();
    inline float Get_Omega();
    inline float Get_Omega_Raw();
    void TIM_Encoder_ElapsedCallback();
    void AS5600_ReadAgle_Raw();

    Class_Filter_Second_Order_Butterworth Filter_Omega;

    int8_t DIR = -1;


protected:

    uint8_t Addr;
    Struct_I2C I2C_Config; 

    int16_t Pre_Pos_Raw = 0;
    int16_t Pos_Raw = 0;

    int Pre_Pos = 0;
    float Pre_Angle = 0.0f;
    float Pre_Omega = 0.0f;

    int Pos = 0;
    float Angle_Raw = 0.0f;
    float Angle = 0.0f;
    float Omega_Raw = 0.0f;
    float Omega = 0.0f;
    int round_count = 0;

    

};

inline int16_t Class_Encoder_AS5600::Get_Pos_Raw()
{
    return Pos_Raw;
}

inline float Class_Encoder_AS5600::Get_Omega_Raw()
{
    return Omega_Raw;
}

inline float Class_Encoder_AS5600::Get_Angle_Raw()
{
    return Angle_Raw;
}

inline int Class_Encoder_AS5600::Get_Round()
{
    return round_count;
}

inline int Class_Encoder_AS5600::Get_Pos()
{
    return Pos;
}

inline float Class_Encoder_AS5600::Get_Angle()
{
    return Angle;
}

inline float Class_Encoder_AS5600::Get_Omega()
{
    return Omega;
}

#endif /*__DVC_AS5600_H*/