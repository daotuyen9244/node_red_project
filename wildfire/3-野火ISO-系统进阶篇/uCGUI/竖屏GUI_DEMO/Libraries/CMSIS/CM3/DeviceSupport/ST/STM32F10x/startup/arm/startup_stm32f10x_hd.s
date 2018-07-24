;******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
;* File Name          : startup_stm32f10x_hd.s
;* Author             : MCD Application Team
;* Version            : V3.4.0
;* Date               : 10/15/2010
;* Description        : STM32F10x High Density Devices vector table for MDK-ARM 
;*                      toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Configure the clock system and also configure the external 
;*                        SRAM mounted on STM3210E-EVAL board to be used as data 
;*                        memory (optional, to be enabled by user)
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>> 
;                        INITIALIZE EXCEPTION VECTORS
; ���ȶ�ջ�ͶѵĴ�С���ж��壬���ڴ���������ʼ�������ж����������һ��������ջ
; ����ַ���ڶ��������Ǹ�λ�жϷ�����ڵ�ַ��Ȼ���ڸ�λ�жϷ����������ת??C/C++��
; ׼ʵʱ���__main����������STM32������Ϊ���ڲ�FLASH�����ж���������ʼ��λΪ0x8000000��
; ��ջ����ַ�����0x8000000��������λ�жϷ�����ڵ�ַ�����0x8000004������STM32��
; ����λ�źź����0x80000004��ȡ����λ�жϷ�����ڵ�ַ�̶�ִ�и�λ�жϷ������
; Ȼ����ת__main�������������C�����硣
; DCDָ������ǿ���һ�οռ䣬������ȼ���C�����еĵ�ַ����&������ʼ�������ж�����
; ����������ʹ��C��.��ÿһ����Ա����һ������ָ�룬�ֱ�ָ������жϷ�����
;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x0000800										;����ջ�ռ�Ĵ�С1024byte��
																		;αָ��AREA����ʾ����һ�δ�СΪStack_Size���ڴ�ռ���Ϊջ��������STACK���ɶ���д��
                AREA    STACK, NOINIT, READWRITE, ALIGN=3				;NOINIT��ָ�������ݶν����������ڴ浥Ԫ����û�н�����ʼֵд���ڴ浥Ԫ�����߽������ڴ浥Ԫֵ��ʼ��Ϊ0
Stack_Mem       SPACE   Stack_Size										;�������� Stack_Size �ֽڵĴ洢��Ԫ����ʼ��Ϊ 0��
__initial_sp															;���__initial_sp����ʾջ�ռ䶥��ַ��
                                                  
; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x0000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3				;ALIGN����ָ�����뷽ʽ2��3������8��Ҳ����8�ֽڶ���
__heap_base															    ;��ʾ�ѿռ���ʼ��ַ
Heap_Mem        SPACE   Heap_Size
__heap_limit															;��ʾ�ѿռ������ַ

                PRESERVE8												;PRESERVE8 ָ��ָ����ǰ�ļ����ֶ�ջ���ֽڶ���
                THUMB													;THUMBָ��
																						 

; Vector Table Mapped to Address 0 at Reset								;ʵ��������CODE��������STM32��FLASH����������ж���������ʼ��ַ��Ϊ0x8000000��
                AREA    RESET, DATA, READONLY							;����һ�����ݶΣ�ֻ�ɶ�����������RESET
                EXPORT  __Vectors										;EXPORT���ڳ���������һ��ȫ�ֵı��__Vectors���ñ�ſ����������ļ�������
                EXPORT  __Vectors_End									;�ڳ���������һ��ȫ�ֵı��__Vectors_End
                EXPORT  __Vectors_Size 									;�ڳ���������һ��ȫ�ֵı��__Vectors_Size
