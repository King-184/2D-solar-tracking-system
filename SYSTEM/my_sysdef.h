/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板-M3
//
//作者：tianqingyong
//修改日期:2020/06/10
//程序功能：封装和简化通用的操作
***********************************************/
#ifndef __MY_SYSDEF_H
#define __MY_SYSDEF_H

#include "stm32f10x.h"
#include <string.h> 
#include <stdio.h> 
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
////////////////////////////////////////////
#ifndef __weak
#define __weak          __attribute__((weak))
#endif /* __weak */
////////////////////////////////////////////
#ifndef __packed
#define __packed        __attribute__((__packed__))
#endif /* __packed */
#ifndef _packed
#define _packed         __packed
#endif /* __packed */
////////////////////////////////////////////
#define Addr_at(n)      __attribute__((section(".ARM.__at_"#n"")))
#define Section_at(n)   __attribute__((section(""#n"")))

#else
#pragma diag_suppress 177
#pragma diag_suppress 550//避免未使用的变量在编译时产生警告信息
#define Addr_at(n)      __attribute__((at(n)))
#ifndef __packed
#define __packed        _packed
#endif /* __packed */
#endif

#ifndef byte
typedef char  byte;
#endif
#ifndef int8
typedef char  int8;
#endif
#ifndef int16
typedef short int int16;
#endif
#ifndef int32
typedef long int int32;
#endif
#ifndef int64
typedef long long int64;
#endif
#ifndef int_8
typedef char  int_8;
#endif
#ifndef int_16
typedef short int int_16;
#endif
#ifndef int_32
typedef long int int_32;
#endif
#ifndef int_64
typedef long long int_64;
#endif
//在stm32f10x.h中有此项定义
#if !defined __STM32F10x_H
#ifndef u8
typedef unsigned char  u8;
#endif
#endif
#ifndef uint_8
typedef unsigned char  uint_8;
#endif
#ifndef uint8_t
typedef unsigned char  uint8_t;
#endif
//在stm32f10x.h中有此项定义
#if !defined __STM32F10x_H
#ifndef u16
typedef unsigned short int  u16;
#endif
#endif
#ifndef uint_16
typedef unsigned short int  uint_16;
#endif
#ifndef uint16_t
typedef unsigned short int  uint16_t;
#endif
//在stm32f10x.h中有此项定义
#if !defined __STM32F10x_H
#ifndef u32
typedef unsigned int  u32;
#endif
#endif
#ifndef uint_32
typedef unsigned int  uint_32;
#endif
//在stm32f10x.h中有此项定义
#if !defined __STM32F10x_H
#ifndef uint32_t
typedef unsigned int  uint32_t;
#endif
#endif
#ifndef u64
typedef unsigned long long  u64;
#endif
#ifndef uint_64
typedef unsigned long long  uint_64;
#endif
#ifndef uint64_t
typedef unsigned long long  uint64_t;
#endif

