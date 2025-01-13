#ifndef BSPPUBLIC_H
#define BSPPUBLIC_H

#include "stm32f10x.h"


#define	NET_SND_LEN_MAX			6200
#define	NET_RCV_LEN_MAX			1050
#define	NET_SND_LEN_OTHER		100


#define HEART_TIM				30			// ����ʱ�䣬��λ��;
#define HEART_DATA				"HUIKEJI"	// ������;
#define	HEART_DATA_LEN			40			// ���������ȣ����40;
#define	TIMEOUT_RESTART			3600		// ��ʱ����ʱ�䣬��λ��;


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
