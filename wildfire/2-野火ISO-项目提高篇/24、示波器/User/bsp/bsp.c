/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : bsp.c
*	版    本 : V1.0
*	说    明 : BSP驱动包
*				
*	修改记录 :
*		版本号    日期          作者                 说明
*		v1.0    2011-10-31   WildFire Team  ST固件库版本 V3.5.0版本。
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             include文件
*********************************************************************************************************
*/
#include <includes.h>
#include "WaveMaker.h"

extern void	InitDsoParam(void);			/* 初始化示波器参数 */
extern void	InitDSO(void);				/* 配置示波器用到的外设：ADC,TIM,DMA等 */
/*
*********************************************************************************************************
*	函 数 名: BSP_Init
*	功能说明: 初始化CPU时钟、按键和LED灯GPIO、设置systick中断。这个函数由应用程序调用。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void  BSP_Init (void)
{    

	//bsp_InitLed();		/* 初始化LED的GPIO */
	InitWaveMaker();
	SetupWaveType(WT_RECT);
	InitDsoParam();			/* 初始化示波器参数 */
	InitDSO();				/* 配置示波器用到的外设：ADC,TIM,DMA等 */
    bsp_InitLCD();
	bsp_InitTouch();
			
	SysTick_Config(BSP_CPU_ClkFreq() / OS_TICKS_PER_SEC);

}
/*
*********************************************************************************************************
*	函 数 名: BSP_CPU_ClkFreq
*	功能说明: 读取CPU时钟频率，单位Hz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint32_t BSP_CPU_ClkFreq(void)
{
	static  RCC_ClocksTypeDef  rcc_clocks;
	
	RCC_GetClocksFreq(&rcc_clocks);
	
	return ((INT32U)rcc_clocks.HCLK_Frequency);
}

/* 
	ST库函数使用了C编译器的断言功能，如果定义了USE_FULL_ASSERT，那么所有的ST库函数将检查函数形参
	是否正确。如果不正确将调用 assert_failed() 函数，这个函数是一个死循环，便于用户检查代码。
	
	关键字 __LINE__ 表示源代码行号。
	关键字__FILE__表示源代码文件名。
	
	可以在工程的配置中加上预编译指令 USE_FULL_ASSERT 使能断言功能。
	断言功能使能后将增大代码大小，一般推荐用户在调试时使能，在正式发布软件是禁止。
*/
#ifdef USE_FULL_ASSERT

/*******************************************************************************
*	函 数 名: assert_failed
*	功能说明: 断言失败时调用此函数
*	形    参：file : 源代码文件名称。关键字__FILE__表示源代码文件名。
*			  line ：代码行号。关键字 __LINE__ 表示源代码行号
*	返 回 值: 无
*/  
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* 
		用户可以添加自己的代码报告源代码文件名和代码行号，比如将错误文件和行号打印到串口
		printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	*/
	
	/* 这是一个死循环，以便于用户查错 */
	while (1)
	{
	}
}
#endif
