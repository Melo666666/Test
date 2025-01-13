//////////////////////////////////////////////////////////////////////////////////
#ifndef __bsp_WIFI232B2_H
#define __bsp_WIFI232B2_H			  	 
#include "stm32f10x.h"	    	
#include "bspPublic.h"


#define		WIFI_KEY_PORT  					GPIOB
#define 	WIFI_KEY_PIN   					GPIO_Pin_5
#define 	WIFI_KEY_GPIO_CLK    			RCC_APB2Periph_GPIOB
#define		WIFI_KEY_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd

/*               WIFI-RESET          */
#define		WIFI_RESET_GPIO_PIN				GPIO_Pin_4
#define		WIFI_RESET_GPIO_PORT			GPIOC
#define		WIFI_RESET_GPIO_CLK				RCC_APB2Periph_GPIOC
#define		WIFI_RESET_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd

/*               WIFI-READY          */
#define		WIFI_READY_GPIO_PIN				GPIO_Pin_5
#define		WIFI_READY_GPIO_PORT			GPIOC
#define		WIFI_READY_GPIO_CLK				RCC_APB2Periph_GPIOC
#define		WIFI_READY_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd

/*               WIFI-NLINK          */
#define		WIFI_LINK_GPIO_PIN				GPIO_Pin_7
#define		WIFI_LINK_GPIO_PORT				GPIOA
#define		WIFI_LINK_GPIO_CLK				RCC_APB2Periph_GPIOC
#define		WIFI_LINK_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd

/*               WIFI-RELOAD          */
#define		WIFI_RELOAD_GPIO_PIN			GPIO_Pin_6
#define		WIFI_RELOAD_GPIO_PORT			GPIOA
#define		WIFI_RELOAD_GPIO_CLK			RCC_APB2Periph_GPIOC
#define		WIFI_RELOAD_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd
// USART GPIO ���ź궨��
//#define		WIFI_USART_GPIO_CLK				(RCC_AHB1Periph_GPIOG)
//#define		WIFI_USART_GPIO_APBxClkCmd		RCC_AHB1PeriphClockCmd
//    
//#define		WIFI_USART_TX_GPIO_PORT			GPIOG   
//#define		WIFI_USART_TX_GPIO_PIN			GPIO_Pin_14
//#define		WIFI_USART_RX_GPIO_PORT			GPIOG
//#define		WIFI_USART_RX_GPIO_PIN			GPIO_Pin_9

//// ����5-USART5
//#define		WIFI_USARTx						USART6
//#define		WIFI_USART_CLK					RCC_APB2Periph_USART6
//#define		WIFI_USART_APBxClkCmd			RCC_APB2PeriphClockCmd
//#define		WIFI_USART_BAUDRATE				57600

//#define		WIFI_USART_IRQ					USART6_IRQn
//#define		WIFI_USART_IRQHandler			USART6_IRQHandler

//#define		WIFI_TX_PinSource				GPIO_PinSource14
//#define		WIFI_RX_PinSource				GPIO_PinSource9
//#define		WIFI_AF_USART					GPIO_AF_USART6


#define WIFI_UART_BUF_LEN		NET_RCV_LEN_MAX
#define	IS_WIFI232B2_READY()	GPIO_ReadInputDataBit(WIFI_READY_GPIO_PORT, WIFI_READY_GPIO_PIN)
#define	IS_KEY_PRESSED()		GPIO_ReadInputDataBit(WIFI_KEY_PORT, WIFI_KEY_PIN)

#define SEND_AT_RETRY_MAX		3



typedef struct
{
	u8 status;						// bit0-����״̬��bit1-����wifi״̬��bit2-���ӷ�����״̬;
	u8 err;							// bit0-����ʧ�ܣ�bit1-��MCUͨѶ����bit2-��������ͨѶ����
	u8 IP[4];						// ģ��IP���������롢���ء�MAC;
	u8 Mask[4];
	u8 Gate[4];
	u8 Mac[6];
	u8 Heart_Buf[HEART_DATA_LEN];	// ��������ʮ������;
	u16 tcp_port;					// TCP������IP���˿�;
	u8 tcp_sever_ip[4];
	u16 resetCnt;					// ��������ͳ��;
	u8 Mode;
	u8 connectFlag;
	u8 keyPressFlag;
	
	u8 rcvFlag;				// ���ڽ��ս�����־;
	u16 rcvLen;				// ���ڽ������ݳ���;
	u8 *rcvBuf;
	u16 sndLen;
	u8 *sndBuf;
	
}wifi232b2_struct;



void wifi232b2_Reset(void);
u8 wifi232b2_Reload(void);

u8 wifi232b2_Init(void);
void set_Tcp_Para(short , short *);
u8 wifi232b2_Con_Wifi(void);
void check_Wifi232b2_Status(void);
void wifi232_Send(u8 *sndBuf, u16 sndLen);
void Wifi232b2_DeInit(void);
u8 wifi232b2_key_scan(void);
void wifi232b2_chg_ip(void);
void wifi232b2_State_Machine();
void wifi232b2_usart_cfg(void);
void WIFI_Task(void);

#endif
