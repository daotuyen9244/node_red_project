/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �������ԣ��ж�ģʽ/EXTIģʽ��
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
	/* ���� led */
	LED_GPIO_Config();
	
	/* ����exti�ж� */
	EXTI_Key_Config(); 
	
	/* ���ô���Ϊ�ж�ģʽ */
	USART1_Config();
	
	printf("\r\n Ұ��ISO�����壬˯��ʵ�� \r\n");

	
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
		
		__WFI();					//����˯��ģʽ���ȴ��жϻ���
	}
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}



/*********************************************END OF FILE**********************/
