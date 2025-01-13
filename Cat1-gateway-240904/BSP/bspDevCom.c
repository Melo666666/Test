#include "bspDevCom.h"
#include "bspDevTag.h"
#include "string.h"
#include "mbcrc.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"


extern TimerHandle_t Dev_Rsp_Timeout;
extern SemaphoreHandle_t Dev_Rcv_Flag;

s16 HoldReg_Dev[DEV_NUM_MAX][e_DevHoldReg_Number]={0};			// ������;
Dev_Com_Para_Struct Dev_Com_Para={0};
dev_para_struct Dev_Para[DEV_NUM_MAX]={0};


//�����ּĴ���
static u8 Modbus_03_Solve(u8 *rcvBuf, u16 rcvLen, dev_unit_struct tempUnit)
{
	u16 regLen=rcvBuf[2];
//	s16 *ptr=NULL;
	
	if(regLen == 2*tempUnit.regNum && rcvLen == regLen+5)		// �ֿ����
	{
		for(u8 i=0; i<tempUnit.regNum; i++)
		{
			HoldReg_Dev[tempUnit.slaveAddr-DEV_SLAVE_MIN_ADDR][tempUnit.startAddr+i]=(rcvBuf[3+2*i]<<8 | rcvBuf[4+2*i]);
		}
		return 1;
	}
	return 0;
}
//������Ĵ���
static u8 Modbus_04_Solve(u8 *rcvBuf, u16 rcvLen, dev_unit_struct tempUnit)
{
//	u16 regLen=rcvBuf[2];
//	s16 *ptr=NULL;
//	
//	if(regLen == 2*tempUnit.regNum && rcvLen == regLen+5)		// �ֿ����
//	{
//		for(u8 i=0; i<tempUnit.regNum; i++)
//		{
//			InputReg_Air[tempUnit.slaveAddr-DEV_SLAVE_MIN_ADDR][tempUnit.startAddr+i]=(rcvBuf[3+2*i]<<8 | rcvBuf[4+2*i]);
//		}
//		return 1;
//	}
	return 0;
}

//д���ּĴ���
static u8 Modbus_06_Solve(u8 *rcvBuf, u16 rcvLen, dev_unit_struct tempUnit)
{
	u16 startAddr=0, regVal=0;
	
	startAddr = (rcvBuf[2]<<8)+rcvBuf[3];
	regVal = (rcvBuf[4]<<8)+rcvBuf[5];
	if(startAddr == tempUnit.startAddr && regVal == tempUnit.regVal[0] && rcvLen == 8)
		return 1;

	return 0;
}

//д����Ĵ���
static u8 Modbus_10_Solve(u8 *rcvBuf, u16 rcvLen, dev_unit_struct tempUnit)
{
	u16 startAddr=0, regNum=0;
	
	startAddr = (rcvBuf[2]<<8)+rcvBuf[3];
	regNum = (rcvBuf[4]<<8)+rcvBuf[5];
	if(startAddr == tempUnit.startAddr && regNum == tempUnit.regNum && rcvLen == 8)
		return 1;

	return 0;
}


void Dev_TX_Service(dev_unit_struct *tempUnit)
{
	u8 tempBuf[100]={0};
	u16 len=0, calCRC;
	
	tempBuf[len++]=tempUnit->slaveAddr;
	tempBuf[len++]=tempUnit->funcCode;
	tempBuf[len++]=tempUnit->startAddr>>8;
	tempBuf[len++]=tempUnit->startAddr;
	
	if(tempUnit->funcCode == 0x03 || tempUnit->funcCode == 0x04)
	{
		tempBuf[len++]=tempUnit->regNum>>8;
		tempBuf[len++]=tempUnit->regNum;
	}
	else if(tempUnit->funcCode == 0x06)
	{
		tempBuf[len++]=tempUnit->regVal[0]>>8;
		tempBuf[len++]=tempUnit->regVal[0];
	}
	else if(tempUnit->funcCode == 0x10)
	{
		tempBuf[len++]=tempUnit->regNum>>8;
		tempBuf[len++]=tempUnit->regNum;
		tempBuf[len++]=tempUnit->regNum*2;
		for(u8 i=0; i<tempUnit->regNum; i++)
		{
			tempBuf[len++]=tempUnit->regVal[i]>>8;
			tempBuf[len++]=tempUnit->regVal[i];
		}
	}
	
	calCRC = usMBCRC16(tempBuf, len);
	tempBuf[len++] = calCRC;
	tempBuf[len++] = calCRC>>8;
	
	Dev_Snd_Buf(tempBuf, len);
}

