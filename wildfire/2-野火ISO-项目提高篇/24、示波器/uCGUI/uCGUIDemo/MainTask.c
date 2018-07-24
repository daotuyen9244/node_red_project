
#include <includes.h>
#include "bsp.h"
#include "GUI.h"
#include "DIALOG.h"
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "dso.h"
#include "FFTInc.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"


/* ʱ��ֶȱ� g_DSO.TimeBaseId��Ϊ��������  */
/* ÿ������Ա�ʾ��������أ�1/1.250000M       */
/* ÿ������Ա�ʾ��������أ�1/1.000000M       */
const uint32_t g_TimeTable[] =
{
					
//	20, 	50, 	100, 
//	200,	500,	1000,
//	2000,	5000,	10000,
//	20000,	50000,	100000,
//	200000,	500000,	1000000
					16,
	20, 	40, 	100, 
	200,	400,	1000,
	2000,	4000,	10000,
	20000,	40000,	100000,
	200000,	400000,	1000000	
			
};

/****************************************************************** 
  ˥��������  
   (1)ÿ�������Ӧ����25���� ��ʾ 1V 
   (2)1.406V ��ӦADC��ֵ 1050  1050/1.406 =746.799;
    ��Ӧ��ϵ���� 25������ == 1V == ADC������746.799
	�ɿɵ� 1V��˥��������746.799/25 = 29.87
*******************************************************************/
#define ATT_COUNT	9
const uint32_t g_AttTable[ATT_COUNT][2] =
{
/* {����,ÿ����ѹ}  1:1 �����Ŵ���10�� ��Ϊǰ�汻�����Ŵ���10��*/
	{1450,	5000},
	{580,	2000},
	{290,	1000},
	{145,	500},
	{58,	200},
	{29,	100},
	{15,	50},
	{6,	    20},
	{3,	    10},
};

typedef struct
{
  unsigned char Hour;
  unsigned char Min;
  unsigned char Sec;
} Time;

#define DSO_VER			 "V1.0"		         /* ��ǰ�汾 */
#define GUI_ID_ADC       (GUI_ID_USER + 0)
#define GUI_ID_DAC  	 (GUI_ID_USER + 1)
#define GUI_ID_FFT   	 (GUI_ID_USER + 2)
#define GUI_ID_Spectrum  (GUI_ID_USER + 3)
#define GUI_ID_OSCLevel  (GUI_ID_USER + 4)


