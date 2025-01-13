#ifndef _modbus_cb_H
#define _modbus_cb_H

#include "stm32f10x.h"

extern signed short usSysRegHoldingBuf[];
extern signed short usRegHoldingBuf[];
extern signed short usRegInputBuf[];
extern u32 g_modbusWriteFlag;


#define MODBUS_STACK_DEPTH		(1024/4)
#define MODBUS_TASK_PRIORITY	(configMAX_PRIORITIES - 1)

#define	SYS_HOLDING_REG_BASE	60000
#define	SYS_HOLDING_REG_NUM		100

#define	SYSF_HOLDING_REG_BASE	0
#define	SYSF_HOLDING_REG_NUM	100

#define	DEV_HOLDING_REG_BASE	100
#define	DEV_HOLDING_REG_NUM		2080

#define	HAND_HOLDING_REG_BASE	2180
#define	HAND_HOLDING_REG_NUM	640

#define	AIR_HOLDING_REG_BASE	2820
#define	AIR_HOLDING_REG_NUM		240


#define	HOLDING_REG_NUM			3060//(2720-SYS_HOLDING_REG_NUM)
#define	DEV_REG_NUM				260
#define	HAND_REG_NUM			80
#define	AIR_REG_NUM				30


#define MB_ADDRESS (1)
#define MB_HOLDREG_START (0 + 1)
#define MB_INPUTREG_START (0 + 1)

#define HOLD_REGISTER_OFFSET (0)
#define HOLD_REGISTER_COUNT (HOLDING_REG_NUM)
//#define HOLD_REGISTER_MAXOFFSET (HOLD_REGISTER_OFFSET + HOLD_REGISTER_COUNT)
#define HOLD_REGISTER_MAXOFFSET (SYS_HOLDING_REG_BASE + SYS_HOLDING_REG_NUM)
#define INPUT_REGISTER_COUNT (e_InputReg_Number)
#define INPUT_REGISTER_OFFSET (0)
#define INPUT_REGISTER_MAXOFFSET (INPUT_REGISTER_COUNT + INPUT_REGISTER_OFFSET)


typedef enum{
	None,
	Mode_Write,
	Mode_Read,
}Data_Mode;

typedef struct{
	Data_Mode mode;
	uint8_t *rcvPtr;
	uint8_t rcvLen;
}Send_Para_Struct; 
 
typedef enum {
	WorkMod_none,
	WorkMod_cool,
	WorkMod_hot,
	WorkMod_air,
	WorkMod_dehum,
}e_WorkMod;

typedef struct{
	
	uint16_t startAddr;
	int16_t regVal;
	
}modbus_unit_struct;

/******************************系统位号*************************************/
typedef enum
{
	e_Sys_DevType	=	0,			// 工作模式
	e_Sys_DevID3,					// 时间
	e_Sys_DevID2,					// 时间
	e_Sys_DevID1,					// 时间
	e_Sys_DevID0,					// 时间
	e_Sys_DevIP1,					// 时间
	e_Sys_DevIP0,					// 时间
	e_Sys_Mask1,					// 时间
	e_Sys_Mask0,					// 时间
	e_Sys_GatewayIP1,				// 时间
	e_Sys_GatewayIP0,				// 时间
	e_Sys_MAC2,						// 时间
	e_Sys_MAC1,						// 时间
	e_Sys_MAC0,						// 时间
	e_Sys_Remote1IP1,				// 时间
	e_Sys_Remote1IP0,				// 时间
	e_Sys_Remote1Port_Cat1,			// 时间
	e_Sys_Remote1Port_Wifi,			// 时间
	e_Sys_Remote2IP1,				// 时间
	e_Sys_Remote2IP0,				// 时间
	e_Sys_Remote2Port_Cat1,			// 时间
	e_Sys_Remote2Port_Wifi,			// 时间
	e_Sys_SysTim_Year,				// 时间
	e_Sys_SysTim_Month,				// 时间
	e_Sys_SysTim_Day,				// 时间
	e_Sys_SysTim_Hour,				// 时间
	e_Sys_SysTim_Min,				// 时间
	e_Sys_SysTim_Sec,				// 时间
	e_Sys_SysTim_Week,				// 时间
	e_Sys_RunTim_Year,				// 时间
	e_Sys_RunTim_Month,				// 时间
	e_Sys_RunTim_Day,				// 时间
	e_Sys_RunTim_Hour,				// 时间
	e_Sys_RunTim_Min,				// 时间
	e_Sys_RunTim_Sec,				// 时间
	
	e_Sys_SoftVer,					// 时间
	e_Sys_HardwVer,					// 时间
	e_Sys_Upgrade,					// 时间
	e_Sys_CfgFlag,					// 时间
	
	e_Sys_Iccid1,					// 三网卡iccid号
	e_Sys_Iccid2,					// 三网卡iccid号
	e_Sys_Iccid3,					// 三网卡iccid号
	e_Sys_Iccid4,					// 三网卡iccid号
	e_Sys_Iccid5,					// 三网卡iccid号
	
	
	e_SysReg_Number,				// 保持寄存器总个数
}eSysHoldingReg;

