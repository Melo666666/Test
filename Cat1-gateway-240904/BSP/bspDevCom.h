#ifndef _BSPDEVCOM_H_
#define _BSPDEVCOM_H_

#include "stm32f10x.h"
#include "bspPublic.h"


#define		DEV_NUM_MAX				8
#define		DEV_SLAVE_MIN_ADDR		1
#define		DEV_SLAVE_MAX_ADDR		8

#define		DEV_WRITE_NUM_MAX		13

#define 	DEV_RCV_LENGTH			(512)
#define 	DEV_SND_LENGTH			(1050)



typedef struct{
	u8 errCnt;
	u8 rcvFlag;
	u16 rcvLen;
	u8 rcvBuf[DEV_RCV_LENGTH];
	u16 sndLen;
	u8 sndBuf[DEV_SND_LENGTH];
	
}Dev_Com_Para_Struct;

typedef enum
{
	Opt_Write,
	Opt_Read,
}ctlCom_Opt;

typedef struct{
	u16 ComHealth;
	
//	s16 TempOffset;
//	s16 HumiOffset;
//	s16 OnOff;
//	s16 Mode;
//	s16 SetTemp;
//	s16 Temp;
//	s16 Humi;
//	s16 rsv1;
//	s16 rsv2;
//	s16 rsv3;
//	s16 rsv4;
//	s16 Alarm;
//	s16 PanID;
//	
//	
//	
//	u8 caliFlag;						// 面板温湿度校准标志;
//	u8 lastOnOff;						// 面板开关记忆;

}dev_para_struct;


#pragma pack (1)
typedef struct{
	uint8_t slaveAddr;
	uint8_t funcCode;
	uint8_t regNum;
	
	uint16_t startAddr;
	int16_t regVal[DEV_WRITE_NUM_MAX];
	
//	uint8_t sndLen;
//	uint8_t *sndBuf;
	
}dev_unit_struct;
#pragma pack()


