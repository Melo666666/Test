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

/******************************ϵͳλ��*************************************/
typedef enum
{
	e_Sys_DevType	=	0,			// ����ģʽ
	e_Sys_DevID3,					// ʱ��
	e_Sys_DevID2,					// ʱ��
	e_Sys_DevID1,					// ʱ��
	e_Sys_DevID0,					// ʱ��
	e_Sys_DevIP1,					// ʱ��
	e_Sys_DevIP0,					// ʱ��
	e_Sys_Mask1,					// ʱ��
	e_Sys_Mask0,					// ʱ��
	e_Sys_GatewayIP1,				// ʱ��
	e_Sys_GatewayIP0,				// ʱ��
	e_Sys_MAC2,						// ʱ��
	e_Sys_MAC1,						// ʱ��
	e_Sys_MAC0,						// ʱ��
	e_Sys_Remote1IP1,				// ʱ��
	e_Sys_Remote1IP0,				// ʱ��
	e_Sys_Remote1Port_Cat1,			// ʱ��
	e_Sys_Remote1Port_Wifi,			// ʱ��
	e_Sys_Remote2IP1,				// ʱ��
	e_Sys_Remote2IP0,				// ʱ��
	e_Sys_Remote2Port_Cat1,			// ʱ��
	e_Sys_Remote2Port_Wifi,			// ʱ��
	e_Sys_SysTim_Year,				// ʱ��
	e_Sys_SysTim_Month,				// ʱ��
	e_Sys_SysTim_Day,				// ʱ��
	e_Sys_SysTim_Hour,				// ʱ��
	e_Sys_SysTim_Min,				// ʱ��
	e_Sys_SysTim_Sec,				// ʱ��
	e_Sys_SysTim_Week,				// ʱ��
	e_Sys_RunTim_Year,				// ʱ��
	e_Sys_RunTim_Month,				// ʱ��
	e_Sys_RunTim_Day,				// ʱ��
	e_Sys_RunTim_Hour,				// ʱ��
	e_Sys_RunTim_Min,				// ʱ��
	e_Sys_RunTim_Sec,				// ʱ��
	
	e_Sys_SoftVer,					// ʱ��
	e_Sys_HardwVer,					// ʱ��
	e_Sys_Upgrade,					// ʱ��
	e_Sys_CfgFlag,					// ʱ��
	
	e_Sys_Iccid1,					// ������iccid��
	e_Sys_Iccid2,					// ������iccid��
	e_Sys_Iccid3,					// ������iccid��
	e_Sys_Iccid4,					// ������iccid��
	e_Sys_Iccid5,					// ������iccid��
	
	
	e_SysReg_Number,				// ���ּĴ����ܸ���
}eSysHoldingReg;

/******************************ϵͳ����*************************************/
typedef enum
{
	e_SysF_DevNum	=	0,			// ����ģʽ
	e_SysF_Dev1Type,				// ʱ��
	e_SysF_Dev2Type,				// ʱ��
	e_SysF_Dev3Type,				// ʱ��
	e_SysF_Dev4Type,				// ʱ��
	e_SysF_Dev5Type,				// ʱ��
	e_SysF_Dev6Type,				// ʱ��
	e_SysF_Dev7Type,				// ʱ��
	e_SysF_Dev8Type,				// ʱ��
	e_SysF_MainCtl,					// ʱ��
	e_SysF_MainCtl_Health,			// ʱ��
	e_SysF_HandOptrNum,				// ʱ��
	e_SysF_AirSensorNum,			// ʱ��
	
	e_SysF_HandMap1_1,				// ʱ��
	e_SysF_HandMap1_2,
	e_SysF_HandMap2_1,				// ʱ��
	e_SysF_HandMap2_2,
	e_SysF_HandMap3_1,				// ʱ��
	e_SysF_HandMap3_2,
	e_SysF_HandMap4_1,				// ʱ��
	e_SysF_HandMap4_2,
	e_SysF_HandMap5_1,				// ʱ��
	e_SysF_HandMap5_2,
	e_SysF_HandMap6_1,				// ʱ��
	e_SysF_HandMap6_2,
	e_SysF_HandMap7_1,				// ʱ��
	e_SysF_HandMap7_2,
	e_SysF_HandMap8_1,				// ʱ��
	e_SysF_HandMap8_2,
	
	e_SysFReg_Number,				// ���ּĴ����ܸ���
}eSysFucHoldingReg;