__Vectors       														;�����ж�������
				DCD     __initial_sp               						; 0,Top of Stack
                DCD     Reset_Handler              						; 1,Reset Handler
                DCD     NMI_Handler                						; 2,NMI Handler
                DCD     HardFault_Handler          						; 3,Hard Fault Handler
                DCD     MemManage_Handler          						; 4,MPU Fault Handler
                DCD     BusFault_Handler           						; 5,Bus Fault Handler
                DCD     UsageFault_Handler        						; 6,Usage Fault Handler
                DCD     0                          						; 7,Reserved
                DCD     0                          						; 8,Reserved
                DCD     0                          						; 9,Reserved
                DCD     0                          						; 10,Reserved
                DCD     SVC_Handler                						; 11,SVCall Handler
                DCD     DebugMon_Handler           						; 12,Debug Monitor Handler
                DCD     0                          						; 13,Reserved
				;IMPORT  OSPendSV
                ;DCD     OSPendSV				   						; for u/COS-II ��ǰ�� PendSV Handler
				;IMPORT  Tmr_TickISR_Handler
                ;DCD     Tmr_TickISR_Handler       						; for u/COS-II ��ǰ�� SysTick Handler
				;IMPORT   OS_CPU_PendSVHandler
				DCD      OS_CPU_PendSVHandler	   						;14,PendSV Handler

				;IMPORT 	 OS_CPU_SysTickHandler
				DCD   	 OS_CPU_SysTickHandler	   						;15,uC/OS-II Tick ISR Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler                             ; 16, INTISR[  0]  Window Watchdog.                   
        		DCD     PVD_IRQHandler                              ; 17, INTISR[  1]  PVD through EXTI Line Detection.    
        		DCD     TAMPER_IRQHandler                           ; 18, INTISR[  2]  Tamper Interrupt.                   
        		DCD     RTC_IRQHandler                           	; 19, INTISR[  3]  RTC Global Interrupt.               
		        DCD     FLASH_IRQHandler                         	; 20, INTISR[  4]  FLASH Global Interrupt.             
		        DCD     RCC_IRQHandler                           	; 21, INTISR[  5]  RCC Global Interrupt.               
		        DCD     EXTI0_IRQHandler                         	; 22, INTISR[  6]  EXTI Line0 Interrupt.               
		        DCD     EXTI1_IRQHandler                         	; 23, INTISR[  7]  EXTI Line1 Interrupt.               
		        DCD     EXTI2_IRQHandler                         	; 24, INTISR[  8]  EXTI Line2 Interrupt.               
		        DCD     EXTI3_IRQHandler                            ; 25, INTISR[  9]  EXTI Line3 Interrupt.                                            								
				DCD     EXTI4_IRQHandler               				; 26, INTISR[ 10]  EXTI Line4 Interrupt.
		        DCD     DMA1_Channel1_IRQHandler                    ; 27, INTISR[ 11]  DMA Channel1 Global Interrupt.      
		        DCD     DMA1_Channel2_IRQHandler                    ; 28, INTISR[ 12]  DMA Channel2 Global Interrupt.      
		        DCD     DMA1_Channel3_IRQHandler                    ; 29, INTISR[ 13]  DMA Channel3 Global Interrupt.      
		        DCD     DMA1_Channel4_IRQHandler                    ; 30, INTISR[ 14]  DMA Channel4 Global Interrupt.      
		        DCD     DMA1_Channel5_IRQHandler                    ; 31, INTISR[ 15]  DMA Channel5 Global Interrupt.      
		        DCD     DMA1_Channel6_IRQHandler                    ; 32, INTISR[ 16]  DMA Channel6 Global Interrupt.      
		        DCD     DMA1_Channel7_IRQHandler                    ; 33, INTISR[ 17]  DMA Channel7 Global Interrupt.      
		        DCD     ADC1_2_IRQHandler                           ; 34, INTISR[ 18]  ADC1 & ADC2 Global Interrupt.       
		        DCD     USB_HP_CAN1_TX_IRQHandler                   ; 35, INTISR[ 19]  USB High Prio / CAN TX  Interrupts. 
		        DCD     USB_LP_CAN1_RX0_IRQHandler                  ; 36, INTISR[ 20]  USB Low  Prio / CAN RX0 Interrupts. 
		        DCD     CAN1_RX1_IRQHandler                        	; 37, INTISR[ 21]  CAN RX1 Interrupt.                  
		        DCD     CAN1_SCE_IRQHandler                       	; 38, INTISR[ 22]  CAN SCE Interrupt.                  
		        DCD     EXTI9_5_IRQHandler                       	; 39, INTISR[ 23]  EXTI Line[9:5] Interrupt.           
		        DCD     TIM1_BRK_IRQHandler                      	; 40, INTISR[ 24]  TIM1 Break  Interrupt.              
		        DCD     TIM1_UP_IRQHandler                          ; 41, INTISR[ 25]  TIM1 Update Interrupt.              
		        DCD     TIM1_TRG_COM_IRQHandler                     ; 42, INTISR[ 26]  TIM1 Trig & Commutation Interrupts. 
		        DCD     TIM1_CC_IRQHandler                       	; 43, INTISR[ 27]  TIM1 Capture Compare Interrupt.     
		        DCD     TIM2_IRQHandler                          	; 44, INTISR[ 28]  TIM2 Global Interrupt.              
		        DCD     TIM3_IRQHandler                          	; 45, INTISR[ 29]  TIM3 Global Interrupt.              
		        DCD     TIM4_IRQHandler                          	; 46, INTISR[ 30]  TIM4 Global Interrupt.              
		        DCD     I2C1_EV_IRQHandler                       	; 47, INTISR[ 31]  I2C1 Event  Interrupt.              
		
		        DCD     I2C1_ER_IRQHandler                       	; 48, INTISR[ 32]  I2C1 Error  Interrupt.              
		        DCD     I2C2_EV_IRQHandler                       	; 49, INTISR[ 33]  I2C2 Event  Interrupt.              
		        DCD     I2C2_ER_IRQHandler                       	; 50, INTISR[ 34]  I2C2 Error  Interrupt.             
		        DCD     SPI1_IRQHandler                          	; 51, INTISR[ 35]  SPI1 Global Interrupt.              
		        DCD     SPI2_IRQHandler                          	; 52, INTISR[ 36]  SPI2 Global Interrupt.              
		        DCD     USART1_IRQHandler                        	; 53, INTISR[ 37]  USART1 Global Interrupt.            
		        DCD     USART2_IRQHandler                        	; 54, INTISR[ 38]  USART2 Global Interrupt.            
		        DCD     USART3_IRQHandler                        	; 55, INTISR[ 39]  USART3 Global Interrupt.            
		        DCD     EXTI15_10_IRQHandler                     	; 56, INTISR[ 40]  EXTI Line [15:10] Interrupts.       
		        DCD     RTCAlarm_IRQHandler                      	; 57, INTISR[ 41]  RTC Alarm EXT Line Interrupt.       
		        DCD     USBWakeUp_IRQHandler                     	; 58, INTISR[ 42]  USB Wakeup from Suspend EXTI Int.   
		        DCD     TIM8_BRK_IRQHandler                      	; 59, INTISR[ 43]  TIM8 Break Interrupt.               
		        DCD     TIM8_UP_IRQHandler                      	; 60, INTISR[ 44]  TIM8 Update Interrupt.              
		        DCD     TIM8_TRG_COM_IRQHandler                  	; 61, INTISR[ 45]  TIM8 Trigg/Commutation Interrupts.  
		        DCD     TIM8_CC_IRQHandler                       	; 62, INTISR[ 46]  TIM8 Capture Compare Interrupt.     
		        DCD     ADC3_IRQHandler                          	; 63, INTISR[ 47]  ADC3 Global Interrupt.              
		
		        DCD     FSMC_IRQHandler                          	; 64, INTISR[ 48]  FSMC Global Interrupt.              
		        DCD     SDIO_IRQHandler                          	; 65, INTISR[ 49]  SDIO Global Interrupt.              
		        DCD     TIM5_IRQHandler                          	; 66, INTISR[ 50]  TIM5 Global Interrupt.              
		        DCD     SPI3_IRQHandler                          	; 67, INTISR[ 51]  SPI3 Global Interrupt.              
		        DCD     UART4_IRQHandler                         	; 68, INTISR[ 52]  UART4 Global Interrupt.             
		        DCD     UART5_IRQHandler                         	; 69, INTISR[ 53]  UART5 Global Interrupt.             
		        DCD     TIM6_IRQHandler                             ; 70, INTISR[ 54]  TIM6 Global Interrupt.              
		        DCD     TIM7_IRQHandler                             ; 71, INTISR[ 55]  TIM7 Global Interrupt.              
		        DCD     DMA2_Channel1_IRQHandler                    ; 72, INTISR[ 56]  DMA2 Channel1 Global Interrupt.     
		        DCD     DMA2_Channel2_IRQHandler                    ; 73, INTISR[ 57]  DMA2 Channel2 Global Interrupt.     
		        DCD     DMA2_Channel3_IRQHandler                    ; 74, INTISR[ 58]  DMA2 Channel3 Global Interrupt.     
		        DCD     DMA2_Channel4_5_IRQHandler                  ; 75, INTISR[ 59]  DMA2 Channel4/5 Global Interrups. 
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors						;�õ�������Ĵ�С,304���ֽ�Ҳ����0x130���ֽ�

                AREA    |.text|, CODE, READONLY			  ;����һ������Σ��ɶ�����������.text
                
