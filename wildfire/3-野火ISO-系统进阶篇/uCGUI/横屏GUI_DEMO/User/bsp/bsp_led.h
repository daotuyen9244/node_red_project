/*
*********************************************************************************************************
*	                                  
*	ģ������ : LEDָʾ������ģ��    
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2011-11-01   WildFire Team  ST�̼���汾 V3.5.0�汾��
*
*	Copyright (C), 2012-2013, WildFire Team
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "bsp_sys.h"

#define LED1  PCout(3)
#define LED2  PCout(4)
#define LED3  PCout(5)


/* ���ⲿ���õĺ������� */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedToggle(uint8_t _no);

#endif