/******************************�豸*************************************/
typedef enum
{
	e_Dev_Health_0	=	100,		// ����ģʽ
	
	/*********************ֱ��ʽ�·��ʪ��**********************************/	
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
	e_Dev_ConfigFlag,  								//�ڻ����ø��±�ʶ
	e_Dev_SWPUB,       								//�ڻ�����汾��
	e_Dev_HWPUB,       								//�ڻ�Ӳ���汾��
	e_Dev_DevType,     								//�ڻ��豸���ͺ�		
	
	e_Dev_DevAddr,											    	//�豸��ַ
	e_Dev_SetONOFF, 										      //ϵͳ����  ������  0
	e_Dev_SetAutoRun,												//����0���Զ� ����1���ֶ� 2	
	e_Dev_SetMod,													  //ģʽ����  ������  1
	e_Dev_SetEnergyConservationMod,					//����ģʽ
	e_Dev_SetSilenceMod,										  //����ģʽ
	e_Dev_SetWaterTempCon,									  //ˮ�¿��Ʒ�ʽ
	e_Dev_SetHeatWaterTemp,                  //��������ˮ��
	e_Dev_SetCoolWaterTemp,                  //��������ˮ��
	e_Dev_SetAddHumEn,              				  //��ʪʹ��
	e_Dev_SetAddHumAct,              				//��ʪ����
	e_Dev_SetEvaporatorTemp,									//��������ǰ�¶�
	e_Dev_SetAirSupplyTemp,						      //�ͷ��¶�����ֵ	
	e_Dev_FANControl_Speed,   								//���ٵ�λ	 

	e_Dev_SetSGS1,														//�������ȶ�1����
	e_Dev_SetSGS2,														//�������ȶ�2����
	e_Dev_SetSGS3,														//�������ȶ�3����
	e_Dev_SetDGS1,														//�������ȶ�1����
	e_Dev_SetDGS2,														//�������ȶ�2����	
	e_Dev_SetDGS3,														//�������ȶ�3����
	e_Dev_SetEEV2GS_Low,											//EEV2���ȶȵ�������
	e_Dev_SetEEV1GS_Low,											//EEV1���ȶȵ�������
	e_Dev_OpenEHeatHJTemp,										//��������Ȼ����¶�
	e_Dev_CloseEHeatHJTemp,									//�رյ���Ȼ����¶�
	
	//��˪
	e_Dev_DefrostingCheckTimeStart,					//��˪�������ʱ�䣨����ģʽ�£�
	e_Dev_DefrostingHJTemp1,					        //��˪�ж������¶�1
	e_Dev_DefrostingHJTemp2,					        //��˪�ж������¶�2
	e_Dev_DefrostingHJTemp3,					        //��˪�ж������¶�3
	e_Dev_DefrostingWJPGTemp1,					      //��˪�ж�����̹��¶�1
	e_Dev_DefrostingWJPGTemp2,					      //��˪�ж�����̹��¶�2
	e_Dev_DefrostingWJPGTemp3,						    //��˪�ж�����̹��¶�3
	e_Dev_DefrostingWJPGTemp4,						    //��˪�ж�����̹��¶�4
	e_Dev_DefrostingCompressorFre,		//��˪ѹ����Ƶ��
	e_Dev_DefrostingEEV1,							//��˪�������ͷ�1����
	e_Dev_DefrostingEEV2,							//��˪�������ͷ�2����	
	e_Dev_DefrostingEEV3,							//��˪�������ͷ�3����	
	e_Dev_DefrostingExitWGTemp,   //��˪�˳�����¶�
 	e_Dev_DefrostingCompressorFreRunMaxTime,   //��˪ѹ�����������ʱ��
 	e_Dev_DefrostingCompressorFreRunMinTime,   //��˪ѹ����������Сʱ��
	
  //���� 
	e_Dev_OilRecovery_CompressorFre,//����Ƶ��60         	
	e_Dev_OilRecovery_IntervalTime,//���ʱ�� ��λ����     
	e_Dev_OilRecovery_ActKeepTime, //���ͱ���ʱ�� ��λ���� 
	e_Dev_OilRecovery_EEV1SetVar,//����ʱ�������ͷ�1����
	e_Dev_OilRecovery_EEV2SetVar,//����ʱ�������ͷ�2����
	e_Dev_OilRecovery_EEV3SetVar,//����ʱ�������ͷ�3����
	
	//��ʪ
	e_Dev_AddWater_FZLowTime, 	//��ˮ���޸��ӳ�ʱʱ��
	e_Dev_AddWater_FZHighTime,	//��ˮ���޸��ӳ�ʱʱ��
	e_Dev_AddHum_FZLowTime,	//��ʪ���޸��ӳ�ʱʱ��
	e_Dev_SubWater_FZLowTime,	//��ˮ���޸��ӳ�ʱʱ��
	e_Dev_CleanWaterTank_IntervalTime,//��ϴˮ����ʱ��
	e_Dev_CleanWaterTank_ActKeepTime,//��ϴˮ�����ʱ��
	e_Dev_CleanWaterTankAddWater_FZLowTime,//��ϴ��ˮ���޸�����Ӧ������ˮʱ��
	e_Dev_AddHumStopErr_IntervalTimeTime,//��ʪ��ˮ����ͣ������-��ˮ���ʱ��
	
	e_Dev_CompressorRun_MinIntervalTime,//ѹ����������С���
	//�����������
	e_Dev_AirFilterUseTime,//�ͻ���ʹ��ʱ��  	
	e_Dev_AirFilterConfigureMaxTime,//�������ʱ��  
	e_Dev_WaterFlowSmallAlarmTempDiff,//ˮ����С�����¶Ȳ�
	e_Dev_DevResonanceFrequency1, //�豸����Ƶ��1
	e_Dev_DevResonanceFrequency2, //�豸����Ƶ��2
	e_Dev_DevResonanceFrequency3, //�豸����Ƶ��3
	e_Dev_DevResonanceFrequency4, //�豸����Ƶ��4	
	
	e_Dev_EvaporatorTempNoOKTime,  //����ǰ�¶Ȳ����ʱ��
	e_Dev_WaterTempNoOKTime,  //ˮ���¶Ȳ����ʱ��
	e_Dev_WaterTempNoOKTempDt_Cool,      //ˮ���¶Ȳ�����¶�ƫ������	
	e_Dev_WaterTempNoOKTempDt_Heat,      //ˮ���¶Ȳ�����¶�ƫ������	
	e_Dev_ZRQ_MAXTEMP_HeatModue,  //��������ǰ����趨�¶�
	e_Dev_ZRQ_MINTEMP_HeatModue,  //��������ǰ��С�趨�¶�
	e_Dev_HeatSongFengTempConEEV2ACTOffset,//�����ͷ��¶ȵ���EEV2��������Χƫ��ֵ
	e_Dev_SubHumInAirOKABSTemp_Val, //��������ǰ�¶ȴ�����ֵ��EEV2������ȶȵ��ڣ�
	e_Dev_SubHumAirOutOKABSTemp_Val, //��������ǰ�¶Ȳ�������ֵ��EEV2�����¶ȵ��ڣ�
	e_Dev_WaterInOKABSTemp_Val, //����ˮ�´�����ֵ��EEV1������ȶȵ��ڣ�
	e_Dev_WaterOutOKABSTemp_Val, //����ˮ�²�������ֵ��EEV1�����¶ȵ��ڣ�
  e_Dev_DSTLow_Temp,						//�������ȶȵ��ڽ���ֵ
	e_Dev_DSTLow_OutTemp,				//�������ȶȵ������ֵ
  e_Dev_CoolModEnergyConserInFDT6Temp, //����ģʽ�豸�Խ��ܰ廻���������¶�ֵ��T6��
	e_Dev_CoolModEnergyConserOutFDT6Temp,//����ģʽ�豸�Խ��ܰ廻���������¶���ƫֵ��T6��
	e_Dev_CoolWaterTempEEV1Close,//����ˮ��EEV1�ر�ֵ(T8)
	e_Dev_CoolWaterTempEEV1Open, //����ˮ��EEV1��ֵ(T8)
	e_Dev_HeatWaterTempEEV1Close,//����ˮ��EEV1�ر�ֵ(T8)
	e_Dev_HeatWaterTempEEV1Open, //����ˮ��EEV1��ֵ(T8)
	
	e_Dev_NoEnergyConserModDevZJNFlag,   //�ǽ���ģʽ���豸�Խ��ܱ�ʶ
	
	e_Dev_DevRunModStatus,			//��������ģʽ
	e_Dev_DevRunModSubstatus,  //��������ģʽ��״̬
	
	e_Dev_InterMachEvapInTemp_T9,   //�ڻ����������¶�T9
	e_Dev_InterMachEvapOutTemp_T10,   //�ڻ����������¶�T10
	e_Dev_InterMachFanInTemp_T11,   //�ڻ������¶�T11
	e_Dev_InterMachBeforeReheatTemp, //�ڻ�����ǰ�¶�
	e_Dev_InterMachAirSupplyTemp,	//�ڻ��ͷ��¶�	
	e_Dev_InterMachXHJSSTemp,	//�ڻ�ѭ����ʪˮ��


  
	e_DevReg_Number,				// ���ּĴ����ܸ���
	
	
	
	/*********************˫��Դ**********************************/	
	e_Dev_Double_ConfigFlag=224,
	
	e_Dev_Double_AirFilterConfigureMaxTime=156,	//�������ʱ��  55
	e_Dev_Double_AirFilterUseTime=157,			//�ͻ���ʹ�����ʱ��  56
	
	
	/*********************ֱ��ʽ�·��ʪ�����ȱ��·�һ���**********************************/	
	e_Dev_HeatPumpAir_ConfigFlag=105,
	
	e_Dev_HeatPumpAir_AirFilterUseTime=164,			//�ͻ���ʹ��ʱ��  	
	e_Dev_HeatPumpAir_AirFilterConfigureMaxTime=165,//�������ʱ��  
	
}eDevHoldingReg;

