//
// Created by PC on 2026/9/3.
//


#include "main.h"
#include "tim.h"
#include "EG2104.h"


void EG2104_init()
{
    EG2104_open();

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0.5f *  __HAL_TIM_GET_AUTORELOAD(&htim1));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0.5f *  __HAL_TIM_GET_AUTORELOAD(&htim1));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0.5f *  __HAL_TIM_GET_AUTORELOAD(&htim1));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0.5f *  __HAL_TIM_GET_AUTORELOAD(&htim1));



}

void EG2104_close()
{
    HAL_GPIO_WritePin(SD1_GPIO_Port,SD1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SD2_GPIO_Port,SD2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SD3_GPIO_Port,SD3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SD4_GPIO_Port,SD4_Pin, GPIO_PIN_RESET);
}

void EG2104_open()
{
    HAL_GPIO_WritePin(SD1_GPIO_Port,SD1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SD2_GPIO_Port,SD2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SD3_GPIO_Port,SD3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SD4_GPIO_Port,SD4_Pin, GPIO_PIN_SET);
}
