#ifndef _BSPAIRCOM_H_
#define _BSPAIRCOM_H_

#include "stm32f10x.h"


#define		AIR_NUM_MAX				8
#define		AIR_SLAVE_MIN_ADDR		0x64
#define		AIR_SLAVE_MAX_ADDR		0x6B

#define		HAND_NUM_MAX			8
#define		HAND_SLAVE_MIN_ADDR		0x01
#define		HAND_SLAVE_MAX_ADDR		0x08


#define		AIR_WRITE_NUM_MAX		e_HandHoldReg_Number

#define 	AIR_RCV_LENGTH			(e_HandHoldReg_Number*2+10)
#define 	AIR_SND_LENGTH			(e_HandHoldReg_Number*2+10)



/******************************系统位号*************************************/
typedef enum
{
/*********************空气质量传感器**********************************/	
	e_Air_Temp,				// 温度校正
	e_Air_Humi,				// 房间当前湿度
	e_Air_PM2_5,			//
	e_Air_PM10,			//
	e_Air_VOC,			//
	e_Air_CO2,			//
	e_Air_AirQualityState,		        //06-----	气体质量状态  bit0:PM2.5  bit1:PM10		 bit2:CO2			bit3:voc 
	e_Air_DevErrorCode,                //07-----	设备故障报警  bit0:PM2.5/PM10模块    bit1:CO2传感器     bit2:voc传感器     bit3:sht31
	e_Air_RGBState,			            //08-----	呼吸灯开关状态
	e_Air_PM25ValueOve,								//9-----	室内PM2.5超标值
	e_Air_PM10ValueOve,						    //10-----	室PM10超标值
	e_Air_CO2ValueOve,					        //11-----	室内二氧化碳超标值
	e_Air_VOCValueOve,									//12-----	室内VOC超标值
	e_Air_PM25Offset,							//13-----	室内PM2.5修正偏移量
	e_Air_PM10Offset,							//14-----	室内PM10修正偏移量
	e_Air_CO2Offset,							//15-----	室内二氧化碳修正偏移量
	e_Air_VOCOffset,							//16-----	室内VOC修正偏移量
	e_Air_WorkMode,										//17-----	室内工作模式
	e_Air_TemOffset,							//18-----	室内温度修正偏移量
	e_Air_HumOffset,							//19-----	室内湿度修正偏移量
	e_Air_Number1,											//20-----	预留1
	e_Air_Number2,											//21-----	预留2
	
	
	e_AirHoldReg_Number,			// 保持寄存器总个数
	
	
}eAirSensorHoldingReg;
/******************************系统位号*************************************/
typedef enum
{
/*********************手操器**********************************/	
	e_Hand_DispTemp,			// 工作模式
	e_Hand_DispHumi,			// 工作模式
	
	e_Hand_Switch,				// 工作模式
	e_Hand_SetMode,				// 工作模式
	e_Hand_SetTemp,				// 工作模式
	e_Hand_SetHumi,				// 工作模式
	e_Hand_AirVol,				// 工作模式
	e_Hand_FilterTim,			// 工作模式
	e_Hand_FilterTimAMax,		// 工作模式
	e_Hand_SchelduEn,			// 工作模式
	e_Hand_Monday1,				// 工作模式
	e_Hand_Monday2,				// 工作模式
	e_Hand_Tues1,				// 工作模式
	e_Hand_Tues2,				// 工作模式
	e_Hand_Wednes1,				// 工作模式
	e_Hand_Wednes2,				// 工作模式
	e_Hand_Thurs1,				// 工作模式
	e_Hand_Thurs2,				// 工作模式
	e_Hand_Friday1,				// 工作模式
	e_Hand_Friday2,				// 工作模式
	e_Hand_Satur1,				// 工作模式
	e_Hand_Satur2,				// 工作模式
	e_Hand_Sunday1,				// 工作模式
	e_Hand_Sunday2,				// 工作模式
	e_Hand_TimeFlag,			// 时间修改标志
	
	e_Hand_HSpeedVol,			
	e_Hand_MSpeedVol,
	e_Hand_LSpeedVol,
	e_Hand_HeatAirTemp,
	e_Hand_CoolAirTemp,
	e_Hand_AirAirTemp,
	
	e_Hand_PM25Offset,			// 室内PM2.5修正偏移量
	e_Hand_PM10Offset,			// 室内PM10修正偏移量
	e_Hand_CO2Offset,			// 室内二氧化碳修正偏移量
	e_Hand_VOCOffset,			// 室内VOC修正偏移量
	e_Hand_TemOffset,			// 室内温度修正偏移量
	e_Hand_HumOffset,			// 室内湿度修正偏移量
	e_Hand_Reserve1,			// 保留
	e_Hand_Reserve2,			// 保留
	e_Hand_Reserve3,			// 保留
	e_Hand_Reserve4,			// 保留
	e_Hand_Reserve5,			// 保留
	e_Hand_Reserve6,			// 保留
	e_Hand_Reserve7,			// 保留
	e_Hand_Reserve8,			// 保留
	e_Hand_Reserve9,			// 保留
	e_Hand_Reserve10,			// 保留
	e_Hand_Reserve11,			// 保留
	e_Hand_Reserve12,			// 保留
	
	e_Hand_PM25,				// 工作模式
	e_Hand_PM10,				// 工作模式
	e_Hand_VOC,					// 工作模式
	e_Hand_CO2,					// 工作模式
	
	e_Hand_Year,				// 时间
	e_Hand_Month,				// 时间
	e_Hand_Day,					// 时间
	e_Hand_Hour,				// 时间
	e_Hand_Min,					// 时间
	e_Hand_Sec,					// 时间
	e_Hand_Week,				// 时间
	
	e_Hand_OutTemp,				// 外机环境温度
	e_Hand_InTemp,				// 内机环境温度
	e_Hand_SetEvaporatorTemp,	// 再热前温度
	e_Hand_PQY,					// 排气压力
	e_Hand_XQP,					// 吸气压力
	e_Hand_XQT_T1,				// 压缩机吸气温度
	e_Hand_PQT_T2,				// 压缩机排气温度
	e_Hand_NowFre,				// 压缩机当前频率
	e_Hand_Err1,				// 故障1
	e_Hand_Err2,				// 故障2
	e_Hand_Err3_1,				// 故障3
	e_Hand_Err3_2,				// 故障4
	
	
	e_HandHoldReg_Number,		// 保持寄存器总个数
	
	
}eHandOptHoldingReg;


typedef struct{
	u8 errCnt;
	u8 rcvFlag;
	u16 rcvLen;
	u8 rcvBuf[AIR_RCV_LENGTH];
	u16 sndLen;
	u8 sndBuf[AIR_SND_LENGTH];
	
}Air_Com_Para_Struct;


typedef struct{
	u16 HandComHealth;
	u16 AirComHealth;


}air_para_struct;


#pragma pack (1)
typedef struct{
	uint8_t slaveAddr;
	uint8_t funcCode;
	uint8_t regNum;
	
	uint16_t startAddr;
	int16_t regVal[AIR_WRITE_NUM_MAX];
	
}air_unit_struct;
#pragma pack()


void Air_485_Config(void);
void Air_Snd_Buf(u8 *sndBuf, u16 sndLen);

void Air_TX_Service(air_unit_struct *tempUnit);
u8 Air_RX_Service(air_unit_struct tempUnit);

#endif

