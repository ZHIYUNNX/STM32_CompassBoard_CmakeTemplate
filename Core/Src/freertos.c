/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "stdio.h"
#include "led_bsp.h"
#include "lcd_bsp.h"
#include "beep_bsp.h"
#include "my_usart.h"
#include "usart.h"
#include "dvc_motorTask.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

  TaskHandle_t Motor_Handle = NULL;
  TaskHandle_t Encoder_Handle = NULL;
  TaskHandle_t USART_BT_Handle = NULL;
  TaskHandle_t LCD_Handle = NULL;
  TaskHandle_t LED_Handle = NULL;
  extern int mycnt;
  extern GPIO_PinState cur_state;
/* USER CODE END Variables */
/* Definitions for LED_Blink */
osThreadId_t LED_BlinkHandle;
const osThreadAttr_t LED_Blink_attributes = {
  .name = "LED_Blink",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void task_led(void * param);
void task_lcd(void * param);
void task_usart_bt(void * param);
void task_Encoder_Start(void * param);

extern void Task_EncoderTest(void * param);
extern void Task_EncoderDisp(void * param);
extern void Task_Motor_2208_Test(void * param);

void task_led(void * param)
{
  while(1)
  {
    // LED_Control(LED_B_Pin, OFF);
    // LED_Control(LED_R_Pin, ON);
    // HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
    // osDelay(pdMS_TO_TICKS(500));
    // LED_Control(LED_R_Pin, OFF);
    // LED_Control(LED_G_Pin, ON);
    // // HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
    // osDelay(pdMS_TO_TICKS(500));
    // LED_Control(LED_G_Pin, OFF);
    // LED_Control(LED_B_Pin, ON);
    // // HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
    // osDelay(pdMS_TO_TICKS(500));
    // mycnt++;
  }
    
} 

void task_lcd(void * param)
{
    /* USER CODE BEGIN StartDefaultTask */
  static uint16_t line_cnt = 0;
  static uint16_t color = RED;
  /* Infinite loop */
  for(;;)
  {
    LCD_SetColors(color, WHITE);
    LCD_ClearLine(LINE(line_cnt - 1));
    LCD_print(line_cnt,"Hello FreeRTOS!%d",line_cnt);
    line_cnt++;
    switch(color)
    {
      case RED:
        color = GREEN;
        break;
      case GREEN:
        color = BLUE;
        break;
      case BLUE:
        color = RED;
        break;
      default:
        color = WHITE;
        break;
    } 
    vTaskDelay(pdMS_TO_TICKS(500));
    if(line_cnt >= 20)
    {
      LCD_ClearLine(LINE(line_cnt - 1));
      line_cnt = 0;
    }
  }
  /* USER CODE END StartDefaultTask */
}

void task_usart_bt(void * param)
{
  Task_EncoderDisp(param);
  while(1)
  {

  }
}

void task_Encoder_Start(void * param)
{
  Task_EncoderTest(param);
  while(1)
  {

  }
}

void task_Motor_Start(void * param)
{
  Task_Motor_2208_Test(param);
  while(1)
  {

  }
}

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LED_Blink */
  LED_BlinkHandle = osThreadNew(StartDefaultTask, NULL, &LED_Blink_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  // xTaskCreate(task_lcd, "Task_LCD", 256, NULL, 2, NULL);
  // xTaskCreate(task_lcd, "Task_LCD", 512, NULL, 3, NULL);
  // xTaskCreate(task_led, "Task_LED", 256, NULL, (osPriority_t) osPriorityNormal, NULL);
  // xTaskCreate(task_usart_bt, "Task_USART_BT", 256, NULL, (osPriority_t) osPriorityNormal, &USART_BT_Handle);
  // xTaskCreate(task_Encoder_Start, "Task_Encoder_Test", 128, NULL, (osPriority_t) osPriorityNormal, &Encoder_Handle);
  // xTaskCreate(task_Motor_Start, "Task_Motor_2208_Test", 256, NULL, (osPriority_t) osPriorityNormal7, &Motor_Handle);
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the LED_Blink thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  static uint16_t line_cnt = 0;
  static uint16_t color = RED;
  /* Infinite loop */
  for(;;)
  {
    LCD_SetColors(color, WHITE);
    LCD_ClearLine(LINE(line_cnt - 1));
    LCD_print(line_cnt,"Hello FreeRTOS!%d",line_cnt);
    line_cnt++;
    switch(color)
    {
      case RED:
        color = GREEN;
        break;
      case GREEN:
        color = BLUE;
        break;
      case BLUE:
        color = RED;
        break;
      default:
        color = WHITE;
        break;
    } 
    vTaskDelay(pdMS_TO_TICKS(500));
    if(line_cnt >= 20)
    {
      LCD_ClearLine(LINE(line_cnt - 1));
      line_cnt = 0;
    }
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

