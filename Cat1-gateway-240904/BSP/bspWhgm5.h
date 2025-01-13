#ifndef _BSPWHGM5_H
#define _BSPWHGM5_H

#include "stm32f10x.h"


#define WHGM5_RELOAD_PORT  		GPIOC
#define WHGM5_RELOAD_PIN   		GPIO_Pin_9
#define WHGM5_RELOAD_CLK    	RCC_APB2Periph_GPIOC

#define WHGM5_RESET_PORT  		GPIOC
#define WHGM5_RESET_PIN   		GPIO_Pin_8
#define WHGM5_RESET_CLK    		RCC_APB2Periph_GPIOC

#define WHGM5_WORK_PORT  		GPIOC
#define WHGM5_WORK_PIN   		GPIO_Pin_1
#define WHGM5_WORK_CLK    		RCC_APB2Periph_GPIOC

#define WHGM5_LINKA_PORT  		GPIOC
#define WHGM5_LINKA_PIN   		GPIO_Pin_0
#define WHGM5_LINKA_CLK    		RCC_APB2Periph_GPIOC

#define	IS_WHGM5_READY()		GPIO_ReadInputDataBit(WHGM5_WORK_PORT, WHGM5_WORK_PIN)
#define	IS_WHGM5_LINKED()		GPIO_ReadInputDataBit(WHGM5_LINKA_PORT, WHGM5_LINKA_PIN)

#define WHGM5_RCV_LENGTH		(1050)
#define WHGM5_SND_LENGTH		(5700)
#define TRY_MAX_CNT				(5)

#define HEART_TIM				30			// 心跳时间，单位秒;
#define HEART_DATA				"HUIKEJI"	// 心跳包;
#define	HEART_DATA_LEN			40			// 心跳包长度，最大40;
#define	TIMEOUT_RESTART			3600		// 超时重启时间，单位秒;


enum rcv_state
{
	RCV_IDLE = 0,
	RCV_RECEIVED,
	RCV_TIMEOUT,
};

#pragma pack(1)

typedef struct{
	u8 errCnt;
	u8 rcvFlag;
	u16 rcvLen;
	u8 *rcvBuf;
	u16 sndLen;
	u8 *sndBuf;
	
}Whgm5_Com_Para_Struct;

typedef struct{
	u8 status;
	u16 comErr;
	u16 fatherAddr;
	u16 shortAddr;
	u8 mac[8];
	u8 modbusAddr;
	u8 onOff;
	u8 lastOnOff;
	u8 rssi;
	u16 temp;
	u16 humi;
	u8 mode;
	u16 heatSetPoint;
	u16 coldSetPoint;
	
}Panel_Para_Struct;

//typedef struct{
//	u8 err;
//	u8 cfgProcess;						// 配置过程标志;
//	u8 curPanelIdx;						// 正在通信的面板序号;
//	u8 panelNum;						// 在线面板个数;
//	u8 cfgPanelNum;						// 配置的面板个数;
//	u8 mode;
//	u8 netOnOff;
//	u16 time;	
//	
//	Panel_Para_Struct panel_para[PANEL_NUM_MAX];
//	
//	u8 cfgPanelMac[PANEL_NUM_MAX][8];	// 配置的面板MAC;
//}Gateway_Para_Struct;	


typedef enum{
	Transparent,
	Not_Know,
	AT_Cmd,
	
}Whgm5_Mode_Enum;


typedef enum{

	NoWorking,
	Working,
}type_state_enum;

typedef enum{

	NoNet,
	NoService,
	
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
//	u8 sockaLink;					// socket连接状态;
	u8 rssi;
	Whgm5_Mode_Enum	mode;
	u8 Heart_Buf[HEART_DATA_LEN];	// 心跳包，十六进制;	
	u8 tcp_sever_ip[4];
	u16 tcp_port;					// TCP服务器IP、端口;
	
//	u8 devType;
//	u8 mac[8];
//	u8 channel;
//	u16 PANID;
//	u16 shortAddr;
//	u8 extPANID[8];
//	u8 networkKey[16];
	
}Whgm5_Para_Struct;

#pragma pack()

/**************************命令码*******************************/
//const char WHGM5_CMD_NAME[][10]=
//{"Z", "S" ,"CLEAR", "E", "ENTM", "WKMOD", "RSTIM", "CSQ", "SYSINFO", 
//"SOCKA", "SOCKEN", "SOCKRSNUM", "SOCKRSTIM", "HEARTEN", "HEARTTP", "HEARTDT", "HEARTTM", "HEARTSORT", "Z", "Z", "Z", };
typedef enum{
	Z,					// 重启模组
	S,					// 保存配置及并重启
	CLEAR,				// 恢复出厂并重启
	E,					// 查询/设置回显使能
	ENTM,				// 退出配置模式
	WKMOD,				// 查询/设置工作模式
	RSTIM,				// 查询/设置设备无数据重启时间
	CSQ,				// 查询信号强度
	SYSINFO,			// 查询连接制式
	SOCKA,				// 查询/设置socketA参数
	SOCKEN,				// 查询/设置socketA使能
	SOCKRSNUM,			// 查询/设置socket最大重连次数
	SOCKRSTIM,			// 查询/设置socket重连时间间隔
	HEARTEN,			// 查询/设置心跳包使能
	HEARTTP,			// 查询/设置心跳包发送方式
	HEARTDT,			// 查询/设置心跳包数据
	HEARTTM,			// 查询/设置心跳包发送间隔
	HEARTSORT,			// 查询/设置心跳包数据类型
	
	
	WHGM5_CMD_MUM,
	
}type_cmd_enum;


#pragma pack()


//void bspWhgm5_Usart_Init(void);
void bspWhgm5_Init(void);
void bspWhgm5_State_Machine(void);
void bspWhgm5_Rcv_Cmd(char *rcvBuf, u16 rcvLen);

void bspWhgm5_Set_TCP_Para(short port, short *severIP);
void bspWhgm5_Set_Heart_Dat(u8 *data, u8 len);

void bspWhgm5_DeInit(void);
void bspWhgm5_Snd_Buf(u8 *sndBuf, u16 sndLen);

void bspWhgm5_Change_IP(void);
void bspWhgm5_Usart_Init(u32 ulBaudRate);
#endif /* LINK72_H */
