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



/******************************ϵͳλ��*************************************/
typedef enum
{
/*********************��������������**********************************/	
	e_Air_Temp,				// �¶�У��
	e_Air_Humi,				// ���䵱ǰʪ��
	e_Air_PM2_5,			//
	e_Air_PM10,			//
	e_Air_VOC,			//
	e_Air_CO2,			//
	e_Air_AirQualityState,		        //06-----	��������״̬  bit0:PM2.5  bit1:PM10		 bit2:CO2			bit3:voc 
	e_Air_DevErrorCode,                //07-----	�豸���ϱ���  bit0:PM2.5/PM10ģ��    bit1:CO2������     bit2:voc������     bit3:sht31
	e_Air_RGBState,			            //08-----	�����ƿ���״̬
	e_Air_PM25ValueOve,								//9-----	����PM2.5����ֵ
	e_Air_PM10ValueOve,						    //10-----	��PM10����ֵ
	e_Air_CO2ValueOve,					        //11-----	���ڶ�����̼����ֵ
	e_Air_VOCValueOve,									//12-----	����VOC����ֵ
	e_Air_PM25Offset,							//13-----	����PM2.5����ƫ����
	e_Air_PM10Offset,							//14-----	����PM10����ƫ����
	e_Air_CO2Offset,							//15-----	���ڶ�����̼����ƫ����
	e_Air_VOCOffset,							//16-----	����VOC����ƫ����
	e_Air_WorkMode,										//17-----	���ڹ���ģʽ
	e_Air_TemOffset,							//18-----	�����¶�����ƫ����
	e_Air_HumOffset,							//19-----	����ʪ������ƫ����
	e_Air_Number1,											//20-----	Ԥ��1
	e_Air_Number2,											//21-----	Ԥ��2
	
	
	e_AirHoldReg_Number,			// ���ּĴ����ܸ���
	
	
}eAirSensorHoldingReg;
/******************************ϵͳλ��*************************************/
typedef enum
{
/*********************�ֲ���**********************************/	
	e_Hand_DispTemp,			// ����ģʽ
	e_Hand_DispHumi,			// ����ģʽ
	
	e_Hand_Switch,				// ����ģʽ
	e_Hand_SetMode,				// ����ģʽ
	e_Hand_SetTemp,				// ����ģʽ
	e_Hand_SetHumi,				// ����ģʽ
	e_Hand_AirVol,				// ����ģʽ
	e_Hand_FilterTim,			// ����ģʽ
	e_Hand_FilterTimAMax,		// ����ģʽ
	e_Hand_SchelduEn,			// ����ģʽ
	e_Hand_Monday1,				// ����ģʽ
	e_Hand_Monday2,				// ����ģʽ
	e_Hand_Tues1,				// ����ģʽ
	e_Hand_Tues2,				// ����ģʽ
	e_Hand_Wednes1,				// ����ģʽ
	e_Hand_Wednes2,				// ����ģʽ
	e_Hand_Thurs1,				// ����ģʽ
	e_Hand_Thurs2,				// ����ģʽ
	e_Hand_Friday1,				// ����ģʽ
	e_Hand_Friday2,				// ����ģʽ
	e_Hand_Satur1,				// ����ģʽ
	e_Hand_Satur2,				// ����ģʽ
	e_Hand_Sunday1,				// ����ģʽ
	e_Hand_Sunday2,				// ����ģʽ
	e_Hand_TimeFlag,			// ʱ���޸ı�־
	
	e_Hand_HSpeedVol,			
	e_Hand_MSpeedVol,
	e_Hand_LSpeedVol,
	e_Hand_HeatAirTemp,
	e_Hand_CoolAirTemp,
	e_Hand_AirAirTemp,
	
	e_Hand_PM25Offset,			// ����PM2.5����ƫ����
	e_Hand_PM10Offset,			// ����PM10����ƫ����
	e_Hand_CO2Offset,			// ���ڶ�����̼����ƫ����
	e_Hand_VOCOffset,			// ����VOC����ƫ����
	e_Hand_TemOffset,			// �����¶�����ƫ����
	e_Hand_HumOffset,			// ����ʪ������ƫ����
	e_Hand_Reserve1,			// ����
	e_Hand_Reserve2,			// ����
	e_Hand_Reserve3,			// ����
	e_Hand_Reserve4,			// ����
	e_Hand_Reserve5,			// ����
	e_Hand_Reserve6,			// ����
	e_Hand_Reserve7,			// ����
	e_Hand_Reserve8,			// ����
	e_Hand_Reserve9,			// ����
	e_Hand_Reserve10,			// ����
	e_Hand_Reserve11,			// ����
	e_Hand_Reserve12,			// ����
	
	e_Hand_PM25,				// ����ģʽ
	e_Hand_PM10,				// ����ģʽ
	e_Hand_VOC,					// ����ģʽ
	e_Hand_CO2,					// ����ģʽ
	
	e_Hand_Year,				// ʱ��
	e_Hand_Month,				// ʱ��
	e_Hand_Day,					// ʱ��
	e_Hand_Hour,				// ʱ��
	e_Hand_Min,					// ʱ��
	e_Hand_Sec,					// ʱ��
	e_Hand_Week,				// ʱ��
	
	e_Hand_OutTemp,				// ��������¶�
	e_Hand_InTemp,				// �ڻ������¶�
	e_Hand_SetEvaporatorTemp,	// ����ǰ�¶�
	e_Hand_PQY,					// ����ѹ��
	e_Hand_XQP,					// ����ѹ��
	e_Hand_XQT_T1,				// ѹ���������¶�
	e_Hand_PQT_T2,				// ѹ���������¶�
	e_Hand_NowFre,				// ѹ������ǰƵ��
	e_Hand_Err1,				// ����1
	e_Hand_Err2,				// ����2
	e_Hand_Err3_1,				// ����3
	e_Hand_Err3_2,				// ����4
	
	
	e_HandHoldReg_Number,		// ���ּĴ����ܸ���
	
	
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

