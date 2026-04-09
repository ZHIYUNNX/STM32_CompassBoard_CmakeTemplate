#include "dvc_servo.h"
#include "string.h"
#include "stdarg.h"

void Class_Servo_Bus::Init(Struct_UART_Manage_Object * __UART_Manage_Object)
{
    UART_Manage_Object = __UART_Manage_Object;
}

void Class_Servo_Bus::Add_Servo(const uint8_t * __IDs, uint8_t __num)
{
    uint8_t id = 0;
    for(int i = 0; i < __num; i++)
    {
        id = *(__IDs + i);
        if(id > 0 && id < 10 && id_map[id] == 0)
        {
            servo[Num_Servo].ID = id;
            id_map[id] = Num_Servo + 1;//id_map的索引对应servo的id，里面元素所存的数据对应servo的索引
            Num_Servo++;
        }
        
    }
}

/*********************************************************************************
 * Function:  CMD_MoveServos
 * Description： 控制多个舵机转动
 * Parameters:   Num:舵机个数,Time:转动时间,...:舵机ID,转动角，舵机ID,转动角度 如此类推
 * Return:       无返回
 * Others:
 **********************************************************************************/
void Class_Servo_Bus::CMD_MoveServos(uint8_t Num, uint16_t Time, ...)
{
	uint8_t index = 7;
	uint8_t i = 0;
	uint16_t temp;
	va_list arg_ptr;  //

	va_start(arg_ptr, Time); //取得可变参数首地址
	if (Num < 1 || Num > 32) {
		return;               //舵机数不能为零和大与32，时间不能小于0
	}
	Servo_TXCommand_Buffer[0] = Servo_TXCommand_Buffer[1] = FRAME_HEADER;//填充帧头
	Servo_TXCommand_Buffer[2] = Num * 3 + 5;                //数据长度 = 要控制舵机数 * 3 + 5
	Servo_TXCommand_Buffer[3] = CMD_SERVO_MOVE;             //舵机移动指令
	Servo_TXCommand_Buffer[4] = Num;                        //要控制舵机数
	Servo_TXCommand_Buffer[5] = GET_LOW_BYTE(Time);         //取得时间的低八位
	Servo_TXCommand_Buffer[6] = GET_HIGH_BYTE(Time);        //取得时间的高八位

	for (i = 0; i < Num; i++) {//从可变参数中取得并循环填充舵机ID和对应目标位置
		temp = va_arg(arg_ptr, int);//可参数中取得舵机ID
		Servo_TXCommand_Buffer[index++] = GET_LOW_BYTE(((uint16_t)temp));
		temp = va_arg(arg_ptr, int);  //可变参数中取得对应目标位置
		Servo_TXCommand_Buffer[index++] = GET_LOW_BYTE(((uint16_t)temp)); //填充目标位置低八位
		Servo_TXCommand_Buffer[index++] = GET_HIGH_BYTE(temp);//填充目标位置高八位
	}

	va_end(arg_ptr);  //置空arg_ptr

	HAL_UART_Transmit(UART_Manage_Object->UART_Handler, Servo_TXCommand_Buffer, Servo_TXCommand_Buffer[2] + 2, HAL_MAX_DELAY);    //发送
}

/*********************************************************************************
 * Function:  CMD_RunActionGroup
 * Description： 运行指定动作组
 * Parameters:   NumOfAction:动作组序号, Times:执行次数
 * Return:       无返回
 * Others:       Times = 0 时无限循环
 **********************************************************************************/
void Class_Servo_Bus::CMD_RunActionGroup(uint8_t numOfAction, uint16_t Times)
{
	Servo_TXCommand_Buffer[0] = Servo_TXCommand_Buffer[1] = FRAME_HEADER;  //填充帧头
	Servo_TXCommand_Buffer[2] = 5;                      //数据长度，数据帧除帧头部分数据字节数，此命令固定为5
	Servo_TXCommand_Buffer[3] = CMD_ACTION_GROUP_RUN;   //填充运行动作组命令
	Servo_TXCommand_Buffer[4] = numOfAction;            //填充要运行的动作组号
	Servo_TXCommand_Buffer[5] = GET_LOW_BYTE(Times);    //取得要运行次数的低八位
	Servo_TXCommand_Buffer[6] = GET_HIGH_BYTE(Times);   //取得要运行次数的高八位

	HAL_UART_Transmit(UART_Manage_Object->UART_Handler, Servo_TXCommand_Buffer, 7, HAL_MAX_DELAY);            //发送
}

