//
// Created by PC on 2026/9/15.
//

#include "main.h"
#include "motor.h"
#include "SVPWM.h"
#include "motor_config.h"
#include "math.h"
#include "tim.h"
#include "uart_printf.h"

float theta = 0.0f;
duty_t duty ;

float motor_goal_set()
{

    float goal_speed_rad_s = 10.0f ;//机械转子转速
    float goal_electrical_speed = goal_speed_rad_s * POLE_PAIRS;//转换为电角速度
    float d_theta = goal_electrical_speed * (1.0f / (APP_COMPUTE_KHZ * 1000.0f));

    return d_theta ;



}

void motor_control()
{
    theta = theta + motor_goal_set();

    if (theta >= 2.0f * M_PI )
    {
        theta = theta - 2.0f * (float)M_PI ;
    }
    else if (theta <= -2.0f * M_PI )
    {
        theta = theta + 2.0f * (float)M_PI ;
    }

    duty = svpwm(theta, 0.1f);

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty.d_u *  __HAL_TIM_GET_AUTORELOAD(&htim1));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty.d_v *  __HAL_TIM_GET_AUTORELOAD(&htim1));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty.d_w *  __HAL_TIM_GET_AUTORELOAD(&htim1));



}

void motor_init()
{

}