/**
  ******************************************************************************
  * @file    main.c
  * @author  armfly
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   NANDFLASH ����
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
#include "bsp_nand.h"
#include "bsp_led.h"

void NAND_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
/*
 * PG9 �ӵ��� NORFLASH �� CE �ˣ�Ϊ�˲��� NORFLASH���ź� Ӱ�쵽 NANDFLASH
 * �������� CE = 1 ,�� PG9 = 1 ,��Ȼ SRAM ����Բ��ɹ�.
 * ������Ӳ�����ʱ��NORFLASH �� CE �ܽ� ������.
 */
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_SetBits(GPIOG,GPIO_Pin_9);
  
  LED_GPIO_Config();	
  
	/* USART1 config 115200 8-N-1 */
	USART1_Config();
  printf("\r\n ��ʼ��ʱ���е㳤�������ĵȴ�...... \r\n");
	NAND_Init();
  
  /* ID = ADF1801D */	
  printf("\r\n NANDFLASH id Ϊ %x \r\n", NAND_ReadID());
  printf("\r\n NANDFLASH Test finished \r\n");
  
	for(;;)
	{
    LED2_TOGGLE;
    NAND_Delay(0x0FFFFF);
	}
}
/*********************************************END OF FILE**********************/
