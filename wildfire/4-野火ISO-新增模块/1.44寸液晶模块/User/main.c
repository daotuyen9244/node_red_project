/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
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
#include "bsp_led.h"
#include "bsp_lcd_st7735s.h"
#include "FIRE_lcd.h"
#include "FIRE_key_event.h"
#include "bsp_SysTick.h"

void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	Site_t site= {0,0};
	
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	/* LCD��ʼ�� */
	LCD_init();

	SysTick_Init();
	
	site.x=20;site.y=20;
	LCD_str(site,"LCD test OK",BLUE,RED);

	key_event_init();

	/* ����2��ʹ�ù̼������IO */
	while (1)
	{	
		deal_key_event();
	}

}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
