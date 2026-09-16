//
// Created by PC on 2026/9/14.
//

#ifndef LEARN_FOC_SVPWM_H
#define LEARN_FOC_SVPWM_H

typedef struct duty
{
    float d_u;
    float d_v;
    float d_w;
} duty_t;


duty_t svpwm(float theta, float s);

#endif //LEARN_FOC_SVPWM_H
