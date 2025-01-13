#include "bspNetCom.h"
#include "string.h"
#include "mbcrc.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"
#include "modbusCB.h"
#include "bspClm920.h"
#include "bspWifi232b2.h"
#include "mbcrc.h"
#include "mb.h"
#include "DevUpDateCode.h"



u8 g_netRcvBuf[NET_RCV_LEN_MAX]={0};
u8 g_netSndBuf[NET_SND_LEN_MAX]={0};
u8 g_netSndBufOther[3][e_SysReg_Number*2+30]={0};	// 0--服务器设备识别报文、1--心跳包、2--升级回复
u8 g_netModuleChgFlag=0, g_netDataRcvFlag=0;
Net_Module_Enum g_netModule = 0;

extern struct_update g_updatePara;
extern wifi232b2_struct wifi232b2Para;
extern Com_Para_Struct Clm920_Com_Para;
extern Clm920_Para_Struct Clm920_Para;

extern QueueHandle_t Net_Snd_Queue;



void net_change_IP(void)
{
	if(g_netModule == Wifi)
	{
		set_Tcp_Para(usSysRegHoldingBuf[e_Sys_Remote1Port_Wifi], &usSysRegHoldingBuf[e_Sys_Remote1IP1]);
		wifi232b2_chg_ip();
	}
	else
	{
		bspClm920_Set_TCP_Para(usSysRegHoldingBuf[e_Sys_Remote1Port_Cat1], &usSysRegHoldingBuf[e_Sys_Remote1IP1]);
		bspClm920_Change_IP();
	}
}
// 等待模块切换到透传模式，最多三秒;
u8 net_wait_trans(void)
{
	u8 i=0, *tempMode=0;
	
	
	if(g_netModule == Wifi){
		tempMode = (u8*)&wifi232b2Para.Mode;
	}
	else{
		tempMode = (u8*)&Clm920_Para.mode;
	}
	
	while(*tempMode != Transparent){
		vTaskDelay(100);
		if(++i > 100)
			return 0;
	}
	return 1;
}


void net_snd_buf(u8 *sndBuf, u16 sndLen)
{
	u8 i=0;
	
	Net_Queue_Data_Struct tempUnit={0};
	
	if(sndLen>NET_SND_LEN_MAX)
	{
		sndLen=0;
		return;
	}
	
	if(0==net_wait_trans())
		return;
	
	if(g_netModule == Wifi){
		wifi232_Send(sndBuf, sndLen);
	}
	else{
		bspClm920_Snd_Buf(sndBuf, sndLen);
	}
}
	
// 首次连接发送自身参数;
void net_snd_info(void)
{
	u16 calCRC=0, sndLen=0;
	Net_Queue_Data_Struct queueData;
	
//	net_wait_trans();
	
	queueData.sndBuf = g_netSndBufOther[0];
	
	queueData.sndBuf[sndLen++]=0x01;
	queueData.sndBuf[sndLen++]=0x01;
	queueData.sndBuf[sndLen++]=0x03;
	queueData.sndBuf[sndLen++]=0xEA;
	queueData.sndBuf[sndLen++]=0x60;
	queueData.sndBuf[sndLen++]=0x00;
	queueData.sndBuf[sndLen++]=e_SysReg_Number*2;
	for(u8 i=0; i<e_SysReg_Number; i++)
	{
		queueData.sndBuf[sndLen++]=usSysRegHoldingBuf[i]>>8;
		queueData.sndBuf[sndLen++]=usSysRegHoldingBuf[i];
	}
	calCRC = usMBCRC16(queueData.sndBuf, sndLen);
	queueData.sndBuf[sndLen++]=calCRC;
	queueData.sndBuf[sndLen++]=calCRC>>8;
	
	memset(&queueData.sndBuf[sndLen], 0xFF, 5);
	sndLen += 5;
	
	queueData.sndLen=sndLen;
	xQueueSendToFront(Net_Snd_Queue, &queueData, 0);
}

void net_snd_heart(void)
{
	Net_Queue_Data_Struct queueData={0};
	
//	net_wait_trans();
	
	if(!g_netDataRcvFlag || (g_netDataRcvFlag & 0x04))	return;		// 升级过程中不发送;
	queueData.sndBuf = g_netSndBufOther[1];
	queueData.sndLen = strlen(HEART_DATA);
	
	memcpy(queueData.sndBuf, HEART_DATA, strlen(HEART_DATA));
	xQueueSend(Net_Snd_Queue, &queueData, 0);
}


