#include "airSensor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "bspAirCom.h"
#include "bspDevCom.h"
#include "bspDevTag.h"
#include "modbusCB.h"
#include "string.h"
#include "bspRtc.h"


u8 g_handChangeFlag=0;		// bit0--手操器修改参数标志，bit1--滤网参数修改;

extern u8 g_devChangeFlag;
extern Air_Com_Para_Struct Air_Com_Para;
extern air_para_struct Air_Para[AIR_NUM_MAX];
extern s16 HoldReg_Air[AIR_NUM_MAX][e_AirHoldReg_Number];
extern s16 HoldReg_Hand[HAND_NUM_MAX][e_HandHoldReg_Number];
extern s16 HoldReg_Dev[DEV_NUM_MAX][e_DevHoldReg_Number];			// 设备参数;



QueueHandle_t Air_Snd_Queue;
TimerHandle_t Air_Query_Timer;
TimerHandle_t Air_Rsp_Timeout;
SemaphoreHandle_t Air_Snd_Flag;
SemaphoreHandle_t Air_Rcv_Flag;			// 接收标志;

/*
extern QueueHandle_t Dev_Snd_Queue;
void air_ctl_dev(void)
{
	air_unit_struct devUnit={0};
	static s16 HoldReg_Last[HAND_NUM_MAX][e_HandHoldReg_Number]={0};
	
	devUnit.slaveAddr = DEV_SLAVE_MIN_ADDR;
	devUnit.funcCode = 0x06;
	
	for(u8 i=0; i<HAND_NUM_MAX; i++)
	{
		if(memcmp(HoldReg_Hand[i], HoldReg_Last[i], 2*(e_Hand_AirVol-e_Hand_Switch+1)))
		{
			if(HoldReg_Hand[i][e_Hand_Switch] != HoldReg_Last[i][e_Hand_Switch])
			{
				devUnit.startAddr = e_RegBuf_SetONOFF;
				devUnit.regVal[0] = HoldReg_Hand[i][e_Hand_Switch];
				xQueueSendToFront(Dev_Snd_Queue, &devUnit, portMAX_DELAY);
			}
			if(HoldReg_Hand[e_Hand_SetMode] != HoldReg_Last[e_Hand_SetMode])
			{
				devUnit.startAddr = e_RegBuf_SetMod;
				devUnit.regVal[0] = HoldReg_Hand[i][e_Hand_Switch];
				xQueueSendToFront(Dev_Snd_Queue, &devUnit, portMAX_DELAY);
			}
			if(HoldReg_Hand[e_Hand_SetHumi] != HoldReg_Last[e_Hand_SetHumi])
			{
//				devUnit.startAddr = e_Hand_SetHumi;
//				devUnit.regVal[0] = HoldReg_Hand[i][e_Hand_SetHumi];
//				xQueueSendToFront(Dev_Snd_Queue, &devUnit, portMAX_DELAY);
			}
			if(HoldReg_Hand[e_Hand_AirVol] != HoldReg_Last[e_Hand_AirVol])
			{
//				devUnit.startAddr = e_Hand_AirVol;
//				devUnit.regVal[0] = HoldReg_Hand[i][e_Hand_AirVol];
//				xQueueSendToFront(Dev_Snd_Queue, &devUnit, portMAX_DELAY);
			}
		}
		// 排程;
		else if(memcmp(&HoldReg_Hand[i][e_Hand_SchelduEn], &HoldReg_Last[i][e_Hand_SchelduEn], 2*(e_Hand_Sunday2-e_Hand_Monday1+1)))
		{
			
		}
		
	}
}
*/


