#ifndef __FIFO_UART_H
#define __FIFO_UART_H

#include "stm32l0xx_hal.h"

typedef void (*FuncPtr_t)(void);
typedef void (*FuncPtrU8_t)(uint8_t);

#define DISABLE_INT()                                { __set_PRIMASK(1); }	/* 关中断 */
#define ENABLE_INT()                                 { __set_PRIMASK(0); }	/* 开中断 */

/* 串口设备结构体 */
typedef struct
{
    UART_HandleTypeDef *huart; /* 串口设备指针 */
    uint8_t *pTxBuf;           /* 发送缓冲区 */
    uint8_t *pRxBuf;           /* 接收缓冲区 */
    uint16_t usTxBufSize;      /* 发送缓冲区大小 */
    uint16_t usRxBufSize;      /* 接收缓冲区大小 */
    __IO uint16_t usTxWrite;   /* 发送缓冲区写指针 */
    __IO uint16_t usTxRead;    /* 发送缓冲区读指针 */
    __IO uint16_t usTxCount;   /* 等待发送的数据个数 */
    __IO uint16_t usRxWrite;   /* 接收缓冲区写指针 */
    __IO uint16_t usRxRead;    /* 接收缓冲区读指针 */
    __IO uint16_t usRxCount;   /* 还未读取的新数据个数 */
    FuncPtr_t SendBeforFunc;   /* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
    FuncPtr_t SendOverFunc;    /* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
    FuncPtrU8_t ReceiveNewFunc; /* 串口收到数据的回调函数指针 */
    uint8_t initDoneFlag;      /* 初始化完成标志 */
} UART_FIFO_Typedef_t;

extern HAL_StatusTypeDef UartSendByte(UART_HandleTypeDef *huart, uint8_t ucByte);
extern HAL_StatusTypeDef UartSendBuffer(UART_HandleTypeDef *huart, uint8_t *ucBuffer, uint16_t usLength);
extern HAL_StatusTypeDef UartReadByte(UART_HandleTypeDef *huart, uint8_t *ucByte);
extern HAL_StatusTypeDef UartSetBaudRate(UART_HandleTypeDef *huart, uint32_t baudRate);

extern void FIFO_UartVarInit(UART_FIFO_Typedef_t *pFIFO_Uart,
                             UART_HandleTypeDef *huart,
                             uint8_t *pTxBuf,
                             uint8_t *pRxBuf,
                             uint16_t usTxBufSize,
                             uint16_t usRxBufSize,
                             FuncPtr_t SendBeforFunc,
                             FuncPtr_t SendOverFunc,
                             FuncPtrU8_t ReceiveNewFunc);
extern void FIFO_UartEnableRxIT(UART_FIFO_Typedef_t *pFIFO_Uart);
extern void FIFO_UartSetSendBeforFunc(UART_FIFO_Typedef_t *pFIFO_Uart, FuncPtr_t SendBeforFunc);
extern void FIFO_UartSetSendOverFunc(UART_FIFO_Typedef_t *pFIFO_Uart, FuncPtr_t SendOverFunc);
extern void FIFO_UartSetReceiveNewFunc(UART_FIFO_Typedef_t *pFIFO_Uart, FuncPtrU8_t ReceiveNewFunc);
extern HAL_StatusTypeDef FIFO_UartSetBaudRate(UART_FIFO_Typedef_t *pFIFO_Uart, uint32_t baudRate);
extern HAL_StatusTypeDef FIFO_UartReadByte(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t *ucByte);
extern uint16_t FIFO_UartReadBuffer(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t *ucBuffer, uint16_t usLen);
extern HAL_StatusTypeDef FIFO_UartSendBuffer(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t *ucBuffer, uint16_t usLen);
extern HAL_StatusTypeDef FIFO_UartSendByte(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t ucByte);
extern void FIFO_UartIRQ(UART_FIFO_Typedef_t *pFIFO_Uart);
#endif /* __FIFO_UART_H */
