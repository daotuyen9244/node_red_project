/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ����ģ�顣
*	�ļ����� : dso.c
*	��    �� : V2.0
*	˵    �� : ʾ����������
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-08-27 armfly  ST�̼���V3.5.0�汾��
*		v2.0    2011-10-16 armfly  �Ż����̽ṹ��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "bsp_tft_lcd.h"
#include "bsp_timer.h"
#include "bsp_button.h"
#include "dso.h"
#include "WaveMaker.h"

#define DSO_VER	"V0.7"		  /* ��ǰ�汾 */

/* ������״̬�� */
enum
{
	S_HELP		= 0,	/* ����ҳ�� */
	S_RUN		= 1,	/* �ɼ�״̬ */
	S_HOLD		= 2		/* ��ͣ״̬ */
};

/* ʱ��ֶȱ� g_DSO.TimeBaseId��Ϊ��������  */
const uint32_t g_TimeTable[] =
{
	20, 	50, 	100, 
	200,	500,	1000,
	2000,	5000,	10000,
	20000,	50000,	100000,
	200000,	500000,	1000000		
};

/* ˥��������  
	g_DSO.Ch1Attenuation = 23
	����˥������:
	(1) 28 === 1V  (1:1)
*/
#define ATT_COUNT	6
const uint32_t g_AttTable[ATT_COUNT][2] =
{
	/* {����,ÿ����ѹ}  1:1 */
	{1400,	5000},
	{560,	2000},
	{280,	1000},
	{140,	500},
	{56,	200},
	{28,	100},
};

static void DsoHelp(uint8_t *pMainStatus);
static void DsoRun(uint8_t *pMainStatus);

/*
	������STM32F103ZE-EK ���߷��䣺

	ʾ����ͨ��1ͬʱ���ӵ�CPU��PC0��PC2��PC3����3��������ΪADC����ʹ��
		PC0 = ADC1_IN10
		PC2 = ADC2_IN12
		PC3 = ADC3_IN13

	ʾ����ͨ��2ֻ���ӵ�CPU��PC1�����������ΪADC����ʹ��
		PC1 = ADC2_IN12

	����ͨ��1��ģ���źţ�����ͬʱ����3��ADC���н�������������Ͽ���ʵ��3Mbps�������� 
	(��������ADC2����DMA�����������仰���д�����)

	ͨ��2ֻ������1��ADC���в�����
	֧��ͨ��1��ͨ��2����ͬ��������
*/

DSO_T g_DSO;	/* ȫ�ֱ�������һ���ṹ�� */