void air_data_update(void)
{
	air_unit_struct tempUnit;
	
	// 空气质量传感器
	for(u8 i=0; i<usRegHoldingBuf[e_SysF_AirSensorNum]; i++)
	{
		usRegHoldingBuf[AIR_HOLDING_REG_BASE+i*AIR_REG_NUM]=Air_Para[i].AirComHealth;
		if(Air_Para[i].AirComHealth < 9999)	Air_Para[i].AirComHealth++;
		memcpy(&usRegHoldingBuf[AIR_HOLDING_REG_BASE+i*AIR_REG_NUM+1], HoldReg_Air[i], 2*(e_Air_VOCValueOve-e_Air_Temp+1));
		if(memcmp(&usRegHoldingBuf[e_Air_PM25Offset_0+i*AIR_REG_NUM], &HoldReg_Air[i][e_Air_PM25Offset], 2*(e_Air_VOCOffset-e_Air_PM25Offset+1)))
		{
			tempUnit.slaveAddr=AIR_SLAVE_MIN_ADDR+i;
			tempUnit.funcCode=0x10;
			tempUnit.regNum=e_Air_VOCOffset-e_Air_PM25Offset+1;
			tempUnit.startAddr=e_Air_PM25Offset;
			memcpy(tempUnit.regVal, &usRegHoldingBuf[i*AIR_REG_NUM+e_Air_PM25Offset_0], 2*tempUnit.regNum);
		
			xQueueSend(Air_Snd_Queue, &tempUnit, 0);
		}
//		if(memcmp(&usRegHoldingBuf[e_Air_TemOffset_0+i*AIR_REG_NUM], &HoldReg_Air[i][e_Air_TemOffset], 2*(e_Air_HumOffset-e_Air_TemOffset+1)))
//		{
//			tempUnit.slaveAddr=AIR_SLAVE_MIN_ADDR+i;
//			tempUnit.funcCode=0x10;
//			tempUnit.regNum=e_Air_HumOffset-e_Air_TemOffset+1;
//			tempUnit.startAddr=e_Air_TemOffset;
//			memcpy(tempUnit.regVal, &usRegHoldingBuf[i*AIR_REG_NUM+e_Air_TemOffset_0], 2*tempUnit.regNum);
//		
//			xQueueSend(Air_Snd_Queue, &tempUnit, 0);
//		}
	}
	
	// 手操器
	for(u8 i=0; i<usRegHoldingBuf[e_SysF_HandOptrNum]; i++)
	{
		usRegHoldingBuf[e_Hand_Health_0+i*HAND_REG_NUM]=Air_Para[i].HandComHealth;
		if(Air_Para[i].HandComHealth < 9999)	Air_Para[i].HandComHealth++;
		if(memcmp(&usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_Switch_0], &HoldReg_Hand[i][e_Hand_Switch], 2*(e_Hand_HumOffset-e_Hand_Switch+1)))
		{
			if(g_modbusWriteFlag & (1<<(8+i)))				// 上位机修改参数
			{
				tempUnit.slaveAddr=HAND_SLAVE_MIN_ADDR+i;
				tempUnit.funcCode=0x06;
				tempUnit.regNum=1;
				tempUnit.startAddr=e_Hand_Switch;
				// 手操器
				if(usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_Switch_0] != HoldReg_Hand[i][e_Hand_Switch])
				{
					tempUnit.startAddr=e_Hand_Switch;
					tempUnit.regVal[0]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_Switch_0];
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				if(usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SetMode_0] != HoldReg_Hand[i][e_Hand_SetMode])
				{
					tempUnit.startAddr=e_Hand_SetMode;
					tempUnit.regVal[0]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SetMode_0];
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				if(usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SetTemp_0] != HoldReg_Hand[i][e_Hand_SetTemp])
				{
					tempUnit.startAddr=e_Hand_SetTemp;
					tempUnit.regVal[0]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SetTemp_0];
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				if(usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SetHumi_0] != HoldReg_Hand[i][e_Hand_SetHumi])
				{
					tempUnit.startAddr=e_Hand_SetHumi;
					tempUnit.regVal[0]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SetHumi_0];
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				if(usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_AirVol_0] != HoldReg_Hand[i][e_Hand_AirVol])
				{
					if(!usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_AirVol_0])	usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_AirVol_0]=1;
					tempUnit.startAddr=e_Hand_AirVol;
					tempUnit.regVal[0]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_AirVol_0];
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				if(usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0] != HoldReg_Hand[i][e_Hand_FilterTim])
				{
					tempUnit.startAddr=e_Hand_FilterTim;
					tempUnit.regVal[0]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0];
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
					// 同步参数到设备;
					g_handChangeFlag|=2;
					if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
						usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_AirFilterUseTime]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0];
					else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Double_Cold)
						usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_Double_AirFilterUseTime]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0];
					else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Heat_Pumb)
						usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_HeatPumpAir_AirFilterUseTime]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0];
				}
				if(usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTimAMax_0] != HoldReg_Hand[i][e_Hand_FilterTimAMax])
				{
					tempUnit.startAddr=e_Hand_FilterTimAMax;
					tempUnit.regVal[0]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTimAMax_0];
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
					// 同步参数到设备;
					g_handChangeFlag|=2;
					if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
						usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_AirFilterConfigureMaxTime]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTimAMax_0];
					else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Double_Cold)
						usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_Double_AirFilterConfigureMaxTime]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTimAMax_0];
					else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Heat_Pumb)
						usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_HeatPumpAir_AirFilterConfigureMaxTime]=usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTimAMax_0];
				}
				if(memcmp(&usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SchelduEn_0], &HoldReg_Hand[i][e_Hand_SchelduEn], 2*(e_Hand_Sunday2-e_Hand_SchelduEn+1)))
				{
					tempUnit.funcCode=0x10;
					tempUnit.regNum=e_Hand_Sunday2-e_Hand_SchelduEn+1;
					tempUnit.startAddr=e_Hand_SchelduEn;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_SchelduEn_0], 2*tempUnit.regNum);
				
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				if(memcmp(&usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_HSpeedVol_0], &HoldReg_Hand[i][e_Hand_HSpeedVol], 2*(e_Hand_AirAirTemp-e_Hand_HSpeedVol+1)))
				{
					tempUnit.funcCode=0x10;
					tempUnit.regNum=e_Hand_AirAirTemp-e_Hand_HSpeedVol+1;
					tempUnit.startAddr=e_Hand_HSpeedVol;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_HSpeedVol_0], 2*tempUnit.regNum);
				
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
					// 同步参数到设备;
					if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
						memcpy(&usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_HSpeedVol], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_HSpeedVol_0], 2*tempUnit.regNum);
				}
				if(memcmp(&usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_PM25Offset_0], &HoldReg_Hand[i][e_Hand_PM25Offset], 2*(e_Hand_VOCOffset-e_Hand_PM25Offset+1)))
				{
					tempUnit.funcCode=0x10;
					tempUnit.regNum=e_Hand_VOCOffset-e_Hand_PM25Offset+1;
					tempUnit.startAddr=e_Hand_PM25Offset;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_PM25Offset_0], 2*tempUnit.regNum);
				
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
					// 同步参数到空气质量传感器;
					memcpy(&usRegHoldingBuf[i*AIR_REG_NUM+e_Air_PM25Offset_0], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_PM25Offset_0], 2*tempUnit.regNum);
				}
				if(memcmp(&usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_TemOffset_0], &HoldReg_Hand[i][e_Hand_TemOffset], 2*(e_Hand_HumOffset-e_Hand_TemOffset+1)))
				{
					tempUnit.funcCode=0x10;
					tempUnit.regNum=e_Hand_HumOffset-e_Hand_TemOffset+1;
					tempUnit.startAddr=e_Hand_TemOffset;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_TemOffset_0], 2*tempUnit.regNum);
				
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				
//				tempUnit.slaveAddr=HAND_SLAVE_MIN_ADDR+i;
//				tempUnit.funcCode=0x10;
//				tempUnit.regNum=e_Hand_Sunday2-e_Hand_Switch+1;
//				tempUnit.startAddr=e_Hand_Switch;
//				memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_Switch_0], 2*(e_Hand_Sunday2-e_Hand_Switch+1));
//				xQueueSendToFront(Air_Snd_Queue, &tempUnit, 0);
				
				g_handChangeFlag|=1;
			}
			else if(g_devChangeFlag)						// 设备修改;
			{
				if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi)
				{
//					memcpy(&usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM], 4);
//					memcpy(&usRegHoldingBuf[e_Hand_HSpeedVol_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_HSpeedVol+i*DEV_REG_NUM], 12);
					
					tempUnit.slaveAddr=HAND_SLAVE_MIN_ADDR+i;
					tempUnit.funcCode=0x10;
					tempUnit.regNum=e_Hand_FilterTimAMax-e_Hand_FilterTim+1;
					tempUnit.startAddr=e_Hand_FilterTim;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0], 2*tempUnit.regNum);
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
					
					tempUnit.funcCode=0x10;
					tempUnit.regNum=e_Hand_AirAirTemp-e_Hand_HSpeedVol+1;
					tempUnit.startAddr=e_Hand_HSpeedVol;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_HSpeedVol_0], 2*tempUnit.regNum);
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
				else
				{
//					if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Double_Cold)
//					{
//						usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM]=usRegHoldingBuf[e_Dev_Double_AirFilterUseTime+i*DEV_REG_NUM];
//	//					usRegHoldingBuf[e_Hand_FilterTimAMax_0+i*HAND_REG_NUM]=usRegHoldingBuf[e_Dev_Double_AirFilterConfigureMaxTime+i*DEV_REG_NUM];
//					}
//					else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Heat_Pumb)
//					{
//						memcpy(&usRegHoldingBuf[e_Hand_FilterTim_0+i*HAND_REG_NUM], &usRegHoldingBuf[e_Dev_HeatPumpAir_AirFilterUseTime+i*DEV_REG_NUM], 2);
//					}
					
					tempUnit.slaveAddr=HAND_SLAVE_MIN_ADDR+i;
					tempUnit.funcCode=0x06;
//					tempUnit.regNum=e_Hand_FilterTimAMax-e_Hand_FilterTim+1;
					tempUnit.startAddr=e_Hand_FilterTim;
					memcpy(tempUnit.regVal, &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0], 2*tempUnit.regNum);
					xQueueSend(Air_Snd_Queue, &tempUnit, 0);
				}
			}
			else											// 手操器修改参数
			{
				memcpy(&usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_Switch_0], &HoldReg_Hand[i][e_Hand_Switch], 2*(e_Hand_HumOffset-e_Hand_Switch+1));
				g_handChangeFlag|=1;
				
				// 同步参数到设备;
				if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi && memcmp(&usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_HSpeedVol], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_HSpeedVol_0], 2*(e_Hand_AirAirTemp_0-e_Hand_HSpeedVol_0+1)))
				{
					memcpy(&usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_HSpeedVol], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_HSpeedVol_0], 2*(e_Dev_AirAirTemp-e_Dev_HSpeedVol+1));
				}
				if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Air_Dehumi && memcmp(&usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_AirFilterUseTime], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0], 2*(e_Hand_FilterTimAMax_0-e_Hand_FilterTim_0+1)))
				{
					memcpy(&usRegHoldingBuf[e_Dev_AirFilterUseTime+i*DEV_REG_NUM], &usRegHoldingBuf[e_Hand_FilterTim_0+i*DEV_REG_NUM], 4);
					g_handChangeFlag|=2;
				}
				else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Double_Cold)
				{
					if(usRegHoldingBuf[e_Dev_Double_AirFilterUseTime+i*DEV_REG_NUM] != usRegHoldingBuf[e_Hand_FilterTim_0+i*DEV_REG_NUM]
					|| usRegHoldingBuf[e_Dev_Double_AirFilterConfigureMaxTime+i*DEV_REG_NUM] != usRegHoldingBuf[e_Hand_FilterTimAMax_0+i*DEV_REG_NUM])
					{
						usRegHoldingBuf[e_Dev_Double_AirFilterUseTime+i*DEV_REG_NUM]=usRegHoldingBuf[e_Hand_FilterTim_0+i*DEV_REG_NUM];
						usRegHoldingBuf[e_Dev_Double_AirFilterConfigureMaxTime+i*DEV_REG_NUM]=usRegHoldingBuf[e_Hand_FilterTimAMax_0+i*DEV_REG_NUM];
						g_handChangeFlag|=2;
					}
				}
				else if(usRegHoldingBuf[e_SysF_Dev1Type+i] == e_Heat_Pumb && memcmp(&usRegHoldingBuf[i*DEV_REG_NUM+e_Dev_HeatPumpAir_AirFilterUseTime], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_FilterTim_0], 2*(e_Hand_FilterTimAMax_0-e_Hand_FilterTim_0+1)))
				{
					memcpy(&usRegHoldingBuf[e_Dev_HeatPumpAir_AirFilterUseTime+i*DEV_REG_NUM], &usRegHoldingBuf[e_Hand_FilterTim_0+i*DEV_REG_NUM], 4);
					g_handChangeFlag|=2;
				}
				
				// 同步参数到空气质量传感器;
				if(memcmp(&usRegHoldingBuf[i*AIR_REG_NUM+e_Air_PM25Offset_0], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_PM25Offset_0], 2*(e_Hand_VOCOffset_0-e_Hand_PM25Offset_0+1)))
