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
#include "bsp_ili9341_lcd.h"

DHT11_Data_TypeDef DHT11_Data;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	/* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();
	
	LCD_Init();	
	LCD_Clear(0, 0, 240, 320, BACKGROUND);
	LCD_DispStr(10, 90, (uint8_t *)"This is a dht11 test demo", WHITE);

	USART1_Config();
	printf("\r\n***Ұ��ISO dht11 ��ʪ�ȴ�����ʵ��***\r\n");
	
	/*��ʼ��DTT11������*/
	DHT11_GPIO_Config(); 	

	/*����Read_DHT11��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
	if( Read_DHT11(&DHT11_Data)==SUCCESS)
	{
		printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
		DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
		printf("\r\n ʪ��:%d,�¶�:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
		
		LCD_DispStr(10, 110, (uint8_t *)"humidity:", WHITE);
		LCD_DisNum(64, 110, DHT11_Data.humi_int, WHITE);
		
		LCD_DispStr(10, 130, (uint8_t *)"temperature:", WHITE);
		LCD_DisNum(82, 130, DHT11_Data.temp_int, WHITE);
	}
	else
	{
		printf("Read DHT11 ERROR!\r\n");
		LCD_DispStr(10, 110, (uint8_t *)"Read DHT11 ERROR", WHITE);
	}
	while(1); 	
}
/*********************************************END OF FILE**********************/