u8 Dev_RX_Service(dev_unit_struct tempUnit)
{
	u8 *pRcv=NULL, rcvLen=0, ret=0;
	u16 calCRC, recCRC, reg_id, reg_num;							//��crc��8λ�͵�8λ����
	
//	if(E180_UartPara.Rcved)
	{
		rcvLen = Dev_Com_Para.rcvLen;
		pRcv = Dev_Com_Para.rcvBuf;
		if(pRcv[0] == tempUnit.slaveAddr &&  rcvLen>5 &&
			pRcv[0]>=DEV_SLAVE_MIN_ADDR && pRcv[0]<=DEV_SLAVE_MAX_ADDR)				//��ַ��ȷ
		{
			calCRC = usMBCRC16(pRcv, rcvLen-2);						//�������������ݵ�CRC
			recCRC = pRcv[rcvLen-2] | (pRcv[rcvLen-1]<<8);			//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
			
			if(calCRC==recCRC)		//CRCУ����ȷ
			{
//				reg_id = pRcv[3] | (pRcv[2]<<8);
				switch(pRcv[1])										//���ݲ�ͬ�Ĺ�������д���
				{
					case 0x03: 		//������Ĵ���
						ret=Modbus_03_Solve(pRcv, rcvLen, tempUnit);
						break;
						
					case 0x04: 		//�����ּĴ���
						ret=Modbus_04_Solve(pRcv, rcvLen, tempUnit);
						break;
					
					case 0x06: 		//д�����Ĵ���
						ret=Modbus_06_Solve(pRcv, rcvLen, tempUnit);
					break;

					case 0x10: 		//д����Ĵ���
						ret=Modbus_10_Solve(pRcv, rcvLen, tempUnit);
						break;
					
					default:
					break;
				}
			}
		}
		Dev_Com_Para.rcvFlag=0;
		Dev_Com_Para.rcvLen=0;
	}

	if(ret)	Dev_Para[tempUnit.slaveAddr-DEV_SLAVE_MIN_ADDR].ComHealth=0;
	return ret;
}
/*********************************************************************/
// ����4-USART1
#define	USARTx						USART1
#define	USART_CLK					RCC_APB2Periph_USART1 
#define	USART_APBxClkCmd			RCC_APB2PeriphClockCmd
#define	USART_BAUDRATE				19200

// USART GPIO ���ź궨��
#define	USART_GPIO_CLK				(RCC_APB2Periph_GPIOA)
#define	USART_GPIO_APBxClkCmd		RCC_APB2PeriphClockCmd
    
#define	USART_TX_GPIO_PORT			GPIOA
#define	USART_TX_GPIO_PIN			GPIO_Pin_9

#define	USART_RX_GPIO_PORT			GPIOA
#define	USART_RX_GPIO_PIN			GPIO_Pin_10

#define	USART_RE_GPIO_PORT			GPIOA
#define	USART_RE_GPIO_PIN			GPIO_Pin_11
#define	_485_SetRx()				while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){};GPIO_ResetBits(USART_RE_GPIO_PORT, USART_RE_GPIO_PIN)
#define	_485_SetTx()				GPIO_SetBits(USART_RE_GPIO_PORT, USART_RE_GPIO_PIN)

//#define	probe4_485_USART_IRQ		UART5_IRQn 
#define	DEV_UART_IRQHandler			USART1_IRQHandler

void Dev_485_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART_APBxClkCmd(USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ��USART RE��GPIO����Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART_RE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(USART_RE_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USARTx, &USART_InitStructure);
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);
}


void Dev_Snd_Buf(u8 *sndBuf, u16 sndLen)
{
	_485_SetTx();
	if(sndLen<1000)
	memcpy(Dev_Com_Para.sndBuf, sndBuf, sndLen);
	Dev_Com_Para.sndLen = sndLen;
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
}

// [Function] ����2�жϴ�����
void DEV_UART_IRQHandler(void)
{
	uint8_t data;
	static u16 sndLen;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	if(USART_GetFlagStatus(USARTx,USART_FLAG_ORE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_ORE);
		USART_ReceiveData(USARTx);
	}		
	
	if(USART_GetFlagStatus(USARTx,USART_FLAG_NE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_NE);
		USART_ReceiveData(USARTx);
	}			
	
	if(USART_GetFlagStatus(USARTx,USART_FLAG_FE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_FE);
		USART_ReceiveData(USARTx);
	}			
	
	if(USART_GetFlagStatus(USARTx,USART_FLAG_PE)!=RESET)
	{
		USART_ClearFlag(USARTx,USART_FLAG_PE);
		USART_ReceiveData(USARTx);
	}	
		
	/* Rx Not empty. */
	if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
	{
		data = USARTx->DR;
		if (Dev_Com_Para.rcvLen < DEV_RCV_LENGTH)
		{
			Dev_Com_Para.rcvBuf[Dev_Com_Para.rcvLen++] = data;
		}
	}
	else if (USART_GetITStatus(USARTx, USART_IT_IDLE) != RESET)
	{
		/* clear idle. */
		data = USARTx->SR;
		data = USARTx->DR;
		/* set state and wake up thread. */
//		if (Dev_Com_Para.rcvFlag == RCV_IDLE)
		{
			Dev_Com_Para.rcvFlag = RCV_RECEIVED;
			xSemaphoreGiveFromISR(Dev_Rcv_Flag, &xHigherPriorityTaskWoken);
		}
		/* stop timer. */
		xTimerStopFromISR(Dev_Rsp_Timeout, &xHigherPriorityTaskWoken);
	}
	/* TxEmpty interrupt. */
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		if (sndLen < Dev_Com_Para.sndLen)
		{
			USARTx->DR = Dev_Com_Para.sndBuf[sndLen++];
		}
		else
		{
			sndLen=0;
			Dev_Com_Para.sndLen=0;
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
			USART_ClearITPendingBit(USARTx, USART_IT_TXE);
			_485_SetRx();
			xTimerStartFromISR(Dev_Rsp_Timeout, &xHigherPriorityTaskWoken);
		}
	}
}

