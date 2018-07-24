/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : bsp_ili9341_lcd.c
*	��    �� : V1.1
*	˵    �� : ����оƬ�ķ��ʵ�ַΪ:  0x60000000
*				
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2011-10-31   WildFire Team  ST�̼���汾 V3.5.0�汾��
*       v1.1    2011-11-07   WildFire Team  ����������ʾ��ͨ��ֱ�ӽ�X,Y������Ϊ�ݼ�������GUI��ʾ��
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/
#include "bsp_ili9341_lcd.h"


/* ILI9341�Ĵ�����ַ */
#define SWRESET 	0x01 //Software Reset
#define RDDIDIF 	0x04 //Read Display Identification Information
#define RDDST   	0x09 //Read Display Status
#define RDDPM   	0x0A //Read Display Power Mode
#define RDDMADCTL	0x0B //Read Display MADCTL
#define RDDCOLMOD	0x0C //Read Display Pixel Format
#define RDDIM		0x0D //Read Display Image Mode
#define RDDSM		0x0E //Read Display Signal Mode
#define RDDSDR		0x0F //Read Display Self-Diagnostic Result
#define SPLIN		0x10 //Enter Sleep Mode
#define SLPOUT		0x11 //Sleep Out
#define PTLON		0x12 //Partial Mode On
#define NORON		0x13 //Normal Display Mode On
#define DINVOFF		0x20 //Display Inversion OFF
#define DINVON		0x21 //Display Inversion ON
#define GAMSET      0x26 //Gamma Set
#define DISPOFF		0x28 //Display OFF
#define DISPON		0x29 //Display ON
#define CASET		0x2A //Column Address Set
#define PASET		0x2B //Page Address Set
#define RAMWR		0x2C //Memory Write
#define RGBSET		0x2D //Color Set
#define RAMRD       0x2E //Memory Read
#define PLTAR		0x30 //Partial Area
#define VSCRDEF		0x33 //Vertical Scrolling Definition
#define TEOFF		0x34 //Tearing Effect Line OFF
#define TEON		0x35 //Tearing Effect Line ON
#define MADCTL		0x36 //Memory Access Control
#define VSCRSADD	0x37 //Vertical Scrolling Start Address
#define IDMOFF		0x38 //Idle Mode OFF
#define IDMON		0x39 //Idle Mode ON
#define PIXSET		0x3A //Pixel Format Set
#define RAMWRC      0x3C //Write Memory Continue
#define RAMRDC		0x3E //Read  Memory Continue
#define STTS		0x44 //Set Tear Scanline 
#define GTSL		0x45 //Get Scan line
#define WRDISBV		0x51 //Write Display Brightness
#define RDDISBV		0x52 //Read Display Brightness Value
#define WRCTRLD		0x53 //Write Control Display
#define RDCTRLD		0x54 //Read Control Display
#define WRCABC		0x55 //Write Content Adaptive Brightness Control
#define RDCABC		0x56 //Read Content Adaptive Brightness Control
#define WRCABCMIN	0x5E //Write CABC Minimum Brightness
#define RDCABCMIN	0x5F //Read CABC Minimum Brightnes
#define RDID1		0xDA //Read ID1
#define RDID2		0xDB //Read ID2
#define RDID3		0xDC //Read ID3
#define IFMODE		0xB0 //Interface Mode Control
#define FRMCTR1		0xB1 //Frame Rate Control (In Normal Mode / Full colors
#define FRMCTR2		0xB2 //Frame Rate Control (In Idle Mode / 8l colors)
#define FRMCTR3		0xB3 //Frame Rate Control (In Partial Mode / Full colors)
#define INVTR		0xB4 //Display Inversion Control
#define PRCTR		0xB5 //Blanking Porch
#define DISCTRL		0xB6 //Display Function Control
#define ETMOD		0xB7 //Entry Mode Set
#define BKCTL1		0xB8 //Backlight Control 1 
#define BKCTL2		0xB9 //Backlight Control 2 
#define BKCTL3		0xBA //Backlight Control 3 
#define BKCTL4		0xBB //Backlight Control 4 
#define BKCTL5		0xBC //Backlight Control 5
#define BKCTL7		0xBE //Backlight Control 7
#define BKCTL8		0xBF //Backlight Control 8
#define PWCTRL1		0xC0 //Power Control 1
#define PWCTRL2		0xC1 //Power Control 2
#define VMCTRL1		0xC5 //VCOM Control 1
#define VMCTRL2		0xC7 //VCOM Control 2
#define NVMWR		0xD0 //NV Memory Write
#define NVMPKEY		0xD1 //NV Memory Protection Key
#define RDNVM		0xD2 //NV Memory Status Read
#define RDID4		0xD3 //Read ID4
#define PGAMCTRL	0xE0 //Positive Gamma Control
#define NGAMCTRL	0xE1 //Negative Gamma Correction
#define DGAMCTRL1	0xE2 //Digital Gamma Control 1
#define DGAMCTRL2	0xE3 //Digital Gamma Control 2
#define IFCTL		0xF6 //16bits Data Format Selection
#define PWCTLA		0xCB //Power control A 
#define PWCTLB		0xCF //Power control B 
#define DTIMCTLA	0xE8 //Driver timing control A 
#define DTIMCTLB	0xEA //Driver timing control B 
#define PWONSCTL	0xED //Power on sequence control 
#define EN3G		0xF2 //Enable_3G 
#define PRCTL		0xF7 //Pump ratio control 


