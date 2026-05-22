/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//
//作者：tianqingyong
//版本：V1.1
//修改日期:2019/12/05
//程序功能：封装和简化输出管脚的操作
//V1.0 2019/10/22 完成基本功能
//V1.1 2019/12/05 删除按键个数宏定义
************************************************/
#ifndef __MY_LED_H
#define __MY_LED_H	 
#include "my_include.h"

#define LED0	 		PinOut(Pins_LED[0])
#define LED1	 		PinOut(Pins_LED[1])

const static MyPinDef Pins_LED[] = {PE5,PB5};

void My_LED_Init(void);//初始化

		 				    
#endif
