//
// Created by PC on 2026/9/14.
//

#include "SVPWM.h"
#include <stdio.h>
#include <math.h>
#define deg_to_rad(a) (M_PI * (a) / 180)


/**
 * @brief 极坐标系下的svpwm
 *
 * @param theta 目标磁矢量角度
 * @param s 目标磁矢量强度
 * @return duty_t 三相桥臂占空比
 */
duty_t svpwm(float theta, float s)
{
    const float rad60 = deg_to_rad(60);
    const int v[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
    int sector = 1 + theta / rad60;
    float t_m = s * sinf(sector * rad60 - theta);
    float t_n = s * sinf(theta - (sector * rad60 - rad60));
    float t_0 = 1 - t_m - t_n;

    duty_t duty;
    duty.d_u = t_m * v[sector - 1][0] + t_n * v[sector % 6][0] + t_0 / 2;
    duty.d_v = t_m * v[sector - 1][1] + t_n * v[sector % 6][1] + t_0 / 2;
    duty.d_w = t_m * v[sector - 1][2] + t_n * v[sector % 6][2] + t_0 / 2;
    return duty;
}



// demo用例
  // int main()
  // {
  //     for (float phi = 0; phi < 360; phi += 10)
  //     {
  //         // 这里我设置磁矢量与转子垂直，这样转子受力最大
  //         duty_t duty = svpwm(deg_to_rad(fmodf(phi + 90, 360)), 1);
  //         printf("%f,%f,%f,\r\n", duty.d_u, duty.d_v, duty.d_w);
  //     }
  //     return 0;
  // }
