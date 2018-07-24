/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : bsp_ili9341_lcd.c
*	��    �� : V1.0
*	˵    �� : ����оƬ�ķ��ʵ�ַΪ:  0x60000000
*				
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2011-10-31   WildFire Team  ST�̼���汾 V3.5.0�汾��
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/
#include "bsp_touch.h"




#define	CHX 	0x90 	//ͨ��Y+��ѡ�������	
#define	CHY 	0xd0	//ͨ��X+��ѡ�������

#define	 SPI_CLK	  GPIO_Pin_7
#define  SPI_CS		  GPIO_Pin_10
#define	 SPI_MOSI	  GPIO_Pin_11
#define	 SPI_MISO	  GPIO_Pin_6
#define  TP_INT_PIN	  GPIO_Pin_9

#define TP_DCLK(a)	\
						if (a)	\
						GPIO_SetBits(GPIOG,GPIO_Pin_7);	\
						else		\
						GPIO_ResetBits(GPIOG,GPIO_Pin_7)
#define TP_CS(a)	\
						if (a)	\
						GPIO_SetBits(GPIOF,GPIO_Pin_10);	\
						else		\
						GPIO_ResetBits(GPIOF,GPIO_Pin_10)
#define TP_DIN(a)	\
						if (a)	\
						GPIO_SetBits(GPIOF,GPIO_Pin_11);	\
						else		\
						GPIO_ResetBits(GPIOF,GPIO_Pin_11)

#define TP_DOUT		 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)//	//�������

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTouch
*	����˵��: XPT2046����оƬ��ʼ��
*             PB7   --> TP_CS
*			  PA5   --> TP_SCK
*			  PA6   --> TP_MISO
*			  PA7   --> TP_MOSI
*			  PB6   --> TP_PEN_INT
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTouch(void) 
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOF, ENABLE);

    //////������SPI���GPIO��ʼ��//////            
    GPIO_InitStructure.GPIO_Pin=SPI_CLK;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = SPI_MISO; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = SPI_CS; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
    GPIO_Init(GPIOF, &GPIO_InitStructure);		

    GPIO_ResetBits(GPIOF,SPI_CS);
}

/*
*********************************************************************************************************
*	�� �� ��: XPT2046_WriteCMD
*	����˵��: д����
*	��    �Σ�CHX 	0x90 	//ͨ��Y+��ѡ������� CHY 	0xd0	//ͨ��X+��ѡ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void XPT2046_WriteCMD(unsigned char cmd) 
{
    unsigned char buf;
    unsigned char i;
//     TP_CS(1);
    TP_DIN(0);
    TP_DCLK(0);
//     TP_CS(0);
    for(i=0;i<8;i++) 
    {
        buf=(cmd>>(7-i))&0x1;
        TP_DIN(buf);
        //Delayus(5);
        TP_DCLK(1);
        //Delayus(5);
        TP_DCLK(0);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: XPT2046_ReadCMD
*	����˵��: ѡ��һ��ģ��ͨ��������ADC��������ADC�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static unsigned short XPT2046_ReadCMD(void) 
{
    unsigned short buf=0,temp;
    unsigned char i;
    TP_DIN(0);
    TP_DCLK(1);
    for(i=0;i<12;i++) 
    {
        TP_DCLK(0);          
        temp= (TP_DOUT) ? 1:0;
        buf|=(temp<<(11-i));
        TP_DCLK(1);
    }
    buf&=0x0fff;

    return(buf);
}

/*
*********************************************************************************************************
*	�� �� ��: TSC2046_ReadAdc
*	����˵��: ѡ��һ��ģ��ͨ��������ADC��������ADC�������
*	��    �Σ�_ucCh = 0x90 ��ʾYͨ���� 0xd0 ��ʾXͨ��
*	�� �� ֵ: 12λADCֵ
*********************************************************************************************************
*/
uint16_t XPT2046_ReadAdc(uint8_t _ucCh)
{
	uint16_t usAdc;

    XPT2046_WriteCMD(_ucCh);

	return 	XPT2046_ReadCMD();
}