/******************************系统功能*************************************/
typedef enum
{
	e_SysF_DevNum	=	0,			// 工作模式
	e_SysF_Dev1Type,				// 时间
	e_SysF_Dev2Type,				// 时间
	e_SysF_Dev3Type,				// 时间
	e_SysF_Dev4Type,				// 时间
	e_SysF_Dev5Type,				// 时间
	e_SysF_Dev6Type,				// 时间
	e_SysF_Dev7Type,				// 时间
	e_SysF_Dev8Type,				// 时间
	e_SysF_MainCtl,					// 时间
	e_SysF_MainCtl_Health,			// 时间
	e_SysF_HandOptrNum,				// 时间
	e_SysF_AirSensorNum,			// 时间
	
	e_SysF_HandMap1_1,				// 时间
	e_SysF_HandMap1_2,
	e_SysF_HandMap2_1,				// 时间
	e_SysF_HandMap2_2,
	e_SysF_HandMap3_1,				// 时间
	e_SysF_HandMap3_2,
	e_SysF_HandMap4_1,				// 时间
	e_SysF_HandMap4_2,
	e_SysF_HandMap5_1,				// 时间
	e_SysF_HandMap5_2,
	e_SysF_HandMap6_1,				// 时间
	e_SysF_HandMap6_2,
	e_SysF_HandMap7_1,				// 时间
	e_SysF_HandMap7_2,
	e_SysF_HandMap8_1,				// 时间
	e_SysF_HandMap8_2,
	
	e_SysFReg_Number,				// 保持寄存器总个数
}eSysFucHoldingReg;

