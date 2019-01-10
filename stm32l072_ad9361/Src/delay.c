/*
**************************************************************************************************************
*	@file	delay.c
*	@author Ysheng
*	@version 
*	@date    
*	@brief 延时函数
***************************************************************************************************************
*/
#include "stm32l0xx_hal.h"
#include <stdint.h>
#include "delay.h"

static uint32_t fac_us = 0;							//us延时倍乘数

//初始化延迟函数
//SYSTICK的时钟固定为AHB时钟的1/8
//SYSCLK:系统时钟频率
void delay_init(uint8_t SYSCLK)
{

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=SYSCLK;						    
}								    

//延时nus
//nus为要延时的us数.	
//注意:nus的值不要大于1000us
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}

void delay_ms(uint32_t tick_1ms)
{
	uint32_t i;
	for(i=0;i<tick_1ms;i++) 
	delay_us(1000);
}

void Delay( float s )
{
  delay_ms( s * 1000.0f );
}

void DelayMs( uint32_t ms )
{   
  delay_ms( ms );
}