/*********************************************************************************
 * Function:  CMD_StopActionGroup
 * Description： 停止动作组运行
 * Parameters:   Speed: 目标速度
 * Return:       无返回
 * Others:
 **********************************************************************************/
void Class_Servo_Bus::CMD_StopActionGroup(void)
{
	Servo_TXCommand_Buffer[0] = FRAME_HEADER;     //填充帧头
	Servo_TXCommand_Buffer[1] = FRAME_HEADER;
	Servo_TXCommand_Buffer[2] = 2;                //数据长度，数据帧除帧头部分数据字节数，此命令固定为2
	Servo_TXCommand_Buffer[3] = CMD_ACTION_GROUP_STOP;   //填充停止运行动作组命令

	HAL_UART_Transmit(UART_Manage_Object->UART_Handler, Servo_TXCommand_Buffer, 4, HAL_MAX_DELAY);      //发送
}

/*********************************************************************************
 * Function:  CMD_SetActionGroupSpeed
 * Description： 设定指定动作组的运行速度
 * Parameters:   NumOfAction: 动作组序号 , Speed:目标速度
 * Return:       无返回
 * Others:
 **********************************************************************************/
void Class_Servo_Bus::CMD_SetActionGroupSpeed(uint8_t numOfAction, uint16_t Speed)
{
	Servo_TXCommand_Buffer[0] = Servo_TXCommand_Buffer[1] = FRAME_HEADER;   //填充帧头
	Servo_TXCommand_Buffer[2] = 5;                       //数据长度，数据帧除帧头部分数据字节数，此命令固定为5
	Servo_TXCommand_Buffer[3] = CMD_ACTION_GROUP_SPEED;  //填充设置动作组速度命令
	Servo_TXCommand_Buffer[4] = numOfAction;             //填充要设置的动作组号
	Servo_TXCommand_Buffer[5] = GET_LOW_BYTE(Speed);     //获得目标速度的低八位
	Servo_TXCommand_Buffer[6] = GET_HIGH_BYTE(Speed);    //获得目标熟读的高八位

	HAL_UART_Transmit(UART_Manage_Object->UART_Handler, Servo_TXCommand_Buffer, 7, HAL_MAX_DELAY);             //发送
}

/*********************************************************************************
 * Function:  CMD_MultServoPosRead
 * Description： 读取多个舵机的位置
 * Parameters:   Num: 舵机数量, ...: 舵机ID列表
 * Return:       无返回
 * Others:
 **********************************************************************************/
void Class_Servo_Bus::CMD_MultServoPosRead(uint8_t Num, ...)
{
    uint8_t id_temp;
    va_list arg_ptr;
    va_start(arg_ptr, Num);

	Servo_TXCommand_Buffer[0] = FRAME_HEADER;     //填充帧头
	Servo_TXCommand_Buffer[1] = FRAME_HEADER;
	Servo_TXCommand_Buffer[2] = Num * 3 + 3;                //数据长度读取电机的个数 * 3 + 3
	Servo_TXCommand_Buffer[3] = CMD_MULT_SERVO_POS_READ;   //填充读取舵机位置命令

    for (int i = 0; i < Num; i++) {//从可变参数中取得并循环填充舵机ID和对应目标位置
		id_temp = va_arg(arg_ptr, int);//可参数中取得舵机ID
		Servo_TXCommand_Buffer[i + 4] = id_temp;
	}

	va_end(arg_ptr);  //置空arg_ptr

	HAL_UART_Transmit(UART_Manage_Object->UART_Handler, Servo_TXCommand_Buffer, Num * 3 + 3, HAL_MAX_DELAY);      //发送
}

