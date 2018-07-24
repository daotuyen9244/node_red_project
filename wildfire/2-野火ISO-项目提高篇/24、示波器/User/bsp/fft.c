#include "stm32f10x.h"
#include "FFTInc.h"
#include "dso.h"
#include "stm32_dsp.h"
#include "math.h"
#include "GUI.h"
#include "DIALOG.h"
				

/******************************************************************************************
* ÿ����Ԫ���ݸ���(��16λ)�д洢�������ݵ�ʵ��������(��16λ)�洢�������ݵ��鲿(����Ϊ0)
* ����FFT�任�����������г����ֵ
*******************************************************************************************/
void powerMag(void)					             //��ͬ�Ĳ�������Ҫ�޸� �����ٶȿ��
{
  int16_t lX,lY;
  uint16_t i;
  for (i=0; i < 512; i++)
  {
    lX= (g_DSO.FFTOUT[i]<<16)>>16;               /* sine_cosine --> cos */
    lY= (g_DSO.FFTOUT[i]>> 16);                  /* sine_cosine --> sin */    
    {
//    float X=  1024*((float)lX)/32768;
//    float Y = 1024*((float)lY)/32768;
//    float Mag = sqrt(X*X+ Y*Y)/1024;		 
	  float X= (float)lX/32;
      float Y = (float)lY/32;
	  float Mag = sqrt(X*X+ Y*Y)/1024;
      g_DSO.FFTMAG[i]= (uint32_t)(Mag*65536);		
    }    
  }
}

/*******************************************************************************
*	������: Audio_Spectrum(void)
*	��  ��:	��
*	��  ��:	��
*	��  �ܣ���ʾͨ��1����
*******************************************************************************/
static void Audio_Spectrum(void)
{
	int16_t i;									 /* �з����� */
	uint16_t *px;
	uint16_t *py;
	int16_t iTemp;
	static uint8_t s_DispFirst = 0;				/* ���ڵ�һ�ε���ʱˢ�� */
	if (s_DispFirst == 0)
	{
		s_DispFirst = 1;
		for(i=0;i<128;i++)
		{
		 g_DSO.xCh2Buf1[i]=220;
		 g_DSO.xCh2Buf1[i]=220;
		 g_DSO.xCh2Buf2[i]=220;
		 g_DSO.xCh2Buf2[i]=220;
	    }	  					  
	}	
	if (g_DSO.BufUsed1 == 0)					  //�����������������ڲ�����һ֡����
	{
		g_DSO.BufUsed1= 1;
	}
	else
	{
		g_DSO.BufUsed1 = 0;
	}

	if (g_DSO.BufUsed1 == 0)
	{
		px = g_DSO.xCh2Buf1;
		py = g_DSO.yCh2Buf1;
	}
	else
	{
		px = g_DSO.xCh2Buf2;
		py = g_DSO.yCh2Buf2;
	}
	for (i = 0; i < 61; i++)				
	{
		px[i] = 15 + i*4;	
		iTemp = g_DSO.Ch2VOffset -g_DSO.FFTMAG[i]/30;
	    if (iTemp < 20)
		{
			iTemp = 20;
		}
		py[i] = iTemp;
	}

//    GUI_GotoXY(323-55,72);
//	GUI_DispFloatFix(g_DSO.FFTMAG[0]/20000,5,2);			  //��ʾֱ�������Ĵ�С     	
	if (g_DSO.BufUsed1 == 0)
	{	
	    for(i=0;i<61;i++)
		{
		   GUI_SetColor(GUI_BLUE);
		   GUI_FillRect(px[i],g_DSO.yCh2Buf2[i],px[i]+2,220);
		   GUI_SetColor(GUI_GREEN);
		   GUI_FillRect(px[i],py[i],px[i]+2,220);
		}
	}
	else
	{
	  for(i=0;i<61;i++)
		{
		   GUI_SetColor(GUI_BLUE);
		   GUI_FillRect(px[i],g_DSO.yCh2Buf1[i],px[i]+2,220);
		   GUI_SetColor(GUI_GREEN);
		   GUI_FillRect(px[i],py[i],px[i]+2,220);

		}
	}
}
/*******************************************************************************
*	������: DispWave_Spectrum
*	��  ��:	��
*	��  ��:	��
*	��  �ܣ���ʾͨ��1����
*******************************************************************************/
static void DispWave_Spectrum(void)
{
	int16_t i;									 /* �з����� */
	uint16_t *px;
	uint16_t *py;
	int16_t iTemp;
	static uint8_t s_DispFirst = 0;				/* ���ڵ�һ�ε���ʱˢ�� */
	if (s_DispFirst == 0)
	{
		s_DispFirst = 1;
		for(i=0;i<128;i++)
		{
		 g_DSO.xCh2Buf1[i]=220;
		 g_DSO.xCh2Buf1[i]=220;
		 g_DSO.xCh2Buf2[i]=220;
		 g_DSO.xCh2Buf2[i]=220;
	    }	  					  
	}	
	if (g_DSO.BufUsed1 == 0)					  //�����������������ڲ�����һ֡����
	{
		g_DSO.BufUsed1= 1;
	}
	else
	{
		g_DSO.BufUsed1 = 0;
	}

	if (g_DSO.BufUsed1 == 0)
	{
		px = g_DSO.xCh2Buf1;
		py = g_DSO.yCh2Buf1;
	}
	else
	{
		px = g_DSO.xCh2Buf2;
		py = g_DSO.yCh2Buf2;
	}
	
	for (i = 0; i < 100; i++)				
	{
		px[i] = 20 + i;
		
		iTemp = g_DSO.Ch2VOffset -g_DSO.FFTMAG[i]/20;
		
		if (iTemp > 200)
		{
			iTemp = 200;
		}
		else if (iTemp <0)
		{
			iTemp =0;
		}
		py[i] = iTemp;
	}
	  GUI_SetColor(GUI_MAGENTA);	
	
//      GUI_GotoXY(323-55,72);
//	  GUI_DispFloatFix(g_DSO.FFTMAG[0]/20000,5,2);			  //��ʾֱ�������Ĵ�С                   
    	if (g_DSO.BufUsed1 == 0)
	{
		GUI_SetColor(GUI_BLUE);
	    GUI_DrawGraph((I16*)g_DSO.yCh2Buf2, 100, 20,20);
	}
	else
	{
		GUI_SetColor(GUI_BLUE);
	    GUI_DrawGraph((I16*)g_DSO.yCh2Buf1, 100, 20,20);			   
	}	
	    GUI_SetColor(GUI_YELLOW);
	    GUI_DrawGraph((I16*)py, 100, 20,20);
}

