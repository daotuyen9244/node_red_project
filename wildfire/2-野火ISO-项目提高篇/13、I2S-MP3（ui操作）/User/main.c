/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ���Mp3(helix�����)
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
#include "bsp_pcm1770.h"
#include "bsp_iis.h"
#include "mp3.h"




/**
  * @brief  mp3���main��������Ҫ��SD���ݵ��ļ�������SD���ϣ�Ȼ���SD���嵽������Ŀ�����
	*					������֧�����ĳ��ļ����������س����ʱ��Ƚ�����
	*					Ϊ��ʡʱ�䣺���Ե�ʱ����԰�ffconf.h�е�codepage936���� 437������cc936.c�ļ�����ccsbcs.c�ļ�		
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
	 //	SysTick_Init();			//�����̰Ѵ�������������ʱ���÷Ǿ�ȷ��ʱ������Ҫsystick��
	 
		/* ��ʼ��LED */
		LED_GPIO_Config();
	
		/* ��ʼ���ⲿFLASH */
		SPI_FLASH_Init();
		//SPI_FLASH_SectorErase(0);
  
		/* ������IO���жϳ�ʼ�� */
		Touch_Init();		
		
		/* ��ʼ������I2S */
		I2S_Bus_Init();
		
		/* ��ʼ��PCM1770 */
		PCM1770Init();
		
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
	
		/* ��ʾMP3����ͼƬ */
		Lcd_show_bmp(0, 0,"/mp3player/ui_window.bmp");

		/* ����MP3������ */	
		player_run();
		
		while(1);

}


/******************* end of file**************************/

