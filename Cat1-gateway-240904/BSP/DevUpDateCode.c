//#include "bsp_spi_flash.h"
//#include "myconfig.h"
#include "DevUpDateCode.h"
#include "string.h"
#include "mbcrc.h"
//#include "bsp_sdram.h"
#include "bsp_InternalFlash.h"
#include "mbport.h"

//#include "modbusCB.h"

S_DevRegHandle s_DevRegHandle;
S_DevUpDateCode s_DevUpDateCode;
S_SlaveDevUpDateCode s_SlaveDevUpDateCode;
//extern signed short int ssi_HeatMasterHoldingReg[REG_BUF_MAX];//寄存器缓冲
//unsigned int ui_UpDateCodeExRamBuf[SDRAM_UPDATECODEBUF_32BITLEN] __attribute__((at(SDRAM_UPDATECODE)));
//unsigned int ui_SlaveUpDateCodeExRamBuf[SDRAM_SLAVEUPDATECODEBUF_32BITLEN] __attribute__((at(SDRAM_SLAVEUPDATECODE)));

//u8 g_updateBuf[2048]={0};
//u8 g_sendBuf[100]={0}, g_sendLen=0;
//extern u8 g_updateFlag;
//extern u8 ucRTUBuf[];

struct_update g_updatePara={0};

#define SPI_FLASH_PageSize 256
void DevUpdate(void)
{
	if(g_updatePara.updateFlag)
	{
		if(g_updatePara.updateFlag==2)
		{
			GetDevUpDateCodeFream_Handle(g_updatePara.rcvBuf, g_updatePara.sndBuf, (u32*)&g_updatePara.sndLen, e_DevUpDateCode_ComTypeFlag_UartUpDate, g_updatePara.sndBuf);
//			memset(&g_updatePara.rcvBuf[14], 0xAAAA, s_DevUpDateCode.usi_NowSegLen*4);
//			Dev_Snd_Buf(&g_updatePara.rcvBuf[14], s_DevUpDateCode.usi_NowSegLen*4);
			MBPortSerialSend((char*)g_updatePara.sndBuf, g_updatePara.sndLen);
			g_updatePara.rcvLen=0;
		}
//		else if(g_updatePara.updateFlag==0x82)
//		{
//			GetDevUpDateCodeFream_Handle(rcvBuf, queueData.sndBuf, (u32*)&queueData.sndLen, e_DevUpDateCode_ComTypeFlag_ErethNetUpDate, queueData.sndBuf);
//			xQueueSend(Net_Snd_Queue, &queueData, 0);
//		}
		else
		{
			DevUpDateCode_Init();
			SlaveDevUpDateCode_Init();
			g_updatePara.updateFlag++;
		}
	}
}


void DevConfigureConVar_Init(void)
{
	s_DevRegHandle.uc_RegConfigStatus=e_RegConfigStatus_Free;
	
}

void Set_Fist_Init_Flag(unsigned short int *usi_Temp)
{
	s_DevRegHandle.usi_MemFirstInitFlag=*usi_Temp;
}

void DevConfigureHandle(void)
{
	unsigned short int i,k;
	unsigned short int usi_Temp;
//	switch(s_DevRegHandle.uc_RegConfigStatus)
//	{
//		case e_RegConfigStatus_Free:
//				if(ssi_HeatMasterHoldingReg[e_InHoldingReg_ConfigFlag]==e_SysVarConUpDataFlag_Start)	
//				{
//					s_DevRegHandle.uc_RegConfigStatus=e_RegConfigStatus_ComStart;
//					
//					//关系统
//					ssi_HeatMasterHoldingReg[e_RegBuf_SetONOFF]=e_SysOff;
//				}
//				
//				if(ssi_HeatMasterHoldingReg[e_InHoldingReg_ConfigFlag]==e_SysVarConUpDataFlag_End)					
//				{
//					s_DevRegHandle.uc_RegConfigStatus=e_RegConfigStatus_ComEnd;
//				}				
//			break;

//		case e_RegConfigStatus_ComStart:
//				if(ssi_HeatMasterHoldingReg[e_InHoldingReg_ConfigFlag]==e_SysVarConUpDataFlag_Start)	
//				{
//					s_DevRegHandle.uc_RegConfigStatus=e_RegConfigStatus_ComStart;
//					
//					//关系统
//					ssi_HeatMasterHoldingReg[e_RegBuf_SetONOFF]=e_SysOff;
//				}
//				
//				if(ssi_HeatMasterHoldingReg[e_InHoldingReg_ConfigFlag]==e_SysVarConUpDataFlag_End)					
//				{
//					s_DevRegHandle.uc_RegConfigStatus=e_RegConfigStatus_ComEnd;
//				}	
//			break;		
//		
//		case e_RegConfigStatus_ComEnd:
//			  //检测关键设备有没有关机(热泵)
//				
//				s_DevRegHandle.uc_RegConfigStatus=e_RegConfigStatus_BackUpToNow;
//			break;				
//		
//		case e_RegConfigStatus_BackUpToNow:
//			 
//			 NVIC_SystemReset();	
//			  
//			break;						
//	}
}

//****************************************程序更新*************************************
void DevUpDateCode_Init(void)
{
	memset(&s_DevUpDateCode,0,sizeof(S_DevUpDateCode));
//	s_DevUpDateCode.ui_TempBufAddr=g_updatePara.rcvBuf;//地址赋值
}

void SlaveDevUpDateCode_Init(void)
{
	memset(&s_SlaveDevUpDateCode,0,sizeof(S_SlaveDevUpDateCode));
	
//	s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TempBufAddr=g_updatePara.rcvBuf;
}

