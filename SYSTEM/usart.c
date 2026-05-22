/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//
//作者：tianqingyong
//版本：V2.3
//修改日期:2020/06/03
//程序功能：封装和简化USART的操作
***********************************************/
#include "usart.h"	 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#if !defined ( __GNUC__ ) || defined (__CC_ARM) /* GNU Compiler */
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 
#endif

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
USART_TypeDef* USART_Printf = USART1;
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	USARTSendByte(USART_Printf,ch);     
	return ch;
}
#endif 
  
void SetPrintfUSART(USART_TypeDef* USARTx)
{
	USART_Printf = USARTx;
}
USART_TypeDef* GetPrintfUSART(void)
{
	return USART_Printf;
}
void USARTSendByte(USART_TypeDef* USARTx, uint8_t dat)
{
	while((USARTx->SR&USART_SR_TC)==0);//等待发送完毕   
//  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	USART_SendData(USARTx,dat);
}
void USARTSendBytes(USART_TypeDef* USARTx, const uint8_t *dat, uint16_t length)
{
	while(length--)//数据长度不为0
	{
	  USARTSendByte(USARTx,*dat);
	  dat++;
	}
}
  
void USARTSendString(USART_TypeDef* USARTx, const char *str)
{
	while(*str!='\0'){
		USARTSendByte(USARTx,*str);
		str++;
	}
}

void USARTx_Init(USART_TypeDef* USARTx, u32 baud)
{
	//GPIO端口设置
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	if(USARTx == USART1)
	{
#if EN_USART1   //如果使能了串口1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1，GPIOA时钟
		//USART1_TX   PA.9
		GPIO_Pin_Init(PA9,GPIO_Mode_AF_PP);//复用推挽输出
		//USART1_RX	  PA.10
		GPIO_Pin_Init(PA10,GPIO_Mode_IN_FLOATING);//浮空输入
		//USART1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
#endif
	}
#if EN_USART2   //如果使能了串口2
	else if(USARTx == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2
		//USART2_TX   PA.2
		GPIO_Pin_Init(PA2,GPIO_Mode_AF_PP);//复用推挽输出
		//USART2_RX	  PA.3
		GPIO_Pin_Init(PA3,GPIO_Mode_IN_FLOATING);//浮空输入
		//USART2 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	}
#endif	
#if EN_USART3   //如果使能了串口3
	else if(USARTx == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3
		//USART3_TX   PB.10
		GPIO_Pin_Init(PB10,GPIO_Mode_AF_PP);//复用推挽输出
		//USART3_RX	  PB.11
		GPIO_Pin_Init(PB11,GPIO_Mode_IN_FLOATING);//浮空输入
		//USART3 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	}
#endif	

//**All notes can be deleted and modified**//

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baud;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_DeInit(USARTx);  //复位串口
	USART_Init(USARTx, &USART_InitStructure); //初始化串口
#if USART1_IT_RXNE>0
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接收中断
#endif
#if USART1_IT_TXE>0
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//开启串口发送中断
#endif
#if USART1_IT_IDLE>0
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//开启串口空闲中断
	/* 开启串口收发DMA请求 */
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
 	My_DMA_Config(DMA1_Channel5,DMA_DIR_PeripheralSRC,(u32)(&USART1->DR),(u32)USART1_RX_Buff,ArrayCount(USART1_RX_Buff));//DMA1通道4,外设为串口1,存储器为SendBuff,长度SEND_BUF_SIZE.
#endif
	USART_Cmd(USARTx, ENABLE);                    //使能串口 
}

#if USART1_IT_IDLE>0
u8 USART1_RX_Buff[RX_Buff_LEN];	//数据缓冲区
u8 readBuffer[RX_Buff_LEN];
bool readMsgFlag;
#endif
#if EN_USART1   //如果使能了串口1
//串口中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#if USART1_IT_RXNE>0
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART1);	//读取接收到的数据
//		My_ESPMessage_Receive(Res);//数据处理
//		ProcessSim800a(Res);//数据处理
		My_UARTMessage_Receive(Res);//数据处理
//		Esp8266StateMachine(Res);//数据处理
//		GPS_Process(Res);//GPS数据处理
//		MessageStateMachine(Res);//自定义串口协议
//		FPM10A_MsgStateMachine(Res);//指纹模块
	} 
#endif
#if USART1_IT_IDLE>0
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//空闲帧中断
	{
		Res=USART1->SR;
		Res=USART1->DR;//软件序列清除IDLE标志位
		Res = RX_Buff_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);//计算接收数据长度
		memcpy(readBuffer,USART1_RX_Buff,Res);
		readMsgFlag = true;
		My_DMA_Enable(DMA1_Channel5,RX_Buff_LEN);
		LED1 = !LED1;
	}
#endif
} 
#endif

#if EN_USART2   //如果使能了串口2
//串口中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART2);	//读取接收到的数据
//		My_MAX301xx_Uart_StateMachine(Res);//数据处理
//		Pm25StateMachine(Res);
	} 
} 
#endif

#if EN_USART3   //如果使能了串口3
//串口中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART3);	//读取接收到的数据
	} 
} 
#endif