/*****************************************************************************************
*	������: FFT_Convert
*	��  ��:	��
*	��  ��:	��
*	��  �ܣ�ʵ��FFTת�������ʾ	   �ǵ��޸Ĳ�����!!!!!! ��Ҫ�ǵ��޸�M ���εļ���
******************************************************************************************/
extern uint8_t SpectrumFlag;
void FFT_Convert(void)
{	
	static uint8_t flag1=1;			  //��־λ����һ�γ�ʼ�� ��Ҫ�����Һ����Һ���
   	uint16_t i,Cycle;
	uint8_t flag;
	
	      for(i=0;i<1024;i++)
		{
		  g_DSO.FFTIN[i]=((int32_t)(g_DSO.Ch1Buf[i]-1900)* 14060) / 1050; //������10000�� ��ֵ��Ҫ����65336
		  g_DSO.FFTIN[i]=g_DSO.FFTIN[i]<<16;
		}
		
		cr4_fft_1024_stm32(g_DSO.FFTOUT,g_DSO.FFTIN,1024);
		
		powerMag();
		
		Cycle=g_DSO.FFTMAG[1];
		flag=1;
		
		for(i=2;i<512;i++)
		{
		  if(Cycle<g_DSO.FFTMAG[i]){Cycle=g_DSO.FFTMAG[i];flag=i;}
		}
		GUI_SetColor(GUI_MAGENTA);
        GUI_DispDecAt(flag*g_DSO.SampleFreq*2/1024 ,323-55,136,6);	   //����Ƶ��
		
	    if(SpectrumFlag == 0)
	   {
	     Audio_Spectrum();
	   }
	   else
	   {
         DispWave_Spectrum();
	   }
}

