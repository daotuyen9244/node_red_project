/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_exti.h"
#include "bsp_led.h"
#include "bsp_usart1.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* ����P[A|B|C|D|E]13Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
	
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

	
}

 /**
  * @brief  ���� PC13 �жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC13) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC(PC13) */
	NVIC_Configuration();

	/* EXTI line gpio config(PC13) */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	/* EXTI line(PC13) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); 
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	
	/* EXTI line gpio config(PA0) */	
//	����Ҫ����PA0����
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 					// ��������	
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	/* EXTI line(PA0) mode config */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
//	
//  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�������ж�
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure); 
	
	
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}



 /**
  * @brief  ���ڼ�ⰴ���Ƿ񱻳�ʱ�䰴��
  * @param  ��
  * @retval 1 ����������ʱ�䰴��  0 ������û�б���ʱ�䰴��
  */
uint8_t PWR_Check_Standby(void)
{			
	uint8_t downCnt =0;																				//��¼���µĴ���
	uint8_t upCnt =0;																					//��¼�ɿ��Ĵ���			

	while(1)																										//��ѭ������return����
	{	
		Delay(0xFFFF);																					//�ӳ�һ��ʱ���ټ��

		if(GPIO_ReadInputDataBit (GPIOC,GPIO_Pin_13) == RESET)	//��⵽���°���
		{
			LED1(ON);LED2(ON);LED3(ON);												//��������LED��

			downCnt++;																					//��¼���´���
			upCnt=0;																						//��������ͷż�¼

			if(downCnt>=100)																		//����ʱ���㹻
			{
				LED1(OFF);LED2(OFF);LED3(OFF);	  	 
				return 1; 																				//��⵽������ʱ�䳤����
			}
		}
		else 
		{
			upCnt++; 																						//��¼�ͷŴ���
			if(upCnt>5)																					//������⵽�ͷų���5��
			{
				LED1(OFF);LED2(OFF);LED3(OFF);									//�ر�����LED��	
				return 0;																				//����ʱ��̫�̣����ǰ�����������
			}
		}//	if(GPIO_ReadInputDataBit 

	}//while

}
/*********************************************END OF FILE**********************/