void Set_DevUpDateCode_Status(unsigned char uc_Temp)
{
	s_DevUpDateCode.uc_UpDateCodeStatus=uc_Temp;
}

void Set_SlaveDevUpDateCode_Status(unsigned char uc_Temp)
{
	s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus=uc_Temp;
}

void DevUpDateCode_TimeCount(void)
{
	s_DevUpDateCode.ui_Time++;
	s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_Time++;
}



void DevUpDateCode_TimeClr(void)
{
	s_DevUpDateCode.ui_Time=0;
}

void SlaveDevUpDateCode_TimeClr(void)
{
	s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_Time=0;
}

unsigned char IsDevUpDateCode_TimeOver(void)
{
	if(s_DevUpDateCode.ui_Time >= DevUpDateCode_MaxTime)
	{
		g_updatePara.updateFlag=0;
		return 0x01;
	}
	
	return 0;
}

unsigned char IsSlaveDevUpDateCode_TimeOver(void)
{
	if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_Time >= DevUpDateCode_MaxTime)
	{
		g_updatePara.updateFlag=0;
		return 0x01;
	}
	
	return 0;
}

void SetSlaveDateCodeComErr(unsigned char uc_Temp)
{
	s_SlaveDevUpDateCode.uc_ComErrFlag=uc_Temp;
}

unsigned char IsSlaveDateCodeComErrStatus(void)
{
	return s_SlaveDevUpDateCode.uc_ComErrFlag;
}

unsigned char SlaveDateCodeComBackHandle(unsigned char * uc_BufPtr)
{
	unsigned int ui_Temp1,ui_Temp2,ui_Temp3, tempCode=0;
	
	*((unsigned char*)&ui_Temp1+3)=*(uc_BufPtr+2);
	*((unsigned char*)&ui_Temp1+2)=*(uc_BufPtr+3);
	*((unsigned char*)&ui_Temp1+1)=*(uc_BufPtr+4);
	*((unsigned char*)&ui_Temp1+0)=*(uc_BufPtr+5);
		
	*((unsigned char*)&ui_Temp2+3)=*(uc_BufPtr+6);
	*((unsigned char*)&ui_Temp2+2)=*(uc_BufPtr+7);
	*((unsigned char*)&ui_Temp2+1)=*(uc_BufPtr+8);
	*((unsigned char*)&ui_Temp2+0)=*(uc_BufPtr+9);
	
	 ui_Temp3=usMBCRC16(uc_BufPtr,13);
		
	if(s_SlaveDevUpDateCode.uc_SlaveType==SlaveInUpdate)
	{
		tempCode=0x15;	 //功能码
	}
	else if(s_SlaveDevUpDateCode.uc_SlaveType==SlaveOutUpdate)
	{
		tempCode=0x16;	 //功能码
	}
		 
//	if(*uc_BufPtr == 99 &&
	if(*uc_BufPtr == 1 && 
		 *(uc_BufPtr+1) == tempCode&&
	   ui_Temp1==s_SlaveDevUpDateCode.ui_TotalSegNum&&
	   ui_Temp2==s_SlaveDevUpDateCode.ui_NowSegNum&&
	  *(uc_BufPtr+10)==s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_SessionNum&&
	  *(uc_BufPtr+12)==0&&
	  *(uc_BufPtr+13)==*((unsigned char*)&ui_Temp3)&&
		*(uc_BufPtr+14)==*((unsigned char*)&ui_Temp3+1)
	  )
	  {
//			if(*(uc_BufPtr+11)==e_SlaveDevUpDateCode_End)
//			{
//				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_ToSlaveEnd);
//			}
			
			if(*(uc_BufPtr+11)==e_SlaveDevUpDateCode_Start)
			{
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_ToSlaveSending);
			}
			if(*(uc_BufPtr+11)==e_SlaveDevUpDateCode_Sending)
			{
				s_SlaveDevUpDateCode.ui_CodePtr+=s_SlaveDevUpDateCode.usi_NowSegLen;
			}
			
			SetSlaveDateCodeComErr(0);
		}
		else
		{
			SetSlaveDateCodeComErr(1);
		}
}

unsigned char IsSlaveDevDateCode(void)
{
	 return s_SlaveDevUpDateCode.uc_SlaveUpDateFlag;
}


unsigned char SetGetDevUpDateCodeFream_ComType(unsigned char uc_ComType)
{
	s_DevUpDateCode.uc_ComTypeFlag=uc_ComType;
}

unsigned char SetGetSlaveDevUpDateCodeFream_ComType(unsigned char uc_ComType)
{
	s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_ComTypeFlag=uc_ComType;
}


unsigned char GetGetDevUpDateCodeFream_ComType(void)
{
	return s_DevUpDateCode.uc_ComTypeFlag;
}

unsigned char GetGetSlaveDevUpDateCodeFream_ComType(void)
{
	return s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_ComTypeFlag;
}

