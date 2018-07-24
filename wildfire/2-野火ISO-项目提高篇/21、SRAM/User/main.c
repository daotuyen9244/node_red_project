/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   sram(IS62WV51216BLL)����
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
#include "bsp_fsmc_sram.h"

/* 
 * 2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
 * 64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
 * 64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
 * 64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
 * 64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF
*/

/*
 * SRAM �ͺ�: IS62WV51216BLL
 *      ����: 8Mbit = 1024 KB
 * ���ʷ�ʽ : 16bit ����
 * ��ַ     : 0X6800 0000 ~ 0X6800 0000 + 8Mbit
 */

#if 1


void SRAM_Delay(__IO u32 nCount);

#define BUFFER_SIZE        0x400      /* 1024���ֽ� */
//#define BUFFER_SIZE        0x40      /* 1024���ֽ� */
#define WRITE_READ_ADDR    0x8000


uint16_t TxBuffer[BUFFER_SIZE];
uint16_t RxBuffer[BUFFER_SIZE];
uint32_t WriteReadStatus = 0, Index = 0;

void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main(void)
{
  /* LED GPIO ��ʼ�� */
	LED_GPIO_Config();
	
	LED2(OFF);
	LED3(OFF);
  
  /* ���� FSMC Bank1 NOR/SRAM3 */
  FSMC_SRAM_Init();
  //LED1(ON);
  
  /* ���Ҫ���͵Ļ����� */
  Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x3212);
  
  /* ��������������д��SRAM���� */
  FSMC_SRAM_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);
  
  SRAM_Delay(0x0FFFFF);
  
  /* ���ո�д��SRAM��������ݶ����� */
  FSMC_SRAM_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);  

  /* ��д��SRM��������ݺʹ�SRAM��������ݶ������ȶ� */   
  for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
  {
    if (RxBuffer[Index] != TxBuffer[Index])
    {
      WriteReadStatus = Index + 1;
    }
  }	

  if (WriteReadStatus == 0)
  {	/* OK */
    /* Turn on LD2 */
    while(1)
    {
      LED2_TOGGLE;
      SRAM_Delay(0x0FFFFF);
    }    
  }                               
  else  
  { /* KO */		
    while(1)
    {
      LED3_TOGGLE;
      SRAM_Delay(0x0FFFFF);
    }
  }
}

/*
 * ��仺����
 */
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

void SRAM_Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

#endif
/* ------------------------------------------------end of file-------------------------------------------- */

