/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ��can����ʵ��(�ж�ģʽ�ͻػ�)������������Ϣͨ��USART1�ڳ����ն��д�ӡ������         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"
#include "can.h" 

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */

int main(void)
{	
	/* USART1 config */
	USART1_Config();
	
	/* LED config */
	LED_GPIO_Config();	
	
	printf( "\r\n ���һ��CAN���ػ�ģʽ���ж�ģʽ�����Գ���...... \r\n" );
	
	USER_CAN_Init();	
	printf( "\r\n CAN �ػ����Գ�ʼ���ɹ�...... \r\n" );
	
	USER_CAN_Test();	
	printf( "\r\n CAN �ػ����Գɹ�...... \r\n" );
	
	
	while (1)
	{
	
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