/*********************直膨式新风除湿机**********************************/
/*
typedef enum
{
	e_InHoldingReg_CodeIAPFlag_1=0,
	e_InHoldingReg_CodeIAPFlag_2,	
	e_InHoldingReg_CodeIAPFlag_3,
	e_InHoldingReg_CodeIAPFlag_4,		
	e_InHoldingReg_ConfigFlag,  								//内机配置更新标识
	e_InHoldingReg_SWPUB,       								//内机软件版本号
	e_InHoldingReg_HWPUB,       								//内机硬件版本号
	e_InHoldingReg_DevType,     								//内机设备类型号		
	
	e_RegBuf_DevAddr,											    	//设备地址
  e_RegBuf_SetONOFF, 										      //系统开关  可设置  0
	e_RegBuf_SetAutoRun,												//等于0是自动 等于1是手动 2	
	e_RegBuf_SetMod,													  //模式设置  可设置  1
  e_RegBuf_SetEnergyConservationMod,					//节能模式
	e_RegBuf_SetSilenceMod,										  //静音模式
	e_RegBuf_SetWaterTempCon,									  //水温控制方式
	e_RegBuf_SetHeatWaterTemp,                  //设置制热水温
	e_RegBuf_SetCoolWaterTemp,                  //设置制冷水温
	e_RegBuf_SetAddHumEn,              				  //加湿使能
	e_RegBuf_SetAddHumAct,              				//加湿动作
	e_RegBuf_SetEvaporatorTemp,									//设置再热前温度
	e_RegBuf_SetAirSupplyTemp,						      //送风温度设置值	
  e_RegBuf_FANControl_Speed,   								//风速档位	 

	e_RegBuf_SetSGS1,														//吸气过热度1设置
	e_RegBuf_SetSGS2,														//吸气过热度2设置
	e_RegBuf_SetSGS3,														//吸气过热度3设置
	e_RegBuf_SetDGS1,														//排气过热度1设置
	e_RegBuf_SetDGS2,														//排气过热度2设置	
	e_RegBuf_SetDGS3,														//排气过热度3设置
	e_RegBuf_SetEEV2GS_Low,											//EEV2过热度调节下限
	e_RegBuf_SetEEV1GS_Low,											//EEV1过热度调节下限
	e_RegBuf_OpenEHeatHJTemp,										//开启电加热环境温度
	e_RegBuf_CloseEHeatHJTemp,									//关闭电加热环境温度
	
	//化霜
	e_RegBuf_DefrostingCheckTimeStart,					//化霜检测启动时间（制热模式下）
	e_RegBuf_DefrostingHJTemp1,					        //化霜判定环境温度1
	e_RegBuf_DefrostingHJTemp2,					        //化霜判定环境温度2
	e_RegBuf_DefrostingHJTemp3,					        //化霜判定环境温度3
	e_RegBuf_DefrostingWJPGTemp1,					      //化霜判定外机盘管温度1
	e_RegBuf_DefrostingWJPGTemp2,					      //化霜判定外机盘管温度2
	e_RegBuf_DefrostingWJPGTemp3,						    //化霜判定外机盘管温度3
	e_RegBuf_DefrostingWJPGTemp4,						    //化霜判定外机盘管温度4
	e_RegBuf_DefrostingCompressorFre,		//化霜压缩机频率
	e_RegBuf_DefrostingEEV1,							//化霜电子膨胀阀1开度
	e_RegBuf_DefrostingEEV2,							//化霜电子膨胀阀2开度	
	e_RegBuf_DefrostingEEV3,							//化霜电子膨胀阀3开度	
  e_RegBuf_DefrostingExitWGTemp,   //化霜退出外管温度
 	e_RegBuf_DefrostingCompressorFreRunMaxTime,   //化霜压缩机运行最大时间
 	e_RegBuf_DefrostingCompressorFreRunMinTime,   //化霜压缩机运行最小时间
	
  //回油 
	e_RegBuf_OilRecovery_CompressorFre,//回油频率60         	
	e_RegBuf_OilRecovery_IntervalTime,//间隔时间 单位分钟     
	e_RegBuf_OilRecovery_ActKeepTime, //回油保持时间 单位分钟 
	e_RegBuf_OilRecovery_EEV1SetVar,//回油时电子膨胀阀1开度
	e_RegBuf_OilRecovery_EEV2SetVar,//回油时电子膨胀阀2开度
	e_RegBuf_OilRecovery_EEV3SetVar,//回油时电子膨胀阀3开度
	
	//加湿
	e_RegBuf_AddWater_FZLowTime, 	//加水低限浮子超时时间
	e_RegBuf_AddWater_FZHighTime,	//加水高限浮子超时时间
	e_RegBuf_AddHum_FZLowTime,	//加湿低限浮子超时时间
	e_RegBuf_SubWater_FZLowTime,	//排水低限浮子超时时间
	e_RegBuf_CleanWaterTank_IntervalTime,//清洗水箱间隔时间
	e_RegBuf_CleanWaterTank_ActKeepTime,//清洗水箱持续时间
	e_RegBuf_CleanWaterTankAddWater_FZLowTime,//清洗加水低限浮子响应后额外加水时间
	e_RegBuf_AddHumStopErr_IntervalTimeTime,//加湿加水三次停机故障-加水间隔时间
	
	e_RegBuf_CompressorRun_MinIntervalTime,//压缩机启动最小间隔
	//滤网更换相关
	e_RegBuf_AirFilterUseTime,//客户已使用时间  	
	e_RegBuf_AirFilterConfigureMaxTime,//配置最大时间  
	e_RegBuf_WaterFlowSmallAlarmTempDiff,//水流量小报警温度差
	e_RegBuf_DevResonanceFrequency1, //设备共振频率1
  e_RegBuf_DevResonanceFrequency2, //设备共振频率2
	e_RegBuf_DevResonanceFrequency3, //设备共振频率3
  e_RegBuf_DevResonanceFrequency4, //设备共振频率4	
	
	e_RegBuf_EvaporatorTempNoOKTime,  //再热前温度不达标时间
	e_RegBuf_WaterTempNoOKTime,  //水热温度不达标时间
	e_RegBuf_WaterTempNoOKTempDt_Cool,      //水热温度不达标温度偏置制冷	
	e_RegBuf_WaterTempNoOKTempDt_Heat,      //水热温度不达标温度偏置制热	
	e_RegBuf_ZRQ_MAXTEMP_HeatModue,  //制热再热前最大设定温度
	e_RegBuf_ZRQ_MINTEMP_HeatModue,  //制热再热前最小设定温度
	e_RegBuf_HeatSongFengTempConEEV2ACTOffset,//制热送风温度调节EEV2允许动作范围偏移值
	e_RegBuf_SubHumInAirOKABSTemp_Val, //制冷再热前温度达标绝对值（EEV2进入过热度调节）
	e_RegBuf_SubHumAirOutOKABSTemp_Val, //制冷再热前温度不达标绝对值（EEV2进入温度调节）
	e_RegBuf_WaterInOKABSTemp_Val, //制冷水温达标绝对值（EEV1进入过热度调节）
	e_RegBuf_WaterOutOKABSTemp_Val, //制冷水温不达标绝对值（EEV1进入温度调节）
  e_RegBuf_DSTLow_Temp,						//排气过热度调节进入值
	e_RegBuf_DSTLow_OutTemp,				//排气过热度调节完成值
  e_RegBuf_CoolModEnergyConserInFDT6Temp, //制冷模式设备自节能板换防冻进入温度值（T6）
	e_RegBuf_CoolModEnergyConserOutFDT6Temp,//制冷模式设备自节能板换防冻结束温度正偏值（T6）
	e_RegBuf_CoolWaterTempEEV1Close,//制冷水温EEV1关闭值(T8)
	e_RegBuf_CoolWaterTempEEV1Open, //制冷水温EEV1打开值(T8)
	e_RegBuf_HeatWaterTempEEV1Close,//制热水温EEV1关闭值(T8)
	e_RegBuf_HeatWaterTempEEV1Open, //制热水温EEV1打开值(T8)
	
	e_RegBuf_NoEnergyConserModDevZJNFlag,   //非节能模式下设备自节能标识
	
	e_RegBuf_DevRunModStatus,			//机器运行模式
	e_RegBuf_DevRunModSubstatus,  //机器运行模式子状态
	
	e_RegBuf_InterMachEvapInTemp_T9,   //内机蒸发进口温度T9
	e_RegBuf_InterMachEvapOutTemp_T10,   //内机蒸发进口温度T10
  e_RegBuf_InterMachFanInTemp_T11,   //内机进风温度T11
	e_RegBuf_InterMachBeforeReheatTemp, //内机再热前温度
	e_RegBuf_InterMachAirSupplyTemp,	//内机送风温度	
	e_RegBuf_InterMachXHJSSTemp,	//内机循环加湿水温

	e_RegBuf_SGS,														//吸气过热度 
	e_RegBuf_DGS,														//排气过热度 	
	e_RegBuf_AddHumSwStatus,     //加湿电磁阀状态
	
  e_RegBuf_InterMachSensorErr1,
  e_RegBuf_InterMachSensorErr2,							//
	e_RegBuf_Err1,
	e_RegBuf_Err2,
	e_RegBuf_Err3_1,
	e_RegBuf_Err3_2,	
	
	e_RegBuf_InterMachFanEn,              //内部风机使能
	e_RegBuf_InterMachFanAct,             //内部风机动作
	
	e_RegBuf_BHEEV1SetVar,		//板换电子膨胀阀1设定值
	e_RegBuf_BHEEV1SetVarMax,	//板换电子膨胀阀1设定最大值	
	e_RegBuf_BHEEV1SetVarMin,	//板换电子膨胀阀1设定最小值
	e_RegBuf_BHEEV1SetInitVar,//板换电子膨胀阀1初始值
	
	e_RegBuf_SUBHUMEEV2SetVar,		  //除湿电子膨胀阀2设定值
	e_RegBuf_SUBHUMEEV2SetVarMax,	  //除湿电子膨胀阀2设定最大值	
	e_RegBuf_SUBHUMEEV2SetVarMin,	  //除湿电子膨胀阀2设定最小值
	e_RegBuf_SUBHUMEEV2SetInitVar,	//除湿电子膨胀阀2初始值
	
	e_RegBuf_AddHeatEEV3SetVar,		  //再热电子膨胀阀3设定值
	e_RegBuf_AddHeatEEV3SetVarMax,	//再热电子膨胀阀3设定最大值	
	e_RegBuf_AddHeatEEV3SetVarMin,	//再热电子膨胀阀3设定最小值
	e_RegBuf_AddHeatEEV3SetInitVar,	//再热电子膨胀阀3初始值
	
	e_RegBuf_MasterPumpCon,			//主水泵控制
	e_RegBuf_CondensateLiftPumpCon,  //冷凝水泵控制
	
  //***********紫外线杀菌控制*********************
	e_RegBuf_UltravioletSterilizationCon_En,       
	e_RegBuf_UltravioletSterilizationCon_ConFlag,	 

  //***********负离子控制*************************
	e_RegBuf_AnionCon_En,   
	e_RegBuf_AnionCon_ConFlag,

  e_RegBuf_EnterZRQYXWC,  //进入再热前允许温差
	e_RegBuf_EnterSWYXWC1,   //进入水温允许温差1
	e_RegBuf_EnterSWYXWC2,   //进入水温允许温差2	
	e_RegBuf_ExitZRQWC1,   //退出再热前温差1
	e_RegBuf_ExitZRQWC2,   //退出再热前温差2	
	e_RegBuf_HeatZRQTempSetVar,   //制热再热前温度目标
	e_RegBuf_HeatSFTempSetVar_High,   //制热送风温度上限
	e_RegBuf_HeatZRQTempSetVar_Low,   //制热再热前温度下限
	e_RegBuf_XHADDHumWaterTempSetVar,   //循环加湿水温目标
	
	e_RegBuf_WaterTempSamplingTime,   //水温采样间隔时间
	e_RegBuf_EvaporatorTempSamplingTime,//再热前温度采样时间
	e_RegBuf_ZRQ_CheckStartTime,        //再热前温度采样开始时间
	e_RegBuf_AirSupplyTempSamplingTime, //送风温度采样时间
	e_RegBuf_SFT_CheckStartTime,				//送风温度采样开始时间
	e_RegBuf_SGS_DGSSamplingTime,       //吸排气过热度采样时间
	e_RegBuf_Sgs_Dgs_Max_CheckStartTime, //吸排气过热度开始检测时间
	e_RegBuf_HighPsPretectCompresser_CheckIntervalTime,//高压保护压缩机检测间隔
	e_RegBuf_LowPsPretectCompresser_CheckIntervalTime,//低压保护压缩机检测间隔
	e_RegBuf_ClrElecExpanCumuErr_IntervalTime, //电子膨胀阀静态误差消除
	e_RegBuf_OutFanSpeed_CheckStartTime, //外部风机检测开始时间
	e_RegBuf_OutFanSpeed_CheckIntervalTime,//外部风机检测压力时间
	e_RegBuf_WaterTempIntegralEnSubVal, //水温积分有效温差范围
	e_RegBuf_EvaporatorTempIntegralEnSubVal,//再热前温度积分有效温差范围
	e_RegBuf_AirSupplyTempIntegralEnSubVal, //送风温度积分有效温差范围
	e_RegBuf_SGS_DGS_IntegralEnSubVal,       //吸排气过热度积分有效温差范围
	e_RegBuf_OutFanSpeed_IntegralEnSubVal,    //外部分机转速积分值有效范围
	e_RegBuf_CompressorFreMax,     			//压缩机频率最大值  
	e_RegBuf_CompressorFreMin,     			//压缩机频率最小值  
	
	e_RegBuf_OutFanSpeed_CoolHighPressVar,//外部风机制冷转速高压目标值
	e_RegBuf_OutFanSpeed_HeatLowPressVar,//外部风机制热转速低压目标值
	e_RegBuf_OutFanSpeed_CoolHJTemp, //外部风机制冷变速点环温
	e_RegBuf_OutFanSpeed_HeatHJTemp, //外部风机制热变速点环温
	e_RegBuf_OutFanSpeed_InitVar1, //外部风机初始转速1
  e_RegBuf_OutFanSpeed_InitVar2, //外部风机初始转速2
  e_RegBuf_OutFanSpeed_Max, //外部风机转速最大值
  e_RegBuf_OutFanSpeed_Min, //外部风机转速最小值

//#ifdef BACKFANSYS
  e_RegBuf_AirSensorComHealth,//外机通讯健康值
	e_RegBuf_AirSensorBackFanTemp,//回风温度
	e_RegBuf_AirSensorBackFanHum,//回风湿度
	e_RegBuf_AirSensorBack25,//回风PM2.5
  e_RegBuf_AirSensorBack10,//回风PM10
	e_RegBuf_AirSensorBackCO2,//回风CO2
	e_RegBuf_AirSensorBackSWPub,//软件版本号
	e_RegBuf_AirSensorBackHWPub,//硬件版本号	
	e_RegBuf_AirSensorBackDevType,//设备型号
	e_RegBuf_AirSensorBackFanTemp_OfferSet,//回风温度校正值
	e_RegBuf_AirSensorBackFanHum_OfferSet,//回风湿度校正值
	e_RegBuf_AirSensorBack25_OfferSet,//回风PM2.5校正值
  e_RegBuf_AirSensorBack10_OfferSet,//回风PM10校正值
	e_RegBuf_AirSensorBackCO2_OfferSet,//回风CO2	校正值
	
	e_RegBuf_Sw_NewFan_Min, //新风阀最小开度
	e_RegBuf_Sw_NewFan_Max, //新风阀最大开度
	e_RegBuf_Sw_NewFan_Init,//新风阀初始值
	e_RegBuf_Sw_NewFan_NowVal,//新风阀开度
	e_RegBuf_Sw_BackFan_Min, //回风阀最小开度
	e_RegBuf_Sw_BackFan_Max, //回风阀最大开度
	e_RegBuf_Sw_BackFan_Init,//回风阀初始值
	e_RegBuf_Sw_BackFan_NowVal,//回风阀开度
	e_RegBuf_FreshAirVolume,//新风量
	e_RegBuf_FreshAirSpeed, //新风风速
	e_RegBuf_FreshAirCrossSectionalArea,//新风截面积
	e_RegBuf_FreshAirVolumeCorrectionCoefficient ,//新风量校正系数
	e_RegBuf_FanTempModCtlr,//风温控制方式选择
	e_RegBuf_Reserve1, 
	e_RegBuf_Reserve2, 	
	e_RegBuf_Reserve3, 		
	e_RegBuf_Reserve4, 		
//#endif
	
//*****************外机参数************************************
	e_OutHoldingReg_ComHealth,//外机通讯健康值
	e_OutHoldingReg_XQT_T1, //压缩机吸气温度
	e_OutHoldingReg_PQT_T2, //压缩机排气温度
	e_OutHoldingReg_LNQPGT_T3,//冷凝器盘管温度
	e_OutHoldingReg_ORT_T4, //室外环境温度
	e_OutHoldingReg_BHJKLM_T5,//板换进口冷媒温度
	e_OutHoldingReg_BHCKLM_T6,//板换出口冷媒温度
	e_OutHoldingReg_BHCS_T7,  //板换出水温度
	e_OutHoldingReg_BHJS_T8,  //板换进水温度
	e_OutHoldingReg_NoUse_T,  //保留未使用温度传感器
	e_OutHoldingReg_XQP,      //吸气压力
	e_OutHoldingReg_PQY,      //排气压力
	e_OutHoldingReg_HPS,      //高压开关
	e_OutHoldingReg_SYYL,      //水压压力

	e_OutHoldingReg_NowFre,      //压缩机当前频率
	e_OutHoldingReg_EEV1_NowStep,//电子膨胀阀当前值
	e_OutHoldingReg_EEV2_NowStep,//电子膨胀阀2当前值
	e_OutHoldingReg_FAN1_Speed, //风机1转速
	e_OutHoldingReg_FAN2_Speed, //风机2转速
	e_OutHoldingReg_MasterPumpStatus,//主水泵状态
	
	e_OutHoldingReg_SensorErr1,  //传感器故障1
	e_OutHoldingReg_SensorErr2,  //传感器故障2	
	e_OutHoldingReg_Err1,       //故障1
	e_OutHoldingReg_BPQ_Err1,       //变频器故障1
	e_OutHoldingReg_BPQ_Err2,       //变频器故障2
	e_OutHoldingReg_BPQ_Err3,       //变频器故障3
	
	e_OutHoldingReg_SetDevType,        //外机类型
	e_OutHoldingReg_SetDevStatus,        //外机状态
	
	e_OutHoldingReg_SetFre,         //频率设定值
	e_OutHoldingReg_SetSTHXF_ONFF,  //四通换向阀
	e_OutHoldingReg_SetEEV1Step,  //设置电子膨胀阀开度
	e_OutHoldingReg_SetEEV2Step,  //设置电子膨胀阀2开度
	e_OutHoldingReg_SetOutFanSpeed, //设置外部风扇速度
	e_OutHoldingReg_SetYSJDJR_ONOFF, //压缩机电加热开关
	e_OutHoldingReg_SetMasterPumpStatus,//设置主水泵状态
	e_OutHoldingReg_ErrClear,					//故障复位
	
	e_OutHoldingReg_InverterModel,    //变频器型号
	e_OutHoldingReg_CompressorModel,  //压缩机型号
	e_OutHoldingReg_UPDATAFLAG,  //升级标识
	e_OutHoldingReg_ConfigFlag,  //配置更新标识
	e_OutHoldingReg_SWPUB,       //软件版本号
	e_OutHoldingReg_HWPUB,       //硬件版本号
	e_OutHoldingReg_DevType,     //设备类型号	
			
//	e_RegBuf_Defalut,
	
	
	e_DevHoldReg_Number,			// 保持寄存器总个数
	
	
}eDeviceHoldingReg;
*/

//extern u16 usRegHoldBufPanel[32][e_PanelHoldReg_Number];
//extern panel_para_struct Panel_Para[PANEL_NUM_MAX];
//extern MainScreen_struct MainScreen_para;
void Dev_485_Config(void);
void Dev_Snd_Buf(u8 *sndBuf, u16 sndLen);

void Dev_TX_Service(dev_unit_struct *tempUnit);
u8 Dev_RX_Service(dev_unit_struct tempUnit);

#endif