u8 net_rcv_data_handle(u8 *rcvBuf, u16 rcvLen)
{
	u8 fucCode=0, errRsp[]={0x03, 0x01, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00};	// 校验错误
	u16 reg_num, calCRC, rcvCRC, reg_id, reg_val, sndLen=0;				//把crc高8位和低8位整合
	eMBRegisterMode eMode;
	eMBErrorCode eRegStatus;
	Net_Queue_Data_Struct queueData={0};
	
	if(rcvLen<2)	return 0;
	
//	net_wait_trans();
	
	queueData.sndBuf = g_netSndBuf;
	// 程序升级包接收;
	if(g_updatePara.updateFlag>=0x81 && g_updatePara.updateFlag<=0x82)
	{
		queueData.sndBuf = g_netSndBufOther[2];
		if(g_updatePara.updateFlag==0x81)
		{
			DevUpDateCode_Init();
			SlaveDevUpDateCode_Init();
			g_updatePara.updateFlag++;
		}
		if(g_updatePara.updateFlag==0x82)
		{
			calCRC = usMBCRC16(rcvBuf, rcvLen-2);					//计算所接收数据的CRC
			rcvCRC = rcvBuf[rcvLen-2] | (rcvBuf[rcvLen-1]<<8);		//接收到的CRC(低字节在前，高字节在后)
			if(calCRC != rcvCRC){
				// 校验错误
				calCRC = usMBCRC16(errRsp, sizeof(errRsp)-2);
				errRsp[sizeof(errRsp)-2] = (u8)calCRC;
				errRsp[sizeof(errRsp)-1] = (u8)(calCRC>>8);
				queueData.sndLen = sizeof(errRsp);
				memcpy(queueData.sndBuf, errRsp, sizeof(errRsp));
				xQueueSend(Net_Snd_Queue, &queueData, 0);
				return 0;
			}
			GetDevUpDateCodeFream_Handle(rcvBuf, queueData.sndBuf, (u32*)&queueData.sndLen, e_DevUpDateCode_ComTypeFlag_ErethNetUpDate, queueData.sndBuf);
			xQueueSend(Net_Snd_Queue, &queueData, 0);
		}
		return 4;
	}
	if(rcvBuf[0]<=3 && rcvBuf[1] == MB_ADDRESS && rcvLen >= 6)	//地址正确
	{
		calCRC = usMBCRC16(rcvBuf, rcvLen-2);					//计算所接收数据的CRC
		rcvCRC = rcvBuf[rcvLen-2] | (rcvBuf[rcvLen-1]<<8);		//接收到的CRC(低字节在前，高字节在后)
		if(calCRC==rcvCRC)		//CRC校验正确
		{
			fucCode = rcvBuf[2];
			reg_id = rcvBuf[4] | (rcvBuf[3]<<8);
			
			if(fucCode==0x03 || fucCode==0x04)
				eMode = MB_REG_READ;
			else if(fucCode==0x06 || fucCode==0x10)
				eMode = MB_REG_WRITE;
			
			if(fucCode == 0x06)
				reg_num=1;
			else
				reg_num=rcvBuf[6] | (rcvBuf[5]<<8);
			
			if(reg_num < 1 || reg_num > HOLDING_REG_NUM)
				return 0;
			
			queueData.sndBuf[sndLen++]=rcvBuf[0];
			queueData.sndBuf[sndLen++]=MB_ADDRESS;
			queueData.sndBuf[sndLen++]=fucCode;
			queueData.sndBuf[sndLen++]=rcvBuf[3];
			queueData.sndBuf[sndLen++]=rcvBuf[4];
			if(fucCode==0x03 || fucCode==0x04)
			{
				queueData.sndBuf[sndLen++]=(reg_num*2)>>8;
				queueData.sndBuf[sndLen++]=reg_num*2;
				eRegStatus = eMBRegHoldingCB(&queueData.sndBuf[sndLen], reg_id+1, reg_num, eMode);
			}
			else
			{
				eRegStatus = eMBRegHoldingCB(&rcvBuf[7], reg_id+1, reg_num, eMode);
				// 升级标识;
				reg_val = rcvBuf[8] | (rcvBuf[7]<<8);
				if(e_Sys_Upgrade == (reg_id-SYS_HOLDING_REG_BASE) && reg_val == 1)
				{
					g_updatePara.updateFlag |= 0x80;
				}
			}
			if( eRegStatus != MB_ENOERR )
			{
				queueData.sndBuf[1]=fucCode+0x80;
//				eStatus = prveMBError2Exception( eRegStatus );
			}
			else
			{
				
				if(fucCode==0x03 || fucCode==0x04)
				{
//					memcpy(&queueData.sndBuf[sndLen], rcvBuf, reg_num * 2);
					sndLen += reg_num * 2;
				}
				else if(fucCode==0x06 || fucCode==0x10)
				{
					memcpy(&queueData.sndBuf[sndLen], &rcvBuf[5], 4);
					sndLen += 4;
				}
			}
			calCRC = usMBCRC16(queueData.sndBuf, sndLen);
			queueData.sndBuf[sndLen++]=(u8)calCRC;
			queueData.sndBuf[sndLen++]=(u8)(calCRC>>8);
			
			memset(&queueData.sndBuf[sndLen], 0xFF, 5);
			sndLen += 5;
			
			queueData.sndLen=sndLen;
			xQueueSend(Net_Snd_Queue, &queueData, 0);
			return 1;
		}
	}
	if(!memcmp(rcvBuf, HEART_DATA, rcvLen))
	{
		return 2;
	}
	
	
	return 0;
}

