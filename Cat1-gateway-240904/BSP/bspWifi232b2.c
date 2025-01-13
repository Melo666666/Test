#include "bspWifi232b2.h"
#include "string.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "timers.h"




wifi232b2_struct wifi232b2Para={0};

extern TimerHandle_t wifi232_rcv_timer;
extern u8 g_infoSndFlag, g_netDataRcvFlag;

/*********************************************************************/
// ����4-USART1
#define  USARTx                   USART3
#define  USART_CLK                RCC_APB1Periph_USART3 
#define  USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART_BAUDRATE           57600

// USART GPIO ���ź궨��
#define  USART_GPIO_CLK          (RCC_APB2Periph_GPIOB)
#define  USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART_TX_GPIO_PORT       GPIOB
#define  USART_TX_GPIO_PIN        GPIO_Pin_10

#define  USART_RX_GPIO_PORT       GPIOB
#define  USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  probe4_485_USART_IRQ                UART5_IRQn 
#define  WIFI_USART_IRQHandler		USART3_IRQHandler

void Wifi232b2_DeInit(void)
{
	USART_Cmd(USARTx, DISABLE);
	USART_DeInit(USARTx);
}

void wifi232b2_gpio_cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	WIFI_KEY_GPIO_APBxClkCmd(WIFI_KEY_GPIO_CLK, ENABLE);
	/******************KEY********************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = WIFI_KEY_PIN;
	GPIO_Init(WIFI_KEY_PORT, &GPIO_InitStructure);
	
	/******************GPIO********************/
	WIFI_RESET_GPIO_APBxClkCmd(WIFI_RESET_GPIO_CLK, ENABLE);
	WIFI_READY_GPIO_APBxClkCmd(WIFI_READY_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = WIFI_RESET_GPIO_PIN;
	GPIO_Init(WIFI_RESET_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(WIFI_RESET_GPIO_PORT, WIFI_RESET_GPIO_PIN);
	
	GPIO_InitStructure.GPIO_Pin = WIFI_RELOAD_GPIO_PIN;
	GPIO_Init(WIFI_RELOAD_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(WIFI_RELOAD_GPIO_PORT, WIFI_RELOAD_GPIO_PIN);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = WIFI_READY_GPIO_PIN;
	GPIO_Init(WIFI_READY_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = WIFI_LINK_GPIO_PIN;
	GPIO_Init(WIFI_LINK_GPIO_PORT, &GPIO_InitStructure);
	
}

void wifi232b2_usart_cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART_APBxClkCmd(USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USARTx, &USART_InitStructure);
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);
	USART_ClearFlag(USARTx, USART_FLAG_TC); // ������������־λ
}


// [Function] ����2�жϴ�����
void WIFI_USART_IRQHandler(void)
{
	uint8_t data;
	static u16 sndLen;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
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
		
	/* Rx Not empty. */
	if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
	{
		data = USARTx->DR;
		if (wifi232b2Para.rcvLen < WIFI_UART_BUF_LEN)
		{
			wifi232b2Para.rcvBuf[wifi232b2Para.rcvLen++] = data;
		}
	}
	else if (USART_GetITStatus(USARTx, USART_IT_IDLE) != RESET)
	{
		/* clear idle. */
		data = USARTx->SR;
		data = USARTx->DR;
		/* set state and wake up thread. */
//		if (wifi232b2Para.rcvFlag == RCV_IDLE)
		{
//			wifi232b2Para.rcvFlag++;// = RCV_RECEIVED;
////			xSemaphoreGiveFromISR(Air_Rcv_Flag, &xHigherPriorityTaskWoken);
		}
//		/* stop timer. */
//		xTimerStopFromISR(Air_Rsp_Timeout, &xHigherPriorityTaskWoken);
		
		xTimerStartFromISR(wifi232_rcv_timer, &xHigherPriorityTaskWoken);
	}
	/* TxEmpty interrupt. */
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		if (sndLen < wifi232b2Para.sndLen)
		{
			USARTx->DR = wifi232b2Para.sndBuf[sndLen++];
		}
		else
		{
			sndLen=0;
			wifi232b2Para.sndLen=0;
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USARTx, USART_IT_TXE);
		}
	}
}

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void delay_ms(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	vTaskDelay(nCount);
}

