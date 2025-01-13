#ifndef __DevUpDateCode_H
#define __DevUpDateCode_H
#include "stm32f10x.h"



#pragma pack(1)
#define MasterUpdate 0x15
#define SlaveInUpdate 0x16	// 内机
#define SlaveOutUpdate 0x17	// 外机

#define SlaveDevComBufLen 1024

typedef struct
{
//	u16 updateBuf[1024];
	u8 rcvBuf[SlaveDevComBufLen+20];
	u16 rcvLen;
	u8 sndBuf[100];
	u32 sndLen;
	u8 updateFlag;	// 1--升级标志，2--升级包接收，3--发送内机和外机升级包, 0x80--网络升级;
}struct_update;


typedef enum
{
	e_RegConfigStatus_Free=0,//空闲
	e_RegConfigStatus_ComStart,//写备份存储区开始
	e_RegConfigStatus_ComEnd,	//写备份存储区结束
	e_RegConfigStatus_BackUpToNow,//备份存储器恢复到常用存储区
	
}E_RegConfigStatus;


typedef enum
{
	e_SysVarConUpDataFlag_Reset=0,
	e_SysVarConUpDataFlag_Start=0x0072,
	e_SysVarConUpDataFlag_End=0x11c3,
}E_SysVarConUpDataFlag;


typedef enum
{
	e_DevUpDateCode_Free=0, //空闲状态
	e_DevUpDateCode_Start,  //程序下载开始
	e_DevUpDateCode_Sending, //程序下载进行中
	e_DevUpDateCode_End,//程序下载结束
	e_DevUpDateCode_MemToFlash,//结束
}E_DevUpDateCode;

typedef enum
{
	e_SlaveDevUpDateCode_Free=0,  //空闲状态
	e_SlaveDevUpDateCode_Start,   //程序下载开始
	e_SlaveDevUpDateCode_Sending, //程序下载进行中
	e_SlaveDevUpDateCode_End,     //程序下载结束
	e_SlaveDevUpDateCode_ToSlaveStart,//给子设备升级程序开始
	e_SlaveDevUpDateCode_ToSlaveSending,//给子设备升级程序发送中
	e_SlaveDevUpDateCode_ToSlaveEnd,//给子设备升级程序结束
}E_SlaveDevUpDateCode;

#define CountTimeVal	100
#define DevUpDateCode_MaxTime 80*(10/(CountTimeVal/100)) //30秒

typedef enum
{
	e_DevUpDateCode_ComTypeFlag_NoUpDateCode=0,
	e_DevUpDateCode_ComTypeFlag_UartUpDate,
	e_DevUpDateCode_ComTypeFlag_ErethNetUpDate,
}E_DevUpDateCode_ComTypeFlag;


#define UART_NowSegLen_Max  1024
#define Eth_NowSegLen_Max   1024

typedef struct DevUpDateCode
{
	unsigned int ui_TotalSegNum;  //总段数
	unsigned int ui_NowSegNum;  //当前段数
	unsigned short int usi_NowSegLen; //当前段长度
	unsigned char uc_UpDateCodeStatus;//1：开始 2：传输中 3：结束  4:搬运数据去FLASH 0：空闲
	unsigned char uc_SessionNum; //本次升级标识 每次升级随机数
	
	unsigned char uc_ComTypeFlag;//0：未升级 1：串口升级 2：以太网升级
	unsigned int  ui_Time;    //计时器
	unsigned int ui_CodePtr;  //记录指针
	unsigned char *ui_TempBufAddr;	
	
}S_DevUpDateCode;

//子设备升级结构体
typedef struct SlaveDevUpDateCode
{
	S_DevUpDateCode s_MasterConSendDevUpDateCode;
	unsigned char uc_SlaveUpDateFlag;
	unsigned int ui_TotalSegNum;  //总段数
	unsigned int ui_NowSegNum;  //当前段数
	unsigned short int usi_NowSegLen; //当前段长度	
	unsigned int ui_CodePtr;  //记录指针
	
	unsigned char uc_ComErrFlag; //通讯错误为0方可进行下一步操作
	
	
	unsigned char uc_SlaveType;	// 1：内机，2：外机
}S_SlaveDevUpDateCode;

#define Fist_Init_Flag_Addr 0x3fff  //0x3fff 为铁电高存储为 16位下数值  铁电为32K*8bit

#define UPDATECODE_LEN_L16BIT 0x3ffe //0x3ffe 为程序更新长度低存储 16位 共32位 存储记录指针长度 +1带表4字节 
#define UPDATECODE_LEN_H16BIT 0x3ffd //0x3ffd 为程序更新长度高存储 16位



//#define Read_VIReg_Flag_Addr 0x3ffe  // 

#define VIReg_VarPtr_Addr  0x2000 //8K 

//#define MemFirstInitFlag_Var   0x1922 //初始化值 不等于便初始化
//#define ReadVIRegFlag_Var      0x1923 //等于该值就 读取值进行覆盖 不等于就读取进行覆盖

typedef struct DevRegHandle
{
	unsigned char uc_RegConfigStatus;       //
	unsigned short int  usi_MemFirstInitFlag;     //存储器第一次初始化标识
//	unsigned int  usi_BackUpVIRegNum;       //备份重要位号数量
//	unsigned short int  usi_ReadVIRegFlag;  //重要位号读取标识  
//	unsigned short int  *usi_VIRegIDPtr;    //重要位号序列存储区指针 存储在内部Flash 后期可以由组态开发平台生成下载到Flash里去
//	unsigned short int  usi_VIRegVarPtr;   //重要位号值序列指针	   存储在铁电存储器上
}S_DevRegHandle;

#pragma pack()

void DevUpdate(void);
void SlaveDevUpDateCodeHandle(void);
extern void DevConfigureConVar_Init(void);
extern void DevConfigureHandle(void);
extern void Set_Fist_Init_Flag(unsigned short int *usi_Temp);

extern void DevUpDateCode_Init(void);
extern void SlaveDevUpDateCode_Init(void);

extern void Set_DevUpDateCode_Status(unsigned char uc_Temp);
extern void DevUpDateCode_TimeCount(void);
extern void DevUpDateCode_TimeClr(void);
extern unsigned char IsDevUpDateCode_TimeOver(void);
extern void DevUpDateCodeHandle(void);
extern unsigned char GetGetDevUpDateCodeFream_ComType(void);
extern unsigned char  GetDevUpDateCodeFream_Handle(unsigned char *uc_temp,unsigned char* uc_sendbuf,unsigned int*ui_sendlentemp ,unsigned char uc_ComType,unsigned char *uc_IpTypeBuf);

extern unsigned char IsSlaveDevDateCode(void);
extern unsigned char SlaveDevDateCodeSend(unsigned char *uc_BufPtr,unsigned short int *usi_ComLen);
extern unsigned char IsSlaveDateCodeComErrStatus(void);
extern unsigned char SlaveDateCodeComBackHandle(unsigned char * uc_BufPtr);



#endif
