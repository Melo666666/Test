#include "bspWhgm5.h"
#include "string.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "timers.h"
//#include "queue.h"
//#include "semphr.h"



//Gateway_Para_Struct Gateway_Para;

Whgm5_Para_Struct Whgm5_Para={0};
Whgm5_Com_Para_Struct Whgm5_Com_Para={0};

//extern QueueHandle_t Net_Snd_Queue;
//extern SemaphoreHandle_t Link72_Rcv_Flag;。
//extern TimerHandle_t rsp_timeout_handle;

static void bspWhgm5_Config(void);

void bspWhgm5_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(WHGM5_RELOAD_CLK | WHGM5_RESET_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = WHGM5_RELOAD_PIN;
	GPIO_Init(WHGM5_RELOAD_PORT, &GPIO_InitStructure);
	GPIO_SetBits(WHGM5_RELOAD_PORT, WHGM5_RELOAD_PIN);
	
	GPIO_InitStructure.GPIO_Pin = WHGM5_RESET_PIN;
	GPIO_Init(WHGM5_RESET_PORT, &GPIO_InitStructure);
	GPIO_SetBits(WHGM5_RESET_PORT, WHGM5_RESET_PIN);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = WHGM5_WORK_PIN;
	GPIO_Init(WHGM5_WORK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = WHGM5_LINKA_PIN;
	GPIO_Init(WHGM5_LINKA_PORT, &GPIO_InitStructure);
}


void bspWhgm5_Init(void)
{
	bspWhgm5_gpio_init();
	bspWhgm5_Usart_Init(115200);
	bspWhgm5_Set_Heart_Dat((u8*)HEART_DATA, strlen(HEART_DATA));
	bspWhgm5_Config();
}

// 恢复出厂设置
void bspWhgm5_Reload(void)
{
	GPIO_ResetBits(WHGM5_RELOAD_PORT, WHGM5_RELOAD_PIN);
	vTaskDelay(3500);
	GPIO_SetBits(WHGM5_RELOAD_PORT, WHGM5_RELOAD_PIN);
}
// 复位
void bspWhgm5_Reset(void)
{
	GPIO_ResetBits(WHGM5_RESET_PORT, WHGM5_RESET_PIN);
	vTaskDelay(200);
	GPIO_SetBits(WHGM5_RESET_PORT, WHGM5_RESET_PIN);
}

// 异或校验
u8 CheckXOR8(u8 *buf, u16 len)
{
	u8 i, check=0;
	
	for(i=0; i<len; i++)
	{
		check ^= buf[i];
	}
	return check;
}

void bspWhgm5_Snd_Cmd(char *cmdBuf)
{
	char cmdLen=0;
	
	cmdLen = strlen(cmdBuf);
	
	bspWhgm5_Snd_Buf((u8*)cmdBuf, cmdLen);
	
	vTaskDelay(200);
}

u32 str2Hex(char *str, char len)
{
	u32 val=0;
	
	for(u8 i=0; i<len; i++)
	{
		if(str[i] <= '9' && str[i] >= '0')
		{
			val = val*10 + str[i]-'0';
		}
	}
	return val;
}

// 进入AT命令模式
static u8 Set_Mode_AT(void)
{
	char rcvBuf[50] = "a", rcvLen=0, tim=0;
	
	Whgm5_Para.mode = Not_Know;
	Whgm5_Com_Para.rcvLen=0;
	Whgm5_Com_Para.rcvFlag=0;
	bspWhgm5_Snd_Cmd("+++");
	while(tim++<30)
	{
		vTaskDelay(100);
		if(Whgm5_Com_Para.rcvFlag)
		{
			rcvLen = Whgm5_Com_Para.rcvLen;
			memcpy(rcvBuf, Whgm5_Com_Para.rcvBuf, rcvLen);
			memset(Whgm5_Com_Para.rcvBuf, 0, rcvLen);
			Whgm5_Com_Para.rcvLen=0;
			Whgm5_Com_Para.rcvFlag=0;
			
			if(!strncmp(rcvBuf, "a", 1))
			{			
				bspWhgm5_Snd_Cmd("a");
				tim=0;
				while(tim++<5)
				{
					vTaskDelay(200);
					if(Whgm5_Com_Para.rcvFlag)
					{
						rcvLen = Whgm5_Com_Para.rcvLen;
						memcpy(rcvBuf, Whgm5_Com_Para.rcvBuf, rcvLen);
						memset(Whgm5_Com_Para.rcvBuf, 0, rcvLen);
						Whgm5_Com_Para.rcvLen=0;
						Whgm5_Com_Para.rcvFlag=0;
						if(!strncmp(rcvBuf, "+ok\r\n", 5))
						{
							Whgm5_Para.mode = AT_Cmd;
							return 1;
						}
					}
				}
			}
			else if(!strncmp(rcvBuf, "\r\n+CME ERROR:58\r\n", 17))
			{
				Whgm5_Para.mode = AT_Cmd;
				return 1;
			}
		}
	}
//	Whgm5_Para.mode = Transparent;
	return 0;
}


// 进入透传模式
static void Set_Mode_ENTM(void)
{
	bspWhgm5_Snd_Cmd("AT+ENTM\r\n");
	Whgm5_Para.mode = Transparent;
}

void bspWhgm5_Set_TCP_Para(short port, short *severIP)
{
	Whgm5_Para.tcp_sever_ip[0]=(u8)(severIP[0]>>8);
	Whgm5_Para.tcp_sever_ip[1]=(u8)severIP[0];
	Whgm5_Para.tcp_sever_ip[2]=(u8)(severIP[1]>>8);
	Whgm5_Para.tcp_sever_ip[3]=(u8)severIP[1];
	
	Whgm5_Para.tcp_port = port;
}

void bspWhgm5_Set_Heart_Dat(u8 *data, u8 len)
{
	if(len < HEART_DATA_LEN)
		memcpy(Whgm5_Para.Heart_Buf, data, len);
}

void bspWhgm5_Change_IP(void)
{
	char i=0, sndBuf[50]="0";
		
	Set_Mode_AT();
	sprintf(sndBuf, "AT+SOCKA=TCP,%d.%d.%d.%d,%d\r\n", Whgm5_Para.tcp_sever_ip[0], Whgm5_Para.tcp_sever_ip[1], Whgm5_Para.tcp_sever_ip[2], Whgm5_Para.tcp_sever_ip[3], Whgm5_Para.tcp_port);
	bspWhgm5_Snd_Cmd(sndBuf);							// socketA的IP、端口;
	bspWhgm5_Snd_Cmd("AT+S\r\n");						// 保存并重启;
	vTaskDelay(10000);
	Whgm5_Para.mode = Transparent;
}

void bspWhgm5_Relink(void)
{
	Set_Mode_AT();
	bspWhgm5_Snd_Cmd("AT+SOCKAEN=OFF\r\n");
	bspWhgm5_Snd_Cmd("AT+S\r\n");						// 保存并重启;
	vTaskDelay(10000);
	bspWhgm5_Snd_Cmd("AT+SOCKAEN=ON\r\n");
	bspWhgm5_Snd_Cmd("AT+S\r\n");						// 保存并重启;
	vTaskDelay(10000);
	Whgm5_Para.mode = Transparent;
//	Set_Mode_ENTM();	
}

void bspWhgm5_Chk_Sta(void)
{
//	if(!sndAT){
//		if(1==IS_WHGM5_READY())		Whgm5_Para.status=Working;
//		else	Whgm5_Para.status=NoWorking;
		
		if(1==IS_WHGM5_LINKED()){
			Whgm5_Para.netStatus=SocketLinked;
			Whgm5_Para.status=Working;
		}
		else{
			Whgm5_Para.netStatus=NoService;
//			Whgm5_Para.status=NoWorking;
		}
//	}
	if(Whgm5_Para.comStatus != ComSuccess){
		Set_Mode_AT();
		bspWhgm5_Snd_Cmd("AT+SYSINFO?\r\n");
		bspWhgm5_Snd_Cmd("AT+CSQ?\r\n");
		bspWhgm5_Snd_Cmd("AT+SOCKALK?\r\n");
		Set_Mode_ENTM();
	}
}


void bspWhgm5_Rcv_Cmd(char *rcvBuf, u16 rcvLen)
{
	const char CMD_BUF[WHGM5_CMD_MUM][10]={"CSQ", "SYSINFO", "SOCKALK"};
	char *tempPtr1=NULL, *tempPtr2=NULL, tempLen=0, cmdLen=0, temp=0;
	
	tempPtr1 = strstr(rcvBuf, "\r\n+");
	if(tempPtr1 == NULL)
	{
		tempPtr1 = strstr(rcvBuf, "\r\nOK\r\n");				// 设置
		if(tempPtr1 == NULL)
		{
			tempPtr1 = strstr(rcvBuf, "\r\nok\r\n");
			if(tempPtr1 == NULL)
				Whgm5_Para.comStatus = ComErr;
		}
		else{
			Whgm5_Para.status = Working;
			Whgm5_Para.comStatus = ComModuNormal;
		}
		return;
	}
	Whgm5_Para.comStatus = ComModuNormal;
	tempPtr1 += 3;
	
	tempPtr2 = strstr(rcvBuf, ":");
	if(tempPtr2 == NULL)
	{
		Whgm5_Para.comStatus = ComErr;
		return;
	}
	tempLen = tempPtr2-tempPtr1;
	tempPtr2 += 1;
	
	for(u8 i=0; i<WHGM5_CMD_MUM; i++)
	{
		cmdLen = strlen(CMD_BUF[i]);
		if(!strncmp(CMD_BUF[i], tempPtr1, cmdLen))
		{
			switch(i)
			{
				case 0:
					Whgm5_Para.rssi = str2Hex(tempPtr2+1, 2);
					if(Whgm5_Para.rssi > 31)
					{
						Whgm5_Para.rssi = 99;
						Whgm5_Para.netStatus = NoService;
					}
					else
					{
						Whgm5_Para.rssi = 113-Whgm5_Para.rssi*2;
//						Whgm5_Para.status = SocketLinked;
					}
				break;
				
				case 1:
					temp = str2Hex(tempPtr2, 1);
					if(temp != 2)	Whgm5_Para.netStatus = NoService;
				break;
				
				case 2:
					if(!strncmp(tempPtr2, "Connected", 9))	Whgm5_Para.netStatus = SocketLinked;
					else	Whgm5_Para.netStatus = NoService;
				break;
				
				default:
					
				break;
			}
		}
	}
	
}


void bspWhgm5_Config(void)
{
	char i=0, sndBuf[50]="0", len = strlen((char*)Whgm5_Para.Heart_Buf);
	
	while(Whgm5_Para.mode != AT_Cmd && i <=5)
	{
		i++;
		Set_Mode_AT();
	}
	
	bspWhgm5_Snd_Cmd("AT+E=ON\r\n");					// 开回显;
	bspWhgm5_Snd_Cmd("AT+SOCKAEN=ON\r\n");				// socketA使能;
	sprintf(sndBuf, "AT+SOCKA=TCP,%d.%d.%d.%d,%d\r\n", Whgm5_Para.tcp_sever_ip[0], Whgm5_Para.tcp_sever_ip[1], Whgm5_Para.tcp_sever_ip[2], Whgm5_Para.tcp_sever_ip[3], Whgm5_Para.tcp_port);
	bspWhgm5_Snd_Cmd(sndBuf);							// socketA的IP、端口;
	bspWhgm5_Snd_Cmd("AT+SOCKASL=LONG\r\n");			// socketA为长连接;
//	bspWhgm5_Snd_Cmd("AT+SHORTATM=10\r\n");				// socketA短连接超时时间;
	bspWhgm5_Snd_Cmd("AT+SOCKBEN=OFF\r\n");				// 禁止socketB;
	bspWhgm5_Snd_Cmd("AT+SOCKRSTIM=10\r\n");			// 重连时间间隔;
	bspWhgm5_Snd_Cmd("AT+SOCKRSNUM=60\r\n");			// 重连次数;
	bspWhgm5_Snd_Cmd("AT+UARTFL=4096\r\n");				// 重连次数;
	bspWhgm5_Snd_Cmd("AT+HEARTEN=ON\r\n");				// 心跳包使能;
	bspWhgm5_Snd_Cmd("AT+HEARTTM=30\r\n");				// 心跳包间隔时间;
	bspWhgm5_Snd_Cmd("AT+HEARTTP=NET\r\n");				// 心跳包发送对象;
	bspWhgm5_Snd_Cmd("AT+HEARTSORT=USER\r\n");			// 心跳包数据类型，自定义;
	memset(sndBuf, 0, sizeof(sndBuf));
	strcpy(sndBuf, "AT+HEARTDT=");
	for(u8 i=0; i<len; i++){
		sprintf(&sndBuf[11+i*2], "%x", Whgm5_Para.Heart_Buf[i]);
	}													// 心跳包数据内容;
	sndBuf[11+len*2]='\r';
	sndBuf[11+len*2+1]='\n';
	bspWhgm5_Snd_Cmd(sndBuf);
	bspWhgm5_Snd_Cmd("AT+S\r\n");						// 保存并重启;
	vTaskDelay(10000);
	Whgm5_Para.mode = Transparent;
}



void bspWhgm5_State_Machine(void)
{
	static u32 durTim=0;
	static type_com_status_enum lastStatus=0;
	
	
	bspWhgm5_Chk_Sta();
	
	if(lastStatus != Whgm5_Para.comStatus)
	{
		lastStatus = Whgm5_Para.comStatus;
		if(Whgm5_Para.comStatus == ComSuccess)
			durTim=0;
	}
	else if(++durTim >= 3)
	{
		durTim=0;
		
		switch((u8)Whgm5_Para.comStatus)
		{
			case ComModuNormal:
				switch((u8)Whgm5_Para.netStatus)
				{
					case SocketLinked:
//						bspWhgm5_Relink();
					break;
					
					case NoService:
						bspWhgm5_Config();
					break;
					
					case NoNet:
						bspWhgm5_Config();
					break;
					
					default:
				
					break;
				}
			break;
				
			case ComFail:
//				if(durTim >= 60)
//				{
//					bspWhgm5_Reload();
//					vTaskDelay(3000);
//					bspWhgm5_Config();
//					durTim=0;
//				}
//				bspWhgm5_Reload();
//				vTaskDelay(3000);
//				bspWhgm5_Config();
				bspWhgm5_Reset();
			break;
			
			case ComErr:
//				if(durTim >= 60)	{bspWhgm5_Reset();durTim=0;}
				bspWhgm5_Reset();
			break;
			
			default:
				
			break;
		}
	}
//	vTaskDelay(1000);
//	durTim++;
}
/*********************************************************************/
// 串口4-USART1
#define	USARTx						USART2
#define	USART_CLK					RCC_APB1Periph_USART2 
#define	USART_APBxClkCmd			RCC_APB1PeriphClockCmd
#define	USART_BAUDRATE				115200

// USART GPIO 引脚宏定义
#define	USART_GPIO_CLK				(RCC_APB2Periph_GPIOA)
#define	USART_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd
    
#define	USART_TX_GPIO_PORT			GPIOA
#define	USART_TX_GPIO_PIN			GPIO_Pin_2

#define	USART_RX_GPIO_PORT			GPIOA
#define	USART_RX_GPIO_PIN			GPIO_Pin_3

//#define	probe4_485_USART_IRQ		UART5_IRQn 
#define	WHGM5_UART_IRQHandler		USART2_IRQHandler


void bspWhgm5_DeInit(void)
{
	USART_Cmd(USARTx, DISABLE);
	USART_DeInit(USARTx);
}
	

void bspWhgm5_Usart_Init(u32 ulBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	
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
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	// 使能串口
	USART_Cmd(USARTx, ENABLE);
	USART_ClearFlag(USARTx, USART_FLAG_TC); // 清除发送溢出标志位
}

extern u8 g_netSndBuf[];
#if 1
// 发送数据函数;
void bspWhgm5_Snd_Buf(u8 *sndBuf, u16 sndLen)
{
	if(Whgm5_Com_Para.sndBuf != sndBuf)
		memcpy(Whgm5_Com_Para.sndBuf, sndBuf, sndLen);
	Whgm5_Com_Para.sndLen = sndLen;
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
}
#else

void bspWhgm5_Snd_Buf(u8 *sndBuf, u16 sndLen)
{
	for(u16 i=0; i<sndLen; i++)
	{
		USART_SendData(USARTx, sndBuf[i]);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		{
		}
	}
}

#endif


// [Function] 串口3中断处理函数
void WHGM5_UART_IRQHandler(void)
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
		if (Whgm5_Com_Para.rcvLen < WHGM5_RCV_LENGTH)
		{
			Whgm5_Com_Para.rcvBuf[Whgm5_Com_Para.rcvLen++] = data;
		}
	}
	else if (USART_GetITStatus(USARTx, USART_IT_IDLE) != RESET)
	{
		/* clear idle. */
		data = USARTx->SR;
		data = USARTx->DR;
		/* set state and wake up thread. */
//		if (Link72_Com_Para.rcvFlag == RCV_IDLE)
		{
			Whgm5_Com_Para.rcvFlag = RCV_RECEIVED;
//			xSemaphoreGiveFromISR(Net_Snd_Queue, &xHigherPriorityTaskWoken);
		}
		/* stop timer. */
//		xTimerStopFromISR(rsp_timeout_handle, &xHigherPriorityTaskWoken);
	}
	/* TxEmpty interrupt. */
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		if (sndLen < Whgm5_Com_Para.sndLen)
		{
			USARTx->DR = Whgm5_Com_Para.sndBuf[sndLen++];
		}
		else
		{
			sndLen=0;
			Whgm5_Com_Para.sndLen=0;
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USARTx, USART_IT_TXE);
		}
	}
}




