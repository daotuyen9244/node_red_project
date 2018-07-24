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
#include "bsp_SysTick.h"
#include "bsp_touch.h"
#include "bsp_spi_flash.h"
#include "bsp_led.h" 
#include "bsp_usart1.h"


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	uint8_t k;
  
  /* ϵͳ��ʱ�� 1us ��ʱ��ʼ�� */
  SysTick_Init();
 
  LED_GPIO_Config();
  
  LCD_Init();	
  /* GRAMɨ�跽��Ϊ���½�->���Ͻ� */
  Lcd_GramScan(2);
  LCD_Clear(0, 0, 320, 240, BACKGROUND);
  
  USART1_Config();
  
  SPI_FLASH_Init();
  //SPI_FLASH_SectorErase(0);
  
  /* ������IO���жϳ�ʼ�� */
  Touch_Init();
/*-------------------------------------------------------------------------------------------------------*/  
  #if 1
  SPI_FLASH_BufferRead(&cal_flag, 0, 1);
  if( cal_flag == 0x55 )
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
/*------------------------------------------------------------------------------------------------------*/  
  //Touch_Init();
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
        LED1_TOGGLE;       
      }
    }		
  }
}

/* ------------------------------------------end of file---------------------------------------- */

