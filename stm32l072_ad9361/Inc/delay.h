/*
**************************************************************************************************************
*	@file	delay.h
*	@author Ysheng
*	@version 
*	@date    
*	@brief	��ʱ����
***************************************************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

#define RTC_CNT Tick

void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);

void delay_ms(uint32_t i);
void Delay( float s );
void DelayMs( uint32_t ms );

#endif /* __DELAY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
