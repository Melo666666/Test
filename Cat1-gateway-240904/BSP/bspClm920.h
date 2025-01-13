#ifndef _BSPCLM920_H
#define _BSPCLM920_H

#include "stm32f10x.h"
#include "bspPublic.h"


#define CLM920_NETSTA_PORT  	GPIOB
#define CLM920_NETSTA_PIN   	GPIO_Pin_12
#define CLM920_NETSTA_CLK    	RCC_APB2Periph_GPIOB

#define CLM920_RESET_PORT  		GPIOC
#define CLM920_RESET_PIN   		GPIO_Pin_8
#define CLM920_RESET_CLK    	RCC_APB2Periph_GPIOC

//#define CLM920_POWER_PORT  		GPIOC
//#define CLM920_POWER_PIN   		GPIO_Pin_1
//#define CLM920_POWER_CLK    	RCC_APB2Periph_GPIOC
#define CLM920_POWER_PORT  		GPIOB
#define CLM920_POWER_PIN   		GPIO_Pin_13
#define CLM920_POWER_CLK    	RCC_APB2Periph_GPIOC


#define CLM920_PWRSWITCH_PORT  	GPIOC
#define CLM920_PWRSWITCH_PIN   	GPIO_Pin_0
#define CLM920_PWRSWITCH_CLK    RCC_APB2Periph_GPIOC

#define	IS_CLM920_READY()		GPIO_ReadInputDataBit(CLM920_WORK_PORT, CLM920_WORK_PIN)
#define	IS_CLM920_LINKED()		GPIO_ReadInputDataBit(CLM920_NETSTA_PORT, CLM920_NETSTA_PIN)

#define CLM920_RCV_LENGTH		NET_RCV_LEN_MAX
#define TRY_MAX_CNT				(5)


#pragma pack(1)

typedef enum{
	LianTong,
	YiDong,
	DianXin,
	
}Clm920_Operater_Enum;


typedef enum{

	NoWorking,
	Working,
}type_state_enum;

typedef enum{

	NoNet,
	NoService,
	NoData,
	SignalBad,
	
	SocketLinked,
	
}type_net_status_enum;

typedef enum{
	
	ComFail,		// 与模块通讯失败，模块异常;
	ComModuNormal,	// 与模块通讯正常，没有网络数据;
	ComErr,			// 数据传输错误;
	ComSuccess,		// 通讯正常;
	
}type_com_status_enum;


typedef struct{
	type_state_enum status;
	type_net_status_enum netStatus;
	type_com_status_enum comStatus;
	Clm920_Operater_Enum netOperator;
//	u8 sockaLink;					// socket连接状态;
	u8 rssi;
	Device_Mode_Enum	mode;
	u8 Heart_Buf[HEART_DATA_LEN];	// 心跳包，十六进制;	
	u8 tcp_sever_ip[4];
	u16 tcp_port;					// TCP服务器IP、端口;
	u16 iccid[5];
	
	
}Clm920_Para_Struct;

#pragma pack()



//void bspCLM920_Usart_Init(void);
void bspClm920_Init(void);
void bspClm920_State_Machine(void);
void bspClm920_Rcv_Cmd(char *rcvBuf, u16 rcvLen);

void bspClm920_Set_TCP_Para(short port, short *severIP);
void bspClm920_Set_Heart_Dat(u8 *data, u8 len);

void bspClm920_DeInit(void);
void bspClm920_Snd_Buf(u8 *sndBuf, u16 sndLen);

void bspClm920_Change_IP(void);
void bspClm920_ChangeOperator(void);
void bspClm920_Usart_Init(u32 ulBaudRate);
#endif /* LINK72_H */
