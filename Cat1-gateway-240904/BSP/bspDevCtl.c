#include "bspDevCtl.h"
#include <math.h>
#include "string.h"
#include "modbusCB.h"
#include "bspDevCom.h"
#include "bspAirCom.h"
#include "bspDevTag.h"




extern s16 HoldReg_Dev[DEV_NUM_MAX][DEV_REG_NUM];
extern u8 g_handChangeFlag;


double Dew_point_Fun(double d_AirTemp, double d_AirRHum)
{

	double d_LogEW,d_DPTemp;

	if(d_AirTemp > Max_AirTemp)
	{
		d_AirTemp=Max_AirTemp;
	}
	
	if(d_AirTemp < Min_AirTemp)
	{
		d_AirTemp=Min_AirTemp;
	}
	
	if(d_AirRHum > Max_AirRH)
	{
		d_AirTemp=Max_AirRH;
	}	
	
	if(d_AirRHum < Min_AirRH)
	{
		d_AirTemp=Max_AirRH;
	}
	

	d_LogEW=(0.66077+(7.5*d_AirTemp)/(237.3+d_AirTemp)+(log10(d_AirRHum)-2));

	d_DPTemp=((0.66077-d_LogEW)*237.3)/(d_LogEW-8.16007);
	
	return d_DPTemp;
}

