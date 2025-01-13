#ifndef __DevUpDateCode_H
#define __DevUpDateCode_H
#include "stm32f10x.h"



#pragma pack(1)
#define MasterUpdate 0x15
#define SlaveInUpdate 0x16	// �ڻ�
#define SlaveOutUpdate 0x17	// ���

#define SlaveDevComBufLen 1024

typedef struct
{
//	u16 updateBuf[1024];
	u8 rcvBuf[SlaveDevComBufLen+20];
	u16 rcvLen;
	u8 sndBuf[100];
	u32 sndLen;
	u8 updateFlag;	// 1--������־��2--���������գ�3--�����ڻ������������, 0x80--��������;
}struct_update;


typedef enum
{
	e_RegConfigStatus_Free=0,//����
	e_RegConfigStatus_ComStart,//д���ݴ洢����ʼ
	e_RegConfigStatus_ComEnd,	//д���ݴ洢������
	e_RegConfigStatus_BackUpToNow,//���ݴ洢���ָ������ô洢��
	
}E_RegConfigStatus;


typedef enum
{
	e_SysVarConUpDataFlag_Reset=0,
	e_SysVarConUpDataFlag_Start=0x0072,
	e_SysVarConUpDataFlag_End=0x11c3,
}E_SysVarConUpDataFlag;


typedef enum
{
	e_DevUpDateCode_Free=0, //����״̬
	e_DevUpDateCode_Start,  //�������ؿ�ʼ
	e_DevUpDateCode_Sending, //�������ؽ�����
	e_DevUpDateCode_End,//�������ؽ���
	e_DevUpDateCode_MemToFlash,//����
}E_DevUpDateCode;

typedef enum
{
	e_SlaveDevUpDateCode_Free=0,  //����״̬
	e_SlaveDevUpDateCode_Start,   //�������ؿ�ʼ
	e_SlaveDevUpDateCode_Sending, //�������ؽ�����
	e_SlaveDevUpDateCode_End,     //�������ؽ���
	e_SlaveDevUpDateCode_ToSlaveStart,//�����豸��������ʼ
	e_SlaveDevUpDateCode_ToSlaveSending,//�����豸������������
	e_SlaveDevUpDateCode_ToSlaveEnd,//�����豸�����������
}E_SlaveDevUpDateCode;

#define CountTimeVal	100
#define DevUpDateCode_MaxTime 80*(10/(CountTimeVal/100)) //30��

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
	unsigned int ui_TotalSegNum;  //�ܶ���
	unsigned int ui_NowSegNum;  //��ǰ����
	unsigned short int usi_NowSegLen; //��ǰ�γ���
	unsigned char uc_UpDateCodeStatus;//1����ʼ 2�������� 3������  4:��������ȥFLASH 0������
	unsigned char uc_SessionNum; //����������ʶ ÿ�����������
	
	unsigned char uc_ComTypeFlag;//0��δ���� 1���������� 2����̫������
	unsigned int  ui_Time;    //��ʱ��
	unsigned int ui_CodePtr;  //��¼ָ��
	unsigned char *ui_TempBufAddr;	
	
}S_DevUpDateCode;

//���豸�����ṹ��
typedef struct SlaveDevUpDateCode
{
	S_DevUpDateCode s_MasterConSendDevUpDateCode;
	unsigned char uc_SlaveUpDateFlag;
	unsigned int ui_TotalSegNum;  //�ܶ���
	unsigned int ui_NowSegNum;  //��ǰ����
	unsigned short int usi_NowSegLen; //��ǰ�γ���	
	unsigned int ui_CodePtr;  //��¼ָ��
	
	unsigned char uc_ComErrFlag; //ͨѶ����Ϊ0���ɽ�����һ������
	
	
	unsigned char uc_SlaveType;	// 1���ڻ���2�����
}S_SlaveDevUpDateCode;

#define Fist_Init_Flag_Addr 0x3fff  //0x3fff Ϊ����ߴ洢Ϊ 16λ����ֵ  ����Ϊ32K*8bit

#define UPDATECODE_LEN_L16BIT 0x3ffe //0x3ffe Ϊ������³��ȵʹ洢 16λ ��32λ �洢��¼ָ�볤�� +1����4�ֽ� 
#define UPDATECODE_LEN_H16BIT 0x3ffd //0x3ffd Ϊ������³��ȸߴ洢 16λ



//#define Read_VIReg_Flag_Addr 0x3ffe  // 

#define VIReg_VarPtr_Addr  0x2000 //8K 

//#define MemFirstInitFlag_Var   0x1922 //��ʼ��ֵ �����ڱ��ʼ��
//#define ReadVIRegFlag_Var      0x1923 //���ڸ�ֵ�� ��ȡֵ���и��� �����ھͶ�ȡ���и���

typedef struct DevRegHandle
{
	unsigned char uc_RegConfigStatus;       //
	unsigned short int  usi_MemFirstInitFlag;     //�洢����һ�γ�ʼ����ʶ
//	unsigned int  usi_BackUpVIRegNum;       //������Ҫλ������
//	unsigned short int  usi_ReadVIRegFlag;  //��Ҫλ�Ŷ�ȡ��ʶ  
//	unsigned short int  *usi_VIRegIDPtr;    //��Ҫλ�����д洢��ָ�� �洢���ڲ�Flash ���ڿ�������̬����ƽ̨�������ص�Flash��ȥ
//	unsigned short int  usi_VIRegVarPtr;   //��Ҫλ��ֵ����ָ��	   �洢������洢����
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
