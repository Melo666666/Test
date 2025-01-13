#include "string.h"
#include "wifi232.h"
#include "bspWifi232b2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "modbusCB.h"
#include "bsp_InternalFlash.h"
#include "bspNetCom.h"
#include "bspClm920.h"
#include "DevUpDateCode.h"


TimerHandle_t wifi232_rcv_timer;			// 接收数据结束判断;
TaskHandle_t wifiHandle1 = NULL, wifiHandle2 = NULL;

extern struct_update g_updatePara;
extern wifi232b2_struct wifi232b2Para;
extern u8 g_netModuleChgFlag, g_netDataRcvFlag;


/* end */
// 定时处理
static void rcv_End_Callback(TimerHandle_t xTimer)
{
	wifi232b2Para.rcvFlag = RCV_RECEIVED;
}


static void Wifi232_Rcv_Thread(void *parameter)
{
	u8 ret=0;
	static u32 noDatTim=0, noDatMin=0, taskDelay=10, timCount=0, checkPeriod=5;
	
	vTaskDelay(1000);			// Link72启动等待时间;

	wifi232b2_Init();
	set_Tcp_Para(usSysRegHoldingBuf[e_Sys_Remote1Port_Wifi], &usSysRegHoldingBuf[e_Sys_Remote1IP1]);
	
//	wifi232b2_Con_Wifi();
//	check_Wifi232b2_Status();
//	if(wifi232b2Para.status == 0x07)
//	{
//		wifi232b2Para.connectFlag=0xAA;
//		g_modbusWriteFlag |= (1<<31);	// 写flash;
//	}
	while(1)
	{
		while(wifi232b2Para.connectFlag != 0xAA)
		{
			if(wifi232b2Para.keyPressFlag)
			{
				wifi232b2Para.keyPressFlag=0;
				wifi232b2_Con_Wifi();
				check_Wifi232b2_Status();
			}
			
			if(noDatTim++ >= 150*1000/taskDelay)
			{
				noDatTim=0;
				g_netModuleChgFlag=1;
			}
			
			if(noDatTim % (30*1000/taskDelay) == 0)
			{
				check_Wifi232b2_Status();
			}
			if(wifi232b2Para.connectFlag == 0xAA)
				g_modbusWriteFlag |= (1<<31);		// 写flash;
			
			vTaskDelay(taskDelay);
		}
		
//		if(wifi232b2_key_scan())
//		{
//			wifi232b2_Con_Wifi();
//		}
		
		if(wifi232b2Para.Mode==Transparent)
		{
			if(wifi232b2Para.rcvFlag)
			{
				ret=net_rcv_data_handle(wifi232b2Para.rcvBuf, wifi232b2Para.rcvLen);
				wifi232b2Para.rcvLen=0;
				wifi232b2Para.rcvFlag=0;
				memset(wifi232b2Para.rcvBuf, 0, 100);
				if(ret)
				{
					wifi232b2Para.status=0x07;
					g_netDataRcvFlag=ret;
					wifi232b2_State_Machine();
					noDatTim=0;
					noDatMin=0;
				}
			}
			else
			{
				if(noDatTim++ >= checkPeriod*1000/taskDelay)
				{
					noDatTim=0;
					g_netDataRcvFlag=0;
					wifi232b2_State_Machine();
//					if(wifi232b2Para.status == 7)
						checkPeriod=60;
					
					if(noDatMin++ >= 5)
					{
						noDatMin=0;
//						if(wifi232b2Para.status!=0x07)
							g_netModuleChgFlag=1;
					}
				}
			}
		}
		vTaskDelay(taskDelay);
	}
}


static void Wifi232_Chk_Thread(void *parameter)
{
	vTaskDelay(20000);
	while(1)
	{
//		if(wifi232b2Para.Mode==Transparent)
//			check_Wifi232b2_Status();
		vTaskDelay(30000);
	}
}


void Wifi232_Thread_Init(void)
{
	BaseType_t xReturned1, xReturned2;
	
	wifi232_rcv_timer = xTimerCreate("rcv_timer", 5, pdFALSE, (void*)1, rcv_End_Callback);
	
	xReturned1 = xTaskCreate(Wifi232_Rcv_Thread, WIFI232_RCV_NAME, 1*WIFI232_STACKDEPTH, 0, WIFI232_RCV_PRIORITY, &wifiHandle1);
//	xReturned2 = xTaskCreate(Wifi232_Chk_Thread, WIFI232_CHK_NAME, 1*WIFI232_STACKDEPTH, 0, WIFI232_CHK_PRIORITY, &wifiHandle2);
	
//	if (xReturned1 != pdPASS || xReturned2 != pdPASS)
	if (xReturned1 != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}