/*
*********************************************************************************************************
*	�� �� ��: DsoMain
*	����˵��: DSO��������һ����ѭ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DsoMain(void)
{
	uint8_t MainStatus = S_RUN;		/* ����ִ��״̬ */

	InitWaveMaker();	/* ��ʼ��DAC������DAC1���Ų���Ƶ��10KHz������2V�����Ҳ� */

	LCD_InitHard();		/* ��ʾ����ʼ�� */

	LCD_SetBackLight(BRIGHT_DEFAULT);  /* ���ñ������� */

	/* ����������ѭ���� */
	while (1)
	{
		switch (MainStatus)
		{
			case S_HELP:
				DsoHelp(&MainStatus);		/* ��ʾ���� */
				break;

			case S_RUN:
				DsoRun(&MainStatus);		/* ȫ�ٲɼ���ʵʱ��ʾ */
				break;

			default:
				break;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: InitDSO
*	����˵��: ��ʾ����ͨ��1���г�ʼ�����á���Ҫ���GPIO�����á�ADC�����á�DMA���á�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitDSO(void)
{
	/* ����GPIO.  */
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* ��GPIO_C �� AFIO ��ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO, ENABLE);

		/* ����PC0Ϊģ������ģʽ */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}


	/* ����DMA1_1 ����CH1 */
	#if 1
	{
		DMA_InitTypeDef DMA_InitStructure;

		/* ʹ�� DMA1 ʱ�� */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

		DMA_DeInit(DMA1_Channel1);		/* ��λDMA1�Ĵ�����ȱʡ״̬ */
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	/* ѡ��ADC1�����ݼĴ�����ΪԴ */
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch1Buf;	/* Ŀ���ַ */
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* ����DMA���䷽������(ADC)��ΪԴ */
		DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	/* ���û�������С */
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	/* �����ַ������ */
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	/* �洢����ַ��Ҫ���� */
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	/* ѡ���ڴ洫�䵥λ��16bit */
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	/* ����ѭ��ģʽ   */
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;	/* ѡ��DMA���ȼ� */
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	/* DMA�������ͣ������ڴ浽�ڴ� */
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);

		/* ʹ�� DMA1 ͨ��1 */
		DMA_Cmd(DMA1_Channel1, ENABLE);
	}
	#endif

	/* ����DMA2_5 ����CH2 */
	#if 1
	{
		DMA_InitTypeDef DMA_InitStructure;

		/* ʹ�� DMA2 ʱ�� */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

		DMA_DeInit(DMA2_Channel5);		/* ��λDMA2�Ĵ�����ȱʡ״̬ */
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	/* ѡ��ADC3�����ݼĴ�����ΪԴ */
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch2Buf;	/* Ŀ���ַ */
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* ����DMA���䷽������(ADC)��ΪԴ */
		DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	/* ���û�������С */
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	/* �����ַ������ */
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	/* �洢����ַ��Ҫ���� */
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	/* ѡ���ڴ洫�䵥λ��16bit */
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	/* ����ѭ��ģʽ   */
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	/* ѡ��DMA���ȼ� */
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	/* DMA�������ͣ������ڴ浽�ڴ� */
		DMA_Init(DMA2_Channel5, &DMA_InitStructure);

		/* Enable DMA2 channel5 */
		DMA_Cmd(DMA2_Channel5, ENABLE);
	}
	#endif

	/* ����ADC1  */
	
	 /* ADCCLK = PCLK2/2 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);

	#if 1
	{
		ADC_InitTypeDef	ADC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		/* ����ת����ֹ */
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;	/* ѡ��TIM1��CC3������ */

		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	/* �����Ҷ���,��λΪ0 */
		ADC_InitStructure.ADC_NbrOfChannel = 1;	/* 1��ͨ�� */
		ADC_Init(ADC1, &ADC_InitStructure);

		/* ADC1 ����ͨ������ */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);

		/* ʹ�� ADC1 �ⲿ����ת�� */
		ADC_ExternalTrigConvCmd(ADC1, ENABLE);

		/* ʹ�� ADC1 */
		ADC_Cmd(ADC1, ENABLE);

		/* ʹ�� ADC1 DMA */
		ADC_DMACmd(ADC1, ENABLE);

		/* ʹ�� ADC1 ��λУ׼�Ĵ��� */
		ADC_ResetCalibration(ADC1);
		/* ��⸴λУ׼�Ĵ��� */
		while(ADC_GetResetCalibrationStatus(ADC1));

		/* ��ʼ ADC1 У׼ */
		ADC_StartCalibration(ADC1);
		/* �ȴ�У׼���� */
		while(ADC_GetCalibrationStatus(ADC1));
	}
	#endif

	/* ����ADC3  */
	#if 1
	{
		ADC_InitTypeDef	ADC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		/* ����ת����ֹ */
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;	/* ѡ��TIM1��CC3������ */
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	/* �����Ҷ���,��λΪ0 */
		ADC_InitStructure.ADC_NbrOfChannel = 1;	/* 1��ͨ�� */
		ADC_Init(ADC3, &ADC_InitStructure);

		/* ADC3 ����ͨ��ת�� */
		ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 1, ADC_SampleTime_1Cycles5);

		/* ʹ�� ADC3 �ⲿ����ת�� */
		ADC_ExternalTrigConvCmd(ADC3, ENABLE);

		/* ʹ�� ADC3 */
		ADC_Cmd(ADC3, ENABLE);

		/* Enable ADC3 DMA */
		ADC_DMACmd(ADC3, ENABLE);

		/* ʹ�� ADC3 ��λУ׼�Ĵ��� */
		ADC_ResetCalibration(ADC3);
		/* ��⸴λУ׼�Ĵ��� */
		while(ADC_GetResetCalibrationStatus(ADC3));

		/* ��ʼ ADC3 У׼ */
		ADC_StartCalibration(ADC3);
		/* �ȴ�У׼���� */
		while(ADC_GetCalibrationStatus(ADC3));
	}
	#endif

	/* ���ò���������ʱ����ʹ��TIM1 CC1 */
	SetSampRate(g_DSO.SampleFreq);	/* �޸Ĳ���Ƶ��(����ʱ100K) */
}