/******************************设备*************************************/
typedef enum
{
	e_Dev_Health_0	=	100,		// 工作模式
	
	/*********************直膨式新风除湿机**********************************/	
	e_Dev_HSpeedVol,
	e_Dev_MSpeedVol,
	e_Dev_LSpeedVol,
	e_Dev_HeatAirTemp,
	e_Dev_CoolAirTemp,
	e_Dev_AirAirTemp,
	e_Dev_Reserve1,
	
	e_Dev_CodeIAPFlag_1,
	e_Dev_CodeIAPFlag_2,	
	e_Dev_CodeIAPFlag_3,
	e_Dev_CodeIAPFlag_4,		
	e_Dev_ConfigFlag,  								//内机配置更新标识
	e_Dev_SWPUB,       								//内机软件版本号
	e_Dev_HWPUB,       								//内机硬件版本号
	e_Dev_DevType,     								//内机设备类型号		
	
	e_Dev_DevAddr,											    	//设备地址
	e_Dev_SetONOFF, 										      //系统开关  可设置  0
	e_Dev_SetAutoRun,												//等于0是自动 等于1是手动 2	
	e_Dev_SetMod,													  //模式设置  可设置  1
	e_Dev_SetEnergyConservationMod,					//节能模式
	e_Dev_SetSilenceMod,										  //静音模式
	e_Dev_SetWaterTempCon,									  //水温控制方式
	e_Dev_SetHeatWaterTemp,                  //设置制热水温
	e_Dev_SetCoolWaterTemp,                  //设置制冷水温
	e_Dev_SetAddHumEn,              				  //加湿使能
	e_Dev_SetAddHumAct,              				//加湿动作
	e_Dev_SetEvaporatorTemp,									//设置再热前温度
	e_Dev_SetAirSupplyTemp,						      //送风温度设置值	
	e_Dev_FANControl_Speed,   								//风速档位	 

	e_Dev_SetSGS1,														//吸气过热度1设置
	e_Dev_SetSGS2,														//吸气过热度2设置
	e_Dev_SetSGS3,														//吸气过热度3设置
	e_Dev_SetDGS1,														//排气过热度1设置
	e_Dev_SetDGS2,														//排气过热度2设置	
	e_Dev_SetDGS3,														//排气过热度3设置
	e_Dev_SetEEV2GS_Low,											//EEV2过热度调节下限
	e_Dev_SetEEV1GS_Low,											//EEV1过热度调节下限
	e_Dev_OpenEHeatHJTemp,										//开启电加热环境温度
	e_Dev_CloseEHeatHJTemp,									//关闭电加热环境温度
	
	//化霜
	e_Dev_DefrostingCheckTimeStart,					//化霜检测启动时间（制热模式下）
	e_Dev_DefrostingHJTemp1,					        //化霜判定环境温度1
	e_Dev_DefrostingHJTemp2,					        //化霜判定环境温度2
	e_Dev_DefrostingHJTemp3,					        //化霜判定环境温度3
	e_Dev_DefrostingWJPGTemp1,					      //化霜判定外机盘管温度1
	e_Dev_DefrostingWJPGTemp2,					      //化霜判定外机盘管温度2
	e_Dev_DefrostingWJPGTemp3,						    //化霜判定外机盘管温度3
	e_Dev_DefrostingWJPGTemp4,						    //化霜判定外机盘管温度4
	e_Dev_DefrostingCompressorFre,		//化霜压缩机频率
	e_Dev_DefrostingEEV1,							//化霜电子膨胀阀1开度
	e_Dev_DefrostingEEV2,							//化霜电子膨胀阀2开度	
	e_Dev_DefrostingEEV3,							//化霜电子膨胀阀3开度	
	e_Dev_DefrostingExitWGTemp,   //化霜退出外管温度
 	e_Dev_DefrostingCompressorFreRunMaxTime,   //化霜压缩机运行最大时间
 	e_Dev_DefrostingCompressorFreRunMinTime,   //化霜压缩机运行最小时间
	
  //回油 
	e_Dev_OilRecovery_CompressorFre,//回油频率60         	
	e_Dev_OilRecovery_IntervalTime,//间隔时间 单位分钟     
	e_Dev_OilRecovery_ActKeepTime, //回油保持时间 单位分钟 
	e_Dev_OilRecovery_EEV1SetVar,//回油时电子膨胀阀1开度
	e_Dev_OilRecovery_EEV2SetVar,//回油时电子膨胀阀2开度
	e_Dev_OilRecovery_EEV3SetVar,//回油时电子膨胀阀3开度
	
	//加湿
	e_Dev_AddWater_FZLowTime, 	//加水低限浮子超时时间
	e_Dev_AddWater_FZHighTime,	//加水高限浮子超时时间
	e_Dev_AddHum_FZLowTime,	//加湿低限浮子超时时间
	e_Dev_SubWater_FZLowTime,	//排水低限浮子超时时间
	e_Dev_CleanWaterTank_IntervalTime,//清洗水箱间隔时间
	e_Dev_CleanWaterTank_ActKeepTime,//清洗水箱持续时间
	e_Dev_CleanWaterTankAddWater_FZLowTime,//清洗加水低限浮子响应后额外加水时间
	e_Dev_AddHumStopErr_IntervalTimeTime,//加湿加水三次停机故障-加水间隔时间
	
	e_Dev_CompressorRun_MinIntervalTime,//压缩机启动最小间隔
	//滤网更换相关
	e_Dev_AirFilterUseTime,//客户已使用时间  	
	e_Dev_AirFilterConfigureMaxTime,//配置最大时间  
	e_Dev_WaterFlowSmallAlarmTempDiff,//水流量小报警温度差
	e_Dev_DevResonanceFrequency1, //设备共振频率1
	e_Dev_DevResonanceFrequency2, //设备共振频率2
	e_Dev_DevResonanceFrequency3, //设备共振频率3
	e_Dev_DevResonanceFrequency4, //设备共振频率4	
	
	e_Dev_EvaporatorTempNoOKTime,  //再热前温度不达标时间
	e_Dev_WaterTempNoOKTime,  //水热温度不达标时间
	e_Dev_WaterTempNoOKTempDt_Cool,      //水热温度不达标温度偏置制冷	
	e_Dev_WaterTempNoOKTempDt_Heat,      //水热温度不达标温度偏置制热	
	e_Dev_ZRQ_MAXTEMP_HeatModue,  //制热再热前最大设定温度
	e_Dev_ZRQ_MINTEMP_HeatModue,  //制热再热前最小设定温度
	e_Dev_HeatSongFengTempConEEV2ACTOffset,//制热送风温度调节EEV2允许动作范围偏移值
	e_Dev_SubHumInAirOKABSTemp_Val, //制冷再热前温度达标绝对值（EEV2进入过热度调节）
	e_Dev_SubHumAirOutOKABSTemp_Val, //制冷再热前温度不达标绝对值（EEV2进入温度调节）
	e_Dev_WaterInOKABSTemp_Val, //制冷水温达标绝对值（EEV1进入过热度调节）
	e_Dev_WaterOutOKABSTemp_Val, //制冷水温不达标绝对值（EEV1进入温度调节）
  e_Dev_DSTLow_Temp,						//排气过热度调节进入值
	e_Dev_DSTLow_OutTemp,				//排气过热度调节完成值
  e_Dev_CoolModEnergyConserInFDT6Temp, //制冷模式设备自节能板换防冻进入温度值（T6）
	e_Dev_CoolModEnergyConserOutFDT6Temp,//制冷模式设备自节能板换防冻结束温度正偏值（T6）
	e_Dev_CoolWaterTempEEV1Close,//制冷水温EEV1关闭值(T8)
	e_Dev_CoolWaterTempEEV1Open, //制冷水温EEV1打开值(T8)
	e_Dev_HeatWaterTempEEV1Close,//制热水温EEV1关闭值(T8)
	e_Dev_HeatWaterTempEEV1Open, //制热水温EEV1打开值(T8)
	
	e_Dev_NoEnergyConserModDevZJNFlag,   //非节能模式下设备自节能标识
	
	e_Dev_DevRunModStatus,			//机器运行模式
	e_Dev_DevRunModSubstatus,  //机器运行模式子状态
	
	e_Dev_InterMachEvapInTemp_T9,   //内机蒸发进口温度T9
	e_Dev_InterMachEvapOutTemp_T10,   //内机蒸发进口温度T10
	e_Dev_InterMachFanInTemp_T11,   //内机进风温度T11
	e_Dev_InterMachBeforeReheatTemp, //内机再热前温度
	e_Dev_InterMachAirSupplyTemp,	//内机送风温度	
	e_Dev_InterMachXHJSSTemp,	//内机循环加湿水温


  
	e_DevReg_Number,				// 保持寄存器总个数
	
	
	
	/*********************双冷源**********************************/	
	e_Dev_Double_ConfigFlag=224,
	
	e_Dev_Double_AirFilterConfigureMaxTime=156,	//配置最大时间  55
	e_Dev_Double_AirFilterUseTime=157,			//客户已使用最大时间  56
	
	
	/*********************直膨式新风除湿机、热泵新风一体机**********************************/	
	e_Dev_HeatPumpAir_ConfigFlag=105,
	
	e_Dev_HeatPumpAir_AirFilterUseTime=164,			//客户已使用时间  	
	e_Dev_HeatPumpAir_AirFilterConfigureMaxTime=165,//配置最大时间  
	
}eDevHoldingReg;

