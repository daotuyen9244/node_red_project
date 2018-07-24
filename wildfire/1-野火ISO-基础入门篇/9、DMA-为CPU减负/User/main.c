/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usart1 DMA TX ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */  
 
// DMA  data memory access 

#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_led.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
static void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
		/* USART1 config 115200 8-N-1 */
		USART1_Config();
	
		USART1_DMA_Config();
	
		LED_GPIO_Config();
	
		printf("\r\n usart1 DMA TX ���� \r\n");
	
		{
			uint16_t i;
			
			/*��佫Ҫ���͵�����*/
			for(i=0;i<SENDBUFF_SIZE;i++)
			{
				SendBuff[i]	 = 'A';
			}
		}
		
		/* USART1 �� DMA����TX���� */
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

		/* ��ʱCPU�ǿ��еģ����Ը����������� */
		
		//����ͬʱ����LED
		for(;;)
		{
			LED1(ON);
			Delay(0xFFFFF);
			LED1(OFF);
			Delay(0xFFFFF);
		}
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
