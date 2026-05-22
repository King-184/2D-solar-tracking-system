/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V2.3
//修改日期:2019/12/17
//程序功能：封装和简化通用的操作
//版本：V1.0	完成基本功能
//版本：V2.0 2019/12/09 修改起始字节和校验和计算方式
//版本：V2.1 2019/12/12 增加地址字节配置选项
//版本：V2.2 2019/12/14 增加自动校验选项
						增加自动回复选项
						固定串口号（宏定义）
//版本：V2.3 2019/12/17 修复一个bug
***********************************************/
#ifndef __SERIAL_MSG_H
#define __SERIAL_MSG_H
#include "my_include.h"

/************************************
自定义协议：
定义	起始字节		数据长度	命令		数据内容		校验和	
标识	smsb slsb		length		cmd	data	data			check	
示例	0x2a 0xd5		0x02		0x04		0x50 0x78		0x31	
**************************************
为了确定信息的起始与结束，定义了起始字节和结束字节，在程序检测到起始字节时则认为是信息的开始，
在此之前接收到的数据如果是完整的信息则交给其他程序处理，如果是不完整的或者是无意义的数据则将其丢弃。
为了使信息容量较大、传输效率较高，本协议使用数据的具体数值表示数据传输的内容，不使用数据所代表的ASCII码。
如数据0x31表示的是数字49，而不是ASCII码字符1。
理论上在完整的数据中，0x00-0xff是都有可能出现的，所以我们将起始字节和结束字节设置为固定的2个字节，
2个字节的代数和为0xff，这样就基本上可以避免在消息中间出现起始字节或结束字节而被误认为是数据的开始或结束（理论上仍然有可能出现，但概率非常低）。
起始字节：2个字节，0x2a、0xd5，0x2a的ASCII字符为'*'，0x2a+0xd5=0xff
数据长度：1个字节，表示数据内容的长度（数据内容的长度是可变的）
命令字节：1个字节，表示当前信息是哪一种信息
数据内容：0~255个字节，长度是可变的，最大255个字节
校验和：1个字节，用于校验接收到的信息是否正确，计算方法是
check = (length+cmd+data[0]+data[1]+...+data[n])&0xff
如上表中：0x31 = (0x02+0x04+0x50+0x78)&0xff

***************************************/
#define NUM_MSG_MAX 			8 //
#define LEN_MSG_SERIAL_MAX 			28	//信息容量
#define MSG_START_MSB	 		0x2a	//串口发送数据起始高字节
#define MSG_START_LSB	 		0xd5	//串口发送数据起始低字节
#define AUTO_CHECK				1//是否使能自动校验数据
#define AUTO_RESPONSE			1//是否使能自动回复
#define ENABLE_ADDR				1//是否使能地址选项
#define USART_MESSAGE			USART1
/*****************************************************************************************/
#define STATE_NONE				0x00
#define STATE_LENGTH			0x01
#if ENABLE_ADDR>0
#define STATE_ADDR_REC_1		0x02
#define STATE_ADDR_REC_2		0x03
#define STATE_ADDR_REC_3		0x04
#define STATE_ADDR_REC_4		0x05
#define STATE_ADDR_SEND_1		0x06
#define STATE_ADDR_SEND_2		0x07
#define STATE_ADDR_SEND_3		0x08
#define STATE_ADDR_SEND_4		0x09
#endif
#define STATE_CMD				0x0A
#define STATE_DATA				0x0B
#define STATE_CHECK				0x0C
/****************************************************************************************/
#define ADDR_MYSELF				0x12345678
#define ADDR_BROADCAST			0xFFFFFFFF
//						  
#define CMD_RESPONSE				0x80//应答指令，与接收到的命令或运算后返回给信息发送者
#define CMD_DEBUG 				0x00//发送字符串

typedef struct 
{		
	u8 length;
#if ENABLE_ADDR>0
	union_u32 addr_rec;
	union_u32 addr_send;
#endif
	u8 cmd;
	u8 payload[LEN_MSG_SERIAL_MAX];
	u8 checkSum;
	boolean enable;
}_serial_msg_obj;	

extern _serial_msg_obj* serialMsg_rec;	//结构体
void SendMessage(_serial_msg_obj msgDat);
#if ENABLE_ADDR>0
void SendMessageTo(u32 addr_rec, _serial_msg_obj msgDat);
void SendMessage_Cmd(u32 addr_rec, u8 cmd);
#else
void SendMessage_Cmd(u8 cmd);
#endif	 	
boolean	MessageIsCorrect(_serial_msg_obj msgDat);
u8 CalculateCheckSum(_serial_msg_obj msgDat);
void MessageStateMachine(u8 msgByte);
void ProcessMessage(void);
void SetGetMessageHook(void ( *GetMessage)( void ));
#endif
