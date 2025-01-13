#ifndef _BSPSERIAL485_H_
#define _BSPSERIAL485_H_

#include "stm32f10x.h"

#define BSP485_USART1_RXEN()		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define BSP485_USART1_TXEN()		GPIO_SetBits(GPIOA, GPIO_Pin_11)
	
#define BSP485_USART2_RXEN()		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){};GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define BSP485_USART2_TXEN()		GPIO_SetBits(GPIOB, GPIO_Pin_0)
	
#define BSP485_USART3_RXEN()		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){};GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define BSP485_USART3_TXEN()		GPIO_SetBits(GPIOB, GPIO_Pin_2)

void bsp485_usart1_init(u32 ulBaudRate);
void bsp485_usart1_sendstring(uint8_t *str, uint16_t strlen);
void bsp485_usart1_print(uint8_t *str);
void bsp485_usart1_tx_enable(void);
void bsp485_usart1_rx_enable(void);

void bsp485_usart2_init(u32 ulBaudRate);
void bsp485_usart2_sendstring(uint8_t *str, uint16_t length);
void bsp485_usart2_tx_enable(void);
void bsp485_usart2_rx_enable(void);

void bsp485_usart3_init(u32 ulBaudRate);
void bsp485_usart3_sendstring(uint8_t *str, uint16_t length);

#endif

