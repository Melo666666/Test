#include "netThread.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bspWifi232b2.h"
#include "modbusCB.h"
#include "clm920.h"
#include "wifi232.h"
#include "string.h"
#include "mbcrc.h"
#include "bspClm920.h"
#include "bspNetCom.h"


extern Com_Para_Struct Clm920_Com_Para;
extern Clm920_Para_Struct Clm920_Para;
extern wifi232b2_struct wifi232b2Para;
extern u8 g_netSndBuf[];
extern u8 g_netRcvBuf[];
extern u8 g_netModuleChgFlag;
extern Net_Module_Enum g_netModule;

extern TaskHandle_t clm920Handle1, clm920Handle2, wifiHandle1, wifiHandle2;

QueueHandle_t Net_Snd_Queue;
Net_Queue_Data_Struct Net_Queue_Data;

u8 g_infoSndFlag=0;			// 第一次连上服务器主动发送报文;

static void net_init(void)
{
	Wifi232_Thread_Init();
	Clm920_Thread_Init();
	
	wifi232b2Para.sndBuf = g_netSndBuf;
	wifi232b2Para.rcvBuf = g_netRcvBuf;
		
	Clm920_Com_Para.sndBuf = g_netSndBuf;
	Clm920_Com_Para.rcvBuf = g_netRcvBuf;
	
	g_netModule=Cat_1;//Cat_1;
	if(g_netModule == Wifi)
	{	
		vTaskSuspend(clm920Handle1);
		vTaskSuspend(clm920Handle2);
	}
	else
	{
		vTaskSuspend(wifiHandle1);
	}
}

static void net_change_module(void)
{
	if(g_netModuleChgFlag)
	{
		g_netModuleChgFlag=0;
		if(g_netModule == Cat_1)
		{
			bspClm920_DeInit();
			wifi232b2Para.rcvFlag=0;
			wifi232b2Para.rcvLen=0;
			wifi232b2Para.status=0;
			wifi232b2Para.Mode=Transparent;
			
			wifi232b2_usart_cfg();
			vTaskSuspend(clm920Handle1);
			vTaskSuspend(clm920Handle2);
			vTaskResume(wifiHandle1);
			
			g_netModule = Wifi;
		}
		else if(g_netModule == Wifi)
		{
			Wifi232b2_DeInit();
			Clm920_Com_Para.rcvFlag=0;
			Clm920_Com_Para.rcvLen=0;
			Clm920_Para.mode=Transparent;
			Clm920_Para.status=NoWorking;
			Clm920_Para.netStatus=NoNet;
			
			bspClm920_Usart_Init(115200);
			vTaskSuspend(wifiHandle1);
//			vTaskDelete(wifiHandle2);
			
			vTaskResume(clm920Handle1);
			vTaskResume(clm920Handle2);
			
			g_netModule = Cat_1;
		}
//		net_init();
		g_infoSndFlag=0;
		
		g_modbusWriteFlag |= (1<<31);	// дflash;
	}
}

/**
 * This function is the entry of led thread.
 */
static void net_thread_entry(void *parameter)
{
	static u16 delayTim=20, heartTim=0;
	
	vTaskDelay(500);
	net_init();
	while(1)
	{
//		if(Whgm5_Para.sockaLink != lastLinkSta)
//		{
//			if(Whgm5_Para.sockaLink)
//			{
//				if(Whgm5_Para.mode == Transparent)
//				{
//					net_snd_info();
//					lastLinkSta=Whgm5_Para.sockaLink;
//				}
//			}
//			else
//				lastLinkSta=Whgm5_Para.sockaLink;
//			
//		}

		if(wifi232b2_key_scan())
		{
			wifi232b2Para.keyPressFlag=1;
			wifi232b2Para.connectFlag=0;
			wifi232b2Para.status=0;
			g_modbusWriteFlag |= (1<<31);	// 写flash;
			if(g_netModule == Cat_1)
				g_netModuleChgFlag=1;
		}
		
//		while(!infoSndFlag){
		if(!g_infoSndFlag){
			if((g_netModule == Wifi && wifi232b2Para.status == 7)
				|| (g_netModule == Cat_1 && Clm920_Para.netStatus==SocketLinked)){
//				vTaskDelay(500);	// 等待退出AT命令模式;
				net_snd_info();
				g_infoSndFlag=1;
			}
//			vTaskDelay(100);
		}
		if(heartTim++ > HEART_TIM*1000/delayTim){
			net_snd_heart();
			heartTim=0;
		}
		
		net_change_module();
		
		if(pdTRUE == xQueueReceive(Net_Snd_Queue, &Net_Queue_Data, 0))
			net_snd_buf(Net_Queue_Data.sndBuf, Net_Queue_Data.sndLen);
			
		vTaskDelay(delayTim);
	}
}
/* end */

/**
 * This function initialize the thread of led.
 */
void netThread_init(void)
{
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;

	memset(&Net_Queue_Data, 0, sizeof(Net_Queue_Data));
	Net_Snd_Queue = xQueueCreate(5, sizeof(Net_Queue_Data));
	if(Net_Snd_Queue == NULL){
		/* error. */
		while(1) {};
	}
	
	xReturned = xTaskCreate(net_thread_entry, NET_NAME, NET_STACKDEPTH, 0, NET_PRIORITY, &xHandle);

	if (xReturned != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}
/* end */
