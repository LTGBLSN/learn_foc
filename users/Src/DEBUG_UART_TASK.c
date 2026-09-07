//
// Created by PC on 2026/9/7.
//
#include "main.h"
#include "stm32g4xx_hal_gpio.h"
#include "uart_printf.h"

void DEBUG_UART_TASK()
{

    usart3_printf("%d,%d \r\n",HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin),HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin));



}