#include "bspDevCom.h"
#include "bspDevTag.h"
#include "devCom.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "modbusCB.h"
#include "string.h"
#include "bspDevCtl.h"
#include "DevUpDateCode.h"
#include "NVIC.h"


extern QueueHandle_t Modbus_Write_Queue;
extern struct_update g_updatePara;
extern u8 g_handChangeFlag;
extern Dev_Com_Para_Struct Dev_Com_Para;
extern s16 HoldReg_Dev[DEV_NUM_MAX][e_DevHoldReg_Number];			// 设备参数;
extern dev_para_struct Dev_Para[DEV_NUM_MAX];



QueueHandle_t Dev_Snd_Queue;
TimerHandle_t Dev_Query_Timer;
TimerHandle_t Dev_Rsp_Timeout;
SemaphoreHandle_t Dev_Snd_Flag;
SemaphoreHandle_t Dev_Rcv_Flag;			// 接收标志;
SemaphoreHandle_t Dev_Upd_Mutex;

u8 g_devChangeFlag=0;			// 设备修改参数标志，设备、手操器共有参数
u16 g_devConfigRegID=0;


void dev_data_update(void)
{
	static u16 timCount=0;
	modbus_unit_struct mbUnit={0};
	dev_unit_struct tempUnit;
	
	for(u8 i=0; i<usRegHoldingBuf[e_SysF_DevNum]; i++)
	{
		usRegHoldingBuf[DEV_HOLDING_REG_BASE+i*DEV_REG_NUM]=Dev_Para[i].ComHealth;
		if(Dev_Para[i].ComHealth < 9999)	Dev_Para[i].ComHealth++;
		
		
		if(g_modbusWriteFlag & (1<<i) || g_handChangeFlag)
		{
			if(g_modbusWriteFlag & (1<<i))
			{
//				if(!memcmp(&usRegHoldingBuf[e_Dev_SetWaterTempCon+i*DEV_REG_NUM], &HoldReg_Dev[i][e_RegBuf_SetWaterTempCon], 2*(e_RegBuf_SetCoolWaterTemp-e_RegBuf_SetWaterTempCon+1)) && 
//				!memcmp(&usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM], &HoldReg_Dev[i][e_RegBuf_AirFilterUseTime], 2*(e_RegBuf_AirFilterConfigureMaxTime-e_RegBuf_AirFilterUseTime+1)))
//				{
//					g_modbusWriteFlag &= ~(1<<i);
//					return;
//				}
				// 配置更新
				if(usRegHoldingBuf[g_devConfigRegID] == 0x0072)
				{
					// 超时，结束配置;
					if(timCount++ > 600)	goto EndConfig;
					
					if(pdTRUE == xQueueReceive(Modbus_Write_Queue, &mbUnit, 0))
					{
						if(mbUnit.startAddr==g_devConfigRegID){			// 开始配置;
							timCount=0;
							xTimerChangePeriod(Dev_Query_Timer, 100, 0);							
						}
						tempUnit.slaveAddr=DEV_SLAVE_MIN_ADDR+i;
						tempUnit.funcCode=0x06;
						tempUnit.regNum=1;
						if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
							tempUnit.startAddr=mbUnit.startAddr-DEV_HOLDING_REG_BASE-i*DEV_HOLDING_REG_NUM-8;
						else
							tempUnit.startAddr=mbUnit.startAddr-DEV_HOLDING_REG_BASE-i*DEV_HOLDING_REG_NUM-1;
					
						tempUnit.regVal[0]=mbUnit.regVal;
						xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
					}
					return;
				}
				else if(usRegHoldingBuf[g_devConfigRegID] == 0x11c3)
				{
EndConfig:
					timCount=0;
					tempUnit.slaveAddr=DEV_SLAVE_MIN_ADDR+i;
					tempUnit.funcCode=0x06;
					tempUnit.regNum=1;
					if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
						tempUnit.startAddr=g_devConfigRegID-DEV_HOLDING_REG_BASE-i*DEV_HOLDING_REG_NUM-8;
					else
						tempUnit.startAddr=g_devConfigRegID-DEV_HOLDING_REG_BASE-i*DEV_HOLDING_REG_NUM-1;
					
					tempUnit.regVal[0]=0x11c3;
					xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
					
					usRegHoldingBuf[g_devConfigRegID]=0;
					xTimerChangePeriod(Dev_Query_Timer, 1000, 0);
				}
				
				g_modbusWriteFlag &= ~(1<<i);
				if(usRegHoldingBuf[e_SysF_Dev1Type+i] != e_Air_Dehumi)	continue;
				
				// 设置电压
				
				//
				tempUnit.slaveAddr=DEV_SLAVE_MIN_ADDR+i;
				tempUnit.funcCode=0x10;
				tempUnit.regNum=e_RegBuf_SetCoolWaterTemp-e_RegBuf_SetWaterTempCon+1;
				tempUnit.startAddr=e_RegBuf_SetWaterTempCon;
				memcpy(tempUnit.regVal, &usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_SetWaterTempCon], 2*tempUnit.regNum);
				xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
				
				// 滤网
				if(memcmp(&HoldReg_Dev[i][e_RegBuf_AirFilterUseTime], &usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM], 4))
				{
					tempUnit.slaveAddr=DEV_SLAVE_MIN_ADDR+i;
					tempUnit.funcCode=0x06;
					tempUnit.regNum=1;
					if(HoldReg_Dev[i][e_RegBuf_AirFilterUseTime] != usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM])
					{
						tempUnit.startAddr=e_RegBuf_AirFilterUseTime;
						memcpy(tempUnit.regVal, &usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_AirFilterUseTime], 2*tempUnit.regNum);
						xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
					}
					if(HoldReg_Dev[i][e_RegBuf_AirFilterConfigureMaxTime] != usRegHoldingBuf[e_Dev_AirFilterConfigureMaxTime+i*DEV_REG_NUM])
					{
						tempUnit.startAddr=e_RegBuf_AirFilterConfigureMaxTime;
						memcpy(tempUnit.regVal, &usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_AirFilterConfigureMaxTime], 2*tempUnit.regNum);
						xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
					}
					
					// 同步设备参数到手操器;
