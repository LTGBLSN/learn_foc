//
// Created by PC on 2026/9/7.
//
#include "main.h"
#include "stm32g4xx_hal_gpio.h"
#include "uart_printf.h"

void DEBUG_UART_TASK()
{
    //待测试频率是否下降
    static uint8_t cnt = 0;

    if (++cnt >= 10)   // 10kHz / 10 = 1kHz
    {
        cnt = 0;
        usart3_printf("%d,%d \r\n",
                      HAL_GetTick(),
                      HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin));

    }
}