//				|| memcmp(&usRegHoldingBuf[i*AIR_REG_NUM+e_Air_TemOffset_0], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_PM25Offset_0], 2*(e_Hand_HumOffset_0-e_Hand_TemOffset_0+1)))
				{
					memcpy(&usRegHoldingBuf[i*AIR_REG_NUM+e_Air_PM25Offset_0], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_PM25Offset_0], 2*(e_Hand_VOCOffset_0-e_Hand_PM25Offset_0+1));
//					memcpy(&usRegHoldingBuf[i*AIR_REG_NUM+e_Air_TemOffset_0], &usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_TemOffset_0], 2*(e_Hand_HumOffset_0-e_Hand_TemOffset_0+1));
				}
			}
		}
		else
		{
			if(g_modbusWriteFlag & (1<<(8+i)))	g_modbusWriteFlag &= ~(1<<(8+i));			// 上位机修改参数
			else if(g_devChangeFlag)			g_devChangeFlag	= 0;						// 设备修改;
		}
		
		memcpy(&usRegHoldingBuf[i*HAND_REG_NUM+e_Hand_DispTemp_0], &HoldReg_Hand[i][e_Hand_DispTemp], 2*(e_Hand_DispHumi-e_Hand_DispTemp+1));
	}

}



/**
 * This function is the entry of led thread.
 */
