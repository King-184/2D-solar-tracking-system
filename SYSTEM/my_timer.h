/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//YXDZ STM32开发板		   
//永鑫电子工作室
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/06/06
//程序功能：封装定时器操作
//V1.0 完成基本功能
//V1.1 精简部分功能
************************************************/	  
 #ifndef __MY_TIMER_H
#define __MY_TIMER_H
#include "my_include.h"	

void TIMx_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc); 
 
#endif
