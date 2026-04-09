#ifndef __ALG_MATH_H
#define __ALG_MATH_H

#include "arm_math.h"
#include "math.h"

#define _sqrt3 1.7320508f
#define _1_sqrt3 0.57735026919f
#define _2_sqrt3 1.15470053838f
#define _sqrt2 1.41421356237f

#define _3PI_2 4.71238898038f
#define _7PI_4 5.49778714378f

float normalizeAngle(float angle);

/**
 * @brief 限幅函数
 *
 * @tparam Type 类型
 * @param x 传入数据
 * @param Min 最小值
 * @param Max 最大值
 */
template<typename Type>
Type Math_Constrain(Type *x, Type Min, Type Max)
{
    if (*x < Min)
    {
        *x = Min;
    }
    else if (*x > Max)
    {
        *x = Max;
    }
    return (*x);
}

/**
 * @brief 求绝对值
 *
 * @tparam Type 类型
 * @param x 传入数据
 * @return Type x的绝对值
 */
template<typename Type>
Type Math_Abs(Type x)
{
    return ((x > 0) ? x : -x);
}



#endif /*__ALG_MATH_H*/