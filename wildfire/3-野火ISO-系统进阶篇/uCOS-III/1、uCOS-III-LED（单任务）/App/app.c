#include "app.h"

/* 
 * ��������Task_LED
 * ����  : LED��ˮ��	
 * ����  ����
 * ���  : ��
 */
void Task_LED(void *p_arg)
{
	OS_ERR err;
  (void)p_arg;                		// 'p_arg' ��û���õ�����ֹ��������ʾ����

  while (1)
  {
      LED1( ON );
      OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
      LED1( OFF);
      
      LED2( ON );
      OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
      LED2( OFF);
      
      LED3( ON );
      OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
      LED3( OFF);
  }
}
