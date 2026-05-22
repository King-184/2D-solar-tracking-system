/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//
//作者：tianqingyong
//修改日期:2020/04/08
//程序功能：封装和简化通用的操作
************************************************/
#include "my_sysdef.h"
#include "my_include.h"

extern int $Super$$main(void);
int $Sub$$main(void)
{
#if !defined (USE_HAL_DRIVER)
    NVIC_Configuration();
    My_SysTick_Init();
#else
    HAL_Init();//初始化HAL库
    Stm32_Clock_Init(RCC_PLL_MUL9);//设置时钟,72M
#endif
    delay_init();//延时函数初始化
#if CODE_ENCRYPR>0
    MyCodeProtect();
#endif
    $Super$$main();
    return 0;
}
void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
    __ASM volatile("wfi");
}
//关闭所有中断
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}
//设置栈顶地址
//addr:栈顶地址
#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
void MSR_MSP(u32 addr) 
{
    __ASM volatile("MSR MSP, r0");//set Main Stack value
    __ASM volatile("BX r14");
}
#else
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0//set Main Stack value
    BX r14
}
#endif


bool StateMachine_GetStr(char *str,u8 msgByte,u8 *state)
{
    if(msgByte == *(str+*state))
    {
        *state = *state + 1;
        if(*state == strlen(str))
        {
            *state = 0;
            return TRUE; //全都相等则返回是
        }
    }
    else
    {
        *state = 0;
    }
    return FALSE; //全都相等则返回是
}
void ByteArrayToHalfWordArray(u8 *byteArray,u16 *halfWordArray,u16 lengthOfByteArray)
{
    u16 i=0;
    union_u16 temp;
    while(i<lengthOfByteArray)
    {
        temp.Bytes.byte_1 = byteArray[i];
        i++;
        if(i==lengthOfByteArray)
        {
            temp.Bytes.byte_0 = 0;
        }
        else
        {
            temp.Bytes.byte_0 = byteArray[i];
        }
//**All notes can be deleted and modified**//
    }
}
void HalfWordArrayToByteArray(u16 *halfWordArray,u8 *byteArray,u16 lengthOfHalfWordArray)
{
    u16 i=0;
    union_u16 temp;
    while(i<lengthOfHalfWordArray)
    {
        temp.value = halfWordArray[i];
        byteArray[i<<1] = temp.Bytes.byte_1;
        byteArray[(i<<1)+1] = temp.Bytes.byte_0;
        i++;
    }
}
void SortShort(short *arr,u16 len,bool toBig)
{
    u16 i,j;
    short temp;
    for(i=0;i<len;i++)
    {
        for(j=0;j<len-i;j++)
        {
            if(arr[j]>arr[j+1])
            {
                temp = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}
int64 GetIntegerFromString(char *str,u16 startIndex,u16 len)
{
    int64 value = 0;
    str += startIndex;
    while(len--)
    {
        value = (value<<8) +value +value + (*str++-0x30);
    }
    return value;
}

