#include "my_include.h"  
#include "my_stepmotor.h"
#include "stm32f10x.h"  // STM32 ?????
#include "stm32f10x_gpio.h"  // GPIO ?????
#include "stm32f10x_rcc.h"  // ?????????
#include "stm32f10x_tim.h"  // ????????(?? PWM)

void GPIO_Init1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ??GPIOB??
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // ??PB15?????
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ??PB14?????
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

char dis0[25];//液晶显示暂存数组
char dis1[25];//液晶显示暂存数组
u16 adcx;
#define F_SIZE      16
#define MyLCD_Show(m,n,p)     LCD_ShowString(LCD_GetPos_X(F_SIZE,m),LCD_GetPos_Y(F_SIZE,n),p,F_SIZE,false)

extern u16 ADC_convered[5];//ADC光敏数据采集
float temp[5];    
float value;
float value1;
char buf[5];
char buf1[5];

#define RONGCHAZHI_UD  50 //容差值
#define RONGCHAZHI_LR  50 //容差值

#define ZHUANDONG_ZZ  30 //转动一个正角度
#define ZHUANDONG_FZ  -30 //转动另一个方向转动

int lighVla_left = 0;//ad采集结果 1	
int lighVla_up = 0;//ad采集结果
int lighVla_right = 0;//ad采集结果
int lighVla_down = 0;//ad采集结果
float batteryVolt = 0;//锂电池电压ad采集结果
float BatCap=80;			  //容量初始化

unsigned char disFlag = 0;//更新显示
unsigned char setMode =0;//设置模式
unsigned char rememberMode =0xff;//记录上一次设置状态

int main(void)
{ 
	unsigned char disYplace=0; //显示所在行递增变量
//	USARTx_Init(USART1,9600);
//	My_LED_Init();
//	My_RTC_Init(false);
	Adc_Init();  //ADC数据采集初始化
	My_KEY_Init(); //按键初始化
	My_StepMotor_Init(); //电机初始化

	LCD_Init();   //tft初始化	  	 
//	TP_Init(); //校准已经包括再此函数中 先执行LCD_Init();  触摸校准调用显示	

	LCD_Clear(Color16_BLACK);//清全屏
	BACK_COLOR=Color16_BLACK;
	FRONT_COLOR=Color16_LIGHTGRAY;	disYplace=0; //显示所在行递增变量		
	MyLCD_Show(2,disYplace++,"双轴追光系统");//显示	
	FRONT_COLOR=Color16_LIGHTBLUE;
	MyLCD_Show(1,disYplace++,"光照参数: ");//显示	
	MyLCD_Show(4,disYplace++,"上: ");//显示	
	MyLCD_Show(1,disYplace,"左: ");//显示	
	MyLCD_Show(9,disYplace++,"右: ");//显示	
	MyLCD_Show(4,disYplace++,"下: ");//显示
	MyLCD_Show(1,disYplace++,"设置: ");//显示	
			
	while(1)
	{ 
		My_KeyScan();  //按键扫描
		if(KeyIsPress(KEY_5))
		{
            setMode++;
			if(setMode >= 2 )setMode =0;//自动模式
		}
		
		switch(setMode)
		{
			case 1: //自动模式

						if((lighVla_left - lighVla_right)>RONGCHAZHI_LR ) //调整向强光方向转动
						{
						   	GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_3);
                GPIO_ResetBits(GPIOB, GPIO_Pin_4);
							
						}
						else if((lighVla_right - lighVla_left )>RONGCHAZHI_LR )//调整向强光方向转动
						{
						    GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_4);
                GPIO_ResetBits(GPIOB, GPIO_Pin_3);
							  							
						}
						else {
						    GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_3);
                GPIO_SetBits(GPIOB, GPIO_Pin_4);
							  
						}
						
						if((lighVla_up - lighVla_down)>RONGCHAZHI_UD ) 
						{
						   	GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_5);
                GPIO_ResetBits(GPIOB, GPIO_Pin_6);
						}
						else if((lighVla_down - lighVla_up )>RONGCHAZHI_UD )
						{
						  	GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_6);
                GPIO_ResetBits(GPIOB, GPIO_Pin_5);
						}		
						else  {
						    GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_6);
                GPIO_SetBits(GPIOB, GPIO_Pin_5);
						}		
						break;
//			case 1: //自动模式