static void air_snd_thread(void *parameter)
{
	air_unit_struct Air_Unit;
	
	Air_485_Config();

	while(1)
	{
		xQueueReceive(Air_Snd_Queue, &Air_Unit, portMAX_DELAY);
		
		Air_TX_Service(&Air_Unit);
		xTimerStart(Air_Rsp_Timeout, portMAX_DELAY);
		
		xSemaphoreTake(Air_Rcv_Flag, portMAX_DELAY);
		
		if(Air_Com_Para.rcvFlag == RCV_TIMEOUT)
		{
			Air_Com_Para.rcvFlag = RCV_IDLE;
//			xQueueSend(Air_Snd_Queue, &Air_Unit, 0);
		}
		else if(Air_Com_Para.rcvFlag == RCV_RECEIVED)
		{
			Air_RX_Service(Air_Unit);
			Air_Com_Para.rcvFlag = RCV_IDLE;
		}
//		air_data_update();
		
		vTaskDelay(50);
	}
}
/* end */
/**
 * This function is the entry of led thread.
 */
static void air_rcv_thread(void *parameter)
{

}
/* end */
// 定时处理
void air_para_query(TimerHandle_t xTimer)
{
	u8 i=0, j=0;
	air_unit_struct airUnit={0};
	
	if(usSysRegHoldingBuf[e_Sys_DevType] != e_Air_Controller)	return;

	for(u8 i=0; i<usRegHoldingBuf[e_SysF_HandOptrNum]; i++)
	{
		// 查询手操器参数
		airUnit.slaveAddr = HAND_SLAVE_MIN_ADDR+i;
		airUnit.funcCode = 0x03;
		airUnit.startAddr = e_Hand_DispTemp;
		airUnit.regNum = e_Hand_Week-e_Hand_DispTemp+1;
		xQueueSend(Air_Snd_Queue, &airUnit, 0);
		
		// 写空气质量参数给手操器
		airUnit.funcCode = 0x10;
		airUnit.startAddr = e_Hand_PM25;
		airUnit.regNum = 4;
		memcpy(airUnit.regVal, &HoldReg_Air[i][e_Air_PM2_5], 8);
		xQueueSend(Air_Snd_Queue, &airUnit, 0);
		
		// 时间同步
		if(memcmp(&usSysRegHoldingBuf[e_Sys_SysTim_Year], &HoldReg_Hand[i][e_Hand_Year], 10)){
			if(HoldReg_Hand[i][e_Hand_TimeFlag]){		// 手操器修改时间
				memcpy(&usSysRegHoldingBuf[e_Sys_SysTim_Year], &HoldReg_Hand[i][e_Hand_Year], 12);
				RTC_Set(usSysRegHoldingBuf[e_Sys_SysTim_Year], usSysRegHoldingBuf[e_Sys_SysTim_Month], usSysRegHoldingBuf[e_Sys_SysTim_Day], usSysRegHoldingBuf[e_Sys_SysTim_Hour], usSysRegHoldingBuf[e_Sys_SysTim_Min], usSysRegHoldingBuf[e_Sys_SysTim_Sec]);
				
				airUnit.funcCode = 0x06;
				airUnit.startAddr = e_Hand_TimeFlag;
				airUnit.regVal[0] = 0;
				xQueueSendToFront(Air_Snd_Queue, &airUnit, 0);
			}
			else if(memcmp(&usSysRegHoldingBuf[e_Sys_SysTim_Year], &HoldReg_Hand[i][e_Hand_Year], 8)	// 年月日时不一样
				|| usSysRegHoldingBuf[e_Sys_SysTim_Year]-HoldReg_Hand[i][e_Hand_Year] >= 3				// 差超过三分钟
				|| usSysRegHoldingBuf[e_Sys_SysTim_Year]-HoldReg_Hand[i][e_Hand_Year] <= -3){
				airUnit.funcCode = 0x10;
				airUnit.startAddr = e_Hand_Year;
				airUnit.regNum = 7;
				memcpy(airUnit.regVal, &usSysRegHoldingBuf[e_Sys_SysTim_Year], 14);
				xQueueSendToFront(Air_Snd_Queue, &airUnit, 0);
			}
		}
		// 设备参数
		airUnit.funcCode = 0x10;
		airUnit.startAddr = e_Hand_OutTemp;
		airUnit.regNum = e_Hand_Err3_2-e_Hand_OutTemp+1;
		if(usRegHoldingBuf[e_SysF_Dev1Type] == e_Air_Dehumi)
		{
			airUnit.regVal[j++]=HoldReg_Dev[0][e_OutHoldingReg_ORT_T4];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_RegBuf_InterMachFanInTemp_T11];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_RegBuf_InterMachBeforeReheatTemp];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_OutHoldingReg_PQY];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_OutHoldingReg_XQP];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_OutHoldingReg_XQT_T1];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_OutHoldingReg_PQT_T2];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_OutHoldingReg_NowFre];
			memcpy(&airUnit.regVal[j], &HoldReg_Dev[0][e_RegBuf_Err1], 8);
		}
		else if(usRegHoldingBuf[e_SysF_Dev1Type] == e_Heat_Pumb)
		{
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAirOut_ORT_T4];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAir_InterMachFanInTemp_T11];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAir_InterMachBeforeReheatTemp];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAirOut_PQY];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAirOut_XQP];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAirOut_XQT_T1];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAirOut_PQT_T2];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_HeatPumpAirOut_NowFre];
			memcpy(&airUnit.regVal[j], &HoldReg_Dev[0][e_HeatPumpAir_Err1], 8);
		}
		else if(usRegHoldingBuf[e_SysF_Dev1Type] == e_Double_Cold)
		{
			airUnit.regVal[j++]=0;
			airUnit.regVal[j++]=HoldReg_Dev[0][e_Double_HuangjingTemp];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_Double_EvaporatorTemp];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_Double_HPS_Value];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_Double_LPS_Value];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_Double_sgt_Temp];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_Double_dgt_Temp];
			airUnit.regVal[j++]=HoldReg_Dev[0][e_Double_InverterFreNow];
			memcpy(&airUnit.regVal[j], &HoldReg_Dev[0][e_Double_NeedReset], 6);
			j+=3;
			airUnit.regVal[j++]=0;
		}
		
		xQueueSend(Air_Snd_Queue, &airUnit, 0);
