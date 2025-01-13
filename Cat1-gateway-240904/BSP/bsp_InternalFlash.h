#ifndef __BSP_INTERNALFLASH_H__
#define __BSP_INTERNALFLASH_H__

#include "stm32f10x.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 1024 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//#define UPDATE_MODE		0x01
//#define NORMAL_MODE		0x00

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
 

#define 	SOFTWARE_STARTMOD_FLAG						(uint32_t)(0x08000000 + 0x0400*80)

#define 	ADDR_OF_CfgFlag								(u32)(0x08000000 + 0x0400*100)	// 运行标志
#define 	ADDR_OF_SysReg								(u32)(ADDR_OF_CfgFlag+4)		// 运行标志
#define 	ADDR_OF_SysFucReg							(u32)(ADDR_OF_SysReg+SYS_HOLDING_REG_NUM*2)		// 面板个数
#define 	ADDR_OF_NetPara								(u32)(ADDR_OF_SysFucReg+SYSF_HOLDING_REG_NUM*2)    // 面板参数

#define 	ADDR_OF_GatewayUpAddr						(u32)(0x08000000 + 0x0400*110)	// 运行标志
#define 	ADDR_OF_MachineUpAddr						(u32)(0x08000000 + 0x0400*200)	// 运行标志

//#define 	ADDR_OF_eHeatMasterMode						(u32)(0x0800FC08)    //工作模式
//#define 	ADDR_OF_eHeatMasterRoomSetTempofcool		(u32)(0x0800FC0C)    //制冷设定温度
//#define 	ADDR_OF_eHeatMasterRoomSetTempofhot		  	(u32)(0x0800FC10)    //制热设定温度
//#define 	ADDR_OF_eHeatMasterRoomSetTempofair 	  	(u32)(0x0800FC14)    //通风设定温度
//#define 	ADDR_OF_MDTemoffset		                	(u32)(0x0800FC18)    //温度偏移量
//#define 	ADDR_OF_MDHumoffset            		    	(u32)(0x0800FC1C)    //湿度偏移
//#define 	ADDR_OF_MDeHeatMasterDpTempOffSet			(u32)(0x0800FC20)    //露点温度偏移量
//#define 	ADDR_OF_MDTopTemoffset            			(u32)(0x0800FC24)    //毛细管温度偏移
//#define 	ADDR_OF_MDHighProtectTemp					(u32)(0x0800FC28)    //毛细管高温保护温度值
//#define 	ADDR_OF_MDDisplayMod    					(u32)(0x0800FC2C)    //显示模式

//#define 	ADDR_OF_MDTemCorVar		                	(u32)(0x0800FC30)    //温度偏移量出场依久98
//#define 	ADDR_OF_MDHumCorVar           		    	(u32)(0x0800FC34)    //湿度偏移出厂

//#define 	ADDR_OF_SysParFirstInitFlag					(u32)(0x0800FC38)
//#define 	ADDR_OF_BootLoaderUpdataFlag				(u32)(0x0800FC4C)  	//BootLoaderUpdateFlag
//#define	  	ADDR_OF_eHeatMasterRoomSetTemp				(u32)(0x0800FC50) 	//温度设置位


//#define SOFTWARE_ADDRESS				(uint32_t)(0x08000000 + 0x0400*9)
//#define SOFTWARE_UPDATE_ADDRESS			(uint32_t)(0x08000000 + 0x0400*36)

//#define SOFTWARE_STARTMOD_FLAG		(uint32_t)(0x08000000 + 0x0400*62)

//#define SOFTWARE_MAX_SIZE			(uint32_t)(0x0400*25)	//限制大小最大25KB


u16 STMFLASH_ReadHalfWord(u32 faddr);		 														  //读出半字  
//void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);		//指定地址开始写入指定长度的数据
//u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);												//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);				//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   				//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

