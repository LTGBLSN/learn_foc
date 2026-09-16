//
// Created by PC on 2026/9/3.
//

#include "main.h"
#include "LED_TASK.h"

#define LED_TASK_TOGGLE_PERIOD_TICKS 500U

void LED_TASK(void)
{
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
    static uint16_t led_task_tick = 0;

    led_task_tick++;
    if (led_task_tick >= LED_TASK_TOGGLE_PERIOD_TICKS)
    {
        led_task_tick = 0;
        HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
    }
}
