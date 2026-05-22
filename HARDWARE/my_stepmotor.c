/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板		   
//
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/04/08
//程序功能：封装和简化64步进电机的操作
************************************************/
#include "my_stepmotor.h"

int32 location_now[ArrayCount(LINES_MOTOR_STEP)];
int32 location_aim[ArrayCount(LINES_MOTOR_STEP)];
StepMotorState motorState[ArrayCount(LINES_MOTOR_STEP)];

void Step_AA(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_OFF;
}
void Step_AB(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_OFF;
}
void Step_BB(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_OFF;
}
void Step_BC(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_OFF;
}
void Step_CC(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_OFF;
}
void Step_CD(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_ON;
}
void Step_DD(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_ON;
}
void Step_DA(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_ON;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_ON;
}
void Step_OFF(u8 ch)
{
	PinOut(LINES_MOTOR_STEP[ch][0])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][1])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][2])=LINE_OFF;
	PinOut(LINES_MOTOR_STEP[ch][3])=LINE_OFF;
}
//初始化步进电机
void My_StepMotor_Init(void)
{
	u8 i;	
	for(i=0;i<ArrayCount(LINES_MOTOR_STEP);i++)
	{
#if	!defined (USE_HAL_DRIVER)
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][0],GPIO_Mode_Out_PP);
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][1],GPIO_Mode_Out_PP);
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][2],GPIO_Mode_Out_PP);
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][3],GPIO_Mode_Out_PP);
#else
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][0],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][1],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][2],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
		GPIO_Pin_Init(LINES_MOTOR_STEP[i][3],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
#endif
		Step_AA(i);
		DelayMs(1);
		Step_AB(i);
		DelayMs(1);
		Step_BB(i);
		DelayMs(1);
		Step_BC(i);
		DelayMs(1);
		Step_BB(i);
		DelayMs(1);
		Step_AB(i);
		DelayMs(1);
		Step_AA(i);
		Step_OFF(i);
		location_now[i] = 0;
		location_aim[i] = 0;
		motorState[i] = StepMotor_Stop;
	}	
}
//步进电机行进1步（可以是负数）
void My_StepMotor_WalkOneStep(u8 ch)
{
	u8 state = 0;
	//如果已经到达目标位置
	if(location_now[ch]==location_aim[ch])
	{
		My_StepMotor_Stop(ch);//步进电机停止
		motorState[ch] = StepMotor_Stop;//步进电机停止
		return;
	}
	state = (location_now[ch]%8 + 8)%8;//计算当前需要接通的线圈
	switch(state)
	{
		case 0:Step_AB(ch);break;
		case 1:Step_BB(ch);break;
		case 2:Step_BC(ch);break;
		case 3:Step_CC(ch);break;
		case 4:Step_CD(ch);break;
		case 5:Step_DD(ch);break;
		case 6:Step_DA(ch);break;
		case 7:Step_AA(ch);break;
	} 
	if(location_now[ch]<location_aim[ch])//如果当前位置小于目标位置
	{   
		location_now[ch]++;//位置+1
		motorState[ch] = StepMotor_Forward;//标记为正转
	}
		if(location_now[ch]>location_aim[ch])//如果当前位置小于目标位置
	{   
		location_now[ch]--;//位置+1
		motorState[ch] = StepMotor_Backward;//标记为正转
	}
	//**All notes can be deleted and modified**//
}
//步进电机相对于当前位置行进多少步（可以是负数）
void My_StepMotor_RotateStep(u8 ch, int32 step)
{
	location_aim[ch] = location_now[ch] + step;//计算目标位置
	My_StepMotor_GetState(ch);//计算当前状态（正反转或停止）
}
//步进电机的目标位置增加多少步（可以是负数）
void My_StepMotor_RotateStepAdd(u8 ch, int32 step)
{
	location_aim[ch] +=  step;//计算目标位置
	My_StepMotor_GetState(ch);//计算当前状态（正反转或停止）
}
//步进电机行进到某一个位置
void My_StepMotor_RotateToLocation(u8 ch, int32 location)
{
	location_aim[ch] = location;//计算目标位置
	My_StepMotor_GetState(ch);//计算当前状态（正反转或停止）
}
//步进电机相对于当前位置行进多少角度（可以是负数）
void My_StepMotor_RotateAngle(u8 ch, double angle)
{
	angle *= 64; 
	My_StepMotor_RotateStep(ch,angle/5.625f);
}
//步进电机的目标位置增加多少步（可以是负数）
void My_StepMotor_RotateAngleAdd(u8 ch, double angle)
{
	angle *= 64; 
	My_StepMotor_RotateStepAdd(ch,angle/5.625f);
}
//步进电机行进到某一个角度
void My_StepMotor_RotateToAngle(u8 ch, double angle)
{
	angle *= 64; 
	My_StepMotor_RotateToLocation(ch,angle/5.625f);
}
//步进电机行进（转动），在定时器中断或某个线程中调用
void My_StepMotor_Go(void)
{
	uint8_t ch;	
	for(ch=0;ch<ArrayCount(LINES_MOTOR_STEP);ch++)
	{
		My_StepMotor_WalkOneStep(ch);//步进电机行进1步
	}
}	
//步进电机停止运动
void My_StepMotor_Stop(u8 ch)
{
	location_aim[ch] = location_now[ch];//将目标位置设置为当前位置
	Step_OFF(ch);//关闭所有线圈
	//**All notes can be deleted and modified**//
}			
//获取步进电机的当前位置
int32 My_StepMotor_GetCurrentLocation(u8 ch)
{
	return location_now[ch];
}	
//获取步进电机的目标位置
int32 My_StepMotor_GetAimLocation(u8 ch)
{
	return location_aim[ch];
}		
//获取步进电机的状态（正反转或停止）
StepMotorState My_StepMotor_GetState(u8 ch)
{
	if(location_now[ch]<location_aim[ch])//当前位置小于目标位置
	{   
		motorState[ch] = StepMotor_Forward;//标记为前进
	}
	else if(location_now[ch]>location_aim[ch])//当前位置大于目标位置
	{		
		motorState[ch] = StepMotor_Backward;//标记为后退
	}
	//**All notes can be deleted and modified**//
	return motorState[ch];
}
