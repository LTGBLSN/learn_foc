//
// Created by PC on 2026/9/7.
//
#include "main.h"
#include "stm32g4xx_hal_gpio.h"
#include "uart_printf.h"
#include "motor.h"

void DEBUG_UART_TASK()
{

    static uint8_t cnt = 0;

    if (++cnt >= 10)   // 10kHz / 10 = 1kHz
    {
        cnt = 0;
        usart3_printf("%f,%f,%f \r\n",
            duty.d_u,
            duty.d_v,
            duty.d_w);
    }
}