void bsp_dev_ctl(void)
{
	u8 devIdx, handIdx, devNumPerHand=0;
	float ludianValue[HAND_NUM_MAX]={0.0};
	
	if(usRegHoldingBuf[e_SysF_Dev1Type] != e_Air_Dehumi)	return;
	if(usRegHoldingBuf[e_SysF_DevNum] > DEV_NUM_MAX || usRegHoldingBuf[e_SysF_HandOptrNum] > HAND_NUM_MAX) return;
	
	for(handIdx=0; handIdx<usRegHoldingBuf[e_SysF_HandOptrNum]; handIdx++)
	{
//		ludianValue[handIdx]=Dew_point_Fun(usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]/10+usRegHoldingBuf[e_Hand_SetTemp_0+devIdx*HAND_REG_NUM]%10*0.1, usRegHoldingBuf[e_Hand_SetHumi_0+handIdx*HAND_REG_NUM]/10+usRegHoldingBuf[e_Hand_SetHumi_0+handIdx*HAND_REG_NUM]%10*0.1);
//		if(ludianValue[handIdx]>15.0)		ludianValue[handIdx]=15.0;
//		else if(ludianValue[handIdx]<5.0)	ludianValue[handIdx]=5.0;
		
//		if(ctl_source == CTL_UPPER)
//		{
//			
//			if(usRegHoldingBuf[e_Dev_SetONOFF+devIdx*DEV_REG_NUM] == 0)
//			{
//				HoldReg_Dev[devIdx][e_RegBuf_SetONOFF]=0;
//				continue;
//			}
//			usRegHoldingBuf[e_Dev_SetEvaporatorTemp+devIdx*DEV_REG_NUM]=ludianValue[devIdx];
//			
//			HoldReg_Dev[devIdx][e_RegBuf_SetMod]=usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM];
//			
//			HoldReg_Dev[devIdx][e_RegBuf_SetAddHumEn]=usRegHoldingBuf[e_Dev_SetAddHumEn+devIdx*DEV_REG_NUM];
//			HoldReg_Dev[devIdx][e_RegBuf_SetAddHumAct]=usRegHoldingBuf[e_Dev_SetAddHumAct+devIdx*DEV_REG_NUM];
//			
//			HoldReg_Dev[devIdx][e_RegBuf_FANControl_Speed]=usRegHoldingBuf[e_Dev_FANControl_Speed+devIdx*DEV_REG_NUM];
//			
//			HoldReg_Dev[devIdx][e_RegBuf_SetEvaporatorTemp]=usRegHoldingBuf[e_Dev_SetEvaporatorTemp+devIdx*DEV_REG_NUM];
//		}
//		else if(ctl_source == CTL_HAND)
//		{
//			if(usRegHoldingBuf[e_Hand_Switch_0+devIdx*HAND_REG_NUM] == 0)
//			{
//				HoldReg_Dev[devIdx][e_RegBuf_SetONOFF]=0;
//				continue;
//			}
//			
//			HoldReg_Dev[devIdx][e_RegBuf_SetMod]=usRegHoldingBuf[e_Dev_SetMod+devIdx*HAND_REG_NUM];
//			if(usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]>usRegHoldingBuf[e_Hand_DispHumi_0+devIdx*HAND_REG_NUM])
//			{
//				HoldReg_Dev[devIdx][e_RegBuf_SetAddHumEn]=1;
//				HoldReg_Dev[devIdx][e_RegBuf_SetAddHumAct]=1;
//			}
//			else
//			{
//				HoldReg_Dev[devIdx][e_RegBuf_SetAddHumEn]=0;
//				HoldReg_Dev[devIdx][e_RegBuf_SetAddHumAct]=0;
//			}
//			HoldReg_Dev[devIdx][e_RegBuf_FANControl_Speed]=usRegHoldingBuf[e_Hand_AirVol_0+devIdx*HAND_REG_NUM];
//			
//			HoldReg_Dev[devIdx][e_RegBuf_SetEvaporatorTemp]=ludianValue[devIdx];

//		}
		for(u8 i=0; i<2; i++)
		{
			for(u8 j=0; j<4; j++)
			{
				devIdx = usRegHoldingBuf[e_SysF_HandMap1_1+2*handIdx+i] & (0x1F<<(j*4));
				if(!devIdx)	continue;
				devIdx -= DEV_SLAVE_MIN_ADDR;
				// 开关
				usRegHoldingBuf[e_Dev_SetONOFF+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Hand_Switch_0+devIdx*HAND_REG_NUM];
				if(usRegHoldingBuf[e_Hand_Switch_0+devIdx*HAND_REG_NUM] > 1)
					usRegHoldingBuf[e_Dev_SetONOFF+devIdx*DEV_REG_NUM]=0;
				
				// 模式
				usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Hand_SetMode_0+devIdx*HAND_REG_NUM];
				if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] >= 3)	usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM]=0;
				
				// 送风温度
				if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 0)			// 除湿模式
				{
					if(usRegHoldingBuf[e_Hand_CoolAirTemp_0+handIdx*HAND_REG_NUM]<150 || usRegHoldingBuf[e_Hand_CoolAirTemp_0+handIdx*HAND_REG_NUM]>450)
					{
						usRegHoldingBuf[e_Hand_CoolAirTemp_0+handIdx*HAND_REG_NUM] = 240;	// 超过范围设置为默认值;
						usRegHoldingBuf[e_Dev_CoolAirTemp+devIdx*DEV_REG_NUM] = 240;
					}
					
					if(usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]!=usRegHoldingBuf[e_Hand_CoolAirTemp_0+handIdx*HAND_REG_NUM])
					{
						g_modbusWriteFlag |= 1<<(8+handIdx);	// 手操器参数修改标志;
						usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]=usRegHoldingBuf[e_Hand_CoolAirTemp_0+handIdx*HAND_REG_NUM];
					}
					usRegHoldingBuf[e_Dev_SetAirSupplyTemp+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Hand_CoolAirTemp_0+handIdx*HAND_REG_NUM];
				}
				else if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 1)		// 热风模式
				{
					if(usRegHoldingBuf[e_Hand_HeatAirTemp_0+handIdx*HAND_REG_NUM]<150 || usRegHoldingBuf[e_Hand_HeatAirTemp_0+handIdx*HAND_REG_NUM]>450)
					{
						usRegHoldingBuf[e_Hand_HeatAirTemp_0+handIdx*HAND_REG_NUM] = 300;	// 超过范围设置为默认值;
						usRegHoldingBuf[e_Dev_HeatAirTemp+devIdx*DEV_REG_NUM] = 300;
					}
					if(usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]!=usRegHoldingBuf[e_Hand_HeatAirTemp_0+handIdx*HAND_REG_NUM])
					{
						g_modbusWriteFlag |= 1<<(8+handIdx);	// 手操器参数修改标志;
						usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]=usRegHoldingBuf[e_Hand_HeatAirTemp_0+handIdx*HAND_REG_NUM];
					}
					usRegHoldingBuf[e_Dev_SetAirSupplyTemp+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Hand_HeatAirTemp_0+handIdx*HAND_REG_NUM];
				}
				else if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 2)		// 通风模式
				{
					if(usRegHoldingBuf[e_Hand_AirAirTemp_0+handIdx*HAND_REG_NUM]<150 || usRegHoldingBuf[e_Hand_AirAirTemp_0+handIdx*HAND_REG_NUM]>450)
					{
						usRegHoldingBuf[e_Hand_AirAirTemp_0+handIdx*HAND_REG_NUM] = 300;	// 超过范围设置为默认值;
						usRegHoldingBuf[e_Dev_AirAirTemp+devIdx*DEV_REG_NUM] = 300;
					}
					if(usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]!=usRegHoldingBuf[e_Hand_AirAirTemp_0+handIdx*HAND_REG_NUM])
					{
						g_modbusWriteFlag |= 1<<(8+handIdx);	// 手操器参数修改标志;
						usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]=usRegHoldingBuf[e_Hand_AirAirTemp_0+handIdx*HAND_REG_NUM];
					}
					usRegHoldingBuf[e_Dev_SetAirSupplyTemp+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Hand_AirAirTemp_0+handIdx*HAND_REG_NUM];
				}