void Class_Servo_Bus::CMD_GetBatteryVoltage()
{
    Servo_TXCommand_Buffer[0] = FRAME_HEADER;     //填充帧头
	Servo_TXCommand_Buffer[1] = FRAME_HEADER;
	Servo_TXCommand_Buffer[2] = 2;                //数据长度，数据帧除帧头部分数据字节数，此命令固定为2
	Servo_TXCommand_Buffer[3] = CMD_GET_BATTERY_VOLTAGE;   //填充读取电池电压命令

	HAL_UART_Transmit(UART_Manage_Object->UART_Handler, Servo_TXCommand_Buffer, 4, HAL_MAX_DELAY);      //发送
}

void Class_Servo_Bus::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
{
    uint8_t Res;
	static bool isGotFrameHeader = false;
	static uint8_t frameHeaderCount = 0;
	static uint8_t dataLength = 2;
	static uint8_t dataCount = 0;


    for(int i = 0; i < Length; i++)
    {
        Res = UART_Manage_Object->Rx_Buffer[i];//(USART1->DR);	//取出接收寄存器数据
        if (!isGotFrameHeader) {  //判断帧头
            if (Res == FRAME_HEADER) 
            {
                frameHeaderCount++;
                if (frameHeaderCount == 2) 
                {//连续收到两个0x55，表示有数据包达到
                    frameHeaderCount = 0;
                    isGotFrameHeader = true;
                    dataCount = 1;
                }
            } 
            else 
            {
                isGotFrameHeader = false;
                dataCount = 0;
                frameHeaderCount = 0;
            }
        }
        if (isGotFrameHeader) 
        { //接收接收数据部分，接收到第二个0x55后也一样进入到接收数据部分
            Servo_RX_Buffer[dataCount] = Res;
            if (dataCount == 2) //第二个0x55后的第一个字节为数据长度
            {
                dataLength = Servo_RX_Buffer[dataCount];
                // if (dataLength < 2 || dataLength > 8) 
                // {
                //     dataLength = 2;
                //     isGotFrameHeader = false;
                // }
            }
            if (dataCount == 3) //第二个0x55后的第二个字节为指令
            {
                RX_Command = Servo_RX_Buffer[dataCount];//将指令记录在类中，在外部的回调函数中获取并判断做出相应操作
            }
            
            dataCount++;
            if (dataCount == dataLength + 2) 
            {//接收到最后一个数据，将串口缓冲区中的数据拷贝到舵机数据接收缓冲区，并重置枕头标志位
                if (isUartRxCplt == false) 
                {
                    isUartRxCplt = true;
                    // memcpy(LobotRxBuf, Servo_RX_Buffer, dataCount);
                }
                isGotFrameHeader = false;
                RXCommand_Judge();
            }
        }
    }
    

}

void Class_Servo_Bus::RXCommand_Judge()
{
    if(isUartRxCplt == true)
    {
        isUartRxCplt = false;
        switch(RX_Command)
        {
            case(CMD_GET_BATTERY_VOLTAGE):
            {
                BatteryVolt = (((uint16_t)(Servo_RX_Buffer[5])) << 8) | (Servo_RX_Buffer[4]);
            }
            break;
            case(CMD_MULT_SERVO_POS_READ):
            {
                uint8_t num_read_servo = Servo_RX_Buffer[4];
                for(int i = 0; i < num_read_servo; i++)
                {
                    uint8_t Get_ID = Servo_RX_Buffer[3*i + 5];
                    uint8_t servo_index_temp = id_map[Get_ID] - 1;
                    if(servo_index_temp != 0)//判断舵机索引映射表中是否被初始化
                    {
                        servo[servo_index_temp].Position = (((uint16_t)(Servo_RX_Buffer[3*i + 7])) << 8) | (Servo_RX_Buffer[3*i + 6]);
                    }
                    
                    
                }
            }
            break;
            default:
                break;
            
        }
    }
    

}