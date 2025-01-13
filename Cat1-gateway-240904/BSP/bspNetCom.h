#ifndef _BSPNETCOM_H_
#define _BSPNETCOM_H_

#include "stm32f10x.h"


// 发送队列参数;
typedef struct
{
//	u8 reSent;						// 重发次数;
//	u8 rcvFrame;					// 该报文应该接收帧数;
//	u8 sndBuf[WHGM5_SND_LENGTH];	// 发送BUF;
	u8 *sndBuf;
	u16 sndLen;
}Net_Queue_Data_Struct;


typedef enum{
	Cat_1,
	Wifi,
	
}Net_Module_Enum;


void net_init(void);
void net_change_IP(void);
void net_snd_info(void);
void net_snd_heart(void);
void net_snd_buf(u8 *sndBuf, u16 sndLen);
u8 net_rcv_data_handle(u8 *rcvBuf, u16 rcvLen);


#endif

