/**
  ******************************************************************************
  * 文件名程: key_drv.c
  * 作    者: 浩然
  * 版    本: V1.0
  * 编写日期: 2024-03-29
  * 功    能: 
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "key_drv.h"

u8 KeyNum = 0;

button_t button[2] = {0};

/**
  * 函数功能: 按键扫描函数
  * 输入参数: 
  * 返 回 值: 键值
  * 说    明：非传统延时消抖方法
  */
static void ReadKey(void)    //读取按键值
{
    button[0].level = KEY1;    //按键1
    button[1].level = KEY2;    //按键2
}

void Key_Process(void)
{
	uint8_t index = 0;
	
	for(index = 0;index < 2;index++)
	{
		switch(button[index].status)
		{
			case 0:
			if(button[index].level == 0)
			{
				button[index].scan_cnt = 0;
				button[index].status = 1;
			}
			break;
			case 1:
			if(button[index].level == 0)
			{
				button[index].scan_cnt++;
			if(button[index].scan_cnt >= 50)
			{
					if(index == 0)
					{
							KeyNum = 2;
							button[index].status = 2;
					}
					if(index == 1)
					{
							KeyNum = 4;
							button[index].status = 2;
					}
			}
			}
			else
			{
				if(button[index].scan_cnt <= 50)
				{
						if(index == 0)
						{
								KeyNum = 1;
								button[index].status = 0;
						}
						if(index == 1)
						{
								KeyNum = 3;
								button[index].status = 0;
						}
				}
			}
			break;
			case 2:
			if(button[index].level == 1)
			{
				button[index].status = 0;
			}
			break;
		}
	}
}

void Key_Scan()
{
    ReadKey();
    Key_Process();
}
