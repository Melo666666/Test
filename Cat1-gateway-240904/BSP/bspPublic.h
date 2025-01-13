#ifndef BSPPUBLIC_H
#define BSPPUBLIC_H

#include "stm32f10x.h"


#define	NET_SND_LEN_MAX			6200
#define	NET_RCV_LEN_MAX			1050
#define	NET_SND_LEN_OTHER		100


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

typedef enum{
	Transparent,
	Not_Know,
	AT_Cmd,
	
}Device_Mode_Enum;


typedef struct{
	u8 errCnt;
	u8 rcvFlag;
	u16 rcvLen;
	u8 *rcvBuf;
	u16 sndLen;
	u8 *sndBuf;
	
}Com_Para_Struct;



u8 CheckXOR8(u8 *buf, u16 len);
u8 str2Hex(char *str, u8 strLen, u8 base);
void str2HexLong(char *str, u8 *convBuf);


#endif /* LINK72_H */