/******************************�ֲ���*************************************/
typedef enum
{
	e_Hand_Health_0	=	2180,		// ����ģʽ
	e_Hand_DispTemp_0,				// ����ģʽ
	e_Hand_DispHumi_0,				// ����ģʽ
	
	e_Hand_Switch_0,				// ����ģʽ
	e_Hand_SetMode_0,				// ����ģʽ
	e_Hand_SetTemp_0,				// ����ģʽ
	e_Hand_SetHumi_0,				// ����ģʽ
	e_Hand_AirVol_0,				// ����ģʽ
	e_Hand_FilterTim_0,				// ����ģʽ
	e_Hand_FilterTimAMax_0,			// ����ģʽ
	e_Hand_SchelduEn_0,				// ����ģʽ
	e_Hand_Monday1_0,				// ����ģʽ
	e_Hand_Monday2_0,				// ����ģʽ
	e_Hand_Tues1_0,					// ����ģʽ
	e_Hand_Tues2_0,					// ����ģʽ
	e_Hand_Wednes1_0,				// ����ģʽ
	e_Hand_Wednes2_0,				// ����ģʽ
	e_Hand_Thurs1_0,				// ����ģʽ
	e_Hand_Thurs2_0,				// ����ģʽ
	e_Hand_Friday1_0,				// ����ģʽ
	e_Hand_Friday2_0,				// ����ģʽ
	e_Hand_Satur1_0,				// ����ģʽ
	e_Hand_Satur2_0,				// ����ģʽ
	e_Hand_Sunday1_0,				// ����ģʽ
	e_Hand_Sunday2_0,				// ����ģʽ	
	e_Hand_TimeFlag_0,				// ʱ���޸ı�־
	
	e_Hand_HSpeedVol_0,			
	e_Hand_MSpeedVol_0,
	e_Hand_LSpeedVol_0,
	e_Hand_HeatAirTemp_0,
	e_Hand_CoolAirTemp_0,
	e_Hand_AirAirTemp_0,
	
	e_Hand_PM25Offset_0,			// ����PM2.5����ƫ����
	e_Hand_PM10Offset_0,			// ����PM10����ƫ����
	e_Hand_CO2Offset_0,				// ���ڶ�����̼����ƫ����
	e_Hand_VOCOffset_0,				// ����VOC����ƫ����
	e_Hand_TemOffset_0,				// �����¶�����ƫ����
	e_Hand_HumOffset_0,				// ����ʪ������ƫ����
	
	
	e_Hand_Health_1	=	2260,		// ����ģʽ
	
	e_HandReg_Number,				// ���ּĴ����ܸ���
}eHandHoldingReg;