u8 wifi232b2_key_scan(void)
{
	u16 pressTim=0;
	if(0==IS_KEY_PRESSED())
	{
		delay_ms(20);
		while(0==IS_KEY_PRESSED())
		{
			pressTim++;
			delay_ms(100);
		}
		if(pressTim > 30){
			return 1;
		}
	}
	return 0;
}

/* ����ATָ���WIFIģ��	sndMsg-Ҫ���͵�ָ�retryCnt-�ط�������Ϊ0xFFʱ��ʾһֱ�ط���needRcv-��Ҫ���ؽ��յ��ı���
rcvMsg-��Ҫ�õ��Ļظ������needRcv��Ϊ0�����ҷ��صĸ�rcvMsg��ͬ���Ὣ���صı��Ŀ�����rcvMsg��*/
static u8 Snd_AT_Wifi232B2(char *sndMsg, char *rcvMsg, u8 retryMax, u8 needRcv)
{
	char i, tim=0, retry=0, *ptr=NULL, rcvLen=0, rcvBuf[50]={0};
	strcpy(rcvBuf, rcvMsg);
ReSend:
	i=0;
	wifi232b2Para.rcvLen = 0;
	wifi232b2Para.rcvFlag= 0;
	if(++retry > retryMax)	return 0;
	wifi232_Send((u8*)sndMsg, strlen(sndMsg));
	while(1)
	{
WaitRsp:		
		delay_ms(20);
		if(wifi232b2Para.rcvFlag)
		{
			// ģ��ظ�ATָ��ַ��䲻������+ok����400ms���ϻظ�;
			if(strcmp(sndMsg, "+++") && strcmp(sndMsg, "a") && rcvLen < strlen(rcvMsg)+strlen(sndMsg))
			{
				if(++tim < 40)	goto WaitRsp;
			}
			rcvLen = wifi232b2Para.rcvLen;
			wifi232b2Para.rcvLen = 0;
			wifi232b2Para.rcvFlag= 0;
			if(needRcv)
			{
				memcpy(rcvMsg, wifi232b2Para.rcvBuf, rcvLen);
				rcvMsg[rcvLen] = '\0';
			}
			if(!strcmp(sndMsg, "+++") || !strcmp(sndMsg, "a"))
			{
				if(rcvLen == strlen(rcvBuf) && !memcmp(wifi232b2Para.rcvBuf, rcvBuf, strlen(rcvBuf))){
					return 1;
				}
			}
			else
			{
				ptr=strstr((char*)wifi232b2Para.rcvBuf, "\r+");
				if(rcvLen > strlen(sndMsg))	rcvLen -= strlen(sndMsg)+1;
				if(ptr)
				{
					ptr+=1;		// ��λ����+����Ҳ���ǻظ�����ʼλ��;
					if(rcvLen == strlen(rcvBuf) && !memcmp(ptr, rcvBuf, strlen(rcvBuf))){
						return 1;
					}
					if(needRcv)
					{
						memcpy(rcvMsg, ptr, rcvLen);
						rcvMsg[rcvLen] = '\0';
					}
				}
			}
			goto ReSend;
		}
		if(++i >= 10) goto ReSend;		// �����;
	}
}

void Wait_Wifi232b2_Ready(void)
{
	u8 i=0;
	while(IS_WIFI232B2_READY())		// �͵�ƽ��ʾ������;
	{
		delay_ms(2000);
		if(++i >= 3)
		{
			
		}
	}
}

// ����wifiģ��
void wifi232b2_Reset(void)
{
	u8 i=0;
	
	GPIO_ResetBits(WIFI_RESET_GPIO_PORT, WIFI_RESET_GPIO_PIN);
	delay_ms(350);		// 300ms����;
	GPIO_SetBits(WIFI_RESET_GPIO_PORT, WIFI_RESET_GPIO_PIN);
	while(IS_WIFI232B2_READY()){		// �͵�ƽ��ʾ������;
		delay_ms(200);
		if(i++ > 50){
			wifi232b2Para.Mode=Transparent;
			return;		
		}
	}
	
	wifi232b2Para.rcvFlag=0;
	wifi232b2Para.rcvLen=0;
	wifi232b2Para.Mode=Transparent;
}

