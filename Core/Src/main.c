/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_bsp.h"
#include "led_bsp.h"
#include "beep_bsp.h"
#include "my_usart.h"
#include "drv_uart.h"
#include "dvc_motorTask.h"
#include "servo_bus_task.h"
#include "dvc_motor.h"
#include "dvc_servo.h"
#include "alg_filter.h"
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

/* USER CODE BEGIN PV */
int mycnt = 0;
GPIO_PinState cur_state;
uint8_t disp_mode = 4;

// extern UART_HandleTypeDef huart1;

extern char get_ch;
extern char get_ch2;
extern BT_mode AT_Mode;
extern uint8_t usart_event_flag;
extern CommandHandler_t command_handler;
extern char cmd_buffer[MAX_CMD_LENGTH];

extern Class_Motor_2208 Motor_0;
extern Class_Filter_Second_Order_Butterworth Filter_Test;
extern Class_Servo_Bus servo_bus;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

extern void Task_EncoderTest(void * param);
extern void Task_EncoderDisp(void * param);
extern void Task_Motor_2208_Test(void * param);


void HandleCommand(const char* command);

void UART_Servo_Call_Back(uint8_t *Buffer, uint16_t Length);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void UART_Servo_Call_Back(uint8_t *Buffer, uint16_t Length)
{
    servo_bus.UART_RxCpltCallback(Buffer, Length);//servo_bus在相应的舵机任务文件中定义dvc_servo_Task.c
    switch (servo_bus.Get_RX_Command())
    {
      case CMD_GET_BATTERY_VOLTAGE:
      {
        usart_printf(&huart1, "BATTERY_VOLTAGE: %f\n",(float)(servo_bus.Get_Voltage() / 1000.0));
      }
      break;
      default:
      {
        for(uint8_t i = 0; i < servo_bus.Get_NumServo(); i++)
        {
          usart_printf(&huart1, "servo %d position: %d\n",servo_bus.Get_Servos(i)->ID,servo_bus.Get_Servos(i)->Position);
        }
        
      }
        break;
    }
    usart_printf(&huart1, "ECHO:%s\n",Buffer);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init(); //初始化ILI9341液晶屏
  ILI9341_GramScan(6);
  HAL_Delay(500);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
  HAL_Delay(1000);

  // Task_EncoderTest_Init(NULL);
  // Task_Motor_2208_Init(NULL);
  // Task_EncoderDisp_Init();

  uint8_t servos_add[2] = {1,2};

  UART_Init(&huart3, UART_Servo_Call_Back, 100);
  servo_bus.Init(&UART3_Manage_Object);
  servo_bus.Add_Servo(servos_add,2);


  //My_Usart_Init：
  ATMode_Switch(0, &AT_Mode);
  My_USART_SetCommandHandler(HandleCommand);
  HAL_UART_Receive_IT(&huart1, (uint8_t*)&get_ch, 1);
  My_USART_Init();
  printf("UART Command System Ready!\r\n");
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// void HandleCommand(const char* command)
// {
//   static int val_int[3] = {0, 0, 0};
//   static float val_float[3] = {0.0f, 0.0f, 0.0f};
//   static char val_str[50] = {0};
//   usart_printf(&huart1, "CMD: %s\r\n", command);

//   if(sscanf(command, "SPD %f", &val_float[0]) == 1)
//   {
//     Motor_0.Set_Omega_Target(val_float[0]);
//     usart_printf(&huart1, "SPD SET TO %f\n", val_float[0]);
//   }
//   else if(sscanf(command, "TOR %f", &val_float[1]) == 1)
//   {
//     Motor_0.Positive_FOC.Set_U_ref(val_float[1], 0.0f);
//     usart_printf(&huart1, "TOR SET TO %f\n", val_float[1]);
//   }
//   else if(sscanf(command, "ANG %f", &val_float[0]) == 1)
//   {
//     Motor_0.Set_Angle_Target(Motor_0.Get_Angle_Now()+val_float[0]);
//     usart_printf(&huart1, "ANG SET TO %f\n", Motor_0.Get_Angle_Now()+val_float[0]);
//   }
//   else if(sscanf(command, "CL_TOR %f", &val_float[1]) == 1)
//   {
//     Motor_0.Set_Torque_Target(val_float[1]);
//     usart_printf(&huart1, "TOR SET TO %f\n", val_float[1]);
//   }
  
//   else if(sscanf(command, "OmgFIL %f %f", &val_float[1], &val_float[2]) == 2)
//   {
//     Motor_0.Encoder.Filter_Omega.Init(val_float[1], val_float[2]);
//     usart_printf(&huart1, "FIL SET TO %f, %f\n", val_float[1], val_float[2]);
//   }
//   else if(sscanf(command, "TorFIL %f %f", &val_float[1], &val_float[2]) == 2)
//   {
//     Motor_0.Filter_Torque.Init(val_float[1], val_float[2]);
//     usart_printf(&huart1, "FIL SET TO %f, %f\n", val_float[1], val_float[2]);
//   }

//   else if(sscanf(command, "TorPID %f %f %f", &val_float[1], &val_float[2], &val_float[3]) == 3)
//   {
//     Motor_0.PID_Torque.Set_K_P(val_float[1]);
//     Motor_0.PID_Torque.Set_K_I(val_float[2]);
//     Motor_0.PID_Torque.Set_K_D(val_float[3]);
//     usart_printf(&huart1, "PID SET TO %f, %f, %f\n", val_float[1], val_float[2], val_float[3]);
//   }
//   else if(sscanf(command, "OmgPID %f %f %f", &val_float[1], &val_float[2], &val_float[3]) == 3)
//   {
//     Motor_0.PID_Omega.Set_K_P(val_float[1]);
//     Motor_0.PID_Omega.Set_K_I(val_float[2]);
//     Motor_0.PID_Omega.Set_K_D(val_float[3]);
//     usart_printf(&huart1, "PID SET TO %f, %f, %f\n", val_float[1], val_float[2], val_float[3]);
//   }
//   else if(sscanf(command, "AngPID %f %f %f", &val_float[1], &val_float[2], &val_float[3]) == 3)
//   {
//     Motor_0.PID_Angle.Set_K_P(val_float[1]);
//     Motor_0.PID_Angle.Set_K_I(val_float[2]);
//     Motor_0.PID_Angle.Set_K_D(val_float[3]);
//     usart_printf(&huart1, "PID SET TO %f, %f, %f\n", val_float[1], val_float[2], val_float[3]);
//   }

//   else if(sscanf(command, "MOD %s",val_str) == 1)
//   {
//     if(strcmp(val_str, "TOR") == 0)
//     {
//       disp_mode = 1;
//       Motor_0.Set_Control_Mode(Torque_Closeloop_Mode);
//       usart_printf(&huart1, "MODE SET TO TORQUE CLOSELOOP\n");
//     }
//     else if(strcmp(val_str, "OP") == 0)
//     {
//       disp_mode = 1;
//       Motor_0.Set_Control_Mode(Torque_Openloop_Mode);
//       usart_printf(&huart1, "MODE SET TO TORQUE OPENLOOP\n");
//     }
//     else if(strcmp(val_str, "OMG") == 0)
//     {
//       disp_mode = 3;
//       Motor_0.Set_Control_Mode(Omega_Closeloop_Mode);
//       usart_printf(&huart1, "MODE SET TO OMEGA CLOSELOOP\n");
//     }
//     else if(strcmp(val_str, "ANG") == 0)
//     {
//       disp_mode = 2;
//       Motor_0.Set_Angle_Target(Motor_0.Get_Angle_Now());
//       Motor_0.Set_Control_Mode(Angle_Torque_Closeloop_Mode);
//       usart_printf(&huart1, "MODE SET TO ANGLE CLOSELOOP\n");
//     }
//     else if(strcmp(val_str, "TEST") == 0)
//     {
//       disp_mode = 4;
//       Motor_0.Set_Control_Mode(Torque_Openloop_Mode);
//       usart_printf(&huart1, "MODE SET TO TORQUE CLOSELOOP\n");
//     }
//     // else if(strcmp(val_str, "ANGOMG") == 0)
//     // {
//     //   disp_mode = 2;
//     //   Motor_0.Set_Angle_Target(Motor_0.Get_Angle_Now());
//     //   Motor_0.Set_Control_Mode(Angle_Omega_Closeloop_Mode);
//     //   usart_printf(&huart1, "MODE SET TO OMEGA CLOSELOOP\n");
//     // }
//     else
//     {
//       disp_mode = 0;
//       Motor_0.Set_Control_Mode(NOT_READY);
//       usart_printf(&huart1, "UNKOWN_MODE,STOP_DISP\n");
//     }
//   }
//   else
//   {
//     usart_printf(&huart1, "UNKOMN_CMD\n");
//   }
    
// }

void HandleCommand(const char* command)
{
  static int val_int[4] = {0, 0, 0};
  static float val_float[3] = {0.0f, 0.0f, 0.0f};
  static char val_str[50] = {0};
  usart_printf(&huart1, "CMD: %s\r\n", command);

  if(sscanf(command, "MOV %d %d", &val_int[0], &val_int[1]) == 2)
  {
    servo_bus.CMD_MoveServos(1, 500, val_int[0], val_int[1]);
    usart_printf(&huart1, "POS SET TO %d\n", val_int[1]);
  }
  if(strcmp(command, "VOL") == 0)
  {
    servo_bus.CMD_GetBatteryVoltage();
    // usart_printf(&huart1, "VOL %d\n", val_int[1]);
  }
  else if(sscanf(command, "READ %d", &val_int[0]) == 1)
  {
    servo_bus.CMD_MultServoPosRead(1,val_int[0]);
    for(uint8_t i = 0; i < servo_bus.Get_NumServo(); i++)
    {
      usart_printf(&huart1, "servo %d position: %d\n",servo_bus.Get_Servos(i)->ID,servo_bus.Get_Servos(i)->Position);
    }
  }
  else
  {
    usart_printf(&huart1, "UNKOMN_CMD\n");
  }
    
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
