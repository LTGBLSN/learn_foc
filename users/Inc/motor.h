//
// Created by PC on 2026/9/15.
//

#ifndef LEARN_FOC_MOTOR_H
#define LEARN_FOC_MOTOR_H
#include "SVPWM.h"

extern float theta ;
extern duty_t duty ;

void motor_control();//电机控制主循环
void motor_init();//电机控制初始化/清错
float motor_goal_set();//电机目标值设定

#endif //LEARN_FOC_MOTOR_H
