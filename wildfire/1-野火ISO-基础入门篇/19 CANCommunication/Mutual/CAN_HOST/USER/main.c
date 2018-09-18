
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

__IO uint32_t flag = 0xff;		 //Used to mark whether data is received or not, and assign it to the interrupt function.
CanTxMsg TxMessage;			     //Send buffer
CanRxMsg RxMessage;				 //Receive buffer


/*
 * Representation of CAN differential signal
 * 1: recessive level H2.5v - L2.5v = 0v
 * 0: dominant level H3.5v - L1.5v = 2v
 */


/*
 * Function name: main
 * Description: The main function of "host"
 * Input : None
 * Output : None
 */
int main(void)
{
	/*Initialize the serial port module*/
 	USART1_Config();
 			
 	/* ÅäÖÃCANÄ£¿é */
	CAN_Config();  

	printf( "\r\n***** This is a dual CAN communication experiment******** \r\n");
	printf( "\r\n This is the \"host side\" feedback£º \r\n");

	/*Set the information to be sent via CAN*/
	 CAN_SetMsg();

	 printf("\r\The content of the message to be sent is:\r\n");
	 printf("\r\n Extended ID number ExtId£º0x%x",TxMessage.ExtId);
	 printf("\r\n The content of the data segment:Data[0]=0x%x £¬Data[1]=0x%x \r\n",TxMessage.Data[0],TxMessage.Data[1]);
	 			
	 /*Send a message "ABCD"**/
	 CAN_Transmit(CAN1, &TxMessage);
	 		
	while( flag == 0xff );					//flag =0 ,success

	printf( "\r\n Successfully received data returned by \"slave\"\r\n ");	
	printf("\r\n The received message is:\r\n"); 
	printf("\r\n Extended ID number ExtId£º0x%x",RxMessage.ExtId);	 
	printf("\r\n The content of the data segment:Data[0]= 0x%x £¬Data[1]=0x%x \r\n",RxMessage.Data[0],RxMessage.Data[1]);

	while(1);
	
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