//		Air_Para[i].HandComHealth++;
	}
	
	// 空气质量传感器参数
	for(u8 i=0; i<usRegHoldingBuf[e_SysF_AirSensorNum]; i++)
	{
		airUnit.slaveAddr = AIR_SLAVE_MIN_ADDR+i;
		airUnit.funcCode = 0x03;
		airUnit.startAddr = e_Air_Temp;
		airUnit.regNum = e_AirHoldReg_Number;
		xQueueSend(Air_Snd_Queue, &airUnit, 0);
		
//		Air_Para[i].AirComHealth++;
	}
	
	air_data_update();
}

// 空气质量传感器回复数据超时处理
void air_rsp_timeOut(TimerHandle_t xTimer)
{
	Air_Com_Para.rcvFlag = RCV_TIMEOUT;
	xSemaphoreGive(Air_Rcv_Flag);
//	xQueueGenericSend(Air_Rcv_Flag, NULL, 0, queueSEND_TO_BACK);
}


/**
 * This function initialize the thread of led.
 */
void airSensorThread_init(void)
{
	BaseType_t xReturned1, xReturned2;
	TaskHandle_t xHandle1 = NULL, xHandle2 = NULL;

	
	Air_Snd_Queue = xQueueCreate(AIR_QUEUE_LENGTH, sizeof(air_unit_struct));
	if(Air_Snd_Queue == NULL){
		/* error. */
		while(1) {};
	}
	
	Air_Rcv_Flag = xSemaphoreCreateBinary();
	Air_Snd_Flag = xSemaphoreCreateBinary();
	if(Air_Rcv_Flag == NULL || Air_Snd_Flag == NULL){
		/* error. */
		while(1) {};
	}
	xSemaphoreGive(Air_Snd_Flag);
	
	Air_Query_Timer = xTimerCreate("AirQyTimer", 1500, pdTRUE, (void *)3, air_para_query);
	Air_Rsp_Timeout = xTimerCreate("AirRspTimeOut", 500, pdFALSE, (void*)4, air_rsp_timeOut);
	if(Air_Query_Timer == NULL || Air_Rsp_Timeout == NULL){
		/* error. */
		while(1) {};
	}
	xTimerStart(Air_Query_Timer, portMAX_DELAY);
	
//	Air_Rsp_Timeout = xTimerCreate("AirRspTimeOut", 500, pdFALSE, (void*)2, air_rsp_timeOut);
//	if(Air_Rsp_Timeout == NULL){
//		/* error. */
//		while(1) {};
//	}
	
	xReturned1 = xTaskCreate(air_snd_thread, AIRSENSOR_SND_NAME, AIRSENSOR_STACKDEPTH, 0, AIRSENSOR_SND_PRIORITY, &xHandle1);
//	xReturned2 = xTaskCreate(air_rcv_thread, AIRSENSOR_RCV_NAME, AIRSENSOR_STACKDEPTH, 0, AIRSENSOR_RCV_PRIORITY, &xHandle2);
//	
	if (xReturned1 != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}
/* end */
