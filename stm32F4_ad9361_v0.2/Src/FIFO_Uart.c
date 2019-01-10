
#include "FIFO_Uart.h"

/**
* 函数功能: 串口发送1个字符数据
* 输入参数: huart:串口结构体handle指针
*          ucByte:发送字符
* 返 回 值: HAL_StatusTypeDef
* 说    明: 无
**/
HAL_StatusTypeDef UartSendByte(UART_HandleTypeDef *huart, uint8_t ucByte)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE))
    {
        huart->Instance->DR = (ucByte & (uint8_t)0x00FF);
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

/**
* 函数功能: 串口发送数据
* 输入参数: huart:串口结构体handle指针
*          ucBuffer:发送数据指针地址
*          usLen:发送数据长度
* 返 回 值: 无
* 说    明: 无
**/
HAL_StatusTypeDef UartSendBuffer(UART_HandleTypeDef *huart, uint8_t *ucBuffer, uint16_t usLen)
{
    int i = 0;

    for (i = 0; i < usLen; i++)
    {
        uint32_t TickStart = HAL_GetTick();
        while (UartSendByte(huart, ucBuffer[i]) == HAL_ERROR)
        {
            if ((HAL_GetTick() - TickStart) > 10) //超时保护
            {
                return HAL_ERROR;
            }
        }
    }
    return HAL_OK;
}

/**
* 函数功能: 串口读取1字节数据
* 输入参数: huart:串口结构体handle指针
*          ucByte:读取字节数据指针地址
* 返 回 值: HAL_StatusTypeDef
* 说    明: 无
**/
HAL_StatusTypeDef UartReadByte(UART_HandleTypeDef *huart, uint8_t *ucByte)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))
    {
        *ucByte = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

/**
* 函数功能: 串口设置波特率
* 输入参数: huart:串口结构体handle指针
*          baudRate:新的波特率
* 返 回 值: HAL_StatusTypeDef
* 说    明: 用在串口初始化之后，修改波特率
**/
HAL_StatusTypeDef UartSetBaudRate(UART_HandleTypeDef *huart, uint32_t baudRate)
{
    huart->Init.BaudRate = baudRate;

    return HAL_UART_Init(huart);
}

/**
* 函数功能: 初始化fFIFO串口变量
* 输入参数: pFIFO_Uart:串口结构体指针
*          huart:串口设备指针
*          pTxBuf:发送缓冲区
*          pRxBuf:接收缓冲区
*          usTxBufSize:发送缓冲区大小
*          usRxBufSize:接收缓冲区大小
*          SendBeforFunc:开始发送之前的回调函数指针
*          SendOverFunc:发送完毕的回调函数指针
*          ReceiveNewFunc:串口收到数据的回调函数指针
* 返 回 值: 无
* 说    明: 用在串口初始化之后，建立缓存串口收发
**/
void FIFO_UartVarInit(UART_FIFO_Typedef_t *pFIFO_Uart,
                      UART_HandleTypeDef *huart,
                      uint8_t *pTxBuf,
                      uint8_t *pRxBuf,
                      uint16_t usTxBufSize,
                      uint16_t usRxBufSize,
                      FuncPtr_t SendBeforFunc,
                      FuncPtr_t SendOverFunc,
                      FuncPtrU8_t ReceiveNewFunc)
{
    pFIFO_Uart->huart = huart;
    pFIFO_Uart->pTxBuf = pTxBuf;
    pFIFO_Uart->pRxBuf = pRxBuf;
    pFIFO_Uart->usTxBufSize = usTxBufSize;
    pFIFO_Uart->usRxBufSize = usRxBufSize;
    pFIFO_Uart->usTxWrite = 0;
    pFIFO_Uart->usTxRead = 0;
    pFIFO_Uart->usRxWrite = 0;
    pFIFO_Uart->usRxRead = 0;
    pFIFO_Uart->usRxCount = 0;
    pFIFO_Uart->usTxCount = 0;
    pFIFO_Uart->SendBeforFunc = SendBeforFunc;
    pFIFO_Uart->SendOverFunc = SendOverFunc;
    pFIFO_Uart->ReceiveNewFunc = ReceiveNewFunc;
    pFIFO_Uart->initDoneFlag = 1;
}

/**
* 函数功能: 手动打开接收中断
* 输入参数: pFIFO_Uart:串口结构体指针
* 返 回 值: void
* 说    明: 无
**/
void FIFO_UartEnableRxIT(UART_FIFO_Typedef_t *pFIFO_Uart)
{
    if (pFIFO_Uart->pRxBuf)
    {
        __HAL_UART_ENABLE_IT(pFIFO_Uart->huart, UART_IT_RXNE);
    }
}

/**
* 函数功能: 设置发送之前的回调函数
* 输入参数: pFIFO_Uart:串口结构体指针
*          SendBeforFunc:开始发送之前的回调函数指针
* 返 回 值: void
* 说    明: 无
**/
void FIFO_UartSetSendBeforFunc(UART_FIFO_Typedef_t *pFIFO_Uart, FuncPtr_t SendBeforFunc)
{
    pFIFO_Uart->SendBeforFunc = SendBeforFunc;
}

/**
* 函数功能: 设置发送完毕的回调函数
* 输入参数: pFIFO_Uart:串口结构体指针
*          SendOverFunc:发送完毕的回调函数指针
* 返 回 值: void
* 说    明: 无
**/
void FIFO_UartSetSendOverFunc(UART_FIFO_Typedef_t *pFIFO_Uart, FuncPtr_t SendOverFunc)
{
    pFIFO_Uart->SendOverFunc = SendOverFunc;
}

/**
* 函数功能: 设置收到数据的回调函数
* 输入参数: pFIFO_Uart:串口结构体指针
*          ReceiveNewFunc:串口收到数据的回调函数指针
* 返 回 值: void
* 说    明: 无
**/
void FIFO_UartSetReceiveNewFunc(UART_FIFO_Typedef_t *pFIFO_Uart, FuncPtrU8_t ReceiveNewFunc)
{
    pFIFO_Uart->ReceiveNewFunc = ReceiveNewFunc;
}

/**
* 函数功能: 串口设置波特率
* 输入参数: pFIFO_Uart:串口结构体指针
*          baudRate:新的波特率
* 返 回 值: HAL_StatusTypeDef
* 说    明: 用在串口初始化之后，修改波特率
**/
HAL_StatusTypeDef FIFO_UartSetBaudRate(UART_FIFO_Typedef_t *pFIFO_Uart, uint32_t baudRate)
{
    return UartSetBaudRate(pFIFO_Uart->huart, baudRate);
}

/**
* 函数功能: 从串口接收缓冲区读取1字节数据
* 输入参数: pFIFO_Uart:接收
*          ucByte:读取字节数据指针地址
* 返 回 值: HAL_StatusTypeDef
* 说    明: 无
**/
HAL_StatusTypeDef FIFO_UartReadByte(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t *ucByte)
{
    uint16_t usCount;

    /* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
    DISABLE_INT();
    usCount = pFIFO_Uart->usRxCount;
    ENABLE_INT();

    if (usCount == 0 || ucByte == NULL || pFIFO_Uart->pRxBuf == NULL) /* 已经没有数据 */
    {
        return HAL_ERROR;
    }
    else
    {
        *ucByte = pFIFO_Uart->pRxBuf[pFIFO_Uart->usRxRead]; /* 从串口接收FIFO取1个数据 */

        /* 改写FIFO读索引 */
        DISABLE_INT();
        if (++pFIFO_Uart->usRxRead >= pFIFO_Uart->usRxBufSize)
        {
            pFIFO_Uart->usRxRead = 0;
        }
        pFIFO_Uart->usRxCount--;
        ENABLE_INT();

        return HAL_OK;
    }
}

/**
* 函数功能: 从串口接收缓冲区读取数据
* 输入参数: pFIFO_Uart:接收
*          usLen:要读的数据长度
* 返 回 值: 读到的数据长度
* 说    明: 无
**/
uint16_t FIFO_UartReadBuffer(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t *ucBuffer, uint16_t usLen)
{
    uint16_t i;

    for (i = 0; i < usLen; i++)
    {
        if (FIFO_UartReadByte(pFIFO_Uart, ucBuffer + i) == HAL_ERROR)
        {
            return i;
        }
    }
    return usLen;
}

/**
  * 函数功能: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
HAL_StatusTypeDef FIFO_UartSendBuffer(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t *ucBuffer, uint16_t usLen)
{
    uint16_t i;

    /* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为发送模式，避免抢占总线 */
    if (pFIFO_Uart->SendBeforFunc)
    {
        pFIFO_Uart->SendBeforFunc();
    }

    if (ucBuffer == NULL || pFIFO_Uart->pTxBuf == NULL)
    {
        return HAL_ERROR;
    }

#if 1
    if ((pFIFO_Uart->usTxCount + usLen) > pFIFO_Uart->usTxBufSize)
    {
        return HAL_BUSY;
    }
#endif
    for (i = 0; i < usLen; i++)
    {
#if 0
        uint32_t TickStart = HAL_GetTick();
        /* 如果发送缓冲区已经满了，则等待缓冲区空 */
        while (1)
        {
            __IO uint16_t usCount;

            DISABLE_INT();
            usCount = pFIFO_Uart->usTxCount;
            ENABLE_INT();

            if (usCount < pFIFO_Uart->usTxBufSize)
            {
                break;
            }
            else
            {
                __HAL_UART_ENABLE_IT(pFIFO_Uart->huart, UART_IT_TXE); // 防止中断未开启会卡死
            }

            if ((HAL_GetTick() - TickStart) > 100) //超时保护
            {
                return HAL_ERROR;
            }
        }
#endif
        /* 将新数据填入发送缓冲区 */
        pFIFO_Uart->pTxBuf[pFIFO_Uart->usTxWrite] = ucBuffer[i];

        DISABLE_INT();
        if (++pFIFO_Uart->usTxWrite >= pFIFO_Uart->usTxBufSize)
        {
            pFIFO_Uart->usTxWrite = 0;
        }
        pFIFO_Uart->usTxCount++;
        ENABLE_INT();
    }
		//stm32L072
		SET_BIT(pFIFO_Uart->huart->Instance->CR1, USART_CR1_TXEIE);
		//stm32F103
    //__HAL_UART_ENABLE_IT(pFIFO_Uart->huart, UART_IT_TXE);
    return HAL_OK;
}

