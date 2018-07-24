/*
*********************************************************************************************************
*	                                  
*	ģ������ : ������������Ӧ��������
*	�ļ����� : app.c
*	��    �� : V1.0
*	˵    �� : uCOS-II����
*	�޸ļ�¼ :
*		�汾��    ����          ����                 ˵��
*		v1.0    2012-10-31   WildFire Team  ST�̼���汾 V3.5.0�汾��
*
*	Copyright (C), 2012-2013, WildFire Team
*		
*********************************************************************************************************
*/

#include <includes.h>	/* ���ļ����������б����.h�ļ� */
#include "GUI.h"

/*
**************************************************************************
*                  ģ����ȫ�ֱ���
**************************************************************************
*/

/* ����ÿ������Ķ�ջ�ռ䣬app_cfg.h�ļ��к궨��ջ��С */
static OS_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
static OS_STK AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE];
static OS_STK AppTaskGUIStk[APP_TASK_GUI_STK_SIZE];	
static OS_STK AppTaskCOMStk[APP_TASK_COM_STK_SIZE];	
static OS_STK AppTaskUpdateStk[APP_TASK_UPDATE_STK_SIZE];	
			
/*
****************************************************************************
*                                      ��������
****************************************************************************
*/

static void AppTaskCreate(void);
static void AppTaskStart(void *p_arg);
static void AppTaskUserIF(void *p_arg);
static void AppTaskGUI(void *p_arg);
static void AppTaskCom(void *p_arg);
extern void MainTask(void);

/*
***************************************************************************
*                       ��������
****************************************************************************
*/

/* ����һ�����䣬    ��ֻ��һ������ָ�룬  OSMboxCreate�����ᴴ������������Դ */
OS_EVENT *AppUserIFMbox;
OS_EVENT *AppMsgMbox;

/* ����һ���ź�����  ��ֻ��һ���ź���ָ�룬OSSEMCreate�����ᴴ������������Դ */
 
OS_EVENT *Semp;
OS_EVENT *AppTouchSemp;