void DevUpDateCodeHandle(void)
{
//	unsigned char uc_Temp22[20];
	
	unsigned int ui_Temp,ui_Temp1,i, tempBuf[2]={0};
	unsigned short int usi_Temp;
	switch(s_DevUpDateCode.uc_UpDateCodeStatus)
	{
		case e_DevUpDateCode_Free:
//			g_updatePara.updateFlag=0;
			DevUpDateCode_TimeClr();
			break;
		
		case e_DevUpDateCode_Start:
			if(IsDevUpDateCode_TimeOver())
			{
				Set_DevUpDateCode_Status(e_DevUpDateCode_Free);
			}
			break;
		
		case e_DevUpDateCode_Sending:
			if(IsDevUpDateCode_TimeOver())
			{
				Set_DevUpDateCode_Status(e_DevUpDateCode_Free);
			}		
			break;
		
		case e_DevUpDateCode_End:
			Set_DevUpDateCode_Status(e_DevUpDateCode_MemToFlash);
			break;
		
		case e_DevUpDateCode_MemToFlash:
			if(s_DevUpDateCode.ui_CodePtr==0)
			{
				Set_DevUpDateCode_Status(e_DevUpDateCode_Free);
			}
			else
			{
				ui_Temp=s_DevUpDateCode.ui_CodePtr*4;
				
				tempBuf[0]=1;
				tempBuf[1]=ui_Temp;
				STMFLASH_Write(SOFTWARE_STARTMOD_FLAG, (u16*)tempBuf, 4);
//				ui_Temp1=ui_Temp/SPI_FLASH_PageSize;
//				if((ui_Temp%SPI_FLASH_PageSize)!=0)
//				{
//					ui_Temp1+=1;
//				}

				//从外部ram写入Flash
//				for(i=0;i<ui_Temp1;i++)
//				{
//					SPI_FLASH_SectorErase(i*SPI_FLASH_PageSize);
//					IWDG_Feed();
//				}			

//				SPI_FLASH_BufferWrite_Bit32((unsigned char*)(s_DevUpDateCode.ui_TempBufAddr),0,ui_Temp);					
//        IWDG_Feed();	
//				SPI_FLASH_BufferRead_Bit32((unsigned char*)s_DevUpDateCode.ui_TempBufAddr,0,ui_Temp);			
//				IWDG_Feed();	
//				//记录程序文件长度

//				usi_Temp=*((unsigned short int*)&s_DevUpDateCode.ui_CodePtr+1);
//				DataSaveToFram(UPDATECODE_LEN_H16BIT,(unsigned char*)usi_Temp,1);

//				usi_Temp=*((unsigned short int*)&s_DevUpDateCode.ui_CodePtr);
//				DataSaveToFram(UPDATECODE_LEN_L16BIT,(unsigned char*)usi_Temp,1);
//				
//				//修改程序升级标识
//				usi_Temp=0xaabb;
//				DataSaveToFram(e_InHoldingReg_CodeIAPFlag_1,(unsigned char*)usi_Temp,1);
//				
//				usi_Temp=0xccdd;
//				DataSaveToFram(e_InHoldingReg_CodeIAPFlag_2,(unsigned char*)usi_Temp,1);

//				usi_Temp=0xeeff;
//				DataSaveToFram(e_InHoldingReg_CodeIAPFlag_3,(unsigned char*)usi_Temp,1);
//				
//				usi_Temp=0x1122;
//				DataSaveToFram(e_InHoldingReg_CodeIAPFlag_4,(unsigned char*)usi_Temp,1);		

        vTaskDelay(100);
//				
				NVIC_SystemReset();
				
			}
		break;
		
	}
}


void SlaveDevUpDateCodeHandle(void)
{
//	unsigned char uc_Temp22[20];
	
	unsigned int ui_Temp,ui_Temp1,i;
	unsigned short int usi_Temp;
	switch(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus)
	{
		case e_SlaveDevUpDateCode_Free:
			SlaveDevUpDateCode_TimeClr();
			s_SlaveDevUpDateCode.uc_SlaveUpDateFlag=0;
//			g_updatePara.updateFlag=0;
			break;
		
		case e_SlaveDevUpDateCode_Start:
			if(IsSlaveDevUpDateCode_TimeOver())
			{
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_Free);
			}
			break;
		
		case e_SlaveDevUpDateCode_Sending:
			if(IsSlaveDevUpDateCode_TimeOver())
			{
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_Free);
			}		
			break;
		
		case e_SlaveDevUpDateCode_End:
			
			if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr==0)
			{
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_Free);
			}
			else
			{
				s_SlaveDevUpDateCode.uc_SlaveUpDateFlag=1;
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_ToSlaveStart);
				SlaveDevUpDateCode_TimeClr();
			}		
			break;
		
		case e_SlaveDevUpDateCode_ToSlaveStart:
			g_updatePara.updateFlag |= 3;
			if(IsSlaveDevUpDateCode_TimeOver())
			{
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_Free);
			}	
		break;
			
		case e_SlaveDevUpDateCode_ToSlaveSending:
			if(IsSlaveDevUpDateCode_TimeOver())
			{
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_Free);
			}				
			break;
		
		case e_SlaveDevUpDateCode_ToSlaveEnd:
		//	if(IsSlaveDevUpDateCode_TimeOver())
			//{
				Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_Free);
				g_updatePara.updateFlag = 0;
			//}				
			break;
	}
}

