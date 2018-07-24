/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       FIRE_KEY.c
 * @brief      KEY��������ʵ��
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-07-10
 */


/*
 * ����ͷ�ļ�
 */
#include "FIRE_key.h"

#define DELAY_MS(x)	Key_Delay(100*x)


//���� �ϡ��¡����ҡ�ok �����ļ�˳������
const KEY_PTx_TypeDef KEY_PTxn[KEY_MAX]={{KEY_UP_PORT,KEY_UP_PIN},{KEY_DOWN_PORT,KEY_DOWN_PIN},{KEY_LEFT_PORT,KEY_LEFT_PIN},{KEY_RIGHT_PORT,KEY_RIGHT_PIN},{KEY_SELECT_PORT,KEY_SELECT_PIN}};


/* ��ʱ�����õ�����ʱ���� */
static void Key_Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}	

/*!
 *  @brief      ��ʼ��key�˿�
 *  @param      ��
 *  @since      v5.0
 *  Sample usage:       KEY_init ();    //��ʼ�� 
 */
void    key_init(void)
{
	uint8_t key = KEY_MAX;
	
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOB��GPIOF������ʱ��*/
	RCC_APB2PeriphClockCmd( KEY_RIGHT_CLK|KEY_LEFT_CLK|KEY_DOWN_CLK|KEY_UP_CLK|KEY_SELECT_CLK, ENABLE); 

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
  
	//��ʼ��ȫ�� ����
	while(key--)
	{
		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = KEY_PTxn[key].pin ;	
		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(KEY_PTxn[key].port , &GPIO_InitStructure);
			
	}

  
}

/*!
 *  @brief      ��ȡkey״̬��������ʱ������
 *  @param      KEY_e           KEY���
 *  @return     KEY_STATUS_e    KEY״̬��KEY_DOWN��KEY_DOWN��
 *  @since      v5.0
 *  Sample usage:
                    if(key_get(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n��������")
                    }
 */
KEY_STATUS_e key_get(KEY_e key)
{
    if(GPIO_ReadInputDataBit(KEY_PTxn[key].port ,KEY_PTxn[key].pin ) == Bit_RESET)
    {
        return KEY_DOWN;
    }
    return KEY_UP;
}


/*!
 *  @brief      ���key״̬������ʱ������
 *  @param      KEY_e           KEY���
 *  @return     KEY_STATUS_e    KEY״̬��KEY_DOWN��KEY_DOWN��
 *  @since      v5.0
 *  Sample usage:
                    if(key_check(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n��������")
                    }
 */
KEY_STATUS_e key_check(KEY_e key)
{
    if(GPIO_ReadInputDataBit(KEY_PTxn[key].port ,KEY_PTxn[key].pin ) == Bit_RESET)
    {
        DELAY_MS(10);
        if(GPIO_ReadInputDataBit(KEY_PTxn[key].port ,KEY_PTxn[key].pin ) == Bit_RESET)
        {
            return KEY_DOWN;
        }
    }
    return KEY_UP;
}


/*********************  ���´�����ʵ�ְ�����ʱɨ�裬������Ϣ��FIFO  ********************/
/*
 * ���尴����ϢFIFO״̬
 */
typedef enum
{
    KEY_MSG_EMPTY,      //û�а�����Ϣ
    KEY_MSG_NORMAL,     //�������а�����Ϣ��������
    KEY_MSG_FULL,       //������Ϣ��
} key_msg_e;

/*
 * ���尴����ϢFIFO��صı���
 */
KEY_MSG_t           key_msg[KEY_MSG_FIFO_SIZE];             //������ϢFIFO
volatile uint8_t    key_msg_front = 0, key_msg_rear = 0;    //����FIFO��ָ��
volatile uint8_t    key_msg_flag = KEY_MSG_EMPTY;           //������ϢFIFO״̬


/*!
 *  @brief      ���Ͱ�����Ϣ��FIFO
 *  @param      KEY_MSG_t       ������Ϣ
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t *keymsg;
                    keymsg.key      = KEY_U;
                    keymsg.status   = KEY_HOLD;
                    send_key_msg(keymsg);                   //����
 */
void send_key_msg(KEY_MSG_t keymsg)
{
    uint8_t tmp;
    //������FIFO��
    if(key_msg_flag == KEY_MSG_FULL)
    {
        //����ֱ�Ӳ�����
        return ;
    }
    key_msg[key_msg_rear].key = keymsg.key;
    key_msg[key_msg_rear].status = keymsg.status;

    key_msg_rear++;

    if(key_msg_rear >= KEY_MSG_FIFO_SIZE)
    {
        key_msg_rear = 0;                       //��ͷ��ʼ
    }

    tmp = key_msg_rear;
    if(tmp == key_msg_front)                   //׷��ƨ���ˣ�����
    {
        key_msg_flag = KEY_MSG_FULL;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }
}


