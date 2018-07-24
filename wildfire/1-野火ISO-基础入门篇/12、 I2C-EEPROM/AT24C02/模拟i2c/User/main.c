/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   AT24C02���ԣ����ģ��I2C��
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
#include "bsp_i2c_ee.h"

 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
		/* USART1 config 115200 8-N-1 */
		USART1_Config();

		printf("eeprom ���ģ��i2c�������� \r\n");		
  
    ee_Test();
  
		for(;;)		{
			
		}
}
/*********************************************END OF FILE**********************/