/*
*********************************************************************************************************
*	�� �� ��: SetSampRate
*	����˵��: �޸Ĳ���Ƶ��
*	��    �Σ�freq : ����Ƶ�� ��λHz
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SetSampRate(uint32_t freq)
{
	/* ��4���� ���ö�ʱ��2��2ͨ��CC2, CC2��ΪADC1�Ĵ���Դ */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t Period;

	TIM_Cmd(TIM1, DISABLE);

	/*
	����Ƶ�ʼ��㹫ʽ ��
		period = 72000000 / freq ;
		
		1200 == 60KHz ����Ƶ��
	*/
	Period = 72000000 / freq;		

	/* ʹ�� TIM1 ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = Period;          
	TIM_TimeBaseStructure.TIM_Prescaler = 0;   /* ����Ƶ�� = 72000 000 / 18 = 4000 000	 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* TIM1 ͨ��1����ΪPWMģʽ */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
	TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	/* TIM1 ������ʹ�� */
	TIM_Cmd(TIM1, ENABLE);

	/* TIM1 ���ʹ�� */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);  
}





/*
*********************************************************************************************************
*	�� �� ��: DispCh1Wave
*	����˵��: ��ʾͨ��1����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispCh1Wave(void)
{
	int16_t i;		/* �з����� */
	uint16_t *px;
	uint16_t *py;
	int16_t iTemp;

	static uint8_t s_DispFirst = 0;		/* ���ڵ�һ�ε���ʱˢ�� */

	/* ��ʾͨ��1��ƽ���, һ���̵�ˮƽ���ߣ��ڲ�������� */
	{
		static uint16_t y = 70;

		LCD_DrawLine(1, y, 6, y, CL_BLUE);

		y = g_DSO.Ch1VOffset;

		if (y < 20)
		{
			y = 20;
		}
		else if (y > 220)
		{
			y = 220;
		}
		LCD_DrawLine(1, y, 6, y, CL_YELLOW);
	}

	if (s_DispFirst == 0)
	{
		/* ֻ�ڽ����ʱ����һ�������Ժ�������������������ص���˸�� */
		s_DispFirst = 1;
		LCD_ClrScr(CL_BLUE);  	/* ������������ɫ */
	}

	if (g_DSO.BufUsed == 0)
	{
		g_DSO.BufUsed = 1;
	}
	else
	{
		g_DSO.BufUsed = 0;
	}

	if (g_DSO.BufUsed == 0)
	{
		px = g_DSO.xCh1Buf1;
		py = g_DSO.yCh1Buf1;
	}
	else
	{
		px = g_DSO.xCh1Buf2;
		py = g_DSO.yCh1Buf2;
	}

	/* CH1ͨ��, ����0Vʱ��ADC����ֵ = 1872 ,���Ǹ��ݶ���������󵽵�ƽ��ֵ */
	for (i = 0; i < 300; i++)
	{
		px[i] = 10 + i;
		iTemp = g_DSO.Ch1VOffset + (int16_t)((1872 - g_DSO.Ch1Buf[i + 1]) * 10) / g_DSO.Ch1Attenuation;

		if (iTemp > 220)
		{
			iTemp = 220;
		}
		else if (iTemp < 20)
		{
			iTemp = 20;
		}
		py[i] = iTemp;
	}

	/* ����Ĵ����ȡ�����֡���Σ�Ȼ������������²��Σ���������ˢ�µ���˸�� */
	/* �����֡���� */
	if (g_DSO.BufUsed == 0)
	{
		LCD_DrawPoints(g_DSO.xCh1Buf2, g_DSO.yCh1Buf2, 300, CL_BLUE);
	}
	else
	{
		LCD_DrawPoints(g_DSO.xCh1Buf1, g_DSO.yCh1Buf1, 300, CL_BLUE);
	}

	/* ��ʾ���µĲ��� */
	LCD_DrawPoints((uint16_t *)px, (uint16_t *)py, 300, CL_YELLOW);
}
/*
*********************************************************************************************************
*	�� �� ��: InitDsoParam
*	����˵��: ��ʼ��ȫ�ֲ�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitDsoParam(void)
{
	g_DSO.Ch1Attenuation = 23;	/* ����ȱʡ˥��ϵ�� */
	g_DSO.Ch2Attenuation = 23;	/* ����ȱʡ˥��ϵ�� */
	g_DSO.Ch1VScale = 1000;	/* ȱʡ��1V */
	g_DSO.Ch2VScale = 1000;	/* ȱʡ��1V */

	g_DSO.Ch1VOffset = 70; 	/* ͨ��1 GND��λ�� */
	g_DSO.Ch2VOffset = 170; /* ͨ��2 GND��λ�� */

	g_DSO.ActiveCH = 1;		/* ȱʡ��CH1 */
	g_DSO.AdjustMode = 1;	/* ȱʡ�ǵ��ڴ�ֱƫ�ƣ� �����л���2���ڷ��� */

	g_DSO.HoldEn = 0; 		/* ��ͣˢ����־ */

	g_DSO.TimeBaseId = 0;	/* ʱ��ֶȲ�ѯ������ */								 
	g_DSO.TimeBase = 	g_TimeTable[g_DSO.TimeBaseId];	 /* ȱʡʱ��ֶ� */
	g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;  /* �������Ƶ�� */


	g_DSO.Ch1AttId = 2;
	g_DSO.Ch1Attenuation = g_AttTable[g_DSO.Ch1AttId][0];
	g_DSO.Ch1VScale =  g_AttTable[g_DSO.Ch1AttId][1];

	g_DSO.Ch2AttId = 2;
	g_DSO.Ch2Attenuation = g_AttTable[g_DSO.Ch2AttId][0];
	g_DSO.Ch2VScale =  g_AttTable[g_DSO.Ch2AttId][1];

}

