#include "bspSerial485.h"
#include "string.h"

// [sF] bsp485_usart1_io_re_init
static inline void bsp485_usart1_io_re_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}
// END

// [sF] bsp485_sendbyte
static void bsp485_sendbyte(USART_TypeDef *USARTx, uint8_t ch)
{
	USART_SendData(USARTx, ch);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
	{
	};
}
// END

// [Function] bsp485_usart1_init
void bsp485_usart1_init(u32 ulBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	bsp485_usart1_io_re_init();

	USART_InitStructure.USART_BaudRate = ulBaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
// END

// [F]
void bsp485_usart1_tx_enable(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
	BSP485_USART1_TXEN();
	USART_ITConfig(USART1, USART_IT_TXE,  ENABLE);
} 
// END

// [F]
void bsp485_usart1_rx_enable(void)
{
	USART_ITConfig(USART1, USART_IT_TXE,  DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	BSP485_USART1_RXEN();
}
// END

// [Function] bsp485_usart1_sendstring
void bsp485_usart1_sendstring(uint8_t *str, uint16_t strlen)
{
	unsigned int k = 0;

//	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

	BSP485_USART1_TXEN();

	do
	{
		bsp485_sendbyte(USART1, str[k++]);
	} while (k < strlen);

	BSP485_USART1_RXEN();
}
// END

// [Function] bsp485_usart1_print
void bsp485_usart1_print(uint8_t *str)
{
	unsigned int k = 0;
	unsigned int strlength = strlen((char *)str);

	BSP485_USART1_TXEN();

	do
	{
		bsp485_sendbyte(USART1, str[k++]);
	} while (k < strlength);

	BSP485_USART1_RXEN();
}
// END

// [Function] bsp485_usart2_init
void bsp485_usart2_init(u32 ulBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// RE/DE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	USART_InitStructure.USART_BaudRate = ulBaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}
// END

// [F]
void bsp485_usart2_tx_enable(void)
{
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
	BSP485_USART2_TXEN();
	USART_ITConfig(USART2, USART_IT_TXE,  ENABLE);
} 
// END

// [F]
void bsp485_usart2_rx_enable(void)
{
	USART_ITConfig(USART2, USART_IT_TXE,  DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	BSP485_USART2_RXEN();
}
// END

// [Function] bsp485_usart2_sendstring
void bsp485_usart2_sendstring(uint8_t *str, uint16_t strlen)
{
	unsigned int k = 0;

	// USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

	BSP485_USART2_TXEN();

	do
	{
		bsp485_sendbyte(USART2, str[k++]);
	} while (k < strlen);

	BSP485_USART2_RXEN();
}
// END

// [Function] bsp485_usart3_init
void bsp485_usart3_init(u32 ulBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = ulBaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
}
// END

// [Function] bsp485_usart3_sendstring
void bsp485_usart3_sendstring(uint8_t *str, uint16_t strlen)
{
	unsigned int k = 0;

	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);

	BSP485_USART3_TXEN();

	do
	{
		bsp485_sendbyte(USART3, str[k++]);
	} while (k < strlen);

	BSP485_USART3_RXEN();
}
// END