/* ���� */
#define LCD_ILI9341_CMD(index)       LCD_WR_REG(index)
#define LCD_ILI9341_Parameter(val)   LCD_WR_Data(val)

/* ����LCD�������õ�����ʱ */
#define DEBUG_DELAY()   Delay(2000)

unsigned char  display_direction = 0;
/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_REG
*	����˵��: д�Ĵ�����ַ����
*	��    ��: index �Ĵ�����ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_WR_REG(unsigned int index)
{
	ILI9341_REG = index;

}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_CMD
*	����˵��: д�Ĵ������ݺ���
*	��    ��: index �Ĵ�����ַ
*			  val   ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	ILI9341_REG = index;	
	ILI9341_RAM = val;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_CMD
*	����˵��: ��GRAM����-
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD_RD_data(void)	
{	
	uint16_t R=0, G=0, B=0 ;

	R = ILI9341_RAM; 	/*FIRST READ OUT DUMMY DATA*/
	R = ILI9341_RAM;  	/*READ OUT RED DATA  */
	B = ILI9341_RAM;  	/*READ OUT BLACK DATA*/
	G = ILI9341_RAM;  	/*READ OUT GREEN DATA*/
	
    return (((R>>11)<<11) | ((G>>10)<<5) | (B>>11)) ;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_WR_Data
*	����˵��: д16λ���ݺ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_WR_Data(unsigned int val)
{   
	ILI9341_RAM = val; 	
}

/*
*********************************************************************************************************
*	�� �� ��: Delay
*	����˵��: ��ʱ
*	��    ��: nCount ��ʱ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_FSMCConfig
*	����˵��: FSMC��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_FSMCConfig(void)
{	
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	
	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 1 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM4 configuration */
	p.FSMC_AddressSetupTime = 2;
	p.FSMC_AddressHoldTime = 0;
	
	/* LCD ����Ҫ����Ϊ4��ֻд�Ļ�����1���� */
	p.FSMC_DataSetupTime = 5;
	
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_B;
	
	/* Color LCD configuration ------------------------------------
	 LCD configured as follow:
	    - Data/Address MUX = Disable
	    - Memory Type = SRAM
	    - Data Width = 16bit
	    - Write Operation = Enable
	    - Extended Mode = Enable
	    - Asynchronous Wait = Disable */
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	/* BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_CtrlLinesConfig
*	����˵��: TFT��GPIO��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_CtrlLinesConfig(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ʹ��FSMCʱ��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
	    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | 
	                            RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG | 
	                            RCC_APB2Periph_GPIOF , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* ����LCD������ƹܽ�*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* ����LCD��λ���ƹܽ�*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 	 
    GPIO_Init(GPIOG, &GPIO_InitStructure);  		   
    
    /* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* ����FSMC���Ӧ�Ŀ�����
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
    GPIO_Init(GPIOG, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; 
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
	/* �򿪱��� */
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

/*
*********************************************************************************************************
*	�� �� ��: Set_direction
*	����˵��: ����ILI9341GRAMָ��ɨ�跽��
*	��    ��: 0�� ����ɨ��
*			  1�� ����ɨ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Set_direction(u8 option)
{

    switch(option)
    {
    case 0:
	{	    /*����*/
	   	DEBUG_DELAY();
        LCD_ILI9341_CMD(0x36);
		DEBUG_DELAY(); 
		LCD_ILI9341_Parameter(0x68);    //����
		DEBUG_DELAY();
		LCD_ILI9341_CMD(0X2A); 
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0x00);	//start 
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0x00);
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0x01);	//end
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0x3F);
		DEBUG_DELAY();		
		LCD_ILI9341_CMD(0X2B); 
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0x00);   //start
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0x00);
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0x00);   //end
		DEBUG_DELAY();
		LCD_ILI9341_Parameter(0xEF);
		DEBUG_DELAY();
		display_direction = 0;		
        }break;
    case 1:
        {		/*����*/
        LCD_ILI9341_CMD(0x36); 
	    LCD_ILI9341_Parameter(0x48);	//���� 
	    LCD_ILI9341_CMD(0X2A); 
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0xEF);	
		
		LCD_ILI9341_CMD(0X2B); 
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x01);
		LCD_ILI9341_Parameter(0x3F);
        display_direction = 1;
        }break;   
    }
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_Reset
*	����˵��: ��λ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_Reset(void)
{			
	/*��λ LCD*/	 
    GPIO_ResetBits(GPIOG, GPIO_Pin_11);
    Delay(0xAFFf<<2);					   
    GPIO_SetBits(GPIOG, GPIO_Pin_11 );	
	Delay(0xAFFf<<2);	
}