// ����wifiģ��
u8 wifi232b2_Reload(void)
{
//	if(Snd_AT_Wifi232B2("AT+RELD\r", "+ok=rebooting��\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
//		while(IS_WIFI232B2_READY()){	// �͵�ƽ��ʾ������;
//			delay_ms(2000);
//		}
//		return 1;
//	}
//	return 0;
	GPIO_ResetBits(WIFI_RELOAD_GPIO_PORT, WIFI_RELOAD_GPIO_PIN);
	delay_ms(3500);
	GPIO_SetBits(WIFI_RELOAD_GPIO_PORT, WIFI_RELOAD_GPIO_PIN);
	return 1;
}

// ����AT����ģʽ
static u8 Set_Mode_AT(void)
{
	wifi232b2Para.Mode = AT_Cmd;
	char rcvBuf[60] = "a";
	if(Snd_AT_Wifi232B2("+++", rcvBuf, SEND_AT_RETRY_MAX, 1)){
		if(Snd_AT_Wifi232B2("a", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
			return 1;
		}
	}
	else if(!strcmp(rcvBuf, "+++"))
		return 1;
	wifi232b2Para.Mode = Transparent;
	return 0;
}

// �رջ���
static u8 Set_Echo_Off(void)
{
	char rcvBuf[60]={0};
	if(!Snd_AT_Wifi232B2("AT+E\r", rcvBuf, 1, 1)){
		if(!strcmp(rcvBuf, "+ok\r\n\r\n")){
			if(!Snd_AT_Wifi232B2("AT+E\r", "AT+E\rok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
				return 1;
			}
		}
		else if(!strcmp(rcvBuf, "AT+E\rok\r\n\r\n")){
			return 1;
		}		
	}
	return 0;
}

// ����͸��ģʽ
static u8 Set_Mode_ENTM(void)
{
	if(Snd_AT_Wifi232B2("AT+ENTM\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
		wifi232b2Para.Mode=Transparent;
		return 1;
	}
	wifi232b2Para.Mode=Transparent;
	return 0;
	
}
// ����APģʽ�����߽����ģʽ��
static u8 Set_Mode_AP(void)
{
	if(Snd_AT_Wifi232B2("AT+WMODE=AP\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
		wifi232b2_Reset();
		return 1;
	}
	return 0;
}

// ����STAģʽ�������ж�ģʽ��
static u8 Set_Mode_STA(void)
{
	if(Snd_AT_Wifi232B2("AT+WMODE=STA\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
		wifi232b2_Reset();
		return 1;
	}
	return 0;
}

// �Ƿ�����WIFI
static u8 Is_STA_Conected(void)
{
//	char retry=0, rcvBuf[50]="error";
//	if(Snd_AT_Wifi232B2("AT+WMODE\r", "+ok=STA\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
//		while(retry++ <= SEND_AT_RETRY_MAX)
//		{
//			if(!Snd_AT_Wifi232B2("AT+WSLK\r", rcvBuf, 1, 1)){
//				if(strcmp(rcvBuf, "+ok=Disconnected\r\n\r\n") && strcmp(rcvBuf, "+ok=RF Off\r\n\r\n") && strncmp(rcvBuf, "+ERR", 4))
//					return 1;
//			}
//		}
//	}
//	return 0;
	if(0==GPIO_ReadInputDataBit(WIFI_LINK_GPIO_PORT, WIFI_LINK_GPIO_PIN))	return 1;
	else	return 0;
}

// ����TCP�������÷�����IP���˿�
static u8 Set_Sever_Para(void)
{
	char sndBuf[50]="0";
	sprintf(sndBuf, "AT+TCPPTB=%d\r", wifi232b2Para.tcp_port);
	if(Snd_AT_Wifi232B2("AT+TCPB=on\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
		if(Snd_AT_Wifi232B2(sndBuf, "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
			sprintf(sndBuf, "AT+TCPADDB=%d.%d.%d.%d\r", wifi232b2Para.tcp_sever_ip[0], wifi232b2Para.tcp_sever_ip[1], wifi232b2Para.tcp_sever_ip[2], wifi232b2Para.tcp_sever_ip[3]);
			if(Snd_AT_Wifi232B2(sndBuf, "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
				return 1;
			}
		}
	}
	return 0;
}
// TCP���ӻ�Ͽ�;
static u8 Set_TCP_Con(u8 connect)
{
	if(connect){
		if(Snd_AT_Wifi232B2("AT+TCPDIS=on\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
			return 1;
		}
		return 0;
	}
	else{
		if(Snd_AT_Wifi232B2("AT+TCPDIS=off\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
			return 1;
		}
		return 0;
	}
}
// TCP�Ƿ�����;
static u8 Is_TCP_Linked(void)
{
	if(Snd_AT_Wifi232B2("AT+TCPLKB\r", "+ok=on\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
		return 1;
	}
	return 0;
}

// �������������ݺͼ��;
static u8 Set_Heart_Para(u8 On)
{
	char i, sndBuf[50]="0", len = strlen((char*)wifi232b2Para.Heart_Buf);
	
	if(On)
	{
		strcpy(sndBuf, "AT+HEARTDT=");
		for(i=0; i<len; i++){
			sprintf(&sndBuf[11+i*2], "%x", wifi232b2Para.Heart_Buf[i]);
		}
		sndBuf[11+len*2]='\r';
		
		if(Snd_AT_Wifi232B2("AT+HEARTEN=on\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
			if(Snd_AT_Wifi232B2(sndBuf, "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
				sprintf(sndBuf, "AT+HEARTTM=%d\r", HEART_TIM);
				if(Snd_AT_Wifi232B2(sndBuf, "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
					if(Snd_AT_Wifi232B2("AT+HEARTTP=NET\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
						return 1;
					}
				}
			}
		}
	}
	else
	{
		if(Snd_AT_Wifi232B2("AT+HEARTEN=off\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
			return 1;
		}
	}
	return 0;
}

// ������ʱ��������
static u8 Set_Timeout_EN(void)
{
	char sndBuf[50]="0";
	sprintf(sndBuf, "AT+TIMEOUTT=%d\r", TIMEOUT_RESTART);
	if(Snd_AT_Wifi232B2("AT+TIMEOUTEN=on\r", "+ok\r\n\r\n", SEND_AT_RETRY_MAX, 0)){
		if(Snd_AT_Wifi232B2(sndBuf, "+ok\r\n\r\n", 3, 0)){
			return 1;
		}
	}
	return 0;
}

// ����Ϊ�ն�ģʽ;
static u8 Set_Mode_Sta(void)
{
	char i, rcvBuf[60] = "error";
Resend:
	i=0;
	while(strcmp(rcvBuf, "+ok"))
	{
		Snd_AT_Wifi232B2("AT+WMODE\r", rcvBuf, 1, 1);
		if(!strcmp(rcvBuf, "+ok=AP\r\n\r\n"))
		{
			Snd_AT_Wifi232B2("AT+WMODE=STA\r", rcvBuf, SEND_AT_RETRY_MAX, 0);
			wifi232b2_Reset();
			Set_Mode_AT();
			goto Resend;
		}
		else if(!strcmp(rcvBuf, "+ok=STA\r\n\r\n"))
		{
			return 1;
		}

		if(++i >= SEND_AT_RETRY_MAX)
		{
			wifi232b2_Reload();
			Set_Mode_AT();
			goto Resend;
		}
	}
	return 0;
}
// ����/��ѯ�Զ���֡��������;
static u8 set_Uart_Len(void)
{
	if(Snd_AT_Wifi232B2("AT+UARTFL=4096\r", "+ok\r\n\r\n", 3, 0)){
		return 1;
	}
	return 0;
}


// ��ѯIP��ַ���������롢���ص�ַ��MAC;
static u8 Query_Wan_Para(void)
{
	char i, retry=0, *ptr1=NULL, *ptr2=NULL, rcvBuf[60]="0", tempBuf[20]="0";
	while(retry++ <= SEND_AT_RETRY_MAX)
	{
		if(!Snd_AT_Wifi232B2("AT+WANN\r", rcvBuf, 1, 1))
		{
			ptr1=strstr(rcvBuf, "+ok=");
			if(ptr1)
			{
				ptr1 += 3;
				ptr1 = strstr(rcvBuf, ",");
				if(ptr1)
				{
					ptr1 += 1;
					ptr2 = strstr(ptr1, ",");
					if(ptr2)
					{
						strncpy(tempBuf, ptr1, ptr2-ptr1);
						str2HexLong(tempBuf, wifi232b2Para.IP);
					}
					ptr2 += 1;
					ptr1 = strstr(ptr2, ",");
					if(ptr1)
					{
						memset(tempBuf, 0, sizeof(tempBuf));
						strncpy(tempBuf, ptr2, ptr1-ptr2);
						str2HexLong(tempBuf, wifi232b2Para.Mask);
					}
					ptr1 += 1;
					ptr2 = strstr(ptr1, "\r\n");
					if(ptr2)
					{
						memset(tempBuf, 0, sizeof(tempBuf));
						strncpy(tempBuf, ptr1, ptr2-ptr1);
						str2HexLong(tempBuf, wifi232b2Para.Gate);
					}
				}
			}
			memset(rcvBuf, 0, sizeof(rcvBuf));
			if(!Snd_AT_Wifi232B2("AT+WMAC\r", rcvBuf, 1, 1))
			{
				ptr1=strstr(rcvBuf, "+ok=");
				if(ptr1)
				{
					ptr1+=4;
					for(i=0; i<6; i++)
					{
						wifi232b2Para.Mac[i] = str2Hex(ptr1, 2, 16);
						ptr1+=2;
					}
					return 1;
				}
			}
		}
	}
	return 0;
}

// ����TCP�˿ں�IP��ַ;
void set_Tcp_Para(short port, short *severIP)
{
	wifi232b2Para.tcp_sever_ip[0]=(u8)(severIP[0]>>8);
	wifi232b2Para.tcp_sever_ip[1]=(u8)severIP[0];
	wifi232b2Para.tcp_sever_ip[2]=(u8)(severIP[1]>>8);
	wifi232b2Para.tcp_sever_ip[3]=(u8)severIP[1];

	wifi232b2Para.tcp_port = port;

}

// �ı�TCP IP�Ͷ˿�;
void wifi232b2_chg_ip(void)
{
	Set_Mode_AT();
	Set_Sever_Para();
	wifi232b2_Reset();
}


// ��������WIFI����ΪAPģʽ������ģ���ȵ����UDP�˿�����Ҫ���ӵ�WIFI��Ϣ;
u8 wifi232b2_Con_Wifi(void)
{
	u16 i, j;
Endless:
	i=0; j=0;	
	Set_Mode_AT();
	
	set_Uart_Len();
	
	while(!Set_Sever_Para()){			// ������IP��ַ���˿ں���Ҫ����������ǰ�����������һ��;
		delay_ms(500);
		if(++i >= 3)	{wifi232b2_Reset();	goto Endless;}
	}
	i=0;	
	while(!Set_Timeout_EN()){			// ���ó�ʱ����;
		delay_ms(500);
		if(++i >= 3)	{wifi232b2_Reset();	goto Endless;}
	}
//	i=0;
//	while(!Set_Heart_Para(1)){			// ����������;
//		delay_ms(500);
//		if(++i >= 3)	{wifi232b2_Reset();	goto Endless;}
//	}
	i=0;
	while(!Set_Mode_AP()){				// 
		delay_ms(500);
		if(++i >= 3)	{wifi232b2_Reset();	goto Endless;}
	}
	
	i=0;
	while(IS_WIFI232B2_READY()){		// �͵�ƽ��ʾ������;
		delay_ms(200);
		if(++i >= 50)	{wifi232b2_Reset();	i=0;}
	}
	i=0;
	while(!IS_WIFI232B2_READY()){		// �ȴ�����WIFI����������;
		delay_ms(200);
		if(++i >= 1500)	{Set_Mode_AT();	Set_Mode_STA();	Set_Mode_ENTM();	return 0;}
	}
	i=0;
	while(IS_WIFI232B2_READY()){		// �͵�ƽ��ʾ������;
		delay_ms(200);
		if(++i >= 50)	{wifi232b2_Reset();	return 0;}
	}
	
//AFTER:
//	if(j++ >= 3)	goto Endless;
//	Set_Mode_AT();

//	while(!Is_STA_Conected()){
//		delay_ms(500);
//		if(++i >= 3)	{wifi232b2_Reset(); goto AFTER;}
//	}
//	while(!Set_TCP_Con()){
//		delay_ms(500);
//		if(++i >= 3)	{wifi232b2_Reset();	goto AFTER;}
//	}
//	while(!Is_TCP_Linked()){
//		delay_ms(500);
//		if(++i >= 3)	{wifi232b2Para.err |= 4; return 0;}
//	}
//	wifi232b2Para.err &= ~4;
//	while(!Query_Wan_Para()){			// IP��MASK��GATEWAY��MAC;
//		delay_ms(2000);
//		if(++i >= 3)	{wifi232b2_Reset();	goto AFTER;}
//	}
//	while(!Set_Mode_ENTM()){
//		delay_ms(2000);
//		if(++i >= 3)	{wifi232b2_Reset();	goto AFTER;}
//	}

	Set_Mode_AT();
	if(Is_STA_Conected())
	{
		Set_TCP_Con(1);
		Is_TCP_Linked();
		
		Query_Wan_Para();
		wifi232b2Para.err &= ~4;
	}
	Set_Mode_ENTM();
	
	return 1;
}
// ���ģ��״̬;
void check_Wifi232b2_Status(void)
{
	u8 cnt1=0, cnt2=0;
	if(!IS_WIFI232B2_READY())
	{
		wifi232b2Para.status |= 1;
		Set_Mode_AT();
		if(Is_STA_Conected())
		{
			wifi232b2Para.connectFlag=0xAA;
			wifi232b2Para.status |= 2;

			if(Is_TCP_Linked()){
				wifi232b2Para.status |= 4;
				wifi232b2Para.err = 0;
			}
			else
			{				
				wifi232b2Para.status &= ~4;
				wifi232b2Para.err |= 4;
			}
		}
		else
		{
			wifi232b2Para.connectFlag=0;
			wifi232b2Para.status &= ~2;
			wifi232b2Para.err |= 2;
		}
		Set_Mode_ENTM();
	}
	else{
		wifi232b2Para.connectFlag=0;
		wifi232b2Para.status = 0;
		wifi232b2Para.err=7;
	}
}
#if 0
// ���ģ��״̬;
void wifi232b2_State_Machine(void)
{
	u8 cnt1=0, cnt2=0;
	if(!IS_WIFI232B2_READY())
	{
		wifi232b2Para.status |= 1;
		Set_Mode_AT();
		while(1)
		{
			if(Is_STA_Conected())
			{
				wifi232b2Para.status |= 2;
				cnt1=0; cnt2=0;
				while(1)
				{
					if(Is_TCP_Linked()){
						Query_Wan_Para();
						wifi232b2Para.status |= 4;
						wifi232b2Para.err = 0;
						break;
					}
					else
					{				
						if(++cnt1 >= 3)
						{
							cnt1 = 0;
							if(cnt2++ >= 3){
								wifi232b2Para.status &= ~4;
								wifi232b2Para.err = 4;
								break;
							}
							Set_Sever_Para();
							Set_Timeout_EN();
							Set_Heart_Para(0);
							wifi232b2_Reset();
							wifi232b2Para.resetCnt++;
							Set_Mode_AT();
							delay_ms(5000);
						}
						delay_ms(5000);
					}
				}
				break;
			}
			else
			{
				if(++cnt1 >= 3)
				{
					cnt1 = 0;
					if(cnt2++ >= 3){
						wifi232b2Para.status &= ~2;
						wifi232b2Para.status &= ~4;
						break;
					}
					Set_Mode_Sta();
					wifi232b2_Reset();
					wifi232b2Para.resetCnt++;
					Set_Mode_AT();
					delay_ms(5000);
				}
				delay_ms(5000);
			}
		}
		Set_Mode_ENTM();
	}
	else	wifi232b2Para.status &= ~1;
}
#else
// ���ģ��״̬;clearFlag--���յ�����ʱ�������ʱ���־
void wifi232b2_State_Machine()
{
	static u32 durTim=0, lastStatus=0;
	
	if(g_netDataRcvFlag){
		durTim=0;
		return;
	}
	
	check_Wifi232b2_Status();
	
	if(lastStatus != wifi232b2Para.status)
	{
		lastStatus = wifi232b2Para.status;
		durTim=0;
	}
	else if(++durTim >= 2)
	{
		durTim=0;
		switch(wifi232b2Para.status)
		{
			case 3:
				Set_Mode_AT();
				Set_Sever_Para();
				Set_Timeout_EN();
				Set_Heart_Para(0);
				wifi232b2_Reset();
			break;
			
			case 0:
			case 1:
			case 2:
			case 4:
			case 5:
				wifi232b2_Reset();
			break;
			
			case 7:
				g_infoSndFlag=0;
//				Set_Mode_AT();
//				Set_TCP_Con(0);
//				delay_ms(1000);
//				Set_TCP_Con(1);
//				Set_Mode_ENTM();
			break;
			
			
			default:
				
			break;
		}
	}
}

#endif

// ģ���ʼ��������Ϊ�ն�ģʽ;
u8 wifi232b2_Init(void)
{
	u8 i=0, retry=0;
	wifi232b2_gpio_cfg();
	wifi232b2_usart_cfg();
//	memset(&wifi232b2Para, 0, sizeof(wifi232b2Para));
	strcpy((char*)wifi232b2Para.Heart_Buf, HEART_DATA);
	while(IS_WIFI232B2_READY()){		// �͵�ƽ��ʾ������;
		delay_ms(2000);
		if(++i >= 5)	{wifi232b2Para.err = 1;	break;}
	}
	if(Set_Mode_AT()){
		set_Uart_Len();
		Set_Heart_Para(0);
		if(Set_Mode_Sta()){
			if(Set_Mode_ENTM()){
				return 1;
			}
		}
	}
	wifi232b2Para.err = 2;
	return 0;
}

void wifi232_Send(u8 *sndBuf, u16 sndLen)
{
	u16 cnt=0;
	
	while(RESET == USART_GetFlagStatus(USARTx, USART_FLAG_TC) && cnt++<60){
		vTaskDelay(20);
	}
	
	if(cnt>1)	vTaskDelay(100);		// ��һ֡���ݷ������ȴ�100ms������һ֡;
	
	wifi232b2Para.sndLen = sndLen;
	if(wifi232b2Para.sndBuf != sndBuf)
		memcpy(wifi232b2Para.sndBuf, sndBuf, sndLen);
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
}

void WIFI_Task(void)
{
	u8 sndBuf[100] = {"This is WIFI232B2 Test, Successfully connected to the server!\r\n"};
	u8 tcpIP[4]={47,102,209,103};
	u16 tcpPort=8888;
	
//	if(wifi232b2_Init())
	{
		wifi232b2_Init();
//		set_Tcp_Para(tcpPort, tcpIP);
		wifi232b2_Con_Wifi();
		
		while(!(wifi232b2Para.status & 4)){	//�ȴ����ӷ�����;
			delay_ms(1000);
			check_Wifi232b2_Status();
		}
		wifi232_Send(sndBuf, strlen((char*)sndBuf));
		while(1)
		{
//			check_Wifi232b2_Status();
			if(wifi232b2Para.rcvFlag)
			{		
				wifi232_Send(wifi232b2Para.rcvBuf, wifi232b2Para.rcvLen);
				wifi232b2Para.rcvLen=0;
				wifi232b2Para.rcvFlag=0;
			}
			delay_ms(2000);
		}
	}
}


/****************************Ӧ�ò���***********************************/