#if !defined (__STM32F10x_H)
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */
#endif
/********************************************************************/
//自定义联合体，方便数据操作，byte_0默认为最低字节（与大小端模式有关）
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
}Bits_8;
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
    u8 bit_8:1;
    u8 bit_9:1;
    u8 bit_10:1;
    u8 bit_11:1;
    u8 bit_12:1;
    u8 bit_13:1;
    u8 bit_14:1;
    u8 bit_15:1;
}Bits_16;
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
    u8 bit_8:1;
    u8 bit_9:1;
    u8 bit_10:1;
    u8 bit_11:1;
    u8 bit_12:1;
    u8 bit_13:1;
    u8 bit_14:1;
    u8 bit_15:1;
    u8 bit_16:1;
    u8 bit_17:1;
    u8 bit_18:1;
    u8 bit_19:1;
    u8 bit_20:1;
    u8 bit_21:1;
    u8 bit_22:1;
    u8 bit_23:1;
    u8 bit_24:1;
    u8 bit_25:1;
    u8 bit_26:1;
    u8 bit_27:1;
    u8 bit_28:1;
    u8 bit_29:1;
    u8 bit_30:1;
    u8 bit_31:1;
}Bits_32;
typedef struct
{
    u8 bit_0:1;
    u8 bit_1:1;
    u8 bit_2:1;
    u8 bit_3:1;
    u8 bit_4:1;
    u8 bit_5:1;
    u8 bit_6:1;
    u8 bit_7:1;
    u8 bit_8:1;
    u8 bit_9:1;
    u8 bit_10:1;
    u8 bit_11:1;
    u8 bit_12:1;
    u8 bit_13:1;
    u8 bit_14:1;
    u8 bit_15:1;
    u8 bit_16:1;
    u8 bit_17:1;
    u8 bit_18:1;
    u8 bit_19:1;
    u8 bit_20:1;
    u8 bit_21:1;
    u8 bit_22:1;
    u8 bit_23:1;
    u8 bit_24:1;
    u8 bit_25:1;
    u8 bit_26:1;
    u8 bit_27:1;
    u8 bit_28:1;
    u8 bit_29:1;
    u8 bit_30:1;
    u8 bit_31:1;
    u8 bit_32:1;
    u8 bit_33:1;
    u8 bit_34:1;
    u8 bit_35:1;
    u8 bit_36:1;
    u8 bit_37:1;
    u8 bit_38:1;
    u8 bit_39:1;
    u8 bit_40:1;
    u8 bit_41:1;
    u8 bit_42:1;
    u8 bit_43:1;
    u8 bit_44:1;
    u8 bit_45:1;
    u8 bit_46:1;
    u8 bit_47:1;
    u8 bit_48:1;
    u8 bit_49:1;
    u8 bit_50:1;
    u8 bit_51:1;
    u8 bit_52:1;
    u8 bit_53:1;
    u8 bit_54:1;
    u8 bit_55:1;
    u8 bit_56:1;
    u8 bit_57:1;
    u8 bit_58:1;
    u8 bit_59:1;
    u8 bit_60:1;
    u8 bit_61:1;
    u8 bit_62:1;
    u8 bit_63:1;
}Bits_64;
/****************************************************************/
typedef union{
    Bits_8 Bits;
    u8 value;
}union_u8;
typedef union{
    Bits_16 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
    }Bytes;
    u16 value;
}union_u16;
typedef union{
    Bits_16 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
    }Bytes;
    int16 value;
}union_int16;
typedef union{
    Bits_32 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
    }Uint16;
    u32 value;
}union_u32;
typedef union{
    Bits_32 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
    }Uint16;
    int32 value;
}union_int32;
typedef union{
    Bits_64 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
        u8 byte_4:8;
        u8 byte_5:8;
        u8 byte_6:8;
        u8 byte_7:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
        u16 uint16_2:16;
        u16 uint16_3:16;
    }Uint16;
    struct
    {
        u32 uint32_0:32;
        u32 uint32_1:32;
    }Uint32;
    u64  value;
}union_u64;
typedef union{
    Bits_64 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
        u8 byte_4:8;
        u8 byte_5:8;
        u8 byte_6:8;
        u8 byte_7:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
        u16 uint16_2:16;
        u16 uint16_3:16;
    }Uint16;
    struct
    {
        u32 uint32_0:32;
        u32 uint32_1:32;
    }Uint32;
    int64  value;
}union_int64;
typedef union{
    Bits_32 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
    }Uint16;
    float value;
}union_float;
typedef union{
    Bits_64 Bits;
    struct
    {
        u8 byte_0:8;
        u8 byte_1:8;
        u8 byte_2:8;
        u8 byte_3:8;
        u8 byte_4:8;
        u8 byte_5:8;
        u8 byte_6:8;
        u8 byte_7:8;
    }Bytes;
    struct
    {
        u16 uint16_0:16;
        u16 uint16_1:16;
        u16 uint16_2:16;
        u16 uint16_3:16;
    }Uint16;
    struct
    {
        u32 uint32_0:32;
        u32 uint32_1:32;
    }Uint32;
    double  value;
}union_double;
/*******************************************************************************************/

/*布尔型变量*/
typedef enum
{
    false =0,
    true =!false
} bool;
typedef enum
{
    Ctrl_AUTO=0,
    Ctrl_MANUAL=!Ctrl_AUTO
}CtrlMode;
typedef enum
{
    Alarm_N=0,
    Alarm_Y=!Alarm_N
}AlarmMode;

