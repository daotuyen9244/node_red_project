/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   dht11��ʪ�ȴ���������ʵ��
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
#include "bsp_SysTick.h"
#include "bsp_dht11.h"
#include "bsp_usart1.h"

DHT11_Data_TypeDef DHT11_Data;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	/* ����SysTick Ϊ1us�ж�һ�� */
	SysTick_Init();

	USART1_Config();
	printf("\r\n***Ұ��ISO dht11 ��ʪ�ȴ�����ʵ��***\r\n");

	/*��ʼ��DTT11������*/
	DHT11_GPIO_Config();

	/*����Read_DHT11��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
	if( Read_DHT11(&DHT11_Data)==SUCCESS)
	{
		printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
		DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
		//printf("\r\n ʪ��:%d,�¶�:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
	}
	else
	{
		printf("Read DHT11 ERROR!\r\n");
	}
	while(1); 	
}
/*********************************************END OF FILE**********************/
