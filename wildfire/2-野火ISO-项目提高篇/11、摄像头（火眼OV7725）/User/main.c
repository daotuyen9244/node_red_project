/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����ͷ����ov7725��������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "bsp_ov7725.h"
#include "bsp_ili9341_lcd.h"

extern uint8_t Ov7725_vsync;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void) 	
{
	/* Һ����ʼ�� */
	LCD_Init();
	
	/* ov7725 gpio ��ʼ�� */
	Ov7725_GPIO_Config();
	
	/* ov7725 �Ĵ������ó�ʼ�� */
	while(Ov7725_Init() != SUCCESS);
	
	/* ov7725 ���ź��߳�ʼ�� */
	VSYNC_Init();	
	Ov7725_vsync = 0;
	
	while(1)
	{
		if( Ov7725_vsync == 2 )
		{
			FIFO_PREPARE;  			/*FIFO׼��*/					
			ImagDisp();					/*�ɼ�����ʾ*/
			Ov7725_vsync = 0;			
		}
	}
}

/*********************************************END OF FILE**********************/

