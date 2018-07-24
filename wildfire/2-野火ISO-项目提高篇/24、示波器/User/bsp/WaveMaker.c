#include "stm32f10x.h"
#include <stdio.h>
#include "math.h"
#include "WaveMaker.h"
#include "dso.h"
/*******************************************************************************
	���Ҳ����ݣ�12bit��1������128����
	������PA4���, PA4 ���� DAC_OUT1�� DAC�Ĳο���ѹ = 3.3V
*******************************************************************************/
const uint16_t g_SineWave128[] = {
	2047 ,
	2147 ,
	2248 ,
	2347 ,
	2446 ,
	2544 ,
	2641 ,
	2737 ,
	2830 ,
	2922 ,
	3012 ,
	3099 ,
	3184 ,
	3266 ,
	3346 ,
	3422 ,
	3494 ,
	3564 ,
	3629 ,
	3691 ,
	3749 ,
	3803 ,
	3852 ,
	3897 ,
	3938 ,
	3974 ,
	4006 ,
	4033 ,
	4055 ,
	4072 ,
	4084 ,
	4092 ,
	4094 ,
	4092 ,
	4084 ,
	4072 ,
	4055 ,
	4033 ,
	4006 ,
	3974 ,
	3938 ,
	3897 ,
	3852 ,
	3803 ,
	3749 ,
	3691 ,
	3629 ,
	3564 ,
	3494 ,
	3422 ,
	3346 ,
	3266 ,
	3184 ,
	3099 ,
	3012 ,
	2922 ,
	2830 ,
	2737 ,
	2641 ,
	2544 ,
	2446 ,
	2347 ,
	2248 ,
	2147 ,
	2047 ,
	1947 ,
	1846 ,
	1747 ,
	1648 ,
	1550 ,
	1453 ,
	1357 ,
	1264 ,
	1172 ,
	1082 ,
	995  ,
	910  ,
	828  ,
	748  ,
	672  ,
	600  ,
	530  ,
	465  ,
	403  ,
	345  ,
	291  ,
	242  ,
	197  ,
	156  ,
	120  ,
	88   ,
	61   ,
	39   ,
	22   ,
	10   ,
	2    ,
	0    ,
	2    ,
	10   ,
	22   ,
	39   ,
	61   ,
	88   ,
	120  ,
	156  ,
	197  ,
	242  ,
	291  ,
	345  ,
	403  ,
	465  ,
	530  ,
	600  ,
	672  ,
	748  ,
	828  ,
	910  ,
	995  ,
	1082 ,
	1172 ,
	1264 ,
	1357 ,
	1453 ,
	1550 ,
	1648 ,
	1747 ,
	1846 ,
	1947
};
/* ���Ҳ� */
const uint16_t g_SineWave32[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

/* ���� */
const uint16_t g_SineWave32_FangBo[32] = {
                      0, 0, 0, 0, 0, 0, 0, 0,  
					  0, 0, 0, 0, 0, 0, 0, 0, 
                      4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
					  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095
};
uint16_t g_Wave[128];
/*******************************************************************************
*	������: InitWaveMaker
*	��  ��:	��
*	��  ��:	��
*	����˵������ʼ�����η�������
*			  ����PA4 ΪDAC_OUT1, ����DMA2 ���������Ƶ���Ƶ�� ��Ƶ����0ʱ
			  TIM_SetAutoreload(TIM6, 370); =72000000/370 ����ǲ��������Ҫ����
			  DMA�Ĵ������� ����DMA_BufferSize = 32 ��=72000000/370/32
			  ��֪��Ϊʲôȥ�������������Ż�������⣿���� ��Ĺ��̲������������
*******************************************************************************/
void InitWaveMaker(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
				
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;			/* ����PA4ΪDAC_OUT1 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* ����DAC����Ĳ���Ƶ��
		  x = 72000000 / Ƶ��
		  ARR�Զ���װ�ؼĴ������ڵ�ֵ(��ʱʱ�䣩
		*/													  
		TIM_PrescalerConfig(TIM6, 0, TIM_PSCReloadMode_Update); //Ԥ��Ƶ=0 
		
		TIM_SetAutoreload(TIM6, 50);	            /* 22 ���100KHz�����Ҳ� */
		TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	 {
	   	DAC_InitTypeDef DAC_InitStructure;	
	    /* DAC channel1 Configuration */
		DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;               //��ʱ��6 TRGO����						 
		DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	   //�������ǲ�����ʹ�ܹر�
		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	   //�������ر�
//		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	 }	
	 {	
	    DMA_InitTypeDef DMA_InitStructure;	
		DMA_DeInit(DMA2_Channel3);
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_Address;
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_Wave;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						     //�洢��������
		DMA_InitStructure.DMA_BufferSize = 128;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						    //ѭ��ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High ;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA2_Channel3, &DMA_InitStructure);
		DMA_Cmd(DMA2_Channel3, ENABLE);
	}
	 /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	 automatically connected to the DAC converter. */
	 DAC_Cmd(DAC_Channel_1, ENABLE);		 //DACͨ��1ʹ��
	 /* Enable DMA for DAC Channel1 */
	 DAC_DMACmd(DAC_Channel_1, ENABLE);		 //DACͨ��1DMA����
}
/*************************************************************************************
����DAC_CR�Ĵ�����MAMPx[3:0]λ�������β��ֻ���ȫ��LFSR�����ݣ������ĵõ���LSFRֵ��
DAC_DHRx����ֵ��ӣ�ȥ�����λ֮�󼴱�д��DAC_DORx�Ĵ���
*************************************************************************************/
void InitNoise()
{  
  DAC_InitTypeDef            DAC_InitStructure;
  GPIO_InitTypeDef           GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  TIM_PrescalerConfig(TIM6, 0, TIM_PSCReloadMode_Update); //Ԥ��Ƶ=0 
  TIM_SetAutoreload(TIM6, 220);	 
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;  //����ͨ���˴����ε�λ�������÷�ֵ
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  DAC_Cmd(DAC_Channel_1, ENABLE);

  DAC_SetChannel1Data(DAC_Align_12b_R, 0);//����ͨ���������ö��뷽ʽ ����ͨ��������������ó�ʼֵ
  TIM_Cmd(TIM6, ENABLE);					
}
 /*******************************************************************************
*	������: InitTriangle
*	��  ��:	��
*	��  ��:	��
*	����˵�����������ǲ�
*			  
*******************************************************************************/
void InitTriangle()
{  
  DAC_InitTypeDef            DAC_InitStructure;
  GPIO_InitTypeDef           GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  TIM_PrescalerConfig(TIM6, 0, TIM_PSCReloadMode_Update); //Ԥ��Ƶ=0 
  TIM_SetAutoreload(TIM6, 22);	 
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  DAC_Cmd(DAC_Channel_1, ENABLE);

  DAC_SetChannel1Data(DAC_Align_12b_R, 0);//����ͨ���������ö��뷽ʽ ����ͨ��������������ó�ʼֵ
  TIM_Cmd(TIM6, ENABLE);					
}
 /*******************************************************************************
*	������: SetupWaveType
*	��  ��:	��
*	��  ��:	��
*	����˵���������ǲ������Ҳ����ǲ�������
*			  
*******************************************************************************/
void SetupWaveType(uint8_t _type)
{
	/*
		���㲨�η��ȡ�
		DAC�Ĵ������ֵ 4096 ��Ӧ3.3V

		�����������2V�ķ��ȣ�ϵ�� = 0.606
		4096 * 0.606 = 2482
	*/
	TIM_Cmd(TIM6, DISABLE);
	switch (_type)
	{
		case WT_SIN:	/* ���Ҳ� */
			{
				uint32_t i;

				for (i = 0; i < 128; i++)
				{
					/* �������Ҳ����� */
					//g_Wave[i] = g_SineWave128[i]*0.606;
					g_Wave[i] = 2048+1024*sin(2*3.1415926f*127*i/128); 
				}

			}
			break;

		case WT_RECT:	/* ���� */
			{
				uint32_t i;

				for (i = 0; i < 64; i++)
				{
					g_Wave[i] = 0;
				}

				for (i = 0; i < 64; i++)
				{
					g_Wave[i] = 2482;
				}
			}
			break;
	}
    TIM_Cmd(TIM6, ENABLE);
}