uint8_t fRefresh ;           //����LCD����ˢ�� 
DSO_T g_DSO;						/* ȫ�ֱ�������һ���ṹ�� */
extern const GUI_BITMAP bmpsu;
uint8_t ConfigRefresh ;
unsigned long Clock_count;
Time     Clock;
extern OS_EVENT *Semp;
uint8_t SpectrumFlag =0;
BUTTON_Handle    ahButton[4];
PROGBAR_Handle   ahProgbar[1];
SCROLLBAR_Handle ahScrollbar[1];
/*
*********************************************************************************************************
*	�� �� ��: InitDSO
*	����˵��: ��ʾ����ͨ��1���г�ʼ�����á���Ҫ���GPIO�����á�ADC�����á�DMA���á�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void InitDSO(void)
{
  	{
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO, ENABLE); //����IO��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			   			//ʹ�ܶ�ʱ��1ʱ��
		RCC_ADCCLKConfig(RCC_PCLK2_Div2);     										//ADCCLK = PCLK2/4 = 18MHz
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	#if 1
	{
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

		DMA_DeInit(DMA1_Channel1);													/* ��λDMA1�Ĵ�����ȱʡ״̬ */
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;					/* ѡ��ADC1�����ݼĴ�����ΪԴ */
		DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)&g_DSO.buffer;						/* Ŀ���ַ */
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							/* ����DMA���䷽������(ADC)��ΪԴ */
		DMA_InitStructure.DMA_BufferSize = SAMPLE_COUNT;							/* ���û�������С */
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			/* �����ַ������ */
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						/* �洢����ַ��Ҫ���� */
//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* ѡ�����贫�䵥λ��16bit */
//		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			/* ѡ���ڴ洫�䵥λ��16bit */
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	    /* ѡ�����贫�䵥λ��16bit */
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;			    /* ѡ���ڴ洫�䵥λ��16bit */
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								/* ����ѭ��ģʽ   */
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;						/* ѡ��DMA���ȼ� */
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								/* DMA�������ͣ������ڴ浽�ڴ� */
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel1, ENABLE);
	}
	#endif
	#if 1
	{
		ADC_InitTypeDef	ADC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2, ENABLE);

		  ADC_DeInit(ADC1);
		  ADC_DeInit(ADC2);
		  /* ADC1 configuration ------------------------------------------------------*/
		  ADC_InitStructure.ADC_Mode = ADC_Mode_FastInterl;
		  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;
		  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		  ADC_InitStructure.ADC_NbrOfChannel = 1;
		  ADC_Init(ADC1, &ADC_InitStructure);
		  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);//18M/(1.5+12.5)=1.2857M������Ƶ��  
		  ADC_ExternalTrigConvCmd(ADC1, ENABLE);
		  
		  /* ADC1 configuration ------------------------------------------------------*/
		  ADC_InitStructure.ADC_Mode = ADC_Mode_FastInterl;
		  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		  ADC_InitStructure.ADC_NbrOfChannel = 1;
		  ADC_Init(ADC2, &ADC_InitStructure);
		  ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5); //18M/(1.5+12.5)=1.2857M������Ƶ��
		  ADC_ExternalTrigConvCmd(ADC2, ENABLE);
		  
		  /* Enable ADC1 */
		  ADC_DMACmd(ADC1, ENABLE);
		  ADC_Cmd(ADC1, ENABLE);  
		  ADC_ResetCalibration(ADC1);
		  while(ADC_GetResetCalibrationStatus(ADC1));
		  ADC_StartCalibration(ADC1);
		  while(ADC_GetCalibrationStatus(ADC1));
		     
		  /* Enable ADC2 */
		  ADC_Cmd(ADC2, ENABLE);
		  ADC_ResetCalibration(ADC2);
		  while(ADC_GetResetCalibrationStatus(ADC2));
		  ADC_StartCalibration(ADC2);
		  while(ADC_GetCalibrationStatus(ADC2));
	}
	#endif
	#if 0
	{
		ADC_InitTypeDef	ADC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							/* ����ת����ֹ */
		//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;		/* ѡ��TIM1��CC1������ */
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;		/* ѡ��TIM2��CC3������ */
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						/* �����Ҷ���,��λΪ0 */
		ADC_InitStructure.ADC_NbrOfChannel = 1;										/* 1��ͨ�� */
		ADC_Init(ADC3, &ADC_InitStructure);		
		ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 1, ADC_SampleTime_1Cycles5);	/* ADC1 regular channels configuration */
		ADC_ExternalTrigConvCmd(ADC3, ENABLE);
		ADC_Cmd(ADC3, ENABLE);
		ADC_DMACmd(ADC3, ENABLE);
		ADC_ResetCalibration(ADC3);
		while(ADC_GetResetCalibrationStatus(ADC3));
		ADC_StartCalibration(ADC3);
		while(ADC_GetCalibrationStatus(ADC3));
	}
	#endif
	//SetSampRate(g_DSO.SampleFreq);													/* ���ò���������ʱ����ʹ��TIM1 CC3�޸Ĳ���Ƶ��(����ʱ100K) */
	#if 1
	{   
	    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		TIM_Cmd(TIM1, DISABLE);	
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 				   //��ʼ����ʱ��1�ļĴ���Ϊ��λֵ
		TIM_TimeBaseStructure.TIM_Period = 72000000 / g_DSO.SampleFreq;    //ARR�Զ���װ�ؼĴ������ڵ�ֵ(��ʱʱ�䣩������Ƶ�ʺ���������»����ж�(Ҳ��˵��ʱʱ�䵽)
		TIM_TimeBaseStructure.TIM_Prescaler = 0;   						   //PSCʱ��Ԥ��Ƶ�� ���磺ʱ��Ƶ��=TIM1CLK/(ʱ��Ԥ��Ƶ+1)
		TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    				   //CR1->CKDʱ��ָ�ֵ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	   //CR1->CMS[1:0]��DIR��ʱ��ģʽ ���ϼ���
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				   //CCMR2�����ϼ���ʱ��һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //CCER ���ʹ��          
		TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2;//CCR3ͬ������TIMx_CNT�ıȽϣ�����OC4�˿��ϲ�������ź� 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    	   //CCER�����������	�ߵ�ƽ��Ч     
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	    //TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);					//CMR2 ����Ԥװ��ʹ��  �����¼�����ʱд����Ч
		//TIM_ARRPreloadConfig(TIM1, ENABLE);		   		 				//CR1  ����ARR�Զ���װ �����¼�����ʱд����Ч
		TIM_Cmd(TIM1, ENABLE);											   
		TIM_CtrlPWMOutputs(TIM1, ENABLE);  								   //ʹ��PWM ���
	}
	#endif
}

