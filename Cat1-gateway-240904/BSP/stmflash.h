#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "stm32f10x.h"  

#define STM32_FLASH_SIZE 64 			//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)

#define UPDATE_MODE		0x01
#define NORMAL_MODE		0x00

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

#define SOFTWARE_ADDRESS				(uint32_t)(0x08000000 + 0x0400*9)
#define SOFTWARE_UPDATE_ADDRESS			(uint32_t)(0x08000000 + 0x0400*36)

#define SOFTWARE_STARTMOD_FLAG		(uint32_t)(0x08000000 + 0x0400*62)

#define SOFTWARE_MAX_SIZE			(uint32_t)(0x0400*25)	//限制大小最大25KB

u16 STMFLASH_ReadHalfWord(u32 faddr);		 						//读出半字  
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

#endif

















