
/******************** (C) COPYRIGHT 2012 WildFire Team ********************** ****
 * File name : main.c
 * Description: Dual CAN communication experiment, this is the host part, send ABCD to the slave
 * Experimental platform: Wildfire STM32 development board
 * Library version : ST3.5.0
 *
 * Author :wildfire team
 * Forum: http://www.amobbs.com/forum-1008-1.html
 * Taobao: http://firestm32.taobao.com
************************************************** ********************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "can.h"

__IO uint32_t flag = 0xff;		// used to mark whether the data is received, assign value in the interrupt function
CanTxMsg TxMessage;			    //Send buffer
CanRxMsg RxMessage; 			//Receive buffer


/*
 * Function name: main
 * Description: The main function of "slave"
 * Input : None
 * Output : None
 */
int main(void)
{

	/* USART1 config */
	USART1_Config();
	
	/* ÅäÖÃCANÄ£¿é */
	CAN_Config();

	printf( "\r\n***** This is a dual CAN communication experiment******** \r\n");
	printf( "\r\n This is the feedback from the \"machine side\": \r\n"); 
	
	/*Waiting for data from the host side*/	
	while( flag == 0xff );
	
	printf( "\r\n Successfully received data returned by \"host\"\r\n ");	
	printf("\r\n The received message is:\r\n"); 
	printf("\r\n Extended ID number ExtId: 0x%x",RxMessage.ExtId);	 
	printf("\r\n The content of the data segment:Data[0]= 0x%x £¬Data[1]=0x%x \r\n",RxMessage.Data[0],RxMessage.Data[1]);					

	/*Set the information to be sent via CAN*/ 
	CAN_SetMsg();			

	 printf("\r\nThe content of the message to be sent is:£º\r\n");
	 printf("\r\n Extended ID number ExtId£º0x%x",TxMessage.ExtId);
	 printf("\r\n The content of the data segment:Data[0]=0x%x £¬Data[1]=0x%x \r\n",TxMessage.Data[0],TxMessage.Data[1]);

	/*Send a message ¡°CDAB¡±**/
	CAN_Transmit(CAN1, &TxMessage);		

  while(1);

}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


