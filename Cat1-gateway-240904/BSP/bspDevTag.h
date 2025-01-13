#ifndef _BSPDEVTAG_H_
#define _BSPDEVTAG_H_

#include "stm32f10x.h"

typedef enum
{
	e_Double_Cold,		// ˫��Դ
	e_Heat_Pumb,		// �ȱ��·�һ���/�·��ʪ��
	e_Air_Dehumi,		// �·��ʪ�����ط�
	
}Dev_Type_Struct;

typedef enum
{
	e_Dev_Gateway=1,
	e_Air_Controller,

}Gateway_Type_Struct;

/******************************ϵͳλ��*************************************/
typedef enum
{
/*********************ֱ��ʽ�·��ʪ��(���ط�)**********************************/	
		//
	e_InHoldingReg_CodeIAPFlag_1=0,
	e_InHoldingReg_CodeIAPFlag_2,	
	e_InHoldingReg_CodeIAPFlag_3,
	e_InHoldingReg_CodeIAPFlag_4,		
	e_InHoldingReg_ConfigFlag,  								//�ڻ����ø��±�ʶ
	e_InHoldingReg_SWPUB,       								//�ڻ�����汾��
	e_InHoldingReg_HWPUB,       								//�ڻ�Ӳ���汾��
	e_InHoldingReg_DevType,     								//�ڻ��豸���ͺ�		
	
	e_RegBuf_DevAddr,											    	//�豸��ַ
	e_RegBuf_SetONOFF, 										      //ϵͳ����  ������  0
	e_RegBuf_SetAutoRun,												//����0���Զ� ����1���ֶ� 2	
	e_RegBuf_SetMod,													  //ģʽ����  ������  1
	e_RegBuf_SetEnergyConservationMod,					//����ģʽ
	e_RegBuf_SetSilenceMod,										  //����ģʽ
	e_RegBuf_SetWaterTempCon,									  //ˮ�¿��Ʒ�ʽ
	e_RegBuf_SetHeatWaterTemp,                  //��������ˮ��
	e_RegBuf_SetCoolWaterTemp,                  //��������ˮ��
	e_RegBuf_SetAddHumEn,              				  //��ʪʹ��
	e_RegBuf_SetAddHumAct,              				//��ʪ����
	e_RegBuf_SetEvaporatorTemp,									//��������ǰ�¶�
	e_RegBuf_SetAirSupplyTemp,						      //�ͷ��¶�����ֵ	
	e_RegBuf_FANControl_Speed,   								//���ٵ�λ	 

	e_RegBuf_SetSGS1,														//�������ȶ�1����
	e_RegBuf_SetSGS2,														//�������ȶ�2����
	e_RegBuf_SetSGS3,														//�������ȶ�3����
	e_RegBuf_SetDGS1,														//�������ȶ�1����
	e_RegBuf_SetDGS2,														//�������ȶ�2����	
	e_RegBuf_SetDGS3,														//�������ȶ�3����
	e_RegBuf_SetEEV2GS_Low,											//EEV2���ȶȵ�������
	e_RegBuf_SetEEV1GS_Low,											//EEV1���ȶȵ�������
	e_RegBuf_OpenEHeatHJTemp,										//��������Ȼ����¶�
	e_RegBuf_CloseEHeatHJTemp,									//�رյ���Ȼ����¶�
	
	//��˪
	e_RegBuf_DefrostingCheckTimeStart,					//��˪�������ʱ�䣨����ģʽ�£�
	e_RegBuf_DefrostingHJTemp1,					        //��˪�ж������¶�1
	e_RegBuf_DefrostingHJTemp2,					        //��˪�ж������¶�2
	e_RegBuf_DefrostingHJTemp3,					        //��˪�ж������¶�3
	e_RegBuf_DefrostingWJPGTemp1,					      //��˪�ж�����̹��¶�1
	e_RegBuf_DefrostingWJPGTemp2,					      //��˪�ж�����̹��¶�2
	e_RegBuf_DefrostingWJPGTemp3,						    //��˪�ж�����̹��¶�3
	e_RegBuf_DefrostingWJPGTemp4,						    //��˪�ж�����̹��¶�4
	e_RegBuf_DefrostingCompressorFre,		//��˪ѹ����Ƶ��
	e_RegBuf_DefrostingEEV1,							//��˪�������ͷ�1����
	e_RegBuf_DefrostingEEV2,							//��˪�������ͷ�2����	
	e_RegBuf_DefrostingEEV3,							//��˪�������ͷ�3����	
	e_RegBuf_DefrostingExitWGTemp,   //��˪�˳�����¶�
 	e_RegBuf_DefrostingCompressorFreRunMaxTime,   //��˪ѹ�����������ʱ��
 	e_RegBuf_DefrostingCompressorFreRunMinTime,   //��˪ѹ����������Сʱ��
	
  //���� 
	e_RegBuf_OilRecovery_CompressorFre,//����Ƶ��60         	
	e_RegBuf_OilRecovery_IntervalTime,//���ʱ�� ��λ����     
	e_RegBuf_OilRecovery_ActKeepTime, //���ͱ���ʱ�� ��λ���� 
	e_RegBuf_OilRecovery_EEV1SetVar,//����ʱ�������ͷ�1����
	e_RegBuf_OilRecovery_EEV2SetVar,//����ʱ�������ͷ�2����
	e_RegBuf_OilRecovery_EEV3SetVar,//����ʱ�������ͷ�3����
	
	//��ʪ
	e_RegBuf_AddWater_FZLowTime, 	//��ˮ���޸��ӳ�ʱʱ��
	e_RegBuf_AddWater_FZHighTime,	//��ˮ���޸��ӳ�ʱʱ��
	e_RegBuf_AddHum_FZLowTime,	//��ʪ���޸��ӳ�ʱʱ��
	e_RegBuf_SubWater_FZLowTime,	//��ˮ���޸��ӳ�ʱʱ��
	e_RegBuf_CleanWaterTank_IntervalTime,//��ϴˮ����ʱ��
	e_RegBuf_CleanWaterTank_ActKeepTime,//��ϴˮ�����ʱ��
	e_RegBuf_CleanWaterTankAddWater_FZLowTime,//��ϴ��ˮ���޸�����Ӧ������ˮʱ��
	e_RegBuf_AddHumStopErr_IntervalTimeTime,//��ʪ��ˮ����ͣ������-��ˮ���ʱ��
	
	e_RegBuf_CompressorRun_MinIntervalTime,//ѹ����������С���
	//�����������
	e_RegBuf_AirFilterUseTime,//�ͻ���ʹ��ʱ��  	
	e_RegBuf_AirFilterConfigureMaxTime,//�������ʱ��  
	e_RegBuf_WaterFlowSmallAlarmTempDiff,//ˮ����С�����¶Ȳ�
	e_RegBuf_DevResonanceFrequency1, //�豸����Ƶ��1
	e_RegBuf_DevResonanceFrequency2, //�豸����Ƶ��2
	e_RegBuf_DevResonanceFrequency3, //�豸����Ƶ��3
	e_RegBuf_DevResonanceFrequency4, //�豸����Ƶ��4	
	
	e_RegBuf_EvaporatorTempNoOKTime,  //����ǰ�¶Ȳ����ʱ��
	e_RegBuf_WaterTempNoOKTime,  //ˮ���¶Ȳ����ʱ��
	e_RegBuf_WaterTempNoOKTempDt_Cool,      //ˮ���¶Ȳ�����¶�ƫ������	
	e_RegBuf_WaterTempNoOKTempDt_Heat,      //ˮ���¶Ȳ�����¶�ƫ������	
	e_RegBuf_ZRQ_MAXTEMP_HeatModue,  //��������ǰ����趨�¶�
	e_RegBuf_ZRQ_MINTEMP_HeatModue,  //��������ǰ��С�趨�¶�
	e_RegBuf_HeatSongFengTempConEEV2ACTOffset,//�����ͷ��¶ȵ���EEV2��������Χƫ��ֵ
	e_RegBuf_SubHumInAirOKABSTemp_Val, //��������ǰ�¶ȴ�����ֵ��EEV2������ȶȵ��ڣ�
	e_RegBuf_SubHumAirOutOKABSTemp_Val, //��������ǰ�¶Ȳ�������ֵ��EEV2�����¶ȵ��ڣ�
	e_RegBuf_WaterInOKABSTemp_Val, //����ˮ�´�����ֵ��EEV1������ȶȵ��ڣ�
	e_RegBuf_WaterOutOKABSTemp_Val, //����ˮ�²�������ֵ��EEV1�����¶ȵ��ڣ�
	e_RegBuf_DSTLow_Temp,						//�������ȶȵ��ڽ���ֵ
	e_RegBuf_DSTLow_OutTemp,				//�������ȶȵ������ֵ
	e_RegBuf_CoolModEnergyConserInFDT6Temp, //����ģʽ�豸�Խ��ܰ廻���������¶�ֵ��T6��
	e_RegBuf_CoolModEnergyConserOutFDT6Temp,//����ģʽ�豸�Խ��ܰ廻���������¶���ƫֵ��T6��
	e_RegBuf_CoolWaterTempEEV1Close,//����ˮ��EEV1�ر�ֵ(T8)
	e_RegBuf_CoolWaterTempEEV1Open, //����ˮ��EEV1��ֵ(T8)
	e_RegBuf_HeatWaterTempEEV1Close,//����ˮ��EEV1�ر�ֵ(T8)
	e_RegBuf_HeatWaterTempEEV1Open, //����ˮ��EEV1��ֵ(T8)
	
	e_RegBuf_NoEnergyConserModDevZJNFlag,   //�ǽ���ģʽ���豸�Խ��ܱ�ʶ
	
	e_RegBuf_DevRunModStatus,			//��������ģʽ
	e_RegBuf_DevRunModSubstatus,  //��������ģʽ��״̬
	
	e_RegBuf_InterMachEvapInTemp_T9,   //�ڻ����������¶�T9
	e_RegBuf_InterMachEvapOutTemp_T10,   //�ڻ����������¶�T10
	e_RegBuf_InterMachFanInTemp_T11,   //�ڻ������¶�T11
	e_RegBuf_InterMachBeforeReheatTemp, //�ڻ�����ǰ�¶�
	e_RegBuf_InterMachAirSupplyTemp,	//�ڻ��ͷ��¶�	
	e_RegBuf_InterMachXHJSSTemp,	//�ڻ�ѭ����ʪˮ��

	e_RegBuf_SGS,														//�������ȶ� 
	e_RegBuf_DGS,														//�������ȶ� 	
	e_RegBuf_AddHumSwStatus,     //��ʪ��ŷ�״̬
	
	e_RegBuf_InterMachSensorErr1,
	e_RegBuf_InterMachSensorErr2,							//
	e_RegBuf_Err1,
	e_RegBuf_Err2,
	e_RegBuf_Err3_1,
	e_RegBuf_Err3_2,	
	
	e_RegBuf_InterMachFanEn,              //�ڲ����ʹ��
	e_RegBuf_InterMachFanAct,             //�ڲ��������
	
	e_RegBuf_BHEEV1SetVar,		//�廻�������ͷ�1�趨ֵ
	e_RegBuf_BHEEV1SetVarMax,	//�廻�������ͷ�1�趨���ֵ	
	e_RegBuf_BHEEV1SetVarMin,	//�廻�������ͷ�1�趨��Сֵ
	e_RegBuf_BHEEV1SetInitVar,//�廻�������ͷ�1��ʼֵ
	
	e_RegBuf_SUBHUMEEV2SetVar,		  //��ʪ�������ͷ�2�趨ֵ
	e_RegBuf_SUBHUMEEV2SetVarMax,	  //��ʪ�������ͷ�2�趨���ֵ	
	e_RegBuf_SUBHUMEEV2SetVarMin,	  //��ʪ�������ͷ�2�趨��Сֵ
	e_RegBuf_SUBHUMEEV2SetInitVar,	//��ʪ�������ͷ�2��ʼֵ
	
	e_RegBuf_AddHeatEEV3SetVar,		  //���ȵ������ͷ�3�趨ֵ
	e_RegBuf_AddHeatEEV3SetVarMax,	//���ȵ������ͷ�3�趨���ֵ	
	e_RegBuf_AddHeatEEV3SetVarMin,	//���ȵ������ͷ�3�趨��Сֵ
	e_RegBuf_AddHeatEEV3SetInitVar,	//���ȵ������ͷ�3��ʼֵ
	
	e_RegBuf_MasterPumpCon,			//��ˮ�ÿ���
	e_RegBuf_CondensateLiftPumpCon,  //����ˮ�ÿ���
	
  //***********������ɱ������*********************
	e_RegBuf_UltravioletSterilizationCon_En,       
	e_RegBuf_UltravioletSterilizationCon_ConFlag,	 

  //***********�����ӿ���*************************
	e_RegBuf_AnionCon_En,   
	e_RegBuf_AnionCon_ConFlag,

	e_RegBuf_EnterZRQYXWC,  //��������ǰ�����²�
	e_RegBuf_EnterSWYXWC1,   //����ˮ�������²�1
	e_RegBuf_EnterSWYXWC2,   //����ˮ�������²�2	
	e_RegBuf_ExitZRQWC1,   //�˳�����ǰ�²�1
	e_RegBuf_ExitZRQWC2,   //�˳�����ǰ�²�2	
	e_RegBuf_HeatZRQTempSetVar,   //��������ǰ�¶�Ŀ��
	e_RegBuf_HeatSFTempSetVar_High,   //�����ͷ��¶�����
	e_RegBuf_HeatZRQTempSetVar_Low,   //��������ǰ�¶�����
	e_RegBuf_XHADDHumWaterTempSetVar,   //ѭ����ʪˮ��Ŀ��
	
	e_RegBuf_WaterTempSamplingTime,   //ˮ�²������ʱ��
	e_RegBuf_EvaporatorTempSamplingTime,//����ǰ�¶Ȳ���ʱ��
	e_RegBuf_ZRQ_CheckStartTime,        //����ǰ�¶Ȳ�����ʼʱ��
	e_RegBuf_AirSupplyTempSamplingTime, //�ͷ��¶Ȳ���ʱ��
	e_RegBuf_SFT_CheckStartTime,				//�ͷ��¶Ȳ�����ʼʱ��
	e_RegBuf_SGS_DGSSamplingTime,       //���������ȶȲ���ʱ��
	e_RegBuf_Sgs_Dgs_Max_CheckStartTime, //���������ȶȿ�ʼ���ʱ��
	e_RegBuf_HighPsPretectCompresser_CheckIntervalTime,//��ѹ����ѹ���������
	e_RegBuf_LowPsPretectCompresser_CheckIntervalTime,//��ѹ����ѹ���������
	e_RegBuf_ClrElecExpanCumuErr_IntervalTime, //�������ͷ���̬�������
	e_RegBuf_OutFanSpeed_CheckStartTime, //�ⲿ�����⿪ʼʱ��
	e_RegBuf_OutFanSpeed_CheckIntervalTime,//�ⲿ������ѹ��ʱ��
	e_RegBuf_WaterTempIntegralEnSubVal, //ˮ�»�����Ч�²Χ
	e_RegBuf_EvaporatorTempIntegralEnSubVal,//����ǰ�¶Ȼ�����Ч�²Χ
	e_RegBuf_AirSupplyTempIntegralEnSubVal, //�ͷ��¶Ȼ�����Ч�²Χ
	e_RegBuf_SGS_DGS_IntegralEnSubVal,       //���������ȶȻ�����Ч�²Χ
	e_RegBuf_OutFanSpeed_IntegralEnSubVal,    //�ⲿ�ֻ�ת�ٻ���ֵ��Ч��Χ
	e_RegBuf_CompressorFreMax,     			//ѹ����Ƶ�����ֵ  
	e_RegBuf_CompressorFreMin,     			//ѹ����Ƶ����Сֵ  
	
	e_RegBuf_OutFanSpeed_CoolHighPressVar,//�ⲿ�������ת�ٸ�ѹĿ��ֵ
	e_RegBuf_OutFanSpeed_HeatLowPressVar,//�ⲿ�������ת�ٵ�ѹĿ��ֵ
	e_RegBuf_OutFanSpeed_CoolHJTemp, //�ⲿ���������ٵ㻷��
	e_RegBuf_OutFanSpeed_HeatHJTemp, //�ⲿ������ȱ��ٵ㻷��
	e_RegBuf_OutFanSpeed_InitVar1, //�ⲿ�����ʼת��1
	e_RegBuf_OutFanSpeed_InitVar2, //�ⲿ�����ʼת��2
	e_RegBuf_OutFanSpeed_Max, //�ⲿ���ת�����ֵ
	e_RegBuf_OutFanSpeed_Min, //�ⲿ���ת����Сֵ

//#ifdef BACKFANSYS
	e_RegBuf_AirSensorComHealth,//���ͨѶ����ֵ
	e_RegBuf_AirSensorBackFanTemp,//�ط��¶�
	e_RegBuf_AirSensorBackFanHum,//�ط�ʪ��
	e_RegBuf_AirSensorBack25,//�ط�PM2.5
	e_RegBuf_AirSensorBack10,//�ط�PM10
	e_RegBuf_AirSensorBackCO2,//�ط�CO2
	e_RegBuf_AirSensorBackSWPub,//����汾��
	e_RegBuf_AirSensorBackHWPub,//Ӳ���汾��	
	e_RegBuf_AirSensorBackDevType,//�豸�ͺ�
	e_RegBuf_AirSensorBackFanTemp_OfferSet,//�ط��¶�У��ֵ
	e_RegBuf_AirSensorBackFanHum_OfferSet,//�ط�ʪ��У��ֵ
	e_RegBuf_AirSensorBack25_OfferSet,//�ط�PM2.5У��ֵ
	e_RegBuf_AirSensorBack10_OfferSet,//�ط�PM10У��ֵ
	e_RegBuf_AirSensorBackCO2_OfferSet,//�ط�CO2	У��ֵ
	
	e_RegBuf_Sw_NewFan_Min, //�·緧��С����
	e_RegBuf_Sw_NewFan_Max, //�·緧��󿪶�
	e_RegBuf_Sw_NewFan_Init,//�·緧��ʼֵ
	e_RegBuf_Sw_NewFan_NowVal,//�·緧����
	e_RegBuf_Sw_BackFan_Min, //�ط緧��С����
	e_RegBuf_Sw_BackFan_Max, //�ط緧��󿪶�
	e_RegBuf_Sw_BackFan_Init,//�ط緧��ʼֵ
	e_RegBuf_Sw_BackFan_NowVal,//�ط緧����
	e_RegBuf_FreshAirVolume,//�·���
	e_RegBuf_FreshAirSpeed, //�·����
	e_RegBuf_FreshAirCrossSectionalArea,//�·�����
	e_RegBuf_FreshAirVolumeCorrectionCoefficient ,//�·���У��ϵ��
	e_RegBuf_FanTempModCtlr,//���¿��Ʒ�ʽѡ��
	e_RegBuf_Reserve1, 
	e_RegBuf_Reserve2, 	
	e_RegBuf_Reserve3, 		
	e_RegBuf_Reserve4, 		
//#endif
	
//*****************�������************************************
	e_OutHoldingReg_ComHealth,//���ͨѶ����ֵ
	e_OutHoldingReg_XQT_T1, //ѹ���������¶�
	e_OutHoldingReg_PQT_T2, //ѹ���������¶�
	e_OutHoldingReg_LNQPGT_T3,//�������̹��¶�
	e_OutHoldingReg_ORT_T4, //���⻷���¶�
	e_OutHoldingReg_BHJKLM_T5,//�廻������ý�¶�
	e_OutHoldingReg_BHCKLM_T6,//�廻������ý�¶�
	e_OutHoldingReg_BHCS_T7,  //�廻��ˮ�¶�
	e_OutHoldingReg_BHJS_T8,  //�廻��ˮ�¶�
	e_OutHoldingReg_NoUse_T,  //����δʹ���¶ȴ�����
	e_OutHoldingReg_XQP,      //����ѹ��
	e_OutHoldingReg_PQY,      //����ѹ��
	e_OutHoldingReg_HPS,      //��ѹ����
	e_OutHoldingReg_SYYL,      //ˮѹѹ��

	e_OutHoldingReg_NowFre,      //ѹ������ǰƵ��
	e_OutHoldingReg_EEV1_NowStep,//�������ͷ���ǰֵ
	e_OutHoldingReg_EEV2_NowStep,//�������ͷ�2��ǰֵ
	e_OutHoldingReg_FAN1_Speed, //���1ת��
	e_OutHoldingReg_FAN2_Speed, //���2ת��
	e_OutHoldingReg_MasterPumpStatus,//��ˮ��״̬
	
	e_OutHoldingReg_SensorErr1,  //����������1
	e_OutHoldingReg_SensorErr2,  //����������2	
	e_OutHoldingReg_Err1,       //����1
	e_OutHoldingReg_BPQ_Err1,       //��Ƶ������1
	e_OutHoldingReg_BPQ_Err2,       //��Ƶ������2
	e_OutHoldingReg_BPQ_Err3,       //��Ƶ������3
	
	e_OutHoldingReg_SetDevType,        //�������
	e_OutHoldingReg_SetDevStatus,        //���״̬
	
	e_OutHoldingReg_SetFre,         //Ƶ���趨ֵ
	e_OutHoldingReg_SetSTHXF_ONFF,  //��ͨ����
	e_OutHoldingReg_SetEEV1Step,  //���õ������ͷ�����
	e_OutHoldingReg_SetEEV2Step,  //���õ������ͷ�2����
	e_OutHoldingReg_SetOutFanSpeed, //�����ⲿ�����ٶ�
	e_OutHoldingReg_SetYSJDJR_ONOFF, //ѹ��������ȿ���
	e_OutHoldingReg_SetMasterPumpStatus,//������ˮ��״̬
	e_OutHoldingReg_ErrClear,					//���ϸ�λ
	
	e_OutHoldingReg_InverterModel,    //��Ƶ���ͺ�
	e_OutHoldingReg_CompressorModel,  //ѹ�����ͺ�
	e_OutHoldingReg_UPDATAFLAG,  //������ʶ
	e_OutHoldingReg_ConfigFlag,  //���ø��±�ʶ
	e_OutHoldingReg_SWPUB,       //����汾��
	e_OutHoldingReg_HWPUB,       //Ӳ���汾��
	e_OutHoldingReg_DevType,     //�豸���ͺ�	
			
//	e_RegBuf_Defalut,
	
	
	e_DevHoldReg_Number,			// ���ּĴ����ܸ���
	
	
}eDeviceHoldingReg;

