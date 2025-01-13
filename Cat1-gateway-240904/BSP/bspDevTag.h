#ifndef _BSPDEVTAG_H_
#define _BSPDEVTAG_H_

#include "stm32f10x.h"

typedef enum
{
	e_Double_Cold,		// 双冷源
	e_Heat_Pumb,		// 热泵新风一体机/新风除湿机
	e_Air_Dehumi,		// 新风除湿机带回风
	
}Dev_Type_Struct;

typedef enum
{
	e_Dev_Gateway=1,
	e_Air_Controller,

}Gateway_Type_Struct;

/******************************系统位号*************************************/
typedef enum
{
/*********************直膨式新风除湿机(带回风)**********************************/	
		//
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

typedef enum  
{
/*********************双冷源一体机*************************/	
	e_Double_ONOFF=0, 										  //系统开关  可设置  0
	e_Double_Mod,													  //模式设置  可设置  1
	e_Double_AutoRun,												//等于0是自动 等于1是手动 2

	e_Double_SetEvaporatorTemp,							//再热前温度设置值  3
	e_Double_EvaporatorTemp,						  	//再热前温度     4
	e_Double_EvaporatorTempFreMin,     			//再热前压缩机频率最小值  5
	e_Double_EvaporatorTempFreMax,     			//再热前压缩机频率最大值  6
	e_Double_EvaporatorTempFreIntegralEnSubVal,	//再热前温度积分有效温差范围	7
	e_Double_EvaporatorTempFrePIDSamplingTime,            //PID采样时间	 8

	e_Double_SetAirSupplyTemp,						//送风温度设置值	9

	e_Double_AirSupplyTemp,						    //10送风温度	 10 A
	e_Double_AirSupplyTempStepEEVMin,     // 送风温度阀最小开度 电子膨胀阀注意是电子或者水阀   11 B  
	e_Double_AirSupplyTempStepEEVMax,     //送风温度阀最大开度  12   C
	e_Double_AirSupplyTempStepSetVal,     //送风温度阀当前开度值 13  D
	e_Double_AirSupplyTempEEVIntegralEnSubVal,//送风温度积分有效温差范围	14  E
	e_Double_AirSupplyTempEEVPIDSamplingTime, //PID采样时间 15  F

	e_Double_AirSupplyHum,						  	//送风湿度	16  10

	e_Double_HuangjingTemp,								//环境温度	17  11
	e_Double_HuangjingHum,								//环境湿度	18  12

	//*****************************吸排气过热度************************************************** 	

	//*****************************吸气过热度************************************************** 
	e_Double_SGS, 												      //吸气过热度	                             19
	e_Double_SetSGS, 												    // 20 设定的吸气过热度	 	                 20

	e_Double_SGS_EEVStepMin, 										//吸气过热度阀开度最小值                   21
	e_Double_SGS_EEVStepMax, 										//吸气过热度阀开度最大值                   22
	e_Double_SGS_EEVStepSetVal,                 //吸气过热度阀当前值                       23
	e_Double_SGS_EEVIntegralEnSubVal,						//吸气过热度积分有效温差范围	             24

	//*****************************排气过热度************************************************** 
	e_Double_DGS, 															//排气过热度		25
	e_Double_SetDGSMin, 												//设定的排气过热度最小值(在吸气过热度调节模式下监控)  26
	e_Double_SetDGSMax, 												//设定的排气过热度最小值（在排气过热度调节模式下监控）27

	e_Double_InverterSetOnOFF,		 					// 30 设置开关             28
	e_Double_InverterSetFre,		 						//设置频率                 29
	e_Double_InverterFreNow,								//当前频率                 30
	e_Double_InverterErrBuf1,								//变频器故障1              31
	e_Double_InverterErrBuf2,								//变频器故障2	             32
	e_Double_InverterErrBuf3,								//变频器故障2	  35         33

	e_Double_InverterType,										//变频器类型             34
	//**************************高低压压力********************************************

	//低压压力
	e_Double_LPS_Value,								//低压压力   35

	//高压压力
	e_Double_HPS_Value,	 //                        36

	e_Double_HighPsPretectCompresser_CheckIntervalTime,			//压缩机间隔检测时间 37
	e_Double_HighPsPretectWaterValve_CheckIntervalTime,			//水阀调节时间       38
	e_Double_HighPsPretectCompresser_SubVal,
	e_Double_HighPsPretectWaterValve_SubVal,
	e_Double_HPS_STOP_COMPERSSER_VAL,												//#define HPS_STOP_COMPERSSER_VAL 400 //4Mpa  系统停机最大压力  39
	e_Double_HPS_SUBFRE_COMPERSSER_VAL,											//#define HPS_SUBFRE_COMPERSSER_VAL 350 //3.5Mpa 压缩机降频压力 40
	e_Double_HPS_STOPSUBFRE_COMPERSSER_VAL,                 //#define HPS_STOPSUBFRE_COMPERSSER_VAL 300 //3.0Mpa 压缩机回归正常逻辑41
	e_Double_LPS_STOP_COMPERSSER_VAL,												//#define LPS_STOP_COMPERSSER_VAL  10  //1.0bar 42
	e_Double_LPS_ADDEEV1_VAL,																//#define LPS_ADDEEV1_VAL   40         //4.0bar 43

	//******************送风温度*****************************************************************************	
	e_Double_SFT_CheckIntervalTime,			//单位 秒 默认1s  44
	e_Double_SFT_CheckStartTime,				  //单位 秒 默认0 45

	//******************在热前温度*****************************************************************************
	e_Double_ZRQ_CheckIntervalTime,			//单位 秒 默认1s  46
	e_Double_ZRQ_CheckStartTime,				  //单位 秒 默认0 47

	//******************吸排气过热度************************************************************************ 
	e_Double_Sgs_Dgs_Max_CheckIntervalTime,			//单位 秒 默认1s 48
	e_Double_Sgs_Dgs_Max_CheckStartTime,				  //单位 秒 默认0 49

	//******************吸排气温度************************************************************************
	//吸气温度
	e_Double_sgt_Temp,										//压缩机吸气温度	           50


	//排气温度
	e_Double_dgt_Temp,										//50压缩机排气温度           51

	//*******************报警故障*************************************************************

	//------------------需重启的报警事件----------------------------------------------
	//1.吸气过热度过低
	//2.吸气过热度过高
	//3.排气过热度过低
	//4.吸气低压过低
	//5.排气压力过高
	////6.吸气温度过低
	//7.排气温度过高
	//8.风机故障报警
	//9.变频器故障             【故障停机】
	e_Double_NeedReset,//                                           52
	//------------------不需重启的报警事件----------------------------------------------
	//1.排气温度传感器故障     【需要用到压缩机的模式】
	//2.吸气温度传感器故障     【需要用到压缩机的模式】
	//3.高压压力传感器故障 	   【需要用到压缩机的模式】
	//4.低压压力传感器故障     【需要用到压缩机的模式】

	//5.再热前温度传感器故障   【需要除湿的模式：制冷、除湿模式】过蒸发器后温度

	//6.主控器通讯故障         【故障停机】
	//7.吸气温度过低           【故障停机】恢复正常运行
	//8.防冻保护故障           【保护或者故障停机】
	e_Double_OFFNoResetAlarmEvt,//                                 53

	//------------------不需停机的报警事件----------------------------------------------
	//1.风压开关报警			 【故障提示】//滤网更换
	//2.环境温度传感器         【故障提示】//
	//3.环境湿度传感器         【故障提示】//
	//4.送风温度传感器         【故障提示】//
	//5.送风湿度传感器         【故障提示】//
	//6.预冷盘管温度传感器     【故障提示】//
	//7.滤网更换报警           【故障提示】//
	//8.积水盘水位过高报警     【故障提示】
	//9.加湿器故障
	e_Double_NOOFFAlarmEvt, //                                    54

	//滤网更换
	e_Double_AirFilterConfigureMaxTime,//配置最大时间  55
	e_Double_AirFilterUseTime,//客户已使用最大时间  56
	e_Double_AirFilterConfigureMaxPress,//配置的失效压差  57
	e_Double_AirFilterPress,//压差  58


	//********************加湿控制*********************************
	e_Double_HumidificationControl_En,  //59
	e_Double_HumidificationControl_ActFlag,//60 

	//****************风机控制*************************************
	e_Double_FANControl_En,//61
	e_Double_FANControl_ActFlag,//62

	e_Double_FANControl_Speed,   //风速档位	63

	e_Double_FANControl_VoltageVal_MinLow,			//最小档风量电压值	64
	e_Double_FANControl_VoltageVal_Low,			    //小档风量电压值    65
	e_Double_FANControl_VoltageVal_Med,			    //中档风量电压值    66
	e_Double_FANControl_VoltageVal_High,			  //高档风量电压值    67
	e_Double_FANControl_VoltageVal_MaxHigh,			  //最高档风量电压值 68

	//***********紫外线杀菌控制*********************
	e_Double_UltravioletSterilizationCon_En,      //69
	e_Double_UltravioletSterilizationCon_ConFlag,	//70

	//***********负离子控制*************************
	e_Double_AnionCon_En,  //71
	e_Double_AnionCon_ConFlag,						//72

	//**********电加热控制**************************
	e_Double_ElectricHeatingEn,           //电加热使能	73
	e_Double_ElectricHeating_ConFlag,			//电加热控制标识 74

	//防冻控制
	e_Double_AntiFreezing_En,
	e_Double_AntiFreezing_ActFlag,								//动作标识  75
	e_Double_AntiFreezing_StartTemp,							//动作温度  默认：5摄氏度 76
	e_Double_AntiFreezing_EndTemp,							//结束温度  默认：10摄氏度  77

	//回油控制
	e_Double_OilRecovery_IntervalTime,//间隔时间 单位分钟    78
	e_Double_OilRecovery_ActKeepTime, //回油保持时间 单位分钟 79
	e_Double_OilRecovery_CompressorFre,//回油频率60         80

	//电子膨胀阀累积误差消除

	e_Double_ClrElecExpanCumuErr_IntervalTime,//间隔时间 单位分钟  81

	e_Double_Wait_DGSOK_IntervalFre,           //等待排气过热度合格压缩机频率 82 
	e_Double_HeatSetEvaporatorTemp,						 //再热前温度设置值  制热模式  83
	e_Double_HeatSetAirSupplyTemp,						 //送风温度设置值	   制热模式  84

	e_Double_IAPBootFlag,											 //
	e_Double_WaterFlowRate,										 //水流量

	e_Double_AirSupplyTempStepEEVMin_Heat,         //制热_送风温度阀最小开度 电子膨胀阀注意是电子或者水阀   
	e_Double_AirSupplyTempStepEEVMax_Heat,         //制热_送风温度阀最大开度  
	e_Double_AirSupplyTempStepSetVal_Heat,         //制热_送风温度阀当前开度值 
	e_Double_AirSupplyTempEEVIntegralEnSubVal_Heat,//制热_送风温度积分有效温差范围		
	//**************************机型*********************************************
	//制热模式：预冷盘管前pid水阀调节送风温度 关闭板换pid水阀
	//制冷模式: 全开预冷盘管前pid水阀，通过板换pid水阀调节送风温度
	//300  风量 1
	//500  风量 2
	//750  风量 3

	//制冷制热模式：开关量三通阀（集合水路作用）+板换pid水阀结合调节送风温度
	//1000 风量 4
	//1500 风量 5
	//2000 风量 6
	e_Double_DevConPub,												     //三恒系统一体机型号

	e_Double_CodePub,													     //程序版本号

	e_Double_IAPSerNumMem,	

	e_Double_HumAddWaterMaxTime, //加湿加水最大时间

	e_Double_HumHighFuziHighToLowTime, //加湿高限浮子落下时间

	e_Double_HumPumpProtectTime,       //加湿泵保护时间

	e_Double_HumErrFlag,               //加湿故障处理标识 默认0

	e_Double_HumSubWaterIntervalTime, //加湿排水间隔 分钟

	e_Double_DevAddr, //设备地址

//	#ifdef Add_SysWaterTempAndDevPGOutTemp	
	e_Double_InWaterTemp,   //一体机进水温度 单位：摄氏度 精确到0.1 只读

	e_Double_YLPGTempStepEEVMin_Cool,         //预冷调节阀最小开度   默认45.0 读写
	e_Double_YLPGTempStepEEVMax_Cool,         //预冷调节阀最大开度  默认90.0  读写
	e_Double_YLPGTempStepSetVal_Cool,         //预冷调节阀当前开度值   读写
	e_Double_YLPGTempEEVIntegralEnSubVal_Cool,//制冷再热前温度积分有效温差范围	默认0.5 摄氏度  读写	
	e_Double_ZRQTemp1_SubHumErr_TZ1, //再热前温度过高保护值 1 Tz1 默认16.0  读写
	e_Double_ZRQTemp2_SubHumErr_TZ2, 	//再热前温度过高保护值2 Tz2  默认18.0 读写
	e_Double_IWTTemp_SubHumOK_TSG,	 //除湿能力不足报警满足水温Tsg 默认25.0 读写
	e_Double_ZRQTemp_SubHumOKErr_T3,	 //除湿能力不足报警满足持续时间 默认30 单位：分钟 读写

	e_Double_SWToYSJWaitTime_T1_Cool,    //持续制冷退出预冷调节阀控制再热前温度持续时间  T1 默认 2分钟 读写
	e_Double_YSJToSWWaitTime_T2_Cool,    //再热前温度过冷持续时间  T1 默认 30分钟 读写
	e_Double_SGSMinVal_Txg,              //吸气过热度低限值 单位摄氏度 默认 1.0 读写
	e_Double_SGTProtectVal_Ts1,          //吸气温度保护值Ts1 单位摄氏度 默认4.0 读写
	e_Double_SGTProtectMinVal_Ts2,       //吸气温度极限值Ts2  单位摄氏度 默认 -2.0 读写
	e_Double_Down1HZSecond_Txjp,              //降1hz时间 单位s 默认2 读写

	//e_Double_DownFanVoltagMinute,        //风机风速电压每分钟下降
//	#endif
	e_Double_FanDevTypeCon,               //风机与设备类型配置匹配 0：老机型施乐百 1：性能优化版机型博乐 2：性能优化版施乐百

	e_Double_CodeIAPFlag_1,
	e_Double_CodeIAPFlag_2,	
	e_Double_CodeIAPFlag_3,
	e_Double_CodeIAPFlag_4,	
	e_Double_ConfigFlag,  								//内机配置更新标识	

	e_Double_OutWaterTemp,           //出水温度
	e_Double_WaterTempDifVar_Err,    //水温差告警值 
	e_Double_AirPreDifProtectEn,     //空气压差防漏水保护使能 1有效
	e_Double_AirPreDifProtectDevType,    //空气压差设备类型 0：负压 1：正压
	e_Double_AirPreDifProtectVal,    		//压差上限值
	e_Double_ActFanSpeed,               //实际风扇转速
	e_Double_ActFanSpeedErrValue,       //制冷风机风速过低故障值 三次停机

	e_DevDoubleHoldReg_Number,
	
}e_DoubleEnum;



typedef enum  
{
/*********************直膨式新风除湿机、热泵新风一体机*************************/
	//
	e_HeatPumpAir_CodeIAPFlag_1=0,
	e_HeatPumpAir_CodeIAPFlag_2,	
	e_HeatPumpAir_CodeIAPFlag_3,
	e_HeatPumpAir_CodeIAPFlag_4,		
	e_HeatPumpAir_ConfigFlag,  								//内机配置更新标识
	e_HeatPumpAir_SWPUB,       								//内机软件版本号
	e_HeatPumpAir_HWPUB,       								//内机硬件版本号
	e_HeatPumpAir_DevType,     								//内机设备类型号		

	e_HeatPumpAir_DevAddr,											    	//设备地址
	e_HeatPumpAir_SetONOFF, 										      //系统开关  可设置  0
	e_HeatPumpAir_SetAutoRun,												//等于0是自动 等于1是手动 2	
	e_HeatPumpAir_SetMod,													  //模式设置  可设置  1
	e_HeatPumpAir_SetEnergyConservationMod,					//节能模式
	e_HeatPumpAir_SetSilenceMod,										  //静音模式
	e_HeatPumpAir_SetWaterTempCon,									  //水温控制方式
	e_HeatPumpAir_SetHeatWaterTemp,                  //设置制热水温
	e_HeatPumpAir_SetCoolWaterTemp,                  //设置制冷水温
	e_HeatPumpAir_SetAddHumEn,              				  //加湿使能
	e_HeatPumpAir_SetAddHumAct,              				//加湿动作
	e_HeatPumpAir_SetEvaporatorTemp,									//设置再热前温度
	e_HeatPumpAir_SetAirSupplyTemp,						      //送风温度设置值	
	e_HeatPumpAir_FANControl_Speed,   								//风速档位	 

	e_HeatPumpAir_SetSGS1,														//吸气过热度1设置
	e_HeatPumpAir_SetSGS2,														//吸气过热度2设置
	e_HeatPumpAir_SetSGS3,														//吸气过热度3设置
	e_HeatPumpAir_SetDGS1,														//排气过热度1设置
	e_HeatPumpAir_SetDGS2,														//排气过热度2设置	
	e_HeatPumpAir_SetDGS3,														//排气过热度3设置
	e_HeatPumpAir_SetEEV2GS_Low,											//EEV2过热度调节下限
	e_HeatPumpAir_SetEEV1GS_Low,											//EEV1过热度调节下限
	e_HeatPumpAir_OpenEHeatHJTemp,										//开启电加热环境温度
	e_HeatPumpAir_CloseEHeatHJTemp,									//关闭电加热环境温度

	//化霜
	e_HeatPumpAir_DefrostingCheckTimeStart,					//化霜检测启动时间（制热模式下）
	e_HeatPumpAir_DefrostingHJTemp1,					        //化霜判定环境温度1
	e_HeatPumpAir_DefrostingHJTemp2,					        //化霜判定环境温度2
	e_HeatPumpAir_DefrostingHJTemp3,					        //化霜判定环境温度3
	e_HeatPumpAir_DefrostingWJPGTemp1,					      //化霜判定外机盘管温度1
	e_HeatPumpAir_DefrostingWJPGTemp2,					      //化霜判定外机盘管温度2
	e_HeatPumpAir_DefrostingWJPGTemp3,						    //化霜判定外机盘管温度3
	e_HeatPumpAir_DefrostingWJPGTemp4,						    //化霜判定外机盘管温度4
	e_HeatPumpAir_DefrostingCompressorFre,		//化霜压缩机频率
	e_HeatPumpAir_DefrostingEEV1,							//化霜电子膨胀阀1开度
	e_HeatPumpAir_DefrostingEEV2,							//化霜电子膨胀阀2开度	
	e_HeatPumpAir_DefrostingEEV3,							//化霜电子膨胀阀3开度	
	e_HeatPumpAir_DefrostingExitWGTemp,   //化霜退出外管温度
	e_HeatPumpAir_DefrostingCompressorFreRunMaxTime,   //化霜压缩机运行最大时间
	e_HeatPumpAir_DefrostingCompressorFreRunMinTime,   //化霜压缩机运行最小时间

	//回油 
	e_HeatPumpAir_OilRecovery_CompressorFre,//回油频率60         	
	e_HeatPumpAir_OilRecovery_IntervalTime,//间隔时间 单位分钟     
	e_HeatPumpAir_OilRecovery_ActKeepTime, //回油保持时间 单位分钟 
	e_HeatPumpAir_OilRecovery_EEV1SetVar,//回油时电子膨胀阀1开度
	e_HeatPumpAir_OilRecovery_EEV2SetVar,//回油时电子膨胀阀2开度
	e_HeatPumpAir_OilRecovery_EEV3SetVar,//回油时电子膨胀阀3开度

	//加湿
	e_HeatPumpAir_AddWater_FZLowTime, 	//加水低限浮子超时时间
	e_HeatPumpAir_AddWater_FZHighTime,	//加水高限浮子超时时间
	e_HeatPumpAir_AddHum_FZLowTime,	//加湿低限浮子超时时间
	e_HeatPumpAir_SubWater_FZLowTime,	//排水低限浮子超时时间
	e_HeatPumpAir_CleanWaterTank_IntervalTime,//清洗水箱间隔时间
	e_HeatPumpAir_CleanWaterTank_ActKeepTime,//清洗水箱持续时间
	e_HeatPumpAir_CleanWaterTankAddWater_FZLowTime,//清洗加水低限浮子响应后额外加水时间
	e_HeatPumpAir_AddHumStopErr_IntervalTimeTime,//加湿加水三次停机故障-加水间隔时间

	e_HeatPumpAir_CompressorRun_MinIntervalTime,//压缩机启动最小间隔
	//滤网更换相关
	e_HeatPumpAir_AirFilterUseTime,//客户已使用时间  	
	e_HeatPumpAir_AirFilterConfigureMaxTime,//配置最大时间  
	e_HeatPumpAir_WaterFlowSmallAlarmTempDiff,//水流量小报警温度差
	e_HeatPumpAir_DevResonanceFrequency1, //设备共振频率1
	e_HeatPumpAir_DevResonanceFrequency2, //设备共振频率2
	e_HeatPumpAir_DevResonanceFrequency3, //设备共振频率3
	e_HeatPumpAir_DevResonanceFrequency4, //设备共振频率4	

	e_HeatPumpAir_EvaporatorTempNoOKTime,  //再热前温度不达标时间
	e_HeatPumpAir_WaterTempNoOKTime,  //水热温度不达标时间
	e_HeatPumpAir_WaterTempNoOKTempDt_Cool,      //水热温度不达标温度偏置制冷	
	e_HeatPumpAir_WaterTempNoOKTempDt_Heat,      //水热温度不达标温度偏置制热	
	e_HeatPumpAir_ZRQ_MAXTEMP_HeatModue,  //制热再热前最大设定温度
	e_HeatPumpAir_ZRQ_MINTEMP_HeatModue,  //制热再热前最小设定温度
	e_HeatPumpAir_HeatSongFengTempConEEV2ACTOffset,//制热送风温度调节EEV2允许动作范围偏移值
	e_HeatPumpAir_SubHumInAirOKABSTemp_Val, //制冷再热前温度达标绝对值（EEV2进入过热度调节）
	e_HeatPumpAir_SubHumAirOutOKABSTemp_Val, //制冷再热前温度不达标绝对值（EEV2进入温度调节）
	e_HeatPumpAir_WaterInOKABSTemp_Val, //制冷水温达标绝对值（EEV1进入过热度调节）
	e_HeatPumpAir_WaterOutOKABSTemp_Val, //制冷水温不达标绝对值（EEV1进入温度调节）
	e_HeatPumpAir_DSTLow_Temp,						//排气过热度调节进入值
	e_HeatPumpAir_DSTLow_OutTemp,				//排气过热度调节完成值
	e_HeatPumpAir_CoolModEnergyConserInFDT6Temp, //制冷模式设备自节能板换防冻进入温度值（T6）
	e_HeatPumpAir_CoolModEnergyConserOutFDT6Temp,//制冷模式设备自节能板换防冻结束温度正偏值（T6）
	e_HeatPumpAir_CoolWaterTempEEV1Close,//制冷水温EEV1关闭值(T8)
	e_HeatPumpAir_CoolWaterTempEEV1Open, //制冷水温EEV1打开值(T8)
	e_HeatPumpAir_HeatWaterTempEEV1Close,//制热水温EEV1关闭值(T8)
	e_HeatPumpAir_HeatWaterTempEEV1Open, //制热水温EEV1打开值(T8)

	e_HeatPumpAir_NoEnergyConserModDevZJNFlag,   //非节能模式下设备自节能标识

	e_HeatPumpAir_DevRunModStatus,			//机器运行模式
	e_HeatPumpAir_DevRunModSubstatus,  //机器运行模式子状态

	e_HeatPumpAir_InterMachEvapInTemp_T9,   //内机蒸发进口温度T9
	e_HeatPumpAir_InterMachEvapOutTemp_T10,   //内机蒸发进口温度T10
	e_HeatPumpAir_InterMachFanInTemp_T11,   //内机进风温度T11
	e_HeatPumpAir_InterMachBeforeReheatTemp, //内机再热前温度
	e_HeatPumpAir_InterMachAirSupplyTemp,	//内机送风温度	
	e_HeatPumpAir_InterMachXHJSSTemp,	//内机循环加湿水温

	e_HeatPumpAir_SGS,														//吸气过热度 
	e_HeatPumpAir_DGS,														//排气过热度 	
	e_HeatPumpAir_AddHumSwStatus,     //加湿电磁阀状态

	e_HeatPumpAir_InterMachSensorErr1,
	e_HeatPumpAir_InterMachSensorErr2,							//
	e_HeatPumpAir_Err1,
	e_HeatPumpAir_Err2,
	e_HeatPumpAir_Err3_1,
	e_HeatPumpAir_Err3_2,

	e_HeatPumpAir_InterMachFanEn,              //内部风机使能
	e_HeatPumpAir_InterMachFanAct,             //内部风机动作

	e_HeatPumpAir_BHEEV1SetVar,		//板换电子膨胀阀1设定值
	e_HeatPumpAir_BHEEV1SetVarMax,	//板换电子膨胀阀1设定最大值	
	e_HeatPumpAir_BHEEV1SetVarMin,	//板换电子膨胀阀1设定最小值
	e_HeatPumpAir_BHEEV1SetInitVar,//板换电子膨胀阀1初始值

	e_HeatPumpAir_SUBHUMEEV2SetVar,		  //除湿电子膨胀阀2设定值
	e_HeatPumpAir_SUBHUMEEV2SetVarMax,	  //除湿电子膨胀阀2设定最大值	
	e_HeatPumpAir_SUBHUMEEV2SetVarMin,	  //除湿电子膨胀阀2设定最小值
	e_HeatPumpAir_SUBHUMEEV2SetInitVar,	//除湿电子膨胀阀2初始值

	e_HeatPumpAir_AddHeatEEV3SetVar,		  //再热电子膨胀阀3设定值
	e_HeatPumpAir_AddHeatEEV3SetVarMax,	//再热电子膨胀阀3设定最大值	
	e_HeatPumpAir_AddHeatEEV3SetVarMin,	//再热电子膨胀阀3设定最小值
	e_HeatPumpAir_AddHeatEEV3SetInitVar,	//再热电子膨胀阀3初始值

	e_HeatPumpAir_MasterPumpCon,			//主水泵控制
	e_HeatPumpAir_CondensateLiftPumpCon,  //冷凝水泵控制

	//***********紫外线杀菌控制*********************
	e_HeatPumpAir_UltravioletSterilizationCon_En,       
	e_HeatPumpAir_UltravioletSterilizationCon_ConFlag,	 

	//***********负离子控制*************************
	e_HeatPumpAir_AnionCon_En,   
	e_HeatPumpAir_AnionCon_ConFlag,

	e_HeatPumpAir_EnterZRQYXWC,  //进入再热前允许温差
	e_HeatPumpAir_EnterSWYXWC1,   //进入水温允许温差1
	e_HeatPumpAir_EnterSWYXWC2,   //进入水温允许温差2	
	e_HeatPumpAir_ExitZRQWC1,   //退出再热前温差1
	e_HeatPumpAir_ExitZRQWC2,   //退出再热前温差2	
	e_HeatPumpAir_HeatZRQTempSetVar,   //制热再热前温度目标
	e_HeatPumpAir_HeatSFTempSetVar_High,   //制热送风温度上限
	e_HeatPumpAir_HeatZRQTempSetVar_Low,   //制热再热前温度下限
	e_HeatPumpAir_XHADDHumWaterTempSetVar,   //循环加湿水温目标

	e_HeatPumpAir_WaterTempSamplingTime,   //水温采样间隔时间
	e_HeatPumpAir_EvaporatorTempSamplingTime,//再热前温度采样时间
	e_HeatPumpAir_ZRQ_CheckStartTime,        //再热前温度采样开始时间
	e_HeatPumpAir_AirSupplyTempSamplingTime, //送风温度采样时间
	e_HeatPumpAir_SFT_CheckStartTime,				//送风温度采样开始时间
	e_HeatPumpAir_SGS_DGSSamplingTime,       //吸排气过热度采样时间
	e_HeatPumpAir_Sgs_Dgs_Max_CheckStartTime, //吸排气过热度开始检测时间
	e_HeatPumpAir_HighPsPretectCompresser_CheckIntervalTime,//高压保护压缩机检测间隔
	e_HeatPumpAir_LowPsPretectCompresser_CheckIntervalTime,//低压保护压缩机检测间隔
	e_HeatPumpAir_ClrElecExpanCumuErr_IntervalTime, //电子膨胀阀静态误差消除
	e_HeatPumpAir_OutFanSpeed_CheckStartTime, //外部风机检测开始时间
	e_HeatPumpAir_OutFanSpeed_CheckIntervalTime,//外部风机检测压力时间
	e_HeatPumpAir_WaterTempIntegralEnSubVal, //水温积分有效温差范围
	e_HeatPumpAir_EvaporatorTempIntegralEnSubVal,//再热前温度积分有效温差范围
	e_HeatPumpAir_AirSupplyTempIntegralEnSubVal, //送风温度积分有效温差范围
	e_HeatPumpAir_SGS_DGS_IntegralEnSubVal,       //吸排气过热度积分有效温差范围
	e_HeatPumpAir_OutFanSpeed_IntegralEnSubVal,    //外部分机转速积分值有效范围
	e_HeatPumpAir_CompressorFreMax,     			//压缩机频率最大值  
	e_HeatPumpAir_CompressorFreMin,     			//压缩机频率最小值  

	e_HeatPumpAir_OutFanSpeed_CoolHighPressVar,//外部风机制冷转速高压目标值
	e_HeatPumpAir_OutFanSpeed_HeatLowPressVar,//外部风机制热转速低压目标值
	e_HeatPumpAir_OutFanSpeed_CoolHJTemp, //外部风机制冷变速点环温
	e_HeatPumpAir_OutFanSpeed_HeatHJTemp, //外部风机制热变速点环温
	e_HeatPumpAir_OutFanSpeed_InitVar1, //外部风机初始转速1
	e_HeatPumpAir_OutFanSpeed_InitVar2, //外部风机初始转速2
	e_HeatPumpAir_OutFanSpeed_Max, //外部风机转速最大值
	e_HeatPumpAir_OutFanSpeed_Min, //外部风机转速最小值

	//*****************外机参数************************************
	e_HeatPumpAirOut_ComHealth,//外机通讯健康值
	e_HeatPumpAirOut_XQT_T1, //压缩机吸气温度
	e_HeatPumpAirOut_PQT_T2, //压缩机排气温度
	e_HeatPumpAirOut_LNQPGT_T3,//冷凝器盘管温度
	e_HeatPumpAirOut_ORT_T4, //室外环境温度
	e_HeatPumpAirOut_BHJKLM_T5,//板换进口冷媒温度
	e_HeatPumpAirOut_BHCKLM_T6,//板换出口冷媒温度
	e_HeatPumpAirOut_BHCS_T7,  //板换出水温度
	e_HeatPumpAirOut_BHJS_T8,  //板换进水温度
	e_HeatPumpAirOut_NoUse_T,  //保留未使用温度传感器
	e_HeatPumpAirOut_XQP,      //吸气压力
	e_HeatPumpAirOut_PQY,      //排气压力
	e_HeatPumpAirOut_HPS,      //高压开关
	e_HeatPumpAirOut_SYYL,      //水压压力

	e_HeatPumpAirOut_NowFre,      //压缩机当前频率
	e_HeatPumpAirOut_EEV1_NowStep,//电子膨胀阀当前值
	e_HeatPumpAirOut_EEV2_NowStep,//电子膨胀阀2当前值
	e_HeatPumpAirOut_FAN1_Speed, //风机1转速
	e_HeatPumpAirOut_FAN2_Speed, //风机2转速
	e_HeatPumpAirOut_MasterPumpStatus,//主水泵状态

	e_HeatPumpAirOut_SensorErr1,  //传感器故障1
	e_HeatPumpAirOut_SensorErr2,  //传感器故障2	
	e_HeatPumpAirOut_Err1,       //故障1
	e_HeatPumpAirOut_BPQ_Err1,       //变频器故障1
	e_HeatPumpAirOut_BPQ_Err2,       //变频器故障2
	e_HeatPumpAirOut_BPQ_Err3,       //变频器故障3

	e_HeatPumpAirOut_SetDevType,        //外机类型
	e_HeatPumpAirOut_SetDevStatus,        //外机状态

	e_HeatPumpAirOut_SetFre,         //频率设定值
	e_HeatPumpAirOut_SetSTHXF_ONFF,  //四通换向阀
	e_HeatPumpAirOut_SetEEV1Step,  //设置电子膨胀阀开度
	e_HeatPumpAirOut_SetEEV2Step,  //设置电子膨胀阀2开度
	e_HeatPumpAirOut_SetOutFanSpeed, //设置外部风扇速度
	e_HeatPumpAirOut_SetYSJDJR_ONOFF, //压缩机电加热开关
	e_HeatPumpAirOut_SetMasterPumpStatus,//设置主水泵状态
	e_HeatPumpAirOut_ErrClear,					//故障复位

	e_HeatPumpAirOut_InverterModel,    //变频器型号
	e_HeatPumpAirOut_CompressorModel,  //压缩机型号
	e_HeatPumpAirOut_UPDATAFLAG,  //升级标识
	e_HeatPumpAirOut_ConfigFlag,  //配置更新标识
	e_HeatPumpAirOut_SWPUB,       //软件版本号
	e_HeatPumpAirOut_HWPUB,       //硬件版本号
	e_HeatPumpAirOut_DevType,     //设备类型号	
			
	e_HeatPumpAir_Defalut,
	
}e_HeatPumpEnum;


#endif

