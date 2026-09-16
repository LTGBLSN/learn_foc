//
// Created by PC on 2026/9/3.
//

#include "LED_TASK.h"
#include "main.h"
#include "tim.h"
#include "DEBUG_UART_TASK.h"
#include "EG2104.h"
#include "motor.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim1.Instance)                        //20KHZ   50US,电机控制
    {
        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin) == 0)
        {
            EG2104_close();                                     //关mos
        }
        else
        {
            EG2104_open();                                      //开mos
            motor_control();
        }


    }

    if(htim->Instance == htim2.Instance)                        //10KHZ   100US,app循环
    {
        LED_TASK();
        DEBUG_UART_TASK();
        motor_goal_set();

    }
}
