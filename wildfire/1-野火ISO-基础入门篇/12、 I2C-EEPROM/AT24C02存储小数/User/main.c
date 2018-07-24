/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I2C EEPROM(AT24C02)���ԣ�������Ϣͨ��USART1��ӡ�ڵ��Եĳ����ն�
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
#include "bsP_i2c_ee.h"
#include "bsP_led.h"
#include <string.h>

#define  EEP_Firstpage      0x00
u8 I2c_Buf_Write[256];
u8 I2c_Buf_Read[256];
void I2C_Test(void);

//void *memcpy(void *dest, const void *src, size_t n);

long double cal_p[6]={0.1, 1.1, 3.1, 4.02, 6.3, 0.01};
long double cal_test[6]={0};
uint8_t cal_flag = 0xaa;

uint8_t k;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{ 
  
  /* ����1��ʼ�� */
	USART1_Config();
	
	printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");
	//LED_GPIO_Config();

	/* I2C �����(AT24C02)ʼ�� */
	I2C_EE_Init();

	printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");	
	 	 
	//I2C_Test();
  
/*-------------------------------------------------------------------*/
  
  printf("\r\n start to write \r\n");	
  I2C_EE_BufferWrite((void*)cal_p, 1, sizeof(cal_p));
  //I2C_EE_BufferWrite((void*)cal_p, 1, sizeof(cal_p));
  
  printf("\r\n start to read \r\n");
  I2C_EE_BufferRead((void*)cal_test, 1, sizeof(cal_test));
	
  for( k=0; k<6; k++ )
    printf("%LF\r\n", cal_test[k]);

  while (1)
  {      
  }
}

/**
  * @brief  I2C(AT24C02)��д����
  * @param  ��
  * @retval ��
  */
void I2C_Test(void)
{
	u16 i;

	printf("д�������\n\r");
    
	for ( i=0; i<=255; i++ ) //��仺��
  {   
    I2c_Buf_Write[i] = i;

    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }

  //��I2c_Buf_Write��˳�����������д��EERPOM�� 
   //LED1(ON);
	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
	//LED1(OFF);   
  
  printf("\n\rд�ɹ�\n\r");
   
   printf("\n\r����������\n\r");
  //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
    //LED2(ON);   
	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 
   //LED2(OFF);
   
  //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			printf("0x%02X ", I2c_Buf_Read[i]);
			printf("����:I2C EEPROMд������������ݲ�һ��\n\r");
			return;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
  printf("I2C(AT24C02)��д���Գɹ�\n\r");
}
/*********************************************END OF FILE**********************/
