/**
  ******************************************************************************
  * @file    bsp_iso_test.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ISO ������Գ��� bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "bsp_iso_test.h"
#include "stm32f10x.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_usart1.h"
#include "bsp_i2c_ee.h"
#include "bsp_touch.h"
#include "bsp_led.h" 
#include "bsp_key.h" 
#include "bsp_spi_flash.h"
#include "bsp_dht11.h"
#include "bsp_ds18b20.h"
#include "bsp_ov7725.h"
#include "bsp_adc.h"
#include "bsp_breathing.h" 
#include "bsp_rda5807m.h"
#include "enc28j60_test.h"
#include "bsp_can.h"
#include "bsp_rtc.h"
#include "bsp_fsmc_sram.h"
#include "bsp_fsmc_nor.h"
#include "bsp_nand.h"
#include "bsp_UsbMassStorage.h"

extern volatile uint8_t rec_cmd;

/*
 * ϵͳ�����λ
 */
void Soft_Reset(void)
{
  __set_FAULTMASK(1);   /* �ر������ж� */  
  NVIC_SystemReset();   /* ϵͳ��λ */
}

/* 
 * ISO ������Բ˵� 
 */
void Test_Menu(void)
{
	printf("\r\n[~~~~~~~~~~~~Ұ�� ISO STM32 ������Գ���~~~~~~~~~~~~~~~]\r\n");
	printf("[------------��ѡ������������������Сд��ĸ----------]\r\n");

	printf("1 - ��ˮ��\r\n");
	printf("2 - ����\r\n");
	printf("3 - EEPROM\r\n");
	printf("4 - FLASH\r\n");
  printf("5 - Һ����ʾӢ��\r\n");
  printf("6 - Һ������\r\n");
  printf("7 - RTC(������)\r\n");
  printf("8 - ADC\r\n");
//  printf("9 - DAC\r\n");
  printf("a - FM������\r\n");
//	printf("b - MP3-helix����\r\n");
  printf("c - DHT11��ʪ��\r\n");
  printf("d - DS18b20�¶ȴ�����\r\n");
  printf("e - ����ͷ(����ov7725)\r\n");
  printf("f - CAN(Loopback)\r\n");
  printf("g - ������\r\n");
  printf("h - ��̫��ENC28J60\r\n");
  printf("i - SRAM(IS62WV51216BLL-55TLI)\r\n");
  printf("j - NORFLASH(S29GL128P10TFI010)\r\n");
  printf("k - NANDFLASH(HY27UF081G2A)\r\n");
  printf("l - Usb-Mass-Storage ����\r\n");
  
  printf("�����������ĸ�������Сд\r\n");
  printf("\r\n");
}

/*
 * ���ݴ��ڲ�ͬ������ִ�в�ͬ�ĳ���
 * �û�Ҳ�������������Ӧ�ĳ���
 */
