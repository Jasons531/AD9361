/*
**************************************************************************************************************
*	@file	delay.c
*	@author Ysheng
*	@version 
*	@date    
*	@brief ��ʱ����
***************************************************************************************************************
*/
#include "stm32l0xx_hal.h"
#include <stdint.h>
#include "delay.h"

static uint32_t fac_us = 0;							//us��ʱ������

//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(uint8_t SYSCLK)
{

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
	fac_us=SYSCLK;						    
}								    

//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ��Ҫ����1000us
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
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

