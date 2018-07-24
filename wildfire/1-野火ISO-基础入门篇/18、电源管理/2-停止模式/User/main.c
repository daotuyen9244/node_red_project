/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ֹͣģʽ����ʵ��
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
#include "bsp_SysTick.h"


void SYSCLKConfig_STOP(void);



/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */ 
int main(void)
{	
	/* ���� led */
	LED_GPIO_Config();

	/* �����ⲿ�ж����� */
	EXTI_Key_Config(); 
	
	/* ���ô��� */
	USART1_Config();
	
  /* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();
	
	printf("\r\n Ұ��ISO�����壬ֹͣģʽʵ�� \r\n");	

	
	/* ʹ�ܵ�Դ����Ԫ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	printf("\r\n ����ֹͣģʽ \r\n");

	/* ����ֹͣģʽ�����õ�ѹ������Ϊ�͹���ģʽ���ȴ��жϻ���*/
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);	
	
	while(1)                            
	{	
		
		/* wait interrupt */
		LED1( ON );			  // ��
		Delay_us(10000);    	// 10000 * 10us = 100ms
		LED1( OFF );		  // ��

		LED2( ON );			  // ��
		Delay_us(10000);   	// 10000 * 10us = 100ms
		LED2( OFF );		  // ��

		LED3( ON );			  // ��
		Delay_us(10000);    	// 10000 * 10us = 100ms
		LED3( OFF );		  // ��	  		
		
	}
}

/**
  * @brief  ͣ�����Ѻ�����ϵͳʱ��: ʹ�� HSE, PLL
  *         ����ѡ��PLL��Ϊϵͳʱ��.
  * @param  None
  * @retval None
  */
void SYSCLKConfig_STOP(void)
{
	ErrorStatus HSEStartUpStatus;
  /* ʹ�� HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* �ȴ� HSE ׼������ */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {

    /* ʹ�� PLL */ 
    RCC_PLLCmd(ENABLE);

    /* �ȴ� PLL ׼������ */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* ѡ��PLL��Ϊϵͳʱ��Դ */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* �ȴ�PLL��ѡ��Ϊϵͳʱ��Դ */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
}
/*********************************************END OF FILE**********************/
