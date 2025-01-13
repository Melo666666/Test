#include "cfgThread.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "modbusCB.h"
#include "bspRtc.h"
#include "bsp_InternalFlash.h"
#include "bspNetCom.h"
#include "string.h"
#include "bspDevCom.h"
#include "bspAirCom.h"
#include "bspWifi232b2.h"
#include "bspClm920.h"
#include "bspDevTag.h"


extern Net_Module_Enum g_netModule;
extern wifi232b2_struct wifi232b2Para;
extern Clm920_Para_Struct Clm920_Para;

QueueHandle_t g_Config_Queue;
QueueHandle_t g_SysRegOpt_Mutex;
extern QueueHandle_t Air_Snd_Queue;
extern QueueHandle_t Dev_Snd_Queue;
//extern QueueHandle_t Air_Snd_Queue;


void write_device(u16 regId, u16 regVal)
{
	dev_unit_struct tempUnit={0};
	u16 regNum=0, regIdBase=0, slaveAddr=0;
	QueueHandle_t tempQueue;
	
	if(regId > HOLDING_REG_NUM-SYS_HOLDING_REG_NUM)	return;
	
	if(regId > DEV_HOLDING_REG_BASE && regId < HAND_HOLDING_REG_BASE)
	{
		regNum=DEV_REG_NUM;
		regIdBase=DEV_HOLDING_REG_BASE;
		slaveAddr=DEV_SLAVE_MIN_ADDR;
		tempQueue=Dev_Snd_Queue;
	}
	else if(regId > HAND_HOLDING_REG_BASE && regId < AIR_HOLDING_REG_BASE)
	{
		regNum=HAND_REG_NUM;
		regIdBase=HAND_HOLDING_REG_BASE;
		slaveAddr=HAND_SLAVE_MIN_ADDR;
		tempQueue=Air_Snd_Queue;
	}
	else if(regId > AIR_HOLDING_REG_BASE && regId < SYS_HOLDING_REG_NUM)
	{
		regNum=AIR_REG_NUM;
		regIdBase=AIR_HOLDING_REG_BASE;
		slaveAddr=AIR_SLAVE_MIN_ADDR;
		tempQueue=Air_Snd_Queue;
	}
	tempUnit.slaveAddr=slaveAddr+(regId-regIdBase)/regNum;
	tempUnit.funcCode=0x06;
	if((regId-regIdBase-(tempUnit.slaveAddr-slaveAddr)*regNum)>=1)
		tempUnit.startAddr=regId-regIdBase-(tempUnit.slaveAddr-slaveAddr)*regNum-1;
	else return;
	if(regId > DEV_HOLDING_REG_BASE && regId < HAND_HOLDING_REG_BASE)
	{
		if(tempUnit.startAddr>=7)
			tempUnit.startAddr-=7;
		else return;
	}
	tempUnit.regVal[0]=regVal;
	
	xQueueSendToFront(tempQueue, &tempUnit, portMAX_DELAY);
}

void set_dev_num(void)
{
	// 最多一个
	usRegHoldingBuf[e_SysF_DevNum]=1;
	usRegHoldingBuf[e_SysF_HandOptrNum]=1;
	usRegHoldingBuf[e_SysF_AirSensorNum]=1;
	
	usRegHoldingBuf[e_SysF_HandMap1_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap1_2] = 0;
	usRegHoldingBuf[e_SysF_HandMap2_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap2_2] = 0;
	usRegHoldingBuf[e_SysF_HandMap3_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap3_2] = 0;
	usRegHoldingBuf[e_SysF_HandMap4_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap4_2] = 0;
	usRegHoldingBuf[e_SysF_HandMap5_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap5_2] = 0;
	usRegHoldingBuf[e_SysF_HandMap6_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap6_2] = 0;
	usRegHoldingBuf[e_SysF_HandMap7_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap7_2] = 0;
	usRegHoldingBuf[e_SysF_HandMap8_1] = 1;
	usRegHoldingBuf[e_SysF_HandMap8_2] = 0;
}	


void set_para_default(void)
{
	u16 tcpPortCat1=9100, tcpPortWifi=9101, tcpAddr[2]={0x2F66, 0xD167};
	
//	usSysRegHoldingBuf[e_Sys_Remote1Port_Cat1]=tcpPortCat1;
//	usSysRegHoldingBuf[e_Sys_Remote1Port_Wifi]=tcpPortWifi;
//	memcpy(&usSysRegHoldingBuf[e_Sys_Remote1IP1], tcpAddr, 4);
	
	set_dev_num();
}


