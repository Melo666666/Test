#include "string.h"
#include "clm920.h"
#include "bspClm920.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "modbusCB.h"
#include "bsp_InternalFlash.h"
#include "bspNetCom.h"


TaskHandle_t clm920Handle1 = NULL, clm920Handle2 = NULL;

TimerHandle_t clm920_rcv_timer;			// 接收数据结束判断;

extern Com_Para_Struct Clm920_Com_Para;
extern Clm920_Para_Struct Clm920_Para;
extern u8 g_netModuleChgFlag, g_netDataRcvFlag;

u8 temptemp[50]={0};

/* end */
// 定时处理
static void rcv_End_Callback(TimerHandle_t xTimer)
{
	Clm920_Com_Para.rcvFlag = RCV_RECEIVED;
}

static void Clm920_Rcv_Thread(void *parameter)
{
	static u32 noDatTim=0, noDatMin=0, taskDelay=10, checkPeriod=60;
	short ret=0, port=9100, tcpAddr[2]={0x662F, 0x67D1};
//	read_flash();
	
	vTaskDelay(500);
	while(1)
	{
		if(Clm920_Com_Para.rcvFlag == RCV_RECEIVED)
		{
			memcpy(temptemp, Clm920_Com_Para.rcvBuf, sizeof(temptemp));
			if(Clm920_Para.mode == AT_Cmd || Clm920_Para.mode == Transparent)
			{
				if(Clm920_Para.mode == AT_Cmd)
				{
					bspClm920_Rcv_Cmd((char*)Clm920_Com_Para.rcvBuf, Clm920_Com_Para.rcvLen);
				}
				else if(Clm920_Para.mode == Transparent)
				{
//					Clm920_Para.netStatus = SocketLinked;
					ret=net_rcv_data_handle(Clm920_Com_Para.rcvBuf, Clm920_Com_Para.rcvLen);
//					if(memcmp(Clm920_Com_Para.rcvBuf, "WH-GM5", 6))		// 开机默认会发送WH-GM5
					if(ret)
					{
						noDatTim=0;
						noDatMin=0;
						g_netDataRcvFlag=ret;
						Clm920_Para.comStatus = ComSuccess;
					}
				}
				Clm920_Com_Para.rcvFlag = RCV_IDLE;
				Clm920_Com_Para.rcvLen = 0;
				Clm920_Com_Para.errCnt = 0;
				memset(Clm920_Com_Para.rcvBuf, 0, 100);
			}
		}
		else
		{
			if(noDatTim++ >= checkPeriod*1000/taskDelay)
			{
				Clm920_Para.comStatus = ComModuNormal;
				Clm920_Para.netStatus = NoData;
				Clm920_Para.status = NoWorking;
				noDatTim=0;
				
//				bspClm920_State_Machine();
//				if(Clm920_Para.netStatus == SocketLinked)
//					checkPeriod=60;
//				else
//					checkPeriod=30;
				
				if(noDatMin++ >= 300/checkPeriod)		// 五分钟;
				{
					noDatMin=0;
					g_netDataRcvFlag=0;
//					vTaskDelay(2000);
//					if(Clm920_Para.netStatus != SocketLinked)
						g_netModuleChgFlag=1;
				}
			}
		}
		vTaskDelay(taskDelay);
	}
}

static void Clm920_Chk_Thread(void *parameter)
{
	static u32 taskDelay=5000;
		
	vTaskDelay(3000);			// Link72启动等待时间;
	bspClm920_Set_TCP_Para(usSysRegHoldingBuf[e_Sys_Remote1Port_Cat1], &usSysRegHoldingBuf[e_Sys_Remote1IP1]);
//	bspClm920_Set_TCP_Para(port, tcpAddr);
	bspClm920_Init();
	while(1)
	{
		vTaskDelay(taskDelay);
		bspClm920_State_Machine();
		if(Clm920_Para.comStatus == ComSuccess)
			taskDelay=60000;
		else
			taskDelay=30000;
	}
}


void Clm920_Thread_Init(void)
{
	BaseType_t xReturned1, xReturned2, xReturned3;

	
//	Link72_Snd_Flag = xSemaphoreCreateBinary();
//	Link72_Rcv_Flag = xSemaphoreCreateBinary();
//	xSemaphoreGive(Link72_Snd_Flag);
	
	clm920_rcv_timer  = xTimerCreate("rcv_timer", 5, pdFALSE, (void*)1, rcv_End_Callback);
//	Clm920_status_check	= xTimerCreate("status_timer", 1000, pdFALSE, (void*)1, Clm920_State_Machine);
//	if(Clm920_status_check == NULL){
//		/* error. */
//		while(1) {};
//	}
	
	xReturned1 = xTaskCreate(Clm920_Rcv_Thread, CLM920_RCV_NAME, 1*CLM920_STACKDEPTH, 0, CLM920_RCV_PRIORITY, &clm920Handle1);
//	xReturned2 = xTaskCreate(Clm920_Snd_Thread, CLM920_SND_NAME, 1*CLM920_STACKDEPTH, 0, CLM920_SND_PRIORITY, &xHandle2);
	xReturned3 = xTaskCreate(Clm920_Chk_Thread, CLM920_CHK_NAME, 1*CLM920_STACKDEPTH, 0, CLM920_CHK_PRIORITY, &clm920Handle2);
	
	if (xReturned1 != pdPASS || xReturned3 != pdPASS)
//	if (xReturned1 != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}