//					g_devChangeFlag=1;
//					memcpy(&usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM], 4);
				}
				
//				memcpy(&usRegHoldingBuf[e_Dev_CodeIAPFlag_1+i*DEV_REG_NUM], HoldReg_Dev[i], 2*(e_RegBuf_DevAddr-e_InHoldingReg_CodeIAPFlag_1+1));
//				memcpy(&usRegHoldingBuf[e_Dev_SetSGS1+i*DEV_REG_NUM], &HoldReg_Dev[i][e_RegBuf_SetAddHumEn], 2*(e_OutHoldingReg_DevType-e_RegBuf_SetSilenceMod+1));
			}
			
//			if(usRegHoldingBuf[e_SysF_Dev1Type+i] != e_Air_Dehumi)	continue;
			
			if(g_handChangeFlag)
			{
//				if(!memcmp(&usRegHoldingBuf[e_Dev_SetONOFF+i*DEV_REG_NUM], &HoldReg_Dev[i][e_RegBuf_SetONOFF], 2*(e_RegBuf_SetMod-e_RegBuf_SetONOFF+1)) && 
//				!memcmp(&usRegHoldingBuf[e_Dev_SetAddHumEn+i*DEV_REG_NUM], &HoldReg_Dev[i][e_RegBuf_SetAddHumEn], 2*(e_RegBuf_FANControl_Speed-e_RegBuf_SetAddHumEn+1)))
//				{
//					g_handChangeFlag=0;
//					return;
//				}
				
				if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
				{
					//
					tempUnit.slaveAddr=DEV_SLAVE_MIN_ADDR+i;
					tempUnit.funcCode=0x10;
					tempUnit.regNum=e_RegBuf_SetMod-e_RegBuf_SetONOFF+1;
					tempUnit.startAddr=e_RegBuf_SetONOFF;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_SetONOFF], 2*tempUnit.regNum);
					xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
					
					//
					tempUnit.regNum=e_RegBuf_FANControl_Speed-e_RegBuf_SetAddHumEn+1;
					tempUnit.startAddr=e_RegBuf_SetAddHumEn;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_SetAddHumEn], 2*tempUnit.regNum);
					xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
					
					// 滤网
					if(g_handChangeFlag & 2)
					{
						tempUnit.regNum=2;
						tempUnit.startAddr=e_RegBuf_AirFilterUseTime;
						memcpy(tempUnit.regVal, &usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_AirFilterUseTime], 2*tempUnit.regNum);
						xQueueSendToFront(Dev_Snd_Queue, &tempUnit, 0);
					}
				}
				else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Double_Cold)
				{
					
				}
				else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Heat_Pumb)
				{
					
				}
				
				g_handChangeFlag=0;
			}
		}
		else
		{		
			if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
			{
				memcpy(&usRegHoldingBuf[e_Dev_Health_0+1+7+i*DEV_REG_NUM], HoldReg_Dev[i], sizeof(HoldReg_Dev[i]));
				// 同步设备参数到手操器;
				if(memcmp(&usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM], 4))
				{
					memcpy(&usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM], 4);
					g_devChangeFlag = 1;	// 设备参数修改标志;
				}
				if(memcmp(&usRegHoldingBuf[e_Hand_HSpeedVol_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_HSpeedVol+i*DEV_REG_NUM], 12))
				{
					g_devChangeFlag=1;
					memcpy(&usRegHoldingBuf[e_Hand_HSpeedVol_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_HSpeedVol+i*DEV_REG_NUM], 12);
				}
			}
			else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Double_Cold)
			{		
				memcpy(&usRegHoldingBuf[e_Dev_Health_0+1+i*DEV_REG_NUM], HoldReg_Dev[i], sizeof(HoldReg_Dev[i]));
				// 同步设备参数到手操器;
				if(usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM] != usRegHoldingBuf[e_Dev_HeatPumpAir_AirFilterUseTime+i*DEV_REG_NUM])
				{
					usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM]=usRegHoldingBuf[e_Dev_Double_AirFilterUseTime+i*DEV_REG_NUM];
					g_devChangeFlag = 1;	// 设备参数修改标志;
				}