; Reset handler
Reset_Handler   PROC									  ;����PROC��ENDP��һ��αָ��ѳ���η�Ϊ���ɸ����̣�ʹ����Ľṹ������
                EXPORT  Reset_Handler             [WEAK]  ;���ⲿû�ж���÷���ʱ�����÷���Reset_Handler  
                IMPORT  __main							  ;IMPORT��αָ������֪ͨ������Ҫʹ�õı����������Դ�ļ��ж���
                IMPORT  SystemInit						  ;��Ҫ�ڵ�ǰԴ�ļ������ã��������۵�ǰԴ�ļ��Ƿ����øñ�ţ��ñ�ž��ᱻ���뵽��ǰԴ�ļ��ķ��ű���
                LDR     R0, =SystemInit
                BLX     R0               
                LDR     R0, =__main						  ;__mainΪ ����ʱ���ṩ�ĺ�������ɶ�ջ���ѵĳ�ʼ���ȹ�������������涨���__user_initial_stackheap
                BX      R0								  ;����__main������C������   
                ENDP
                
; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK] ;WEAK����������ͬ����������ڸñ�ű�����,����˵������������˵Ļ���
                B       .								  ;����������
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
OS_CPU_PendSVHandler   PROC				
                EXPORT  OS_CPU_PendSVHandler       [WEAK]
                B       .			
               ENDP
