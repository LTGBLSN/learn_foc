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

duty_t compute_p_duty ;
duty_t compute_c_duty ;

static void set_PWM_duty(float d_u , float d_v , float d_w , float duty_max);

float motor_dheta_goal_set()
{

    float goal_speed_rad_s = 20.0f ;//机械转子转速
    float goal_electrical_speed = goal_speed_rad_s * POLE_PAIRS;//转换为电角速度
    float d_theta = goal_electrical_speed * (1.0f / (APP_COMPUTE_KHZ * 1000.0f));

    return d_theta ;



}

void motor_control()
{
    theta = theta + motor_dheta_goal_set();

    if (theta >= 2.0f * M_PI )
    {
        theta = theta - 2.0f * (float)M_PI ;
    }
    else if (theta <= -2.0f * M_PI )
    {
        theta = theta + 2.0f * (float)M_PI ;
    }

    compute_p_duty = p_svpwm(theta, 0.11f);
    compute_c_duty = c_svpwm(theta, 0.0f , 0.11f);

    // set_PWM_duty(compute_p_duty.d_u , compute_p_duty.d_v , compute_p_duty.d_w ,MOTOR_PWM_LIMITER) ;
    set_PWM_duty(compute_c_duty.d_u , compute_c_duty.d_v , compute_c_duty.d_w ,MOTOR_PWM_LIMITER) ;


}


/**
 * @brief 设置三相PWM占空比，并将每相限制在允许范围内。
 * @param d_u U相归一化占空比，输入范围为0.0f到1.0f。
 * @param d_v V相归一化占空比，输入范围为0.0f到1.0f。
 * @param d_w W相归一化占空比，输入范围为0.0f到1.0f。
 * @param duty_max 三相最终输出最大值，限幅形式
 * @return 无；通过更新TIM1的三个比较寄存器输出占空比。
 */
static void set_PWM_duty(float d_u , float d_v , float d_w , float duty_max)
{
    const float duty_min = 0.0f;

    if (d_u < duty_min) d_u = duty_min;
    else if (d_u > duty_max) d_u = duty_max;

    if (d_v < duty_min) d_v = duty_min;
    else if (d_v > duty_max) d_v = duty_max;

    if (d_w < duty_min) d_w = duty_min;
    else if (d_w > duty_max) d_w = duty_max;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, d_u * __HAL_TIM_GET_AUTORELOAD(&htim1));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, d_v * __HAL_TIM_GET_AUTORELOAD(&htim1));
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, d_w * __HAL_TIM_GET_AUTORELOAD(&htim1));
}

void motor_init()
{

}
