#include "my_usart.h"

extern GPIO_PinState cur_state;


uint8_t dma_rx_buffer[RX_BUFFER_SIZE];
uint8_t dma_tx_buffer[TX_BUFFER_SIZE];

uint8_t usart_event_flag = 0;

//串口命令相关变量


char cmd_buffer[MAX_CMD_LENGTH];//串口接收缓冲
char get_ch;
static uint16_t cmd_index = 0;//命令字符数索引
CommandHandler_t command_handler = NULL;

char get_ch2;
// static char at_cmd_buffer[MAX_CMD_LENGTH];
static char AT_Echo_buffer[MAX_CMD_LENGTH];//AT命令回显缓冲
BT_mode AT_Mode = AT_MODE_OFF;
static uint16_t at_cmd_index = 0;//命令字符数索引

//microlib printf重新定向

// int fputc(int ch, FILE *f)
// {

// 	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 15);
// 	return (ch);
// }


// int fgetc(FILE *f)
// {		
// 	int ch;
// 	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 15);	
// 	return (ch);
// }


#ifdef __GNUC__
// 重定向printf到串口
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

// 如果使用scanf，还需要实现__io_getchar
int __io_getchar(void) {
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
    return ch;
}
#endif

void ATMode_Switch(uint8_t state, BT_mode * AT_Mode_ptr)
{

    *AT_Mode_ptr = state;
    if(state)
    {
        HAL_GPIO_WritePin(BT_KEY_GPIO_Port, BT_KEY_Pin, GPIO_PIN_SET);
        cur_state = HAL_GPIO_ReadPin(BT_KEY_GPIO_Port, BT_KEY_Pin);
        at_cmd_index = 0;
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&AT_Echo_buffer[at_cmd_index], 1);
    }
    else 
    {
        HAL_GPIO_WritePin(BT_KEY_GPIO_Port, BT_KEY_Pin, GPIO_PIN_RESET);
        cur_state = HAL_GPIO_ReadPin(BT_KEY_GPIO_Port, BT_KEY_Pin);
        cmd_index = 0;
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&cmd_buffer[cmd_index], 1);
    }
}

void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart1,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}

//初始化串口中断接收
void My_USART_Init(void)
{
    cmd_index = 0;
    // 启动串口中断接收（每次接收1个字节）
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&cmd_buffer[cmd_index], 1);
    printf("UART Command System Ready!\r\n");
}

//设置命令处理回调函数
void My_USART_SetCommandHandler(CommandHandler_t handler)
{
    command_handler = handler;
}

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == huart1.Instance)
//     {
//         switch(AT_Mode)
//         {
//             case AT_MODE_OFF:
//                 // 数据已经被HAL库写入cmd_buffer[cmd_index]
//                 uint8_t received_char = cmd_buffer[cmd_index];
//                 // 检查是否收到命令结束符（回车或换行），在串口助手输入命令时记得添加\n再输出
//                 if (received_char == '\r' || received_char == '\n') 
//                 {
//                     if (cmd_index > 0)  // 确保不是空命令
//                     {
//                         // 添加字符串结束符
//                         cmd_buffer[cmd_index] = '\0';
//                         // 调用命令处理函数
//                         if (command_handler != NULL) 
//                         {
//                             command_handler(cmd_buffer);
//                         }
                        
//                     }
//                     cmd_index = 0;  // 重置缓冲区索引
//                 }
//                 else 
//                 {
//                     // 普通字符，继续接收
//                     cmd_index++;
                    
//                     // 防止缓冲区溢出
//                     if (cmd_index >= MAX_CMD_LENGTH - 1) 
//                     {
//                         printf("Command buffer overflow, resetting...\r\n");
//                         cmd_index = 0;  // 缓冲区满，从头开始
//                     }
//                 }
//                 // 重新启动中断接收（指向新的位置）
//                 if(AT_Mode == AT_MODE_OFF)
//                 {
//                     HAL_UART_Receive_IT(&huart1, (uint8_t*)&cmd_buffer[cmd_index], 1);
//                 }
                
//                 break;
            
//             case AT_MODE_ON:
                
