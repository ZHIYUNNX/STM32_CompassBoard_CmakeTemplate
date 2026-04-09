#include "alg_slope.h"



void Class_Slope::Init(float __Increase_Step, float __Decrease_Step, Enum_Slope_FirstMode __Slope_FirstMode)
{
    Increase_Step = __Increase_Step;
    Decrease_Step = __Decrease_Step;
    Slope_FirstMode = __Slope_FirstMode;
}

void Class_Slope::TIM_Calculate_PeriodElapsedCallback()
{
    if(Slope_FirstMode == SLOPE_REAL_FIRST)
    {
        if((Now_Real <= Target && Now_Planning <= Now_Real )||(Now_Real >= Target && Now_Planning >= Now_Real ))
        {
            Out = Now_Real;
        }
    }

    else if(Now_Planning > 0)
    {
        if(Target > Now_Planning)
        {
            if(Target - Now_Planning > Increase_Step)
            {
                Out += Increase_Step;
            }
            else if(Target - Now_Planning < Increase_Step)
            {
                Out = Target;
            }
            
        }
        else if(Target < Now_Planning)
        {
            if(Now_Planning - Target > Decrease_Step)
            {
                Out -= Decrease_Step;
            }
            else if(Now_Planning - Target < Decrease_Step)
            {
                Out = Target;
            }
        }
    }

    else if(Now_Planning < 0)
    {
        if(Target < Now_Planning)
        {
            if(Now_Planning - Target > Increase_Step)
            {
                Out -= Increase_Step;
            }
            else if(Now_Planning - Target < Increase_Step)
            {
                Out = Target;
            }
        }
        else if(Target > Now_Planning)
        {
            if(Now_Planning - Target < Decrease_Step)
            {
                Out += Decrease_Step;
            }
            else if(Now_Planning - Target > Decrease_Step)
            {
                Out = Target;
            }
        }
    }

    else
    {
        if(Target > Now_Planning)
        {
            if(Target - Now_Planning > Increase_Step)
            {
                Out += Increase_Step;
            }
            else if(Target - Now_Planning < Increase_Step)
            {
                Out = Target;
            }
        }
        else if(Target < Now_Planning)
        {
            if(Now_Planning - Target > Increase_Step)
            {
                Out -= Increase_Step;
            }
            else if(Now_Planning - Target < Increase_Step)
            {
                Out = Target;
            }
        }
    }

    Now_Planning = Target;
}