//				usRegHoldingBuf[128]
//				if(usRegHoldingBuf[e_Dev_SetAirSupplyTemp+devIdx*DEV_REG_NUM]<150 || usRegHoldingBuf[e_Dev_SetAirSupplyTemp+devIdx*DEV_REG_NUM]>450)
//				{
//					g_modbusWriteFlag |= 1<<(8+handIdx);	// 手操器参数修改标志;
//					if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 0)			// 除湿模式
//					{
//						usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]=240;
//						usRegHoldingBuf[e_Hand_CoolAirTemp_0+handIdx*HAND_REG_NUM]=240;
//					}
//					else if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 1)		// 热风模式
//					{
//						usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]=300;
//						usRegHoldingBuf[e_Hand_HeatAirTemp_0+handIdx*HAND_REG_NUM]=300;
//					}
//					else if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 2)		// 通风模式
//					{
//						usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]=300;
//						usRegHoldingBuf[e_Hand_AirAirTemp_0+handIdx*HAND_REG_NUM]=300;
//					}
//				}
			
				// 露点温度
				ludianValue[handIdx]=Dew_point_Fun(usRegHoldingBuf[e_Hand_SetTemp_0+handIdx*HAND_REG_NUM]/10+usRegHoldingBuf[e_Hand_SetTemp_0+devIdx*HAND_REG_NUM]%10*0.1, usRegHoldingBuf[e_Hand_SetHumi_0+handIdx*HAND_REG_NUM]/10+usRegHoldingBuf[+handIdx*HAND_REG_NUM]%10*0.1);			
				if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 0)		// 除湿模式
				{
					ludianValue[handIdx] = ludianValue[handIdx]*10.0-5.0;
					if(ludianValue[handIdx]>150.0)		ludianValue[handIdx]=150.0;
					else if(ludianValue[handIdx]<50.0)	ludianValue[handIdx]=50.0;
					usRegHoldingBuf[e_Dev_SetEvaporatorTemp+devIdx*DEV_REG_NUM]=ludianValue[handIdx];
				}
				
				// 制热模式下，空气进风温度高于制热送风温度，则把设备设置为通风模式
				if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 1 && usRegHoldingBuf[e_Dev_InterMachFanInTemp_T11+devIdx*DEV_REG_NUM] > usRegHoldingBuf[e_Dev_SetAirSupplyTemp+devIdx*DEV_REG_NUM])
				{
					usRegHoldingBuf[e_Hand_SetMode_0+devIdx*HAND_REG_NUM]=2;
					g_modbusWriteFlag |= 1<<(8+handIdx);
				}
				
				// 加湿
				if(usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]<400 || usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]>650)
				{
					if(usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]<400)	usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]=400;
					if(usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]>650)	usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]=650;	
				}
				if(usRegHoldingBuf[e_Dev_SetMod+devIdx*DEV_REG_NUM] == 1)		// 热风模式
				{
					if(usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]>usRegHoldingBuf[e_Hand_DispHumi_0+devIdx*HAND_REG_NUM]+50)
					{
						usRegHoldingBuf[e_Dev_SetAddHumEn+devIdx*DEV_REG_NUM]=1;
						usRegHoldingBuf[e_Dev_SetAddHumAct+devIdx*DEV_REG_NUM]=1;
					}
					else if(usRegHoldingBuf[e_Hand_SetHumi_0+devIdx*HAND_REG_NUM]+50<usRegHoldingBuf[e_Hand_DispHumi_0+devIdx*HAND_REG_NUM])	
					{
						usRegHoldingBuf[e_Dev_SetAddHumEn+devIdx*DEV_REG_NUM]=0;
						usRegHoldingBuf[e_Dev_SetAddHumAct+devIdx*DEV_REG_NUM]=0;
					}
				}
				else
				{
					usRegHoldingBuf[e_Dev_SetAddHumEn+devIdx*DEV_REG_NUM]=0;
					usRegHoldingBuf[e_Dev_SetAddHumAct+devIdx*DEV_REG_NUM]=0;
				}
				if(usRegHoldingBuf[e_Dev_SetAddHumEn+devIdx*DEV_REG_NUM]>1 || usRegHoldingBuf[e_Dev_SetAddHumAct+devIdx*DEV_REG_NUM]>1)
				{
					usRegHoldingBuf[e_Dev_SetAddHumEn+devIdx*DEV_REG_NUM]=0;
					usRegHoldingBuf[e_Dev_SetAddHumAct+devIdx*DEV_REG_NUM]=0;
				}
				
				// 风速
				if(usRegHoldingBuf[e_Dev_HSpeedVol+devIdx*DEV_REG_NUM] * usRegHoldingBuf[e_Dev_MSpeedVol+devIdx*DEV_REG_NUM] * usRegHoldingBuf[e_Dev_LSpeedVol+devIdx*DEV_REG_NUM]==0)
				{
					g_modbusWriteFlag |= 1<<(8+handIdx);	// 手操器参数修改标志;
					if(!usRegHoldingBuf[e_Dev_HSpeedVol+devIdx*DEV_REG_NUM])
					{
						usRegHoldingBuf[e_Dev_HSpeedVol+devIdx*DEV_REG_NUM]=60;
						usRegHoldingBuf[e_Hand_HSpeedVol_0+handIdx*HAND_REG_NUM]=60;
					}
					if(!usRegHoldingBuf[e_Dev_MSpeedVol+devIdx*DEV_REG_NUM])
					{
						usRegHoldingBuf[e_Dev_MSpeedVol+devIdx*DEV_REG_NUM]=50;
						usRegHoldingBuf[e_Hand_MSpeedVol_0+handIdx*HAND_REG_NUM]=50;
					}
					if(!usRegHoldingBuf[e_Dev_LSpeedVol+devIdx*DEV_REG_NUM])
					{
						usRegHoldingBuf[e_Dev_LSpeedVol+devIdx*DEV_REG_NUM]=40;
						usRegHoldingBuf[e_Hand_LSpeedVol_0+handIdx*HAND_REG_NUM]=40;
					}
				}
					
				if(usRegHoldingBuf[e_Hand_AirVol_0+devIdx*HAND_REG_NUM] == 1)
					usRegHoldingBuf[e_Dev_FANControl_Speed+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Dev_LSpeedVol+devIdx*DEV_REG_NUM];
				else if(usRegHoldingBuf[e_Hand_AirVol_0+devIdx*HAND_REG_NUM] == 2)
					usRegHoldingBuf[e_Dev_FANControl_Speed+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Dev_MSpeedVol+devIdx*DEV_REG_NUM];
				else if(usRegHoldingBuf[e_Hand_AirVol_0+devIdx*HAND_REG_NUM] == 3)
					usRegHoldingBuf[e_Dev_FANControl_Speed+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Dev_HSpeedVol+devIdx*DEV_REG_NUM];
				else
					usRegHoldingBuf[e_Dev_FANControl_Speed+devIdx*DEV_REG_NUM]=usRegHoldingBuf[e_Dev_MSpeedVol+devIdx*DEV_REG_NUM];
				
				g_handChangeFlag|=1;
			}
		}
	}
}


