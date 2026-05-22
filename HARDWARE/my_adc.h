/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//
//作者：tianqingyong
//版本：V1.0
//修改日期:2019/10/25
//程序功能：封装和简化ADC的初始化等操作，适用于标准库
************************************************/ 
#ifndef __MY_ADC_H
#define __MY_ADC_H	
#include "my_include.h"    

#define Volt_Ref_ADC				3.275
#define Volt_ACS712_ZERO			2.51
////使用5V供电，输出1/2分压
#define ADC_GAS_HIGH				3000////有害气体量最大时ADC的采集值
#define ADC_GAS_LOW					200////有害气体量最小时ADC的采集值
//使用5V供电，输出1/2分压
#define ADC_SMOKE_HIGH				900////烟雾量最大时ADC的采集值
#define ADC_SMOKE_LOW				280////烟雾量最小时ADC的采集值
////使用3.3V供电，输出不分压
//#define ADC_SMOKE_HIGH			2500////烟雾量最大时ADC的采集值
//#define ADC_SMOKE_LOW				1400////烟雾量最小时ADC的采集值
//使用3.3V供电，10K上拉电阻(光照越强阻值越小,负相关的非线性曲线)
#define ADC_LIGHT_HIGH				4000////光照量最小时ADC的采集值
#define ADC_LIGHT_LOW				10////光照量最大时ADC的采集值

const static MyPinDef Pins_ADC[] = {PA0,PA1,PA2,PA3,PA4};

#define My_ADC_GetGas(m,n,t)						My_ADC_GetPercent(m,n,t,ADC_GAS_LOW,ADC_GAS_HIGH)//m通道采集n次
#define My_ADC_GetSmoke(m,n,t)					My_ADC_GetPercent(m,n,t,ADC_SMOKE_LOW,ADC_SMOKE_HIGH)//m通道采集n次
#define My_ADC_GetLight(m,n,t)					(100-My_ADC_GetPercent(m,n,t,ADC_LIGHT_LOW,ADC_LIGHT_HIGH))//m通道采集n次
#define My_ADC_GetVoltage(m,ch,n,scale)			((float)My_ADC_GetAverage(m,ch,n)*Volt_Ref_ADC*scale/4095)
#define My_ADC_GetCapacity_Li(m,n,t)				My_ADC_GetPercent(m,n,t,3.4,4.15)//m通道采集n次

void My_ADC_Init(ADC_TypeDef* ADCx);
u16  My_ADC_GetValue(ADC_TypeDef* ADCx,u8 ch); 
u16 My_ADC_GetAverage(ADC_TypeDef* ADCx,u8 ch,u8 times);
u8 My_ADC_GetPercent(ADC_TypeDef* ADCx,u8 ch,u8 times,float adcVlaueMin,float adcValueMax);
float My_ADC_GetCurrent_ASC712(ADC_TypeDef* ADCx,u8 ch,u8 times,float scale);



static void ADC_MODE_CONFIG_(void);
static void ADC_GPIO_CONFIG(void);
void Adc_Init(void);




#endif 
