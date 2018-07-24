/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����ң��ʵ��
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
#include "bsp_irda.h" 
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "bsp_ili9341_lcd.h"
#include <stdio.h>

#define CLI()      __set_PRIMASK(1)		/* �ر����ж� */  
#define SEI() __set_PRIMASK(0)				/* �������ж� */ 

extern uint8_t  frame_flag;
extern uint8_t  isr_cnt;
extern uint8_t  frame_cnt;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */ 
int main(void)
{	
	uint8_t key_val;
  char lcd_key_val[20];
  
  /* config the led */
	LED_GPIO_Config();
	LED1_ON;
  
	/* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();
  /* ��������SysTick���ж����ȼ�Ϊ��ߣ�Ҫ��ȻSysTick��ʱ�ж���ռ����IO EXTI�ж�
   * ��ΪSysTick��ʼ��ʱĬ�����õ����ȼ�����͵�
   * ���ߵ�����������ʱ������ʱ��ʱ��Ҫ���ö�ʱ�������ȼ�����IO EXTI�жϵ����ȼ�
   */
  NVIC_SetPriority (SysTick_IRQn, 0);
  
  
  /* USART1 config 115200 8-N-1 */
	USART1_Config();
	printf("\r\n ����һ������ң�ط��������ʵ�� \r\n");
  
  LCD_Init();  
	LCD_Clear(0, 0, 240, 320, BACKGROUND);
  LCD_DispStr(30, 30, (uint8_t *)"This is a irda test demo", RED);
  
  /* ��ʼ���������ͷCP1838�õ���IO */
	IrDa_Init();  

	for(;;)
	{	  
    if( frame_flag == 1 ) /* һ֡�������ݽ������ */
    {
      key_val = IrDa_Process();
      printf("\r\n ��������frame_cnt=%d \r\n",frame_cnt);
      printf("\r\n �жϴ���isr_cnt=%d \r\n",isr_cnt);
      
      sprintf(lcd_key_val,"key_val:%c",key_val);
      LCD_DispStr(30, 40, lcd_key_val, RED);
      
      /* ��ͬ��ң��������Ӧ��ͬ�ļ�ֵ����Ҫʵ�ʲ��� */
      switch( key_val )
      {
        case 0:
        LED1_TOGGLE;
        printf("\r\n Error \r\n");
        LCD_DispStr(30, 50, (uint8_t *)"Error", RED);
        break;
        
        case 162:
        LED1_TOGGLE;
        printf("\r\n POWER \r\n");        
        break;
        
        case 226:
          LED1_TOGGLE;
          printf("\r\n MENU \r\n");
        break;
        
        case 34:
          LED1_TOGGLE;
          printf("\r\n TEST \r\n");
        break;
        
        case 2:
          LED1_TOGGLE;
          printf("\r\n + \r\n");
        break;
        
        case 194:
          LED1_TOGGLE;
          printf("\r\n RETURN \r\n");
        break;
        
        case 224:
          LED1_TOGGLE;
          printf("\r\n |<< \r\n");
        break;
        
        case 168:
          LED1_TOGGLE;
          printf("\r\n > \r\n");
        break;
        
        case 144:
          LED1_TOGGLE;
          printf("\r\n >>| \r\n");
        break;
        
        case 104:
          LED1_TOGGLE;
          printf("\r\n 0 \r\n");
        break;
        
        case 152:
          LED1_TOGGLE;
          printf("\r\n - \r\n");
        break;
        
        case 176:
          LED1_TOGGLE;
          printf("\r\n C \r\n");
        break;
        
        case 48:
          LED1_TOGGLE;
          printf("\r\n 1 \r\n");
        break;
        
        case 24:
          LED1_TOGGLE;
          printf("\r\n 2 \r\n");
        break;
        
        case 122:
          LED1_TOGGLE;
          printf("\r\n 3 \r\n");
        break;
        
        case 16:
          LED1_TOGGLE;
          printf("\r\n 4 \r\n");
        break;
        
        case 56:
          LED1_TOGGLE;
          printf("\r\n 5 \r\n");
        break;
        
        case 90:
          LED1_TOGGLE;
          printf("\r\n 6 \r\n");
        break;
        
          case 66:
          LED1_TOGGLE;
          printf("\r\n 7 \r\n");
        break;
        
        case 74:
          LED1_TOGGLE;
          printf("\r\n 8 \r\n");
        break;
        
        case 82:
          LED1_TOGGLE;
          printf("\r\n 9 \r\n");
        break;
        
        default:       
        break;
      }      
    }
	}
}
/*********************************************END OF FILE**********************/
