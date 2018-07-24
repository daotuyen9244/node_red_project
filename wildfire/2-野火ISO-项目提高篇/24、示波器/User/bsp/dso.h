/****************************************************************************
* Copyright (C), 2009-2010, www.armfly.com
*
* �ļ���: dso.h
* ���ݼ���: ͷ�ļ�
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2010-02-09 armfly  �������ļ�
*
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DSO_H
#define __DSO_H

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC2_DR_Address    ((uint32_t)0x4001284C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)

#define	SAMPLE_COUNT	(1024) 	/* ������ȣ�������������� */
/*
	ʾ������ص����ݽṹ
*/
typedef struct
{
	uint16_t Ch1Buf[SAMPLE_COUNT*2];	 /* ͨ��1���ݻ����� */
	uint32_t buffer[SAMPLE_COUNT];
    int       FFTIN [1024];
	int	      FFTOUT[1024];
	long      FFTMAG[512];
	long      FFTPHASE[512];
	uint32_t TimeBaseIdHold;			/* ��ͣʱ��ʱ�� */

	uint32_t TimeBaseId;			/* ʱ������, ���ɵõ� us��λ��ʱ�� */
	uint32_t SampleFreq;			/* ����Ƶ�ʣ���λHz */
	uint32_t TimeBase;				/* ʱ�� ���ɵõ� us��λ��ʱ�� */

	uint8_t  Ch1AttId;				/* CH1 ˥���������� */
	uint8_t  Ch2AttId;				/* CH2 ˥���������� */
	int32_t  Ch1Attenuation; 		/* ����1����˥��ϵ��(ԭʼ����x10���ٳ��������)  */
	int32_t  Ch2Attenuation; 		/* ����2����˥��ϵ��(ԭʼ����x10���ٳ��������)  */
	uint16_t Ch1VScale;				/* ͨ��1��ֱ�ֶ�ֵmV��λ */
	uint16_t Ch2VScale;				/* ͨ��2��ֱ�ֶ�ֵmV��λ */

	uint32_t TriggerLevel;			/* ������ƽ(ADC��������Ƚ�ֵ) */
	uint32_t TriggerUpEdge;			/* 1��ʾ����ģʽ�����أ�0��ʾ������ */

	uint8_t Ch1DC;					/* 0�ǽ��� 1��ʾCH1��ֱ�� */
	uint8_t Ch2DC;					/* 0�ǽ��� 1��ʾCH2��ֱ�� */   

	int16_t Ch1VOffset;				/* ͨ��1 GND��λ��, ����Ϊ���� */
	int16_t Ch2VOffset;				/* ͨ��1 GND��λ��, ����Ϊ���� */

	uint8_t ActiveCH;				/* ��ǰ����ͨ�� 1��ʾCH1, 2��ʾCH2 */
	uint8_t AdjustMode;				/* ��ǰ����ģʽ��0��ʾ���ڷ��ȣ�1��ʾ����ƫ�� */

	/* ʹ��2����������ɲ��εĲ��������� */
	uint16_t xCh1Buf1[310];		/* �������ݣ��������� */
	uint16_t yCh1Buf1[310];		/* �������ݣ��������� */
	uint16_t xCh1Buf2[310];		/* �������ݣ��������� */
	uint16_t yCh1Buf2[310];		/* �������ݣ��������� */

	uint16_t xCh2Buf1[256];		/* �������ݣ��������� */
	uint16_t yCh2Buf1[256];		/* �������ݣ��������� */
	uint16_t xCh2Buf2[256];		/* �������ݣ��������� */
	uint16_t yCh2Buf2[256];		/* �������ݣ��������� */
	uint8_t  BufUsed;			/* 0��ʾ��ǰ��Buf1�� 1��ʾ��Buf2 */
	uint8_t  BufUsed1;
	int16_t Ch1Changx;

	uint8_t HoldEn;				/* ���βɼ���ͣ��־ 1��ʾ��ͣ��0��ʾ���� */
}DSO_T;

/* 
	����֧�ֵĲ���Ƶ��
*/
typedef enum
{
	SR_1K = 0,				/*  */

}SAMPLE_RATE_E;

void InitDSO(void);
void SetSampRate(uint32_t freq);
void Delay(uint32_t nCount);
void InitDsoParam(void);
void DispButton(void);
void DispDSO(void);
void StopADC(void);
void StartADC(void);
void AdjustAtt(uint8_t ch, uint8_t mode);
void DecSampleFreq(void);
void IncSampleFreq(void);
extern DSO_T g_DSO;

extern DSO_T g_DSO;
#endif


