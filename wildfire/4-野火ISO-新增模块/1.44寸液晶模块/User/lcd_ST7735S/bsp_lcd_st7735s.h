/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       FIRE_LCD_ST7735S.h
 * @brief      LCD ������
 * @author     Ұ��Ƽ�
 * @version    
 * @date       2013-09-07
 */

#ifndef _BSP_LCD_ST7735S_H_
#define _BSP_LCD_ST7735S_H_

#include "stm32f10x.h"
#include "FIRE_lcd.h"


#define ST7735S_DELAY()         Lcd_Delay(10);                //do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)
#define ST7735S_DELAY_MS(ms)    Lcd_Delay(ms*1000)


//�û�����
#define ST7735S_DIR_DEFAULT     0       //����Ĭ�Ϸ��� (0~3)

#if (USE_LCD == LCD_ST7735S)

/*********************** API�ӿ� ******************************/
//�ṩAPI�ӿڸ�LCD����

#define LCD_H                   ST7735S_get_h()                 //��
#define LCD_W                   ST7735S_get_w()                 //��

#define LCD_INIT()              LCD_ST7735S_init()              //��ʼ��
#define LCD_PTLON(site,size)    LCD_ST7735S_ptlon(site,size)    //����
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //дģʽ
#define LCD_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8_t)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8_t)(data) );}while(0)       //д����
#define LCD_WR_CMD(cmd)         LCD_ST7735S_WR_8CMD(cmd)        //����
#define LCD_SET_DIR(opt)        LCD_ST7735S_dir(opt)            //����

#define LCD_DIR                 ST7735S_get_dir()               //��ȡ����


//ͨ���������ã��ٶ�̫���ˣ��ĳɺ궨�����
//#define LCD_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //д����
//#define LCD_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //����

#endif  //(USE_LCD == LCD_ST7735S)

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)									{p->BSRR=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)									{p->BRR=i;}				//����͵�ƽ
#define digitalToggle(p,i)							{p->ODR ^=i;}			//�����ת״̬

#define digitalOut8HData(p,data)		   	{p->ODR = ((data<<8) | (p->ODR & 0xff));}	//���IO�ĸ�8λ,��8λ����

/* �������IO�ĺ� */
#define LCD_ST7735S_WR_H			digitalHi(GPIOC,GPIO_Pin_5)
#define LCD_ST7735S_WR_L			digitalLo(GPIOC,GPIO_Pin_5)

#define LCD_ST7735S_RD_H			digitalHi(GPIOA,GPIO_Pin_1)
#define LCD_ST7735S_RD_L			digitalLo(GPIOA,GPIO_Pin_1)

#define LCD_ST7735S_CS_H			digitalHi(GPIOA,GPIO_Pin_3)
#define LCD_ST7735S_CS_L			digitalLo(GPIOA,GPIO_Pin_3)

#define LCD_ST7735S_RS_H			digitalHi(GPIOA,GPIO_Pin_0)
#define LCD_ST7735S_RS_L			digitalLo(GPIOA,GPIO_Pin_0)

#define LCD_ST7735S_RST_H			digitalHi(GPIOA,GPIO_Pin_2)
#define LCD_ST7735S_RST_L			digitalLo(GPIOA,GPIO_Pin_2)

#define LCD_ST7735S_P0(data)	digitalOut8HData(GPIOB,data)


#define LCD_ST7735S_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735S_RD_H;\
                                LCD_ST7735S_RS_L;\
                                LCD_ST7735S_CS_L;\
                                LCD_ST7735S_P0(cmd);\
                                LCD_ST7735S_WR_L;\
                                LCD_ST7735S_WR_H;\
                                LCD_ST7735S_CS_H;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;����һ��������

														
#define LCD_ST7735S_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735S_RD_H;\
                                LCD_ST7735S_RS_H;\
                                LCD_ST7735S_CS_L;\
                                LCD_ST7735S_P0(data);\
                                LCD_ST7735S_WR_L;\
                                LCD_ST7735S_WR_H;\
                                LCD_ST7735S_CS_H;\
                            }while(0)   //LCD_WR=0;������д�����ݵ�RAM

														
														
#define LCD_ST7735S_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8_t)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8_t)(data) );}while(0)       //д����
														

void LCD_ST7735S_init(void);
void LCD_ST7735S_dir(uint8_t option);
void LCD_ST7735S_ptlon(Site_t site, Size_t size);
uint16_t ST7735S_get_h(void);
uint16_t ST7735S_get_w(void);
uint8_t  ST7735S_get_dir(void);


#endif  //_BSP_LCD_ST7735S_H_




