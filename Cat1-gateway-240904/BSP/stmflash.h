#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "stm32f10x.h"  

#define STM32_FLASH_SIZE 64 			//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)

#define UPDATE_MODE		0x01
#define NORMAL_MODE		0x00

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

#define SOFTWARE_ADDRESS				(uint32_t)(0x08000000 + 0x0400*9)
#define SOFTWARE_UPDATE_ADDRESS			(uint32_t)(0x08000000 + 0x0400*36)

#define SOFTWARE_STARTMOD_FLAG		(uint32_t)(0x08000000 + 0x0400*62)

#define SOFTWARE_MAX_SIZE			(uint32_t)(0x0400*25)	//���ƴ�С���25KB

u16 STMFLASH_ReadHalfWord(u32 faddr);		 						//��������  
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

#endif

