unsigned char SlaveDevDateCodeSend(unsigned char *uc_BufPtr,unsigned short int *usi_ComLen)
{
	unsigned short int usi_temp1;
	unsigned char uc_Temp;
	unsigned int i,i_temp;
	switch(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus)
	{
		case e_SlaveDevUpDateCode_ToSlaveStart:
			 s_SlaveDevUpDateCode.ui_TotalSegNum = s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr*4/SlaveDevComBufLen;
		   
		   if((s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr*4)%SlaveDevComBufLen)
			 {
				 s_SlaveDevUpDateCode.ui_TotalSegNum +=1;
			 }
		
			 s_SlaveDevUpDateCode.ui_NowSegNum=0;
			 
			 s_SlaveDevUpDateCode.usi_NowSegLen=0;
			 
			 s_SlaveDevUpDateCode.ui_CodePtr=0;

			 if(s_SlaveDevUpDateCode.uc_SlaveType==SlaveInUpdate)
			 {
				*uc_BufPtr=1;	       //设备地址
				*(uc_BufPtr+1)=0x15;	 //功能码
			 }
			 else if(s_SlaveDevUpDateCode.uc_SlaveType==SlaveOutUpdate)
			 {
				 *uc_BufPtr=1;	       //设备地址
				*(uc_BufPtr+1)=0x16;	 //功能码
			 }
			 *(uc_BufPtr+2)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum+3);
			 *(uc_BufPtr+3)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum+2);
			 *(uc_BufPtr+4)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum+1);
			 *(uc_BufPtr+5)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum);
			 
			 *(uc_BufPtr+6)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum+3);
			 *(uc_BufPtr+7)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum+2);
			 *(uc_BufPtr+8)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum+1);
			 *(uc_BufPtr+9)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum);	

			 *(uc_BufPtr+10)=s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_SessionNum;
       *(uc_BufPtr+11)=e_SlaveDevUpDateCode_Start;
			 
			 *(uc_BufPtr+12)=*((unsigned char*)&s_SlaveDevUpDateCode.usi_NowSegLen+1);
			 *(uc_BufPtr+13)=*((unsigned char*)&s_SlaveDevUpDateCode.usi_NowSegLen);
			 
			 usi_temp1=usMBCRC16(uc_BufPtr,14);
			 *(uc_BufPtr+14)=*((unsigned char*)&usi_temp1);
			 *(uc_BufPtr+15)=*((unsigned char*)&usi_temp1+1);
			 
			 *usi_ComLen=16;
       			 
			break;
			
		case e_SlaveDevUpDateCode_ToSlaveSending:
//			if(s_SlaveDevUpDateCode.ui_TotalSegNum>=s_SlaveDevUpDateCode.ui_NowSegNum)
//			{
//				s_SlaveDevUpDateCode.ui_CodePtr=s_SlaveDevUpDateCode.ui_NowSegNum*((SlaveDevComBufLen)/4);
//			}			
			
			
			if(!IsSlaveDateCodeComErrStatus())
			 {
				s_SlaveDevUpDateCode.ui_NowSegNum++;
			 }
			 
			if(s_SlaveDevUpDateCode.ui_NowSegNum>=s_SlaveDevUpDateCode.ui_TotalSegNum)
			{
				 uc_Temp=e_SlaveDevUpDateCode_End;
			}
			else
			{
				uc_Temp=e_SlaveDevUpDateCode_Sending;
			}				
				
			usi_temp1=s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr-s_SlaveDevUpDateCode.ui_CodePtr;
		   
			 if(usi_temp1>=((SlaveDevComBufLen)/4))	
       {				 
					s_SlaveDevUpDateCode.usi_NowSegLen=((SlaveDevComBufLen)/4);
			 }
			 else
			 {
				 s_SlaveDevUpDateCode.usi_NowSegLen=usi_temp1;
			 }
			 if(s_SlaveDevUpDateCode.uc_SlaveType==SlaveInUpdate)
			 {
				*uc_BufPtr=1;	       //设备地址
				*(uc_BufPtr+1)=0x15;	 //功能码
			 }
			 else if(s_SlaveDevUpDateCode.uc_SlaveType==SlaveOutUpdate)
			 {
				 *uc_BufPtr=1;	       //设备地址
				*(uc_BufPtr+1)=0x16;	 //功能码
			 }
			 *(uc_BufPtr+2)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum+3);
			 *(uc_BufPtr+3)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum+2);
			 *(uc_BufPtr+4)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum+1);
			 *(uc_BufPtr+5)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_TotalSegNum);
			 *(uc_BufPtr+6)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum+3);
			 *(uc_BufPtr+7)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum+2);
			 *(uc_BufPtr+8)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum+1);
			 *(uc_BufPtr+9)=*((unsigned char*)&s_SlaveDevUpDateCode.ui_NowSegNum);			
			 *(uc_BufPtr+10)=s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_SessionNum;
			 *(uc_BufPtr+11)=uc_Temp;
			 *(uc_BufPtr+12)=*((unsigned char*)&s_SlaveDevUpDateCode.usi_NowSegLen+1);
			 *(uc_BufPtr+13)=*((unsigned char*)&s_SlaveDevUpDateCode.usi_NowSegLen);

//			 for(i=0;i<s_SlaveDevUpDateCode.usi_NowSegLen;i++)
//			 {
//				  i_temp=s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TempBufAddr[s_SlaveDevUpDateCode.ui_CodePtr+i];
//				  *(uc_BufPtr+14+i*4)=*((unsigned char*)&i_temp+0);
//				  *(uc_BufPtr+15+i*4)=*((unsigned char*)&i_temp+1);
//				  *(uc_BufPtr+16+i*4)=*((unsigned char*)&i_temp+2);
//				  *(uc_BufPtr+17+i*4)=*((unsigned char*)&i_temp+3);
//			 }

			STMFLASH_Read(ADDR_OF_MachineUpAddr+s_SlaveDevUpDateCode.ui_CodePtr*4, (u16*)(uc_BufPtr+14), s_SlaveDevUpDateCode.usi_NowSegLen*2);
//			s_SlaveDevUpDateCode.ui_CodePtr += s_SlaveDevUpDateCode.usi_NowSegLen;
			// memcpy((uc_BufPtr+14),&(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TempBufAddr[s_SlaveDevUpDateCode.ui_CodePtr]),s_SlaveDevUpDateCode.usi_NowSegLen*4);        