/*
*********************************************************************************************************
*	�� �� ��: IncSampleFreq
*	����˵��: ���Ӳ���Ƶ�ʣ��� 1-2-5 ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void IncSampleFreq(void)
{
	/*	ʱ����ֶȣ�ÿ1������ʱ��)
		20us 	      1250000
		50us 		   500000
		100us		   250000
		200us		   125000
		500us		    50000
		1ms				 2500
		2ms				 1250
		5ms				  500 
		10ms			  250
		20ms			  125
		50ms			   50
		100ms			   25
	*/
	if (g_DSO.TimeBaseId < (sizeof(g_TimeTable) / 4) - 1)
	{
		g_DSO.TimeBaseId++;
	}
													 
	g_DSO.TimeBase = 	g_TimeTable[g_DSO.TimeBaseId];
	g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;

	SetSampRate(g_DSO.SampleFreq);	/* �ı����Ƶ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: DecSampleFreq
*	����˵��: ���Ͳ���Ƶ�ʣ��� 1-2-5 ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DecSampleFreq(void)
{
	if (g_DSO.TimeBaseId > 0)
	{
		g_DSO.TimeBaseId--;
	}
													 
	g_DSO.TimeBase = g_TimeTable[g_DSO.TimeBaseId];
	g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;

	SetSampRate(g_DSO.SampleFreq);	/* �ı����Ƶ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: AdjustAtt
*	����˵��: ���ڲ�����ʾ����
*	��    �Σ�	ch   : ͨ���ţ�1��2
*				mode : 0 ���ͣ� 1����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AdjustAtt(uint8_t ch, uint8_t mode)
{

	if (ch == 1)
	{
		if (mode == 0) 	/* ���� */
		{
			if (g_DSO.Ch1AttId > 0)
			{
				g_DSO.Ch1AttId--;
			}
		}
		else		/* ���� */
		{
			if (g_DSO.Ch1AttId < ATT_COUNT - 1)
			{
				g_DSO.Ch1AttId++;
			}
		}

		g_DSO.Ch1Attenuation = g_AttTable[g_DSO.Ch1AttId][0];
		g_DSO.Ch1VScale =  g_AttTable[g_DSO.Ch1AttId][1];
	}
	else if (ch == 2)
	{
		if (mode == 0) 	/* ���� */
		{
			if (g_DSO.Ch2AttId > 0)
			{
				g_DSO.Ch2AttId--;
			}
		}
		else		/* ���� */
		{
			if (g_DSO.Ch2AttId < ATT_COUNT - 1)
			{
				g_DSO.Ch2AttId++;
			}
		}
		g_DSO.Ch2Attenuation = g_AttTable[g_DSO.Ch2AttId][0];
		g_DSO.Ch2VScale =  g_AttTable[g_DSO.Ch2AttId][1];
	}
}

