/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/06/06
//程序功能：封装和简化软件模拟IIC的操作
************************************************/	 	
#ifndef __MY_IIC_H
#define __MY_IIC_H
#include "my_include.h"	
#define IIC_BIT_ACK			0
#define IIC_BIT_NOACK		1  

#define PIN_IIC_SCL		PA4
#define PIN_IIC_SDA		PA5

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
u8 IIC_WaitAck(void);
u8 IIC_Write_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(u8 ack);//IIC读取一个字节
void IIC_WriteOneByte(u8 daddr,u8 addr,u8 data);
//*********************************************************
void IIC_WriteBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len);
void IIC_WriteString(u8 SlaveAddress, u8 REG_Address,char *buf);
u8 IIC_ReadOneByte(u8 daddr,u8 addr);
void IIC_ReadBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len);	  
#endif
