/*
*********************************************************************************************************
*	�� �� ��: SetSampRate
*	����˵��: �޸Ĳ���Ƶ��
*	��    �Σ�freq : ����Ƶ�� ��λHz
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void SetSampRate(uint32_t freq)
{
  TIM1->ARR=72000000 / freq;  									   //�趨�������Զ���װֵ 
}
/*
*********************************************************************************************************
*	�� �� ��: DispCh1Wave
*	����˵��: ��ʾͨ��1����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void DispCh1Wave(void)
{
	int16_t i,j = 0;		/* �з����� */
	uint16_t *px;
	uint16_t *py;
	int16_t iTemp;
    uint16_t VccMax,VccMin;
	uint16_t Average;

      for(i=0;i<1024;i++)
	{ 
		
		g_DSO.Ch1Buf[j++]=g_DSO.buffer[i]>>16;		//ADC2
		g_DSO.Ch1Buf[j++]=g_DSO.buffer[i] & 0xFFFF;	//ADC1
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

	  GUI_SetColor(GUI_MAGENTA);
	
	 /* ����Ĵ�������ʵ�� ���ֵ����Сֵ�ò�ֵ  */
	 VccMax=g_DSO.Ch1Buf[0];
	 VccMin=g_DSO.Ch1Buf[0];
	 for(i=1;i<1024;i++)
	 {
	    if(VccMax<g_DSO.Ch1Buf[i])VccMax=g_DSO.Ch1Buf[i];
		if(VccMin>g_DSO.Ch1Buf[i])VccMin=g_DSO.Ch1Buf[i];
	 }
	  VccMax=VccMax-VccMin;
      GUI_GotoXY(323-55,104);
	  GUI_DispFloatFix((VccMax* 1.406)/1050,5,2);
	 
	 /* CH1ͨ��, ����0Vʱ��ADC����ֵ = 1900 ,1024�����󵽵�ƽ��ֵ */
	 /* ����1:1��ʱ�� 1.406��Ӧ��ADֵ��2951 -1901 = 1050  */
	  for(i=1;i<21;i++)
	 {
	    Average +=g_DSO.Ch1Buf[i];
	 } 
	 Average = Average / 20;
	 GUI_GotoXY(323-55,40);
	 GUI_DispFloatFix(((Average-1900) * 1.406)/1050,5,2);
	 

	 GUI_DispDecAt(g_DSO.Ch1Buf[0], 323-55, 72, 4);
	/* ����Ĵ�������ʵ�ֲ��ε�ˢ��  */
	for (i = 0; i < 300; i++)
	{
		px[i] = 15 + i;
		iTemp = g_DSO.Ch1VOffset + (int16_t)((1900 - g_DSO.Ch1Buf[i + 1]) * 10)/g_DSO.Ch1Attenuation ;
		if (iTemp > 200)
		{
			iTemp = 200;
		}
		else if (iTemp < 0)
		{
			iTemp = 0;
		}
		py[i] = iTemp;
	}

	/* ����Ĵ����ȡ�����֡���Σ�Ȼ������������²��Σ���������ˢ�µ���˸�� */
	/* �����֡���� */
	if (g_DSO.BufUsed == 0)
	{
		GUI_SetColor(GUI_BLUE);
	    GUI_DrawGraph((signed short *)g_DSO.yCh1Buf2, 250, 14,20);

	}
	else
	{
		GUI_SetColor(GUI_BLUE);
	    GUI_DrawGraph((signed short *)g_DSO.yCh1Buf1, 250, 14,20);
	}

	/* ��ʾ���µĲ��� */
	GUI_SetColor(GUI_YELLOW);
	GUI_DrawGraph((I16 *)py, 250, 14,20);
}
/*
*********************************************************************************************************
*	�� �� ��: InitDsoParam
*	����˵��: ��ʼ��ȫ�ֲ�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void InitDsoParam(void)
{				

	g_DSO.Ch1VOffset = 100; 				/* ͨ��1 GND��λ�� */
	g_DSO.Ch2VOffset = 220;

	g_DSO.AdjustMode = 0;					/* ȱʡ�ǵ��ڷŴ��˥�� */

	g_DSO.HoldEn = 0;
	g_DSO.TimeBaseId = 0;									 
	g_DSO.TimeBase   = g_TimeTable[g_DSO.TimeBaseId];
	g_DSO.SampleFreq = 25000000 / g_DSO.TimeBase;


	g_DSO.Ch1AttId = 2;
	g_DSO.Ch1Attenuation = g_AttTable[g_DSO.Ch1AttId][0];		/* ȱʡ��1V */
	g_DSO.Ch1VScale =  g_AttTable[g_DSO.Ch1AttId][1];			/* ����ȱʡ˥��ϵ�� */

}

