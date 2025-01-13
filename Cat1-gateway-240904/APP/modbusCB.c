
/* ----------------------- Modbus includes ----------------------------------*/
#include "modbusCB.h"
#include "mb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "bsp_InternalFlash.h"
#include "queue.h"
#include "cfgThread.h"
#include "DevUpDateCode.h"



QueueHandle_t Modbus_Write_Queue;

extern struct_update g_updatePara;
extern u8 g_wtFlashFlag, g_mbRcved;
extern QueueHandle_t g_Config_Queue;
extern u16 g_devConfigRegID;
eMBErrorCode write_holdReg_handle(u16 regIdx, u8 *regBuf);

/* ----------------------- Static variables ---------------------------------*/
signed short usSysRegHoldingBuf[SYS_HOLDING_REG_NUM] = {0};
signed short usRegHoldingBuf[HOLDING_REG_NUM] = {0};
signed short usRegInputBuf[e_InputReg_Number] = {0};

u32 g_modbusWriteFlag = 0;

void Modbus_Task(void* para)
{
//	eMBErrorCode errcode;
	eMBEventType    eEvent;
	static u8 timCount=0;
	
//	vTaskDelay(500);
	/* 初始化freemodbus协议 */
	eMBInit(MB_RTU, MB_ADDRESS, 0, 115200, MB_PAR_NONE);
	eMBEnable();
	while (1)
	{
		// 程序升级包接收;
		if(g_updatePara.updateFlag>0 && g_updatePara.updateFlag<=2)
		{
			if(g_mbRcved)
			{
				DevUpdate();
				g_mbRcved=0;
			}
//			else if(timCount++>10)
//			{
//				timCount=0;
//				DevUpDateCode_TimeCount();
//				DevUpDateCodeHandle();
//				SlaveDevUpDateCodeHandle();
//			}
		}
		else
			eMBPoll();
		
		if(g_mbRcved)
			usRegHoldingBuf[e_SysF_MainCtl_Health]=0;
		
		// 升级时间处理函数;
		if((g_updatePara.updateFlag&0x7F)>0 && (g_updatePara.updateFlag&0x7F)<=3)
		{
			if(timCount++>10)
			{
				timCount=0;
				DevUpDateCode_TimeCount();
				DevUpDateCodeHandle();
				SlaveDevUpDateCodeHandle();
			}
		}
//		else if(g_updatePara.updateFlag==0)
//			usSysRegHoldingBuf[e_Sys_Upgrade]=0;
		// 升级过程中，中断查询;
//		if(g_updatePara.updateFlag)
//		{
//			usRegHoldingBuf[e_SysF_DevNum]=0;
//			usRegHoldingBuf[e_SysF_HandOptrNum]=0;
//			usRegHoldingBuf[e_SysF_AirSensorNum]=0;
//		}
//		else
//		{
//			usRegHoldingBuf[e_SysF_DevNum]=1;
//			usRegHoldingBuf[e_SysF_HandOptrNum]=1;
//			usRegHoldingBuf[e_SysF_AirSensorNum]=1;
//		}
		
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

void modbus_init(void)
{
	BaseType_t xReturned;
	TaskHandle_t modbus_handle;
	
	Modbus_Write_Queue = xQueueCreate(10, sizeof(modbus_unit_struct));
	if(Modbus_Write_Queue == NULL){
		/* error. */
		while(1) {};
	}
	
	xReturned = xTaskCreate(Modbus_Task, "modbus_task", MODBUS_STACK_DEPTH, NULL, MODBUS_TASK_PRIORITY, &modbus_handle);
	if (xReturned != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}

/**
 * implement interface - holdingcallback of freemodbus.//usRegHoldingBuf[11]
 */
eMBErrorCode eMBRegHoldingCB(uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, eMBRegisterMode eMode)
{
	uint16_t reg_id, temp=0;
	modbus_unit_struct tempUnit={0};
	static uint16_t tempBuf[SYS_HOLDING_REG_NUM]={0};
	eMBErrorCode result = MB_ENOERR;

	if (usAddress < MB_HOLDREG_START)
	{
		return MB_EINVAL;
	}
	reg_id = usAddress - MB_HOLDREG_START + HOLD_REGISTER_OFFSET;
	if ((reg_id>=SYS_HOLDING_REG_BASE && (reg_id + usNRegs) > HOLD_REGISTER_MAXOFFSET)
		|| (reg_id<SYS_HOLDING_REG_BASE && (reg_id + usNRegs) > HOLD_REGISTER_COUNT))
	{
		return MB_EINVAL;
	}
	/* 解析数据帧 */
	if (eMode == MB_REG_READ)
	{
		for(u16 i=reg_id; i<reg_id+usNRegs; i++)
		{
			if(reg_id >= SYS_HOLDING_REG_BASE)
			{
				*pucRegBuffer++ = ( unsigned char )( usSysRegHoldingBuf[i-SYS_HOLDING_REG_BASE] >> 8 );
				*pucRegBuffer++ = ( unsigned char )( usSysRegHoldingBuf[i-SYS_HOLDING_REG_BASE] & 0xFF );
			}
			else
			{
				*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[i] >> 8 );
				*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[i] & 0xFF );
			}
		}
	}
	else if (eMode == MB_REG_WRITE)
	{
		for(u16 i=reg_id; i<reg_id+usNRegs; i++)
		{
			temp = *pucRegBuffer++ << 8;
			temp |= *pucRegBuffer++;
			
			if(i >= SYS_HOLDING_REG_BASE)
			{
				//配置更新
				if(e_Sys_CfgFlag == (i-SYS_HOLDING_REG_BASE) && temp == 0x72)
				{
					memcpy(tempBuf, usSysRegHoldingBuf, sizeof(tempBuf));
					tempBuf[(e_Sys_CfgFlag)] = 0x72;
				}
				
				if(e_Sys_CfgFlag == (i-SYS_HOLDING_REG_BASE) && temp == 0x11C3 && tempBuf[e_Sys_CfgFlag] == 0x72)
				{
//					memcpy(usSysRegHoldingBuf, temBuf, sizeof(temBuf));
					
					tempBuf[(e_Sys_CfgFlag)] = 0;
					xQueueSend(g_Config_Queue, tempBuf, 0);
				}
				
				tempBuf[(i-SYS_HOLDING_REG_BASE)] = temp;
				
				// 升级标识;
				if(e_Sys_Upgrade == (i-SYS_HOLDING_REG_BASE) && temp == 1)
				{
//					__disable_irq();
//					STMFLASH_Write(SOFTWARE_STARTMOD_FLAG, &temp, 1);
//					NVIC_SystemReset();
					g_updatePara.updateFlag=temp;
				}
			}
//			else
//			{
//				usRegHoldingBuf[i] = temp;
//				if(i>DEV_HOLDING_REG_BASE)
//					write_device(i, temp);
//				else
//				{
//					if(i>=reg_id+usNRegs-1)
//					{
//						g_modbusWriteFlag=1;
//					}
//				}
//			}
			else
			{
				usRegHoldingBuf[i] = temp;
//				write_holdReg_handle(reg_id+i, &pucRegBuffer[i*2]);
//				if(i>=e_Sys_DevType && i<e_Sys_DevType+100)	g_modbusWriteFlag |= 1<<0;
				if(i>=SYSF_HOLDING_REG_BASE && i<DEV_HOLDING_REG_BASE)	g_modbusWriteFlag |= 1<<31;
				if(i>=DEV_HOLDING_REG_BASE && i<HAND_HOLDING_REG_BASE)
				{
					g_modbusWriteFlag |= 1<<((i-DEV_HOLDING_REG_BASE)/DEV_REG_NUM);
					if(usRegHoldingBuf[g_devConfigRegID] == 0x0072)	// 配置更新;
					{
						tempUnit.startAddr=i;
						tempUnit.regVal=temp;
						xQueueSend(Modbus_Write_Queue, &tempUnit, 0);
					}
				}
				if(i>=HAND_HOLDING_REG_BASE && i<AIR_HOLDING_REG_BASE)	g_modbusWriteFlag |= 1<<(8+(i-HAND_HOLDING_REG_BASE)/HAND_REG_NUM);
				if(i>=AIR_HOLDING_REG_BASE && i<HOLDING_REG_NUM)		g_modbusWriteFlag |= 1<<(16+(i-AIR_HOLDING_REG_BASE)/AIR_REG_NUM);
				
				
			}
		}
	}
	else
	{
		result = MB_EINVAL;
	}
	
	return result;
}
/* end */

/**
 * implement interface - read input register.
 */
eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
	uint16_t reg_id;
	eMBErrorCode result = MB_ENOERR;

	if (usAddress < MB_INPUTREG_START)
	{
		return MB_EINVAL;
	}
	reg_id = usAddress - MB_INPUTREG_START + INPUT_REGISTER_OFFSET;
	if (reg_id + usNRegs > INPUT_REGISTER_MAXOFFSET)
	{
		return MB_EINVAL;
	}
	
	for(u8 i=reg_id; i<reg_id+usNRegs; i++)
	{
		*pucRegBuffer++ = ( unsigned char )( usRegInputBuf[i] >> 8 );
		*pucRegBuffer++ = ( unsigned char )( usRegInputBuf[i] & 0xFF );
	}

	return result;
}
/* end */