/******************************手操器*************************************/
typedef enum
{
	e_Hand_Health_0	=	2180,		// 工作模式
	e_Hand_DispTemp_0,				// 工作模式
	e_Hand_DispHumi_0,				// 工作模式
	
	e_Hand_Switch_0,				// 工作模式
	e_Hand_SetMode_0,				// 工作模式
	e_Hand_SetTemp_0,				// 工作模式
	e_Hand_SetHumi_0,				// 工作模式
	e_Hand_AirVol_0,				// 工作模式
	e_Hand_FilterTim_0,				// 工作模式
	e_Hand_FilterTimAMax_0,			// 工作模式
	e_Hand_SchelduEn_0,				// 工作模式
	e_Hand_Monday1_0,				// 工作模式
	e_Hand_Monday2_0,				// 工作模式
	e_Hand_Tues1_0,					// 工作模式
	e_Hand_Tues2_0,					// 工作模式
	e_Hand_Wednes1_0,				// 工作模式
	e_Hand_Wednes2_0,				// 工作模式
	e_Hand_Thurs1_0,				// 工作模式
	e_Hand_Thurs2_0,				// 工作模式
	e_Hand_Friday1_0,				// 工作模式
	e_Hand_Friday2_0,				// 工作模式
	e_Hand_Satur1_0,				// 工作模式
	e_Hand_Satur2_0,				// 工作模式
	e_Hand_Sunday1_0,				// 工作模式
	e_Hand_Sunday2_0,				// 工作模式	
	e_Hand_TimeFlag_0,				// 时间修改标志
	
	e_Hand_HSpeedVol_0,			
	e_Hand_MSpeedVol_0,
	e_Hand_LSpeedVol_0,
	e_Hand_HeatAirTemp_0,
	e_Hand_CoolAirTemp_0,
	e_Hand_AirAirTemp_0,
	
	e_Hand_PM25Offset_0,			// 室内PM2.5修正偏移量
	e_Hand_PM10Offset_0,			// 室内PM10修正偏移量
	e_Hand_CO2Offset_0,				// 室内二氧化碳修正偏移量
	e_Hand_VOCOffset_0,				// 室内VOC修正偏移量
	e_Hand_TemOffset_0,				// 室内温度修正偏移量
	e_Hand_HumOffset_0,				// 室内湿度修正偏移量
	
	
	e_Hand_Health_1	=	2260,		// 工作模式
	
	e_HandReg_Number,				// 保持寄存器总个数
}eHandHoldingReg;