typedef enum  
{
/*********************˫��Դһ���*************************/	
	e_Double_ONOFF=0, 										  //ϵͳ����  ������  0
	e_Double_Mod,													  //ģʽ����  ������  1
	e_Double_AutoRun,												//����0���Զ� ����1���ֶ� 2

	e_Double_SetEvaporatorTemp,							//����ǰ�¶�����ֵ  3
	e_Double_EvaporatorTemp,						  	//����ǰ�¶�     4
	e_Double_EvaporatorTempFreMin,     			//����ǰѹ����Ƶ����Сֵ  5
	e_Double_EvaporatorTempFreMax,     			//����ǰѹ����Ƶ�����ֵ  6
	e_Double_EvaporatorTempFreIntegralEnSubVal,	//����ǰ�¶Ȼ�����Ч�²Χ	7
	e_Double_EvaporatorTempFrePIDSamplingTime,            //PID����ʱ��	 8

	e_Double_SetAirSupplyTemp,						//�ͷ��¶�����ֵ	9

	e_Double_AirSupplyTemp,						    //10�ͷ��¶�	 10 A
	e_Double_AirSupplyTempStepEEVMin,     // �ͷ��¶ȷ���С���� �������ͷ�ע���ǵ��ӻ���ˮ��   11 B  
	e_Double_AirSupplyTempStepEEVMax,     //�ͷ��¶ȷ���󿪶�  12   C
	e_Double_AirSupplyTempStepSetVal,     //�ͷ��¶ȷ���ǰ����ֵ 13  D
	e_Double_AirSupplyTempEEVIntegralEnSubVal,//�ͷ��¶Ȼ�����Ч�²Χ	14  E
	e_Double_AirSupplyTempEEVPIDSamplingTime, //PID����ʱ�� 15  F

	e_Double_AirSupplyHum,						  	//�ͷ�ʪ��	16  10

	e_Double_HuangjingTemp,								//�����¶�	17  11
	e_Double_HuangjingHum,								//����ʪ��	18  12

	//*****************************���������ȶ�************************************************** 	

	//*****************************�������ȶ�************************************************** 
	e_Double_SGS, 												      //�������ȶ�	                             19
	e_Double_SetSGS, 												    // 20 �趨���������ȶ�	 	                 20

	e_Double_SGS_EEVStepMin, 										//�������ȶȷ�������Сֵ                   21
	e_Double_SGS_EEVStepMax, 										//�������ȶȷ��������ֵ                   22
	e_Double_SGS_EEVStepSetVal,                 //�������ȶȷ���ǰֵ                       23
	e_Double_SGS_EEVIntegralEnSubVal,						//�������ȶȻ�����Ч�²Χ	             24

	//*****************************�������ȶ�************************************************** 
	e_Double_DGS, 															//�������ȶ�		25
	e_Double_SetDGSMin, 												//�趨���������ȶ���Сֵ(���������ȶȵ���ģʽ�¼��)  26
	e_Double_SetDGSMax, 												//�趨���������ȶ���Сֵ�����������ȶȵ���ģʽ�¼�أ�27

	e_Double_InverterSetOnOFF,		 					// 30 ���ÿ���             28
	e_Double_InverterSetFre,		 						//����Ƶ��                 29
	e_Double_InverterFreNow,								//��ǰƵ��                 30
	e_Double_InverterErrBuf1,								//��Ƶ������1              31
	e_Double_InverterErrBuf2,								//��Ƶ������2	             32
	e_Double_InverterErrBuf3,								//��Ƶ������2	  35         33

	e_Double_InverterType,										//��Ƶ������             34
	//**************************�ߵ�ѹѹ��********************************************

	//��ѹѹ��
	e_Double_LPS_Value,								//��ѹѹ��   35

	//��ѹѹ��
	e_Double_HPS_Value,	 //                        36

	e_Double_HighPsPretectCompresser_CheckIntervalTime,			//ѹ����������ʱ�� 37
	e_Double_HighPsPretectWaterValve_CheckIntervalTime,			//ˮ������ʱ��       38
	e_Double_HighPsPretectCompresser_SubVal,
	e_Double_HighPsPretectWaterValve_SubVal,
	e_Double_HPS_STOP_COMPERSSER_VAL,												//#define HPS_STOP_COMPERSSER_VAL 400 //4Mpa  ϵͳͣ�����ѹ��  39
	e_Double_HPS_SUBFRE_COMPERSSER_VAL,											//#define HPS_SUBFRE_COMPERSSER_VAL 350 //3.5Mpa ѹ������Ƶѹ�� 40
	e_Double_HPS_STOPSUBFRE_COMPERSSER_VAL,                 //#define HPS_STOPSUBFRE_COMPERSSER_VAL 300 //3.0Mpa ѹ�����ع������߼�41
	e_Double_LPS_STOP_COMPERSSER_VAL,												//#define LPS_STOP_COMPERSSER_VAL  10  //1.0bar 42
	e_Double_LPS_ADDEEV1_VAL,																//#define LPS_ADDEEV1_VAL   40         //4.0bar 43

	//******************�ͷ��¶�*****************************************************************************	
	e_Double_SFT_CheckIntervalTime,			//��λ �� Ĭ��1s  44
	e_Double_SFT_CheckStartTime,				  //��λ �� Ĭ��0 45

	//******************����ǰ�¶�*****************************************************************************
	e_Double_ZRQ_CheckIntervalTime,			//��λ �� Ĭ��1s  46
	e_Double_ZRQ_CheckStartTime,				  //��λ �� Ĭ��0 47

	//******************���������ȶ�************************************************************************ 
	e_Double_Sgs_Dgs_Max_CheckIntervalTime,			//��λ �� Ĭ��1s 48
	e_Double_Sgs_Dgs_Max_CheckStartTime,				  //��λ �� Ĭ��0 49

	//******************�������¶�************************************************************************
	//�����¶�
	e_Double_sgt_Temp,										//ѹ���������¶�	           50


	//�����¶�
	e_Double_dgt_Temp,										//50ѹ���������¶�           51

	//*******************��������*************************************************************

	//------------------�������ı����¼�----------------------------------------------
	//1.�������ȶȹ���
	//2.�������ȶȹ���
	//3.�������ȶȹ���
	//4.������ѹ����
	//5.����ѹ������
	////6.�����¶ȹ���
	//7.�����¶ȹ���
	//8.������ϱ���
	//9.��Ƶ������             ������ͣ����
	e_Double_NeedReset,//                                           52
	//------------------���������ı����¼�----------------------------------------------
	//1.�����¶ȴ���������     ����Ҫ�õ�ѹ������ģʽ��
	//2.�����¶ȴ���������     ����Ҫ�õ�ѹ������ģʽ��
	//3.��ѹѹ������������ 	   ����Ҫ�õ�ѹ������ģʽ��
	//4.��ѹѹ������������     ����Ҫ�õ�ѹ������ģʽ��

	//5.����ǰ�¶ȴ���������   ����Ҫ��ʪ��ģʽ�����䡢��ʪģʽ�������������¶�

	//6.������ͨѶ����         ������ͣ����
	//7.�����¶ȹ���           ������ͣ�����ָ���������
	//8.������������           ���������߹���ͣ����
	e_Double_OFFNoResetAlarmEvt,//                                 53

	//------------------����ͣ���ı����¼�----------------------------------------------
	//1.��ѹ���ر���			 ��������ʾ��//��������
	//2.�����¶ȴ�����         ��������ʾ��//
	//3.����ʪ�ȴ�����         ��������ʾ��//
	//4.�ͷ��¶ȴ�����         ��������ʾ��//
	//5.�ͷ�ʪ�ȴ�����         ��������ʾ��//
	//6.Ԥ���̹��¶ȴ�����     ��������ʾ��//
	//7.������������           ��������ʾ��//
	//8.��ˮ��ˮλ���߱���     ��������ʾ��
	//9.��ʪ������
	e_Double_NOOFFAlarmEvt, //                                    54

	//��������
	e_Double_AirFilterConfigureMaxTime,//�������ʱ��  55
	e_Double_AirFilterUseTime,//�ͻ���ʹ�����ʱ��  56
	e_Double_AirFilterConfigureMaxPress,//���õ�ʧЧѹ��  57
	e_Double_AirFilterPress,//ѹ��  58


	//********************��ʪ����*********************************
	e_Double_HumidificationControl_En,  //59
	e_Double_HumidificationControl_ActFlag,//60 

	//****************�������*************************************
	e_Double_FANControl_En,//61
	e_Double_FANControl_ActFlag,//62

	e_Double_FANControl_Speed,   //���ٵ�λ	63

	e_Double_FANControl_VoltageVal_MinLow,			//��С��������ѹֵ	64
	e_Double_FANControl_VoltageVal_Low,			    //С��������ѹֵ    65
	e_Double_FANControl_VoltageVal_Med,			    //�е�������ѹֵ    66
	e_Double_FANControl_VoltageVal_High,			  //�ߵ�������ѹֵ    67
	e_Double_FANControl_VoltageVal_MaxHigh,			  //��ߵ�������ѹֵ 68

	//***********������ɱ������*********************
	e_Double_UltravioletSterilizationCon_En,      //69
	e_Double_UltravioletSterilizationCon_ConFlag,	//70

	//***********�����ӿ���*************************
	e_Double_AnionCon_En,  //71
	e_Double_AnionCon_ConFlag,						//72

	//**********����ȿ���**************************
	e_Double_ElectricHeatingEn,           //�����ʹ��	73
	e_Double_ElectricHeating_ConFlag,			//����ȿ��Ʊ�ʶ 74

	//��������
	e_Double_AntiFreezing_En,
	e_Double_AntiFreezing_ActFlag,								//������ʶ  75
	e_Double_AntiFreezing_StartTemp,							//�����¶�  Ĭ�ϣ�5���϶� 76
	e_Double_AntiFreezing_EndTemp,							//�����¶�  Ĭ�ϣ�10���϶�  77

	//���Ϳ���
	e_Double_OilRecovery_IntervalTime,//���ʱ�� ��λ����    78
	e_Double_OilRecovery_ActKeepTime, //���ͱ���ʱ�� ��λ���� 79
	e_Double_OilRecovery_CompressorFre,//����Ƶ��60         80

	//�������ͷ��ۻ��������

	e_Double_ClrElecExpanCumuErr_IntervalTime,//���ʱ�� ��λ����  81

	e_Double_Wait_DGSOK_IntervalFre,           //�ȴ��������ȶȺϸ�ѹ����Ƶ�� 82 
	e_Double_HeatSetEvaporatorTemp,						 //����ǰ�¶�����ֵ  ����ģʽ  83
	e_Double_HeatSetAirSupplyTemp,						 //�ͷ��¶�����ֵ	   ����ģʽ  84

	e_Double_IAPBootFlag,											 //
	e_Double_WaterFlowRate,										 //ˮ����

	e_Double_AirSupplyTempStepEEVMin_Heat,         //����_�ͷ��¶ȷ���С���� �������ͷ�ע���ǵ��ӻ���ˮ��   
	e_Double_AirSupplyTempStepEEVMax_Heat,         //����_�ͷ��¶ȷ���󿪶�  
	e_Double_AirSupplyTempStepSetVal_Heat,         //����_�ͷ��¶ȷ���ǰ����ֵ 
	e_Double_AirSupplyTempEEVIntegralEnSubVal_Heat,//����_�ͷ��¶Ȼ�����Ч�²Χ		
	//**************************����*********************************************
	//����ģʽ��Ԥ���̹�ǰpidˮ�������ͷ��¶� �رհ廻pidˮ��
	//����ģʽ: ȫ��Ԥ���̹�ǰpidˮ����ͨ���廻pidˮ�������ͷ��¶�
	//300  ���� 1
	//500  ���� 2
	//750  ���� 3

	//��������ģʽ����������ͨ��������ˮ·���ã�+�廻pidˮ����ϵ����ͷ��¶�
	//1000 ���� 4
	//1500 ���� 5
	//2000 ���� 6
	e_Double_DevConPub,												     //����ϵͳһ����ͺ�

	e_Double_CodePub,													     //����汾��

	e_Double_IAPSerNumMem,	

	e_Double_HumAddWaterMaxTime, //��ʪ��ˮ���ʱ��

	e_Double_HumHighFuziHighToLowTime, //��ʪ���޸�������ʱ��

	e_Double_HumPumpProtectTime,       //��ʪ�ñ���ʱ��

	e_Double_HumErrFlag,               //��ʪ���ϴ����ʶ Ĭ��0

	e_Double_HumSubWaterIntervalTime, //��ʪ��ˮ��� ����

	e_Double_DevAddr, //�豸��ַ

//	#ifdef Add_SysWaterTempAndDevPGOutTemp	
	e_Double_InWaterTemp,   //һ�����ˮ�¶� ��λ�����϶� ��ȷ��0.1 ֻ��

	e_Double_YLPGTempStepEEVMin_Cool,         //Ԥ����ڷ���С����   Ĭ��45.0 ��д
	e_Double_YLPGTempStepEEVMax_Cool,         //Ԥ����ڷ���󿪶�  Ĭ��90.0  ��д
	e_Double_YLPGTempStepSetVal_Cool,         //Ԥ����ڷ���ǰ����ֵ   ��д
	e_Double_YLPGTempEEVIntegralEnSubVal_Cool,//��������ǰ�¶Ȼ�����Ч�²Χ	Ĭ��0.5 ���϶�  ��д	
	e_Double_ZRQTemp1_SubHumErr_TZ1, //����ǰ�¶ȹ��߱���ֵ 1 Tz1 Ĭ��16.0  ��д
	e_Double_ZRQTemp2_SubHumErr_TZ2, 	//����ǰ�¶ȹ��߱���ֵ2 Tz2  Ĭ��18.0 ��д
	e_Double_IWTTemp_SubHumOK_TSG,	 //��ʪ�������㱨������ˮ��Tsg Ĭ��25.0 ��д
	e_Double_ZRQTemp_SubHumOKErr_T3,	 //��ʪ�������㱨���������ʱ�� Ĭ��30 ��λ������ ��д

	e_Double_SWToYSJWaitTime_T1_Cool,    //���������˳�Ԥ����ڷ���������ǰ�¶ȳ���ʱ��  T1 Ĭ�� 2���� ��д
	e_Double_YSJToSWWaitTime_T2_Cool,    //����ǰ�¶ȹ������ʱ��  T1 Ĭ�� 30���� ��д
	e_Double_SGSMinVal_Txg,              //�������ȶȵ���ֵ ��λ���϶� Ĭ�� 1.0 ��д
	e_Double_SGTProtectVal_Ts1,          //�����¶ȱ���ֵTs1 ��λ���϶� Ĭ��4.0 ��д
	e_Double_SGTProtectMinVal_Ts2,       //�����¶ȼ���ֵTs2  ��λ���϶� Ĭ�� -2.0 ��д
	e_Double_Down1HZSecond_Txjp,              //��1hzʱ�� ��λs Ĭ��2 ��д

	//e_Double_DownFanVoltagMinute,        //������ٵ�ѹÿ�����½�
//	#endif
	e_Double_FanDevTypeCon,               //������豸��������ƥ�� 0���ϻ���ʩ�ְ� 1�������Ż�����Ͳ��� 2�������Ż���ʩ�ְ�

	e_Double_CodeIAPFlag_1,
	e_Double_CodeIAPFlag_2,	
	e_Double_CodeIAPFlag_3,
	e_Double_CodeIAPFlag_4,	
	e_Double_ConfigFlag,  								//�ڻ����ø��±�ʶ	

	e_Double_OutWaterTemp,           //��ˮ�¶�
	e_Double_WaterTempDifVar_Err,    //ˮ�²�澯ֵ 
	e_Double_AirPreDifProtectEn,     //����ѹ���©ˮ����ʹ�� 1��Ч
	e_Double_AirPreDifProtectDevType,    //����ѹ���豸���� 0����ѹ 1����ѹ
	e_Double_AirPreDifProtectVal,    		//ѹ������ֵ
	e_Double_ActFanSpeed,               //ʵ�ʷ���ת��
	e_Double_ActFanSpeedErrValue,       //���������ٹ��͹���ֵ ����ͣ��

	e_DevDoubleHoldReg_Number,
	
}e_DoubleEnum;



