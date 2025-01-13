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
//	u8 caliFlag;						// �����ʪ��У׼��־;
//	u8 lastOnOff;						// ��忪�ؼ���;

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


/*********************ֱ��ʽ�·��ʪ��**********************************/
/*
typedef enum
{
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
*/

//extern u16 usRegHoldBufPanel[32][e_PanelHoldReg_Number];
//extern panel_para_struct Panel_Para[PANEL_NUM_MAX];
//extern MainScreen_struct MainScreen_para;
void Dev_485_Config(void);
void Dev_Snd_Buf(u8 *sndBuf, u16 sndLen);

void Dev_TX_Service(dev_unit_struct *tempUnit);
u8 Dev_RX_Service(dev_unit_struct tempUnit);

#endif