void Board_Test(void)
{  
  while( 1 )
  {    
    switch( rec_cmd )
    {
      case '1':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:LED1 LE2 LED3 ������˸ \r\n");
        LED_Test();         
        break;
      
      case '2':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:KEY1����LED1��ת,KEY2����LE2��ת \r\n");
        Key_Test();        
        break;
      
      case '3':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:���ڴ�ӡ��:eeprom��д���Գɹ� \r\n");
        Eeprom_Test();
        break;
      
      case '4':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:���ڴ�ӡ��:2M����flash(W25Q16)���Գɹ�!\r\n");
        SpiFlash_Test();      
        break;
      
      case '5':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:Һ������ʾӢ���ַ��ͱ仯������\r\n");
        LCD_Test();             
        break;

      case '6':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:Һ������ʮ�ֽ��棬�û��ʵ��ʮ�ֽ�����д�����У��\r\n         У�����ʵ�ֻ��ʹ���\r\n");
        Touch_Test();         
        break;
      
      case '7':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽�� \r\n");
        printf("1-������ʾ�ڴ���������ʱ��\r\n");
        printf("ʵ������:�����������ũ����ʱ���\r\n");
        RTC_Test();
        break;
      
      case '8':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:����ÿ��һ��ʱ���ӡ���ɼ����ĵ�ѹֵ \r\n         ��תISO�ϵĵ�λ������ʹ�õ�ѹֵ��0~3.25֮��䶯\r\n");
        ADC_Test();      
        break;
      
//      case '9':
//        printf("���س����ز˵� \r\n");
//      
//        break;
      
      case 'a':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽�� \r\n");
        printf("1-��ISO�ϵ�MP3-FM-SPK���ز���FM��λ\r\n");
        printf("2-���϶�����ͬʱ����Ҳ�����ߣ����Ѳ���̨��ʱ����ƶ������߽����ź�\r\n");
        printf("ʵ������:KEY1�����Զ���̨���ѵ�̨LED1����KEY2�����Զ���̨���ѵ�̨LED2�� \r\n");
        RDA5807_FM_Test();
        break;
      
//      case 'b': 
//        printf("���س����ز˵� \r\n");

//        break;

      case 'c':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽�� \r\n");
        printf("1-��ISO�ϲ���DHT11ģ�� \r\n");
        printf("ʵ������:���ڴ�ӡ��:ʪ��Ϊxxx%RH,�¶�Ϊxxx�� \r\n");
        DHT11_Test();          
        break;
      
      case 'd': 
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽�� \r\n");
        printf("1-��ISO�ϲ���DS18b20ģ�� \r\n");
        printf("ʵ������:����ÿ��1s��ӡһ���¶�ֵ��ͬʱled1��˸һ��\r\n");
        DS18B20_Test();         
        break;
      
      case 'e':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽�� \r\n");
        printf("1-��ISO�ϲ�������ͷģ��(����OV7725)\r\n");
        printf("ʵ������:Һ��ʵʱ��ʾ����ͷ�Ļ�����ͼ��\r\n");
        Camera_Test();        
        break; 
      
      case 'f':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:���ڴ�ӡ��Ϣ:CAN�ػ����Գɹ�ͬʱLED1��LED2��\r\n");
        CAN_Loopback_Test();      
        break;
      
      case 'g':
        printf("���س����ز˵� \r\n");
        printf("ʵ������:LED1��PWM�źŵĿ������ں���\r\n");
        Breathing_Test();        
        break;
      
      case 'h':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽�� \r\n");
        printf("1-��ISO�ϲ���ENC28J60��̫��ģ�飬��̫��ģ�����������ӵ��Ի���·����\r\n");
        printf("2-��DOSģʽ����������: ping 192.168.1.15 \r\n");
        printf("3-���Ե�IP��ַ���߾������ڵĵ��Ե�IP��ַ������192.168.1.x\r\n  ��������192.168.1.15 \r\n");
        ENC28J60_Test();      
        break;
      
      case 'i':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽��\r\n");
        printf("1-��ISO�ϲ���SRAMģ��\r\n");
        printf("ʵ������:���ڴ�ӡ��SRAM���Գɹ���ͬʱLED2��˸\r\n");
        SRAM_Test();      
        break;
      
      case 'j':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽��\r\n");
        printf("1-��ISO�ϲ���NORFLASHģ��\r\n");
        printf("ʵ������:���ڴ�ӡ��NORFLASH���Գɹ���ͬʱLED2��˸\r\n");
        printf("\r\n ����ʱ���е㳤�������ĵȴ�...... \r\n");
        NORFLASH_Test();      
        break;
      
      case 'k':
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽��\r\n");
        printf("1-��ISO�ϲ���NANDFLASHģ��\r\n");
        printf("ʵ������:���ڴ�ӡ�NANDFLASH Test finished,ͬʱLE1��˸r\n");
        NANDFLASH_Test();      
        break;
      
      case 'l': 
        printf("���س����ز˵� \r\n");
        printf("ʵ�鲽��\r\n");
        printf("1-��ISO�ϲ���SD����������С��32GB\r\n");
        printf("2-��microusb�����ӵ��Ժ�ISO�ϵĹ����microusb��(������ usb to uart) \r\n");
        printf("ʵ������:���ڻ��ӡ��:Ұ�� ISO STM32 USB MASS STORAGE ʵ��\r\n         LED1��˸��ͬʱSD�����ڵ�����ģ���һ��U��\r\n");
        UsbMassStorage_Test();
        break;      
                        
      default:       
        break;
    }
  }
}


/* 
 * ISO ������� 
 */
void ISO_Test(void)
{  
  /* Һ����ʼ�����ڴ��ڣ���Ȼ����bug��������ʲôԭ����Ҳ��֪��:( */
  LCD_Init();
  
  USART1_Config();

  Test_Menu();
  
  Board_Test();

	while( 1 ){}
}
/* ---------------------------------------end of file------------------------------------- */
