/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* USER CODE BEGIN Includes */

#include <stdbool.h>	 
	 
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

#define DEBUG_LEVEL	  		2					//调试等级，配合DEBUG调试宏控制调试输出范围,大于该等级的调试不输出
	 
#define NODEBUG		 3
		
#define APP        2

#define NORMAL     2

#define WARNING    2

#define ERROR      2
	
#define DEBUG_APP(level, fmt, arg...)  if(level <= APP)	printf("App:"__FILE__",line : %d,"fmt"\r\n",__LINE__,##arg); 	
#define DEBUG_WARNING(level, fmt, arg...)  if(level <= WARNING)	printf("Warning:"__FILE__",line : %d,"fmt"\r\n",__LINE__,##arg); 
#define DEBUG_ERROR(level, fmt, arg...)  if(level <= ERROR)	printf("Error:"__FILE__",line : %d,"fmt"\r\n",__LINE__,##arg); 


#define RXBUFFERSIZE   			1 //缓存大小

#define USART_REC_LEN  			516  	//定义最大接收字节数 516

typedef struct{
	
	uint32_t rxtime;
	
	uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
	//接收状态
	//bit15，	接收完成标志
	//bit14，	接收到0x0d
	//bit13~0，	接收到的有效字节数目
	uint16_t USART_RX_Len;       //接收数据长度

	uint8_t aRxBuffer[1];//HAL库使用的串口接收缓冲
	
	bool USART_TX_STATE;
	
	bool Rx_State;
	
}UART_RX;

extern UART_RX UART_RX_DATA1;	 
	 
/* USER CODE END Private defines */

extern void Error_Handler(void);

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

extern void InitUartFifo(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