//			 if(!IsSlaveDateCodeComErrStatus()) 
//			 {
//				 s_SlaveDevUpDateCode.ui_CodePtr+=s_SlaveDevUpDateCode.usi_NowSegLen;
//			 }
			 
       usi_temp1=usMBCRC16(uc_BufPtr,(14+s_SlaveDevUpDateCode.usi_NowSegLen*4));		

			 *(uc_BufPtr+(14+s_SlaveDevUpDateCode.usi_NowSegLen*4))=*((unsigned char*)&usi_temp1);
			 *(uc_BufPtr+(15+s_SlaveDevUpDateCode.usi_NowSegLen*4))=*((unsigned char*)&usi_temp1+1);
			 
			 if(uc_Temp == e_SlaveDevUpDateCode_End)
			 {
//				 Set_DevUpDateCode_Status(e_SlaveDevUpDateCode_End);
				 Set_SlaveDevUpDateCode_Status(e_SlaveDevUpDateCode_ToSlaveEnd);
				 s_SlaveDevUpDateCode.uc_SlaveUpDateFlag=0;
			 }
			 
			 *usi_ComLen=16+s_SlaveDevUpDateCode.usi_NowSegLen*4;

			break;
				
  		case e_SlaveDevUpDateCode_ToSlaveEnd:
//			g_updatePara.updateFlag=0;
			break;
	}
}

unsigned char IsSlaveDevDateCodeOK(void)
{
	if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus == e_SlaveDevUpDateCode_ToSlaveStart||
		 s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus == e_SlaveDevUpDateCode_ToSlaveSending||
	   s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus == e_SlaveDevUpDateCode_ToSlaveEnd
	)
	{
		return 0;
	}
	
	return 1;
}

unsigned char GetDevUpDateCodeFream_FunCode(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
	{
		return *(uc_temp+1);
	}
	else if(uc_ComType == e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
	{
		return *(uc_temp+2);
	}	
}

unsigned int GetDevUpDateCodeFream_TotalSegNum(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
	{
		return (*(uc_temp+2)<<24|*(uc_temp+3)<<16|*(uc_temp+4)<<8|*(uc_temp+5));
	}
	else if(uc_ComType == e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
	{
		return (*(uc_temp+3)<<24|*(uc_temp+4)<<16|*(uc_temp+5)<<8|*(uc_temp+6));
	}
}

unsigned int GetDevUpDateCodeFream_NowSegNum(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
	{
		return (*(uc_temp+6)<<24|*(uc_temp+7)<<16|*(uc_temp+8)<<8|*(uc_temp+9));
	}
	else if(uc_ComType == e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
	{
		return (*(uc_temp+7)<<24|*(uc_temp+8)<<16|*(uc_temp+9)<<8|*(uc_temp+10));
	}
}

unsigned char GetDevUpDateCodeFream_SessionNum(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
	{
		return *(uc_temp+10);
	}
	else if(uc_ComType == e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
	{
		return *(uc_temp+11);		
	}
}

unsigned char GetDevUpDateCodeFream_UpDateCodeStatus(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
	{
		return *(uc_temp+11);
	}
	else if(uc_ComType == e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
	{
		return *(uc_temp+12);		
	}
}

unsigned short GetDevUpDateCodeFream_NowSegLen(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
	{
		return (*(uc_temp+12)<<8|*(uc_temp+13));
	}
	else if(uc_ComType == e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
	{
		return (*(uc_temp+13)<<8|*(uc_temp+14));		
	}
}

unsigned char IsGetDevUpDateCodeFream_NowSegLenOk(unsigned char * uc_temp,unsigned int ui_TempMaxLen,unsigned char uc_ComType)
{
	if(GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType)>=ui_TempMaxLen)
	{
		return 0;
	}
	
	return 1;
}

unsigned char IsGetDevUpDateCodeFream_UpDateCodeStatusOk(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType)==e_DevUpDateCode_Start||
		 GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType)==e_DevUpDateCode_Sending||
	   GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType)==e_DevUpDateCode_End
	)
	{
		return 1;
	}
	return 0;
}

unsigned char IsGetSlaveDevUpDateCodeFream_UpDateCodeStatusOk(unsigned char * uc_temp,unsigned char uc_ComType)
{
	if(GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType)==e_SlaveDevUpDateCode_Start||
		 GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType)==e_SlaveDevUpDateCode_Sending||
	   GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType)==e_SlaveDevUpDateCode_End
	)
	{
		return 1;
	}
	return 0;	
}