//				usRegHoldingBuf[e_Hand_FilterTimAMax_0+i*HAND_REG_NUM]=usRegHoldingBuf[e_Dev_Double_AirFilterConfigureMaxTime+i*DEV_REG_NUM];
			}		
			else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Heat_Pumb)
			{
				memcpy(&usRegHoldingBuf[e_Dev_Health_0+1+i*DEV_REG_NUM], HoldReg_Dev[i], sizeof(HoldReg_Dev[i]));
				// 同步设备参数到手操器;
				if(usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM] != usRegHoldingBuf[e_Dev_HeatPumpAir_AirFilterUseTime+i*DEV_REG_NUM])
				{
					memcpy(&usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_HeatPumpAir_AirFilterUseTime+i*DEV_REG_NUM], 2);
					g_devChangeFlag = 1;	// 设备参数修改标志;
				}	
			}
			
		}
	}
}


/**
 * This function is the entry of led thread.
 */
static void dev_snd_thread(void *parameter)
{
	static u8 resendCnt=0;
	dev_unit_struct Dev_Unit;
	
	Dev_485_Config();

//	for(u16 i=0; i<2620; i++)
//	{
//		usRegHoldingBuf[i] = i;
//	}
	
	while(1)
	{
		
		if(g_updatePara.updateFlag<3)
		{
			xSemaphoreTake(Dev_Upd_Mutex, portMAX_DELAY);
			// 发送
			xQueueReceive(Dev_Snd_Queue, &Dev_Unit, portMAX_DELAY);
			
			Dev_TX_Service(&Dev_Unit);
	//		xTimerStart(Dev_Rsp_Timeout, portMAX_DELAY);
			
			// 接收
			xSemaphoreTake(Dev_Rcv_Flag, portMAX_DELAY);		
			if(Dev_Com_Para.rcvFlag == RCV_TIMEOUT)
			{
				Dev_Com_Para.rcvFlag = RCV_IDLE;
				xQueueSend(Dev_Snd_Queue, &Dev_Unit, 0);
			}
			else if(Dev_Com_Para.rcvFlag == RCV_RECEIVED)
			{
				Dev_RX_Service(Dev_Unit);
				Dev_Com_Para.rcvFlag = RCV_IDLE;
			}
			xSemaphoreGive(Dev_Upd_Mutex);
		}

		vTaskDelay(50);
	}
}
/* end */
/**
 * This function is the entry of led thread.
 */
