/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   Һ����������ʵ��
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
#include "bsp_SysTick.h"
#include "bsp_touch.h"

extern volatile unsigned char touch_flag;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	/* ϵͳ��ʱ�� 1us ��ʱ��ʼ�� */
  SysTick_Init();
  
  LCD_Init();	
  /* GRAMɨ�跽��Ϊ���½�->���Ͻ� */
  Lcd_GramScan(2);
  LCD_Clear(0, 0, 320, 240, BACKGROUND);
  
/*------------------------------------------------------------------------------------------------------*/
  
  /* ������IO���жϳ�ʼ�� */
  Touch_Init();
  
  /* �ȴ�������У����� */
  while(Touch_Calibrate() !=0);
  
  /* ����ȡɫ���ʼ�� */
  Palette_Init();
  
	while( 1 )
  {
    if(touch_flag == 1)			/*������ʰ�����*/
    {
      /*��ȡ�������*/
      if(Get_touch_point(&display, Read_2046_2(), &touch_para ) !=DISABLE)      
      {					
        Palette_draw_point(display.x,display.y);	 										
      }
    }		
  }
}

/* ------------------------------------------end of file---------------------------------------- */