unsigned char  GetDevUpDateCodeFream_Handle(unsigned char *uc_temp,unsigned char* uc_sendbuf,unsigned int*ui_sendlentemp ,unsigned char uc_ComType,unsigned char *uc_IpTypeBuf)
{
	unsigned char uc_ErrFlagTemp;
	unsigned int i;
	unsigned short int usi_temp1;
	unsigned int *ui_TempPtr;
	
	void * v_Temp;
				
	uc_ErrFlagTemp=0;
	

	//状态判断
	if(GetDevUpDateCodeFream_FunCode(uc_temp,uc_ComType)==MasterUpdate)
	{	
		if(!IsGetDevUpDateCodeFream_UpDateCodeStatusOk(uc_temp,uc_ComType))
		{
			uc_ErrFlagTemp|=0x08;
		}
	
		//长度判读
		if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
		{
				if(GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType) > UART_NowSegLen_Max)
				{
					uc_ErrFlagTemp|=0x20;
				}
				
				v_Temp=uc_IpTypeBuf;
		}
		else if(uc_ComType==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
		{
				if(GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType) > Eth_NowSegLen_Max)
				{
					uc_ErrFlagTemp|=0x20;
				}
						
				uc_sendbuf=uc_IpTypeBuf;
		}
		
		//获取总段数
		
		switch(GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType))
		{
			case e_DevUpDateCode_Start:
				DevUpDateCode_TimeClr();
				//获取总段数
				s_DevUpDateCode.ui_TotalSegNum=GetDevUpDateCodeFream_TotalSegNum(uc_temp,uc_ComType);
				s_DevUpDateCode.ui_NowSegNum=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType);
				s_DevUpDateCode.uc_SessionNum=GetDevUpDateCodeFream_SessionNum(uc_temp,uc_ComType);
				if(s_DevUpDateCode.ui_NowSegNum!=0)
				{
					uc_ErrFlagTemp|=0x02;
				}
				
				if(uc_ErrFlagTemp==0)
				{
					SetGetDevUpDateCodeFream_ComType(uc_ComType);
					s_DevUpDateCode.usi_NowSegLen=GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType);
					s_DevUpDateCode.ui_CodePtr=0;
					
					if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						ui_TempPtr=(unsigned int*)(uc_temp+14);
//						ui_TempPtr=(unsigned char*)(uc_temp+14);
					}
					else if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
						ui_TempPtr=(unsigned int*)(uc_temp+15);
//						ui_TempPtr=(unsigned char*)(uc_temp+15);
					}
					
					//数据移动到外部存储器
//					for(i=0;i<s_DevUpDateCode.usi_NowSegLen;i++)
//					{
//					
//						*(s_DevUpDateCode.ui_TempBufAddr+s_DevUpDateCode.ui_CodePtr)=*(ui_TempPtr+i);
//						
//						s_DevUpDateCode.ui_CodePtr++;
//					}
					
//					STMFLASH_Write(ADDR_OF_GatewayUpAddr+s_DevUpDateCode.ui_CodePtr*4, (u16*)ui_TempPtr, s_DevUpDateCode.usi_NowSegLen*2);
//					s_DevUpDateCode.ui_CodePtr += s_DevUpDateCode.usi_NowSegLen;
				}
				else
				{
					s_DevUpDateCode.usi_NowSegLen=0;
					s_DevUpDateCode.ui_CodePtr=0;
					SetGetDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
				}
				
				if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
				{
					//数据搬送完回包			
						memcpy(uc_sendbuf,uc_temp,12);
						*(uc_sendbuf+12)=uc_ErrFlagTemp;
						usi_temp1=usMBCRC16(uc_sendbuf,13);
						*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
						*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
						*ui_sendlentemp=15;
						
				}
				else if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
				{
						memcpy(uc_sendbuf,uc_temp,13);
						*(uc_sendbuf+13)=uc_ErrFlagTemp;
						usi_temp1=usMBCRC16(uc_sendbuf,14);
						*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
						*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
						*ui_sendlentemp=16;					
				}			
					
				if(uc_ErrFlagTemp)	
				{
						return 0;
				}
				
				s_DevUpDateCode.uc_UpDateCodeStatus=GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType);
				break;
				
				case e_DevUpDateCode_Sending:
					if(s_DevUpDateCode.ui_TotalSegNum!=GetDevUpDateCodeFream_TotalSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x01;
					}
					
					if((s_DevUpDateCode.ui_NowSegNum+1)!=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x02;
					}
					else
					{
						s_DevUpDateCode.ui_NowSegNum=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType);
					}
					
					if(s_DevUpDateCode.uc_SessionNum!=GetDevUpDateCodeFream_SessionNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x04;
					}
					
					if(uc_ErrFlagTemp==0)
					{
						SetGetDevUpDateCodeFream_ComType(uc_ComType);
						s_DevUpDateCode.usi_NowSegLen=GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType);
						
						if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+14);
//							ui_TempPtr=(unsigned char*)(uc_temp+14);
						}
						else if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+15);
//							ui_TempPtr=(unsigned char*)(uc_temp+15);
						}
										
						//数据移动到外部存储器
//						for(i=0;i<s_DevUpDateCode.usi_NowSegLen;i++)
//						{
//							*(s_DevUpDateCode.ui_TempBufAddr+s_DevUpDateCode.ui_CodePtr)=*(ui_TempPtr+i);
//							
//							s_DevUpDateCode.ui_CodePtr++;
//						}
//						memset(ui_TempPtr, 0xBBBB, s_DevUpDateCode.usi_NowSegLen*4);
						STMFLASH_Write(ADDR_OF_GatewayUpAddr+s_DevUpDateCode.ui_CodePtr*4, (u16*)ui_TempPtr, s_DevUpDateCode.usi_NowSegLen*2);
						s_DevUpDateCode.ui_CodePtr += s_DevUpDateCode.usi_NowSegLen;
					}
					else
					{
						s_DevUpDateCode.usi_NowSegLen=0;
						s_DevUpDateCode.ui_CodePtr=0;
						SetGetDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
					}
					
					if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						//数据搬送完回包			
							memcpy(uc_sendbuf,uc_temp,12);
							*(uc_sendbuf+12)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,13);
							*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=15;
							
					}
					else if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
							memcpy(uc_sendbuf,uc_temp,13);
							*(uc_sendbuf+13)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,14);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=16;					
					}			
						
					if(uc_ErrFlagTemp)	
					{
							return 0;
					}				
					
					s_DevUpDateCode.uc_UpDateCodeStatus=GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType);
				break;
					
				case e_DevUpDateCode_End:
					
					if(s_DevUpDateCode.ui_TotalSegNum!=GetDevUpDateCodeFream_TotalSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x01;
					}
					
					if((s_DevUpDateCode.ui_NowSegNum+1)!=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x02;
					}
					else
					{
						s_DevUpDateCode.ui_NowSegNum=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType);
					}
					
					if(s_DevUpDateCode.uc_SessionNum!=GetDevUpDateCodeFream_SessionNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x04;
					}
					
					if(uc_ErrFlagTemp==0)
					{
						SetGetDevUpDateCodeFream_ComType(uc_ComType);
						s_DevUpDateCode.usi_NowSegLen=GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType);
						
						if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+14);
