#include "bspClm920.h"
#include "string.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "timers.h"




//Gateway_Para_Struct Gateway_Para;

Clm920_Para_Struct Clm920_Para={0};
Com_Para_Struct Clm920_Com_Para={0};

//extern QueueHandle_t Net_Snd_Queue;
//extern SemaphoreHandle_t Link72_Rcv_Flag;
extern TimerHandle_t clm920_rcv_timer;
extern u8 g_infoSndFlag;


static void bspClm920_Config(void);

void bspClm920_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(CLM920_NETSTA_CLK | CLM920_RESET_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_InitStructure.GPIO_Pin = CLM920_RESET_PIN;
	GPIO_Init(CLM920_RESET_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(CLM920_RESET_PORT, CLM920_RESET_PIN);
	
	GPIO_InitStructure.GPIO_Pin = CLM920_POWER_PIN;
	GPIO_Init(CLM920_POWER_PORT, &GPIO_InitStructure);
//	GPIO_SetBits(CLM920_POWER_PORT, CLM920_POWER_PIN);
	GPIO_ResetBits(CLM920_POWER_PORT, CLM920_POWER_PIN);
	
	GPIO_InitStructure.GPIO_Pin = CLM920_PWRSWITCH_PIN;
	GPIO_Init(CLM920_POWER_PORT, &GPIO_InitStructure);
	GPIO_SetBits(CLM920_PWRSWITCH_PORT, CLM920_PWRSWITCH_PIN);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = CLM920_NETSTA_PIN;
	GPIO_Init(CLM920_NETSTA_PORT, &GPIO_InitStructure);
}


void bspClm920_Init(void)
{
	bspClm920_gpio_init();
	bspClm920_Usart_Init(115200);
//	GPIO_ResetBits(CLM920_POWER_PORT, CLM920_POWER_PIN);
//	vTaskDelay(500);
//	GPIO_SetBits(CLM920_POWER_PORT, CLM920_POWER_PIN);
	
	vTaskDelay(6000);
	bspClm920_Set_Heart_Dat((u8*)HEART_DATA, strlen(HEART_DATA));
	bspClm920_Config();
}

// 恢复出厂设置
//void bspWhgm5_Reload(void)
//{
//	GPIO_ResetBits(WHGM5_RELOAD_PORT, WHGM5_RELOAD_PIN);
//	vTaskDelay(3500);
//	GPIO_SetBits(WHGM5_RELOAD_PORT, WHGM5_RELOAD_PIN);
//}



void bspClm920_Snd_Cmd(char *cmdBuf)
{
	char cmdLen=0;
	
	cmdLen = strlen(cmdBuf);
	
	bspClm920_Snd_Buf((u8*)cmdBuf, cmdLen);
	
	vTaskDelay(200);
}

// 进入AT命令模式
static u8 Set_Mode_AT(void)
{
	char rcvBuf[50] = "a", rcvLen=0, tim=0;
	
	Clm920_Para.mode = Not_Know;
	Clm920_Com_Para.rcvLen=0;
	Clm920_Com_Para.rcvFlag=0;
	bspClm920_Snd_Cmd("+++");
	while(tim++<5)
	{
		vTaskDelay(100);
		if(Clm920_Com_Para.rcvFlag)
		{
			rcvLen = Clm920_Com_Para.rcvLen;
			memcpy(rcvBuf, Clm920_Com_Para.rcvBuf, rcvLen);
			memset(Clm920_Com_Para.rcvBuf, 0, rcvLen);
			Clm920_Com_Para.rcvLen=0;
			Clm920_Com_Para.rcvFlag=0;
			
			if(!strncmp(rcvBuf, "a", 1))
			{			
				bspClm920_Snd_Cmd("a");
				tim=0;
				while(tim++<5)
				{
					vTaskDelay(50);
					if(Clm920_Com_Para.rcvFlag)
					{
						rcvLen = Clm920_Com_Para.rcvLen;
						memcpy(rcvBuf, Clm920_Com_Para.rcvBuf, rcvLen);
						memset(Clm920_Com_Para.rcvBuf, 0, rcvLen);
						Clm920_Com_Para.rcvLen=0;
						Clm920_Com_Para.rcvFlag=0;
						if(!strncmp(rcvBuf, "+ok", 3))
						{
							Clm920_Para.mode = AT_Cmd;
							return 1;
						}
					}
				}
			}
//			else if(!strncmp(rcvBuf, "\r\n+CME ERROR:58\r\n", 17))
//			{
//				Clm920_Para.mode = AT_Cmd;
//				return 1;
//			}
		}
	}
//	Clm920_Para.mode = Transparent;
	Clm920_Para.comStatus = ComFail;
	return 0;
}


// 进入透传模式
static void Set_Mode_ENTM(void)
{
	bspClm920_Snd_Cmd("AT+ENTM\r\n");
	Clm920_Para.mode = Transparent;
}

// 软件复位
void bspClm920_Reset(void)
{
	Set_Mode_AT();
	bspClm920_Snd_Cmd("AT+REBOOT\r\n");
	Clm920_Para.mode = Not_Know;
	vTaskDelay(6000);
	Clm920_Para.mode = Transparent;
}

// 硬件复位
void bspClm920_HWReset(void)
{
	GPIO_ResetBits(CLM920_RESET_PORT, CLM920_RESET_PIN);
	vTaskDelay(500);
	GPIO_SetBits(CLM920_RESET_PORT, CLM920_RESET_PIN);
	vTaskDelay(6000);
}

void bspClm920_Set_TCP_Para(short port, short *severIP)
{
	Clm920_Para.tcp_sever_ip[0]=(u8)(severIP[0]>>8);
	Clm920_Para.tcp_sever_ip[1]=(u8)severIP[0];
	Clm920_Para.tcp_sever_ip[2]=(u8)(severIP[1]>>8);
	Clm920_Para.tcp_sever_ip[3]=(u8)severIP[1];
	
	Clm920_Para.tcp_port = port;
}

void bspClm920_Set_Heart_Dat(u8 *data, u8 len)
{
	if(len < HEART_DATA_LEN)
		memcpy(Clm920_Para.Heart_Buf, data, len);
}

void bspClm920_Change_IP(void)
{
	char i=0, sndBuf[100]="0";
	
	Set_Mode_AT();
	sprintf(sndBuf, "AT+SOCKETCFG=\"socket/host\",0,\"%d.%d.%d.%d\",%d\r\n", Clm920_Para.tcp_sever_ip[0], Clm920_Para.tcp_sever_ip[1], Clm920_Para.tcp_sever_ip[2], Clm920_Para.tcp_sever_ip[3], Clm920_Para.tcp_port);
	bspClm920_Snd_Cmd(sndBuf);							// 设置IP、端口;
	Set_Mode_ENTM();
}

void bspWhgm5_Relink(void)
{
	Set_Mode_AT();
	bspClm920_Snd_Cmd("AT+SOCKAEN=OFF\r\n");
	bspClm920_Snd_Cmd("AT+S\r\n");						// 保存并重启;
	vTaskDelay(10000);
	bspClm920_Snd_Cmd("AT+SOCKAEN=ON\r\n");
	bspClm920_Snd_Cmd("AT+S\r\n");						// 保存并重启;
	vTaskDelay(10000);
	Clm920_Para.mode = Transparent;
//	Set_Mode_ENTM();	
}

void bspClm920_Chk_Sta(void)
{
	u16 temp[5]={0};
//	if(!sndAT){
//		if(1==IS_WHGM5_READY())		Clm920_Para.status=Working;
//		else	Clm920_Para.status=NoWorking;}
		
//		if(1==IS_CLM920_LINKED()){
//			Clm920_Para.netStatus=SocketLinked;
//			Clm920_Para.status=Working;
//		}
//		else{
//			Clm920_Para.netStatus=NoService;
////			Clm920_Para.status=NoWorking;
//		}
//	}
	if(Clm920_Para.netStatus != SocketLinked){
		Set_Mode_AT();
//		bspClm920_Snd_Cmd("AT+SOCKETLIST=0\r\n");
		bspClm920_Snd_Cmd("AT+ENNAT\r\n");
		vTaskDelay(100);
//		bspClm920_Snd_Cmd("AT+CPIN?\r\n");
		bspClm920_Snd_Cmd("AT+CIMI\r\n");
		bspClm920_Snd_Cmd("AT+CSQ\r\n");
		bspClm920_Snd_Cmd("AT+EXNAT\r\n");
		Set_Mode_ENTM();
	}
	
	if(0==memcmp(Clm920_Para.iccid, temp, sizeof(Clm920_Para.iccid)))
	{
		Set_Mode_AT();
		bspClm920_Snd_Cmd("AT+ENNAT\r\n");
		vTaskDelay(100);
		bspClm920_Snd_Cmd("AT+ICCID\r\n");					// 查询卡ICCID号
		bspClm920_Snd_Cmd("AT+EXNAT\r\n");
		Set_Mode_ENTM();
	}
}


void bspClm920_Rcv_Cmd(char *rcvBuf, u16 rcvLen)
{
	const char CMD_BUF[][10]={"CSQ", "CIMI", "ICCID"};
	char *tempPtr1=NULL, *tempPtr2=NULL, tempLen=0, cmdLen=0, temp=0;
	
	tempPtr1 = strstr(rcvBuf, "AT+");
	if(tempPtr1 == NULL)
		return;
	
	Clm920_Para.comStatus = ComModuNormal;
	tempPtr1 += 3;
	
	tempPtr2 = strstr(rcvBuf, "\r\n");
	if(tempPtr2 == NULL)
	{
//		Clm920_Para.comStatus = ComErr;
		return;
	}
	tempLen = tempPtr2-tempPtr1;
	tempPtr2 += 2;
	
	for(u8 i=0; i<sizeof(CMD_BUF)/sizeof(CMD_BUF[0]); i++)
	{
		cmdLen = strlen(CMD_BUF[i]);
		if(!strncmp(CMD_BUF[i], tempPtr1, cmdLen))
		{
			switch(i)
			{
				case 0:
					Clm920_Para.rssi = str2Hex(tempPtr2+6, 2, 10);
					if(Clm920_Para.rssi > 31)
					{
						Clm920_Para.rssi = 99;
						Clm920_Para.netStatus = NoService;
					}
					else
					{
						Clm920_Para.netStatus=SocketLinked;
						Clm920_Para.rssi = 113-Clm920_Para.rssi*2;
						if(Clm920_Para.rssi > 95)	Clm920_Para.netStatus = SignalBad;
					}
				break;
				
				case 1:
					temp = str2Hex(tempPtr2+3, 4, 10);
					if(temp == 11)	Clm920_Para.netOperator=DianXin;
					else if(temp == 1 || temp == 6 || temp == 9)	Clm920_Para.netOperator=LianTong;
					else	Clm920_Para.netOperator=YiDong;
				break;
				
				case 2:
					tempPtr2 += 8;
					for(i=0; i<5; i++)
					{
						Clm920_Para.iccid[i] = str2Hex(tempPtr2+4*i, 2, 16);
						Clm920_Para.iccid[i] <<= 8;
						Clm920_Para.iccid[i] += str2Hex(tempPtr2+2+4*i, 2, 16);
					}
				break;
				
				default:
					
				break;
			}
		}
	}
}

// 配置cat1模块
static void bspClm920_Config(void)
{
	char i=0, sndBuf[100]="0", len = strlen((char*)Clm920_Para.Heart_Buf);
	
	Set_Mode_AT();
	
	sprintf(sndBuf, "AT+SOCKETCFG=\"socket/host\",0,\"%d.%d.%d.%d\",%d\r\n", Clm920_Para.tcp_sever_ip[0], Clm920_Para.tcp_sever_ip[1], Clm920_Para.tcp_sever_ip[2], Clm920_Para.tcp_sever_ip[3], Clm920_Para.tcp_port);
	bspClm920_Snd_Cmd(sndBuf);							// 设置IP、端口;
	sprintf(sndBuf, "AT+SOCKETCFG=\"socket/heart\",0,0,\"%s\",\"\"\r\n", Clm920_Para.Heart_Buf);	// 心跳间隔时间设置0即不使能;
	bspClm920_Snd_Cmd(sndBuf);							// 设置心跳包;
	
	bspClm920_Snd_Cmd("AT+ENNAT\r\n");
	vTaskDelay(100);
	
	bspClm920_Snd_Cmd("AT+CSIM=10,\"80F2EE9400\"\r\n");	// 启动优选上次卡片
//	bspClm920_Snd_Cmd("ATI\r\n");
	bspClm920_Snd_Cmd("AT+ICCID\r\n");					// 查询卡ICCID号
	
	bspClm920_Snd_Cmd("AT+EXNAT\r\n");
	
	Set_Mode_ENTM();
}


void bspClm920_ChangeOperator(void)
{
	char sndBuf[50]="0";
	u16 temp=100;

	if(Clm920_Para.netOperator == LianTong)		temp=200;
	else if(Clm920_Para.netOperator == YiDong)	temp=300;
	else if(Clm920_Para.netOperator == DianXin)	temp=100;
	
	Set_Mode_AT();
	bspClm920_Snd_Cmd("AT+ENNAT\r\n");					// 进入通用AT命令模式;
	vTaskDelay(100);
	sprintf(sndBuf, "AT+CSIM=10,\"80F2EE0%d\"\r\n", temp);
	bspClm920_Snd_Cmd(sndBuf);
	bspClm920_Snd_Cmd("AT+EXNAT\r\n");
	Set_Mode_ENTM();
	
//	bspClm920_Reset();
}


void bspClm920_State_Machine(void)
{
	static u32 durTim=0;
	static type_com_status_enum lastStatus=0;
	
	
	bspClm920_Chk_Sta();
	
	if(lastStatus != Clm920_Para.comStatus)
	{
		lastStatus = Clm920_Para.comStatus;
		if(Clm920_Para.comStatus == ComSuccess)
			durTim=0;
	}
	else if(++durTim >= 3)
	{
		durTim=0;
		
		switch((u8)Clm920_Para.comStatus)
		{
			case ComModuNormal:
				switch((u8)Clm920_Para.netStatus)
				{
					case SocketLinked:
						g_infoSndFlag=0;
						// bspClm920_ChangeOperator();
					break;
					
					case SignalBad:
						bspClm920_ChangeOperator();
					break;
					
					case NoData:
						bspClm920_ChangeOperator();
					break;
					
					case NoService:
//						bspClm920_Config();
						bspClm920_ChangeOperator();
					break;
					
					case NoNet:
						bspClm920_Config();
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
				
//				bspClm920_Reset();
				bspClm920_HWReset();
//				bspClm920_ChangeOperator();
			break;
			
			case ComErr:
				bspClm920_HWReset();
//				if(durTim >= 60)	{bspWhgm5_Reset();durTim=0;}
//				bspClm920_Reset();
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
#define	CLM920_UART_IRQHandler		USART2_IRQHandler


void bspClm920_DeInit(void)
{
	USART_Cmd(USARTx, DISABLE);
	USART_DeInit(USARTx);
}
	

void bspClm920_Usart_Init(u32 ulBaudRate)
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

#if 1
// 发送数据函数;
void bspClm920_Snd_Buf(u8 *sndBuf, u16 sndLen)
{
	u16 cnt=0;
	
	while(RESET == USART_GetFlagStatus(USARTx, USART_FLAG_TC) && cnt++<50){
		vTaskDelay(20);
	}
	
	if(cnt>1)	vTaskDelay(100);		// 上一帧数据发送完后等待100ms发送下一帧;
	
	if(Clm920_Com_Para.sndBuf != sndBuf)
		memcpy(Clm920_Com_Para.sndBuf, sndBuf, sndLen);
	Clm920_Com_Para.sndLen = sndLen;
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
void CLM920_UART_IRQHandler(void)
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
		if (Clm920_Com_Para.rcvLen < CLM920_RCV_LENGTH)
		{
			Clm920_Com_Para.rcvBuf[Clm920_Com_Para.rcvLen++] = data;
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
//			Clm920_Com_Para.rcvFlag = RCV_RECEIVED;
//			xSemaphoreGiveFromISR(Net_Snd_Queue, &xHigherPriorityTaskWoken);
		}
		/* stop timer. */
//		xTimerStopFromISR(rsp_timeout_handle, &xHigherPriorityTaskWoken);
		xTimerStartFromISR(clm920_rcv_timer, &xHigherPriorityTaskWoken);
	}
	/* TxEmpty interrupt. */
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		if (sndLen < Clm920_Com_Para.sndLen)
		{
			USARTx->DR = Clm920_Com_Para.sndBuf[sndLen++];
		}
		else
		{
			sndLen=0;
			Clm920_Com_Para.sndLen=0;
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USARTx, USART_IT_TXE);
		}
	}
}