/*����һ���ڴ��*/
/*����һ���ڴ���� 2���ڴ�� ÿ���ڴ��2050���ֽ�*/
OS_MEM   *CommMem; 
/*
****************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    �Σ���
*	�� �� ֵ: ��
****************************************************************************
*/
int main(void)
{
	INT8U  err;

	/* ��ʼ��"uC/OS-II"�ں� */
	OSInit();
  
	/* ����һ����������Ҳ���������񣩡���������ᴴ�����е�Ӧ�ó������� */
	OSTaskCreateExt(AppTaskStart,	/* ����������ָ�� */
                    (void *)0,		/* ���ݸ�����Ĳ��� */
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1], /* ָ������ջջ����ָ�� */
                    APP_TASK_START_PRIO,	/* ��������ȼ�������Ψһ������Խ�����ȼ�Խ�� */
                    APP_TASK_START_PRIO,	/* ����ID��һ����������ȼ���ͬ */
                    (OS_STK *)&AppTaskStartStk[0],/* ָ������ջջ�׵�ָ�롣OS_STK_GROWTH ������ջ�������� */
                    APP_TASK_START_STK_SIZE, /* ����ջ��С */
                    (void *)0,	/* һ���û��ڴ�����ָ�룬����������ƿ�TCB����չ����
                       ���������л�ʱ����CPU����Ĵ��������ݣ���һ�㲻�ã���0���� */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* ����ѡ���� */
					/*  �������£�
						OS_TASK_OPT_STK_CHK      ʹ�ܼ������ջ��ͳ������ջ���õĺ�δ�õ�
						OS_TASK_OPT_STK_CLR      �ڴ�������ʱ����������ջ
						OS_TASK_OPT_SAVE_FP      ���CPU�и���Ĵ��������������л�ʱ���渡��Ĵ���������
					*/                  

	/* ָ����������ƣ����ڵ��ԡ���������ǿ�ѡ�� */
	OSTaskNameSet(APP_TASK_START_PRIO, APP_TASK_START_NAME, &err);
	/*ucosII�Ľ��ļ�������0    ���ļ�������0-4294967295*/ 
	OSTimeSet(0);			 
	/* ����������ϵͳ������Ȩ����uC/OS-II */
	OSStart();
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(��BSP_Init��ʵ��)
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����2
*********************************************************************************************************
*/
static void AppTaskStart(void *p_arg)
{		
   (void)p_arg;   /* �����ڱ���������澯����������������κ�Ŀ����� */
	 
	/* BSP ��ʼ���� BSP = Board Support Package �弶֧�ְ����������Ϊ�ײ�������*/
	BSP_Init();
	/* ���CPU������ͳ��ģ���ʼ�����ú�����������CPUռ����
	ע�⣺�˺������ӳ�100ms�ٷ��� */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
	#endif			   
	/* ����Ӧ�ó�������� */
	AppTaskCreate();
	

	/* �������壬������һ����ѭ�� */
	while (1)     
	{   
		OSTimeDlyHMSM(0, 0, 1,0);										
	}      
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUIUpdate
*	����˵��: ��������Ҫ����		
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����2
*********************************************************************************************************
*/
static void AppTaskGUIUpdate(void *p_arg)
{
	
	(void)p_arg;
		  
	while(1)
	{
		bsp_LedToggle(0);				
		OSTimeDlyHMSM(0, 0, 0, 500); 						  	 	       											  
	}   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUI
*	����˵��: 
*			  			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
    �� �� ����OS_LOWEST_PRIO - 3
*********************************************************************************************************
*/

static void AppTaskGUI(void *p_arg)
{
    (void)p_arg;		/* ����������澯 */
		
	while (1) 
	{
		MainTask();				 
	}
}
/*
*********************************************************************************************************
*	�� �� ��: AppTaskUserIF
*	����˵��: �˺�����Ҫ�õ�������JoyStick�ļ�ֵ�Ա����GUI�����λ�á�
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����2
*********************************************************************************************************
*/
static void AppTaskUserIF(void *p_arg)
{
   (void)p_arg;	             /* ������������� */
  
	while (1) 
	{       		
		OSTimeDlyHMSM(0, 0, 1, 5);   
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCom
*	����˵��: 		
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����16
*********************************************************************************************************
*/
uint8_t flag=0;
static void AppTaskCom(void *p_arg)
{
//	 INT8U err;	
	(void)p_arg;
		  

	while(1){
	
	/* �ⲿ�жϵķ�ʽ����ʹ��������� ��ʱ������ */
	#if 0
	   if(flag == 1)
	   {   
		   OSSemPend(AppTouchSemp, 0, &err);
		   while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == Bit_RESET)
		   {
		   	 bsp_LedToggle(3);
	         GUI_TOUCH_Exec();
			 OSTimeDlyHMSM(0, 0, 0, 5); 
			 	 	
		   }
		   flag = 0;
		   EXTI->IMR|= (1<<6);
		   GUI_TOUCH_Exec();
		   OSTimeDlyHMSM(0, 0, 0, 20);  		   
		}
	#endif

	   GUI_TOUCH_Exec();	
	   OSTimeDlyHMSM(0, 0, 0, 20); 						  	 	       											  
   } 
   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	INT8U      err;


	/* ����һ������(MBOX), ����Kbd�����UserIF����֮���ͨ�� */
	/* ����һ������(MBOX), RTC�жϺ�GUI֮��ͨѶ  */
	AppUserIFMbox = OSMboxCreate((void *)0);
	AppMsgMbox = OSMboxCreate((void *)0); 
	Semp      =	OSSemCreate(1);
	AppTouchSemp = OSSemCreate(0);

	
	OSTaskCreateExt(AppTaskUserIF,
                    (void *)0,
                    (OS_STK *)&AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE - 1],
                    APP_TASK_USER_IF_PRIO,
                    APP_TASK_USER_IF_PRIO,
                    (OS_STK *)&AppTaskUserIFStk[0],
                    APP_TASK_USER_IF_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_USER_IF_PRIO, APP_TASK_USER_IF_NAME, &err);

	OSTaskCreateExt(AppTaskGUI,
                    (void *)0,
                    (OS_STK *)&AppTaskGUIStk[APP_TASK_GUI_STK_SIZE - 1],
                    APP_TASK_GUI_PRIO,
                    APP_TASK_GUI_PRIO,
                    (OS_STK *)&AppTaskGUIStk[0],
                    APP_TASK_GUI_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_GUI_PRIO, APP_TASK_GUI_NAME, &err);

	OSTaskCreateExt(AppTaskCom,
                    (void *)0,
                    (OS_STK *)&AppTaskCOMStk[APP_TASK_COM_STK_SIZE-1],
                    APP_TASK_COM_PRIO,
                    APP_TASK_COM_PRIO,
                    (OS_STK *)&AppTaskCOMStk[0],
                    APP_TASK_COM_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_COM_PRIO, APP_TASK_COM_NAME, &err);


	OSTaskCreateExt(AppTaskGUIUpdate,
                    (void *)0,
                    (OS_STK *)&AppTaskUpdateStk[APP_TASK_UPDATE_STK_SIZE-1],
                    APP_TASK_UPDATE_PRIO,
                    APP_TASK_UPDATE_PRIO,
                    (OS_STK *)&AppTaskUpdateStk[0],
                    APP_TASK_UPDATE_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskNameSet(APP_TASK_UPDATE_PRIO, APP_TASK_UPDATE_NAME, &err);
}

/*
*********************************************************************************************************
*                                          App_TaskCreateHook()
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    //OSProbe_TaskCreateHook(ptcb);
#else
    (void)ptcb;
#endif
}

/*
*********************************************************************************************************
*                                           App_TaskDelHook()
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                             App_TaskIdleHook()
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif


/*
*********************************************************************************************************
*                                            TASK RETURN HOOK (APPLICATION)
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************
*/

#if OS_VERSION >= 289
void  App_TaskReturnHook (OS_TCB  *ptcb)
{
    (void)ptcb;
}
#endif


/*
*********************************************************************************************************
*                                   App_TaskStatHook()
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Returns    : none
*********************************************************************************************************
*/

void  App_TaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           App_TaskSwHook()
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    //OSProbe_TaskSwHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                           App_TCBInitHook()
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*
*********************************************************************************************************
*                                               App_TimeTickHook()
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{
#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN > 0)
    // OSProbe_TickHook();
#endif
}
#endif

#endif                                                                  /* End of OS_APP_HOOKS_EN                                   */