//                 uint8_t at_char = AT_Echo_buffer[at_cmd_index];
//                 if (at_char == '\0') 
//                 {
//                     if(at_cmd_index > 0)
//                     {
//                         // 添加字符串结束符
                        
//                         AT_Echo_buffer[at_cmd_index] = '\0';
//                         command_handler(AT_Echo_buffer);
//                         // 调用命令处理函数
                        
//                     }
//                     else if(at_cmd_index == 0)
//                     {
//                         ATMode_Switch(0, &AT_Mode);
//                         printf("No AT Echo received\r\n");
//                     }
//                     at_cmd_index = 0;  //检测到回显指令字符串结尾重置索引
//                 }
//                 else 
//                 {
//                     // 普通字符，继续接收
//                     at_cmd_index++;
//                     // 防止缓冲区溢出
//                     if (at_cmd_index >= MAX_CMD_LENGTH - 1) 
//                     {
//                         ATMode_Switch(0, &AT_Mode);
//                         printf("AT Echo buffer overflow\r\n");
//                         ATMode_Switch(1, &AT_Mode);
//                         at_cmd_index = 0;  // 缓冲区满，从头开始
//                     }
//                 }
//                 // 重新启动中断接收（指向新的位置）
//                 if(AT_Mode == AT_MODE_ON)
//                 {
//                     HAL_UART_Receive_IT(&huart1, (uint8_t*)&AT_Echo_buffer[at_cmd_index], 1);
//                 }
                
//                 break;
//         }

//     }

//     if (huart->Instance == huart1.Instance)
//     {
//         HAL_UART_Receive_IT(&huart1, (uint8_t *)&get_ch, 1);
//         printf("%c", get_ch);
//     }

// //     if (huart->Instance == huart1.Instance)
// //     {
// //         HAL_UART_Receive_IT(&huart1, (uint8_t *)&get_ch2, 1);
// //         printf("%c", get_ch2);
// //     }
// }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == huart1.Instance) 
    {
        // 数据已经被HAL库写入cmd_buffer[cmd_index]
        uint8_t received_char = cmd_buffer[cmd_index];
        
        // 检查是否收到命令结束符（回车或换行），在串口助手输入命令时记得添加\n再输出
        if (received_char == '\r' || received_char == '\n') 
        {
            if (cmd_index > 0)  // 确保不是空命令
            {
                cmd_buffer[cmd_index] = '\0';  // 添加字符串结束符
                
                // 调用命令处理函数
                if (command_handler != NULL) 
                {
                    command_handler(cmd_buffer);
                }
            }
            cmd_index = 0;  // 重置缓冲区索引
        }
        else 
        {
            // 普通字符，继续接收
            cmd_index++;
            
            // 防止缓冲区溢出
            if (cmd_index >= MAX_CMD_LENGTH - 1) 
            {
                printf("Command buffer overflow, resetting...\r\n");
                cmd_index = 0;  // 缓冲区满，从头开始
            }
        }
        
        // 重新启动中断接收（指向新的位置）
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&cmd_buffer[cmd_index], 1);
    }
}

// void HandleCommand(const char* command)
// {
//     printf("CMD: %s\r\n", command);
//     if(AT_Mode == AT_MODE_OFF)
//     {
//         if(strncmp(command, "AT", 2) == 0)
//         {
//             ATMode_Switch(1, &AT_Mode);
//             printf("%s",command);
//             HAL_UART_Receive_IT(&huart1, (uint8_t*)&AT_Echo_buffer[at_cmd_index], 1);
//         }
//     }
//     else
//     {
//         ATMode_Switch(0, &AT_Mode);
//         printf("%s",AT_Echo_buffer);
//     }
    
// }

void usart_printf(UART_HandleTypeDef *huart, char * format, ...)
{
    char buffer[128];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    buffer[127] = '\0';
    HAL_UART_Transmit(huart, (uint8_t*)buffer, len, HAL_MAX_DELAY);
}

// void USART_SendString(const char * string)
// {
//   HAL_UART_Transmit(&huart1, string, strlen(string), HAL_MAX_DELAY);
// }

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
// {
//     if (huart->Instance == USART2)
//     {
//       HAL_UARTEx_ReceiveToIdle_DMA(huart, dma_rx_buffer, RX_BUFFER_SIZE);
//     }

// }
