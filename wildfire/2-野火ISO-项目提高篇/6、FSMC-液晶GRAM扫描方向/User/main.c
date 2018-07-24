/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����lcd��ʾӢ���ַ�
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
#include "bsp_ili9341_lcd.h"

extern void Lcd_Delay(__IO uint32_t nCount);

uint32_t n;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	LCD_Init();
	
  /* ͨ���޸�GRAM��ɨ�跽��Ȼ��ͨ����Һ��������ɫ�飬�Ϳ���֪��GRAM��ʵ��ɨ�跽���� */
  Lcd_GramScan(2);
  LCD_Clear(0, 0, 320, 240, BACKGROUND);
  LCD_DispStr(10, 110, (uint8_t *)"LCD Gram test123456456789456123456789......", RED);
  
	LCD_Clear(0, 0, 100, 10, RED);
  //LCD_Clear(30, 10, 1, 20, RED);
//	LCD_DispChar(60, 60, 'A', RED);
//	
//	LCD_DispStr(10, 10, (uint8_t *)"This is a lcd demo to display ascii", RED);	
//	
//	LCD_DispStr(40, 100, (uint8_t *)"count:", RED);
//	
//	for( n=0; n<500000; n++ )
//	{
//		LCD_DisNum(100, 100, n, RED);
//		Lcd_Delay(0xAFFf>>4);
//	}
	
	while( 1 ){}
}