/*
*********************************************************************************************************
*	�� �� ��: StopADC
*	����˵��: ��ͣADC����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void StopADC(void)
{
	TIM_Cmd(TIM1, DISABLE);

	/* ��ֹ DMA1 ͨ��1 */
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	/* ��ֹ DMA2 ͨ��5 */
	DMA_Cmd(DMA2_Channel5, DISABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: StartADC
*	����˵��: ��������ADC����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void StartADC(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel1);		/* ��λDMA1�Ĵ�����ȱʡ״̬ */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	/* ѡ��ADC1�����ݼĴ�����ΪԴ */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch1Buf;	/* Ŀ���ַ */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* ����DMA���䷽������(ADC)��ΪԴ */
	DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	/* ���û�������С */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	/* �����ַ������ */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	/* �洢����ַ��Ҫ���� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	/* ѡ���ڴ洫�䵥λ��16bit */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	/* ����ѭ��ģʽ   */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	/* ѡ��DMA���ȼ� */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	/* DMA�������ͣ������ڴ浽�ڴ� */
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_DeInit(DMA2_Channel5);		/* ��λDMA2�Ĵ�����ȱʡ״̬ */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	/* ѡ��ADC1�����ݼĴ�����ΪԴ */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DSO.Ch2Buf;	/* Ŀ���ַ */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* ����DMA���䷽������(ADC)��ΪԴ */
	DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;	/* ���û�������С */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	/* �����ַ������ */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	/* �洢����ַ��Ҫ���� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	/* ѡ���ڴ洫�䵥λ��16bit */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	/* ����ѭ��ģʽ   */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	/* ѡ��DMA���ȼ� */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	/* DMA�������ͣ������ڴ浽�ڴ� */
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);

	/* ʹ�� DMA2 ͨ��1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ʹ�� DMA2 ͨ��5 */
	DMA_Cmd(DMA2_Channel5, ENABLE);

	TIM_Cmd(TIM1, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: DsoRun
*	����˵��: DSOȫ������״̬
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DsoRun(uint8_t *pMainStatus)
{
	uint8_t KeyCode;
	uint8_t fRefresh = 1;	/* LCDˢ�±�־ */
	


	LCD_ClrScr(CL_BLUE);  		/* ������������ɫ */

	bsp_StartTimer(1, 150);		/* ������ʱ��1��100msˢ��1�� */
	while (*pMainStatus == S_RUN)
	{
		CPU_IDLE();

		if (fRefresh)
		{
			fRefresh = 0;

			if (g_DSO.HoldEn == 1)		
			{
				DispDSO();
			}
		}

		if (bsp_CheckTimer(1))
		{
			bsp_StartTimer(1, 150);		/* ������ʱ��1��100msˢ��1�� */

			/* ����״̬��ÿ��100msˢ��1�β��� */
			if (g_DSO.HoldEn == 0)
			{
			
			}
		}

		/* ��ȡ����������0��ʾ�м����� */
		KeyCode = bsp_GetKey();
		if (KeyCode > 0)
		{
			/* �м����� */
			switch (KeyCode)
			{
				case  KEY_DOWN_TAMPER:	/* TAMPER ����ͨ��ѡ��(CH1��CH2) */
					if (g_DSO.ActiveCH == 1)
					{
						g_DSO.ActiveCH = 2;
					}
					else
					{
						g_DSO.ActiveCH = 1;
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case  KEY_DOWN_WAKEUP:	/* WAKEUP ��, ����ģʽѡ��(���Ȼ��ߴ�ֱƫ��) */
					/* �˳�,����ȫ������״̬ */
					*pMainStatus = S_HELP;
					break;

				case  KEY_DOWN_USER:	/* USER �� */
					if (g_DSO.HoldEn == 0)
					{
						g_DSO.HoldEn = 1;

						/* ������ͣʱ��ʱ��,Ϊ��ˮƽ��չ�� */
						g_DSO.TimeBaseIdHold = g_DSO.TimeBaseId;	

						StopADC();
					}
					else
					{
						g_DSO.HoldEn = 0;
						StartADC();				
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case KEY_DOWN_JOY_LEFT:	/* ҡ��LEFT������ */
					if (g_DSO.HoldEn == 0)
					{
						DecSampleFreq();	/* �ݼ�����Ƶ�� */
						fRefresh = 1;		/* ����ˢ��LCD */
					}
					else
					{
						; /* ����ˮƽ�ƶ��������� */
					}
					break;

				case KEY_DOWN_JOY_RIGHT:	/* ҡ��RIGHT������ */
					if (g_DSO.HoldEn == 0)
					{
						IncSampleFreq();  	/* ��������Ƶ�� */					
						fRefresh = 1;		/* ����ˢ��LCD */
					}
					else
					{
						; /* ����ˮƽ�ƶ��������� */
					}
					break;

				case  KEY_DOWN_JOY_OK:	/* ҡ��OK�� */
					if (g_DSO.AdjustMode == 0)
					{
						g_DSO.AdjustMode = 1;
					}
					else
					{
						g_DSO.AdjustMode = 0;
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case KEY_DOWN_JOY_UP:	/* ҡ��UP������ */
					if (g_DSO.ActiveCH == 1) 	/* ��ǰ�������CH1 */
					{
						if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(1, 1);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO.Ch1VOffset -= 5;
						}
					}
					else	/* ��ǰ�������CH2 */
					{
						if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(2, 1);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO.Ch2VOffset -= 5;
						}
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case KEY_DOWN_JOY_DOWN:		/* ҡ��DOWN������ */
					if (g_DSO.ActiveCH == 1) 	/* ��ǰ�������CH1 */
					{
						if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(1, 0);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO.Ch1VOffset += 5;
						}
					}
					else	/* ��ǰ�������CH2 */
					{
						if (g_DSO.AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(2, 0);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO.Ch2VOffset += 5;
						}
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				default:
					break;
			}
		}
	}
}
