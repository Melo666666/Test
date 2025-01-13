#include "NVIC.h"

#if 1
// 配置NVIC,用以管理各个中断
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}
#else
// 配置NVIC,用以管理各个中断
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
#endif


//uart1--dev, uart2\uart3--wireless, uart5--upper;
// 设置设备串口最高优先级;
void set_dev_priority(Dev_Priority_Enum level)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	switch(level)
    {
        case NoSend:
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
            NVIC_Init(&NVIC_InitStructure);
        break;
        
        case UpperSend:
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
            NVIC_Init(&NVIC_InitStructure);
        break;
        
        case UartSend:
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
            NVIC_Init(&NVIC_InitStructure);
        break;
        
        case NetSend:
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
            NVIC_Init(&NVIC_InitStructure);
        break;

        default:
        break;

    }
	
   
}	