//							ui_TempPtr=(unsigned char*)(uc_temp+14);
						}
						else if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+15);
//							ui_TempPtr=(unsigned char*)(uc_temp+15);
						}					
						//数据移动到外部存储器
//						for(i=0;i<s_DevUpDateCode.usi_NowSegLen;i++)
//						{
//							*(s_DevUpDateCode.ui_TempBufAddr+s_DevUpDateCode.ui_CodePtr)=*(ui_TempPtr+i);
//							
//							s_DevUpDateCode.ui_CodePtr++;
//						}
//						memset(ui_TempPtr, 0xAAAA, s_DevUpDateCode.usi_NowSegLen*4);
						STMFLASH_Write(ADDR_OF_GatewayUpAddr+s_DevUpDateCode.ui_CodePtr*4, (u16*)ui_TempPtr, s_DevUpDateCode.usi_NowSegLen*2);
						s_DevUpDateCode.ui_CodePtr += s_DevUpDateCode.usi_NowSegLen;
					}
					else
					{
						s_DevUpDateCode.usi_NowSegLen=0;
						s_DevUpDateCode.ui_CodePtr=0;
						SetGetDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
					}
					
					if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						//数据搬送完回包			
							memcpy(uc_sendbuf,uc_temp,12);
							*(uc_sendbuf+12)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,13);
							*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=15;
							
					}
					else if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
							memcpy(uc_sendbuf,uc_temp,13);
							*(uc_sendbuf+13)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,14);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=16;					
					}			
						
					if(uc_ErrFlagTemp)	
					{
							return 0;
					}	
					s_DevUpDateCode.uc_UpDateCodeStatus=GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType); 				
				break;
			
			default:
					s_DevUpDateCode.usi_NowSegLen=0;
					s_DevUpDateCode.ui_CodePtr=0;
					SetGetDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
			
					if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						//数据搬送完回包			
							memcpy(uc_sendbuf,uc_temp,12);
							*(uc_sendbuf+12)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,13);
							*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=15;
							
					}
					else if(GetGetDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
							memcpy(uc_sendbuf,uc_temp,13);
							*(uc_sendbuf+13)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,14);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=16;					
					}				
				return 0;
			break;
			
			
		}	
	}
	else if(GetDevUpDateCodeFream_FunCode(uc_temp,uc_ComType)==SlaveInUpdate
		|| GetDevUpDateCodeFream_FunCode(uc_temp,uc_ComType)==SlaveOutUpdate)
	{
		if(!IsGetSlaveDevUpDateCodeFream_UpDateCodeStatusOk(uc_temp,uc_ComType))
		{
			uc_ErrFlagTemp|=0x08;
		}		
		
		//长度判读
		if(uc_ComType==e_DevUpDateCode_ComTypeFlag_UartUpDate)
		{
				if(GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType) > UART_NowSegLen_Max)
				{
					uc_ErrFlagTemp|=0x20;
				}
				
				v_Temp=uc_IpTypeBuf;
		}
		else if(uc_ComType==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
		{
				if(GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType) > Eth_NowSegLen_Max)
				{
					uc_ErrFlagTemp|=0x20;
				}
						
				uc_sendbuf=uc_IpTypeBuf;
		}	
		
		if(!IsSlaveDevDateCodeOK())
		{//升级中
			uc_ErrFlagTemp|=0x40;
		}

		switch(GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType))
		{
			case e_SlaveDevUpDateCode_Start:
				SlaveDevUpDateCode_TimeClr();
				//获取总段数
				s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TotalSegNum=GetDevUpDateCodeFream_TotalSegNum(uc_temp,uc_ComType);
				s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_NowSegNum=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType);
				s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_SessionNum=GetDevUpDateCodeFream_SessionNum(uc_temp,uc_ComType);
				if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_NowSegNum!=0)
				{
					uc_ErrFlagTemp|=0x02;
				}
				
				if(uc_ErrFlagTemp==0)
				{
					SetGetSlaveDevUpDateCodeFream_ComType(uc_ComType);
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen=GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType);
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr=0;
					
					if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						ui_TempPtr=(unsigned int*)(uc_temp+14);
						s_SlaveDevUpDateCode.uc_SlaveType=*(uc_temp+1);
					}
					else if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
						ui_TempPtr=(unsigned int*)(uc_temp+15);
						s_SlaveDevUpDateCode.uc_SlaveType=*(uc_temp+2);
					}
					
					//数据移动到外部存储器
