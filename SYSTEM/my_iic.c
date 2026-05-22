/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/06/06
//程序功能：封装和简化软件模拟IIC的操作
************************************************/	 	
#include "my_iic.h"  

//初始化IIC
void IIC_Init(void)
{			
    GPIO_Pin_Init(PIN_IIC_SCL,GPIO_Mode_Out_PP);
    GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);
	PinOut(PIN_IIC_SCL)=1;
	PinOut(PIN_IIC_SDA)=1;

}
//产生IIC起始信号
void IIC_Start(void)
{
	GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);     //sda线输出
	PinOut(PIN_IIC_SDA)=1;	  	  
	PinOut(PIN_IIC_SCL)=1;
	delay_us(4);
 	PinOut(PIN_IIC_SDA)=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	PinOut(PIN_IIC_SCL)=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);//sda线输出
	PinOut(PIN_IIC_SCL)=0;
	PinOut(PIN_IIC_SDA)=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	PinOut(PIN_IIC_SCL)=1; 
	PinOut(PIN_IIC_SDA)=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_WaitAck(void)
{
	u8 ucErrTime=0;
	GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_IPU);      //SDA设置为输入  
	PinOut(PIN_IIC_SDA)=1;delay_us(1);	   
	PinOut(PIN_IIC_SCL)=1;delay_us(1);	 
	while(PinRead(PIN_IIC_SDA))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	PinOut(PIN_IIC_SCL)=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答,入口参数:ack (0:ACK 1:NAK)
void IIC_SendAck(u8 ack)
{
	PinOut(PIN_IIC_SCL)=0;
	GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP);
	PinOut(PIN_IIC_SDA)=ack;
	delay_us(2);
	PinOut(PIN_IIC_SCL)=1;
	delay_us(2);
	PinOut(PIN_IIC_SCL)=0;
}				 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
u8 IIC_Write_Byte(u8 txd)
{                        
    u8 t;   
	GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_Out_PP); 	    
    PinOut(PIN_IIC_SCL)=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        PinOut(PIN_IIC_SDA)=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		PinOut(PIN_IIC_SCL)=1;
		delay_us(30); 
		PinOut(PIN_IIC_SCL)=0;	
		delay_us(30);
    }	 
	return IIC_WaitAck();
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	GPIO_Pin_Init(PIN_IIC_SDA,GPIO_Mode_IPU);//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        PinOut(PIN_IIC_SCL)=0; 
        delay_us(2);
		PinOut(PIN_IIC_SCL)=1;
        receive<<=1;
        if(PinRead(PIN_IIC_SDA))receive++;   
		delay_us(1); 
    }			
    IIC_SendAck(ack); //发送ACK   
    return receive;
}

//写寄存器
//SlaveAddress:器件地址
//REG_Address:寄存器地址
//REG_data:要写入寄存器的值
//返回值:无
void IIC_WriteOneByte(u8 SlaveAddress, u8 REG_Address,u8 REG_data) 
{
	IIC_Start();  				 					//产生IIC起始信号
	IIC_Write_Byte(SlaveAddress);	//发送写器件指令
	IIC_Write_Byte(REG_Address);   //发送寄存器地址
	IIC_Write_Byte(REG_data);     	//发送值				
	IIC_Stop();										//发送IIC停止信号	   
}
//*********************************************************
void IIC_WriteBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len)
{
	IIC_Start();  				 					//产生IIC起始信号
	IIC_Write_Byte(SlaveAddress);	//发送写器件指令
	IIC_Write_Byte(REG_Address);   //发送寄存器地址
	while(len--)
	{
		IIC_Write_Byte(*buf++);     	//发送值			
	}
	IIC_Stop();										//发送IIC停止信号	
}
void IIC_WriteString(u8 SlaveAddress, u8 REG_Address,char *buf)
{
	IIC_WriteBytes(SlaveAddress,REG_Address,(u8 *)buf,strlen(buf));
}
//读寄存器
//SlaveAddress:器件地址
//REG_Address:寄存器地址
//返回值:读到的值
u8 IIC_ReadOneByte(u8 SlaveAddress, u8 REG_Address) 		
{
	u8 temp=0;		 
	IIC_Start();  				 					//产生IIC起始信号
	IIC_Write_Byte(SlaveAddress);	//发送写器件指令	 
	IIC_Write_Byte(REG_Address);   //发送寄存器地址
	
	IIC_Start();  	 	   							//重新启动，产生IIC起始信号
	IIC_Write_Byte(SlaveAddress+1);	//发送读器件指令
	temp=IIC_Read_Byte(0);					//读取一个字节,不继续再读,发送NAK
	IIC_Stop();											//发送IIC停止信号
	return temp;											//返回读到的值
}

//*********************************************************
void IIC_ReadBytes(u8 SlaveAddress, u8 REG_Address,u8 *buf,u16 len)
{
    IIC_Start();                          //起始信号
    IIC_Write_Byte(SlaveAddress);           //发送设备地址+写信号
    IIC_Write_Byte(REG_Address);                   //发送存储单元地址	
    IIC_Start();                          //起始信号
    IIC_Write_Byte(SlaveAddress+1);         //发送设备地址+读信号
	while(--len)
	{
		*buf++ = IIC_Read_Byte(IIC_BIT_ACK);
	}
		*buf++ = IIC_Read_Byte(IIC_BIT_NOACK);
    IIC_Stop();                          //停止信号
}