/******************************空气质量传感器*************************************/
typedef enum
{
	e_Air_Health_0	=	2820,	// 工作模式
	e_Air_Temp_0,				// 温度校正
	e_Air_Humi_0,				// 房间当前湿度
	e_Air_PM25_0,			//
	e_Air_PM10_0,			//
	e_Air_VOC_0,			//
	e_Air_CO2_0,			//
	e_Air_AirQualityState_0,		        //06-----	气体质量状态  bit0:PM2.5  bit1:PM10		 bit2:CO2			bit3:voc 
	e_Air_DevErrorCode_0,                //07-----	设备故障报警  bit0:PM2.5/PM10模块    bit1:CO2传感器     bit2:voc传感器     bit3:sht31
	e_Air_RGBState_0,			            //08-----	呼吸灯开关状态
	e_Air_PM25ValueOve_0,								//9-----	室内PM2.5超标值
	e_Air_PM10ValueOve_0,						    //10-----	室PM10超标值
	e_Air_CO2ValueOve_0,					        //11-----	室内二氧化碳超标值
	e_Air_VOCValueOve_0,									//12-----	室内VOC超标值
	e_Air_PM25Offset_0,							//13-----	室内PM2.5修正偏移量
	e_Air_PM10Offset_0,							//14-----	室内PM10修正偏移量
	e_Air_CO2Offset_0,							//15-----	室内二氧化碳修正偏移量
	e_Air_VOCOffset_0,							//16-----	室内VOC修正偏移量
	e_Air_WorkMode_0,										//17-----	室内工作模式
	e_Air_TemOffset_0,							//18-----	室内温度修正偏移量
	e_Air_HumOffset_0,							//19-----	室内湿度修正便宜量
	e_Air_Number1_0,											//20-----	预留1
	e_Air_Number2_0,											//21-----	预留2
	
//	e_Air_Health_1	=	2525,	// 工作模式
	
	e_AirReg_Number,				// 保持寄存器总个数
}eAirHoldingReg;


/******************************系统位号*************************************/
typedef enum
{
	e_RegBuf_ComHeal_1	=	0,		// 通讯健康值
	
	e_InputReg_Number	=	1,	// 输入存器总个数
}eInputReg;


void Data_Handle(void);
void modbus_init(void);

#endif