// 手操器排程;
void bsp_dev_schedule(void)
{
	u8 temp=0, tempHour=0, tempMin=0, tempWeek=usSysRegHoldingBuf[e_Sys_SysTim_Week];
	static u8 performFlag[7][2]={0};
	static s16 tempSchedule[14]={0};
	
//	memset(performFlag, 1, sizeof(performFlag));
	// 排程时间更改，清执行标志;
	if(memcmp(tempSchedule, &usRegHoldingBuf[e_Hand_Monday1_0], sizeof(tempSchedule)))
	{
		for(u8 i=0; i<sizeof(tempSchedule)/2; i++)
		{
			if(tempSchedule[i] != usRegHoldingBuf[e_Hand_Monday1_0+i])
				performFlag[i/2][i%2]=0;
		}
		memcpy(tempSchedule, &usRegHoldingBuf[e_Hand_Monday1_0], sizeof(tempSchedule));
	}
	
	if((usRegHoldingBuf[e_Hand_SchelduEn_0] & (1<<15)) == 0)	return;
	// 清执行标志;
	if(tempWeek == 1)
	{
		performFlag[6][0]=0;
		performFlag[6][1]=0;
	}
	else
	{
		performFlag[tempWeek-2][0]=0;
		performFlag[tempWeek-2][1]=0;
	}//usRegHoldingBuf[224]
	temp=usRegHoldingBuf[e_Hand_SchelduEn_0];
	if((usRegHoldingBuf[e_Hand_SchelduEn_0] & (1<<(tempWeek-1))) == 0)	return;
	
	for(u8 i=0; i<2; i++)
	{
		if(performFlag[tempWeek-1][i])	continue;
		
		tempHour = (usRegHoldingBuf[e_Hand_Monday1_0+i+2*(tempWeek-1)] & 0x1F00)>>8;
		tempMin = (usRegHoldingBuf[e_Hand_Monday1_0+i+2*(tempWeek-1)] & 0xFF);
		if((usSysRegHoldingBuf[e_Sys_SysTim_Hour] == tempHour && usSysRegHoldingBuf[e_Sys_SysTim_Min] >= tempMin)
		|| (usSysRegHoldingBuf[e_Sys_SysTim_Hour] > tempHour))
		{
			temp = (usRegHoldingBuf[e_Hand_Monday1_0+i+2*(tempWeek-1)] & (1<<15))>>15;
//			if((usRegHoldingBuf[e_Hand_SchelduEn_0] & (1<<(6+tempWeek))) == 1)
//				usRegHoldingBuf[e_Dev_SetONOFF] = temp;
				usRegHoldingBuf[e_Hand_Switch_0] = temp;
			g_modbusWriteFlag |= (1<<8);
//			else
//				usRegHoldingBuf[e_Dev_SetEnergyConservationMod] = usRegHoldingBuf[e_Hand_Monday1_0+i+2*(tempWeek-1)] & (1<<15);
			performFlag[tempWeek-1][i] = 1;
		}
	}
}


