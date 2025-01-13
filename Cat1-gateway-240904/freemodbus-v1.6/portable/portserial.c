/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */
#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "DevUpDateCode.h"



static void prvvUARTTxReadyISR(void);
static void prvvUARTRxISR(void);


extern struct_update g_updatePara;
u16 g_mbRcved=0;		// 判断主控器是否在线;
/*********************************************************************/
// 串口4-USART1
#define	USARTx						UART5
#define	USART_CLK					RCC_APB1Periph_UART5 
#define	USART_APBxClkCmd			RCC_APB1PeriphClockCmd
//#define	USART_BAUDRATE				9600

// USART GPIO 引脚宏定义
#define	USART_GPIO_CLK				(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
#define	USART_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd
    
#define	USART_TX_GPIO_PORT			GPIOC
#define	USART_TX_GPIO_PIN			GPIO_Pin_12

#define	USART_RX_GPIO_PORT			GPIOD
#define	USART_RX_GPIO_PIN			GPIO_Pin_2

#define	USART_RE_GPIO_PORT			GPIOB
#define	USART_RE_GPIO_PIN			GPIO_Pin_3
#define	_485_SetRx()				while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){};GPIO_ResetBits(USART_RE_GPIO_PORT, USART_RE_GPIO_PIN)
#define	_485_SetTx()				GPIO_SetBits(USART_RE_GPIO_PORT, USART_RE_GPIO_PIN)

//#define	probe4_485_USART_IRQ		UART5_IRQn 
#define	MODBUS_UART_IRQHandler		UART5_IRQHandler


void Modbus_485_Config(u32 ulBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	
	// 禁止PB3作为JTDO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	
	// 打开串口外设的时钟
	USART_APBxClkCmd(USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 将USART RE的GPIO配置为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = USART_RE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(USART_RE_GPIO_PORT, &GPIO_InitStructure);

	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = ulBaudRate;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USARTx, &USART_InitStructure);
	
	// 使能串口接收中断
//	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	// 使能串口
	USART_Cmd(USARTx, ENABLE);
}


void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (TRUE == xRxEnable)
    {
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
        _485_SetRx();
    }
    else
    {
        USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
    }

    if (TRUE == xTxEnable)
    {
        _485_SetTx();
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
    }
    else
    {
        USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
    }
}

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
	Modbus_485_Config(ulBaudRate);
    return TRUE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
	USART_SendData(USARTx, ucByte);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){};
    return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    *pucByte = (char)USART_ReceiveData(USARTx);
    return TRUE;
}

BOOL MBPortSerialSend(CHAR *ucBuf, USHORT sndLen)
{
	_485_SetTx();
	for(u16 i=0; i<sndLen; i++)
		xMBPortSerialPutByte(ucBuf[i]);
	_485_SetRx();
}


/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR(void)
{
    pxMBFrameCBByteReceived();
}

// [Function] 串口1中断处理函数
void MODBUS_UART_IRQHandler(void)
{
	if(USART_GetFlagStatus(USARTx,USART_FLAG_ORE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_ORE);
		USART_ReceiveData(USARTx);
	}		

	if(USART_GetFlagStatus(USARTx,USART_FLAG_NE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_NE);
		USART_ReceiveData(USARTx);
	}			

	if(USART_GetFlagStatus(USARTx,USART_FLAG_FE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_FE);
		USART_ReceiveData(USARTx);
	}			

	if(USART_GetFlagStatus(USARTx,USART_FLAG_PE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_PE);
		USART_ReceiveData(USARTx);
	}	

	if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
	{
		if(g_updatePara.updateFlag>0 && g_updatePara.updateFlag<3)
		{
			if(g_updatePara.rcvLen<SlaveDevComBufLen+20)
				g_updatePara.rcvBuf[g_updatePara.rcvLen++]=USARTx->DR;
			else
				USARTx->DR;
		}
		else
			prvvUARTRxISR();
		USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	}

	if (USART_GetITStatus(USARTx, USART_IT_IDLE) != RESET)
	{
		/* clear idle. */
		USARTx->SR;
		USARTx->DR;
		g_mbRcved=1;
	}
	
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		prvvUARTTxReadyISR();
	}
}
// END
