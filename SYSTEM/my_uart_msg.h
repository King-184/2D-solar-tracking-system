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
#ifndef __MY_UART_MSG_H
#define __MY_UART_MSG_H
#include "my_include.h"

/************************************
自定义协议：
定义	起始字节	数据内容	结束字节
标识	start		data		end
示例	0x2a		0x50 0x78	0x23
***************************************/
#define COUNT_MSG_MAX 							4 //
#define LEN_MSG_UART_MAX						32	//信息容量
#define MSG_START	 							0x2a	//串口发送数据起始高字节
#define MSG_END	 								0x23	//串口发送数据起始低字节
#define USE_BUF_UART_MESSAGE					1
/****************************************************************************************/

#if USE_BUF_UART_MESSAGE>0
#define My_UARTMessage_Receive					SaveToUartMessageBuffer
#else
#define My_UARTMessage_Receive					UartStateMachine
#endif

typedef struct 
{		
	bool enable:1;
	u16 length:15;
	char payload[LEN_MSG_UART_MAX];
}_uart_msg_obj;

extern _uart_msg_obj* uartMsgRec;	//结构体

#if USE_BUF_UART_MESSAGE>0
void SaveToUartMessageBuffer(u8 dat);
#endif
void SendUartMessage(USART_TypeDef* USARTx, _uart_msg_obj msgDat);
void UartStateMachine(u8 msgByte);
void ProcessUartMessage(void);
void OnGetUartMessage(void);
#endif
