/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/04/08
//程序功能：封装和简化64步进电机的操作
************************************************/
#ifndef __MOTOR_STEP_H__
#define __MOTOR_STEP_H__
#include "my_include.h"

typedef enum
{
	StepMotor_Backward=-1,
	StepMotor_Stop=0,
	StepMotor_Forward=1
}StepMotorState;

static const MyPinDef LINES_MOTOR_STEP[][4] = {{PB0,PB1,PB8,PB3},{PB4,PB5,PB6,PB7}};//滑台与5线4相步进电机中间两根线是相反的(设计中使用的滑台行程是800步) 

#define VOLT_VALID	1

#if	 VOLT_VALID>0
	#define	LINE_ON		1
	#define	LINE_OFF	0
#else
	#define	LINE_ON		0
	#define	LINE_OFF	1
#endif
//步进电机接口及位置初始化
void My_StepMotor_Init(void);
//步进电机转动step步数，在应用程序中调用
void My_StepMotor_RotateStep(u8 ch, int32 step);
//步进电机的目标位置增加多少步（可以是负数）
void My_StepMotor_RotateStepAdd(u8 ch, int32 step);
//步进电机行进到某一个位置
void My_StepMotor_RotateToLocation(u8 ch, int32 location);
//步进电机相对于当前位置行进多少角度（可以是负数）
void My_StepMotor_RotateAngle(u8 ch, double angle);
//步进电机的目标位置增加多少步（可以是负数）
void My_StepMotor_RotateAngleAdd(u8 ch, double angle);
//步进电机行进到某一个角度
void My_StepMotor_RotateToAngle(u8 ch, double angle);
//步进电机转动处理，在定时器中断或单独一个线程中处理
void My_StepMotor_Go(void);
//步进电机停转，在应用程序中调用
void My_StepMotor_Stop(u8 ch);		
//获取步进电机当前的位置
int32 My_StepMotor_GetCurrentLocation(u8 ch);
//获取步进电机的目标位置
int32 My_StepMotor_GetAimLocation(u8 ch);	
StepMotorState My_StepMotor_GetState(u8 ch);

#endif