/*
*********************************************************************************************************
*	�� �� ��: Set_direction
*	����˵��: ����ILI9341GRAMָ��ɨ�跽��
*	��    ��: 0�� ����ɨ��
*			  1�� ����ɨ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLCD(void)
{
	/* ����LCD���ƿ���GPIO */
	LCD_CtrlLinesConfig();

	/* ����FSMC�ӿڣ��������� */
	LCD_FSMCConfig();

	/* ��λ */
	LCD_Reset();
		
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCF);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x81);
	LCD_ILI9341_Parameter(0x30);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xED);
	LCD_ILI9341_Parameter(0x64);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x12);
	LCD_ILI9341_Parameter(0x81);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xE8);
	LCD_ILI9341_Parameter(0x85);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x78);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCB);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x2C);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x34);
	LCD_ILI9341_Parameter(0x02);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xF7);
	LCD_ILI9341_Parameter(0x20);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xEA);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB1);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x1B);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB6);
	LCD_ILI9341_Parameter(0x0A);
	LCD_ILI9341_Parameter(0xA2);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC0);
	LCD_ILI9341_Parameter(0x35);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC1);
	LCD_ILI9341_Parameter(0x11);
	
	LCD_ILI9341_CMD(0xC5);
	LCD_ILI9341_Parameter(0x45);
	LCD_ILI9341_Parameter(0x45);
	
	LCD_ILI9341_CMD(0xC7);
	LCD_ILI9341_Parameter(0xA2);
	
	LCD_ILI9341_CMD(0xF2);
	LCD_ILI9341_Parameter(0x00);
	
	LCD_ILI9341_CMD(0x26);
	LCD_ILI9341_Parameter(0x01);
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xE0); //Set Gamma
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x26);
	LCD_ILI9341_Parameter(0x24);
	LCD_ILI9341_Parameter(0x0B);
	LCD_ILI9341_Parameter(0x0E);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x54);
	LCD_ILI9341_Parameter(0xA8);
	LCD_ILI9341_Parameter(0x46);
	LCD_ILI9341_Parameter(0x0C);
	LCD_ILI9341_Parameter(0x17);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_CMD(0XE1); //Set Gamma
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x19);
	LCD_ILI9341_Parameter(0x1B);
	LCD_ILI9341_Parameter(0x04);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x2A);
	LCD_ILI9341_Parameter(0x47);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x30);
	LCD_ILI9341_Parameter(0x38);
	LCD_ILI9341_Parameter(0x0F);
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x36); 
	LCD_ILI9341_Parameter(0x68);    /*����  ���Ͻǵ�(���)�����½�(�յ�)ɨ�跽ʽ*/
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x01);
	LCD_ILI9341_Parameter(0x3F);
	
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0xEF);

	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x3a); // Memory Access Control
	LCD_ILI9341_Parameter(0x55);
	LCD_ILI9341_CMD(0x11); //Exit Sleep
	LCD_ILI9341_CMD(0x29); //display on
    
	Set_direction(0);	  //����0-->��ʼ��Ĭ��Ϊ������ʽ
	LCD_ILI9341_CMD(0x2c); 						
}

