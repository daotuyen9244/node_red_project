
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       FIRE_LCD.h
 * @brief      LCD ������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-09-02
 */
 
#ifndef _APP_LCD_H
#define _APP_LCD_H

#include "stm32f10x.h"

#define LCD_ILI9341     1
#define LCD_ST7735S     2       //LCD_ST7735S �� LCD_ST7735R ���һ�����Ĵ����������в�ͬ
#define LCD_ST7735R     3       //

#define USE_LCD         LCD_ST7735R             //ѡ��ʹ�õ� LCD



/*
 * ��������ṹ��
 */
typedef struct
{
    uint16_t x;
    uint16_t y;
} Site_t;

/*
 * ������δ�С�ṹ��
 */
typedef struct
{
    uint16_t W;       //��
    uint16_t H;       //��
} Size_t;

/******������ɫ*****/
#define RED     0XF800    //��ɫ
#define GREEN   0X07E0    //��ɫ
#define BLUE    0X001F    //��ɫ
#define BRED    0XF81F
#define GRED    0XFFE0    //��ɫ
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //��ɫ
#define WHITE   0XFFFF    //��ɫ


#define FCOLOUR         BLUE    //����������ɫ
#define BCOLOUR         RED     //���屳����ɫ

//��ȡRGB��3Ԫ�أ�rgb565������16λ
#define RGB565_R(rgb565)                ((uint8_t)(((uint16_t)(rgb565)>>11) &0x1F))
#define RGB565_G(rgb565)                ((uint8_t)(((uint16_t)(rgb565)>> 5) &0x3F))
#define RGB565_B(rgb565)                ((uint8_t)( (uint16_t)(rgb565)      &0x1F))


#define GRAY_2_RGB565(gray)             ((uint16_t)((((uint8_t)(gray)>>3)<<11)|(((uint8_t)(gray)>>2)<<5)|((uint8_t)(gray)>>3)))

#define RGB565_2_GRAY(rgb565)           ((uint8_t)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8))  //  31*235+63*613+31*625+1  = 255*256


#define RGB565_H(rgb565)                ((uint8_t)(((uint16_t)(rgb565))>>8))
#define RGB565_L(rgb565)                (uint8_t)(rgb565))

#define RGB24_RGB565(R,G,B)             ((uint16_t)((((uint8_t)(R)>>3)<<11)|(((uint8_t)(G)>>2)<<5)|((uint8_t)(B)>>3)))
#define RGB555_RGB565(rgb555)           (((((uint16_t)(rgb555)) & (uint16_t)(~0x1F)) << 1) |  (((uint16_t)(rgb555)) & 0x1F))
#define BGR555_RGB565(bgr555)           ((((uint16_t)(bgr555)<<11) & (uint16_t)(0x1F<<11)) | (((uint16_t)(bgr555)) & (uint16_t)(0x1F<<5)) | (((uint16_t)(bgr555)>>10) & (uint16_t)(0x1F)))



//��Ҫ�ײ�ʵ�ֵļ����궨�壺
//LCD_H                         //��
//LCD_W                         //��

//LCD_INIT()                    //��ʼ��
//LCD_PTLON(site,size)          //����
//LCD_RAMWR()                   //дģʽ
//LCD_WR_DATA(data)             //д����
//LCD_WR_DATA_B(data)           //д����(���)��16λҺ����LCD_WR_DATAһ�¼��ɣ�8λҺ������˳��

//����Ϊ��ѡ
//LCD_RAMRD()                   //��ģʽ
//LCD_SET_DIR(opt)              //������ʾ����(0~3)
//LCD_DIR                       //��ȡ��ʾ����(0~3)


//����Ϊ�ڲ����ã���������ĺ궨�����
//LCD_WR_CMD(cmd)               //����
//LCD_RD_DATA()                 //������

//8λҺ��������Ҫ������Ĭ����16λ��
//LCD_WR_8CMD(cmd)               //����
//LCD_WR_8DATA(data)            //д����


//�����ֿⶨ�� Ӣ����ĸ�� ��ߡ�
#define LCD_EN_W        8
#define LCD_EN_H        16


/***************  LCD��ʼ��  ***************/

void LCD_init(void);                                                                                    //LCD��ʼ��

/***************  LCD�滭  ***************/

extern void LCD_point          (Site_t,uint16_t rgb565);                 //����
extern void LCD_rectangle      (Site_t,Size_t,uint16_t rgb565);                 //������
extern void LCD_char           (Site_t,uint8_t ascii,uint16_t Color, uint16_t bkColor);  //��ʾ8*16�ַ�
extern void LCD_str            (Site_t,uint8_t *Str,uint16_t Color, uint16_t bkColor);  //��ʾ8*16�ַ���

extern void LCD_num            (Site_t,uint32_t num,uint16_t Color, uint16_t bkColor);  //��ʾ����
#define MAX_NUM_BIT 5                                                                                   //���ֵ����λ������������������Ļ�������֣�
#define LCD_num_C(site,num,color,bkColor)       LCD_num_BC(site, num,MAX_NUM_BIT,color,bkColor)     //��ʾ���֣�����������Ļ�������֣�
extern void LCD_num_BC         (Site_t,uint32_t num, uint8_t max_num_bit,uint16_t Color, uint16_t bkColor);  //��ʾ���֣�����������Ļ�������֣�

extern void LCD_Img_gray       (Site_t site, Size_t size, uint8_t *img);                   //��ʾ�Ҷ�ͼ��
extern void LCD_Img_gray_Z     (Site_t site, Size_t size, uint8_t *img, Size_t imgsize);   //��ʾ�Ҷ�ͼ��(������)

#define BINARY_BGCOLOR  WHITE       //�����ֵ��ͼ�񱳾���ɫ
#define BINARY_COLOR    BLACK       //�����ֵ��ͼ��ǰ����ɫ
extern void LCD_Img_Binary     (Site_t site, Size_t size, uint8_t *img);                   //��ʾ��ֵ��ͼ��
extern void LCD_Img_Binary_Z   (Site_t site, Size_t size, uint8_t *img, Size_t imgsize);   //��ʾ��ֵ��ͼ��(������)


#endif