//					for(i=0;i<s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen;i++)
//					{
//					
//						*(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TempBufAddr+s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr)=*(ui_TempPtr+i);
//						
//						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr++;
//					}
					STMFLASH_Write(ADDR_OF_MachineUpAddr+s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr*4, (u16*)ui_TempPtr, s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen*2);
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr += s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen;
				}
				else
				{
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen=0;
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr=0;
					SetGetSlaveDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
				}
				
				if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
				{
					//数据搬送完回包			
						memcpy(uc_sendbuf,uc_temp,12);
						*(uc_sendbuf+12)=uc_ErrFlagTemp;
						usi_temp1=usMBCRC16(uc_sendbuf,13);
						*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
						*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
						*ui_sendlentemp=15;
						
				}
				else if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
				{
						memcpy(uc_sendbuf,uc_temp,13);
						*(uc_sendbuf+13)=uc_ErrFlagTemp;
						usi_temp1=usMBCRC16(uc_sendbuf,14);
						*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
						*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
						*ui_sendlentemp=16;					
				}			
					
				if(uc_ErrFlagTemp)	
				{
						return 0;
				}
				
				s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus=GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType);
				break;
				
				case e_SlaveDevUpDateCode_Sending:
					if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TotalSegNum!=GetDevUpDateCodeFream_TotalSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x01;
					}
					
					if((s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_NowSegNum+1)!=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x02;
					}
					else
					{
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_NowSegNum=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType);
					}
					
					if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_SessionNum!=GetDevUpDateCodeFream_SessionNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x04;
					}
					
					if(uc_ErrFlagTemp==0)
					{
						SetGetSlaveDevUpDateCodeFream_ComType(uc_ComType);
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen=GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType);
						
						if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+14);
						}
						else if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+15);
						}
										
						//数据移动到外部存储器
//						for(i=0;i<s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen;i++)
//						{
//							*(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TempBufAddr+s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr)=*(ui_TempPtr+i);
//							
//							s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr++;
//						}
						STMFLASH_Write(ADDR_OF_MachineUpAddr+s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr*4, (u16*)ui_TempPtr, s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen*2);
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr += s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen;
					}
					else
					{
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen=0;
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr=0;
						SetGetSlaveDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
					}
					
					if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						//数据搬送完回包			
							memcpy(uc_sendbuf,uc_temp,12);
							*(uc_sendbuf+12)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,13);
							*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=15;
							
					}
					else if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
							memcpy(uc_sendbuf,uc_temp,13);
							*(uc_sendbuf+13)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,14);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=16;					
					}			
						
					if(uc_ErrFlagTemp)	
					{
							return 0;
					}				
					
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus=GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType);
				break;
					
				case e_SlaveDevUpDateCode_End:
					
					if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TotalSegNum!=GetDevUpDateCodeFream_TotalSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x01;
					}
					
					if((s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_NowSegNum+1)!=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x02;
					}
					else
					{
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_NowSegNum=GetDevUpDateCodeFream_NowSegNum(uc_temp,uc_ComType);
					}
					
					if(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_SessionNum!=GetDevUpDateCodeFream_SessionNum(uc_temp,uc_ComType))
					{
						uc_ErrFlagTemp|=0x04;
					}
					
					if(uc_ErrFlagTemp==0)
					{
						SetGetSlaveDevUpDateCodeFream_ComType(uc_ComType);
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen=GetDevUpDateCodeFream_NowSegLen(uc_temp,uc_ComType);
						
						if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+14);
						}
						else if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
						{
							ui_TempPtr=(unsigned int*)(uc_temp+15);
						}					
						//数据移动到外部存储器
//						for(i=0;i<s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen;i++)
//						{
//							*(s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_TempBufAddr+s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr)=*(ui_TempPtr+i);
//							
//							s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr++;
//						}
						STMFLASH_Write(ADDR_OF_MachineUpAddr+s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr*4, (u16*)ui_TempPtr, s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen*2);
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr += s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen;
					}
					else
					{
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen=0;
						s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr=0;
						SetGetSlaveDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
					}
					
					if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						//数据搬送完回包			
							memcpy(uc_sendbuf,uc_temp,12);
							*(uc_sendbuf+12)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,13);
							*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=15;
							
					}
					else if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
							memcpy(uc_sendbuf,uc_temp,13);
							*(uc_sendbuf+13)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,14);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=16;					
					}			
						
					if(uc_ErrFlagTemp)	
					{
							return 0;
					}	
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.uc_UpDateCodeStatus=GetDevUpDateCodeFream_UpDateCodeStatus(uc_temp,uc_ComType); 				
				break;
			
			default:
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.usi_NowSegLen=0;
					s_SlaveDevUpDateCode.s_MasterConSendDevUpDateCode.ui_CodePtr=0;
					SetGetSlaveDevUpDateCodeFream_ComType(e_DevUpDateCode_ComTypeFlag_NoUpDateCode);
			
					if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_UartUpDate)
					{
						//数据搬送完回包			
							memcpy(uc_sendbuf,uc_temp,12);
							*(uc_sendbuf+12)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,13);
							*(uc_sendbuf+13)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=15;
							
					}
					else if(GetGetSlaveDevUpDateCodeFream_ComType()==e_DevUpDateCode_ComTypeFlag_ErethNetUpDate)
					{
							memcpy(uc_sendbuf,uc_temp,13);
							*(uc_sendbuf+13)=uc_ErrFlagTemp;
							usi_temp1=usMBCRC16(uc_sendbuf,14);
							*(uc_sendbuf+14)=*((unsigned char*)&usi_temp1);
							*(uc_sendbuf+15)=*((unsigned char*)&usi_temp1+1);
							*ui_sendlentemp=16;
					}				
				return 0;
			break;
			
			
		}			
		
	}
	return 1;
}
