#ifndef __USART_DEBUG_H
#define _USART_DUBUG_H

#define MAX_USART_DEBUG_FLOAT_INPUT_NUM 4
#define MAX_USART_DEBUG_INT_INPUT_NUM 2
#define MAX_USART_DEBUG_OUTPUT_NUM 12
#define MAX_USART_DEBUG_COMMAND_NAME_LENGTH 100

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "drv_uart.h"
#include "my_usart.h"

typedef enum
{

}Enum_Plot_Mode;

typedef struct 
{
    uint8_t PlotData_Num;


}Strcut_Plot_Mode;


class Class_Usart_Debug
{
    public:
        void Init(Struct_UART_Manage_Object * __UART_Manage_Object, char** UART_RX_Command_List,
                  uint8_t __UART_RX_Command_Num, uint8_t __Plot_Mode_Num, char __command_identifier = '#',
                  uint8_t __Max_Input_Num = 4);


        uint8_t Get_Command_Index();

        void Judge_Command_Data(uint8_t index);

        void Set_Data(uint8_t Number, ...);
        void UART_CmdRxCpltCallback(uint8_t *Rx_Data);
    protected:

        //绑定的UART
        Struct_UART_Manage_Object *UART_Manage_Object;
        //接收指令字典的数量
        uint8_t UART_RX_Command_Num;
        //接收指令字典列表指针
        char ** UART_RX_Command_List;

        int8_t UART_RX_Command_Index;

        float float_value[MAX_USART_DEBUG_FLOAT_INPUT_NUM];
        int int_value[MAX_USART_DEBUG_INT_INPUT_NUM];
        
        char command_identifier;

        uint8_t Max_Input_Num = 4;

        uint8_t Plot_Mode_Num;
        Strcut_Plot_Mode Plot_Mode_List[3];

        void * pTx_Data[MAX_USART_DEBUG_OUTPUT_NUM];
        uint8_t pTx_Data_Number;
        

        uint8_t Judge_Command_Name();
        void Judge_Command_Value(int flag);
        void Out_Put();


};


#endif