/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   Һ����������LED����
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
#include "bsp_usart1.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_sdfs_app.h"
#include "bsp_bmp.h"
#include "bsp_SysTick.h"
#include "bsp_touch.h"
#include "bsp_spi_flash.h"
#include "bsp_led.h" 
#include "even_process.h"




/*
��������ϵͳ��ת����


��������
				column 240
			 
			 x
        _ _ _ _ _ _
       ^           |
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |  320        
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
	(0,0) - - - - - ->  y       
				
Һ����ʾ���弰�����Ⱥ�����
				
				column 240
			 
										x
  (0,0) ----------->
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |  320        
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
			 V- - - - - - 

       y
			 
Һ����ʾͼ��
				
				column 240		 
									 X	
				-----------A
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |  320        
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
			 |           |
		 Y <- - - - - -(0,0) 

       

ת����ϵ��
Y�� = 320 - X����
X�� = Y����

Xͼ�� = X����
Yͼ�� = 240-Y����

Yͼ�� = 240 - X��
Xͼ�� = 320 - Y��


*/




/**
  * @brief  LCD�������Ƽ���ʾ���̣�
	*					ʾ�����ʹ�ô��������˻������Լ�ʹ��LCD��ʾ����
  * @param  ��
  * @retval ��
 */
int main(void)
{
		uint8_t k;
	
		/* ��ʼ��LCD */
		LCD_Init();	
		LCD_Clear(0, 0, 240, 320, BACKGROUND);		
		
		/* ��ʼ��sd���ļ�ϵͳ����Ϊ���ֵ��ֿ������sd������ */
		Sd_fs_init();	
	
		/* ��ʼ������ */
	  USART1_Config();
	
		/* ϵͳ��ʱ�� 1us ��ʱ��ʼ�� */
	 	SysTick_Init();				
	 
		/* ��ʼ��LED */
		LED_GPIO_Config();	
	
		/* ��ʼ���ⲿFLASH */
		SPI_FLASH_Init();
		//SPI_FLASH_SectorErase(0);
  
		/* ������IO���жϳ�ʼ�� */
		Touch_Init();			
		
		/*-------------------------------------------------------------------------------------------------------*/  
		#if 1
		SPI_FLASH_BufferRead(&cal_flag, 0, 1);
		if( cal_flag == 0x45 )
		{
			SPI_FLASH_BufferRead((void*)cal_p, 1, sizeof(cal_p));
			SPI_FLASH_CS_HIGH();
			for( k=0; k<6; k++ )
						printf("\r\n rx = %LF \r\n",cal_p[k]);
		}
		else
		{
			/* �ȴ�������У����� */
			while(Touch_Calibrate() !=0);
		}
		#elif 0
		/* �ȴ�������У����� */
		while(Touch_Calibrate() !=0);
		#endif
	
		/* ��ʾLED���ƽ���ͼƬ */
		Lcd_show_bmp(0, 0,"/led/ui_window.bmp");

		while(1)
		{
			even_process();
		}

}


/******************* end of file**************************/

