/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//
//作者：tianqingyong
//版本：V2.3
//修改日期:2020/06/03
//程序功能：封装和简化USART的操作
***********************************************/
#ifndef __USART_H
#define __USART_H
#include "my_include.h"

#define EN_USART2  1

#define EN_USART1 				1			//使能串口1
#define USART1_IT_RXNE		1
#define USART1_IT_TXE		0
#define USART1_IT_IDLE		0

#if USART1_IT_IDLE>0
#define RX_Buff_LEN			256
extern u8 USART1_RX_Buff[RX_Buff_LEN];	//发送数据缓冲区
extern u8 readBuffer[RX_Buff_LEN];
extern bool readMsgFlag;
#endif

void SetPrintfUSART(USART_TypeDef* USARTx);
USART_TypeDef* GetPrintfUSART(void);
void USARTSendByte(USART_TypeDef* USARTx, uint8_t Data);
void USARTSendBytes(USART_TypeDef* USARTx, const uint8_t *Data, uint16_t length);
void USARTSendString(USART_TypeDef* USARTx, const char *str);
void USARTx_Init(USART_TypeDef* USARTx, u32 baud);
#endif


