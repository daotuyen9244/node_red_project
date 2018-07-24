/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   rtc ���ԣ���ʾʱ���ʽΪ: xx:xx:xx
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
#include "bsp_rtc.h"

// N = 2^32/365/24/60/60 = 136 ��

/*ʱ��ṹ��*/
struct rtc_time systmtime;


/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main()
{			
	  	/* ����RTC���ж����ȼ� */
	  RTC_NVIC_Config();
	
	  USART1_Config();
	
	  RTC_CheckAndConfig(&systmtime);
	
	  /* Display time in infinite loop */
	  Time_Show(&systmtime);
}

/***********************************END OF FILE*********************************/

