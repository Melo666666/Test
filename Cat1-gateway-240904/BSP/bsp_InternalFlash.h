#ifndef __BSP_INTERNALFLASH_H__
#define __BSP_INTERNALFLASH_H__

#include "stm32f10x.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 1024 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//#define UPDATE_MODE		0x01
//#define NORMAL_MODE		0x00

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ
 

#define 	SOFTWARE_STARTMOD_FLAG						(uint32_t)(0x08000000 + 0x0400*80)

#define 	ADDR_OF_CfgFlag								(u32)(0x08000000 + 0x0400*100)	// ���б�־
#define 	ADDR_OF_SysReg								(u32)(ADDR_OF_CfgFlag+4)		// ���б�־
#define 	ADDR_OF_SysFucReg							(u32)(ADDR_OF_SysReg+SYS_HOLDING_REG_NUM*2)		// ������
#define 	ADDR_OF_NetPara								(u32)(ADDR_OF_SysFucReg+SYSF_HOLDING_REG_NUM*2)    // ������

#define 	ADDR_OF_GatewayUpAddr						(u32)(0x08000000 + 0x0400*110)	// ���б�־
#define 	ADDR_OF_MachineUpAddr						(u32)(0x08000000 + 0x0400*200)	// ���б�־

//#define 	ADDR_OF_eHeatMasterMode						(u32)(0x0800FC08)    //����ģʽ
//#define 	ADDR_OF_eHeatMasterRoomSetTempofcool		(u32)(0x0800FC0C)    //�����趨�¶�
//#define 	ADDR_OF_eHeatMasterRoomSetTempofhot		  	(u32)(0x0800FC10)    //�����趨�¶�
//#define 	ADDR_OF_eHeatMasterRoomSetTempofair 	  	(u32)(0x0800FC14)    //ͨ���趨�¶�
//#define 	ADDR_OF_MDTemoffset		                	(u32)(0x0800FC18)    //�¶�ƫ����
//#define 	ADDR_OF_MDHumoffset            		    	(u32)(0x0800FC1C)    //ʪ��ƫ��
//#define 	ADDR_OF_MDeHeatMasterDpTempOffSet			(u32)(0x0800FC20)    //¶���¶�ƫ����
//#define 	ADDR_OF_MDTopTemoffset            			(u32)(0x0800FC24)    //ëϸ���¶�ƫ��
//#define 	ADDR_OF_MDHighProtectTemp					(u32)(0x0800FC28)    //ëϸ�ܸ��±����¶�ֵ
//#define 	ADDR_OF_MDDisplayMod    					(u32)(0x0800FC2C)    //��ʾģʽ

//#define 	ADDR_OF_MDTemCorVar		                	(u32)(0x0800FC30)    //�¶�ƫ������������98
//#define 	ADDR_OF_MDHumCorVar           		    	(u32)(0x0800FC34)    //ʪ��ƫ�Ƴ���

//#define 	ADDR_OF_SysParFirstInitFlag					(u32)(0x0800FC38)
//#define 	ADDR_OF_BootLoaderUpdataFlag				(u32)(0x0800FC4C)  	//BootLoaderUpdateFlag
//#define	  	ADDR_OF_eHeatMasterRoomSetTemp				(u32)(0x0800FC50) 	//�¶�����λ


//#define SOFTWARE_ADDRESS				(uint32_t)(0x08000000 + 0x0400*9)
//#define SOFTWARE_UPDATE_ADDRESS			(uint32_t)(0x08000000 + 0x0400*36)

//#define SOFTWARE_STARTMOD_FLAG		(uint32_t)(0x08000000 + 0x0400*62)

//#define SOFTWARE_MAX_SIZE			(uint32_t)(0x0400*25)	//���ƴ�С���25KB


u16 STMFLASH_ReadHalfWord(u32 faddr);		 														  //��������  
//void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);		//ָ����ַ��ʼд��ָ�����ȵ�����
//u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);												//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);				//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   				//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

