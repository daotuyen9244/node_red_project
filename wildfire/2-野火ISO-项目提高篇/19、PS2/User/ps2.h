/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��ʱ����	  (��������ԭ�ӿ�����)
*	�ļ����� : bsp_ps2.h
*	��    �� : V1.0
*	˵    �� : �������           
*	�޸ļ�¼ : ������޸ļ�¼Ҳ��ָ��ԭ�ӿ�������޸ļ�¼
*			   
*
*   �汾��     ����       ����      ˵��
*   v1.0     2012-10-26  Eric2013  ST�̼���汾 V3.5.0�汾��		
*   BLOG: http://blog.sina.com.cn/u/2565749395
*********************************************************************************************************
*/
#ifndef __PS2_H
#define __PS2_H	 	   
#include "sys.h"	
#include "stm32f10x.h"
//����ӿڶ���
//PS2���� 		  				    
#define PS2_SCL PCin(4)	//PC11
#define PS2_SDA PCin(5)	//PC10
//PS2���
#define PS2_SCL_OUT PCout(4)	//PC11
#define PS2_SDA_OUT PCout(5)	//PC10

//����PS2_SCL�������״̬.		  
#define PS2_SET_SCL_IN()  {GPIOC->CRL&=0XFFF0FFFF;GPIOC->CRL|=0X00080000;}
#define PS2_SET_SCL_OUT() {GPIOC->CRL&=0XFFF0FFFF;GPIOC->CRL|=0X00030000;}	  
//����PS2_SDA�������״̬.		  
#define PS2_SET_SDA_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00800000;}
#define PS2_SET_SDA_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00300000;} 

#define MOUSE    0X20 //���ģʽ
#define KEYBOARD 0X10 //����ģʽ
#define CMDMODE  0X00 //��������
//PS2_Status��ǰ״̬��־
//[5:4]:��ǰ������ģʽ;[7]:���յ�һ������
//[6]:У�����;[3:0]:�յ������ݳ���;	 
extern u8 PS2_Status;       //����Ϊ����ģʽ
extern u8 PS2_DATA_BUF[16]; //ps2���ݻ�����                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
extern u8 MOUSE_ID;

void PS2_Init(void);
u8 PS2_Send_Cmd(u8 cmd);
void PS2_Set_Int(u8 en);
u8 PS2_Get_Byte(void);
void PS2_En_Data_Report(void);  
void PS2_Dis_Data_Report(void);		  				    
#endif





















