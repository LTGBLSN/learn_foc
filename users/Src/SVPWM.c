//
// Created by PC on 2026/9/14.
//

#include "SVPWM.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "main.h"
#define deg_to_rad(a) (M_PI * (a) / 180)



/**
 * @brief 笛卡尔坐标系下的svpwm
 *
 * @param phi 转子角度
 * @param d d轴强度单位比例
 * @param q q轴强度单位比例
 * @return duty_t 三相桥臂占空比
 */
duty_t c_svpwm(float phi, float d, float q)
{
    const float rad60 = deg_to_rad(60);
    const int v[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
    const int K_to_sector[] = {4, 6, 5, 5, 3, 1, 2, 2};
    float cos_phi = cosf(phi);
    float sin_phi = sinf(phi);
    float alpha = cos_phi * d - sin_phi * q;
    float beta = sin_phi * d + cos_phi * q;

    bool A = beta > 0;
    bool B = fabsf(beta) > M_SQRT3 * fabsf(alpha);
    bool C = alpha > 0;

    int K = 4 * A + 2 * B + C;
    int sector = K_to_sector[K];

    float t_m = sinf(sector * rad60) * alpha - cosf(sector * rad60) * beta;
    float t_n = beta * cosf(sector * rad60 - rad60) - alpha * sinf(sector * rad60 - rad60);
    float t_0 = 1 - t_m - t_n;

    duty_t duty;
    duty.d_u = t_m * v[sector - 1][0] + t_n * v[sector % 6][0] + t_0 / 2;
    duty.d_v = t_m * v[sector - 1][1] + t_n * v[sector % 6][1] + t_0 / 2;
    duty.d_w = t_m * v[sector - 1][2] + t_n * v[sector % 6][2] + t_0 / 2;
    return duty;
}

// int main()
// {
//     for (float phi = 0; phi < 360; phi += 10)
//     {
//         duty_t duty = c_svpwm(deg_to_rad(phi), 0, 1);
//         printf("%f,%f,%f,\r\n", duty.d_u, duty.d_v, duty.d_w);
//     }
//     return 0;
// }

/**
 * @brief 极坐标系下的svpwm
 *
 * @param theta 目标磁矢量角度
 * @param s 目标磁矢量强度
 * @return duty_t 三相桥臂占空比
 */
duty_t p_svpwm(float theta, float s)
{
    const float two_pi = 2.0f * (float)M_PI;
    const float rad60 = two_pi / 6.0f;
    const int v[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};

    /* Keep theta in [0, 2*pi) so the sector indexes stay within v. */
    theta = fmodf(theta, two_pi);
    if (theta < 0.0f)
    {
        theta += two_pi;
    }

    /* s is the linear-region modulation index. */
    if (s < 0.0f)
    {
        s = 0.0f;
    }
    else if (s > 1.0f)
    {
        s = 1.0f;
    }

    const int sector = (int)floorf(theta / rad60);
    const int next_sector = (sector + 1) % 6;
    const float alpha = theta - (float)sector * rad60;
    const float t_m = s * sinf(rad60 - alpha);
    const float t_n = s * sinf(alpha);
    float t_0 = 1.0f - t_m - t_n;

    /* Avoid a tiny negative zero-vector time caused by floating-point roundoff. */
    if (t_0 < 0.0f)
    {
        t_0 = 0.0f;
    }

    duty_t duty;
    duty.d_u = t_m * v[sector][0] + t_n * v[next_sector][0] + t_0 / 2.0f;
    duty.d_v = t_m * v[sector][1] + t_n * v[next_sector][1] + t_0 / 2.0f;
    duty.d_w = t_m * v[sector][2] + t_n * v[next_sector][2] + t_0 / 2.0f;
    return duty;
}



// demo用例
  // int main()
  // {
  //     for (float phi = 0; phi < 360; phi += 10)
  //     {
  //         // 这里我设置磁矢量与转子垂直，这样转子受力最大
  //         duty_t duty = p_svpwm(deg_to_rad(fmodf(phi + 90, 360)), 1);
  //         printf("%f,%f,%f,\r\n", duty.d_u, duty.d_v, duty.d_w);
  //     }
  //     return 0;
  // }
