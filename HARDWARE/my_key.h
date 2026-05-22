/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//
//作者：tianqingyong
//版本：V2.0
//修改日期:2020/04/08
//程序功能：封装和简化按键的操作,C文件无须做任何修改
//V1.0 完成基本功能
//V1.1 修改为可定义每个按键按下的状态
//V1.2 2019/11/03 删除在定时器中断调用的功能
//V1.3 2019/12/05 删除按键个数宏定义
//V1.4 2019/12/09 增加 KeyIsReleased 宏定义
//V1.5 2019/12/18 修复一个bug
					将判断宏定义改为子程序调用
//V1.6 2019/12/25 增加长按状态的判断
//V1.7 2020/01/06 修正了部分按键状态判断的函数
					IO初始化根据按下的状态进行初始化
					增加了按键事件判断函数
//V1.8 2020/02/26 优化了按键状态判断程序
//V2.0 2020/04/08 增加了模拟按键按下的功能函数
					增加了按键枚举类型
************************************************/ 
#ifndef __MY_KEY_H
#define __MY_KEY_H	 
#include "my_include.h"

typedef enum
{
	KEY_1=0,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
}My_KeyDef;

//按键定义
const static MyPinDef Pins_Key[] = {PA5,PA6,PA7,PA8,PA11};  //A7 左 A6 右  A5	上  A8 下

#define KeyFlagType			u32					  
#define KEY_STATE_PRESS		0x00000000  //按键按下时IO口的值，32个位对应32个按键的状态
#define COUNT_LONG_PRESS	1000 			//这个数值要在实际调试中测试
#define LONG_PRESS			0  //按键长按使能定义，为0时长按功能无效

void My_KEY_Init(void);//IO初始化
void My_KeyScan(void);
#if	LONG_PRESS>0
bool KeyIsLongPress(My_KeyDef key);
bool KeyIsLongPressed(My_KeyDef key);
#endif
bool KeyIsPress(My_KeyDef key);
bool KeyIsPressed(My_KeyDef key);
bool KeyIsRelease(My_KeyDef key);
bool KeyIsReleased(My_KeyDef key);
bool HasKeyEvent(void);
#if	LONG_PRESS>0
void My_Key_PerformLongPress(My_KeyDef key);
void My_Key_PerformLongPressed(My_KeyDef key);
#endif
void My_Key_PerformPress(My_KeyDef key);
void My_Key_PerformPressed(My_KeyDef key);
void My_Key_PerformRelease(My_KeyDef key);
			    
#endif
