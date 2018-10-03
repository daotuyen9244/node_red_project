
/**
  ************************************************** ****************************
  * @file bsp_chipid.c
  * @author
  * @version V1.0
  * @date 2013-xx-xx
  * @brief Get_ChipID bsp
  ************************************************** ****************************
  * @attention
  *
  * Experimental platform: wildfire iSO STM32 development board
  * Forum: http://www.chuxue123.com
  * Taobao: http://firestm32.taobao.com
  *
  ************************************************** ****************************
  */
  
#include "bsp_chipid.h"

uint32_t ChipUniqueID[3];


/**
  * @brief Get_ChipID
  * @param no
  * @retval none
  */
void Get_ChipID(void)
{
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // High byte
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // Low byte
}
/*********************************************END OF FILE**********************/
