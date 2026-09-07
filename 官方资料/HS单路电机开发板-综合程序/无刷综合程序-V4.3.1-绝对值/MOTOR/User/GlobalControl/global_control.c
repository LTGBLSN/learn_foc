
/**
  ******************************************************************************
  * 文件名程: 
  * 作    者: 浩然
  * 版    本: V1.0
  * 编写日期: 
  * 功    能: 
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/

#include "global_control.h"
#include "motor_system.h"
#include "led_task.h"
#include "lcd_task.h"
#include "lcd_mid.h"
#include "key_task.h"
#include "usart_task.h"

extern volatile u16 LedTaskTim;
extern volatile u16 LcdTaskTim;
extern volatile u16 KeyTaskTim;
extern volatile u16 UsartTaskTim;

/**
  * 函数功能: 全局初始化
  * 输入参数:
  * 返回参数:
  * 说    明: 
  */
void Global_Init(void)
{
    HAL_Delay(100);                                         //延时等待电源稳定
    LCD_Display_Logo();                                     //LCD显示浩盛LOGO
    Motor_System_Init();                                    //电机系统初始化
    
    HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);   //ADC校准
    HAL_Delay(10);                                          //等待ADC校准完成
    HAL_ADC_Start_DMA(&hadc2,(u32 *)MC.Sample.AdcBuff,3);   //启动ADC开启DMA搬运
    
//    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);         //启动编码器接口 

    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);          //设置初始占空比
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);          //设置初始占空比
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);          //设置初始占空比

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);               //开启对应通道PWM输出
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);               //开启对应通道PWM输出
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);               //开启对应通道PWM输出

    HAL_TIM_Base_Start_IT(&htim1);                          //开启定时器中断
    HAL_TIM_Base_Start_IT(&htim2);                          //开启定时器中断
    HAL_TIM_Base_Start(&htim6);
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);     //使能SD1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);     //使能SD2
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);     //使能SD3
}

/**
  * 函数功能: 主循环
  * 输入参数:
  * 返回参数:
  * 说    明: 
  */
void Global_Loop(void)
{
    Lcd_Task();
}

/**
  * 函数功能: 定时器中断回调函数
  * 输入参数:
  * 返回参数:
  * 说    明: 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim1.Instance)                        //20KHZ   50US
    {
        HAL_ADCEx_InjectedStart_IT(&hadc2);                     //开启ADC注入通道中断
    }
    
    if(htim->Instance == htim2.Instance)                        //10KHZ   100US
    {
        LedTaskTim++;                                           //LED任务计时
        LcdTaskTim++;                                           //LCD任务计时
        KeyTaskTim++;                                           //按键扫描任务计时
        UsartTaskTim++;                                         //串口任务计时
        
        Led_Task();
        Key_Task();
        Usart_Task();
    }
}

/**
  * 函数功能: 编码器原始数据读取
  * 输入参数: 
  * 返回参数: 编码器原始位置信息
  * 说    明: 
  */
u16 Encoder_Data_Get(void)
{
    /*注释部分为KTH7823绝对值模式，使用需要修改引脚配置（CUBEMX）以及更改编码器板接线*/
    u8 angle_reg[2] = {0x00,0x00};
    u8 reg_org_data[2] = {0};
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi1, (u8 *)angle_reg, (u8 *)&reg_org_data[0], 2, 0xFFFF);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_SET);
    return (reg_org_data[0] << 8 | reg_org_data[1]) >> 2;
//    return TIM3->CNT;
}

void Target_Set(void)
{
    switch(MC.Motor.RunMode)
    {
        case CURRENT_CLOSE_LOOP:                                           //电流闭环
        {                        
            MC.IqPid.Ref = MC.Sample.AdcBuff[1] * 0.002f;                    //使用波轮电位器给电机目标电流（电流闭环模式下）
        }break;    
        
        case SPEED_CURRENT_LOOP:                                           //速度闭环
        {        
            MC.Speed.MechanicalSpeedSet  =  Speed_Set_Dir * MC.Sample.AdcBuff[1] * 0.5f;            //使用波轮电位器给电机目标转速（速度闭环模式下）
            if(MC.Speed.MechanicalSpeedSet <= 5 && MC.Speed.MechanicalSpeedSet >= -5)
            {
                MC.Speed.MechanicalSpeedSet = 0;                               //消除电位器在0位附近采样值抖动引起电机蠕动
            }                
        }break;    
        
        case POS_SPEED_CURRENT_LOOP:                                       //位置闭环
        {
            MC.Position.MechanicalPosSet = -MC.Sample.AdcBuff[1];            //使用波轮电位器给电机目标位置（位置闭环模式下）
        }break;    
        case STRONG_DRAG_SMO_SPEED_CURRENT_LOOP:
        {    
            MC.Speed.MechanicalSpeedSet  =  Speed_Set_Dir * MC.Sample.AdcBuff[1] * 0.5f;             //使用波轮电位器给电机目标转速（速度闭环模式下）
        }break;                
        
        case HFI_SMO_SPEED_CURRENT_CLOSE:                                      
        {       
            MC.Speed.MechanicalSpeedSet  =  Speed_Set_Dir * MC.Sample.AdcBuff[1] * 0.5f;            //使用波轮电位器给电机目标转速（速度闭环模式下）
            if(MC.Speed.MechanicalSpeedSet <= 5 && MC.Speed.MechanicalSpeedSet >= -5)
            {
                MC.Speed.MechanicalSpeedSet = 0;                               //消除电位器在0位附近采样值抖动引起电机蠕动
            }                
        }break;    
    }
}

/**
  * 函数功能: ADC注入中断回调函数
  * 输入参数:
  * 返回参数:
  * 说    明: 20KHZ频率即50US执行一次
  */
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{        
    MC.Sample.IuRaw = ADC2->JDR1;              //获取相电流
    MC.Sample.IwRaw = ADC2->JDR2;              //获取相电流
    MC.Sample.BusRaw = MC.Sample.AdcBuff[0];   //获取母线电压
    MC.EAngle.EncoderVal = Encoder_Data_Get(); //获取编码器值
    Target_Set();                              //电位器给定目标值
    Motor_System_Run();                        //电机系统运行

    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,MC.Foc.DutyCycleA);     //更新PWM比较值
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,MC.Foc.DutyCycleB);     //更新PWM比较值
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,MC.Foc.DutyCycleC);     //更新PWM比较值
}





