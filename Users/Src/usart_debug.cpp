#include "usart_debug.h"

/**
 * @brief 添加被发送的数据
 *
 * @param Number 添加的数据数量
 * @param ... 每个数据的指针, 至多12个
 */
void Class_Usart_Debug::Set_Data(uint8_t Number, ...)
{
    va_list data_ptr;
    va_start(data_ptr, Number);
    for (int i = 0; i < Number; i++)
    {
        pTx_Data[i] = (void *)va_arg(data_ptr, int);
    }
    va_end(data_ptr);
    pTx_Data_Number = Number;
}

uint8_t Class_Usart_Debug::Judge_Command_Name()
{
    char tmp_command_name[MAX_USART_DEBUG_COMMAND_NAME_LENGTH];
    uint8_t flag;

    

    for(flag = 0; UART_Manage_Object->Rx_Buffer[flag] != command_identifier && UART_Manage_Object->Rx_Buffer[flag] != 0; flag++)
    {
        tmp_command_name[flag] = UART_Manage_Object->Rx_Buffer[flag];
    }
    tmp_command_name[flag] = '\0';

    for(int i = 0; i < UART_RX_Command_Num; i++)
    {
        if(strcmp(tmp_command_name, (char *)((int)UART_RX_Command_List + MAX_USART_DEBUG_COMMAND_NAME_LENGTH * i)) == 0)
        {
            UART_RX_Command_Index = i; 
            return (flag + 1);
        }
    }

    UART_RX_Command_Index = -1;
    return (flag + 1);
    
}

void Class_Usart_Debug::Judge_Command_Data(uint8_t index)
{
    uint8_t flag = 0;
    int8_t input_num = 0;
    char format_buffer[30];
    void *data_ptr[5];
    flag = Judge_Command_Name();

    input_num = UART_Manage_Object->Rx_Buffer[flag] - '0';

    if(input_num < 0 || input_num > 4)
    {
        usart_printf(UART_Manage_Object->UART_Handler,(char*)"Error: Too much or too less input(0~4)\n");
        UART_RX_Command_Index = -1;
        return;
    }

    for(uint8_t i = 1; i <= input_num; i++)
    {
        uint8_t float_num = 0;
        uint8_t int_num = 0;
        if(UART_Manage_Object->Rx_Buffer[flag + i] == 'f')
        {
            memcpy(format_buffer + (i - 1) * 3, "%f,", 3);
            data_ptr[i-1] = (float *)(&float_value[float_num]);
            float_num++;
        }
        else if (UART_Manage_Object->Rx_Buffer[flag + i] == 'd')
        {
            memcpy(format_buffer + (i - 1) * 3, "%d,", 3);
            data_ptr[i-1] = (int *)(&int_value[int_num]);
            int_num++;
        }
        else
        {
            UART_RX_Command_Index = -1;
            return;
        }
    }

    format_buffer[(input_num*3)-1] = '\0';

    sscanf((char *)UART_Manage_Object->Rx_Buffer + flag + 1, format_buffer, data_ptr[0], data_ptr[1], data_ptr[2], data_ptr[3]);

}

void Class_Usart_Debug::UART_CmdRxCpltCallback(uint8_t *Rx_Data)
{
    //解析接收到的指令
}

void Class_Usart_Debug::Out_Put()
{
    
}