static void dev_update_thread(void *parameter)
{
	static u8 lastFlag=0;
	while(1)
	{
//		if(lastFlag != (g_updatePara.updateFlag & 0x7F))
//		{
//			if(lastFlag==3 && g_updatePara.updateFlag==0){						// 结束;
//				
////				xSemaphoreGive(Dev_Rcv_Flag);
//				set_dev_priority(0);
//			}
//			else if(lastFlag!=3 && (g_updatePara.updateFlag & 0x7F)==3){		// 开始升级;
//				set_dev_priority(1);
//			}
//			lastFlag=(g_updatePara.updateFlag & 0x7F);
//		}
		
		if((g_updatePara.updateFlag & 0x7F)==3)
		{	
			xSemaphoreTake(Dev_Upd_Mutex, portMAX_DELAY);
			
			SlaveDevDateCodeSend(Dev_Com_Para.sndBuf, (u16*)&Dev_Com_Para.sndLen);
			Dev_Snd_Buf(Dev_Com_Para.sndBuf, Dev_Com_Para.sndLen);
//			xTimerStart(Dev_Rsp_Timeout, portMAX_DELAY);
		
			// 接收
			xSemaphoreTake(Dev_Rcv_Flag, portMAX_DELAY);

			if(Dev_Com_Para.rcvFlag == RCV_TIMEOUT)
			{
				SetSlaveDateCodeComErr(1);
				Dev_Com_Para.rcvFlag = RCV_IDLE;
			}
			else if(Dev_Com_Para.rcvFlag == RCV_RECEIVED)
			{
				SlaveDateCodeComBackHandle(Dev_Com_Para.rcvBuf);
				Dev_Com_Para.rcvFlag = RCV_IDLE;
				Dev_Com_Para.rcvLen=0;
			}
			xSemaphoreGive(Dev_Upd_Mutex);
		}
		
		vTaskDelay(50);
	}
}
/* end */
// 定时处理
void dev_para_query(TimerHandle_t xTimer)
{
	u8 sndCnt=0, queryRest=0;
	static u8 devCtlTim=0;
	dev_unit_struct devUnit;
	
	if(usSysRegHoldingBuf[e_Sys_DevType] == e_Air_Controller)
	{
		if(usRegHoldingBuf[e_SysF_Dev1Type] == e_Air_Dehumi)
			g_devConfigRegID=e_Dev_ConfigFlag;
	}
	else if(usSysRegHoldingBuf[e_Sys_DevType] == e_Dev_Gateway)
	{
		if(usRegHoldingBuf[e_SysF_Dev1Type] == e_Double_Cold)
			g_devConfigRegID=e_Dev_Double_ConfigFlag;
		else if(usRegHoldingBuf[e_SysF_Dev1Type] == e_Heat_Pumb)
			g_devConfigRegID=e_Dev_HeatPumpAir_ConfigFlag;
		else	return;
	}
	else	return;
	
	if(usRegHoldingBuf[g_devConfigRegID] == 0)		// 配置过程中，暂停查询;
	{
		for(u8 i=0; i<usRegHoldingBuf[e_SysF_DevNum]; i++)
		{
//			Dev_Para[i].ComHealth++;
					
			// 一次查询一百个;
			devUnit.slaveAddr = DEV_SLAVE_MIN_ADDR+i;
			devUnit.funcCode = 0x03;
			if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
			{
				sndCnt = e_DevHoldReg_Number/100;
				queryRest = e_DevHoldReg_Number%100;
			}
			else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Double_Cold)
			{
				sndCnt = e_DevDoubleHoldReg_Number/100;
				queryRest = e_DevDoubleHoldReg_Number%100;
			}
			else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Heat_Pumb)
			{
				sndCnt = e_HeatPumpAir_Defalut/100;
				queryRest = e_HeatPumpAir_Defalut%100;
			}
			for(u8 i=0; i<sndCnt; i++)
			{
				devUnit.startAddr = i*100;
				devUnit.regNum = 100;
				xQueueSend(Dev_Snd_Queue, &devUnit, 0);
			}
					
			devUnit.startAddr = sndCnt*100;
			devUnit.regNum = queryRest;
			xQueueSend(Dev_Snd_Queue, &devUnit, 0);
		}
		if(usSysRegHoldingBuf[e_Sys_DevType] == e_Air_Controller)
		{
			if(++devCtlTim > 5)
			{
				devCtlTim=0;
				bsp_dev_ctl();
				bsp_dev_schedule();
			}
		}
	}
	
	dev_data_update();
}