/*
*********************************************************************************************************
*	�� �� ��: Set_direction
*	����˵��: ������Ļ����
*	��    ��: x      ������
*             y      ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetCursor(u16 x,u16 y)	
{				
	/*����*/
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(x>>8);
	LCD_ILI9341_Parameter(x-((x>>8)<<8));
    LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(y>>8);
	LCD_ILI9341_Parameter(y-((y>>8)<<8)); 
	LCD_ILI9341_CMD(0x2c);  	     
}

/*
*********************************************************************************************************
*	�� �� ��: SetWindows
*	����˵��: ���ô�������
*	��    ��: StartX     ����ʼ����
*             StartY     ����ʼ����
*             EndX       �н�������
*             EndY       �н�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY) 
{
	/*����*/
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(StartX>>8);	//start 
	LCD_ILI9341_Parameter(StartX-((StartX>>8)<<8));
	LCD_ILI9341_Parameter((EndX)>>8);	//end
	LCD_ILI9341_Parameter((EndX)-(((EndX)>>8)<<8));
	
	LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(StartY>>8);   //start
	LCD_ILI9341_Parameter(StartY-((StartY>>8)<<8));
	LCD_ILI9341_Parameter((EndY)>>8);   //end
	LCD_ILI9341_Parameter((EndY)-(((EndY)>>8)<<8)); 
	LCD_ILI9341_CMD(0x2c); 
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_Clear
*	����˵��: ����Ļ����ָ������ɫ��������������� 0xffff
*	��    ��: dat      ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_Clear(u16 dat)	
{
    u32 n;
    		
	/*����*/
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter((319)>>8);
	LCD_ILI9341_Parameter((319)-(((319)>>8)<<8));
	
	LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter((239)>>8);
	LCD_ILI9341_Parameter((239)-(((239)>>8)<<8));	
	LCD_ILI9341_CMD(0x2c);
	for(n=0; n<320*240; n++)
	LCD_WR_Data(dat); 
}

