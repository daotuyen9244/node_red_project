/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   USART printf example
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

 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
		/* USART1 config 115200 8-N-1 */
		USART1_Config();

		printf("\r\n this is a usart printf demo \r\n");

		printf("\r\n ��ӭʹ��Ұ�� iSO STM32 ������ \r\n");		

		for(;;)
		{
			
		}
}
/*********************************************END OF FILE**********************/