// 空气质量传感器回复数据超时处理
void dev_rsp_timeout(TimerHandle_t xTimer)
{
	Dev_Com_Para.rcvFlag = RCV_TIMEOUT;
	xSemaphoreGive(Dev_Rcv_Flag);
//	xQueueGenericSend(Dev_Rcv_Flag, NULL, 0, queueSEND_TO_BACK);
}


/**
 * This function initialize the thread of led.
 */
void devComThread_init(void)
{
	BaseType_t xReturned1, xReturned2;
	TaskHandle_t xHandle1 = NULL, xHandle2 = NULL;

	
	Dev_Snd_Queue = xQueueCreate(DEV_QUEUE_LENGTH, sizeof(dev_unit_struct));
	if(Dev_Snd_Queue == NULL){
		/* error. */
		while(1) {};
	}
	
	Dev_Snd_Flag = xSemaphoreCreateBinary();
	Dev_Rcv_Flag = xSemaphoreCreateBinary();
	if(Dev_Snd_Flag == NULL || Dev_Rcv_Flag == NULL){
		/* error. */
		while(1) {};
	}
	xSemaphoreGive(Dev_Snd_Flag);
	
	Dev_Query_Timer = xTimerCreate("devQyTimer", 1000, pdTRUE, (void *)1, dev_para_query);
	Dev_Rsp_Timeout = xTimerCreate("devRspTimeOut", 500, pdFALSE, (void*)2, dev_rsp_timeout);
	if(Dev_Query_Timer == NULL || Dev_Rsp_Timeout == NULL){
		/* error. */
		while(1) {};
	}
	xTimerStart(Dev_Query_Timer, portMAX_DELAY);
	
	Dev_Upd_Mutex = xSemaphoreCreateMutex();
	if(Dev_Upd_Mutex == NULL){
		/* error. */
		while(1) {};
	}
	xSemaphoreGive(Dev_Upd_Mutex);
	
//	Dev_Rsp_Timeout = xTimerCreate("devRspTimeOut", 500, pdFALSE, (void*)1, dev_rsp_timeout);
//	if(Dev_Rsp_Timeout == NULL){
//		/* error. */
//		while(1) {};
//	}

	xReturned1 = xTaskCreate(dev_snd_thread, DEV_SND_NAME, DEV_STACKDEPTH, 0, DEV_SND_PRIORITY, &xHandle1);
	xReturned2 = xTaskCreate(dev_update_thread, DEV_RCV_NAME, DEV_STACKDEPTH, 0, DEV_RCV_PRIORITY, &xHandle2);
//	
	if (xReturned1 != pdPASS || xReturned2 != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}
/* end */
