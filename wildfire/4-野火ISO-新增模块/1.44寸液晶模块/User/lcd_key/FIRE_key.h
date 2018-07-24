/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       FIRE_KEY.h
 * @brief      KEY����ͷ�ļ�
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-07-10
 */

#ifndef __FIRE_KEY_H__
#define __FIRE_KEY_H__

#include "stm32f10x.h"


//�����Ƕ��尴����ʱ�䣬��λΪ �� 10ms���ж�ʱ�䣩
#define KEY_DOWN_TIME           1       //����ȷ�ϰ���ʱ��
#define KEY_HOLD_TIME           50      //����holdȷ��ʱ�䣬���253��������Ҫ�޸� keytime ������
                                        //�������һֱ����ȥ����ÿ�� KEY_HOLD_TIME - KEY_DOWN_TIME ʱ��ᷢ��һ�� KEY_HOLD ��Ϣ

//���尴����ϢFIFO��С
#define KEY_MSG_FIFO_SIZE       20      //��� 255��������Ҫ�޸�key_msg_front/key_msg_rear����

//�����˿ڵ�ö��
typedef enum
{
    KEY_U,  //��
    KEY_D,  //��

    KEY_L,  //��
    KEY_R,  //��

//  KEY_A,  //ȡ��
    KEY_B,  //ѡ��

//  KEY_START,  //��ʼ
//  KEY_STOP,   //ֹͣ

    KEY_MAX,
} KEY_e;

/* ����ʹ�õ���GPIO���ź�ʱ�� */

#define KEY_RIGHT_PORT	GPIOA
#define KEY_RIGHT_PIN		GPIO_Pin_8
#define KEY_RIGHT_CLK		RCC_APB2Periph_GPIOA

#define KEY_LEFT_PORT		GPIOB
#define KEY_LEFT_PIN		GPIO_Pin_5
#define KEY_LEFT_CLK		RCC_APB2Periph_GPIOB

#define KEY_UP_PORT			GPIOD
#define KEY_UP_PIN			GPIO_Pin_3
#define KEY_UP_CLK			RCC_APB2Periph_GPIOD

#define KEY_DOWN_PORT		GPIOC
#define KEY_DOWN_PIN		GPIO_Pin_6
#define KEY_DOWN_CLK		RCC_APB2Periph_GPIOC

#define KEY_SELECT_PORT	GPIOC
#define KEY_SELECT_PIN	GPIO_Pin_7
#define KEY_SELECT_CLK	RCC_APB2Periph_GPIOC

/*
 * ���� KEY ��Ŷ�Ӧ�Ĺܽ�
 */
typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
}KEY_PTx_TypeDef;


//key״̬�궨��
typedef enum
{
    KEY_DOWN  =   0,         //��������ʱ��Ӧ��ƽ
    KEY_UP    =   1,         //��������ʱ��Ӧ��ƽ

    KEY_HOLD,

} KEY_STATUS_e;

//������Ϣ�ṹ��
typedef struct
{
    KEY_e           key;
    KEY_STATUS_e    status;
} KEY_MSG_t;


void            key_init(void);            // KEY��ʼ������(key С�� KEY_MAX ʱ��ʼ�� ��Ӧ�˿ڣ������ʼ��ȫ���˿�)
KEY_STATUS_e    key_check(KEY_e key);           //���key״̬������ʱ������


//��ʱɨ�谴��
uint8_t get_key_msg(KEY_MSG_t *keymsg);         //��ȡ������Ϣ������1��ʾ�а�����Ϣ��0Ϊ�ް�����Ϣ
void    key_IRQHandler(void);                   //��Ҫ��ʱɨ����жϸ�λ��������ʱʱ��Ϊ10ms��


#endif  //__FIRE_KEY_H__