typedef enum  
{
/*********************ֱ��ʽ�·��ʪ�����ȱ��·�һ���*************************/
	//
	e_HeatPumpAir_CodeIAPFlag_1=0,
	e_HeatPumpAir_CodeIAPFlag_2,	
	e_HeatPumpAir_CodeIAPFlag_3,
	e_HeatPumpAir_CodeIAPFlag_4,		
	e_HeatPumpAir_ConfigFlag,  								//�ڻ����ø��±�ʶ
	e_HeatPumpAir_SWPUB,       								//�ڻ�����汾��
	e_HeatPumpAir_HWPUB,       								//�ڻ�Ӳ���汾��
	e_HeatPumpAir_DevType,     								//�ڻ��豸���ͺ�		

	e_HeatPumpAir_DevAddr,											    	//�豸��ַ
	e_HeatPumpAir_SetONOFF, 										      //ϵͳ����  ������  0
	e_HeatPumpAir_SetAutoRun,												//����0���Զ� ����1���ֶ� 2	
	e_HeatPumpAir_SetMod,													  //ģʽ����  ������  1
	e_HeatPumpAir_SetEnergyConservationMod,					//����ģʽ
	e_HeatPumpAir_SetSilenceMod,										  //����ģʽ
	e_HeatPumpAir_SetWaterTempCon,									  //ˮ�¿��Ʒ�ʽ
	e_HeatPumpAir_SetHeatWaterTemp,                  //��������ˮ��
	e_HeatPumpAir_SetCoolWaterTemp,                  //��������ˮ��
	e_HeatPumpAir_SetAddHumEn,              				  //��ʪʹ��
	e_HeatPumpAir_SetAddHumAct,              				//��ʪ����
	e_HeatPumpAir_SetEvaporatorTemp,									//��������ǰ�¶�
	e_HeatPumpAir_SetAirSupplyTemp,						      //�ͷ��¶�����ֵ	
	e_HeatPumpAir_FANControl_Speed,   								//���ٵ�λ	 

	e_HeatPumpAir_SetSGS1,														//�������ȶ�1����
	e_HeatPumpAir_SetSGS2,														//�������ȶ�2����
	e_HeatPumpAir_SetSGS3,														//�������ȶ�3����
	e_HeatPumpAir_SetDGS1,														//�������ȶ�1����
	e_HeatPumpAir_SetDGS2,														//�������ȶ�2����	
	e_HeatPumpAir_SetDGS3,														//�������ȶ�3����
	e_HeatPumpAir_SetEEV2GS_Low,											//EEV2���ȶȵ�������
	e_HeatPumpAir_SetEEV1GS_Low,											//EEV1���ȶȵ�������
	e_HeatPumpAir_OpenEHeatHJTemp,										//��������Ȼ����¶�
	e_HeatPumpAir_CloseEHeatHJTemp,									//�رյ���Ȼ����¶�

	//��˪
	e_HeatPumpAir_DefrostingCheckTimeStart,					//��˪�������ʱ�䣨����ģʽ�£�
	e_HeatPumpAir_DefrostingHJTemp1,					        //��˪�ж������¶�1
	e_HeatPumpAir_DefrostingHJTemp2,					        //��˪�ж������¶�2
	e_HeatPumpAir_DefrostingHJTemp3,					        //��˪�ж������¶�3
	e_HeatPumpAir_DefrostingWJPGTemp1,					      //��˪�ж�����̹��¶�1
	e_HeatPumpAir_DefrostingWJPGTemp2,					      //��˪�ж�����̹��¶�2
	e_HeatPumpAir_DefrostingWJPGTemp3,						    //��˪�ж�����̹��¶�3
	e_HeatPumpAir_DefrostingWJPGTemp4,						    //��˪�ж�����̹��¶�4
	e_HeatPumpAir_DefrostingCompressorFre,		//��˪ѹ����Ƶ��
	e_HeatPumpAir_DefrostingEEV1,							//��˪�������ͷ�1����
	e_HeatPumpAir_DefrostingEEV2,							//��˪�������ͷ�2����	
	e_HeatPumpAir_DefrostingEEV3,							//��˪�������ͷ�3����	
	e_HeatPumpAir_DefrostingExitWGTemp,   //��˪�˳�����¶�
	e_HeatPumpAir_DefrostingCompressorFreRunMaxTime,   //��˪ѹ�����������ʱ��
	e_HeatPumpAir_DefrostingCompressorFreRunMinTime,   //��˪ѹ����������Сʱ��

	//���� 
	e_HeatPumpAir_OilRecovery_CompressorFre,//����Ƶ��60         	
	e_HeatPumpAir_OilRecovery_IntervalTime,//���ʱ�� ��λ����     
	e_HeatPumpAir_OilRecovery_ActKeepTime, //���ͱ���ʱ�� ��λ���� 
	e_HeatPumpAir_OilRecovery_EEV1SetVar,//����ʱ�������ͷ�1����
	e_HeatPumpAir_OilRecovery_EEV2SetVar,//����ʱ�������ͷ�2����
	e_HeatPumpAir_OilRecovery_EEV3SetVar,//����ʱ�������ͷ�3����

	//��ʪ
	e_HeatPumpAir_AddWater_FZLowTime, 	//��ˮ���޸��ӳ�ʱʱ��
	e_HeatPumpAir_AddWater_FZHighTime,	//��ˮ���޸��ӳ�ʱʱ��
	e_HeatPumpAir_AddHum_FZLowTime,	//��ʪ���޸��ӳ�ʱʱ��
	e_HeatPumpAir_SubWater_FZLowTime,	//��ˮ���޸��ӳ�ʱʱ��
	e_HeatPumpAir_CleanWaterTank_IntervalTime,//��ϴˮ����ʱ��
	e_HeatPumpAir_CleanWaterTank_ActKeepTime,//��ϴˮ�����ʱ��
	e_HeatPumpAir_CleanWaterTankAddWater_FZLowTime,//��ϴ��ˮ���޸�����Ӧ������ˮʱ��
	e_HeatPumpAir_AddHumStopErr_IntervalTimeTime,//��ʪ��ˮ����ͣ������-��ˮ���ʱ��

	e_HeatPumpAir_CompressorRun_MinIntervalTime,//ѹ����������С���
	//�����������
	e_HeatPumpAir_AirFilterUseTime,//�ͻ���ʹ��ʱ��  	
	e_HeatPumpAir_AirFilterConfigureMaxTime,//�������ʱ��  
	e_HeatPumpAir_WaterFlowSmallAlarmTempDiff,//ˮ����С�����¶Ȳ�
	e_HeatPumpAir_DevResonanceFrequency1, //�豸����Ƶ��1
	e_HeatPumpAir_DevResonanceFrequency2, //�豸����Ƶ��2
	e_HeatPumpAir_DevResonanceFrequency3, //�豸����Ƶ��3
	e_HeatPumpAir_DevResonanceFrequency4, //�豸����Ƶ��4	

	e_HeatPumpAir_EvaporatorTempNoOKTime,  //����ǰ�¶Ȳ����ʱ��
	e_HeatPumpAir_WaterTempNoOKTime,  //ˮ���¶Ȳ����ʱ��
	e_HeatPumpAir_WaterTempNoOKTempDt_Cool,      //ˮ���¶Ȳ�����¶�ƫ������	
	e_HeatPumpAir_WaterTempNoOKTempDt_Heat,      //ˮ���¶Ȳ�����¶�ƫ������	
	e_HeatPumpAir_ZRQ_MAXTEMP_HeatModue,  //��������ǰ����趨�¶�
	e_HeatPumpAir_ZRQ_MINTEMP_HeatModue,  //��������ǰ��С�趨�¶�
	e_HeatPumpAir_HeatSongFengTempConEEV2ACTOffset,//�����ͷ��¶ȵ���EEV2��������Χƫ��ֵ
	e_HeatPumpAir_SubHumInAirOKABSTemp_Val, //��������ǰ�¶ȴ�����ֵ��EEV2������ȶȵ��ڣ�
	e_HeatPumpAir_SubHumAirOutOKABSTemp_Val, //��������ǰ�¶Ȳ�������ֵ��EEV2�����¶ȵ��ڣ�
	e_HeatPumpAir_WaterInOKABSTemp_Val, //����ˮ�´�����ֵ��EEV1������ȶȵ��ڣ�
	e_HeatPumpAir_WaterOutOKABSTemp_Val, //����ˮ�²�������ֵ��EEV1�����¶ȵ��ڣ�
	e_HeatPumpAir_DSTLow_Temp,						//�������ȶȵ��ڽ���ֵ
	e_HeatPumpAir_DSTLow_OutTemp,				//�������ȶȵ������ֵ
	e_HeatPumpAir_CoolModEnergyConserInFDT6Temp, //����ģʽ�豸�Խ��ܰ廻���������¶�ֵ��T6��
	e_HeatPumpAir_CoolModEnergyConserOutFDT6Temp,//����ģʽ�豸�Խ��ܰ廻���������¶���ƫֵ��T6��
	e_HeatPumpAir_CoolWaterTempEEV1Close,//����ˮ��EEV1�ر�ֵ(T8)
	e_HeatPumpAir_CoolWaterTempEEV1Open, //����ˮ��EEV1��ֵ(T8)
	e_HeatPumpAir_HeatWaterTempEEV1Close,//����ˮ��EEV1�ر�ֵ(T8)
	e_HeatPumpAir_HeatWaterTempEEV1Open, //����ˮ��EEV1��ֵ(T8)

	e_HeatPumpAir_NoEnergyConserModDevZJNFlag,   //�ǽ���ģʽ���豸�Խ��ܱ�ʶ

	e_HeatPumpAir_DevRunModStatus,			//��������ģʽ
	e_HeatPumpAir_DevRunModSubstatus,  //��������ģʽ��״̬

	e_HeatPumpAir_InterMachEvapInTemp_T9,   //�ڻ����������¶�T9
	e_HeatPumpAir_InterMachEvapOutTemp_T10,   //�ڻ����������¶�T10
	e_HeatPumpAir_InterMachFanInTemp_T11,   //�ڻ������¶�T11
	e_HeatPumpAir_InterMachBeforeReheatTemp, //�ڻ�����ǰ�¶�
	e_HeatPumpAir_InterMachAirSupplyTemp,	//�ڻ��ͷ��¶�	
	e_HeatPumpAir_InterMachXHJSSTemp,	//�ڻ�ѭ����ʪˮ��

	e_HeatPumpAir_SGS,														//�������ȶ� 
	e_HeatPumpAir_DGS,														//�������ȶ� 	
	e_HeatPumpAir_AddHumSwStatus,     //��ʪ��ŷ�״̬

	e_HeatPumpAir_InterMachSensorErr1,
	e_HeatPumpAir_InterMachSensorErr2,							//
	e_HeatPumpAir_Err1,
	e_HeatPumpAir_Err2,
	e_HeatPumpAir_Err3_1,
	e_HeatPumpAir_Err3_2,

	e_HeatPumpAir_InterMachFanEn,              //�ڲ����ʹ��
	e_HeatPumpAir_InterMachFanAct,             //�ڲ��������

	e_HeatPumpAir_BHEEV1SetVar,		//�廻�������ͷ�1�趨ֵ
	e_HeatPumpAir_BHEEV1SetVarMax,	//�廻�������ͷ�1�趨���ֵ	
	e_HeatPumpAir_BHEEV1SetVarMin,	//�廻�������ͷ�1�趨��Сֵ
	e_HeatPumpAir_BHEEV1SetInitVar,//�廻�������ͷ�1��ʼֵ

	e_HeatPumpAir_SUBHUMEEV2SetVar,		  //��ʪ�������ͷ�2�趨ֵ
	e_HeatPumpAir_SUBHUMEEV2SetVarMax,	  //��ʪ�������ͷ�2�趨���ֵ	
	e_HeatPumpAir_SUBHUMEEV2SetVarMin,	  //��ʪ�������ͷ�2�趨��Сֵ
	e_HeatPumpAir_SUBHUMEEV2SetInitVar,	//��ʪ�������ͷ�2��ʼֵ

	e_HeatPumpAir_AddHeatEEV3SetVar,		  //���ȵ������ͷ�3�趨ֵ
	e_HeatPumpAir_AddHeatEEV3SetVarMax,	//���ȵ������ͷ�3�趨���ֵ	
	e_HeatPumpAir_AddHeatEEV3SetVarMin,	//���ȵ������ͷ�3�趨��Сֵ
	e_HeatPumpAir_AddHeatEEV3SetInitVar,	//���ȵ������ͷ�3��ʼֵ

	e_HeatPumpAir_MasterPumpCon,			//��ˮ�ÿ���
	e_HeatPumpAir_CondensateLiftPumpCon,  //����ˮ�ÿ���

	//***********������ɱ������*********************
	e_HeatPumpAir_UltravioletSterilizationCon_En,       
	e_HeatPumpAir_UltravioletSterilizationCon_ConFlag,	 

	//***********�����ӿ���*************************
	e_HeatPumpAir_AnionCon_En,   
	e_HeatPumpAir_AnionCon_ConFlag,

	e_HeatPumpAir_EnterZRQYXWC,  //��������ǰ�����²�
	e_HeatPumpAir_EnterSWYXWC1,   //����ˮ�������²�1
	e_HeatPumpAir_EnterSWYXWC2,   //����ˮ�������²�2	
	e_HeatPumpAir_ExitZRQWC1,   //�˳�����ǰ�²�1
	e_HeatPumpAir_ExitZRQWC2,   //�˳�����ǰ�²�2	
	e_HeatPumpAir_HeatZRQTempSetVar,   //��������ǰ�¶�Ŀ��
	e_HeatPumpAir_HeatSFTempSetVar_High,   //�����ͷ��¶�����
	e_HeatPumpAir_HeatZRQTempSetVar_Low,   //��������ǰ�¶�����
	e_HeatPumpAir_XHADDHumWaterTempSetVar,   //ѭ����ʪˮ��Ŀ��

	e_HeatPumpAir_WaterTempSamplingTime,   //ˮ�²������ʱ��
	e_HeatPumpAir_EvaporatorTempSamplingTime,//����ǰ�¶Ȳ���ʱ��
	e_HeatPumpAir_ZRQ_CheckStartTime,        //����ǰ�¶Ȳ�����ʼʱ��
	e_HeatPumpAir_AirSupplyTempSamplingTime, //�ͷ��¶Ȳ���ʱ��
	e_HeatPumpAir_SFT_CheckStartTime,				//�ͷ��¶Ȳ�����ʼʱ��
	e_HeatPumpAir_SGS_DGSSamplingTime,       //���������ȶȲ���ʱ��
	e_HeatPumpAir_Sgs_Dgs_Max_CheckStartTime, //���������ȶȿ�ʼ���ʱ��
	e_HeatPumpAir_HighPsPretectCompresser_CheckIntervalTime,//��ѹ����ѹ���������
	e_HeatPumpAir_LowPsPretectCompresser_CheckIntervalTime,//��ѹ����ѹ���������
	e_HeatPumpAir_ClrElecExpanCumuErr_IntervalTime, //�������ͷ���̬�������
	e_HeatPumpAir_OutFanSpeed_CheckStartTime, //�ⲿ�����⿪ʼʱ��
	e_HeatPumpAir_OutFanSpeed_CheckIntervalTime,//�ⲿ������ѹ��ʱ��
	e_HeatPumpAir_WaterTempIntegralEnSubVal, //ˮ�»�����Ч�²Χ
	e_HeatPumpAir_EvaporatorTempIntegralEnSubVal,//����ǰ�¶Ȼ�����Ч�²Χ
	e_HeatPumpAir_AirSupplyTempIntegralEnSubVal, //�ͷ��¶Ȼ�����Ч�²Χ
	e_HeatPumpAir_SGS_DGS_IntegralEnSubVal,       //���������ȶȻ�����Ч�²Χ
	e_HeatPumpAir_OutFanSpeed_IntegralEnSubVal,    //�ⲿ�ֻ�ת�ٻ���ֵ��Ч��Χ
	e_HeatPumpAir_CompressorFreMax,     			//ѹ����Ƶ�����ֵ  
	e_HeatPumpAir_CompressorFreMin,     			//ѹ����Ƶ����Сֵ  

	e_HeatPumpAir_OutFanSpeed_CoolHighPressVar,//�ⲿ�������ת�ٸ�ѹĿ��ֵ
	e_HeatPumpAir_OutFanSpeed_HeatLowPressVar,//�ⲿ�������ת�ٵ�ѹĿ��ֵ
	e_HeatPumpAir_OutFanSpeed_CoolHJTemp, //�ⲿ���������ٵ㻷��
	e_HeatPumpAir_OutFanSpeed_HeatHJTemp, //�ⲿ������ȱ��ٵ㻷��
	e_HeatPumpAir_OutFanSpeed_InitVar1, //�ⲿ�����ʼת��1
	e_HeatPumpAir_OutFanSpeed_InitVar2, //�ⲿ�����ʼת��2
	e_HeatPumpAir_OutFanSpeed_Max, //�ⲿ���ת�����ֵ
	e_HeatPumpAir_OutFanSpeed_Min, //�ⲿ���ת����Сֵ

	//*****************�������************************************
	e_HeatPumpAirOut_ComHealth,//���ͨѶ����ֵ
	e_HeatPumpAirOut_XQT_T1, //ѹ���������¶�
	e_HeatPumpAirOut_PQT_T2, //ѹ���������¶�
	e_HeatPumpAirOut_LNQPGT_T3,//�������̹��¶�
	e_HeatPumpAirOut_ORT_T4, //���⻷���¶�
	e_HeatPumpAirOut_BHJKLM_T5,//�廻������ý�¶�
	e_HeatPumpAirOut_BHCKLM_T6,//�廻������ý�¶�
	e_HeatPumpAirOut_BHCS_T7,  //�廻��ˮ�¶�
	e_HeatPumpAirOut_BHJS_T8,  //�廻��ˮ�¶�
	e_HeatPumpAirOut_NoUse_T,  //����δʹ���¶ȴ�����
	e_HeatPumpAirOut_XQP,      //����ѹ��
	e_HeatPumpAirOut_PQY,      //����ѹ��
	e_HeatPumpAirOut_HPS,      //��ѹ����
	e_HeatPumpAirOut_SYYL,      //ˮѹѹ��

	e_HeatPumpAirOut_NowFre,      //ѹ������ǰƵ��
	e_HeatPumpAirOut_EEV1_NowStep,//�������ͷ���ǰֵ
	e_HeatPumpAirOut_EEV2_NowStep,//�������ͷ�2��ǰֵ
	e_HeatPumpAirOut_FAN1_Speed, //���1ת��
	e_HeatPumpAirOut_FAN2_Speed, //���2ת��
	e_HeatPumpAirOut_MasterPumpStatus,//��ˮ��״̬

	e_HeatPumpAirOut_SensorErr1,  //����������1
	e_HeatPumpAirOut_SensorErr2,  //����������2	
	e_HeatPumpAirOut_Err1,       //����1
	e_HeatPumpAirOut_BPQ_Err1,       //��Ƶ������1
	e_HeatPumpAirOut_BPQ_Err2,       //��Ƶ������2
	e_HeatPumpAirOut_BPQ_Err3,       //��Ƶ������3

	e_HeatPumpAirOut_SetDevType,        //�������
	e_HeatPumpAirOut_SetDevStatus,        //���״̬

	e_HeatPumpAirOut_SetFre,         //Ƶ���趨ֵ
	e_HeatPumpAirOut_SetSTHXF_ONFF,  //��ͨ����
	e_HeatPumpAirOut_SetEEV1Step,  //���õ������ͷ�����
	e_HeatPumpAirOut_SetEEV2Step,  //���õ������ͷ�2����
	e_HeatPumpAirOut_SetOutFanSpeed, //�����ⲿ�����ٶ�
	e_HeatPumpAirOut_SetYSJDJR_ONOFF, //ѹ��������ȿ���
	e_HeatPumpAirOut_SetMasterPumpStatus,//������ˮ��״̬
	e_HeatPumpAirOut_ErrClear,					//���ϸ�λ

	e_HeatPumpAirOut_InverterModel,    //��Ƶ���ͺ�
	e_HeatPumpAirOut_CompressorModel,  //ѹ�����ͺ�
	e_HeatPumpAirOut_UPDATAFLAG,  //������ʶ
	e_HeatPumpAirOut_ConfigFlag,  //���ø��±�ʶ
	e_HeatPumpAirOut_SWPUB,       //����汾��
	e_HeatPumpAirOut_HWPUB,       //Ӳ���汾��
	e_HeatPumpAirOut_DevType,     //�豸���ͺ�	
			
	e_HeatPumpAir_Defalut,
	
}e_HeatPumpEnum;


#endif