/*
*********************************************************************************************************
*	�� �� ��: GetPoint
*	����˵��: ��ȡָ���������ɫֵ
*	��    ��: x      ������
*             y      ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u16 GetPoint(u16 x,u16 y)
{ 
    u16 temp;
	SetWindows(x,y,x,y);
    LCD_ILI9341_CMD(0x2e);
	temp=LCD_RD_data();
    return (temp);
}

/*
*********************************************************************************************************
*	�� �� ��: SetPoint
*	����˵��: ��ָ�����껭��
*	��    ��: x      ������
*             y      ������
*             point  �����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetPoint(u16 x,u16 y,u16 point)	
{   	
	/*����*/
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(x>>8);
	LCD_ILI9341_Parameter(x-((x>>8)<<8));
	LCD_ILI9341_Parameter((x)>>8);
	LCD_ILI9341_Parameter((x)-(((x)>>8)<<8));
	
	LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(y>>8);
	LCD_ILI9341_Parameter(y-((y>>8)<<8));
	LCD_ILI9341_Parameter((y)>>8);
	LCD_ILI9341_Parameter((y)-(((y)>>8)<<8));
	LCD_ILI9341_CMD(0x2c);
	LCD_WR_Data(point); 	

}

/*
*********************************************************************************************************
*	�� �� ��: LCD_open_windows
*	����˵��: ���ô�������
*	��    ��: -x    �������
           	  -y	�������
              -len  ������ 
              -wid ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_open_windows(u16 x,u16 y,u16 len,u16 wid)
{                    

    if(display_direction == 0)		/*����Ǻ���ѡ��*/
    {

		LCD_ILI9341_CMD(0X2A); 
		LCD_ILI9341_Parameter(x>>8);	//start 
		LCD_ILI9341_Parameter(x-((x>>8)<<8));
		LCD_ILI9341_Parameter((x+len-1)>>8);	//end
		LCD_ILI9341_Parameter((x+len-1)-(((x+len-1)>>8)<<8));
		
		LCD_ILI9341_CMD(0X2B); 
		LCD_ILI9341_Parameter(y>>8);   //start
		LCD_ILI9341_Parameter(y-((y>>8)<<8));
		LCD_ILI9341_Parameter((y+wid-1)>>8);   //end
		LCD_ILI9341_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));
     
    }
    else
    {
		LCD_ILI9341_CMD(0X2B); 
		LCD_ILI9341_Parameter(x>>8);
		LCD_ILI9341_Parameter(x-((x>>8)<<8));
		LCD_ILI9341_Parameter((x+len-1)>>8);
		LCD_ILI9341_Parameter((x+len-1)-(((x+len-1)>>8)<<8));

	    LCD_ILI9341_CMD(0X2A); 
		LCD_ILI9341_Parameter(y>>8);
		LCD_ILI9341_Parameter(y-((y>>8)<<8));
		LCD_ILI9341_Parameter((y+wid-1)>>8);
		LCD_ILI9341_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));	
        
    }

      LCD_ILI9341_CMD(0x2c);     
}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_DrawHLine
*	����˵��: ��ˮƽ�� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor)
{
	uint16_t i;
	
	LCD_open_windows(_usX1, _usY1,_usX2-_usX1+1,1);

	for (i = 0; i <_usX2-_usX1+1; i++)
	{
		ILI9341_RAM = _usColor;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_DrawVLine
*	����˵��: ����ֱƽ�� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor)
{
	uint16_t i;
	
	for (i = _usY1; i <=_usY2; i++)
	{	
		SetPoint(_usX1, i, _usColor);	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: LCD9341_DrawVLine
*	����˵��: ��������� ��UCGUI�Ľӿں���
*	��    ��: X,Y���������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD9341_FillRect(uint16_t _usX1 , uint16_t _usY1 ,  uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{                    
   uint32_t n, temp;

   LCD_open_windows(_usX1,_usY1,_usX2-_usX1+1,_usY2 -_usY1+1); 
     
   temp = (u32)(_usX2-_usX1+1)*(_usY2 -_usY1+1);
       
   for(n=0; n<temp; n++)
   {
		ILI9341_RAM =_usColor;
   }
	 	  
}