OS_CPU_SysTickHandler   PROC               
                EXPORT  OS_CPU_SysTickHandler      [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler            [WEAK]
                EXPORT  PVD_IRQHandler             [WEAK]
                EXPORT  TAMPER_IRQHandler          [WEAK]
                EXPORT  RTC_IRQHandler             [WEAK]
                EXPORT  FLASH_IRQHandler           [WEAK]
                EXPORT  RCC_IRQHandler             [WEAK]
                EXPORT  EXTI0_IRQHandler           [WEAK]
                EXPORT  EXTI1_IRQHandler           [WEAK]
                EXPORT  EXTI2_IRQHandler           [WEAK]
                EXPORT  EXTI3_IRQHandler           [WEAK]
                EXPORT  EXTI4_IRQHandler           [WEAK]
                EXPORT  DMA1_Channel1_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel2_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel3_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel4_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel5_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel6_IRQHandler   [WEAK]
                EXPORT  DMA1_Channel7_IRQHandler   [WEAK]
                EXPORT  ADC1_2_IRQHandler          [WEAK]
                EXPORT  USB_HP_CAN1_TX_IRQHandler  [WEAK]
                EXPORT  USB_LP_CAN1_RX0_IRQHandler [WEAK]
                EXPORT  CAN1_RX1_IRQHandler        [WEAK]
                EXPORT  CAN1_SCE_IRQHandler        [WEAK]
                EXPORT  EXTI9_5_IRQHandler         [WEAK]
                EXPORT  TIM1_BRK_IRQHandler        [WEAK]
                EXPORT  TIM1_UP_IRQHandler         [WEAK]
                EXPORT  TIM1_TRG_COM_IRQHandler    [WEAK]
                EXPORT  TIM1_CC_IRQHandler         [WEAK]
                EXPORT  TIM2_IRQHandler            [WEAK]
                EXPORT  TIM3_IRQHandler            [WEAK]
                EXPORT  TIM4_IRQHandler            [WEAK]
                EXPORT  I2C1_EV_IRQHandler         [WEAK]
                EXPORT  I2C1_ER_IRQHandler         [WEAK]
                EXPORT  I2C2_EV_IRQHandler         [WEAK]
                EXPORT  I2C2_ER_IRQHandler         [WEAK]
                EXPORT  SPI1_IRQHandler            [WEAK]
                EXPORT  SPI2_IRQHandler            [WEAK]
                EXPORT  USART1_IRQHandler          [WEAK]
                EXPORT  USART2_IRQHandler          [WEAK]
                EXPORT  USART3_IRQHandler          [WEAK]
                EXPORT  EXTI15_10_IRQHandler       [WEAK]
                EXPORT  RTCAlarm_IRQHandler        [WEAK]
                EXPORT  USBWakeUp_IRQHandler       [WEAK]
                EXPORT  TIM8_BRK_IRQHandler        [WEAK]
                EXPORT  TIM8_UP_IRQHandler         [WEAK]
                EXPORT  TIM8_TRG_COM_IRQHandler    [WEAK]
                EXPORT  TIM8_CC_IRQHandler         [WEAK]
                EXPORT  ADC3_IRQHandler            [WEAK]
                EXPORT  FSMC_IRQHandler            [WEAK]
                EXPORT  SDIO_IRQHandler            [WEAK]
                EXPORT  TIM5_IRQHandler            [WEAK]
                EXPORT  SPI3_IRQHandler            [WEAK]
                EXPORT  UART4_IRQHandler           [WEAK]
                EXPORT  UART5_IRQHandler           [WEAK]
                EXPORT  TIM6_IRQHandler            [WEAK]
                EXPORT  TIM7_IRQHandler            [WEAK]
                EXPORT  DMA2_Channel1_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel2_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel3_IRQHandler   [WEAK]
                EXPORT  DMA2_Channel4_5_IRQHandler [WEAK]

WWDG_IRQHandler
PVD_IRQHandler
TAMPER_IRQHandler
RTC_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_IRQHandler
DMA1_Channel3_IRQHandler
DMA1_Channel4_IRQHandler
DMA1_Channel5_IRQHandler
DMA1_Channel6_IRQHandler
DMA1_Channel7_IRQHandler
ADC1_2_IRQHandler
USB_HP_CAN1_TX_IRQHandler
USB_LP_CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_IRQHandler
TIM1_UP_IRQHandler
TIM1_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTCAlarm_IRQHandler
USBWakeUp_IRQHandler
TIM8_BRK_IRQHandler
TIM8_UP_IRQHandler
TIM8_TRG_COM_IRQHandler
TIM8_CC_IRQHandler
ADC3_IRQHandler
FSMC_IRQHandler
SDIO_IRQHandler
TIM5_IRQHandler
SPI3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIM6_IRQHandler
TIM7_IRQHandler
DMA2_Channel1_IRQHandler
DMA2_Channel2_IRQHandler
DMA2_Channel3_IRQHandler
DMA2_Channel4_5_IRQHandler
                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB			   ;�ж��Ƿ�ʹ��DEF:__MICROLIB��micro lib��
                
                 EXPORT  __initial_sp				   ;ʹ�õĻ���ջ����ַ����ʼĩ��ַ����ȫ����
                 EXPORT  __heap_base				   ;�ԣ�ʹ�ⲿ�������ʹ��
                 EXPORT  __heap_limit
                
                 ELSE								   ;���ʹ��Ĭ��C������ʱ
                
                 IMPORT  __use_two_region_memory	   ;����ȫ�ֱ��__use_two_region_memory
                 EXPORT  __user_initial_stackheap	   ;����ȫ�ֱ��__user_initial_stackheap�����������Ҳ�ɵ��ô˱��
                 									   ;����ж�ջ�Ͷѵĸ�ֵ����__main����ִ�й����е���
__user_initial_stackheap							   ;���__user_initial_stackheap����ʾ�û���ջ��ʼ���������

                 LDR     R0, =  Heap_Mem			   ;�����ʼ��ַ
                 LDR     R1, =(Stack_Mem + Stack_Size) ;����ջ�Ĵ�С
                 LDR     R2, = (Heap_Mem +  Heap_Size) ;����ѵĴ�С
                 LDR     R3, = Stack_Mem			   ;����ջ��ָ��
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE*****