typedef enum
{
    TIM_CH_NULL=0x00,TIM_CH_1=0x01,TIM_CH_2=0x02,TIM_CH_12=0x03,
    TIM_CH_3=0x04,TIM_CH_13=0x05,TIM_CH_23=0x06,TIM_CH_123=0x07,
    TIM_CH_4=0x08,TIM_CH_14=0x09,TIM_CH_24=0x0A,TIM_CH_124=0x0B,
    TIM_CH_34=0x0C,TIM_CH_134=0x0D,TIM_CH_234=0x0E,TIM_CH_ALL=0x0F
}TIM_Channel;
/*******************************************************************************************/

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum)           ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)                  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)          MEM_ADDR(BITBAND(addr, bitnum))
//IO口地址映射
#define GPIOA_ODR_Addr                  (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr                  (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr                  (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr                  (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr                  (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr                  (GPIOF_BASE+12) //0x40011C0C
#define GPIOG_ODR_Addr                  (GPIOG_BASE+12) //0x4001200C

#define GPIOA_IDR_Addr                  (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr                  (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr                  (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr                  (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr                  (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr                  (GPIOF_BASE+8) //0x40011C08 
#define GPIOG_IDR_Addr                  (GPIOG_BASE+8) //0x40012008 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)                        BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PAin(n)                         BIT_ADDR(GPIOA_IDR_Addr,n)  //输入

#define PBout(n)                        BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
#define PBin(n)                         BIT_ADDR(GPIOB_IDR_Addr,n)  //输入

#define PCout(n)                        BIT_ADDR(GPIOC_ODR_Addr,n)  //输出
#define PCin(n)                         BIT_ADDR(GPIOC_IDR_Addr,n)  //输入

#define PDout(n)                        BIT_ADDR(GPIOD_ODR_Addr,n)  //输出
#define PDin(n)                         BIT_ADDR(GPIOD_IDR_Addr,n)  //输入

#define PEout(n)                        BIT_ADDR(GPIOE_ODR_Addr,n)  //输出
#define PEin(n)                         BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)                        BIT_ADDR(GPIOF_ODR_Addr,n)  //输出
#define PFin(n)                         BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)                        BIT_ADDR(GPIOG_ODR_Addr,n)  //输出
#define PGin(n)                         BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

/***************************程序兼容性宏定义****************************************/
#define boolean                         bool
#define TRUE                            true
#define FALSE                           false
#define DelayMs                         delay_ms
#define ArrayCount(table)               (sizeof(table)/sizeof(table[0]))
#define My_memcmp(m,n,t)                (memcmp(m,n,t)==0)
#define My_memcpy                       memcpy//C 库函数 void *memcpy(void *str1, const void *str2, size_t n) 从存储区 str2 复制 n 个字节到存储区 str1
#define DataStr_Complie                 __DATE__//编译时自动保存编译时的日期
#define TimeStr_Complie                 __TIME__//编译时自动保存编译时的时间
/************************定义Pin控制器件控制电平************************************/
#define LED_ON                          0
#define LED_OFF                         1
#define RLY_ON                          1
#define RLY_OFF                         0
#define BEEP_ON                         1
#define BEEP_OFF                        0
#define LOCK_ON                         1
#define LOCK_OFF                        0
#define PUMP_ON                         1
#define PUMP_OFF                        0

/*******************************************************************************************/

void NVIC_Configuration(void);
//以下为汇编函数
void WFI_SET(void);//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);//开启所有中断
void MSR_MSP(u32 addr);//设置堆栈地址
bool StateMachine_GetStr(char *str,u8 msgByte,u8 *state);
void ByteArrayToHalfWordArray(u8 *byteArray,u16 *halfWordArray,u16 lengthOfByteArray);
void HalfWordArrayToByteArray(u16 *halfWordArray,u8 *byteArray,u16 lengthOfHalfWordArray);
void SortShort(short *arr,u16 len,bool toBig);
int64 GetIntegerFromString(char *str,u16 startIndex,u16 len);

#endif
