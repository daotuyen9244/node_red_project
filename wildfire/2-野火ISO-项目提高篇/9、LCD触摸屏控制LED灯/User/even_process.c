#include "even_process.h"
#include "bsp_led.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_touch.h"
#include "bsp_usart1.h"
#include "bsp_bmp.h"
#include <stdio.h>

EVEN even = E_GETCHAR;								//�¼���־
BUTTON_STATE red_button 		= S_OFF;	//����״̬ ��
BUTTON_STATE green_button 	= S_OFF;	//����״̬ ��
BUTTON_STATE yellow_button 	= S_OFF;	//����״̬ ��

extern uint8_t recv_char;							//���ڽ��յ��ַ���


/**
  * @brief  �¼���������ѭ������
  * @param  ��
  * @retval ��
 */
void even_process(void)
{
	char lcd_char[20];
	switch(even)
	{
		case E_BUTTON1:									//LED1							
		
				if(red_button == S_OFF)		//ԭ��Ϊ��״̬
				{
					Lcd_show_bmp(38, 184,"/led/ui_r_button.bmp");			//��ʾ��״̬��ť
					red_button = S_ON;																//����״̬��־
				}
				else											//ԭ��Ϊ��״̬
				{
					Lcd_show_bmp(38, 184,"/led/ui_rb_button.bmp");		//��ʾ��״̬��ť
					red_button = S_OFF;																//����״̬��־
				}        
				
				LED1_TOGGLE;							//LED״̬��ת
					
				even = E_NULL;
				
	  break;
		
		case E_BUTTON2:										//LED2
			
				if(green_button == S_OFF)		//ԭ��Ϊ��״̬
				{
					Lcd_show_bmp(38, 95,"/led/ui_g_button.bmp");			//��ʾ��״̬��ť
					green_button = S_ON;															//����״̬��־
				}
				else											//ԭ��Ϊ��״̬
				{
					Lcd_show_bmp(38, 95,"/led/ui_gb_button.bmp");			//��ʾ��״̬��ť
					green_button = S_OFF;															//����״̬��־
					}
		
				LED2_TOGGLE;
				
				even = E_NULL;
		
		break;
		
		case E_BUTTON3:											//LED3
			
				if(yellow_button == S_OFF)		//ԭ��Ϊ��״̬
				{
					Lcd_show_bmp(38, 11,"/led/ui_y_button.bmp");			//��ʾ��״̬��ť
					yellow_button = S_ON;															//����״̬��־
				}
				else											//ԭ��Ϊ��״̬
				{
					Lcd_show_bmp(38, 11,"/led/ui_yb_button.bmp");			//��ʾ��״̬��ť
					yellow_button = S_OFF;														//����״̬��־
					}	
		
				LED3_TOGGLE;
		
				even = E_NULL;
		
		break;
		
		case E_GETCHAR:								//���ڽ��յ�����
			if(recv_char == 0)					//���ϵ�ʱ
			{
				Lcd_GramScan( 1 );
				LCD_DispEnCh(20,180,"û�н��յ���������",BLUE);
			}
			else	
			{			
        Lcd_GramScan( 1 );
        LCD_Clear(20,180,180,16,BACKGROUND)	;					
        sprintf(lcd_char,"��������:%c",recv_char);		//ƴ���ַ���
        LCD_DispEnCh(20,180,lcd_char,BLUE);						//��ʾ�յ����ַ�
			}
			
			even = E_NULL;
		
		break;
		
		default:
			break;
	}
}


/**
  * @brief  touch_process�жϷ����������ݴ���λ�ø��´�����־
  * @param  ��  
  * @retval ��
  */
void touch_process(void)
{
	 if(touch_flag == 1)															/*������ʰ�����*/
    {		
      /* ��ȡ������� */
      if(Get_touch_point(&display, Read_2046_2(), &touch_para ) !=DISABLE)      
      {		
				printf("\r\n x=%d,y=%d",display.x,display.y);
				
				if(display.x>=34 && display.x<=76)						//�������ķ�Χ
				{
						if(display.y>=15 && display.y<=46)				// LED1
						{
								even = E_BUTTON1;
								printf("\r\n LED1");
					  }
						else if(display.y>=100 && display.y<=135)	// LED2
						{
								even = E_BUTTON2;
								printf("\r\n LED2");
						}
						else if(display.y>=182 && display.y<=216)	// LED3
						{
								even = E_BUTTON3;
								printf("\r\n LED3");
						}	
				}
			}				
		}
}
/* ----------------------------------------end of file --------------------------------------------*/

