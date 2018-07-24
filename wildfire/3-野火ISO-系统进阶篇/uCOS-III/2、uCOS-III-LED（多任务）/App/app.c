#include "app.h"

extern	OS_TCB	StartUp_TCB;					

static  OS_TCB	LED1_TCB;		   				
static	CPU_STK	LED1_Stk[TASK_LED1_STK_SIZE];	   

static  OS_TCB	LED2_TCB;		   				
static	CPU_STK	LED2_Stk[TASK_LED2_STK_SIZE];	   	

static  OS_TCB	LED3_TCB;		   					
static	CPU_STK	LED3_Stk[TASK_LED3_STK_SIZE];	   

/* 
 * ��������Task_Start
 * ����  : ��������
 *			   ���ȼ�Ϊ3��
 *		     ����LED1��LED2��LED3������
 * ����  ����
 * ���  : ��
 */
void Task_Start(void *p_arg)
{
	OS_ERR err;
  (void)p_arg;	

	//��������LED1	
	OSTaskCreate((OS_TCB     *)&LED1_TCB,					            // ������ƿ�ָ��          
               (CPU_CHAR   *)"LED1",						            // ��������
               (OS_TASK_PTR )Task_LED1,					            // �������ָ��
               (void       *)0,							                // ���ݸ�����Ĳ���parg
               (OS_PRIO     )TASK_LED1_PRIO,				        // �������ȼ�
               (CPU_STK    *)&LED1_Stk[0],				          // �����ջ����ַ
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE/10,		      // ��ջʣ�ྯ����
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE,			      // ��ջ��С
               (OS_MSG_QTY  )0,							                // �ɽ��յ������Ϣ������
               (OS_TICK     )0,							                // ʱ��Ƭ��תʱ��
               (void       *)0,							                // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	        // ����ѡ��
               (OS_ERR     *)&err);						              // ����ֵ

	//��������LED2	
	OSTaskCreate((OS_TCB     *)&LED2_TCB,					     
               (CPU_CHAR   *)"LED2",						
               (OS_TASK_PTR )Task_LED2,					
               (void       *)0,							
               (OS_PRIO     )TASK_LED2_PRIO,				
               (CPU_STK    *)&LED2_Stk[0],				
               (CPU_STK_SIZE)TASK_LED2_STK_SIZE/10,		
               (CPU_STK_SIZE)TASK_LED2_STK_SIZE,			
               (OS_MSG_QTY  )0,							
               (OS_TICK     )0,							
               (void       *)0,							
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	 
               (OS_ERR     *)&err);					

	//��������LED3	
	OSTaskCreate((OS_TCB     *)&LED3_TCB,					  
               (CPU_CHAR   *)"LED3",						
               (OS_TASK_PTR )Task_LED3,				
               (void       *)0,							
               (OS_PRIO     )TASK_LED3_PRIO,				
               (CPU_STK    *)&LED3_Stk[0],				
               (CPU_STK_SIZE)TASK_LED3_STK_SIZE/10,		
               (CPU_STK_SIZE)TASK_LED3_STK_SIZE,			
               (OS_MSG_QTY  )0,						
               (OS_TICK     )0,							
               (void       *)0,							
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	 
               (OS_ERR     *)&err);	
               
  //����ɾ���Լ�	
	OSTaskDel(&StartUp_TCB,&err);							 
}


/* 
 * ��������Task_LED1
 * ����  : LED����1��
 *		     ���ȼ�Ϊ4��
 *		     ��100ms�ļ���������ر�LED1
 * ����  ����
 * ���  : ��
 */
void Task_LED1(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;                	

  while (1)
  {
    LED1( ON );
    OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);	//��ʱ����100ms
    LED1( OFF);     
    OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

/* 
 * ��������Task_LED2
 * ����  : 	LED����2��
 *			    ���ȼ�Ϊ5��
 *		      ��200ms�ļ���������ر�LED2
 * ����  ����
 * ���  : ��
 */
void Task_LED2(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;                	

  while (1)
  {
    LED2( ON );
    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);	 //��ʱ����200ms
    LED2( OFF);
    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

/* 
 * ��������Task_LED3
 * ����  : LED����3��
 *			   ���ȼ�Ϊ6��
 *		     ��300ms�ļ���������ر�LED3
 * ����  ����
 * ���  : ��
 */
void Task_LED3(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;      

  while (1)
  {
    LED3( ON );
    OSTimeDlyHMSM(0, 0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);	 //��ʱ����300ms
    LED3( OFF);
    OSTimeDlyHMSM(0, 0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

/* -------------------------------------end of file------------------------------------ */
