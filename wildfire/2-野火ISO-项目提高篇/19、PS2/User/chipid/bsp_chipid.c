/**
  ******************************************************************************
  * @file    bsp_chipid.c
  * @author  
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   Get_ChipID bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_chipid.h"

uint32_t ChipUniqueID[3];

/**
  * @brief  Get_ChipID
  * @param  ��
  * @retval ��
  */
void Get_ChipID(void)
{
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // ���ֽ�
}
/*********************************************END OF FILE**********************/