//						if((lighVla_left - lighVla_right)>RONGCHAZHI_LR ) //调整向强光方向转动
//						{My_StepMotor_RotateAngle(0,ZHUANDONG_FZ);}
//						else if((lighVla_right - lighVla_left )>RONGCHAZHI_LR )//调整向强光方向转动
//						{My_StepMotor_RotateAngle(0,ZHUANDONG_ZZ);}
//						else {My_StepMotor_Stop(0);}
//						
//						if((lighVla_up - lighVla_down)>RONGCHAZHI_UD ) //调整向强光方向转动
//						{My_StepMotor_RotateAngle(1,ZHUANDONG_FZ);}
//						else if((lighVla_down - lighVla_up )>RONGCHAZHI_UD )//调整向强光方向转动
//						{My_StepMotor_RotateAngle(1,ZHUANDONG_ZZ);}		
//						else  {My_StepMotor_Stop(1);}		
//						break;                        
			case 0: //手动模式							
						if(KeyIsPressed(KEY_2))//按键按下
						{
								delay_ms(1000);
								if(KeyIsPressed(KEY_2))
								{
							  GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_4);
                GPIO_ResetBits(GPIOB, GPIO_Pin_3);//转动 度数 正值一个方向 负值另一个方向 
								}
						}
						else if(KeyIsPressed(KEY_4))//按键按下
						{
							  delay_ms(1000);
								if(KeyIsPressed(KEY_4))
								{
							  GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_3);
                GPIO_ResetBits(GPIOB, GPIO_Pin_4);
								}//转动 度数 正值一个方向 负值另一个方向
						}		
						else {
						    GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_3);
                GPIO_SetBits(GPIOB, GPIO_Pin_4);
						}			

						if(KeyIsPressed(KEY_1))//按键按下
						{
							  delay_ms(1000);
								if(KeyIsPressed(KEY_1))
								{
							  GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_6);
                GPIO_ResetBits(GPIOB, GPIO_Pin_5);
								}//转动 度数 正值一个方向 负值另一个方向
						}						
						else if(KeyIsPressed(KEY_3))//按键按下
						{
							  delay_ms(1000);
								if(KeyIsPressed(KEY_3))
								{
							  GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_5);
                GPIO_ResetBits(GPIOB, GPIO_Pin_6);
								}//转动 度数 正值一个方向 负值另一个方向
						}
						else {
						    GPIO_Init1();
                GPIO_SetBits(GPIOB, GPIO_Pin_6);
                GPIO_SetBits(GPIOB, GPIO_Pin_5);
						}	
						break;
			default: break;		
		}

		 lighVla_left =  1000*(float)ADC_convered[0]/4096; //读取ad值 转化为0-1000
		 lighVla_up =  1000*(float)ADC_convered[1]/4096; //读取ad值 转化为0-1000
		 lighVla_right =  1000*(float)ADC_convered[2]/4096; //读取ad值 转化为0-1000
		 lighVla_down = 1000*(float)ADC_convered[3]/4096; //读取ad值 转化为0-1000
		
		if(disFlag == 1)//更新显示
		{
			
			disFlag =0;
			FRONT_COLOR=Color16_LIGHTBLUE;	disYplace=2; //显示所在行递增变量		
			sprintf(dis0,"上:%d  ",lighVla_up);//打印
			MyLCD_Show(4,disYplace++,dis0);//显示			
			
			sprintf(dis0,"%d ",lighVla_left);//打印
			MyLCD_Show(4,disYplace,dis0);//显示				
			sprintf(dis0,"%d ",lighVla_right);//打印
			MyLCD_Show(12,disYplace++,dis0);//显示	
			
			sprintf(dis0,"%d ",lighVla_down);//打印
			MyLCD_Show(7,disYplace++,dis0);//显示					
			if(rememberMode != setMode)
			{
				rememberMode = setMode;//记录设置模式
				if(setMode == 0)MyLCD_Show(8,disYplace++,"手动 ");//显示		
               // else if(setMode == 1)MyLCD_Show(8,disYplace++,"自动2 ");//显示		
				else if(setMode == 1)MyLCD_Show(8,disYplace++,"自动  ");//显示		
			}
			
			adcx=(float)ADC_convered[4];   
			batteryVolt  = (float)adcx*(3.3/4096*2) ;//锂电池电压
			if(batteryVolt>4.15)
			{BatCap=0.99;}
			else if(batteryVolt<3.4)
			{BatCap=0;}
			else
			{BatCap =( batteryVolt-3.4)/(4.15-3.4);}//正常情况下计算比例 

			if(batteryVolt<0.30)
      {FRONT_COLOR=Color16_RED;	}
			sprintf(dis0,"B:%3.1fv Q:%02d%% ",batteryVolt,(int)(BatCap*100));		//打印	
      //printf("%d",adcx);
			MyLCD_Show(1,6,dis0);//显示			
		}
	}											    
}	
