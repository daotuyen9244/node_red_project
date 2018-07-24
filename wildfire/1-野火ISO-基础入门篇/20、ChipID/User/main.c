/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ��ȡоƬ��96bitID��FLASH��С
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
#include "bsp_chipid.h"
#include "bsp_led.h" 

extern uint32_t ChipUniqueID[3];
void Delay(__IO uint32_t nCount);


 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	LED_GPIO_Config();
  
  /* USART1 config 115200 8-N-1 */
	USART1_Config();
/*------------------------------------------------------------------------------------------------------*/
	/* to get the chipid and put it in ChipUniqueID[3] */
	Get_ChipID();
  
	/* printf the chipid */
	printf("\r\nоƬ��ΨһIDΪ: %X-%X-%X\r\n",
	        ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
	
	/* printf the flash memory amount */
	printf("\r\nоƬflash������Ϊ: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));
/*------------------------------------------------------------------------------------------------------*/	
	
	for(;;)
	{
		LED1_TOGGLE;
    Delay(0x0FFFFF);
	}
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/
