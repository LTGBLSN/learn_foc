//
// Created by PC on 2026/9/3.
//

#include "LED_TASK.h"
#include "main.h"
#include "tim.h"
#include "DEBUG_UART_TASK.h"
#include "EG2104.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim1.Instance)                        //20KHZ   50US
    {
        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == 0)
        {
            EG2104_close();
        }
        else
        {
            EG2104_open();
        }


    }

    if(htim->Instance == htim2.Instance)                        //10KHZ   100US
    {
        LED_TASK();
        DEBUG_UART_TASK();
    }
}