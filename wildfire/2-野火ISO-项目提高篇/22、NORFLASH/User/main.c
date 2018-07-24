/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   nordflash��д����
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
#include "bsp_fsmc_nor.h"

/* 2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
 * 64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
 * 64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
 * 64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
 * 64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF
*/

/*
 * NOR �ͺ�: S29GL128P
 *      ����: 16MB ,ÿ������128K�ֽڣ�һ��128������
 * ���ʷ�ʽ : 16bit ����
 * ��ַ     : 0x6400 0000 - 0x64FF FFFF
 */

 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	/* ���ô���Ϊ 115200 8-N-1 */
	USART1_Config();  

  NORFLASH_Test();
  
  while(1);
}
/*********************************************END OF FILE**********************/
