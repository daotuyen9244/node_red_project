#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"
#include "spi_enc28j60.h"
#include "web_server.h"

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����	
 */
int main (void)
{
  /* ����ϵͳʱ��Ϊ72M */
	SystemInit();
  
	/* ENC28J60 SPI �ӿڳ�ʼ�� */
  SPI_Enc28j60_Init();
	
	/* ENC28J60 WEB ������� */  
  Web_Server();

}

