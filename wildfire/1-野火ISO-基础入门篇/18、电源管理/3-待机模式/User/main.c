/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ��������ʵ��
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
#include "bsp_exti.h" 
#include "bsp_usart1.h"

static void Delay(__IO u32 nCount);


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */ 
int main(void)
{	
	/* config the led */
	LED_GPIO_Config();
	
	/* exti line config */
	EXTI_Key_Config(); 
	
	USART1_Config();	
	
	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET)
	{
		printf("\r\n ʹ�ܵ�Դ����ʱ�ӵ�Ԫǰ�ļ�⣬�������Ѹ�λ \r\n");

	}
	else
		printf("\r\n ʹ�ܵ�Դ����ʱ�ӵ�Ԫǰ�ļ�⣬�ϵ縴λ \r\n");
	
	
	/* ʹ�ܵ�Դ����Ԫ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	
	
	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET)
	{
		printf("\r\n ʹ�ܺ��⣬�������Ѹ�λ \r\n");

	}
	else
		printf("\r\n ʹ�ܺ��⣬�ϵ縴λ \r\n");
	
	
	while(1)                            
	{	
		/* wait interrupt */
		LED1( ON );			  // ��
		Delay(0xFFFFF);
		LED1( OFF );		  // ��

		LED2( ON );			  // ��
		Delay(0xFFFFF);
		LED2( OFF );		  // ��

		LED3( ON );			  // ��
		Delay(0xFFFFF);
		LED3( OFF );		  // ��	
	
	}
}


static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/*********************************************END OF FILE**********************/