void write_flash(void)
{
	u16 cfgFlag = 0, tempBuf[256]={0};
	
	set_dev_num();
	
//	portDISABLE_INTERRUPTS();	// 关闭中断;

	cfgFlag = 0xAAAA;
	STMFLASH_Write(ADDR_OF_CfgFlag, &cfgFlag, 1);

	memcpy(&tempBuf, usSysRegHoldingBuf, SYS_HOLDING_REG_NUM);
	memcpy(&tempBuf[SYS_HOLDING_REG_NUM], usRegHoldingBuf, SYSF_HOLDING_REG_NUM);
//	memcpy(&tempBuf[SYS_HOLDING_REG_NUM+SYSF_HOLDING_REG_NUM], &g_netModule, 1);

	tempBuf[SYS_HOLDING_REG_NUM+SYSF_HOLDING_REG_NUM]=g_netModule;
	tempBuf[SYS_HOLDING_REG_NUM+SYSF_HOLDING_REG_NUM+1]=wifi232b2Para.connectFlag;
	
	STMFLASH_Write(ADDR_OF_SysReg, tempBuf, SYS_HOLDING_REG_NUM+SYSF_HOLDING_REG_NUM+2);

//	portENABLE_INTERRUPTS();	// 开启中断;

}

void read_flash(void)
{
	u8 i, temp[5]={0x01, 0x14};
	u16 cfgFlag = 0, tempBuf[256]={0};
	
//	portDISABLE_INTERRUPTS();		// 关闭中断;
	STMFLASH_Read(ADDR_OF_CfgFlag, &cfgFlag, 1);

//	runFlag = 0xAAAC;
	if(cfgFlag == 0xAAAA)
	{
		STMFLASH_Read(ADDR_OF_SysReg, tempBuf, sizeof(tempBuf)/2);
		memcpy(usSysRegHoldingBuf, &tempBuf, SYS_HOLDING_REG_NUM);
		memcpy(usRegHoldingBuf, &tempBuf[SYS_HOLDING_REG_NUM], SYSF_HOLDING_REG_NUM);
		g_netModule=tempBuf[SYS_HOLDING_REG_NUM+SYSF_HOLDING_REG_NUM];
		wifi232b2Para.connectFlag=tempBuf[SYS_HOLDING_REG_NUM+SYSF_HOLDING_REG_NUM+1];
		usRegHoldingBuf[e_SysF_MainCtl_Health]=0;
//		STMFLASH_Read(ADDR_OF_SysReg, (u16*)usSysRegHoldingBuf, SYS_HOLDING_REG_NUM);
//		STMFLASH_Read(ADDR_OF_SysFucReg, (u16*)usRegHoldingBuf, SYSF_HOLDING_REG_NUM);
//		STMFLASH_Read(ADDR_OF_NetPara, &cfgFlag, 1);
//		g_netModule = cfgFlag;
//		STMFLASH_Read(ADDR_OF_NetPara+2, &cfgFlag, 1);
//		wifi232b2Para.connectFlag=cfgFlag;
	}
	else
	{
		set_para_default();
	}
//	portENABLE_INTERRUPTS();		// 开启中断;
}



void sysTimUpdate(void)
{
	u32 secPerDay=24*60*60;
	
	usSysRegHoldingBuf[e_Sys_SysTim_Year] = calendar.w_year;
	usSysRegHoldingBuf[e_Sys_SysTim_Month] = calendar.w_month;
	usSysRegHoldingBuf[e_Sys_SysTim_Day] = calendar.w_date;
	usSysRegHoldingBuf[e_Sys_SysTim_Hour] = calendar.hour;
	usSysRegHoldingBuf[e_Sys_SysTim_Min] = calendar.min;
	usSysRegHoldingBuf[e_Sys_SysTim_Sec] = calendar.sec;
	usSysRegHoldingBuf[e_Sys_SysTim_Week] = calendar.week;
	
//	for(u8 i=0; i<8; i++)
//	{
//		if(usRegHoldingBuf[e_Dev_Health_0+i*DEV_REG_NUM]<9999)		usRegHoldingBuf[e_Dev_Health_0+i*DEV_REG_NUM]++;
//		if(usRegHoldingBuf[e_Hand_Health_0+i*HAND_REG_NUM]<9999)	usRegHoldingBuf[e_Hand_Health_0+i*HAND_REG_NUM]++;
//		if(usRegHoldingBuf[e_Air_Health_0+i*AIR_REG_NUM]<9999)		usRegHoldingBuf[e_Air_Health_0+i*AIR_REG_NUM]++;	
//	}
	if(usRegHoldingBuf[e_SysF_MainCtl_Health]<9999)	usRegHoldingBuf[e_SysF_MainCtl_Health]++;

	// 系统运行时间
	usSysRegHoldingBuf[e_Sys_RunTim_Day] = calendar.sysRunSec/secPerDay;
	usSysRegHoldingBuf[e_Sys_RunTim_Hour] = (calendar.sysRunSec%secPerDay)/3600;
	usSysRegHoldingBuf[e_Sys_RunTim_Min] = ((calendar.sysRunSec%secPerDay)%3600)/60;
	usSysRegHoldingBuf[e_Sys_RunTim_Sec] = ((calendar.sysRunSec%secPerDay)%3600)%60;
	
	// 设备类型
	if(usSysRegHoldingBuf[e_Sys_DevType] == e_Air_Controller)
	{
		for(u8 i=e_SysF_Dev1Type; i <= e_SysF_Dev8Type; i++)
			usRegHoldingBuf[i]=e_Air_Dehumi;
	}
	else
	{
		if(usRegHoldingBuf[e_SysF_Dev1Type] != e_Double_Cold && usRegHoldingBuf[e_SysF_Dev1Type] != e_Heat_Pumb)
			for(u8 i=e_SysF_Dev1Type; i <= e_SysF_Dev8Type; i++)
				usRegHoldingBuf[i]=e_Double_Cold;
	}
	// iccid号
	memcpy(&usSysRegHoldingBuf[e_Sys_Iccid1], Clm920_Para.iccid, sizeof(Clm920_Para.iccid));
}


