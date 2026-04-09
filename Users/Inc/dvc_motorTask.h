#ifndef DVC_MOTOR_TASK_H_
#define DVC_MOTOR_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "my_usart.h"




void Task_EncoderTest_Init(void * param);

void Task_EncoderTest(void * param);

void Task_EncoderDisp_Init();

void Task_EncoderDisp(void * param);

void Task_Motor_2208_Init(void * param);

void Task_Motor_2208_Test(void * param);



#ifdef __cplusplus
}
#endif

#endif /* DVC_MOTOR_TASK_H_ */
