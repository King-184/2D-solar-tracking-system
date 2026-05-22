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
#include "serial_msg.h"
 
_serial_msg_obj serialMsg[NUM_MSG_MAX];	//结构体
_serial_msg_obj* serialMsg_rec;	//结构体
void ( *GetMessageHook )( void ) = NULL;//获取到消息的回调（钩子）函数

void SendMessage(_serial_msg_obj msgDat){
	u8 i = 0;
	USARTSendByte(USART_MESSAGE,MSG_START_MSB);
	USARTSendByte(USART_MESSAGE,MSG_START_LSB);
	USARTSendByte(USART_MESSAGE,msgDat.length);
#if ENABLE_ADDR>0
	USARTSendByte(USART_MESSAGE,msgDat.addr_rec.Bytes.byte_3);
	USARTSendByte(USART_MESSAGE,msgDat.addr_rec.Bytes.byte_2);
	USARTSendByte(USART_MESSAGE,msgDat.addr_rec.Bytes.byte_1);
	USARTSendByte(USART_MESSAGE,msgDat.addr_rec.Bytes.byte_0);
	USARTSendByte(USART_MESSAGE,msgDat.addr_send.Bytes.byte_3);
	USARTSendByte(USART_MESSAGE,msgDat.addr_send.Bytes.byte_2);
	USARTSendByte(USART_MESSAGE,msgDat.addr_send.Bytes.byte_1);
	USARTSendByte(USART_MESSAGE,msgDat.addr_send.Bytes.byte_0);
#endif
	USARTSendByte(USART_MESSAGE,msgDat.cmd);
	for(i=0;i<msgDat.length;i++){
		USARTSendByte(USART_MESSAGE,msgDat.payload[i]);
	}
	USARTSendByte(USART_MESSAGE,CalculateCheckSum(msgDat));
}
#if ENABLE_ADDR>0
void SendMessageTo(u32 addr_rec,_serial_msg_obj msgDat){
	msgDat.addr_rec.value = addr_rec;
	msgDat.addr_send.value = ADDR_MYSELF;
	SendMessage(msgDat);
}
void SendMessage_Cmd(u32 addr_rec, u8 cmd){
	_serial_msg_obj msgDat;
	msgDat.addr_rec.value = addr_rec;
	msgDat.addr_send.value = ADDR_MYSELF;
#else
void SendMsg(USART_TypeDef* USARTx, u8 cmd){
	_serial_msg_obj msgDat;
#endif
	msgDat.length = 0;
	msgDat.cmd = cmd;
	SendMessage(msgDat);
}

boolean	MessageIsCorrect(_serial_msg_obj msgDat){
	if(CalculateCheckSum(msgDat) == msgDat.checkSum){
		return TRUE;
	}
	return FALSE;
}

u8 CalculateCheckSum(_serial_msg_obj msgDat){
	u8 check = 0,i=0;
	check += msgDat.length;
#if ENABLE_ADDR>0
	check += msgDat.addr_rec.Bytes.byte_3;
	check += msgDat.addr_rec.Bytes.byte_2;
	check += msgDat.addr_rec.Bytes.byte_1;
	check += msgDat.addr_rec.Bytes.byte_0;
	check += msgDat.addr_send.Bytes.byte_3;
	check += msgDat.addr_send.Bytes.byte_2;
	check += msgDat.addr_send.Bytes.byte_1;
	check += msgDat.addr_send.Bytes.byte_0;
#endif
	check += msgDat.cmd;
	for(i=0;i<msgDat.length;i++){
		check += msgDat.payload[i];
	}
	return check;
}	  
void MessageStateMachine(u8 msgByte)
{
	static u8 index_msg_read=0;
	static u8 state_msg=STATE_NONE;
	static u8 index_dat=0;
	static u8 msg_get_last=0;
	switch (state_msg)
	{
		case STATE_NONE:
			if (msg_get_last == MSG_START_MSB && msgByte == MSG_START_LSB)
			{
				msg_get_last = msgByte;
				state_msg = STATE_LENGTH;
			}
			break;
		case STATE_LENGTH://获取数据长度
			serialMsg[index_msg_read].length = msgByte;//
#if ENABLE_ADDR>0
			state_msg = STATE_ADDR_REC_1;
			break;
		case STATE_ADDR_REC_1:
			serialMsg[index_msg_read].addr_rec.Bytes.byte_3 = msgByte;
			state_msg = STATE_ADDR_REC_2;
			break;
		case STATE_ADDR_REC_2:
			serialMsg[index_msg_read].addr_rec.Bytes.byte_2 = msgByte;
			state_msg = STATE_ADDR_REC_3;
			break;
		case STATE_ADDR_REC_3:
			serialMsg[index_msg_read].addr_rec.Bytes.byte_1 = msgByte;
			state_msg = STATE_ADDR_REC_4;
			break;
		case STATE_ADDR_REC_4:
			serialMsg[index_msg_read].addr_rec.Bytes.byte_0 = msgByte;
			state_msg = STATE_ADDR_SEND_1;
			break;
		case STATE_ADDR_SEND_1:
			serialMsg[index_msg_read].addr_send.Bytes.byte_3 = msgByte;
			state_msg = STATE_ADDR_SEND_2;
			break;
		case STATE_ADDR_SEND_2:
			serialMsg[index_msg_read].addr_send.Bytes.byte_2 = msgByte;
			state_msg = STATE_ADDR_SEND_3;
			break;
		case STATE_ADDR_SEND_3:
			serialMsg[index_msg_read].addr_send.Bytes.byte_1 = msgByte;
			state_msg = STATE_ADDR_SEND_4;
			break;
		case STATE_ADDR_SEND_4:
			serialMsg[index_msg_read].addr_send.Bytes.byte_0 = msgByte;
#endif
			state_msg = STATE_CMD;
			break;
		case STATE_CMD:
			serialMsg[index_msg_read].cmd = msgByte;//获取数据命令字
			if (serialMsg[index_msg_read].length == 0)//如果数据长度为0
			{
				state_msg = STATE_CHECK;//接收校验码
			}
			else
			{
				index_dat = 0;
				state_msg = STATE_DATA;//接收数据
			}
			break;
		case STATE_DATA:
			serialMsg[index_msg_read].payload[index_dat] = msgByte;//保存数据
			index_dat++;
			if (index_dat == serialMsg[index_msg_read].length)
			{
				state_msg = STATE_CHECK;//数据接收完成，下一个字节是校验和
			}
			break;
		case STATE_CHECK:
			serialMsg[index_msg_read].checkSum = msgByte;
			state_msg = STATE_NONE;//
			serialMsg[index_msg_read].enable = TRUE;//将此数据设置为有效
			index_msg_read++;//指向下一条信息
			index_msg_read = index_msg_read%NUM_MSG_MAX;//如果是最后一条则跳转到第一条，防止数组越界
			break;
			default:break;
	}
	msg_get_last = msgByte;
}

void ProcessMessage(void){
	static u8 index_msg_process=0;
	if(serialMsg[index_msg_process].enable == TRUE)//如果接收到了一个完整的数据
	{   
	#if AUTO_CHECK>0
		if(MessageIsCorrect(serialMsg[index_msg_process]))//如果接收到的信息是正确的
	#endif
		{
		#if ENABLE_ADDR>0
			if(serialMsg[index_msg_process].addr_rec.value == ADDR_MYSELF || serialMsg[index_msg_process].addr_rec.value == ADDR_BROADCAST)
		#endif
			{
			#if AUTO_RESPONSE>0
				//回复信息		
				_serial_msg_obj serialMsg_send;		
				serialMsg_send.length = 0;	
			#if ENABLE_ADDR>0
				serialMsg_send.addr_rec = serialMsg[index_msg_process].addr_send;
				serialMsg_send.addr_send.value = ADDR_MYSELF;
			#endif
				serialMsg_send.cmd = CMD_RESPONSE | serialMsg[index_msg_process].cmd;	
				serialMsg_send.checkSum = CalculateCheckSum(serialMsg_send);
				SendMessage(serialMsg_send);
			#endif
				serialMsg_rec = &serialMsg[index_msg_process];
				if(GetMessageHook!=NULL){
					GetMessageHook();
				}
			}
		}
		serialMsg[index_msg_process].enable = FALSE;
		//指向下一条信息
		index_msg_process++;
		index_msg_process = index_msg_process%NUM_MSG_MAX;
	}
}
void SetGetMessageHook(void ( *GetMessage)( void )){
	GetMessageHook = GetMessage;
}

