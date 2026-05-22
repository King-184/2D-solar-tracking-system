/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ 51开发板
//
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/02/24
//程序功能：封装和简化通用的操作
//版本：2020/02/17	V1.0	完成基本功能
//版本：2020/02/24	V1.1	优化部分功能
***********************************************/
#include "my_uart_msg.h"

_uart_msg_obj uartMsg[COUNT_MSG_MAX];	//结构体 
_uart_msg_obj* uartMsgRec;	//结构体
void ( *GetUartMessageHook )( void ) = NULL;//获取到消息的回调（钩子）函数


#if USE_BUF_UART_MESSAGE>0
static u8 recBuffer[128];
static u8 index_save_buf = 0;
static u8 index_read_buf = 0;
void SaveToUartMessageBuffer(u8 dat)
{
	recBuffer[index_save_buf]=dat;
	index_save_buf++;
	index_save_buf%=ArrayCount(recBuffer);
}
#endif
void SendUartMessage(USART_TypeDef* USARTx, _uart_msg_obj msgDat)
{
	u8 i = 0;
	USARTSendByte(USARTx, '*');
	for(i=0;i<msgDat.length;i++)
	{
		USARTSendByte(USARTx, msgDat.payload[i]);
	}
	USARTSendByte(USARTx, '#');
}
void UartStateMachine(u8 msgByte)
{
	static u8 index_msg_read=0;
	static u8 index_dat=0;
	if (msgByte == MSG_START)
	{
		index_dat=1;
		return;
	}
	if(msgByte == MSG_END)
	{
		uartMsg[index_msg_read].payload[index_dat-1] = 0;
		uartMsg[index_msg_read].length = index_dat-1;
		uartMsg[index_msg_read].enable = TRUE;
		index_dat = 0;
		index_msg_read++;//指向下一条信息
		index_msg_read %= COUNT_MSG_MAX;//如果是最后一条则跳转到第一条，防止数组越界
	}
	else if(index_dat>0 && index_dat<=LEN_MSG_UART_MAX)
	{
		uartMsg[index_msg_read].payload[index_dat-1] = msgByte;
		index_dat++;
	}
}

void ProcessUartMessage(void)
{
	static u8 index_msg_process=0;
	u8 i;
	
#if USE_BUF_UART_MESSAGE>0
	while(index_read_buf!=index_save_buf)
	{
		UartStateMachine(recBuffer[index_read_buf]);
		index_read_buf++;
		index_read_buf%=ArrayCount(recBuffer);
	}
#endif
	if(uartMsg[index_msg_process].enable == TRUE)//如果接收到了一个完整的数据
	{   
		uartMsgRec = &uartMsg[index_msg_process];
		OnGetUartMessage();
		uartMsg[index_msg_process].enable = FALSE;
		for(i=0;i<LEN_MSG_UART_MAX;i++)
		{
			uartMsg[index_msg_process].payload[i]=0;
		}
		//指向下一条信息
		index_msg_process++;
		index_msg_process %= COUNT_MSG_MAX;
	}
}

void __weak OnGetUartMessage(void)
{
	
}

