 /**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ���� 2M����flash���ԣ�����������Ϣͨ������1�ڵ��Եĳ����ն��д�ӡ����
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
#include "bsp_spi_flash.h"


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
//#define  sFLASH_ID              0xEF3015     //W25X16
#define  sFLASH_ID              0xEF4015	 //W25Q16
     

/* ���ͻ�������ʼ�� */
//uint8_t Tx_Buffer[] = " ��л��ѡ��Ұ��stm32������\r\n                http://firestm32.taobao.com";
//uint8_t Rx_Buffer[BufferSize];
long double Tx_Buffer[7] = {0};
long double Rx_Buffer[7] = {0};

uint8_t cal_flag = 0;
uint8_t cal_f = 0;

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// ����ԭ������
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{ 	
	uint8_t k;
  
  /* ���ô���1Ϊ��115200 8-N-1 */
	USART1_Config();
	printf("\r\n ����һ��2M����flash(W25X16)ʵ�� \r\n");
	
	/* 2M����flash W25X16��ʼ�� */
	SPI_FLASH_Init();
	
	/* Get SPI Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	
	Delay( 200 );
	
	/* Get SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();
	
	printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* Check the SPI Flash ID */
	if (FlashID == sFLASH_ID)  /* #define  sFLASH_ID  0xEF3015 */
	{	
		printf("\r\n ��⵽�����flash W25X16 !\r\n");    
		
    #if 0
		/* �����ͻ�����������д��flash�� */
		SPI_FLASH_BufferWrite((void*)Tx_Buffer, FLASH_WriteAddress, sizeof(Tx_Buffer));
    for( k=0; k<7; k++ )
      printf("\r\n д�������Ϊ��%LF \r\n", Tx_Buffer[k]);
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		SPI_FLASH_BufferRead((void*)Rx_Buffer, FLASH_ReadAddress, sizeof(Rx_Buffer));
    for( k=0; k<7; k++ )
      printf("\r\n ����������Ϊ��%LF \r\n", Rx_Buffer[k]);
	  #elif 1
    SPI_FLASH_BufferRead(&cal_flag, 0, 1);
    if( cal_flag == 0xCC )
    {
//      SPI_FLASH_BufferRead((void*)Rx_Buffer, 1, sizeof(Rx_Buffer));
//        for( k=0; k<7; k++ )
//          printf("\r\n rx = %LF \r\n",Rx_Buffer[k]);
      
            SPI_FLASH_BufferRead((void*)Tx_Buffer, 1, sizeof(Tx_Buffer));
        for( k=0; k<7; k++ )
          printf("\r\n rx = %LF \r\n",Tx_Buffer[k]);
    }    
    else
    {
      cal_flag = 0xCC;
      SPI_FLASH_SectorErase(0);
      SPI_FLASH_BufferWrite(&cal_flag, 0, 1); 
      
      for( k=0; k<7; k++ )
          Tx_Buffer[k] = k +0.1;
      
      SPI_FLASH_BufferWrite((void*)Tx_Buffer, 1, sizeof(Tx_Buffer));
      
      for( k=0; k<7; k++ )
        printf("\r\n tx = %LF \r\n",Tx_Buffer[k]);
    }   
    
    #endif
    
    #if 0
		/* ���д�������������������Ƿ���� */
		TransferStatus1 = Buffercmp((void*)Tx_Buffer, (void*)Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			printf("\r\n 2M����flash(W25X16)���Գɹ�!\n\r");
		}
		else
		{        
			printf("\r\n 2M����flash(W25X16)����ʧ��!\n\r");
		}
    #endif
	}// if (FlashID == sFLASH_ID)
	else
	{    
		printf("\r\n ��ȡ���� W25X16 ID!\n\r");
	}
	
	SPI_Flash_PowerDown();  
	while(1);  
}

/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
