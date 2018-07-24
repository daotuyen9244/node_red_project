/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��ʱ����	  (��������ԭ�ӿ�����)
*	�ļ����� : bsp_mouse.c
*	��    �� : V1.0
*	˵    �� : �������           
*	�޸ļ�¼ : ������޸ļ�¼Ҳ��ָ��ԭ�ӿ�������޸ļ�¼
*			   
*
*   �汾��     ����       ����      ˵��
*   v1.0     2012-10-26  Eric2013  ST�̼���汾 V3.5.0�汾��		
*   BLOG: http://blog.sina.com.cn/u/2565749395
*********************************************************************************************************
*/
#include "mouse.h"
#include "delay.h"


u8 MOUSE_ID;   /* ����������ID */ 
PS2_Mouse MouseX;

/*
*********************************************************************************************************
*	�� �� ��: Mouse_Data_Pro 
*	����˵��: ����MOUSE������	 
*	��    �Σ���			  
*	�� �� ֵ: �� 
*********************************************************************************************************
*/
void Mouse_Data_Pro(void)
{			    			    
    MouseX.x_pos+=(signed char)PS2_DATA_BUF[1];
    MouseX.y_pos-=(signed char)PS2_DATA_BUF[2];
    MouseX.z_pos+=(signed char)PS2_DATA_BUF[3];		  
	MouseX.bt_mask=PS2_DATA_BUF[0]&0X07;       /* ȡ������ */
}

/*
*********************************************************************************************************
*	�� �� ��: Init_Mouse 
*	����˵��: ���ڳ�ʼ�����	 
*	��    �Σ���			  
*	�� �� ֵ: ����0��ʾ�ɹ����� ����û�гɹ�
*********************************************************************************************************
*/	 
u8 Init_Mouse(void)
{
	u8 t;
			 
	PS2_Init();							     
	delay_ms(800);               /* �ȴ��ϵ縴λ��� */ 			 	 
	PS2_Status=CMDMODE;          /* ��������ģʽ	 */
	
	/* ��λ���		 */
	t=PS2_Send_Cmd(PS_RESET);     
	if(t!=0)	return 1;
	t=PS2_Get_Byte();			  
    if(t!=0XFA) return 2;
	t=0;

	/* �ȴ���λ���     */ 
	while((PS2_Status&0x80)==0)  
	{
		t++;
		delay_ms(10);      
		if(t>50)return 3;
	}

	PS2_Get_Byte();     		/* �õ�0XAA    */
	PS2_Get_Byte();				/* �õ�ID 0X00 */
					 
	/* �������ģʽ�������ʼ������ �ֱ�����200,100,80������ */
	PS2_Send_Cmd(SET_SAMPLE_RATE);		/* �������ò����� */
    if(PS2_Get_Byte()!=0XFA)return 4;	/* ����ʧ�� 	  */  
	PS2_Send_Cmd(0XC8);					/* ������200      */
    if(PS2_Get_Byte()!=0XFA)return 5;	/* ����ʧ��       */

	PS2_Send_Cmd(SET_SAMPLE_RATE);		/* �������ò����� */
    if(PS2_Get_Byte()!=0XFA)return 6;	/* ����ʧ��       */
	PS2_Send_Cmd(0X64);					/* ������100      */
    if(PS2_Get_Byte()!=0XFA)return 7;	/* ����ʧ��       */

	PS2_Send_Cmd(SET_SAMPLE_RATE);		/* �������ò����� */
    if(PS2_Get_Byte()!=0XFA)return 8;	/* ����ʧ��       */
	PS2_Send_Cmd(0X50);				    /* ������80       */
    if(PS2_Get_Byte()!=0XFA)return 9;	/* ����ʧ��       */

	/* ������� */		   
	PS2_Send_Cmd(GET_DEVICE_ID); 		/* ��ȡID        */
    if(PS2_Get_Byte()!=0XFA)return 10;	/* ����ʧ��      */
	MOUSE_ID=PS2_Get_Byte();			/* �õ�MOUSE ID  */
		 
	PS2_Send_Cmd(SET_SAMPLE_RATE);		/* �ٴν������ò�����  */
    if(PS2_Get_Byte()!=0XFA)return 11;	/* ����ʧ��  	*/
	PS2_Send_Cmd(0X0A);					/* ������10  	*/
    if(PS2_Get_Byte()!=0XFA)return 12;	/* ����ʧ��  	*/
				 
	PS2_Send_Cmd(GET_DEVICE_ID); 		/* ��ȡID   	*/
    if(PS2_Get_Byte()!=0XFA)return 13;	/* ����ʧ��  	*/
	MOUSE_ID=PS2_Get_Byte();			/* �õ�MOUSE ID	*/
		 
	PS2_Send_Cmd(SET_RESOLUTION);  		/* ���÷ֱ��� 	*/
    if(PS2_Get_Byte()!=0XFA)return 14;	/* ����ʧ��   	*/
	   
 	PS2_Send_Cmd(0X03);					/* 8��/mm     	*/
    if(PS2_Get_Byte()!=0XFA)return 15;	/* ����ʧ��		*/ 

	PS2_Send_Cmd(SET_SCALING11);   		/* �������ű���Ϊ1:1*/
    if(PS2_Get_Byte()!=0XFA)return 16;	/* ����ʧ��		*/ 
	  
 	PS2_Send_Cmd(SET_SAMPLE_RATE); 		/* ���ò�����	*/
    if(PS2_Get_Byte()!=0XFA)return 17;	/* ����ʧ��		*/   
 	PS2_Send_Cmd(0X28);					/* 40			*/
    if(PS2_Get_Byte()!=0XFA)return 18;	/* ����ʧ��		*/ 
	   
	PS2_Send_Cmd(EN_DATA_REPORT);   	/* ʹ�����ݱ��� */
    if(PS2_Get_Byte()!=0XFA)return 19;	/* ����ʧ��  	*/

	PS2_Status=MOUSE;					/* �������ģʽ */
	return 0;							/* �޴���,��ʼ���ɹ�  */   
}




