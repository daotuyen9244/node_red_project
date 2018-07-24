/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       FIRE_LCD_ST7735R.h
 * @brief      LCD ������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-09-07
 */

#ifndef _FIRE_LCD_ST7735R_H_
#define _FIRE_LCD_ST7735R_H_

//#include "MK60_gpio.h"
#include "FIRE_LCD.h"

#define ST7735R_DELAY()         Lcd_Delay(10);                //do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)
#define ST7735R_DELAY_MS(ms)    Lcd_Delay(ms*1000)


//�û�����
#define ST7735R_DIR_DEFAULT     0       //����Ĭ�Ϸ��� (0~3)

#if (USE_LCD == LCD_ST7735R)

/*********************** API�ӿ� ******************************/
//�ṩAPI�ӿڸ�LCD����

#define LCD_H                   ST7735R_get_h()                 //��
#define LCD_W                   ST7735R_get_w()                 //��

#define LCD_INIT()              LCD_ST7735R_init()              //��ʼ��
#define LCD_PTLON(site,size)    LCD_ST7735R_ptlon(site,size)    //����
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //дģʽ
#define LCD_WR_DATA(data)       do{LCD_ST7735R_WR_8DATA((uint8_t)((data)>>8) );LCD_ST7735R_WR_8DATA( (uint8_t)(data) );}while(0)       //д����
#define LCD_WR_CMD(cmd)         LCD_ST7735R_WR_8CMD(cmd)        //����
#define LCD_SET_DIR(opt)        LCD_ST7735R_dir(opt)            //����

#define LCD_DIR                 ST7735R_get_dir()               //��ȡ����


//ͨ���������ã��ٶ�̫���ˣ��ĳɺ궨�����
//#define LCD_WR_8DATA(data)      LCD_ST7735R_wr_data(data)       //д����
//#define LCD_WR_8CMD(cmd)        LCD_ST7735R_wr_cmd(cmd)         //����

#endif  //(USE_LCD == LCD_ST7735R)

/********************* ST7735R�ڲ�ʵ�� ***********************/
//LCD �ĹܽŶ���

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)									{p->BSRR=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)									{p->BRR=i;}				//����͵�ƽ
#define digitalToggle(p,i)							{p->ODR ^=i;}			//�����ת״̬

#define digitalOut8HData(p,data)		   	{p->ODR = ((data<<8) | (p->ODR & 0xff));}	//���IO�ĸ�8λ,��8λ����


#define LCD_ST7735R_WR_H			digitalHi(GPIOC,GPIO_Pin_5)
#define LCD_ST7735R_WR_L			digitalLo(GPIOC,GPIO_Pin_5)

#define LCD_ST7735R_RD_H			digitalHi(GPIOA,GPIO_Pin_1)
#define LCD_ST7735R_RD_L			digitalLo(GPIOA,GPIO_Pin_1)

#define LCD_ST7735R_CS_H			digitalHi(GPIOA,GPIO_Pin_3)
#define LCD_ST7735R_CS_L			digitalLo(GPIOA,GPIO_Pin_3)

#define LCD_ST7735R_RS_H			digitalHi(GPIOA,GPIO_Pin_0)
#define LCD_ST7735R_RS_L			digitalLo(GPIOA,GPIO_Pin_0)

#define LCD_ST7735R_RST_H			digitalHi(GPIOA,GPIO_Pin_2)
#define LCD_ST7735R_RST_L			digitalLo(GPIOA,GPIO_Pin_2)

#define LCD_ST7735R_P0(data)	digitalOut8HData(GPIOB,data)


#define LCD_ST7735R_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735R_RD_H;\
                                LCD_ST7735R_RS_L;\
                                LCD_ST7735R_CS_L;\
                                LCD_ST7735R_P0(cmd);\
                                LCD_ST7735R_WR_L;\
                                LCD_ST7735R_WR_H;\
                                LCD_ST7735R_CS_H;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;����һ��������

														
#define LCD_ST7735R_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735R_RD_H;\
                                LCD_ST7735R_RS_H;\
                                LCD_ST7735R_CS_L;\
                                LCD_ST7735R_P0(data);\
                                LCD_ST7735R_WR_L;\
                                LCD_ST7735R_WR_H;\
                                LCD_ST7735R_CS_H;\
                            }while(0)   //LCD_WR=0;������д�����ݵ�RAM

														
														
#define LCD_ST7735R_WR_DATA(data)       do{LCD_ST7735R_WR_8DATA((uint8_t)((data)>>8) );LCD_ST7735R_WR_8DATA( (uint8_t)(data) );}while(0)       //д����
														



void     LCD_ST7735R_init(void);
void     LCD_ST7735R_dir(uint8_t option);
void     LCD_ST7735R_ptlon(Site_t site, Size_t size);
uint16_t   ST7735R_get_h(void);
uint16_t   ST7735R_get_w(void);
uint8_t    ST7735R_get_dir(void);


void LCD_ST7735R_wr_data(uint8_t data);
void LCD_ST7735R_wr_cmd(uint8_t cmd);


#endif  //_FIRE_LCD_ST7735R_H_
