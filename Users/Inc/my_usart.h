#ifndef __MY_USART_H__
#define __MY_USART_H__

#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256
#define MAX_CMD_LENGTH 64

typedef enum{
    AT_MODE_OFF = 0,
    AT_MODE_ON
}BT_mode;

// 命令处理回调函数类型
typedef void (*CommandHandler_t)(const char* command);

void My_USART_Init(void);
void My_USART_SetCommandHandler(CommandHandler_t handler);
void usart_printf(UART_HandleTypeDef *huart, char * format, ...);
void USART_SendString(const char * string);
void Usart_SendString(uint8_t *str);
// void HandleCommand(const char* command);
void ATMode_Switch(uint8_t state, BT_mode * AT_Mode_ptr);

#ifdef __cplusplus
}
#endif

#endif /*__MY_USART_H__*/

