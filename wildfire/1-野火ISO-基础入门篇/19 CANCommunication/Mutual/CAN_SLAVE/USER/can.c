#include "can.h" 

extern   CanTxMsg TxMessage;


/*
 * Function name: CAN_GPIO_Config
 * Description: CAN GPIO configuration, PB8 pull-up input, PB9 push-pull output
 * Input : None
 * Output : None
 * Call: internal call
 */
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
   	
  	/*Peripheral clock settings*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  	/*IO settings*/
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	/* Configure CAN pin: RX */									               // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // Pull up input
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    /* Configure CAN pin: TX */									               // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // Multiplexed push-pull output
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

/*
 * Function name: CAN_NVIC_Config
 * Description: CAN NVIC configuration, 1st priority group, 0, 0 priority
 * Input : None
 * Output : None
 * Call: internal call
 */
static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*÷–∂œ…Ë÷√*/
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //Preemption priority 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //Subpriority is 0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
* Function name: CAN_Mode_Config
 * Description : CAN mode configuration
 * Input : None
 * Output : None
 * Call: internal call
 */
static void CAN_Mode_Config(void)
{
   	CAN_InitTypeDef        CAN_InitStructure;
	 	/************************CAN communication parameter setting**********************************/
	/*CAN unit initialization*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	/*CAN unit initialization*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM Off time trigger communication mode enable
    CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM automatic offline management
    CAN_InitStructure.CAN_AWUM=ENABLE;			   //MCR-AWUM uses automatic wake mode
    CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART prohibits automatic retransmission of messages. DISABLE-automatic retransmission
    CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM Receive FIFO Lock Mode DISABLE-New message will overwrite the original message when it overflows
    CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP transmit FIFO priority DISABLE-priority depends on the message identifier
    CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW resynchronize jump width 2 time units
    CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;		   //BTR-TS1 time period 1 occupies 6 time units
    CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 time period 2 occupies 3 time units
    CAN_InitStructure.CAN_Prescaler =4;		   	   //BTR-BRP baud rate divider defines the length of time unit 36/(1+6+3)/4=0.9Mbps
	CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * Function name: CAN_Filter_Config
 * Description: CAN filter configuration
 * Input : None
 * Output : None
 * Call: internal call
 */
static void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN filter initialization*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						// Filter group 0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//Work in the identifier mask bit mode
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//The filter bit width is a single 32 bits.°£
	/* Enable the message identifier filter to perform comparison filtering according to the content of the identifier. If the extension ID is not as follows, it will be discarded. If it is, it will be stored in FIFO0. */

    CAN_FilterInitStructure.CAN_FilterIdHigh= (((u32)0x1314<<3)&0xFFFF0000)>>16;				//ID high to be filtere 
    CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //ID low to be filtered 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0xFFFF;			//Filter high 16 bits per bit must match
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0xFFFF;			//Filter lower 16 bits per bit must match
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//Filter is associated to FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//Enable filter
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN communication interrupt enable*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


/*
 * Function name: CAN_Config
 * Description: Fully configured CAN function
 * Input : None
 * Output : None
 * Call: external call
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}


/*
* Function name: CAN_SetMsg
 * Description : CAN communication message content setting
 * Input : None
 * Output : None
 * Call: external call
 */	 	 
void CAN_SetMsg(void)
{	  
  //TxMessage.StdId=0x00;						 
  TxMessage.ExtId=0x1314;					 //Extended ID used
  TxMessage.IDE=CAN_ID_EXT;					 //Extended mode
  TxMessage.RTR=CAN_RTR_DATA;				 //Sending data
  TxMessage.DLC=2;							 //The data length is 2 bytes
  TxMessage.Data[0]=0xDC;
  TxMessage.Data[1]=0xBA;
}
/*************************END OF FILE******************************/