/******************************��������������*************************************/
typedef enum
{
	e_Air_Health_0	=	2820,	// ����ģʽ
	e_Air_Temp_0,				// �¶�У��
	e_Air_Humi_0,				// ���䵱ǰʪ��
	e_Air_PM25_0,			//
	e_Air_PM10_0,			//
	e_Air_VOC_0,			//
	e_Air_CO2_0,			//
	e_Air_AirQualityState_0,		        //06-----	��������״̬  bit0:PM2.5  bit1:PM10		 bit2:CO2			bit3:voc 
	e_Air_DevErrorCode_0,                //07-----	�豸���ϱ���  bit0:PM2.5/PM10ģ��    bit1:CO2������     bit2:voc������     bit3:sht31
	e_Air_RGBState_0,			            //08-----	�����ƿ���״̬
	e_Air_PM25ValueOve_0,								//9-----	����PM2.5����ֵ
	e_Air_PM10ValueOve_0,						    //10-----	��PM10����ֵ
	e_Air_CO2ValueOve_0,					        //11-----	���ڶ�����̼����ֵ
	e_Air_VOCValueOve_0,									//12-----	����VOC����ֵ
	e_Air_PM25Offset_0,							//13-----	����PM2.5����ƫ����
	e_Air_PM10Offset_0,							//14-----	����PM10����ƫ����
	e_Air_CO2Offset_0,							//15-----	���ڶ�����̼����ƫ����
	e_Air_VOCOffset_0,							//16-----	����VOC����ƫ����
	e_Air_WorkMode_0,										//17-----	���ڹ���ģʽ
	e_Air_TemOffset_0,							//18-----	�����¶�����ƫ����
	e_Air_HumOffset_0,							//19-----	����ʪ������������
	e_Air_Number1_0,											//20-----	Ԥ��1
	e_Air_Number2_0,											//21-----	Ԥ��2
	
//	e_Air_Health_1	=	2525,	// ����ģʽ
	
	e_AirReg_Number,				// ���ּĴ����ܸ���
}eAirHoldingReg;


/******************************ϵͳλ��*************************************/
typedef enum
{
	e_RegBuf_ComHeal_1	=	0,		// ͨѶ����ֵ
	
	e_InputReg_Number	=	1,	// ��������ܸ���
}eInputReg;


void Data_Handle(void);
void modbus_init(void);

#endif