// 写单个字节的处理;
eMBErrorCode write_holdReg_handle(u16 regIdx, u8 *regBuf)
{
//	u8 panelIdx;
//	u16 temp=0;
	eMBErrorCode result = MB_ENOERR;
	
//	if(regIdx == e_RegBuf_Mode)
//	{
//		if(regBuf[1] <= WorkMod_dehum && regBuf[0] <= 1){
//			usRegHoldingBuf[e_RegBuf_Mode] = (regBuf[0]<<8) + regBuf[1];
//		}
//		else	result = MB_EINVAL;
//		// prdQuePanel_Callback中已经操作过了;
//	}
//	else if(regIdx == e_RegBuf_Time)
//	{
//		// 暂时用不着;
//	}
//	else if(regIdx == e_NetOperation)	// 打开/关闭网络;
//	{
//		usRegHoldingBuf[regIdx] = regBuf[1];
//		
//		if(regBuf[1] == 1)
//			Link72_Open_Net();			// 打开网络;
//		else
//			Link72_Close_Net();			// 关闭网络;
//	}
//	else if(regIdx >= e_RegBuf_SetTemp_1 && regIdx < e_RegBuf_MAC1_1)
//	{
//		panelIdx = (regIdx-e_RegBuf_SetTemp_1)/2;
//		if(regIdx%2 == 0)				// 设定温度;
//		{
//			temp = (regBuf[0]<<8) + regBuf[1];
//			if(temp >= 160 && temp <= 300)
//			{
//				usRegHoldingBuf[regIdx] = temp;
//				g_modbusWriteFlag[panelIdx] |= 1;
//			}
//			else	result = MB_EINVAL;
//		}
//		else							// 启停;
//		{
//			if(regBuf[1] <= 1)
//			{
//				usRegHoldingBuf[regIdx] = regBuf[1];
//				g_modbusWriteFlag[panelIdx] |= 2;
////				Link72_Snd_Handle(TYPE_ZCL_SEND, ZCL_CMD, NULL, panelIdx, ATTRID_SYSTEM_ONOFF, &regBuf[1]);			// 开关面板;
//			}
//			else	result = MB_EINVAL;
//		}
//	}
//	else if(regIdx == e_ConfigGateway)	// 配置网关;
//	{
//		if(regBuf[0] == 1)				// 重置网关;
//			reset_gateway();
//		else if(regBuf[1] == 0x01)		// 开始配置;
//		{
//			Gateway_Para.cfgProcess = 1;
//		}
//		else if(regBuf[1] == 0xAA)		// 配置完成;
//		{
//			Gateway_Para.cfgProcess = 0;
//			Gateway_Para.cfgPanelNum = Gateway_Para.panelNum;
//			for(u8 i=0; i<Gateway_Para.cfgPanelNum; i++)
//				memcpy(Gateway_Para.cfgPanelMac[i], Gateway_Para.panel_para[i].mac, 8);
//			g_wtFlashFlag=1;
//		}
//	}
	return result;
}

	
	
