/**
  ******************************************************************************
  * 文件名程: key_task.c
  * 作    者: 浩然
  * 版    本: V1.0
  * 编写日期: 2024-03-29
  * 功    能: 
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "key_task.h"
#include "key_drv.h"

volatile u16 KeyTaskId = 10;
volatile u16 KeyTaskTim = 0;

/**
  * 函数功能: 进行LED的工作闪烁流程
  * 输入参数:
  * 返 回 值: 
  * 说    明:
  */
void Key_Task(void)
{
	switch(KeyTaskId)
	{
		case 10:
		{
			if(KeyTaskTim>=100)        //10ms
			{
				KeyTaskTim = 0;
				KeyTaskId = 20;
			}
		}
		break;
		
		case 20:
		{ 
      Key_Scan();
			KeyTaskId = 10; 
		}
		break;
		
    default:
      break;			
	}
}
