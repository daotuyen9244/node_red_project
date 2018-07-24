/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   Һ����ʾ����
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

/*
 * Ҫ��������ʾ�ɹ�
 * ����ֿ⿽����sd���ϣ�Ȼ���sd���嵽������Ŀ�����
 */
int main(void)
{
   /* USART1 config */
		//USART1_Config();
		//printf("\r\n this is a fatfs test demo \r\n");
	
		LCD_Init();
	
		/* ����Lcd Gram ɨ�跽��Ϊ: ���½�->���Ͻ� */		
		Lcd_GramScan( 3 );	
		LCD_Clear(0, 0, 320, 240, BACKGROUND);	
		
		/* ��ʼ��sd���ļ�ϵͳ����Ϊ���ֵ��ֿ��bmpͼƬ������sd������ */
		Sd_fs_init();
	
		/* ��ʾsd�������bmpͼƬ */
		Lcd_show_bmp(0, 0,"/wildfire.bmp");
		Lcd_show_bmp( 10,10,"/pic1.bmp");		
    Lcd_show_bmp( 170,0,"/pic2.bmp"); 	
	
		Screen_shot(0,0,320,240,"/myscreen");
		
		while(1);
}

	  

