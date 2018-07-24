
#include "stm32f10x.h"
#include "bsp_usart2_485.h"
#include "bsp_usart1.h"

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 ������Ϊ������ʵ��ʱ�ȸ��ӻ��ϵ�
 */
int main(void)
{
  uint8_t temp = 0;     

  /* USART2 config */
  USART2_Config();  //����2�����շ�485������
  USART1_Config();  //����1�������������Ϣ
  printf("\r\n485ʵ�������\\r\n");

  while(1)
  {
    GPIO_SetBits(GPIOB,GPIO_Pin_5); //���뷢��ģʽ
    USART_SendData(USART2, temp);	 //��������
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);//��ѯֱ�������������	
    printf("\r\n�������ݳɹ���\r\n"); //ʹ�ô���1��ӡ������Ϣ���ն�

    GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//�������ģʽ
    while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET); //��ѯֱ��485���յ�����

    temp = USART_ReceiveData(USART2);
    printf("���յ�������Ϊ%d \r\n",temp);				
  }
}