/**
* 函数功能: 向串口发送1个字节
* 输入参数: pFIFO_Uart:发送串口
*          ucByte:要发的字节
* 返 回 值: 无
* 说    明: 无
**/
HAL_StatusTypeDef FIFO_UartSendByte(UART_FIFO_Typedef_t *pFIFO_Uart, uint8_t ucByte)
{
    return FIFO_UartSendBuffer(pFIFO_Uart, &ucByte, 1);
}

/**
  * 函数功能: 供中断服务程序调用，通用串口中断处理函数
  * 输入参数: pFIFO_Uart:发送串口
  * 返 回 值: 无
  * 说    明: 放在串口IRQHandler中断里面
  */
void FIFO_UartIRQ(UART_FIFO_Typedef_t *pFIFO_Uart)
{
    if (pFIFO_Uart)
    {
        /* 判断是否初始化完成  */
        if (pFIFO_Uart->initDoneFlag)
        { /* 处理接收中断  */
            if (__HAL_UART_GET_IT_SOURCE(pFIFO_Uart->huart, UART_IT_RXNE))
            {
                /* 从串口接收数据寄存器读取数据存放到接收FIFO */
                uint8_t ch;
                if (UartReadByte(pFIFO_Uart->huart, &ch) == HAL_OK && pFIFO_Uart->pRxBuf)
                {
                    pFIFO_Uart->pRxBuf[pFIFO_Uart->usRxWrite] = ch;
                    if (++pFIFO_Uart->usRxWrite >= pFIFO_Uart->usRxBufSize)
                    {
                        pFIFO_Uart->usRxWrite = 0;
                    }
                    if (pFIFO_Uart->usRxCount < pFIFO_Uart->usRxBufSize)
                    {
                        pFIFO_Uart->usRxCount++;
                    }
                    else
                    {
                        pFIFO_Uart->usRxRead = pFIFO_Uart->usRxWrite;
                    }
                    /* 回调函数,通知应用程序收到新数据,一般是发送1个消息或者设置一个标记 */
                    if (pFIFO_Uart->ReceiveNewFunc)
                    {
                        pFIFO_Uart->ReceiveNewFunc(ch);
                    }
                }
            }
            /* 处理发送缓冲区空中断 */
            if (__HAL_UART_GET_IT_SOURCE(pFIFO_Uart->huart, UART_IT_TXE))
            {
                if (pFIFO_Uart->usTxCount == 0)
                {
                    /* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
                    __HAL_UART_DISABLE_IT(pFIFO_Uart->huart, UART_IT_TXE);

                    /* 数据发送完毕中断 */
                    if (pFIFO_Uart->SendOverFunc)
                    {
                        pFIFO_Uart->SendOverFunc();
                    }
                }
                else
                {
                    /* 从发送FIFO取1个字节写入串口发送数据寄存器 */
                    if (UartSendByte(pFIFO_Uart->huart, pFIFO_Uart->pTxBuf[pFIFO_Uart->usTxRead]) == HAL_OK)
                    {
                        if (++pFIFO_Uart->usTxRead >= pFIFO_Uart->usTxBufSize)
                        {
                            pFIFO_Uart->usTxRead = 0;
                        }
                        pFIFO_Uart->usTxCount--;
                    }
                }
            }
        }
    }
}
