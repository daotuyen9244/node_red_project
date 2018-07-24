/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : bsp_ili9341_lcd.h
*	��    �� : V1.0
*	˵    �� : ����оƬ�ķ��ʵ�ַΪ:  0x60000000
*				
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2011-10-31   WildFire Team  ST�̼���汾 V3.5.0�汾��
*       v1.1    2011-11-07   WildFire Team  ����������ʾ��ͨ��ֱ�ӽ�X,Y������Ϊ�ݼ�������GUI��ʾ��
*
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/

#include "stm32f10x.h"


/* ����LCD�������ķ��ʵ�ַ */
#define ILI9341_REG		*(__IO uint16_t *)(0x6C000000)
#define ILI9341_RAM		*(__IO uint16_t *)(0x6D000000)


void bsp_InitLCD(void);


void SetCursor(u16 x,u16 y);
void SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
void SetPoint(u16 x,u16 y,u16 point);

void LCD_Clear(u16 dat);
void LCD_open_windows(u16 x,u16 y,u16 len,u16 wid);
void LCD9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void LCD9341_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
void LCD9341_FillRect(uint16_t _usX1 , uint16_t _usY1 ,  uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);

u16 GetPoint(u16 x,u16 y);

void bsp_InitLCD(void);