void sysConfig(void)
{
	static u16 tempBuf[8]={0};
	
	RTC_Set(usSysRegHoldingBuf[e_Sys_SysTim_Year], usSysRegHoldingBuf[e_Sys_SysTim_Month], usSysRegHoldingBuf[e_Sys_SysTim_Day], usSysRegHoldingBuf[e_Sys_SysTim_Hour], usSysRegHoldingBuf[e_Sys_SysTim_Min], usSysRegHoldingBuf[e_Sys_SysTim_Sec]);
	
	if(memcmp(tempBuf, &usSysRegHoldingBuf[e_Sys_Remote1IP1], sizeof(tempBuf)))
	{
		net_change_IP();
		memcpy(tempBuf, &usSysRegHoldingBuf[e_Sys_Remote1IP1], sizeof(tempBuf));
	}
	g_modbusWriteFlag |= 1<<31;
//	vTaskDelay(1500);
//	NVIC_SystemReset();
}
/**
 * This function is the entry of led thread.
 */
static void sys_thread_entry(void *parameter)
{
	RTC_Init();
	
//	vTaskDelay(5000);
	read_flash();
	usSysRegHoldingBuf[e_Sys_SoftVer] = SOFTVISION;
	usSysRegHoldingBuf[e_Sys_HardwVer] = HARDVISION;
	while(1)
	{
		if(g_modbusWriteFlag & (1<<31))
		{
			g_modbusWriteFlag &= ~(1<<31);
			write_flash();
		}
		xSemaphoreTake(g_SysRegOpt_Mutex, portMAX_DELAY);
		sysTimUpdate();
		xSemaphoreGive(g_SysRegOpt_Mutex);
		
		vTaskDelay(1000);
	}
}
/* end */
/**
 * This function is the entry of led thread.
 */
static void cfg_thread_entry(void *parameter)
{
	u16 tempBuf[SYS_HOLDING_REG_NUM]={0};
	
	
	while(1)
	{
		xQueueReceive(g_Config_Queue, tempBuf, portMAX_DELAY);
		
		xSemaphoreTake(g_SysRegOpt_Mutex, portMAX_DELAY);
		memcpy(usSysRegHoldingBuf, tempBuf, sizeof(tempBuf));
		sysConfig();
		xSemaphoreGive(g_SysRegOpt_Mutex);
		
		vTaskDelay(100);
	}
}
/* end */

/**
 * This function initialize the thread of led.
 */
void cfgThread_init(void)
{
	BaseType_t xReturned1, xReturned2;
	TaskHandle_t xHandle1 = NULL, xHandle2 = NULL;
	
	g_SysRegOpt_Mutex = xSemaphoreCreateMutex();
	g_Config_Queue = xQueueCreate(1, SYS_HOLDING_REG_NUM*2);
	if(g_SysRegOpt_Mutex == NULL || g_Config_Queue == NULL){
		/* error. */
		while(1) {};
	}
	xReturned1 = xTaskCreate(cfg_thread_entry, CFG_NAME, CFG_STACKDEPTH, 0, CFG_PRIORITY, &xHandle1);
	xReturned2 = xTaskCreate(sys_thread_entry, SYS_NAME, SYS_STACKDEPTH, 0, SYS_PRIORITY, &xHandle2);
	
	if (xReturned1 != pdPASS || xReturned2 != pdPASS)
	{
		/* error. */
		while(1) {};
	}
}
/* end */