/*
*********************************************************************************************************
*	�� �� ��: IncSampleFreq
*	����˵��: ���Ӳ���Ƶ�ʣ��� 1-2-5 ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void IncSampleFreq(void)
{
	/*	ʱ����ֶȣ�ÿ1������ʱ��)
	    10us          2500000		
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
 void DecSampleFreq(void)
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
 void AdjustAtt(uint8_t ch, uint8_t mode)
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
 void StopADC(void)
{
		DMA1_Channel1->CCR&=0xffffffe;          					//�ر�DMA����
}


/*******************************************************************************
*	������: DispButton
*	��  ��:	��
*	��  ��:	��
*	��  �ܣ���ʾ���δ����ұߵĹ��ܰ�ť������չ��
*******************************************************************************/
 void DispButton(void)
{
	GUI_SetColor(GUI_GREEN);
				
	GUI_DispStringAt(  "VCC",          323-55, 24);
	GUI_DispStringAt(  "          V",   323-55, 40);
	GUI_DispStringAt(   "ADC",         323-55, 56);
	GUI_DispStringAt(  "          N",     323-55, 72);
    GUI_DispStringAt(   "VPP",         323-55, 88);
	GUI_DispStringAt(  "          V",   323-55, 104);
	GUI_DispStringAt(  "PERIOD",       323-55, 120);
	GUI_DispStringAt(  "           H", 323-55, 136);
}
/*******************************************************************************
*	������: DispChInfo
*	��  ��:	��
*	��  ��:	��
*	��  �ܣ���ʾͨ����Ϣ
*******************************************************************************/
static void DispChInfo(void)
{
	char buf[32];   							/* �ַ���ʾ������ */
											

	GUI_SetColor(GUI_GREEN);   					/* ���� */
	GUI_DispStringAt( DSO_VER,15, 2);
		
	if (g_DSO.Ch1DC == 1)						/* ��ʾͨ��1��Ϣ */
	{
		strcpy(buf, "CH1 ");
	}
	else
	{
		strcpy(buf, "CH1 ");
	}

	if (g_DSO.Ch1VScale >= 1000)
	{
		sprintf(&buf[4], "%d.00V/div", g_DSO.Ch1VScale / 1000);
	}
	else
	{
		sprintf(&buf[4], "%dmV/div", g_DSO.Ch1VScale);
	}
	GUI_DispStringAt( buf,15, 224);
	
	
	/* ��ʾʱ�� */
	if (g_DSO.TimeBase <= 1000)
	{
		sprintf(buf, "Time %3dus/div", g_DSO.TimeBase/2);
	}
	else if (g_DSO.TimeBase < 1000000)
	{
		sprintf(buf, "Time %3dms/div  ", g_DSO.TimeBase / 2000);
	}
	else
	{
		sprintf(buf, "Time %3ds/div   ", g_DSO.TimeBase / 2000000);	
	}
	GUI_DispStringAt(buf, 170, 224);
	                                                                                                                                                                                                                                             
	
	if (g_DSO.AdjustMode == 1)
	{
        GUI_DispStringAt("ChangeFreq   ",170, 2);
	}
	else
	{
		GUI_DispStringAt("ChangeScope ",170, 2);
	}
	
	
	sprintf(buf, "Freq:%7dHz  ",	g_DSO.SampleFreq*2);
    GUI_DispStringAt(buf,52, 2);
}
/*
*********************************************************************************************************
*	�� �� ��: StartADC
*	����˵��: ��������ADC����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartADC(void)
{
	#if 1
	  DMA1_Channel1->CPAR=ADC1_DR_Address; 	 				    //DMA1 �����ַ 
	  DMA1_Channel1->CMAR=(uint32_t)&g_DSO.buffer; 			    //DMA1,�洢����ַ
	  DMA1_Channel1->CNDTR=SAMPLE_COUNT;    					//DMA1,����������
	  DMA1_Channel1->CCR=0X00000000;							//��λ
	  DMA1_Channel1->CCR|=0<<4;  								//����������
	  DMA1_Channel1->CCR|=0<<5;  								//��ͨģʽ
	  DMA1_Channel1->CCR|=0<<6;  								//�����ַ������ģʽ
	  DMA1_Channel1->CCR|=1<<7;  								//�洢������ģʽ
	  DMA1_Channel1->CCR|=2<<8;  								//�������ݿ��Ϊ32λ
	  DMA1_Channel1->CCR|=2<<10; 								//�洢�����ݿ��32λ
	  DMA1_Channel1->CCR|=2<<12; 								//�����ȼ�
	  DMA1_Channel1->CCR|=0<<14; 								//�Ǵ洢�����洢��ģʽ	
   #endif

	  DMA1_Channel1->CCR|=1;          							//����DMA����
	  //TIM1->CR1|=1;									    		//������ʱ��1
}
 void DispFrame(void)
{
	uint16_t x,y;
	GUI_SetColor(GUI_WHITE);					
	GUI_DrawRect(13, 19, 265, 221);						//�������������ֵ��Ӱ�� 
	for (x = 0; x < 11; x++)							/* ���ƴ�ֱ�̶ȵ� */
	{
		for (y = 0; y < 41; y++)
		{
			GUI_DrawPixel(14 + (x * 25), 20 + (y * 5));
		}
	}
	for (y = 0; y < 9; y++)								/* ����ˮƽ�̶ȵ� */
	{
		for (x = 0; x < 51; x++)
		{
			GUI_DrawPixel(14 + (x * 5), 20 + (y * 25));
		}
	}
	for (y = 0; y < 41; y++)							/* ���ƴ�ֱ���Ŀ̶ȵ� */
	{	 
		GUI_DrawPixel(13 + (5 * 25), 20 + (y * 5));
		GUI_DrawPixel(15 + (5 * 25), 20 + (y * 5));
	}	
	for (x = 0; x < 51; x++)							/* ����ˮƽ���Ŀ̶ȵ� */
	{	 
		GUI_DrawPixel(14 + (x * 5), 19 + (4 * 25));
		GUI_DrawPixel(14 + (x * 5), 21 + (4 * 25));
	}
}