/*!
 *  @brief      ��FIFO���ȡ������Ϣ
 *  @param      KEY_MSG_t       ������Ϣ
 *  @return     �Ƿ��ȡ������Ϣ��1Ϊ��ȡ�ɹ���0Ϊû��ȡ��������Ϣ��
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t keymsg;
                    if(get_key_msg(&keymsg) == 1)
                    {
                        printf("\n���°���KEY%d,����Ϊ%d��0Ϊ���£�1Ϊ����2Ϊ������",keymsg.key,keymsg.status);
                    }
 */
uint8_t get_key_msg(KEY_MSG_t *keymsg)
{
    uint8_t tmp;

    if(key_msg_flag == KEY_MSG_EMPTY)               //������ϢFIFOΪ�գ�ֱ�ӷ���0
    {
        return 0;
    }

    keymsg->key = key_msg[key_msg_front].key;       //��FIFO�����л�ȡ����ֵ
    keymsg->status = key_msg[key_msg_front].status; //��FIFO�����л�ȡ��������

    key_msg_front++;                                //FIFO����ָ���1��ָ����һ����Ϣ

    if(key_msg_front >= KEY_MSG_FIFO_SIZE)          //FIFOָ�����������0��ʼ����
    {
        key_msg_front = 0;                          //��ͷ��ʼ������ѭ���������飩
    }

    tmp = key_msg_rear;
    if(key_msg_front == tmp)                        //�Ƚ϶��׺Ͷ�β�Ƿ�һ����һ�����ʾFIFO�ѿ���
    {
        key_msg_flag = KEY_MSG_EMPTY;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }

    return 1;
}

/*!
 *  @brief      ��ʱ���key״̬
 *  @since      v5.0
 *  @note       �˺�����Ҫ���� ��ʱ�жϸ�λ�������ʱ10msִ��һ��
 */
void key_IRQHandler(void)
{

    KEY_e   keynum;
    static uint8_t keytime[KEY_MAX];                          //��̬���飬��������鰴��ʱ��

    KEY_MSG_t keymsg;                                       //������Ϣ

    for(keynum = (KEY_e)0 ; keynum < KEY_MAX; keynum ++)    //ÿ��������ѯ
    {
        if(key_get(keynum) == KEY_DOWN)                     //�жϰ����Ƿ���
        {
            keytime[keynum]++;                              //����ʱ���ۼ�

            if(keytime[keynum] <= KEY_DOWN_TIME)            //�ж�ʱ���Ƿ�û��������ȷ�ϰ���ʱ��
            {
                continue;                                   //û�ﵽ��������ȴ�
            }
            else if(keytime[keynum] == KEY_DOWN_TIME + 1 )  //�ж�ʱ���Ƿ�Ϊ����ȷ�ϰ���ʱ��
            {
                //ȷ�ϰ�������
                keymsg.key = keynum;
                keymsg.status = KEY_DOWN;
                send_key_msg(keymsg);                       //�Ѱ���ֵ�Ͱ������ͷ�����Ϣ��FIFO
            }
            else if(keytime[keynum] <= KEY_HOLD_TIME)       //�Ƿ�û��������HOLD����ȷ��ʱ��
            {
                continue;                                   //û������������ȴ�
            }
            else if(keytime[keynum]  == KEY_HOLD_TIME + 1)  //�Ƿ�Ϊ����holdȷ��ʱ��
            {
                //ȷ�ϳ���HOLD
                keymsg.key = keynum;
                keymsg.status = KEY_HOLD;
                send_key_msg(keymsg);                       //����
                keytime[keynum] = KEY_DOWN_TIME + 1;
            }
            else
            {
                keytime[keynum] = KEY_DOWN_TIME + 1;        //�����ظ���� hold ״̬
            }
        }
        else
        {
            if(keytime[keynum] > KEY_DOWN_TIME)             //���ȷ�Ϲ����°���
            {
                keymsg.key = keynum;
                keymsg.status = KEY_UP;
                send_key_msg(keymsg);                       //���Ͱ���������Ϣ
            }

            keytime[keynum] = 0;                            //ʱ���ۼ���0
        }
    }
}