/*
*********************************************************************************************************
*	�� �� ��: _cbBkWin
*	����˵��: �ص���������ײ�ģ�Ҳ�������洰��
*	��    �Σ�WM_MESSAGE* pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbBkWin(WM_MESSAGE* pMsg) {

  int NCode, Id;
  switch (pMsg->MsgId) {
 case WM_PAINT:
    break;
 case WM_NOTIFY_PARENT: 
     Id = WM_GetId(pMsg->hWinSrc); 
     NCode = pMsg->Data.v;
      switch (Id) {        
      case GUI_ID_ADC:
	      switch(NCode)
          {
            case WM_NOTIFICATION_RELEASED:
				if (g_DSO.AdjustMode == 0)		   /* ���ڷ��ȷŴ��� */
				{
					AdjustAtt(1, 1);
				}
				else 							    /* ��������ƫ�� */
				{
					DecSampleFreq();  				/* ��������Ƶ�� */	
				}
				fRefresh = 1;				        /* ����ˢ��LCD */
                 break;
          }
        break;				  
	  case GUI_ID_DAC:
	      switch(NCode)
          {
            case WM_NOTIFICATION_RELEASED:
				if (g_DSO.AdjustMode == 0)		   /* ���ڷ��ȷŴ��� */
				{
					AdjustAtt(1, 0);
				}
				else 							    /* ��������ƫ�� */
				{
					IncSampleFreq();				   /* �ݼ�����Ƶ�� */
				}
				fRefresh = 1;				        /* ����ˢ��LCD */
                break;
          }
        break;
	  case GUI_ID_FFT:   
	   	  switch(NCode)
          {
            case WM_NOTIFICATION_RELEASED:
				if (g_DSO.AdjustMode == 0)		   
				{
					g_DSO.AdjustMode = 1;
				}
				else
				{
					g_DSO.AdjustMode = 0;
				}
				fRefresh = 1;						/* ����ˢ��LCD */
                 break;
          }
        break;
	  case GUI_ID_Spectrum:
	       switch(NCode)
	       {
	         case WM_NOTIFICATION_RELEASED:
                if(SpectrumFlag == 1)
				{
				   GUI_Clear();
				   DispButton();
				   SpectrumFlag = 0;
				   BUTTON_SetText(ahButton[3],"Spectr");
				}
				else
				{
				   GUI_Clear();
				   DispButton();
				   SpectrumFlag = 1;
				   BUTTON_SetText(ahButton[3],"Spetrum2");
				}   
	            break;
	       }
        break;
	   case GUI_ID_OSCLevel:
	         switch(NCode)
            {
                case WM_NOTIFICATION_VALUE_CHANGED:
                    g_DSO.Ch1VOffset = SCROLLBAR_GetValue(ahScrollbar[0]);
                    break;
            }
            break;   
      }
	 break;
 case WM_TOUCH:

     break;
 case WM_DELETE:

     break;
  default:
    WM_DefaultProc(pMsg);
  }
}
/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void MainTask(void) 
{ 
    GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
	WM_EnableMemdev(WM_HBKWIN);
	WM_SetCallback(WM_HBKWIN, &_cbBkWin);


	GUI_SetBkColor(GUI_BLUE);
	GUI_SetFont(&GUI_Font16B_1);
	GUI_Clear();


    ahButton[0] = BUTTON_Create(318-52,152,75-21, 20, GUI_ID_ADC,  BUTTON_CF_SHOW );
	BUTTON_SetTextColor(ahButton[0],BUTTON_CI_UNPRESSED,GUI_GREEN);
    BUTTON_SetFont(ahButton[0],&GUI_Font16B_1);
	BUTTON_SetText(ahButton[0],"Inc");
		
	ahButton[1] = BUTTON_Create(318-52,174,75-21, 20,  GUI_ID_DAC,  BUTTON_CF_SHOW );
	BUTTON_SetTextColor(ahButton[1],BUTTON_CI_UNPRESSED,GUI_GREEN);
    BUTTON_SetFont(ahButton[1],&GUI_Font16B_1);
	BUTTON_SetText(ahButton[1],"Dec");

	ahButton[2] = BUTTON_Create(318-52,196,75-21, 20,  GUI_ID_FFT,  BUTTON_CF_SHOW );
	BUTTON_SetTextColor(ahButton[2],BUTTON_CI_UNPRESSED,GUI_GREEN);
    BUTTON_SetFont(ahButton[2],&GUI_Font16B_1);
	BUTTON_SetText(ahButton[2],"Switch");

	ahButton[3] = BUTTON_Create(318-52,218,75-21, 20,  GUI_ID_Spectrum, BUTTON_CF_SHOW );
	BUTTON_SetTextColor(ahButton[3],BUTTON_CI_UNPRESSED,GUI_GREEN);
    BUTTON_SetFont(ahButton[3],&GUI_Font16B_1);
	BUTTON_SetText(ahButton[3],"Spectr");

	
	ahProgbar[0]=PROGBAR_Create(318-52,2,75-21,20,WM_CF_SHOW);
    PROGBAR_SetBarColor(ahProgbar[0],0,GUI_MAGENTA);
    PROGBAR_SetBarColor(ahProgbar[0],1,GUI_RED);
    PROGBAR_SetFont(ahProgbar[0],&GUI_Font16B_1);
	PROGBAR_SetMinMax(ahProgbar[0],0,100);
	WIDGET_SetEffect(ahProgbar[0], &WIDGET_Effect_3D);

	ahScrollbar[0]=SCROLLBAR_Create(0,11,12,217,0,GUI_ID_OSCLevel,WM_CF_SHOW,SCROLLBAR_CF_VERTICAL);
    SCROLLBAR_SetNumItems(ahScrollbar[0],200);
	SCROLLBAR_SetValue(ahScrollbar[0],100);
	
	DispButton();
	DispChInfo();
	ConfigRefresh=0;
	fRefresh =1;
	while(1)
	{ 
	   if(ConfigRefresh == 0) 
	   { 
		  if (fRefresh)
		  {
			  fRefresh = 0;
			  if(g_DSO.HoldEn == 1)
			{		 
			   DispChInfo();	
			}	 
			  
		  }
		  if (g_DSO.HoldEn == 0)
		  {
		    StopADC();	  			 /* ��ͣ���� */			 
	 		DispFrame();
			DispChInfo();
			DispCh1Wave();
			FFT_Convert();
		    StartADC();	  			 /* ��ʼ���� */
		  }
		  PROGBAR_SetValue(ahProgbar[0],OSCPUUsage);
	   }
		GUI_Delay(50); 			 
	}
}





